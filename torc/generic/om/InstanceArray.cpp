// TORC - Copyright 2010 University of Southern California. All Rights Reserved.

#include "torc/generic/om/DumpRestoreConfig.hpp"

#ifndef HAVE_CONFIG_H
#include "torc/generic/config.h"
#endif

//BOOST
#include <boost/bind.hpp>
#include <boost/mem_fn.hpp>
#ifdef GENOM_SERIALIZATION
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>
#endif //GENOM_SERIALIZATION

#include "torc/generic/om/InstanceArray.hpp"

#ifdef GENOM_SERIALIZATION
BOOST_CLASS_EXPORT( torc::generic::InstanceArray )
#endif //GENOM_SERIALIZATION

namespace torc {

namespace generic {

/**
 * Create an instance array
 *
 * @param[in] inName Name of the instance array to be created.
 * @param[in] inViewPtr Pointer to parented(view) object.
 * @param[in] inMaster Pointer to master object.
 * @param[in] inSize Size of the instance array.
 * @param[in] inFactory Factory for the child.
 * @param[in] inOriginalName Original name of the instance array [optional].
 *
 * @return Pointer to created instance array.
 **/

InstanceArraySharedPtr
InstanceArray::Factory::newInstanceArrayPtr( const std::string &inName,
    const ViewSharedPtr &inViewPtr,
    const ViewSharedPtr &inMaster,
    const size_t &inSize,
    const ChildFactorySharedPtr &inFactory,
    const std::string &inOriginalName ) throw(Error) {
    try
    {
        std::vector<size_t> limits;
        limits.push_back( inSize );
        return newInstanceArrayPtr( inName, inViewPtr, inMaster, limits, inFactory, inOriginalName );  
    }
    catch( Error &e )
    {
        e.setCurrentLocation(
            __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}

/**
 * Create an instance array
 *
 * @param[in] inName Name of the instance array to be created.
 * @param[in] inViewPtr Pointer to parented(view) object.
 * @param[in] inMaster Pointer to master object.
 * @param[in] inLimits Dimensions of the vector.
 * @param[in] inFactory Factory for the child.
 * @param[in] inOriginalName Original name of the instance array [optional].
 *
 * @return Pointer to created instance array.
 **/

InstanceArraySharedPtr
InstanceArray::Factory::newInstanceArrayPtr( const std::string &inName,
    const ViewSharedPtr &inViewPtr,
    const ViewSharedPtr &inMaster,
    const std::vector<size_t> &inLimits,
    const ChildFactorySharedPtr &inFactory,
    const std::string &inOriginalName ) throw(Error) {
    try
    {
        InstanceArraySharedPtr newInstanceArray;
        create( newInstanceArray );
        newInstanceArray->setName(inName);
        //newInstanceArray->setMaster( inMaster );
        newInstanceArray->constructChildren( inFactory, inLimits);
        newInstanceArray->bindToMasterView( inMaster );
        newInstanceArray->setOriginalName( inOriginalName );
        inViewPtr->addInstance( newInstanceArray );
        return newInstanceArray;
    }
    catch( Error &e )
    {
        e.setCurrentLocation(
            __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}
void
InstanceArray::accept(BaseVisitor & inoutVisitor) throw(Error) {
    try
    {
        runVisitor( *this, inoutVisitor );
    }
    catch( Error &e )
    {
        e.setCurrentLocation(
                __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}

void
InstanceArray::setParent(
        const ViewSharedPtr &inParent ) throw() {
    ParentedObject<View>::setParent( inParent );
    BaseVectorType::List children;
    getCreatedChildren( children );
    std::for_each( children.begin(), children.end(),
            boost::bind( boost::mem_fn(
                    &Instance::setParent), _1, getParent() ));
}

void
InstanceArray::bindToMasterView(
    const ViewSharedPtr &inMaster,
    bool inMapPortReferences ) throw(Error) {
    typedef std::vector< InstanceSharedPtr >
                                                Children;
    Children children;
    getChildren( children );
    Children::iterator child = children.begin();
    Children::iterator cEnd = children.end();
    for( ; child != cEnd; ++child )
    {
        try
        {
            (*child)->bindToMasterView( inMaster );
        }
        catch( Error &e )
        {
            e.setCurrentLocation(
                __FUNCTION__, __FILE__, __LINE__ );
            throw;
        }
    }
    Instance::bindToMasterView( inMaster, false );
}

void
InstanceArray::addPortReference(
    const PortReferenceSharedPtr & inPortRef) throw(Error) {
    //TBD::ERROR UNSUPPORTED
}

PortReferenceSharedPtr
InstanceArray::findPortReference(
            const std::string &inName) throw(Error) {
    //TBD::ERROR UNSUPPORTED
    return PortReferenceSharedPtr();
}

void
InstanceArray::removePortReference(
        const std::string &inName) throw(Error) {
    //TBD::ERROR UNSUPPORTED
}

void
InstanceArray::getPortReferences(
    std::vector< PortReferenceSharedPtr > &outPortRefs
        ) const throw() {
    return;
}


void
InstanceArray::setPortReferences(
    const std::vector< PortReferenceSharedPtr > &inSource
    ) throw( Error ) {
    //TBD::ERROR UNSUPPORTED
}

/**
 * Flatten an instance so that internal references
 */
void InstanceArray::flatten() throw(Error) {
    //TBD
}

void
InstanceArray::onChildCreate( const boost::shared_ptr<BaseVectorType::ChildType> &inCreatedChild
            ) const throw(Error) {
   inCreatedChild->setName( getName() );
}

InstanceArray::InstanceArray()
    :Instance(),
    Vector<Instance, InstanceArrayMember,
        InstanceArrayMember::Factory, false>() {
}

InstanceArray::~InstanceArray() throw() {
}

#ifdef GENOM_SERIALIZATION
template<class Archive> void
InstanceArray::serialize( Archive &ar, unsigned int ) {
    ar & boost::serialization::base_object<Instance>( *this );
    ar & boost::serialization::base_object< BaseVectorType >( *this );
}

//TO SATISFY THE LINKER
template void
InstanceArray::serialize<boost::archive::binary_iarchive>(
        boost::archive::binary_iarchive & ar, const unsigned int);

template void
InstanceArray::serialize<boost::archive::binary_oarchive>(
        boost::archive::binary_oarchive & ar, const unsigned int);

#endif //GENOM_SERIALIZATION


} // namespace torc::generic

} // namespace torc
