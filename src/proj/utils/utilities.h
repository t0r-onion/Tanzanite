#pragma once

#include "../inc.h"

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