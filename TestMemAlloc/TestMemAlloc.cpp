// TestMemAlloc.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <ctype.h>        //   Memory Manager.  This example is for illustration, these
#include <string>
#include <string.h>       //   headers do not redefine the new/delete operators, thus 
#include <time.h>         //   the new_off and new_on headers are not required.
#include <vector>
#include <map>
//#include "new_off.h"
//#include "new_on.h"

#include "MemoryMgr.h"	

class testClass
{
public:
	std::string m_name;
	std::map<int, std::string> m_map;

	std::string getName(){ return m_name; }
	std::string getValue(int key)
	{
		std::map<int, std::string>::iterator it = m_map.find(key);
		if (it != m_map.end())
		{
			return it->second;
		}

	}
};


int _tmain(int argc, _TCHAR* argv[])
{
	/*std::string str = "abc";
	std::string* pstr = new std::string("test");
	int size1 = sizeof(std::string);
	std::vector<std::string>* pv = new std::vector<std::string>();
	int size2 = sizeof(*pv);
	pv->push_back(str);
	delete pv;
	delete pstr;
	testClass* ptc = new testClass();
	ptc->m_name = "wah";*/
	testClass* ptc = new testClass();
	ptc->m_name = "wah";
	delete ptc;
	/*int* b = new int(5);
	delete b;*/
	dumpLogReport();
	dumpMemoryAllocations();
	
	return 0;
}

