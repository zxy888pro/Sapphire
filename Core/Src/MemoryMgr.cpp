#include <MemoryMgr.h>
#include <stdarg.h> 
#include <path.h>
#include <stringHelper.h>

namespace Sapphire
{

	int   NumAllocations = 0;
	char  LOGFILE[40] = "memoryLogFile.log";
	const char* const s_allocationTypes[] = { "Unknown", "new", "new[]", "malloc", "calloc",
		"realloc", "delete", "delete[]", "free" };


	const char * formatOwnerString(const char *file, int line)
	{
		Path path = file;
		return StringFormatA("%s (%d ��)", path.getFileName().c_str(), line).c_str();
	}


	void log(char *s, ...)
	{
		static char buffer[2048];            
		va_list	list;
		va_start(list, s);
		vsprintf(buffer, s, list);
		va_end(list);

		FILE	*fp = fopen(LOGFILE, "ab");   
		if (!fp) return;

		fprintf(fp, "%s\r\n", buffer);      
		fclose(fp);                         
	}

	//��ʽ���ַ���������ָ���,
	char *insertCommas(unsigned long value)
	{
		static	char	str[30];
		for (int ii = 0; ii < 30; ++ii) str[ii] = NULL;
		sprintf(str, "%d", value);
		if (strlen(str) > 3) {
			memmove(&str[strlen(str) - 3], &str[strlen(str) - 4], 4);
			str[strlen(str) - 4] = ',';
		}
		if (strlen(str) > 7) {
			memmove(&str[strlen(str) - 7], &str[strlen(str) - 8], 8);
			str[strlen(str) - 8] = ',';
		}
		if (strlen(str) > 11) {
			memmove(&str[strlen(str) - 11], &str[strlen(str) - 12], 12);
			str[strlen(str) - 12] = ',';
		}
		return str;
	}

	//��ʽ���ڴ��С�ַ���
	char *memorySizeString(unsigned long size, bool lengthenString = true)
	{
		static	char	str[90];
		if (size > (1024 * 1024))
			sprintf(str, "%s (%7.2fM)", insertCommas(size), size / (1024.0 * 1024.0));
		else if (size > 1024)
			sprintf(str, "%s (%7.2fK)", insertCommas(size), size / 1024.0);
		else
			sprintf(str, "%s bytes     ", insertCommas(size), size);
		return str;
	}

	SAPPHIRE_API	void* AllocateMemory(const char *file, int line, size_t size, MemAllocType type, void *address /*= NULL*/)
	{
		return NULL;
	}

	SAPPHIRE_API void DeallocateMemory(const char *file, int line, void *address, MemAllocType type)
	{

	}

	SAPPHIRE_API void BreakOnAllocation(int allocationCount)
	{

	}

	SAPPHIRE_API void BreakOnDeallocation(void *address)
	{

	}

	SAPPHIRE_API void BreakOnReallocation(void *address)
	{

	}

	SAPPHIRE_API void setPaddingSize(int size /*= 4*/)
	{

	}

	SAPPHIRE_API void setOwner(const char *file, int line)
	{

	}

	SAPPHIRE_CLASS void* operator new(size_t size){ return malloc(size); }

	SAPPHIRE_CLASS void* operator new[](size_t size)  { return malloc(size); }

	SAPPHIRE_CLASS void* operator new(size_t size, const char *file, int line)
	{
		return AllocateMemory(file, line, size, MemAllocType::MAT_New);
	}

	SAPPHIRE_CLASS	void* operator new[](size_t size, const char *file, int line)
	{
		return AllocateMemory(file, line, size, MemAllocType::MAT_NewArray);
	}

	SAPPHIRE_CLASS	void operator delete(void *address)
	{
		if (!address) return;
		DeallocateMemory(__FILE__, __LINE__, address, MemAllocType::MAT_Delete);
	}
	SAPPHIRE_CLASS void operator delete[](void *address)
	{
		if (!address) return;
		DeallocateMemory(__FILE__, __LINE__, address, MemAllocType::MAT_DeleteArray);
	}
	SAPPHIRE_CLASS void operator delete(void *address, const char *file, int line)   { free(address); }
	SAPPHIRE_CLASS void operator delete[](void *address, const char *file, int line) { free(address); }


	class MemoryMgr
	{

	};

	myStack::myStack()
	{

	}

