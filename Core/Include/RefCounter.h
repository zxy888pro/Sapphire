#pragma once
#include "SapphireDef.h"

namespace Sapphire
{
	struct RefCount
	{
		RefCount() :
			refs_(0),
			weakRefs_(0)
		{
		}

		~RefCount()
		{
			refs_ = -1;
			weakRefs_ = -1;
		}

		int refs_;
		int weakRefs_;
	};

	class SAPPHIRE_API RefCounter
	{
	public:
		RefCounter();
		virtual ~RefCounter();

		void AddRef();
		void ReleaseRef();
		int Refs() const;
		int WeakRefs() const;

		RefCount* RefCountPtr() { return refCount_; }

	private:
		RefCounter(const RefCounter& rhs);

		RefCounter& operator =(const RefCounter& rhs);

		RefCount* refCount_;
	};

}