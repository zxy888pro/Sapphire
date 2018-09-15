#pragma once
#include <SapphireDef.h>
#include <unordered_map>

namespace Sapphire
{
	template<class _Kty, class _Ty,  class _Hasher = hash<_Kty>, class _Keyeq = equal_to<_Kty>, class _Alloc = allocator<pair<const _Kty, _Ty> >>
	class SAPPHIRE_CLASS HashMap : public std::unordered_map < _Kty, _Ty, _Hasher, _Alloc >
	{
	public:
		typedef unordered_map<_Kty, _Ty, _Hasher, _Keyeq, _Alloc> _Myt;
		typedef _Uhash_compare<_Kty, _Hasher, _Keyeq> _Mytraits;
		typedef _Hash < _Umap_traits < _Kty, _Ty,
			_Mytraits, _Alloc, false > > _Mybase;
		typedef _Hasher hasher;
		typedef _Kty key_type;
		typedef _Ty mapped_type;
		typedef _Ty referent_type;	// extra
		typedef _Keyeq key_equal;
		typedef _Mytraits key_compare;	// extra

		HashMap() 
			: _Mybase(key_compare(), allocator_type())
		{};
		~HashMap(){};

		HashMap()
			: _Mybase(_Right,
			_Right._List._Getal().select_on_container_copy_construction())


		{	// construct map by copying _Right
		}

		HashMap(const _Myt& _Right, const allocator_type& _Al)
			: _Mybase(_Right, _Al)
		{	// construct map by copying _Right, allocator
		}

		explicit HashMap(size_type _Buckets)
			: _Mybase(key_compare(), allocator_type())
		{	// construct empty map from defaults, ignore initial size
			this->rehash(_Buckets);
		}

		HashMap(size_type _Buckets, const hasher& _Hasharg)
			: _Mybase(key_compare(_Hasharg), allocator_type())
		{	// construct empty map from hasher
			this->rehash(_Buckets);
		}

		HashMap(size_type _Buckets, const hasher& _Hasharg,
			const _Keyeq& _Keyeqarg)
			: _Mybase(key_compare(_Hasharg, _Keyeqarg), allocator_type())
		{	// construct empty map from hasher and equality comparator
			this->rehash(_Buckets);
		}

		HashMap(size_type _Buckets, const hasher& _Hasharg,
			const _Keyeq& _Keyeqarg, const allocator_type& _Al)
			: _Mybase(key_compare(_Hasharg, _Keyeqarg), _Al)
		{	// construct empty map from hasher and equality comparator
			this->rehash(_Buckets);
		}

	};
}