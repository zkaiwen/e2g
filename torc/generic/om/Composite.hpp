// TORC - Copyright 2010 University of Southern California. All Rights Reserved.

#ifndef TORC_GENERIC_COMPOSITE_HPP
#define TORC_GENERIC_COMPOSITE_HPP

#include "torc/generic/om/DumpRestoreConfig.hpp"

#include <cstddef>
#include <vector>

//BOOST
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#ifdef GENOM_SERIALIZATION
#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/weak_ptr.hpp>
#endif //GENOM_SERIALIZATION

#include "torc/generic/om/CompositionType.hpp"
#include "torc/generic/util/Error.hpp"
#include "torc/generic/om/SelfReferencing.hpp"

namespace torc {

namespace generic {

/**
 * @brief Interface for objects that can be composed within each other
 *
 * The Composite class defines an interface for objects that can occur singly (Scalar), as Arrays (Vector) or bundles (Bundle). This class defines polymorphic methods that will be overridden by subsequent implementations.
 */
template<typename _Type>
class Composite: public SelfReferencing<_Type>
{
#ifdef GENOM_SERIALIZATION
	friend class boost::serialization::access;
#endif
  public:
    typedef _Type Type;

    /**
     * Shared Pointer to object of type Type
     */
    typedef boost::shared_ptr<Type> Pointer;
    typedef boost::weak_ptr<Type> WeakPointer;

    /**
     * List of Shared Pointers to objects of type Type
     */
    typedef std::vector<Pointer> List;
    typedef size_t               SizeType;


  protected:
    virtual ~Composite() throw() {
    };


  public:
    Composite();

    /**
     * Get the type of this composition.
     *
     * @return Composition type for this object
     */
    virtual CompositionType
    getCompositionType() const throw() = 0;

    /**
     * Get the total number of bits of the composition
     * @return Number of bits
     */
    virtual size_t
    getSize() const throw() = 0;

    /**
     * Get children of this composition.
     *
     * @note This is relevant for Vector and Bundle compositions only. Other compositions return a NULL pointer
     *
     * @param[out] outChildren A list of all children for this composition
     */
    virtual void
    getChildren( List &outChildren ) const throw(Error) = 0;


    /**
     * Get a specific member of this composition.
     *
     * @note This is relevant for Vector composition only. Other compositions return a NULL pointer
     *
     * @param[in] inIndices A list of indices to be accessed. The number of indices must be equal to the number of dimensions.
     *
     * @return A pointer to the child situated at the specified indices. For non-relevant types a NULL pointer is returned
     *
     * @exception Error Index dimensions mismatch
     */
    virtual const Pointer
    get(const std::vector<SizeType> & inIndices) const throw(Error) = 0;

    /**
     * Set a pointer to a parnt composition.
     *
     * @param[in] inParentCollection pointer to the parent collection (Vector/Bundle)
     */
    inline virtual void
    setParentCollection(const Pointer & inParentCollection) throw();

    /**
     * Get a pointer to the parent collection
     *
     * @return A pointer to the parent collection if present, empty pointer otherwise
     */
    inline virtual Pointer
    getParentCollection() const throw();

  private:

#ifdef GENOM_SERIALIZATION
	template<class Archive> void
	serialize( Archive &ar, unsigned int );
#endif //GENOM_SERIALIZATION

    Composite(const Composite<_Type> &rhs) throw();

    Composite<_Type> &
    operator=(const Composite<_Type> &rhs) throw();

    WeakPointer mParentCollection;

};

template<typename _Type>
Composite<_Type>::Composite()
    : SelfReferencing<_Type>(),
	mParentCollection() {
}

/**
 * Set a pointer to a parnt composition.
 *
 * @param[in] parentCollection pointer to the parent collection (Vector/Bundle)
 */
template<typename _Type>
inline void
Composite<_Type>::setParentCollection(
    const typename Composite<_Type>::Pointer & inParentCollection) throw() {
    mParentCollection = inParentCollection;
}

/**
 * Get a pointer to the parent collection
 *
 * @return A pointer to the parent collection if present, empty pointer otherwise
 */
template<typename _Type>
inline typename Composite<_Type>::Pointer
Composite<_Type>::getParentCollection() const throw() {
    return mParentCollection.lock();
}

#ifdef GENOM_SERIALIZATION
template<typename _Type>
template<class Archive> void
Composite<_Type>::serialize( Archive &ar, unsigned int ) {
	ar & boost::serialization::base_object< SelfReferencing<_Type> >(
													*this );
	//ar & mParentCollection;
}
#endif //GENOM_SERIALIZATION

} // namespace torc::generic

} // namespace torc
#endif
