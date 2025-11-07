#pragma once

#include "./Command.hpp"

class NickCommand: public Command {
    public:
        void execute(Server &server, Client &client, const std::vector<std::string> &args);
};