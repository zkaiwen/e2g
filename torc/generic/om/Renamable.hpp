// TORC - Copyright 2010 University of Southern California. All Rights Reserved.

#ifndef TORC_GENERIC_RENAMABLE_HPP
#define TORC_GENERIC_RENAMABLE_HPP

#include "torc/generic/om/DumpRestoreConfig.hpp"

#include <string>

//BOOST
#ifdef GENOM_SERIALIZATION
#include <boost/serialization/access.hpp>
#endif //GENOM_SERIALIZATION

namespace torc {

namespace generic {

/**
 * @brief Represents objects that can be renamed
 *
 * EDIF provides a way to store displayable string values with different objects using the (rename ...) construct. The Renamable interface will be generalized by objects that need to support this feature.
 */
class Renamable
{
#ifdef GENOM_SERIALIZATION
	friend class boost::serialization::access;
#endif
  public:
    typedef std::string Name;

    /**
     * Get the new name provided for the object.
     *
     * @return Name of the object
     */
    inline virtual Name
    getOriginalName() const throw();

    /**
     * Set the new name provided for the object.
     *
     * @param[in] inSource Name of the object
     */
    virtual void
    setOriginalName(const Name &inSource) throw();

  protected:
    Renamable();

  public:	
    virtual
    ~Renamable() throw();

  private:
    Renamable( const Renamable & );
    Renamable &
    operator = ( const Renamable & );

  private:
#ifdef GENOM_SERIALIZATION
    template<class Archive> void
    serialize( Archive &ar, unsigned int );
#endif //GENOM_SERIALIZATION

     Name mOriginalName;
};

/**
 * Get the new name provided for the object.
 *
 * @return Name of the object
 */
inline Renamable::Name
Renamable::getOriginalName() const throw() {
  return mOriginalName;
}

} // namespace torc::generic

} // namespace torc
#endif
