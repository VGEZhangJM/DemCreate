// DCR_Cells.cpp: implementation of the CDCR_Cells class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DCR_Cells.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDCR_Cells::CDCR_Cells()
{
	//
	m_bUsed		= false;

	m_pPFDHead	= NULL;
	m_pPFDLast	= NULL;
	
	m_pLFDHead	= NULL;
	m_pLFDLast	= NULL;

	m_pTVDHead	= NULL;
	m_pTVDLast	= NULL;

	m_pTEDHead	= NULL;
	m_pTEDLast	= NULL;
}

CDCR_Cells::~CDCR_Cells()
{
	CDCR_PointFeatureAddress	*pfd1,*pfd2;
	CDCR_LineFeatureAddress		*lfd1,*lfd2;
	CDCR_TinVertexAddress		*tvd1,*tvd2;
	CDCR_TinEdgeAddress			*ted1,*ted2;
	
	if(m_pPFDHead != NULL)
	{
		pfd1	= pfd2	= m_pPFDHead;
		while(pfd1 != NULL)
		{
			pfd1	= pfd1->m_pNext;
			delete	pfd2;
			pfd2	= NULL;
			pfd2	= pfd1;
		};
		m_pPFDHead	= NULL;
		m_pPFDLast	= NULL;
	}

	if(m_pLFDHead != NULL)
	{
		lfd1	= lfd2	= m_pLFDHead;
		while(lfd1 != NULL)
		{
			lfd1	= lfd1->m_pNext;
			delete	lfd2;
			lfd2	= NULL;
			lfd2	= lfd1;
		};
		m_pLFDHead	= NULL;
		m_pLFDLast	= NULL;
	}

	if(m_pTVDHead != NULL)
	{
		tvd1	= tvd2	= m_pTVDHead;
		while(tvd1 != NULL)
		{
			tvd1	= tvd1->m_pNext;
			delete	tvd2;
			tvd2	= NULL;
			tvd2	= tvd1;
		};
		m_pTVDHead	= NULL;
		m_pTVDLast	= NULL;
	}

	if(m_pTEDHead != NULL)
	{
		ted1	= ted2	= m_pTEDHead;
		while(ted1 != NULL)
		{
			ted1	= ted1->m_pNext;
			delete	ted2;
			ted2	= NULL;
			ted2	= ted1;
		};
		m_pTEDHead	= NULL;
		m_pTEDLast	= NULL;
	}
}
