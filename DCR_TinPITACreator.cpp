// DCR_TinPITACreator.cpp: implementation of the CDCR_TinPITACreator class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DCR_TinPITACreator.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDCR_TinPITACreator::CDCR_TinPITACreator()
{
	//�����б�
	m_pVertexHead		= NULL;
	m_pVertexLast		= NULL;
	m_iVertexCount		= 0;

	//͹���б�
	m_pConvexHullHead	= NULL;
	m_pConvexHullLast	= NULL;

	//���б�
	m_pEdgeHead			= NULL;
	m_pEdgeLast			= NULL;
	m_iEdgeCount		= 0;

	//�������б�
	m_pTriangleHead		= NULL;
	m_pTriangleLast		= NULL;
	m_iTriangleCount	= 0;
	
	//
	m_dMapXA			= 1.0;
	m_dMapXB			= 0.0;
	m_dMapYA			= -1.0;
	m_dMapYB			= 0.0;

	//��ͼ����
	m_dMapLeft		= 0.0;
	m_dMapRight		= 0.0;
	m_dMapBottom	= 0.0;
	m_dMapTop		= 0.0;

	//��ͼ������С
	m_dCellSizeOfY	= 0.0;
	m_dCellSizeOfX	= 0.0;
	
	//��ͼ�ֿ���
	m_nCellNumOfY	= 15;
	m_nCellNumOfX	= 15;

	//��������Χ
	m_dCellLeft		= m_dCellRight	= 0;
	m_dCellBottom	= m_dCellTop	= 0;

	m_pCells		= NULL;

	//
	m_pTriangleLOPHead	= NULL;
	m_pTriangleLOPLast	= NULL;
}

CDCR_TinPITACreator::~CDCR_TinPITACreator()
{
	if(m_pCells != NULL)
	{
		delete	[]m_pCells;
		m_pCells	= NULL;
	}
	Release();
}

//////////////////////////////////////////////////////////////////////
// 01 ͨ������ID��ȡ��������
// ����ֵ��bool��true��ʾ�ɹ���false��ʾʧ��
//////////////////////////////////////////////////////////////////////
bool CDCR_TinPITACreator::GetVertexCoordFromID(int iVertexID, double &dX, double &dY, double &dZ)
{
	CDCR_TinVertex	*pVertex;

	pVertex	= m_pVertexHead;
	while(pVertex != NULL)
	{
		if(pVertex->m_nID == iVertexID)
		{
			dX	= pVertex->m_xx;
			dY	= pVertex->m_yy;
			dZ	= pVertex->m_zz;

			return	true;
		}		

		pVertex	= pVertex->m_pNext;
	}

	return	false;
}

//////////////////////////////////////////////////////////////////////
// �ͷű�
//////////////////////////////////////////////////////////////////////
void CDCR_TinPITACreator::ReleaseEdge()
{
	CDCR_TinEdge	*pEdge1,*pEdge2;

	if(m_pEdgeHead != NULL)
	{
		pEdge1	= pEdge2	= m_pEdgeHead;
		do
		{
			pEdge1		= pEdge1->m_pNext;
			delete	pEdge2;
			pEdge2		= pEdge1;
		}while(pEdge1 != NULL);

		m_pEdgeHead	= NULL;
		m_pEdgeLast	= NULL;
	}	
}

//////////////////////////////////////////////////////////////////////
// �ͷ�������
//////////////////////////////////////////////////////////////////////
void CDCR_TinPITACreator::ReleaseTriangle()
{
	CDCR_TinTriangle	*pTriangle1,*pTriangle2;

	if(m_pTriangleHead != NULL)
	{
		pTriangle1	= pTriangle2	= m_pTriangleHead;
		do
		{
			pTriangle1		= pTriangle1->m_pNext;
			delete	pTriangle2;
			pTriangle2		= pTriangle1;
		}while(pTriangle1 != NULL);

		m_pTriangleHead	= NULL;
		m_pTriangleLast	= NULL;
	}	
}

//////////////////////////////////////////////////////////////////////
// �ͷŶ���
//////////////////////////////////////////////////////////////////////
void CDCR_TinPITACreator::ReleaseVertex()
{
	CDCR_TinVertex	*pVertex1,*pVertex2;

	if(m_pVertexHead != NULL)
	{
		pVertex1	= pVertex2	= m_pVertexHead;
		do
		{
			pVertex1		= pVertex1->m_pNext;
			delete	pVertex2;
			pVertex2		= pVertex1;
		}while(pVertex1 != NULL);

		m_pVertexHead	= NULL;
		m_pVertexLast	= NULL;
	}	
}

//////////////////////////////////////////////////////////////////////
// �ͷ�͹���Ķ���
//////////////////////////////////////////////////////////////////////
void CDCR_TinPITACreator::ReleaseConverHull()
{
	CDCR_TinVertex	*pVertex1,*pVertex2;

	if(m_pConvexHullHead != NULL)
	{
		pVertex1	= pVertex2	= m_pConvexHullHead;
		do
		{
			pVertex1		= pVertex1->m_pNext;
			delete	pVertex2;
			pVertex2		= pVertex1;
		}while(pVertex1 != NULL);

		m_pConvexHullHead	= NULL;
		m_pConvexHullLast	= NULL;
	}	
}

//////////////////////////////////////////////////////////////////////
// 001 �ͷ�TIN���ݼ�����������
//////////////////////////////////////////////////////////////////////
void CDCR_TinPITACreator::Release( )
{
	//
	ReleaseVertex();

	//
	ReleaseTriangle();

	//
	ReleaseEdge();

	//
	ReleaseConverHull();

	//
	ReleaseTriangleLOP();
}

//////////////////////////////////////////////////////////////////////
// 002 ���Ӷ�������
//////////////////////////////////////////////////////////////////////
void CDCR_TinPITACreator::AddVertex(double dX,double dY,double dZ)
{
	CDCR_TinVertex*	pVertex;

	m_iVertexCount++;

	pVertex	= new	CDCR_TinVertex;
	pVertex->Add(m_iVertexCount,dX,dY,dZ);
		
	if(m_pVertexHead == NULL)
	{
		m_pVertexHead	= pVertex;
		m_pVertexLast	= pVertex;
	}
	else
	{	
		m_pVertexLast->m_pNext	= pVertex;
		m_pVertexLast			= pVertex;
	}
}

//////////////////////////////////////////////////////////////////////
// 003 ������ʾ����
//////////////////////////////////////////////////////////////////////
void CDCR_TinPITACreator::SetCoordMapping(double dXA,double dXB,double dYA,double dYB)
{
	m_dMapXA	= dXA;
	m_dMapXB	= dXB;

	m_dMapYA	= dYA;
	m_dMapYB	= dYB;
}

void CDCR_TinPITACreator::SetClientRect(CRect rRect)
{
	m_rRect	= rRect;
}

//////////////////////////////////////////////////////////////////////
// �����������Ӷ���
//////////////////////////////////////////////////////////////////////
bool CDCR_TinPITACreator::AddVertexToCells(CDCR_TinVertex *pVertex)
{
	if(pVertex == NULL)				return	false;

	int							ci;	
	int							xi,yi;
	CDCR_TinVertexAddress		*tvd;
		
	//��������һ����
	xi	= (int)((pVertex->m_xx - m_dCellLeft) / m_dCellSizeOfX);
	yi	= (int)((pVertex->m_yy - m_dCellBottom) / m_dCellSizeOfY);

	if(xi < 0)				xi	= 0;
	if(yi < 0)				yi	= 0;
	if(xi >= m_nCellNumOfX)	xi	= m_nCellNumOfX-1;
	if(yi >= m_nCellNumOfY)	yi	= m_nCellNumOfY-1;

	ci	= yi * m_nCellNumOfX + xi;

	if(m_pCells[ci].m_pTVDHead == NULL)
	{
		tvd							= new	CDCR_TinVertexAddress;	
		tvd->m_pVertex				= pVertex;

		m_pCells[ci].m_pTVDHead		= tvd;
		m_pCells[ci].m_pTVDLast		= tvd;
	}
	else
	{
		tvd							= new	CDCR_TinVertexAddress;	
		tvd->m_pVertex				= pVertex;

		m_pCells[ci].m_pTVDLast->m_pNext	= tvd;
		m_pCells[ci].m_pTVDLast				= tvd;
	}

	return	true;
}

