// bslstl_unorderedmultiset.h                                         -*-C++-*-
#ifndef INCLUDED_BSLSTL_UNORDEREDMULTISET
#define INCLUDED_BSLSTL_UNORDEREDMULTISET

#ifndef INCLUDED_BSLS_IDENT
#include <bsls_ident.h>
#endif
BSLS_IDENT("$Id: $")

//@PURPOSE: Provide an STL-compliant unordered_multiset class.
//
//@CLASSES:
//   bsl::unordered_multiset : hashed-map container
//
//@SEE_ALSO: bsl+stdhdrs
//
//@AUTHOR: Alisdair Meredith (ameredith1), Stefano Pacifico (spacifico1)
//
//@DESCRIPTION: This component defines a single class template
// 'unordered_multiset', implementing the standard container holding a
// collection of multiple keys with no guarantees on ordering.
//
// An instantiation of 'unordered_multiset' is an allocator-aware,
// value-semantic type whose salient attributes are its size (number of keys)
// and the set of keys the 'unordered_multiset' contains, without regard to
// their order.  If 'unordered_multiset' is instantiated with a key type that
// is not itself value-semantic, then it will not retain all of its
// value-semantic qualities.  In particular, if the key type cannot be tested
// for equality, then an 'unordered_multiset' containing that type cannot be
// tested for equality.  It is even possible to instantiate
// 'unordered_multiset' with a key type that does not have an accessible
// copy-constructor, in which case the 'unordered_multiset' will not be
// copyable.  Note that the equality operator 'operator==' for each element is
// used to determine when two 'unordered_multiset' objects have the same value,
// and not the equality comparator supplied at construction.
//
// An 'unordered_multiset' meets the requirements of an unordered associative
// container with forward iterators in the C++11 standard [unord].  The
// 'unordered_multiset' implemented here adheres to the C++11 standard, except
// that it does not have interfaces that take rvalue references,
// 'initializer_list', 'emplace', or operations taking a variadic number of
// template parameters.  Note that excluded C++11 features are those that
// require (or are greatly simplified by) C++11 compiler support.
//
///Requirements on 'KEY'
///--------------------- 
// An 'unordered_multiset' instantiation is a fully "Value-Semantic Type" (see
// {'bsldoc_glossary'}) only if the supplied 'KEY_TYPE' template parameters is
// fully value-semantic.  It is possible to instantiate an 'unordered_multiset'
// with 'KEY_TYPE' parameter arguments that do not provide a full set of
// value-semantic operations, but then some methods of the container may not be
// instantiable.  The following terminology, adopted from the C++11 standard,
// is used in the function documentation of 'unordered_multiset' to describe a
// function's requirements for the 'KEY' template parameter.  These terms are
// also defined in section [utility.arg.requirements] of the C++11 standard.
// Note that, in the context of an 'unordered_multiset' instantiation, the
// requirements apply specifically to the 'unordered_multiset's entry type,
// 'value_type', which is an alias for 'KEY_TYPE'.
//
//: "default-constructible": The type provides an accessible default
//:                          constructor.
//:
//: "copy-constructible": The type provides an accessible copy constructor.
//:
//: "equality-comparable": The type provides an equality-comparison operator
//:     that defines an equivalence relationship and is both reflexive and
//:     transitive.
//
///Memory Allocation
///-----------------
// The type supplied as a set's 'ALLOCATOR' template parameter determines how
// that set will allocate memory.  The 'unordered_multiset' template supports
// allocators meeting the requirements of the C++11 standard
// [allocator.requirements], and in addition it supports scoped-allocators
// derived from the 'bslma::Allocator' memory allocation protocol.  Clients
// intending to use 'bslma' style allocators should use the template's default
// 'ALLOCATOR' type: The default type for the 'ALLOCATOR' template parameter,
// 'bsl::allocator', provides a C++11 standard-compatible adapter for a
// 'bslma::Allocator' object.
//
///'bslma'-Style Allocators
/// - - - - - - - - - - - -
// If the parameterized 'ALLOCATOR' type of an 'unordered_multiset'
// instantiation is 'bsl::allocator', then objects of that set type will
// conform to the standard behavior of a 'bslma'-allocator-enabled type.  Such
// a set accepts an optional 'bslma::Allocator' argument at construction.  If
// the address of a 'bslma::Allocator' object is explicitly supplied at
// construction, it will be used to supply memory for the 'unordered_multiset'
// throughout its lifetime; otherwise, the 'unordered_multiset' will use the
// default allocator installed at the time of the 'unordered_multiset's
// construction (see 'bslma_default').  In addition to directly allocating
// memory from the indicated 'bslma::Allocator', an 'unordered_multiset'
// supplies that allocator's address to the constructors of contained objects
// of the parameterized 'KEY' types with the
// 'bslalg::TypeTraitUsesBslmaAllocator' trait.
//
///Operations
///----------
// This section describes the run-time complexity of operations on instances
// of 'unordered_multiset':
//..
//  Legend
//  ------
//  'K'             - parameterized 'KEY' type of the unordered multiset
//  'a', 'b'        - two distinct objects of type 'unordered_multiset<K>'
//  'n', 'm'        - number of elements in 'a' and 'b' respectively
//  'value_type'    - unoredered_multiset<K>::value_type
//  'c'             - comparator providing an ordering for objects of type 'K'
//  'al             - an STL-style memory allocator
//  'i1', 'i2'      - two iterators defining a sequence of 'value_type' objects
//  'k'             - an object of type 'K'
//  'v'             - an object of type 'value_type'
//  'p1', 'p2'      - two iterators belonging to 'a'
//  distance(i1,i2) - the number of elements in the range [i1, i2)
//  distance(p1,p2) - the number of elements in the range [p1, p2)
//
//  +----------------------------------------------------+--------------------+
//  | Operation                                          | Complexity         |
//  +====================================================+====================+
//  | unordered_multiset<K> a; (default construction)    | O[1]               |
//  | unordered_multiset<K> a(al);                       |                    |
//  | unordered_multiset<K> a(c, al);                    |                    |
//  +----------------------------------------------------+--------------------+
//  | unordered_multiset<K> a(b); (copy construction)    | Average: O[n]      |
//  | unordered_multiset<K> a(b, al);                    | Worst:   O[n^2]    |
//  +----------------------------------------------------+--------------------+
//  | unordered_multiset<K> a(n);                        | O[n]               |
//  | unordered_multiset<K> a(n, hf);                    |                    |
//  | unordered_multiset<K> a(n, hf, eq);                |                    |
//  | unordered_multiset<K> a(n, hf, eq, al);            |                    |
//  +----------------------------------------------------+--------------------+
//  | unordered_multiset<K> a(i1, i2);                   | Average: O[        |
//  | unordered_multiset<K> a(i1, i2, n)                 |   distance(i1, i2)]|
//  | unordered_multiset<K> a(i1, i2, n, hf);            | Worst:   O[n^2]    |
//  | unordered_multiset<K> a(i1, i2, n, hf, eq);        |                    |
//  | unordered_multiset<K> a(i1, i2, n, hf, eq, al);    |                    |
//  |                                                    |                    |
//  | a.~unordered_multiset<K>(); (destruction)          | O[n]               |
//  +----------------------------------------------------+--------------------+
//  | a = b;          (assignment)                       | Average: O[n]      |
//  |                                                    | Worst:   O[n^2]    |
//  +----------------------------------------------------+--------------------+
//  | a.begin(), a.end(), a.cbegin(), a.cend(),          | O[1]               |
//  +----------------------------------------------------+--------------------+
//  | a == b, a != b                                     | Best:  O[n]        |
//  |                                                    | Worst: O[n^2]      |
//  +----------------------------------------------------+--------------------+
//  | a.swap(b), swap(a,b)                               | O[1] if 'a' and    |
//  |                                                    | 'b' use the same   |
//  |                                                    | allocator,         |
//  |                                                    | O[n + m] otherwise |
//  +----------------------------------------------------+--------------------+
//  | a.key_eq()                                         | O[1]               |
//  +----------------------------------------------------+--------------------+
//  | a.hash_function()                                  | O[1]               |
//  +----------------------------------------------------+--------------------+
//  | a.size()                                           | O[1]               |
//  +----------------------------------------------------+--------------------+
//  | a.max_size()                                       | O[1]               |
//  +----------------------------------------------------+--------------------+
//  | a.empty()                                          | O[1]               |
//  +----------------------------------------------------+--------------------+
//  | get_allocator()                                    | O[1]               |
//  +----------------------------------------------------+--------------------+
//  | a[k]                                               | O[n]               |
//  +----------------------------------------------------+--------------------+
//  | a.at(k)                                            | O[n]               |
//  +----------------------------------------------------+--------------------+
//  | a.insert(value_type(k, v))                         | Average: O[1]      |
//  |                                                    | Worst:   O[n]      |
//  +----------------------------------------------------+--------------------+
//  | a.insert(p1, value_type(k, v))                     | Average: O[1]      |
//  |                                                    | Worst:   O[n]      |
//  +----------------------------------------------------+--------------------+
//  | a.insert(i1, i2)                                   | Average O[         |
//  |                                                    |   distance(i1, i2)]|
//  |                                                    | Worst:  O[ n *     |
//  |                                                    |   distance(i1, i2)]|
//  +----------------------------------------------------+--------------------+
//  | a.erase(p1)                                        | Average: O[1]      |
//  |                                                    | Worst:   O[n]      |
//  +----------------------------------------------------+--------------------+
//  | a.erase(k)                                         | Average: O[        |
//  |                                                    |         a.count(k)]|
//  |                                                    | Worst:   O[n]      |
//  +----------------------------------------------------+--------------------+
//  | a.erase(p1, p2)                                    | Average: O[        |
//  |                                                    |   distance(p1, p2)]|
//  |                                                    | Worst:   O[n]      |
//  +----------------------------------------------------+--------------------+
//  | a.clear()                                          | O[n]               |
//  +----------------------------------------------------+--------------------+
//  | a.find(k)                                          | Average: O[1]      |
//  |                                                    | Worst:   O[n]      |
//  +----------------------------------------------------+--------------------+
//  | a.count(k)                                         | Average: O[1]      |
//  |                                                    | Worst:   O[n]      |
//  +----------------------------------------------------+--------------------+
//  | a.equal_range(k)                                   | Average: O[        |
//  |                                                    |         a.count(k)]|
//  |                                                    | Worst:   O[n]      |
//  +----------------------------------------------------+--------------------+
//  | a.bucket_count()                                   | O[1]               |
//  +----------------------------------------------------+--------------------+
//  | a.max_bucket_count()                               | O[1]               |
//  +----------------------------------------------------+--------------------+
//  | a.bucket(k)                                        | O[1]               |
//  +----------------------------------------------------+--------------------+
//  | a.bucket_size(k)                                   | O[a.bucket_size(k)]|
//  +----------------------------------------------------+--------------------+
//  | a.load_factor()                                    | O[1]               |
//  +----------------------------------------------------+--------------------+
//  | a.max_load_factor()                                | O[1]               |
//  | a.max_load_factor(z)                               | O[1]               |
//  +----------------------------------------------------+--------------------+
//  | a.rehash(k)                                        | Average: O[n]      |
//  |                                                    | Worst:   O[n^2]    |
//  +----------------------------------------------------+--------------------+
//  | a.resize(k)                                        | Average: O[n]      |
//  |                                                    | Worst:   O[n^2]    |
//  +----------------------------------------------------+--------------------+
//..
//
///Usage
///-----

