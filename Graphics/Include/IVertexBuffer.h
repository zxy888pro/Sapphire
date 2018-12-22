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

		//Ҫд�뻺����ʱ����������������ɹ�������һ��ָ�롣 ��ѡ�Ƿ���������Χ������
		virtual void* Lock(unsigned start, unsigned count, bool discard = false) = 0;
		//������������Ӧ�øı䵽GPU
		virtual void UnLock() = 0;
		
		virtual bool  IsLocked() const = 0;

		virtual bool SetData(void* ptr) = 0;

		virtual bool SetDataRange(const void* data, unsigned start, unsigned count, bool discard = false) = 0;

		virtual bool SetSize(uint vertexCount, unsigned elementMask, bool dynamic = false) = 0;

		virtual uint GetUID() const = 0;

		//����CPU�˵�Ӱ������
		virtual byte* GetShadowData() const = 0;
		//����CPU�˵�Ӱ�����ݵ�����ָ��
		virtual SharedArrayPtr<unsigned char> GetShadowDataShared() const = 0;

	};
}