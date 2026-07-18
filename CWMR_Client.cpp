#include "CWMR_Client.h"

#include "Utility.h"

#include "EClientSocket.h"
#include "Execution.h"
#include "CommissionAndFeesReport.h"

#include <print>
#include <chrono>

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
	printContractDetailsMsg(contractDetails);
	std::println("ContractDetails end. ReqId: {}", reqId);
}

void CWMR_Client::bondContractDetails(int reqId, const ContractDetails& contractDetails) {
	std::println("BondContractDetails begin. ReqId: {}", reqId);
	printBondContractDetailsMsg(contractDetails);
	std::println("BondContractDetails end. ReqId: {}", reqId);
}

void CWMR_Client::contractDetailsEnd(int reqId) {
	std::println("ContractDetailsEnd. ReqId: {}", reqId);
}

void CWMR_Client::execDetails(int reqId, const Contract& contract, const Execution& execution) {
	std::println("ExecDetails. ReqId: {}, Contnract - ConId: {}, Symbol: {}, SecType: {}, "
		"LastTradeDateOrContractMonth: {}, Strike: {}, Right: {}, Multiplier: {}, "
		"Exchange: {}, Currency: {}, LocalSymbol: {}, TradingClass: {}, "
		"Execution - OrderId: {}, ExecId: {}, Time: {}, AcctNumber: {}, "
		"Exchange: {}, Side: {}, Shares: {}, Price: {}, PermId: {}, CllientId: {}, "
		"Liquidation: {}, CumQty: {}, AvgPrice: {}, OrderRef: {}, EvRule: {}, "
		"EvMultiplier: {}, ModelCode: {}, LastLiquidity: {}, PrendingPriceRevision: {}, "
		"Submitter: {}, OptExerciseOrLapeseType: {}",
		reqId, contract.conId, contract.symbol, contract.secType,
		contract.lastTradeDateOrContractMonth, Utility::doubleMaxString(contract.strike),
		contract.right, contract.multiplier, contract.exchange, contract.currency, contract.localSymbol,
		contract.tradingClass, Utility::intMaxString(execution.orderId), execution.execId, execution.time, execution.acctNumber,
		execution.exchange, execution.side, DecimalFunctions::decimalStringToDisplay(execution.shares), Utility::doubleMaxString(execution.price), Utility::llongMaxString(execution.permId), 
		Utility::intMaxString(execution.clientId),
		execution.liquidation == 1 ? "True" : "False", DecimalFunctions::decimalStringToDisplay(execution.cumQty), Utility::doubleMaxString(execution.avgPrice), execution.orderRef, execution.evRule,
		Utility::doubleMaxString(execution.evMultiplier), execution.modelCode, Utility::intMaxString(execution.lastLiquidity), 
		execution.pendingPriceRevision ? "Yes" : "No",	execution.submitter, Utility::getOptionExerciseTypeName(execution.optExerciseOrLapseType));
}

void CWMR_Client::execDetailsEnd(int reqId){
	std::println("ExecDetailsEnds. ReqId: {}", reqId);
}

void CWMR_Client::error(int id, time_t errorTime, int errorCode, const std::string& errorString,
	const std::string& advancedOrderRejectJson) {
	
	std::string errorTimeStr;
	if (errorTime > 0) {
		auto tp = std::chrono::system_clock::time_point{
			std::chrono::milliseconds{errorTime}
		};
		errorTimeStr = std::format("{:%c}", std::chrono::floor<std::chrono::seconds>(tp));
	}

	if (!advancedOrderRejectJson.empty()) {
		std::println("Error. Id: {}, Time: {}, Code: {}, Msg: {}, AdvancedOrderRejectJson: {}",
			id, errorTimeStr, errorCode, errorString, advancedOrderRejectJson);
	}
	else {
		std::println("Error. Id: {}, Time: {}, Code: {}, Msg: {}", id, errorTimeStr, errorCode, errorString);
	}
}

void CWMR_Client::updateMktDepth(int reqId, int position, int operation, int side, 
	double price, Decimal size) {
	std::println("UpdateMarketDepth. {} - Positionn: {}, Operation: {}, Side: {}, "
		"Price: {}, Size: {}", reqId, Utility::intMaxString(position), operation, side,
		Utility::doubleMaxString(price), DecimalFunctions::decimalStringToDisplay(size));
}