//////////////////////////////////////////////////////////////////////
// �������ӵ�����������
//////////////////////////////////////////////////////////////////////
bool CDCR_TinPITACreator::AddEdgeToCells(double dX,double dY,CDCR_TinEdge *pEdge)
{
	if(pEdge == NULL)				return	false;

	int							ci;	
	int							xi,yi;
	CDCR_TinEdgeAddress			*pTED;
		
	//��������һ����
	xi	= (int)((dX - m_dCellLeft) / m_dCellSizeOfX);
	yi	= (int)((dY - m_dCellBottom) / m_dCellSizeOfY);

	if(xi < 0)				xi	= 0;
	if(yi < 0)				yi	= 0;
	if(xi >= m_nCellNumOfX)	xi	= m_nCellNumOfX-1;
	if(yi >= m_nCellNumOfY)	yi	= m_nCellNumOfY-1;

	ci	= yi * m_nCellNumOfX + xi;

	if(m_pCells[ci].m_pTEDHead == NULL)
	{
		pTED						= new	CDCR_TinEdgeAddress;	
		pTED->m_pEdge				= pEdge;

		m_pCells[ci].m_pTEDHead		= pTED;
		m_pCells[ci].m_pTEDLast		= pTED;
	}
	else
	{
		pTED						= new	CDCR_TinEdgeAddress;	
		pTED->m_pEdge				= pEdge;

		m_pCells[ci].m_pTEDLast->m_pNext	= pTED;
		m_pCells[ci].m_pTEDLast				= pTED;
	}

	return	true;	
}

//////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////
void CDCR_TinPITACreator::SetMapExt(double dLeft,double dRight,double dBottom,double dTop)
{
	m_dMapLeft		= dLeft;
	m_dMapRight		= dRight;
	m_dMapBottom	= dBottom;
	m_dMapTop		= dTop;
}

//////////////////////////////////////////////////////////////////////
// ��������
//////////////////////////////////////////////////////////////////////
void CDCR_TinPITACreator::CellBuild()
{
	int i,j,ci;

	//��ʼ��
	if(m_pCells != NULL)
	{
		delete	[]m_pCells;
		m_pCells	= NULL;
	}
	
	//���¼�������ߴ�
	double	dSize;
	dSize	= sqrt( (m_dMapRight-m_dMapLeft)*(m_dMapTop-m_dMapBottom)/m_iVertexCount );
	m_dCellSizeOfX	= m_dCellSizeOfY	= 4*dSize/3;

	//�ȼ�������ߴ�
	m_nCellNumOfX	= (int)((m_dMapRight-m_dMapLeft)/m_dCellSizeOfX);
	m_nCellNumOfY	= (int)((m_dMapTop-m_dMapBottom)/m_dCellSizeOfY);

	//Ȼ��������Χ�����»������񻮷�����Χ
	m_dCellLeft		= m_dMapLeft - m_dCellSizeOfX/2;
	m_dCellRight	= m_dMapRight + m_dCellSizeOfX/2;
	m_dCellBottom	= m_dMapBottom - m_dCellSizeOfY/2;
	m_dCellTop		= m_dMapTop + m_dCellSizeOfY/2;

	//���¼�������ߴ�
	m_dCellSizeOfX	= (m_dCellRight-m_dCellLeft)/m_nCellNumOfX;
	m_dCellSizeOfY	= (m_dCellTop-m_dCellBottom)/m_nCellNumOfY;

	m_pCells		= new	CDCR_Cells[(m_nCellNumOfX*m_nCellNumOfY)];

	for(i=0;i<m_nCellNumOfY;i++)
	{
		for(j=0;j<m_nCellNumOfX;j++)
		{
			ci	= i*m_nCellNumOfX + j;
						
			m_pCells[ci].m_pPFDHead	= NULL;
			m_pCells[ci].m_pPFDLast	= NULL;
			m_pCells[ci].m_pLFDHead	= NULL;
			m_pCells[ci].m_pLFDLast	= NULL;
			m_pCells[ci].m_pTVDHead	= NULL;
			m_pCells[ci].m_pTVDLast	= NULL;
		}
	}

	//
	CDCR_TinVertex	*pVertex;

	pVertex	= m_pVertexHead;
	while(pVertex != NULL)
	{		
		//��������
		AddVertexToCells(pVertex);

		pVertex	= pVertex->m_pNext;
	}
}

//////////////////////////////////////////////////////////////////////
// 004 �����������㷨����TIN
//////////////////////////////////////////////////////////////////////
bool CDCR_TinPITACreator::Create(CDC *pDC)
{
	//���ͷ�,�ߺ�������
	{
		//
		ReleaseConverHull();
		
		//
		ReleaseEdge();

		//
		ReleaseTriangle();
	}

	//��������
	CellBuild();

	//��ȡ�ĸ��ǵ�
	GetMaxMinVextex();

	//����͹��
	CreateConvexHull(pDC);

	//�����һ��
	InsertFirstVertex(pDC);
	
	//����������
	InsertOrtherVertex(pDC);

	return	true;	
}

//////////////////////////////////////////////////////////////////////
// 005 ���Ʊ�
//////////////////////////////////////////////////////////////////////
void CDCR_TinPITACreator::DrawEdge(CDC* pDC,CDCR_TinEdge *pEdge)
{
	if(pEdge != NULL)
	{
		int	nVertexID1,nVertexID2;
		nVertexID1	= pEdge->m_nVertexID[0];
		nVertexID2	= pEdge->m_nVertexID[1];

		double	dX1,dY1,dZ1;
		double	dX2,dY2,dZ2;

		GetVertexCoordFromID(nVertexID1,dX1,dY1,dZ1);
		GetVertexCoordFromID(nVertexID2,dX2,dY2,dZ2);
		
		POINT	ptPoints[2];

		ptPoints[0].x	= (int)(dX1*m_dMapXA + m_dMapXB + 0.5);
		ptPoints[0].y	= (int)(dY1*m_dMapYA + m_dMapYB + 0.5);

		ptPoints[1].x	= (int)(dX2*m_dMapXA + m_dMapXB + 0.5);
		ptPoints[1].y	= (int)(dY2*m_dMapYA + m_dMapYB + 0.5);
		
		pDC->Polyline(ptPoints,2);
	}
}

//////////////////////////////////////////////////////////////////////
// 005 ����������
//////////////////////////////////////////////////////////////////////
void CDCR_TinPITACreator::DrawTriangle(CDC* pDC,CDCR_TinTriangle *pTriangle)
{
	int		iX,iY;
	POINT	ptPoints[4];
	iX	= 0;
	iY	= 0;
	for(int j=0;j<3;j++)
	{
		ptPoints[j].x	= (int)(pTriangle->m_dX[j]*m_dMapXA + m_dMapXB + 0.5);
		ptPoints[j].y	= (int)(pTriangle->m_dY[j]*m_dMapYA + m_dMapYB + 0.5);

		iX	= iX + ptPoints[j].x;
		iY	= iY + ptPoints[j].y;
	}
	ptPoints[3]	= ptPoints[0];

	iX	= iX/3;
	iY	= iY/3;

	CPen	*pOldPen,*pNewPen;

	pNewPen	= new	CPen;
	pNewPen->CreatePen(PS_SOLID,1,RGB(255,0,0));
	pOldPen	= pDC->SelectObject(pNewPen);

	pDC->Polyline(ptPoints,4);

	pDC->SelectObject(pOldPen);
	delete	pNewPen;

	CString	szInfo;
	szInfo.Format("%ld",pTriangle->m_nID);
	pDC->SetBkMode(TRANSPARENT);
	//pDC->TextOut(iX,iY,szInfo);
}

//////////////////////////////////////////////////////////////////////
// 006 ����TIN
//////////////////////////////////////////////////////////////////////
void CDCR_TinPITACreator::Draw(CDC *pDC)
{
	//
	DrawConvexHull(pDC);
	
	CDCR_TinTriangle	*pTriangle;

	pTriangle	= m_pTriangleHead;
	while(pTriangle != NULL)
	{
		if(false == pTriangle->m_bDelete)
			DrawTriangle(pDC,pTriangle);

		pTriangle	= pTriangle->m_pNext;
	}
}

//////////////////////////////////////////////////////////////////////
// 02 ��ȡ�Ƕ�
//////////////////////////////////////////////////////////////////////
double CDCR_TinPITACreator::GetAngle(double dX, double dY)
{
	double	dAngle;

	if(fabs(dY) < 1e-10)
	{
		if(dX > 0.0)					dAngle	= 0.0;
		else							dAngle	= PI;
	}
	else	if(dY > 1e-10)
	{
		if(dX > 1e-10)					dAngle	= atan2(fabs(dY),fabs(dX));		
		else	if(fabs(dX) < 1e-10)	dAngle	= PI/2;
		else	if(dX < 1e-10)			dAngle	= PI-atan2(fabs(dY),fabs(dX));
	}
	else
	{
		if(dX > 1e-10)					dAngle	= PI*2-atan2(fabs(dY),fabs(dX));
		else	if(fabs(dX) < 1e-10)	dAngle	= PI*3/2;
		else	if(dX < 1e-10)			dAngle	= PI+atan2(fabs(dY),fabs(dX));
	}
	return	dAngle;
}

