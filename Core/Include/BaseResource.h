#ifndef _BASE_RESOURCE_H_
#define _BASE_RESOURCE_H_

#include "SapphireDef.h"
#include <SubSystem.h>
#include "handle.h"


namespace Sapphire
{
#define RHANDLE uint
#define INVALID_RHANDLE 0xFFFFFFFF
#define IS_INVALID_RHANDLE(_rh)	((_rh == INVALID_RHANDLE) ? true : false)
#define IS_VALID_RHANDLE(_rh)	((_rh == INVALID_RHANDLE) ? false : true)


	enum ResoureType
	{
		ResourceType_Image,
		ResoureType_Texture,
		ResourceType_Model,
		ResourceType_Shader,
		ResourceType_XML,
		ResourceType_Unkown,
		ResourceType_Max
	};

	enum ResourceState
	{
		ResourceState_Unload,
		ResourceState_Loading,
		ResourceState_Loaded
	};

	//��Դ����
	class SAPPHIRE_CLASS  BaseResource : public BaseObject
	{
		SAPPHIRE_OBJECT(BaseResource, BaseObject)
	public:

		enum PriorityType
		{
			RES_LOW_PRIORITY = 0,
			RES_MED_PRIORITY,
			RES_HIGH_PRIORITY
		};
		/*BaseResource(const Core* pCore) :
			BaseObject(pCore)
			{
			m_resName = "";
			m_eType = ResourceType_Unkown;
			}*/
		BaseResource(Core* pCore,const char* name = ""):
			BaseObject(pCore)
		{ 
			Clear(); 
			m_resName = name;
			m_eType = ResourceType_Unkown;
			m_eState = ResourceState_Unload;
		}
		virtual ~BaseResource()	{ Destroy(); }

		virtual void Clear();
		virtual const std::string& GetName() const { return m_resName; }
		virtual bool Create()	{ return false; }
		virtual void Destroy()	{}  //��ʱ����,��Dispose
		
		virtual bool Load(){ return false; } //ֱ������Դ������
		virtual bool Load(const char* resPath) { m_resName = resPath; return Load(); }; //����ָ����Դ·��
		virtual bool Recreate() = 0;  //���´������൱�����¼��� (��ResourceLoader���������¼�����)
		virtual void Dispose() = 0;

		virtual void Save() {};  //���±�����Դ���ļ�

		ResourceState GetState() const { return m_eState; }
		void SetState(ResourceState state) { m_eState = state; }

		virtual void OnLoadStart() {}; //�첽��ʼ����ʱ�ص�
		virtual void OnLoadEnd() {};  //�첽�������ʱ�ص�
		virtual void OnLoadError() {}; //�첽����ʧ��ʱ����
		

		virtual size_t GetSize() = 0;
		virtual bool IsDisposed() = 0;

		ResoureType  GetType() const { return m_eType; }
        //��Դ���ȼ�
		inline void SetPriority(PriorityType priority)		{ m_Priority = priority; }
		inline PriorityType GetPriority()					{ return m_Priority; }

		//������
		inline void SetReferenceCount(uint nCount)			{ m_nRefCount = nCount; }
		inline uint GetReferenceCount()						{ return m_nRefCount; }
		
		inline bool IsLocked()								{ return (m_nRefCount > 0) ? true : false; }

		inline void SetLastAccess(time_t LastAccess)		{ m_LastAccess = LastAccess; }
		inline time_t GetLastAccess()						{ return m_LastAccess; }

		//����������Դ˳��
		virtual bool operator < (BaseResource& container);

	protected:

		PriorityType	m_Priority;
		uint			m_nRefCount;
		time_t			m_LastAccess;
		ResoureType     m_eType;
		std::string     m_resName;  //����ResourceCache, ��Դ·����Ϊ��Դ��
		volatile ResourceState   m_eState;  //��Դ����״̬

	};

	template <class T>
	class ptr_less
	{
	public:
		inline bool operator ()(T left, T right)
		{
			return ((*left) < (*right));
		}
	};


