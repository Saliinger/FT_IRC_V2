#include "../include/Server.hpp"
#include <cstdlib>
#include <iostream>

int main(int ac, char **av)
{
	if (ac != 3)
	{
		std::cout << "Usage: ./ircserv <port> <pass>." << std::endl;
		return 1;
	}

	std::string pass(av[2]);
	int port = atoi(av[1]);

	Server runtime(pass, port);
	try
	{
		runtime.run();
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}

	return runtime.getSig();
}