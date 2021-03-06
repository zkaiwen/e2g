/*******************************************************************************
* TORC - Copyright 2010 University of Southern California. All Rights Reserved.
*
* FILE : driver.cpp
*
* DATE : 08-July-2010
*
* DESCRIPTION : Defination of the torc::generic::Driver class
*
* REVISION HISTORY:
*
* SI        REVISION        AUTHOR               CHANGES          PRs
*[0]    Initial Version    Niladri
*[1]    Initial Version    Santanu
*
*******************************************************************************/

#include <fstream>
#include <sstream>

#include "torc/generic/parser/Driver.hpp"
#include "torc/generic/parser/ParserHelpers.hpp"
#include "torc/generic/om/PortAttributes.hpp"
#include "torc/generic/om/NetAttributes.hpp"
#include "torc/generic/parser/Scanner.hpp"

namespace torc {
namespace generic {

Driver::Driver(const EdifContextSharedPtr &inEdifCntx)
    : mTraceScanning(false),
      mTraceParsing(false),
      mEdifCntx(inEdifCntx),
      mErrorObj() {
}

Driver::~Driver() throw() {
}

bool
Driver::parseStream(std::istream& inStream,
        const std::string& inStreamName) {
    mStreamName = inStreamName;
    Scanner scanner(&inStream);
    scanner.set_debug(mTraceScanning);
    this->mLexer = &scanner;

    Parser parser(*this);
    parser.set_debug_level(mTraceParsing);
    return (parser.parse() == 0);
}

bool
Driver::parseFile(const std::string &inFileName) {
    std::ifstream in(inFileName.c_str());
    if (!in.good())
    {
        return false;
    }
    return parseStream(in, inFileName);
}

bool
Driver::parseString(const std::string &inString,
                    const std::string& inStreamName) {
    std::istringstream iss(inString);
    return parseStream(iss, inStreamName);
}

void
Driver::error(const location& inLocation, const std::string& inMessage) {
    std::cerr << "There was error at:"<<
                inLocation << ": " << inMessage << std::endl;
    std::cout << "There was error at:"<<
                inLocation << ": " << inMessage << std::endl;
}

void
Driver::error(const std::string& inMessage) {
    std::cerr << inMessage << std::endl;
}

void
Driver::setParserError( const Error &inSource) throw() {
  mErrorObj =  ErrorSharedPtr( new Error(inSource) );
}

} // namespace generic
} // namespace torc
