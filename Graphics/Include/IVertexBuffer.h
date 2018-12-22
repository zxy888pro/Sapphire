#pragma once
#include "Graphics.h"
#include "ArrayPtr.h"


namespace Sapphire
{
	struct IVertexBuffer
	{
		virtual uint GetVertexSize() const = 0;

		virtual uint GetVertexCount() const = 0;

		virtual uint GetElementMask() const = 0;

		virtual uint GetElementOffset(VertexElement element) const = 0;

		virtual bool IsDynamic() const = 0;

		virtual bool IsShadowed() const = 0;

		virtual void SetShadowed(bool enable) = 0;

		//要写入缓冲区时，如果锁定缓冲区成功，返回一个指针。 可选是否丢弃超出范围的数据
		virtual void* Lock(unsigned start, unsigned count, bool discard = false) = 0;
		//解锁缓冲区，应用改变到GPU
		virtual void UnLock() = 0;
		
		virtual bool  IsLocked() const = 0;

		virtual bool SetData(void* ptr) = 0;

		virtual bool SetDataRange(const void* data, unsigned start, unsigned count, bool discard = false) = 0;

		virtual bool SetSize(uint vertexCount, unsigned elementMask, bool dynamic = false) = 0;

		virtual uint GetUID() const = 0;

		//返回CPU端的影子数据
		virtual byte* GetShadowData() const = 0;
		//返回CPU端的影子数据的智能指针
		virtual SharedArrayPtr<unsigned char> GetShadowDataShared() const = 0;

	};
}