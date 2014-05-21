// TORC - Copyright 2010 University of Southern California. All Rights Reserved. 

#ifndef TORC_GENERIC_VECTOR_PORT_BIT_HPP
#define TORC_GENERIC_VECTOR_PORT_BIT_HPP

#include "torc/generic/om/PointerTypes.hpp"
#include "torc/generic/om/DumpRestoreConfig.hpp"

#ifdef GENOM_SERIALIZATION
#include <boost/serialization/access.hpp>
#endif //GENOM_SERIALIZATION

#include "torc/generic/om/Port.hpp"
#include "torc/generic/om/VectorBit.hpp"
#include "torc/generic/util/Error.hpp"
#include "torc/generic/om/FactoryType.hpp"
#include "torc/generic/om/VisitorType.hpp"

namespace torc { namespace generic { class BaseVisitor; }  } 

namespace torc {

namespace generic {

/**
 * @brief Represents a bit of a port.
 * 
 * The VectorPortBit class represents a bit of an array port. Such objects cannot be created in EDIF. They are automatically crated when a particular member of an array is accessed using the (member ...) syntax.
 */
class VectorPortBit
    : public Port,
    public VectorBit<Port> {

#ifdef GENOM_SERIALIZATION
    friend class boost::serialization::access;
#endif //GENOM_SERIALIZATION

  public:
    typedef FactoryType<VectorPortBit> Factory;
    typedef VisitorType<VectorPortBit> Visitor;

    virtual void
    accept(BaseVisitor & inoutVisitor) throw(Error);

    virtual Connection
    connect(const NetSharedPtr &inNet) throw(Error);

    using Connectable::disconnect;

    virtual void
    disconnect(const Connection &inConnection) throw(Error);

    virtual void
    getConnectedNets(
        std::vector< NetSharedPtr > &outNets,
        bool inSkipChildConnections = false) const throw(Error);

    VectorPortBit();

    ~VectorPortBit() throw();

  private:
#ifdef GENOM_SERIALIZATION
    template<class Archive> void
    serialize( Archive &ar, unsigned int );
#endif //GENOM_SERIALIZATION
};

} // namespace torc::generic

} // namespace torc
#endif
