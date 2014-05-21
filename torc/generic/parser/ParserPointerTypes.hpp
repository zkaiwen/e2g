// TORC - Copyright 2010 University of Southern California. All Rights Reserved.

#ifndef PARSER_POINTER_TYPES_HPP
#define PARSER_POINTER_TYPES_HPP

//BOOST
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

namespace torc {
namespace generic {

class EdifContext;
typedef boost::shared_ptr<EdifContext> EdifContextSharedPtr;
typedef boost::weak_ptr<EdifContext> EdifContextWeakPtr;

class Linker;
typedef boost::shared_ptr<Linker> LinkerSharedPtr;
typedef boost::weak_ptr<Linker> LinkerWeakPtr;

} // namespace torc::generic

} // namespace torc
#endif

