#include "json.hpp"
#include "requests.hpp"
#include "constants.hpp"
#include "helpers.hpp"
#include "client_command.hpp"

using namespace std;
using json = nlohmann::json;

int main(void)
{
  /* Open a TCP connection to the server */
  int sockfd;

  string login_cookie;
  string token;

  string empty_cookie;
  string empty_token;

  while (true)
  {
    string user_input;
    getline(cin, user_input);

    user_input.erase(remove_if(user_input.begin(), user_input.end(), ::isspace), user_input.end());

    client_command cmd = parse_command(user_input);

    switch (cmd)
    {
      case client_command::REGISTER:
      {
        sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);

        array<std::string, 2> info = user_info("register");

        if (info[0].empty() || info[1].empty())
        {
          break;
        }

        json j;
        j["username"] = info[0];
        j["password"] = info[1];

        string bd = j.dump();

        string message = compute_post_request(HOST, REGISTER_ROUTE, CONTENT_TYPE, bd, empty_cookie, empty_token);
        send_to_server(sockfd, message);

        string response = receive_from_server(sockfd);
        string error = extract_server_error(response);

        if (error.empty())
        {
          cout << "You registered successfully." << endl;
        }
        else
        {
          cout << error << endl;
        }

        close_connection(sockfd);

        break;
      }
      case client_command::LOGIN:
      {
        sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);

        std::array<std::string, 2> info = user_info("login");

        json j;
        j["username"] = info[0];
        j["password"] = info[1];

        string bd = j.dump();

        string message = compute_post_request(HOST, LOGIN_ROUTE, CONTENT_TYPE, bd, empty_cookie, empty_token);
        send_to_server(sockfd, message);

        string response = receive_from_server(sockfd);

        login_cookie = extract_cookie(response);
        string error = extract_server_error(response);

        if (error.empty())
        {
          cout << "You logged in successfully." << endl;
        }
        else
        {
          cout << error << endl;
        }

        close_connection(sockfd);

        break;
      }
      case client_command::ENTER_LIBRARY:
      {
        sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);

        if (!check_login(login_cookie))
        {
          cout << "You must be logged in to enter the library." << endl;
          break;
        }

        string message = compute_get_request(HOST, ACCESS_ROUTE, login_cookie, empty_token);
        send_to_server(sockfd, message);

        string response = receive_from_server(sockfd);
        token = extract_token(response);

        cout << "You can now access the library." << endl;

        close_connection(sockfd);

        break;
      }
      case client_command::GET_BOOKS:
      {
        sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);

        if (!check_login(login_cookie))
        {
          cout << "You must be logged in to get the books." << endl;
          break;
        }

        if (!check_token(token))
        {
          cout << "You must have access to the library to get books." << endl;
          break;
        }

        string message = compute_get_request(HOST, BOOKS_ROUTE, login_cookie, token);
        send_to_server(sockfd, message);

        string response = receive_from_server(sockfd);
        get_books(response);

        close_connection(sockfd);

        break;
      }
      case client_command::GET_BOOK:
      {
        sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);

        if (!check_login(login_cookie))
        {
          cout << "You must be logged in to get a book." << endl;
          break;
        }

        if (!check_token(token))
        {
          cout << "You must have access to the library to get a book." << endl;
          break;
        }

        string id = retrieve_book_id();

        string url = string(BOOKS_ROUTE) + "/" + id;

        string message = compute_get_request(HOST, url.c_str(), login_cookie, token);
        send_to_server(sockfd, message);

        string response = receive_from_server(sockfd);
        string error = extract_server_error(response);
        string book = get_book(response);

        if (error.empty())
        {
          cout << book << endl;
        }
        else
        {
          cout << error << endl;
        }

        close_connection(sockfd);

        break;
      }

      case client_command::ADD_BOOK:
      {
        sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);

        if (!check_login(login_cookie))
        {
          cout << "You must be logged in to add a book." << endl;
          break;
        }

        if (!check_token(token))
        {
          cout << "You must have access to the library to add books." << endl;
          break;
        }

        string bd = create_book();

        string message = compute_post_request(HOST, BOOKS_ROUTE, CONTENT_TYPE, bd, login_cookie, token);
        send_to_server(sockfd, message);

        string response = receive_from_server(sockfd);
        string error = extract_server_error(response);

        if (error.empty())
        {
          cout << "Book added successfully." << endl;
        }
        else
        {
          cout << error << endl;
        }

        close_connection(sockfd);

        break;
      }
      case client_command::DELETE_BOOK:
      {
        sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);

        if (!check_login(login_cookie))
        {
          cout << "You must be logged in to delete a book." << endl;
          break;
        }

        if (!check_token(token))
        {
          cout << "You must have access to the library to delete books." << endl;
          break;
        }

        string id = retrieve_book_id();

        string url = string(BOOKS_ROUTE) + "/" + id;

        string message = compute_delete_request(HOST, url.c_str(), login_cookie, token);
        send_to_server(sockfd, message);

        string response = receive_from_server(sockfd);
        string error = extract_server_error(response);

        if (error.empty())
        {
          cout << "Book deleted successfully." << endl;
        }
        else
        {
          cout << error << endl;
        }

        close_connection(sockfd);

        break;
      }
      case client_command::LOGOUT:
      {
        sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);

        if (!check_login(login_cookie))
        {
          cout << "You must be logged in to logout." << endl;
          break;
        }

        string message = compute_get_request(HOST, LOGOUT_ROUTE, login_cookie, empty_token);
        send_to_server(sockfd, message);

        string response = receive_from_server(sockfd);
        string error = extract_server_error(response);

        if (error.empty())
        {
          cout << "You logged out successfully." << endl;
        }
        else
        {
          cout << error << endl;
        }

        token.clear();
        login_cookie.clear();

        close_connection(sockfd);

        break;
      }
      case client_command::EXIT:
      {
        close_connection(sockfd);

        cout << "Exiting..." << endl;
        return 0;
      }
      case client_command::INVALID:
      {
        cout << "Invalid command." << endl;
        break;
      }
    }
  }

  return 0;
}