void CWMR_Client::updateMktDepthL2(int reqId, int position, const std::string& marketMaker,
	int operation, int side, double price, Decimal size, bool isSmartDepth) {
	std::println("UpdateMarketDepthL2. {} - Position: {}, Operation: {}, Side: {}, "
		"Price: {}, Size: {}, isSmartDepth: {}", reqId, Utility::intMaxString(position),
		operation, side, Utility::doubleMaxString(price), DecimalFunctions::decimalStringToDisplay(size),
		isSmartDepth);
}

void CWMR_Client::updateNewsBulletin(int msgId, int msgType, const std::string& newMessage, const std::string& originExch){
	std::println("News Bulletins. {} - Type: {}, Message: {}, Exhcange of Origin: {}",
		msgId, msgType, newMessage, originExch);
}

void CWMR_Client::managedAccounts(const std::string& accountsList) {
	std::println("Account List: {}", accountsList);
}

void CWMR_Client::receiveFA(faDataType pFaDataType, const std::string& cxml) {
	std::println("Receiving Fa: {}", std::to_underlying(pFaDataType));
	std::println("{}", cxml);
}

void CWMR_Client::historicalData(int reqId, const Bar& bar) {
	std::println("HistoricalData. ReqId: {} - Date: {}, Openn: {}, High: {}, Low: {}, "
		"Close: {}, Volume: {}, Count: {}, WAP: {}", reqId, bar.time, Utility::doubleMaxString(bar.open),
		Utility::doubleMaxString(bar.high), Utility::doubleMaxString(bar.low), Utility::doubleMaxString(bar.close),
		DecimalFunctions::decimalStringToDisplay(bar.volume), Utility::intMaxString(bar.count), DecimalFunctions::decimalStringToDisplay(bar.wap));
}

void CWMR_Client::historicalDataEnd(int reqId, const std::string& startDateStr, const std::string& endDateStr) {
	std::println("HistoricalDataEnd. ReqId: {}, StartDate: {}, EndDate: {}", reqId, startDateStr, endDateStr);
}

void CWMR_Client::scannerParameters(const std::string& xml) {
	std::println("ScannerParameters. {}", xml);
}

void CWMR_Client::scannerData(int reqId, int rank, const ContractDetails& contractDetails,
	const std::string& distance, const std::string& benchmark, const std::string& projection,
	const std::string& legsStr) {
	std::println("ScannerData. ReqId: {}, Rank: {}, Symbol: {}, SecType:{}, Currency: {}, "
		"Distance: {}, Benchmark: {}, Projection: {}, Legs String: {}",
		reqId, rank, contractDetails.contract.symbol, contractDetails.contract.secType, 
		contractDetails.contract.currency, distance, benchmark, projection, legsStr);
}

void CWMR_Client::scannerDataEnd(int reqId) {
	std::println("ScannerDataEnd. ReqId: {}", reqId);
}

void CWMR_Client::realtimeBar(int reqId, long long time, double open, double high, double low,
	double close, Decimal volume, Decimal wap, int count) {
	std::println("RealTimeBar. ReqId: {} - Time: {}, Open: {}, High: {}, Low: {}, Close: {}, "
		"Volume: {}, Count: {}, WAP: {}", reqId, Utility::llongMaxString(time), Utility::doubleMaxString(open), Utility::doubleMaxString(high),
		Utility::doubleMaxString(low), Utility::doubleMaxString(close), DecimalFunctions::decimalStringToDisplay(volume),
		Utility::intMaxString(count), DecimalFunctions::decimalStringToDisplay(wap));
}

void CWMR_Client::currentTime(long long time) {
	const std::chrono::sys_seconds tp{std::chrono::seconds{time}};
	const std::chrono::zoned_time localTp{std::chrono::current_zone(), tp};
	const std::string localTimeStr = std::format("{:%Y-%m-%d %H:%M:%S}", localTp);

	std::println("Current Time. Time: {}, Local Time: {}", Utility::llongMaxString(time), localTimeStr);

	// TODO: ADD STATE
}

