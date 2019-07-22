#pragma once
#include "ImageRes.h"
#include "XML/XMLFile.h"
#include "GLTexture.h"



namespace Sapphire
{
	//OpenGL的2D纹理
	class SAPPHIRE_CLASS GLTexture2D : public GLTexture
	{
		SAPPHIRE_OBJECT(GLTexture2D, GLTexture)
	public:

		GLTexture2D(Core* pCore, IGraphicDriver* pDriver, const char* resName = "");

		virtual ~GLTexture2D();

		virtual void Release() override;

		virtual bool Recreate() override;

		virtual void Dispose() override;

		virtual bool IsDisposed() override;

		virtual size_t GetSize() override;

		virtual void ReRequest() override;

		virtual void OnDeviceLost() override;

		virtual void OnDeviceReset() override;

		virtual bool Create() override;

		virtual void RenderSurfaceUpdate() override;	      

		virtual void OnLoadStart() override;

		virtual void OnLoadEnd() override;

		virtual void OnLoadError() override;

		


		virtual void Invoke(ushort eEventType, ushort eEvent, EventContext* src, void* eventData = NULL) override;
	public:

		bool	Load(const char* resName);  //加载资源
		bool	Load();
		//改變紋理對象的大小
		bool SetSize(int width, int height, PixelFormat eformat, TextureUsage eUsage = TEXTURE_STATIC);
		bool SetData(uint level, int x, int y, int width, int height, const void* data); //设置纹理数据 
		bool SetData(SharedPtr<ImageRes> img, bool useAlpha = false);  //设置纹理数据,-包括mipmap ,  在只有一個通道的時候useAlpha表示是alpha還是明度
		//获取渲染表面
		GLRenderSurface* GetRenderSurface() const { return m_renderSurface; }

	protected:

		SharedPtr<ImageRes>   m_imageRes;    //对应的Image资源
		SharedPtr<XMLFile>    m_parameters;   //配置参数
		/// 渲染表面, 每个纹理对应一个渲染表面
		SharedPtr<GLRenderSurface> m_renderSurface;
	};

}
