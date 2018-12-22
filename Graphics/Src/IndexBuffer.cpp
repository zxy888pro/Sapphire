#include "IndexBuffer.h"


namespace Sapphire
{

	IndexBuffer::IndexBuffer():
		m_indexCount(0),
		m_indexSize(0),
		m_eLockState(LOCK_NONE),
		m_scratchBuffer(0),
		m_ulockCount(0),
		m_ulockStart(0),
		m_bShadowed(false),
		m_bDynamic(false)
	{
		m_pGraphicDriver = GraphicDriver::GetSingletonPtr();
		if (m_pGraphicDriver)
		{
			m_bShadowed = true;
		}
	}

	IndexBuffer::~IndexBuffer()
	{
		Release();
	}

	void IndexBuffer::Release()
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
					if (m_pGraphicDriver->GetIndexBuffer() == this)
						m_pGraphicDriver->SetIndexBuffer(0); //��ն��㻺����
				}

				m_pGraphicDriver->BindVBO(0);
				glDeleteBuffers(1, &m_uHwUID);
			}

			m_uHwUID = 0;
		}
	}

	void IndexBuffer::OnDeviceReset()
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

	bool IndexBuffer::IsDynamic() const
	{
		return m_bDynamic;
	}

	bool IndexBuffer::IsShadowed() const
	{
		return m_bShadowed;
	}

	void IndexBuffer::SetShadowed(bool enable)
	{
		if (m_pGraphicDriver == NULL)
			enable = true;   //���û��GraphicDriver ���ܹر�Ӱ�ӻ�����
		if (enable != m_bShadowed)
		{
			if (enable && m_indexCount && m_indexSize)
			{
				m_shadowBuffer = new byte[m_indexCount * m_indexSize];
			}
			else
			{
				m_shadowBuffer.Reset();
			}
			m_bShadowed = true;
		}
	}

	bool IndexBuffer::SetSize(unsigned indexCount, bool largeIndices, bool dynamic /*= false*/)
	{
		UnLock();
		m_bDynamic = dynamic;
		m_indexCount = indexCount;
		m_indexSize = (unsigned)(largeIndices ? sizeof(unsigned) : sizeof(unsigned short));  //16/32λ����


		if (m_bShadowed && m_indexCount && m_indexSize)
		{
			//����Ӱ���ڴ�
			m_shadowBuffer = new byte[m_indexCount * m_indexSize];
		}
		else
		{
			//�ͷŵ�
			m_shadowBuffer.Reset();
		}
		//��Ҫ���´���VBO����
		return Create();
	}

	bool IndexBuffer::SetData(const void* data)
	{
		if (!data)
		{
			SAPPHIRE_LOGERROR("Null pointer for index buffer data");
			return false;
		}
		if (!m_indexSize)
		{
			SAPPHIRE_LOGERROR("Index size not defined, can not set index buffer data");
			return false;
		}
		if (m_shadowBuffer && data != m_shadowBuffer.Get())
		{
			memcpy(m_shadowBuffer.Get(), data, m_indexCount * m_indexSize);  //�������ݵ�Ӱ�ӻ���
		}
		if (m_uHwUID)
		{
			if (!m_pGraphicDriver->IsDeviceLost())  //�豸û�ж�ʧ����EBO����������
			{
				m_pGraphicDriver->SetIndexBuffer(this);
				glBufferData(GL_ARRAY_BUFFER, m_indexCount* m_indexSize, data, m_bDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
			}
			else
			{
				SAPPHIRE_LOGERROR("Index buffer data assignment while device is lost");
				m_bDataPending = true;
			}
		}
		m_bDataLost = false;
		return true;
	}

	bool IndexBuffer::SetDataRange(const void* data, unsigned start, unsigned count, bool discard /*= false*/)
	{
		if (start == 0 && count == m_indexCount)
			SetData(data);
		if (!data)
		{
			SAPPHIRE_LOGERROR("Null pointer for index buffer data");
			return false;
		}
		if (!m_indexSize)
		{
			SAPPHIRE_LOGERROR("Index size not defined, can not set index buffer data");
			return false;
		}
		if (start + count > m_indexCount)
		{
			SAPPHIRE_LOGERROR("Illegal range for setting new index buffer data");
			return false;
		}
		if (!count)
			return true;
		if (m_shadowBuffer && m_shadowBuffer.Get() + start * m_indexSize != data)
			memcpy(m_shadowBuffer.Get() + start * m_indexSize, data, count * m_indexSize);

		if (m_uHwUID)
		{
			if (!m_pGraphicDriver->IsDeviceLost())
			{
				m_pGraphicDriver->SetIndexBuffer(this);
				if (!discard || start != 0)
					glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, start * m_indexSize, count * m_indexSize, data);
				else
					glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * m_indexSize, data, m_bDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
			}
			else
			{
				SAPPHIRE_LOGWARNING("Index buffer data assignment while device is lost");
				m_bDataPending = true;
			}
		}

		return true;
	}

	void* IndexBuffer::Lock(unsigned start, unsigned count, bool discard /*= false*/)
	{
		if (m_eLockState != LOCK_NONE)
		{
			SAPPHIRE_LOGERROR("Index buffer already locked");
			return NULL;
		}
		if (m_indexSize == 0)
		{
			SAPPHIRE_LOGERROR("Vertex elements not defined, can not lock vertex buffer");
			return NULL;
		}
		if (start + count > m_indexCount)
		{
			SAPPHIRE_LOGERROR("Illegal range for locking vertex buffer");
			return NULL;
		}
		if (!count)
			return 0;
		m_ulockStart = start;
		m_ulockCount = count;

		if (m_shadowBuffer)
		{
			m_eLockState = LOCK_SHADOW;
			return m_shadowBuffer.Get() + start * m_indexSize;
		}
		else if (m_pGraphicDriver)
		{
			//û��Ӱ�ӻ��棬������ʱ��
			m_eLockState = LOCK_SCRATCH;
			m_scratchBuffer = m_pGraphicDriver->ReserveScratchBuffer(count * m_indexSize);
			return m_scratchBuffer;
		}
		return NULL;
	}

	void IndexBuffer::UnLock()
	{
		switch (m_eLockState)
		{
		case Sapphire::LOCK_SHADOW:
			SetDataRange(m_shadowBuffer.Get() + m_ulockStart * m_indexSize, m_ulockStart, m_ulockCount); //���»������ϴ���GPU
			m_eLockState = LOCK_NONE;
			break;
		case Sapphire::LOCK_SCRATCH:
			SetDataRange(m_scratchBuffer, m_ulockStart, m_ulockCount);  //���µ�GPU���Ƿ���ʱ������
			if (m_pGraphicDriver)
				m_pGraphicDriver->FreeScratchBuffer(m_scratchBuffer);
			m_scratchBuffer = 0;
			m_eLockState = LOCK_NONE;
			break;
		default:
			break;
		}
	}

	bool IndexBuffer::IsLocked() const
	{
		return m_eLockState == LOCK_NONE;
	}

	uint IndexBuffer::GetIndexSize() const
	{
		return m_indexSize;
	}

	uint IndexBuffer::GetIndexCount() const
	{
		return m_indexCount;
	}

	byte* IndexBuffer::GetShadowData() const
	{
		return m_shadowBuffer.Get();
	}

	Sapphire::SharedArrayPtr<unsigned char> IndexBuffer::GetShadowDataShared() const
	{
		return m_shadowBuffer;
	}

	uint IndexBuffer::GetUID() const
	{
		return m_uHwUID;
	}

	bool IndexBuffer::Create()
	{
		if (!m_indexCount)
		{
			Release();
			return true;
		}

		if (m_pGraphicDriver)
		{
			if (m_pGraphicDriver->IsDeviceLost())
			{
				SAPPHIRE_LOGWARNING("Index buffer creation while device is lost");
				return true;
			}

			if (!m_uHwUID)
				glGenBuffers(1, &m_uHwUID);
			if (!m_uHwUID)
			{
				SAPPHIRE_LOGERROR("Failed to create index buffer");
				return false;
			}

			m_pGraphicDriver->SetIndexBuffer(this);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexCount * m_indexSize, 0, m_bDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
		}

		return true;
	}

	bool IndexBuffer::UpdateToGPU()
	{
		if (glIsBuffer(m_uHwUID) && m_shadowBuffer)
			return SetData(m_shadowBuffer.Get());
		else
			return false;
	}

}