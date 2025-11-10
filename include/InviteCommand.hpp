#pragma once

#include "./Command.hpp"

class InviteCommand: public Command {
    public:
        void execute(Server &server, Client &client, const std::vector<std::string> &args);
};
