// TORC - Copyright 2010 University of Southern California. All Rights Reserved.

#ifndef TORC_GENERIC_SCALE_FACTOR_HPP
#define TORC_GENERIC_SCALE_FACTOR_HPP

#include "torc/generic/om/DumpRestoreConfig.hpp"

//BOOST
#ifdef GENOM_SERIALIZATION
#include <boost/serialization/access.hpp>
#endif //GENOM_SERIALIZATION

#include "torc/generic/om/Value.hpp"

namespace torc {

namespace generic {

/**
 * Represents the technology dependent scale factor. This is used by clients to perform calculations using the units involved.
 */
class ScaleFactor
{
#ifdef GENOM_SERIALIZATION
    friend class boost::serialization::access;
#endif //GENOM_SERIALIZATION

  public:
    /**
     * Get the EDIF scale.
     *
     * @return EDIF scale.
     */
    inline const Value::Number
    getEdifValue() const throw();

    /**
     * Set the EDIF scale.
     *
     * @param[in] inSource EDIF scale.
     */
    void
    setEdifValue(const Value::Number & inSource) throw();

    /**
     * Get the SI untis scale.
     *
     * @return SI scale.
     */
    inline const Value::Number
    getSiValue() const throw();

    /**
     * Set the SI scale.
     *
     */
    void
    setSiValue(const Value::Number & inSource) throw();

    ScaleFactor();

    ScaleFactor( const ScaleFactor &inSource );

    ~ScaleFactor();

    ScaleFactor &
    operator = ( const ScaleFactor &inSource );

  private:
#ifdef GENOM_SERIALIZATION
    template<class Archive> void
    serialize( Archive &ar, unsigned int );
#endif //GENOM_SERIALIZATION

    Value::Number mEdifValue;
    Value::Number mSiValue;
};

/**
 * Get the EDIF scale.
 *
 * @return EDIF scale.
 */
inline const Value::Number
ScaleFactor::getEdifValue() const throw() {
  return mEdifValue;
}

/**
 * Get the SI untis scale.
 *
 * @return SI scale.
 */
inline const Value::Number
ScaleFactor::getSiValue() const throw() {
  return mSiValue;
}


} // namespace torc::generic

} // namespace torc
#endif
