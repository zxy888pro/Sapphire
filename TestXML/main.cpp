#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
 
using namespace std;

int main() {
 
	unordered_map<string, string>* pMap = new unordered_map<string, string>();
	pair<string,string> pair = make_pair("jack", "smith");
	pMap->insert(pair);
	string name = (*pMap)["jack"];
	cout << name<< endl;
	delete pMap;
	getchar();

	return 0;
}