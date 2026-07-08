#include "CWMR_Client.h"

#include "EClientSocket.h"

#include <print>

CWMR_Client::CWMR_Client() : 
	m_osSignal(2000),
	m_pClient(new EClientSocket(this, &m_osSignal)),
	m_orderId(0),
	m_extraAuth(false)
{
}

CWMR_Client::~CWMR_Client()
{
	if (m_pReader)
	{
		m_pReader.reset();
	}

	delete m_pClient;
}

void CWMR_Client::setConnectOptions(const std::string& connectOptions)
{
	m_pClient->setConnectOptions(connectOptions);
}

void CWMR_Client::processMessages()
{
	m_osSignal.waitForSignal();
	errno = 0;
	m_pReader->processMsgs();
}

bool CWMR_Client::connect(const std::string& host, int port, int clientId)
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
		std::println(std::cerr, "Cannot connect to {}:{} clientId{}", m_pClient->host(), m_pClient->port(), clientId);
	}

	return bRes;
}

bool CWMR_Client::isConnected() const
{
	return m_pClient->isConnected();
}

void CWMR_Client::tickPrice(int reqId, TickType field, double price, const TickAttrib& attribs) {}
void CWMR_Client::tickSize(int reqId, TickType field, Decimal size) {}
void CWMR_Client::tickOptionComputation(int reqId, TickType tickType, int tickAttrib, double impliedVol, double delta,
	double optPrice, double pvDividend, double gamma, double vega, double theta, double undPrice) {
}
void CWMR_Client::tickGeneric(int reqId, TickType tickType, double value) {}
void CWMR_Client::tickString(int reqId, TickType tickType, const std::string& value) {}
void CWMR_Client::tickEFP(int reqId, TickType tickType, double basisPoints, const std::string& formattedBasisPoints,
	double totalDividends, int holdDays, const std::string& futureLastTradeDate, double dividendImpact, double dividendsToLastTradeDate) {
}
void CWMR_Client::orderStatus(int orderId, const std::string& status, Decimal filled,
	Decimal remaining, double avgFillPrice, long long permId, int parentId,
	double lastFillPrice, int clientId, const std::string& whyHeld, double mktCapPrice) {
}
void CWMR_Client::openOrder(int orderId, const Contract&, const Order&, const OrderState&) {}
void CWMR_Client::openOrderEnd() {}

void CWMR_Client::winError(const std::string& str, int lastError) {}
void CWMR_Client::connectionClosed() {
	std::println("Connection closed");
}

void CWMR_Client::updateAccountValue(const std::string& key, const std::string& val,
	const std::string& currency, const std::string& accountName) {
	std::println("Account Value. Key: {}, Value: {}, Currency: {}, AccountName: {}", key, val, currency, accountName);
}

void CWMR_Client::updatePortfolio(const Contract& contract, Decimal position,
	double marketPrice, double marketValue, double averageCost,
	double unrealizedPNL, double realizedPNL, const std::string& accountName) {
	std::println("Update Portfolio. Symbol: {}, SecType: {}, Currency: {}, Position: {}, MarketPrice: {}, MarketValue: {}, AverageCost: {}, UnrealizedPNL: {}, RealizedPNL: {}, AccountName: {}",
		contract.symbol, contract.secType, contract.currency, (unsigned long long)position, marketPrice, marketValue, averageCost, unrealizedPNL, realizedPNL, accountName);
}

void CWMR_Client::updateAccountTime(const std::string& timeStamp) {
	std::println("Update Account Time. Time: {}", timeStamp);
}

void CWMR_Client::accountDownloadEnd(const std::string& accountName) {
	std::println("Account Download End. AccountName: {}", accountName);
}

void CWMR_Client::nextValidId(int orderId) {
	std::println("Next Valid Id: {}", orderId);
	m_orderId = orderId;
}

void CWMR_Client::contractDetails(int reqId, const ContractDetails& contractDetails) {
	std::println("ContractDetails begin. ReqId: {}", reqId);
	printContractMsg(contractDetails.contract);
	//TODO: print other contract details
}

void CWMR_Client::bondContractDetails(int reqId, const ContractDetails& contractDetails) {
	std::println("BondContractDetails begin. ReqId: {}", reqId);
}

void CWMR_Client::contractDetailsEnd(int reqId) {
	std::println("ContractDetailsEnd. ReqId: {}", reqId);
}

void CWMR_Client::printContractMsg(const Contract& contract) {
	std::println("\tContractId: {}", contract.conId);
	std::println("\tSymbol: {}", contract.symbol);
	std::println("\tSecType: {}", contract.secType);
	std::println("\tLastTradeDateOrContractMonth: {}", contract.lastTradeDateOrContractMonth);
	std::println("\tLastTradeDate: {}", contract.lastTradeDate);
	std::println("\tStrike: {}", contract.strike);
	std::println("\tRight: {}", contract.right);
	std::println("\tMultiplier: {}", contract.multiplier);
	std::println("\tExchange: {}", contract.exchange);
	std::println("\tPrimaryExchange: {}", contract.primaryExchange);
	std::println("\tCurrency: {}", contract.currency);
	std::println("\tLocalSymbol: {}", contract.localSymbol);
	std::println("\tTradingClass: {}", contract.tradingClass);
}

