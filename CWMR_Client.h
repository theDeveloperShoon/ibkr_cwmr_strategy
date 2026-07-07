#pragma once
#ifndef CWMR_STRATEGY_CWMR_CLIENT_H
#define CWMR_STRATEGY_CWMR_CLIENT_H

#include "EWrapper.h"
#include "EReaderOSSignal.h"
#include "EReader.h" 

#include <memory>

class CWMR_Client : public EWrapper
{
public:
	CWMR_Client();
	~CWMR_Client();


public:
	// --- Virtual Overrides for EWrapper interface methods ---
	#include "EWrapper_prototypes.h"

private:
	EReaderOSSignal m_osSignal;
	// EClientSocket* const m_pClient;
	std::unique_ptr<EReader> m_pReader;
	bool m_extraAuth;

};



#endif // !CWMR_STRATEGY_CWMR_CLIENT_H
