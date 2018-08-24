#pragma once
#include <Graphics.h>



namespace Sapphire
{
	class SAPPHIRE_CLASS GraphicDriverException :public BaseException
	{
	public:
		enum GraphicDriverErrorCode
		{
			GDError_NullPointerError,
			GDError_ComponentNotCreate,
			GDError_TextureOperationError,
			GDError_UnknownError,
		};

		GraphicDriverException();
		GraphicDriverException(GraphicDriverErrorCode ecode);
		GraphicDriverException(std::string msg);
		GraphicDriverException(std::string msg, GraphicDriverErrorCode ecode);

		GraphicDriverErrorCode GetErrorCode();
	private:
		GraphicDriverErrorCode m_errorCode;
	};
}
