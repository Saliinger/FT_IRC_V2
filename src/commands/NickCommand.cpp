#include "../../include/NickCommand.hpp"
#include "../../include/Client.hpp"
#include "../../include/Server.hpp"

#include <cctype>
#include <iostream>

static bool isValidNickname(const std::string &nick)
{
    if (nick.empty() || nick.length() > 9)
        return (false);
    if (!std::isalpha(nick[0]) && nick[0] != '[' && nick[0] != ']' && nick[0] != '{' && nick[0] != '}' &&
        nick[0] != '\\' && nick[0] != '|' && nick[0] != '_' && nick[0] != '^')
        return (false);
    for (size_t i = 1; i < nick.length(); ++i)
    {
        char c = nick[i];
        if (!std::isalnum(c) && c != '-' && c != '[' && c != ']' && c != '{' && c != '}' && c != '\\' && c != '|' &&
            c != '_' && c != '^')
            return (false);
    }
    return (true);
}

void NickCommand::execute(Server &server, Client &client, const std::vector<std::string> &args)
{
    std::string clientName = client.getNickname();
    std::string targetName = clientName.empty() ? "*" : clientName;

    if (args.empty())
    {
        client.sendMessage(formatError(ERR_NEEDMOREPARAMS, targetName, "NICK", "Not enough parameters") + "\r\n");
        return;
    }

    std::string newNick = args[0];

    if (!isValidNickname(newNick))
    {
        client.sendMessage(formatError(ERR_ERRONEUSNICKNAME, targetName, newNick, "Erroneous nickname") + "\r\n");
        return;
    }
    if (server.isNicknameUsed(newNick))
    {
        client.sendMessage(formatError(ERR_NICKNAMEINUSE, targetName, newNick, "Nickname is already in use") + "\r\n");
        return;
    }

    std::string oldNick = client.getNickname();

    if (!oldNick.empty())
        client.sendMessage(formatReply(oldNick, "NICK", newNick) + "\r\n");
    client.setNickname(newNick);

    if (!client.isRegistered() && !client.getUsername().empty())
        client.registerClient();
}
