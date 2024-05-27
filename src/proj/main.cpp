#include "inc.h"

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
        LOG(INFO) << "Received GET Request.";

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
        LOG(INFO) << "Served GET Request.";
    }
    else if (method == "POST")
    {
        LOG(INFO) << "Received POST Request.";

        std::string request_body;
        size_t content_length = 0;
        std::string line;

        while (std::getline(request_stream, line) && !line.empty())
        {
            if (line.substr(0, 15) == "Content-Length: ")
            {
                content_length = std::stoul(line.substr(15));
            }
        }

        if (content_length > 0)
        {
            std::vector<char> buffer(content_length, 0);

            request_stream.read(buffer.data(), content_length);
            request_body = std::string(buffer.data(), content_length);
        }

        std::string response_body = "Received POST data: " + request_body;

        response =
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: text/plain\r\n"
                "Content-Length: " + std::to_string(response_body.size()) + "\r\n"
                                                                            "\r\n" + response_body;

        LOG(INFO) << "Served POST Request.";
    }
    else
    {
        response = get_error_response(405, "Method Not Allowed");
    }

    send ( client_socket , response.c_str ( ) , response.size ( ) , 0 );
    CLOSESOCKET ( client_socket );
}

int main ( )
{
#ifdef _WIN32
    WSADATA wsData;
    if ( WSAStartup ( MAKEWORD ( 2 , 2 ) , &wsData ) != 0 )
    {
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

    while ( true )
    {
        if ( ( client_socket = accept ( server_fd , reinterpret_cast< struct sockaddr* >( &address ) , reinterpret_cast< socklen_t* >( &addrlen ) ) ) < 0 )
        {
            perror ( "accept" );
            CLOSESOCKET ( server_fd );
            return 1;
        }

        std::thread ( handle_client , client_socket ).detach ( );
    }

#ifdef _WIN32
    WSACleanup ( );
#endif

    return 0;
}