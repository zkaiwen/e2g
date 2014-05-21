// TORC - Copyright 2010 University of Southern California. All Rights Reserved.

#ifndef TORC_GENERIC_STATUS_HPP
#define TORC_GENERIC_STATUS_HPP

#include <vector>

#include "torc/generic/om/PointerTypes.hpp"
#include "torc/generic/om/Commentable.hpp"
#include "torc/generic/om/Visitable.hpp"
#include "torc/generic/om/VisitorType.hpp"
#include "torc/generic/om/UserDataContainer.hpp"
#include "torc/generic/om/SelfReferencing.hpp"
#include "torc/generic/util/Error.hpp"
#include "torc/generic/om/FactoryType.hpp"
#include "torc/generic/om/StatusContainer.hpp"

namespace torc { namespace generic { class Written; }  } 
namespace torc { namespace generic { class BaseVisitor; }  } 

namespace torc {

namespace generic {

/**
 * @brief Represents EDIF status construct
 * 
 * Status is used to convey accounting and problem analysis information for the design.
 */
class Status :
    public Commentable,
    public Visitable,
    public UserDataContainer,
    public SelfReferencing<Status> {

    friend class FactoryType<Status>;

  public:

    /**
     * Convenience class to visit a status.
     */
    typedef VisitorType<Status> Visitor;

    /**
     * Convenience class to create a status.
     */
    class Factory: public FactoryType<Status>
    {
        public:
        using FactoryType<Status>::create;
        /**
         * Create a status.
         *
         * @param[in] inContainer Pointer of the status container.
         *
         * @return Pointer to created status.
         */
        StatusSharedPtr
        virtual newStatusPtr( const StatusContainerSharedPtr & inContainer ) throw(Error);            

    };

    virtual
    ~Status() throw();

    /**
     * Get the vector of written statements.
     *
     * @return vector Containing written statements
     */    
    inline void
    getWrittens( std::vector< WrittenSharedPtr > &outValues) const throw();

    /**
     * Set the vector of written statements.
     *
     * @param[in] inSource Vector containing written statements
     */    
    void
    setWrittens( const std::vector< WrittenSharedPtr > &inSource ) throw();

    /**
     * Add a written statement to the vector of written statements. 
     * If an empty pointer is supplied, it returns without doing anything.
     *
     * @param[in] inWritten A pointer to a written object.
     *
     * @exception Error Written could not be added.
     */
    void
    addWritten(WrittenSharedPtr & inWritten) throw(Error);

    /**
     * Recive a visitor to this class. The visit method of the visitor is called 
     * and a reference to this object is passed as a parameter. It has to be noted however, 
     * that a dynamic_cast is performed inside this method. If the cast fails, 
     * an appropriate exception is thrown by this method. This sitation can arise when
     * the passed Visitor object does not inherit from the appropriate visitor specialization. 
     * See Visitor documentation for more details.
     * 
     * @param[in,out] visitor A reference to the visitor object
     * @exception Error Visitor type inappropriate for visiting this object 
     * or any other error thrown by the Visitor::throw() method.
     */
    virtual void 
    accept(BaseVisitor & visitor) throw(Error);

  protected:  
    Status();

  private:
    std::vector< WrittenSharedPtr > mWrittens;

};

inline void
Status::getWrittens(
    std::vector< WrittenSharedPtr > &outValues ) const throw() {
    outValues.insert( outValues.end(),
                mWrittens.begin(), mWrittens.end() );
}

} // namespace torc::generic

} // namespace torc
#endif
