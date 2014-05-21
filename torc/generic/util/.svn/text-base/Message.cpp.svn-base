// TORC - Copyright 2010 University of Southern California. All Rights Reserved.

#ifndef HAVE_CONFIG_H
#include "torc/generic/config.h"
#endif

#include "torc/generic/util/Message.hpp"

namespace torc {

namespace generic {

void
Message::setMessage(const std::string &inSource) throw() {
  mMessage = inSource;
}

void
Message::setSeverity(
    const MessageSeverity & inSource) throw() {
  mSeverity = inSource;
}

Message::Message()
    :mMessage(),
    mSeverity( eMessageSeveritySuppressed ) {
}

Message::Message( const std::string &inMessage,
            MessageSeverity inSeverity )
    :mMessage( inMessage ),
    mSeverity( inSeverity ) {
}

Message::~Message() throw() {
}

Message::Message(const Message & source)
    :mMessage( source.mMessage ),
    mSeverity( source.mSeverity ) {
}

Message &
Message::operator=(const Message & source) throw() {
    if( this != &source )
    {
        mMessage = source.mMessage;
        mSeverity = source.mSeverity;
    }
    return *this;
}


} // namespace torc::generic

} // namespace torc
