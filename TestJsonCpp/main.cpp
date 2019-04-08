#include <json/json.h>
#include <iostream>
#include "Sapphire.h"
#include "FileStream.h"


using namespace std;

int main()
{
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