// Prevent 'bslstl' headers from being included directly in 'BSL_OVERRIDES_STD'
// mode.  Doing so is unsupported, and is likely to cause compilation errors.
#if defined(BSL_OVERRIDES_STD) && !defined(BSL_STDHDRS_PROLOGUE_IN_EFFECT)
#error "<bslstl_unorderedmultiset.h> header can't be included directly in \
BSL_OVERRIDES_STD mode"
#endif

#ifndef INCLUDED_BSLSCM_VERSION
#include <bslscm_version.h>
#endif

#ifndef INCLUDED_BSLSTL_ALLOCATOR
#include <bslstl_allocator.h>  // Can probably escape with a fwd-decl, but not
#endif                         // very user friendly

#ifndef INCLUDED_BSLSTL_ALLOCATORTRAITS
#include <bslstl_allocatortraits.h>
#endif

#ifndef INCLUDED_BSLSTL_EQUALTO
#include <bslstl_equalto.h>
#endif

#ifndef INCLUDED_BSLSTL_HASH
#include <bslstl_hash.h>
#endif

#ifndef INCLUDED_BSLSTL_HASHTABLE
#include <bslstl_hashtable.h>
#endif

#ifndef INCLUDED_BSLSTL_HASHTABLEBUCKETITERATOR
#include <bslstl_hashtablebucketiterator.h>
#endif

