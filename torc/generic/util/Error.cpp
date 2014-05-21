// TORC - Copyright 2010 University of Southern California. All Rights Reserved.

#ifndef HAVE_CONFIG_H
#include "torc/generic/config.h"
#endif


#include "torc/generic/util/Error.hpp"

namespace torc {

namespace generic {

Error::StackFrameInfo::StackFrameInfo(
            const std::string &inFunction,
            const std::string &inFile, uint32_t inLine)
    :mFunction( inFunction ),
    mFile( inFile ),
    mLine( inLine ) {
}

Error::StackFrameInfo::~StackFrameInfo() throw() {
}

Error::StackFrameInfo::StackFrameInfo(
    const Error::StackFrameInfo & source)
    :mFunction( source.mFunction ),
    mFile( source.mFile ),
    mLine( source.mLine ) {
}

Error::StackFrameInfo &
Error::StackFrameInfo::operator=(
    const Error::StackFrameInfo & source) throw() {
    if( this != &source ) {
        mFunction = source.mFunction;
        mFile = source.mFile;
        mLine = source.mLine;
    }
    return *this;
}

Error::Error(MessageId inId,
        const Context &inContext,
        const std::string &inFunction,
        const std::string &inFile, uint32_t inLine)
    :mStackTrace(),
    mContextData( inContext ),
    mErrorMessageId( inId ) {
    StackFrameInfo info( inFunction, inFile, inLine );
    mStackTrace.push_back( info );
}

/**
 * @overload
 */
Error::Error(MessageId inId,
        const std::string &inFunction,
        const std::string &inFile, uint32_t inLine)
    :mStackTrace(),
    mContextData(),
    mErrorMessageId( inId ) {
    StackFrameInfo info( inFunction, inFile, inLine );
    mStackTrace.push_back( info );
}

Error::~Error() throw() {
}

Error::Error(const Error & source) throw()
    :mStackTrace( source.mStackTrace ),
    mContextData( source.mContextData ),
    mErrorMessageId( source.mErrorMessageId ) {
}

Error &
Error::operator=(const Error & source) throw() {
    if( this != &source ) {
        mStackTrace = source.mStackTrace;
        mContextData = source.mContextData;
        mErrorMessageId = source.mErrorMessageId;
    }
    return *this;
}

void
Error::setCurrentLocation( const std::string &inFunction,
        const std::string &inFile, uint32_t inLine) throw() {
    StackFrameInfo info( inFunction, inFile, inLine );
    mStackTrace.push_back( info );
}

void
Error::saveContextData(const std::string &inName,
        const boost::any &inSource) throw() {
    mContextData[ inName ] = inSource;
}


} // namespace torc::generic

} // namespace torc
