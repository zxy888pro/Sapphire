#include <Str.h>
#include "stringHelper.h"
#include <time.h>
#include <stdarg.h>
#include <Math/Vector2.h>
#include <Math/Vector3.h>
#include <Math/Vector4.h>
#include <Math/Matrix3x3.h>
#include <Math/Matrix3x4.h>
#include <Math/Matrix4x4.h>
#include <Math/Quaternion.h>
#include <Math/Rect.h>


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



	uint CStringLength(const char* str)
	{
		if (!str)
			return 0;
#ifdef _MSC_VER
		return (unsigned)strlen(str);
#else
		const char* ptr = str;
		while (*ptr)
			++ptr;
		return (unsigned)(ptr - str);
#endif
	}

	unsigned CountElements(const char* buffer, char separator)
	{
		if (!buffer)
			return 0;

		const char* endPos = buffer + CStringLength(buffer);
		const char* pos = buffer;
		unsigned ret = 0;

		while (pos < endPos)
		{
			if (*pos != separator)
				break;
			++pos;
		}

		while (pos < endPos)
		{
			const char* start = pos;

			while (start < endPos)
			{
				if (*start == separator)
					break;

				++start;
			}

			if (start == endPos)
			{
				++ret;
				break;
			}

			const char* end = start;

			while (end < endPos)
			{
				if (*end != separator)
					break;

				++end;
			}

			++ret;
			pos = end;
		}

		return ret;
	}

	unsigned GetStringListIndex(const String& value, const String* strings, unsigned defaultIndex, bool caseSensitive)
	{
		return GetStringListIndex(value.c_str(), strings, defaultIndex, caseSensitive);
	}

	unsigned GetStringListIndex(const char* value, const String* strings, unsigned defaultIndex, bool caseSensitive)
	{
		unsigned i = 0;

		while (!strings[i].empty())
		{
			if (!strings[i].Compare(value, caseSensitive))
				return i;
			++i;
		}

		return defaultIndex;
	}

	unsigned GetStringListIndex(const char* value, const char** strings, unsigned defaultIndex, bool caseSensitive)
	{
		unsigned i = 0;

		while (strings[i])
		{
			if (!String::Compare(value, strings[i], caseSensitive))
				return i;
			++i;
		}

		return defaultIndex;
	}

	bool ToBool(const String& source)
	{
		return ToBool(source.c_str());
	}

	bool ToBool(const char* source)
	{
		unsigned length = CStringLength(source);

		for (unsigned i = 0; i < length; ++i)
		{
			char c = (char)tolower(source[i]);
			if (c == 't' || c == 'y' || c == '1')
				return true;
			else if (c != ' ' && c != '\t')
				break;
		}

		return false;
	}

	int ToInt(const String& source)
	{
		return ToInt(source.c_str());
	}

	int ToInt(const char* source)
	{
		if (!source)
			return 0;

		// Explicitly ask for base 10 to prevent source starts with '0' or '0x' from being converted to base 8 or base 16, respectively
		return (int)strtol(source, 0, 10);
	}

	unsigned ToUInt(const String& source)
	{
		return ToUInt(source.c_str());
	}

	unsigned ToUInt(const char* source)
	{
		if (!source)
			return 0;

		return (unsigned)strtoul(source, 0, 10);
	}

	float ToFloat(const String& source)
	{
		return ToFloat(source.c_str());
	}

	float ToFloat(const char* source)
	{
		if (!source)
			return 0;

		return (float)strtod(source, 0);
	}

	double ToDouble(const String& source)
	{
		return ToDouble(source.c_str());
	}

	double ToDouble(const char* source)
	{
		if (!source)
			return 0;

		return strtod(source, 0);
	}

	Color ToColor(const String& source)
	{
		return ToColor(source.c_str());
	}

	Color ToColor(const char* source)
	{
		Color ret;

		unsigned elements = CountElements(source, ' ');
		if (elements < 3)
			return ret;

		char* ptr = (char*)source;
		ret.r_ = (float)strtod(ptr, &ptr);
		ret.g_ = (float)strtod(ptr, &ptr);
		ret.b_ = (float)strtod(ptr, &ptr);
		if (elements > 3)
			ret.a_ = (float)strtod(ptr, &ptr);

		return ret;
	}


	Quaternion ToQuaternion(const String& source)
	{
		return ToQuaternion(source.c_str());
	}

	Quaternion ToQuaternion(const char* source)
	{
		unsigned elements = CountElements(source, ' ');
		char* ptr = (char*)source;

		if (elements < 3)
			return Quaternion::IDENTITY;
		else if (elements < 4)
		{
			// 3 coords specified: conversion from Euler angles
			float x, y, z;
			x = (float)strtod(ptr, &ptr);
			y = (float)strtod(ptr, &ptr);
			z = (float)strtod(ptr, &ptr);

			return Quaternion(x, y, z);
		}
		else
		{
			// 4 coords specified: full quaternion
			Quaternion ret;
			ret.w_ = (float)strtod(ptr, &ptr);
			ret.x_ = (float)strtod(ptr, &ptr);
			ret.y_ = (float)strtod(ptr, &ptr);
			ret.z_ = (float)strtod(ptr, &ptr);

			return ret;
		}
	}

	Vector2 ToVector2(const String& source)
	{
		return ToVector2(source.c_str());
	}

	Vector2 ToVector2(const char* source)
	{
		Vector2 ret(Vector2::ZERO);

		unsigned elements = CountElements(source, ' ');
		if (elements < 2)
			return ret;

		char* ptr = (char*)source;
		ret.x_ = (float)strtod(ptr, &ptr);
		ret.y_ = (float)strtod(ptr, &ptr);

		return ret;
	}

	Vector3 ToVector3(const String& source)
	{
		return ToVector3(source.c_str());
	}

	Vector3 ToVector3(const char* source)
	{
		Vector3 ret(Vector3::ZERO);

		unsigned elements = CountElements(source, ' ');
		if (elements < 3)
			return ret;

		char* ptr = (char*)source;
		ret.x_ = (float)strtod(ptr, &ptr);
		ret.y_ = (float)strtod(ptr, &ptr);
		ret.z_ = (float)strtod(ptr, &ptr);

		return ret;
	}

	Rect ToRect(const String& source)
	{
		return ToRect(source.c_str());
	}

	Rect ToRect(const char* source)
	{
		Rect ret(Rect::ZERO);

		unsigned elements = CountElements(source, ' ');
		if (elements < 4)
			return ret;

		char* ptr = (char*)source;
		ret.min_.x_ = (float)strtod(ptr, &ptr);
		ret.min_.y_ = (float)strtod(ptr, &ptr);
		ret.max_.x_ = (float)strtod(ptr, &ptr);
		ret.max_.y_ = (float)strtod(ptr, &ptr);

		return ret;
	}

	IntRect ToIntRect(const String& source)
	{
		return ToIntRect(source.c_str());
	}

	IntRect ToIntRect(const char* source)
	{
		IntRect ret(IntRect::ZERO);

		unsigned elements = CountElements(source, ' ');
		if (elements < 4)
			return ret;

		char* ptr = (char*)source;
		ret.left_ = (int)strtol(ptr, &ptr, 10);
		ret.top_ = (int)strtol(ptr, &ptr, 10);
		ret.right_ = (int)strtol(ptr, &ptr, 10);
		ret.bottom_ = (int)strtol(ptr, &ptr, 10);

		return ret;
	}

	Vector4 ToVector4(const String& source, bool allowMissingCoords)
	{
		return ToVector4(source.c_str(), allowMissingCoords);
	}

	Vector4 ToVector4(const char* source, bool allowMissingCoords)
	{
		Vector4 ret(Vector4::ZERO);

		unsigned elements = CountElements(source, ' ');
		char* ptr = (char*)source;

		if (!allowMissingCoords)
		{
			if (elements < 4)
				return ret;

			ret.x_ = (float)strtod(ptr, &ptr);
			ret.y_ = (float)strtod(ptr, &ptr);
			ret.z_ = (float)strtod(ptr, &ptr);
			ret.w_ = (float)strtod(ptr, &ptr);

			return ret;
		}
		else
		{
			if (elements > 0)
				ret.x_ = (float)strtod(ptr, &ptr);
			if (elements > 1)
				ret.y_ = (float)strtod(ptr, &ptr);
			if (elements > 2)
				ret.z_ = (float)strtod(ptr, &ptr);
			if (elements > 3)
				ret.w_ = (float)strtod(ptr, &ptr);

			return ret;
		}
	}


	Matrix3x3 ToMatrix3x3(const String& source)
	{
		return ToMatrix3x3(source.c_str());
	}

	Matrix3x3 ToMatrix3x3(const char* source)
	{
		Matrix3x3 ret(Matrix3x3::ZERO);

		unsigned elements = CountElements(source, ' ');
		if (elements < 9)
			return ret;

		char* ptr = (char*)source;
		ret.m00_ = (float)strtod(ptr, &ptr);
		ret.m01_ = (float)strtod(ptr, &ptr);
		ret.m02_ = (float)strtod(ptr, &ptr);
		ret.m10_ = (float)strtod(ptr, &ptr);
		ret.m11_ = (float)strtod(ptr, &ptr);
		ret.m12_ = (float)strtod(ptr, &ptr);
		ret.m20_ = (float)strtod(ptr, &ptr);
		ret.m21_ = (float)strtod(ptr, &ptr);
		ret.m22_ = (float)strtod(ptr, &ptr);

		return ret;
	}

	Matrix3x4 ToMatrix3x4(const String& source)
	{
		return ToMatrix3x4(source.c_str());
	}

	Matrix3x4 ToMatrix3x4(const char* source)
	{
		Matrix3x4 ret(Matrix3x4::ZERO);

		unsigned elements = CountElements(source, ' ');
		if (elements < 12)
			return ret;

		char* ptr = (char*)source;
		ret.m00_ = (float)strtod(ptr, &ptr);
		ret.m01_ = (float)strtod(ptr, &ptr);
		ret.m02_ = (float)strtod(ptr, &ptr);
		ret.m03_ = (float)strtod(ptr, &ptr);
		ret.m10_ = (float)strtod(ptr, &ptr);
		ret.m11_ = (float)strtod(ptr, &ptr);
		ret.m12_ = (float)strtod(ptr, &ptr);
		ret.m13_ = (float)strtod(ptr, &ptr);
		ret.m20_ = (float)strtod(ptr, &ptr);
		ret.m21_ = (float)strtod(ptr, &ptr);
		ret.m22_ = (float)strtod(ptr, &ptr);
		ret.m23_ = (float)strtod(ptr, &ptr);

		return ret;
	}

	Matrix4x4 ToMatrix4x4(const String& source)
	{
		return ToMatrix4x4(source.c_str());
	}

	Matrix4x4 ToMatrix4x4(const char* source)
	{
		Matrix4x4 ret(Matrix4x4::ZERO);

		unsigned elements = CountElements(source, ' ');
		if (elements < 16)
			return ret;

		char* ptr = (char*)source;
		ret.m00_ = (float)strtod(ptr, &ptr);
		ret.m01_ = (float)strtod(ptr, &ptr);
		ret.m02_ = (float)strtod(ptr, &ptr);
		ret.m03_ = (float)strtod(ptr, &ptr);
		ret.m10_ = (float)strtod(ptr, &ptr);
		ret.m11_ = (float)strtod(ptr, &ptr);
		ret.m12_ = (float)strtod(ptr, &ptr);
		ret.m13_ = (float)strtod(ptr, &ptr);
		ret.m20_ = (float)strtod(ptr, &ptr);
		ret.m21_ = (float)strtod(ptr, &ptr);
		ret.m22_ = (float)strtod(ptr, &ptr);
		ret.m23_ = (float)strtod(ptr, &ptr);
		ret.m30_ = (float)strtod(ptr, &ptr);
		ret.m31_ = (float)strtod(ptr, &ptr);
		ret.m32_ = (float)strtod(ptr, &ptr);
		ret.m33_ = (float)strtod(ptr, &ptr);

		return ret;
	}

	IntVector2 ToIntVector2(const String& source)
	{
		return ToIntVector2(source.c_str());
	}

	IntVector2 ToIntVector2(const char* source)
	{
		IntVector2 ret(IntVector2::ZERO);

		unsigned elements = CountElements(source, ' ');
		if (elements < 2)
			return ret;

		char* ptr = (char*)source;
		ret.x_ = (int)strtol(ptr, &ptr, 10);
		ret.y_ = (int)strtol(ptr, &ptr, 10);

		return ret;
	}

	String ToString(void* value)
	{
		return ToStringHex((unsigned)(size_t)value);
	}

	String ToStringHex(unsigned value)
	{
		char tempBuffer[STRING_BUFFERSIZE];
		sprintf(tempBuffer, "%08x", value);
		return String(tempBuffer);
	}

	
}