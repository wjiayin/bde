// bslstl_rbtree_nodeallocator.t.cpp                                  -*-C++-*-
#include <bslstl_rbtree_nodeallocator.h>

#include <bslalg_rbtreeprimitives.h>
#include <bslalg_rbtreenode.h>

#include <bslma_allocator.h>
#include <bslma_testallocator.h>
#include <bslma_defaultallocatorguard.h>

#include <bsls_assert.h>

#include <algorithm>

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

using namespace BloombergLP;
using namespace std;
using namespace bslstl;

//=============================================================================
//                              TEST PLAN
//-----------------------------------------------------------------------------
//                              Overview
//                              --------
//
// Global Concerns:
//: o Pointer/reference parameters are declared 'const'.
//: o No memory is ever allocated.
//: o Precondition violations are detected in appropriate build modes.
//-----------------------------------------------------------------------------
// CLASS METHODS
// ----------------------------------------------------------------------------
// [ 1] BREATHING TEST
//-----------------------------------------------------------------------------
//=============================================================================

//=============================================================================
//                  STANDARD BDE ASSERT TEST MACRO
//-----------------------------------------------------------------------------
// NOTE: THIS IS A LOW-LEVEL COMPONENT AND MAY NOT USE ANY C++ LIBRARY
// FUNCTIONS, INCLUDING IOSTREAMS.
static int testStatus = 0;

static void aSsErT(bool b, const char *s, int i) {
    if (b) {
        printf("Error " __FILE__ "(%d): %s    (failed)\n", i, s);
        if (testStatus >= 0 && testStatus <= 100) ++testStatus;
    }
}

# define ASSERT(X) { aSsErT(!(X), #X, __LINE__); }

