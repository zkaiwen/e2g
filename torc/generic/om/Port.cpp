// TORC - Copyright 2010 University of Southern California. All Rights Reserved. 

#include "torc/generic/om/DumpRestoreConfig.hpp"

#ifndef HAVE_CONFIG_H
#include "torc/generic/config.h"
#endif 

#ifdef GENOM_SERIALIZATION
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/is_abstract.hpp>
#endif //GENOM_SERIALIZATION

#include "torc/generic/om/Port.hpp"
#include "torc/generic/om/PortAttributes.hpp"

#ifdef GENOM_SERIALIZATION
#endif //GENOM_SERIALIZATION

namespace torc {

namespace generic {

Port::Port()
    : Composite<Port>(),
    Commentable(),
    Connectable(),
    Extern(),
    Nameable(),
    PropertyContainer(),
    Renamable(),
    Visitable(),
    ParentedObject<View>(),
    UserDataContainer(),
    mDirection( ePortDirectionUndefined ),
    mAttributes() {
}

Port::~Port() throw() {
}

/**
 * Set the direction of port
 * 
 * @param[in] inSource Direction of port
 */
void
Port::setDirection(const PortDirection &inSource) throw() {
  mDirection = inSource;
}

/**
 * Set the attributes of the port. Attributes include dcFaninLoad, dcFanoutLoad_ etc.
 * 
 * @note This method does not check whether prior set of properties exist or not. Newly set property will remove old properties.
 * 
 * @param[in] inSource Pointer to PortAttributes object.
 */
void
Port::setAttributes(
    const PortAttributesSharedPtr & inSource) throw() {
  mAttributes = inSource;
}

#ifdef GENOM_SERIALIZATION
template<class Archive> void
Port::serialize( Archive &ar, unsigned int ) {
    ar & boost::serialization::base_object<Commentable>( *this );
    ar & boost::serialization::base_object<Connectable>( *this );
    ar & boost::serialization::base_object<Extern>( *this );
    ar & boost::serialization::base_object<Nameable>( *this );
    ar & boost::serialization::base_object<
                                        PropertyContainer>( *this );
    ar & boost::serialization::base_object<Renamable>( *this );
    ar & boost::serialization::base_object<Visitable>( *this );
    ar & boost::serialization::base_object<
                                    Composite<Port> >( *this );
    ar & mDirection;
    ar & mAttributes;
    //We do not serialize the parent pointer
}

//TO SATISFY THE LINKER
template void
Port::serialize<boost::archive::binary_iarchive>(
        boost::archive::binary_iarchive & ar, const unsigned int);

template void
Port::serialize<boost::archive::binary_oarchive>(
        boost::archive::binary_oarchive & ar, const unsigned int);

#endif //GENOM_SERIALIZATION


} // namespace torc::generic

} // namespace torc
