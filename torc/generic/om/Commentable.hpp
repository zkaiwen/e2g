// TORC - Copyright 2010 University of Southern California. All Rights Reserved.

#ifndef TORC_GENERIC_COMMENTABLE_HPP
#define TORC_GENERIC_COMMENTABLE_HPP

#include "torc/generic/om/DumpRestoreConfig.hpp"

#include <string>
#include <vector>

#ifdef GENOM_SERIALIZATION
#include <boost/serialization/access.hpp>
#include <boost/serialization/weak_ptr.hpp>
#endif //GENOM_SERIALIZATION

namespace torc {

namespace generic {

/**
 * @brief Represents all classes that can hold user comments
 *
 * The Commentable class holds a list of user comments that the users can modify. This class is inherited by all classes that can hold comments by user. Comments can be programatically inserted by clients or can be specified in edif files by <i>(comment "string1" ..)</i> syntax.
 */
class Commentable
{
#ifdef GENOM_SERIALIZATION
	friend class boost::serialization::access;
#endif

  public:
    /**
     * Add a comment to the object
     *
     * @param[in] comment The comment to add to an existing list of comments
     */
    void
    addComment(const std::string & comment) throw();

    /**
     * Get the list of all comments
     *
     * @return A list of all comments on this object
     */
    inline const std::vector<std::string> &
    getComments() const throw();

    /**
     * Get the list of all comments
     *
     * @param[in] inSource A list of all comments to be set on this object
     */
    void
    setComments(const std::vector<std::string> & inSource) throw();


  protected:
    Commentable();


  public:
    virtual
    ~Commentable() throw();


  private:
#ifdef GENOM_SERIALIZATION
	template<class Archive> void
	serialize( Archive &ar, unsigned int );
#endif //GENOM_SERIALIZATION

    Commentable(const Commentable & source) throw();

    Commentable &
    operator=(const Commentable & source) throw();

    std::vector<std::string> mComments;
};

/**
 * Get the list of all comments
 *
 * @return A list of all comments on this object
 */
inline const std::vector<std::string> &
Commentable::getComments() const throw() {
  return mComments;
}

} // namespace torc::generic

} // namespace torc
#endif
