// bdema_bufferedsequentialpool.h                                     -*-C++-*-
#ifndef INCLUDED_BDEMA_BUFFEREDSEQUENTIALPOOL
#define INCLUDED_BDEMA_BUFFEREDSEQUENTIALPOOL

#ifndef INCLUDED_BDES_IDENT
#include <bdes_ident.h>
#endif
BDES_IDENT("$Id: $")

//@PURPOSE: Provide sequential memory using an external buffer and a fallback.
//
//@CLASSES:
//  bdema_BufferedSequentialPool: pool using an external buffer and a fallback
//
//@SEE_ALSO: bdema_buffermanager, bdema_sequentialpool
//
//@AUTHOR: Arthur Chiu (achiu21)
//
//@DESCRIPTION: This component provides a maximally efficient sequential memory
// pool, 'bdema_BufferedSequentialPool', that dispenses heterogeneous memory
// blocks (of varying, user-specified sizes) from an external buffer.  If an
// allocation request exceeds the remaining free memory space in the external
// buffer, the pool will fall back to a sequence of dynamically allocated
// buffers.  Users can optionally specify a growth strategy at construction
// that governs the growth rate of the dynamically-allocated buffers.  If no
// growth strategy is specified at construction, geometric growth is used.
// Users can also optionally specify an alignment strategy at construction that
// governs the alignment of allocated memory blocks.  If no alignment strategy
// is specified, natural alignment is used.
//
// 'bdema_BufferedSequentialPool' is typically used when users have a
// reasonable estimation of the amount of memory needed.  This amount of memory
// would typically be created directly on the program stack, and used as the
// initial external buffer of the pool for fast memory allocation.  While the
// buffer has sufficient capacity, memory allocations using the pool will not
// trigger *any* dynamic memory allocation, will have optimal locality of
// reference, and will not require deallocation upon destruction.
//
// Once the external buffer is exhausted, subsequent allocation requests
// require dynamic memory allocation, and the performance of the pool degrades.
//
///Warning
///-------
// Note that, even when a buffer having 'n' bytes of memory is supplied at
// construction, it does *not* mean that 'n' bytes of memory are available
// before dynamic memory allocation is triggered.  This is due to memory
// alignment requirements.  If the buffer supplied is not aligned, the first
// call to the 'allocate' method will automatically skip one or more bytes such
// that the memory allocated is properly aligned.  The number of bytes that are
// wasted depends on whether natural alignment or maximum alignment is used
// (see 'bsls_alignment' for more details).
//
///Usage
///-----
///Example 1: Using 'bdema_BufferedSequentialPool' for Efficient Allocations
///- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Suppose we define a container class, 'my_BufferedIntDoubleArray', that holds
// both 'int' and 'double' values.  The class can be implemented using two
// parallel arrays: one storing the type information, and the other storing
// pointers to the 'int' and 'double' values.  Furthermore, if we can
// approximate the amount of memory needed, we can use a
// 'bdema_BufferedSequentialPool' for memory allocation for maximum efficiency:
//..
//  // my_bufferedintdoublearray.h
//
//  class my_BufferedIntDoubleArray {
//      // This class implements an efficient container for an array that
//      // stores both 'int' and 'double' values.
//
//      // DATA
//      char  *d_typeArray_p;   // array indicating the type of corresponding
//                              // values stored in 'd_valueArray_p'
//
//      void **d_valueArray_p;  // array of pointers to the values stored
//
//      int    d_length;        // number of values stored
//
//      int    d_capacity;      // physical capacity of the type and value
//                              // arrays
//
//      bdema_BufferedSequentialPool
//             d_pool;          // buffered sequential memory pool used to
//                              // supply memory
//
//    private:
//      // PRIVATE MANIPULATORS
//      void increaseCapacity();
//          // Increase the capacity of the internal arrays used to store
//          // elements added to this array by at least one element.
//
//    public:
//      // TYPES
//      enum Type { MY_INT, MY_DOUBLE };
//
//      // CREATORS
//      my_BufferedIntDoubleArray(char            *buffer,
//                                int              size,
//                                bslma_Allocator *basicAllocator = 0);
//          // Create a fast 'int'-'double' array that initially allocates
//          // memory sequentially from the specified 'buffer' having the
//          // specified 'size' (in bytes).  Optionally specify a
//          // 'basicAllocator' used to supply memory if 'buffer' capacity is
//          // exceeded.  If 'basicAllocator' is 0, the currently installed
//          // default allocator is used.
//
//      ~my_BufferedIntDoubleArray();
//          // Destroy this array and all elements held by it.
//
//      // ...
//
//      // MANIPULATORS
//      void appendInt(int value);
//          // Append the specified 'int' 'value' to this array.
//
//      void appendDouble(double value);
//          // Append the specified 'double' 'value' to this array.
//
//      void removeAll();
//          // Remove all elements from this array.
//
//      // ...
//  };
//..
// The use of a buffered sequential pool and the 'release' method allows the
// 'removeAll' method to quickly deallocate memory of all elements:
//..
//  // MANIPULATORS
//  inline
//  void my_BufferedIntDoubleArray::removeAll()
//  {
//      d_pool.release();  // *very* efficient if 'd_pool' has not exhausted
//                         // buffer supplied at construction.
//
//      d_length = 0;
//  }
//..
// The buffered sequential pool optimizes the allocation of memory by using a
// buffer supplied at construction.  As described in the "Description" section,
// the need for *all* dynamic memory allocations are eliminated provided that
// the buffer is not exhausted.  The pool provides maximal memory allocation
// efficiency:
//..
//  // my_bufferedintdoublearray.cpp
//  #include <my_bufferedintdoublearray.h>
//
//  enum { INITIAL_SIZE = 1, GROWTH_FACTOR = 2 };
//
//  // PRIVATE MANIPULATORS
//  void my_BufferedIntDoubleArray::increaseCapacity()
//  {
//      // Implementation elided.
//      // ...
//  }
//
//  // CREATORS
//  my_BufferedIntDoubleArray::my_BufferedIntDoubleArray(
//                                             char            *buffer,
//                                             int              size,
//                                             bslma_Allocator *basicAllocator)
//  : d_length(0)
//  , d_capacity(INITIAL_SIZE)
//  , d_pool(buffer, size, basicAllocator)
//  {
//      d_typeArray_p  = static_cast<char *>(
//                        d_pool.allocate(d_capacity * sizeof *d_typeArray_p));
//      d_valueArray_p = static_cast<void **>(
//                       d_pool.allocate(d_capacity * sizeof *d_valueArray_p));
//  }
//..
// Note that in the destructor, all outstanding memory blocks are deallocated
// automatically when 'd_pool' is destroyed:
//..
//  my_BufferedIntDoubleArray::~my_BufferedIntDoubleArray()
//  {
//      assert(0 <= d_length);
//      assert(0 <= d_capacity);
//      assert(d_length <= d_capacity);
//  }
//
//  // MANIPULATORS
//  void my_BufferedIntDoubleArray::appendInt(int value)
//  {
//      if (d_length >= d_capacity) {
//          increaseCapacity();
//      }
//
//      int *item = static_cast<int *>(d_pool.allocate(sizeof *item));
//      *item = value;
//
//      d_typeArray_p[d_length]  = static_cast<char>
//                                         (my_BufferedIntDoubleArray::MY_INT);
//      d_valueArray_p[d_length] = item;
//
//      ++d_length;
//  }
//
//  void my_BufferedIntDoubleArray::appendDouble(double value)
//  {
//      if (d_length >= d_capacity) {
//          increaseCapacity();
//      }
//
//      double *item = static_cast<double *>(d_pool.allocate(sizeof *item));
//      *item = value;
//
//      d_typeArray_p[d_length]  = static_cast<char>(
//                                       my_BufferedIntDoubleArray::MY_DOUBLE);
//      d_valueArray_p[d_length] = item;
//
//      ++d_length;
//  }
//..
///Example 2: Implementing an Allocator Using 'bdema_BufferedSequentialPool'
///- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// 'bslma_Allocator' is used throughout the interfaces of BDE components.
// Suppose we would like to create a fast allocator, 'my_FastAllocator', that
// allocates memory from a buffer in a similar fashion to
// 'bdema_BufferedSequentialPool'.  This class can be used directly to
// implement such an allocator.
//
// Note that the documentation for this class is simplified for this usage
// example.  Please see 'bdema_bufferedsequentialallocator' for full
// documentation of a similar class.
//..
//  class my_FastAllocator : public bslma_Allocator {
//      // This class implements the 'bslma_Allocator' protocol to provide a
//      // fast allocator of heterogeneous blocks of memory (of varying,
//      // user-specified sizes) from an external buffer whose address and size
//      // are supplied at construction.
//
//      // DATA
//      bdema_BufferedSequentialPool d_pool;  // memory manager for allocated
//                                            // memory blocks
//
//      // CREATORS
//      my_FastAllocator(char            *buffer,
//                       int              size,
//                       bslma_Allocator *basicAllocator = 0);
//          // Create an allocator for allocating memory blocks from the
//          // specified external 'buffer' of the specified 'size' (in bytes).
//          // Optionally specify a 'basicAllocator' used to supply memory
//          // should the capacity of 'buffer' be exhausted.  If
//          // 'basicAllocator' is 0, the currently installed default allocator
//          // is used.
//
//      ~my_FastAllocator();
//          // Destroy this allocator.  All memory allocated from this
//          // allocator is released.
//
//      // MANIPULATORS
//      virtual void *allocate(int size);
//          // Return the address of a contiguous block of memory of the
//          // specified 'size' (in bytes).
//
//      virtual void deallocate(void *address);
//          // This method has no effect as all memory allocated by this
//          // allocator is managed.
//  };
//
//  // CREATORS
//  inline
//  my_FastAllocator::my_FastAllocator(char            *buffer,
//                                     int              size,
//                                     bslma_Allocator *basicAllocator)
//  : d_pool(buffer, size, basicAllocator)
//  {
//  }
//
//  inline
//  my_FastAllocator::~my_FastAllocator()
//  {
//      d_pool.release();
//  }
//
//  // MANIPULATORS
//  inline
//  void *my_FastAllocator::allocate(int size)
//  {
//      return d_pool.allocate(size);
//  }
//
//  inline
//  void my_FastAllocator::deallocate(void *address)
//  {
//  }
//..

