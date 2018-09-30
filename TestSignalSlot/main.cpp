#include <iostream>
#include <Sapphire.h>
#include <sigslot/sigslot.h>
#include <RefCounter.h>
#include <Ptr.h>
#include <queue>

#define ENUM2STR(val) #val

using namespace Sapphire;

class TestC :public RefCounter
{
public:
	std::string name;
	int id;

	struct Compare
	{
		bool operator()(const TestC* lhs, const TestC* rhs) const
		{
			return lhs->id > rhs->id;
		}
	};

	static bool ComparePtr(const TestC* lhs, const TestC* rhs)
	{
		return lhs->id > rhs->id;
	}
};



class SignalSrc  
{
public:

	 

	sigslot::signal2<int, std::string>  testSignal;


};

class SignalHandler : public sigslot::has_slots<>
{
public:

	void OnTestVoidFunc()
	{
		std::cout << "OnTestVoidFunc" << std::endl;
	}
	
	void OnTestSignal(int id, std::string msg)
	{
		std::cout << "id= " << id << "  msg=" << msg << std::endl;
	}
	static void OnTestFunc(){};
};

enum EType
{
	ET_ONE,
	ET_TWO,
	ET_END
};

template<class declclass>
void Test(declclass*  t, void(declclass::*pfun)())
{
	(t->*pfun)();
}

int main()
{
	SharedPtr<TestC> ptr1 = SharedPtr<TestC>(new TestC());
	ptr1->id = 1;
	ptr1->name = "lily";
	SharedPtr<TestC> ptr2 = SharedPtr<TestC>(new TestC());
	ptr2->id = 2;
	ptr2->name = "jopk";
	SharedPtr<TestC> ptr3 = SharedPtr<TestC>(new TestC());
	ptr3->id = 3;
	ptr3->name = "bns";
	SharedPtr<TestC> ptr4 = SharedPtr<TestC>(new TestC());
	ptr4->id = 4;
	ptr4->name = "upbo";

	SignalHandler handler2;
	Test<SignalHandler>(&handler2, &SignalHandler::OnTestVoidFunc);
	
	std::priority_queue<SharedPtr<TestC>, std::vector<SharedPtr<TestC>>, TestC::Compare> queue;
	queue.push(ptr1);
	queue.push(ptr3);
	queue.push(ptr4);
	queue.push(ptr2);
	while (!queue.empty())
	{
		TestC* ptr = queue.top();
		std::cout << ptr->id << ptr->name << std::endl;
		queue.pop();
	}

	std::vector<SharedPtr<TestC>> list;
	list.push_back(ptr2);
	list.push_back(ptr3);
	list.push_back(ptr1);
	std::sort(list.begin(), list.end(), TestC::ComparePtr);
	SharedPtr<TestC> tptr = SharedPtr<TestC>();
	SharedPtr<TestC> tptr2;
	SharedPtr<TestC> tptr3;
	tptr = tptr2;
	if (tptr.Null())
	{
		std::cout << "tptr is NULL" << std::endl;
	}
	tptr = ptr1;
	WeakPtr<TestC> wptr = tptr;
	{
		std::vector<SharedPtr<TestC>>::iterator  it = std::find(list.begin(), list.end(), wptr);
		if (it != list.end())
		{
			std::cout << "found it ! " << ptr1->name << std::endl;
			list.erase(it);
		}
	}


	SignalSrc src1;
	SignalHandler handler1;
	src1.testSignal.connect(&handler1, &SignalHandler::OnTestSignal);
	std::string str = ENUM2STR(ET_ONE);
	src1.testSignal(1, str);
	
	typedef void(SignalHandler::*funcPtr)(int id, std::string msg);
	funcPtr f = &SignalHandler::OnTestSignal;
	(handler1.*f)(1,"sss");
	ulong funcAddr = reinterpret_cast<ulong>(&SignalHandler::OnTestFunc);
	getchar();

	return 0;
}