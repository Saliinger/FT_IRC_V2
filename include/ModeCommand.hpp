#pragma once

#include <iostream>
#include <cstdlib>

#include "Command.hpp"

class ModeCommand : public Command
{
public:
	void execute(Server &server, Client &client, const std::vector<std::string> &args);
};
