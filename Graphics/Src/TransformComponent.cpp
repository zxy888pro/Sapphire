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
		return GetWorldTransform() * position; //��ȡ����仯����任pos
	}

	Sapphire::Vector3 TransformComponent::LocalToWorld(const Vector4& vector) const
	{
		return GetWorldTransform() * vector;
	}

	Sapphire::Vector2 TransformComponent::LocalToWorld2D(const Vector2& vector) const
	{
		Vector3 result = LocalToWorld(Vector3(vector)); //���Ա任3d��������ת��Ϊ2d
		return Vector2(result.x_, result.y_);
	}

	Sapphire::Vector3 TransformComponent::WorldToLocal(const Vector3& position) const
	{
		return GetWorldTransform().Inverse() * position;  //worldTransform����任
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
		//�����0����
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
		//һֱ�ݹ鵽���ڵ�
		SetPosition((parent == scene || !scene) ? position : parentTransform->GetWorldTransform().Inverse() * position);
	}

	void TransformComponent::SetWorldRotation(const Quaternion& rotation)
	{
		Node* parent = m_pNode->GetParent().Get();
		Node* scene = dynamic_cast<Node*>(m_pNode->GetScene());
		//һֱ�ݹ鵽���ڵ�
		const TransformComponent* parentTransform = parent->GetComponent<TransformComponent>();
		SetRotation((parent == scene || !parent) ? rotation : parentTransform->GetWorldRotation().Inverse() * rotation);  //��Set�ֲ�Rotation����ͨ��UpdateWorldTransform����WorldRatation
	}

	void TransformComponent::SetWorldDirection(const Vector3& direction)
	{
		Node* parent = m_pNode->GetParent().Get();
		Node* scene = dynamic_cast<Node*>(m_pNode->GetScene());
		const TransformComponent* parentTransform = parent->GetComponent<TransformComponent>();
		//һֱ�ݹ鵽���ڵ�
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
			// ���Ų������ƶ��ٶȣ��ֲ����ŶԾֲ��ռ�任��Ӱ��
			m_position += m_rotation * delta;  //�����������ƶ�
			break;

		case TS_PARENT:
			m_position += delta; //�Ǹ��ռ䣬ֱ�����
			break;

		case TS_WORLD:  //������ռ����ƶ�,������Ǹ��ڵ㣬���Ȼ�ȡ���ڵ������仯����任->��delta�任�����ڵ�ռ�ȥ
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
			m_rotation = (m_rotation * delta).Normalized(); //�Ⱥ�˳��
			break;

		case TS_PARENT:
			m_rotation = (delta * m_rotation).Normalized();
			break;

		case TS_WORLD:
			if (parent == scene || !parent)
				m_rotation = (delta * m_rotation).Normalized();
			else
			{
				Quaternion worldRotation = GetWorldRotation(); //��ȡ������ռ����ת
				//�ȼ�������ռ�����תrotation��Ȼ���������ռ������ת�任���ֲ��ռ䣬�ٱ任�ֲ��ռ��rotation
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

		Quaternion oldRotation = m_rotation; //�ȱ���ɵ���ת

		switch (space)
		{
		case TS_LOCAL:
			parentSpacePoint = GetTransform() * point;  //�����������ڸ��ռ��λ��
			m_rotation = (m_rotation * delta).Normalized();   //������ת
			break;

		case TS_PARENT:
			parentSpacePoint = point;					 //�����������ڸ��ռ��λ��
			m_rotation = (delta * m_rotation).Normalized();       //������ת
			break;

		case TS_WORLD:
			if (parent == scene || !parent)
			{
				parentSpacePoint = point;
				m_rotation = (delta * m_rotation).Normalized();
			}
			else
			{
				parentSpacePoint = parentTransform->GetWorldTransform().Inverse() * point;  //�����������ڸ��ռ��λ��
				Quaternion worldRotation = GetWorldRotation();
				m_rotation = m_rotation * worldRotation.Inverse() * delta * worldRotation;
			}
			break;
		}

		Vector3 oldRelativePos = oldRotation.Inverse() * (m_position - parentSpacePoint);  //�ɵ����λ��,  �ȼ������ǰ����Ŀ�����������ٰ�������ص���תǰ��λ��
		m_position = parentSpacePoint  + (m_rotation * oldRelativePos); //�ٰ�Ŀ���λ�� + ����ת����ǰ�Ƕȵ����� = ��ǰλ��

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
			worldSpaceTarget = GetWorldTransform() * target;  //ͳһת��������ռ�
			break;

		case TS_PARENT:
			worldSpaceTarget = (parent == scene || !parent) ? target : parentTransform->GetWorldTransform() * target;
			break;

		case TS_WORLD:
			worldSpaceTarget = target;
			break;
		}

		Vector3 lookDir = worldSpaceTarget - GetWorldPosition();  //���������Ŀ�������
		// ���Ŀ��ǳ��ӽ�������������޷��������㳯��
		if (lookDir.Equals(Vector3::ZERO))
			return false;
		Quaternion newRotation;
		// �������ʧ�ܣ�ʲô������
		if (!newRotation.FromLookRotation(lookDir, up)) //����һ����ת
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
			UpdateWorldTransform();  //�������ȸ���

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
			m_worldRotation = parentTransform->GetWorldRotation() * m_rotation; //�ø��ڵ������ռ���ת��ת*�ֲ���ת=�ýڵ������ռ���ת
		}
		//Ҫͬ����Node
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