//////////////////////////////////////////////////////////////////////
// 02 ��ȡ���
//////////////////////////////////////////////////////////////////////
double CDCR_TinPITACreator::GetArea(double x1, double y1, double x2, double y2, double x3, double y3)
{
	double m_dArea;
	
	//m_dArea	= ((x2-x1)*(y3-y1)-(x3-x1)*(y2-y1))/2;
	m_dArea	= ((x2-x1)*(y3-y1)-(x3-x1)*(y2-y1));

	return	m_dArea;
}

//////////////////////////////////////////////////////////////////////
// 01 ��ȡ�����ľ���
//////////////////////////////////////////////////////////////////////
double CDCR_TinPITACreator::GetDistance(double x1, double y1, double z1, double x2, double y2, double z2)
{
	double	dDistance;

	//dDistance	= sqrt( (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) + (z1-z2)*(z1-z2) );
	dDistance	= sqrt( (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) );

	return	dDistance;
}

//////////////////////////////////////////////////////////////////////
// 04 ��������������
// ��ȡ���ҽǶ�ֵ
//////////////////////////////////////////////////////////////////////
double CDCR_TinPITACreator::GetCosValue(double dX, double dY, double dZ, double dX1, double dY1, double dZ1,double dX2, double dY2,double dZ2)
{
	double	a,b,c;

	a	= GetDistance(dX1,dY1,dZ1,dX2,dY2,dZ2);
	b	= GetDistance(dX,dY,dZ,dX1,dY1,dZ1);
	c	= GetDistance(dX,dY,dZ,dX2,dY2,dZ2);

	double	cosa;
	cosa	= (b*b+c*c-a*a)/(2*b*c);

	return	cosa;
}


//////////////////////////////////////////////////////////////////////
// 02 ��ȡ�����㼯�е������С����
//////////////////////////////////////////////////////////////////////
void CDCR_TinPITACreator::GetMaxMinVextex()
{
	if(m_pVertexHead == NULL)	return;

	CDCR_TinVertex	*pVertex;

	//x+y�������Сֵ
	double	dMinXAndY,dMaxXAndY;
	//x-y�������Сֵ
	double	dMinXMinY,dMaxXMinY;
	
	//x+y�������Сֵ
	CDCR_TinVertex	*m_pMinXAddY	= NULL;
	CDCR_TinVertex	*m_pMaxXAddY	= NULL;
	//x-y�������Сֵ
	CDCR_TinVertex	*m_pMinXMinY	= NULL;
	CDCR_TinVertex	*m_pMaxXMinY	= NULL;

	pVertex	= m_pVertexHead;
	while(pVertex != NULL)
	{
		//
		double	dXAndY;
		dXAndY	= pVertex->m_xx + pVertex->m_yy;

		if(pVertex == m_pVertexHead)
		{
			dMinXAndY	= dMaxXAndY		= dXAndY;
			m_pMinXAddY	= m_pMaxXAddY	= pVertex;
		}
		else
		{
			if(dMinXAndY>dXAndY)	{	dMinXAndY	= dXAndY;	m_pMinXAddY	= pVertex;	}
			if(dMaxXAndY<dXAndY)	{	dMaxXAndY	= dXAndY;	m_pMaxXAddY	= pVertex;	}
		}
		
		//
		double	dXMinY;
		dXMinY	= pVertex->m_xx - pVertex->m_yy;

		if(pVertex == m_pVertexHead)
		{
			dMinXMinY	= dMaxXMinY		= dXMinY;
			m_pMinXMinY	= m_pMaxXMinY	= pVertex;
		}
		else
		{
			if(dMinXMinY>dXMinY)	{	dMinXMinY	= dXMinY;	m_pMinXMinY	= pVertex;	}
			if(dMaxXMinY<dXMinY)	{	dMaxXMinY	= dXMinY;	m_pMaxXMinY	= pVertex;	}
		}

		pVertex	= pVertex->m_pNext;
	}
	
	//��͹���б�������
	//x+y��Сֵ
	AddVertexToConvexHull(m_pMinXAddY);
	m_pMinXAddY->m_bUsed	= true;

	//x-y���ֵ
	AddVertexToConvexHull(m_pMaxXMinY);
	m_pMaxXMinY->m_bUsed	= true;

	//x+y���ֵ
	AddVertexToConvexHull(m_pMaxXAddY);
	m_pMaxXAddY->m_bUsed	= true;

	//x-y��Сֵ
	AddVertexToConvexHull(m_pMinXMinY);
	m_pMinXMinY->m_bUsed	= true;
}

//////////////////////////////////////////////////////////////////////
// 02 ����ӵ�͹���㼯�У�����㼯��ԭʼ������㼯������֮����
// �� ID��Ҫһ�£���Ϊ��ЩҪ��Ϊ������������
//////////////////////////////////////////////////////////////////////
void CDCR_TinPITACreator::AddVertexToConvexHull(CDCR_TinVertex *pVertex)
{
	if(pVertex == NULL)		return;

	bool	bExisted;
	CDCR_TinVertex	*pTempVertex;

	bExisted= false;
	pTempVertex	= m_pConvexHullHead;
	while(pTempVertex != NULL)
	{
		if(pTempVertex->m_nID == pVertex->m_nID)
		{
			bExisted	= true;
			break;		
		}

		pTempVertex	= pTempVertex->m_pNext;
	}

	if(!bExisted)
	{
		pTempVertex	= new	CDCR_TinVertex;
		pTempVertex->m_nID	= pVertex->m_nID;
		pTempVertex->m_xx	= pVertex->m_xx;
		pTempVertex->m_yy	= pVertex->m_yy;
		pTempVertex->m_zz	= pVertex->m_zz;	
		
		if(m_pConvexHullHead == NULL)
		{
			m_pConvexHullHead	= pTempVertex;
			m_pConvexHullLast	= pTempVertex;
		}
		else
		{
			m_pConvexHullLast->m_pNext	= pTempVertex;
			m_pConvexHullLast			= pTempVertex;
		}
	}
}

//////////////////////////////////////////////////////////////////////
// 02 ��ָ����λ�ú����ڵ�
// �� ID��Ҫһ�£���Ϊ��ЩҪ��Ϊ������������
//////////////////////////////////////////////////////////////////////
void CDCR_TinPITACreator::AddVertexToConvexHull(CDCR_TinVertex *pInsertAtVertex, CDCR_TinVertex *pVertex)
{
	CDCR_TinVertex*	pTempVertex;
	
	pTempVertex	= new	CDCR_TinVertex;
	pTempVertex->m_nID	= pVertex->m_nID;
	pTempVertex->m_xx	= pVertex->m_xx;
	pTempVertex->m_yy	= pVertex->m_yy;
	pTempVertex->m_zz	= pVertex->m_zz;	

	//��pInsertAtVertex�������
	pVertex		= m_pConvexHullHead;
	while(pVertex != NULL)
	{
		if(pVertex == pInsertAtVertex)
		{
			break;
		}

		pVertex	= pVertex->m_pNext;
	}

	if(pVertex != NULL)
	{	
		//����������һλ����
		if(pVertex->m_pNext == NULL)
		{
			m_pConvexHullLast->m_pNext	= pTempVertex;
			m_pConvexHullLast			= pTempVertex;
		}
		else
		{
			CDCR_TinVertex	*pMyVertex;
			//
			pMyVertex				= pVertex->m_pNext;

			pVertex->m_pNext		= pTempVertex;
	
			pTempVertex->m_pNext	= pMyVertex;
		}
	}
}


//////////////////////////////////////////////////////////////////////
//���ݶ���ID����͹���㼯�����
//////////////////////////////////////////////////////////////////////
CDCR_TinVertex* CDCR_TinPITACreator::GetIndexFromConvexHull(int nVertexID)
{
	CDCR_TinVertex	*pVertex;

	int	iIndex;
	iIndex	= -1;

	pVertex	= m_pConvexHullHead;
	while(pVertex != NULL)
	{
		//
		if(pVertex->m_nID == nVertexID)
		{
			return	pVertex;
		}

		pVertex	= pVertex->m_pNext;
	}
	
	return	NULL;
}

//////////////////////////////////////////////////////////////////////
//���ݶ���ID����͹���㼯�����
//////////////////////////////////////////////////////////////////////
CDCR_TinVertex* CDCR_TinPITACreator::GetIndexFromVertex(int nVertexID)
{
	CDCR_TinVertex	*pVertex;

	int	iIndex;
	iIndex	= -1;

	pVertex	= m_pVertexHead;
	while(pVertex != NULL)
	{
		//
		if(pVertex->m_nID == nVertexID)
		{
			return	pVertex;
		}

		pVertex	= pVertex->m_pNext;
	}
	
	return	NULL;
}

