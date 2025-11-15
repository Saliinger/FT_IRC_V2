#include "../../include/TopicCommand.hpp"

void TopicCommand::execute(Server &server, Client &client, const std::vector<std::string> &args)
{
	// check if topic is op only
	// check if the client is op in that case

	Channel *channel = server.getChannel(args[0]);
	if (channel->getChannelMode(MODE_I))
	{
		if (!channel->isOperator(&client))
		{
			std::cout << "Error: op only topic" << std::endl;
			return;
			// error need to be sent to the clien
		}
	}
	channel->setTopic(args[1]);
	// need to send the updated topic
}

// 1. Check registration 
// 2. Check parameters
// 3. Check channel exists
// 4. Check user is on channel
// 5. If only channel name provided - VIEW topic
// 6. Setting topic - check if +t mode is set
// 7. Set new topic and broadcast
