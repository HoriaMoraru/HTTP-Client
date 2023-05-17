#ifndef _REQUESTS_
#define _REQUESTS_

#include <string>

// computes and returns a GET request string (query_params
// and cookies can be set to NULL if not needed)
std::string compute_get_request(const char *host, const char *url,
                                std::string &cookie, std::string &token);

// computes and returns a POST request string (cookies can be NULL if not needed)
std::string compute_post_request(const char *host, const char *url, const char *content_type,
                                 std::string &body_data, std::string &cookie, std::string &token);

// computes and returns a DELETE request string 
std::string compute_delete_request(const char *host, const char *url,
                                   std::string &cookie, std::string &token);
                                   
#endif /* _REQUESTS_ */
