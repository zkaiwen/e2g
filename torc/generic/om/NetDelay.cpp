// TORC - Copyright 2010 University of Southern California. All Rights Reserved.

#include "torc/generic/om/NetDelay.hpp"

namespace torc {

namespace generic {

/**
 * Set derivation
 * 
 * @param[in] value Derivation value
 */
void 
NetDelay::setDerivation(const Derivation & value) throw() {
    mDerivation = value;
}

/**
 * Set delay value
 * 
 * @param[in] value Delay value. Must be Value::MiNoMax
 */
void 
NetDelay::setDelay(const Value::MiNoMax & value) throw() {
    mDelay = value;
}

NetDelay::NetDelay()
    :mDerivation( eDerivationUndefined ),
    mDelay() {
}

NetDelay::~NetDelay() throw() {
}

NetDelay::NetDelay(const NetDelay & source) throw()
    : mDerivation( source.mDerivation ),
    mDelay( source.mDelay ) {
}

NetDelay &
NetDelay::operator=(const NetDelay & source) throw() {
    if( this != &source )
    {
        mDerivation = source.mDerivation;
        mDelay = source.mDelay;
    }
    return *this;
}

} // namespace torc::generic

} // namespace torc
