#pragma once
#include "Sapphire.h"
#include "GraphicsDefs.h"
#include "Math/Frustum.h"
#include "Math/Ray.h"
#include "Component.h"


namespace Sapphire
{
	static const float DEFAULT_NEARCLIP = 0.1f;
	static const float DEFAULT_FARCLIP = 1000.0f;
	static const float DEFAULT_CAMERA_FOV = 45.0f;
	static const float DEFAULT_ORTHOSIZE = 20.0f;

	static const unsigned VO_NONE = 0x0;
	static const unsigned VO_LOW_MATERIAL_QUALITY = 0x1;
	static const unsigned VO_DISABLE_SHADOWS = 0x2;
	static const unsigned VO_DISABLE_OCCLUSION = 0x4;

	//������
	class SAPPHIRE_CLASS Camera :public Component
	{
		SAPPHIRE_OBJECT(Camera, Component)

	public:

		Camera(Core* pCore);
		virtual ~Camera();
		
		
		void SetNearClip(float nearClip);
		
		void SetFarClip(float farClip);

		void SetFov(float fov);

		void SetOrthoSize(float orthoSize);

		void SetOrthoSize(const Vector2& orthoSize);

		void SetAspectRatio(float aspectRatio);

		void SetFillMode(FillMode mode);

		void SetZoom(float zoom);
 
		void SetLodBias(float bias);
		/// ����ViewMask�����������Ƿ�ɼ�
		void SetViewMask(unsigned mask);
		/// ���ø��Ǳ�־
		void SetViewOverrideFlags(unsigned flags);
	 
		void SetOrthographic(bool enable);
		/// �Ƿ�����ӿڴ�С�Զ����ݱ�
		void SetAutoAspectRatio(bool enable);
		/// ����ͶӰƫ��. (offset in pixels) / (viewport dimensions.)
		void SetProjectionOffset(const Vector2& offset);
		/// ���÷���ģʽ
		void SetUseReflection(bool enable);
		/// ����������ռ�ķ���ƽ�棨����ģʽ��
		void SetReflectionPlane(const Plane& plane);
		/// �����Ƿ��Զ������ƽ��
		void SetUseClipping(bool enable);
		/// �����Զ������ƽ��
		void SetClipPlane(const Plane& plane);
		/// ���ô�ֱ��ת�����OpenGL/Dx�����Ĳ�ͬ������
		void SetFlipVertical(bool enable);

		///
		float GetFarClip() const { return farClip_; }

	
		float GetNearClip() const;

	
		float GetFov() const { return fov_; }


		float GetOrthoSize() const { return orthoSize_; }


		float GetAspectRatio() const { return aspectRatio_; }


		float GetZoom() const { return zoom_; }


		float GetLodBias() const { return lodBias_; }


		unsigned GetViewMask() const { return viewMask_; }


		unsigned GetViewOverrideFlags() const { return viewOverrideFlags_; }


		FillMode GetFillMode() const { return fillMode_; }


		bool IsOrthographic() const { return orthographic_; }


		bool GetAutoAspectRatio() const { return autoAspectRatio_; }


		const Frustum& GetFrustum() const;

		const Matrix4x4& GetProjection() const;

		Matrix4x4 GetProjection(bool apiSpecific) const;

		const Matrix3x4& GetView() const;

		void GetFrustumSize(Vector3& nearPoint, Vector3& farPont) const;
 
		float GetHalfViewSize() const;
		///�������Զ�����������Զ������ָ����׵��
		Frustum GetSplitFrustum(float nearClip, float farClip) const;
		/// ���ع۲�ռ����׵��
		Frustum GetViewSpaceFrustum() const;
		///
		Frustum GetViewSpaceSplitFrustum(float nearClip, float farClip) const;
		/// ���ط��ű�׼����Ļ�����(0.0 - 1.0)����Ļ����
		Ray GetScreenRay(float x, float y) const;
		// ת��һ������ռ�ĵ㵽��׼����Ļ�����(0.0 - 1.0)
		Vector2 WorldToScreenPoint(const Vector3& worldPos) const;
		// ת��һ����׼����Ļ�����(0.0 - 1.0)����ȵ�����ռ�ĵ�
		Vector3 ScreenToWorldPoint(const Vector3& screenPos) const;

