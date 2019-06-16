#include <json/json.h>
#include <iostream>
#include "Sapphire.h"
#include "AsynTaskPool.h"
#include "Thread.h"
#include "FileStream.h"
#include "RingQueue.h"
#include "Variant.h"
#include "Math/Matrix4x4.h"
//#define jimi_b2(x)              (        (x) | (        (x) >>  1))
//#define jimi_b4(x)              ( jimi_b2(x) | ( jimi_b2(x) >>  2))
//#define jimi_b8(x)              ( jimi_b4(x) | ( jimi_b4(x) >>  4))
//#define jimi_b16(x)             ( jimi_b8(x) | ( jimi_b8(x) >>  8))
//#define jimi_b32(x)             (jimi_b16(x) | (jimi_b16(x) >> 16))
//#define jimi_b64(x)             (jimi_b32(x) | (jimi_b32(x) >> 32))

using namespace std;
 //unsigned int roundUpToNextPowerOfTwo(unsigned int x)
 //{
	//   x--;
	//   x |= x >> 1;  // handle  2 bit numbers
	//   x |= x >> 2;  // handle  4 bit numbers
	//   x |= x >> 4;  // handle  8 bit numbers
	//   x |= x >> 8;  // handle 16 bit numbers
	//   x |= x >> 16; // handle 32 bit numbers
	//   x++;
	//
	// return x;
	// }

class ThreadA : public Sapphire::Thread
{

public:

	virtual void ThreadFunc() override
	{
		int count = 100;
		while (count>0)
		{
			std::cout << "ThreadA Count:" << count << std::endl;
			--count;
			sapphire_sleep(100);
		}
		std::cout << "ThreadA over" << std::endl;
	}

};


class TaskA : public Sapphire::AsynTask
{

public:

	TaskA()
	{
		 
	}
	TaskA(const char* name) :AsynTask(name)
	{
		 
	}

	virtual void run() override
	{
		int count = 100;
		while (count > 0)
		{
			printf("%s count %d \n", taskName.c_str(), count);
			--count;
			sapphire_sleep(100);
		}
		printf("%s is over \n", taskName.c_str());
		
	}
private:
	 

};

void TestFunc()
{
	using namespace Sapphire;
	
	String name1 = "adbklg";
	StringHash name1hash(name1);
	ResourceRef ref1(name1hash, name1);
	String name2 = "mkalsl";
	StringHash name2hash(name1);
	ResourceRef ref2(name1hash, name1);
	String name3 = "jvksdf";
	StringHash name3hash(name1);
	ResourceRef ref3(name1hash, name1);
	StringVector sv;
	sv.push_back(name2);
	sv.push_back(name3);
	ResourceRefList list1(name1hash, sv);
	Variant v = list1;
	ResourceRefList list2 = v.GetResourceRefList();
	int _size = list2.names_.size();
	bool ret = list1 == list2;
	Variant v2 = v;
	list2 = v2.GetResourceRefList();
	v2.Clear();
	return;
}

Sapphire::RingQueue<float> queue;
int main()
{
	/*int t = jimi_b2(64);
	t = jimi_b32(64);
	t = roundUpToNextPowerOfTwo(128);*/
	
	
	using namespace Sapphire;
	
	TestFunc();

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

	/*ThreadA* pTask1 = new ThreadA();
	pTask1->Run();
	WaitForSingleObject(pTask1->GetHandle(), INFINITE);
	safeDelete(pTask1);
	std::cout << "end" << std::endl;*/
	Core core;
	AsynTaskPool pool(&core,4,2,100);
	pool.Initialize();
	TaskA* ta1 = new TaskA("task1");
	TaskA* ta2 = new TaskA("task2");
	TaskA* ta3 = new TaskA("task3");
	pool.AddTask(ta1);
	pool.AddTask(ta2);
	pool.AddTask(ta3);
	getchar();
	return 0;
}