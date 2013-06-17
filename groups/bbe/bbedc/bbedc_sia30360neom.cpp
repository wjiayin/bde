// bbedc_sia30360neom.cpp       -*-C++-*-
#include <bdes_ident.h>
BDES_IDENT_RCSID(bbedc_sia30360neom_cpp,"$Id$ $CSID$")


///IMPLEMENTATION NOTES
///--------------------
// This component uses a static method 'computeDaysDiff' to enhance insulation.

#include <bbedc_sia30360neom.h>

#include <bdet_date.h>

namespace BloombergLP {

// STATIC METHODS
inline
static int computeDaysDiff(const bdet_Date& beginDate,
                           const bdet_Date& endDate)
    // Return the number of days between the specified 'beginDate' and
    // 'endDate' according to the SIA 30/360 no-end-of-month day-count
    // convention.  If 'beginDate <= endDate', then the result is non-negative.
    // Note that reversing the order of 'beginDate' and 'endDate' negates the
    // result.
{
    int y1, m1, d1, y2, m2, d2;
    int negationFlag = beginDate > endDate;
    if (negationFlag) {
        endDate.getYearMonthDay(  &y1, &m1, &d1);
        beginDate.getYearMonthDay(&y2, &m2, &d2);
    } else {
        beginDate.getYearMonthDay(&y1, &m1, &d1);
        endDate.getYearMonthDay(  &y2, &m2, &d2);
    }

    if (31 == d1) {
      d1 = 30;
    }

    if (30 == d1 && 31 == d2) {
      d2 = 30;
    }

    int result = (y2 - y1) * 360 + (m2 - m1) * 30 + d2 - d1;

    if (negationFlag) {
        result = -result;
    }

    return result;
}

                        // -------------------------
                        // struct bbedc_Sia30360neom
                        // -------------------------

// CLASS METHODS
int bbedc_Sia30360neom::daysDiff(const bdet_Date& beginDate,
                                 const bdet_Date& endDate)
{
    return computeDaysDiff(beginDate, endDate);
}

double bbedc_Sia30360neom::yearsDiff(const bdet_Date& beginDate,
                                     const bdet_Date& endDate)
{
    return (double)computeDaysDiff(beginDate, endDate) / 360.0;
}

}  // close namespace BloombergLP

// ---------------------------------------------------------------------------
// NOTICE:
//      Copyright (C) Bloomberg L.P., 2009
//      All Rights Reserved.
//      Property of Bloomberg L.P. (BLP)
//      This software is made available solely pursuant to the
//      terms of a BLP license agreement which governs its use.
// ----------------------------- END-OF-FILE ---------------------------------