//////////////////////////////////////////////////////////////////////
// 07 ͨ�������ñ�,Ӧ���ǿ���ͨ�����жϳ����Ǹ������θ����
//////////////////////////////////////////////////////////////////////
CDCR_TinEdge* CDCR_TinPITACreator::GetEdgeWithVertex(int nVertexID1, int nVertexID2)
{
	CDCR_TinEdge*	pEdge;
	
	pEdge	= m_pEdgeHead;

	while(pEdge != NULL)
	{
		if((pEdge->m_nVertexID[0]==nVertexID1&&pEdge->m_nVertexID[1]==nVertexID2) || 
			(pEdge->m_nVertexID[0]==nVertexID2&&pEdge->m_nVertexID[1]==nVertexID1))
		{
			return	pEdge;	
		}

		pEdge	= pEdge->m_pNext;
	}

	return	NULL;
}

//////////////////////////////////////////////////////////////////////
// ����͹��
//////////////////////////////////////////////////////////////////////
void CDCR_TinPITACreator::DrawConvexHull(CDC* pDC)
{
	//����
	POINT	*ptPoints;
	int		iCount;
	CDCR_TinVertex	*pVertex;
	
	iCount	= 0;
	pVertex	= m_pConvexHullHead;
	while(pVertex != NULL)
	{
		iCount++;

		pVertex	= pVertex->m_pNext;
	}

	ptPoints	= new	POINT[iCount+1];
	
	int		i;

	i		= 0;
	pVertex	= m_pConvexHullHead;
	while(pVertex != NULL)
	{
		ptPoints[i].x	= (int)(pVertex->m_xx*m_dMapXA + m_dMapXB + 0.5);
		ptPoints[i].y	= (int)(pVertex->m_yy*m_dMapYA + m_dMapYB + 0.5);

		//CString	szInfo;
		//szInfo.Format("%ld",pVertex->m_nID);

		//pDC->TextOut(ptPoints[i].x,ptPoints[i].y,szInfo);

		i++;
		pVertex	= pVertex->m_pNext;
	}
	ptPoints[i]	= ptPoints[0];
	
	pDC->Polyline(ptPoints,iCount+1);
	
	delete	[]ptPoints;
}

//////////////////////////////////////////////////////////////////////
//����͹��
//////////////////////////////////////////////////////////////////////
void CDCR_TinPITACreator::CreateConvexHull(CDC* pDC)
{	
	int				iNullInsertNum;
	int				iCount;
	CDCR_TinVertex	*pVertex;

	do
	{
		iNullInsertNum	= 0;
		iCount			= 0;

		//����͹���еĶ������
		pVertex			= m_pConvexHullHead;
		while(pVertex != NULL)
		{
			iCount++;	
			pVertex	= pVertex->m_pNext;
		}
		
		//��¼͹���еĶ���ID��
		int		*iArray;
		iArray			= new	int[iCount+1];

		{
			int		i;
			i				= 0;

			pVertex			= m_pConvexHullHead;
			while(pVertex != NULL)
			{
				iArray[i]	= pVertex->m_nID;
				i++;

				pVertex	= pVertex->m_pNext;
			}
			iArray[i]	= iArray[0];
		}

		//��Զ��㴮����������ѭ��
		for(int j=0;j<iCount;j++)
		{
			//���ݶ����Ż�ȡ���
			CDCR_TinVertex	*pVertex1,*pVertex2;
			pVertex1	= GetIndexFromConvexHull(iArray[j]);
			pVertex2	= GetIndexFromConvexHull(iArray[j+1]);
			
			if(pVertex1==NULL || pVertex2==NULL)
			{
				//��˵�������⣬ǿ���˳�!
				break;
			}	
			
			//���͹���е��������һ�����Ѿ����ù����Ͳ���Ҫ�����ˣ���ʹ��Ҳ�����ù���
			if(pVertex1->m_bUsed == true)
			{
				iNullInsertNum++;
				continue;
			}

			double	dX1,dY1,dX2,dY2;
			dX1			= pVertex1->m_xx;
			dY1			= pVertex1->m_yy;

			dX2			= pVertex2->m_xx;
			dY2			= pVertex2->m_yy;
					
			//�����ж�
			bool			bFirstValue;
			double			dNewArea;
			CDCR_TinVertex	*pNewVertex;

			pNewVertex	= NULL;
			bFirstValue	= true;
			dNewArea	= -1.0;

			pVertex		= m_pVertexHead;
			while(pVertex != NULL)
			{
				if(pVertex->m_bUsed == false)
				{
					double	dNewX,dNewY;

					dNewX	= pVertex->m_xx;
					dNewY	= pVertex->m_yy;

					//���������������������Ϊ����λ���Ҳ�
					double	dTempArea;
					dTempArea	= GetArea(dX1,dY1,dNewX,dNewY,dX2,dY2);

					if(dTempArea>0.0 || fabs(dTempArea)<1e-10)
					{
						////λ���м�
						if( ((dX1-dNewX)>0.0&&(dNewX-dX2)>0.0) || ((dX1-dNewX)<0.0&&(dNewX-dX2)<0.0) || ((dY1-dNewY)>0.0&&(dNewY-dY2)>0.0) || ((dY1-dNewY)<0.0&&(dNewY-dY2)<0.0))		
						{
							if(bFirstValue)
							{
								dNewArea	= dTempArea;
								pNewVertex	= pVertex;
								bFirstValue	= false;
							}
							else
							{
								if(dNewArea < dTempArea)
								{
									dNewArea	= dTempArea;
									pNewVertex	= pVertex;
								}
							}
						}
					}
				}

				pVertex	= pVertex->m_pNext;
			}

			if(pNewVertex == NULL)
			{
				//
				iNullInsertNum++;

				pVertex1->m_bUsed	= true;
			}
			else	
			{
				//���뵽�б���
				AddVertexToConvexHull(pVertex1,pNewVertex);
				
				//
				pNewVertex->m_bUsed		= true;
				
				//����ѭ��
				break;
			}
		}

		delete	[]iArray;

	}while(iCount != iNullInsertNum);
}

//////////////////////////////////////////////////////////////////////
// ͨ�������εĶ�����ҵ�������ָ��
//////////////////////////////////////////////////////////////////////
bool CDCR_TinPITACreator::GetTriangleFromID(int iLeftTriangleID, int iRightTriangleID, CDCR_TinTriangle **pLeftTriangle, CDCR_TinTriangle **pRightTriangle)
{
	//�˳���־����ΪҪ������������
	int		iItem;
	iItem	= 0;
	
	CDCR_TinTriangle	*pTriangle;

	pTriangle	= m_pTriangleHead;
	while(pTriangle != NULL)
	{
		if(pTriangle->m_nID == iLeftTriangleID)	
		{
			*pLeftTriangle	= pTriangle;

			iItem++;
		}

		if(pTriangle->m_nID == iRightTriangleID)
		{
			*pRightTriangle	= pTriangle;

			iItem++;
		}
		
		if(iItem == 2)	return	true;
	}

	return	false;
}

