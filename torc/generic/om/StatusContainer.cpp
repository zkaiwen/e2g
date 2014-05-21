// TORC - Copyright 2010 University of Southern California. All Rights Reserved.

#include "torc/generic/om/StatusContainer.hpp"
#include "torc/generic/om/Status.hpp"

namespace torc {

namespace generic {

StatusContainer::StatusContainer() 
    :mStatuses() {
}

StatusContainer::~StatusContainer() throw() {
}

/**
 * Set a vector of pointer to Status object
 *
 * @param[in] inStatus The vector of pointer to Status object
 */
void 
StatusContainer::setStatuses(const std::vector< StatusSharedPtr > & inStatus) throw() {
    std::vector< StatusSharedPtr >::const_iterator it = inStatus.begin();
    for( ; it != inStatus.end(); it++ ) {
        mStatuses.push_back( *it );
    }
}

/**
 * Add a status to the list of statuses. If an empty pointer is supplied, 
 * it returns without doing anything.
 * 
 * @param[in] inStatus A pointer to a status object.
 * 
 * @exception Error Status could not be added.
 */
void 
StatusContainer::addStatus(const StatusSharedPtr & inStatus) throw(Error) {
    mStatuses.push_back( inStatus );
}

} // namespace torc::generic

} // namespace torc