	template <class T>
	class ptr_greater
	{
	public:
		inline bool operator ()(T left, T right)
		{
			return !((*left) < (*right));
		}
	};

	typedef std::map<RHANDLE, BaseResource*> ResMap;
	typedef ResMap::iterator ResMapItor;
	typedef ResMap::value_type ResMapPair;


	//////////////////////////////////////////////////////////////////////////
	//��Դ�������
	class ResourceContainer : public BaseObject
	{
		SAPPHIRE_OBJECT(ResourceContainer, BaseObject)

	public:

		ResourceContainer(Core* pCore);
		virtual ~ResourceContainer()	{ Destroy(); }

		void Clear();

		bool Create(uint nMaxSize);
		void Destroy();

	   
		bool SetMaximumMemory(size_t nMem);  //������Դ�����������ռ���ڴ�����
		size_t GetMaximumMemory()		{ return m_nMaximumMemory; }  


		// --------------------------------------------------------------------------
		// ��Դ�ֵ����

		//ֱ�ӿ�dll����ResMap�ᵼ��ջָ����󣬱���ͨ����װ��ӷ��ʲ��ܱ�֤��ȫ
		//��λ����һ����Դ
		inline void GotoBegin()
		{
			m_CurrentResource = m_ResourceMap.begin();
		}
		//ȡ�õ�ǰ��Դ
		inline BaseResource* GetCurrentResource()
		{
			return (*m_CurrentResource).second;
		}
		//��һ����Դ
		inline bool GotoNext()
		{
			m_CurrentResource++;  return IsValid();
		}
		//��ǰ��Դ�ͷ���Ч
		inline bool IsValid()
		{
			return (m_CurrentResource != m_ResourceMap.end()) ? true : false;
		}


		// -----------------------------------------------------------------------
		// һ����Դ����

		// Ԥ�����ڴ�,�Ա������Դ��ֱ�ӿ���
		bool ReserveMemory(size_t nMem);
		//�����YԴ
		bool InsertResource(RHANDLE* rhUniqueID, BaseResource* pResource);
		bool InsertResource(RHANDLE rhUniqueID, BaseResource* pResource);

		bool RemoveResource(BaseResource* pResource);
		bool RemoveResource(RHANDLE rhUniqueID);

		// ������Դ���ͷ��ڴ�
		bool DestroyResource(BaseResource* pResource);
		bool DestroyResource(RHANDLE rhUniqueID);

		// ��ȡָ������Դ�� �����Դ�Ѿ������٣���ô���ؽ�һ��
		BaseResource* GetResource(RHANDLE rhUniqueID);


		// ������Դ����֤���ᱻ�Զ�����
		BaseResource* Lock(RHANDLE rhUniqueID);

		//������Դ
		int Unlock(RHANDLE rhUniqueID);
		int Unlock(BaseResource* pResource);

		// ͨ����Դָ���ȡ�洢���
		RHANDLE FindResourceHandle(BaseResource* pResource);

	protected:

		// �ڲ�����
		inline void AddMemory(size_t nMem)		{ m_nCurrentUsedMemory += nMem; }
		inline void RemoveMemory(size_t nMem)	{ m_nCurrentUsedMemory -= nMem; }
		//�@ȡ��һ�����õ��YԴ���,������r�������yһ����͵������a
		uint GetNextResHandle()					{ return --m_rhNextResHandle; }

		//������ͷ���Դ
		bool CheckForOverallocation();

	protected:
		RHANDLE			m_rhNextResHandle;
		size_t			m_nCurrentUsedMemory;	//��ǰ�ڴ�ռ��
		size_t			m_nMaximumMemory;		//����ڴ�����
		bool			m_bResourceReserved;   //�Ƿ�Ԥ�����ڴ�
		ResMapItor		m_CurrentResource;     //��ǰ��Դ������
		ResMap			m_ResourceMap;         //��Դ����
	};

}


#endif