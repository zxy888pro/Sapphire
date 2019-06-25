#pragma once

#include <RefCounter.h>

#include <cassert>
#include <cstddef>


namespace Sapphire
{
	/// Shared pointer template class with intrusive reference counting.
	// ����ʽ���ü�����shared pointerģ����
	template <class T> class SharedPtr
	{
	public:
		/// Construct a null shared pointer.
		SharedPtr() :
			ptr_(0)
		{
		}

		/// Copy-construct from another shared pointer.
		//���ƹ��캯��
		SharedPtr(const SharedPtr<T>& rhs) :
			ptr_(rhs.ptr_)
		{
			AddRef();
		}

		/// Construct from a raw pointer.
		//��һ��ԭʼָ�빹��
		explicit SharedPtr(T* ptr) :
			ptr_(ptr)
		{
			AddRef();
		}

		/// Destruct. Release the object reference.
		~SharedPtr()
		{
			ReleaseRef();
		}

		/// Assign from another shared pointer.
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

		/// Assign from a raw pointer.
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

		/// Point to the object.
		// ָ��ԭʼ����
		T* operator ->() const
		{
			assert(ptr_);
			return ptr_;
		}

		/// Dereference the object.
		//������ö���
		T& operator *() const
		{
			assert(ptr_);
			return *ptr_;
		}

		/// Subscript the object if applicable.
		//����õĻ���ȡ������±�
		T& operator [](const int index)
		{
			assert(ptr_);
			return ptr_[index];
		}

		/// Test for less than with another shared pointer.
		//  shared pointer �Ƚ�
		bool operator <(const SharedPtr<T>& rhs) const { return ptr_ < rhs.ptr_; }

		/// Test for equality with another shared pointer.
		bool operator ==(const SharedPtr<T>& rhs) const { return ptr_ == rhs.ptr_; }

		/// Test for inequality with another shared pointer.
		bool operator !=(const SharedPtr<T>& rhs) const { return ptr_ != rhs.ptr_; }

		 
		//����ת������  �Զ�ת����ԭʼָ��
		operator T*() const { return ptr_; } 

		/// Reset to null and release the object reference.
		//  ���õ��ղ����ͷŶ�������
		void Reset() { ReleaseRef(); }

		/// Detach without destroying the object even if the refcount goes zero. To be used for scripting language interoperation.
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

		/// Perform a static cast from a shared pointer of another type.
		//������һ�����͵�shared pointer��̬ת��
		template <class U> void StaticCast(const SharedPtr<U>& rhs)
		{
			ReleaseRef();
			ptr_ = static_cast<T*>(rhs.Get());
			AddRef();
		}

		/// Perform a dynamic cast from a shared pointer of another type.
		//������һ�����͵�shared pointer��̬ת��
		template <class U> void DynamicCast(const SharedPtr<U>& rhs)
		{
			ReleaseRef();
			ptr_ = dynamic_cast<T*>(rhs.Get());
			AddRef();
		}

		/// Check if the pointer is null.
		//  ���ָ���Ƿ�Ϊ��
		bool Null() const { return ptr_ == 0; }

		/// Check if the pointer is not null.
		// ���ָ���Ƿ�Ϊ��
		bool NotNull() const { return ptr_ != 0; }

		/// Return the raw pointer.
		//  ����ԭʼָ��
		T* Get() const { return ptr_; }

		/// Return the object's reference count, or 0 if the pointer is null.
		//  ���ض�������ô���, ���Ϊ0ָ��Ϊ��
		int Refs() const { return ptr_ ? ptr_->Refs() : 0; }

		/// Return the object's weak reference count, or 0 if the pointer is null.
		//  ���ض����������
		int WeakRefs() const { return ptr_ ? ptr_->WeakRefs() : 0; }

		/// Return pointer to the RefCount structure.
		//  �������ü����ṹ��
		RefCount* RefCountPtr() const { return ptr_ ? ptr_->RefCountPtr() : 0; }

		/// Return hash value for HashSet & HashMap.
		// ���ض���HashSet��HashMap��hashֵ
		unsigned ToHash() const { return (unsigned)((size_t)ptr_ / sizeof(T)); }

	private:
		/// Prevent direct assignment from a shared pointer of another type.
		template <class U> SharedPtr<T>& operator =(const SharedPtr<U>& rhs);

		/// Add a reference to the object pointed to.
		void AddRef()
		{
			if (ptr_)
				ptr_->AddRef();
		}

		/// Release the object reference and delete it if necessary.
		//�ͷŶ�������ã������Ҫɾ������
		void ReleaseRef()
		{
			if (ptr_)
			{
				ptr_->ReleaseRef();
				ptr_ = 0;
			}
		}

		/// Pointer to the object.
		//�����ָ��
		T* ptr_;
	};

	/// Perform a static cast from one shared pointer type to another.
	//��̬ת��
	template <class T, class U> SharedPtr<T> StaticCast(const SharedPtr<U>& ptr)
	{
		SharedPtr<T> ret;
		ret.StaticCast(ptr);
		return ret;
	}

	/// Perform a dynamic cast from one weak pointer type to another.
	// ��̬ת��
	template <class T, class U> SharedPtr<T> DynamicCast(const SharedPtr<U>& ptr)
	{
		SharedPtr<T> ret;
		ret.DynamicCast(ptr);
		return ret;
	}

