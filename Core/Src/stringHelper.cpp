#include "stringHelper.h"
#include <time.h>
#include <stdarg.h>

namespace Sapphire
{

	std::string StringFormatA(const char *fmt, ...)
	{
		char szBuffer[FORMAT_MSG_BUFFER_SIZE + 1] = { 0 };
		va_list args;
		va_start(args, fmt);
		vsnprintf(szBuffer, FORMAT_MSG_BUFFER_SIZE, fmt, args);
		va_end(args);
		std::string strRet = szBuffer;
		return strRet;
	}

	int ReplaceA(std::string& source, const std::string& replaceThis, const std::string& replaceWith)
	{
		unsigned nextPos = 0;
		int ret = 0;
		while (nextPos < source.length())
		{
			unsigned pos = source.find(replaceThis, nextPos);
			if (pos == std::string::npos)
				break;
			source.replace(pos, replaceThis.length(), replaceWith);
			nextPos = pos + replaceWith.length();
			ret++;
		}
		return ret;
	}

	int ReplaceW(std::wstring& source, const std::wstring& replaceThis, const std::wstring& replaceWith)
	{
		unsigned nextPos = 0;
		int ret = 0;
		while (nextPos < source.length())
		{
			unsigned pos = source.find(replaceThis, nextPos);
			if (pos == std::string::npos)
				break;
			source.replace(pos, replaceThis.length(), replaceWith);
			nextPos = pos + replaceWith.length();
			ret++;
		}
		return ret;
	}

	std::string genTimeStr(std::string format)
	{
		char buf[512] = { 0 };
		time_t tt = time(NULL);//这句返回的只是一个时间cuo
		tm* t = localtime(&tt);
		if (format == "")
		{
			format = "%d%02d%02d%02d%02d%02d";
		}
		sprintf(buf, format.c_str(),
			t->tm_year + 1900,
			t->tm_mon + 1,
			t->tm_mday,
			t->tm_hour,
			t->tm_min,
			t->tm_sec);

		return std::string(buf);
	}

	std::wstring StringFormatW(wchar_t *fmt, ...)
	{
		wchar_t szBuffer[FORMAT_MSG_BUFFER_SIZE + 1] = { 0 };
		va_list args;
		va_start(args, fmt);
		vswprintf(szBuffer, FORMAT_MSG_BUFFER_SIZE, fmt, args);
		va_end(args);
		std::wstring strRet = szBuffer;
		return strRet;
	}

}