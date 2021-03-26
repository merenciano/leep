#include "connection.h"

#include <future>

namespace leep {

Connection::Connection()
{

}

Connection::~Connection()
{

}

void Connection::connect()
{
    std::async(std::launch::async, &Connection::platformConnection, this);
}

} // namespace leep

#ifdef LEEP_WIN
#include "networking/socket/windows/win-connection.cpp"
#endif
#ifdef LEEP_LINUX
#include "networking/socket/linux/linux-connection.cpp"
#endif