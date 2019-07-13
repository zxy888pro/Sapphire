#include "VectorBuffer.h"



namespace Sapphire
{



	VectorBuffer::VectorBuffer()
	{

	}

	VectorBuffer::VectorBuffer(const std::vector<byte>& data)
	{
		SetData(data);
	}

	VectorBuffer::VectorBuffer(const void* data, unsigned size)
	{
		SetData(data, size);
	}

	VectorBuffer::VectorBuffer(Deserializer& source, unsigned size)
	{
		SetData(source, size);
	}

	unsigned VectorBuffer::Read(void* dest, unsigned size)
	{
		if (size + m_uPosition > m_uSize)
			size = m_uSize - m_uPosition;  //讀取不能超過最大範圍
		if (!size)
			return 0;

		unsigned char* srcPtr = &buffer_[m_uPosition]; //起始位置
		unsigned char* destPtr = (unsigned char*)dest;
		m_uPosition += size;

		unsigned copySize = size;
		while (copySize >= sizeof(unsigned))
		{
			*((unsigned*)destPtr) = *((unsigned*)srcPtr);
			srcPtr += sizeof(unsigned);
			destPtr += sizeof(unsigned);
			copySize -= sizeof(unsigned);
		}
		if (copySize & sizeof(unsigned short))
		{
			*((unsigned short*)destPtr) = *((unsigned short*)srcPtr);
			srcPtr += sizeof(unsigned short);
			destPtr += sizeof(unsigned short);
		}
		if (copySize & 1)
			*destPtr = *srcPtr;

		return size;
	}

	unsigned VectorBuffer::Seek(unsigned position)
	{
		if (position > m_uSize)
			position = m_uSize;

		m_uPosition = position;
		return m_uPosition;
	}

	unsigned VectorBuffer::Write(const void* data, unsigned size)
	{
		if (!size)
			return 0;

		if (size + m_uPosition > m_uSize)
		{
			m_uSize = size + m_uPosition;
			buffer_.resize(m_uSize);
		}

		unsigned char* srcPtr = (unsigned char*)data;
		unsigned char* destPtr = &buffer_[m_uPosition];
		m_uPosition += size;

		unsigned copySize = size;
		while (copySize >= sizeof(unsigned))
		{
			*((unsigned*)destPtr) = *((unsigned*)srcPtr);
			srcPtr += sizeof(unsigned);
			destPtr += sizeof(unsigned);
			copySize -= sizeof(unsigned);
		}
		if (copySize & sizeof(unsigned short))
		{
			*((unsigned short*)destPtr) = *((unsigned short*)srcPtr);
			srcPtr += sizeof(unsigned short);
			destPtr += sizeof(unsigned short);
		}
		if (copySize & 1)
			*destPtr = *srcPtr;

		return size;
	}

	void VectorBuffer::SetData(const std::vector<byte>& data)
	{
		buffer_ = data;
		m_uPosition = 0;
		m_uSize = data.size();
	}

	void VectorBuffer::SetData(const void* data, unsigned size)
	{
		if (!data)
			size = 0;

		buffer_.resize(size);
		if (size)
			memcpy(&buffer_[0], data, size);

		m_uPosition = 0;
		m_uSize = size;
	}

	void VectorBuffer::SetData(Deserializer& source, unsigned size)
	{
		buffer_.resize(size);
		unsigned actualSize = source.Read(&buffer_[0], size);
		if (actualSize != size)
			buffer_.resize(actualSize);

		m_uPosition = 0;
		m_uSize = actualSize;
	}

	void VectorBuffer::Clear()
	{
		buffer_.clear();
		m_uPosition = 0;
		m_uSize = 0;
	}

	void VectorBuffer::Resize(unsigned size)
	{
		buffer_.resize(size);
		m_uSize = size;
		if (m_uPosition > m_uSize)
			m_uPosition = m_uSize;
	}

}