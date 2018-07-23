#include "Sapphire.h"
#include "Core.h"
#ifdef SAPPHIRE_WIN
#include <windows.h>
#include <sysinfoapi.h>
#else

#endif

namespace Sapphire
{
	bool Is64bitSystem()
	{
#ifdef SAPPHIRE_WIN
		SYSTEM_INFO si;
		GetNativeSystemInfo(&si);
		if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 ||
			si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64)
			return TRUE;
		else
			return FALSE;
#else
		return false;
#endif

	}

}

inline SAPPHIRE_API void Mem_Set_WORD(void *dest, ushort data, int count)
{
	//�ú���һ�����16λ�ֽ��ڴ�
#ifdef _MSC_VER
	_asm
	{
		mov edi, dest; //EDIָ��ָ��Ŀ���ڴ��ַ 
		mov ecx, count; //���ƶ���������ECX 
		mov ax, data; //��16λ���ݷ���AX
		rep stosw; //repָ�����ظ���������,ֱ��ECXΪ0��ÿִ��һ��ECX-1��STOS�ǽ�EAXֵ������ES:ediָ��ĵ�ַ; ���������direction flag, ��ôedi���ڸ�ָ��ִ�к��С, ���û������direction flag, ��ôedi��ֵ������.
	}
#else
	memset(dest, data, size);
#endif
}

///////////////////////////////////////////////////////////

inline SAPPHIRE_API void Mem_Set_QUAD(void *dest, uint data, int count)
{
#ifdef _MSC_VER
	// �ú���һ�����32λ�ֽ��ڴ�
	_asm
	{
		mov edi, dest;
		mov ecx, count;
		mov eax, data;
		rep stosd;
	}
#else
	memset(dest, data, size);
#endif
}