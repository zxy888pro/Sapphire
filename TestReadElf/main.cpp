#include <iostream>
#include <stdio.h>
#include "elf.h"
#include <direct.h>
#include <string>
#include <string.h>
#include <errno.h>



int main()
{
	 
	char* stringTableSection;
	unsigned idx = 0;
	FILE* elfFp = fopen("libYzClient.so", "rb+");
	Elf32_Ehdr elfHdr;
	Elf32_Shdr sectHdr;
	if (elfFp != NULL)
	{
		memset(&elfHdr, 0, sizeof(Elf32_Ehdr));
		memset(&sectHdr, 0, sizeof(Elf32_Shdr));
		fread(&elfHdr, 1, sizeof(Elf32_Ehdr), elfFp); //首先读取ElfHeader
		//section header的 sh_name 成员包含其对应的section header字符串
		//字符串表section的索引，此节区由 ELF 头的 e_shstrndx 成员给出。
		fseek(elfFp, elfHdr.e_shoff + elfHdr.e_shstrndx * sizeof(sectHdr), SEEK_SET); //定位到section header偏移地址
		fread(&sectHdr, 1, sizeof(sectHdr), elfFp); 
		stringTableSection = (char*)malloc(sectHdr.sh_size);  
		fseek(elfFp, sectHdr.sh_offset, SEEK_SET);  //跳转到字符串表section的偏移地址
		fread(stringTableSection, 1, sectHdr.sh_size, elfFp);

		//读取所有section header
		for (idx = 0; idx < elfHdr.e_shnum; idx++)
		{
			const char* name = "";

			fseek(elfFp, elfHdr.e_shoff + idx * sizeof(sectHdr), SEEK_SET);
			fread(&sectHdr, 1, sizeof(sectHdr), elfFp);

			if (sectHdr.sh_name)//sh_name对应stringTable的索引
			{
				name = stringTableSection + sectHdr.sh_name;
				std::string sectionName = name;
				if ((sectionName.find("SO_VERSION") != std::string::npos) && (sectHdr.sh_type != SHT_NOBITS))
				{
					fseek(elfFp, sectHdr.sh_offset, SEEK_SET);
					char* szbuf = new char[sectHdr.sh_size]; 
					fread(szbuf, 1, sectHdr.sh_size, elfFp);
					std::string secContent = szbuf;
					delete szbuf;
				}
			}
				
			printf("%2u %s\n", idx, name);
		}

	}
	else
	{
		std::cout << "not found file!" << std::endl;
		fprintf(stderr, "%s \n", strerror(errno));
	}
	getchar();
	return 0;
}