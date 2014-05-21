// TORC - Copyright 2010 University of Southern California. All Rights Reserved.

#ifndef TORC_GENERIC_MESSAGE_HPP
#define TORC_GENERIC_MESSAGE_HPP

#include <string>
#include "torc/generic/util/MessageSeverity.hpp"

namespace torc {

namespace generic {

/**
 * The actual message object. It consists of the actual message string and the currently set severity of this message.
 */
class Message
{
  public:
    inline const std::string
    getMessage() const throw();

    void
    setMessage(const std::string & inSource) throw();

    inline const MessageSeverity
    getSeverity() const throw();

    void
    setSeverity(const MessageSeverity & inSource) throw();

    Message();

    Message( const std::string &inMessage,
            MessageSeverity inSeverity );

    ~Message() throw();

    Message(const Message &inSource);

    Message &
    operator=(const Message &inSource) throw();

  private:
    std::string mMessage;
    MessageSeverity mSeverity;

};
inline const std::string
Message::getMessage() const throw() {
  return mMessage;
}

inline const MessageSeverity
Message::getSeverity() const throw() {
  return mSeverity;
}

} // namespace torc::generic

} // namespace torc
#endif