void CWMR_Client::deltaNeutralValidation(int reqId, const DeltaNeutralContract& deltaNeutralContract) {
	std::println("DeltaNeutralValidation. ReqId: {}, ConId: {}, Delta: {}, Price: {}",
		reqId, deltaNeutralContract.conId, Utility::doubleMaxString(deltaNeutralContract.delta),
		Utility::doubleMaxString(deltaNeutralContract.price));
}

void CWMR_Client::tickSnapshotEnd(int reqId) {
	std::println("TickSnapshotEnd. ReqId: {}", reqId);
}

void CWMR_Client::marketDataType(int reqId, int marketDataType) {
	std::println("MarketDataType. ReqId: {}, MarketDataType: {}", reqId, marketDataType);
}

void CWMR_Client::commissionAndFeesReport(const CommissionAndFeesReport& commissionAndFeesReport) {
	std::println("CommissionAndFeesReport. ExecId: {}, CommissionAndFees: {}, Currency: {}, RealizedPNL: {}",
		commissionAndFeesReport.execId, commissionAndFeesReport.commissionAndFees, commissionAndFeesReport.currency,
		commissionAndFeesReport.realizedPNL);
}

void CWMR_Client::position(const std::string& account, const Contract& contract, Decimal position, double avgCost) {
	std::println("Position. Account: {}, Symbol: {}, SecType: {}, Currency: {}, Position: {}, AvgCost: {}",
		account, contract.symbol, contract.secType, contract.currency, 
		DecimalFunctions::decimalStringToDisplay(position), 
		Utility::doubleMaxString(avgCost));
}

void CWMR_Client::positionEnd() {
	std::println("PositionEnd");
}

void CWMR_Client::accountSummary(int reqId, const std::string& account, const std::string& tag,
	const std::string& value, const std::string& currency) {
	std::println("AccountSummary. ReqId: {}, Account: {}, Tag: {}, Value: {}, Currency: {}",
		reqId, account, tag, value, currency);
}

void CWMR_Client::accountSummaryEnd(int reqId) {
	std::println("AccountSummaryEnd. ReqId: {}", reqId);
}

void CWMR_Client::verifyMessageAPI(const std::string& apiData) {
	std::println("VerifyMessageAPI. ApiData: {}", apiData);
}

void CWMR_Client::verifyCompleted(bool isSuccessful, const std::string& errorText) {
	std::println("VerifyCompleted. IsSuccessful: {} - Error: {}", isSuccessful ? "Yes" : "No", errorText);
}

void CWMR_Client::verifyAndAuthMessageAPI(const std::string& apiData, const std::string& xyzChallange) {
	std::println("VerifyAndAuthMessageAPI. ApiData: {}, XyzChallenge: {}", apiData, xyzChallange);
}

void CWMR_Client::printContractMsg(const Contract& contract) {
	std::println("\tContractId: {}", contract.conId);
	std::println("\tSymbol: {}", contract.symbol);
	std::println("\tSecType: {}", contract.secType);
	std::println("\tLastTradeDateOrContractMonth: {}", contract.lastTradeDateOrContractMonth);
	std::println("\tLastTradeDate: {}", contract.lastTradeDate);
	std::println("\tStrike: {}", Utility::doubleMaxString(contract.strike));
	std::println("\tRight: {}", contract.right);
	std::println("\tMultiplier: {}", contract.multiplier);
	std::println("\tExchange: {}", contract.exchange);
	std::println("\tPrimaryExchange: {}", contract.primaryExchange);
	std::println("\tCurrency: {}", contract.currency);
	std::println("\tLocalSymbol: {}", contract.localSymbol);
	std::println("\tTradingClass: {}", contract.tradingClass);
}