//////////////////////////////////////////////////////////////////////
// TIN��Lop�ֲ��Ż�
//////////////////////////////////////////////////////////////////////
void CDCR_TinPITACreator::LopTin(CDC* pDC)
{
	//
	CDCR_TinTriangleLOP	*pTTL;

	pTTL	= m_pTriangleLOPHead;
	while(pTTL != NULL)
	{
		//��ȡһ�����ν��д���
		if(false == pTTL->m_bUsed)
		{
			CDCR_TinTriangle	*pUnTriangle;
			CDCR_TinTriangle	*pTriangle;
			
			pUnTriangle	= pTTL->m_pUnTriangle;
			pTriangle	= pTTL->m_pTriangle;
			
			if(pUnTriangle==NULL || pTriangle==NULL)
			{
				pTTL->m_bUsed	= true;
				continue;
			}

			//����ı߶���
			int		iVertexID1,iVertexID2;
			double	dX1,dY1,dZ1;
			double	dX2,dY2,dZ2;
			iVertexID1	= pTTL->m_iVertexID1;
			iVertexID2	= pTTL->m_iVertexID2;

			//��ȡ�Ѵ��������εĶ�������
			//����Ϊ1-2-3��
			int		iVertexID3;
			double	dX3,dY3,dZ3;

			pTriangle->GetVertexCoord(iVertexID1,dX1,dY1,dZ1,iVertexID2,dX2,dY2,dZ2,iVertexID3,dX3,dY3,dZ3);
			
			double	dTempX1,dTempY1,dTempZ1;
			double	dTempX2,dTempY2,dTempZ2;

			//��ȡδ���������εĶ�������
			//����Ϊ1-2-4��
			int		iVertexID4;
			double	dX4,dY4,dZ4;

			pUnTriangle->GetVertexCoord(iVertexID1,dTempX1,dTempY1,dTempZ1,iVertexID2,dTempX2,dTempY2,dTempZ2,iVertexID4,dX4,dY4,dZ4);
			
			//���ھ���Ҫ�ж�iVertexID4�Ƿ���(iVertexID1,iVertexID2,iVertexID3)��������֮��
			//����ж����������������ҵ��ĵ������Ƿ�һ��
			if((dX1==dTempX1 && dY1==dTempY1 && dZ1==dTempZ1) && (dX2==dTempX2 && dY2==dTempY2 && dZ2==dTempZ2))
			{
				//����˵��ʲô��˼���൱�ھ���1-2-3 �� 1-2-4 ����������LOP��ʵ�ڲ��оͽ���
				/*
				double	P,Q;
				P	= ( (dX1-dX3)*(dY2-dY3)-(dX2-dX3)*(dY1-dY3) ) * ( (dX1-dX4)*(dX2-dX4)+(dY1-dY4)*(dY2-dY4) );
				Q	= ( (dX1-dX3)*(dX2-dX3)+(dY1-dY3)*(dY2-dY3) ) * ( (dX1-dX4)*(dY2-dY4)-(dX2-dX4)*(dY1-dY4) );
				//f=P/Q
				//f>1  : ��Բ��
				//f==1 : ��Բ��
				//f<1  : ��Բ��
				bool	bExChange	= false;
				//����0
				if(fabs(Q) < 1e-10)
					bExChange	= true;
				else	
				{
					if(P/Q > 1)	bExChange	= true;
				}
				*/
				bool	bExChange	= false;
				double	dX,dY;
				if(IntersectLineLine(dX1,dY1,dX2,dY2,dX3,dY3,dX4,dY4,dX,dY))
				{
					//���ڽ����
					if(IsInCircle(dX1,dY1,dX2,dY2,dX3,dY3,dX4,dY4,dX,dY))
					{
						bExChange	= true;
					}
				}	

				//��Ҫ����
				//LOP����,2010-11-1�������������!!!!!!
				//��ʱû���⣬����Ч���е��2010-11-2
				if(bExChange)
				{
					//��������������
					
					CDCR_TinTriangle	*pNewTriangle1,*pNewTriangle2;
					//��һ����������1-3-4
					{
						pNewTriangle1					= new	CDCR_TinTriangle;
						pNewTriangle1->m_nID			= m_iTriangleCount+1;
						pNewTriangle1->m_nVertexID[0]	= iVertexID1;
						pNewTriangle1->m_nVertexID[1]	= iVertexID3;
						pNewTriangle1->m_nVertexID[2]	= iVertexID4;
						pNewTriangle1->SetVertexCoord(iVertexID1,dX1,dY1,dZ1,iVertexID3,dX3,dY3,dZ3,iVertexID4,dX4,dY4,dZ4);
						AddTriangle(pNewTriangle1);
						m_iTriangleCount++;
					}

					//�ڶ�����������2-3-4
					{
						pNewTriangle2					= new	CDCR_TinTriangle;
						pNewTriangle2->m_nID			= m_iTriangleCount+1;
						pNewTriangle2->m_nVertexID[0]	= iVertexID2;
						pNewTriangle2->m_nVertexID[1]	= iVertexID3;
						pNewTriangle2->m_nVertexID[2]	= iVertexID4;
						pNewTriangle2->SetVertexCoord(iVertexID2,dX2,dY2,dZ2,iVertexID3,dX3,dY3,dZ3,iVertexID4,dX4,dY4,dZ4);
						AddTriangle(pNewTriangle2);
						m_iTriangleCount++;
					}

					//����һ���±�
					CDCR_TinEdge	*pNewEdge1;
					//�±�3-4
					{
						pNewEdge1						= new	CDCR_TinEdge;
						pNewEdge1->m_nID				= m_iEdgeCount+1;
						pNewEdge1->m_nVertexID[0]		= iVertexID3;
						pNewEdge1->m_nVertexID[1]		= iVertexID4;
						pNewEdge1->m_nTriangleID[0]		= pNewTriangle1->m_nID;
						pNewEdge1->m_nTriangleID[1]		= pNewTriangle2->m_nID;
						pNewEdge1->m_pLeftTriangle		= pNewTriangle1;
						pNewEdge1->m_pRightTriangle		= pNewTriangle2;

						m_iEdgeCount++;
						
						AddEdge(pNewEdge1);

						//
						AddEdgeToCells((dX3+dX4)/2,(dY3+dY4)/2,pNewEdge1);

						//ʵʱ����
						DrawEdge(pDC,pNewEdge1);
					}
					
					//ɾ��1-2��
					CDCR_TinEdge		*pEdge;
					pEdge	= GetEdgeFromCells((dX1+dX2)/2,(dY1+dY2)/2,iVertexID1,iVertexID2);
					if(pEdge != NULL)
						pEdge->m_bDelete	= true;

					//���������ɱ�
					CDCR_TinEdge		*pOldEdge;
					
					//��һ���ɱ� 1-3,������������LOP����������
					{
						//
						pOldEdge	= GetEdgeFromCells((dX1+dX3)/2,(dY1+dY3)/2,iVertexID1,iVertexID3);
						
						if(pOldEdge)
						{
							//
							pOldEdge->UpdateTriangle(pTriangle,pNewTriangle1);
						}
					}

					//�ڶ����ɱ� 1-4,������������LOP����������
					{
						//
						pOldEdge	= GetEdgeFromCells((dX1+dX4)/2,(dY1+dY4)/2,iVertexID1,iVertexID4);
						
						if(pOldEdge)
						{
							//
							pOldEdge->UpdateTriangle(pUnTriangle,pNewTriangle1);
							
							//�������Ϊ���棬�������
							if(pOldEdge->m_pLeftTriangle == pNewTriangle1)
							{	
								//pOldEdge->m_pRightTriangle�Ǵ�������
								if(pOldEdge->m_pRightTriangle != NULL)
								{
									//����LOP������
									AddTriangleToLOP(pOldEdge->m_pRightTriangle,pNewTriangle1,iVertexID1,iVertexID4);
								}
							}
							else	
							if(pOldEdge->m_pRightTriangle == pNewTriangle1)
							{
								if(pOldEdge->m_pLeftTriangle != NULL)
								{
									//����LOP������
									AddTriangleToLOP(pOldEdge->m_pLeftTriangle,pNewTriangle1,iVertexID1,iVertexID4);
								}
							}
						}
					}

					//�������ɱ� 2-3,������������LOP����������
					{
						//
						pOldEdge	= GetEdgeFromCells((dX2+dX3)/2,(dY2+dY3)/2,iVertexID2,iVertexID3);
						
						if(pOldEdge)
						{
							//
							pOldEdge->UpdateTriangle(pTriangle,pNewTriangle2);
						}
					}

					//�������ɱ� 2-4,������������LOP����������
					{
						//
						pOldEdge	= GetEdgeFromCells((dX2+dX4)/2,(dY2+dY4)/2,iVertexID2,iVertexID4);
						
						if(pOldEdge)
						{
							//
							pOldEdge->UpdateTriangle(pUnTriangle,pNewTriangle2);

							//�������Ϊ���棬�������
							if(pOldEdge->m_pLeftTriangle == pNewTriangle2)
							{	
								//pOldEdge->m_pRightTriangle�Ǵ�������
								if(pOldEdge->m_pRightTriangle != NULL)
								{
									//����LOP������
									AddTriangleToLOP(pOldEdge->m_pRightTriangle,pNewTriangle2,iVertexID2,iVertexID4);
								}
							}
							else	
							if(pOldEdge->m_pRightTriangle == pNewTriangle2)
							{
								if(pOldEdge->m_pLeftTriangle != NULL)
								{
									//����LOP������
									AddTriangleToLOP(pOldEdge->m_pLeftTriangle,pNewTriangle2,iVertexID2,iVertexID4);
								}
							}
						}
					}

					//ɾ���ɵ�������
					pUnTriangle->m_bDelete	= true;
					pTriangle->m_bDelete	= true;
				}
			}//if((dX1==dTempX1 && dY1==dTempY1 && dZ1==dTempZ1) && (dX2==dTempX2 && dY2==dTempY2 && dZ2==dTempZ2))
				
			pTTL->m_bUsed		= true;
		}//if(false == pTED->m_bUsed)
	
		pTTL	= pTTL->m_pNext;
	}
}

