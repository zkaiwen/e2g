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

#include "torc/generic/om/EdifVersion.hpp"

namespace torc {

namespace generic {

EdifVersion::EdifVersion()
    :mFirst(2),
    mSecond(0),
    mThird(0) {
}

EdifVersion::~EdifVersion() throw() {
}

EdifVersion::EdifVersion(const EdifVersion & source) throw()
    :mFirst( source.mFirst ),
    mSecond( source.mSecond ),
    mThird( source.mThird ) {
}

EdifVersion &
EdifVersion::operator=(const EdifVersion & source) throw() {
    if( this != &source )
    {
        mFirst = source.mFirst;
        mSecond = source.mSecond;
        mThird = source.mThird;
    }
    return *this;
}

#ifdef GENOM_SERIALIZATION
template<class Archive> void
EdifVersion::serialize( Archive &ar, unsigned int ) {
    ar & mFirst;
    ar & mSecond;
    ar & mThird;
}

//TO SATISFY THE LINKER
template void
EdifVersion::serialize<boost::archive::binary_iarchive>(
        boost::archive::binary_iarchive & ar, const unsigned int);

template void
EdifVersion::serialize<boost::archive::binary_oarchive>(
        boost::archive::binary_oarchive & ar, const unsigned int);


#endif //GENOM_SERIALIZATION


} // namespace torc::generic

} // namespace torc
