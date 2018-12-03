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

			if (!m_pGraphicDriver->IsDeviceLost())  //设备已经丢失
			{
				for (unsigned i = 0; i < MAX_VERTEX_STREAMS; ++i)
				{
					if (m_pGraphicDriver->GetVertexBuffer(i) == this)
						m_pGraphicDriver->SetVertexBuffer(0); //清空顶点缓冲区
				}

				m_pGraphicDriver->BindVBO(0);
				glDeleteBuffers(1, &m_uHwUID);
			}

			m_uHwUID = 0;
		}
	}


	void VertexBuffer::OnDeviceReset()
	{
		if (!m_uHwUID) //重新创建对象
		{
			Create();
			m_bDataLost = !UpdateToGPU(); //更新到GPU失败，数据丢失
		}
		else if (m_bDataPending)
			m_bDataLost = !UpdateToGPU();  //如果数据处于等待中，则再更新到GPU中

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
		//如果有影子数据就复制到其中
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
			//创建GPU对象
				glGenBuffers(1, &m_uHwUID);
			
			if (!m_uHwUID)
			{
				SAPPHIRE_LOGERROR("Create VertexBuffer Failed!");
				return false;
			}
			//设为当前VBO
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