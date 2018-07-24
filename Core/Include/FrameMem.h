#ifndef __MEMORY_H__
#define __MEMORY_H__

#include "Sapphire.h"

#define ALIGNUP( nAddress, nBytes ) ( (((uint)nAddress) + (nBytes)-1) & (~((nBytes)-1)) )

namespace Sapphire
{
	typedef struct {
		byte *pFrame;
		int nHeapNum;
	} FrameMem;



	//初始化时调用
	//nByteAlignment必须是2的倍数
	SAPPHIRE_API int InitFrameMemorySystem(int nSizeInBytes, int nByteAlignment);

	SAPPHIRE_API void ShutdownFrameMemorySystem(void);
	//分配帧内存
	SAPPHIRE_API void *AllocFrameMemory(int nBytes, int nHeapNum);

	SAPPHIRE_API FrameMem GetFrame(int nHeapNum);

	SAPPHIRE_API void ReleaseFrame(FrameMem Frame);
}



#endif
