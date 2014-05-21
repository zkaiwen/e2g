// TORC - Copyright 2010 University of Southern California. All Rights Reserved.

#ifndef TORC_GENERIC_CLONEABLE_HPP
#define TORC_GENERIC_CLONEABLE_HPP

#include "torc/generic/om/DumpRestoreConfig.hpp"

//BOOST
#include <boost/shared_ptr.hpp>

#ifdef GENOM_SERIALIZATION
#include <boost/serialization/access.hpp>
#include <boost/serialization/weak_ptr.hpp>
#endif //GENOM_SERIALIZATION

/**
 * @file Cloneable.hpp
 * @brief Interface for Cloneable
 *
 * This file contains the interface for the Cloneable class
 */
namespace torc {

namespace generic {

/**
 * @brief Represents all EOM classes that can be cloned(copied).
 *
 * The Cloneable class defines a method called clone() that is overdidden by derived classes to provide a polymorphic copy operation.
 */
template<class _CloneableType>
class Cloneable
{
#ifdef GENOM_SERIALIZATION
	friend class boost::serialization::access;
#endif
  protected:
    /**
     * Constructor to be used by inheriting classes
     */
    Cloneable() throw();


  public:
    virtual
    ~Cloneable() throw();


  private:
    Cloneable(const Cloneable<_CloneableType> & source) throw();

    Cloneable<_CloneableType> &
    operator=(const Cloneable<_CloneableType> & source) throw();


  public:
    /**
     * Create a copy of this object. This method is polymorphic.
     *
     * @return Pointer to a copy of this object
     */
     virtual boost::shared_ptr<_CloneableType>
     clone() throw() = 0;

#ifdef GENOM_SERIALIZATION
	template<class Archive> void
	serialize( Archive &ar, unsigned int );
#endif //GENOM_SERIALIZATION

};
/**
 * Constructor to be used by inheriting classes
 */
template<class _CloneableType>
Cloneable<_CloneableType>::Cloneable() throw() {
}

template<class _CloneableType>
Cloneable<_CloneableType>::~Cloneable() throw() {
}

#ifdef GENOM_SERIALIZATION
template<class _CloneableType>
template<class Archive> void
Cloneable<_CloneableType>::serialize( Archive &ar, unsigned int ) {
}
#endif //GENOM_SERIALIZATION

} // namespace torc::generic

} // namespace torc

#endif
