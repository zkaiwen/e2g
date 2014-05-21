// TORC - Copyright 2010 University of Southern California. All Rights Reserved.

#ifndef TORC_GENERIC_FACTORY_TYPE_HPP
#define TORC_GENERIC_FACTORY_TYPE_HPP

#include "torc/generic/om/SelfReferencing.hpp"

namespace torc {

namespace generic {

/**
 * @brief A placeholder for a factory method
 *
 * The FactoryType template acts as a placeholder for a virtual function create that can be inherited by objects that want to create an object of an EOM type. This is typedeffed inside individual classes to generate a nested typename for a factory that can be used to create an object of that type. For example <i>Cell::Factory</i> can be used to create an object of type Cell.
 * @note All objects creatable by FactoryType must inherit from SelfReferencing policy template
 */
template<typename _Tp>
class FactoryType
{
  public:
    /**
     * A rename of the parameter type
     */
    typedef _Tp Type;

    /**
     * A pointer to an object of type <i>Type</i>
     */
    typedef typename SelfReferencing<Type>::Pointer Pointer;

    /**
     * A weak pointer to an object of type <i>Type</i>
     */
    typedef typename SelfReferencing<Type>::WeakPointer WeakPointer;

    /**
     * Create an object of the Type specification of Factory.  If the objet has a boost::weak_ptr to itself, it is the task of this method to set this after the object is created.
     *
     * @return A pointer to the freshly created object.
     */
    virtual void
    create( Pointer &outPointer ) throw(Error)
    {
        try
        {
            Pointer temp( new _Tp() ); //Do not remove
            outPointer = temp;
            WeakPointer weakPtr( outPointer );
            outPointer->setWeakThis( weakPtr );
        }
        catch( std::exception &e ) //May receive std::bad_alloc
        {
            //TBD::ERROR
        }
    }

  public:
    FactoryType() {
    }


  public:
    virtual
    ~FactoryType() throw() {
    }

  private:
    FactoryType(const FactoryType<_Tp> & source) throw();

    FactoryType<_Tp> &
    operator=(const FactoryType<_Tp> & source) throw();


};

} // namespace torc::generic

} // namespace torc
#endif
