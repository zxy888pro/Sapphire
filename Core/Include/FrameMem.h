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



	//��ʼ��ʱ����
	//nByteAlignment������2�ı���
	SAPPHIRE_API int InitFrameMemorySystem(int nSizeInBytes, int nByteAlignment);

	SAPPHIRE_API void ShutdownFrameMemorySystem(void);
	//����֡�ڴ�
	SAPPHIRE_API void *AllocFrameMemory(int nBytes, int nHeapNum);

	SAPPHIRE_API FrameMem GetFrame(int nHeapNum);

	SAPPHIRE_API void ReleaseFrame(FrameMem Frame);
}



#endif
