#include "include.h"

<<<<<<< Updated upstream
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif
#include "utils/logger.h"

const int PORT = 8080;
const int BUFFER_SIZE = 1024;

#ifdef _WIN32
#define CLOSESOCKET closesocket
#else
#define CLOSESOCKET close
#endif

std::string get_error_response
( int status_code , const std::string& error_message )
{
    std::string error_page_path = "error.html";
    std::ifstream error_file ( error_page_path );
    if ( !error_file.is_open ( ) ) {
        return "HTTP/1.1 500 Internal Server Error\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: 22\r\n"
            "\r\n"
            "500 Internal Server Error";
    }

    std::stringstream buffer;
    buffer << error_file.rdbuf ( );
    std::string error_content = buffer.str ( );
    error_file.close ( );

    std::string status_code_str = std::to_string ( status_code );
    size_t pos = error_content.find ( "%ERROR_CODE%" );
    while ( pos != std::string::npos ) 
    {
        error_content.replace ( pos , std::string ( "%ERROR_CODE%" ).length ( ) , status_code_str );
        pos = error_content.find ( "%ERROR_CODE%" , pos + status_code_str.length ( ) );
    }

    pos = error_content.find ( "%ERROR_MESSAGE%" );
    if ( pos != std::string::npos ) 
    {
        error_content.replace ( pos , std::string ( "%ERROR_MESSAGE%" ).length ( ) , error_message );
    }

    std::string error_response =
        "HTTP/1.1 " + status_code_str + " Not Found\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: " + std::to_string ( error_content.size ( ) ) + "\r\n"
        "\r\n" + error_content;

    return error_response;
}

bool ends_with 
( const std::string& str , const std::string& suffix ) 
{
    return str.size ( ) >= suffix.size ( ) &&
        str.compare ( str.size ( ) - suffix.size ( ) , suffix.size ( ) , suffix ) == 0;
}

std::string get_mime_type
( const std::string& path ) 
{
    if ( ends_with ( path , ".html" ) ) return "text/html";
    if ( ends_with ( path , ".css" ) ) return "text/css";
    if ( ends_with ( path , ".js" ) ) return "application/javascript";
    if ( ends_with ( path , ".png" ) ) return "image/png";
    if ( ends_with ( path , ".jpg" ) ) return "image/jpeg";
    if ( ends_with ( path , ".gif" ) ) return "image/gif";
    return "application/octet-stream";
}

std::string read_file
( const std::string& file_path , bool is_binary = false ) 
{
    std::ifstream file ( file_path , is_binary ? std::ios::binary : std::ios::in );
    if ( !file.is_open ( ) ) return "";

    std::stringstream buffer;
    buffer << file.rdbuf ( );
    return buffer.str ( );
}

void handle_client
( int client_socket )
{
    char buffer [ BUFFER_SIZE ];
    memset ( buffer , 0 , BUFFER_SIZE );

    recv ( client_socket , buffer , BUFFER_SIZE - 1 , 0 );
    std::string request ( buffer );

    std::istringstream request_stream ( request );
    std::string method;
    std::string path;
    request_stream >> method >> path;

    std::string response;
    if ( method == "GET" ) 
    {
        std::string file_path = "." + path;
        if ( file_path == "./" ) file_path = "./index.html";
        bool is_binary = ends_with ( file_path , ".png" ) || ends_with ( file_path , ".jpg" ) || ends_with ( file_path , ".gif" );

        std::string response_body = read_file ( file_path , is_binary );
        if ( response_body.empty ( ) )
        {
            response = get_error_response ( 404 , "Page Not Found" );
        }
        else 
        {
            std::string mime_type = get_mime_type ( file_path );
            response =
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: " + mime_type + "\r\n"
                "Content-Length: " + std::to_string ( response_body.size ( ) ) + "\r\n"
                "\r\n" + response_body;
        }
    }
    else
    {
        response = get_error_response ( 405 , "Method Not Allowed" );
    }

    send ( client_socket , response.c_str ( ) , response.size ( ) , 0 );
    CLOSESOCKET ( client_socket );
}

=======
>>>>>>> Stashed changes
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

<<<<<<< Updated upstream
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
    address.sin_port = htons ( PORT );

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

    LOG ( INFO ) << "Tanzanite Web Server is running on port " << PORT;

    while ( true ) {
        if ( ( client_socket = accept ( server_fd , reinterpret_cast< struct sockaddr* >( &address ) , reinterpret_cast< socklen_t* >( &addrlen ) ) ) < 0 )
        {
            perror ( "accept" );
            CLOSESOCKET ( server_fd );
            return 1;
        }

        std::thread ( handle_client , client_socket ).detach ( );
    }

#ifdef _WIN32
=======
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
>>>>>>> Stashed changes
    WSACleanup ( );
#endif

    return 0;
}
