#ifndef _HELPERS_
#define _HELPERS_

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
#include <sstream>
#include <array>

// shows the current error
void error(const char *msg);

// opens a connection with server host_ip on port portno, returns a socket
int open_connection(const char *host_ip, const int portno, int ip_type, int socket_type, int flag);

// closes a server connection on socket sockfd
void close_connection(int sockfd);

// send a message to a server
void send_to_server(int sockfd, std::string &message);

// receives and returns the message from a server
std::string receive_from_server(int sockfd);

// extracts and returns the cookie from a server response
std::string extract_cookie(std::string &response);

// extracts and returns the token from a server response
std::string extract_token(std::string &response);

// checks if a string is a number
bool isNumber(std::string &s);

// creates a JSON object for a register request
std::array<std::string, 2> user_info(const std::string &type);

// creates a JSON object for a add_book POST request
std::string create_book();

// checks if the user is logged in
bool check_login(std::string &login_cookie);

// check if the user has acces to the library
bool check_token(std::string &token);

// extract the error from the server
std::string extract_server_error(const std::string &response);

// extract the book from the server
std::string get_book(std::string &response);

// extract all books from the server
void get_books(std::string &response);

// retrieves a book id from the client
std::string retrieve_book_id();

#endif /* _HELPERS_ */
