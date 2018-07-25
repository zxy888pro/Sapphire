#pragma once
#include <vector>
#include <cassert>


namespace Sapphire
{

	//Handle��
	template<typename TAG>
	class Handle
	{
		union
		{
			enum
			{
				//ʹ��λ��Ĵ�С
				MAX_BITS_INDEX = 16,
				MAX_BITS_MAGIC = 16,
				//�����ö�������Ҫ�ȽϵĴ�С
				MAX_INDEX = (1 << MAX_BITS_INDEX) - 1, //65535
				MAX_MAGIC = (1 << MAX_BITS_MAGIC) - 1 //65635

			};

			struct
			{
				uint m_Index : MAX_BITS_INDEX;  //�����Ӧ��Դ����Դ���������
				uint m_Magic : MAX_BITS_MAGIC;  //���ڼ�������ħ����

			};
			uint m_Handle;  //�����Ϊ��Դ��ʶ��
		};

	public:

		Handle(void):m_Handle(0){}

		void Init(unsigned int index);

		uint GetIndex(void) const { return m_Index;  }

		uint GetMagic(void) const { return m_Magic; }

		uint GetHandle(void) const { return m_Handle;}

		bool IsNull(void) const { return (!m_Handle); }

		operator uint (void) const { return m_Handle; }  //����ת���������ֱ��ת��uint��������ʾת��
	};

	template<typename TAG>
	void Sapphire::Handle<TAG>::Init(unsigned int index)
	{
		assert(IsNull());    //�����ظ�Init
		assert(index <= MAX_INDEX);

		static uint s_autoMagic = 0;   //����ħ����

		if (++s_autoMagic > MAX_MAGIC)  //����
		{
			s_autoMagic = 1;   //0���ڿվ��
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


	///��Handle������
	template <typename DATA, typename HANDLE>
	class HandleMgr
	{
	private:

		typedef std::vector<DATA>		UserVec;
		typedef std::vector<uint>		MagicVec;
		typedef std::vector<uint>		FreeVec;

		UserVec  m_UserData;     //ʵ����Դ
		MagicVec m_MagicNumbers;	//��Ӧ��ħ������
		FreeVec	 m_FreeSlots;       //�����б�

	public:

		HandleMgr(void){}
		~HandleMgr(void){}

		//�������
		
		//������
		DATA* Acquire(HANDLE&  handle);

		//�ͷ�
		void Release(HANDLE handle) ;

		//��ȡ��Դ����
		DATA* Dereference(HANDLE handle);

		const DATA* Dereference(HANDLE  handle) const;

		//��ȡ��ʹ�õľ����
		uint GetUsedHandleCount(void) const
		{
			return{ m_MagicNumbers.size() - m_FreeSlots.size(); }
		}
		//�Ƿ��п��þ��
		bool HasUsedHandles(void) const
		{
			return !!GetUsedHandleCount();
		}




	};

	//�ͷ�һ�����
	template <typename DATA, typename HANDLE>
	void Sapphire::HandleMgr<DATA, HANDLE>::Release(HANDLE handle) 
	{
		// ȡ�þ����Ӧ��Դ������
		unsigned int index = handle.GetIndex();

		// ��֤��Դ��Ч��
		assert(index < m_UserData.size());
		assert(m_MagicNumbers[index] == handle.GetMagic());

		//�Ƴ���,��ӵ������б�
		this->m_MagicNumbers[index] = 0;  //��Դ������Ӧ��ħ������ΪNULL
		this->m_FreeSlots.push_back(index);
	}

	///��ȡһ�����
	template <typename DATA, typename HANDLE>
	DATA* Sapphire::HandleMgr<DATA, HANDLE>::Acquire(HANDLE& handle)
	{
		//����
		unsigned index;
		if (m_FreeSlots.empty())
		{
			//ÿһ���ѷ������������Ӧ��ħ����
			//ȡ��һ��������
			index = m_MagicNumbers.size();
			handle.Init(index);
			//���һ��DATA��Դ
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

		//�������Ч�ԣ� 
		uint index = handle.GetIndex();
		if ((index >= m_UserData.size())
			|| (m_MagicNumbers[index] != handle.GetMagic()))
		{
			//��Ч���
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