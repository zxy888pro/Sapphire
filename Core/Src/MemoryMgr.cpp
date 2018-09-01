#include <MemoryMgr.h>
#include <stdarg.h> 
#include <path.h>
#include <stringHelper.h>
#include <assert.h>
#include <Core.h>

int   NumAllocations = 0;
char  LOGFILE[40] = "memoryLogFile.log";
const char* const s_allocationTypes[] = { "Unknown", "new", "new[]", "malloc", "calloc",
"realloc", "delete", "delete[]", "free" };

char* mystrrchr(const char *s, int c)
{
	if (s == NULL)
	{
		return NULL;
	}

	char *p_char = NULL;
	while (*s != '\0')
	{
		if (*s == (char)c)
		{
			p_char = (char *)s;
		}
		s++;
	}

	return p_char;
}

char *sourceFileStripper(const char *sourceFile)
{
	if (!sourceFile) return NULL;
	char *ptr = mystrrchr(sourceFile, '\\');
	if (ptr) return ptr + 1;
	ptr = mystrrchr(sourceFile, '/');
	if (ptr) return ptr + 1;
	return (char*)sourceFile;
}


const char * formatOwnerString(const char *file, int line)
{
	static char	str[90];
	memset(str, 0, sizeof(str));
	sprintf(str, "%s(%05d)", sourceFileStripper(file), line);
	return str;
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
	using namespace Sapphire;
	MemoryNode *memory = NULL;
	setOwner(file, line);
	assert(Core::GetSingletonPtr());
	if (Core::GetSingletonPtr() == NULL)
	{
		return NULL;
	}
	MemoryManager* pMemMgr = Core::GetSingletonPtr()->GetMemoryMgr();
	assert(pMemMgr);
	if (pMemMgr == NULL)
	{
		return NULL;
	}
	if (!pMemMgr &&  pMemMgr->IsInitialized() == false) {
		if (NumAllocations != 0) {
			log("The Memory Manager has already been released from memory, however an allocation was requested");
			log("\t%-40s", formatOwnerString(file, line));
		}
		return malloc(size);  //����ֱ�ӷ����ڴ�
	}

	/*if (size == 0)
	size = 1; */

	// ��������Ƿ����жϴ���
	m_assert(pMemMgr->m_totalMemoryAllocations != pMemMgr->m_breakOnAllocationCount);

	// ����������
	m_assert(type != MemAllocType::MAT_Unknown);

	//������ط���
	if (type == MemAllocType::MAT_Realloc) {
		//���Ƴ�֮ǰ������ڴ�ڵ�
		MemoryNode *memory = pMemMgr->removeMemoryNode(address);

		m_assert(memory);
		if (!memory) {
			pMemMgr->log("Request to reallocate RAM that was never allocated.");
		}

		// ��֤�������ͣ�ֻ��malloc/calloc/realloc����Ľڵ�����ط���
		m_assert(memory->allocationType == MemAllocType::MAT_Malloc ||
			memory->allocationType == MemAllocType::MAT_Calloc ||
			memory->allocationType == MemAllocType::MAT_Realloc);

		//�Ƿ����ط������ж�
		m_assert((memory->options & BREAK_ON_REALLOC) == 0x0);

		memory->actualSize = size + pMemMgr->m_paddingSize * sizeof(long) * 2;
		memory->reportedSize = size;
		//�ط����ڴ�
		memory->actualAddress = realloc(memory->actualAddress, memory->actualSize);
	}
	else {
		// ����һ���µ��ڴ��
		memory = pMemMgr->allocateMemory();

		//  
		m_assert(memory != NULL);
		if (memory == NULL) {
			pMemMgr->log("Could not allocate memory for memory tracking.  Out of memory.");
		}

		memory->actualSize = size + pMemMgr->m_paddingSize * sizeof(long) * 2;
		memory->reportedSize = size;
		memory->actualAddress = malloc(memory->actualSize);
		memory->options = 0;
	}

	memory->reportedAddress = (char*)memory->actualAddress + pMemMgr->m_paddingSize * sizeof(long);
	memory->sourceLine = line;
	memory->paddingSize = pMemMgr->m_paddingSize;
	memory->allocationType = type;
	Path p = file;

	strcpy(memory->sourceFile, p.getFileName().c_str());

	if (pMemMgr->m_logAlways) {
		pMemMgr->log("Memory Allocation        : %-40s %8s(0x%08p) : %s", formatOwnerString(file, line),
			s_allocationTypes[type], memory->reportedAddress, memorySizeString(size));
	}

	// ��֤����
	m_assert(memory->actualAddress);
	if (!memory->actualAddress) {
		pMemMgr->log("Request for allocation failed.  Out of memory.");
	}

	// ��ʼ����������ݣ���������ͷ�
	if (type == MemAllocType::MAT_Calloc)
		memory->InitializeMemory(0x00000000);
	else
		memory->InitializeMemory(0xBAADC0DE);


	// ����MemoryNode��hashlist
	pMemMgr->insertMemoryNode(memory);
	return memory->reportedAddress;

}

