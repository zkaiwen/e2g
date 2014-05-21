// TORC - Copyright 2010 University of Southern California. All Rights Reserved.

#ifndef TORC_GENERIC_PORT_DELAY_HPP
#define TORC_GENERIC_PORT_DELAY_HPP

#include "torc/generic/om/DumpRestoreConfig.hpp"

#ifdef GENOM_SERIALIZATION
#include <boost/serialization/access.hpp>
#endif //GENOM_SERIALIZATION

#include "torc/generic/om/Derivation.hpp"
#include "torc/generic/om/Value.hpp"

namespace torc {

namespace generic {

/**
 * @brief Represents the portDelay attribute on Port or PortReference
 *
 * The Portdelay class stores the delay inSource for a port. Currently it supports storing only static delay values. Load dependent delay inSource support (using the loadDelay construct) is not present.
 */
class PortDelay
{
  public:
    /**
     * @enum DelayType
     *
     * Types of possible delay
     */
	enum Type
	{
		eTypeDelay,
		eTypeLoadDelay
	};
#ifdef GENOM_SERIALIZATION
    friend class boost::serialization::access;
#endif //GENOM_SERIALIZATION

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
     * @param[in] inSource Derivation inSource
     */
    void
    setDerivation(const Derivation & inSource) throw();

	/**
	 * Get whether value is delay or loadDelay
	 *
	 * @return Type
	 */
	inline const PortDelay::Type
	getType() const throw();

    /**
     * Set whether value is delay or loadDelay
     *
     * @param[in] inSource Type of data
     */
    void
    setType(Type inSource) throw();

	/**
	 * Get delay
	 *
	 * @return MiNoMax value
	 */
	inline const Value::MiNoMax
	getDelay() const throw();

    /**
     * Set delay
     *
     * @param[in] inSource Delay value.
     */
    void
    setDelay(const Value::MiNoMax & inSource) throw();

	/**
	 * Get the AcLoad value. This is valid when value type is loadDelay
	 *
	 * @return MiNoMax value
	 */
	inline const Value::MiNoMax
	getAcLoad() const throw();

    /**
     * Set AC Load
     *
     * @param[in] inSource Load value
     */
    void
    setAcLoad(const Value::MiNoMax & inSource) throw();

	/**
	 * Get transition data as written in EDIF
	 *
	 * @return string
	 */
	inline const std::string
	getTransition() const throw();

    /**
     * Set EDIF string corresponding to <i>transition</i> statement.
     *
     * @param[in] inSource String containing EDIF (transition ...)
     */
    void
    setTransition(const std::string &inSource) throw();

	/**
	 * Get becomes data as written in EDIF
	 *
	 * @return string
	 */
	inline const std::string
	getBecomes() const throw();

    /**
     * Set EDIF string corresponding to <i>becomes</i> statement.
     *
     * @param[in] inSource String containing EDIF (becomes ...)
     */
    void
    setBecomes(const std::string &inSource) throw();

    PortDelay();

    ~PortDelay() throw();

    PortDelay(const PortDelay & source);

    PortDelay &
    operator=(const PortDelay & source) throw();

  private:
#ifdef GENOM_SERIALIZATION
    template<class Archive> void
    serialize( Archive &ar, unsigned int );
#endif //GENOM_SERIALIZATION

    Derivation mDerivation;
	Type mType;
    Value::MiNoMax mDelay;
    Value::MiNoMax mAcLoad;
	std::string mTransition;
	std::string mBecomes;
};

inline const Derivation
PortDelay::getDerivation() const throw() {
  return mDerivation;
}

inline const PortDelay::Type
PortDelay::getType() const throw() {
  return mType;
}

inline const Value::MiNoMax
PortDelay::getDelay() const throw() {
  return mDelay;
}

inline const Value::MiNoMax
PortDelay::getAcLoad() const throw() {
  return mAcLoad;
}

inline const std::string
PortDelay::getTransition() const throw() {
  return mTransition;
}

inline const std::string
PortDelay::getBecomes() const throw() {
  return mBecomes;
}


} // namespace torc::generic

} // namespace torc
#endif
