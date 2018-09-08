#pragma once

#include "SapphireDef.h"

namespace Sapphire
{
	class SAPPHIRE_CLASS MemoryStream
	{
	public:

		MemoryStream();
		
		MemoryStream(ulong bufferSize = 1024);
		
		MemoryStream(void* pData, ulong size, ulong bufferSize = 1024);
		
		virtual ~MemoryStream();

		virtual bool Open(void* pData, ulong size);

		virtual ulonglong Read(void* buffer, ulong toRead);

		virtual bool  Write(const void* buffer, ulong toWrite);

		virtual bool Seek(ulonglong offset, int from);

		virtual bool SeekToBegin();

		virtual bool SeekToEnd();

		virtual ulonglong GetCurPos();

		virtual bool Flush();

		virtual ulonglong  GetLength();

		virtual bool Release();

	private:


	};
}