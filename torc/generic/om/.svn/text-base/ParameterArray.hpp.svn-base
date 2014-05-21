// TORC - Copyright 2010 University of Southern California. All Rights Reserved.

#ifndef TORC_GENERIC_PARAMETER_ARRAY_HPP
#define TORC_GENERIC_PARAMETER_ARRAY_HPP

#include "torc/generic/om/PointerTypes.hpp"
#include "torc/generic/om/DumpRestoreConfig.hpp"

//BOOST
#ifdef GENOM_SERIALIZATION
#include <boost/serialization/access.hpp>
#endif //GENOM_SERIALIZATION

#include "torc/generic/util/Error.hpp"
#include "torc/generic/om/FactoryType.hpp"
#include "torc/generic/om/Parameter.hpp"
#include "torc/generic/om/ParameterArrayElement.hpp"
#include "torc/generic/om/Vector.hpp"
#include "torc/generic/om/VisitorType.hpp"

namespace torc { namespace generic { class BaseVisitor; }  }

namespace torc {

namespace generic {

/**
 * @brief Represents a parameter array.
 *
 * The ParameterArray class represents an array of parameters.
 */
class ParameterArray :
    public Parameter,
    public Vector<Parameter,
                ParameterArrayElement,
                ParameterArrayElement::Factory,
                false> {
#ifdef GENOM_SERIALIZATION
    friend class boost::serialization::access;
#endif //GENOM_SERIALIZATION

    friend class FactoryType<ParameterArray>;

  public:
    typedef Vector<Parameter, ParameterArrayElement,
            ParameterArrayElement::Factory, false> BaseVectorType;
    typedef VisitorType<ParameterArray> Visitor;

    /**
     * Convenience class to create a parameter array.
     */
    class Factory: public FactoryType<ParameterArray>
    {
        public:
            using FactoryType<ParameterArray>::create;
        /**
         * Create a parameter array.
         *
         * @param[in] inName Name of the parameter array to be created.
         * @param[in] inSize Size of the instance array.
         * @param[in] inFactory Factory for the child.
         * @param[in] inOriginalName Original name of the parameter array [optional].
         *
         * @return Pointer to created parameter array.
         */
            virtual ParameterArraySharedPtr
            newParameterArrayPtr( const std::string &inName,
                const size_t &inSize,
                const ChildFactorySharedPtr &inFactory
                            = BaseVectorType::ChildFactorySharedPtr(
                                                new BaseVectorType::ChildFactory() ),
                const std::string &inOriginalName = std::string() ) throw(Error);

        /**
         * Create a parameter array.
         *
         * @param[in] inName Name of the parameter array to be created.
         * @param[in] inLimits Dimensions of the vector.
         * @param[in] inFactory Factory for the child.
         * @param[in] inOriginalName Original name of the parameter array [optional].
         *
         * @return Pointer to created parameter array.
         */
            virtual ParameterArraySharedPtr
            newParameterArrayPtr( const std::string &inName,
                const std::vector<size_t> &inLimits,
                const ChildFactorySharedPtr &inFactory
                            = BaseVectorType::ChildFactorySharedPtr(
                                                new BaseVectorType::ChildFactory() ),
                const std::string &inOriginalName = std::string() ) throw(Error);
    };

    virtual
    ~ParameterArray() throw();

    /**
     * Recive a inoutVisitor to this class. The visit method of the inoutVisitor is called and a reference to this object is passed as a parameter. It has to be noted however, that a dynamic_cast is performed inside this method. If the cast fails, an appropriate exception is thrown by this method. This sitation can arise when the passed Visitor object does not inherit from the appropriate inoutVisitor specialization. See Visitor documentation for more details.
     *
     * @param[in,out] inoutVisitor A reference to the inoutVisitor object
     * @exception Error Visitor type inappropriate for visiting this object or any other error thrown by the Visitor::throw() method.
     */
    virtual void
    accept(BaseVisitor & inoutVisitor) throw(Error);

  protected:
    virtual void
    onChildCreate( const boost::shared_ptr<BaseVectorType::ChildType> &inCreatedChild
            ) const throw(Error);

    ParameterArray();

  private:
#ifdef GENOM_SERIALIZATION
    template<class Archive> void
    serialize( Archive &ar, unsigned int);
#endif //GENOM_SERIALIZATION
};

} // namespace torc::generic

} // namespace torc
#endif
