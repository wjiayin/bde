// balxml_encodingstyle.h   -*-C++-*-   GENERATED FILE -- DO NOT EDIT
#ifndef INCLUDED_BALXML_ENCODINGSTYLE
#define INCLUDED_BALXML_ENCODINGSTYLE

#ifndef INCLUDED_BSLS_IDENT
#include <bsls_ident.h>
#endif
BSLS_IDENT_RCSID(baexml_encodingstyle_h,"$Id$ $CSID$ $CCId$")
BSLS_IDENT_PRAGMA_ONCE

//@PURPOSE: Provide value-semantic attribute classes
//
//@AUTHOR: Rohan Bhindwale (rbhindwale@bloomberg.net)

#ifndef INCLUDED_BALSCM_VERSION
#include <balscm_version.h>
#endif

#ifndef INCLUDED_BSLALG_TYPETRAITS
#include <bslalg_typetraits.h>
#endif

#ifndef INCLUDED_BDLAT_ATTRIBUTEINFO
#include <bdlat_attributeinfo.h>
#endif

#ifndef INCLUDED_BDLAT_ENUMERATORINFO
#include <bdlat_enumeratorinfo.h>
#endif

#ifndef INCLUDED_BDLAT_TYPETRAITS
#include <bdlat_typetraits.h>
#endif

#ifndef INCLUDED_BSLS_ASSERT
#include <bsls_assert.h>
#endif

#ifndef INCLUDED_BSL_IOSFWD
#include <bsl_iosfwd.h>
#endif

#ifndef INCLUDED_BSL_OSTREAM
#include <bsl_ostream.h>
#endif

#ifndef INCLUDED_BSL_STRING
#include <bsl_string.h>
#endif

namespace BloombergLP {

namespace balxml {

                            // ==========================
                            // class EncodingStyle
                            // ==========================

struct EncodingStyle {
    // Enumeration of encoding style (COMPACT or PRETTY).
    // This struct is generated using baexml_generateoptions.pl

  public:
    // TYPES
    enum Value {
        e_COMPACT = 0
      , e_PRETTY  = 1
#ifndef BDE_OMIT_INTERNAL_DEPRECATED
      , BAEXML_COMPACT = e_COMPACT
      , BAEXML_PRETTY = e_PRETTY
      , COMPACT = BAEXML_COMPACT
      , PRETTY  = BAEXML_PRETTY
#endif // BDE_OMIT_INTERNAL_DEPRECATED
    };

    enum {
        k_NUM_ENUMERATORS = 2
#ifndef BDE_OMIT_INTERNAL_DEPRECATED
      , NUM_ENUMERATORS = k_NUM_ENUMERATORS
#endif  // BDE_OMIT_INTERNAL_DEPRECATED
    };

    // CONSTANTS
    static const char CLASS_NAME[];

    static const bdeat_EnumeratorInfo ENUMERATOR_INFO_ARRAY[];

    // CLASS METHODS
    static const char *toString(Value value);
        // Return the string representation exactly matching the enumerator
        // name corresponding to the specified enumeration 'value'.

    static int fromString(Value        *result,
                          const char   *string,
                          int           stringLength);
        // Load into the specified 'result' the enumerator matching the
        // specified 'string' of the specified 'stringLength'.  Return 0 on
        // success, and a non-zero value with no effect on 'result' otherwise
        // (i.e., 'string' does not match any enumerator).

    static int fromString(Value              *result,
                          const bsl::string&  string);
        // Load into the specified 'result' the enumerator matching the
        // specified 'string'.  Return 0 on success, and a non-zero value with
        // no effect on 'result' otherwise (i.e., 'string' does not match any
        // enumerator).

    static int fromInt(Value *result, int number);
        // Load into the specified 'result' the enumerator matching the
        // specified 'number'.  Return 0 on success, and a non-zero value with
        // no effect on 'result' otherwise (i.e., 'number' does not match any
        // enumerator).

    static bsl::ostream& print(bsl::ostream& stream, Value value);
        // Write to the specified 'stream' the string representation of
        // the specified enumeration 'value'.  Return a reference to
        // the modifiable 'stream'.
};

// FREE OPERATORS
inline
bsl::ostream& operator<<(bsl::ostream& stream, EncodingStyle::Value rhs);
    // Format the specified 'rhs' to the specified output 'stream' and
    // return a reference to the modifiable 'stream'.


// TRAITS
}

BDLAT_DECL_ENUMERATION_TRAITS(balxml::EncodingStyle)

namespace balxml {


// ============================================================================
//                         INLINE FUNCTION DEFINITIONS
// ============================================================================


                            // --------------------------
                            // class EncodingStyle
                            // --------------------------

// CLASS METHODS
inline
int EncodingStyle::fromString(Value *result, const bsl::string& string)
{
    return fromString(result,
                      string.c_str(),
                      static_cast<int>(string.length()));
}

inline
bsl::ostream& EncodingStyle::print(bsl::ostream&      stream,
                                 EncodingStyle::Value value)
{
    return stream << toString(value);
}
}  // close package namespace


// FREE FUNCTIONS

inline
bsl::ostream& balxml::operator<<(
        bsl::ostream& stream,
        EncodingStyle::Value rhs)
{
    return EncodingStyle::print(stream, rhs);
}

}  // close namespace BloombergLP
#endif

// GENERATED BY BLP_BAS_CODEGEN_3.6.1 Mon Jan  3 12:39:26 2011
// ----------------------------------------------------------------------------
// NOTICE:
//      Copyright (C) Bloomberg L.P., 2011
//      All Rights Reserved.
//      Property of Bloomberg L.P. (BLP)
//      This software is made available solely pursuant to the
//      terms of a BLP license agreement which governs its use.
// ------------------------------ END-OF-FILE ---------------------------------
