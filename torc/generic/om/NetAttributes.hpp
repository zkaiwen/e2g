// TORC - Copyright 2010 University of Southern California. All Rights Reserved.

#ifndef TORC_GENERIC_NET_ATTRIBUTES_HPP
#define TORC_GENERIC_NET_ATTRIBUTES_HPP

#include "torc/generic/om/NetDelay.hpp"

namespace torc {

namespace generic {

/**
 * @brief Represents attributes of a Net object
 * 
 * The NetAttributes class stores some optional attributes of a Net. 
 */
class NetAttributes
{
  public:
    /**
     * Get the criticality info of a net for routing.
     *
     * @return Integer value representing criticality info of a net.
    */
    inline const int32_t 
    getCriticality() const throw();

    /**
     * Set the criticality info of a net for routing.
     *
     * @param[in] value Integer value representing criticality info of a net.
    */
    void 
    setCriticality(const int32_t & value) throw();

    /**
     * Get delay information of net
     *
     * @return NetDelay 
     */
    inline const NetDelay 
    getNetDelay() const throw();

    /**
     * Set delay information of net
     *
     * @param[in] inSource NetDelay, the delay information of a net. 
     */
    void
    setNetDelay(const NetDelay & inSource) throw();

    inline const bool
    getIsNetDelaySet() const throw();

    inline const bool
    getIsCriticalitySet() const throw();

    NetAttributes();

    ~NetAttributes() throw();

    NetAttributes(const NetAttributes & source);

    NetAttributes & 
    operator=(const NetAttributes & source) throw();

  private:
    /**
     * This attribute is used to describe the relative importance of the net 
     * compared to other nets, for routing purpose.
     */
    int32_t mCriticality;

    /**
     * This is an attributes of a net to specify a delay 
     * and its derivation for the given set of transitions.
     */
    NetDelay mNetDelay;
    bool mCriticalitySet;
    bool mNetDelaySet;

};

inline const int32_t 
NetAttributes::getCriticality() const throw() {
    return mCriticality;
}

inline const NetDelay 
NetAttributes::getNetDelay() const throw() {
    return mNetDelay;
}

inline const bool
NetAttributes::getIsCriticalitySet() const throw() {
  return mCriticalitySet;
}

inline const bool
NetAttributes::getIsNetDelaySet() const throw() {
  return mNetDelaySet;
}

} // namespace torc::generic

} // namespace torc
#endif
