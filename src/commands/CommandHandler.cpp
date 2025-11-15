#include "../../include/CommandHandler.hpp"
#include "../../include/Client.hpp"
#include "../../include/Server.hpp"
#include "../../include/NickCommand.hpp"
#include "../../include/PassCommand.hpp"
#include "../../include/UserCommand.hpp"
#include "../../include/JoinCommand.hpp"
#include "../../include/PingCommand.hpp"
#include "../../include/PongCommand.hpp"
#include "../../include/KickCommand.hpp"
#include "../../include/InviteCommand.hpp"
#include "../../include/PrivmsgCommand.hpp"
#include "../../include/QuitCommand.hpp"
#include "../../include/PartCommand.hpp"

#include <iostream>
#include <sstream>

CommandHandler::CommandHandler()
{
    _commands["NICK"] = new NickCommand();
    _commands["PASS"] = new PassCommand();
    _commands["USER"] = new UserCommand();
    _commands["JOIN"] = new JoinCommand();
    _commands["PING"] = new PingCommand();
    _commands["PONG"] = new PongCommand();
    _commands["KICK"] = new KickCommand();
    _commands["INVITE"] = new InviteCommand();
	_commands["PRIVMSG"] = new PrivmsgCommand();
	_commands["QUIT"] = new QuitCommand();
	_commands["PART"] = new PartCommand();
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