#ifndef __FILE_STREAM__
#define __FILE_STREAM__

#include "SapphireDef.h"
#include "path.h"
#include <IStream.h>
#include <fstream>
#include <exception>

namespace Sapphire
{


	enum FileMode
	{
		FILE_READ = 0x1,
		FILE_WRITE = 0x2,
		FILE_CREATE = 0x4,
		FILE_EXIST = 0x8,
		FILE_APPEND = 0x10,
		FILE_STRING = 0x20
	};

	

	class SAPPHIRE_CLASS FileStream : public IStream
	{
	public:

		const static int FileBegin = 0;
		const static int FileEnd = 1;
		const static int FileCurPos = 2;

		FileStream(ulong bufferSize = 1024);
		FileStream(Path filePath, uint mode, ulong bufferSize = 1024);

		virtual ~FileStream();

		virtual bool Open(Path filePath, uint mode);

		virtual ulonglong Read(void* buffer, ulong toRead);

		virtual bool  Write(const void* buffer, ulong toWrite);

		virtual bool Seek(ulonglong offset, int from);

		virtual bool SeekToBegin();

		virtual bool SeekToEnd();

		virtual ulonglong GetCurPos();

		virtual bool Flush();

		virtual ulonglong  GetLength();

		virtual bool IsOpen();

		virtual bool Remove();

		virtual bool IsExist();

		virtual bool Release();

		virtual std::string ReadString(int nCharCount = 0); //如果不指定字符数，那么一直读取到文件最后一个字符

		virtual bool ReadLine(std::string& str);

		virtual bool Open(void* pData, ulong size) override;

		virtual const Path& GetPath() const;

	private:

		//byte* m_buffer = NULL;
		std::fstream m_fstream;
		Path m_filePath;
		uint m_mode;

	};

}


#endif