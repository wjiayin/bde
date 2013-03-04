// memory.h                                                           -*-C++-*-

//@PURPOSE: Provide backwards compatibility for STLPort header
//
//@SEE_ALSO: package bsl+stdhdrs
//
//@AUTHOR: Arthur Chiu (achiu21)
//
//@DESCRIPTION: This component provides backwards compatibility for Bloomberg
// libraries that depend on non-standard STLPort header files.  This header
// includes the corresponding C++ header and provides any additional symbols
// that the STLPort header would define via transitive includes.

/* STLPort reasoning: */
/* The header <memory.h> is not part of either the C or C++ standards, but is
 * in widespread use none the less.  It contains a portion of what is included
 * in the standard C header, <string.h>.  In the Sun compiler, <memory.h> is
 * broken wrt the use of namespaces, but <string.h> is fine.  Therefore, the
 * easiest way to provide the functionality of <memory.h> is to simply forward
 * it to <string.h>
 */

#include <bsl_cstring.h>

using std::memmove;
using std::memcpy;
using std::memchr;
using std::memcmp;
using std::memset;

// ----------------------------------------------------------------------------
// Copyright (C) 2013 Bloomberg L.P.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
// ----------------------------- END-OF-FILE ----------------------------------
