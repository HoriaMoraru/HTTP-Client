#include "helpers.hpp"
#include "requests.hpp"
#include "constants.hpp"

// GET request generation
std::string compute_get_request(const char *host, const char *url, std::string &cookie, std::string &token)
{
    std::stringstream ss;

    ss << "GET " << url << " HTTP/1.1\r\n";
    ss << "Host: " << host << "\r\n";

    if (!cookie.empty())
    {
        ss << "Cookie: " << cookie << "\r\n";
    }

    if (!token.empty())
    {
        ss << "Authorization: Bearer " << token << "\r\n";
    }

    ss << "Connection: close\r\n";
    ss << "\r\n";

    return ss.str();
}

// POST request generation
std::string compute_post_request(const char *host, const char *url, const char *content_type,
                                 std::string &body_data, std::string &cookie, std::string &token)
{
    std::stringstream ss;

    ss << "POST " << url << " HTTP/1.1\r\n";
    ss << "Host: " << host << "\r\n";
    ss << "Content-Type: " << content_type << "\r\n";
    ss << "Content-Length: " << body_data.length() << "\r\n";

    if (!cookie.empty())
    {
        ss << "Cookie: " << cookie << "\r\n";
    }

    if (!token.empty())
    {
        ss << "Authorization: Bearer " << token << "\r\n";
    }

    ss << "Connection: close\r\n";
    ss << "\r\n";
    ss << body_data << "\r\n";

    return ss.str();
}

// DELETE request generation
std::string compute_delete_request(const char *host, const char *url,
                                   std::string &cookie, std::string &token)
{
    std::stringstream ss;

    ss << "DELETE " << url << " HTTP/1.1\r\n";
    ss << "Host: " << host << "\r\n";

    if (!cookie.empty())
    {
        ss << "Cookie: " << cookie << "\r\n";
    }

    if (!token.empty())
    {
        ss << "Authorization: Bearer " << token << "\r\n";
    }

    ss << "Connection: close\r\n";
    ss << "\r\n";

    return ss.str();
}
