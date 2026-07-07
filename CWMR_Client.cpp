#include "CWMR_Client.h"

#include "EClientSocket.h"

CWMR_Client::CWMR_Client() : 
	m_osSignal(2000),
	m_pReader(std::make_unique<EReader>(new EClientSocket(this, &m_osSignal))),
	m_extraAuth(false)
{
}