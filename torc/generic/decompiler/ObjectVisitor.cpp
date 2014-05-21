// TORC - Copyright 2010 University of Southern California. All Rights Reserved. 

#ifndef HAVE_CONFIG_H
#include "torc/generic/config.h"
#endif 

#include "torc/generic/decompiler/ObjectVisitor.hpp"

namespace torc {

namespace generic {

ObjectVisitor::ObjectVisitor()
    :Root::Visitor(),
    Library::Visitor(),
    Cell::Visitor(),
    Design::Visitor(),
    View::Visitor(),
    ScalarPort::Visitor(),
    VectorPort::Visitor(),
    VectorPortBit::Visitor(),
    PortBundle::Visitor(),
    ScalarNet::Visitor(),
    VectorNet::Visitor(),
    VectorNetBit::Visitor(),
    NetBundle::Visitor(),
    SingleInstance::Visitor(),
    InstanceArray::Visitor(),
    InstanceArrayMember::Visitor(),
    Property::Visitor(),
    SingleParameter::Visitor(),
    ParameterArray::Visitor(),
    ParameterArrayElement::Visitor(),
    ScalarPortReference::Visitor(),
    VectorPortReference::Visitor(),
    VectorPortBitReference::Visitor(),
    PortBundleReference::Visitor(),
    PortList::Visitor() {
    //UserDataContainer::Visitor() {
}

ObjectVisitor::~ObjectVisitor() throw() {
}


} // namespace torc::generic

} // namespace torc
