#include <iostream>
#include "Sapphire.h"
#include "Str.h"
#include "stringHelper.h"
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
	Variant v2;
	v2.SetBuffer(testStr.data(), len);
	bool ret = v == v2;
	VariantMap vmap;
	vmap["v1"] = v;
	vmap["v2"] = v2;

	return;

}


void TestString()
{
	using namespace Sapphire;
	String str = "   °®µÄsakkABJ J  ";
	String str_small = str.ToUpper();
	String str_trim = str.Trimmed();
	int ret = str.ReplaceSubString("°®µÄ", "bosmo");
	ret = str.Find("bosmo", str.Length(),true);
	return;

}

int main() {
  
	TestVariant();
	TestString();
	return 0;
}