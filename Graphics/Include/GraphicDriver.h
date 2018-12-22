#pragma once
#include <Graphics.h>

namespace Sapphire
{
	struct ITextureMgr;
	struct IImageMgr;
	struct ITexture;
	struct IVertexBuffer;
	struct IIndexBuffer;

	///���㶥����µ�CPU�˻�����
	struct ScratchBuffer
	{
		ScratchBuffer() :
			m_size(0),
			m_bReserved(false)
		{
		}

		SharedArrayPtr<unsigned char> data_;
		unsigned m_size;
		bool m_bReserved;
	};


	//OpenGL ��Ӳ������������ࡣ  
	//����IGraphicDriver�ӿ�, �޹�ͼ��API
	class GraphicDriver : public Singleton<GraphicDriver>
	{
	public:

		typedef std::unordered_map<std::string, ImageType>   ImageTypeNameMap;

		GraphicDriver();
		virtual ~GraphicDriver();

		void Init();

		void Release();

		Sapphire::ITextureMgr* getTextureMgr() const { return m_pTextureMgr; }

		Sapphire::IImageMgr* getImageMgr() const { return m_pImageMgr; }

		//��һ������ָ��������Ԫ
		//�ȼ����Ӧ������Ԫ
		//Ȼ����������
		void BindTexture(ITexture* pTexture, TextureUnit unit);

		void BindVBO(uint uHwUID);  //��VBO����

		bool  IsDeviceLost();

		bool GetAnisotropySupport(){ return m_bAnisotropySupport; }

		uint GetMaxAnisotropyLevels();

		PixelFormat GetPixelFormat(ImageType eImgType);

		PixelFormat GetPixelFormat(std::string szImageType);


		int getTextureQuality() const { return m_nTextureQuality; }

		void setTextureQuality(int val) { m_nTextureQuality = val; }

		virtual IVertexBuffer* GetVertexBuffer(uint index) const;

		virtual void  SetVertexBuffer(IVertexBuffer* vertexBuffer);

		virtual IIndexBuffer* GetIndexBuffer() const;

		virtual void SetIndexBuffer(IIndexBuffer* pIndexBuffer); //����������������ͬʱ����

		//����һ��ScratchBuffer
		virtual void* ReserveScratchBuffer(ulong size);
		//�ͷ�һ��ScratchBuffer
		virtual void FreeScratchBuffer(void* buffer);
		//����̫��ScratchBuffer
		virtual void CleanScratchBuffers();

	public:

		static int GetHWTextureWarpParam(TextureAddressMode mode);

		static int GetHWTextureFormat(PixelFormat eFormat);

		static int GetSWTextureFormat(PixelFormat eFormat);

		static uint GetHWTextureDataType(PixelFormat format);
		//Ӳ��Alpha��ʽ
		static int GetHWAlphaFormat();
		//Ӳ�����ȸ�ʽ
		static int GetHWLuminanceFormat();
		//Ӳ�����Ⱥ�alpha��ʽ
		static int GetHWLuminanceAlphaFormat();
		//Ӳ��RGB��ʽ
		static int GetHWRGBFormat();
		//Ӳ��RGBA��ʽ
		static int GetHWRGBAFormat();
		
		
	private:

		void CheckFeature();

		ITextureMgr*   m_pTextureMgr;
		IImageMgr*     m_pImageMgr;

		int            m_nTextureQuality;
		bool		   m_bAnisotropySupport;
		uint           m_nMaxTextureUnits;
		ImageTypeNameMap   m_imagetypeNames;
		
		uint m_curBindVBO;  //��ǰ��VBO
		uint m_curBindUBO;  //��ǰUBO
		uint m_curBindFBO;  //��ǰ��FBO
		uint m_sysFBO;      //ϵͳFBO

		//���õĶ��㻺������
		IVertexBuffer* m_vertexBuffers[MAX_VERTEX_STREAMS];

		///��ǰʹ��indexBuffer
		IIndexBuffer* m_indexBuffer;

		//ScratchBuffers
		std::vector<ScratchBuffer> m_scratchBuffers;
		ulong m_maxScratchBufferRequest; //��ǰ֡�����ScratchBuffer���ֵ

	};

}
