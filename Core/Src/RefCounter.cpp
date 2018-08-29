#include "RefCounter.h"
#include <assert.h>

namespace Sapphire
{
	RefCounter::RefCounter() :
		refCount_(new RefCount())
	{
		(refCount_->weakRefs_)++;
	}

	RefCounter::~RefCounter()
	{
		assert(refCount_);
		assert(refCount_->refs_ == 0);
		assert(refCount_->weakRefs_ > 0);

		refCount_->refs_ = -1;
		(refCount_->weakRefs_)--;
		if (!refCount_->weakRefs_)
			delete refCount_;

		refCount_ = 0;
	}

	void RefCounter::AddRef()
	{
		assert(refCount_->refs_ >= 0);
		(refCount_->refs_)++;
	}

	void RefCounter::ReleaseRef()
	{
		assert(refCount_->refs_ > 0);
		(refCount_->refs_)--;
		if (!refCount_->refs_)
			delete this;
	}

	int RefCounter::Refs() const
	{
		return refCount_->refs_;
	}

	int RefCounter::WeakRefs() const
	{
		return refCount_->weakRefs_ - 1;
	}
}