#ifndef INCLUDED_BDESCM_VERSION
#include <bdescm_version.h>
#endif

#ifndef INCLUDED_BDEMA_BUFFERMANAGER
#include <bdema_buffermanager.h>
#endif

#ifndef INCLUDED_BDEMA_INFREQUENTDELETEBLOCKLIST
#include <bdema_infrequentdeleteblocklist.h>
#endif

#ifndef INCLUDED_BSLS_ALIGNMENT
#include <bsls_alignment.h>
#endif

#ifndef INCLUDED_BSLS_BLOCKGROWTH
#include <bsls_blockgrowth.h>
#endif

#ifndef INCLUDED_BSLS_TYPES
#include <bsls_types.h>
#endif

#ifndef INCLUDED_BSL_CSTDDEF
#include <bsl_cstddef.h>  // for 'bsl::size_t'
#endif

namespace BloombergLP {

class bslma_Allocator;

                        // ==================================
                        // class bdema_BufferedSequentialPool
                        // ==================================

class bdema_BufferedSequentialPool {
    // This class implements a fast memory pool that efficiently dispenses
    // heterogeneous blocks of memory (of varying, user-specified sizes) from
    // an external buffer whose address and size (in bytes) are supplied at
    // construction.  If an allocation request exceeds the remaining free
    // memory space in the external buffer, memory will be supplied by an
    // (optional) allocator supplied also at construction; if no allocator is
    // supplied, the currently installed default allocator will be used.  This
    // class is *exception* *neutral*: If memory cannot be allocated, the
    // behavior is defined by the (optional) allocator supplied at
    // construction.  Note that in no cases will the buffered sequential pool
    // attempt to deallocate the external buffer.

