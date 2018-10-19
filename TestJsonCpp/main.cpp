#include <json/json.h>
#include <iostream>


using namespace std;

int main()
{
	const char* str = "{\"uploadid\": \"UP000000\",\"code\": 100,\"msg\": \"\",\"files\": \"\"}";
	Json::CharReaderBuilder b;
	Json::CharReader* reader = b.newCharReader();
	Json::Value root;
	JSONCPP_STRING errs;
	if (reader->parse(str, str + strlen(str), &root, &errs));
	{
		string uploadId = root["uploadid"].asString();
		cout << uploadId << endl;
	}
	getchar();
	return 0;
}