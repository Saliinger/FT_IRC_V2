#include "../../include/PassCommand.hpp"
#include "../../include/Server.hpp"
#include "../../include/Client.hpp"

void PassCommand::execute(Server &server, Client &client, const std::vector<std::string> &args) {
    if (args.empty()) {
        client.sendMessage("461 ERR_NEEDMOREPARAMS PASS :Not enough parameters");
        return;
    }

    if (client.isRegistered()) {
        client.sendMessage("462 ERR_ALREADYREGISTERED :You may not reregister");
        return;
    }

    std::string password = args[0];

    if (server.getPassword() != password) {
        client.sendMessage("464 ERR_PASSWDMISMATCH :Password incorrect");
        client.unAuthenticate();
        return;
    }

    client.authenticate();
}