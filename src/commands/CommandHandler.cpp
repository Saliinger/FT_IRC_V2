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
    std::istringstream iss(input);
    std::string cmd;
    std::vector<std::string> args;
    std::string token;

    iss >> cmd;
    bool colonFound = false;
    std::string lastArg;

    while (iss >> token)
    {
        if (!colonFound && token[0] == ':')
        {
            colonFound = true;
            lastArg = token.substr(1);
        }
        else if (colonFound)
            lastArg += " " + token;
        else
            args.push_back(token);
    }
    if (colonFound)
        args.push_back(lastArg);
    for (size_t i = 0; i < cmd.size(); ++i)
        cmd[i] = toupper(cmd[i]);
    if (_commands.find(cmd) != _commands.end())
        _commands[cmd]->execute(server, client, args);
    else
        client.sendMessage("Unknown command: " + cmd);
}