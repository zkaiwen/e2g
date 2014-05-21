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

#include "torc/generic/om/Parameter.hpp"

#ifdef GENOM_SERIALIZATION
BOOST_IS_ABSTRACT(
        torc::generic::Composite< torc::generic::Parameter > )
#endif //GENOM_SERIALIZATION

namespace torc {

namespace generic {

Parameter::Parameter()
    : Composite<Parameter>(),
    Nameable(),
    Renamable(),
    Visitable(),
    mValue(),
    mUnit( eUnitUndefined ) {
}

Parameter::~Parameter() throw() {
}

void
Parameter::setValue(const Value & inSource) throw() {
  mValue = inSource;
}

const Unit
Parameter::getUnit() const throw() {
  return mUnit;
}

void
Parameter::setUnit(const Unit & inSource) throw() {
  mUnit = inSource;
}

#ifdef GENOM_SERIALIZATION
template<class Archive> void
Parameter::serialize( Archive &ar, unsigned int) {
    ar & boost::serialization::base_object< Composite<Parameter> >(
                                                            *this );
    ar & boost::serialization::base_object< Nameable >( *this );
    ar & boost::serialization::base_object<Renamable>( *this );
    ar & boost::serialization::base_object< Visitable >( *this );
    ar & mValue;
    ar & mUnit;
}

//TO SATISFY THE LINKER
template void
Parameter::serialize<boost::archive::binary_oarchive>(
    boost::archive::binary_oarchive & ar, const unsigned int);

template void
Parameter::serialize<boost::archive::binary_iarchive>(
    boost::archive::binary_iarchive & ar, const unsigned int);

#endif //GENOM_SERIALIZATION

} // namespace torc::generic

} // namespace torc
