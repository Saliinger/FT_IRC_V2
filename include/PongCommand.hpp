#pragma once

#include "./Command.hpp"

class PongCommand: public Command {
    public:
        void execute(Server &server, Client &client, const std::vector<std::string> &args);
};