		/// ����ͶӰƫ��
		const Vector2& GetProjectionOffset() const { return projectionOffset_; }

		/// �Ƿ���
		bool GetUseReflection() const { return useReflection_; }

		 
		const Plane& GetReflectionPlane() const { return reflectionPlane_; }

		/// �Ƿ��Զ������
		bool GetUseClipping() const { return useClipping_; }

	 
		const Plane& GetClipPlane() const { return clipPlane_; }

	 
		bool GetFlipVertical() const { return flipVertical_; }

		///  �Ƿ����޳��� Ӱ�촹ֱ��ת�ͷ���
		bool GetReverseCulling() const { return flipVertical_ ^ useReflection_; }

		/// ���ص�һ����ľ��룬������ģʽֻ��z����
		float GetDistance(const Vector3& worldPos) const;
		///���ص�һ����ľ����ƽ����������ģʽֻ��z����
		float GetDistanceSquared(const Vector3& worldPos) const;
		/// ���ص�һ�������ڵ��LOD ���ž���
		float GetLodDistance(float distance, float scale, float bias) const;
		/// ����һ���������ᳯ�����������ռ���ת
		Quaternion GetFaceCameraRotation(const Vector3& position, const Quaternion& rotation, FaceCameraMode mode);
		/// ����һ������ռ�İ������䲻�������ŵ�tranform����׵�����ľ���
		Matrix3x4 GetEffectiveWorldTransform() const;
		/// ���ض�Ӧ��Ⱦ����ӰͶ��ͶӰ�����Ƿ���Ч
		bool IsProjectionValid() const;

		/// ���ú��ݱȲ����ر��Զ����ݱ�ħ�ޣ���View�ڲ�����
		void SetAspectRatioInternal(float aspectRatio);
		// ���÷���ƽ������
		void SetReflectionPlaneAttr(const Vector4& value);
		/// ��ȡ����ƽ������
		Vector4 GetReflectionPlaneAttr() const;
		/// ���ü���ƽ������
		void SetClipPlaneAttr(const Vector4& value);
		/// ��ȡ����ƽ������
		Vector4 GetClipPlaneAttr() const;

	protected:


	private:

		//����Ķ���
		//�۲����
		mutable Matrix3x4 view_;
		//ͶӰ����
		mutable Matrix4x4 projection_;
		//��׵��
		mutable Frustum frustum_;

		//���־
		mutable bool viewDirty_;
		mutable bool projectionDirty_;
		mutable bool frustumDirty_;

		///�Ƿ�����
		bool orthographic_;
		///���������
		float nearClip_;
		///Զ�������
		float farClip_;
		/// ��Ұ
		float fov_;
		/// ���������С
		float orthoSize_;
		/// ���ݱ�
		float aspectRatio_;
		/// ����ֵ
		float zoom_;
		//Lod Bias
		float lodBias_;
		//���ģʽ
		FillMode  fillMode_;
		/// View mask.
		unsigned viewMask_;
		// view ���Ǳ�־
		unsigned viewOverrideFlags_;
		/// ͶӰƫ��
		Vector2 projectionOffset_;
		/// ����ƽ��
		Plane reflectionPlane_;
		/// ����ƽ��
		Plane clipPlane_;

		/// �������
		Matrix3x4 reflectionMatrix_;

		/// �Զ����ݱ�
		bool autoAspectRatio_;
		///��ֱ��ת
		bool flipVertical_;
		///ʹ�÷���ģʽ
		bool useReflection_;
		/// ���Զ������ƽ��
		bool useClipping_;
	};

}