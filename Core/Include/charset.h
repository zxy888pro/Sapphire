#pragma once
#include "SapphireDef.h"
#include <iostream>
#include <string>

namespace Sapphire
{
	 std::string  UnicodeToAnsi(const std::wstring& unicode);
	 std::wstring AnsiToUnicode(const std::string& ansi);

	 std::string  AnsiToUtf8(const std::string& strSrc);
	 std::string  Utf8ToAnsi(const std::string& strSrc);

	 std::string  UnicodeToUtf8(const std::wstring& wstrSrc);
	 std::wstring Utf8ToUnicode(const std::string& strSrc);

	 std::string  GBKToUtf8(const std::string& gbk);
	 std::string  Utf8ToGBK(const std::string& utf8);

	 std::wstring GB2312ToUnicode(const std::string& gb2312);
	 std::string  UnicodeToGB2312(const std::wstring& unicode);

	 std::wstring BIG5ToUnicode(const std::string& big5);
	 std::string  UnicodeToBIG5(const std::wstring& unicode);

	 std::string  FBIG5ToGB2312(const std::string& big5);
	 std::string  GB2312ToFBIG5(const std::string gb2312);

	 bool IsUTF8(const void* pBuffer, long size);
}