#ifndef INCLUDED_BSLSTL_HASHTABLEITERATOR
#include <bslstl_hashtableiterator.h>
#endif

#ifndef INCLUDED_BSLSTL_ITERATORUTIL
#include <bslstl_iteratorutil.h>
#endif

#ifndef INCLUDED_BSLSTL_PAIR
#include <bslstl_pair.h>  // result type of 'equal_range' method
#endif

#ifndef INCLUDED_BSLSTL_UNORDEREDSETKEYCONFIGURATION
#include <bslstl_unorderedsetkeyconfiguration.h>
#endif

#ifndef INCLUDED_BSLALG_BIDIRECTIONALLINK
#include <bslalg_bidirectionallink.h>
#endif

#ifndef INCLUDED_BSLALG_BIDIRECTIONALNODE
#include <bslalg_bidirectionalnode.h>
#endif

#ifndef INCLUDED_BSLALG_TYPETRAITHASSTLITERATORS
#include <bslalg_typetraithasstliterators.h>
#endif

#ifndef INCLUDED_BSLMA_USESBSLMAALLOCATOR
#include <bslma_usesbslmaallocator.h>
#endif

#ifndef INCLUDED_BSLMF_ISBITWISEMOVEABLE
#include <bslmf_isbitwisemoveable.h>
#endif

