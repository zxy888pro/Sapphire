#pragma once

#include "Sapphire.h"
#include "Component.h"
#include "Math/Quaternion.h"
#include "Math/Matrix3x4.h"

namespace Sapphire
{
	enum TransformSpace
	{
		TS_LOCAL = 0,
		TS_PARENT,
		TS_WORLD
	};

	//所有位移/旋转/缩放等几何变化
	class SAPPHIRE_CLASS TransformComponent : public Component
	{
		SAPPHIRE_OBJECT(TransformComponent, Component)
	public:
		TransformComponent(Core* pCore);
		virtual ~TransformComponent();

		//转换坐标
		Vector3 LocalToWorld(const Vector3& position) const;

		Vector3 LocalToWorld(const Vector4& vector) const;

		Vector2 LocalToWorld2D(const Vector2& vector) const;

		Vector3 WorldToLocal(const Vector3& position) const;

		Vector2 WorldToLocal2D(const Vector2& vector) const;

		Vector3 WorldToLocal(const Vector4& vector) const;

		//////////////////父节点空间的Transform设置//////////////////////////

		//设置局部坐标位置
		void SetPosition(const Vector3& position);
		//设置局部坐标位置 (2D 模式)
		void SetPosition2D(const Vector2& position);
		//设置局部旋转
		void SetRotation(const Quaternion& rotation);
		//设置局部旋 （2D模式）
		void SetRotation2D(float rotation) { SetRotation(Quaternion(rotation)); }
		//设置局部缩放
		void SetScale(float scale);
		//设置局部缩放  
		void SetScale(const Vector3& scale);
		//设置局部缩放  （2D模式）
		void SetScale2D(const Vector2& scale) { SetScale(Vector3(scale, 1.0f)); }
		//设置局部缩放  （2D模式）
		void SetScale2D(float x, float y) { SetScale(Vector3(x, y, 1.0f)); }
		//设置朝向
		void SetDirection(const Vector3& direction);
		//设置变换
		void SetTransform(const Vector3& position, const Quaternion& rotation);
		//设置变换统一缩放
		void SetTransform(const Vector3& position, const Quaternion& rotation, float scale);
		//设置变换非统一缩放
		void SetTransform(const Vector3& position, const Quaternion& rotation, const Vector3& scale);
		//设置变换  （2D模式）
		void SetTransform2D(const Vector2& position, float rotation) { SetTransform(Vector3(position), Quaternion(rotation)); }
		//设置变换  （2D模式）
		void SetTransform2D(const Vector2& position, float rotation, float scale)
		{
			SetTransform(Vector3(position), Quaternion(rotation), scale);
		}
		//设置变换非统一缩放  （2D模式）
		void SetTransform2D(const Vector2& position, float rotation, const Vector2& scale)
		{
			SetTransform(Vector3(position), Quaternion(rotation), Vector3(scale, 1.0f));
		}
		//修改在局部空间的缩放
		void Scale(float scale);
		//修改在局部空间的缩放
		void Scale(const Vector3& scale);
		//修改在局部空间的缩放
		void Scale2D(const Vector2& scale) { Scale(Vector3(scale, 1.0f)); }


		/// 设置世界空间位
		void SetWorldPosition(const Vector3& position);

		/// 设置世界空间位置 （2D模式）
		void SetWorldPosition2D(const Vector2& position) { SetWorldPosition(Vector3(position)); }

		/// 设置世界空间位置 （2D模式）
		void SetWorldPosition2D(float x, float y) { SetWorldPosition(Vector3(x, y, 0.0f)); }

		/// 设置世界空间旋转 （2D模式）
		void SetWorldRotation(const Quaternion& rotation);

		/// 设置世界空间旋转 （2D模式）
		void SetWorldRotation2D(float rotation) { SetWorldRotation(Quaternion(rotation)); }

		/// 设置世界空间的方向
		void SetWorldDirection(const Vector3& direction);
		/// 设置世界空间的缩放
		void SetWorldScale(float scale);
		/// 设置世界空间的缩放
		void SetWorldScale(const Vector3& scale);

