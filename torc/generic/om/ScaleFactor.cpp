// TORC - Copyright 2010 University of Southern California. All Rights Reserved.

#include "torc/generic/om/DumpRestoreConfig.hpp"

#ifndef HAVE_CONFIG_H
#include "torc/generic/config.h"
#endif

//BOOST
#ifdef GENOM_SERIALIZATION
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#endif //GENOM_SERIALIZATION

#include "torc/generic/om/ScaleFactor.hpp"

namespace torc {

namespace generic {

/**
 * Set the EDIF scale.
 *
 * @param[in] inSource EDIF scale.
 */
void ScaleFactor::setEdifValue(const Value::Number & inSource) throw() {
  mEdifValue = inSource;
}

/**
 * Set the SI scale.
 *
 */
void ScaleFactor::setSiValue(const Value::Number & inSource) throw() {
  mSiValue = inSource;
}

ScaleFactor::ScaleFactor()
    :mEdifValue(),
    mSiValue() {
}

ScaleFactor::ScaleFactor( const ScaleFactor &inSource )
    :mEdifValue( inSource.mEdifValue ),
    mSiValue( inSource.mEdifValue ) {
}

ScaleFactor::~ScaleFactor() {
}

ScaleFactor &
ScaleFactor::operator = ( const ScaleFactor &inSource ) {
    if( this != &inSource )
    {
        mEdifValue = inSource.mEdifValue;
        mSiValue = inSource.mSiValue;
    }
    return *this;
}

#ifdef GENOM_SERIALIZATION
template<class Archive> void
ScaleFactor::serialize( Archive &ar, unsigned int ) {
    ar & mEdifValue;
    ar & mSiValue;
}

//TO SATISFY THE LINKER
template void
ScaleFactor::serialize<boost::archive::binary_iarchive>(
        boost::archive::binary_iarchive & ar, const unsigned int);

template void
ScaleFactor::serialize<boost::archive::binary_oarchive>(
        boost::archive::binary_oarchive & ar, const unsigned int);

#endif //GENOM_SERIALIZATION

} // namespace torc::generic

} // namespace torc
