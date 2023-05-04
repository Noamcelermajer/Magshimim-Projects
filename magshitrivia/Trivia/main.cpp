#include "Server.h"
#include <iostream>
#include <WS2tcpip.h>
#pragma comment (lib, "ws2_32.lib")

int main()
{
	Server server = Server();

	server.run();

	system("pause");
	return 0;
}