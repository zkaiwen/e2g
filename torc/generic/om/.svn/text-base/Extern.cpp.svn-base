// TORC - Copyright 2010 University of Southern California. All Rights Reserved. 

#include "torc/generic/om/DumpRestoreConfig.hpp"

#ifndef HAVE_CONFIG_H
#include "torc/generic/config.h"
#endif 

#ifdef GENOM_SERIALIZATION
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/weak_ptr.hpp>
#endif //GENOM_SERIALIZATION

#include "torc/generic/om/Extern.hpp"

namespace torc {

namespace generic {

/**
 * Set whether an item is extern or not.
 * 
 * @param[in] isExtern True if extern, false otherwise
 */
void
Extern::setIsExtern(bool inIsExtern) throw() {
	mIsExtern = inIsExtern;
}

Extern::Extern()
	:mIsExtern(false) {
}

Extern::~Extern() throw() {
}

#ifdef GENOM_SERIALIZATION
template<class Archive> void
Extern::serialize( Archive &ar, unsigned int ) {
	ar & mIsExtern;
}

//TO SATISFY THE LINKER
template void
Extern::serialize<boost::archive::binary_iarchive>(
        boost::archive::binary_iarchive & ar, const unsigned int);

template void
Extern::serialize<boost::archive::binary_oarchive>(
        boost::archive::binary_oarchive & ar, const unsigned int);

#endif //GENOM_SERIALIZATION

} // namespace torc::generic

} // namespace torc
