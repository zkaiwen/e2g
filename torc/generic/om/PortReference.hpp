// TORC - Copyright 2010 University of Southern California. All Rights Reserved.

#ifndef TORC_GENERIC_PORT_REFERENCE_HPP
#define TORC_GENERIC_PORT_REFERENCE_HPP

#include "torc/generic/om/PointerTypes.hpp"
#include "torc/generic/om/DumpRestoreConfig.hpp"

//BOOST
#ifdef GENOM_SERIALIZATION
#include <boost/serialization/access.hpp>
#endif //GENOM_SERIALIZATION

#include "torc/generic/om/Composite.hpp"
#include "torc/generic/om/Connectable.hpp"
#include "torc/generic/om/Instance.hpp"
#include "torc/generic/om/ParentedObject.hpp"
#include "torc/generic/om/PropertyContainer.hpp"
#include "torc/generic/om/Visitable.hpp"

namespace torc { namespace generic { class Port; }  }
namespace torc { namespace generic { class PortAttributes; }  }

namespace torc {

namespace generic {

/**
 * @brief Represents the usable instance of a port of a cell in another cell
 *
 * The PortReference class is used to provide an interface for port usage of an instantiated cell. In other words, it more or less models the (portRef ...) keyword in EDIF. However, unlike a simple portRef, this can have properties that can be assigned to a portRef using (portInstance ...) syntax. This interface can be used to refer to all types of port references( scalar, vector, vector-bit and bundle).
 */
class PortReference
    : public Connectable,
    public PropertyContainer,
    public Visitable,
    virtual public Composite<PortReference>,
    public ParentedObject<Instance> {

#ifdef GENOM_SERIALIZATION
    friend class boost::serialization::access;
#endif //GENOM_SERIALIZATION
        
  public:
    PortReference();

    virtual
    ~PortReference() throw();

  private:
    PortReference(const PortReference & source);

    PortReference &
    operator=(const PortReference & source) throw();

  public:
    /**
     * Get the master(referred) port.
     *
     * @return The master port.
     */
    inline PortSharedPtr
    getMaster() const throw();

    /**
     * Set master port.
     *
     * @param[in] inMaster Set the master port.
     */
    virtual void
    bindToMasterPort(const PortSharedPtr &inMaster) throw(Error);

    /**
     * Get the attributes of the port. Attributes include dcFaninLoad, dcFanoutLoad_ etc.
     * 
     * @return Pointer to PortAttributes object.
     */
    inline const PortAttributesSharedPtr
    getAttributes() const throw();

    /**
     * Set the attributes of the port. Attributes include dcFaninLoad, dcFanoutLoad_ etc.
     * 
     * @note This method does not check whether prior set of properties exist or not. Newly set property will remove old properties.
     * 
     * @param[in] inSource Pointer to PortAttributes object.
     */
    void
    setAttributes(const PortAttributesSharedPtr & inSource) throw();

    /**
     * Get name of port-ref. This will return the same name as the master.
     *
     * @return Name of the master if master is set. Otherwise returns empty string.
     */
    std::string
    getName() throw();

  private:
#ifdef GENOM_SERIALIZATION
    template<class Archive> void
    serialize( Archive &ar, unsigned int );
#endif //GENOM_SERIALIZATION

    PortSharedPtr mMaster;
    PortAttributesSharedPtr mAttributes;
};

/**
 * Get the master(referred) port.
 *
 * @return The master port.
 */
inline PortSharedPtr
PortReference::getMaster() const throw() {
  return mMaster;
}

/**
 * Get the attributes of the port. Attributes include dcFaninLoad, dcFanoutLoad_ etc.
 * 
 * @return Pointer to PortAttributes object.
 */
inline const PortAttributesSharedPtr
PortReference::getAttributes() const throw() {
  return mAttributes;
}

} // namespace torc::generic

} // namespace torc
#endif
