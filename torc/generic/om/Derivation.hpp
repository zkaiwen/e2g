// TORC - Copyright 2010 University of Southern California. All Rights Reserved.

#ifndef TORC_GENERIC_DERIVATION_HPP
#define TORC_GENERIC_DERIVATION_HPP


namespace torc {

namespace generic {

/**
 * @enum  Derivation Type. 
 * Types of possible derivation.
 * It identify the classification of timing information being provided.
 */
enum Derivation
{
  eDerivationUndefined,
  eDerivationCalculated,
  eDerivationMeasured,
  eDerivationRequired

};

} // namespace torc::generic

} // namespace torc
#endif
