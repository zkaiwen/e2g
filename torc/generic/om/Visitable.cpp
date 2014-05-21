// TORC - Copyright 2010 University of Southern California. All Rights Reserved. 

#include "torc/generic/om/DumpRestoreConfig.hpp"

#ifndef HAVE_CONFIG_H
#include "torc/generic/config.h"
#endif 

#ifdef GENOM_SERIALIZATION
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#endif //GENOM_SERIALIZATION

#include "torc/generic/om/Visitable.hpp"
#include "torc/generic/om/VisitorType.hpp"

namespace torc {

namespace generic {

Visitable::Visitable() {
}

Visitable::~Visitable() throw() {
}

#ifdef GENOM_SERIALIZATION
template<class Archive> void
Visitable::serialize( Archive &ar, unsigned int ) {
}

//TO SATISFY THE LINKER
template void
Visitable::serialize<boost::archive::binary_iarchive>(
        boost::archive::binary_iarchive & ar, const unsigned int);

template void
Visitable::serialize<boost::archive::binary_oarchive>(
        boost::archive::binary_oarchive & ar, const unsigned int);

#endif //GENOM_SERIALIZATION

} // namespace torc::generic

} // namespace torc
