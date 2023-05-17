#include <string>
#include <stdexcept>

enum class client_command
{
    REGISTER,
    LOGIN,
    ENTER_LIBRARY,
    GET_BOOKS,
    GET_BOOK,
    ADD_BOOK,
    DELETE_BOOK,
    LOGOUT,
    EXIT,
    INVALID
};

client_command parse_command(const std::string &user_cmd)
{
    if (user_cmd == "register")
    {
        return client_command::REGISTER;
    }
    else if (user_cmd == "login")
    {
        return client_command::LOGIN;
    }
    else if (user_cmd == "enter_library")
    {
        return client_command::ENTER_LIBRARY;
    }
    else if (user_cmd == "get_books")
    {
        return client_command::GET_BOOKS;
    }
    else if (user_cmd == "get_book")
    {
        return client_command::GET_BOOK;
    }
    else if (user_cmd == "add_book")
    {
        return client_command::ADD_BOOK;
    }
    else if (user_cmd == "delete_book")
    {
        return client_command::DELETE_BOOK;
    }
    else if (user_cmd == "logout")
    {
        return client_command::LOGOUT;
    }
    else if (user_cmd == "exit")
    {
        return client_command::EXIT;
    }
    else
    {
        return client_command::INVALID;
    }
}