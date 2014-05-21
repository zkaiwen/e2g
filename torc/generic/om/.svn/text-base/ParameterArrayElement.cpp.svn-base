// TORC - Copyright 2010 University of Southern California. All Rights Reserved.

#include "torc/generic/om/DumpRestoreConfig.hpp"

#ifndef HAVE_CONFIG_H
#include "torc/generic/config.h"
#endif

#ifdef GENOM_SERIALIZATION
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>
#endif //GENOM_SERIALIZATION

#include "torc/generic/om/ParameterArrayElement.hpp"
#include "torc/generic/om/VisitorType.hpp"

#ifdef GENOM_SERIALIZATION
BOOST_CLASS_EXPORT( torc::generic::ParameterArrayElement )
#endif //GENOM_SERIALIZATION

namespace torc {

namespace generic {

ParameterArrayElement::ParameterArrayElement() {
}

ParameterArrayElement::~ParameterArrayElement() throw() {
}

void
ParameterArrayElement::accept(
        BaseVisitor &inoutVisitor) throw(Error)
{
    try
    {
        runVisitor( *this, inoutVisitor );
    }
    catch( Error &e )
    {
        e.setCurrentLocation(
            __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}


#ifdef GENOM_SERIALIZATION
template<class Archive> void
ParameterArrayElement::serialize( Archive &ar, unsigned int) {
    ar & boost::serialization::base_object< Parameter >( *this );
    ar & boost::serialization::base_object< VectorBit<Parameter> >(
                                                            *this );
}

//TO SATISFY THE LINKER
template void
ParameterArrayElement::serialize<boost::archive::binary_oarchive>(
    boost::archive::binary_oarchive & ar, const unsigned int);

template void
ParameterArrayElement::serialize<boost::archive::binary_iarchive>(
    boost::archive::binary_iarchive & ar, const unsigned int);

#endif //GENOM_SERIALIZATION

} // namespace torc::generic

} // namespace torc
