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

//格式化字符串，加入分隔符,
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

//格式化内存大小字符串
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
		return malloc(size);  //尝试直接分配内存
	}

	/*if (size == 0)
	size = 1; */

	// 分配次数是否到了中断次数
	m_assert(pMemMgr->m_totalMemoryAllocations != pMemMgr->m_breakOnAllocationCount);

	// 检查分配类型
	m_assert(type != MemAllocType::MAT_Unknown);

	//如果是重分配
	if (type == MemAllocType::MAT_Realloc) {
		//先移除之前分配的内存节点
		MemoryNode *memory = pMemMgr->removeMemoryNode(address);

		m_assert(memory);
		if (!memory) {
			pMemMgr->log("Request to reallocate RAM that was never allocated.");
		}

		// 验证分配类型，只有malloc/calloc/realloc分配的节点才能重分配
		m_assert(memory->allocationType == MemAllocType::MAT_Malloc ||
			memory->allocationType == MemAllocType::MAT_Calloc ||
			memory->allocationType == MemAllocType::MAT_Realloc);

		//是否在重分配是中断
		m_assert((memory->options & BREAK_ON_REALLOC) == 0x0);

		memory->actualSize = size + pMemMgr->m_paddingSize * sizeof(long) * 2;
		memory->reportedSize = size;
		//重分配内存
		memory->actualAddress = realloc(memory->actualAddress, memory->actualSize);
	}
	else {
		// 创建一个新的内存块
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

	// 验证分配
	m_assert(memory->actualAddress);
	if (!memory->actualAddress) {
		pMemMgr->log("Request for allocation failed.  Out of memory.");
	}

	// 初始化分配的内容，方便跟踪释放
	if (type == MemAllocType::MAT_Calloc)
		memory->InitializeMemory(0x00000000);
	else
		memory->InitializeMemory(0xBAADC0DE);


	// 插入MemoryNode到hashlist
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

	// 移除内存节点
	MemoryNode *memory = pMemMgr->removeMemoryNode(address);


	if (!memory) {
		if (address != NULL)
		{
			free(address);
		}
		return;
	}

	// 如果有日志信息
	if (pMemMgr->m_logAlways && info != NULL) {
		pMemMgr->log("Memory Deallocation      : %-40s %8s(0x%08p) : %s",
			formatOwnerString(info->fileName, info->lineNumber),
			s_allocationTypes[type], address, memorySizeString(memory->reportedSize));
	}



	m_assert(type != MAT_Unknown);

	// 校验内存错误
	pMemMgr->validateMemoryUnit(memory);

	// 检验正确的释放方式
	m_assert(type == MAT_Delete       && memory->allocationType == MAT_New ||
		type == MAT_DeleteArray && memory->allocationType == MAT_NewArray ||
		type == MAT_Free         && memory->allocationType == MAT_Malloc ||
		type == MAT_Free         && memory->allocationType == MAT_Calloc ||
		type == MAT_Free         && memory->allocationType == MAT_Realloc);

	// 是否是在释放的时候中断
	m_assert((memory->options & BREAK_ON_DEALLOC) == 0x0);

	//释放内存
	free(memory->actualAddress);

	// 释放内存节点
	pMemMgr->deallocateMemory(memory);

	//释放记录保存文件名和行号的信息
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
		// 有内存没有回收
		// 如果内存泄露，必须清理内存管理器分配的剩余内存
		if (m_numAllocations != 0) {
			for (int ii = 0; ii < HASH_SIZE; ++ii) {
				while (m_hashTable[ii]) {
					MemoryNode *ptr = m_hashTable[ii];
					m_hashTable[ii] = m_hashTable[ii]->next;
					free(ptr->actualAddress);      // 释放剩余泄露的内存
					free(ptr);                     // 释放创建的MemoryNode
				}
			}
		}

		// 清理记录入口栈
		while (!m_topStack.empty()) {
			StackNode* pNode = m_topStack.pop();
			if (pNode != NULL)
			{
				free(pNode);
				pNode = NULL;
			}

		}

		// 清理内存缓存
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
		//通过节点的地址计算出hash表项
		int hashIndex = getHashIndex(node->reportedAddress);
		//尾插
		node->next = m_hashTable[hashIndex];
		node->prev = NULL;

		if (m_hashTable[hashIndex]) 
			m_hashTable[hashIndex]->prev = node;

		m_hashTable[hashIndex] = node;

		//收集信息
		m_numAllocations++;
		// 增加当前已分配的内存
		m_allocatedMemory += node->reportedSize;
		//如果已分配内存大于峰值内存，更新峰值内存
		if (m_allocatedMemory > m_peakMemoryAllocation)   
			m_peakMemoryAllocation = m_allocatedMemory;
		//已分配数大于峰值分配数，更新分配数峰值
		if (m_numAllocations > m_peakTotalNumAllocations) 
			m_peakTotalNumAllocations = m_numAllocations;

		//增加分配总数
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

			//减少分配数
			m_numAllocations--;
			//减少已分配内存
			m_allocatedMemory -= ptr->reportedSize;
		}
		return ptr;
	}

	bool MemoryManager::validateMemoryUnit(MemoryNode *node)
	{
		bool success = true;
		unsigned int ii;
		unsigned int totalBytesUsed = 0, boundViolations = 0;

		// 检测内存越界
		// BeginPadding和EndPadding地址
		long *beginning = (long*)node->actualAddress;
		long *ending = (long*)((char*)node->actualAddress + node->actualSize - node->paddingSize*sizeof(long));
		for (ii = 0; ii < node->paddingSize; ++ii) {
			if (beginning[ii] != PADDING || ending[ii] != PADDING) {
				success = false;  // 报告有越界
				//padding填充每个字节都应该等于PADDING
				boundViolations++;
			}
		}

		if (!success) m_numBoundsViolations++;

		//尝试检测实际实际分配内存
		// 初始化memory padding 检测越界
		long *lptr = (long*)node->reportedAddress;
		//以long为单位划分块数，//以long为单位填充predefineBody
		unsigned int len = node->reportedSize / sizeof(long);
		unsigned int cnt;  
		for (cnt = 0; cnt < len; ++cnt) {
			if (lptr[cnt] != node->predefinedBody)       
				totalBytesUsed += sizeof(long);
		}
		//判断不能被整除的余数部分
		char *cptr = (char*)(&lptr[cnt]);
		len = node->reportedSize - len*sizeof(long);
		for (cnt = 0; cnt < len; ++cnt) {
			if (cptr[cnt] != (char)node->predefinedBody) 
				totalBytesUsed++;
		}

		m_totalMemoryUsed += totalBytesUsed;

		//使用的字节不等于节点分配大小
		//有泄漏
		if (m_logAlways && totalBytesUsed != node->reportedSize) {          //报告百分比
			this->log("Unused Memory Detected   : %-40s %8s(0x%08p) : %s",   //  浪费掉的空间
				formatOwnerString(node->sourceFile, node->sourceLine),
				s_allocationTypes[node->allocationType], node->reportedAddress,
				memorySizeString(node->reportedSize - totalBytesUsed));
		}
		//有越界
		if (m_logAlways && !success) {                                      // 内存越界检测
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
			//没有缓存，创建新MemoryNode开销
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
			//取出一个缓存的MemoryNode
			MemoryNode *ptr = m_memoryCache;
			m_memoryCache = m_memoryCache->next;
			return ptr;
		}
	}

	void MemoryManager::dumpLogReport(void)
	{
		if (m_cleanLogFileOnFirstRun) {      // 清理日志
			_unlink(LOGFILE);                 //删除文件
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
		fprintf(fp, "*********           内存报告 for: %02d/%02d/%04d %02d:%02d:%02d            ********* \r\n", time->tm_mon + 1, time->tm_mday, time->tm_year + 1900, time->tm_hour, time->tm_min, time->tm_sec);
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

		if (m_numAllocations != 0) {  // 是否有内存泄漏
			fclose(fp);              
			dumpMemoryAllocations();    // 显示内存泄漏
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
		// 初始化内存，并检测越界
		long *beginning = (long*)actualAddress; //起始地址
		long *ending = (long*)((char*)actualAddress + actualSize - paddingSize*sizeof(long));  //结束地址（减一个padding大小，方便比较首位padding）
		for (unsigned short ii = 0; ii < paddingSize; ++ii) {
			beginning[ii] = ending[ii] = PADDING;
		}

		// 初始化内存
		beginning = (long*)reportedAddress;  //申请的地址
		unsigned int len = reportedSize / sizeof(long);    //以long为单位进行填充BODY
		unsigned int cnt;
		for (cnt = 0; cnt < len; ++cnt) {                         // 初始化主内存
			beginning[cnt] = body;
		}
		char *cptr = (char*)(&beginning[cnt]);  //余数部分
		len = reportedSize - len*sizeof(long);
		for (cnt = 0; cnt < len; ++cnt) {    // 初始化剩下内存
			cptr[cnt] = (char)body;
		}

		predefinedBody = body;
	}

}