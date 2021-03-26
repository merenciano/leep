#ifdef LEEP_WIN
#include "networking/socket/windows/win-server.cpp"
#endif
#ifdef LEEP_LINUX
#include "networking/socket/linux/linux-server.cpp"
#endif

#include "server.h"
#include <future>

namespace leep {

Server::Server()
{

}

Server::~Server()
{

}

void Server::run()
{
    std::async(std::launch::async, PlatformRunServer);
}

} // namespace leep