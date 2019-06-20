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

	//相机组件
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
		/// 设置ViewMask，决定对象是否可见
		void SetViewMask(unsigned mask);
		/// 设置覆盖标志
		void SetViewOverrideFlags(unsigned flags);
	 
		void SetOrthographic(bool enable);
		/// 是否根据视口大小自动横纵比
		void SetAutoAspectRatio(bool enable);
		/// 设置投影偏移. (offset in pixels) / (viewport dimensions.)
		void SetProjectionOffset(const Vector2& offset);
		/// 设置反射模式
		void SetUseReflection(bool enable);
		/// 设置在世界空间的反射平面（反射模式）
		void SetReflectionPlane(const Plane& plane);
		/// 设置是否自定义剪裁平面
		void SetUseClipping(bool enable);
		/// 设置自定义剪裁平面
		void SetClipPlane(const Plane& plane);
		/// 设置垂直翻转，解决OpenGL/Dx采样的不同的问题
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
		///返回由自定义近剪裁面和远剪裁面分割的视椎体
		Frustum GetSplitFrustum(float nearClip, float farClip) const;
		/// 返回观察空间的视椎体
		Frustum GetViewSpaceFrustum() const;
		///
		Frustum GetViewSpaceSplitFrustum(float nearClip, float farClip) const;
		/// 返回符号标准化屏幕坐标的(0.0 - 1.0)的屏幕射线
		Ray GetScreenRay(float x, float y) const;
		// 转换一个世界空间的点到标准化屏幕坐标的(0.0 - 1.0)
		Vector2 WorldToScreenPoint(const Vector3& worldPos) const;
		// 转换一个标准化屏幕坐标的(0.0 - 1.0)和深度到世界空间的点
		Vector3 ScreenToWorldPoint(const Vector3& screenPos) const;

		/// 返回投影偏移
		const Vector2& GetProjectionOffset() const { return projectionOffset_; }

		/// 是否反射
		bool GetUseReflection() const { return useReflection_; }

		 
		const Plane& GetReflectionPlane() const { return reflectionPlane_; }

		/// 是否自定义剪裁
		bool GetUseClipping() const { return useClipping_; }

	 
		const Plane& GetClipPlane() const { return clipPlane_; }

	 
		bool GetFlipVertical() const { return flipVertical_; }

		///  是否反向剔除， 影响垂直翻转和反射
		bool GetReverseCulling() const { return flipVertical_ ^ useReflection_; }

		/// 返回到一个点的距离，在正交模式只用z坐标
		float GetDistance(const Vector3& worldPos) const;
		///返回到一个点的距离的平方，在正交模式只用z坐标
		float GetDistanceSquared(const Vector3& worldPos) const;
		/// 返回到一个场景节点的LOD 缩放距离
		float GetLodDistance(float distance, float scale, float bias) const;
		/// 返回一个在中心轴朝向相机的世界空间旋转
		Quaternion GetFaceCameraRotation(const Vector3& position, const Quaternion& rotation, FaceCameraMode mode);
		/// 返回一个世界空间的包括反射不包括缩放的tranform的视椎体计算的矩阵
		Matrix3x4 GetEffectiveWorldTransform() const;
		/// 返回对应渲染和摄影投射投影参数是否有效
		bool IsProjectionValid() const;

		/// 设置横纵比参数关闭自动横纵比魔兽，在View内部调用
		void SetAspectRatioInternal(float aspectRatio);
		// 设置反射平面属性
		void SetReflectionPlaneAttr(const Vector4& value);
		/// 获取反射平面属性
		Vector4 GetReflectionPlaneAttr() const;
		/// 设置剪裁平面属性
		void SetClipPlaneAttr(const Vector4& value);
		/// 获取返回平面属性
		Vector4 GetClipPlaneAttr() const;

	protected:


	private:

		//缓存的对象
		//观察矩阵
		mutable Matrix3x4 view_;
		//投影矩阵
		mutable Matrix4x4 projection_;
		//视椎体
		mutable Frustum frustum_;

		//脏标志
		mutable bool viewDirty_;
		mutable bool projectionDirty_;
		mutable bool frustumDirty_;

		///是否正交
		bool orthographic_;
		///近裁面距离
		float nearClip_;
		///远裁面距离
		float farClip_;
		/// 视野
		float fov_;
		/// 正交视域大小
		float orthoSize_;
		/// 横纵比
		float aspectRatio_;
		/// 缩放值
		float zoom_;
		//Lod Bias
		float lodBias_;
		//填充模式
		FillMode  fillMode_;
		/// View mask.
		unsigned viewMask_;
		// view 覆盖标志
		unsigned viewOverrideFlags_;
		/// 投影偏移
		Vector2 projectionOffset_;
		/// 反射平面
		Plane reflectionPlane_;
		/// 剪裁平面
		Plane clipPlane_;

		/// 反射矩阵
		Matrix3x4 reflectionMatrix_;

		/// 自动横纵比
		bool autoAspectRatio_;
		///垂直翻转
		bool flipVertical_;
		///使用反射模式
		bool useReflection_;
		/// 用自定义剪裁平面
		bool useClipping_;
	};

}