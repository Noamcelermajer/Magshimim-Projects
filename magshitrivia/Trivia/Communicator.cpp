#include "Communicator.h"
#include <time.h>
#include <chrono>

#define BUFSIZE_DEFAULT 2048

void Communicator::endConnection(const std::string& info, const SOCKET& user)
{
	if (info != "|not_logged|,not_logged" && m_clients[user] != nullptr)
	{
		std::string username = "";
		std::string state = "";
		bool passedComma = false;
		for (unsigned int i = 0; i < info.length(); i++)
		{
			if (info[i] != ',' && !passedComma)
				username += info[i];

			else if (info[i] != ',' && passedComma)
				state += info[i];

			else if (info[i] == ',')
				passedComma = true;
		}

		Request req;
		if (state == "menu")
		{

			req = { 0, 0, "{\"type\": \"signout\", \"username\" : " + username + "}" };
			(*m_clients[user]).handleRequest(req);
		}

		else if (state == "in_room_admin")
		{

			req = { 0, 0, "{\"type\": \"close_room\"}" };
			(*m_clients[user]).handleRequest(req);
			delete m_clients[user];
			m_clients[user] = m_handlerFactory.createMenuRequestHandler(LoggedUser(username));
			req = { 0, 0, "{\"type\": \"signout\", \"username\" : " + username + "}" };
			(*m_clients[user]).handleRequest(req);
		}

		else if (state == "in_room_user")
		{
			req = { 0, 0, "{\"type\": \"leave_room\"}" };
			(*m_clients[user]).handleRequest(req);
			delete m_clients[user];
			m_clients[user] = m_handlerFactory.createMenuRequestHandler(LoggedUser(username));
			req = { 0, 0, "{\"type\": \"signout\", \"username\" : " + username + "}" };
			(*m_clients[user]).handleRequest(req);
		}

		else if (state == "in_game")
		{
			req = { 0, 0, "{\"type\": \"back_to_menu\"}" };
			(*m_clients[user]).handleRequest(req);
			delete m_clients[user];
			m_clients[user] = m_handlerFactory.createMenuRequestHandler(LoggedUser(username));
			req = { 0, 0, "{\"type\": \"signout\", \"username\" : " + username + "}" };
			(*m_clients[user]).handleRequest(req);
		}
	}
}

void Communicator::startThreadForNewClient(const SOCKET& user)
{
	auto j = m_clients.begin()++;
	size_t threadNum = m_clients.size();

	std::cout << "Thread #" + std::to_string(threadNum) + ": " + "New connection!" << std::endl;
	
	std::string info = requestSequencer(user, threadNum);
	
	std::cout << "Thread #" + std::to_string(threadNum) + ": " + "Connection stopped." << std::endl;

	endConnection(info, user);
	if(m_clients[user] != nullptr)
		delete m_clients[user];

	m_clients[user] = nullptr;
	closesocket(user);
}

void cleanseBuffers(char* buf1, char* buf2, unsigned int length)
{
	for (unsigned int i = 0; i < length; i++) //Cleanses the buffer to get rid of the last request.
	{
		buf1[i] = 0;
		buf2[0] = 0;
	}
}

void Communicator::handleInGameCrash(const SOCKET& user, const std::string& username, Room** room)
{
	(**room).removeUser(LoggedUser(username));
	Request req = { 0, 0, "{\"type\": \"back_to_menu\"}" };
	(*m_clients[user]).handleRequest(req);
	delete m_clients[user];
	m_clients[user] = m_handlerFactory.createMenuRequestHandler(LoggedUser(username));
	if (room != nullptr && *room != nullptr) m_handlerFactory.deleteRoomAfterGameEnd(room);
	req = { 0, 0, "{\"type\": \"signout\", \"username\" : " + username + "}" };
	(*m_clients[user]).handleRequest(req);
	*room = nullptr;
}

std::string Communicator::requestSequencer(const SOCKET& user, const size_t threadNum) //All request handling happens here!
{
	Request req = { };
	RequestResult resp = { "", nullptr, "" };
	RequestResult* respP = 0;
	Room* room = nullptr;
	unsigned int id = 1;
	char buf[BUFSIZE_DEFAULT] = { 0 };
	std::string bufStr = "";
	char curTime[BUFSIZE_DEFAULT] = { 0 };
	int timeout = 0;
	std::string username = "|not_logged|";
	std::string state = "not_logged";
	time_t rawtime;
	tm newtime;

	while (recv(user, buf, BUFSIZE_DEFAULT - 1, NULL) != 0 && timeout < BUFSIZE_DEFAULT)
	{
		bufStr = buf;
		if (bufStr.size() > 0)
		{
			time(&rawtime);
			req = { id, rawtime, bufStr };
			respP = (*m_clients[user]).handleRequest(req);
			resp = *respP;
			
			localtime_s(&newtime, &req.recivalTime);
			strftime(curTime, BUFSIZE_DEFAULT, "%H:%M:%S", &newtime);
			std::cout << "{Thread #" << threadNum << ", " << username << "} " << curTime << ": " << resp.response << " (" << req.id << ")" << std::endl;
			
			handleResponse(respP, user, room);
			switchStates(resp, req, user, &username, &state, &room);

			cleanseBuffers(buf, curTime, BUFSIZE_DEFAULT);
			timeout = 0;
			id++;
			delete respP;
		}

		else
			timeout++;
	}

	if (state == "in_game")
	{
		handleInGameCrash(user, username, &room);
	}

	return username + "," + state;
}

