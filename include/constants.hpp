#ifndef _CONSTANTS_
#define _CONSTANTS_

constexpr auto REGISTER_ROUTE = "/api/v1/tema/auth/register";
constexpr auto LOGIN_ROUTE = "/api/v1/tema/auth/login";
constexpr auto LOGOUT_ROUTE = "/api/v1/tema/auth/logout";
constexpr auto ACCESS_ROUTE = "/api/v1/tema/library/access";
constexpr auto BOOKS_ROUTE = "/api/v1/tema/library/books";

constexpr auto CONTENT_TYPE = "application/json";

constexpr auto HOST = "34.254.242.81";
constexpr auto PORT = 8080;

constexpr auto HEADER_TERMINATOR = "\r\n\r\n";
constexpr auto CONTENT_LENGTH = "Content-Length: ";

constexpr auto BUFLEN = 4096;

#endif /* _CONSTANTS_ */