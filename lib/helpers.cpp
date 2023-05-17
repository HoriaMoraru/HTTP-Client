#include "helpers.hpp"
#include "constants.hpp"
#include "json.hpp"

void error(const char *msg)
{
  perror(msg);
  exit(0);
}

int open_connection(const char *host_ip, const int portno, int ip_type, int socket_type, int flag)
{
  struct sockaddr_in serv_addr;
  int sockfd = socket(ip_type, socket_type, flag);
  if (sockfd < 0)
    error("ERROR opening socket");

  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = ip_type;
  serv_addr.sin_port = htons(portno);
  inet_aton(host_ip, &serv_addr.sin_addr);

  /* Connect the socket */
  if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    error("ERROR connecting");

  return sockfd;
}

void close_connection(int sockfd)
{
  close(sockfd);
}

void send_to_server(int sockfd, std::string &message)
{
  int bytes, sent = 0;
  int total = message.size();

  do
  {
    bytes = send(sockfd, message.data() + sent, total - sent, 0);
    if (bytes < 0)
    {
      error("ERROR writing message to socket");
    }

    if (bytes == 0)
    {
      break;
    }

    sent += bytes;
  } while (sent < total);
}

std::string receive_from_server(int sockfd)
{
  std::string response;
  int received = 0;
  size_t header_end = 0;
  size_t content_length = 0;

  do
  {
    char buf[BUFLEN];
    received = recv(sockfd, buf, BUFLEN, 0);
    if (received < 0)
    {
      error("ERROR reading response from socket");
    }

    if (received == 0)
    {
      break;
    }

    response.append(buf, received);

    header_end = response.find(HEADER_TERMINATOR);
    if (header_end != std::string::npos)
    {
      header_end += strlen(HEADER_TERMINATOR);

      size_t content_length_start = response.find(CONTENT_LENGTH);
      if (content_length_start != std::string::npos)
      {
        content_length_start += strlen(CONTENT_LENGTH);
        content_length = std::stoi(response.substr(content_length_start, header_end - content_length_start));
        break;
      }
    }
  } while (true);

  size_t total = header_end + content_length;

  response.reserve(total);

  while (response.length() < total)
  {
    char buf[BUFLEN];
    received = recv(sockfd, buf, BUFLEN, 0);

    if (received < 0)
    {
      error("ERROR reading response from socket");
    }

    if (received == 0)
    {
      break;
    }

    response.append(buf, received);
  }
  return response;
}

std::string extract_cookie(std::string &response)
{
  std::string login_cookie;
  size_t cookie_start = response.find("Set-Cookie: ");
  if (cookie_start != std::string::npos)
  {
    cookie_start += strlen("Set-Cookie: ");
    size_t cookie_end = response.find(";", cookie_start);
    login_cookie = response.substr(cookie_start, cookie_end - cookie_start);
  }

  return login_cookie;
}

std::string extract_token(std::string &response)
{
  std::string token;
  size_t token_start = response.find("{");
  size_t token_end = response.find("}");

  token = response.substr(token_start, token_end - token_start + 1);

  nlohmann::json j = nlohmann::json::parse(token);
  token = j["token"];

  return token;
}

bool isNumber(std::string &s)
{
  return !s.empty() && std::find_if(s.begin(),
                                    s.end(), [](char c)
                                    { return !std::isdigit(c); }) == s.end();
}

bool isInvalidCredentials(const std::string &username, const std::string &password)
{
  return (username.find(' ') != std::string::npos || password.find(' ') != std::string::npos || username.empty() || password.empty());
}

std::array<std::string, 2> user_info(const std::string &type)
{
  std::array<std::string, 2> info;
  std::string username, password;

  if (type == "register")
  {
    std::cout << "Enter your username. NOTE: It must not contain empty spaces: ";
    std::getline(std::cin, username);

    std::cout << "Enter your password. NOTE: It must not contain empty spaces: ";
    std::getline(std::cin, password);

    if (isInvalidCredentials(username, password))
    {
      std::cout << "Credentials are not correct. NOTE: Credentials MUST not contain empty spaces." << std::endl;
      return {};
    }
  }
  else if (type == "login")
  {
    std::cout << "Enter your username: ";
    std::getline(std::cin, username);

    std::cout << "Enter your password: ";
    std::getline(std::cin, password);
  }

  info[0] = username;
  info[1] = password;

  return info;
}

std::string create_book()
{
  nlohmann::json j;
  std::string title, author, genre, page_count, publisher;
  std::string book;

  std::cout << "Enter the title of the book: ";
  getline(std::cin, title);

  std::cout << "Enter the author of the book: ";
  getline(std::cin, author);

  std::cout << "Enter the genre of the book: ";
  getline(std::cin, genre);

  while (!isNumber(page_count))
  {
    std::cout << "Enter the number of pages. NOTE: This MUST be a number: ";
    getline(std::cin, page_count);
    page_count.erase(std::remove_if(page_count.begin(), page_count.end(), ::isspace), page_count.end());
  }

  std::cout << "Enter the publisher of the book: ";
  getline(std::cin, publisher);

  j["title"] = title;
  j["author"] = author;
  j["genre"] = genre;
  j["page_count"] = page_count;
  j["publisher"] = publisher;

  book = j.dump();

  return book;
}

bool check_login(std::string &login_cookie)
{
  if (login_cookie.empty())
  {
    return false;
  }

  return true;
}

bool check_token(std::string &token)
{
  if (token.empty())
  {
    return false;
  }

  return true;
}

std::string extract_server_error(const std::string &response)
{
  std::size_t startPos = response.find('{');
  if (startPos == std::string::npos)
  {
    return "";
  }

  std::string jsonString = response.substr(startPos);

  try
  {
    nlohmann::json jsonData = nlohmann::json::parse(jsonString);

    std::string errorMessage = jsonData["error"];

    return errorMessage;
  }
  catch (const std::exception &e)
  {
    return "";
  }
}

std::string get_book(std::string &response)
{
  std::size_t start_pos = response.find('{');
  std::size_t end_pos = response.rfind('}');

  std::string json_str = response.substr(start_pos, end_pos - start_pos + 1);

  nlohmann::json j = nlohmann::json::parse(json_str);

  std::stringstream ss;

  ss << "Title: " << j["title"] << std::endl;
  ss << "Author: " << j["author"] << std::endl;
  ss << "Publisher: " << j["publisher"] << std::endl;
  ss << "Genre: " << j["genre"] << std::endl;
  ss << "Page count: " << j["page_count"];

  return ss.str();
}

void get_books(std::string &response)
{
  std::size_t start_pos = response.find('[');
  std::size_t end_pos = response.rfind(']');

  std::string json_str = response.substr(start_pos, end_pos - start_pos + 1);

  nlohmann::json j = nlohmann::json::parse(json_str);

  std::vector<nlohmann::json> books = j;

  if (books.size() == 0)
  {
    std::cout << "There are no books in the library." << std::endl;
    return;
  }

  for (const auto &book : books)
  {
    std::cout << "ID: " << book["id"] << ", Title: " << book["title"] << std::endl;
  }
}

std::string retrieve_book_id()
{
  std::string id;

  while (true)
  {
    std::cout << "Type the ID of the book. NOTE: This MUST be a number: ";
    std::getline(std::cin, id);

    // Remove whitespace characters from the ID
    id.erase(std::remove_if(id.begin(), id.end(), ::isspace), id.end());

    // Check if the ID is a valid number
    if (isNumber(id))
    {
      break;
    }
    else
    {
      std::cout << "Invalid ID. Please try again." << std::endl;
    }
  }

  return id;
}