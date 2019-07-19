#include "VertexBuffer.h"
#include "GLGraphicDriver.h"


namespace Sapphire
{

	//ÿ��Ԫ�صĴ�С����
	const unsigned VertexBuffer::elementSize[] =
	{
		3 * sizeof(float), // Position
		3 * sizeof(float), // Normal
		4 * sizeof(unsigned char), // Color
		2 * sizeof(float), // Texcoord1
		2 * sizeof(float), // Texcoord2
		3 * sizeof(float), // Cubetexcoord1
		3 * sizeof(float), // Cubetexcoord2
		4 * sizeof(float), // Tangent
		4 * sizeof(float), // Blendweights
		4 * sizeof(unsigned char), // Blendindices
		4 * sizeof(float), // Instancematrix1
		4 * sizeof(float), // Instancematrix2
		4 * sizeof(float) // Instancematrix3
	};

	//����Ԫ�ط�����
	const unsigned VertexBuffer::elementComponents[] =
	{
		3, // Position
		3, // Normal
		4, // Color
		2, // Texcoord1
		2, // Texcoord2
		3, // Cubetexcoord1
		3, // Cubetexcoord2
		4, // Tangent
		4, // Blendweights
		4, // Blendindices
		4, // Instancematrix1
		4, // Instancematrix2
		4 // Instancematrix3
	};

	//����Ԫ����������
	const unsigned VertexBuffer::elementType[] =
	{
		GL_FLOAT, // Position
		GL_FLOAT, // Normal
		GL_UNSIGNED_BYTE, // Color
		GL_FLOAT, // Texcoord1
		GL_FLOAT, // Texcoord2
		GL_FLOAT, // Cubetexcoord1
		GL_FLOAT, // Cubetexcoord2
		GL_FLOAT, // Tangent
		GL_FLOAT, // Blendweights
		GL_UNSIGNED_BYTE, // Blendindices
		GL_FLOAT, // Instancematrix1
		GL_FLOAT, // Instancematrix2
		GL_FLOAT // Instancematrix3
	};

	//����Ԫ���Ƿ��׼��
	const unsigned VertexBuffer::elementNormalize[] =
	{
		GL_FALSE, // Position
		GL_FALSE, // Normal
		GL_TRUE, // Color
		GL_FALSE, // Texcoord1
		GL_FALSE, // Texcoord2
		GL_FALSE, // Cubetexcoord1
		GL_FALSE, // Cubetexcoord2
		GL_FALSE, // Tangent
		GL_FALSE, // Blendweights
		GL_FALSE, // Blendindices
		GL_FALSE, // Instancematrix1
		GL_FALSE, // Instancematrix2
		GL_FALSE // Instancematrix3
	};

	uint VertexBuffer::GetUID() const
	{
		return m_uHwUID;
	}

	VertexBuffer::VertexBuffer(Core* pCore, IGraphicDriver* pDriver) :
		BaseObject(pCore),
		GPUObject(pDriver),
		m_vertexCount(0),
		m_vertexSize(0),
		m_bShadowed(false),
		m_bDynmic(false),
		m_elementMask(0),
		m_eLockState(LOCK_NONE),
		m_lockScratchData(0),
		m_shadowData(0),
		m_uLockStart(0),
		m_uLockCount(0)
	{
		UpdateElementOffset();
		m_pGraphicDriver = dynamic_cast<GLGraphicDriver*>(pDriver);
		m_assert(m_pGraphicDriver);
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
		return m_bDynmic;
	}

	bool VertexBuffer::IsShadowed() const
	{
		return m_bShadowed;
	}

	void VertexBuffer::SetShadowed(bool enable)
	{
		if (m_pGraphicDriver == NULL)
			enable = true;   //���û��GraphicDriver ���ܹر�Ӱ�ӻ�����
		if (enable != m_bShadowed)
		{
			if (enable && m_vertexCount && m_vertexSize)
			{
				m_shadowData = new byte[m_vertexCount && m_vertexSize];
			}
			else
			{
				m_shadowData.Reset();
			}
			m_bShadowed = true;
		}
	}

	void* VertexBuffer::Lock(unsigned start, unsigned count, bool discard /*= false*/)
	{
		if (m_eLockState != LOCK_NONE)
		{
			SAPPHIRE_LOGERROR("Vertex buffer already locked");
			return NULL;
		}
		if (m_vertexSize == 0)
		{
			SAPPHIRE_LOGERROR("Vertex elements not defined, can not lock vertex buffer");
			return NULL;
		}
		if (start + count > m_vertexCount)
		{
			SAPPHIRE_LOGERROR("Illegal range for locking vertex buffer");
			return NULL;
		}
		if (!count)
			return 0;
		m_uLockStart = start;
		m_uLockCount = count;

		if (m_shadowData)
		{
			m_eLockState = LOCK_SHADOW;
			return m_shadowData.Get() + start * m_vertexSize;
		}
		else if (m_pGraphicDriver)
		{
			//û��Ӱ�ӻ��棬������ʱ��
			m_eLockState = LOCK_SCRATCH;
			m_lockScratchData = m_pGraphicDriver->ReserveScratchBuffer(count * m_vertexSize); 
			return m_lockScratchData;
		}

		return NULL;
	}

