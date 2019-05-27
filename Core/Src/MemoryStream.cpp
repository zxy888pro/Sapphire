#include "Sapphire.h"
#include "MemoryStream.h"


namespace Sapphire
{

	MemoryStream::MemoryStream()
	{

	}

	MemoryStream::MemoryStream(ulong bufferSize /*= 1024*/)
	{

	}

	MemoryStream::MemoryStream(void* pData, ulong size, ulong bufferSize /*= 1024*/)
	{

	}

	MemoryStream::~MemoryStream()
	{

	}

	bool MemoryStream::Open(void* pData, ulong size)
	{
		return false;
	}

	bool MemoryStream::Open(Path filePath, uint mode)
	{
		return false;
	}

	ulonglong MemoryStream::Read(void* buffer, ulong toRead)
	{
		return 0;
	}

	bool MemoryStream::Write(const void* buffer, ulong toWrite)
	{
		return false;
	}

	bool MemoryStream::Seek(ulonglong offset, int from)
	{
		return false;
	}

	bool MemoryStream::SeekToBegin()
	{
		return false;
	}

	bool MemoryStream::SeekToEnd()
	{
		return false;
	}

	ulonglong MemoryStream::GetCurPos()
	{
		return 0;
	}

	bool MemoryStream::Flush()
	{
		return false;
	}

	ulonglong MemoryStream::GetLength()
	{
		return 0;
	}

	bool MemoryStream::Release()
	{
		return false;
	}

	std::string MemoryStream::ReadString(int nCharCount)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	bool MemoryStream::ReadLine(std::string& str)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	bool MemoryStream::IsOpen()
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

}