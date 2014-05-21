// TORC - Copyright 2010 University of Southern California. All Rights Reserved. 

#include "torc/generic/om/DumpRestoreConfig.hpp"

#ifndef HAVE_CONFIG_H
#include "torc/generic/config.h"
#endif 

#ifdef GENOM_SERIALIZATION
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>
#endif //GENOM_SERIALIZATION

#include "torc/generic/om/Commentable.hpp"

namespace torc {

namespace generic {

/**
 * Add a comment to the object
 * 
 * @param[in] comment The comment to add to an existing list of comments
 */
void Commentable::addComment(const std::string & comment) throw() {
    mComments.push_back( comment );
}

/**
 * Get the list of all comments
 * 
 * @param[in] inSource A list of all comments to be set on this object
 */
void Commentable::setComments(const std::vector<std::string> & inSource) throw() {
  mComments = inSource;
}

Commentable::Commentable()
    :mComments() {
}

Commentable::~Commentable() throw() {
}

#ifdef GENOM_SERIALIZATION
template<class Archive> void
Commentable::serialize( Archive &ar, unsigned int ) {
    ar & mComments;
}

//TO SATISFY THE LINKER
template void
Commentable::serialize<boost::archive::binary_iarchive>(
        boost::archive::binary_iarchive & ar, const unsigned int);

template void
Commentable::serialize<boost::archive::binary_oarchive>(
        boost::archive::binary_oarchive & ar, const unsigned int);

#endif //GENOM_SERIALIZATION

} // namespace torc::generic

} // namespace torc
