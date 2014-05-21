#ifdef HAVE_CONFIG_H
#include "torc/generic/config.h"
#endif //HAVE_CONFIG_H

#ifdef LOGGING
#include <cstdio>
#include <cstdarg>
#endif

#include "torc/generic/util/Log.hpp"

namespace {

#ifdef LOGGING

class Logger {
  public:
    void
    write( const char *fmt, va_list ap );

    void
    openLogFile( const std::string &inFileName );

    Logger();
    ~Logger();

    static Logger *
    instance();

  private:
    FILE *mStream;  
};


void
Logger::write( const char *fmt, va_list ap )
{
    vfprintf( mStream, fmt, ap );
    return;
}

void
Logger::openLogFile( const std::string &inFileName ) {
    if( !inFileName.empty() )
    {
        FILE *fp = fopen( inFileName.c_str(), "w" );
        if( fp )
        {
            if( mStream && mStream != stdout )
            {
                fclose( mStream );
            }
            mStream = fp;
        }
    }
}

Logger::Logger()
    :mStream(stdout) {
}

Logger::~Logger() {
    if( mStream && mStream != stdout )
    {
        fclose( mStream );
		mStream = NULL;
    }
}

Logger *
Logger::instance() {
    static Logger obj;
    return &obj;
}

#endif //LOGGING

}

namespace torc {
namespace generic {
void
openLogFile( const std::string &logFileName )
{
#ifdef LOGGING
    Logger::instance()->openLogFile( logFileName );
#endif
    return;
}

void
log( const char *fmt, ... )
{
#ifdef LOGGING
    va_list args;
    va_start( args, fmt );
    Logger::instance()->write( fmt, args );
    va_end( args );
#endif
    return;
}

} //namespace torc
} //namespace generic

