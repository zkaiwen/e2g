// TORC - Copyright 2010 University of Southern California. All Rights Reserved. 

#ifndef TORC_GENERIC_NAMEABLE_HPP
#define TORC_GENERIC_NAMEABLE_HPP

#include "torc/generic/om/DumpRestoreConfig.hpp"

//BOOST
#ifdef GENOM_SERIALIZATION
#include <boost/serialization/access.hpp>
#include <boost/serialization/weak_ptr.hpp>
#endif //GENOM_SERIALIZATION

#include "torc/generic/util/Error.hpp"

namespace torc {

namespace generic {

/**
 * @brief An object that has a name
 * 
 * The Nameable class provides an interface for and implements functionality that allows an object of a derivd class to have a string name.
 */
class Nameable
{
#ifdef GENOM_SERIALIZATION
	friend class boost::serialization::access;
#endif

  protected:
    explicit Nameable();

  public:
    virtual ~Nameable() throw();

  private:
    Nameable(const Nameable & source) throw();

    Nameable &
	operator=(const Nameable & source) throw();

  public:
    /**
     * Get the object name
     * 
     * @return Name of the object
     */
    inline virtual const std::string
	getName() const throw();

    /**
     * Set a name for this object
     * 
     * @param[in] inSource Name of the object
     * 
     * @exception Error Could not set name, because name is empty (Will be used by array members)
     *       <ul>
     *         <li>
     *             Id : eMessageIdErrorEmptyItemName
     *         </li>
     *         <li> Context Data
     *           <ul>
     *               <li>Nameable - <i>String</i></li>
     *           </ul>
     *         </li>
     *       </ul>
     */
    void
	setName(const std::string & inSource) throw(Error);

  private:
#ifdef GENOM_SERIALIZATION
	template<class Archive> void
	serialize( Archive &ar, unsigned int );
#endif //GENOM_SERIALIZATION

    std::string mName;
};

/**
 * Get the object name
 * 
 * @return Name of the object
 */
inline const std::string
Nameable::getName() const throw() {
  return mName;
}

} // namespace torc::generic

} // namespace torc
#endif //TORC_GENERIC_NAMEABLE_HPP