    // DATA
    char                *d_initialBuffer_p;  // external buffer supplied at
                                             // construction

    int                  d_initialSize;      // size of external buffer

    bdema_BufferManager  d_buffer;           // memory manager for current
                                             // buffer

    bsls_BlockGrowth::Strategy
                         d_growthStrategy;   // growth strategy for block list

    int                  d_maxBufferSize;    // maximum internal buffer size

    bdema_InfrequentDeleteBlockList
                         d_blockList;        // memory manager used to supply
                                             // dynamically allocated memory
                                             // blocks

  private:
    // PRIVATE ACCESSORS
    int calculateNextBufferSize(int size) const;
        // Return a next buffer size (in bytes) that is sufficiently large to
        // satisfy a memory allocation request of the specified 'size' (in
        // bytes), or the maximum buffer size if the buffer can no longer grow.

  private:
    // NOT IMPLEMENTED
    bdema_BufferedSequentialPool(const bdema_BufferedSequentialPool&);
    bdema_BufferedSequentialPool& operator=(
                                 const bdema_BufferedSequentialPool&);

  public:
    // CREATORS
    bdema_BufferedSequentialPool(
                               char                       *buffer,
                               int                         size,
                               bslma_Allocator            *basicAllocator = 0);
    bdema_BufferedSequentialPool(
                               char                       *buffer,
                               int                         size,
                               bsls_BlockGrowth::Strategy  growthStrategy,
                               bslma_Allocator            *basicAllocator = 0);
    bdema_BufferedSequentialPool(
                               char                       *buffer,
                               int                         size,
                               bsls_Alignment::Strategy    alignmentStrategy,
                               bslma_Allocator            *basicAllocator = 0);
    bdema_BufferedSequentialPool(
                               char                       *buffer,
                               int                         size,
                               bsls_BlockGrowth::Strategy  growthStrategy,
                               bsls_Alignment::Strategy    alignmentStrategy,
                               bslma_Allocator            *basicAllocator = 0);
        // Create a buffered sequential pool for allocating memory blocks from
        // the specified external 'buffer' having the specified 'size' (in
        // bytes), or from an internal buffer (after the external 'buffer' is
        // exhausted).  Optionally specify a 'growthStrategy' used to control
        // buffer growth.  If a 'growthStrategy' is not specified, geometric
        // growth is used.  Optionally specify an 'alignmentStrategy' used to
        // align allocated memory blocks.  If an 'alignmentStrategy' is not
        // specified, natural alignment is used.  Optionally specify a
        // 'basicAllocator' used to supply memory should the capacity of
        // 'buffer' be exhausted.  If 'basicAllocator' is 0, the currently
        // installed default allocator is used.  The behavior is undefined
        // unless '0 < size', and 'buffer' has at least 'size' bytes.  Note
        // that, due to alignment effects, it is possible that not all 'size'
        // bytes of memory in 'buffer' can be used for allocation.

