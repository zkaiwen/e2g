// TORC - Copyright 2010 University of Southern California. All Rights Reserved.

#ifndef TORC_GENERIC_NET_DELAY_HPP
#define TORC_GENERIC_NET_DELAY_HPP

#include "torc/generic/om/Derivation.hpp"
#include "torc/generic/om/Value.hpp"

namespace torc {

namespace generic {

/**
 * @brief Represents the netDelay attribute of Net
 * 
 * The Netdelay class stores the delay value for a net. 
 * Currently it supports string only static delay values.
 */
class NetDelay {
  public:
    /**
     * Get derivation
     * 
     * @return Derivation value
     */
    inline const Derivation 
    getDerivation() const throw();

    /**
     * Set derivation
     * 
     * @param[in] value Derivation value
     */
    void 
    setDerivation(const Derivation & value) throw();

    /**
     * Get delay value
     * 
     * @return Value containing Value::MiNoMax
     */
    inline const Value::MiNoMax 
    getDelay() const throw();

    /**
     * Set delay value
     * 
     * @param[in] value Delay value. Must be Value::MiNoMax
     */
    void 
    setDelay(const Value::MiNoMax & value) throw();

    NetDelay();

    ~NetDelay() throw();

    NetDelay(const NetDelay & source) throw();

    NetDelay & 
    operator=(const NetDelay & source) throw();

  private:
    Derivation mDerivation;
    Value::MiNoMax mDelay;

    /**
     * TBD:: Need to support transition & becomes and net delay in 2nd phase  
     * std::string mTransition;
     * std::string mBecomes;
    */
};
/**
 * Get derivation
 * 
 * @return Derivation value
 */
inline const Derivation 
NetDelay::getDerivation() const throw() {
    return mDerivation;
}

/**
 * Get delay value
 * 
 * @return Value containing Value::MiNoMax
 */
inline const Value::MiNoMax 
NetDelay::getDelay() const throw() {
    return mDelay;
}

} // namespace torc::generic

} // namespace torc
#endif
