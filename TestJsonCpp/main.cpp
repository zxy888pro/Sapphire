#include <json/json.h>
#include <iostream>
#include "Sapphire.h"
#include "RefCounter.h"
#include "Ptr.h"
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

class TestSharePtr : public Sapphire::RefCounter
{
public:
	TestSharePtr(const char* str)
	{
		testStr = str;
		name = str;
		cout << "Create TestSharePtr :" << str << endl;
	}
	~TestSharePtr()
	{
		cout << "Delete TestSharePtr :" << testStr << endl;
	}

	void TestFunc(TestSharePtr* other)
	{
		name = other->name;
		testStr = other->testStr;
	}
	void Test()
	{
		cout << name << endl;
	}
protected:
	std::string name;
private:
	std::string testStr;
};

class TestClassA
{
public:
	TestClassA(const char* str)
	{
		name = str;
	}
	void TestFunc(TestClassA* pta)
	{
		name = pta->name;
	}
	
private:

	std::string name;
};

Sapphire::SharedPtr<TestSharePtr> TestFunc(Sapphire::SharedPtr<TestSharePtr> pTestShare)
{
	Sapphire::SharedPtr<TestSharePtr> tmp = pTestShare;
	return tmp;
}




Sapphire::RingQueue<float> queue;
int main()
{
	/*int t = jimi_b2(64);
	t = jimi_b32(64);
	t = roundUpToNextPowerOfTwo(128);*/
	TestClassA a1("waht");
	TestClassA a2("waht");
	a1.TestFunc(&a2);
	
	using namespace Sapphire;
	TestSharePtr* tp = new TestSharePtr("abc");
	TestSharePtr* ntp = new TestSharePtr("gku");
	tp->TestFunc(ntp);
	
	bool ret = false;
	WeakPtr<TestSharePtr> wp1;
	WeakPtr<TestSharePtr> wp2;
	wp1 = tp;
	wp1->Test();
	{
		SharedPtr<TestSharePtr> tp1 = TestFunc(SharedPtr<TestSharePtr>(tp));
		wp1 = tp1;
		int a = 0;
		TestSharePtr* pTsp = wp1.Get();
		ret = wp1.Expired();
		if (wp1 == tp1)
		{
			ret = true; 
		}
		if (tp1 == wp1)//���Ͳ�һ���Զ���������ת������
		{
			ret = true;
		}
		if (tp1 == tp)
		{
			ret = true;
		}
		if (wp1 == tp)
		{
			ret = true;
		}
		wp2 = tp;
	}
	ret = wp1.Expired();
	TestSharePtr* pTsp = wp1.Get();
	safeDelete(ntp)

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
			Json::Value ObjectArray; //��ȡ����
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