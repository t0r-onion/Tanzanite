#ifndef TANZANITE_RESPONSE_H
#define TANZANITE_RESPONSE_H

#include "../inc.h"

std::string get_error_response
        ( int status_code , const std::string& error_message )
{
    std::string error_page_path = "error.html";
    std::ifstream error_file ( error_page_path );
    if ( !error_file.is_open ( ) )
    {
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

    size_t memory_usage = get_memory_usage ( );
    std::string memory_usage_str = std::to_string ( memory_usage / ( 1024 * 1024 ) ) + " MB"; // Convert to MB
    pos = error_content.find ( "%MEMORY_USAGE%" );
    if ( pos != std::string::npos )
    {
        error_content.replace ( pos , std::string ( "%MEMORY_USAGE%" ).length ( ) , memory_usage_str );
    }

    std::string error_response =
            "HTTP/1.1 " + status_code_str + " Not Found\r\n"
                                            "Content-Type: text/html\r\n"
                                            "Content-Length: " + std::to_string ( error_content.size ( ) ) + "\r\n"
                                                                                                             "\r\n" + error_content;

    return error_response;
}

std::string macro_response ( const std::string& html_content )
{
    size_t pos;

    size_t memory_usage = get_memory_usage ( );
    std::string memory_usage_str = std::to_string ( memory_usage / ( 1024 * 1024 ) ) + " MB"; // Convert to MB
    std::string memory_usage_placeholder = "%MEMORY_USAGE%";

    pos = html_content.find ( memory_usage_placeholder );
    if ( pos != std::string::npos )
    {
        std::string before = html_content.substr ( 0 , pos );
        std::string after = html_content.substr ( pos + memory_usage_placeholder.length ( ) );
        std::string modified_content = before + memory_usage_str + after;

        std::string macro_response =
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: text/html\r\n"
                "Content-Length: " + std::to_string ( modified_content.size ( ) ) + "\r\n"
                                                                                    "\r\n" + modified_content;
        return macro_response;
    }
    else {
        // If the placeholder is not found, return the original HTML content
        std::string macro_response =
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: text/html\r\n"
                "Content-Length: " + std::to_string ( html_content.size ( ) ) + "\r\n"
                                                                                "\r\n" + html_content;
        return macro_response;
    }
}

#endif
