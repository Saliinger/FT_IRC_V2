#pragma once

#include "Modes.hpp"

#include <string>
#include <map>
#include <list>

class Client;

class Channel
{
	private:
		std::string						_channelName;
		std::string						_topic;
		std::string						_password;
		std::map<int, Client *>			_clientList;
		std::map<int, Client *>			_operatorList;
		std::list<Client *>				_inviteList;
		Modes							_modes;
		size_t							_clientLimit;


	public:
		Channel(std::string channelName);
		~Channel();

		// Getters & Setters
		std::string						getChannelName() const;
		const std::map<int, Client *>	&getClients() const;
		const std::map<int, Client *>	&getOperators() const;
		const std::string				&getTopic() const;
		const std::string				&getPassword() const;
		bool							getChannelMode(ChannelModes mode) const;
		size_t							getClientLimit() const;

		void							setTopic(const std::string &topic);
		void							setPassword(const std::string &pass);
		void							setChannelMode(ChannelModes mode, bool b);
		void							setClientLimit(size_t limit);

		// Methodes
		bool							addClient(Client *client);
		void							removeClient(Client *client);
		bool							setOperator(Client *client);
		bool							removeOperator(Client *client);
		bool 							isOperator(Client *client) const;
		bool							isClient(Client *client) const;
		void							sendMessageToClients(int fd, const std::string &message);
		void							addToInviteList(Client *client);
		void							removeFromInviteList(Client *client);
		void							clearList();
		bool							isInvited(Client *client) const;
};
