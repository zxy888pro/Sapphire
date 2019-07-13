#include "MemoryBuffer.h"




namespace Sapphire
{



	MemoryBuffer::MemoryBuffer(void* data, unsigned size):
		Deserializer(size),
		buffer_((unsigned char*)data),
		readOnly_(false)
	{
		if (!buffer_)
			m_uSize = 0;
	}

	MemoryBuffer::MemoryBuffer(const void* data, unsigned size) :
		Deserializer(size),
		buffer_((unsigned char*)data),
		readOnly_(true)
	{
		if (!buffer_)
			m_uSize = 0;
	}

	MemoryBuffer::MemoryBuffer(std::vector<byte>& data) :
		Deserializer(data.size()),
		buffer_(&data[0]),
		readOnly_(false)
	{

	}



	unsigned MemoryBuffer::Read(void* dest, unsigned size)
	{
		if (size + m_uPosition > m_uSize)
			size = m_uSize - m_uPosition;
		if (!size)
			return 0;

		unsigned char* srcPtr = &buffer_[m_uPosition];
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

	unsigned MemoryBuffer::Seek(unsigned position)
	{
		if (position > m_uSize)
			position = m_uSize;

		m_uPosition = position;
		return m_uPosition;
	}

	unsigned MemoryBuffer::Write(const void* data, unsigned size)
	{
		if (size + m_uPosition > m_uSize)
			size = m_uSize - m_uPosition;
		if (!size)
			return 0;

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

}