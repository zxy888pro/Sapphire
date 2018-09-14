#pragma once
#include "SapphireDef.h"
#include <Variant.h>
#include <Color.h>

namespace Sapphire
{

#define FORMAT_MSG_BUFFER_SIZE (204800)

	int ReplaceA(std::string& source, const std::string& replaceThis, const std::string& replaceWith);

	int ReplaceW(std::wstring& source, const std::wstring& replaceThis, const std::wstring& replaceWith);

	std::string genTimeStr(std::string format);

	std::string StringFormatA(const char *fmt, ...);

	std::wstring StringFormatW(wchar_t *fmt, ...);

	SAPPHIRE_API uint CStringLength(const char* str);

	SAPPHIRE_API unsigned CountElements(const char* buffer, char separator);

	/// Return an index to a string list corresponding to the given string, or a default value if not found. The string list must be empty-terminated.
	SAPPHIRE_API unsigned GetStringListIndex(const String& value, const String* strings, unsigned defaultIndex, bool caseSensitive = false);
	/// Return an index to a string list corresponding to the given C string, or a default value if not found. The string list must be empty-terminated.
	SAPPHIRE_API unsigned GetStringListIndex(const char* value, const String* strings, unsigned defaultIndex, bool caseSensitive = false);
	/// Return an index to a C string list corresponding to the given C string, or a default value if not found. The string list must be empty-terminated.
	SAPPHIRE_API unsigned GetStringListIndex(const char* value, const char** strings, unsigned defaultIndex, bool caseSensitive = false);

	/// Parse a bool from a string. Check for the first non-empty character (converted to lowercase) being either 't', 'y' or '1'.
	SAPPHIRE_API bool ToBool(const String& source);
	/// Parse a bool from a C string. Check for the first non-empty character (converted to lowercase) being either 't', 'y' or '1'.
	SAPPHIRE_API bool ToBool(const char* source);
	/// Parse a float from a string.
	SAPPHIRE_API float ToFloat(const String& source);
	/// Parse a float from a C string.
	SAPPHIRE_API float ToFloat(const char* source);
	/// Parse a double from a string.
	SAPPHIRE_API double ToDouble(const String& source);
	/// Parse a double from a C string.
	SAPPHIRE_API double ToDouble(const char* source);
	/// Parse an integer from a string.
	SAPPHIRE_API int ToInt(const String& source);
	/// Parse an integer from a C string.
	SAPPHIRE_API int ToInt(const char* source);
	/// Parse an unsigned integer from a string.
	SAPPHIRE_API unsigned ToUInt(const String& source);
	/// Parse an unsigned integer from a C string.
	SAPPHIRE_API unsigned ToUInt(const char* source);
	/// Parse a Color from a string.
	SAPPHIRE_API Color ToColor(const String& source);
	/// Parse a Color from a C string.
	SAPPHIRE_API Color ToColor(const char* source);
	/// Parse a Quaternion from a string. If only 3 components specified, convert Euler angles (degrees) to quaternion.
	SAPPHIRE_API Quaternion ToQuaternion(const String& source);
	/// Parse a Quaternion from a C string. If only 3 components specified, convert Euler angles (degrees) to quaternion.
	SAPPHIRE_API Quaternion ToQuaternion(const char* source);
	/// Parse a Vector2 from a string.
	SAPPHIRE_API Vector2 ToVector2(const String& source);
	/// Parse a Vector2 from a C string.
	SAPPHIRE_API Vector2 ToVector2(const char* source);
	/// Parse a Vector3 from a string.
	SAPPHIRE_API Vector3 ToVector3(const String& source);
	/// Parse a Vector3 from a C string.
	SAPPHIRE_API Vector3 ToVector3(const char* source);
	/// Parse a Vector4 from a string.
	SAPPHIRE_API Vector4 ToVector4(const String& source, bool allowMissingCoords = false);
	/// Parse a Vector4 from a C string.
	SAPPHIRE_API Vector4 ToVector4(const char* source, bool allowMissingCoords = false);
	/// Parse an IntRect from a string.
	SAPPHIRE_API IntRect ToIntRect(const String& source);
	/// Parse an IntRect from a C string.
	SAPPHIRE_API IntRect ToIntRect(const char* source);
	/// Parse an IntVector2 from a string.
	SAPPHIRE_API IntVector2 ToIntVector2(const String& source);
	/// Parse an IntVector2 from a C string.
	SAPPHIRE_API IntVector2 ToIntVector2(const char* source);
	/// Parse a Rect from a string.
	SAPPHIRE_API Rect ToRect(const String& source);
	/// Parse a Rect from a C string.
	SAPPHIRE_API Rect ToRect(const char* source);
	/// Parse a Matrix3 from a string.
	SAPPHIRE_API Matrix3x3 ToMatrix3x3(const String& source);
	/// Parse a Matrix3 from a C string.
	SAPPHIRE_API Matrix3x3 ToMatrix3x3(const char* source);
	/// Parse a Matrix3x4 from a string.
	SAPPHIRE_API Matrix3x4 ToMatrix3x4(const String& source);
	/// Parse a Matrix3x4 from a C string.
	SAPPHIRE_API Matrix3x4 ToMatrix3x4(const char* source);
	/// Parse a Matrix4 from a string.
	SAPPHIRE_API Matrix4x4 ToMatrix4x4(const String& source);
	/// Parse a Matrix4 from a C string.
	SAPPHIRE_API Matrix4x4 ToMatrix4x4(const char* source);
	/// Convert a pointer to string (returns hexadecimal.)
	SAPPHIRE_API String ToString(void* value);
	/// Convert an unsigned integer to string as hexadecimal.
	SAPPHIRE_API String ToStringHex(unsigned value);
	/// Convert a byte buffer to a string.
	SAPPHIRE_API void BufferToString(String& dest, const void* data, unsigned size);

}