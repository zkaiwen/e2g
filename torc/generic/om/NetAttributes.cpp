// TORC - Copyright 2010 University of Southern California. All Rights Reserved.

#include "torc/generic/om/NetAttributes.hpp"

namespace torc {

namespace generic {

void 
NetAttributes::setCriticality(const int32_t & inSource) throw() {
    mCriticality = inSource;
    mCriticalitySet = true;
}

void 
NetAttributes::setNetDelay(const NetDelay & inSource) throw() {
    mNetDelay = inSource;
    mNetDelaySet = true;
}

NetAttributes::NetAttributes() 
    : mCriticality(),
    mNetDelay(),
    mCriticalitySet( false ),
    mNetDelaySet( false ) {
}

NetAttributes::~NetAttributes() throw() {
}

NetAttributes::NetAttributes(
        const NetAttributes & inSource) 
    : mCriticality( inSource.mCriticality ),
    mNetDelay( inSource.mNetDelay ),
    mCriticalitySet( inSource.mCriticalitySet ),
    mNetDelaySet( inSource.mNetDelaySet) {
}

NetAttributes & 
NetAttributes::operator=(
        const NetAttributes & inSource) throw() {
    if( this != &inSource )
    {
        mCriticality = inSource.mCriticality;
        mNetDelay = inSource.mNetDelay;
        mCriticalitySet = inSource.mCriticalitySet;
        mNetDelaySet = inSource.mNetDelaySet;
    }
    return *this;
}

} // namespace torc::generic

} // namespace torc
