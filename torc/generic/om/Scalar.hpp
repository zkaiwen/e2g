// TORC - Copyright 2010 University of Southern California. All Rights Reserved.

#ifndef TORC_GENERIC_SCALAR_HPP
#define TORC_GENERIC_SCALAR_HPP

#include "torc/generic/om/DumpRestoreConfig.hpp"

#include <vector>

#ifdef GENOM_SERIALIZATION
#include <boost/serialization/access.hpp>
#endif //GENOM_SERIALIZATION

#include "torc/generic/om/Composite.hpp"
#include "torc/generic/om/CompositionType.hpp"
#include "torc/generic/util/Error.hpp"

namespace torc {

namespace generic {

/**
 * @brief A single object with no child objects
 */
template<typename _Type>
class Scalar : virtual public Composite<_Type>
{
#ifdef GENOM_SERIALIZATION
    friend class boost::serialization::access;
#endif //GENOM_SERIALIZATION

  public:
    typedef typename Composite<_Type>::Type     Type;
    typedef typename Composite<_Type>::Pointer  Pointer;
    typedef typename Composite<_Type>::List     List;
    typedef typename Composite<_Type>::SizeType SizeType;

    virtual
    ~Scalar() throw();

    /**
     * Get composition type for this object
     *
     * @return The CompositionType inSource eCompositionTypeScalar is returned
     */
    virtual CompositionType
    getCompositionType() const throw();

    /**
     * Get the total number of bits of the composition
     * @return Number of bits
     */
    virtual SizeType
    getSize() const throw();

    /**
     * Get a specific member of this composition.
     *
     * @param[in] indices A list of indices to be accessed. The number of indices must be equal to the number of dimensions.
     *
     * @return An empty pointer is returned
     */
    virtual const Pointer
    get(const std::vector<SizeType> & indices) const throw(Error);

    /**
     * Get children of this composition.
     * @param[out] outChildren A list of all children for this composition
     * @note This wil return empty list for scalar
     */
    virtual void
    getChildren( List &outChildren ) const throw(Error);

  protected:
    Scalar();

  private:
#ifdef GENOM_SERIALIZATION
    template<class Archive> void
    serialize( Archive &ar, unsigned int );
#endif //GENOM_SERIALIZATION

    Scalar(const Scalar<_Type> & source) throw();
    Scalar<_Type> &
    operator=(const Scalar<_Type> & source) throw();
};

template<typename _Type>
Scalar<_Type>::~Scalar() throw() {
}


/**
 * Get composition type for this object
 *
 * @return The CompositionType inSource eCompositionTypeScalar is returned
 */
template<typename _Type>
CompositionType
Scalar<_Type>::getCompositionType() const throw() {
    return eCompositionTypeScalar;
}

/**
 * Get the total number of bits of the composition
 * @return Number of bits
 */
template<typename _Type>
typename Scalar<_Type>::SizeType
Scalar<_Type>::getSize() const throw()
{
    return 1;
}

/**
 * Get a specific member of this composition.
 *
 * @param[in] indices A list of indices to be accessed. The number of indices must be equal to the number of dimensions.
 *
 * @return An empty pointer is returned
 */
template<typename _Type>
const typename Scalar<_Type>::Pointer
Scalar<_Type>::get(const std::vector<typename Scalar<_Type>::SizeType> & indices) const throw(Error) {
    return Pointer();
}

/**
 * Get children of this composition.
 *
 * @param[out] outChildren A list of all children for this composition
 */
template<typename _Type>
void
Scalar<_Type>::getChildren(
    typename Scalar<_Type>::List &outChildren ) const throw(Error) {
    return;
}

template<typename _Type>
Scalar<_Type>::Scalar()
    :Composite<_Type>() {
}

#ifdef GENOM_SERIALIZATION
template<typename _Type>
template<class Archive> void
Scalar<_Type>::serialize( Archive &ar, unsigned int ) {
    //NOTHING TO DUMP
}
#endif //GENOM_SERIALIZATION


} // namespace torc::generic

} // namespace torc
#endif
