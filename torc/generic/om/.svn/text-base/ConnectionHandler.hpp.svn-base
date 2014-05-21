#ifndef TORC_GENERIC_CONNECTIONHANDLER_HPP
#define TORC_GENERIC_CONNECTIONHANDLER_HPP

#include "torc/generic/om/PointerTypes.hpp"
#include "torc/generic/util/Error.hpp"

namespace torc {
namespace generic {

class ConnectionHandler {

  public:	
	void
	connectPortToNet( const PortSharedPtr &inPort ) throw(Error);

	void
	disconnectPortFromNet( const PortSharedPtr &inPort ) throw(Error);

	void
	connectPortRefToNet(
			const PortReferenceSharedPtr &inPort ) throw(Error);

	void
	disconnectPortRefFromNet(
			const PortReferenceSharedPtr &inPort ) throw(Error);

	void
	connectPortListToNet(
			const PortListSharedPtr &inPortList ) throw(Error);

	void
	disconnectPortListFromNet(
			const PortListSharedPtr &inPortList ) throw(Error);

	ConnectionHandler( const NetSharedPtr &inNet )
		:mNet( inNet ) {
	}

  private:	
	NetSharedPtr mNet;
};

} //namespace generic
} //namespace torc

#endif //TORC_GENERIC_CONNECTIONHANDLER_HPP
