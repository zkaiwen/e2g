// TORC - Copyright 2010 University of Southern California. All Rights Reserved.

#ifndef TORC_GENERIC_PROPERTY_CONTAINER_HPP
#define TORC_GENERIC_PROPERTY_CONTAINER_HPP

#include "torc/generic/om/PointerTypes.hpp"
#include "torc/generic/om/DumpRestoreConfig.hpp"

//BOOST
#include <boost/shared_ptr.hpp>
#ifdef GENOM_SERIALIZATION
#include <boost/serialization/access.hpp>
#endif //GENOM_SERIALIZATION

#include "torc/generic/om/SymTab.hpp"

namespace torc { namespace generic { class Property; }  }

namespace torc {

namespace generic {

/**
 * @brief Represents objects that have properties
 *
 * The PropertyContainer interface is generalized by classes tat need to hold one or more properties. A property is identified by name in the container.
 */
class PropertyContainer {
#ifdef GENOM_SERIALIZATION
	friend class boost::serialization::access;
#endif

  public:
    /**
     * Get a property
     *
     * @param[in] inName Name of the property to be retreived
     *
     * @return Pointer to the property object if present, empty pointer otherwise.
     */
    PropertySharedPtr
    getProperty(const std::string & inName) throw();

    /**
     * Save the inSource of  a property
     *
     * @param[in] inName Name of the property to be saved
     * @param[in] inProperty Pointer to the property object if present, empty pointer otherwise.
     *
     * @return bool True if saved, false otherwise.
     */
    bool
    setProperty(const std::string & inName,
        const PropertySharedPtr &inProperty) throw();

    /**
     * Get the map of all properties
     *
     * @param[out] outProperties The map of properties
     */
    inline void
    getProperties(
        std::map< std::string, PropertySharedPtr > &outProperties) const throw();

    /**
     * Set the map of properties
     *
     * @param[in] inSource The map of properties
     */
    void
    setProperties(
        const std::map< std::string, PropertySharedPtr > & inSource) throw();

   /**
    * Apply action on all properties.
    * @param[in] action Action to be applied
    *
    */
    template<typename _Action>
    inline void
    applyOnAllProperties( const _Action &action ) throw(Error); 

  protected:
    PropertyContainer();

  public:
    ~PropertyContainer() throw();

  private:
    PropertyContainer(const PropertyContainer & source) throw();
    PropertyContainer & operator=(const PropertyContainer & source) throw();

#ifdef GENOM_SERIALIZATION
	template<class Archive> void
	serialize( Archive &ar, unsigned int );
#endif //GENOM_SERIALIZATION

    SymTab< std::string, PropertySharedPtr > mProperties;
};

/**
 * Get the map of all properties
 *
 * @return The map of properties
 */
inline void
PropertyContainer::getProperties(
    std::map< std::string, PropertySharedPtr > &outProperties    ) const throw() {
  mProperties.getValueMap( outProperties );
  return;
}

template<typename _Action>
inline void
PropertyContainer::applyOnAllProperties( const _Action &action ) throw(Error)
{
    try
    {
        mProperties.applyOnAll( action );
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