void Communicator::switchStates(const RequestResult& resp, const Request& req, const SOCKET& user, std::string* username, std::string* state, Room** room)
{
	//State Switcher #1 (User not logged -> User in menu)
	if (resp.response == "login: success" || resp.response == "signup: success") 
	{
		delete m_clients[user];
		m_clients[user] = m_handlerFactory.createMenuRequestHandler(LoggedUser(resp.username));
		*username = resp.username;
		*state = "menu";
	}

	//State Switcher #2 (User in menu -> User not logged)
	else if (resp.response == "signout: success") 
	{
		delete m_clients[user];
		m_clients[user] = m_handlerFactory.createLoginRequestHandler();
		*username = "|not_logged|";
		*state = "not_logged";
	}

	//State switcher #3 (User in menu -> User in room)
	else if (resp.response == "join_room: success") 
	{
		delete m_clients[user];
		nlohmann::json data = nlohmann::json::parse(req.buffer);
		*room = &m_handlerFactory.getRoomById(data["room_id"]);
		m_clients[user] = m_handlerFactory.createRoomMemberRequestHandler(LoggedUser(resp.username), **room);
		*state = "in_room_user";
	}

	else if (resp.response == "create_room: success")
	{
		delete m_clients[user];
		nlohmann::json data = nlohmann::json::parse(req.buffer);
		*room = &m_handlerFactory.getRoomByName(data["room_name"].dump());
		m_clients[user] = m_handlerFactory.createRoomAdminRequestHandler(LoggedUser(resp.username), **room);
		*state = "in_room_admin";
	}

	//State switcher #4 (User in room -> User in menu)
	else if (resp.response == "err_room_shutdown")
	{
		delete m_clients[user];
		*room = nullptr;
		nlohmann::json data = nlohmann::json::parse(req.buffer);
		m_clients[user] = m_handlerFactory.createMenuRequestHandler(LoggedUser(resp.username));
		*state = "menu";
	}

	else if (resp.response == "leave_room: success") 
	{
		delete m_clients[user];
		*room = nullptr;
		m_clients[user] = m_handlerFactory.createMenuRequestHandler(LoggedUser(resp.username));
		*state = "menu";
	}

	else if (resp.response == "close_room: success")
	{
		delete m_clients[user];
		*room = nullptr;
		m_clients[user] = m_handlerFactory.createMenuRequestHandler(LoggedUser(resp.username));
		*state = "menu";
	}

	//State switcher #5 (User in room -> User in game)
	else if (resp.response == "start_game: success")
	{
		delete m_clients[user];
		m_handlerFactory.adminCreateRoom(**room);
		m_clients[user] = m_handlerFactory.createGameRequestHandler(LoggedUser(resp.username), **room);
		*state = "in_game";
	}

	else if (resp.response == "err_game_started")
	{
		delete m_clients[user];
		m_clients[user] = m_handlerFactory.createGameRequestHandler(LoggedUser(resp.username), **room);
		*state = "in_game";
	}

	//State switcher #6 (User in game -> User in menu)
	else if (resp.response == "back_to_menu: success")
	{
		delete m_clients[user];
		(**room).removeUser(LoggedUser(resp.username));
		m_clients[user] = m_handlerFactory.createMenuRequestHandler(LoggedUser(resp.username));
		*state = "menu";
		m_handlerFactory.deleteRoomAfterGameEnd(room);
		*room = nullptr;
	}
}

void Communicator::handleResponse(const RequestResult* resp, const SOCKET& user, const Room* room)
{
	JsonPacketSerializer serializer = JsonPacketSerializer();
	std::string finalData = serializer.serializeResponse(resp);

	send(user, finalData.c_str(), finalData.length(), NULL);
}

Communicator::Communicator() : m_clients(std::map<SOCKET, IRequestHandler*>()), m_handlerFactory(RequestHandlerFactory())
{

}

void Communicator::bindAndListen()
{

	WSADATA wsData;
	WORD ver = WINVERSION;
	int ws = WSAStartup(ver, &wsData);

	if (ws != 0)
	{
		std::cout << "Can't initialize winsock." << std::endl;
	}

	else
	{
		handleRequests();
	}
}

void Communicator::handleRequests()
{
	SOCKET listener = socket(AF_INET, SOCK_STREAM, NULL);

	if (listener == INVALID_SOCKET)
	{
		std::cout << "Error creating socket." << std::endl;
	}

	else
	{
		sockaddr_in hint;
		hint.sin_family = AF_INET;
		hint.sin_port = htons(PORT);
		hint.sin_addr.S_un.S_addr = INADDR_ANY;

		bind(listener, (sockaddr*)& hint, sizeof(hint));
		listen(listener, SOMAXCONN);

		sockaddr_in client;
		int clientSize = sizeof(client);
		SOCKET cSock;
		std::thread threader;
		
		while (true)
		{
			cSock = accept(listener, (sockaddr*)& client, &clientSize);
			m_clients[cSock] = m_handlerFactory.createLoginRequestHandler();
			threader = makeThread(cSock);
			threader.detach();
		}

		closesocket(listener);
	}
}

std::thread Communicator::makeThread(const SOCKET& user)
{
	return std::thread(&Communicator::startThreadForNewClient, this, user);
}