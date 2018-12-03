#include "VertexBuffer.h"



namespace Sapphire
{

	VertexBuffer::VertexBuffer():
		m_vertexCount(0),
		m_vertexSize(0),
		m_bShadowed(false),
		m_bDynmic(false),
		m_elementMask(0)
	{
		UpdateElementOffset();
		m_pGraphicDriver = GraphicDriver::GetSingletonPtr();
		if (m_pGraphicDriver)
		{
			m_bShadowed = true;
		}
	}

	VertexBuffer::~VertexBuffer()
	{
		Release();
	}

	void VertexBuffer::Release()
	{
		UnLock();

		if (m_uHwUID)
		{
			if (!m_pGraphicDriver)
				return;

			if (!m_pGraphicDriver->IsDeviceLost())  //�豸�Ѿ���ʧ
			{
				for (unsigned i = 0; i < MAX_VERTEX_STREAMS; ++i)
				{
					if (m_pGraphicDriver->GetVertexBuffer(i) == this)
						m_pGraphicDriver->SetVertexBuffer(0); //��ն��㻺����
				}

				m_pGraphicDriver->BindVBO(0);
				glDeleteBuffers(1, &m_uHwUID);
			}

			m_uHwUID = 0;
		}
	}


	void VertexBuffer::OnDeviceReset()
	{
		if (!m_uHwUID) //���´�������
		{
			Create();
			m_bDataLost = !UpdateToGPU(); //���µ�GPUʧ�ܣ����ݶ�ʧ
		}
		else if (m_bDataPending)
			m_bDataLost = !UpdateToGPU();  //������ݴ��ڵȴ��У����ٸ��µ�GPU��

		m_bDataPending = false;
	}

	uint VertexBuffer::GetVertexSize() const
	{
		return m_vertexSize;
	}

	uint VertexBuffer::GetVertexCount() const
	{
		return m_vertexCount;
	}

	uint VertexBuffer::GetElementMask() const
	{
		return m_elementMask;
	}

	uint VertexBuffer::GetElementOffset(VertexElement element) const
	{
		return m_elementOffset[element];
	}

	bool VertexBuffer::IsDynamic() const
	{
		return false;
	}

	bool VertexBuffer::IsShadowed() const
	{
		return m_bShadowed;
	}

	void VertexBuffer::SetShadowed(bool enable)
	{
		m_bShadowed = enable;
	}

	void* VertexBuffer::Lock(unsigned start, unsigned count, bool discard /*= false*/)
	{
		return NULL;
	}

	void VertexBuffer::UnLock()
	{

	}

	bool VertexBuffer::SetData(const void* ptr)
	{
		if (ptr == NULL)
		{
			SAPPHIRE_LOGERROR("Null Pointer for Vertex Buffer Data");
			return false;
		}
		//�����Ӱ�����ݾ͸��Ƶ�����
		if (m_shadowData.NotNull() && ptr != m_shadowData.Get())
		{
			memcpy(m_shadowData.Get(), ptr, m_vertexCount * m_vertexSize);
		}
		if (m_uHwUID)
		{
			if (!m_pGraphicDriver->IsDeviceLost())
			{
				m_pGraphicDriver->BindVBO(m_uHwUID);
				glBufferData(GL_ARRAY_BUFFER, m_vertexSize* m_vertexCount, ptr, m_bDynmic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
			}
			else
			{
				SAPPHIRE_LOGERROR("Set VertexBufferData Failed! device is lost!");
				m_bDataPending = true;
			}
		}
		m_bDataLost = false;
		return true;
	}

	bool VertexBuffer::SetDataRange(const void* data, unsigned start, unsigned count, bool discard /*= false*/)
	{
		return false;
	}

	bool VertexBuffer::SetSize(uint vertexCount, unsigned elementMask, bool dynamic /*= false*/)
	{
		return false;
	}

	byte* VertexBuffer::GetShadowData() const
	{
		return NULL;
	}

	Sapphire::SharedArrayPtr<unsigned char> VertexBuffer::GetShadowDataShared() const
	{
		return m_shadowData;
	}

	bool VertexBuffer::UpdateElementOffset()
	{
		return false;
	}

	bool VertexBuffer::Create()
	{
		if (!m_vertexCount || !m_elementMask)
		{
			Release();
			return true;
		}
		if (m_pGraphicDriver)
		{
			if (m_pGraphicDriver->IsDeviceLost())
			{
				SAPPHIRE_LOGERROR("VertexBuffer create Failed! device is Lost!");
				return false;
			}
			if (!m_uHwUID)
			//����GPU����
				glGenBuffers(1, &m_uHwUID);
			
			if (!m_uHwUID)
			{
				SAPPHIRE_LOGERROR("Create VertexBuffer Failed!");
				return false;
			}
			//��Ϊ��ǰVBO
			m_pGraphicDriver->BindVBO(m_uHwUID);
			glBufferData(GL_ARRAY_BUFFER, m_vertexCount * m_vertexSize, NULL, m_bDynmic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);

		}

		return false;
	}

	bool VertexBuffer::UpdateToGPU()
	{
		return false;
	}

}