	/// Weak pointer template class with intrusive reference counting. Does not keep the object pointed to alive.
	// ����ʽ���ü�����������ָ��ģ���ࡣ  ���ܱ�֤����ָ����
	template <class T> class WeakPtr
	{
	public:
		/// Construct a null weak pointer.
		WeakPtr() :
			ptr_(0),
			refCount_(0)
		{
		}

		/// Copy-construct from another weak pointer.
		WeakPtr(const WeakPtr<T>& rhs) :
			ptr_(rhs.ptr_),
			refCount_(rhs.refCount_)
		{
			AddRef();
		}

		/// Construct from a shared pointer.
		WeakPtr(const SharedPtr<T>& rhs) :
			ptr_(rhs.Get()),
			refCount_(rhs.RefCountPtr())
		{
			AddRef();
		}

		/// Construct from a raw pointer.
		explicit WeakPtr(T* ptr) :
			ptr_(ptr),
			refCount_(ptr ? ptr->RefCountPtr() : 0)
		{
			AddRef();
		}

		/// Destruct. Release the weak reference to the object.
		~WeakPtr()
		{
			ReleaseRef();
		}

		/// Assign from a shared pointer.
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

		/// Assign from a weak pointer.
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

		/// Assign from a raw pointer.
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

		/// Convert to a shared pointer. If expired, return a null shared pointer.
		//  ת����һ��shared pointer�� ���ʧЧ�����ؿյ�shared pointer
		SharedPtr<T> Lock() const
		{
			if (Expired())
				return SharedPtr<T>();
			else
				return SharedPtr<T>(ptr_);
		}

		/// Return raw pointer. If expired, return null.
		//  ����ԭʼָ�룬���ʧЧ�����ؿ�
		T* Get() const
		{
			if (Expired())
				return 0;
			else
				return ptr_;
		}

		/// Point to the object.
		//  ָ��ԭʼָ�����
		T* operator ->() const
		{
			T* rawPtr = Get();
			assert(rawPtr);
			return rawPtr;
		}

		/// Dereference the object.
		//  ������ö���
		T& operator *() const
		{
			T* rawPtr = Get();
			assert(rawPtr);
			return *rawPtr;
		}

		/// Subscript the object if applicable.
		// ������Է��ʶ����±�
		T& operator [](const int index)
		{
			T* rawPtr = Get();
			assert(rawPtr);
			return (*rawPtr)[index];
		}

		/// Test for equality with another weak pointer.
		//  �Ƚ�������ָ���Ƿ���ͬ
		bool operator ==(const WeakPtr<T>& rhs) const { return ptr_ == rhs.ptr_ && refCount_ == rhs.refCount_; }

		/// Test for inequality with another weak pointer.
		//  �Ƚ�������ָ���Ƿ���ͬ
		bool operator !=(const WeakPtr<T>& rhs) const { return ptr_ != rhs.ptr_ || refCount_ != rhs.refCount_; }

		/// Test for less than with another weak pointer.
		bool operator <(const WeakPtr<T>& rhs) const { return ptr_ < rhs.ptr_; }

		/// Convert to a raw pointer, null if the object is expired.
		// ת����ԭʼָ�룬�����Ч�����ؿ�
		operator T*() const { return Get(); }

		/// Reset to null and release the weak reference.
		//  �������õ��ղ��ͷ�������
		void Reset() { ReleaseRef(); }

		/// Perform a static cast from a weak pointer of another type.
		/// ��һ����������Ӧ��һ����̬ת��
		template <class U> void StaticCast(const WeakPtr<U>& rhs)
		{
			ReleaseRef();
			ptr_ = static_cast<T*>(rhs.Get());
			refCount_ = rhs.refCount_;
			AddRef();
		}

		/// Perform a dynamic cast from a weak pointer of another type.
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

		/// Check if the pointer is null.
		//  ���ָ���Ƿ�Ϊ��
		bool Null() const { return refCount_ == 0; }

		/// Check if the pointer is not null.
		//  ���ָ���Ƿ�Ϊ��
		bool NotNull() const { return refCount_ != 0; }

		/// Return the object's reference count, or 0 if null pointer or if object has expired.
		//  ���ض�������������� ���Ϊ0�������Ѿ�ʧЧ
		int Refs() const { return (refCount_ && refCount_->refs_ >= 0) ? refCount_->refs_ : 0; }

		/// Return the object's weak reference count.
		//  ���ض��������������
		int WeakRefs() const
		{
			if (!Expired())
				return ptr_->WeakRefs();
			else
				return refCount_ ? refCount_->weakRefs_ : 0;
		}

		/// Return whether the object has expired. If null pointer, always return true.
		//  ���ض����Ƿ���Ч�� �������С��0����Ч
		bool Expired() const { return refCount_ ? refCount_->refs_ < 0 : true; }

		/// Return pointer to the RefCount structure.
		//  �������ü����ṹ��
		RefCount* RefCountPtr() const { return refCount_; }

		/// Return hash value for HashSet & HashMap.
		//  ����hashֵ
		unsigned ToHash() const { return (unsigned)((size_t)ptr_ / sizeof(T)); }

	private:
		/// Prevent direct assignment from a weak pointer of different type.
		template <class U> WeakPtr<T>& operator =(const WeakPtr<U>& rhs);

		/// Add a weak reference to the object pointed to.
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

		/// Release the weak reference. Delete the Refcount structure if necessary.
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

		/// Pointer to the object.
		// ԭʼ����
		T* ptr_;
		/// Pointer to the RefCount structure.
		//���ü�����ָ��
		RefCount* refCount_;
	};

	/// Perform a static cast from one weak pointer type to another.
	template <class T, class U> WeakPtr<T> StaticCast(const WeakPtr<U>& ptr)
	{
		WeakPtr<T> ret;
		ret.StaticCast(ptr);
		return ret;
	}

	/// Perform a dynamic cast from one weak pointer type to another.
	template <class T, class U> WeakPtr<T> DynamicCast(const WeakPtr<U>& ptr)
	{
		WeakPtr<T> ret;
		ret.DynamicCast(ptr);
		return ret;
	}
}