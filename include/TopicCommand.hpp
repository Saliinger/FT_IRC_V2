#pragma once

#include "./Command.hpp"

class TopicCommand: public Command {
    public:
        void execute(Server &server, Client &client, const std::vector<std::string> &args);
};