// TORC - Copyright 2010 University of Southern California. All Rights Reserved.

#ifndef TORC_GENERIC_SINGLE_PARAMETER_HPP
#define TORC_GENERIC_SINGLE_PARAMETER_HPP

#include "torc/generic/om/PointerTypes.hpp"
#include "torc/generic/om/DumpRestoreConfig.hpp"

//BOOST
#ifdef GENOM_SERIALIZATION
#include <boost/serialization/access.hpp>
#endif //GENOM_SERIALIZATION

#include "torc/generic/om/Parameter.hpp"
#include "torc/generic/om/Scalar.hpp"
#include "torc/generic/om/VisitorType.hpp"
#include "torc/generic/om/FactoryType.hpp"
#include "torc/generic/util/Error.hpp"

namespace torc { namespace generic { class BaseVisitor; }  }

namespace torc {

namespace generic {

/**
 * Represents a single parameter inSource.
 */
class SingleParameter
    : public Parameter,
    public Scalar<Parameter> {
#ifdef GENOM_SERIALIZATION
    friend class boost::serialization::access;
#endif //GENOM_SERIALIZATION
    
    friend class FactoryType<SingleParameter>;

  public:
    typedef VisitorType<SingleParameter> Visitor;

    /**
     * Convenience class to create a single parameter.
     */
    class Factory: public FactoryType<SingleParameter>
    {
        public:
            using FactoryType<SingleParameter>::create;
        /**
         * Create a single parameter.
         *
         * @param[in] inName Name of the single parameter to be created.
         * @param[in] inValue Value of the parameter.
         * @param[in] inUnit Unit of the parameter.
         * @param[in] inOriginalName Original name of the single parameter [optional].
         *
         * @return Pointer to created single parameter.
         */   
            virtual SingleParameterSharedPtr
            newSingleParameterPtr( const std::string &inName,
                const Value &inValue,
                const Unit  &inUnit = eUnitUndefined,
                const std::string &inOriginalName = std::string()) throw(Error);
    };

    virtual
    ~SingleParameter() throw();

    /**
     * Recive a inoutVisitor to this class. The visit method of the inoutVisitor is called and a reference to this object is passed as a parameter. It has to be noted however, that a dynamic_cast is performed inside this method. If the cast fails, an appropriate exception is thrown by this method. This sitation can arise when the passed Visitor object does not inherit from the appropriate inoutVisitor specialization. See Visitor documentation for more details.
     *
     * @param[in,out] inoutVisitor A reference to the inoutVisitor object
     * @exception Error Visitor type inappropriate for visiting this object or any other error thrown by the Visitor::throw() method.
     */
    virtual void
    accept(BaseVisitor &inoutVisitor) throw(Error);

  protected:  
    SingleParameter();

  private:
#ifdef GENOM_SERIALIZATION
    template<class Archive> void
    serialize( Archive &ar, unsigned int);
#endif //GENOM_SERIALIZATION

};

} // namespace torc::generic

} // namespace torc
#endif
