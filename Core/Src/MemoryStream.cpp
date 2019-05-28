#include "Sapphire.h"
#include "MemoryStream.h"
#include <sstream>

namespace Sapphire
{

	MemoryStream::MemoryStream()
	{
		m_uGrowBytes = 1024;
		m_uPos = 0;
		m_uBufSize = 0;
		m_pBuffer = NULL;
		m_uFileSize = 0;
		m_bAutoDeleteBuffer = true;
	}

	MemoryStream::MemoryStream(ulong growBytes /*= 1024*/)
	{
		m_assert(m_uGrowBytes <= 0xffffffff);
		m_uGrowBytes = growBytes;
		m_uPos = 0;
		m_uBufSize = 0;
		m_pBuffer = NULL;
		m_uFileSize = 0;
		m_bAutoDeleteBuffer = true;
	}

	MemoryStream::MemoryStream(void* pData, ulong size, ulong growBytes /*= 1024*/)
	{
		m_assert(pData != NULL && size != 0);
		m_assert(growBytes <= 0xffffffff);

		m_uGrowBytes = growBytes;
		m_uPos = 0;
		m_uBufSize = size;
		m_pBuffer =(byte*) pData;
		m_uFileSize = growBytes ==0 ? size : 0;
		m_bAutoDeleteBuffer = false;
	}

	MemoryStream::~MemoryStream()
	{
		if (m_pBuffer)
			Release();
		m_assert(m_pBuffer == NULL);

		m_uGrowBytes = 0;
		m_uPos = 0;
		m_uBufSize = 0;
		m_uFileSize = 0;
		m_bAutoDeleteBuffer = false;
	}

	bool MemoryStream::Open(void* pData, ulong size)
	{
		m_assert(pData != NULL && size != 0);
		if (m_pBuffer)
			Release();

		m_uPos = 0;
		m_uBufSize = size;
		m_pBuffer = (byte*)pData;
		m_uFileSize = m_uGrowBytes == 0 ? size : 0;
		m_bAutoDeleteBuffer = false;
		return true;
	}

	bool MemoryStream::Open(Path filePath, uint mode)
	{
		return false;
	}

	ulonglong MemoryStream::Read(void* buffer, ulong toRead)
	{
		if (buffer == NULL || toRead == 0)
			return -1;

		if (m_uPos >= m_uFileSize) // ������
			return -1;

		if (m_pBuffer == NULL) return false;

		uint bytesOfRead = toRead;
		if (m_uPos + toRead > m_uFileSize)
			bytesOfRead = (uint)(m_uFileSize - m_uPos);

		memcpy((byte*)buffer, (byte*)m_pBuffer + m_uPos, bytesOfRead);
		m_uPos += bytesOfRead;
		return bytesOfRead;
	}

	bool MemoryStream::Write(const void* buffer, ulong toWrite)
	{
		if (buffer == NULL || toWrite == 0)
			return false;

		if (m_uPos + toWrite > m_uBufSize)
		{
			if (m_uGrowBytes == 0)
				return false;

			GrowFile(m_uPos + toWrite);
		}

		m_assert(m_uPos + toWrite <= m_uBufSize);  

		memcpy((byte*)m_pBuffer + m_uPos, (byte*)buffer, toWrite);

		m_uPos += toWrite;

		if (m_uPos > m_uFileSize)
			m_uFileSize = m_uPos;

		return true;
	}

	bool MemoryStream::Seek(ulonglong offset, int from)
	{
		if (m_pBuffer == NULL)
			return false;

		UINT64 pos = 0;
		switch (from)
		{
		case ESTREAM_SEEK_BEGIN:
			pos = 0;
			break;
		case ESTREAM_SEEK_END:
			pos = m_uFileSize;
			break;
		case ESTREAM_SEEK_CUR:
			pos = m_uPos;
			break;
		default:
			return false;
		}
		pos += offset;

		if (pos < 0)
			return false;

		if (pos > m_uFileSize)
		{
			if (m_uGrowBytes > 0)
				GrowFile(pos);
		}
		m_uPos = pos;
		return true;
	}

	bool MemoryStream::SeekToBegin()
	{
		return Seek(0, ESTREAM_SEEK_BEGIN);
	}

	bool MemoryStream::SeekToEnd()
	{
		 return Seek(0, ESTREAM_SEEK_END);;
	}

	ulonglong MemoryStream::GetCurPos()
	{
		return m_uPos;
	}

	bool MemoryStream::Flush()
	{
		return true;
	}

	ulonglong MemoryStream::GetLength()
	{
		return m_uPos;
	}

	bool MemoryStream::Release()
	{
		m_assert(m_uFileSize <= m_uBufSize);

		m_uPos = 0;
		m_uBufSize = 0;
		m_uFileSize = 0;
		if (m_pBuffer && m_bAutoDeleteBuffer)
			free(m_pBuffer);
		m_pBuffer = NULL;
		return true;
	}



	std::string MemoryStream::ReadString(int nCharCount)
	{
		std::stringstream ss;
		if (!IsOpen())
		{
			SAPPHIRE_LOGERROR("error: MemoryStream is not Open, Buffer is NULL!");
			return "";
		}
		 
		char ch;
		int iCount = 0;
		;
		while (Read(&ch, sizeof(ch)) && iCount < nCharCount && ch != '\0') //// ���������ֽ���ɣ�ÿ���ֽ����λΪ1 ��С��0.   
		{
			ss << ch;
			++iCount;
		}
		return ss.str();
	}

	bool MemoryStream::ReadLine(std::string& str)
	{
		std::stringstream ss;
		if (!IsOpen())
		{
			SAPPHIRE_LOGERROR("error: MemoryStream is not Open, Buffer is NULL!");
			return "";
		}

		char ch;
		int iCount = 0;
		;
		while (Read(&ch, sizeof(ch)) && ch != '\0') //// ���������ֽ���ɣ�ÿ���ֽ����λΪ1 ��С��0.   
		{
			//�����س����߻��ж���Ϊһ�еĽ���
			if (ch == '\r')//�ǻس�/r
			{
				if (Read(&ch, sizeof(ch)))
				{
					if (ch != '\n')  //�س�����Ӧ�ýӻ���/n, ���ǵĻ������������ַ�Seek����һ��
					{
						Seek(-1, ESTREAM_SEEK_CUR); 
					}
					break;
				}else
				{
					break;
				}
			}
			ss << ch;
			++iCount;
		}
		str == ss.str();
		return true;
	}

	bool MemoryStream::IsOpen()
	{
		return m_pBuffer == NULL;
	}

	bool MemoryStream::GrowFile(UINT newLen)
	{
		//����´�С���ڵ�ǰ��������С
		if (newLen > m_uBufSize)
		{
			// ׼�����´���
			uint newBufSize = m_uBufSize;

			m_assert(m_uGrowBytes != 0);

			while (newBufSize < newLen)
				newBufSize += m_uGrowBytes;

			//���·���һ�����ڴ�
			byte* newBuf = NULL;
			if (m_pBuffer == NULL)
			{
				//Ϊ��ֱ�����·������
				newBuf = new byte[newBufSize];
			}
			else
			{
				newBuf = new byte[newBufSize];
				memcpy(newBuf, m_pBuffer, m_uBufSize); //�Ѿɵ�����ȥ
				delete[] m_pBuffer; //ɾ���ɵ�
			}

			if (newBuf == NULL)
				return false;

			m_pBuffer = newBuf;
			m_uBufSize = newBufSize;
		}
		return true;
	}

}