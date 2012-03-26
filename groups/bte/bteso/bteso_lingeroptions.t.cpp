// bteso_lingeroptions.t.cpp                                          -*-C++-*-

#include <bteso_lingeroptions.h>

#include <bdex_instreamfunctions.h>
#include <bdex_outstreamfunctions.h>
#include <bdex_testinstream.h>
#include <bdex_testinstreamexception.h>
#include <bdex_testoutstream.h>

#include <bslma_default.h>
#include <bslma_testallocator.h>

#include <bsl_climits.h>     // 'INT_MIN', 'INT_MAX'
#include <bsl_cstdlib.h>
#include <bsl_iostream.h>
#include <bsl_sstream.h>

#include <bslalg_hastrait.h>

#include <bsls_asserttest.h>

#include <sys/socket.h>
#include <bsls_platform.h>
#include <bteso_platform.h>

using namespace BloombergLP;
using namespace bsl;

// ============================================================================
//                             TEST PLAN
// ----------------------------------------------------------------------------
//                             Overview
//                             --------
// The component under test implements a single, simply constrained
// (value-semantic) attribute class.  The Primary Manipulators and Basic
// Accessors are therefore, respectively, the attribute setters and getters,
// each of which follows our standard unconstrained attribute-type naming
// conventions: 'setAttributeName' and 'attributeName'.
//
// Primary Manipulators:
//: o 'setLingerFlag'
//: o 'setTimeout'
//
// Basic Accessors:
//: o 'lingerFlag'
//: o 'timeout'
//
// Certain standard value-semantic-type test cases are omitted:
//: o [ 8] -- 'swap' is not implemented for this class.
//
// Global Concerns:
//: o The test driver is robust w.r.t. reuse in other, similar components.
//: o ACCESSOR methods are declared 'const'.
//: o CREATOR & MANIPULATOR pointer/reference parameters are declared 'const'.
//: o Precondition violations are detected in appropriate build modes.
//: o No memory is allocated from any allocator.
//
// Global Assumptions:
//: o ACCESSOR methods are 'const' thread-safe.
//: o Individual attribute types are presumed to be *alias-safe*; hence, only
//:   certain methods require the testing of this property:
//:   o copy-assignment
// ----------------------------------------------------------------------------
// CLASS METHODS
// [10] int maxSupportedBdexVersion();
//
// CREATORS
// [ 2] bteso_LingerOptions();
// [ 3] bteso_LingerOptions(int timeout, bool lingerFlag);
// [ 7] bteso_LingerOptions(const bteso_LingerOptions& original);
// [ 2] ~bteso_LingerOptions();
//
// MANIPULATORS
// [ 9] operator=(const bteso_LingerOptions& rhs);
// [10] STREAM& bdexStreamIn(STREAM& stream, int version);
// [ 2] setLingerFlag(bool value);
// [ 2] setTimeout(int value);
//
// ACCESSORS
// [10] STREAM& bdexStreamOut(STREAM& stream, int version) const;
// [ 4] bool lingerFlag() const;
// [ 4] int timeout() const;
//
// [ 5] ostream& print(ostream& s, int level = 0, int sPL = 4) const;
//
// FREE OPERATORS
// [ 6] bool operator==(const bteso_LingerOptions& lhs, rhs);
// [ 6] bool operator!=(const bteso_LingerOptions& lhs, rhs);
// [ 5] operator<<(ostream& s, const bteso_LingerOptions& d);
// ----------------------------------------------------------------------------
// [ 1] BREATHING TEST
// [12] USAGE EXAMPLE
// [ *] CONCERN: This test driver is reusable w/other, similar components.
// [ 3] CONCERN: All creator/manipulator ptr./ref. parameters are 'const'.
// [ 4] CONCERN: All accessor methods are declared 'const'.
// [ 8] CONCERN: Precondition violations are detected when enabled.

// ============================================================================
//                    STANDARD BDE ASSERT TEST MACROS
// ----------------------------------------------------------------------------

static int testStatus = 0;

static void aSsErT(int c, const char *s, int i)
{
    if (c) {
        cout << "Error " << __FILE__ << "(" << i << "): " << s
             << "    (failed)" << endl;
        if (testStatus >= 0 && testStatus <= 100) ++testStatus;
    }
}
# define ASSERT(X) { aSsErT(!(X), #X, __LINE__); }

// ============================================================================
//                  STANDARD BDE LOOP-ASSERT TEST MACROS
// ----------------------------------------------------------------------------

