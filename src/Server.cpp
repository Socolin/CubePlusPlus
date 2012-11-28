//============================================================================
// Name        : Server.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C, Ansi-style
//============================================================================

#include "Network/NetworkManager.h"
#include "Network/Opcode.h"
#include <iostream>

int main(void)

{
	Network::initOpcode();
	Network::NetworkManager manager;
	if (!manager.StartServer(25565))
	{
		std::cerr << "Not started" << std::endl;
		return 1;
	}
	while (1)
	{
		manager.ReceiveData();
	}
	return 0;
}

