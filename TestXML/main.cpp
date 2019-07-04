#include "Sapphire.h"
#include "Variant.h"

void TestVariant()
{
	using namespace Sapphire;
	std::string testStr = "abcd123";
	int len = testStr.length();
	std::vector<byte> data;
	data.resize(len);
	memcpy(&data[0], testStr.data(), len);
	Variant v = data;
	std::vector<byte> data2 = v.GetBuffer();
	return;

}

int main() {
  
	TestVariant();
	return 0;
}