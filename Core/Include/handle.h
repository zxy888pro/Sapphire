#pragma once
#include <vector>
#include <cassert>


namespace Sapphire
{

	//Handle类
	template<typename TAG>
	class Handle
	{
		union
		{
			enum
			{
				//使用位域的大小
				MAX_BITS_INDEX = 16,
				MAX_BITS_MAGIC = 16,
				//解引用断言所需要比较的大小
				MAX_INDEX = (1 << MAX_BITS_INDEX) - 1, //65535
				MAX_MAGIC = (1 << MAX_BITS_MAGIC) - 1 //65635

			};

			struct
			{
				uint m_Index : MAX_BITS_INDEX;  //句柄对应资源在资源数组的索引
				uint m_Magic : MAX_BITS_MAGIC;  //用于检验句柄的魔术数

			};
			uint m_Handle;  //句柄作为资源标识符
		};

	public:

		Handle(void):m_Handle(0){}

		void Init(unsigned int index);

		uint GetIndex(void) const { return m_Index;  }

		uint GetMagic(void) const { return m_Magic; }

		uint GetHandle(void) const { return m_Handle;}

		bool IsNull(void) const { return (!m_Handle); }

		operator uint (void) const { return m_Handle; }  //类型转换运算符，直接转换uint，不用显示转换
	};

	template<typename TAG>
	void Sapphire::Handle<TAG>::Init(unsigned int index)
	{
		assert(IsNull());    //不能重复Init
		assert(index <= MAX_INDEX);

		static uint s_autoMagic = 0;   //自增魔法术

		if (++s_autoMagic > MAX_MAGIC)  //回绕
		{
			s_autoMagic = 1;   //0用于空句柄
		}

		this->m_Index = index;
		m_Magic = s_autoMagic;
	}

	template <typename TAG>
	inline bool operator != (Handle<TAG> l, Handle<TAG> r)
	{
		return l.GetHandle() != r.GetHandle();
	}

	template <typename TAG>
	inline bool operator == (Handle<TAG> l, Handle<TAG> r)
	{
		return l.GetHandle() != r.GetHandle();
	}


	///简单Handle管理器
	template <typename DATA, typename HANDLE>
	class HandleMgr
	{
	private:

		typedef std::vector<DATA>		UserVec;
		typedef std::vector<uint>		MagicVec;
		typedef std::vector<uint>		FreeVec;

		UserVec  m_UserData;     //实际资源
		MagicVec m_MagicNumbers;	//对应的魔法号码
		FreeVec	 m_FreeSlots;       //空闲列表

	public:

		HandleMgr(void){}
		~HandleMgr(void){}

		//句柄方法
		
		//分配句柄
		DATA* Acquire(HANDLE&  handle);

		//释放
		void Release(HANDLE handle) ;

		//获取资源引用
		DATA* Dereference(HANDLE handle);

		const DATA* Dereference(HANDLE  handle) const;

		//获取已使用的句柄数
		uint GetUsedHandleCount(void) const
		{
			return{ m_MagicNumbers.size() - m_FreeSlots.size(); }
		}
		//是否还有可用句柄
		bool HasUsedHandles(void) const
		{
			return !!GetUsedHandleCount();
		}




	};

	//释放一个句柄
	template <typename DATA, typename HANDLE>
	void Sapphire::HandleMgr<DATA, HANDLE>::Release(HANDLE handle) 
	{
		// 取得句柄对应资源的索引
		unsigned int index = handle.GetIndex();

		// 验证资源有效性
		assert(index < m_UserData.size());
		assert(m_MagicNumbers[index] == handle.GetMagic());

		//移除它,添加到空闲列表
		this->m_MagicNumbers[index] = 0;  //资源索引对应的魔法数置为NULL
		this->m_FreeSlots.push_back(index);
	}

	///获取一个句柄
	template <typename DATA, typename HANDLE>
	DATA* Sapphire::HandleMgr<DATA, HANDLE>::Acquire(HANDLE& handle)
	{
		//索引
		unsigned index;
		if (m_FreeSlots.empty())
		{
			//每一个已分配的索引都对应着魔法数
			//取得一个索引号
			index = m_MagicNumbers.size();
			handle.Init(index);
			//添加一个DATA资源
			m_UserData.push_back(DATA());
			m_MagicNumbers.push_back(handle.GetMagic());
		}
		else
		{
			index = m_FreeSlots.back();
			handle.Init(index);
			m_FreeSlots.pop_back();
			m_MagicNumbers[index] = handle.GetMagic();
		}
		return &(m_UserData[index]);

	}


	template <typename DATA, typename HANDLE>
	inline DATA* HandleMgr <DATA, HANDLE>
		::Dereference(HANDLE handle)
	{
		if (handle.IsNull())  return (0);

		//检查句柄有效性， 
		uint index = handle.GetIndex();
		if ((index >= m_UserData.size())
			|| (m_MagicNumbers[index] != handle.GetMagic()))
		{
			//无效句柄
			assert(0);
			return (0);
		}

		return &(m_UserData[index]);
	}

	template <typename DATA, typename HANDLE>
	inline const DATA* HandleMgr <DATA, HANDLE>
		::Dereference(HANDLE handle) const
	{
		typedef HandleMgr <DATA, HANDLE> ThisType;
		return (const_cast <ThisType*> (this)->Dereference(handle));
	}
}