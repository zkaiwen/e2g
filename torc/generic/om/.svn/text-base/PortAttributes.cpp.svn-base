// TORC - Copyright 2010 University of Southern California. All Rights Reserved. 

#include "torc/generic/om/DumpRestoreConfig.hpp"

#ifndef HAVE_CONFIG_H
#include "torc/generic/config.h"
#endif 

#ifdef GENOM_SERIALIZATION
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#endif //GENOM_SERIALIZATION

#include "torc/generic/om/PortAttributes.hpp"

namespace torc {

namespace generic {

void
PortAttributes::setUnused( const Value &inSource ) throw() {
	mUnused = inSource;
}

void
PortAttributes::setDesignator( const Value &inSource ) throw() {
	mDesignator = inSource;
}

void
PortAttributes::setAcLoad(const Value & inSource) throw() {
  mAcLoad = inSource;
}

void
PortAttributes::setPortDelay(const PortDelay & inSource) throw() {
  mPortDelay = inSource;
  mPortDelaySet = true;
}

void
PortAttributes::setDcFaninLoad(const Value & inSource) throw() {
  mDcFaninLoad = inSource;
}

void
PortAttributes::setDcFanoutLoad(const Value & inSource) throw() {
  mDcFanoutLoad = inSource;
}

void
PortAttributes::setDcMaxFanin(const Value & inSource) throw() {
  mDcMaxFanin = inSource;
}

void
PortAttributes::setDcMaxFanout(const Value & inSource) throw() {
  mDcMaxFanout = inSource;
}

PortAttributes::PortAttributes()
    :mDcFaninLoad(),
    mDcFanoutLoad(),
    mDcMaxFanin(),
    mDcMaxFanout(),
    mAcLoad(),
    mPortDelay(),
	mPortDelaySet( false ) {
}

PortAttributes::~PortAttributes() throw() {
}

PortAttributes::PortAttributes(
        const PortAttributes & inSource)
    :mDcFaninLoad( inSource.mDcFaninLoad ),
    mDcFanoutLoad( inSource.mDcFanoutLoad ),
    mDcMaxFanin( inSource.mDcMaxFanin ),
    mDcMaxFanout( inSource.mDcMaxFanout ),
    mAcLoad( inSource.mAcLoad ),
    mPortDelay( inSource.mPortDelay ),
	mPortDelaySet( inSource.mPortDelaySet ) {
}

PortAttributes &
PortAttributes::operator=(
        const PortAttributes & inSource) throw() {
    if( this != &inSource )
    {
        mDcFaninLoad = inSource.mDcFaninLoad;
        mDcFanoutLoad = inSource.mDcFanoutLoad;
        mDcMaxFanin = inSource.mDcMaxFanin;
        mDcMaxFanout = inSource.mDcMaxFanout;
        mAcLoad = inSource.mAcLoad;
        mPortDelay = inSource.mPortDelay;
        mPortDelaySet = inSource.mPortDelaySet;
    }
    return *this;
}

#ifdef GENOM_SERIALIZATION
template<class Archive> void
PortAttributes::serialize( Archive &ar, unsigned int ) {
    ar & mDcFaninLoad;
    ar & mDcFanoutLoad;
    ar & mDcMaxFanin;
    ar & mDcMaxFanout;
    ar & mAcLoad;
    ar & mPortDelay;
    ar & mPortDelaySet;
}

//TO SATISFY THE LINKER
template void
PortAttributes::serialize<boost::archive::binary_iarchive>(
        boost::archive::binary_iarchive & ar, const unsigned int);

template void
PortAttributes::serialize<boost::archive::binary_oarchive>(
        boost::archive::binary_oarchive & ar, const unsigned int);

#endif //GENOM_SERIALIZATION


} // namespace torc::generic

} // namespace torc
