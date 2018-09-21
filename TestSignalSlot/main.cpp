#include <iostream>
#include <Sapphire.h>
#include <sigslot/sigslot.h>

#define ENUM2STR(val) #val

using namespace Sapphire;

class SignalSrc  
{
public:

	 

	sigslot::signal2<int, std::string>  testSignal;


};

class SignalHandler : public sigslot::has_slots<>
{
public:

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

int main()
{
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