//////////////////////////////////////////////////////////////////////
//����������
//////////////////////////////////////////////////////////////////////
void CDCR_TinPITACreator::InsertOrtherVertex(CDC *pDC)
{
	int		iTotal	= 0;

	CDCR_TinVertex	*pVertex;

	pVertex	= m_pVertexHead;
	while(pVertex != NULL)
	{
		if(pVertex->m_bUsed == false)
		{
			//�����ҵ��ö������ڵ�������
			int					iPVertexID;
			double				dPX,dPY,dPZ;

			iPVertexID	= pVertex->m_nID;
			dPX			= pVertex->m_xx;
			dPY			= pVertex->m_yy;
			dPZ			= pVertex->m_zz;

			//���ݶ����ȡ�ö������ڵ�������
			CDCR_TinTriangle	*pTriangle;
			pTriangle		= GetTriangleFromPoint(dPX,dPY);

			if(pTriangle != NULL)
			{
				int		iVertexID1,iVertexID2,iVertexID3;
				double	dX1,dY1,dZ1;
				double	dX2,dY2,dZ2;
				double	dX3,dY3,dZ3;
				
				//��ȡ��������εĶ���������Ϣ
				pTriangle->GetVertexCoordEx(iVertexID1,dX1,dY1,dZ1,iVertexID2,dX2,dY2,dZ2,iVertexID3,dX3,dY3,dZ3);
				
				//����������������
				CDCR_TinTriangle	*pNewTriangle1,*pNewTriangle2,*pNewTriangle3;

				//��һ��������1-2-P
				{
					pNewTriangle1					= new	CDCR_TinTriangle;
					pNewTriangle1->m_nID			= m_iTriangleCount+1;
					pNewTriangle1->m_nVertexID[0]	= iVertexID1;
					pNewTriangle1->m_nVertexID[1]	= iVertexID2;
					pNewTriangle1->m_nVertexID[2]	= iPVertexID;
					pNewTriangle1->SetVertexCoord(iVertexID1,dX1,dY1,dZ1,iVertexID2,dX2,dY2,dZ2,iPVertexID,dPX,dPY,dPZ);
					AddTriangle(pNewTriangle1);
					m_iTriangleCount++;
				}

				//�ڶ���������2-3-P
				{
					pNewTriangle2					= new	CDCR_TinTriangle;
					pNewTriangle2->m_nID			= m_iTriangleCount+1;
					pNewTriangle2->m_nVertexID[0]	= iVertexID2;
					pNewTriangle2->m_nVertexID[1]	= iVertexID3;
					pNewTriangle2->m_nVertexID[2]	= iPVertexID;
					pNewTriangle2->SetVertexCoord(iVertexID2,dX2,dY2,dZ2,iVertexID3,dX3,dY3,dZ3,iPVertexID,dPX,dPY,dPZ);
					AddTriangle(pNewTriangle2);
					m_iTriangleCount++;
				}
				//������������3-1-P
				{
					pNewTriangle3					= new	CDCR_TinTriangle;
					pNewTriangle3->m_nID			= m_iTriangleCount+1;
					pNewTriangle3->m_nVertexID[0]	= iVertexID3;
					pNewTriangle3->m_nVertexID[1]	= iVertexID1;
					pNewTriangle3->m_nVertexID[2]	= iPVertexID;
					pNewTriangle3->SetVertexCoord(iVertexID3,dX3,dY3,dZ3,iVertexID1,dX1,dY1,dZ1,iPVertexID,dPX,dPY,dPZ);
					AddTriangle(pNewTriangle3);
					m_iTriangleCount++;
				}
				
				//���������±�
				CDCR_TinEdge		*pEdge;

				//��һ����1-P
				{
					pEdge						= new	CDCR_TinEdge;
					pEdge->m_nID				= m_iEdgeCount+1;
					pEdge->m_nVertexID[0]		= iVertexID1;
					pEdge->m_nVertexID[1]		= iPVertexID;
					pEdge->m_nTriangleID[0]		= pNewTriangle1->m_nID;
					pEdge->m_nTriangleID[1]		= pNewTriangle3->m_nID;

					pEdge->m_pLeftTriangle		= pNewTriangle1;
					pEdge->m_pRightTriangle		= pNewTriangle3;

					m_iEdgeCount++;
					
					AddEdge(pEdge);

					//
					AddEdgeToCells((dX1+dPX)/2,(dY1+dPY)/2,pEdge);

					//ʵʱ����
					DrawEdge(pDC,pEdge);
				}
				
				//�ڶ�����2-P
				{
					pEdge						= new	CDCR_TinEdge;
					pEdge->m_nID				= m_iEdgeCount+1;
					pEdge->m_nVertexID[0]		= iVertexID2;
					pEdge->m_nVertexID[1]		= iPVertexID;
					pEdge->m_nTriangleID[0]		= pNewTriangle1->m_nID;
					pEdge->m_nTriangleID[1]		= pNewTriangle2->m_nID;

					pEdge->m_pLeftTriangle		= pNewTriangle1;
					pEdge->m_pRightTriangle		= pNewTriangle2;

					m_iEdgeCount++;
					
					AddEdge(pEdge);

					//
					AddEdgeToCells((dX2+dPX)/2,(dY2+dPY)/2,pEdge);

					//ʵʱ����
					DrawEdge(pDC,pEdge);
				}

				//��������3-P
				{
					pEdge						= new	CDCR_TinEdge;
					pEdge->m_nID				= m_iEdgeCount+1;
					pEdge->m_nVertexID[0]		= iVertexID3;
					pEdge->m_nVertexID[1]		= iPVertexID;
					pEdge->m_nTriangleID[0]		= pNewTriangle2->m_nID;
					pEdge->m_nTriangleID[1]		= pNewTriangle3->m_nID;

					pEdge->m_pLeftTriangle		= pNewTriangle2;
					pEdge->m_pRightTriangle		= pNewTriangle3;

					m_iEdgeCount++;
					
					AddEdge(pEdge);

					//
					AddEdgeToCells((dX3+dPX)/2,(dY3+dPY)/2,pEdge);

					//ʵʱ����
					DrawEdge(pDC,pEdge);
				}

				//���������ɱ�
				CDCR_TinEdge		*pOldEdge;

				//���
				ReleaseTriangleLOP();
				
				//��һ���ɱ� 1-2
				{
					//
					pOldEdge	= GetEdgeFromCells((dX1+dX2)/2,(dY1+dY2)/2,iVertexID1,iVertexID2);
					
					//
					pOldEdge->UpdateTriangle(pTriangle,pNewTriangle1);
					
					//�������Ϊ���棬�������
					if(pOldEdge->m_pLeftTriangle == pNewTriangle1)
					{	
						//pOldEdge->m_pRightTriangle�Ǵ�������
						if(pOldEdge->m_pRightTriangle != NULL)
						{
							//����LOP������
							AddTriangleToLOP(pOldEdge->m_pRightTriangle,pNewTriangle1,iVertexID1,iVertexID2);
						}
					}
					else	
					if(pOldEdge->m_pRightTriangle == pNewTriangle1)
					{
						if(pOldEdge->m_pLeftTriangle != NULL)
						{
							//����LOP������
							AddTriangleToLOP(pOldEdge->m_pLeftTriangle,pNewTriangle1,iVertexID1,iVertexID2);
						}
					}
				}

				//�ڶ����ɱ� 2-3
				{
					//
					pOldEdge	= GetEdgeFromCells((dX2+dX3)/2,(dY2+dY3)/2,iVertexID2,iVertexID3);
					
					//
					pOldEdge->UpdateTriangle(pTriangle,pNewTriangle2);

					//�������Ϊ���棬�������
					if(pOldEdge->m_pLeftTriangle == pNewTriangle2)
					{
						if(pOldEdge->m_pRightTriangle != NULL)
						{
							//����LOP������
							AddTriangleToLOP(pOldEdge->m_pRightTriangle,pNewTriangle2,iVertexID2,iVertexID3);
						}
					}
					else	
					if(pOldEdge->m_pRightTriangle == pNewTriangle2)
					{
						if(pOldEdge->m_pLeftTriangle != NULL)
						{
							//����LOP������
							AddTriangleToLOP(pOldEdge->m_pLeftTriangle,pNewTriangle2,iVertexID2,iVertexID3);
						}
					}
				}
						
				//�������ɱ� 3-1
				{
					//
					pOldEdge	= GetEdgeFromCells((dX3+dX1)/2,(dY3+dY1)/2,iVertexID3,iVertexID1);
					
					//
					pOldEdge->UpdateTriangle(pTriangle,pNewTriangle3);

					//�������Ϊ���棬�������
					if(pOldEdge->m_pLeftTriangle == pNewTriangle3)
					{
						if(pOldEdge->m_pRightTriangle != NULL)
						{
							//����LOP������
							AddTriangleToLOP(pOldEdge->m_pRightTriangle,pNewTriangle3,iVertexID3,iVertexID1);
						}
					}
					else	
					if(pOldEdge->m_pRightTriangle == pNewTriangle3)
					{
						if(pOldEdge->m_pLeftTriangle != NULL)
						{
							//����LOP������
							AddTriangleToLOP(pOldEdge->m_pLeftTriangle,pNewTriangle3,iVertexID3,iVertexID1);
						}
					}
				}

				//ɾ����������
				pTriangle->m_bDelete	= true;
			}//if(pTriangle != NULL)


			LopTin(pDC);

			iTotal++;
			//�Ż�
			//if(iTotal == 1)
				//TRACE("ok");
				//break;

			pVertex->m_bUsed	= true;
		}
		
		pVertex	= pVertex->m_pNext;
	}
}

