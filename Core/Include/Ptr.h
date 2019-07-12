#pragma once

#include <RefCounter.h>

#include <cassert>
#include <cstddef>


namespace Sapphire
{

	// ����ʽ���ü�����shared pointerģ����
	template <class T> class SharedPtr
	{
	public:

		SharedPtr() :
			ptr_(0)
		{
		}

		//���ƹ��캯��
		SharedPtr(const SharedPtr<T>& rhs) :
			ptr_(rhs.ptr_)
		{
			AddRef();
		}


		//��һ��ԭʼָ�빹��
		explicit SharedPtr(T* ptr) :
			ptr_(ptr)
		{
			AddRef();
		}


		~SharedPtr()
		{
			ReleaseRef();
		}


		//������һ������ָ�����
		SharedPtr<T>& operator =(const SharedPtr<T>& rhs)
		{
			if (ptr_ == rhs.ptr_)
				return *this;
			//�ͷ�ԭ���������
			ReleaseRef();
			ptr_ = rhs.ptr_;
			//����¶��������
			AddRef();

			return *this;
		}


		//��һ��ԭָ�����
		SharedPtr<T>& operator =(T* ptr)
		{
			if (ptr_ == ptr)
				return *this;

			ReleaseRef();
			ptr_ = ptr;
			AddRef();

			return *this;
		}

		// ָ��ԭʼ����
		T* operator ->() const
		{
			assert(ptr_);
			return ptr_;
		}


		//������ö���
		T& operator *() const
		{
			assert(ptr_);
			return *ptr_;
		}


		//����õĻ���ȡ������±�
		T& operator [](const int index)
		{
			assert(ptr_);
			return ptr_[index];
		}


		//  shared pointer �Ƚ�
		bool operator <(const SharedPtr<T>& rhs) const { return ptr_ < rhs.ptr_; }


		bool operator ==(const SharedPtr<T>& rhs) const { return ptr_ == rhs.ptr_; }


		bool operator !=(const SharedPtr<T>& rhs) const { return ptr_ != rhs.ptr_; }

		 
		//����ת������  �Զ�ת����ԭʼָ��
		operator T*() const { return ptr_; } 


		//  ���õ��ղ����ͷŶ�������
		void Reset() { ReleaseRef(); }


		//  ������ü�����0���룬�������ٶ���  ���ڽű����ԵĽ���
		void Detach()
		{
			if (ptr_)
			{
				RefCount* refCount = RefCountPtr();
				++refCount->refs_; // 2 refs  +1
				Reset(); // 1 ref   -1
				--refCount->refs_; // 0 refs  -1
			}
		}


		//������һ�����͵�shared pointer��̬ת��
		template <class U> void StaticCast(const SharedPtr<U>& rhs)
		{
			ReleaseRef();
			ptr_ = static_cast<T*>(rhs.Get());
			AddRef();
		}

		//������һ�����͵�shared pointer��̬ת��
		template <class U> void DynamicCast(const SharedPtr<U>& rhs)
		{
			ReleaseRef();
			ptr_ = dynamic_cast<T*>(rhs.Get());
			AddRef();
		}


		//  ���ָ���Ƿ�Ϊ��
		bool Null() const { return ptr_ == 0; }


		// ���ָ���Ƿ�Ϊ��
		bool NotNull() const { return ptr_ != 0; }


		//  ����ԭʼָ��
		T* Get() const { return ptr_; }


		//  ���ض�������ô���, ���Ϊ0ָ��Ϊ��
		int Refs() const { return ptr_ ? ptr_->Refs() : 0; }

		//  ���ض����������
		int WeakRefs() const { return ptr_ ? ptr_->WeakRefs() : 0; }


		//  �������ü����ṹ��
		RefCount* RefCountPtr() const { return ptr_ ? ptr_->RefCountPtr() : 0; }


		// ���ض���HashSet��HashMap��hashֵ
		unsigned ToHash() const { return (unsigned)((size_t)ptr_ / sizeof(T)); }

	private:

		template <class U> SharedPtr<T>& operator =(const SharedPtr<U>& rhs);

		void AddRef()
		{
			if (ptr_)
				ptr_->AddRef();
		}


		//�ͷŶ�������ã������Ҫɾ������
		void ReleaseRef()
		{
			if (ptr_)
			{
				ptr_->ReleaseRef();
				ptr_ = 0;
			}
		}


		//�����ָ��
		T* ptr_;
	};

