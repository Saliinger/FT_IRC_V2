#pragma once

#include "./Command.hpp"

class UserCommand: public Command {
    public:
        void execute(Server &server, Client &client, const std::vector<std::string> &args);
};