	void myStack::init()
	{
		m_head = NULL; m_count = 0;
	}

	bool myStack::empty()
	{
		return (m_count == 0);
	}

	StackNode* myStack::top()
	{
		return m_head;
	}

	void myStack::push(StackNode *n)
	{
		if (!n) return; n->next = m_head; m_head = n; m_count++;
	}

	StackNode* myStack::pop()
	{
		StackNode *n = m_head; if (n) m_head = m_head->next; m_count--; return n;
	}

	MemoryManager::MemoryManager(void)
	{

	}

	MemoryManager::~MemoryManager(void)
	{

	}

	void MemoryManager::initialize(void)
	{
		m_breakOnAllocationCount = -1;
		m_paddingSize = 4;
		m_logAlways = true;
		m_cleanLogFileOnFirstRun = true;

		m_totalMemoryAllocated = m_totalMemoryUsed = m_totalMemoryAllocations = 0;
		m_peakMemoryAllocation = m_numAllocations = m_peakTotalNumAllocations = 0;
		m_overheadMemoryCost = m_totalOverHeadMemoryCost = m_peakOverHeadMemoryCost = 0;
		m_allocatedMemory = m_numBoundsViolations = 0;

		for (int ii = 0; ii < HASH_SIZE; ++ii) {
			m_hashTable[ii] = NULL;
		}

		m_topStack.init();

		m_memoryCache = NULL;
	}

	void MemoryManager::release(void)
	{
		// ���ڴ�û�л���
		// ����ڴ�й¶�����������ڴ�����������ʣ���ڴ�
		if (m_numAllocations != 0) {
			for (int ii = 0; ii < HASH_SIZE; ++ii) {
				while (m_hashTable[ii]) {
					MemoryNode *ptr = m_hashTable[ii];
					m_hashTable[ii] = m_hashTable[ii]->next;
					free(ptr->actualAddress);      // �ͷ�ʣ��й¶���ڴ�
					free(ptr);                     // �ͷŴ�����MemoryNode
				}
			}
		}

		// �����¼���ջ
		while (!m_topStack.empty()) {
			StackNode* pNode = m_topStack.pop();
			if (pNode != NULL)
			{
				free(pNode);
				pNode = NULL;
			}

		}

		// �����ڴ滺��
		MemoryNode *ptr;
		while (m_memoryCache) {
			ptr = m_memoryCache;
			if (ptr != NULL)
			{
				m_memoryCache = ptr->next;
				free(ptr);
			}
		}
	}

	void MemoryManager::insertMemoryNode(MemoryNode *node)
	{
		//ͨ���ڵ�ĵ�ַ�����hash����
		int hashIndex = getHashIndex(node->reportedAddress);
		//β��
		node->next = m_hashTable[hashIndex];
		node->prev = NULL;

		if (m_hashTable[hashIndex]) 
			m_hashTable[hashIndex]->prev = node;

		m_hashTable[hashIndex] = node;

		//�ռ���Ϣ
		m_numAllocations++;

		m_allocatedMemory += node->reportedSize;

		if (m_allocatedMemory > m_peakMemoryAllocation)   
			m_peakMemoryAllocation = m_allocatedMemory;

		if (m_numAllocations > m_peakTotalNumAllocations) 
			m_peakTotalNumAllocations = m_numAllocations;

		m_totalMemoryAllocated += node->reportedSize;
		m_totalMemoryAllocations++;
	}

	MemoryNode * MemoryManager::getMemoryNode(void *address)
	{
		MemoryNode *ptr = m_hashTable[getHashIndex(address)];
		while (ptr && ptr->reportedAddress != address) {
			ptr = ptr->next;
		}
		return ptr;
	}

	MemoryNode * MemoryManager::removeMemoryNode(void *address)
	{
		int hashIndex = getHashIndex(address);

		MemoryNode *ptr = m_hashTable[hashIndex];
		while (ptr && ptr->reportedAddress != address) {
			ptr = ptr->next;
		}

		if (ptr) {
			if (ptr->next) 
				ptr->next->prev = ptr->prev;
			if (ptr->prev) 
				ptr->prev->next = ptr->next;
			else           
				m_hashTable[hashIndex] = ptr->next;

			m_numAllocations--;
			m_allocatedMemory -= ptr->reportedSize;
		}
		return ptr;
	}

