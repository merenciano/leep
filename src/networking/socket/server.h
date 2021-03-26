#ifndef __LEEP_NETWORKING_SOCKET_SERVER_H__
#define __LEEP_NETWORKING_SOCKET_SERVER_H__

namespace leep
{
    class Server
    {
    public:
        Server();
        ~Server();

        void run();

    private:
        void startServer();
    };
}

#endif // __LEEP_NETWORKING_SOCKET_SERVER_H__