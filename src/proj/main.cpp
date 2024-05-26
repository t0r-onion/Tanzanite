#include <iostream>
#include <thread>
#include <vector>
#include <sstream>
#include <fstream>
#include <cstring>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/socket.h>
#include <algorithm>

const int PORT = 8080;
const int BUFFER_SIZE = 1024;

std::string get_error_response(int status_code, const std::string& error_message) {
    std::string error_page_path = "error.html";
    std::ifstream error_file(error_page_path);
    if (!error_file.is_open()) {
        return "HTTP/1.1 500 Internal Server Error\r\n"
               "Content-Type: text/plain\r\n"
               "Content-Length: 22\r\n"
               "\r\n"
               "500 Internal Server Error";
    }

    std::stringstream buffer;
    buffer << error_file.rdbuf();
    std::string error_content = buffer.str();
    error_file.close();

    std::string status_code_str = std::to_string(status_code);
    size_t pos = error_content.find("%ERROR_CODE%");
    while (pos != std::string::npos) {
        error_content.replace(pos, std::string("%ERROR_CODE%").length(), status_code_str);
        pos = error_content.find("%ERROR_CODE%", pos + status_code_str.length());
    }

    pos = error_content.find("%ERROR_MESSAGE%");
    if (pos != std::string::npos) {
        error_content.replace(pos, std::string("%ERROR_MESSAGE%").length(), error_message);
    }

    std::string error_response =
            "HTTP/1.1 " + status_code_str + " Not Found\r\n"
                                            "Content-Type: text/html\r\n"
                                            "Content-Length: " + std::to_string(error_content.size()) + "\r\n"
                                                                                                        "\r\n" + error_content;

    return error_response;
}


bool ends_with(const std::string& str, const std::string& suffix) {
    return str.size() >= suffix.size() &&
           str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

std::string get_mime_type(const std::string& path) {
    if (ends_with(path, ".html")) return "text/html";
    if (ends_with(path, ".css")) return "text/css";
    if (ends_with(path, ".js")) return "application/javascript";
    if (ends_with(path, ".png")) return "image/png";
    if (ends_with(path, ".jpg")) return "image/jpeg";
    if (ends_with(path, ".gif")) return "image/gif";
    return "application/octet-stream";
}
std::string read_file(const std::string& file_path) {
    std::ifstream file(file_path);
    if (!file.is_open()) return "404 Not Found";

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);

    read(client_socket, buffer, BUFFER_SIZE - 1);
    std::string request(buffer);

    std::istringstream request_stream(request);
    std::string method;
    std::string path;
    request_stream >> method >> path;

    std::string response;
    if (method == "GET") {
        std::string file_path = "." + path;
        if (file_path == "./") file_path = "./index.html";
        std::string response_body = read_file(file_path);
        if (response_body == "404 Not Found") {
            response = get_error_response(404, "Page Not Found");
        } else {
            std::string mime_type = get_mime_type(file_path);
            response =
                    "HTTP/1.1 200 OK\r\n"
                    "Content-Type: " + mime_type + "\r\n"
                                                   "Content-Length: " + std::to_string(response_body.size()) + "\r\n"
                                                                                                               "\r\n" + response_body;
        }
    } else {
        response = get_error_response(405, "Method Not Allowed");
    }

    write(client_socket, response.c_str(), response.size());
    close(client_socket);
}

int main() {
    int server_fd, client_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    std::cout << "Tanzanite Server is running on port " << PORT << std::endl;

    while (true) {
        if ((client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            close(server_fd);
            exit(EXIT_FAILURE);
        }

        std::thread(handle_client, client_socket).detach();
    }

    return 0;
}
