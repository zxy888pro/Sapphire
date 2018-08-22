#include <GraphicException.h>

namespace Sapphire
{

	GraphicDriverException::GraphicDriverException()
	{
		m_message = "";
		m_errorCode = GDError_UnknownError;
	}

	GraphicDriverException::GraphicDriverException(GraphicDriverErrorCode ecode)
	{
		m_message = "";
		m_errorCode = ecode;
	}

	GraphicDriverException::GraphicDriverException(std::string msg)
	{
		m_errorCode = GDError_UnknownError;
		m_message = msg;
	}

	GraphicDriverException::GraphicDriverException(std::string msg, GraphicDriverErrorCode ecode)
	{
		m_errorCode = ecode;
		m_message = msg;
	}

	GraphicDriverException::GraphicDriverErrorCode GraphicDriverException::GetErrorCode()
	{
		return m_errorCode;
	}

}