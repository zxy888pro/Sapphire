#pragma once
#include "SapphireDef.h"

namespace Sapphire
{

#define FORMAT_MSG_BUFFER_SIZE (204800)

	int ReplaceA(std::string& source, const std::string& replaceThis, const std::string& replaceWith);

	int ReplaceW(std::wstring& source, const std::wstring& replaceThis, const std::wstring& replaceWith);

	std::string genTimeStr(std::string format);

	std::string StringFormatA(const char *fmt, ...);

	std::wstring StringFormatW(wchar_t *fmt, ...);


}