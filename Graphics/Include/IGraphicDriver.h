#pragma once

#include "Graphics.h"


namespace Sapphire
{
	struct  ITextureMgr;
	struct  IImageMgr;
	struct  ITexture;
	struct  IDisplayContext;
	class   GPUObject;
	struct  IVertexBuffer;
	struct  IShaderVariation;
	

	//OpenGL ��Ӳ������������ࡣ  
	//�޹�ͼ��API
	class SAPPHIRE_CLASS IGraphicDriver : public SubSystem
	{
		SAPPHIRE_OBJECT(IGraphicDriver, SubSystem)

	public:

		IGraphicDriver(Core* pCore):
			SubSystem(pCore)
		{
			 
		}
		virtual ~IGraphicDriver(){}

		virtual void Init() = 0;
		virtual void Release() = 0;

		/// ��Ⱦ֡�Ŀ�ʼ������豸��Ч������Ⱦ������true
		virtual bool BeginFrame() = 0;
		/// ��Ⱦ֡����������˫������
		virtual void EndFrame() = 0;

		virtual void PrepareDraw() = 0;
		virtual void* GetMainWindow() = 0;
		virtual ITextureMgr* getTextureMgr() const = 0;
		
		virtual IImageMgr* getImageMgr() const = 0;
		virtual GraphicDriverType getDriverType() const = 0;

		virtual PixelFormat GetPixelFormat(ImageType eImgType) = 0;

		virtual PixelFormat GetPixelFormat(std::string szImageType) = 0;

		virtual bool    IsInitialized() = 0;   //�Ƿ��ʼ�����

		virtual void SetViewport(const IntRect& rect) = 0; //�����ӿ�

		virtual bool  IsDeviceLost() = 0;  //�豸�Ƿ�ʧ

		virtual bool SetDisplayMode(int x, int y, int width, int height, bool bFullScreen, bool bVsync, int multiSample, bool tripleBuffer, bool resizable) = 0;

		virtual  void SetColorWrite(bool enable) = 0;

		virtual void SetDepthWrite(bool enable) = 0;
		
		virtual  void SetDepthTest(CompareMode mode) = 0;

		//���ö��㻺��������
		virtual void  SetVertexBuffer(IVertexBuffer* vertexBuffer) = 0;

		//����ģ�����
		virtual  void SetStencilTest(bool enable, CompareMode mode = CMP_ALWAYS, StencilOp pass = OP_KEEP, StencilOp fail = OP_KEEP, StencilOp zFail = OP_KEEP,
			unsigned stencilRef = 0, unsigned compareMask = M_MAX_UNSIGNED, unsigned writeMask = M_MAX_UNSIGNED) = 0;

		//��������
		virtual void Clear(unsigned flags, const Color& color = Color(0.0f, 0.0f, 0.0f, 0.0f), float depth = 1.0f, unsigned stencil = 0) = 0;

		///���Ʒ��������ĵļ�����
		virtual void Draw(PrimitiveType type, unsigned vertexStart, unsigned vertexCount) = 0;
		///�����������ļ�����
		virtual void Draw(PrimitiveType type, unsigned indexStart, unsigned indexCount, unsigned minVertex, unsigned vertexCount) = 0;

		///  �����޳�ģʽ
		virtual void SetCullMode(CullMode mode) = 0;
		///  �������ģʽ
		virtual void SetFillMode(FillMode mode) = 0;
		//   ���û��ģʽ
		virtual void SetBlendMode(BlendMode mode) = 0;
		//   �Ƿ�֧��SRGB��ɫ�ռ�
		virtual bool GetSRGBSupport() const = 0;

		//�Ƿ�֧�ָ������Թ���
		virtual bool GetAnisotropySupport() const = 0;

		//    ��ȡĬ�ϵ��������ģʽ
		virtual TextureFilterMode GetDefaultTextureFilterMode() const = 0;

		//		��ȡ������˵ȼ�
		virtual unsigned GetTextureAnisotropy() const = 0;
		//      ��ȡ��ʾ�豸����
		virtual IDisplayContext*   GetDisplayContext() const = 0;

		/// ���һ�����ٵ�GPUObject������GPUObject����
		virtual void AddGPUObject(GPUObject*  gpuObj) = 0;
		/// �Ƴ�һ��GPUObject������GPUObject����
		virtual void RemoveGPUObject(GPUObject* gpuObj) = 0;

		virtual int getTextureQuality() const = 0;
		//		���������
		virtual void BindTexture(ITexture* pTexture, TextureUnit unit) = 0;

		//��������RenderTarget
		virtual void ResetRenderTargets() = 0;

		//��ȡshaderVaration
		virtual IShaderVariation* GetShader(ShaderType type, const std::string& name, const std::string& defines = "") const = 0;
		//��ȡshaderVaration
		virtual IShaderVariation* GetShader(ShaderType type, const char* name, const char* defines) const = 0;
		//��ȡshader��Դ·��
		virtual const Path& GetShaderPath() const = 0;
		//����shader��Դ·��
		virtual void  SetShaderPath(std::string path) = 0; 

		//����һ��float
		virtual void SetShaderParameter(StringHash param, float value) = 0;
		//����һ��float������
		virtual void SetShaderParameter(StringHash param, const float* data, unsigned count) = 0;
		//����һ��Vector2
		virtual void SetShaderParameter(StringHash param, const Vector2& vector) = 0;
		//����һ��Vector4
		virtual void SetShaderParameter(StringHash param, const Vector4& vector) = 0;
		//����һ��Vector3
		virtual void SetShaderParameter(StringHash param, const Vector3& vector) = 0;
		///����һ��3x3����
		virtual void SetShaderParameter(StringHash param, const Matrix3x3& matrix) = 0;
		///����һ��4x3����
		virtual void SetShaderParameter(StringHash param, const Matrix3x4& matrix) = 0;
		//����һ��4x4����
		virtual void SetShaderParameter(StringHash param, const Matrix4x4& matrix) = 0;
		///����һ��Color
		virtual void SetShaderParameter(StringHash param, const Color& color) = 0;
		//�Ƿ��в���
		virtual bool HasShaderParameter(StringHash param) = 0;
		//���������Ƿ���Ҫ���£���������ڲ������
		virtual bool NeedParameterUpdate(ShaderParameterGroup group, const void* source) = 0;
		//����Shader����Դ
		virtual void ClearParameterSources() =0;
		///����任Shader����Դ
		virtual void ClearTransformSources() = 0;
		///����ָ����shader����Դ��
		virtual void ClearParameterSource(ShaderParameterGroup group) = 0;

	protected:

		 

	};
}