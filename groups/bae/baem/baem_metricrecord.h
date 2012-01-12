// baem_metricrecord.h              -*-C++-*-
#ifndef INCLUDED_BAEM_METRICRECORD
#define INCLUDED_BAEM_METRICRECORD

#ifndef INCLUDED_BDES_IDENT
#include <bdes_ident.h>
#endif
BDES_IDENT("$Id: baem_metricrecord.h,v 1.8 2008/04/16 20:00:49 hversche Exp $")

//@PURPOSE: Provide an aggregated record of the value of a metric.
//
//@CLASSES:
//   baem_MetricRecord: an aggregated record of the value of a metric
//
//@SEE_ALSO: baem_metricregistry, baem_metric, baem_metricsmanager
//
//@AUTHOR: Henry Verschell (hverschell)
//
//@DESCRIPTION: This component implements an unconstrainted pure-attribute
// class used to represent the aggregated value of a metric.  A
// 'baem_MetricRecord' contains a 'baem_MetricId' object, and a set of
// aggregate values summarizing the recorded values for that metric.  The
// attributes held by 'baem_MetricRecord' are given in the following table:
//..
//  Attribute      Type                    Description             Default
//  ----------   ---------        ---------------------------      -------
//   metricId     baem_MetricId    identifies the metric            invalid id
//   count        int              count of metric values           0
//   total        double           total of metric values           0.0
//   min          double           minimum metric value             Infinity
//   max          double           maximum metric value             -Infinity
//..
//
///Thread Safety
///-------------
// 'baem_MetricRecord' is *const* *thread-safe*, meaning that accessors may be
// invoked concurrently from different threads, but it is not safe to
// access or modify a 'baem_MetricRecord' in one thread while another thread
// modifies the same object.
//
///Usage
///-----
// The following example demonstrates how a 'baem_MetricRecord' can be used
// to describe a set of metric values.  In the example we create a
// 'RequestProcessor' class that collects information about the sizes of the
// requests it has processed, and provides a 'loadRequestSizeInformation'
// method that populates a 'baem_MetricRecord' containing that information.
//..
//  class RequestProcessor {
//      // This class defines a request processor that provides metric
//      // information about the sizes of the requests it has processed.
//
//      // DATA
//      unsigned int d_numRequests;       // number of requests processed
//      unsigned int d_totalRequestSize;  // total size of the requests
//      unsigned int d_minRequestSize;    // minimum request size
//      unsigned int d_maxRequestSize;    // maximum request size
//
//    public:
//      // CREATORS
//      RequestProcessor()
//          // Create this 'RequestProcessor'.
//      : d_numRequests(0)
//      , d_totalRequestSize(0)
//      , d_minRequestSize(INT_MAX)
//      , d_maxRequestSize(0)
//      {
//      }
//
//      // ...
//
//      // MANIPULATORS
//      void processRequest(const bsl::string& request)
//          // Process the specified 'request'.
//      {
//          ++d_numRequests;
//          d_totalRequestSize += request.size();
//          d_minRequestSize   =  bsl::min(d_minRequestSize, request.size());
//          d_maxRequestSize   =  bsl::max(d_maxRequestSize, request.size());
//
//          // Process the request.
//
//      }
//..
// Now we declare a function that populates a 'baem_MetricRecord' describing
// the sizes of the requests that the request processor has processed:
//..
//      // ACCESSORS
//      void loadRequestSizeInformation(baem_MetricRecord *record) const
//          // Populate the specified 'record' with information regarding
//          // the sizes of the requests that have been processed by this
//          // object.
//      {
//          if (0 < d_numRequests) {
//              record->count()  = d_numRequests;
//              record->total()  = d_totalRequestSize;
//              record->min()    = d_minRequestSize;
//              record->max()    = d_maxRequestSize;
//          }
//      }
//
//      // ...
//
//  };
//..
// We can create an instance of this 'RequestProcessor' class and use it to
// process a couple of requests:
//..
//  RequestProcessor requestProcessor;
//
//  requestProcessor.processRequest("123");
//  requestProcessor.processRequest("12345");
//..
// Now we create a 'baem_MetricRecord' to hold the aggregated metrics values.
// Note that we create a 'baem_MetricId' object by hand, but in practice
// an id should be obtained from a 'baem_MetricRegistry' object (such as the
// one owned by a 'baem_MetricsManager').
//..
//  baem_Category           myCategory("MyCategory");
//  baem_MetricDescription  description(&myCategory, "RequestSize");
//  baem_MetricId           requestSizeId(&description);
//
//  // In practice, obtain 'requestSizeId' from a 'baem_MetricRegistry' object.
//  baem_MetricRecord requestSize(requestSizeId);
//..
// Finally we retrieve the information about the request sizes of the
// requests processed by 'requestProcessor'.  Note that the count of requests
// should be 2, the total size of the requests should be 8 (3 + 5), the
// minimum size should be 3, and the maximum size should be 5.
//..
//  requestProcessor.loadRequestSizeInformation(&requestSize);
//      assert(2 == requestSize.count());
//      assert(8 == requestSize.total());
//      assert(3 == requestSize.min());
//      assert(5 == requestSize.max());
//..

