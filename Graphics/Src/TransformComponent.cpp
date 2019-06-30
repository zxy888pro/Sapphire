#include "TransformComponent.h"
#include "Node.h"
#include "Scene.h"


namespace Sapphire
{

	TransformComponent::TransformComponent(Core* pCore) :Component(pCore)
	{
		m_eCompType = ComponentType_Transform;
	}

	TransformComponent::~TransformComponent()
	{

	}

	Sapphire::Vector3 TransformComponent::LocalToWorld(const Vector3& position) const
	{
		return GetWorldTransform() * position; //获取世界变化矩阵变换pos
	}

	Sapphire::Vector3 TransformComponent::LocalToWorld(const Vector4& vector) const
	{
		return GetWorldTransform() * vector;
	}

	Sapphire::Vector2 TransformComponent::LocalToWorld2D(const Vector2& vector) const
	{
		Vector3 result = LocalToWorld(Vector3(vector)); //先以变换3d向量，在转换为2d
		return Vector2(result.x_, result.y_);
	}

	Sapphire::Vector3 TransformComponent::WorldToLocal(const Vector3& position) const
	{
		return GetWorldTransform().Inverse() * position;  //worldTransform的逆变换
	}

	Sapphire::Vector3 TransformComponent::WorldToLocal(const Vector4& vector) const
	{
		return GetWorldTransform().Inverse() * vector;
	}

	void TransformComponent::SetPosition(const Vector3& position)
	{
		m_position = position;
		m_pNode->MarkDirty();
	}

	void TransformComponent::SetPosition2D(const Vector2& position)
	{
		SetPosition(Vector3(position));
	}