	bool MemoryManager::validateMemoryUnit(MemoryNode *node)
	{
		bool success = true;
		unsigned int ii;
		unsigned int totalBytesUsed = 0, boundViolations = 0;

		// ����ڴ�Խ��
		// BeginPadding��EndPadding��ַ
		long *beginning = (long*)node->actualAddress;
		long *ending = (long*)((char*)node->actualAddress + node->actualSize - node->paddingSize*sizeof(long));
		for (ii = 0; ii < node->paddingSize; ++ii) {
			if (beginning[ii] != PADDING || ending[ii] != PADDING) {
				success = false;  // ������Խ��
				//padding���ÿ���ֽڶ�Ӧ�õ���PADDING
				boundViolations++;
			}
		}

		if (!success) m_numBoundsViolations++;

		//���Լ��ʵ��ʵ�ʷ����ڴ�
		// ��ʼ��memory padding ���Խ��
		long *lptr = (long*)node->reportedAddress;
		//��longΪ��λ���ֿ���
		unsigned int len = node->reportedSize / sizeof(long);
		unsigned int cnt;  
		for (cnt = 0; cnt < len; ++cnt) {
			if (lptr[cnt] != node->predefinedBody)       
				totalBytesUsed += sizeof(long);
		}
		char *cptr = (char*)(&lptr[cnt]);
		len = node->reportedSize - len*sizeof(long);
		for (cnt = 0; cnt < len; ++cnt) {
			if (cptr[cnt] != (char)node->predefinedBody) 
				totalBytesUsed++;
		}

		m_totalMemoryUsed += totalBytesUsed;

		//ʹ�õ��ֽڲ����ڽڵ�����С
		if (m_logAlways && totalBytesUsed != node->reportedSize) {          //����ٷֱ�
			this->log("Unused Memory Detected   : %-40s %8s(0x%08p) : %s",   //  �˷ѵ��Ŀռ�
				formatOwnerString(node->sourceFile, node->sourceLine),
				s_allocationTypes[node->allocationType], node->reportedAddress,
				memorySizeString(node->reportedSize - totalBytesUsed));
		}

		if (m_logAlways && !success) {                                      // �ڴ�Խ����
			this->log("Bounds Violation Detected: %-40s %8s(0x%08p)",        //   
				formatOwnerString(node->sourceFile, node->sourceLine),
				s_allocationTypes[node->allocationType], node->reportedAddress);
		}

		return success;
	}

	void MemoryManager::deallocateMemory(MemoryNode *node)
	{
		m_overheadMemoryCost -= (node->paddingSize * 2 * sizeof(long));
		node->next = m_memoryCache;
		m_memoryCache = node;
	}

	MemoryNode* MemoryManager::allocateMemory(void)
	{
		if (!m_memoryCache) {
			int overhead = m_paddingSize * 2 * sizeof(long) + sizeof(MemoryNode);
			m_overheadMemoryCost += overhead;
			m_totalOverHeadMemoryCost += overhead;

			if (m_overheadMemoryCost > m_peakOverHeadMemoryCost) {
				m_peakOverHeadMemoryCost = m_overheadMemoryCost;
			}

			return (MemoryNode*)malloc(sizeof(MemoryNode));
		}
		else {
			int overhead = m_paddingSize * 2 * sizeof(long);
			m_overheadMemoryCost += overhead;
			m_totalOverHeadMemoryCost += overhead;

			if (m_overheadMemoryCost > m_peakOverHeadMemoryCost) {
				m_peakOverHeadMemoryCost = m_overheadMemoryCost;
			}

			MemoryNode *ptr = m_memoryCache;
			m_memoryCache = m_memoryCache->next;
			return ptr;
		}
	}

	void MemoryManager::dumpLogReport(void)
	{

	}

	void MemoryManager::dumpMemoryAllocations(void)
	{

	}

	void MemoryManager::log(char *s, ...)
	{

	}

	int MemoryManager::getHashIndex(void *address)
	{
		return ((unsigned int)address >> 4) & (HASH_SIZE - 1);
	}

	int MemoryManager::calculateUnAllocatedMemory()
	{
		int memory = 0;
		for (int ii = 0; ii < HASH_SIZE; ++ii) {
			for (MemoryNode *ptr = m_hashTable[ii]; ptr; ptr = ptr->next) {
				memory += ptr->reportedSize;
			}
		}
		return memory;
	}

}