#ifndef INCLUDED_BSLMF_NESTEDTRAITDECLARATION
#include <bslmf_nestedtraitdeclaration.h>
#endif

#ifndef INCLUDED_CSTDDEF
#include <cstddef>  // for 'std::size_t'
#define INCLUDED_CSTDDEF
#endif

#ifndef INCLUDED_ITERATOR
#include <iterator>  // for 'std::iterator_traits'
#define INCLUDED_ITERATOR
#endif

namespace bsl {

template <class KEY_TYPE,
          class HASH  = bsl::hash<KEY_TYPE>,
          class EQUAL = bsl::equal_to<KEY_TYPE>,
          class ALLOC = bsl::allocator<KEY_TYPE> >
class unordered_multiset
{
    typedef bsl::allocator_traits<ALLOC>                       AllocatorTraits;
        // This typedef is an alias for the allocator traits type associated
        // with this container.

  public:
    // types
    typedef KEY_TYPE key_type;
    typedef KEY_TYPE value_type;
    typedef HASH     hasher;
    typedef EQUAL    key_equal;
    typedef ALLOC    allocator_type;

    typedef typename allocator_type::reference         reference;
    typedef typename allocator_type::const_reference   const_reference;

    typedef typename AllocatorTraits::size_type        size_type;
    typedef typename AllocatorTraits::difference_type  difference_type;
    typedef typename AllocatorTraits::pointer          pointer;
    typedef typename AllocatorTraits::const_pointer    const_pointer;

  private:
    typedef ::BloombergLP::bslalg::BidirectionalLink             HashTableLink;

    typedef ::BloombergLP::bslstl::UnorderedSetKeyConfiguration<value_type>
                                                                    ListPolicy;
    typedef ::BloombergLP::bslstl::HashTable<ListPolicy, HASH, EQUAL, ALLOC>
                                                                          Impl;

  public:
    // TRAITS
    BSLMF_NESTED_TRAIT_DECLARATION_IF(
                         unordered_multiset,
                         ::BloombergLP::bslmf::IsBitwiseMoveable,
                         ::BloombergLP::bslmf::IsBitwiseMoveable<Impl>::value);

    typedef ::BloombergLP::bslstl::HashTableIterator<value_type,
                                                     difference_type> iterator;
    typedef iterator                                            const_iterator;

    typedef ::BloombergLP::bslstl::HashTableBucketIterator<value_type,
                                                           difference_type>
                                                                local_iterator;
    typedef local_iterator                                const_local_iterator;

  private:
    // DATA
    Impl d_impl;

  public:
    // CREATORS
    explicit unordered_multiset(size_type n = 0,
                                const hasher& hf = hasher(),
                                const key_equal& eql = key_equal(),
                                const allocator_type& a = allocator_type());

    template <class InputIterator>
    unordered_multiset(InputIterator f, InputIterator l,
                       size_type n = 0,
                       const hasher& hf = hasher(),
                       const key_equal& eql = key_equal(),
                       const allocator_type& a = allocator_type());

    unordered_multiset(const unordered_multiset&);

    explicit unordered_multiset(const allocator_type&);

    unordered_multiset(const unordered_multiset&, const allocator_type&);

    ~unordered_multiset();
    unordered_multiset& operator=(const unordered_multiset&);

    allocator_type get_allocator() const;

    // size and capacity
    bool empty() const;
    size_type size() const;
    size_type max_size() const;

    // iterators
    iterator begin();
    const_iterator begin() const;
    iterator end();
    const_iterator end() const;
    const_iterator cbegin() const;
    const_iterator cend() const;

    // modifiers
    iterator insert(const value_type& obj);
    iterator insert(const_iterator hint, const value_type& obj);
    template <class InputIterator>
    void insert(InputIterator first, InputIterator last);

    iterator erase(const_iterator position);
    size_type erase(const key_type& k);
    iterator erase(const_iterator first, const_iterator last);

    void clear();

    void swap(unordered_multiset&);

    // observers
    hasher hash_function() const;
    key_equal key_eq() const;

    // lookup
    iterator find(const key_type& k);
    const_iterator find(const key_type& k) const;

    size_type count(const key_type& k) const;

