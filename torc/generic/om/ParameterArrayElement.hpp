// TORC - Copyright 2010 University of Southern California. All Rights Reserved.

#ifndef TORC_GENERIC_PARAMETER_ARRAY_ELEMENT_HPP
#define TORC_GENERIC_PARAMETER_ARRAY_ELEMENT_HPP

#include "torc/generic/om/PointerTypes.hpp"
#include "torc/generic/om/DumpRestoreConfig.hpp"

//BOOST
#ifdef GENOM_SERIALIZATION
#include <boost/serialization/access.hpp>
#endif //GENOM_SERIALIZATION

#include "torc/generic/util/Error.hpp"
#include "torc/generic/om/FactoryType.hpp"
#include "torc/generic/om/Parameter.hpp"
#include "torc/generic/om/VectorBit.hpp"
#include "torc/generic/om/VisitorType.hpp"

namespace torc { namespace generic { class BaseVisitor; }  }

namespace torc {

namespace generic {

/**
 * Represents an element of a paramter array.
 */
class ParameterArrayElement :
    public Parameter,
    public VectorBit<Parameter> {
#ifdef GENOM_SERIALIZATION
    friend class boost::serialization::access;
#endif //GENOM_SERIALIZATION
    
  public:
    typedef VisitorType<ParameterArrayElement> Visitor;
    typedef FactoryType<ParameterArrayElement> Factory;

    ParameterArrayElement();

    virtual
    ~ParameterArrayElement() throw();

    /**
     * Recive a inoutVisitor to this class. The visit method of the inoutVisitor is called and a reference to this object is passed as a parameter. It has to be noted however, that a dynamic_cast is performed inside this method. If the cast fails, an appropriate exception is thrown by this method. This sitation can arise when the passed Visitor object does not inherit from the appropriate inoutVisitor specialization. See Visitor documentation for more details.
     *
     * @param[in,out] inoutVisitor A reference to the inoutVisitor object
     * @exception Error Visitor type inappropriate for visiting this object or any other error thrown by the Visitor::throw() method.
     */
    virtual void
    accept(BaseVisitor &inoutVisitor) throw(Error);


  private:
#ifdef GENOM_SERIALIZATION
    template<class Archive> void
    serialize( Archive &ar, unsigned int);
#endif //GENOM_SERIALIZATION

};

} // namespace torc::generic

} // namespace torc
#endif
