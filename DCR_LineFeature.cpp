// DCR_LineFeature.cpp: implementation of the CDCR_LineFeature class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DCR_LineFeature.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDCR_LineFeature::CDCR_LineFeature()
{
	//缺省都是大于等于2
	m_iPointNum		= 0;
	//具体的三维坐标点
	m_pPointCoord	= NULL;

	//
	m_dLeft			= m_dRight	= 0.0;
	m_dBottom		= m_dTop	= 0.0;

	//
	m_pNext			= NULL;
}

CDCR_LineFeature::~CDCR_LineFeature()
{
	if(m_pPointCoord != NULL)
	{
		delete	[]m_pPointCoord;
	}
}

//
void CDCR_LineFeature::AddLine(int iPointNum, double *dX, double *dY, double dZ)
{
	m_iPointNum	= iPointNum;

	m_pPointCoord	= new	SMap_3DPointCoord[m_iPointNum];
	
	for(int i=0;i<m_iPointNum;i++)
	{
		m_pPointCoord[i].x	= dX[i];
		m_pPointCoord[i].y	= dY[i];
		m_pPointCoord[i].z	= dZ;
	

		if(i==0)
		{
			m_dLeft			= m_dRight	= dX[i];
			m_dBottom		= m_dTop	= dY[i];
		}
		else
		{
			if(m_dLeft > dX[i])		m_dLeft		= dX[i];
			if(m_dRight < dX[i])	m_dRight	= dX[i];
			if(m_dBottom > dY[i])	m_dBottom	= dY[i];
			if(m_dTop < dY[i])		m_dTop		= dY[i];
		}
	}
}

//
void CDCR_LineFeature::OnDraw(CDC *pDC,double dXA,double dXB,double dYA,double dYB,COLORREF crColor)
{
	if(m_iPointNum > 0)
	{
		POINT	*ptPoints;
		ptPoints	= new	POINT[m_iPointNum];

		for(int i=0;i<m_iPointNum;i++)
		{
			ptPoints[i].x	= (int)(m_pPointCoord[i].x*dXA + dXB + 0.5);
			ptPoints[i].y	= (int)(m_pPointCoord[i].y*dYA + dYB + 0.5);
		}

		CPen	*pNewPen,*pOldPen;

		pNewPen	= new	CPen;
		pNewPen->CreatePen(PS_SOLID,1,crColor);
		pOldPen	= pDC->SelectObject(pNewPen);

		pDC->Polyline(ptPoints,m_iPointNum);

		pDC->SelectObject(pOldPen);
		delete	pNewPen;
		
		delete	[]ptPoints;
		ptPoints	= NULL;
	}
}