		/// 设置世界空间缩放 （2D模式）
		void SetWorldScale2D(const Vector2& scale) { SetWorldScale(Vector3(scale, 1.0f)); }

		/// 设置世界空间缩放 （2D模式）
		void SetWorldScale2D(float x, float y) { SetWorldScale(Vector3(x, y, 1.0f)); }

		/// 设置世界空间变换
		void SetWorldTransform(const Vector3& position, const Quaternion& rotation);
		/// 设置世界空间变换
		void SetWorldTransform(const Vector3& position, const Quaternion& rotation, float scale);
		/// 设置世界空间变换
		void SetWorldTransform(const Vector3& position, const Quaternion& rotation, const Vector3& scale);

		/// 设置世界空间变换 （2D模式）
		void SetWorldTransform2D(const Vector2& position, float rotation)
		{
			SetWorldTransform(Vector3(position), Quaternion(rotation));
		}

		/// 设置世界空间变换 （2D模式）
		void SetWorldTransform2D(const Vector2& position, float rotation, float scale)
		{
			SetWorldTransform(Vector3(position), Quaternion(rotation), scale);
		}
		/// 设置世界空间变换 （2D模式）
		void SetWorldTransform2D(const Vector2& position, float rotation, const Vector2& scale)
		{
			SetWorldTransform(Vector3(position), Quaternion(rotation), Vector3(scale, 1.0f));
		}
		//在指定的坐标空间移动node
		void Translate(const Vector3& delta, TransformSpace space = TS_LOCAL);

		void Translate2D(const Vector2& delta, TransformSpace space = TS_LOCAL) { Translate(Vector3(delta), space); }

		//在指定的空间，旋转这个node
		void Rotate(const Quaternion& delta, TransformSpace space = TS_LOCAL);
		//在指定的空间，旋转这个node  （2D模式）
		void Rotate2D(float delta, TransformSpace space = TS_LOCAL) { Rotate(Quaternion(delta), space); }
		//围绕一个点旋转这个节点
		void RotateAround(const Vector3& point, const Quaternion& delta, TransformSpace space = TS_LOCAL);
		//围绕一个点旋转这个节点  （2D模式)
		void RotateAround2D(const Vector2& point, float delta, TransformSpace space = TS_LOCAL)
		{
			RotateAround(Vector3(point), Quaternion(delta), space);
		}

		//绕X轴旋转
		void Pitch(float angle, TransformSpace space = TS_LOCAL);

		//绕Y轴旋转
		void Yaw(float angle, TransformSpace space = TS_LOCAL);

		//绕z轴旋转
		void Roll(float angle, TransformSpace space = TS_LOCAL);

		///朝向一个目标位置。 注意UP向量一直为世界空间
		bool LookAt(const Vector3& target, const Vector3& up = Vector3::UP, TransformSpace space = TS_WORLD);

		//获取局部空间位置
	    const Vector3& GetPosition() const { return m_position; }

		//获取局部空间位置 (2D模式)
		const Vector2& GetPosition2D() const { return Vector2(m_position.x_, m_position.y_); }

		//获取局部空间的旋转
		const Quaternion& GetRotation() const { return m_rotation; }
		

		Vector3 GetWorldPosition() const;

		Vector2 GetWorldPosition2D() const;

		Quaternion GetWorldRotation() const;

		float GetWorldRotation2D() const;

		Vector3 GetWorldDirection() const;


		Vector3 GetWorldUp() const;

		Vector3 GetWorldRight() const;

		Vector3 GetWorldScale() const;


		Vector2 GetWorldScale2D() const;

		const Matrix3x4& GetWorldTransform() const;


		bool IsDirty() const { return m_bDirty; }


	protected:

		///位置
		Vector3 m_position;
		/// 旋转
		Quaternion m_rotation;
		/// 缩放
		Vector3 m_scale;
		/// 世界空间的旋转
		mutable Quaternion m_worldRotation;
		/// 世界空间的transform矩阵
		mutable Matrix3x4 m_worldTransform;

		//dirty标志
		mutable bool m_bDirty;
	};

}

