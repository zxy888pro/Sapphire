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
		fread(&elfHdr, 1, sizeof(Elf32_Ehdr), elfFp); //���ȶ�ȡElfHeader
		//section header�� sh_name ��Ա�������Ӧ��section header�ַ���
		//�ַ�����section���������˽����� ELF ͷ�� e_shstrndx ��Ա������
		fseek(elfFp, elfHdr.e_shoff + elfHdr.e_shstrndx * sizeof(sectHdr), SEEK_SET); //��λ��section headerƫ�Ƶ�ַ
		fread(&sectHdr, 1, sizeof(sectHdr), elfFp); 
		stringTableSection = (char*)malloc(sectHdr.sh_size);  
		fseek(elfFp, sectHdr.sh_offset, SEEK_SET);  //��ת���ַ�����section��ƫ�Ƶ�ַ
		fread(stringTableSection, 1, sectHdr.sh_size, elfFp);

		//��ȡ����section header
		for (idx = 0; idx < elfHdr.e_shnum; idx++)
		{
			const char* name = "";

			fseek(elfFp, elfHdr.e_shoff + idx * sizeof(sectHdr), SEEK_SET);
			fread(&sectHdr, 1, sizeof(sectHdr), elfFp);

			if (sectHdr.sh_name)//sh_name��ӦstringTable������
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