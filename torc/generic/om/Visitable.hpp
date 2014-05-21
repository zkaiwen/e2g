// TORC - Copyright 2010 University of Southern California. All Rights Reserved. 

#ifndef TORC_GENERIC_VISITABLE_HPP
#define TORC_GENERIC_VISITABLE_HPP

#include "torc/generic/om/DumpRestoreConfig.hpp"

#ifdef GENOM_SERIALIZATION
#include <boost/serialization/access.hpp>
#include <boost/serialization/is_abstract.hpp>
#endif //GENOM_SERIALIZATION

#include "torc/generic/util/Error.hpp"

namespace torc { namespace generic { class BaseVisitor; }  } 

namespace torc {

namespace generic {

/**
 * @brief An object that receives a inoutVisitor
 * 
 * The Visitable class provides an interface to all classes that want to make themselves visitable by the clients. Typically such classes will be leaf types that cannot be directly accessed by clients without using a dynamic_cast. This design is loosely based on the  acyclic inoutVisitor concept defined by Alexandrescu in "Modern C++ Design".
 */
class Visitable
{
#ifdef GENOM_SERIALIZATION
	friend class boost::serialization::access;
#endif
  protected:
    Visitable();


  public:
    virtual
    ~Visitable() throw();


  private:
    Visitable(const Visitable & source) throw();

    Visitable & operator=(const Visitable & source) throw();


  public:
    /**
     * Recive a inoutVisitor to this class. The visit method of the inoutVisitor is called and a reference to this object is passed as a parameter. It has to be noted however, that a dynamic_cast is performed inside this method. If the cast fails, an appropriate exception is thrown by this method. This sitation can arise when the passed Visitor object does not inherit from the appropriate inoutVisitor specialization. See Visitor documentation for more details.
     * 
     * @param[in,out] inoutVisitor A reference to the inoutVisitor object
     * @exception Error Visitor type inappropriate for visiting this object or any other error thrown by the Visitor::throw() method.
     */
    virtual void
    accept(BaseVisitor & inoutVisitor) throw(Error) = 0;

  public:
#ifdef GENOM_SERIALIZATION
	template<class Archive> void
	serialize( Archive &ar, unsigned int );
#endif //GENOM_SERIALIZATION

};

} // namespace torc::generic

} // namespace torc

#ifdef GENOM_SERIALIZATION
BOOST_IS_ABSTRACT( torc::generic::Visitable )
#endif //GENOM_SERIALIZATION

#endif
