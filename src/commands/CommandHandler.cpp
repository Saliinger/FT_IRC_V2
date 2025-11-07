#include "../../include/CommandHandler.hpp"

#include <iostream>
#include <sstream>

CommandHandler::CommandHandler()
{
    _commands["NEWCOMEX"] = new NewComEx();
}

CommandHandler::~CommandHandler()
{
    for (std::map<std::string, Command*>::iterator it = _commands.begin(); it != _commands.end(); ++it)
        delete it->second;
}

void    CommandHandler::handleCommand(Server &server, Client &client, const std::string &input)
{
    std::istringstream          iss(input);
    std::string                 cmd;
    iss >> cmd;
    
    std::vector<std::string>    args;
    std::string                 arg;
    while (iss >> arg)
        args.push_back(arg);
    for (size_t i = 0; i < cmd.size(); ++i)
        cmd[i] = toupper(cmd[i]);
    if (_commands.find(cmd) != _commands.end())
        _commands[cmd]->execute(server, client, args);
    else
        client.sendMessage("Unknown command: " + cmd);
}