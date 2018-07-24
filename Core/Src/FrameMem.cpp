#include "FrameMem.h"
#include <malloc.h>
#include <assert.h>


namespace Sapphire
{
	static int _nByteAlignment;	  //�ڴ������ֽ���
	static byte *_pMemoryBlock;	   // ������ڴ��
	static byte *_apBaseAndCap[2];  // [0]=��ָ��, [1]=��ָ��
	static byte *_apFrame[2];		  // [0]=��λָ��, [1]=��λָ��

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
		//ȷ��nSizeInBytes��nByteAlignment��������
		nSizeInBytes = ALIGNUP(nSizeInBytes, nByteAlignment);

		// ��һ�η����ڴ��
		_pMemoryBlock = (byte *)malloc(nSizeInBytes + nByteAlignment);
		if (_pMemoryBlock == 0) {
			//�����ڴ�ʧ��!
			return 1;
		}

		_nByteAlignment = nByteAlignment;

		// ���û���ַ
		_apBaseAndCap[0] = (byte *)ALIGNUP(_pMemoryBlock, nByteAlignment);

		//���ö�����
		_apBaseAndCap[1] = (byte *)ALIGNUP(_pMemoryBlock + nSizeInBytes, nByteAlignment);

		// ��ʼ������ָ��
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

		// ���ȶ�����Ҫ�Ĵ�ѧ
		nBytes = ALIGNUP(nBytes, _nByteAlignment);

		//�����Ч�ڴ�
		if (_apFrame[0] + nBytes > _apFrame[1]) {
			//�ڴ治��
			return 0;
		}

		//�����ڴ�

		if (nHeapNum) {
			// �������·���

			_apFrame[1] -= nBytes;
			pMem = _apFrame[1];

		}
		else {
			// �������Ϸ���

			pMem = _apFrame[0];
			_apFrame[0] += nBytes;
		}

		return (void *)pMem;
	}

	// ����һ���Ժ���ͷŵ�֡�ڴ���
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
		//����Ƿ�Խ��
		assert(Frame.nHeapNum == 1 || (uint)Frame.pFrame <= (uint)_apFrame[0]);

		assert(Frame.nHeapNum == 0 || (uint)Frame.pFrame >= (uint)_apFrame[1]);

		//�ͷ�֡
		_apFrame[Frame.nHeapNum] = Frame.pFrame;
	}

}

