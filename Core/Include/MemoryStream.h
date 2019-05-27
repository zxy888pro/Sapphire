#pragma once

#include "SapphireDef.h"
#include <IStream.h>

namespace Sapphire
{
	class SAPPHIRE_CLASS MemoryStream : public IStream
	{
	public:

		MemoryStream();
		
		MemoryStream(ulong bufferSize = 1024);
		
		MemoryStream(void* pData, ulong size, ulong bufferSize = 1024);
		
		virtual ~MemoryStream();

		virtual bool Open(void* pData, ulong size);

		virtual bool Open(Path filePath, uint mode);

		virtual ulonglong Read(void* buffer, ulong toRead);

		virtual bool  Write(const void* buffer, ulong toWrite);

		virtual bool Seek(ulonglong offset, int from);

		virtual bool SeekToBegin();

		virtual bool SeekToEnd();

		virtual ulonglong GetCurPos();

		virtual bool Flush();

		virtual ulonglong  GetLength();

		virtual bool Release();

		virtual std::string ReadString(int nCharCount) override;

		virtual bool ReadLine(std::string& str) override;

		virtual bool IsOpen() override;

	private:


	};
}