    bdema_BufferedSequentialPool(
                               char                       *buffer,
                               int                         size,
                               int                         maxBufferSize,
                               bslma_Allocator            *basicAllocator = 0);
    bdema_BufferedSequentialPool(
                               char                       *buffer,
                               int                         size,
                               int                         maxBufferSize,
                               bsls_BlockGrowth::Strategy  growthStrategy,
                               bslma_Allocator            *basicAllocator = 0);
    bdema_BufferedSequentialPool(
                               char                       *buffer,
                               int                         size,
                               int                         maxBufferSize,
                               bsls_Alignment::Strategy    alignmentStrategy,
                               bslma_Allocator            *basicAllocator = 0);
    bdema_BufferedSequentialPool(
                               char                       *buffer,
                               int                         size,
                               int                         maxBufferSize,
                               bsls_BlockGrowth::Strategy  growthStrategy,
                               bsls_Alignment::Strategy    alignmentStrategy,
                               bslma_Allocator            *basicAllocator = 0);
        // Create a buffered sequential pool for allocating memory blocks from
        // the specified external 'buffer' having the specified 'size' (in
        // bytes), or from an internal buffer (after the external 'buffer' is
        // exhausted) where the buffer growth is limited to the specified
        // 'maxBufferSize' (in bytes).  Optionally specify a 'growthStrategy'
        // used to control buffer growth.  If a 'growthStrategy' is not
        // specified, geometric growth is used.  Optionally specify an
        // 'alignmentStrategy' used to align allocated memory blocks.  If an
        // 'alignmentStrategy' is not specified, natural alignment is used.
        // Optionally specify a 'basicAllocator' used to supply memory should
        // the capacity of 'buffer' be exhausted.  If 'basicAllocator' is 0,
        // the currently installed default allocator is used.  The behavior is
        // undefined unless '0 < size', 'size <= maxBufferSize', and 'buffer'
        // has at least 'size' bytes.  Note that, due to alignment effects, it
        // is possible that not all 'size' bytes of memory in 'buffer' can be
        // used for allocation.

    ~bdema_BufferedSequentialPool();
        // Destroy this buffered sequential pool.  All memory allocated from
        // this pool is released.

    // MANIPULATORS
    void *allocate(bsls_Types::size_type size);
        // Return the address of a contiguous block of memory of the specified
        // 'size' (in bytes) according to the alignment strategy specified at
        // construction.  If the allocation request exceeds the remaining free
        // memory space in the external buffer supplied at construction, use
        // memory obtained from the allocator supplied at construction.  The
        // behavior is undefined unless '0 < size'.

    template <class TYPE>
    void deleteObjectRaw(const TYPE *object);
        // Destroy the specified 'object'.  Note that the memory is not
        // deallocated because there is no 'deallocate' method in
        // 'bslma_BufferedSequentialPool'.

    template <class TYPE>
    void deleteObject(const TYPE *object);
        // Destroy the specified 'object'.  Note that this method has the same
        // effect as the 'deleteObjectRaw' method (since no deallocation is
        // involved), and exists for consistency across memory pools.

    void release();
        // Release all memory currently allocated through this pool.  This
        // method deallocates all memory (if any) allocated with the allocator
        // provided at construction, and make available memory from the entire
        // external buffer supplied at construction for subsequent allocations,
        // but has no effect on the contents of the buffer.  Note that this
        // pool is reset to its initial state by this method.
};

}  // close namespace BloombergLP

