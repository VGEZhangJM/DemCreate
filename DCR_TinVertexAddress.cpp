// DCR_TinVertexAddress.cpp: implementation of the CDCR_TinVertexAddress class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DCR_TinVertexAddress.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDCR_TinVertexAddress::CDCR_TinVertexAddress()
{
	//
	m_pNext		= NULL;
	//
	m_pVertex	= NULL;
}

CDCR_TinVertexAddress::~CDCR_TinVertexAddress()
{

}

//
void CDCR_TinVertexAddress::SetVertex(CDCR_TinVertex *pVertex)
{
	m_pVertex	= pVertex;
}

//
CDCR_TinVertex* CDCR_TinVertexAddress::GetVertex()
{
	return	m_pVertex;
}
