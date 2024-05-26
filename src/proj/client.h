#ifndef TANZANITE_CLIENT_H
#define TANZANITE_CLIENT_H

#include "include.h"
#include "response.h"

namespace client
{
    void handle_client
            ( int client_socket )
    {
        char buffer [ data::BUFFER_SIZE ];
        memset ( buffer , 0 , data::BUFFER_SIZE );

        recv ( client_socket , buffer , data::BUFFER_SIZE - 1 , 0 );
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
            bool is_binary = util::ends_with ( file_path , ".png" ) || util::ends_with ( file_path , ".jpg" ) || util::ends_with ( file_path , ".gif" );

            std::string response_body = util::read_file ( file_path , is_binary );
            if ( response_body.empty ( ) )
            {
                response = client::get_error_response ( 404 , "Page Not Found" );
            }
            else
            {
                std::string mime_type = util::get_mime_type ( file_path );
                response =
                        "HTTP/1.1 200 OK\r\n"
                        "Content-Type: " + mime_type + "\r\n"
                                                       "Content-Length: " + std::to_string ( response_body.size ( ) ) + "\r\n"
                                                                                                                        "\r\n" + response_body;
            }
        }
        else
        {
            response = client::get_error_response ( 405 , "Method Not Allowed" );
        }

        send ( client_socket , response.c_str ( ) , response.size ( ) , 0 );
        closesocket ( client_socket );
    }
}

#endif