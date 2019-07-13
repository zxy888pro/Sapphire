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
			//ImageRes* img1 = new ImageRes(pCore, "container2.png");
			//resourceLoader->LoadResource(img1);
			resourceLoader->LoadResource(xml1);
			while (!getExitFlag())
			{
				BaseResource* resource = resourceCache->GetResource("NinjaSnowWarShaders.xml");
				if (resource)
				{
					XMLFile* xmlFile = dynamic_cast<XMLFile*>(resource);
					XMLElement root =  xmlFile->GetRoot();
					XMLElement child = root.GetChild("shader");
					SAPPHIRE_LOGERROR(StringFormatA("resource name %s   size %d", resource->GetName(), resource->GetSize()));
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