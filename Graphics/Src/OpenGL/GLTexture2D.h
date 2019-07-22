#pragma once
#include "ImageRes.h"
#include "XML/XMLFile.h"
#include "GLTexture.h"



namespace Sapphire
{
	//OpenGL��2D����
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

		bool	Load(const char* resName);  //������Դ
		bool	Load();
		//��׃�y�팦��Ĵ�С
		bool SetSize(int width, int height, PixelFormat eformat, TextureUsage eUsage = TEXTURE_STATIC);
		bool SetData(uint level, int x, int y, int width, int height, const void* data); //������������ 
		bool SetData(SharedPtr<ImageRes> img, bool useAlpha = false);  //������������,-����mipmap ,  ��ֻ��һ��ͨ���ĕr��useAlpha��ʾ��alpha߀������
		//��ȡ��Ⱦ����
		GLRenderSurface* GetRenderSurface() const { return m_renderSurface; }

	protected:

		SharedPtr<ImageRes>   m_imageRes;    //��Ӧ��Image��Դ
		SharedPtr<XMLFile>    m_parameters;   //���ò���
		/// ��Ⱦ����, ÿ�������Ӧһ����Ⱦ����
		SharedPtr<GLRenderSurface> m_renderSurface;
	};

}
