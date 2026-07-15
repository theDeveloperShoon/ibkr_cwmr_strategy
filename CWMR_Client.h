#pragma once
#ifndef CWMR_STRATEGY_CWMR_CLIENT_H
#define CWMR_STRATEGY_CWMR_CLIENT_H

#include "EWrapper.h"
#include "EReaderOSSignal.h"
#include "EReader.h" 

#include <memory>

class EClientSocket;

class CWMR_Client : public EWrapper
{
public:
	CWMR_Client();
	~CWMR_Client();

	void setConnectOptions(const std::string&);
	void processMessages();

public: 
	bool connect(const std::string& host, int port, int clientId = 0);
	bool isConnected() const;

public:
	// --- Virtual Overrides for EWrapper interface methods ---
	#include "EWrapper_prototypes.h"

private: 
	void printContractMsg(const Contract& contract);
	void printContractDetailsMsg(const ContractDetails& contractDetails);
	void printContractDetailsSecIdList(const TagValueListSPtr& secIdList);
	void printContractDetailsIneligibilityReasonList(const IneligibilityReasonListSPtr& ineligibilityReasonList);
	void printBondContractDetailsMsg(const ContractDetails& contractDetails);

private:
	EReaderOSSignal m_osSignal;
	 EClientSocket* const m_pClient;
	std::unique_ptr<EReader> m_pReader;

	int m_orderId;
	bool m_extraAuth;

};



#endif // !CWMR_STRATEGY_CWMR_CLIENT_H
