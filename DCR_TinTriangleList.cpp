// DCR_TinTriangleList.cpp: implementation of the CDCR_TinTriangleList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "vr_demcreator.h"
#include "DCR_TinTriangleList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDCR_TinTriangleList::CDCR_TinTriangleList()
{
	m_pTriangleList.RemoveAll();
}

CDCR_TinTriangleList::~CDCR_TinTriangleList()
{
	m_pTriangleList.RemoveAll();
}

//
void CDCR_TinTriangleList::Add(int iID1,double dX1,double dY1,int iID2,double dX2,double dY2,int iID3,double dX3,double dY3)
{
	tagTRIANGLE	tagTriangle;

	tagTriangle.iID1= iID1;
	tagTriangle.dX1	= dX1;
	tagTriangle.dY1	= dY1;
	tagTriangle.iID2= iID2;
	tagTriangle.dX2	= dX2;
	tagTriangle.dY2	= dY2;
	tagTriangle.iID3= iID3;
	tagTriangle.dX3	= dX3;
	tagTriangle.dY3	= dY3;
	
	m_pTriangleList.Add(tagTriangle);
}

//
void CDCR_TinTriangleList::Clone(CDCR_TinTriangleList *pList)
{
	if(pList == NULL)	return;

	m_pTriangleList.RemoveAll();

	for(int i=0;i<pList->m_pTriangleList.GetSize();i++)
	{
		int		iID1;
		double	dX1;
		double	dY1;
		int		iID2;
		double	dX2;
		double	dY2;
		int		iID3;
		double	dX3;
		double	dY3;

		iID1= pList->m_pTriangleList[i].iID1;
		dX1	= pList->m_pTriangleList[i].dX1;
		dY1	= pList->m_pTriangleList[i].dY1;
		iID2= pList->m_pTriangleList[i].iID2;
		dX2	= pList->m_pTriangleList[i].dX2;
		dY2	= pList->m_pTriangleList[i].dY2;
		iID3= pList->m_pTriangleList[i].iID3;
		dX3	= pList->m_pTriangleList[i].dX3;
		dY3	= pList->m_pTriangleList[i].dY3;

		Add(iID1,dX1,dY1,iID2,dX2,dY2,iID3,dX3,dY3);
	}
}

//
void CDCR_TinTriangleList::Release()
{
	m_pTriangleList.RemoveAll();
}

void CDCR_TinTriangleList::Draw(CDC *pDC,double dXA,double dXB,double dYA,double dYB)
{
	CPen	*pOldPen,*pNewPen;

	pNewPen	= new	CPen;
	pNewPen->CreatePen(PS_SOLID,1,RGB(192,192,192));
	pOldPen	= pDC->SelectObject(pNewPen);

	for(int i=0;i<m_pTriangleList.GetSize();i++)
	{		
		POINT	ptPoints[4];

		ptPoints[0].x	= (long)(dXA*m_pTriangleList[i].dX1 + dXB + 0.5);
		ptPoints[0].y	= (long)(dYA*m_pTriangleList[i].dY1 + dYB + 0.5);

		ptPoints[1].x	= (long)(dXA*m_pTriangleList[i].dX2 + dXB + 0.5);
		ptPoints[1].y	= (long)(dYA*m_pTriangleList[i].dY2 + dYB + 0.5);

		ptPoints[2].x	= (long)(dXA*m_pTriangleList[i].dX3 + dXB + 0.5);
		ptPoints[2].y	= (long)(dYA*m_pTriangleList[i].dY3 + dYB + 0.5);

		ptPoints[3].x	= (long)(dXA*m_pTriangleList[i].dX1 + dXB + 0.5);
		ptPoints[3].y	= (long)(dYA*m_pTriangleList[i].dY1 + dYB + 0.5);
		
		pDC->Polyline(ptPoints,4);

		CString	szInfo;
		szInfo.Format("%ld",m_pTriangleList[i].iID1);
		//pDC->TextOut(ptPoints[0].x,ptPoints[0].y,szInfo);

		szInfo.Format("%ld",m_pTriangleList[i].iID2);
		//pDC->TextOut(ptPoints[1].x,ptPoints[1].y,szInfo);

		szInfo.Format("%ld",m_pTriangleList[i].iID3);
		//pDC->TextOut(ptPoints[2].x,ptPoints[2].y,szInfo);
	}

	pDC->SelectObject(pOldPen);
	delete	pNewPen;
}