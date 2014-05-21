// TORC - Copyright 2010 University of Southern California. All Rights Reserved.

#ifndef TORC_GENERIC_EXTERN_HPP
#define TORC_GENERIC_EXTERN_HPP

#include "torc/generic/om/DumpRestoreConfig.hpp"

//BOOST
#ifdef GENOM_SERIALIZATION
#include <boost/serialization/access.hpp>
#endif //GENOM_SERIALIZATION

namespace torc {

namespace generic {

/**
 * @brief Used to imeplement extrenal object referencing.
 *
 * The Extern interface is used by objects to create placeholders for objects that could not be found by the Linker. In itself, the class is pretty simple and uses a single boolean inSource to indicate whether an object is a real object or a place holder. Simplistically, extern objects are used to represent libraries, cells, views and ports declared using the EDIF (extern ... ) syntax. However, in case of multifile parsing, the Linker will try to remove externs from the design hierarchy with newly discovered items.
 */
class Extern
{
#ifdef GENOM_SERIALIZATION
	friend class boost::serialization::access;
#endif

  public:
    /**
     * Get whether the item is an extern(placeholder) or an actual item.
     *
     * @return True if extern, false otherwise
     */
    inline bool
    getIsExtern() const throw();

    /**
     * Get whether the item is an extern(placeholder) or an actual item.
     *
     * @return True if extern, false otherwise
     */
    inline bool
    isExtern() const throw();

    /**
     * Set whether an item is extern or not.
     *
     * @param[in] isExtern True if extern, false otherwise
     */
    void
    setIsExtern(bool inIsExtern) throw();


  protected:
    Extern();


  public:
    virtual ~Extern() throw();


  private:
    Extern(const Extern & source) throw();

    Extern &
    operator=(const Extern & source) throw();

  private:
#ifdef GENOM_SERIALIZATION
	template<class Archive> void
	serialize( Archive &ar, unsigned int );
#endif //GENOM_SERIALIZATION

    bool mIsExtern;
};

/**
 * Get whether the item is an extern(placeholder) or an actual item.
 *
 * @return True if extern, false otherwise
 */
inline bool
Extern::getIsExtern() const throw() {
    return mIsExtern;
}

/**
 * Get whether the item is an extern(placeholder) or an actual item.
 *
 * @return True if extern, false otherwise
 */
inline bool
Extern::isExtern() const throw() {
    return mIsExtern;
}

} // namespace torc::generic

} // namespace torc
#endif
