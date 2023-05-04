#include "Server.h"

Server::Server() : m_communicator(Communicator())
{

}

void Server::run()
{
	m_communicator.bindAndListen();
}