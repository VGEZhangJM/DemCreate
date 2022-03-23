// DCR_TinEdgeAddress.cpp: implementation of the CDCR_TinEdgeAddress class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DCR_TinEdgeAddress.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDCR_TinEdgeAddress::CDCR_TinEdgeAddress()
{
	//
	m_pNext	= NULL;
	//
	m_bUsed	= false;
	//
	m_pEdge	= NULL;
}

CDCR_TinEdgeAddress::~CDCR_TinEdgeAddress()
{

}

//
void CDCR_TinEdgeAddress::SetEdge(CDCR_TinEdge *pEdge)
{
	m_pEdge	= pEdge;
}

//
CDCR_TinEdge* CDCR_TinEdgeAddress::GetEdge()
{
	return	m_pEdge;
}
