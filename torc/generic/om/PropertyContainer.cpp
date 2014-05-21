// TORC - Copyright 2010 University of Southern California. All Rights Reserved.

#include "torc/generic/om/DumpRestoreConfig.hpp"

#ifndef HAVE_CONFIG_H
#include "torc/generic/config.h"
#endif

#ifdef GENOM_SERIALIZATION
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#endif //GENOM_SERIALIZATION

#include "torc/generic/om/PropertyContainer.hpp"
#include "torc/generic/om/Property.hpp"

namespace torc {

namespace generic {

/**
 * Get a property
 *
 * @param[in] name Name of the property to be retreived
 *
 * @return Pointer to the property object if present, empty pointer otherwise.
 */
PropertySharedPtr
PropertyContainer::getProperty(
        const std::string & inName) throw() {
    PropertySharedPtr property;
    if( !inName.empty() )
    {
        mProperties.get( inName, property );
    }
    return property;
}

/**
 * Save the inSource of  a property
 *
 * @param[in] inName Name of the property to be saved
 * @param[in] inPoperty Pointer to the property object if present, empty pointer otherwise.
 *
 * @return bool True if saved, false otherwise.
 */
bool
PropertyContainer::setProperty(
    const std::string & inName,
    const PropertySharedPtr &inProperty) throw() {
    if( !inName.empty() )
    {
        return mProperties.set( inName, inProperty );
    }
    return false;
}

/**
 * Set the map of properties
 *
 * @param[in] inSource The map of properties
 */
void
PropertyContainer::setProperties(
    const std::map< std::string, PropertySharedPtr > & inSource) throw() {
    std::map< std::string, PropertySharedPtr >::const_iterator it = inSource.begin();
  for( ; it != inSource.end(); it++ )
  {
    setProperty( (*it).first, (*it).second );
  }
}

PropertyContainer::PropertyContainer()
	:mProperties() {
}

PropertyContainer::~PropertyContainer() throw() {
}

#ifdef GENOM_SERIALIZATION
template<class Archive> void
PropertyContainer::serialize( Archive &ar, unsigned int ) {
	ar & mProperties;
}


//TO SATISFY THE LINKER
template void
PropertyContainer::serialize<boost::archive::binary_iarchive>(
        boost::archive::binary_iarchive & ar, const unsigned int);

template void
PropertyContainer::serialize<boost::archive::binary_oarchive>(
        boost::archive::binary_oarchive & ar, const unsigned int);

#endif //GENOM_SERIALIZATION

} // namespace torc::generic

} // namespace torc