#ifndef INCLUDED_BAESCM_VERSION
#include <baescm_version.h>
#endif

#ifndef INCLUDED_BAEM_METRICID
#include <baem_metricid.h>
#endif

#ifndef INCLUDED_BSL_IOSFWD
#include <bsl_iosfwd.h>
#endif

namespace BloombergLP {

                      // =======================
                      // class baem_MetricRecord
                      // =======================

class baem_MetricRecord {
    // Each instance of this class represents the aggregated value of a
    // metric.  A metric record contains a 'baem_MetricId' object
    // (identifying the metric), the number of times the measured event
    // has occurred as well as the minimum, maximum, and total of the measured
    // value.  The default 'metricId' is the invalid id value, the default
    // 'count' is 0, the default 'total' is 0.0, the default 'min' is the
    // defined 'DEFAULT_MIN' constant (the representation for positive
    // infinity), and the default 'max' is the defined 'DEFAULT_MAX' constant
    // (the representation for negative infinity).

    // DATA
    baem_MetricId d_metricId;  // id for the metric
    int           d_count;     // aggregated count of events
    double        d_total;     // total of values across events
    double        d_min;       // minimum value across events
    double        d_max;       // maximum value across events

  public:
    // PUBLIC CONSTANTS
    static const double DEFAULT_MIN;    // default minimum value
    static const double DEFAULT_MAX;    // default maximum value

    // CREATORS
    baem_MetricRecord();
        // Create a metric record having default values for its metric
        // 'metricId', 'count', 'total', 'min', and 'max' attributes.  The
        // default 'metricId' is the invalid id value, the default 'count' is
        // 0, the default 'total' is 0.0,  the default 'min' is the defined
        // 'DEFAULT_MIN' constant (the representation for positive infinity),
        // and the default 'max' is the defined 'DEFAULT_MAX' constant (the
        // representation for negative infinity).

    baem_MetricRecord(const baem_MetricId& metricId);
        // Create a metric record having the specified 'metricId',
        // and default values for the 'total', 'count', 'min', and 'max'
        // attributes.  The default 'count' is 0, the default 'total' is 0.0,
        // the default 'min' is the defined 'DEFAULT_MIN' constant (the
        // representation for positive infinity), and the default 'max' is the
        // defined 'DEFAULT_MAX' constant (the representation for negative
        // infinity).

    baem_MetricRecord(const baem_MetricId& metricId,
                      int                  count,
                      double               total,
                      double               min,
                      double               max);
        // Create a metric record having the specified 'metricId', 'count',
        // 'total', 'min', and 'max' attribute values.

    baem_MetricRecord(const baem_MetricRecord& original);
        // Create a metric record having the value of the specified 'original'
        // record.

    // ~baem_MetricRecord();
        // Destroy this object.  Note that this trivial destructor is generated
        // by the compiler.

    // MANIPULATORS
    baem_MetricRecord& operator=(const baem_MetricRecord& rhs);
        // Assign to this metric record the value of the specified 'rhs'
        // record, and return a reference to this modifiable record.

    baem_MetricId& metricId();
        // Return a reference to the modifiable 'metricId' attribute
        // representing the identifier for the metric being recorded.

    int& count();
        // Return a reference to the modifiable 'count' attribute representing
        // the number of individually recorded values.

    double& total();
        // Return a reference to the modifiable 'total' attribute representing
        // the sum of the individually recorded values.

    double& max();
        // Return a reference to the modifiable 'max' attribute representing
        // the maximum of the individually recorded values.

    double& min();
        // Return a reference to the modifiable 'min' attribute representing
        // the minimum of the individually recorded values.

    // ACCESSORS
    const baem_MetricId& metricId() const;
        // Return a reference to the non-modifiable 'metricId' attribute
        // representing the identifier for the metric being recorded.

    const int& count() const;
        // Return a reference to the non-modifiable 'count' attribute
        // representing the number of individually recorded values.

    const double& total() const;
        // Return a reference to the non-modifiable 'total' attribute
        // representing the sum of the individually recorded values.

    const double& max() const;
        // Return a reference to the non-modifiable 'max' attribute
        // representing the maximum of the individually recorded values.