	void VertexBuffer::UnLock()
	{
		switch (m_eLockState)
		{
		case Sapphire::LOCK_SHADOW:
			SetDataRange(m_shadowData.Get() + m_uLockStart * m_vertexSize, m_uLockStart, m_uLockCount); //���»������ϴ���GPU
			m_eLockState = LOCK_NONE;
			break;
		case Sapphire::LOCK_SCRATCH:
			SetDataRange(m_lockScratchData, m_uLockStart, m_uLockCount);  //���µ�GPU���Ƿ���ʱ������
			if (m_pGraphicDriver)
				m_pGraphicDriver->FreeScratchBuffer(m_lockScratchData);
			m_lockScratchData = 0;
			m_eLockState = LOCK_NONE;
			break;
		default:
			break;
		}

	}

	bool VertexBuffer::IsLocked() const
	{
		return m_eLockState == LOCK_NONE;
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

	//����һ��������
	bool VertexBuffer::SetDataRange(const void* data, unsigned start, unsigned count, bool discard /*= false*/)
	{
		//�����ȫ����ֱ��SetData
		if (start == 0 && count == m_vertexCount)
			return SetData(data);
		if (!data)
		{
			SAPPHIRE_LOGERROR("Null pointer for vertex buffer data");
			return false;
		}
		if (!m_vertexSize)
		{
			SAPPHIRE_LOGERROR("Vertex elements not defined, can not set vertex buffer data");
			return false;
		}
		if (start + count > m_vertexCount)
		{
			SAPPHIRE_LOGERROR("Illegal range for setting new vertex buffer data");
			return false;	
		}

		if (!count)
			return true;
		if (m_shadowData && m_shadowData.Get() + start * m_vertexSize != data) //����ڴ��ַ
		{
			memcpy(m_shadowData.Get() + start * m_vertexSize, data, count * m_vertexSize);
		}
		if (m_uHwUID)
		{
			if (!m_pGraphicDriver->IsDeviceLost())
			{
				m_pGraphicDriver->BindVBO(m_uHwUID);
				if (!discard || start != 0) //�����Ļ�����BufferData
					glBufferSubData(GL_ARRAY_BUFFER, start * m_vertexSize, count * m_vertexSize, data);
				else
					glBufferData(GL_ARRAY_BUFFER, count * m_vertexSize, data, m_bDynmic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
			}
			else
			{
				SAPPHIRE_LOGERROR("Vertex buffer data assignment while device is lost");
				m_bDataPending = true;
			}
		}
		return true;
	}

	bool VertexBuffer::SetSize(uint vertexCount, unsigned elementMask, bool dynamic /*= false*/)
	{
		UnLock(); 
		m_bDynmic = dynamic;
		m_vertexCount = vertexCount;
		m_elementMask = elementMask;

		UpdateElementOffset(); 

		if (m_bShadowed && m_vertexCount && m_vertexSize)
		{
			//����Ӱ���ڴ�
			m_shadowData = new byte[m_vertexCount* m_vertexSize];
		}
		else
		{
			//�ͷŵ�
			m_shadowData.Reset();
		}
		//��Ҫ���´���VBO����
		return Create();
	}

	byte* VertexBuffer::GetShadowData() const
	{
		return m_shadowData.Get();
	}

	Sapphire::SharedArrayPtr<unsigned char> VertexBuffer::GetShadowDataShared() const
	{
		return m_shadowData;
	}

	bool VertexBuffer::UpdateElementOffset()
	{
		uint elementOffset = 0;
		for (unsigned i = 0; i < MAX_VERTEX_ELEMENTS; ++i)
		{
			//���θ���ÿһ��Ԫ�ص�ƫ�Ƶ�ַ
			if (m_elementMask & (1 << i))   //��Ԫ���ڴa����ȡ���x��ÿ��Ԫ��
			{
				m_elementOffset[i] = elementOffset; //��0�_ʼ
				//��һ��Ԫ�ص�ƫ�Ƶ�ַ=��ǰƫ�Ƶ�ַ+��ǰԪ�ش�С
				elementOffset += elementSize[i]; 
			}
			else
				m_elementOffset[i] = NO_ELEMENT;
		}
		//���һ��ƫ�Ƶ�ַ�൱���ܴ�С
		m_vertexSize = elementOffset;
		return true;
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
		if (glIsBuffer(m_uHwUID)&&m_shadowData)
			return SetData(m_shadowData.Get());
		else
			return false;
	}

}