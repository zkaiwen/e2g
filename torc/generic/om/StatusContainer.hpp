// TORC - Copyright 2010 University of Southern California. All Rights Reserved.

#ifndef TORC_GENERIC_STATUS_CONTAINER_HPP
#define TORC_GENERIC_STATUS_CONTAINER_HPP

#include <iostream>
#include <vector>

#include "torc/generic/om/PointerTypes.hpp"
#include "torc/generic/util/Error.hpp"

namespace torc { namespace generic { class Status; }  } 

namespace torc {

namespace generic {

/**
 * @brief Represents objects that have status
 * 
 * The StatusContainer interface is generalized by classes that need to hold status.
 */
class StatusContainer
{
  public:
    /**
     * Get a vector of pointer to Status object
     * 
     * @param[out] outStatus The vector of pointer to Status object
     */
    inline void 
    getStatuses(std::vector< StatusSharedPtr > & outStatus) const throw();

    /**
     * Set a vector of pointer to Status object
     * 
     * @param[in] inStatus The vector of pointer to Status object
     */
    void 
    setStatuses(const std::vector< StatusSharedPtr > & inStatus) throw();

    /**
     * Add a status to the vector of statuses. If an empty pointer is supplied, 
     * it returns without doing anything.
     * 
     * @param[in] inStatus A pointer to a status object.
     * 
     * @exception Error Status could not be added.
     */
    void 
    addStatus(const StatusSharedPtr & inStatus) throw(Error);

   /**
    * Apply action on all statuses.
    * @param[in] action Action to be applied
    *
    */
    template<typename _Action>
    inline void
    applyOnAllStatuses( const _Action &action ) throw(Error);    

    ~StatusContainer() throw();

  protected:
    StatusContainer();

  private:
    StatusContainer(const StatusContainer & source) throw();

    StatusContainer & operator=(const StatusContainer & source) throw();

    std::vector< StatusSharedPtr > mStatuses;

};
/**
 * Get a vector of pointer to Status object
 * 
 * @param[out] outStatus The vector of pointer to Status object
 */
inline void 
StatusContainer::getStatuses(std::vector< StatusSharedPtr > & outStatus) const throw() {
    outStatus.insert( outStatus.end(),
                mStatuses.begin(), mStatuses.end() );
}

/**
 * Apply action on all statuses.
 * @param[in] action Action to be applied
 *
 */
template<typename _Action>
inline void
StatusContainer::applyOnAllStatuses( const _Action &action ) throw(Error) { 
    try
    {
        std::vector< StatusSharedPtr >::iterator it = mStatuses.begin();
        for(; it != mStatuses.end(); ++ it ) 
        { 
            action( *it );    
        }
    }
    catch(Error &e)
    {
        e.setCurrentLocation( __FUNCTION__, __FILE__, __LINE__ );
        throw;
    }
}

} // namespace torc::generic

} // namespace torc
#endif