	void TransformComponent::SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
		m_pNode->MarkDirty();
	}

	void TransformComponent::SetScale(float scale)
	{
		SetScale(Vector3(scale, scale, scale));
	}

	void TransformComponent::SetScale(const Vector3& scale)
	{
		m_scale = scale;
		//避免除0错误
		if (m_scale.x_ == 0.0f)
			m_scale.x_ = M_EPSILON;
		if (m_scale.y_ == 0.0f)
			m_scale.y_ = M_EPSILON;
		if (m_scale.z_ == 0.0f)
			m_scale.z_ = M_EPSILON;

		m_pNode->MarkDirty();
	}

	void TransformComponent::SetDirection(const Vector3& direction)
	{
		SetRotation(Quaternion(Vector3::FORWARD, direction));
	}

	void TransformComponent::SetTransform(const Vector3& position, const Quaternion& rotation)
	{
		m_position = position;
		m_rotation = rotation;
		m_pNode->MarkDirty();
	}

	void TransformComponent::SetTransform(const Vector3& position, const Quaternion& rotation, float scale)
	{
		SetTransform(position, rotation, Vector3(scale, scale, scale));
	}

	void TransformComponent::SetTransform(const Vector3& position, const Quaternion& rotation, const Vector3& scale)
	{
		m_position = position;
		m_rotation = rotation;
		m_scale = scale;
		m_pNode->MarkDirty();
	}

	void TransformComponent::Scale(float scale)
	{
		Scale(Vector3(scale, scale, scale));
	}

	void TransformComponent::Scale(const Vector3& scale)
	{
		m_scale *= scale;
		m_pNode->MarkDirty();
	}

	
	void TransformComponent::SetWorldPosition(const Vector3& position)
	{
		Node* parent = m_pNode->GetParent().Get();
		Node* scene = dynamic_cast<Node*>(m_pNode->GetScene());
		const TransformComponent* parentTransform = parent->GetComponent<TransformComponent>();
		//一直递归到根节点
		SetPosition((parent == scene || !scene) ? position : parentTransform->GetWorldTransform().Inverse() * position);
	}

	void TransformComponent::SetWorldRotation(const Quaternion& rotation)
	{
		Node* parent = m_pNode->GetParent().Get();
		Node* scene = dynamic_cast<Node*>(m_pNode->GetScene());
		//一直递归到根节点
		const TransformComponent* parentTransform = parent->GetComponent<TransformComponent>();
		SetRotation((parent == scene || !parent) ? rotation : parentTransform->GetWorldRotation().Inverse() * rotation);  //先Set局部Rotation，在通过UpdateWorldTransform更新WorldRatation
	}

	void TransformComponent::SetWorldDirection(const Vector3& direction)
	{
		Node* parent = m_pNode->GetParent().Get();
		Node* scene = dynamic_cast<Node*>(m_pNode->GetScene());
		const TransformComponent* parentTransform = parent->GetComponent<TransformComponent>();
		//一直递归到根节点
		Vector3 localDirection = (parent == scene || !parent) ? direction : parentTransform->GetWorldRotation().Inverse() * direction;
		SetRotation(Quaternion(Vector3::FORWARD, localDirection));
	}

	void TransformComponent::SetWorldScale(float scale)
	{
		SetWorldScale(Vector3(scale, scale, scale));
	}

	void TransformComponent::SetWorldScale(const Vector3& scale)
	{
		Node* parent = m_pNode->GetParent().Get();
		Node* scene = dynamic_cast<Node*>(m_pNode->GetScene());
		const TransformComponent* parentTransform = parent->GetComponent<TransformComponent>();
		SetScale((parent == scene || !parent) ? scale : scale / parentTransform->GetWorldScale());
	}

	void TransformComponent::SetWorldTransform(const Vector3& position, const Quaternion& rotation)
	{
		SetWorldPosition(position);
		SetWorldRotation(rotation);
	}

	void TransformComponent::SetWorldTransform(const Vector3& position, const Quaternion& rotation, float scale)
	{
		SetWorldPosition(position);
		SetWorldRotation(rotation);
		SetWorldScale(scale);
	}

	void TransformComponent::SetWorldTransform(const Vector3& position, const Quaternion& rotation, const Vector3& scale)
	{
		SetWorldPosition(position);
		SetWorldRotation(rotation);
		SetWorldScale(scale);
	}

	void TransformComponent::Translate(const Vector3& delta, TransformSpace space /*= TS_LOCAL*/)
	{
		Node* parent = m_pNode->GetParent().Get();
		Node* scene = dynamic_cast<Node*>(m_pNode->GetScene());
		const TransformComponent* parentTransform = parent->GetComponent<TransformComponent>();
		switch (space)
		{
		case TS_LOCAL:
			// 缩放不依赖移动速度，局部缩放对局部空间变换无影响
			m_position += m_rotation * delta;  //朝着自身方向移动
			break;

		case TS_PARENT:
			m_position += delta; //是父空间，直接相加
			break;

		case TS_WORLD:  //在世界空间中移动,如果不是根节点，就先获取父节点的世界变化的逆变换->将delta变换到父节点空间去
			m_position += (parent == scene || !parent) ? delta : parentTransform->GetWorldTransform().Inverse() * Vector4(delta, 0.0f);
			break;
		}

		m_pNode->MarkDirty();
	}

	void TransformComponent::Rotate(const Quaternion& delta, TransformSpace space /*= TS_LOCAL*/)
	{
		Node* parent = m_pNode->GetParent().Get();
		Node* scene = dynamic_cast<Node*>(m_pNode->GetScene());
		const TransformComponent* parentTransform = parent->GetComponent<TransformComponent>();
		switch (space)
		{
		case TS_LOCAL:
			m_rotation = (m_rotation * delta).Normalized(); //先后顺序
			break;

		case TS_PARENT:
			m_rotation = (delta * m_rotation).Normalized();
			break;

		case TS_WORLD:
			if (parent == scene || !parent)
				m_rotation = (delta * m_rotation).Normalized();
			else
			{
				Quaternion worldRotation = GetWorldRotation(); //获取在世界空间的旋转
				//先计算世界空间中旋转rotation，然后乘以世界空间的逆旋转变换到局部空间，再变换局部空间的rotation
				//rotation = rotation * (Quaternion.Inverse(rotation) * eulerRot * rotation);
				m_rotation = m_rotation * worldRotation.Inverse() * delta * worldRotation;   
			}
			break;
		}

		m_pNode->MarkDirty();
	}

	void TransformComponent::RotateAround(const Vector3& point, const Quaternion& delta, TransformSpace space /*= TS_LOCAL*/)
	{
		Node* parent = m_pNode->GetParent().Get();
		Node* scene = dynamic_cast<Node*>(m_pNode->GetScene());
		const TransformComponent* parentTransform = parent->GetComponent<TransformComponent>();
		Vector3 parentSpacePoint;

		Quaternion oldRotation = m_rotation; //先保存旧的旋转

		switch (space)
		{
		case TS_LOCAL:
			parentSpacePoint = GetTransform() * point;  //计算出这个点在父空间的位置
			m_rotation = (m_rotation * delta).Normalized();   //计算旋转
			break;

		case TS_PARENT:
			parentSpacePoint = point;					 //计算出这个点在父空间的位置
			m_rotation = (delta * m_rotation).Normalized();       //计算旋转
			break;

		case TS_WORLD:
			if (parent == scene || !parent)
			{
				parentSpacePoint = point;
				m_rotation = (delta * m_rotation).Normalized();
			}
			else
			{
				parentSpacePoint = parentTransform->GetWorldTransform().Inverse() * point;  //计算出这个点在父空间的位置
				Quaternion worldRotation = GetWorldRotation();
				m_rotation = m_rotation * worldRotation.Inverse() * delta * worldRotation;
			}
			break;
		}

		Vector3 oldRelativePos = oldRotation.Inverse() * (m_position - parentSpacePoint);  //旧的相对位置,  先计算出当前朝向目标点的向量，再把向量逆回到旋转前的位置
		m_position = parentSpacePoint  + (m_rotation * oldRelativePos); //再把目标点位置 + 再旋转到当前角度的向量 = 当前位置

		m_pNode->MarkDirty();
	}

	void TransformComponent::Pitch(float angle, TransformSpace space /*= TS_LOCAL*/)
	{
		Rotate(Quaternion(angle, Vector3::UP), space);
	}

	void TransformComponent::Yaw(float angle, TransformSpace space /*= TS_LOCAL*/)
	{
		Rotate(Quaternion(angle, Vector3::RIGHT), space);
	}

	void TransformComponent::Roll(float angle, TransformSpace space /*= TS_LOCAL*/)
	{
		Rotate(Quaternion(angle, Vector3::FORWARD), space);
	}

	bool TransformComponent::LookAt(const Vector3& target, const Vector3& up /*= Vector3::UP*/, TransformSpace space /*= TS_WORLD*/)
	{

		Node* parent = m_pNode->GetParent().Get();
		Node* scene = dynamic_cast<Node*>(m_pNode->GetScene());
		const TransformComponent* parentTransform = parent->GetComponent<TransformComponent>();
		Vector3 worldSpaceTarget;

		switch (space)
		{
		case TS_LOCAL:
			worldSpaceTarget = GetWorldTransform() * target;  //统一转换到世界空间
			break;

		case TS_PARENT:
			worldSpaceTarget = (parent == scene || !parent) ? target : parentTransform->GetWorldTransform() * target;
			break;

		case TS_WORLD:
			worldSpaceTarget = target;
			break;
		}

		Vector3 lookDir = worldSpaceTarget - GetWorldPosition();  //计算出朝向目标的向量
		// 如果目标非常接近，这种情况下无法正常计算朝向
		if (lookDir.Equals(Vector3::ZERO))
			return false;
		Quaternion newRotation;
		// 如果设置失败，什么都不做
		if (!newRotation.FromLookRotation(lookDir, up)) //构建一个旋转
			return false;

		SetWorldRotation(newRotation);
		return true;
	}

	Sapphire::Vector3 TransformComponent::GetWorldPosition() const
	{
		if (m_bDirty)
			UpdateWorldTransform();

		return m_worldTransform.GetTranslation();
	}

	Sapphire::Vector2 TransformComponent::GetWorldPosition2D() const
	{
		Vector3 worldPosition = GetWorldPosition();
		return Vector2(worldPosition.x_, worldPosition.y_);
	}

	Sapphire::Quaternion TransformComponent::GetWorldRotation() const
	{
		if (m_bDirty)
			UpdateWorldTransform();  //如果脏的先更新

		return m_worldRotation;
	}

	float TransformComponent::GetWorldRotation2D() const
	{
		return GetWorldRotation().RollAngle();
	}

	Sapphire::Vector3 TransformComponent::GetWorldDirection() const
	{
		if (m_bDirty)
			UpdateWorldTransform();

		return m_worldRotation * Vector3::FORWARD;
	}

	Sapphire::Vector3 TransformComponent::GetWorldUp() const
	{
		if (m_bDirty)
			UpdateWorldTransform();

		return m_worldRotation * Vector3::UP;
	}

	Sapphire::Vector3 TransformComponent::GetWorldRight() const
	{
		if (m_bDirty)
			UpdateWorldTransform();

		return m_worldRotation * Vector3::RIGHT;
	}

	Sapphire::Vector3 TransformComponent::GetWorldScale() const
	{
		if (m_bDirty)
			UpdateWorldTransform();

		return m_worldTransform.GetScale();
	}

	Sapphire::Vector2 TransformComponent::GetWorldScale2D() const
	{
		Vector3 worldScale = GetWorldScale();
		return Vector2(worldScale.x_, worldScale.y_);
	}

	const Sapphire::Matrix3x4& TransformComponent::GetWorldTransform() const
	{
		if (m_bDirty)
			UpdateWorldTransform();

		return m_worldTransform;
	}

	void TransformComponent::UpdateWorldTransform() const
	{
		Node* parent = m_pNode->GetParent().Get();
		Node* scene = dynamic_cast<Node*>(m_pNode->GetScene());
		const TransformComponent* parentTransform = parent->GetComponent<TransformComponent>();
		Matrix3x4 transform = GetTransform();

		if (parent == scene || !parent)
		{
			m_worldTransform = transform;
			m_worldRotation = m_rotation;
		}
		else
		{
			m_worldTransform = parentTransform->GetWorldTransform() * transform;
			m_worldRotation = parentTransform->GetWorldRotation() * m_rotation; //用父节点的世界空间旋转旋转*局部旋转=该节点的世界空间旋转
		}
		//要同步到Node
		m_bDirty = false;
		m_pNode->ClearDirtyFlag();
	}

	Sapphire::Vector2 TransformComponent::WorldToLocal2D(const Vector2& vector) const
	{
		Vector3 result = WorldToLocal(Vector3(vector));
		return Vector2(result.x_, result.y_);
	}

	void TransformComponent::SetTransformSilent(const Vector3& position, const Quaternion& rotation, const Vector3& scale)
	{
		m_position = position;
		m_rotation = rotation;
		m_scale = scale;
	}

}