SAPPHIRE_API void DeallocateMemory(const char *file, int line, void *address, MemAllocType type)
{
	using namespace Sapphire;
	setOwner(file, line);
	//assert(Core::GetSingletonPtr());
	if (Core::GetSingletonPtr() == NULL)
	{
		return;
	}
	MemoryManager* pMemMgr = Core::GetSingletonPtr()->GetMemoryMgr();
	//assert(pMemMgr);
	if (pMemMgr == NULL)
	{
		return;
	}


	StackNode *info = pMemMgr->m_topStack.empty() ? NULL : pMemMgr->m_topStack.top();

	// �Ƴ��ڴ�ڵ�
	MemoryNode *memory = pMemMgr->removeMemoryNode(address);


	if (!memory) {
		if (address != NULL)
		{
			free(address);
		}
		return;
	}

	// �������־��Ϣ
	if (pMemMgr->m_logAlways && info != NULL) {
		pMemMgr->log("Memory Deallocation      : %-40s %8s(0x%08p) : %s",
			formatOwnerString(info->fileName, info->lineNumber),
			s_allocationTypes[type], address, memorySizeString(memory->reportedSize));
	}



	m_assert(type != MAT_Unknown);

	// У���ڴ����
	pMemMgr->validateMemoryUnit(memory);

	// ������ȷ���ͷŷ�ʽ
	m_assert(type == MAT_Delete       && memory->allocationType == MAT_New ||
		type == MAT_DeleteArray && memory->allocationType == MAT_NewArray ||
		type == MAT_Free         && memory->allocationType == MAT_Malloc ||
		type == MAT_Free         && memory->allocationType == MAT_Calloc ||
		type == MAT_Free         && memory->allocationType == MAT_Realloc);

	// �Ƿ������ͷŵ�ʱ���ж�
	m_assert((memory->options & BREAK_ON_DEALLOC) == 0x0);

	//�ͷ��ڴ�
	free(memory->actualAddress);

	// �ͷ��ڴ�ڵ�
	pMemMgr->deallocateMemory(memory);

	//�ͷż�¼�����ļ������кŵ���Ϣ
	if (info) {
		pMemMgr->m_topStack.pop();
		free(info);
	}
}

SAPPHIRE_API void BreakOnAllocation(int allocationCount)
{
	using namespace Sapphire;
	assert(Core::GetSingletonPtr());
	if (Core::GetSingletonPtr() == NULL)
	{
		return;
	}
	MemoryManager* pMemMgr = Core::GetSingletonPtr()->GetMemoryMgr();
	if (pMemMgr && allocationCount > 0)
		pMemMgr->m_breakOnAllocationCount = allocationCount;
}

SAPPHIRE_API void BreakOnDeallocation(void *address)
{
	using namespace Sapphire;
	assert(Core::GetSingletonPtr());
	if (Core::GetSingletonPtr() == NULL)
	{
		return;
	}
	MemoryManager* pMemMgr = Core::GetSingletonPtr()->GetMemoryMgr();
	if (pMemMgr == NULL || address == NULL)
		return;
	MemoryNode *node = pMemMgr->getMemoryNode(address);
	node->options |= BREAK_ON_DEALLOC;
}

