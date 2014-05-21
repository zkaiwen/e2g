// TORC - Copyright 2010 University of Southern California. All Rights Reserved.

#ifndef TORC_GENERIC_SCALAR_PORT_REFERENCE_HPP
#define TORC_GENERIC_SCALAR_PORT_REFERENCE_HPP

#include "torc/generic/om/PointerTypes.hpp"
#include "torc/generic/om/DumpRestoreConfig.hpp"

//BOOST
#ifdef GENOM_SERIALIZATION
#include <boost/serialization/access.hpp>
#endif //GENOM_SERIALIZATION


#include "torc/generic/om/PortReference.hpp"
#include "torc/generic/om/Scalar.hpp"
#include "torc/generic/om/FactoryType.hpp"
#include "torc/generic/om/VisitorType.hpp"
#include "torc/generic/util/Error.hpp"

namespace torc { namespace generic { class BaseVisitor; }  }

namespace torc {

namespace generic {

/**
 * @brief Represents areference to a standalone port
 *
 * The ScalarPortReference class represents a reference to a a standalone port object.
 */
class ScalarPortReference
    : public PortReference,
    public Scalar<PortReference> {
#ifdef GENOM_SERIALIZATION
    friend class boost::serialization::access;
#endif //GENOM_SERIALIZATION

    friend class FactoryType<ScalarPortReference>;

  public:
    typedef VisitorType<ScalarPortReference> Visitor;

    /**
     * Convenience class to create a scalar port ref.
     */
    class Factory: public FactoryType<ScalarPortReference>
    {
        public:
            using FactoryType<ScalarPortReference>::create;
        /**
         * Create a scalar port ref.
         *
         * @param[in] inInstancePtr Pointer to parented(Instance) object.
         * @param[in] inPortPtr Pointer to master(Port) object. 
         * @param[in] inParentCollection Pointer to parent bundle.
         *
         * @return Pointer to created scalar port ref.
         **/
            virtual ScalarPortReferenceSharedPtr
            newScalarPortReferencePtr( const InstanceSharedPtr &inInstancePtr,
                const PortSharedPtr &inPortPtr,
                const PortBundleReferenceSharedPtr &inParentCollection
                            = PortBundleReferenceSharedPtr() ) throw(Error);
    };
    
    virtual void
    accept(BaseVisitor & inoutVisitor) throw(Error);

    /**
     * @exception Error Pointer to the Net object does not exists
     *       <ul>
     *         <li>
     *             Id : eMessageIdErrorPointerToItemDoesNotExist
     *         </li>
     *         <li> Context Data
     *           <ul>
     *               <li>Pointe to Net object - <i>Net</i></li>
     *           </ul>
     *         </li>
     *       </ul>
     *
     * @exception Error  Net size does not match with scalar port reference size
     *       <ul>
     *         <li>
     *             Id : eMessageIdErrorItemSizeMismatch
     *         </li>
     *         <li> Context Data
     *           <ul>
     *               <li>Net Size - <i>SizeType</i></li>
     *               <li>Scalar port reference Size - <i>SizeType</i></li>
     *           </ul>
     *         </li>
     *       </ul>
     *
     */

    virtual Connection
    connect(const NetSharedPtr &inNet) throw(Error);

    using Connectable::disconnect;

    virtual void
    disconnect(const Connection &inConnection) throw(Error);

    ~ScalarPortReference() throw();

  protected:
    ScalarPortReference();

  private:
#ifdef GENOM_SERIALIZATION
    template<class Archive> void
    serialize( Archive &ar, unsigned int );
#endif //GENOM_SERIALIZATION

};

} // namespace torc::generic

} // namespace torc
#endif
