// DCR_TinTriangleLOP.cpp: implementation of the CDCR_TinTriangleLOP class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DCR_TinTriangleLOP.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDCR_TinTriangleLOP::CDCR_TinTriangleLOP()
{
	//
	m_pUnTriangle	= NULL;
	m_pTriangle		= NULL;

	//
	m_iVertexID1	= -1;
	m_iVertexID2	= -1;

	m_bUsed			= false;

	m_pNext			= NULL;
}

CDCR_TinTriangleLOP::~CDCR_TinTriangleLOP()
{

}
