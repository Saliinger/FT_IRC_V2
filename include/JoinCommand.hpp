#pragma once

#include "./Command.hpp"

class JoinCommand: public Command {
    public:
        void execute(Server &server, Client &client, const std::vector<std::string> &args);
};
