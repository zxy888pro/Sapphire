#pragma once
#include "GraphicsDefs.h"
#include "Graphics.h"
#include "GraphicDriver.h"
#include "GPUObject.h"
#include "ArrayPtr.h"
#include "IVertexBuffer.h"


namespace Sapphire
{
	class VertexBuffer : public BaseObject, public GPUObject , public IVertexBuffer
	{
	public:
		VertexBuffer();
		virtual ~VertexBuffer();


		virtual void Release() override;

		virtual void OnDeviceReset() override;

		virtual uint GetVertexSize() const;

		virtual uint GetVertexCount() const;

		virtual uint GetElementMask() const;

		virtual uint GetElementOffset(VertexElement element) const;	

		virtual bool IsDynamic() const;

		virtual bool IsShadowed() const;

		virtual void SetShadowed(bool enable);

		//要写入缓冲区时，如果锁定缓冲区成功，返回一个指针。 可选是否丢弃超出范围的数据
		virtual void* Lock(unsigned start, unsigned count, bool discard = false);
		//解锁缓冲区，应用改变到GPU
		virtual void UnLock();

		virtual bool SetData(const void* ptr);

		virtual bool SetDataRange(const void* data, unsigned start, unsigned count, bool discard = false);

		virtual bool SetSize(uint vertexCount, unsigned elementMask, bool dynamic = false);

		//返回CPU端的影子数据
		virtual byte* GetShadowData() const;
		//返回CPU端的影子数据的智能指针
		virtual SharedArrayPtr<unsigned char> GetShadowDataShared() const;


	private:

		bool UpdateElementOffset();

		bool Create();

		bool UpdateToGPU();

		SharedArrayPtr<byte> m_shadowData;
		//顶点数
		uint  m_vertexCount;
		//顶点大小
		uint  m_vertexSize;
		//顶点元素位掩码
		uint  m_elementMask;
		//记录每个元素的偏移地址
		uint  m_elementOffset[MAX_VERTEX_ELEMENTS];
		//是否有影子缓冲区
		bool  m_bShadowed;

		bool  m_bDynmic;

		GraphicDriver* m_pGraphicDriver;
		
	};
}