// Note that the 'new' and 'delete' operators are declared outside the
// 'BloombergLP' namespace so that they do not hide the standard placement
// 'new' and 'delete' operators (i.e.,
// 'void *operator new(bsl::size_t, void *)' and
// 'void operator delete(void *)').
//
// Also note that only the scalar versions of operators 'new' and 'delete' are
// provided, because overloading 'new' (and 'delete') with their array versions
// would cause dangerous ambiguity.  Consider what would have happened had we
// overloaded the array version of 'operator new':
//..
//   void *operator new[](bsl::size_t                                size,
//                        BloombergLP::bdema_BufferedSequentialPool& pool);
//..
// The user of the pool class would have expected to be able to use operator
// 'new' as follows:
//..
//   new (*pool) my_Type[...];
//..
// The problem is that this expression returns an array that cannot be safely
// deallocated.  On the one hand, there is no syntax in C++ to invoke an
// overloaded 'operator delete'; on the other hand, the pointer returned by
// 'operator new' cannot be passed to the 'deallocate' method directly
// because the pointer is different from the one returned by the 'allocate'
// method.  The compiler offsets the value of this pointer by a header, which
// is used to maintain the number of objects in the array (so that the
// 'operator delete' can destroy the right number of objects).

// FREE OPERATORS
void *operator new(bsl::size_t                                size,
                   BloombergLP::bdema_BufferedSequentialPool& pool);
    // Return a block of memory of the specified 'size' (in bytes) allocated
    // from the specified 'pool'.  Note that an object may allocate additional
    // memory internally, requiring the allocator to be passed in as a
    // constructor argument:
    //..
    //  my_Type *newMyType(bdema_BufferedSequentialPool *pool,
    //                     bslma_Allocator              *basicAllocator)
    //  {
    //      return new (*pool) my_Type(..., basicAllocator);
    //  }
    //..
    // Also note that the analogous version of 'operator delete' should not be
    // called directly.  Instead, this component provides a static template
    // member function, 'deleteObject', parameterized by 'TYPE' that performs
    // the following:
    //..
    //  void deleteMyType(bdema_BufferedSequentialPool *pool, my_Type *t)
    //  {
    //      t->~my_Type();
    //  }
    //..

void operator delete(void                                       *address,
                     BloombergLP::bdema_BufferedSequentialPool&  pool);
    // Use the specified 'pool' to deallocate the memory at the specified
    // 'address'.  The behavior is undefined unless 'address' was allocated
    // using 'pool' and has not already been deallocated.  This operator is
    // supplied solely to allow the compiler to arrange for it to be called in
    // case of an exception.

// ===========================================================================
//                        INLINE FUNCTION DEFINITIONS
// ===========================================================================

namespace BloombergLP {

                        // ----------------------------------
                        // class bdema_BufferedSequentialPool
                        // ----------------------------------

// CREATORS
inline
bdema_BufferedSequentialPool::~bdema_BufferedSequentialPool()
{
    d_blockList.release();
}

// MANIPULATORS
template <class TYPE>
inline
void bdema_BufferedSequentialPool::deleteObjectRaw(const TYPE *object)
{
    if (0 != object) {
#ifndef BSLS_PLATFORM__CMP_SUN
        object->~TYPE();
#else
        const_cast<TYPE *>(object)->~TYPE();
#endif
    }
}

template <class TYPE>
inline
void bdema_BufferedSequentialPool::deleteObject(const TYPE *object)
{
    deleteObjectRaw(object);
}

inline
void bdema_BufferedSequentialPool::release()
{
    d_buffer.replaceBuffer(d_initialBuffer_p, d_initialSize);
    d_blockList.release();
}

}  // close namespace BloombergLP

// FREE OPERATORS
inline
void *operator new(bsl::size_t                                size,
                   BloombergLP::bdema_BufferedSequentialPool& pool)
{
    return pool.allocate(size);
}

inline
void operator delete(void *, BloombergLP::bdema_BufferedSequentialPool&)
{
    // NOTE: there is no deallocation from this allocation mechanism.
}

#endif

// ---------------------------------------------------------------------------
// NOTICE:
//      Copyright (C) Bloomberg L.P., 2010
//      All Rights Reserved.
//      Property of Bloomberg L.P. (BLP)
//      This software is made available solely pursuant to the
//      terms of a BLP license agreement which governs its use.
// ----------------------------- END-OF-FILE ---------------------------------
