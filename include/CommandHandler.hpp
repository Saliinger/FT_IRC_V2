#pragma once

#include <map>
#include <string>
#include <vector>
#include "./Command.hpp"

class CommandHandler {
    private:
        std::map<std::string, Command *> _commands;
    public:
        CommandHandler();
        ~CommandHandler();

        void    handleCommand(Server &server, Client &client, const std::string &input);
};