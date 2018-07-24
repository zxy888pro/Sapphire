#include "FrameMem.h"
#include <malloc.h>
#include <assert.h>


namespace Sapphire
{
	static int _nByteAlignment;	  //内存对齐的字节数
	static byte *_pMemoryBlock;	   // 分配的内存块
	static byte *_apBaseAndCap[2];  // [0]=基指针, [1]=顶指针
	static byte *_apFrame[2];		  // [0]=低位指针, [1]=高位指针

	SAPPHIRE_API int GetObjectSize(const char *pszObjectName)
	{
		return 0;
	}

	SAPPHIRE_API int LoadFromDisk(const char *pszObjectName, void *pLoadAddress)
	{

		return 0;
	}

	SAPPHIRE_API int InitFrameMemorySystem(int nSizeInBytes, int nByteAlignment)
	{
		//确保nSizeInBytes是nByteAlignment的整数倍
		nSizeInBytes = ALIGNUP(nSizeInBytes, nByteAlignment);

		// 第一次分配内存块
		_pMemoryBlock = (byte *)malloc(nSizeInBytes + nByteAlignment);
		if (_pMemoryBlock == 0) {
			//分配内存失败!
			return 1;
		}

		_nByteAlignment = nByteAlignment;

		// 设置基地址
		_apBaseAndCap[0] = (byte *)ALIGNUP(_pMemoryBlock, nByteAlignment);

		//设置顶底怎
		_apBaseAndCap[1] = (byte *)ALIGNUP(_pMemoryBlock + nSizeInBytes, nByteAlignment);

		// 初始化两边指针
		_apFrame[0] = _apBaseAndCap[0];
		_apFrame[1] = _apBaseAndCap[1];

		return 0;
	}

	SAPPHIRE_API void ShutdownFrameMemorySystem(void)
	{
		free(_pMemoryBlock);
	}

	
	SAPPHIRE_API void * AllocFrameMemory(int nBytes, int nHeapNum)
	{
		byte *pMem;

		// 首先对齐需要的大学
		nBytes = ALIGNUP(nBytes, _nByteAlignment);

		//检查有效内存
		if (_apFrame[0] + nBytes > _apFrame[1]) {
			//内存不够
			return 0;
		}

		//分配内存

		if (nHeapNum) {
			// 从上往下分配

			_apFrame[1] -= nBytes;
			pMem = _apFrame[1];

		}
		else {
			// 从下往上分配

			pMem = _apFrame[0];
			_apFrame[0] += nBytes;
		}

		return (void *)pMem;
	}

	// 返回一个以后可释放的帧内存句柄
	// nHeapNum : 0=lower, 1=upper.
	SAPPHIRE_API FrameMem GetFrame(int nHeapNum)
	{
		FrameMem Frame;

		Frame.pFrame = _apFrame[nHeapNum];
		Frame.nHeapNum = nHeapNum;

		return Frame;
	}

	SAPPHIRE_API void ReleaseFrame(FrameMem Frame)
	{
		//检查是否越界
		assert(Frame.nHeapNum == 1 || (uint)Frame.pFrame <= (uint)_apFrame[0]);

		assert(Frame.nHeapNum == 0 || (uint)Frame.pFrame >= (uint)_apFrame[1]);

		//释放帧
		_apFrame[Frame.nHeapNum] = Frame.pFrame;
	}

}

