// DCR_TinVertex.cpp: implementation of the CDCR_TinVertex class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DCR_TinVertex.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDCR_TinVertex::CDCR_TinVertex()
{
	m_nID	= -1;
	m_zz	= 0.0;
	m_yy	= 0.0;
	m_xx	= 0.0;

	m_bUsed	= false;

	m_pNext	= NULL;
}

CDCR_TinVertex::~CDCR_TinVertex()
{

}

void CDCR_TinVertex::Add(int iID, double dX, double dY, double dZ)
{
	m_xx	= dX;
	m_yy	= dY;
	m_zz	= dZ;
	m_nID	= iID;	
}
