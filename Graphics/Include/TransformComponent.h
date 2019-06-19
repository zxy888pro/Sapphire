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

	//����λ��/��ת/���ŵȼ��α仯
	class SAPPHIRE_CLASS TransformComponent : public Component
	{
		SAPPHIRE_OBJECT(TransformComponent, Component)
	public:
		TransformComponent(Core* pCore);
		virtual ~TransformComponent();

		//ת������
		Vector3 LocalToWorld(const Vector3& position) const;

		Vector3 LocalToWorld(const Vector4& vector) const;

		Vector2 LocalToWorld2D(const Vector2& vector) const;

		Vector3 WorldToLocal(const Vector3& position) const;

		Vector2 WorldToLocal2D(const Vector2& vector) const;

		Vector3 WorldToLocal(const Vector4& vector) const;

		//////////////////���ڵ�ռ��Transform����//////////////////////////

		//���þֲ�����λ��
		void SetPosition(const Vector3& position);
		//���þֲ�����λ�� (2D ģʽ)
		void SetPosition2D(const Vector2& position);
		//���þֲ���ת
		void SetRotation(const Quaternion& rotation);
		//���þֲ��� ��2Dģʽ��
		void SetRotation2D(float rotation) { SetRotation(Quaternion(rotation)); }
		//���þֲ�����
		void SetScale(float scale);
		//���þֲ�����  
		void SetScale(const Vector3& scale);
		//���þֲ�����  ��2Dģʽ��
		void SetScale2D(const Vector2& scale) { SetScale(Vector3(scale, 1.0f)); }
		//���þֲ�����  ��2Dģʽ��
		void SetScale2D(float x, float y) { SetScale(Vector3(x, y, 1.0f)); }
		//���ó���
		void SetDirection(const Vector3& direction);
		//���ñ任
		void SetTransform(const Vector3& position, const Quaternion& rotation);
		//���ñ任ͳһ����
		void SetTransform(const Vector3& position, const Quaternion& rotation, float scale);
		//���ñ任��ͳһ����
		void SetTransform(const Vector3& position, const Quaternion& rotation, const Vector3& scale);
		//���ñ任  ��2Dģʽ��
		void SetTransform2D(const Vector2& position, float rotation) { SetTransform(Vector3(position), Quaternion(rotation)); }
		//���ñ任  ��2Dģʽ��
		void SetTransform2D(const Vector2& position, float rotation, float scale)
		{
			SetTransform(Vector3(position), Quaternion(rotation), scale);
		}
		//���ñ任��ͳһ����  ��2Dģʽ��
		void SetTransform2D(const Vector2& position, float rotation, const Vector2& scale)
		{
			SetTransform(Vector3(position), Quaternion(rotation), Vector3(scale, 1.0f));
		}
		//�޸��ھֲ��ռ������
		void Scale(float scale);
		//�޸��ھֲ��ռ������
		void Scale(const Vector3& scale);
		//�޸��ھֲ��ռ������
		void Scale2D(const Vector2& scale) { Scale(Vector3(scale, 1.0f)); }


		/// ��������ռ�λ
		void SetWorldPosition(const Vector3& position);

		/// ��������ռ�λ�� ��2Dģʽ��
		void SetWorldPosition2D(const Vector2& position) { SetWorldPosition(Vector3(position)); }

		/// ��������ռ�λ�� ��2Dģʽ��
		void SetWorldPosition2D(float x, float y) { SetWorldPosition(Vector3(x, y, 0.0f)); }

		/// ��������ռ���ת ��2Dģʽ��
		void SetWorldRotation(const Quaternion& rotation);

		/// ��������ռ���ת ��2Dģʽ��
		void SetWorldRotation2D(float rotation) { SetWorldRotation(Quaternion(rotation)); }

		/// ��������ռ�ķ���
		void SetWorldDirection(const Vector3& direction);
		/// ��������ռ������
		void SetWorldScale(float scale);
		/// ��������ռ������
		void SetWorldScale(const Vector3& scale);

		/// ��������ռ����� ��2Dģʽ��
		void SetWorldScale2D(const Vector2& scale) { SetWorldScale(Vector3(scale, 1.0f)); }

		/// ��������ռ����� ��2Dģʽ��
		void SetWorldScale2D(float x, float y) { SetWorldScale(Vector3(x, y, 1.0f)); }

		/// ��������ռ�任
		void SetWorldTransform(const Vector3& position, const Quaternion& rotation);
		/// ��������ռ�任
		void SetWorldTransform(const Vector3& position, const Quaternion& rotation, float scale);
		/// ��������ռ�任
		void SetWorldTransform(const Vector3& position, const Quaternion& rotation, const Vector3& scale);

		/// ��������ռ�任 ��2Dģʽ��
		void SetWorldTransform2D(const Vector2& position, float rotation)
		{
			SetWorldTransform(Vector3(position), Quaternion(rotation));
		}

		/// ��������ռ�任 ��2Dģʽ��
		void SetWorldTransform2D(const Vector2& position, float rotation, float scale)
		{
			SetWorldTransform(Vector3(position), Quaternion(rotation), scale);
		}
		/// ��������ռ�任 ��2Dģʽ��
		void SetWorldTransform2D(const Vector2& position, float rotation, const Vector2& scale)
		{
			SetWorldTransform(Vector3(position), Quaternion(rotation), Vector3(scale, 1.0f));
		}
		//��ָ��������ռ��ƶ�node
		void Translate(const Vector3& delta, TransformSpace space = TS_LOCAL);

		void Translate2D(const Vector2& delta, TransformSpace space = TS_LOCAL) { Translate(Vector3(delta), space); }

		//��ָ���Ŀռ䣬��ת���node
		void Rotate(const Quaternion& delta, TransformSpace space = TS_LOCAL);
		//��ָ���Ŀռ䣬��ת���node  ��2Dģʽ��
		void Rotate2D(float delta, TransformSpace space = TS_LOCAL) { Rotate(Quaternion(delta), space); }
		//Χ��һ������ת����ڵ�
		void RotateAround(const Vector3& point, const Quaternion& delta, TransformSpace space = TS_LOCAL);
		//Χ��һ������ת����ڵ�  ��2Dģʽ)
		void RotateAround2D(const Vector2& point, float delta, TransformSpace space = TS_LOCAL)
		{
			RotateAround(Vector3(point), Quaternion(delta), space);
		}

		//��X����ת
		void Pitch(float angle, TransformSpace space = TS_LOCAL);

		//��Y����ת
		void Yaw(float angle, TransformSpace space = TS_LOCAL);

		//��z����ת
		void Roll(float angle, TransformSpace space = TS_LOCAL);

		///����һ��Ŀ��λ�á� ע��UP����һֱΪ����ռ�
		bool LookAt(const Vector3& target, const Vector3& up = Vector3::UP, TransformSpace space = TS_WORLD);

		//��ȡ�ֲ��ռ�λ��
	    const Vector3& GetPosition() const { return m_position; }

		//��ȡ�ֲ��ռ�λ�� (2Dģʽ)
		const Vector2& GetPosition2D() const { return Vector2(m_position.x_, m_position.y_); }

		//��ȡ�ֲ��ռ����ת
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

		///λ��
		Vector3 m_position;
		/// ��ת
		Quaternion m_rotation;
		/// ����
		Vector3 m_scale;
		/// ����ռ����ת
		mutable Quaternion m_worldRotation;
		/// ����ռ��transform����
		mutable Matrix3x4 m_worldTransform;

		//dirty��־
		mutable bool m_bDirty;
	};

}

