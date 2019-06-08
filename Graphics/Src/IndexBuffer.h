#pragma once

#include "Graphics.h"
#include "GPUObject.h"
#include "IIndexBuffer.h"

namespace Sapphire
{
	class GLGraphicDriver;

	class IndexBuffer : public BaseObject, public GPUObject, public IIndexBuffer
	{
	public:
		IndexBuffer(Core* pCore);

		virtual ~IndexBuffer();

	
		virtual void Release() override;

		virtual void OnDeviceReset() override;

		virtual bool IsDynamic() const;

		virtual bool IsShadowed() const;

		virtual void SetShadowed(bool enable);

		virtual bool SetSize(unsigned indexCount, bool largeIndices, bool dynamic = false);

		virtual bool SetData(const void* data);

		virtual bool SetDataRange(const void* data, unsigned start, unsigned count, bool discard = false);

		//要写入缓冲区时，如果锁定缓冲区成功，返回一个指针。 可选是否丢弃超出范围的数据
		virtual void* Lock(unsigned start, unsigned count, bool discard = false);
		//解锁缓冲区，应用改变到GPU
		virtual void UnLock();

		virtual bool IsLocked() const;

		virtual uint GetIndexSize() const;

		virtual uint GetIndexCount() const;

		//返回CPU端的影子数据
		virtual byte* GetShadowData() const;
		//返回CPU端的影子数据的智能指针
		virtual SharedArrayPtr<unsigned char> GetShadowDataShared() const;


		virtual uint GetUID() const override;

	private:

		bool Create();

		bool UpdateToGPU();

		//影子缓冲区
		SharedArrayPtr<byte> m_shadowBuffer;  //影子缓冲和Scratch缓冲区别只是固定和临时
		//索引数
		uint  m_indexCount;
		//索引大小
		uint  m_indexSize;
		LockState  m_eLockState;
		uint    m_ulockStart;
		uint    m_ulockCount;
		//Scratch Buffer
		void*   m_scratchBuffer;

		bool  m_bShadowed;

		bool  m_bDynamic;

		GLGraphicDriver*  m_pGraphicDriver;


	};
}