//=============================================================================
//                  STANDARD BDE LOOP-ASSERT TEST MACROS
//-----------------------------------------------------------------------------
# define LOOP_ASSERT(I,X) { \
    if (!(X)) { P_(I); aSsErT(!(X), #X, __LINE__); } }

# define LOOP2_ASSERT(I,J,X) { \
    if (!(X)) { P(I) P_(J);   \
                aSsErT(!(X), #X, __LINE__); } }

#define LOOP3_ASSERT(I,J,K,X) { \
    if (!(X)) { P(I) P(J) P_(K) \
                aSsErT(!(X), #X, __LINE__); } }

//=============================================================================
//                  SEMI-STANDARD TEST OUTPUT MACROS
//-----------------------------------------------------------------------------
#define Q(X) printf("<| " #X " |>\n");      // Quote identifier literally.
#define P(X) dbg_print(#X " = ", X, "\n");  // Print identifier and value.
#define P_(X) dbg_print(#X " = ", X, ", "); // P(X) without '\n'
#define L_ __LINE__                         // current Line number
#define T_ putchar('\t');                   // Print a tab (w/o newline)

// ============================================================================
//                  NEGATIVE-TEST MACRO ABBREVIATIONS
// ----------------------------------------------------------------------------

#define ASSERT_SAFE_PASS(EXPR) BSLS_ASSERTTEST_ASSERT_SAFE_PASS(EXPR)
#define ASSERT_SAFE_FAIL(EXPR) BSLS_ASSERTTEST_ASSERT_SAFE_FAIL(EXPR)
#define ASSERT_PASS(EXPR)      BSLS_ASSERTTEST_ASSERT_PASS(EXPR)
#define ASSERT_FAIL(EXPR)      BSLS_ASSERTTEST_ASSERT_FAIL(EXPR)
#define ASSERT_OPT_PASS(EXPR)  BSLS_ASSERTTEST_ASSERT_OPT_PASS(EXPR)
#define ASSERT_OPT_FAIL(EXPR)  BSLS_ASSERTTEST_ASSERT_OPT_FAIL(EXPR)

//=============================================================================
//                  GLOBAL HELPER FUNCTIONS FOR TESTING
//-----------------------------------------------------------------------------

typedef bslalg::RbTreePrimitives Op;
typedef bslalg::RbTreeNode   Node;

// Fundamental-type-specific print functions.
inline void dbg_print(bool b) { printf(b ? "true" : "false"); fflush(stdout); }
inline void dbg_print(char c) { printf("%c", c); fflush(stdout); }
inline void dbg_print(unsigned char c) { printf("%c", c); fflush(stdout); }
inline void dbg_print(signed char c) { printf("%c", c); fflush(stdout); }
inline void dbg_print(short val) { printf("%d", (int)val); fflush(stdout); }
inline void dbg_print(unsigned short val) {
    printf("%d", (int)val); fflush(stdout);
}
inline void dbg_print(int val) { printf("%d", val); fflush(stdout); }
inline void dbg_print(unsigned int val) { printf("%u", val); fflush(stdout); }
inline void dbg_print(long val) { printf("%ld", val); fflush(stdout); }
inline void dbg_print(unsigned long val) {
    printf("%lu", val); fflush(stdout);
}
inline void dbg_print(long long val) { printf("%lld", val); fflush(stdout); }
inline void dbg_print(unsigned long long val) {
    printf("%llu", val); fflush(stdout);
}
inline void dbg_print(float val) {
    printf("'%f'", (double)val); fflush(stdout);
}
inline void dbg_print(double val) { printf("'%f'", val); fflush(stdout); }
inline void dbg_print(long double val) {
    printf("'%Lf'", val); fflush(stdout);
}
inline void dbg_print(const char* s) { printf("\"%s\"", s); fflush(stdout); }
inline void dbg_print(char* s) { printf("\"%s\"", s); fflush(stdout); }
inline void dbg_print(void* p) { printf("%p", p); fflush(stdout); }

// Generic debug print function (3-arguments).
template <typename T>
void dbg_print(const char* s, const T& val, const char* nl) {
    printf("%s", s); dbg_print(val);
    printf("%s", nl);
    fflush(stdout);
}

//=============================================================================
//                  GLOBAL TYPEDEFS/CONSTANTS FOR TESTING
//-----------------------------------------------------------------------------

class AllocatingIntType {
    // DATA
    bslma_Allocator *d_allocator_p;
    int             *d_value_p;

  public:

    // CREATORS
    AllocatingIntType()
    {
        // This methods should never be called.

        BSLS_ASSERT_OPT(false);
    }
    AllocatingIntType(const AllocatingIntType&)   
    {
        // This methods should never be called.

        BSLS_ASSERT_OPT(false);
    }

    AllocatingIntType(bslma_Allocator *allocator) 
    : d_allocator_p(bslma_Default::allocator(allocator))
    {
        d_value_p  = static_cast<int *>(d_allocator_p->allocate(sizeof(int)));
        *d_value_p = 0xabcd;
    }

    AllocatingIntType(const AllocatingIntType&  original,
                      bslma_Allocator          *allocator) 
    : d_allocator_p(bslma_Default::allocator(allocator))
    {
        d_value_p  = static_cast<int *>(d_allocator_p->allocate(sizeof(int)));
        *d_value_p = *original.d_value_p;
    }

    ~AllocatingIntType()
    {
        BSLS_ASSERT(0 != d_value_p);
        d_allocator_p->deleteObject(d_value_p);
    }

    int& value() { return *d_value_p; }

    const int value() const { return *d_value_p; }
};

//=============================================================================
//                                USAGE EXAMPLE
//-----------------------------------------------------------------------------
 
//=============================================================================
//                                 MAIN PROGRAM
//-----------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    int  test = argc > 1 ? atoi(argv[1]) : 0;
    bool verbose = argc > 2;
    bool veryVerbose = argc > 3;
    bool veryVeryVerbose = argc > 4;
    bool veryVeryVeryVerbose = argc > 5;

    printf("TEST " __FILE__ " CASE %d\n", test);

    switch (test) { case 0:
      case 2: {
          if (verbose) printf("\nUSAGE EXAMPLE"
                              "\n=============\n");

      } break;
      case 1: {
        // --------------------------------------------------------------------
        // BREATHING TEST:
        //   Developers' Sandbox.
        //
        // Plan:
        //   Perform and ad-hoc test of the primary modifiers and accessors.
        //
        // Testing:
        //   This "test" *exercises* basic functionality, but *tests* nothing.
        // --------------------------------------------------------------------

        if (verbose) printf("\nBREATHING TEST"
                            "\n==============\n");

        {
            if (veryVerbose) {
                printf("\tTest int-node w/ bslma_Allocator\n");
            }

            typedef RbTree_NodeAllocator<int, bsl::allocator<int> > Obj;
            typedef RbTree_Node<int>                                Node;

            bslma_TestAllocator da, ta;
            bslma_DefaultAllocatorGuard daGuard(&da);

            Obj x(&ta); const Obj& X = x;
            Node *value = x.createDefaultValueNode();
            ASSERT(0 != value);
            ASSERT(0 == da.numBlocksInUse());
            ASSERT(1 == ta.numBlocksInUse());
            x.releaseNode(value);
            ASSERT(0 == da.numBlocksInUse());
            ASSERT(0 == ta.numBlocksInUse());
            
            value = x.createNode(0xabcd);
            ASSERT(0 != value);
            ASSERT(0xabcd == value->d_value);
            ASSERT(0 == da.numBlocksInUse());
            ASSERT(1 == ta.numBlocksInUse());
            x.releaseNode(value);
            ASSERT(0 == da.numBlocksInUse());
            ASSERT(0 == ta.numBlocksInUse());                
        }
        {
            if (veryVerbose) {
                printf("\tTest allocating-node w/ bslma_Allocator\n");
            }

            typedef AllocatingIntType AllocType;
            typedef RbTree_NodeAllocator
                                   <AllocType, bsl::allocator<AllocType> > Obj;
            typedef RbTree_Node<AllocatingIntType> Node;

            bslma_TestAllocator da, ta;
            bslma_DefaultAllocatorGuard daGuard(&da);


            Obj x(&ta); const Obj& X = x;

            Node *value = x.createDefaultValueNode();
            ASSERT(0 != value);
            ASSERT(0xabcd == value->d_value.value());
            ASSERT(0 == da.numBlocksInUse());
            ASSERT(2 == ta.numBlocksInUse());
            x.releaseNode(value);
            ASSERT(0 == da.numBlocksInUse());
            ASSERT(0 == ta.numBlocksInUse());

            bslma_TestAllocator ta2;
            AllocType myInt(&ta2);
            myInt.value() = 0xdbca;

            ASSERT(0 == da.numBlocksInUse());
            ASSERT(0 == ta.numBlocksInUse());

            value = x.createNode(myInt);
            ASSERT(0 != value);
            ASSERT(0xdbca == value->d_value.value());
            ASSERT(0 == da.numBlocksInUse());
            ASSERT(2 == ta.numBlocksInUse());
            x.releaseNode(value);
            ASSERT(0 == da.numBlocksInUse());
            ASSERT(0 == ta.numBlocksInUse());
        }
       } break;
      default: {
        fprintf(stderr, "WARNING: CASE `%d' NOT FOUND.\n", test);
        testStatus = -1;
      }
    }

    if (testStatus > 0) {
        fprintf(stderr, "Error, non-zero test status = %d.\n", testStatus);
    }
    return testStatus;
}

// ---------------------------------------------------------------------------
// NOTICE:
//      Copyright (C) Bloomberg L.P., 2004
//      All Rights Reserved.
//      Property of Bloomberg L.P. (BLP)
//      This software is made available solely pursuant to the
//      terms of a BLP license agreement which governs its use.
// ----------------------------- END-OF-FILE ---------------------------------