    pair<iterator, iterator> equal_range(const key_type& k);
    pair<const_iterator, const_iterator> equal_range(const key_type& k) const;

    // bucket interface
    size_type bucket_count() const;
    size_type max_bucket_count() const;
    size_type bucket_size(size_type n) const;
    size_type bucket(const key_type& k) const;

    local_iterator       begin(size_type n);
    const_local_iterator begin(size_type n) const;
    local_iterator       end(size_type n);
    const_local_iterator end(size_type n) const;
    const_local_iterator cbegin(size_type n) const;
    const_local_iterator cend(size_type n) const;

    // hash policy
    float load_factor() const;
    float max_load_factor() const;
    void max_load_factor(float z);
    void rehash(size_type n);
    void reserve(size_type n);


    friend // must be defined inline, as no syntax to declare out-of-line
    bool operator==(const unordered_multiset& lhs,
                    const unordered_multiset& rhs) {
        return lhs.d_impl == rhs.d_impl;
    }

};

template <class KEY_TYPE, class HASH, class EQUAL, class ALLOC>
void swap(unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>& x,
          unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>& y);

template <class KEY_TYPE, class HASH, class EQUAL, class ALLOC>
bool operator!=(const unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>& lhs,
                const unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>& rhs);

}  // close namespace bsl

// ============================================================================
//                                TYPE TRAITS
// ============================================================================

// Type traits for STL *unordered* *associative* containers:
//: o An unordered associative container defines STL iterators.
//: o An unordered associative container is bitwise moveable if the both
//:      functors and the allocator are bitwise moveable.
//: o An unordered associative container uses 'bslma' allocators if the
//:      parameterized 'ALLOCATOR' is convertible from 'bslma::Allocator*'.

namespace BloombergLP {
namespace bslalg {

template <class KEY, class HASH, class EQUAL, class ALLOCATOR>
struct HasStlIterators<bsl::unordered_multiset<KEY, HASH, EQUAL, ALLOCATOR> >
     : bsl::true_type
{};

}  // close namespace bslalg

namespace bslma {

template <class KEY, class HASH, class EQUAL, class ALLOC>
struct UsesBslmaAllocator<bsl::unordered_multiset<KEY, HASH, EQUAL, ALLOC> >
    : bsl::is_convertible<Allocator*, ALLOC>::type
{};

}  // close namespace bslma
}  // close namespace BloombergLP

// ===========================================================================
//                  TEMPLATE AND INLINE FUNCTION DEFINITIONS
// ===========================================================================

namespace bsl
{

                        //-------------------------
                        // class unordered_multiset
                        //-------------------------

// CREATORS
template <class KEY_TYPE,
          class HASH,
          class EQUAL,
          class ALLOC>
unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>::
unordered_multiset(size_type        n,
                   const hasher&    hf,
                   const key_equal& eql,
                   const allocator_type& a)
: d_impl(hf, eql, n, a)
{
}

template <class KEY_TYPE,
          class HASH,
          class EQUAL,
          class ALLOC>
template <class InputIterator>
unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>::
unordered_multiset(InputIterator    first,
                   InputIterator    last,
                   size_type        n,
                   const hasher&    hf,
                   const key_equal& eql,
                   const allocator_type& a)
: d_impl(hf, eql, n, a)
{   // Could pick initial number of buckets based on length of [first,last).
    // Reduces number of rehashes, and no risk of unduly low load_factor on
    // a multi-container.
    this->insert(first, last);
}

template <class KEY_TYPE,
          class HASH,
          class EQUAL,
          class ALLOC>
unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>::
unordered_multiset(const allocator_type& a)
: d_impl(HASH(), EQUAL(), 0, a)
{
}

template <class KEY_TYPE,
          class HASH,
          class EQUAL,
          class ALLOC>
unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>::
unordered_multiset(const unordered_multiset& other, const allocator_type& a)
: d_impl(other.d_impl, a)
{
}

template <class KEY_TYPE,
          class HASH,
          class EQUAL,
          class ALLOC>
unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>::
unordered_multiset(const unordered_multiset& other)
: d_impl(other.d_impl,
         AllocatorTraits::select_on_container_copy_construction(
                                                        other.get_allocator()))
{
}

template <class KEY_TYPE,
          class HASH,
          class EQUAL,
          class ALLOC>
unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>::~unordered_multiset()
{
    // All memory management is handled by the base 'd_impl' member.
}

template <class KEY_TYPE,
          class HASH,
          class EQUAL,
          class ALLOC>
unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>&
unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>::operator=(
                                               const unordered_multiset& other)
{
    // Actually, need to check propagate_on_copy_assign trait
    unordered_multiset(other, get_allocator()).swap(*this);
    return *this;

}

template <class KEY_TYPE,
          class HASH,
          class EQUAL,
          class ALLOC>
ALLOC
unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>::get_allocator() const
{
    return d_impl.allocator();
}

