#ifndef __LEEP_NETWORKING_SOCKET_CONNECTION_H__
#define __LEEP_NETWORKING_SOCKET_CONNECTION_H__

#include <list>

namespace leep
{
    class Connection
    {
    public:
        Connection();
        ~Connection();

        void connect();

    private:
        void platformConnection();
        std::list<void*> send_data_;
        std::list<void*> recv_data_;
    };
}

#endif // __LEEP_NETWORKING_SOCKET_CONNECTION_H__