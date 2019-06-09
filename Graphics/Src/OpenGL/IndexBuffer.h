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

		//Ҫд�뻺����ʱ����������������ɹ�������һ��ָ�롣 ��ѡ�Ƿ���������Χ������
		virtual void* Lock(unsigned start, unsigned count, bool discard = false);
		//������������Ӧ�øı䵽GPU
		virtual void UnLock();

		virtual bool IsLocked() const;

		virtual uint GetIndexSize() const;

		virtual uint GetIndexCount() const;

		//����CPU�˵�Ӱ������
		virtual byte* GetShadowData() const;
		//����CPU�˵�Ӱ�����ݵ�����ָ��
		virtual SharedArrayPtr<unsigned char> GetShadowDataShared() const;


		virtual uint GetUID() const override;

	private:

		bool Create();

		bool UpdateToGPU();

		//Ӱ�ӻ�����
		SharedArrayPtr<byte> m_shadowBuffer;  //Ӱ�ӻ����Scratch��������ֻ�ǹ̶�����ʱ
		//������
		uint  m_indexCount;
		//������С
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