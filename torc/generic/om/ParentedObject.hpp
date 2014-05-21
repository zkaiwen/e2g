// TORC - Copyright 2010 University of Southern California. All Rights Reserved.

#ifndef TORC_GENERIC_PARENTED_OBJECT_HPP
#define TORC_GENERIC_PARENTED_OBJECT_HPP

#include "torc/generic/om/DumpRestoreConfig.hpp"

#include <string>

//BOOST
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#ifdef GENOM_SERIALIZATION
#include <boost/serialization/access.hpp>
#include <boost/serialization/weak_ptr.hpp>
#endif //GENOM_SERIALIZATION

namespace torc {

namespace generic {

/**
 * @brief An object that has a parent
 *
 * EDIF 2 0 0 defines a nested hierarchy of objects in a design. As such most of the non-root elements have a corresponding parent object. However, the type of parent depends on the type of object. The ParentedObject template therefore provides a mechanism for storing a pointer to the parent object.
 */
template<typename _ParentType>
class ParentedObject {

#ifdef GENOM_SERIALIZATION
	friend class boost::serialization::access;
#endif

  protected:
    ParentedObject();


  public:
    virtual
    ~ParentedObject() throw();


  private:
    ParentedObject(const ParentedObject<_ParentType> & source) throw();

    ParentedObject<_ParentType> &
    operator=(const ParentedObject<_ParentType> & source) throw();


  public:
    /**
     * Get a pointer to the parent object
     *
     * @return Pointer to parent
     */
    inline const boost::shared_ptr<_ParentType>
    getParent() const throw();

    /**
     * Set a pointer to the parent
     *
     * @param[in] inSource Set a pointer to the parent
     */
    virtual void
    setParent(const boost::shared_ptr<_ParentType> & inSource) throw();

  private:
#ifdef GENOM_SERIALIZATION
	template<class Archive> void
	serialize( Archive &ar, unsigned int );
#endif //GENOM_SERIALIZATION

    boost::weak_ptr<_ParentType> mParent;
};

template<typename _ParentType>
ParentedObject<_ParentType>::ParentedObject()
    :mParent() {
}

template<typename _ParentType>
ParentedObject<_ParentType>::~ParentedObject() throw() {
}

/**
 * Get a pointer to the parent object
 *
 * @return Pointer to parent
 */
template<typename _ParentType>
inline const boost::shared_ptr<_ParentType>
ParentedObject<_ParentType>::getParent() const throw() {
  return mParent.lock();
}

/**
 * Set a pointer to the parent
 *
 * @param[in] inSource Set a pointer to the parent
 */
template<typename _ParentType>
void
ParentedObject<_ParentType>::setParent(
    const boost::shared_ptr<_ParentType> & inSource) throw() {
  mParent = inSource;
}

#ifdef GENOM_SERIALIZATION
template<typename _ParentType>
template<class Archive> void
ParentedObject<_ParentType>::serialize( Archive &ar, unsigned int ) {
	ar & mParent;
}
#endif //GENOM_SERIALIZATION

} // namespace torc::generic

} // namespace torc
#endif
