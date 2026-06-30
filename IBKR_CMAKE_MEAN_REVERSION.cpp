// IBKR_CMAKE_MEAN_REVERSION.cpp : Defines the entry point for the application.
//

#include "IBKR_CMAKE_MEAN_REVERSION.h"
#include "EWrapper.h"
#include "EClientSocket.h"
#include "EReaderOSSignal.h"
#include "EReaderSignal.h"
#include "EReader.h"
#include <print>

using namespace std;

// Used to hanndle messages and events from the Interactive Brokers API
class Client : public EWrapper
{
public:
	Client() :
		m_osSignal(2000),
		m_pClient(new EClientSocket(this, &m_osSignal)),
		m_extraAuth(false)
	{
	}

	virtual ~Client() override = default;

	void setConnectOptions(const std::string& connectOptions) 
	{
		m_pClient->setConnectOptions(connectOptions);
	}

	bool connect(const std::string& host, int port, int clientId = 0)
	{
		std::string targetHost = host.empty() ? "127.0.0.1" : host;

		std::println("Connecting to {}:{} clientId {}", targetHost, port, clientId);

		bool bRes = m_pClient->eConnect(targetHost.c_str(), port, clientId, m_extraAuth);

		if (bRes)
		{
			std::println("Connnected to {}:{} clientId:{} serverVersion: {}", m_pClient->host(), m_pClient->port(), clientId, m_pClient->EClient::serverVersion());

			m_pReader = std::make_unique<EReader>(m_pClient, &m_osSignal);
			m_pReader->start();
		}
		else
		{
			std::println(std::cerr,"Cannot connect to {}:{} clientId{}", m_pClient->host(), m_pClient->port(), clientId);
		}


		return bRes;
	}

	//virtual void error(int id, time_t errorTime, int errorCode, const std::string& errorString, const std::string& advancedOrderRejectJson) override
	//{
	//	std::cerr << "Error. Id: " << id << ", Code: " << errorCode << ", Msg: " << errorString << std::endl;
	//}

	//virtual void connectAck() override
	//{
	//	std::cout << "Connected to TWS." << std::endl;
	//}

	// --- Virtual Overrides for EWrapper interface methods ---
	#include "EWrapper_prototypes.h"
private: 
	EReaderOSSignal m_osSignal;
	EClientSocket* const m_pClient;
	std::unique_ptr<EReader> m_pReader;
	bool m_extraAuth;
};

int main()
{
	cout << "Hello CMake." << endl;
	return 0;
}
