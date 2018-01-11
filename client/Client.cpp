#include <iostream>
#include <string>
#include <slikenet/peerinterface.h>
#include <slikenet/messageidentifiers.h>

using namespace std;
using namespace SLNet;

const int PORT = 32123;

RakPeerInterface* peer;

void RunClient()
{
	//if(!peer)
		peer = RakPeerInterface::GetInstance();

	SocketDescriptor sd;
	sd.socketFamily = AF_INET;
	StartupResult r = peer->Startup(1, &sd, 1);
	assert(r == RAKNET_STARTED);

	peer->SetMaximumIncomingConnections(1);
	peer->SetTimeoutTime(60 * 1000, UNASSIGNED_SYSTEM_ADDRESS);

	cout << "SLikeNet initialized.\n";

	cout << "Password: ";
	string pswd;
	cin >> pswd;
	auto result = peer->Connect("localhost", PORT, pswd.c_str(), pswd.length());
	assert(result == CONNECTION_ATTEMPT_STARTED);
		
	while(true)
	{
		for(auto packet = peer->Receive(); packet; peer->DeallocatePacket(packet), packet = peer->Receive())
		{
			switch(packet->data[0])
			{
			case ID_CONNECTION_REQUEST_ACCEPTED:
				cout << "Connected successfully, disconnecting.\n";
				peer->DeallocatePacket(packet);
				peer->Shutdown(0);
				return;
			case ID_INVALID_PASSWORD:
				cout << "Invalid password, disconnecting.\n";
				peer->DeallocatePacket(packet);
				peer->Shutdown(0);
				return;
			case ID_ALREADY_CONNECTED:
				cout << "Already connected, disconnecting.\n";
				peer->DeallocatePacket(packet);
				peer->Shutdown(0);
				return;
			default:
				cout << "Other message '" << (int)packet->data[0] << "'\n";
				break;
			}
		}

		Sleep(100);
	}

	RakPeerInterface::DestroyInstance(peer);
}

int main()
{
	cout << "CLIENT\n";
	

	while(true)
	{
		RunClient();
	}

	return 0;
}
