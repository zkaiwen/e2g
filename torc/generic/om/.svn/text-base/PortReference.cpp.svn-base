// TORC - Copyright 2010 University of Southern California. All Rights Reserved.

#include "torc/generic/om/DumpRestoreConfig.hpp"

#ifndef HAVE_CONFIG_H
#include "torc/generic/config.h"
#endif

#ifdef GENOM_SERIALIZATION
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/type_traits/is_abstract.hpp>
#endif //GENOM_SERIALIZATION

#include "torc/generic/om/Port.hpp"
#include "torc/generic/om/PortReference.hpp"

#ifdef GENOM_SERIALIZATION
BOOST_IS_ABSTRACT(
        torc::generic::Composite<torc::generic::PortReference> )
#endif //GENOM_SERIALIZATION

namespace torc {

namespace generic {

PortReference::PortReference()
    : Composite<PortReference>(),
    Connectable(),
    PropertyContainer(),
    Visitable(),
    ParentedObject<Instance>() {
}

PortReference::~PortReference() throw() {
}

/**
 * Set master port.
 *
 * @param[in] inSource Set the master port.
 */
void
PortReference::bindToMasterPort(
    const PortSharedPtr &inMaster) throw(Error) {
  mMaster = inMaster;
}

void
PortReference::setAttributes(
        const PortAttributesSharedPtr & inSource) throw() {
    mAttributes = inSource;
}
std::string
PortReference::getName() throw() {
    return (mMaster)
            ? mMaster->getName()
            :std::string();
}

#ifdef GENOM_SERIALIZATION
template<class Archive> void
PortReference::serialize( Archive &ar, unsigned int ) {
    ar & boost::serialization::base_object<Connectable>( *this );
    ar & boost::serialization::base_object<PropertyContainer>( *this );
    ar & boost::serialization::base_object<Visitable>( *this );
    ar & boost::serialization::base_object<
                                Composite<PortReference> >( *this );
    //Master and parent are reset during restore by parent
}

//TO SATISFY THE LINKER
template void
PortReference::serialize<boost::archive::binary_iarchive>(
        boost::archive::binary_iarchive & ar, const unsigned int);

template void
PortReference::serialize<boost::archive::binary_oarchive>(
        boost::archive::binary_oarchive & ar, const unsigned int);

#endif //GENOM_SERIALIZATION

} // namespace torc::generic

} // namespace torc
