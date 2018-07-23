#include "path.h"
#include "FileStream.h"
#include "baseException.h"
#include "charset.h"
#include <cassert>

namespace Sapphire
{
 


	FileStream::FileStream(ulong bufferSize)
	{
		m_filePath = "";
		//m_buffer = new byte[bufferSize];
	}

	FileStream::FileStream(Path filePath, uint mode, ulong bufferSize)
	{
		//m_buffer = new byte[bufferSize];
		Open(filePath, mode);
	}

	FileStream::~FileStream()
	{
		//safeDeleteArray(m_buffer);
	}

	bool FileStream::Open(Path filePath, uint mode)
	{
		if (IsOpen())
		{
			assert(Release());
		}
		if (mode & FileMode::FILE_EXIST)
		{
			if (!FileIsExistA(filePath.c_str()))
			{
				throw FileStreamException(filePath, FileStreamException::FSErrorCode::FSError_FileNotExist);
			}
			m_filePath = filePath;
		}
		std::ios_base::openmode iosmode =  0;
		if (mode & FileMode::FILE_READ)
		{
			iosmode |= std::ios_base::in;
		}
		if (mode & FileMode::FILE_WRITE)
		{
			iosmode |= std::ios_base::out;
		}
		if (mode & FileMode::FILE_APPEND)
		{
			iosmode |= std::ios_base::app;
		}
		if (mode & FileMode::FILE_CREATE)
		{
			iosmode |= std::ios_base::out;
			iosmode &= ~std::ios_base::in;
			iosmode &= std::ios_base::app;
		}
		iosmode |= std::ios_base::binary;
		m_fstream.open(filePath.c_str(), iosmode);
		
		if (!m_fstream.good())
		{
			return false;
		}

		return true;
	}

	ulonglong FileStream::Read(void* buffer, ulong toRead)
	{
		if (!IsOpen())
		{
			throw FileStreamException(FileStreamException::FSErrorCode::FSError_FileNotExist);
		}
		m_fstream.read((char*)buffer, toRead);
		ulonglong ret = m_fstream.gcount();
		return ret;
	}

	bool FileStream::Write(const void* buffer, ulong toWrite)
	{
		if (!IsOpen())
		{
			throw FileStreamException(FileStreamException::FSErrorCode::FSError_FileNotExist);
		}
		m_fstream.write((char*)buffer, toWrite);
		return true;
		
	}

	bool FileStream::Seek(ulonglong offset, int from)
	{
		if (!IsOpen())
		{
			throw FileStreamException(FileStreamException::FSErrorCode::FSError_FileNotExist);
		}
		std::ios::seekdir skdir = std::ios::beg;
		switch (from)
		{
		case FileBegin:
			skdir = std::ios::beg;
			break;
		case FileEnd:
			skdir = std::ios::end;
			break;
		case FileCurPos:
			skdir = std::ios::cur;
			break;
		default:
			break;
		}
		m_fstream.seekg(offset, skdir);
		return true;
	}

	bool FileStream::SeekToBegin()
	{
		Seek(0, FileBegin);
		return true;
	}

	bool FileStream::SeekToEnd()
	{
		Seek(0, FileEnd);
		return true;
	}

	ulonglong FileStream::GetCurPos()
	{
		if (!IsOpen())
		{
			throw FileStreamException(FileStreamException::FSErrorCode::FSError_FileNotExist);
		}
		ulonglong pos = m_fstream.tellg();
		return pos;
	}

	bool FileStream::Flush()
	{
		return true;
	}

	ulonglong FileStream::GetLength()
	{

		if (!IsOpen())
		{
			throw FileStreamException(FileStreamException::FSErrorCode::FSError_FileNotExist);
		}
		ulonglong oldPos = GetCurPos();
		Seek(0, FileEnd);
		m_fstream.seekg(0, FileEnd);
		ulonglong len = m_fstream.tellg();
		Seek(oldPos, FileBegin);
		return len;
	}

	bool FileStream::IsOpen()
	{
		return m_fstream.is_open();
	}

	bool FileStream::Remove()
	{
		if (IsOpen())
		{
			m_fstream.close();
		}
		if (FileIsExistA(m_filePath.c_str()))
			remove(m_filePath.c_str());
		else
			return false;
		
		return true;
	}

	bool FileStream::IsExist()
	{
		return FileIsExistA(m_filePath.c_str());
	}

	bool FileStream::Release()
	{
		if (IsOpen())
		{
			m_fstream.close();
		}
		m_filePath = "";

		return true;
	}

}