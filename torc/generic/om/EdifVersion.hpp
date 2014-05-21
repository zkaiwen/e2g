// TORC - Copyright 2010 University of Southern California. All Rights Reserved.

#ifndef TORC_GENERIC_EDIF_VERSION_HPP
#define TORC_GENERIC_EDIF_VERSION_HPP

#include "torc/generic/om/DumpRestoreConfig.hpp"

//BOOST
#include<boost/cstdint.hpp>

#ifdef GENOM_SERIALIZATION
#include <boost/serialization/access.hpp>
#endif //GENOM_SERIALIZATION

namespace torc {

namespace generic {

/**
 * Version of EDIF being parsed. This release supports only EDIF 2 0 0.
 */
struct EdifVersion {

#ifdef GENOM_SERIALIZATION
    friend class boost::serialization::access;
#endif //GENOM_SERIALIZATION

  public:
    EdifVersion();

    ~EdifVersion() throw();

    EdifVersion(const EdifVersion & source) throw();

    EdifVersion &
    operator=(const EdifVersion & source) throw();

  public:
    uint8_t mFirst;
    uint8_t mSecond;
    uint8_t mThird;

  private:
#ifdef GENOM_SERIALIZATION
    template<class Archive> void
    serialize( Archive &ar, unsigned int );
#endif //GENOM_SERIALIZATION

};

} // namespace torc::generic

} // namespace torc
#endif