SAPPHIRE_API void BreakOnReallocation(void *address)
{
	using namespace Sapphire;
	assert(Core::GetSingletonPtr());
	if (Core::GetSingletonPtr() == NULL)
	{
		return;
	}
	MemoryManager* pMemMgr = Core::GetSingletonPtr()->GetMemoryMgr();
	if (pMemMgr == NULL || address == NULL)
		return;
	MemoryNode *node = pMemMgr->getMemoryNode(address);
	node->options |= BREAK_ON_REALLOC;
}

SAPPHIRE_API void setPaddingSize(int size /*= 4*/)
{
	using namespace Sapphire;
	assert(Core::GetSingletonPtr());
	if (Core::GetSingletonPtr() == NULL)
	{
		return;
	}
	MemoryManager* pMemMgr = Core::GetSingletonPtr()->GetMemoryMgr();
	if (pMemMgr && size > 0)
		pMemMgr->m_paddingSize = size;
}

SAPPHIRE_API void setOwner(const char *file, int line)
{
	using namespace Sapphire;
	//assert(Core::GetSingletonPtr());
	if (Core::GetSingletonPtr() == NULL)
	{
		return;
	}
	MemoryManager* pMemMgr = Core::GetSingletonPtr()->GetMemoryMgr();
	//assert(pMemMgr);
	if (pMemMgr == NULL)
	{
		return;
	}
	if (pMemMgr) {
		StackNode *n = (StackNode*)malloc(sizeof(StackNode));
		n->fileName = file;
		n->lineNumber = line;
		pMemMgr->m_topStack.push(n);
	}
}

void* operator new(size_t size){ return malloc(size); }

void* operator new[](size_t size)  { return malloc(size); }

void* operator new(size_t size, const char *file, int line)
{
	return AllocateMemory(file, line, size, MemAllocType::MAT_New);
}

void* operator new[](size_t size, const char *file, int line)
{
	return AllocateMemory(file, line, size, MemAllocType::MAT_NewArray);
}

void operator delete(void *address)
{
	if (!address) return;
	DeallocateMemory(__FILE__, __LINE__, address, MemAllocType::MAT_Delete);
}
void operator delete[](void *address)
{
	if (!address) return;
	DeallocateMemory(__FILE__, __LINE__, address, MemAllocType::MAT_DeleteArray);
}
void operator delete(void *address, const char *file, int line)   { free(address); }
void operator delete[](void *address, const char *file, int line) { free(address); }



namespace Sapphire
{

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
		m_bIsInitialized = false;
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
		m_bIsInitialized = true;
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
		// ���ӵ�ǰ�ѷ�����ڴ�
		m_allocatedMemory += node->reportedSize;
		//����ѷ����ڴ���ڷ�ֵ�ڴ棬���·�ֵ�ڴ�
		if (m_allocatedMemory > m_peakMemoryAllocation)   
			m_peakMemoryAllocation = m_allocatedMemory;
		//�ѷ��������ڷ�ֵ�����������·�������ֵ
		if (m_numAllocations > m_peakTotalNumAllocations) 
			m_peakTotalNumAllocations = m_numAllocations;

		//���ӷ�������
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

			//���ٷ�����
			m_numAllocations--;
			//�����ѷ����ڴ�
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
		//��longΪ��λ���ֿ�����//��longΪ��λ���predefineBody
		unsigned int len = node->reportedSize / sizeof(long);
		unsigned int cnt;  
		for (cnt = 0; cnt < len; ++cnt) {
			if (lptr[cnt] != node->predefinedBody)       
				totalBytesUsed += sizeof(long);
		}
		//�жϲ��ܱ���������������
		char *cptr = (char*)(&lptr[cnt]);
		len = node->reportedSize - len*sizeof(long);
		for (cnt = 0; cnt < len; ++cnt) {
			if (cptr[cnt] != (char)node->predefinedBody) 
				totalBytesUsed++;
		}

		m_totalMemoryUsed += totalBytesUsed;

