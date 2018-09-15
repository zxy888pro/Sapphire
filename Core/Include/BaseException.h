#ifndef __BASE_EXCEPTION_H__
#define __BASE_EXCEPTION_H__
#include "SapphireDef.h"

namespace Sapphire
{
	class SAPPHIRE_CLASS BaseException
	{
	public:
		BaseException();
		BaseException(std::string message);
		virtual ~BaseException();

		std::string& GetErrorMessage();

	protected:

		std::string m_message;
	};

	class SAPPHIRE_CLASS FileStreamException :public BaseException
	{
	public:
		enum FSErrorCode
		{
			FSError_FileNotExist,
			FSErrorCode_OpenFileError,
			FSErrorCode_OpenModeError,
			FSError_UnknownError

		};

		FileStreamException();
		FileStreamException(FSErrorCode ecode);
		FileStreamException(std::string msg);
		FileStreamException(std::string msg, FSErrorCode ecode);

		FSErrorCode GetErrorCode();
	private:
		FSErrorCode m_errorCode;
	};

	class SAPPHIRE_CLASS StringException :public BaseException
	{
	public:
		enum SErrorCode
		{
			SError_InvalidIndex,
			SError_OutOfRange,
			SError_CodeError,
			SError_UnknownError

		};

		StringException();
		StringException(SErrorCode ecode);
		StringException(std::string msg);
		StringException(std::string msg, SErrorCode ecode);

		SErrorCode GetErrorCode();
	private:
		SErrorCode m_errorCode;
	};


	
}




#endif // !__BASE_EXCEPTION_H__
