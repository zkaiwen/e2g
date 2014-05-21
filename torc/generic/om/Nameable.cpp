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

#include "torc/generic/om/Nameable.hpp"

namespace torc {

namespace generic {

Nameable::Nameable() {
}

Nameable::~Nameable() throw() {
}

/**
 * Set a name for this object
 *
 * @param[in] inSource Name of the object
 *
 * @exception Error Could not set name, because name is empty (Will be used by array members)
 */
void
Nameable::setName(const std::string & inSource) throw(Error) {
    if( inSource.empty() )
    {
        Error e( eMessageIdErrorEmptyItemName,
            __FUNCTION__, __FILE__, __LINE__ );
        e.saveContextData("Nameable", inSource);
        throw e;
    }
    mName = inSource;
}

#ifdef GENOM_SERIALIZATION
template<class Archive> void
Nameable::serialize( Archive &ar, unsigned int ) {
    ar & mName;
}

//TO SATISFY THE LINKER
template void
Nameable::serialize<boost::archive::binary_iarchive>(
        boost::archive::binary_iarchive & ar, const unsigned int);

template void
Nameable::serialize<boost::archive::binary_oarchive>(
        boost::archive::binary_oarchive & ar, const unsigned int);

#endif //GENOM_SERIALIZATION
} // namespace torc::generic

} // namespace torc
