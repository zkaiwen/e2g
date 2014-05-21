// TORC - Copyright 2010 University of Southern California. All Rights Reserved. 

#ifndef TORC_GENERIC_EDIF_PARSER_HPP
#define TORC_GENERIC_EDIF_PARSER_HPP

//BOOST
#include <boost/shared_ptr.hpp>

#include "torc/generic/om/PointerTypes.hpp"
#include "torc/generic/parser/ParserPointerTypes.hpp"
#include "torc/generic/util/Error.hpp"

namespace torc { namespace generic { class ObjectFactory; }  } 
namespace torc { namespace generic { class ParserOptions; }  } 
namespace torc { namespace generic { class Root; }  } 
namespace torc { namespace generic { class Linker; }  } 

namespace torc {

namespace generic {

/**
 * @brief The Top level parser for parsing EDIF files.
 * 
 * Most of this class will be generated during implementation. This document will be updated acoordingly.
 */
class EdifParser
{
  public:

	/**
	 * Parse a given edif file
	 * @param[in] inFileName Name of file to be parsed
	 * @param[out] outRoot Root to be populated
	 * @param[out] outLinker Linker to be used for this parse 
	 * @param[in] inFactory Factory object to be used for parsing
	 * @param[in] inOptions Options to the parser
	 */
    void
    parse(const std::string &inFileName,
        const RootSharedPtr &outRoot,
        const LinkerSharedPtr &outLinker,
        const ObjectFactorySharedPtr &inFactory,
        const ParserOptions &inOptions) throw(Error);

	/**
	 * Parse a given set of edif files
	 * @param[in] inFileNames Names of file to be parsed
	 * @param[out] outRoot Root to be populated
	 * @param[out] outLinker Linker to be used for this parse 
	 * @param[in] inFactory Factory object to be used for parsing
	 * @param[in] inOptions Options to the parser
	 */
    void
    parse(const std::vector<std::string> &inFileNames,
        const RootSharedPtr &outRoot,
        const LinkerSharedPtr &outLinker,
        const ObjectFactorySharedPtr &inFactory,
        const ParserOptions &inOptions) throw(Error);

    EdifParser();
    EdifParser(const EdifParser &inSource);

    virtual
    ~EdifParser() throw();

    EdifParser &
    operator=(const EdifParser &inSource) throw();
};

} // namespace torc::generic

} // namespace torc
#endif
