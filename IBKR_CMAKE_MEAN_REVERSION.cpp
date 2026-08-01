// IBKR_CMAKE_MEAN_REVERSION.cpp : Defines the entry point for the application.
//

#include "IBKR_CMAKE_MEAN_REVERSION.h"


#include "EWrapper.h"
#include "EClientSocket.h"
#include "EReaderOSSignal.h"
#include "EReaderSignal.h"
#include "EReader.h"

#include "CWMR_Client.h"


#include <print>
#include <thread>

using namespace std;

const unsigned MAX_ATTEMPTS = 50;
const unsigned SLEEP_TIME = 10; // milliseconds

int main()
{
	int clientId = 0;
	unsigned attempt = 0;
	std::println("Starting IBKR C++ Mean Reversion Application {}...", attempt);
	for (;;)
	{
		++attempt;

		CWMR_Client client;

		client.connect("127.0.0.1", 7497, clientId);

		while (client.isConnected())
		{
			client.processMessages();
		}

		if (attempt >= MAX_ATTEMPTS)
		{
			break;
		}

		std::println("Sleeping {} seconds before next attempt", SLEEP_TIME);
		std::this_thread::sleep_for(std::chrono::seconds(SLEEP_TIME));
	}

	std::println("Exiting IBKR C++ Mean Reversion Application after {} attempts", attempt);
	
	return 0;
}
