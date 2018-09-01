#include "stdafx.h"
#include <Sapphire.h>
#include <Graphics.h>
#include <GraphicDriver.h>
#include "Camera.h"
#include <MemoryMgr.h>


void init()
{

	new Sapphire::Core();
	new Sapphire::GraphicDriver();


	Sapphire::GraphicDriver::GetSingletonPtr()->Init();

	Sapphire::IImageMgr* pImgMgr = Sapphire::GraphicDriver::GetSingletonPtr()->getImageMgr();
	new Sapphire::Camera();
}

void release()
{
	Sapphire::Camera* pcam = Sapphire::Camera::GetSingletonPtr();
	safeDelete(pcam);
	Sapphire::GraphicDriver* pGd = Sapphire::GraphicDriver::GetSingletonPtr();
	pGd->Release();
	safeDelete(pGd);
	Sapphire::Core* pCore = Sapphire::Core::GetSingletonPtr();
	pCore->Release();
	safeDelete(pCore);
}

void TestMemoryTrack()
{
	using namespace Sapphire;

	/*int* p2 = _New int[5];
	_Delete[] p2;*/

	int* p1 = (int*)_Malloc(sizeof(int));
	*p1 = 5;
	_Free(p1);

	/*char* pszTmp = _New char[256];
	memset(pszTmp, 0, 256);
	_Delete[] pszTmp;
	*/
}

int main()
{
	using namespace Sapphire;

	init();
	TestMemoryTrack();
	
	Core::GetSingletonPtr()->GetMemoryMgr()->dumpLogReport();
	release();



	return 0;
}