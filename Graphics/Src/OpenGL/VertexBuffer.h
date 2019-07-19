#pragma once
#include "GraphicsDefs.h"
#include "Graphics.h"
#include "GPUObject.h"
#include "ArrayPtr.h"
#include "IVertexBuffer.h"


namespace Sapphire
{
	class GLGraphicDriver;

	class VertexBuffer : public BaseObject, public GPUObject , public IVertexBuffer
	{
	public:
		VertexBuffer( Core* pCore, IGraphicDriver* pDriver);
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

		//Ҫд�뻺����ʱ����������������ɹ�������һ��ָ�롣 ��ѡ�Ƿ���������Χ������
		virtual void* Lock(unsigned start, unsigned count, bool discard = false);
		//������������Ӧ�øı䵽GPU
		virtual void UnLock();

		virtual bool  IsLocked() const;

		virtual bool SetData(const void* ptr);  //����GPU������

		virtual bool SetDataRange(const void* data, unsigned start, unsigned count, bool discard = false);  //����GPU��һ������

		virtual bool SetSize(uint vertexCount, unsigned elementMask, bool dynamic = false);

		//����CPU�˵�Ӱ������
		virtual byte* GetShadowData() const;
		//����CPU�˵�Ӱ�����ݵ�����ָ��
		virtual SharedArrayPtr<unsigned char> GetShadowDataShared() const;

		//����ÿ��Ԫ�ش�С
		const static unsigned elementSize[]; 
		//����ÿ��Ԫ�ط�����
		static const unsigned elementComponents[];
		//ÿ��Ԫ�ض�ӦOpenGL����
		static const unsigned elementType[];
		//ÿ��Ԫ�صı�׼������
		static const unsigned elementNormalize[];


		virtual uint GetUID() const override;

	private:

		bool UpdateElementOffset();  //����Ԫ�ص�ƫ��ֵ

		bool Create();

		bool UpdateToGPU();

		SharedArrayPtr<byte> m_shadowData;
		//������
		uint  m_vertexCount;
		//�����С
		uint  m_vertexSize;
		//����Ԫ��λ����
		uint  m_elementMask;
		//��¼ÿ��Ԫ�ص�ƫ�Ƶ�ַ
		uint  m_elementOffset[MAX_VERTEX_ELEMENTS];
		//�Ƿ���Ӱ�ӻ�����
		bool  m_bShadowed;

		bool  m_bDynmic;
		//����������״̬
		LockState  m_eLockState;
		ulong   m_uLockStart;
		ulong   m_uLockCount;
		void*   m_lockScratchData;


		GLGraphicDriver* m_pGraphicDriver;
		
	};
}