// TORC - Copyright 2010 University of Southern California. All Rights Reserved. 

#ifndef TORC_GENERIC_COMPOSITION_TYPE_HPP
#define TORC_GENERIC_COMPOSITION_TYPE_HPP

/**
 * @file CompositionType.hpp
 * @brief Contains definition for CompositionType
 * 
 * This file contains the contains the definition for CompositionType
 */
namespace torc {

namespace generic {

/**
 * @enum CompositionType
 * @brief Defines possible Composition types
 * 
 * Used in composite objects, this type indicates whether an object is a Scalar, a Vector, a bit of a vector (VectorBit) or a Bundle
 */
enum CompositionType
{
  eCompositionTypeScalar,
  eCompositionTypeVector,
  eCompositionTypeVectorBit,
  eCompositionTypeBundle

};

} // namespace torc::generic

} // namespace torc
#endif
