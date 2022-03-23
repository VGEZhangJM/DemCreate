// DCR_PointFeature.cpp: implementation of the CDCR_PointFeature class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DCR_PointFeature.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDCR_PointFeature::CDCR_PointFeature()
{
	//缺省都是为1
	m_iPointNum		= 1;

	//
	m_dLeft			= m_dRight	= 0.0;
	m_dBottom		= m_dTop	= 0.0;

	//
	m_bUsed			= false;

	//
	m_pNext			= NULL;
}

CDCR_PointFeature::~CDCR_PointFeature()
{
}

void CDCR_PointFeature::AddPoint(double dX, double dY, double dZ)
{
	m_iPointNum		= 1;

	m_pPointCoord.x	= dX;
	m_pPointCoord.y	= dY;
	m_pPointCoord.z	= dZ;


	//
	m_dLeft			= m_dRight	= dX;
	m_dBottom		= m_dTop	= dY;
}

//
void CDCR_PointFeature::OnDraw(CDC *pDC,double dXA,double dXB,double dYA,double dYB,COLORREF crColor)
{
	//if(m_iPointNum > 0)
	{
		int	iX,iY;
		iX	= (int)(m_pPointCoord.x*dXA + dXB + 0.5);
		iY	= (int)(m_pPointCoord.y*dYA + dYB + 0.5);
		CRect	rect;
		rect.left	= iX-2;	rect.right	= iX+2;
		rect.bottom	= iY+2;	rect.top	= iY-2;

		//CPen	*pNewPen,*pOldPen;
		CBrush	*pNewBrush,*pOldBrush;

		//pNewPen		= new	CPen;
		//pNewPen->CreatePen(PS_SOLID,1,crColor);
		//pOldPen		= pDC->SelectObject(pNewPen);

		pNewBrush	= new	CBrush;
		pNewBrush->CreateSolidBrush(crColor);
		pOldBrush	= pDC->SelectObject(pNewBrush);

		pDC->FillRect(rect,pNewBrush);

		//pDC->SelectObject(pOldPen);
		//delete	pNewPen;

		pDC->SelectObject(pOldBrush);
		delete	pNewBrush;
	}
}