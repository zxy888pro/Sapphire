#include "baseException.h"

Sapphire::BaseException::BaseException()
{

}

Sapphire::BaseException::~BaseException()
{

}

std::string& Sapphire::BaseException::GetErrorMessage()
{
	return m_message;
}

Sapphire::BaseException::BaseException(std::string message)
{
	m_message = message;
}

Sapphire::FileStreamException::FileStreamException()
{
	
	m_message = "";
	m_errorCode = FSError_UnknownError;
}

Sapphire::FileStreamException::FileStreamException(FSErrorCode ecode)
{
	m_message = "";
	m_errorCode = ecode;
}

Sapphire::FileStreamException::FileStreamException(std::string msg, FSErrorCode ecode) :BaseException(msg)
{
	m_errorCode = ecode;

}

Sapphire::FileStreamException::FileStreamException(std::string msg) :BaseException(msg)
{
	m_errorCode = FSError_UnknownError;
}

Sapphire::FileStreamException::FSErrorCode Sapphire::FileStreamException::GetErrorCode()
{
	return m_errorCode;
}
