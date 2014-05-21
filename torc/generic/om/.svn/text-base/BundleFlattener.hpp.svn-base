// TORC - Copyright 2010 University of Southern California. All Rights Reserved.

#ifndef TORC_GENERIC_BUNDLE_FLATTENER_HPP
#define TORC_GENERIC_BUNDLE_FLATTENER_HPP

#include "torc/generic/om/PointerTypes.hpp"
#include "torc/generic/om/DumpRestoreConfig.hpp"

#include "torc/generic/om/Bundle.hpp"
#include "torc/generic/om/ScalarPort.hpp"
#include "torc/generic/om/VectorPort.hpp"
#include "torc/generic/om/PortBundle.hpp"
#include "torc/generic/om/VisitorApplier.hpp"

namespace torc {

namespace generic {

/**
 * @brief Flatten a bundle to bits 
 *
 * The BundleFlattener class is used to flatten a bundle, 
 */
template<typename _BaseType, typename _Scalar, typename _Vector,
        typename _VectorBit, typename _Bundle>
class BundleFlattener :
         public _Scalar::Visitor,
         public _Vector::Visitor,
         public _VectorBit::Visitor,   
         public _Bundle::Visitor {
  public:

    typedef typename _BaseType::List      List;

    void
    visit( _Scalar &scalar ) throw(Error);

    void
    visit( _Vector &vector ) throw(Error);

    void
    visit( _VectorBit &vectorBit ) throw(Error);

    void
    visit( _Bundle &bundle ) throw(Error);

    inline void
    getChildren( typename Bundle<_BaseType>::List &outChildren ) const throw(Error);


    BundleFlattener();

    ~BundleFlattener() throw(); 

   private:
    typename Bundle<_BaseType>::List mChildren;
};


/**
 * Get children of this composition.
 *
 * @param[out] outChildren A list of all children for this composition
 */
template<typename _BaseType, typename _Scalar, typename _Vector,
        typename _VectorBit, typename _Bundle>
void
BundleFlattener<_BaseType, _Scalar, _Vector, _VectorBit, _Bundle>
::getChildren(
    typename Bundle<_BaseType>::List &outChildren ) const throw(Error) {
    outChildren.insert( outChildren.end(),
                mChildren.begin(), mChildren.end() );
    return;
}

template<typename _BaseType, typename _Scalar, typename _Vector,
        typename _VectorBit, typename _Bundle>
void
BundleFlattener<_BaseType, _Scalar, _Vector, _VectorBit, _Bundle>
::visit( _Scalar &scalar ) throw(Error) {
    mChildren.push_back( scalar.getSharedThis() );
}

template<typename _BaseType, typename _Scalar, typename _Vector,
        typename _VectorBit, typename _Bundle>
void
BundleFlattener<_BaseType, _Scalar, _Vector, _VectorBit, _Bundle>
::visit( _Vector &vector ) throw(Error) {
    try
    {
        typename _Vector::List children;
        vector.getChildren( children );
        BundleFlattener<_BaseType, _Scalar, _Vector,
                            _VectorBit, _Bundle> flattener;
        VisitorApplier< BundleFlattener<_BaseType, _Scalar,
            _Vector, _VectorBit, _Bundle> > applier( flattener ); 
        for_each( children.begin(), children.end(), applier );
    }
    catch( Error &e )
    {
        e.setCurrentLocation( __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}

template<typename _BaseType, typename _Scalar, typename _Vector,
        typename _VectorBit, typename _Bundle>
void
BundleFlattener<_BaseType, _Scalar, _Vector, _VectorBit, _Bundle>
::visit( _VectorBit &vectorBit ) throw(Error) {
    mChildren.push_back( vectorBit.getSharedThis() );

}

template<typename _BaseType, typename _Scalar, typename _Vector,
        typename _VectorBit, typename _Bundle>
void
BundleFlattener<_BaseType, _Scalar, _Vector, _VectorBit, _Bundle>
::visit( _Bundle &bundle ) throw(Error) {
    try
    {
        BundleFlattener<_BaseType, _Scalar, _Vector,
                            _VectorBit, _Bundle> flattener;
        VisitorApplier< BundleFlattener<_BaseType, _Scalar, _Vector, _VectorBit, _Bundle> >
                                            applier( flattener ); 
        bundle.applyOnAllChildren( applier );

    }
    catch( Error &e )
    {
        e.setCurrentLocation( __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}

template<typename _BaseType, typename _Scalar, typename _Vector,
        typename _VectorBit, typename _Bundle>
BundleFlattener<_BaseType, _Scalar, _Vector, _VectorBit, _Bundle>
::BundleFlattener()
    :mChildren( ) {
}

template<typename _BaseType, typename _Scalar, typename _Vector,
        typename _VectorBit, typename _Bundle>
BundleFlattener<_BaseType, _Scalar, _Vector, _VectorBit, _Bundle>
::~BundleFlattener() throw() {
}

} // namespace torc::generic

} // namespace torc
#endif
