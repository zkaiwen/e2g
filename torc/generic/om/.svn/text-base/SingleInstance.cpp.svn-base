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

#include "torc/generic/om/SingleInstance.hpp"

#ifdef GENOM_SERIALIZATION
BOOST_CLASS_EXPORT( torc::generic::SingleInstance )
#endif //GENOM_SERIALIZATION

namespace torc {

namespace generic {

/**
 * Create a single instance.
 * @param[in] inName Name of the single instance to be created.
 * @param[in] inViewPtr Pointer to parented(View) object.
 * @param[in] inMasterPtr Pointer to master(View) object.
 * @param[in] inOriginalName Original name of the single instance [optional].
 *
 * @return Pointer to created single instance.
 */
SingleInstanceSharedPtr
SingleInstance::Factory::newSingleInstancePtr( const std::string &inName,
    const ViewSharedPtr &inViewPtr,
    const ViewSharedPtr &inMasterPtr,
    const std::string &inOriginalName ) throw(Error) {
    try
    {
        SingleInstanceSharedPtr newSingleInstance;
        create( newSingleInstance );
        newSingleInstance->setName(inName);
        newSingleInstance->setMaster( inMasterPtr );
        newSingleInstance->setOriginalName( inOriginalName );
        inViewPtr->addInstance( newSingleInstance );
        return newSingleInstance;
    }
    catch( Error &e )
    {
        e.setCurrentLocation(
            __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }   
}

void
SingleInstance::accept(
    BaseVisitor & inoutVisitor) throw(Error) {
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

/**
 * Flatten an instance so that internal references
 */
void
SingleInstance::flatten() throw(Error) {
    //TBD
}

SingleInstance::SingleInstance()
    : Instance(),
    Scalar<Instance>() {
}

SingleInstance::~SingleInstance() throw() {
}

#ifdef GENOM_SERIALIZATION
template<class Archive> void
SingleInstance::serialize( Archive &ar, unsigned int ) {
    ar & boost::serialization::base_object<Instance>( *this );
    ar & boost::serialization::base_object<
                                    Scalar<Instance> >( *this );
}

//TO SATISFY THE LINKER
template void
SingleInstance::serialize<boost::archive::binary_iarchive>(
        boost::archive::binary_iarchive & ar, const unsigned int);

template void
SingleInstance::serialize<boost::archive::binary_oarchive>(
        boost::archive::binary_oarchive & ar, const unsigned int);

#endif //GENOM_SERIALIZATION

} // namespace torc::generic

} // namespace torc
