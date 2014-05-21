#ifdef HAVE_CONFIG_H
#include "torc/generic/config.h"
#endif //HAVE_CONFIG_H


#include "torc/generic/om/ConnectionHandler.hpp"
#include "torc/generic/om/Net.hpp"

namespace torc {
namespace generic {
void
ConnectionHandler::connectPortToNet(
                    const PortSharedPtr &inPort ) throw(Error)
try
{
    mNet->addConnectedPort( inPort );
}
catch( Error &e )
{
    e.setCurrentLocation(
        __FUNCTION__, __FILE__, __LINE__ );
    throw;
}

void
ConnectionHandler::disconnectPortFromNet(
                    const PortSharedPtr &inPort ) throw(Error)
try
{
    mNet->removeConnectedPort( inPort );
}
catch( Error &e )
{
    e.setCurrentLocation(
        __FUNCTION__, __FILE__, __LINE__ );
    throw;
}


void
ConnectionHandler::connectPortRefToNet(
        const PortReferenceSharedPtr &inPortRef ) throw(Error)
try
{
    mNet->addConnectedPortReference( inPortRef );
}
catch( Error &e )
{
    e.setCurrentLocation(
        __FUNCTION__, __FILE__, __LINE__ );
    throw;
}


void
ConnectionHandler::disconnectPortRefFromNet(
        const PortReferenceSharedPtr &inPortRef ) throw(Error)
try
{
    mNet->removeConnectedPortReference( inPortRef );
}
catch( Error &e )
{
    e.setCurrentLocation(
        __FUNCTION__, __FILE__, __LINE__ );
    throw;
}

void
ConnectionHandler::connectPortListToNet(
        const PortListSharedPtr &inPortList ) throw(Error)
try
{
    mNet->addConnectedPortList( inPortList );
}
catch( Error &e )
{
    e.setCurrentLocation(
        __FUNCTION__, __FILE__, __LINE__ );
    throw;
}


void
ConnectionHandler::disconnectPortListFromNet(
        const PortListSharedPtr &inPortList ) throw(Error)
try
{
    mNet->removeConnectedPortList( inPortList );
}
catch( Error &e )
{
    e.setCurrentLocation(
        __FUNCTION__, __FILE__, __LINE__ );
    throw;
}

} //namespace generic
} //namespace torc