    const double& min() const;
        // Return a reference to the non-modifiable 'min' attribute
        // representing the minimum of the individually recorded values.

    bsl::ostream& print(bsl::ostream& stream) const;
        // Write a description of this record to the specified 'stream' and
        // return a reference to the modifiable 'stream'.
};

// FREE OPERATORS
inline
bool operator==(const baem_MetricRecord& lhs, const baem_MetricRecord& rhs);
    // Return 'true' if the specified 'lhs' and 'rhs' metric records have the
    // same value and 'false' otherwise.  Two records have the same value if
    // they have the same values for their 'metricId', 'count', 'total',
    // 'min', and 'max' attributes, respectively.

inline
bool operator!=(const baem_MetricRecord& lhs, const baem_MetricRecord& rhs);
    // Return 'true' if the specified 'lhs' and 'rhs' metric records do not
    // have the same value and 'false' otherwise.  Two records do not have
    // same value if they differ in their respective values for 'metricId',
    // 'count', 'total', 'min', or 'max' attributes.

inline
bsl::ostream& operator<<(bsl::ostream&            stream,
                         const baem_MetricRecord& record);
    // Write a formatted description of the specified 'record' to the specified
    // 'stream' and return a reference to the modifiable 'stream'.


// ===========================================================================
//                      INLINE FUNCTION DEFINITIONS
// ===========================================================================

                          // -----------------------
                          // class baem_MetricRecord
                          // -----------------------

// CREATORS
inline
baem_MetricRecord::baem_MetricRecord()
: d_metricId()
, d_count(0)
, d_total(0.0)
, d_min(DEFAULT_MIN)
, d_max(DEFAULT_MAX)
{
}

inline
baem_MetricRecord::baem_MetricRecord(const baem_MetricId& metricId)
: d_metricId(metricId)
, d_count(0)
, d_total(0.0)
, d_min(DEFAULT_MIN)
, d_max(DEFAULT_MAX)
{
}

inline
baem_MetricRecord::baem_MetricRecord(const baem_MetricId& metricId,
                                     int                  count,
                                     double               total,
                                     double               min,
                                     double               max)
: d_metricId(metricId)
, d_count(count)
, d_total(total)
, d_min(min)
, d_max(max)
{
}

inline
baem_MetricRecord::baem_MetricRecord(const baem_MetricRecord& original)
: d_metricId(original.d_metricId)
, d_count(original.d_count)
, d_total(original.d_total)
, d_min(original.d_min)
, d_max(original.d_max)
{
}

// MANIPULATORS
inline
baem_MetricRecord& baem_MetricRecord::operator=(const baem_MetricRecord& rhs)
{
    d_metricId = rhs.d_metricId;
    d_count    = rhs.d_count;
    d_total    = rhs.d_total;
    d_min      = rhs.d_min;
    d_max      = rhs.d_max;
    return *this;
}

inline
baem_MetricId& baem_MetricRecord::metricId()
{
    return d_metricId;
}

inline
int& baem_MetricRecord::count()
{
    return d_count;
}

inline
double& baem_MetricRecord::total()
{
    return d_total;
}

inline
double& baem_MetricRecord::max()
{
    return d_max;
}

inline
double& baem_MetricRecord::min()
{
    return d_min;
}

// ACCESSORS
inline
const baem_MetricId& baem_MetricRecord::metricId() const
{
    return d_metricId;
}

inline
const int& baem_MetricRecord::count() const
{
    return d_count;
}

inline
const double& baem_MetricRecord::total() const
{
    return d_total;
}

inline
const double& baem_MetricRecord::max() const
{
    return d_max;
}

inline
const double& baem_MetricRecord::min() const
{
    return d_min;
}

// FREE OPERATORS
inline
bool operator==(const baem_MetricRecord& lhs, const baem_MetricRecord& rhs)
{
    return lhs.metricId() == rhs.metricId()
        && lhs.count()    == rhs.count()
        && lhs.total()    == rhs.total()
        && lhs.min()      == rhs.min()
        && lhs.max()      == rhs.max();
}

inline
bool operator!=(const baem_MetricRecord& lhs, const baem_MetricRecord& rhs)
{
    return !(lhs == rhs);
}

inline
bsl::ostream& operator<<(bsl::ostream&            stream,
                         const baem_MetricRecord& rhs)
{
    return rhs.print(stream);
}

}  // close namespace BloombergLP

#endif

// ---------------------------------------------------------------------------
// NOTICE:
//      Copyright (C) Bloomberg L.P., 2009
//      All Rights Reserved.
//      Property of Bloomberg L.P. (BLP)
//      This software is made available solely pursuant to the
//      terms of a BLP license agreement which governs its use.
// ----------------------------- END-OF-FILE ---------------------------------
