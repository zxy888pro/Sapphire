#pragma once

#include <SapphireDef.h>
#include <path.h>

namespace Sapphire
{
	enum ESTREAM_SEEK
	{
		ESTREAM_SEEK_BEGIN,
		ESTREAM_SEEK_END,
		ESTREAM_SEEK_CUR,
		ESTREAM_SEEK_SET
	};

	struct IStream
	{
		virtual bool Open(void* pData, ulong size) = 0;

		virtual bool Open(Path filePath, uint mode) = 0;

		virtual bool IsOpen() = 0;

		virtual ulonglong Read(void* buffer, ulong toRead) = 0;

		virtual bool  Write(const void* buffer, ulong toWrite) = 0;

		virtual bool Seek(ulonglong offset, int from) = 0;

		virtual bool SeekToBegin() = 0;

		virtual bool SeekToEnd() = 0;

		virtual ulonglong GetCurPos() = 0;

		virtual bool Flush() = 0;

		virtual ulonglong  GetLength() = 0;

		virtual std::string ReadString(int nCharCount) = 0;

		virtual bool ReadLine(std::string& str) = 0;

		virtual bool Release() = 0;
	};
}