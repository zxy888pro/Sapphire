#include "ConstantBuffer.h"
#include "GLGraphicDriver.h"

namespace Sapphire
{

	ConstantBuffer::ConstantBuffer(Core* pCore, IGraphicDriver* pDriver):
		BaseObject(pCore),
		GPUObject(pDriver),
		m_uSize(0),
		m_bDirty(false),
		m_pDriver(NULL)
	{
		m_shadowData.Reset();
		m_pDriver = dynamic_cast<GLGraphicDriver*>(pDriver);
		m_assert(pDriver);
	}

	ConstantBuffer::~ConstantBuffer()
	{
		Release();
	}

	void ConstantBuffer::Release()
	{
		throw std::logic_error("The method or operation is not implemented.");
	}


	void ConstantBuffer::OnDeviceReset()
	{
		if (m_uSize)
			SetSize(m_uSize); //重新创建
	}



	bool ConstantBuffer::SetSize(uint size)
	{
		if (!size)
		{
			SAPPHIRE_LOGERROR("Can not create zero-sized constant buffer");
			return false;
		}

		// Round up to next 16 bytes
		size += 15;
		size &= 0xfffffff0;

		m_uSize = size;
		m_bDirty = false;
		m_shadowData = new unsigned char[m_uSize];
		memset(m_shadowData.Get(), 0, m_uSize);

		if (m_pDriver)
		{
#ifndef GL_ES_VERSION_2_0
			if (!m_uHwUID)
				glGenBuffers(1, &m_uHwUID); //创建缓冲区对象
			m_pDriver->BindUBO(m_uHwUID); //绑定缓冲区对象
			//为UBO更新数据
			glBufferData(GL_UNIFORM_BUFFER, m_uSize, m_shadowData.Get(), GL_DYNAMIC_DRAW);
#endif
		}

		return true;
	}

	uint ConstantBuffer::GetSize() const
	{
		return m_uSize;
	}

	void ConstantBuffer::SetParameter(uint offset, uint size, const void* pdata)
	{
		if (offset + size > m_uSize)
			return; // 越界了

		memcpy(&m_shadowData[offset], pdata, size);
		m_bDirty = true;
	}

	void ConstantBuffer::SetVector3ArrayParameter(unsigned offset, unsigned rows, const void* data)
	{
		if (offset + rows * 4 * sizeof(float) > m_uSize)
			return; //越界了

		float* dest = (float*)&m_shadowData[offset];
		const float* src = (const float*)data;

		while (rows--)
		{
			*dest++ = *src++;
			*dest++ = *src++;
			*dest++ = *src++;
			++dest; // 跳过w坐标
		}

		m_bDirty = true;
	}

	void ConstantBuffer::Apply()
	{
		if (m_bDirty && m_uHwUID)
		{
#ifndef GL_ES_VERSION_2_0
			m_pDriver->BindUBO(m_uHwUID);
			glBufferData(GL_UNIFORM_BUFFER, m_uSize, m_shadowData.Get(), GL_DYNAMIC_DRAW);
#endif
			m_bDirty = false;
		}
	}

	bool ConstantBuffer::IsDirty()
	{
		return m_bDirty;
	}

}