void CWMR_Client::printContractDetailsMsg(const ContractDetails& contractDetails) {
	std::println("\tMarketName: {}", contractDetails.marketName);
	std::println("\tMinTick: {}", Utility::doubleMaxString(contractDetails.minTick));
	std::println("\tPriceMagnifier: {}", Utility::intMaxString(contractDetails.priceMagnifier));
	std::println("\tOrderTypes: {}", contractDetails.orderTypes);
	std::println("\tValidExchanges: {}", contractDetails.validExchanges);
	std::println("\tUnderConId: {}", Utility::intMaxString(contractDetails.underConId));
	std::println("\tLongName: {}", contractDetails.longName);
	std::println("\tContractMonth: {}", contractDetails.contractMonth);
	std::println("\tIndustry: {}", contractDetails.industry);
	std::println("\tCategory: {}", contractDetails.category);
	std::println("\tSubcategory: {}", contractDetails.subcategory);
	std::println("\tTimeZoneId: {}", contractDetails.timeZoneId);
	std::println("\tTradingHours: {}", contractDetails.tradingHours);
	std::println("\tLiquidHours: {}", contractDetails.liquidHours);
	std::println("\tEvRule: {}", contractDetails.evRule);
	std::println("\tEvMultiplier: {}", Utility::doubleMaxString(contractDetails.evMultiplier));
	std::println("\tAggGroup: {}", Utility::intMaxString(contractDetails.aggGroup));
	std::println("\tUnderSymbol: {}", contractDetails.underSymbol);
	std::println("\tUnderSecType: {}", contractDetails.underSecType);
	std::println("\tMarketRuleIds: {}", contractDetails.marketRuleIds);
	std::println("\tRealExpirationDate: {}", contractDetails.realExpirationDate);
	std::println("\tLastTradeTime: {}", contractDetails.lastTradeTime);
	std::println("\tStockType: {}", contractDetails.stockType);
	std::println("\tMinSize: {}", DecimalFunctions::decimalStringToDisplay(contractDetails.minSize));
	std::println("\tSizeIncrement: {}", DecimalFunctions::decimalStringToDisplay(contractDetails.sizeIncrement));
	std::println("\tSuggestedSizeIncrement: {}", DecimalFunctions::decimalStringToDisplay(contractDetails.suggestedSizeIncrement));
	std::println("\tMinAlgoSize: {}", DecimalFunctions::decimalStringToDisplay(contractDetails.minAlgoSize));
	std::println("\tLastPricePrecision: {}", DecimalFunctions::decimalStringToDisplay(contractDetails.lastPricePrecision));
	std::println("\tLastSizePrecision: {}", DecimalFunctions::decimalStringToDisplay(contractDetails.lastSizePrecision));
	std::println("\tEventContract1: {}", contractDetails.eventContract1);
	std::println("\tEventContractDescription1: {}", contractDetails.eventContractDescription1);
	std::println("\tEventContractDescription2: {}", contractDetails.eventContractDescription2);
	if (contractDetails.contract.secType == "FUND") {
		std::println("\tFund Data:");
		std::println("\t\tFundName: {}", contractDetails.fundName);
		std::println("\t\tFundFamily: {}", contractDetails.fundFamily);
		std::println("\t\tFundType: {}", contractDetails.fundType);
		std::println("\t\tFundFrontLoad: {}", contractDetails.fundFrontLoad);
		std::println("\t\tFundBackLoad: {}", contractDetails.fundBackLoad);
		std::println("\t\tFundBackLoadTimeInterval: {}", contractDetails.fundBackLoadTimeInterval);
		std::println("\t\tFundManagementFee: {}", contractDetails.fundManagementFee);
		std::println("\t\tFundClosed: {}", contractDetails.fundClosed ? "Yes" : "No");
		std::println("\t\tFundClosedForNewInvestors: {}", contractDetails.fundClosedForNewInvestors ? "Yes" : "No");
		std::println("\t\tFundClosedForNewMoney: {}", contractDetails.fundClosedForNewMoney ? "Yes" : "No");
		std::println("\t\tFundNotifyAmount: {}", contractDetails.fundNotifyAmount);
		std::println("\t\tFundMinimumInitialPurchase: {}", contractDetails.fundMinimumInitialPurchase);
		std::println("\t\tFundSubsequentMinimumPurchase: {}", contractDetails.fundSubsequentMinimumPurchase);
		std::println("\t\tFundBlueSkyStates: {}", contractDetails.fundBlueSkyStates);
		std::println("\t\tFundBlueSkyTerritories: {}", contractDetails.fundBlueSkyTerritories);
		std::println("\t\tFundDistributionPolicyIndicator: {}", Utility::getFundDistributionPolicyIndicatorString(contractDetails.fundDistributionPolicyIndicator));
		std::println("\t\tFundAssetType: {}", Utility::getFundAssetTypeName(contractDetails.fundAssetType));
	}
	printContractDetailsSecIdList(contractDetails.secIdList);
	printContractDetailsIneligibilityReasonList(contractDetails.ineligibilityReasonList);
}