		//ʹ�õ��ֽڲ����ڽڵ�����С
		//��й©
		if (m_logAlways && totalBytesUsed != node->reportedSize) {          //����ٷֱ�
			this->log("Unused Memory Detected   : %-40s %8s(0x%08p) : %s",   //  �˷ѵ��Ŀռ�
				formatOwnerString(node->sourceFile, node->sourceLine),
				s_allocationTypes[node->allocationType], node->reportedAddress,
				memorySizeString(node->reportedSize - totalBytesUsed));
		}
		//��Խ��
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
			//û�л��棬������MemoryNode����
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
			//ȡ��һ�������MemoryNode
			MemoryNode *ptr = m_memoryCache;
			m_memoryCache = m_memoryCache->next;
			return ptr;
		}
	}

	void MemoryManager::dumpLogReport(void)
	{
		if (m_cleanLogFileOnFirstRun) {      // ������־
			_unlink(LOGFILE);                 //ɾ���ļ�
			m_cleanLogFileOnFirstRun = false;  
		}

		FILE	*fp = fopen(LOGFILE, "ab"); 
		if (!fp) return;

		time_t t = time(NULL);
		tm *time = localtime(&t);

		int memoryLeak = calculateUnAllocatedMemory();
		int totalMemoryDivider = m_totalMemoryAllocated != 0 ? m_totalMemoryAllocated : 1;

		//Header
		fprintf(fp, "\r\n");
		fprintf(fp, "******************************************************************************* \r\n");
		fprintf(fp, "*********           �ڴ汨�� for: %02d/%02d/%04d %02d:%02d:%02d            ********* \r\n", time->tm_mon + 1, time->tm_mday, time->tm_year + 1900, time->tm_hour, time->tm_min, time->tm_sec);
		fprintf(fp, "******************************************************************************* \r\n");
		fprintf(fp, "\r\n");

		//Report
		fprintf(fp, "                     T O T A L  M E M O R Y  U S A G E                          \r\n");
		fprintf(fp, "------------------------------------------------------------------------------- \r\n");
		fprintf(fp, "           Total Number of Dynamic Allocations: %10s\r\n", insertCommas(m_totalMemoryAllocations));
		fprintf(fp, "      Reported Memory usage to the Application: %s\r\n", memorySizeString(m_totalMemoryAllocated));
		fprintf(fp, "          Actual Memory use by the Application: %s\r\n", memorySizeString(m_totalOverHeadMemoryCost + m_totalMemoryAllocated));
		fprintf(fp, "                      Memory Tracking Overhead: %s\r\n", memorySizeString(m_totalOverHeadMemoryCost));
		fprintf(fp, "\r\n");

		fprintf(fp, "                      P E A K  M E M O R Y  U S A G E                           \r\n");
		fprintf(fp, "------------------------------------------------------------------------------- \r\n");
		fprintf(fp, "            Peak Number of Dynamic Allocations: %10s\r\n", insertCommas(m_peakTotalNumAllocations));
		fprintf(fp, " Peak Reported Memory usage to the application: %s\r\n", memorySizeString(m_peakMemoryAllocation));
		fprintf(fp, "     Peak Actual Memory use by the Application: %s\r\n", memorySizeString(m_peakOverHeadMemoryCost + m_peakMemoryAllocation));
		fprintf(fp, "                 Peak Memory Tracking Overhead: %s\r\n", memorySizeString(m_peakOverHeadMemoryCost));
		fprintf(fp, "\r\n");

		fprintf(fp, "                          U N U S E D  M E M O R Y                              \r\n");
		fprintf(fp, "------------------------------------------------------------------------------- \r\n");
		fprintf(fp, "  Percentage of Allocated Memory Actually Used: %10.2f %%\r\n", (float)(1 - (m_totalMemoryAllocated - m_totalMemoryUsed) / (float)totalMemoryDivider) * 100);
		fprintf(fp, "       Percentage of Allocated Memory Not Used: %10.2f %%\r\n", (float)(m_totalMemoryAllocated - m_totalMemoryUsed) / (float)totalMemoryDivider * 100);
		fprintf(fp, "        Memory Allocated but not Actually Used: %s\r\n", memorySizeString(m_totalMemoryAllocated - m_totalMemoryUsed));
		fprintf(fp, "\r\n");

		fprintf(fp, "                      B O U N D S  V I O L A T I O N S                          \r\n");
		fprintf(fp, "------------------------------------------------------------------------------- \r\n");
		fprintf(fp, "            Number of Memory Bounds Violations: %10s\r\n", insertCommas(m_numBoundsViolations));
		fprintf(fp, "\r\n");

		fprintf(fp, "                           M E M O R Y  L E A K S                               \r\n");
		fprintf(fp, "------------------------------------------------------------------------------- \r\n");
		fprintf(fp, "                        Number of Memory Leaks: %10s\r\n", insertCommas(m_numAllocations));
		fprintf(fp, "                 Amount of Memory Un-Allocated: %s\r\n", memorySizeString(memoryLeak));
		fprintf(fp, "   Percentage of Allocated Memory Un-Allocated: %10.2f %%\r\n", (float)(1 - (m_totalMemoryAllocated - memoryLeak) / (float)totalMemoryDivider) * 100);
		fprintf(fp, "\r\n");

		if (m_numAllocations != 0) {  // �Ƿ����ڴ�й©
			fclose(fp);              
			dumpMemoryAllocations();    // ��ʾ�ڴ�й©
		}
		else {
			fclose(fp);
		}
	}

	void MemoryManager::dumpMemoryAllocations(void)
	{
		if (m_cleanLogFileOnFirstRun) {      
			_unlink(LOGFILE);                 
			m_cleanLogFileOnFirstRun = false;  
		}

		FILE	*fp = fopen(LOGFILE, "ab");  
		if (!fp) return;

		fprintf(fp, "              C U R R E N T L Y  A L L O C A T E D  M E M O R Y                 \r\n");
		fprintf(fp, "------------------------------------------------------------------------------- \r\n");

		for (int ii = 0, cnt = 1; ii < HASH_SIZE; ++ii) {
			for (MemoryNode *ptr = m_hashTable[ii]; ptr; ptr = ptr->next) {
				fprintf(fp, "** Allocation # %2d\r\n", cnt++);
				fprintf(fp, "Total Memory Size : %s\r\n", memorySizeString(ptr->reportedSize, false));
				fprintf(fp, "Source File       : %s\r\n", ptr->sourceFile);
				fprintf(fp, "Source Line       : %d\r\n", ptr->sourceLine);
				fprintf(fp, "Allocation Type   : %s\r\n", s_allocationTypes[ptr->allocationType]);
				fprintf(fp, "\r\n");
			}
		}

		fprintf(fp, "------------------------------------------------------------------------------- \r\n");
		fprintf(fp, "******************************************************************************* \r\n");
		fprintf(fp, "\r\n");

		fclose(fp);
	}

	void MemoryManager::log(char *s, ...)
	{
		if (m_cleanLogFileOnFirstRun) {       
			_unlink(LOGFILE);                  
			m_cleanLogFileOnFirstRun = false;   
		}

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

	bool MemoryManager::IsInitialized() const
	{
		return m_bIsInitialized;
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

	void MemoryNode::InitializeMemory(long body /*= BODY*/)
	{
		// ��ʼ���ڴ棬�����Խ��
		long *beginning = (long*)actualAddress; //��ʼ��ַ
		long *ending = (long*)((char*)actualAddress + actualSize - paddingSize*sizeof(long));  //������ַ����һ��padding��С������Ƚ���λpadding��
		for (unsigned short ii = 0; ii < paddingSize; ++ii) {
			beginning[ii] = ending[ii] = PADDING;
		}

		// ��ʼ���ڴ�
		beginning = (long*)reportedAddress;  //����ĵ�ַ
		unsigned int len = reportedSize / sizeof(long);    //��longΪ��λ�������BODY
		unsigned int cnt;
		for (cnt = 0; cnt < len; ++cnt) {                         // ��ʼ�����ڴ�
			beginning[cnt] = body;
		}
		char *cptr = (char*)(&beginning[cnt]);  //��������
		len = reportedSize - len*sizeof(long);
		for (cnt = 0; cnt < len; ++cnt) {    // ��ʼ��ʣ���ڴ�
			cptr[cnt] = (char)body;
		}

		predefinedBody = body;
	}

}