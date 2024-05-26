#include "include.h"

int main ( )
{
#ifdef _WIN32
    WSADATA wsData;
    if ( WSAStartup ( MAKEWORD ( 2 , 2 ) , &wsData ) != 0 ) {
        std::cerr << "WSAStartup failed\n";
        return 1;
    }
#endif

    int server_fd , client_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof ( address );

    if ( ( server_fd = socket ( AF_INET , SOCK_STREAM , 0 ) ) == 0 ) 
    {
        perror ( "socket failed" );
        return 1;
    }

    #ifdef _WIN32
        const int REUSE_OPTION = SO_REUSEADDR;
    #else
        const int REUSE_OPTION = SO_REUSEADDR | SO_REUSEPORT;
    #endif

    if ( setsockopt ( server_fd , SOL_SOCKET , REUSE_OPTION , reinterpret_cast< const char* >( &opt ) , sizeof ( opt ) ) )
    {
        perror ( "setsockopt" );
        CLOSESOCKET ( server_fd );
        return 1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons ( data::PORT );

    if ( bind ( server_fd , reinterpret_cast< struct sockaddr* >( &address ) , sizeof ( address ) ) < 0 )
    {
        perror ( "bind failed" );
        CLOSESOCKET ( server_fd );
        return 1;
    }

    if ( listen ( server_fd , 3 ) < 0 )
    {
        perror ( "listen" );
        CLOSESOCKET ( server_fd );
        return 1;
    }

    LOG ( INFO ) << "Tanzanite Web Server is running on port " << data::PORT;

    while ( true ) {
        if ( ( client_socket = accept ( server_fd , reinterpret_cast< struct sockaddr* >( &address ) , reinterpret_cast< socklen_t* >( &addrlen ) ) ) < 0 )
        {
            perror ( "accept" );
            CLOSESOCKET ( server_fd );
            return 1;
        }

        std::thread ( client::handle_client , client_socket ).detach ( );
    }

#ifdef _WIN32
    WSACleanup ( );
#endif

    return 0;
}
