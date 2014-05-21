// TORC - Copyright 2010 University of Southern California. All Rights Reserved.

#include "torc/generic/om/TimeStamp.hpp"

namespace torc {

namespace generic {

TimeStamp::TimeStamp(uint32_t inYear, uint32_t inMonth, uint32_t inDay, 
                uint32_t inHour, uint32_t inMinute, uint32_t inSecond)
    :mYear( inYear ),
    mMonth( inMonth ),
    mDay( inDay ),
    mHour( inHour ),
    mMinute( inMinute ),
    mSecond( inSecond ) {
}

TimeStamp::TimeStamp()
    :mYear(0),
    mMonth(0),
    mDay(0),
    mHour(0),
    mMinute(0),
    mSecond(0) {
}

TimeStamp::TimeStamp( const TimeStamp & source )
    :mYear( source.mYear ),
    mMonth( source.mMonth ),
    mDay( source.mDay ),
    mHour( source.mHour ),
    mMinute( source.mMinute ),
    mSecond( source.mSecond ) {
}

TimeStamp &
TimeStamp::operator =( const TimeStamp & source ) throw() {
    if( this != &source ) {
        mYear = source.mYear;
        mMonth = source.mMonth;
        mDay = source.mDay;
        mHour = source.mHour;
        mMinute = source.mMinute;
        mSecond = source.mSecond;
    }
    return *this; 
}

TimeStamp::~TimeStamp() throw() {
}

/**
 * Set year information
 * 
 * @param[in] inYear Year information 
 */
void 
TimeStamp::setYear(const uint32_t & inYear) throw() {
    mYear = inYear;
}

/**
 * Set month information
 * 
 * @param[in] inMonth Month information 
 */
void 
TimeStamp::setMonth(const uint32_t & inMonth) throw() {
    mMonth = inMonth;
}

/**
 * Set day information
 * 
 * @param[in] inDay Day information 
 */
void 
TimeStamp::setDay(const uint32_t & inDay) throw() {
    mDay = inDay;
}

/**
 * Set hour information
 * 
 * @param[in] inHour Hour information 
 */
void 
TimeStamp::setHour(const uint32_t & inHour) throw() {
    mHour = inHour;
}

/**
 * Set minute information
 * 
 * @param[in] inMinute Minute information 
 */
void 
TimeStamp::setMinute(uint32_t inMinute) throw() {
    mMinute = inMinute;
}

/**
 * Set second information
 * 
 * @param[in] inSecond Second information 
 */
void 
TimeStamp::setSecond(uint32_t inSecond) throw() {
    mSecond = inSecond;
}


} // namespace torc::generic

} // namespace torc
