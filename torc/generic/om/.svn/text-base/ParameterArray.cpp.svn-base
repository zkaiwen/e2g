// TORC - Copyright 2010 University of Southern California. All Rights Reserved. 

#include "torc/generic/om/DumpRestoreConfig.hpp"

#ifndef HAVE_CONFIG_H
#include "torc/generic/config.h"
#endif 

#ifdef GENOM_SERIALIZATION
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>
#endif //GENOM_SERIALIZATION

#include "torc/generic/om/ParameterArray.hpp"
#include "torc/generic/om/VisitorType.hpp"

#ifdef GENOM_SERIALIZATION
BOOST_CLASS_EXPORT( torc::generic::ParameterArray )
#endif //GENOM_SERIALIZATION

namespace torc {

namespace generic {

void
ParameterArray::onChildCreate( const boost::shared_ptr<BaseVectorType::ChildType> &inCreatedChild
            ) const throw(Error) {
   inCreatedChild->setName( getName() );
}

ParameterArray::ParameterArray()
    : Parameter(),
    Vector<Parameter,
                ParameterArrayElement,
                ParameterArrayElement::Factory,
                false>() {
}

ParameterArray::~ParameterArray() throw() {
}

/**
 * Create a parameter array.
 *
 * @param[in] inName Name of the parameter array to be created.
 * @param[in] inUnit Unit of the parameter array.
 * @param[in] inFactory Factory for the child.
 * @param[in] inOriginalName Original name of the parameter array [optional].    
 *
 * @return Pointer to created parameter array.
 */
ParameterArraySharedPtr
ParameterArray::Factory::newParameterArrayPtr( const std::string &inName,
    const size_t &inSize,
    const ChildFactorySharedPtr &inFactory,
    const std::string &inOriginalName ) throw(Error) {
    try
    {
        std::vector<size_t> limits;
        limits.push_back( inSize );
        return newParameterArrayPtr( inName, limits, inFactory, inOriginalName );
    }
    catch( Error &e )
    {
        e.setCurrentLocation(
            __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}

/**
 * Create a parameter array.
 *
 * @param[in] inName Name of the parameter array to be created.
 * @param[in] inLimits Dimensions of the vector.
 * @param[in] inValue Value of the parameter array.
 * @param[in] inUnit Unit of the parameter array.
 * @param[in] inFactory Factory for the child.
 * @param[in] inOriginalName Original name of the parameter array [optional].    
 *
 * @return Pointer to created parameter array.
 */
ParameterArraySharedPtr
ParameterArray::Factory::newParameterArrayPtr( const std::string &inName,
    const std::vector<size_t> &inLimits,
    const ChildFactorySharedPtr &inFactory,
    const std::string &inOriginalName ) throw(Error) {
    try
    {
        ParameterArraySharedPtr newParameterArray;
        create( newParameterArray );
        newParameterArray->setName(inName);
        newParameterArray->setOriginalName( inOriginalName );
        newParameterArray->constructChildren( inFactory, inLimits);
        return newParameterArray;
    }
    catch( Error &e )
    {
        e.setCurrentLocation(
            __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}

void
ParameterArray::accept(
        BaseVisitor &inoutVisitor) throw(Error)
{
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

#ifdef GENOM_SERIALIZATION
template<class Archive> void
ParameterArray::serialize( Archive &ar, unsigned int) {
    ar & boost::serialization::base_object< Parameter >( *this );
    ar & boost::serialization::base_object< BaseVectorType >( *this );
}

//TO SATISFY THE LINKER
template void
ParameterArray::serialize<boost::archive::binary_oarchive>(
    boost::archive::binary_oarchive & ar, const unsigned int);

template void
ParameterArray::serialize<boost::archive::binary_iarchive>(
    boost::archive::binary_iarchive & ar, const unsigned int);

#endif //GENOM_SERIALIZATION


} // namespace torc::generic

} // namespace torc
