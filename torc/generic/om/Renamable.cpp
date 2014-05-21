// TORC - Copyright 2010 University of Southern California. All Rights Reserved.

#include "torc/generic/om/DumpRestoreConfig.hpp"

#ifndef HAVE_CONFIG_H
#include "torc/generic/config.h"
#endif

#ifdef GENOM_SERIALIZATION
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/string.hpp>
#endif //GENOM_SERIALIZATION

#include "torc/generic/om/Renamable.hpp"

namespace torc {

namespace generic {

/**
 * Set the new name provided for the object.
 *
 * @param[in] inSource Name of the object
 */
void
Renamable::setOriginalName( const Renamable::Name &inSource) throw() {
  mOriginalName = inSource;
}

Renamable::Renamable()
    :mOriginalName() {
}

Renamable::~Renamable() throw()
{
}

#ifdef GENOM_SERIALIZATION
template<class Archive> void
Renamable::serialize( Archive &ar, unsigned int ) {
	ar & mOriginalName;
}

//TO SATISFY THE LINKER
template void
Renamable::serialize<boost::archive::binary_iarchive>(
        boost::archive::binary_iarchive & ar, const unsigned int);

template void
Renamable::serialize<boost::archive::binary_oarchive>(
        boost::archive::binary_oarchive & ar, const unsigned int);

#endif //GENOM_SERIALIZATION

} // namespace torc::generic

} // namespace torc
