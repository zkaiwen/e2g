// TORC - Copyright 2010 University of Southern California. All Rights Reserved. 

#ifndef TORC_GENERIC_OBJECT_VISITOR_HPP
#define TORC_GENERIC_OBJECT_VISITOR_HPP


#include "torc/generic/om/Root.hpp"
#include "torc/generic/om/Library.hpp"
#include "torc/generic/om/Cell.hpp"
#include "torc/generic/om/Design.hpp"
#include "torc/generic/om/View.hpp"
#include "torc/generic/om/ScalarPort.hpp"
#include "torc/generic/om/VectorPort.hpp"
#include "torc/generic/om/VectorPortBit.hpp"
#include "torc/generic/om/PortBundle.hpp"
#include "torc/generic/om/ScalarNet.hpp"
#include "torc/generic/om/VectorNet.hpp"
#include "torc/generic/om/VectorNetBit.hpp"
#include "torc/generic/om/NetBundle.hpp"
#include "torc/generic/om/SingleInstance.hpp"
#include "torc/generic/om/InstanceArray.hpp"
#include "torc/generic/om/InstanceArrayMember.hpp"
#include "torc/generic/om/SingleParameter.hpp"
#include "torc/generic/om/ParameterArray.hpp"
#include "torc/generic/om/ParameterArrayElement.hpp"
#include "torc/generic/om/Property.hpp"
#include "torc/generic/om/ScalarPortReference.hpp"
#include "torc/generic/om/VectorPortReference.hpp"
#include "torc/generic/om/VectorPortBitReference.hpp"
#include "torc/generic/om/PortBundleReference.hpp"
#include "torc/generic/om/PortList.hpp"
#include "torc/generic/om/Status.hpp"
#include "torc/generic/om/Permutable.hpp"

namespace torc {

namespace generic {

/**
 * A inoutVisitor that can visit all types of leaf type elements in the library. All visit methods will ned to be overridden in the deriving class.
 */
class ObjectVisitor
    : public Root::Visitor,
    public Library::Visitor,
    public Cell::Visitor,
    public Design::Visitor,
    public View::Visitor,
    public ScalarPort::Visitor,
    public VectorPort::Visitor,
    public VectorPortBit::Visitor,
    public PortBundle::Visitor,
    public ScalarNet::Visitor,
    public VectorNet::Visitor,
    public VectorNetBit::Visitor,
    public NetBundle::Visitor,
    public SingleInstance::Visitor,
    public InstanceArray::Visitor,
    public InstanceArrayMember::Visitor,
    public Property::Visitor,
    public SingleParameter::Visitor,
    public ParameterArray::Visitor,
    public ParameterArrayElement::Visitor,
    public ScalarPortReference::Visitor,
    public VectorPortReference::Visitor,
    public VectorPortBitReference::Visitor,
    public PortBundleReference::Visitor,
    public PortList::Visitor,
    public Status::Visitor,
    public Permutable::Visitor {

  protected:
    ObjectVisitor();
  public:
    virtual
    ~ObjectVisitor() throw();
};

} // namespace torc::generic

} // namespace torc
#endif