    // size and capacity
template <class KEY_TYPE,
          class HASH,
          class EQUAL,
          class ALLOC>
bool
unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>::empty() const
{
    return 0 == d_impl.size();
}

template <class KEY_TYPE,
          class HASH,
          class EQUAL,
          class ALLOC>
typename unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>::size_type
unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>::size() const
{
    return d_impl.size();
}

template <class KEY_TYPE,
          class HASH,
          class EQUAL,
          class ALLOC>
typename unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>::size_type
unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>::max_size() const
{
    return d_impl.maxSize();
}

    // iterators
template <class KEY_TYPE,
          class HASH,
          class EQUAL,
          class ALLOC>
typename unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>::iterator
unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>::begin()
{
    return iterator(d_impl.elementListRoot());
}

template <class KEY_TYPE,
          class HASH,
          class EQUAL,
          class ALLOC>
typename unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>::const_iterator
unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>::begin() const
{
    return const_iterator(d_impl.elementListRoot());
}


template <class KEY_TYPE,
          class HASH,
          class EQUAL,
          class ALLOC>
typename unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>::iterator
unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>::end()
{
    return iterator();
}

template <class KEY_TYPE,
          class HASH,
          class EQUAL,
          class ALLOC>
typename unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>::const_iterator
unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>::end() const
{
    return const_iterator();
}

template <class KEY_TYPE,
          class HASH,
          class EQUAL,
          class ALLOC>
typename unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>::const_iterator
unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>::cbegin() const
{
    return const_iterator(d_impl.elementListRoot());
}

template <class KEY_TYPE,
          class HASH,
          class EQUAL,
          class ALLOC>
typename unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>::const_iterator
unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>::cend() const
{
    return const_iterator();
}

template <class KEY_TYPE,
          class HASH,
          class EQUAL,
          class ALLOC>
inline
typename unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>::iterator
unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>::insert(const value_type& obj)
{
    return iterator(d_impl.insert(obj));
}

template <class KEY_TYPE,
          class HASH,
          class EQUAL,
          class ALLOC>
typename unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>::iterator
unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>::
insert(const_iterator hint, const value_type& obj)
{
    return iterator(d_impl.insert(obj, hint.node()));
}

template <class KEY_TYPE,
          class HASH,
          class EQUAL,
          class ALLOC>
template <class InputIterator>
void
unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>::
insert(InputIterator first, InputIterator last)
{
    if (size_t maxInsertions =
            ::BloombergLP::bslstl::IteratorUtil::insertDistance(first, last)) {
        this->reserve(this->size() + maxInsertions);
    }

    // Typically will create an un-necessary temporary dereferencing each
    // iterator and casting to a reference of 'const value_type&'.
    while (first != last) {
        this->insert(*first++);
    }
}

template <class KEY_TYPE,
          class HASH,
          class EQUAL,
          class ALLOC>
typename unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>::iterator
unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>::erase(
                                                       const_iterator position)
{
    BSLS_ASSERT(position != this->end());
    return iterator(d_impl.remove(position.node()));
}

template <class KEY_TYPE,
          class HASH,
          class EQUAL,
          class ALLOC>
typename unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>::size_type
unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>::erase(const key_type& key)
{   // As an alternative implementation, the table could return an extracted
    // "slice" list from the underlying table, and now need merely:
    //   iterate each node, destroying the associated value
    //   reclaim each node (potentially returning to a node-pool)
    typedef ::BloombergLP::bslalg::BidirectionalNode<value_type> BNode;

    if (HashTableLink *target = d_impl.find(key)) {
        target = d_impl.remove(target);
        size_type result = 1;
        while (target && this->key_eq()(
              key,
              ListPolicy::extractKey(static_cast<BNode *>(target)->value()))) {
            target = d_impl.remove(target);
            ++result;
        }
        return result;
    }
    else {
        return 0;
    }
}

template <class KEY_TYPE,
          class HASH,
          class EQUAL,
          class ALLOC>
typename unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>::iterator
unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>::
erase(const_iterator first, const_iterator last)
{   // bad answer here, need to turn 'first' into a non-const iterator

    // 7 Most of the library's algorithmic templates that operate on data
    // structures have interfaces that use ranges.  A range is a pair of
    // iterators that designate the beginning and end of the computation.  A
    // range '[i,i)' is an empty range; in general, a range '[i,j)' refers to
    // the elements in the data structure starting with the element pointed to
    // by 'i' and up to but not including the element pointed to by 'j'.  Range
    // [i,j) is valid if and only if 'j' is reachable from 'i'.  The result of
    // the application of functions in the library to invalid ranges is
    // undefined.
#if defined BDE_BUILD_TARGET_SAFE2
    // Check that 'first' and 'last' are valid iterators referring to this
    // container.
    if (first == last) {
        iterator it = this->begin();
        while(it != first) {
            BSLS_ASSERT(it != this->end());
        }
        while(it != last) {
            BSLS_ASSERT(it != this->end());
        }
    }
#endif

    // more efficient to:
    // 1. unlink a set of nodes
    // 2. destroy their values
    // 3. reclaim their memory
    // merge steps 2/3 to avoid multiple list walks?
    // tricky issue of fixing up bucket indices as well

    // implementation must handle the case that 'last' is 'end', which will be
    // invalidated when the preceding element is erased.

    while (first != last) {
        first = this->erase(first);
    }

    return iterator(first.node()); // convert from const_iterator
}

template <class KEY_TYPE,
          class HASH,
          class EQUAL,
          class ALLOC>
void
unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>::clear()
{
    d_impl.removeAll();
}

template <class KEY_TYPE,
          class HASH,
          class EQUAL,
          class ALLOC>
void
unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>::
swap(unordered_multiset& other)
{
    // assert that allocators are compatible
    // TBD
    d_impl.swap(other.d_impl);
}