#define LOOP_ASSERT(I,X) {                                                    \
    if (!(X)) { cout << #I << ": " << I << "\n"; aSsErT(1, #X, __LINE__);}}

#define LOOP2_ASSERT(I,J,X) {                                                 \
    if (!(X)) { cout << #I << ": " << I << "\t" << #J << ": "                 \
              << J << "\n"; aSsErT(1, #X, __LINE__); } }

#define LOOP3_ASSERT(I,J,K,X) {                                               \
   if (!(X)) { cout << #I << ": " << I << "\t" << #J << ": " << J << "\t"     \
              << #K << ": " << K << "\n"; aSsErT(1, #X, __LINE__); } }

#define LOOP4_ASSERT(I,J,K,L,X) {                                             \
   if (!(X)) { cout << #I << ": " << I << "\t" << #J << ": " << J << "\t" <<  \
       #K << ": " << K << "\t" << #L << ": " << L << "\n";                    \
       aSsErT(1, #X, __LINE__); } }

#define LOOP5_ASSERT(I,J,K,L,M,X) {                                           \
   if (!(X)) { cout << #I << ": " << I << "\t" << #J << ": " << J << "\t" <<  \
       #K << ": " << K << "\t" << #L << ": " << L << "\t" <<                  \
       #M << ": " << M << "\n";                                               \
       aSsErT(1, #X, __LINE__); } }

// ============================================================================
//                  SEMI-STANDARD TEST OUTPUT MACROS
// ----------------------------------------------------------------------------

#define P(X) cout << #X " = " << (X) << endl; // Print identifier and value.
#define Q(X) cout << "<| " #X " |>" << endl;  // Quote identifier literally.
#define P_(X) cout << #X " = " << (X) << ", " << flush; // 'P(X)' without '\n'
#define T_ cout << "\t" << flush;             // Print tab w/o newline.
#define L_ __LINE__                           // current Line number

// ============================================================================
//                  NEGATIVE-TEST MACRO ABBREVIATIONS
// ----------------------------------------------------------------------------

#define ASSERT_FAIL(expr)      BSLS_ASSERTTEST_ASSERT_FAIL(expr)
#define ASSERT_PASS(expr)      BSLS_ASSERTTEST_ASSERT_PASS(expr)
#define ASSERT_SAFE_FAIL(expr) BSLS_ASSERTTEST_ASSERT_SAFE_FAIL(expr)
#define ASSERT_SAFE_PASS(expr) BSLS_ASSERTTEST_ASSERT_SAFE_PASS(expr)

// ============================================================================
//                     GLOBAL TYPEDEFS FOR TESTING
// ----------------------------------------------------------------------------

typedef bteso_LingerOptions Obj;
typedef bdex_TestInStream   In;
typedef bdex_TestOutStream  Out;

// ============================================================================
//                                 TYPE TRAITS
// ----------------------------------------------------------------------------

BSLMF_ASSERT((bslalg_HasTrait<Obj, bslalg_TypeTraitBitwiseMoveable>::VALUE));

// ============================================================================
//                             GLOBAL TEST DATA
// ----------------------------------------------------------------------------

// Define DEFAULT DATA (potentially) used by test cases 3, 7, (8), 9, and (10)

struct DefaultDataRow {
    int         d_line;           // source line number
    int         d_timeout;
    bool        d_lingerFlag;
};

static
const DefaultDataRow DEFAULT_DATA[] =
{
    //LINE  TIMEOUT   FLAG
    //----  -------   ----

    // default (must be first)
    { L_,         0,  false },

    // 'timeout'
    { L_,         1,  false },
    { L_,   INT_MAX,  false },

    // 'lingerFlag'
    { L_,         0,   true },

    // other
    { L_,         1,   true },
    { L_,   INT_MAX,   true },
};
const int DEFAULT_NUM_DATA = sizeof DEFAULT_DATA / sizeof *DEFAULT_DATA;

// ============================================================================
//                               USAGE EXAMPLE
// ----------------------------------------------------------------------------
///Usage
///-----
// In this section we show intended usage of this component.
//
///Example 1: Creating functions to set linger option
/// - - - - - - - - - - - - - - - - - - - - - - - - -
// This component is designed to be used at a higher level to set the linger
// options for a stream-based socket.  This example shows how to create a
// function that takes 'bteso_LingerOptions' as an argument and sets the linger
// options of a socket.  We will assume Berkeley socket API is available to
// configure the socket.
//
// First, we define a cross-platform compatible typedef for a socket handle:
//..
    #ifdef BSLS_PLATFORM__OS_WINDOWS
        typedef SOCKET Handle;
    #else
        typedef int Handle;
    #endif
//..
// Then, we declare the function, 'setLingerOptions', that takes a 'Handle' and
// a 'bteso_LingerOptions' object, and sets the linger options for 'Handle':
//..
    int setLingerOptions(Handle                     handle,
                         const bteso_LingerOptions& lingerOptions)
    {
//..
// Next, we define a 'typedef' for the 'struct' needed to set the linger
// options:
//..
    #if defined(BSLS_PLATFORM__OS_WINDOWS) || defined(BSLS_PLATFORM__OS_CYGWIN)
        typedef LINGER LingerData;
    #else
        typedef linger LingerData;
    #endif
//..
// Then, we initialize a 'LingerData' object with data from 'lingerOptions',
// which will be supplied to the 'setsockopt' system call:
//..
        LingerData linger;
        linger.l_onoff  = lingerOptions.lingerFlag();
        linger.l_linger = lingerOptions.timeout();
//..
// Next, we configure the linger options for the socket:
//..
        return ::setsockopt(handle,
                            SOL_SOCKET,
                            SO_LINGER,
                            reinterpret_cast<void *>(&linger),
                            sizeof linger);
    }
//..

// ============================================================================
//                            MAIN PROGRAM
// ----------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    int                 test = argc > 1 ? atoi(argv[1]) : 0;
    bool             verbose = argc > 2;
    bool         veryVerbose = argc > 3;
    bool     veryVeryVerbose = argc > 4;
    bool veryVeryVeryVerbose = argc > 5;

    cout << "TEST " << __FILE__ << " CASE " << test << endl;

    // CONCERN: This test driver is reusable w/other, similar components.

    // CONCERN: In no case is memory allocated from the global allocator.

    bslma_TestAllocator globalAllocator("global", veryVeryVeryVerbose);
    bslma_Default::setGlobalAllocator(&globalAllocator);

    switch (test) { case 0:
      case 11: {
        // --------------------------------------------------------------------
        // USAGE EXAMPLE
        //   Extracted from component header file.
        //
        // Concerns:
        //: 1 The usage example provided in the component header file compiles,
        //:   links, and runs as shown.
        //
        // Plan:
        //: 1 Incorporate usage example from header into test driver, remove
        //:   leading comment characters, and replace 'assert' with 'ASSERT'.
        //:   (C-1)
        //
        // Testing:
        //   USAGE EXAMPLE
        // --------------------------------------------------------------------

        if (verbose) cout << endl
                          << "USAGE EXAMPLE" << endl
                          << "=============" << endl;

// Then, we create a new socket using the 'socket' function from Berkeley API:
//..
    Handle socketHandle = ::socket(AF_INET, SOCK_STREAM, 0);
//..
// Now, we create a 'bteso_LingerOptions' object, 'option', indicating an
// associated socket should block for 2 seconds when closing a stream with
// untransmitted data (i.e., lingering):
//..
    bteso_LingerOptions option(true, 2);
//..
// Finally, we call 'setLingerOptions' (defined above), to configure the
// options for the socket:
//..
    setLingerOptions(socketHandle, option);
//..
      } break;
      case 10: {
        // --------------------------------------------------------------------
        // BDEX STREAMING
        //   Ensure that we can externalize the value of any object of the
        //   class, and then unexternalize that value back into any object of
        //   the class.
        //
        // Concerns:
        //: 1 The class method 'maxSupportedBdexVersion' returns the expected
        //:   value.
        //:
        //: 2 The signature and return type of 'bdexStreamOut' and
        //:   'bdexStreamIn' are standard.
        //:
        //: 3 'bdexStreamOut' and 'bdexStreamIn' correctly externalize and
        //:   unexternalize the object respectively when given a valid stream.
        //:
        //: 4 'bdexStreamOut' does not stream out more data than necessary.
        //:
        //: 5 A stream that was valid remains valid after a successful
        //:   'bdexStreamOut' or 'bdexStreamIn' operation.
        //:
        //: 6 'bdexStreamIn' does not modify the object when given an empty or
        //:   invalid stream.
        //:
        //: 7 'bdexStreamOut' and 'bdexStreamIn' invalidate the supplied stream
        //:   if supplied with an unsupported version number.
        //:
        //: 8 'bdexStreamIn' invalidate the input stream if it contains invalid
        //:   data.
        //:
        //: 9 'bdexStreamIn' invalidate the input stream if it contains
        //:   incomplete data.
        //:
        //:10 'bdexStreamIn' puts the object into a valid state if the input
        //:   stream contains invalid data.
        //:
        //:11 BDEX streaming is exception neutral.
        //
        // Plan:
        //: 1 Invoke 'maxSupportedBdexVersion' and verify the expected version
        //:   number is returned.  (C-1)
        //:
        //: 2 Use the address of 'bdexStreamIn' and 'bdexStreamOut' to
        //:   initialize a member-function pointer having the appropriate
        //:   signature and return type.  (C-2)
        //:
        //: 3 Using the table-driven technique, specify a set of (unique) valid
        //:   object values (one per row) in terms of their individual
        //:   attributes, including (a) first, the default value, and (b)
        //:   boundary values corresponding to every range of values that each
        //:   individual attribute can independently attain.
        //:
        //: 4 For each supported version, 'V':  (C-3..5, 11)
        //:
        //:   1 For each row 'R1' in the table of P-3:  (C-3..5, 11)
        //:
        //:     1 Use the value constructor to create a 'const' 'Obj', 'Z',
        //:       each having the value corresponding to 'R1'.
        //:
        //:     2 Create an empty 'bdex_TestOutStream', 'out'.
        //:
        //:     3 Invoke 'bdexStreamOut', passing 'V' as the version number and
        //:       'out' as the output stream, to externalize values of 'Z'.
        //:
        //:     4 For each row 'R2' in the table of P-3:  (C-3..5, 11)
        //:
        //:       1 Use the value constructor to create a modifiable 'Obj',
        //:         'mX', having the value 'R2'.
        //:
        //:       2 Create a 'bdex_TestInStream', 'in', having the same data as
        //:         'out'.
        //:
        //:       3 Invoke 'bdexStreamIn', passing 'V' as the version number
        //:         and 'in' as the input stream,  to unexternalization data in
        //:         'in' to 'mX' in the presence of exception (using the
        //:         'BDEX_EXCEPTION_TEST_*' macros).  (C-11)
        //:
        //:       4 Use the equality-comparison operator to verify that 'mX'
        //:         now has the same value as 'Z'.  (C-3)
        //:
        //:       5 Verify that 'in' is valid and empty.  (C-4..5)
        //:
        //: 5 For each supported version, 'V':  (C-6)
        //:
        //:   1 For each row (representing a distinct object value, 'R') in the
        //:     table described in P-3:  (C-6)
        //:
        //:     1 Use the value constructor to create a 'const' 'Obj' 'Z' and a
        //:       modifiable 'Obj' 'mX' having the value 'R'.
        //:
        //:     2 Create an empty 'bdex_TestInStream', 'in'.
        //:
        //:     3 Invoke 'bdexStreamIn', passing 'V' as the version number and
        //:       'in' as the input stream,  to unexternalization data in 'in'
        //:       to 'mX'.
        //:
        //:     4 Use the equality-comparison operator to verify that 'mX'
        //:       still has the same value as 'Z'.
        //:
        //:     5 Verify that 'in' is now invalid.
        //:
        //:     3 Invoke 'bdexStreamIn' again, passing 'V' as the version
        //:       number and 'in' as the input stream,  to unexternalization
        //:       data in 'in' to 'mX'.
        //:
        //:     7 Use the equality-comparison operator to verify that 'mX'
        //:       still has the same value as 'Z'.
        //:
        //:     8 Verify that 'in' is still invalid.  (C-6)
        //:
        //: 6 For each supported version, 'V':  (C-10)
        //:
        //:   1 For each row (representing a distinct object value, 'R') in the
        //:     table described in P-3:  (C-10)
        //:
        //:     1 Use the value constructor to create a 'const' 'Obj' 'Z'
        //:       having the value 'R'.
        //:
        //:     2 Create an empty 'bdex_TestOutStream', 'out'.
        //:
        //:     3 Invoke 'bdexStreamOut', passing 'V' as the version number and
        //:       'out' as the output stream, to externalize values of 'Z'.
        //:
        //:     4 For 'i' equals 0 to the size of the data in 'out':  (C-10)
        //:
        //:       1 Create a 'bdex_TestOutStream' 'in' having the first 'i'
        //:         bytes of the data in 'out'.
        //:
        //:       2 Create a modifiable 'Obj' 'mX' using the default
        //:         constructor.
        //:
        //:       3 Invoke 'bdexStreamIn', passing 'V' as the version number
        //:         and 'in' as the input stream,  to unexternalization data in
        //:         'in' to 'mX'.
        //:
        //:       4 Verify that 'in' is invalid.  (C-9)
        //:
        //:       5 Allow the destructor to verify class invariant in the
        //:         appropriate build mode (C-10)
        //:
        //: 7 For each supported version, 'V':  (C-8, 10)
        //:   1 Create a 'bdex_TestInStream', 'in', with invalid data.
        //:
        //:   2 Create a modifiable 'Obj' 'mX' using the default constructor.
        //:
        //:   3 Invoke 'bdexStreamIn', passing 'V' as the version number and
        //:     'in' as the input stream,  to unexternalization data in 'in' to
        //:     'mX'.
        //:
        //:   4 Verify the input stream is invalidated.  (C-8)
        //:
        //:   5 Allow the destructor to verify class invariant in the
        //:     appropriate build mode (C-10)
        //:
        //: 8 Invoke 'bdexStreamOut' and 'bdexStreamIn', passing 0 and
        //:   'maxSupportedBdexVersion() + 1' as the version number, and verify
        //:   that the supplied stream is invalidated.  (C-7)
        //
        // Testing:
        //   int maxSupportedBdexVersion();
        //   STREAM& bdexStreamIn(STREAM& stream, int version);
        //   STREAM& bdexStreamOut(STREAM& stream, int version) const;
        // --------------------------------------------------------------------

        if (verbose) cout << endl
                          << "BDEX STREAMING" << endl
                          << "==============" << endl;

        if (verbose) cout << "\nTesting 'maxSupportedBdexVersion'." << endl;
        {
            ASSERT(1 == Obj::maxSupportedBdexVersion());
        }

        if (verbose) cout <<
                  "\nAssign the address of the methods to a variable." << endl;
        {
            typedef In&  (Obj::*inFuncPtr)(In&, int);
            typedef Out& (Obj::*outFuncPtr)(Out&, int) const;

            // Verify that the signature and return type are standard.

            inFuncPtr  streamIn  = &Obj::bdexStreamIn<In>;
            outFuncPtr streamOut = &Obj::bdexStreamOut<Out>;

            (void)streamIn;   // Quash potential compiler warning.
            (void)streamOut;  // Quash potential compiler warning.
        }

        const int MAX_VERSION = Obj::maxSupportedBdexVersion();

        if (verbose) cout << "\nUse table of distinct object values." << endl;

        const int NUM_DATA                     = DEFAULT_NUM_DATA;
        const DefaultDataRow (&DATA)[NUM_DATA] = DEFAULT_DATA;

        if (verbose) cout <<
         "\nTesting 'streamOut' and (valid) 'streamIn' functionality." << endl;

        for (int version = 1; version <= MAX_VERSION; ++version) {
            const int V = version;
            if (veryVerbose) { T_ P(V) }

            for (int ti = 0; ti < NUM_DATA; ++ti) {
                const int  LINE1    = DATA[ti].d_line;
                const int  TIMEOUT1 = DATA[ti].d_timeout;
                const bool FLAG1    = DATA[ti].d_lingerFlag;

                const Obj Z(TIMEOUT1, FLAG1);

                if (veryVerbose) { T_ P(LINE1) P(Z) }

                Out out;
                Z.bdexStreamOut(out, V);
                LOOP_ASSERT(LINE1, out);

                // Verify that each new value overwrites every old value
                // and that the input stream is emptied, but remains valid.

                for (int tj = 0; tj < NUM_DATA; ++tj) {
                    const int  LINE2    = DATA[tj].d_line;
                    const int  TIMEOUT2 = DATA[tj].d_timeout;
                    const bool FLAG2    = DATA[tj].d_lingerFlag;

                    Obj mX(TIMEOUT2, FLAG2);  const Obj& X = mX;

                    if (veryVerbose) { T_ P_(LINE2) P(X) }

                    In in(out.data(), out.length());   In &testInStream = in;
                    in.setSuppressVersionCheck(1);

                    BEGIN_BDEX_EXCEPTION_TEST { in.reset();
                        if (veryVeryVerbose) { T_ T_ Q(ExceptionTestBody) }
                        mX.bdexStreamIn(in, V);
                    } END_BDEX_EXCEPTION_TEST

                    LOOP2_ASSERT(LINE1, LINE2, Z == X);
                    LOOP2_ASSERT(LINE1, LINE2, in);
                    LOOP2_ASSERT(LINE1, LINE2, in.isEmpty());
                }
            }
        }

        if (verbose) cout << "\nOn empty and invalid streams." << endl;

        for (int version = 1; version <= MAX_VERSION; ++version) {
            const int V = version;
            if (veryVerbose) { T_ P(V) }

            for (int ti = 0; ti < NUM_DATA; ++ti) {
                const int  LINE    = DATA[ti].d_line;
                const int  TIMEOUT = DATA[ti].d_timeout;
                const bool FLAG    = DATA[ti].d_lingerFlag;

                const Obj Z(TIMEOUT, FLAG);
                Obj mX(Z);  const Obj& X = mX;

                if (veryVerbose) { T_ P_(LINE) P(Z) P(X) }

                LOOP3_ASSERT(LINE, Z, X, Z == X);

                In in;
                in.setSuppressVersionCheck(1);

                LOOP_ASSERT(LINE, in);
                LOOP_ASSERT(LINE, in.isEmpty());

                // Ensure that reading from an empty or invalid input stream
                // leaves the stream invalid and the target object unchanged.

                mX.bdexStreamIn(in, V);

                LOOP_ASSERT(LINE, !in);
                LOOP2_ASSERT(LINE, X, Obj() == X);

                mX.bdexStreamIn(in, V);

                LOOP_ASSERT(LINE, !in);
                LOOP2_ASSERT(LINE, X, Obj() == X);
            }
        }

        if (verbose) cout <<
                         "\nOn incomplete (but otherwise valid) data." << endl;

        for (int version = 1; version <= MAX_VERSION; ++version) {
            const int V = version;
            if (veryVerbose) { T_ P(V) }

            for (int ti = 0; ti < NUM_DATA; ++ti) {
                const int         LINE    = DATA[ti].d_line;
                const int         TIMEOUT = DATA[ti].d_timeout;
                const bool        FLAG    = DATA[ti].d_lingerFlag;

                const Obj Z(TIMEOUT, FLAG);

                if (veryVerbose) { T_ P_(LINE) P(Z) }

                Out out;
                Z.bdexStreamOut(out, V);
                LOOP_ASSERT(LINE, out);

                for (int i = 0; i < out.length(); ++i) {
                    if (veryVerbose) { T_ P(i) }

                    In in(out.data(), i);
                    in.setSuppressVersionCheck(1);

                    Obj mX;  const Obj& X = mX;

                    mX.bdexStreamIn(in, V);

                    LOOP_ASSERT(i, !in);
                    LOOP_ASSERT(i, Obj() == X);
                }
            }
        }

        if (verbose) cout << "\nStream with invalid data." << endl;

        for (int version = 1; version <= MAX_VERSION; ++version) {
            const int V = version;
            if (veryVerbose) { T_ P(V) }

            const int  D1 = 0;
            const bool D2 = false;

            {
                Out out;

                bdex_OutStreamFunctions::streamOut(out, -1, 1);
                bdex_OutStreamFunctions::streamOut(out, D2, 1);

                In in(out.data(), out.length());
                Obj mX;
                in.setSuppressVersionCheck(1);
                mX.bdexStreamIn(in, V);

                ASSERT(!in);
            }
            {
                Out out;

                bdex_OutStreamFunctions::streamOut(out, D1, 1);
                bdex_OutStreamFunctions::streamOut(out, D2, 1);

                In in(out.data(), out.length());
                Obj mX;
                in.setSuppressVersionCheck(1);
                mX.bdexStreamIn(in, V);

                ASSERT(in);
            }
        }

        if (verbose) cout << "\nBad version." << endl;
        {
            const Obj Z;
            {
                Out out;
                Z.bdexStreamOut(out, 0);
                ASSERT(!out);
            }
            {
                Out out;
                Z.bdexStreamOut(out, 2);
                ASSERT(!out);
            }
            {
                Out out;
                Z.bdexStreamOut(out, 1);
                ASSERT(out);

                Obj mX;
                In in(out.data(), out.length());
                in.setSuppressVersionCheck(1);

                mX.bdexStreamIn(in, 0);

                ASSERT(!in);
                in.reset();

                mX.bdexStreamIn(in, 2);

                ASSERT(!in);
            }
        }
      } break;
      case 9: {
        // --------------------------------------------------------------------
        // COPY-ASSIGNMENT OPERATOR
        //   Ensure that we can assign the value of any object of the class to
        //   any object of the class, such that the two objects subsequently
        //   have the same value.
        //
        // Concerns:
        //: 1 The assignment operator can change the value of any modifiable
        //:   target object to that of any source object.
        //:
        //: 2 The signature and return type are standard.
        //:
        //: 3 The reference returned is to the target object (i.e., '*this').
        //:
        //: 4 The value of the source object is not modified.
        //:
        //: 5 Assigning an object to itself behaves as expected (alias-safety).
        //
        // Plan:
        //: 1 Use the address of 'operator=' to initialize a member-function
        //:   pointer having the appropriate signature and return type for the
        //:   copy-assignment operator defined in this component.  (C-2)
        //:
        //: 2 Using the table-driven technique, specify a set of (unique) valid
        //:   object values (one per row) in terms of their individual
        //:   attributes, including (a) first, the default value, and (b)
        //:   boundary values corresponding to every range of values that each
        //:   individual attribute can independently attain.
        //:
        //: 3 For each row 'R1' (representing a distinct object value, 'V') in
        //:   the table described in P-3:  (C-1, 3..4)
        //:
        //:   1 Create two 'const' 'Obj', 'Z' and 'ZZ', each having the value
        //:     'V'.
        //:
        //:   2 Execute an inner loop that iterates over each row 'R2'
        //:     (representing a distinct object value, 'W') in the table
        //:     described in P-3:
        //:
        //:   3 For each of the iterations (P-3.2):  (C-1, 3..4)
        //:
        //:     1 Use the value constructor to create a modifiable 'Obj', 'mX',
        //:       having the value 'W'.
        //:
        //:     2 Assign 'mX' from 'Z'.
        //:
        //:     3 Verify that the address of the return value is the same as
        //:       that of 'mX'.  (C-3)
        //:
        //:     4 Use the equality-comparison operator to verify that: (C-1, 4)
        //:
        //:       1 The target object, 'mX', now has the same value as that of
        //:         'Z'.  (C-1)
        //:
        //:       2 'Z' still has the same value as that of 'ZZ'.  (C-4)
        //:
        //: 4 Repeat steps similar to those described in P-2 except that, this
        //:   time, the source object, 'Z', is a reference to the target
        //:   object, 'mX', and both 'mX' and 'ZZ' are initialized to have the
        //:   value 'V'.  For each row (representing a distinct object value,
        //:   'V') in the table described in P-2:  (C-5)
        //:
        //:   1 Use the value constructor to create a modifiable 'Obj' 'mX';
        //:     also use the value constructor to create a 'const' 'Obj' 'ZZ'.
        //:
        //:   2 Let 'Z' be a reference providing only 'const' access to 'mX'.
        //:
        //:   3 Assign 'mX' from 'Z'.
        //:
        //:   4 Verify that the address of the return value is the same as that
        //:     of 'mX'.  (C-3)
        //:
        //:   5 Use the equality-comparison operator to verify that the
        //:     target object, 'Z', still has the same value as that of 'ZZ'.
        //:     (C-5)
        //
        // Testing:
        //   operator=(const bteso_LingerOptions& rhs);
        // --------------------------------------------------------------------

        if (verbose) cout << endl
                          << "COPY-ASSIGNMENT OPERATOR" << endl
                          << "========================" << endl;

        if (verbose) cout <<
                 "\nAssign the address of the operator to a variable." << endl;
        {
            typedef Obj& (Obj::*operatorPtr)(const Obj&);

            // Verify that the signature and return type are standard.

            operatorPtr operatorAssignment = &Obj::operator=;

            (void)operatorAssignment;  // quash potential compiler warning
        }

        if (verbose) cout << "\nUse table of distinct object values." << endl;

        const int NUM_DATA                     = DEFAULT_NUM_DATA;
        const DefaultDataRow (&DATA)[NUM_DATA] = DEFAULT_DATA;

        if (verbose) cout <<
                         "\nCopy-assign every value into every value." << endl;

        for (int ti = 0; ti < NUM_DATA; ++ti) {
            const int  LINE1    = DATA[ti].d_line;
            const int  TIMEOUT1 = DATA[ti].d_timeout;
            const bool FLAG1    = DATA[ti].d_lingerFlag;

            const Obj  Z(TIMEOUT1, FLAG1);
            const Obj ZZ(TIMEOUT1, FLAG1);

            if (veryVerbose) { T_ P_(LINE1) P_(Z) P(ZZ) }

            // Ensure the first row of the table contains the
            // default-constructed value.

            static bool firstFlag = true;
            if (firstFlag) {
                LOOP3_ASSERT(LINE1, Obj(), Z, Obj() == Z);
                firstFlag = false;
            }

            for (int tj = 0; tj < NUM_DATA; ++tj) {
                const int  LINE2    = DATA[tj].d_line;
                const int  TIMEOUT2 = DATA[tj].d_timeout;
                const bool FLAG2    = DATA[tj].d_lingerFlag;

                Obj mX(TIMEOUT2, FLAG2);  const Obj& X = mX;

                if (veryVerbose) { T_ P_(LINE2) P(X) }

                LOOP4_ASSERT(LINE1, LINE2, Z, X, (Z == X) == (LINE1 == LINE2));

                Obj *mR = &(mX = Z);
                LOOP4_ASSERT(LINE1, LINE2,  Z,   X,  Z == X);
                LOOP4_ASSERT(LINE1, LINE2, mR, &mX, mR == &mX);

                LOOP4_ASSERT(LINE1, LINE2, ZZ, Z, ZZ == Z);
            }

            if (verbose) cout << "Testing self-assignment" << endl;

            {
                      Obj mX(TIMEOUT1, FLAG1);
                const Obj ZZ(TIMEOUT1, FLAG1);

                const Obj& Z = mX;

                LOOP3_ASSERT(LINE1, ZZ, Z, ZZ == Z);

                Obj *mR = &(mX = Z);
                LOOP3_ASSERT(LINE1, ZZ,   Z, ZZ == Z);
                LOOP3_ASSERT(LINE1, mR, &mX, mR == &mX);
            }
        }
      } break;
      case 8: {
        // --------------------------------------------------------------------
        // SWAP MEMBER AND FREE FUNCTIONS
        //   Ensure that, when member and free 'swap' are implemented, we can
        //   exchange the values of any two objects.
        //
        // Concerns:
        //   N/A
        //
        // Plan:
        //   N/A
        //
        // Testing:
        //  Reservered for 'swap' testing.
        // --------------------------------------------------------------------

      } break;
      case 7: {
        // --------------------------------------------------------------------
        // COPY CONSTRUCTOR
        //   Ensure that we can create a distinct object of the class from any
        //   other one, such that the two objects have the same value.
        //
        // Concerns:
        //: 1 The copy constructor creates an object having the same value as
        //:   that of the supplied original object.
        //:
        //: 2 The original object is passed as a reference providing
        //:   non-modifiable access to that object.
        //:
        //: 3 The value of the original object is unchanged.
        //
        // Plan:
        //: 1 Using the table-driven technique, specify a set of (unique) valid
        //:   object values (one per row) in terms of their individual
        //:   attributes, including (a) first, the default value, and (b)
        //:   boundary values corresponding to every range of values that each
        //:   individual attribute can independently attain.
        //:
        //: 2 For each row (representing a distinct object value, 'V') in the
        //:   table described in P-1:  (C-1..3)
        //:
        //:   1 Use the value constructor to create two 'const' 'Obj', 'Z' and
        //:     'ZZ', each having the value 'V'.
        //:
        //:   2 Use the copy constructor to create an object 'X',
        //:     supplying it the 'const' object 'Z'.  (C-2)
        //:
        //:   3 Use the equality-comparison operator to verify that:
        //:     (C-1, 3)
        //:
        //:     1 The newly constructed object, 'X', has the same value as 'Z'.
        //:       (C-1)
        //:
        //:     2 'Z' still has the same value as 'ZZ'.  (C-3)
        //
        // Testing:
        //   bteso_LingerOptions(const bteso_LingerOptions& o);
        // --------------------------------------------------------------------

        if (verbose) cout << endl
                          << "COPY CONSTRUCTOR" << endl
                          << "================" << endl;

        if (verbose) cout << "\nUse table of distinct object values." << endl;

        const int NUM_DATA                     = DEFAULT_NUM_DATA;
        const DefaultDataRow (&DATA)[NUM_DATA] = DEFAULT_DATA;

        if (verbose) cout <<
                        "\nCopy construct an object from every value." << endl;

        for (int ti = 0; ti < NUM_DATA; ++ti) {
            const int  LINE    = DATA[ti].d_line;
            const int  TIMEOUT = DATA[ti].d_timeout;
            const bool FLAG    = DATA[ti].d_lingerFlag;

            const Obj  Z(TIMEOUT, FLAG);
            const Obj ZZ(TIMEOUT, FLAG);

            if (veryVerbose) { T_ P_(Z) P(ZZ) }

            Obj mX(Z);  const Obj& X = mX;

            if (veryVerbose) { T_ T_ P(X) }

            // Ensure the first row of the table contains the
            // default-constructed value.

            static bool firstFlag = true;
            if (firstFlag) {
                LOOP3_ASSERT(LINE, Obj(), Z,
                             Obj() == Z)
                firstFlag = false;
            }

            // Verify the value of the object.

            LOOP3_ASSERT(LINE,  Z, X,  Z == X);

            // Verify that the value of 'Z' has not changed.

            LOOP3_ASSERT(LINE, ZZ, Z, ZZ == Z);
        }  // end foreach row

      } break;
      case 6: {
        // --------------------------------------------------------------------
        // EQUALITY-COMPARISON OPERATORS
        //   Ensure that '==' and '!=' are the operational definition of value.
        //
        // Concerns:
        //: 1 Two objects, 'X' and 'Y', compare equal if and only if each of
        //:   their corresponding salient attributes respectively compares
        //:   equal.
        //:
        //: 2 All salient attributes participate in the comparison.
        //:
        //: 3 No non-salient attributes (i.e., 'allocator') participate.
        //:
        //: 4 'true  == (X == X)'  (i.e., identity)
        //:
        //: 5 'false == (X != X)'  (i.e., identity)
        //:
        //: 6 'X == Y' if and only if 'Y == X'  (i.e., commutativity)
        //:
        //: 7 'X != Y' if and only if 'Y != X'  (i.e., commutativity)
        //:
        //: 8 'X != Y' if and only if '!(X == Y)'
        //:
        //: 9 Comparison is symmetric with respect to user-defined conversion
        //:   (i.e., both comparison operators are free functions).
        //:
        //:10 Non-modifiable objects can be compared (i.e., objects or
        //:   references providing only non-modifiable access).
        //:
        //:11 The equality operator's signature and return type are standard.
        //:
        //:12 The inequality operator's signature and return type are standard.
        //
        // Plan:
        //: 1 Use the respective addresses of 'operator==' and 'operator!=' to
        //:   initialize function pointers having the appropriate signatures
        //:   and return types for the two homogeneous, free equality-
        //:   comparison operators defined in this component.
        //:   (C-9..12)
        //:
        //: 2 Using the table-driven technique, specify a set of distinct
        //:   object values (one per row) in terms of their individual salient
        //:   attributes such that for each salient attribute, there exists a
        //:   pair of rows that differ (slightly) in only the column
        //:   corresponding to that attribute.
        //:
        //: 3 For each row 'R1' in the table of P-3:  (C-1..8)
        //:
        //:   1 Create a single object, and use it to verify the reflexive
        //:     (anti-reflexive) property of equality (inequality) in the
        //:     presence of aliasing.  (C-4..5)
        //:
        //:   2 For each row 'R2' in the table of P-3:  (C-1..3, 6..8)
        //:
        //:     1 Record, in 'EXP', whether or not distinct objects created
        //:       from 'R1' and 'R2', respectively, are expected to have the
        //:       same value.
        //:
        //:     2 Create an object 'X' having the value 'R1'.
        //:
        //:     3 Create an object 'Y' having the value 'R2'.
        //:
        //:     4 Verify the commutativity property and expected return value
        //:       for both '==' and '!='.  (C-1..3, 6..8)
        //
        // Testing:
        //   bool operator==(const bteso_LingerOptions& lhs, rhs);
        //   bool operator!=(const bteso_LingerOptions& lhs, rhs);
        // --------------------------------------------------------------------

        if (verbose) cout << endl
                          << "EQUALITY-COMPARISON OPERATORS" << endl
                          << "=============================" << endl;

        if (verbose) cout <<
                "\nAssign the address of each operator to a variable." << endl;
        {
            typedef bool (*operatorPtr)(const Obj&, const Obj&);

            // Verify that the signatures and return types are standard.

            operatorPtr operatorEq = operator==;
            operatorPtr operatorNe = operator!=;

            (void)operatorEq;  // quash potential compiler warnings
            (void)operatorNe;
        }

        if (verbose) cout <<
            "\nDefine appropriate individual attribute values, 'Ai' and 'Bi'."
                                                                       << endl;
        // ---------------
        // Attribute Types
        // ---------------

        typedef int  T1;               // 'timeout'
        typedef bool T2;               // 'lingerFlag'

        // -----------------------------
        // Attribute 1 Values: 'timeout'
        // -----------------------------

        const T1 A1 = 1;               // baseline
        const T1 B1 = INT_MAX;

        // --------------------------------
        // Attribute 2 Values: 'lingerFlag'
        // --------------------------------

        const T2 A2 = false;           // baseline
        const T2 B2 = true;

        if (verbose) cout <<
            "\nCreate a table of distinct, but similar object values." << endl;

        static const struct {
            int  d_line;        // source line number
            int  d_timeout;
            bool d_lingerFlag;
        } DATA[] = {

        // The first row of the table below represents an object value
        // consisting of "baseline" attribute values (A1..An).  Each subsequent
        // row differs (slightly) from the first in exactly one attribute
        // value (Bi).

        //LINE  TIMEOUT  FLAG
        //----  -------  ----

        { L_,       A1,   A2 },       // baseline

        { L_,       B1,   A2 },
        { L_,       A1,   B2 },

        };
        const int NUM_DATA = sizeof DATA / sizeof *DATA;

        if (verbose) cout << "\nCompare every value with every value." << endl;

        for (int ti = 0; ti < NUM_DATA; ++ti) {
            const int  LINE1    = DATA[ti].d_line;
            const int  TIMEOUT1 = DATA[ti].d_timeout;
            const bool FLAG1    = DATA[ti].d_lingerFlag;

            if (veryVerbose) { T_ P_(LINE1) P_(TIMEOUT1) P_(FLAG1) }

            // Ensure an object compares correctly with itself (alias test).
            {
                const Obj X(TIMEOUT1, FLAG1);

                LOOP2_ASSERT(LINE1, X,   X == X);
                LOOP2_ASSERT(LINE1, X, !(X != X));
            }

            for (int tj = 0; tj < NUM_DATA; ++tj) {
                const int  LINE2    = DATA[tj].d_line;
                const int  TIMEOUT2 = DATA[tj].d_timeout;
                const bool FLAG2    = DATA[tj].d_lingerFlag;

                if (veryVerbose) { T_ T_ P_(LINE2) P_(TIMEOUT2) P_(FLAG2) }

                const bool EXP = ti == tj;  // expected for equality comparison

                const Obj X(TIMEOUT1, FLAG1);
                const Obj Y(TIMEOUT2, FLAG2);

                if (veryVerbose) { T_ T_ T_ P_(EXP) P_(X) P(Y) }

                // Verify value, and commutativity.

                LOOP4_ASSERT(LINE1, LINE2, X, Y,  EXP == (X == Y));
                LOOP4_ASSERT(LINE1, LINE2, Y, X,  EXP == (Y == X));

                LOOP4_ASSERT(LINE1, LINE2, X, Y, !EXP == (X != Y));
                LOOP4_ASSERT(LINE1, LINE2, Y, X, !EXP == (Y != X));
            }
        }
      } break;
      case 5: {
        // --------------------------------------------------------------------
        // PRINT AND OUTPUT OPERATOR
        //   Ensure that the value of the object can be formatted appropriately
        //   on an 'ostream' in some standard, human-readable form.
        //
        // Concerns:
        //: 1 The 'print' method writes the value to the specified 'ostream'.
        //:
        //: 2 The 'print' method writes the value in the intended format.
        //:
        //: 3 The output using 's << obj' is the same as 'obj.print(s, 0, -1)',
        //:   but with each "attributeName = " elided.
        //:
        //: 4 The 'print' method signature and return type are standard.
        //:
        //: 5 The 'print' method returns the supplied 'ostream'.
        //:
        //: 6 The output 'operator<<' signature and return type are standard.
        //:
        //: 7 The output 'operator<<' returns the supplied 'ostream'.
        //
        // Plan:
        //: 1 Use the addresses of the 'print' member function and 'operator<<'
        //:   free function defined in this component to initialize,
        //:   respectively, member-function and free-function pointers having
        //:   the appropriate signatures and return types.  (C-4, 6)
        //:
        //: 2 Using the table-driven technique, define twelve carefully
        //:   selected combinations of (two) object values ('A' and 'B'),
        //:   having distinct values for each corresponding salient attribute,
        //:   and various values for the two formatting parameters, along with
        //:   the expected output
        //:     ( 'value' x  'level'   x 'spacesPerLevel' ):
        //:     1 { A   } x {  0     } x {  0, 1, -1 }  -->  3 expected outputs
        //:     2 { A   } x {  3, -3 } x {  0, 2, -2 }  -->  6 expected outputs
        //:     3 { B   } x {  2     } x {  3        }  -->  1 expected output
        //:     4 { A B } x { -9     } x { -9        }  -->  2 expected output
        //:
        //: 3 For each row in the table defined in P-2.1:  (C-1..3, 5, 7)
        //:
        //:   1 Using a 'const' 'Obj', supply each object value and pair of
        //:     formatting parameters to 'print', unless the parameters are,
        //:     arbitrarily, (-9, -9), in which case 'operator<<' will be
        //:     invoked instead.
        //:
        //:   2 Use a standard 'ostringstream' to capture the actual output.
        //:
        //:   3 Verify the address of what is returned is that of the
        //:     supplied stream.  (C-5, 7)
        //:
        //:   4 Compare the contents captured in P-2.2.2 with what is
        //:     expected.  (C-1..3)
        //
        // Testing:
        //   ostream& print(ostream& s, int level = 0, int sPL = 4) const;
        //   operator<<(ostream& s, const bteso_LingerOptions& d);
        // --------------------------------------------------------------------

        if (verbose) cout << endl
                          << "PRINT AND OUTPUT OPERATOR" << endl
                          << "=========================" << endl;

        if (verbose) cout << "\nAssign the addresses of 'print' and "
                             "the output 'operator<<' to variables." << endl;
        {
            typedef ostream& (Obj::*funcPtr)(ostream&, int, int) const;
            typedef ostream& (*operatorPtr)(ostream&, const Obj&);

            // Verify that the signatures and return types are standard.

            funcPtr     printMember = &Obj::print;
            operatorPtr operatorOp  = operator<<;

            (void)printMember;  // quash potential compiler warnings
            (void)operatorOp;
        }

        if (verbose) cout <<
             "\nCreate a table of distinct value/format combinations." << endl;

        static const struct {
            int         d_line;           // source line number
            int         d_level;
            int         d_spacesPerLevel;

            int         d_timeout;
            bool        d_lingerFlag;

            const char *d_expected_p;
        } DATA[] = {

#define NL "\n"
#define SP " "

        // ------------------------------------------------------------------
        // P-2.1.1: { A } x { 0 }     x { 0, 1, -1 }  -->  3 expected outputs
        // ------------------------------------------------------------------

        //LINE L SPL  OFF   FLAG  EXP
        //---- - ---  ---   ----  ---

        { L_,  0,  0,  89,  true, "["                                        NL
                                  "timeout = 89"                             NL
                                  "lingerFlag = true"                        NL
                                  "]"                                        NL
                                                                             },

        { L_,  0,  1,  89,  true, "["                                        NL
                                  " timeout = 89"                            NL
                                  " lingerFlag = true"                       NL
                                  "]"                                        NL
                                                                             },

        { L_,  0, -1,  89,  true, "["                                        SP
                                  "timeout = 89"                             SP
                                  "lingerFlag = true"                        SP
                                  "]"
                                                                             },

        // ------------------------------------------------------------------
        // P-2.1.2: { A } x { 3, -3 } x { 0, 2, -2 }  -->  6 expected outputs
        // ------------------------------------------------------------------

        //LINE L SPL  OFF   FLAG  EXP
        //---- - ---  ---   ----  ---

        { L_,  3,  0,  89,  true, "["                                        NL
                                  "timeout = 89"                             NL
                                  "lingerFlag = true"                        NL
                                  "]"                                        NL
                                                                             },

        { L_,  3,  2,  89,  true, "      ["                                  NL
                                  "        timeout = 89"                     NL
                                  "        lingerFlag = true"                NL
                                  "      ]"                                  NL
                                                                             },

        { L_,  3, -2,  89,  true, "      ["                                  SP
                                  "timeout = 89"                             SP
                                  "lingerFlag = true"                        SP
                                  "]"
                                                                             },

        { L_, -3,  0,  89,  true, "["                                        NL
                                  "timeout = 89"                             NL
                                  "lingerFlag = true"                        NL
                                  "]"                                        NL
                                                                             },

        { L_, -3,  2,  89,  true, "["                                        NL
                                  "        timeout = 89"                     NL
                                  "        lingerFlag = true"                NL
                                  "      ]"                                  NL
                                                                             },

        { L_, -3, -2,  89,  true, "["                                        SP
                                  "timeout = 89"                             SP
                                  "lingerFlag = true"                        SP
                                  "]"
                                                                             },
        // -----------------------------------------------------------------
        // P-2.1.3: { B } x { 2 }     x { 3 }         -->  1 expected output
        // -----------------------------------------------------------------

        //LINE L SPL  OFF   FLAG  EXP
        //---- - ---  ---   ----  ---

        { L_,  2,  3,   7, false, "      ["                                  NL
                                  "         timeout = 7"                     NL
                                  "         lingerFlag = false"              NL
                                  "      ]"                                  NL
                                                                             },

        // -----------------------------------------------------------------
        // P-2.1.4: { A B } x { -9 }   x { -9 }      -->  2 expected outputs
        // -----------------------------------------------------------------

        //LINE L SPL  OFF   FLAG  EXP
        //---- - ---  ---   ----  ---

        { L_, -9, -9,  89,  true, "[ 89 true ]"                              },

        { L_, -9, -9,   7, false, "[ 7 false ]"                              },

#undef NL
#undef SP

        };
        const int NUM_DATA = sizeof DATA / sizeof *DATA;

        if (verbose) cout << "\nTesting with various print specifications."
                          << endl;
        {
            for (int ti = 0; ti < NUM_DATA; ++ti) {
                const int         LINE   = DATA[ti].d_line;
                const int         L      = DATA[ti].d_level;
                const int         SPL    = DATA[ti].d_spacesPerLevel;
                const int         OFF    = DATA[ti].d_timeout;
                const bool        FLAG   = DATA[ti].d_lingerFlag;
                const char *const EXP    = DATA[ti].d_expected_p;

                if (veryVerbose) { T_ P_(L) P_(SPL) P_(OFF) P_(FLAG)}

                if (veryVeryVerbose) { T_ T_ Q(EXPECTED) cout << EXP; }

                const Obj X(OFF, FLAG);

                ostringstream os;

                if (-9 == L && -9 == SPL) {

                    // Verify supplied stream is returned by reference.

                    LOOP_ASSERT(LINE, &os == &(os << X));

                    if (veryVeryVerbose) { T_ T_ Q(operator<<) }
                }
                else {

                    // Verify supplied stream is returned by reference.

                    LOOP_ASSERT(LINE, &os == &X.print(os, L, SPL));

                    if (veryVeryVerbose) { T_ T_ Q(print) }
                }

                // Verify output is formatted as expected.

                if (veryVeryVerbose) { P(os.str()) }

                LOOP3_ASSERT(LINE, EXP, os.str(), EXP == os.str());
            }
        }
      } break;
      case 4: {
        // --------------------------------------------------------------------
        // BASIC ACCESSORS
        //   Ensure each basic accessor properly interprets object state.
        //
        // Concerns:
        //: 1 Each accessor returns the value of the corresponding attribute
        //:   of the object.
        //:
        //: 2 Each accessor method is declared 'const'.
        //
        // Plan:
        //   In case 3 we demonstrated that all basic accessors work properly
        //   with respect to attributes initialized by the value constructor.
        //   Here we use the default constructor and primary manipulators,
        //   which were fully tested in case 2, to further corroborate that
        //   these accessors are properly interpreting object state.
        //
        //: 1 Use the default constructor to create an object (having default
        //:   attribute values).
        //:
        //: 2 Verify that each basic accessor, invoked on a reference providing
        //:   non-modifiable access to the object created in P2, returns the
        //:   expected value.  (C-2)
        //:
        //: 3 For each salient attribute (contributing to value):  (C-1)
        //:   1 Use the corresponding primary manipulator to set the attribute
        //:     to a unique value.
        //:
        //:   2 Use the corresponding basic accessor to verify the new
        //:     expected value.  (C-1)
        //
        // Testing:
        //   bool lingerFlag() const;
        //   int timeout() const;
        // --------------------------------------------------------------------

        if (verbose) cout << endl
                          << "BASIC ACCESSORS" << endl
                          << "===============" << endl;

        // Attribute Types

        typedef int  T1;     // 'timeout'
        typedef bool T2;     // 'lingerFlag'

        if (verbose) cout << "\nEstablish suitable attribute values." << endl;

        // -----------------------------------------------------
        // 'D' values: These are the default-constructed values.
        // -----------------------------------------------------

        const T1 D1   = 0;        // 'timeout'
        const T2 D2   = false;    // 'lingerFlag'

        // -------------------------------------------------------
        // 'A' values: Boundary values.
        // -------------------------------------------------------

        const T1 A1   = INT_MAX;
        const T2 A2   = true;

        if (verbose) cout << "\nCreate an object." << endl;

        Obj mX;  const Obj& X = mX;

        if (verbose) cout <<
                "\nVerify all basic accessors report expected values." << endl;
        {
            const T1& timeout = X.timeout();
            LOOP2_ASSERT(D1, timeout, D1 == timeout);

            const T2& lingerFlag = X.lingerFlag();
            LOOP2_ASSERT(D2, lingerFlag, D2 == lingerFlag);
        }

        if (verbose) cout <<
            "\nApply primary manipulators and verify expected values." << endl;

        if (veryVerbose) { T_ Q(timeout) }
        {
            mX.setTimeout(A1);

            const T1& timeout = X.timeout();
            LOOP2_ASSERT(A1, timeout, A1 == timeout);
        }

        if (veryVerbose) { T_ Q(lingerFlag) }
        {
            mX.setLingerFlag(A2);

            const T2& lingerFlag = X.lingerFlag();
            LOOP2_ASSERT(A2, lingerFlag, A2 == lingerFlag);
        }
      } break;
      case 3: {
        // --------------------------------------------------------------------
        // VALUE CTOR
        //   Ensure that we can put an object into any initial state relevant
        //   for thorough testing.
        //
        // Concerns:
        //: 1 The value constructor can create an object having any value that
        //:   does not violate the constructor's documented preconditions.
        //:
        //: 2 Any argument can be 'const'.
        //:
        //: 3 QoI: Asserted precondition violations are detected when enabled.
        //
        // Plan:
        //: 1 Using the table-driven technique:
        //:   1 Specify a set of (unique) valid object values (one per row) in
        //:     terms of their individual attributes, including (a) first, the
        //:     default value, and (b) boundary values corresponding to every
        //:     range of values that each individual attribute can
        //:     independently attain.
        //:
        //: 2 For each row (representing a distinct object value, 'V') in the
        //:   table described in P-1: (C-1..2)
        //:
        //:   1 Use the value constructor to create an object having the value
        //:     'V' supplying all the arguments as 'const'.  (C-2)
        //:
        //:   2 Use the (as yet unproven) salient attribute accessors to verify
        //:     that all of the attributes of each object have their expected
        //:     values.  (C-1)
        //:
        //: 3 Verify that, in appropriate build modes, defensive checks are
        //:   triggered for invalid attribute values, but not triggered for
        //:   adjacent valid ones (using the 'BSLS_ASSERTTEST_*' macros).
        //:   (C-3)
        //
        // Testing:
        //   bteso_LingerOptions(int o, bool f);
        // --------------------------------------------------------------------

        if (verbose) cout << endl
                          << "VALUE CTOR" << endl
                          << "==========" << endl;

        if (verbose) cout << "\nUse table of distinct object values." << endl;

        const int NUM_DATA                     = DEFAULT_NUM_DATA;
        const DefaultDataRow (&DATA)[NUM_DATA] = DEFAULT_DATA;

        if (verbose) cout << "\nCreate an object with every value." << endl;

        for (int ti = 0; ti < NUM_DATA; ++ti) {
            const int  LINE    = DATA[ti].d_line;
            const int  TIMEOUT = DATA[ti].d_timeout;
            const bool FLAG    = DATA[ti].d_lingerFlag;

            if (veryVerbose) { T_ P_(TIMEOUT) P_(FLAG) }

            Obj mX(TIMEOUT, FLAG);  const Obj& X = mX;

            if (veryVerbose) { T_ T_ P(X) }

            // Use untested functionality to help ensure the first row
            // of the table contains the default-constructed value.

            static bool firstFlag = true;
            if (firstFlag) {
                LOOP3_ASSERT(LINE, Obj(), X, Obj() == X)
                firstFlag = false;
            }

            // -------------------------------------
            // Verify the object's attribute values.
            // -------------------------------------

            LOOP3_ASSERT(LINE, TIMEOUT, X.timeout(), TIMEOUT == X.timeout());

            LOOP3_ASSERT(LINE, FLAG, X.lingerFlag(),
                         FLAG == X.lingerFlag());
        }

        if (verbose) cout << "\nNegative Testing." << endl;
        {
            bsls_AssertFailureHandlerGuard hG(bsls_AssertTest::failTestDriver);

            if (veryVerbose) cout << "\t'timeout'" << endl;
            {
                ASSERT_SAFE_FAIL(Obj(-1, false));
                ASSERT_SAFE_PASS(Obj( 0, false));
            }
        }
      } break;
      case 2: {
        // --------------------------------------------------------------------
        // DEFAULT CTOR, PRIMARY MANIPULATORS, & DTOR
        //   Ensure that we can use the default constructor to create an
        //   object (having the default-constructed value), use the primary
        //   manipulators to put that object into any state relevant for
        //   thorough testing, and use the destructor to destroy it safely.
        //
        // Concerns:
        //: 1 An object created with the default constructor has the
        //:   contractually specified default value.
        //:
        //: 2 Any argument can be 'const'.
        //:
        //: 3 Each attribute is modifiable independently.
        //:
        //: 4 Each attribute can be set to represent any value that does not
        //:   violate that attribute's documented constraints.
        //:
        //: 5 QoI: Asserted precondition violations are detected when enabled.
        //
        // Plan:
        //: 1 Create three sets of attribute values for the object: 'D', 'A',
        //:   and 'B'.  'D' values correspond to the attribute values, and 'A'
        //:   and 'B' values are chosen to be distinct boundary values where
        //:   possible.
        //:
        //: 2 Use the default constructor to create an object 'X'.
        //:
        //: 3 Use the individual (as yet unproven) salient attribute
        //:   accessors to verify the default-constructed value.  (C-1)
        //:
        //: 4 For each attribute 'i', in turn, create a local block.  Then
        //:   inside the block, using brute force, set that attribute's
        //:   value, passing a 'const' argument representing each of the
        //:   three test values, in turn (see P-1), first to 'Ai', then to
        //:   'Bi', and finally back to 'Di'.  After each transition, use the
        //:   (as yet unproven) basic accessors to verify that only the
        //:   intended attribute value changed.  (C-2, 4)
        //:
        //: 5 Corroborate that attributes are modifiable independently by
        //:   first setting all of the attributes to their 'A' values.  Then
        //:   incrementally set each attribute to its 'B' value and verify
        //:   after each manipulation that only that attribute's value
        //:   changed.  (C-3)
        //:
        //: 6 Verify that, in appropriate build modes, defensive checks are
        //:   triggered for invalid attribute values, but not triggered for
        //:   adjacent valid ones (using the 'BSLS_ASSERTTEST_*' macros).
        //:   (C-5)
        //
        // Testing:
        //   bteso_LingerOptions();
        //   ~bteso_LingerOptions();
        //   setLingerFlag(bool value);
        //   setTimeout(int value);
        // --------------------------------------------------------------------

        if (verbose) cout << endl
                       << "DEFAULT CTOR, PRIMARY MANIPULATORS, & DTOR" << endl
                       << "==========================================" << endl;

        if (verbose) cout << "\nEstablish suitable attribute values." << endl;

        // 'D' values: These are the default-constructed values.

        const int  D1   = 0;        // 'timeout'
        const bool D2   = false;    // 'lingerFlag'

        // 'A' values.

        const int  A1   = 1;
        const bool A2   = true;

        // 'B' values.

        const int    B1 = INT_MAX;
        const bool   B2 = false;

        if (verbose) cout <<
                     "Create an object using the default constructor." << endl;

        Obj mX;  const Obj& X = mX;

        if (verbose) cout << "Verify the object's attribute values." << endl;

        // -------------------------------------
        // Verify the object's attribute values.
        // -------------------------------------

        LOOP2_ASSERT(D1, X.timeout(),    D1 == X.timeout());
        LOOP2_ASSERT(D2, X.lingerFlag(), D2 == X.lingerFlag());

        if (verbose) cout <<
               "Verify that each attribute is independently settable." << endl;

        // ---------
        // 'timeout'
        // ---------
        {
            mX.setTimeout(A1);
            ASSERT(A1 == X.timeout());
            ASSERT(D2 == X.lingerFlag());

            mX.setTimeout(B1);
            ASSERT(B1 == X.timeout());
            ASSERT(D2 == X.lingerFlag());

            mX.setTimeout(D1);
            ASSERT(D1 == X.timeout());
            ASSERT(D2 == X.lingerFlag());
        }

        // ------------
        // 'lingerFlag'
        // ------------
        {
            mX.setLingerFlag(A2);
            ASSERT(D1 == X.timeout());
            ASSERT(A2 == X.lingerFlag());

            mX.setLingerFlag(B2);
            ASSERT(D1 == X.timeout());
            ASSERT(B2 == X.lingerFlag());

            mX.setLingerFlag(D2);
            ASSERT(D1 == X.timeout());
            ASSERT(D2 == X.lingerFlag());
        }

        if (verbose) cout << "Corroborate attribute independence." << endl;
        {
            // ---------------------------------------
            // Set all attributes to their 'A' values.
            // ---------------------------------------

            mX.setTimeout(A1);
            mX.setLingerFlag(A2);

            ASSERT(A1 == X.timeout());
            ASSERT(A2 == X.lingerFlag());

            // ---------------------------------------
            // Set all attributes to their 'B' values.
            // ---------------------------------------

            mX.setTimeout(B1);

            ASSERT(B1 == X.timeout());
            ASSERT(A2 == X.lingerFlag());

            mX.setLingerFlag(B2);

            ASSERT(B1 == X.timeout());
            ASSERT(B2 == X.lingerFlag());
        }

        if (verbose) cout << "\nNegative Testing." << endl;
        {
            bsls_AssertFailureHandlerGuard hG(bsls_AssertTest::failTestDriver);

            Obj obj;

            if (veryVerbose) cout << "\ttimeout" << endl;
            {
                ASSERT_SAFE_FAIL(obj.setTimeout(-1));
                ASSERT_SAFE_PASS(obj.setTimeout( 0));
            }
        }
      } break;
      case 1: {
        // --------------------------------------------------------------------
        // BREATHING TEST
        //   This case exercises (but does not fully test) basic functionality.
        //
        // Concerns:
        //: 1 The class is sufficiently functional to enable comprehensive
        //:   testing in subsequent test cases.
        //
        // Plan:
        //: 1 Create an object 'w' (default ctor).       { w:D             }
        //: 2 Create an object 'x' (copy from 'w').      { w:D x:D         }
        //: 3 Set 'x' to 'A' (value distinct from 'D').  { w:D x:A         }
        //: 4 Create an object 'y' (init. to 'A').       { w:D x:A y:A     }
        //: 5 Create an object 'z' (copy from 'y').      { w:D x:A y:A z:A }
        //: 6 Set 'z' to 'D' (the default value).        { w:D x:A y:A z:D }
        //: 7 Assign 'w' from 'x'.                       { w:A x:A y:A z:D }
        //: 8 Assign 'w' from 'z'.                       { w:D x:A y:A z:D }
        //: 9 Assign 'x' from 'x' (aliasing).            { w:D x:A y:A z:D }
        //
        // Testing:
        //   BREATHING TEST
        // --------------------------------------------------------------------

        if (verbose) cout << endl
                          << "BREATHING TEST" << endl
                          << "==============" << endl;

        // Attribute Types

        typedef int  T1;        // 'timeout'
        typedef bool T2;        // 'lingerFlag'

        // Attribute 1 Values: 'timeout'

        const T1 D1 = 0;        // default value
        const T1 A1 = 60;

        // Attribute 2 Values: 'lingerFlag'

        const T2 D2 = false;    // default value
        const T2 A2 = true;

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

        if (verbose) cout << "\n 1. Create an object 'w' (default ctor)."
                             "\t\t{ w:D             }" << endl;

        Obj mW;  const Obj& W = mW;

        if (veryVerbose) cout << "\ta. Check initial value of 'w'." << endl;
        if (veryVeryVerbose) { T_ T_ P(W) }

        ASSERT(D1 == W.timeout());
        ASSERT(D2 == W.lingerFlag());

        if (veryVerbose) cout <<
                  "\tb. Try equality operators: 'w' <op> 'w'." << endl;

        ASSERT(1 == (W == W));        ASSERT(0 == (W != W));

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

        if (verbose) cout << "\n 2. Create an object 'x' (copy from 'w')."
                             "\t\t{ w:D x:D         }" << endl;

        Obj mX(W);  const Obj& X = mX;

        if (veryVerbose) cout << "\ta. Check initial value of 'x'." << endl;
        if (veryVeryVerbose) { T_ T_ P(X) }

        ASSERT(D1 == X.timeout());
        ASSERT(D2 == X.lingerFlag());

        if (veryVerbose) cout <<
                   "\tb. Try equality operators: 'x' <op> 'w', 'x'." << endl;

        ASSERT(1 == (X == W));        ASSERT(0 == (X != W));
        ASSERT(1 == (X == X));        ASSERT(0 == (X != X));

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

        if (verbose) cout << "\n 3. Set 'x' to 'A' (value distinct from 'D')."
                             "\t\t{ w:D x:A         }" << endl;

        mX.setTimeout(A1);
        mX.setLingerFlag(A2);

        if (veryVerbose) cout << "\ta. Check new value of 'x'." << endl;
        if (veryVeryVerbose) { T_ T_ P(X) }

        ASSERT(A1 == X.timeout());
        ASSERT(A2 == X.lingerFlag());

        if (veryVerbose) cout <<
             "\tb. Try equality operators: 'x' <op> 'w', 'x'." << endl;

        ASSERT(0 == (X == W));        ASSERT(1 == (X != W));
        ASSERT(1 == (X == X));        ASSERT(0 == (X != X));

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

        if (verbose) cout << "\n 4. Create an object 'y' (init. to 'A')."
                             "\t\t{ w:D x:A y:A     }" << endl;

        Obj mY(A1, A2);  const Obj& Y = mY;

        if (veryVerbose) cout << "\ta. Check initial value of 'y'." << endl;
        if (veryVeryVerbose) { T_ T_ P(Y) }

        ASSERT(A1 == Y.timeout());
        ASSERT(A2 == Y.lingerFlag());

        if (veryVerbose) cout <<
             "\tb. Try equality operators: 'y' <op> 'w', 'x', 'y'" << endl;

        ASSERT(0 == (Y == W));        ASSERT(1 == (Y != W));
        ASSERT(1 == (Y == X));        ASSERT(0 == (Y != X));
        ASSERT(1 == (Y == Y));        ASSERT(0 == (Y != Y));

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

        if (verbose) cout << "\n 5. Create an object 'z' (copy from 'y')."
                             "\t\t{ w:D x:A y:A z:A }" << endl;

        Obj mZ(Y);  const Obj& Z = mZ;

        if (veryVerbose) cout << "\ta. Check initial value of 'z'." << endl;
        if (veryVeryVerbose) { T_ T_ P(Z) }

        ASSERT(A1 == Z.timeout());
        ASSERT(A2 == Z.lingerFlag());

        if (veryVerbose) cout <<
           "\tb. Try equality operators: 'z' <op> 'w', 'x', 'y', 'z'." << endl;

        ASSERT(0 == (Z == W));        ASSERT(1 == (Z != W));
        ASSERT(1 == (Z == X));        ASSERT(0 == (Z != X));
        ASSERT(1 == (Z == Y));        ASSERT(0 == (Z != Y));
        ASSERT(1 == (Z == Z));        ASSERT(0 == (Z != Z));

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

        if (verbose) cout << "\n 6. Set 'z' to 'D' (the default value)."
                             "\t\t\t{ w:D x:A y:A z:D }" << endl;

        mZ.setTimeout(D1);
        mZ.setLingerFlag(D2);

        if (veryVerbose) cout << "\ta. Check new value of 'z'." << endl;
        if (veryVeryVerbose) { T_ T_ P(Z) }

        ASSERT(D1 == Z.timeout());
        ASSERT(D2 == Z.lingerFlag());

        if (veryVerbose) cout <<
           "\tb. Try equality operators: 'z' <op> 'w', 'x', 'y', 'z'." << endl;

        ASSERT(1 == (Z == W));        ASSERT(0 == (Z != W));
        ASSERT(0 == (Z == X));        ASSERT(1 == (Z != X));
        ASSERT(0 == (Z == Y));        ASSERT(1 == (Z != Y));
        ASSERT(1 == (Z == Z));        ASSERT(0 == (Z != Z));

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

        if (verbose) cout << "\n 7. Assign 'w' from 'x'."
                             "\t\t\t\t{ w:A x:A y:A z:D }" << endl;
        mW = X;

        if (veryVerbose) cout << "\ta. Check new value of 'w'." << endl;
        if (veryVeryVerbose) { T_ T_ P(W) }

        ASSERT(A1 == W.timeout());
        ASSERT(A2 == W.lingerFlag());

        if (veryVerbose) cout <<
           "\tb. Try equality operators: 'w' <op> 'w', 'x', 'y', 'z'." << endl;

        ASSERT(1 == (W == W));        ASSERT(0 == (W != W));
        ASSERT(1 == (W == X));        ASSERT(0 == (W != X));
        ASSERT(1 == (W == Y));        ASSERT(0 == (W != Y));
        ASSERT(0 == (W == Z));        ASSERT(1 == (W != Z));

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

        if (verbose) cout << "\n 8. Assign 'w' from 'z'."
                             "\t\t\t\t{ w:D x:A y:A z:D }" << endl;
        mW = Z;

        if (veryVerbose) cout << "\ta. Check new value of 'w'." << endl;
        if (veryVeryVerbose) { T_ T_ P(W) }

        ASSERT(D1 == W.timeout());
        ASSERT(D2 == W.lingerFlag());

        if (veryVerbose) cout <<
           "\tb. Try equality operators: 'x' <op> 'w', 'x', 'y', 'z'." << endl;

        ASSERT(1 == (W == W));        ASSERT(0 == (W != W));
        ASSERT(0 == (W == X));        ASSERT(1 == (W != X));
        ASSERT(0 == (W == Y));        ASSERT(1 == (W != Y));
        ASSERT(1 == (W == Z));        ASSERT(0 == (W != Z));

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

        if (verbose) cout << "\n 9. Assign 'x' from 'x' (aliasing)."
                             "\t\t\t{ w:D x:A y:A z:D }" << endl;
        mX = X;

        if (veryVerbose) cout << "\ta. Check (same) value of 'x'." << endl;
        if (veryVeryVerbose) { T_ T_ P(X) }

        ASSERT(A1 == X.timeout());
        ASSERT(A2 == X.lingerFlag());

        if (veryVerbose) cout <<
           "\tb. Try equality operators: 'x' <op> 'w', 'x', 'y', 'z'." << endl;

        ASSERT(0 == (X == W));        ASSERT(1 == (X != W));
        ASSERT(1 == (X == X));        ASSERT(0 == (X != X));
        ASSERT(1 == (X == Y));        ASSERT(0 == (X != Y));
        ASSERT(0 == (X == Z));        ASSERT(1 == (X != Z));

      } break;
      default: {
        cerr << "WARNING: CASE `" << test << "' NOT FOUND." << endl;
        testStatus = -1;
      }
    }

    // CONCERN: In no case is memory allocated from the global allocator.

    LOOP_ASSERT(globalAllocator.numBlocksTotal(),
                0 == globalAllocator.numBlocksTotal());

    if (testStatus > 0) {
        cerr << "Error, non-zero test status = " << testStatus << "." << endl;
    }

    return testStatus;
}

// ----------------------------------------------------------------------------
// NOTICE:
//      Copyright (C) Bloomberg L.P., 2011
//      All Rights Reserved.
//      Property of Bloomberg L.P. (BLP)
//      This software is made available solely pursuant to the
//      terms of a BLP license agreement which governs its use.
// ----------------------------- END-OF-FILE ----------------------------------
