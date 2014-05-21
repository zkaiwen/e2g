// TORC - Copyright 2010 University of Southern California. All Rights Reserved.

#include "torc/generic/om/UserDataContainer.hpp"

namespace torc {

namespace generic {

/**
 * Set the list of userData
 *
 * @param[in] inSource The list of UserData
 */
void
UserDataContainer::setUserData(const std::list< std::string > & inSource) throw() {
    std::list< std::string >::const_iterator it = inSource.begin();
    for( ; it != inSource.end(); it++ ) {
        mUserData.push_back( *it );
    }
}

/**
 * Add an user data to the list of user data 
 * @param[in] inSource An user data as string 
 */
void
UserDataContainer::addUserData( const std::string &inSource ) throw() {
    mUserData.push_back( inSource ); 
}

UserDataContainer::UserDataContainer()
     :mUserData() {
}

UserDataContainer::~UserDataContainer() throw() {
}

} // namespace torc::generic

} // namespace torc
