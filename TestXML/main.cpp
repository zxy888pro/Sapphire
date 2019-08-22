#include "Sapphire.h"
#include "Core.h"
#include "GraphicDriver.h"
#include "ResourceLoader.h"
#include "ResourceCache.h"
#include "path.h"
#include <iostream>
#include "ImageRes.h"
#include "XML/XMLFile.h"

namespace Sapphire
{
	class TestRef : public RefCounter
	{
	public:
		int a = 0;
	};

	class TestMutexClass
	{
	public:

		TestMutexClass()
		{

		}

		void TestAdd(const char* name)
		{
			//ResGuard<MutexEx> reslock(m_mutex);
			std::unordered_map<std::string, int>::iterator it = m_testDict.find(name);
			if (it != m_testDict.end())
			{
				m_testDict[name] += 1;
			}
			else
			{
				m_testDict[name] = 1;
			}
			 
		}

		int Get(const char* name)
		{
			//ResGuard<MutexEx> reslock(m_mutex);
			std::unordered_map<std::string, int>::iterator it = m_testDict.find(name);
			if (it != m_testDict.end())
			{
				return it->second;
			}
			return 0;
			 
		}

		void Set(const char* name, int val)
		{
			m_testDict[name] = val;
		}

	private:
		MutexEx   m_mutex;
		std::unordered_map<std::string, int>  m_testDict;
	};
}

Sapphire::TestMutexClass tmc;

namespace Sapphire
{
	
	class Test1
	{
	public:

		int a = 0;
		bool c = true;
		float b = 0;
	};
	

	class TestThreadLock : public Thread
	{
	public:

		TestThreadLock()
		{

		}
		~TestThreadLock()
		{

		}



		virtual void ThreadFunc() override
		{
			ulong handlle = reinterpret_cast<uint>(GetHandle());
			String idStr(handlle);
			printf("thread Id %s  is Started \n", idStr.c_str());
			while (m_number)
			{
				
				
				int num = tmc.Get("number");
				num++;
				printf("thread Id %s   number is %d \n", idStr.c_str(), num);
				tmc.Set("number",num);
				Sleep(200);
				m_number--;
			}
			printf("thread Id %s  is Terminated \n", idStr.c_str());
		}

	private:

		int m_number = 10;

	};

	class App : public Thread
	{


	public:

		App()
		{

		}
		virtual ~App()
		{

		}

		void Initialize()
		{
			pCore = new Core();
			LogUtil::getInstancePtr()->Init("log.txt");
			LogUtil::LogMsgLn("初始化程序");
			pCore->Init();
			MathHelper::SetRandomSeed(GetTickCount());
			createGraphicDriver();

			resourceLoader = new ResourceLoader(pCore);
			asynTaskPool = new AsynTaskPool(pCore, 8, 2, 100);
			resourceCache = new ResourceCache(pCore);
			pCore->RegisterSubSystem(resourceLoader, ESubSystemType::ESST_RESOURCELOADER);
			pCore->RegisterSubSystem(asynTaskPool, ESubSystemType::ESST_ASYNTASKPOOL);
			pCore->RegisterSubSystem(resourceCache, ESubSystemType::ESST_RESOURCECACHE);
			resourceLoader->Initialize();
			asynTaskPool->Initialize();
		}
		virtual void ThreadFunc() override
		{
			XMLFile* xml1 = new XMLFile(pCore, "NinjaSnowWarShaders.xml");
			XMLFile* xml2 = new XMLFile(pCore, "NinjaSnowWar.xml");
			//ImageRes* img1 = new ImageRes(pCore, "container2.png");
			//resourceLoader->LoadResource(img1);
			resourceLoader->LoadResource(xml1);
			resourceLoader->LoadResource(xml2);
			while (!getExitFlag())
			{
				BaseResource* resource = resourceCache->GetResource("NinjaSnowWarShaders.xml");
				if (resource)
				{
					XMLFile* xmlFile = dynamic_cast<XMLFile*>(resource);
					XMLElement root = xmlFile->GetRoot();
					XMLElement element = root.GetChild();
					while (!element.IsNull())
					{
						String vs = element.GetAttribute("vs");
						String vsdefines = element.GetAttribute("vsdefines");
						SAPPHIRE_LOGERROR(StringFormatA("xmlElement name %s  vs= %s  vsdefine= %s ", element.GetName().c_str(), vs.c_str(), vsdefines.c_str()));
						element = element.GetNext();
					}
					
					SAPPHIRE_LOGERROR(StringFormatA("resource name %s   size %d", resource->GetName().c_str(), resource->GetSize()));

					break;
				}
				Sleep(50);
			}
		}



		bool getExitFlag() const { return bExitFlag; }
		void setExitFlag(bool val) { bExitFlag = val; }

		void Close()
		{

			resourceLoader->Release();
			asynTaskPool->Close();
			resourceCache->Clear();
			safeDelete(resourceLoader);
			safeDelete(asynTaskPool);
			safeDelete(resourceCache);
			pCore->Release();
		}

	private:
		Core* pCore;
		bool bExitFlag = false;
		ResourceLoader* resourceLoader;
		AsynTaskPool* asynTaskPool;
		ResourceCache* resourceCache;

	};
}


int main() {
	 
	//测试资源加载
	using namespace Sapphire;
	
	/*tmc = TestMutexClass();
	for (int i = 0; i < 4; ++i)
	{
	TestThreadLock* thread = new TestThreadLock();
	thread->Run();
	}*/
	std::string str1 = "tas";
	std::string cont1 = "";
	std::string cont2 = "-sdk";
	str1 += cont1 + cont2;
	Test1*  ta1[10];
	memset(ta1, 0, sizeof(ta1));
	const char* pszStr = "hello jack";
	const char* pszStr2 = "hello jack2";
	TestRef* tr = new TestRef();
	std::map<int, std::map<int,SharedPtr<TestRef>>> tesstMap;
	tesstMap[1] = std::map<int, SharedPtr<TestRef>>();
	auto i =  tesstMap.find(1);
	i->second.insert(std::make_pair(1, SharedPtr<TestRef>(tr)));
	auto k = i->second.find(1);
	k->second->a = 25;
	tesstMap.erase(i);
 

	App* app = new App();
	app->Initialize();
	app->Run();
	Path curDir = GetCurrentDir();
	std::cout << curDir.c_str() << std::endl;


	char c = getchar();
	while (c != 'q')
	{
		c = getchar();
	}
	app->Close();
	return 0;
}