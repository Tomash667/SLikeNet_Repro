#include <iostream>
#include <string>
#include <slikenet/peerinterface.h>
#include <slikenet/messageidentifiers.h>

using namespace std;
using namespace SLNet;

const int PORT = 32123;
const string PSWD = "123";
const int MAX_PLAYERS = 16;

int main()
{
	cout << "SERVER\n";
	auto peer = RakPeerInterface::GetInstance();

	SocketDescriptor sd(PORT, 0);
	sd.socketFamily = AF_INET;
	StartupResult r = peer->Startup(MAX_PLAYERS, &sd, 1);
	assert(r == RAKNET_STARTED);

	peer->SetIncomingPassword(PSWD.c_str(), PSWD.length());

	peer->SetMaximumIncomingConnections(MAX_PLAYERS);
	peer->SetTimeoutTime(60 * 1000, UNASSIGNED_SYSTEM_ADDRESS);
	peer->SetOfflinePingResponse("TEST", 4);
	cout << "SLikeNet initialized.\n";

	//==========================================================================================

	while(true)
	{
		for(auto packet = peer->Receive(); packet; peer->DeallocatePacket(packet), packet = peer->Receive())
		{
			switch(packet->data[0])
			{
			case ID_UNCONNECTED_PING_OPEN_CONNECTIONS:
				cout << "Ping\n";
				break;
			case ID_NEW_INCOMING_CONNECTION:
				cout << "Client connected\n";
				break;
			case ID_DISCONNECTION_NOTIFICATION:
				cout << "Client disconnected\n";
				break;
			default:
				cout << "Other message '" << (int)packet->data[0] << "'\n";
				break;
			}
		}

		Sleep(100);
	}



	return 0;
}
