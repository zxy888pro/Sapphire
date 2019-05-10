#include <json/json.h>
#include <iostream>
#include "Sapphire.h"
#include "FileStream.h"

#define jimi_b2(x)              (        (x) | (        (x) >>  1))
#define jimi_b4(x)              ( jimi_b2(x) | ( jimi_b2(x) >>  2))
#define jimi_b8(x)              ( jimi_b4(x) | ( jimi_b4(x) >>  4))
#define jimi_b16(x)             ( jimi_b8(x) | ( jimi_b8(x) >>  8))
#define jimi_b32(x)             (jimi_b16(x) | (jimi_b16(x) >> 16))
#define jimi_b64(x)             (jimi_b32(x) | (jimi_b32(x) >> 32))

using namespace std;
 unsigned int roundUpToNextPowerOfTwo(unsigned int x)
 {
	   x--;
	   x |= x >> 1;  // handle  2 bit numbers
	   x |= x >> 2;  // handle  4 bit numbers
	   x |= x >> 4;  // handle  8 bit numbers
	   x |= x >> 8;  // handle 16 bit numbers
	   x |= x >> 16; // handle 32 bit numbers
	   x++;
	
	 return x;
	 }
int main()
{
	int t = jimi_b2(64);
	t = jimi_b32(64);
	t = roundUpToNextPowerOfTwo(128);
	

	using namespace Sapphire;
	const char* str = "{\"uploadid\": \"UP000000\",\"code\": 100,\"msg\": \"\",\"files\": \"\"}";
		FileStream fs("images\\1.json", FileMode::FILE_EXIST | FileMode::FILE_READ | FileMode::FILE_READ | FileMode::FILE_STRING);
	if (fs.IsOpen())
	{
		std::string jsonStr = fs.ReadString(MAX_JSON_LENGTH);
		fs.Release();
		Json::CharReaderBuilder b;
		Json::CharReader* reader = b.newCharReader();
		Json::Value root;
		JSONCPP_STRING errs;
		if (reader->parse(jsonStr.c_str(), jsonStr.c_str() + strlen(jsonStr.c_str()), &root, &errs));
		{
			/*string uploadId = root["uploadid"].asString();
			cout << uploadId << endl;*/
			Json::Value ObjectArray; //¶ÁÈ¡Êý×é
			ObjectArray = root["cubes"];
			for (int i = 0; i < ObjectArray.size(); i++)
			{

				string right = ObjectArray[i]["+x"].asString();
				int nSize = ObjectArray[i]["size"].asInt();
				cout << "nSize = " << nSize << endl;
			}
			
		}
	}
	
	getchar();
	return 0;
}