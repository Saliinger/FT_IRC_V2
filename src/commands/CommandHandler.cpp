#include "../../include/CommandHandler.hpp"
#include "../../include/Client.hpp"
#include "../../include/Server.hpp"
#include "../../include/NickCommand.hpp"
#include "../../include/PassCommand.hpp"

#include <iostream>
#include <sstream>

CommandHandler::CommandHandler()
{
    _commands["NICK"] = new NickCommand();
    _commands["PASS"] = new PassCommand();
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
    std::vector<std::string>    args;
    std::string                 arg;
    
    iss >> cmd;
    while (iss >> arg)
        args.push_back(arg);
    for (size_t i = 0; i < cmd.size(); ++i)
        cmd[i] = toupper(cmd[i]);
    if (_commands.find(cmd) != _commands.end())
        _commands[cmd]->execute(server, client, args);
    else
        client.sendMessage("Unknown command: " + cmd);
}