    // observers
template <class KEY_TYPE,
          class HASH,
          class EQUAL,
          class ALLOC>
typename unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>::hasher
unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>::hash_function() const
{
    return d_impl.hasher();
}

template <class KEY_TYPE,
          class HASH,
          class EQUAL,
          class ALLOC>
typename unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>::key_equal
unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>::key_eq() const
{
    return d_impl.comparator();
}


// lookup
template <class KEY_TYPE,
          class HASH,
          class EQUAL,
          class ALLOC>
typename unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>::iterator
unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>::find(const key_type& k)
{
    return iterator(d_impl.find(k));
}

template <class KEY_TYPE,
          class HASH,
          class EQUAL,
          class ALLOC>
typename unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>::const_iterator
unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>::find(const key_type& k) const
{
    return const_iterator(d_impl.find(k));
}

template <class KEY_TYPE,
          class HASH,
          class EQUAL,
          class ALLOC>
typename unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>::size_type
unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>::
count(const key_type& k) const
{
    typedef ::BloombergLP::bslalg::BidirectionalNode<value_type> BNode;

    size_type result = 0;
    for (HashTableLink *cursor = d_impl.find(k);
         cursor;
         ++result, cursor = cursor->nextLink()) {

        BNode *cursorNode = static_cast<BNode *>(cursor);
        if (!this->key_eq()(k, ListPolicy::extractKey(cursorNode->value()))) {
            break;
        }
    }
    return result;
}

template <class KEY_TYPE,
          class HASH,
          class EQUAL,
          class ALLOC>
inline
bsl::pair<typename unordered_multiset<KEY_TYPE,
                                     HASH,
                                     EQUAL,
                                     ALLOC>::iterator,
          typename unordered_multiset<KEY_TYPE,
                                     HASH,
                                     EQUAL,
                                     ALLOC>::iterator>
unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>::
equal_range(const key_type& k)
{
    HashTableLink *first;
    HashTableLink *last;
    d_impl.findRange(&first, &last, k);
    return bsl::pair<iterator, iterator>(iterator(first), iterator(last));
}

template <class KEY_TYPE,
          class HASH,
          class EQUAL,
          class ALLOC>
inline
bsl::pair<typename unordered_multiset<KEY_TYPE,
                                     HASH,
                                     EQUAL,
                                     ALLOC>::const_iterator,
          typename unordered_multiset<KEY_TYPE,
                                     HASH,
                                     EQUAL,
                                     ALLOC>::const_iterator>
unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>::
equal_range(const key_type& k) const
{
    HashTableLink *first;
    HashTableLink *last;
    d_impl.findRange(&first, &last, k);
    return bsl::pair<const_iterator, const_iterator>(const_iterator(first),
                                                     const_iterator(last));
}

