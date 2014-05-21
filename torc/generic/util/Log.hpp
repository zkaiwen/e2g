#ifndef TORC_GENERIC_LOG_HPP
#define TORC_GENERIC_LOG_HPP

#include <string>

namespace torc {
namespace generic {

void
openLogFile( const std::string &logFileName );

void
log( const char *fmt, ... );

} //namespace torc
} //namespace generic

#endif //TORC_GENERIC_LOG_HPP