//////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////
CDCR_TinEdge* CDCR_TinPITACreator::GetEdgeFromCells(double dX, double dY, int iVertexID1, int iVertexID2)
{
	//�ҵ�dX��dY���ڵĸ���
	int		I,J,iC;
	I	= (int)((dX - m_dCellLeft) / m_dCellSizeOfX);
	J	= (int)((dY - m_dCellBottom) / m_dCellSizeOfY);

	if(I < 0)				I	= 0;
	if(J < 0)				J	= 0;
	if(I >= m_nCellNumOfX)	I	= m_nCellNumOfX-1;
	if(J >= m_nCellNumOfY)	J	= m_nCellNumOfY-1;
	
	//
	iC	= J * m_nCellNumOfX + I;

	//
	CDCR_TinEdgeAddress	*pTED;

	pTED	= m_pCells[iC].m_pTEDHead;
	while(pTED != NULL)
	{
		if(false == pTED->m_pEdge->m_bDelete)
		{
			if((pTED->m_pEdge->m_nVertexID[0]==iVertexID1&&pTED->m_pEdge->m_nVertexID[1]==iVertexID2) || (pTED->m_pEdge->m_nVertexID[0]==iVertexID2&&pTED->m_pEdge->m_nVertexID[1]==iVertexID1))
				return	pTED->m_pEdge;
		}

		pTED	= pTED->m_pNext;
	}
	
	return	NULL;
}

//////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////
void CDCR_TinPITACreator::AddTriangle(CDCR_TinTriangle *pTriangle)
{
	if(m_pTriangleHead == NULL)
	{
		m_pTriangleHead	= pTriangle;
		m_pTriangleLast	= pTriangle;
	}
	else
	{
		m_pTriangleLast->m_pNext	= pTriangle;
		m_pTriangleLast				= pTriangle;
	}

	m_iTriangleCount++;
}

//////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////
void CDCR_TinPITACreator::AddEdge(CDCR_TinEdge *pEdge)
{
	if(m_pEdgeHead == NULL)
	{
		m_pEdgeHead	= pEdge;
		m_pEdgeLast	= pEdge;
	}
	else
	{
		m_pEdgeLast->m_pNext	= pEdge;
		m_pEdgeLast				= pEdge;
	}

	m_iEdgeCount++;
}

//////////////////////////////////////////////////////////////////////
// �����һ��
//////////////////////////////////////////////////////////////////////
void CDCR_TinPITACreator::InsertFirstVertex(CDC *pDC)
{
	CDCR_TinVertex	*pVertex;

	double	dMinX,dMaxX,dMinY,dMaxY;
	
	//���ҵ㼯�ķ�Χ
	pVertex	= m_pVertexHead;
	while(pVertex != NULL)
	{
		if(pVertex == m_pVertexHead)
		{
			dMinX	= dMaxX	= pVertex->m_xx;
			dMinY	= dMaxY	= pVertex->m_yy;
		}
		else
		{
			if(dMinX > pVertex->m_xx)	dMinX	= pVertex->m_xx;
			if(dMaxX < pVertex->m_xx)	dMaxX	= pVertex->m_xx;
			if(dMinY > pVertex->m_yy)	dMinY	= pVertex->m_yy;
			if(dMaxY < pVertex->m_yy)	dMaxY	= pVertex->m_yy;
		}
		
		pVertex	= pVertex->m_pNext;
	}
	
	//�ҵ����ĵ�
	double	dX,dY;
	dX	= (dMinX+dMaxX)/2;
	dY	= (dMinY+dMaxY)/2;

	//������dX,dY����ĵ�
	//ȷ��ѭ������������m_nCellNumOfX��m_nCellNumOfY�Ĵ���
	int	iMaxCount;

	iMaxCount	= m_nCellNumOfX;
	if(iMaxCount < m_nCellNumOfY)	iMaxCount	= m_nCellNumOfY;
	
	//�ҵ�dX��dY���ڵĸ���
	int		I,J;
	I	= (int)((dX - m_dCellLeft) / m_dCellSizeOfX);
	J	= (int)((dY - m_dCellBottom) / m_dCellSizeOfY);
	
	CDCR_TinVertex	*pNewVertex;
	pNewVertex	= NULL;

	bool			bFirstValue;
	double			dMinDistance;

	bFirstValue	= true;
	dMinDistance= 0.0;

	//ѭ���Ĵ���
	for(int k=1;k<iMaxCount;k++)
	{
		int		iBegin,iEnd;
		int		jBegin,jEnd;
		
		iBegin	= I-k;	iEnd	= I+k;
		jBegin	= J-k;	jEnd	= J+k;
		

		int		iC;
		int		iI,iJ;
		for(int j=jBegin;j<=jEnd;j++)
		{
			for(int i=iBegin;i<=iEnd;i++)
			{
				iI	= i;	iJ	= j;

				if(iI < 0)				iI	= 0;
				if(iJ < 0)				iJ	= 0;
				if(iI >= m_nCellNumOfX)	iI	= m_nCellNumOfX-1;
				if(iJ >= m_nCellNumOfY)	iJ	= m_nCellNumOfY-1;
				
				//
				iC	= iJ * m_nCellNumOfX + iI;

				//
				if(m_pCells[iC].m_bUsed == false)
				{
					CDCR_TinVertexAddress	*pTVD;
					
					pTVD	= m_pCells[iC].m_pTVDHead;
					while(pTVD != NULL)
					{			
						if(pTVD->m_pVertex->m_bUsed == false)
						{
							//�����ж�
							double	dTempValue;
							double	dTempXX,dTempYY,dTempZZ;

							dTempXX		= pTVD->m_pVertex->m_xx;
							dTempYY		= pTVD->m_pVertex->m_yy;
							dTempZZ		= pTVD->m_pVertex->m_zz;

							dTempValue	= GetDistance(dX,dY,0,dTempXX,dTempYY,0);

							if(bFirstValue)
							{
								dMinDistance= dTempValue;
								pNewVertex	= pTVD->m_pVertex;

								bFirstValue	= false;
							}
							else
							{
								if(dMinDistance > dTempValue)
								{
									dMinDistance= dTempValue;
									pNewVertex	= pTVD->m_pVertex;
								}
							}
						}
						
						pTVD	= pTVD->m_pNext;
					}
				}

				//�����
				m_pCells[iC].m_bUsed	= true;
			}
		}

		//�ж�������
		if(pNewVertex != NULL)
		{	
			break;
		}
	}
		
	//�ҵ���
	if(pNewVertex != NULL)
	{
		//ʹ����
		pNewVertex->m_bUsed	= true;

		int		iCount;
		iCount		= 0;
		pVertex		= m_pConvexHullHead;
		while(pVertex != NULL)
		{
			iCount++;
			pVertex	= pVertex->m_pNext;
		}

		//��¼͹���еĶ���ID��
		int		*iArray;
		iArray			= new	int[iCount+1];

		{
			int		i;
			i				= 0;

			pVertex			= m_pConvexHullHead;
			while(pVertex != NULL)
			{
				iArray[i]	= pVertex->m_nID;
				i++;

				pVertex	= pVertex->m_pNext;
			}
			iArray[i]	= iArray[0];
		}

		//
		int		iNewVertexID;
		double	dNewX,dNewY,dNewZ;
		iNewVertexID	= pNewVertex->m_nID;
		dNewX			= pNewVertex->m_xx;
		dNewY			= pNewVertex->m_yy;
		dNewZ			= pNewVertex->m_zz;

		//��Զ��㴮����������ѭ��
		for(int j=0;j<iCount;j++)
		{
			//���ݶ����Ż�ȡ���
			CDCR_TinVertex	*pVertex1,*pVertex2;
			pVertex1	= GetIndexFromVertex(iArray[j]);
			pVertex2	= GetIndexFromVertex(iArray[j+1]);

			if(pVertex1==NULL || pVertex2==NULL)
			{
				//��˵�������⣬ǿ���˳�!
				break;
			}
			
			int		iVertexID1,iVertexID2;
			double	dX1,dY1,dZ1,dX2,dY2,dZ2;
			iVertexID1	= pVertex1->m_nID;
			dX1			= pVertex1->m_xx;
			dY1			= pVertex1->m_yy;
			dZ1			= pVertex1->m_zz;

			iVertexID2	= pVertex2->m_nID;
			dX2			= pVertex2->m_xx;
			dY2			= pVertex2->m_yy;
			dZ2			= pVertex2->m_zz;

			//����������
			CDCR_TinTriangle	*pTriangle;
			pTriangle	= new	CDCR_TinTriangle;
			pTriangle->m_nID	= m_iTriangleCount+1;
			pTriangle->m_nVertexID[0]	= iNewVertexID;
			pTriangle->m_nVertexID[1]	= iVertexID1;
			pTriangle->m_nVertexID[2]	= iVertexID2;
			pTriangle->SetVertexCoord(iNewVertexID,dNewX,dNewY,dNewZ,iVertexID1,dX1,dY1,dZ1,iVertexID2,dX2,dY2,dZ2);
			AddTriangle(pTriangle);

			CDCR_TinEdge		*pEdge;

			//���ӵ�һ����
			pEdge		= GetEdgeFromCells((dNewX+dX1)/2,(dNewY+dY1)/2,iNewVertexID,iVertexID1);
			if(pEdge != NULL)
			{
				//������е����⣬2010-11-1
				pEdge->m_nTriangleID[1]		= pTriangle->m_nID;
				pEdge->m_pRightTriangle		= pTriangle;
			}
			else
			{
				//��һ����
				pEdge						= new	CDCR_TinEdge;
				pEdge->m_nID				= m_iEdgeCount+1;
				pEdge->m_nVertexID[0]		= iVertexID1;
				pEdge->m_nVertexID[1]		= iNewVertexID;
				pEdge->m_nTriangleID[0]		= pTriangle->m_nID;
				pEdge->m_nTriangleID[1]		= 0;
				//
				pEdge->m_pLeftTriangle		= pTriangle;
				
				AddEdge(pEdge);
				
				AddEdgeToCells((dNewX+dX1)/2,(dNewY+dY1)/2,pEdge);
				
				//
				m_iEdgeCount++;

				//ʵʱ����
				DrawEdge(pDC,pEdge);
			}

			//���ӵڶ�����
			pEdge		= GetEdgeFromCells((dX1+dX2)/2,(dY1+dY2)/2,iVertexID2,iVertexID2);
			if(pEdge != NULL)
			{
				pEdge->m_nTriangleID[1]		= pTriangle->m_nID;
				pEdge->m_pRightTriangle		= pTriangle;
			}
			else
			{
				//��һ����
				pEdge						= new	CDCR_TinEdge;
				pEdge->m_nID				= m_iEdgeCount+1;
				pEdge->m_nVertexID[0]		= iVertexID1;
				pEdge->m_nVertexID[1]		= iVertexID2;
				pEdge->m_nTriangleID[0]		= pTriangle->m_nID;
				pEdge->m_nTriangleID[1]		= 0;

				pEdge->m_pLeftTriangle		= pTriangle;
				
				AddEdge(pEdge);
				
				AddEdgeToCells((dX1+dX2)/2,(dY1+dY2)/2,pEdge);

				//
				m_iEdgeCount++;

				//ʵʱ����
				DrawEdge(pDC,pEdge);
			}

			//���ӵ�������
			pEdge		= GetEdgeFromCells((dX2+dNewX)/2,(dY2+dNewY)/2,iVertexID2,iNewVertexID);
			if(pEdge != NULL)
			{
				pEdge->m_nTriangleID[1]		= pTriangle->m_nID;

				pEdge->m_pRightTriangle		= pTriangle;
			}
			else
			{
				//��һ����
				pEdge						= new	CDCR_TinEdge;
				pEdge->m_nID				= m_iEdgeCount+1;
				pEdge->m_nVertexID[0]		= iVertexID2;
				pEdge->m_nVertexID[1]		= iNewVertexID;
				pEdge->m_nTriangleID[0]		= pTriangle->m_nID;
				pEdge->m_nTriangleID[1]		= 0;

				pEdge->m_pLeftTriangle		= pTriangle;
				
				AddEdge(pEdge);
				
				AddEdgeToCells((dX2+dNewX)/2,(dY2+dNewY)/2,pEdge);
				
				//
				m_iEdgeCount++;

				//ʵʱ����
				DrawEdge(pDC,pEdge);
			}

			pVertex1->m_bUsed	= true;
			pVertex2->m_bUsed	= true;
		}

		delete	[]iArray;
	}
}