	//��̬ת��
	template <class T, class U> SharedPtr<T> StaticCast(const SharedPtr<U>& ptr)
	{
		SharedPtr<T> ret;
		ret.StaticCast(ptr);
		return ret;
	}


	// ��̬ת��
	template <class T, class U> SharedPtr<T> DynamicCast(const SharedPtr<U>& ptr)
	{
		SharedPtr<T> ret;
		ret.DynamicCast(ptr);
		return ret;
	}

	// ����ʽ���ü�����������ָ��ģ���ࡣ  ���ܱ�֤����ָ����
	template <class T> class WeakPtr
	{
	public:
		
		WeakPtr() :
			ptr_(0),
			refCount_(0)
		{
		}

		
		WeakPtr(const WeakPtr<T>& rhs) :
			ptr_(rhs.ptr_),
			refCount_(rhs.refCount_)
		{
			AddRef();
		}

		
		WeakPtr(const SharedPtr<T>& rhs) :
			ptr_(rhs.Get()),
			refCount_(rhs.RefCountPtr())
		{
			AddRef();
		}

		
		explicit WeakPtr(T* ptr) :
			ptr_(ptr),
			refCount_(ptr ? ptr->RefCountPtr() : 0)
		{
			AddRef();
		}

		
		~WeakPtr()
		{
			ReleaseRef();
		}

		
		//  ��shared pointer ����
		WeakPtr<T>& operator =(const SharedPtr<T>& rhs)
		{
			//�ж��ǲ���ͬһ������
			if (ptr_ == rhs.Get() && refCount_ == rhs.RefCountPtr())
				return *this;

			ReleaseRef();
			ptr_ = rhs.Get();
			refCount_ = rhs.RefCountPtr();
			AddRef();

			return *this;
		}

		
		// ��һ��������ָ�����
		WeakPtr<T>& operator =(const WeakPtr<T>& rhs)
		{
			if (ptr_ == rhs.ptr_ && refCount_ == rhs.refCount_)
				return *this;

			ReleaseRef();
			ptr_ = rhs.ptr_;
			refCount_ = rhs.refCount_;
			AddRef();

			return *this;
		}

		
		//  ��һ��ԭʼָ�����
		WeakPtr<T>& operator =(T* ptr)
		{
			RefCount* refCount = ptr ? ptr->RefCountPtr() : 0;

			if (ptr_ == ptr && refCount_ == refCount)
				return *this;

			ReleaseRef();
			ptr_ = ptr;
			refCount_ = refCount;
			AddRef();

			return *this;
		}

		
		//  ת����һ��shared pointer�� ���ʧЧ�����ؿյ�shared pointer
		SharedPtr<T> Lock() const
		{
			if (Expired())
				return SharedPtr<T>();
			else
				return SharedPtr<T>(ptr_);
		}

		
		//  ����ԭʼָ�룬���ʧЧ�����ؿ�
		T* Get() const
		{
			if (Expired())
				return 0;
			else
				return ptr_;
		}

		
		//  ָ��ԭʼָ�����
		T* operator ->() const
		{
			T* rawPtr = Get();
			assert(rawPtr);
			return rawPtr;
		}

		
		//  ������ö���
		T& operator *() const
		{
			T* rawPtr = Get();
			assert(rawPtr);
			return *rawPtr;
		}

		
		// ������Է��ʶ����±�
		T& operator [](const int index)
		{
			T* rawPtr = Get();
			assert(rawPtr);
			return (*rawPtr)[index];
		}

		
		//  �Ƚ�������ָ���Ƿ���ͬ
		bool operator ==(const WeakPtr<T>& rhs) const { return ptr_ == rhs.ptr_ && refCount_ == rhs.refCount_; }

		
		//  �Ƚ�������ָ���Ƿ���ͬ
		bool operator !=(const WeakPtr<T>& rhs) const { return ptr_ != rhs.ptr_ || refCount_ != rhs.refCount_; }

		
		bool operator <(const WeakPtr<T>& rhs) const { return ptr_ < rhs.ptr_; }

		
		// ת����ԭʼָ�룬�����Ч�����ؿ�
		operator T*() const { return Get(); }

		
		//  �������õ��ղ��ͷ�������
		void Reset() { ReleaseRef(); }

		
		/// ��һ����������Ӧ��һ����̬ת��
		template <class U> void StaticCast(const WeakPtr<U>& rhs)
		{
			ReleaseRef();
			ptr_ = static_cast<T*>(rhs.Get());
			refCount_ = rhs.refCount_;
			AddRef();
		}

		
		/// ��һ����������Ӧ��һ����̬ת��
		template <class U> void DynamicCast(const WeakPtr<U>& rhs)
		{
			ReleaseRef();
			ptr_ = dynamic_cast<T*>(rhs.Get());

			if (ptr_)
			{
				refCount_ = rhs.refCount_;
				AddRef();
			}
			else
				refCount_ = 0;
		}

		
		//  ���ָ���Ƿ�Ϊ��
		bool Null() const { return refCount_ == 0; }

		
		//  ���ָ���Ƿ�Ϊ��
		bool NotNull() const { return refCount_ != 0; }

		
		//  ���ض�������������� ���Ϊ0�������Ѿ�ʧЧ
		int Refs() const { return (refCount_ && refCount_->refs_ >= 0) ? refCount_->refs_ : 0; }

		
		//  ���ض��������������
		int WeakRefs() const
		{
			if (!Expired())
				return ptr_->WeakRefs();
			else
				return refCount_ ? refCount_->weakRefs_ : 0;
		}

		
		//  ���ض����Ƿ���Ч�� �������С��0����Ч
		bool Expired() const { return refCount_ ? refCount_->refs_ < 0 : true; }

		
		//  �������ü����ṹ��
		RefCount* RefCountPtr() const { return refCount_; }

		
		//  ����hashֵ
		unsigned ToHash() const { return (unsigned)((size_t)ptr_ / sizeof(T)); }

	private:
		
		template <class U> WeakPtr<T>& operator =(const WeakPtr<U>& rhs);

		
		//  Ϊ�������һ��������
		void AddRef()
		{
			if (refCount_)
			{
				//���������ü���
				assert(refCount_->weakRefs_ >= 0);
				++(refCount_->weakRefs_);
			}
		}

		
		// �ͷ�һ�������ã�  ���Ҫɾ�����ýṹ��
		void ReleaseRef()
		{
			if (refCount_)
			{
				//ֻ�������ö��������ü���
				assert(refCount_->weakRefs_ > 0);
				--(refCount_->weakRefs_);
				//�����ü���ʧЧ���ͷ����ü���ָ��
				if (Expired() && !refCount_->weakRefs_)
					delete refCount_;
			}

			ptr_ = 0;
			refCount_ = 0;
		}

		
		// ԭʼ����
		T* ptr_;
		
		//���ü�����ָ��
		RefCount* refCount_;
	};

	
	template <class T, class U> WeakPtr<T> StaticCast(const WeakPtr<U>& ptr)
	{
		WeakPtr<T> ret;
		ret.StaticCast(ptr);
		return ret;
	}

	
	template <class T, class U> WeakPtr<T> DynamicCast(const WeakPtr<U>& ptr)
	{
		WeakPtr<T> ret;
		ret.DynamicCast(ptr);
		return ret;
	}
}