    // bucket interface
template <class KEY_TYPE,
          class HASH,
          class EQUAL,
          class ALLOC>
inline
typename unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>::size_type
unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>::bucket_count() const
{
    return d_impl.numBuckets();
}

template <class KEY_TYPE,
          class HASH,
          class EQUAL,
          class ALLOC>
inline
typename unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>::size_type
unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>::max_bucket_count() const
{
    return d_impl.maxNumOfBuckets();
}

template <class KEY_TYPE,
          class HASH,
          class EQUAL,
          class ALLOC>
inline
typename unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>::size_type
unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>::
bucket_size(size_type n) const
{
    BSLS_ASSERT_SAFE(n < this->bucket_count());
    return d_impl.countElementsInBucket(n);
}

template <class KEY_TYPE,
          class HASH,
          class EQUAL,
          class ALLOC>
inline
typename unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>::size_type
unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>::
bucket(const key_type& k) const
{
    return d_impl.bucketIndexForKey(k);
}

template <class KEY_TYPE,
          class HASH,
          class EQUAL,
          class ALLOC>
inline
typename unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>::local_iterator
unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>::begin(size_type n)
{
    BSLS_ASSERT_SAFE(n < this->bucket_count());
    return local_iterator(&d_impl.bucketAtIndex(n));
}

template <class KEY_TYPE,
          class HASH,
          class EQUAL,
          class ALLOC>
inline
typename unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>::const_local_iterator
unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>::begin(size_type n) const
{
    BSLS_ASSERT_SAFE(n < this->bucket_count());
    return const_local_iterator(&d_impl.bucketAtIndex(n));
}

template <class KEY_TYPE,
          class HASH,
          class EQUAL,
          class ALLOC>
inline
typename unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>::local_iterator
unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>::end(size_type n)
{
    BSLS_ASSERT_SAFE(n < this->bucket_count());
    return local_iterator(0, &d_impl.bucketAtIndex(n));
}

template <class KEY_TYPE,
          class HASH,
          class EQUAL,
          class ALLOC>
inline
typename
 unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>::const_local_iterator
unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>::end(size_type n) const
{
    BSLS_ASSERT_SAFE(n < this->bucket_count());
    return const_local_iterator(0, &d_impl.bucketAtIndex(n));
}

template <class KEY_TYPE,
          class HASH,
          class EQUAL,
          class ALLOC>
inline
typename
 unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>::const_local_iterator
unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>::cbegin(size_type n) const
{
    BSLS_ASSERT_SAFE(n < this->bucket_count());
    //SP: invoke begin(n)?
    return const_local_iterator(&d_impl.bucketAtIndex(n));
}

template <class KEY_TYPE,
          class HASH,
          class EQUAL,
          class ALLOC>
inline
typename unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>::const_local_iterator
unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>::cend(size_type n) const
{
    BSLS_ASSERT_SAFE(n < this->bucket_count());
    //SP: invoke end(n)?
    return const_local_iterator(0, &d_impl.bucketAtIndex(n));
}

    // hash policy
template <class KEY_TYPE,
          class HASH,
          class EQUAL,
          class ALLOC>
inline
float unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>::load_factor() const
{
    return d_impl.loadFactor();
}

template <class KEY_TYPE,
          class HASH,
          class EQUAL,
          class ALLOC>
inline
float unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>::max_load_factor() const
{
    return d_impl.maxLoadFactor();
}

template <class KEY_TYPE,
          class HASH,
          class EQUAL,
          class ALLOC>
inline
void unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>::max_load_factor(float z)
{
    d_impl.maxLoadFactor(z);
}

template <class KEY_TYPE,
          class HASH,
          class EQUAL,
          class ALLOC>
inline
void
unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>::rehash(size_type n)
{
    return d_impl.rehash(n);
}

template <class KEY_TYPE,
          class HASH,
          class EQUAL,
          class ALLOC>
inline
void
unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>::reserve(size_type n)
{
    return d_impl.rehashForNumElements(n);
}

}  // close namespace bsl

//----------------------------------------------------------------------------
//                  free functions and operators
//----------------------------------------------------------------------------

template <class KEY_TYPE,
          class HASH,
          class EQUAL,
          class ALLOC>
inline
void
bsl::swap(bsl::unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>& x,
          bsl::unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>& y)
{
    x.swap(y);
}

template <class KEY_TYPE,
          class HASH,
          class EQUAL,
          class ALLOC>
inline
bool
bsl::operator!=(
              const bsl::unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>& lhs,
              const bsl::unordered_multiset<KEY_TYPE, HASH, EQUAL, ALLOC>& rhs)
{
    return !(lhs == rhs);
}

#endif

// ---------------------------------------------------------------------------
// NOTICE:
//      Copyright (C) Bloomberg L.P., 2012
//      All Rights Reserved.
//      Property of Bloomberg L.P. (BLP)
//      This software is made available solely pursuant to the
//      terms of a BLP license agreement which governs its use.
// ----------------------------- END-OF-FILE ---------------------------------