//���ݶ���������ȡ���ö������ڵ�������
CDCR_TinTriangle* CDCR_TinPITACreator::GetTriangleFromPoint(double dX, double dY)
{
	//
	CDCR_TinTriangle	*pTriangle;

	pTriangle		= m_pTriangleHead;
	while(pTriangle != NULL)
	{
		if(false == pTriangle->m_bDelete)
		{
			//��ĳһ�������ڲ�
			if(pTriangle->IsInTriangle(dX,dY))
			{
				return	pTriangle;
			}
		}

		pTriangle	= pTriangle->m_pNext;
	}
	
	return	NULL;
}

//���ߵ�ַ�б������ӱ�
void CDCR_TinPITACreator::AddTriangleToLOP(CDCR_TinTriangle *pUnTriangle,CDCR_TinTriangle *pTriangle,int iVertexID1,int iVertexID2)
{
	CDCR_TinTriangleLOP	*pTTL;

	pTTL	= m_pTriangleLOPHead;
	while(pTTL != NULL)
	{
		if(pTTL->m_pTriangle == pTriangle)
			return;

		pTTL	= pTTL->m_pNext;
	}

	pTTL	= new	CDCR_TinTriangleLOP;
	pTTL->m_pUnTriangle	= pUnTriangle;
	pTTL->m_pTriangle	= pTriangle;
	pTTL->m_iVertexID1	= iVertexID1;
	pTTL->m_iVertexID2	= iVertexID2;

	if(m_pTriangleLOPHead == NULL)
	{
		m_pTriangleLOPHead	= pTTL;
		m_pTriangleLOPLast	= pTTL;
	}
	else
	{
		m_pTriangleLOPLast->m_pNext	= pTTL;
		m_pTriangleLOPLast			= pTTL;
	}
}

//
void CDCR_TinPITACreator::ReleaseTriangleLOP()
{
	CDCR_TinTriangleLOP	*pTTL1,*pTTL2;

	if(m_pTriangleLOPHead != NULL)
	{
		pTTL1	= pTTL2	= m_pTriangleLOPHead;
		do
		{
			pTTL1		= pTTL1->m_pNext;
			delete	pTTL2;
			pTTL2		= pTTL1;
		}while(pTTL1 != NULL);

		m_pTriangleLOPHead	= NULL;
		m_pTriangleLOPLast	= NULL;
	}
}

//ֱ�߶��Ƿ񽻲棬�������
bool CDCR_TinPITACreator::IntersectLineLine(double dX1, double dY1, double dX2, double dY2, double dX3, double dY3, double dX4, double dY4, double &dX, double &dY)
{
	double	a1	= dY2-dY1;
	double	b1	= dX1-dX2;

	double	c1	= dX1*dY2 - dX2*dY1;

	double	a2	= dY4-dY3;
	double	b2	= dX3-dX4;

	double	c2	= dX3*dY4 - dX4*dY3;
	
	double	detab	= a1*b2 - a2*b1;
	
	//
	if(fabs(detab) < 1e-10)
	{
		return	false;
	}
	
	//
	dX	= (c1*b2-c2*b1)/detab;
	dY	= (a1*c2-a2*c1)/detab;

	bool	bLineOne	= false;
	//�ж��Ƿ����ӳ�����
	if(fabs(dX2-dX1) > fabs(dY2-dY1))	//�������
	{
		if(dX2>dX1)	
		{
			if(dX2>dX && dX>dX1)	bLineOne	= true;
		}
		else
		{
			if(dX2<dX && dX<dX1)	bLineOne	= true;
		}
	}
	else
	{
		if(dY2>dY1)	
		{
			if(dY2>dY && dY>dY1)	bLineOne	= true;
		}
		else
		{
			if(dY2<dY && dY<dY1)	bLineOne	= true;
		}
	}

	bool	bLineTwo	= false;
	//�ж��Ƿ����ӳ�����
	if(fabs(dX4-dX3) > fabs(dY4-dY3))	//�������
	{
		if(dX4>dX3)	
		{
			if(dX4>dX && dX>dX3)	bLineTwo	= true;
		}
		else
		{
			if(dX4<dX && dX<dX3)	bLineTwo	= true;
		}
	}
	else
	{
		if(dY4>dY3)	
		{
			if(dY4>dY && dY>dY3)	bLineTwo	= true;
		}
		else
		{
			if(dY4<dY && dY<dY3)	bLineTwo	= true;
		}
	}
	
	if(bLineOne && bLineTwo)	return	true;

	return	false;
}

//
//(dX1,dY1)\(dX2,dY2)\(dX3,dY3)
//(dXP,dYP)
//�����(dXD,dYD)
bool CDCR_TinPITACreator::IsInCircle(double dX1, double dY1, double dX2, double dY2, double dX3, double dY3, double dXP, double dYP, double dXD, double dYD)
{
	double	dPD;
	dPD	= (dXP-dXD)*(dXP-dXD) + (dYP-dYD)*(dYP-dYD);

	double	dD3;
	dD3	= (dXD-dX3)*(dXD-dX3) + (dYD-dY3)*(dYD-dY3);

	double	dD1;
	dD1	= (dXD-dX1)*(dXD-dX1) + (dYD-dY1)*(dYD-dY1);

	double	dD2;
	dD2	= (dXD-dX2)*(dXD-dX2) + (dYD-dY2)*(dYD-dY2);

	//��Բ�ڣ���Ҫ����
	if(dPD*dD3 < dD1*dD2)	return	true;
	
	return	false;
}