void CWMR_Client::printContractDetailsSecIdList(const TagValueListSPtr& secIdList) {
	if (secIdList) {
		std::println("\tSecIdList: {");
		for (const auto& tag : *secIdList) {
			std::print("{}={}", tag->tag, tag->value);
		}
		std::println("}");
	}
}


void CWMR_Client::printContractDetailsIneligibilityReasonList(const IneligibilityReasonListSPtr& ineligibilityReasonList) {
	if (ineligibilityReasonList) {
		std::println("\tIneligibilityReasonList: {");
		for (const auto& reason : *ineligibilityReasonList) {
			std::print("[id: {}, description: {}];", reason->id, reason->description);
		}
		std::println("}");
	}
}

void CWMR_Client::printBondContractDetailsMsg(const ContractDetails& contractDetails) {
	std::println("\tSymbol: {}", contractDetails.contract.symbol);
	std::println("\tSecType: {}", contractDetails.contract.secType);
	std::println("\tCusip: {}", contractDetails.cusip);
	std::println("\tCoupon: {}", Utility::doubleMaxString(contractDetails.coupon));
	std::println("\tMaturity: {}", contractDetails.maturity);
	std::println("\tIssueDate: {}", contractDetails.issueDate);
	std::println("\tRatings: {}", contractDetails.ratings);
	std::println("\tBondType: {}", contractDetails.bondType);
	std::println("\tCouponType: {}", contractDetails.couponType);
	std::println("\tConvertible: {}", contractDetails.convertible ? "Yes" : "No");
	std::println("\tCallable: {}", contractDetails.callable ? "Yes" : "No");
	std::println("\tPutable: {}", contractDetails.putable ? "Yes" : "No");
	std::println("\tDescAppend: {}", contractDetails.descAppend);
	std::println("\tExchange: {}", contractDetails.contract.exchange);
	std::println("\tCurrency: {}", contractDetails.contract.currency);
	std::println("\tMarketName: {}", contractDetails.marketName);
	std::println("\tTradingClass: {}", contractDetails.contract.tradingClass);
	std::println("\tConId: {}", Utility::intMaxString(contractDetails.contract.conId));
	std::println("\tMinTick: {}", Utility::doubleMaxString(contractDetails.minTick));
	std::println("\tOrderTypes: {}", contractDetails.orderTypes);
	std::println("\tValidExchanges: {}", contractDetails.validExchanges);
	std::println("\tNextOptionsDate: {}", contractDetails.nextOptionDate);
	std::println("\tNextOptionsType: {}", contractDetails.nextOptionType);
	std::println("\tNextOptionsPartial: {}", contractDetails.nextOptionPartial ? "Yes" : "No");
	std::println("\tNotes: {}", contractDetails.notes);
	std::println("\tLongName: {}", contractDetails.longName);
	std::println("\tTradingZoneId: {}", contractDetails.timeZoneId);
	std::println("\tTradingHours: {}", contractDetails.tradingHours);
	std::println("\tLiquidHours: {}", contractDetails.liquidHours);
	std::println("\tEvRule: {}", contractDetails.evRule);
	std::println("\tEvMultiplier: {}", Utility::doubleMaxString(contractDetails.evMultiplier));
	std::println("\tAggGroup: {}", Utility::intMaxString(contractDetails.aggGroup));
	std::println("\tMarketRuleIds: {}", contractDetails.marketRuleIds);
	std::println("\tTimeZoneId: {}", contractDetails.timeZoneId);
	std::println("\tLastTradeTime: {}", contractDetails.lastTradeTime);
	std::println("\tMinSize: {}", DecimalFunctions::decimalStringToDisplay(contractDetails.minSize));
	std::println("\tSizeIncrement: {}", DecimalFunctions::decimalStringToDisplay(contractDetails.sizeIncrement));
	std::println("\tSuggestedSizeIncrement: {}", DecimalFunctions::decimalStringToDisplay(contractDetails.suggestedSizeIncrement));
	printContractDetailsSecIdList(contractDetails.secIdList);
}