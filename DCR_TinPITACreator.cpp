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
	//顶点列表
	m_pVertexHead		= NULL;
	m_pVertexLast		= NULL;
	m_iVertexCount		= 0;

	//凸包列表
	m_pConvexHullHead	= NULL;
	m_pConvexHullLast	= NULL;

	//边列表
	m_pEdgeHead			= NULL;
	m_pEdgeLast			= NULL;
	m_iEdgeCount		= 0;

	//三角形列表
	m_pTriangleHead		= NULL;
	m_pTriangleLast		= NULL;
	m_iTriangleCount	= 0;
	
	//
	m_dMapXA			= 1.0;
	m_dMapXB			= 0.0;
	m_dMapYA			= -1.0;
	m_dMapYB			= 0.0;

	//地图区域
	m_dMapLeft		= 0.0;
	m_dMapRight		= 0.0;
	m_dMapBottom	= 0.0;
	m_dMapTop		= 0.0;

	//地图格网大小
	m_dCellSizeOfY	= 0.0;
	m_dCellSizeOfX	= 0.0;
	
	//地图分块数
	m_nCellNumOfY	= 15;
	m_nCellNumOfX	= 15;

	//划分网格范围
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
// 01 通过顶点ID获取顶点坐标
// 返回值：bool，true表示成功，false表示失败
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
// 释放边
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
// 释放三角形
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
// 释放顶点
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
// 释放凸包的顶点
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
// 001 释放TIN数据及采样点数据
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
// 002 增加顶点坐标
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
// 003 设置显示参数
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
// 往格网中增加顶点
//////////////////////////////////////////////////////////////////////
bool CDCR_TinPITACreator::AddVertexToCells(CDCR_TinVertex *pVertex)
{
	if(pVertex == NULL)				return	false;

	int							ci;	
	int							xi,yi;
	CDCR_TinVertexAddress		*tvd;
		
	//仅仅考虑一个点
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
// 将边增加到分区格网中
//////////////////////////////////////////////////////////////////////
bool CDCR_TinPITACreator::AddEdgeToCells(double dX,double dY,CDCR_TinEdge *pEdge)
{
	if(pEdge == NULL)				return	false;

	int							ci;	
	int							xi,yi;
	CDCR_TinEdgeAddress			*pTED;
		
	//仅仅考虑一个点
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
// 格网分区
//////////////////////////////////////////////////////////////////////
void CDCR_TinPITACreator::CellBuild()
{
	int i,j,ci;

	//初始化
	if(m_pCells != NULL)
	{
		delete	[]m_pCells;
		m_pCells	= NULL;
	}
	
	//重新计算网格尺寸
	double	dSize;
	dSize	= sqrt( (m_dMapRight-m_dMapLeft)*(m_dMapTop-m_dMapBottom)/m_iVertexCount );
	m_dCellSizeOfX	= m_dCellSizeOfY	= 4*dSize/3;

	//先计算格网尺寸
	m_nCellNumOfX	= (int)((m_dMapRight-m_dMapLeft)/m_dCellSizeOfX);
	m_nCellNumOfY	= (int)((m_dMapTop-m_dMapBottom)/m_dCellSizeOfY);

	//然后外扩范围，重新划分网格划分网格范围
	m_dCellLeft		= m_dMapLeft - m_dCellSizeOfX/2;
	m_dCellRight	= m_dMapRight + m_dCellSizeOfX/2;
	m_dCellBottom	= m_dMapBottom - m_dCellSizeOfY/2;
	m_dCellTop		= m_dMapTop + m_dCellSizeOfY/2;

	//重新计算网格尺寸
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
		//建立网格
		AddVertexToCells(pVertex);

		pVertex	= pVertex->m_pNext;
	}
}

//////////////////////////////////////////////////////////////////////
// 004 三角网生长算法生成TIN
//////////////////////////////////////////////////////////////////////
bool CDCR_TinPITACreator::Create(CDC *pDC)
{
	//先释放,边和三角形
	{
		//
		ReleaseConverHull();
		
		//
		ReleaseEdge();

		//
		ReleaseTriangle();
	}

	//创建格网
	CellBuild();

	//获取四个角点
	GetMaxMinVextex();

	//创建凸包
	CreateConvexHull(pDC);

	//插入第一点
	InsertFirstVertex(pDC);
	
	//插入其他点
	InsertOrtherVertex(pDC);

	return	true;	
}

//////////////////////////////////////////////////////////////////////
// 005 绘制边
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
// 005 绘制三角形
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
// 006 绘制TIN
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
// 02 获取角度
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
// 02 获取面积
//////////////////////////////////////////////////////////////////////
double CDCR_TinPITACreator::GetArea(double x1, double y1, double x2, double y2, double x3, double y3)
{
	double m_dArea;
	
	//m_dArea	= ((x2-x1)*(y3-y1)-(x3-x1)*(y2-y1))/2;
	m_dArea	= ((x2-x1)*(y3-y1)-(x3-x1)*(y2-y1));

	return	m_dArea;
}

//////////////////////////////////////////////////////////////////////
// 01 获取两点间的距离
//////////////////////////////////////////////////////////////////////
double CDCR_TinPITACreator::GetDistance(double x1, double y1, double z1, double x2, double y2, double z2)
{
	double	dDistance;

	//dDistance	= sqrt( (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) + (z1-z2)*(z1-z2) );
	dDistance	= sqrt( (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) );

	return	dDistance;
}

//////////////////////////////////////////////////////////////////////
// 04 三角形生长法：
// 获取余弦角度值
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
// 02 获取采样点集中的最大最小顶点
//////////////////////////////////////////////////////////////////////
void CDCR_TinPITACreator::GetMaxMinVextex()
{
	if(m_pVertexHead == NULL)	return;

	CDCR_TinVertex	*pVertex;

	//x+y的最大最小值
	double	dMinXAndY,dMaxXAndY;
	//x-y的最大最小值
	double	dMinXMinY,dMaxXMinY;
	
	//x+y的最大最小值
	CDCR_TinVertex	*m_pMinXAddY	= NULL;
	CDCR_TinVertex	*m_pMaxXAddY	= NULL;
	//x-y的最大最小值
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
	
	//向凸包列表中增加
	//x+y最小值
	AddVertexToConvexHull(m_pMinXAddY);
	m_pMinXAddY->m_bUsed	= true;

	//x-y最大值
	AddVertexToConvexHull(m_pMaxXMinY);
	m_pMaxXMinY->m_bUsed	= true;

	//x+y最大值
	AddVertexToConvexHull(m_pMaxXAddY);
	m_pMaxXAddY->m_bUsed	= true;

	//x-y最小值
	AddVertexToConvexHull(m_pMinXMinY);
	m_pMinXMinY->m_bUsed	= true;
}

//////////////////////////////////////////////////////////////////////
// 02 将点加到凸包点集中，这个点集和原始采样点点集有相似之处。
// 即 ID号要一致，因为这些要作为基本的三角网
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
// 02 在指定的位置后插入节点
// 即 ID号要一致，因为这些要作为基本的三角网
//////////////////////////////////////////////////////////////////////
void CDCR_TinPITACreator::AddVertexToConvexHull(CDCR_TinVertex *pInsertAtVertex, CDCR_TinVertex *pVertex)
{
	CDCR_TinVertex*	pTempVertex;
	
	pTempVertex	= new	CDCR_TinVertex;
	pTempVertex->m_nID	= pVertex->m_nID;
	pTempVertex->m_xx	= pVertex->m_xx;
	pTempVertex->m_yy	= pVertex->m_yy;
	pTempVertex->m_zz	= pVertex->m_zz;	

	//在pInsertAtVertex后面插入
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
		//在链表的最后一位增加
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
//根据顶点ID返回凸包点集的序号
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
//根据顶点ID返回凸包点集的序号
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
// 07 通过顶点获得边,应该是可以通过他判断出是那个三角形赋予的
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
// 绘制凸包
//////////////////////////////////////////////////////////////////////
void CDCR_TinPITACreator::DrawConvexHull(CDC* pDC)
{
	//绘制
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
//创建凸包
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

		//计算凸包中的顶点个数
		pVertex			= m_pConvexHullHead;
		while(pVertex != NULL)
		{
			iCount++;	
			pVertex	= pVertex->m_pNext;
		}
		
		//记录凸包中的顶点ID串
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

		//针对顶点串，进行两两循环
		for(int j=0;j<iCount;j++)
		{
			//根据顶点编号获取序号
			CDCR_TinVertex	*pVertex1,*pVertex2;
			pVertex1	= GetIndexFromConvexHull(iArray[j]);
			pVertex2	= GetIndexFromConvexHull(iArray[j+1]);
			
			if(pVertex1==NULL || pVertex2==NULL)
			{
				//这说明出问题，强行退出!
				break;
			}	
			
			//如果凸包中的这个和下一个点已经被用过，就不需要在做了，即使做也是无用功。
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
					
			//进行判断
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

					//计算三角形面积，如果面积为正则位于右侧
					double	dTempArea;
					dTempArea	= GetArea(dX1,dY1,dNewX,dNewY,dX2,dY2);

					if(dTempArea>0.0 || fabs(dTempArea)<1e-10)
					{
						////位于中间
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
				//插入到列表中
				AddVertexToConvexHull(pVertex1,pNewVertex);
				
				//
				pNewVertex->m_bUsed		= true;
				
				//跳出循环
				break;
			}
		}

		delete	[]iArray;

	}while(iCount != iNullInsertNum);
}

//////////////////////////////////////////////////////////////////////
// 通过三角形的顶点号找到三角形指针
//////////////////////////////////////////////////////////////////////
bool CDCR_TinPITACreator::GetTriangleFromID(int iLeftTriangleID, int iRightTriangleID, CDCR_TinTriangle **pLeftTriangle, CDCR_TinTriangle **pRightTriangle)
{
	//退出标志，因为要找两个三角形
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
// TIN的Lop局部优化
//////////////////////////////////////////////////////////////////////
void CDCR_TinPITACreator::LopTin(CDC* pDC)
{
	//
	CDCR_TinTriangleLOP	*pTTL;

	pTTL	= m_pTriangleLOPHead;
	while(pTTL != NULL)
	{
		//提取一三角形进行处理
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

			//处理的边顶点
			int		iVertexID1,iVertexID2;
			double	dX1,dY1,dZ1;
			double	dX2,dY2,dZ2;
			iVertexID1	= pTTL->m_iVertexID1;
			iVertexID2	= pTTL->m_iVertexID2;

			//提取已处理三角形的顶点坐标
			//命名为1-2-3；
			int		iVertexID3;
			double	dX3,dY3,dZ3;

			pTriangle->GetVertexCoord(iVertexID1,dX1,dY1,dZ1,iVertexID2,dX2,dY2,dZ2,iVertexID3,dX3,dY3,dZ3);
			
			double	dTempX1,dTempY1,dTempZ1;
			double	dTempX2,dTempY2,dTempZ2;

			//提取未处理三角形的顶点坐标
			//命名为1-2-4；
			int		iVertexID4;
			double	dX4,dY4,dZ4;

			pUnTriangle->GetVertexCoord(iVertexID1,dTempX1,dTempY1,dTempZ1,iVertexID2,dTempX2,dTempY2,dTempZ2,iVertexID4,dX4,dY4,dZ4);
			
			//现在就是要判断iVertexID4是否在(iVertexID1,iVertexID2,iVertexID3)的三角形之中
			//最好判断下两个三角形中找到的点坐标是否一致
			if((dX1==dTempX1 && dY1==dTempY1 && dZ1==dTempZ1) && (dX2==dTempX2 && dY2==dTempY2 && dZ2==dTempZ2))
			{
				//现在说明什么意思，相当于就是1-2-3 和 1-2-4 两个三角形LOP，实在不行就交换
				/*
				double	P,Q;
				P	= ( (dX1-dX3)*(dY2-dY3)-(dX2-dX3)*(dY1-dY3) ) * ( (dX1-dX4)*(dX2-dX4)+(dY1-dY4)*(dY2-dY4) );
				Q	= ( (dX1-dX3)*(dX2-dX3)+(dY1-dY3)*(dY2-dY3) ) * ( (dX1-dX4)*(dY2-dY4)-(dX2-dX4)*(dY1-dY4) );
				//f=P/Q
				//f>1  : 在圆外
				//f==1 : 在圆上
				//f<1  : 在圆内
				bool	bExChange	= false;
				//等于0
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
					//存在交叉点
					if(IsInCircle(dX1,dY1,dX2,dY2,dX3,dY3,dX4,dY4,dX,dY))
					{
						bExChange	= true;
					}
				}	

				//需要交换
				//LOP交换,2010-11-1：这里出问题了!!!!!!
				//暂时没问题，但是效率有点低2010-11-2
				if(bExChange)
				{
					//新增两个三角形
					
					CDCR_TinTriangle	*pNewTriangle1,*pNewTriangle2;
					//第一个新三角形1-3-4
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

					//第二个新三角形2-3-4
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

					//增加一条新边
					CDCR_TinEdge	*pNewEdge1;
					//新边3-4
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

						//实时绘制
						DrawEdge(pDC,pNewEdge1);
					}
					
					//删除1-2边
					CDCR_TinEdge		*pEdge;
					pEdge	= GetEdgeFromCells((dX1+dX2)/2,(dY1+dY2)/2,iVertexID1,iVertexID2);
					if(pEdge != NULL)
						pEdge->m_bDelete	= true;

					//处理四条旧边
					CDCR_TinEdge		*pOldEdge;
					
					//第一条旧边 1-3,仅仅处理并不往LOP数组中增加
					{
						//
						pOldEdge	= GetEdgeFromCells((dX1+dX3)/2,(dY1+dY3)/2,iVertexID1,iVertexID3);
						
						if(pOldEdge)
						{
							//
							pOldEdge->UpdateTriangle(pTriangle,pNewTriangle1);
						}
					}

					//第二条旧边 1-4,不仅处理且往LOP数组中增加
					{
						//
						pOldEdge	= GetEdgeFromCells((dX1+dX4)/2,(dY1+dY4)/2,iVertexID1,iVertexID4);
						
						if(pOldEdge)
						{
							//
							pOldEdge->UpdateTriangle(pUnTriangle,pNewTriangle1);
							
							//如果左面为新面，则处理旧面
							if(pOldEdge->m_pLeftTriangle == pNewTriangle1)
							{	
								//pOldEdge->m_pRightTriangle是待处理面
								if(pOldEdge->m_pRightTriangle != NULL)
								{
									//增加LOP数组中
									AddTriangleToLOP(pOldEdge->m_pRightTriangle,pNewTriangle1,iVertexID1,iVertexID4);
								}
							}
							else	
							if(pOldEdge->m_pRightTriangle == pNewTriangle1)
							{
								if(pOldEdge->m_pLeftTriangle != NULL)
								{
									//增加LOP数组中
									AddTriangleToLOP(pOldEdge->m_pLeftTriangle,pNewTriangle1,iVertexID1,iVertexID4);
								}
							}
						}
					}

					//第三条旧边 2-3,仅仅处理并不往LOP数组中增加
					{
						//
						pOldEdge	= GetEdgeFromCells((dX2+dX3)/2,(dY2+dY3)/2,iVertexID2,iVertexID3);
						
						if(pOldEdge)
						{
							//
							pOldEdge->UpdateTriangle(pTriangle,pNewTriangle2);
						}
					}

					//第三条旧边 2-4,不仅处理且往LOP数组中增加
					{
						//
						pOldEdge	= GetEdgeFromCells((dX2+dX4)/2,(dY2+dY4)/2,iVertexID2,iVertexID4);
						
						if(pOldEdge)
						{
							//
							pOldEdge->UpdateTriangle(pUnTriangle,pNewTriangle2);

							//如果左面为新面，则处理旧面
							if(pOldEdge->m_pLeftTriangle == pNewTriangle2)
							{	
								//pOldEdge->m_pRightTriangle是待处理面
								if(pOldEdge->m_pRightTriangle != NULL)
								{
									//增加LOP数组中
									AddTriangleToLOP(pOldEdge->m_pRightTriangle,pNewTriangle2,iVertexID2,iVertexID4);
								}
							}
							else	
							if(pOldEdge->m_pRightTriangle == pNewTriangle2)
							{
								if(pOldEdge->m_pLeftTriangle != NULL)
								{
									//增加LOP数组中
									AddTriangleToLOP(pOldEdge->m_pLeftTriangle,pNewTriangle2,iVertexID2,iVertexID4);
								}
							}
						}
					}

					//删除旧的三角形
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
//插入其他点
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
			//首先找到该顶点所在的三角形
			int					iPVertexID;
			double				dPX,dPY,dPZ;

			iPVertexID	= pVertex->m_nID;
			dPX			= pVertex->m_xx;
			dPY			= pVertex->m_yy;
			dPZ			= pVertex->m_zz;

			//根据顶点获取该顶点所在的三角形
			CDCR_TinTriangle	*pTriangle;
			pTriangle		= GetTriangleFromPoint(dPX,dPY);

			if(pTriangle != NULL)
			{
				int		iVertexID1,iVertexID2,iVertexID3;
				double	dX1,dY1,dZ1;
				double	dX2,dY2,dZ2;
				double	dX3,dY3,dZ3;
				
				//获取组成三角形的顶点坐标信息
				pTriangle->GetVertexCoordEx(iVertexID1,dX1,dY1,dZ1,iVertexID2,dX2,dY2,dZ2,iVertexID3,dX3,dY3,dZ3);
				
				//增加三个新三角形
				CDCR_TinTriangle	*pNewTriangle1,*pNewTriangle2,*pNewTriangle3;

				//第一个三角形1-2-P
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

				//第二个三角形2-3-P
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
				//第三个三角形3-1-P
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
				
				//增加三条新边
				CDCR_TinEdge		*pEdge;

				//第一条边1-P
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

					//实时绘制
					DrawEdge(pDC,pEdge);
				}
				
				//第二条边2-P
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

					//实时绘制
					DrawEdge(pDC,pEdge);
				}

				//第三条边3-P
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

					//实时绘制
					DrawEdge(pDC,pEdge);
				}

				//处理三条旧边
				CDCR_TinEdge		*pOldEdge;

				//清空
				ReleaseTriangleLOP();
				
				//第一条旧边 1-2
				{
					//
					pOldEdge	= GetEdgeFromCells((dX1+dX2)/2,(dY1+dY2)/2,iVertexID1,iVertexID2);
					
					//
					pOldEdge->UpdateTriangle(pTriangle,pNewTriangle1);
					
					//如果左面为新面，则处理旧面
					if(pOldEdge->m_pLeftTriangle == pNewTriangle1)
					{	
						//pOldEdge->m_pRightTriangle是待处理面
						if(pOldEdge->m_pRightTriangle != NULL)
						{
							//增加LOP数组中
							AddTriangleToLOP(pOldEdge->m_pRightTriangle,pNewTriangle1,iVertexID1,iVertexID2);
						}
					}
					else	
					if(pOldEdge->m_pRightTriangle == pNewTriangle1)
					{
						if(pOldEdge->m_pLeftTriangle != NULL)
						{
							//增加LOP数组中
							AddTriangleToLOP(pOldEdge->m_pLeftTriangle,pNewTriangle1,iVertexID1,iVertexID2);
						}
					}
				}

				//第二条旧边 2-3
				{
					//
					pOldEdge	= GetEdgeFromCells((dX2+dX3)/2,(dY2+dY3)/2,iVertexID2,iVertexID3);
					
					//
					pOldEdge->UpdateTriangle(pTriangle,pNewTriangle2);

					//如果左面为新面，则处理旧面
					if(pOldEdge->m_pLeftTriangle == pNewTriangle2)
					{
						if(pOldEdge->m_pRightTriangle != NULL)
						{
							//增加LOP数组中
							AddTriangleToLOP(pOldEdge->m_pRightTriangle,pNewTriangle2,iVertexID2,iVertexID3);
						}
					}
					else	
					if(pOldEdge->m_pRightTriangle == pNewTriangle2)
					{
						if(pOldEdge->m_pLeftTriangle != NULL)
						{
							//增加LOP数组中
							AddTriangleToLOP(pOldEdge->m_pLeftTriangle,pNewTriangle2,iVertexID2,iVertexID3);
						}
					}
				}
						
				//第三条旧边 3-1
				{
					//
					pOldEdge	= GetEdgeFromCells((dX3+dX1)/2,(dY3+dY1)/2,iVertexID3,iVertexID1);
					
					//
					pOldEdge->UpdateTriangle(pTriangle,pNewTriangle3);

					//如果左面为新面，则处理旧面
					if(pOldEdge->m_pLeftTriangle == pNewTriangle3)
					{
						if(pOldEdge->m_pRightTriangle != NULL)
						{
							//增加LOP数组中
							AddTriangleToLOP(pOldEdge->m_pRightTriangle,pNewTriangle3,iVertexID3,iVertexID1);
						}
					}
					else	
					if(pOldEdge->m_pRightTriangle == pNewTriangle3)
					{
						if(pOldEdge->m_pLeftTriangle != NULL)
						{
							//增加LOP数组中
							AddTriangleToLOP(pOldEdge->m_pLeftTriangle,pNewTriangle3,iVertexID3,iVertexID1);
						}
					}
				}

				//删除该三角形
				pTriangle->m_bDelete	= true;
			}//if(pTriangle != NULL)


			LopTin(pDC);

			iTotal++;
			//优化
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
	//找到dX、dY所在的格网
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
// 插入第一点
//////////////////////////////////////////////////////////////////////
void CDCR_TinPITACreator::InsertFirstVertex(CDC *pDC)
{
	CDCR_TinVertex	*pVertex;

	double	dMinX,dMaxX,dMinY,dMaxY;
	
	//查找点集的范围
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
	
	//找到中心点
	double	dX,dY;
	dX	= (dMinX+dMaxX)/2;
	dY	= (dMinY+dMaxY)/2;

	//搜索离dX,dY最近的点
	//确定循环的最大次数，m_nCellNumOfX和m_nCellNumOfY的大者
	int	iMaxCount;

	iMaxCount	= m_nCellNumOfX;
	if(iMaxCount < m_nCellNumOfY)	iMaxCount	= m_nCellNumOfY;
	
	//找到dX、dY所在的格网
	int		I,J;
	I	= (int)((dX - m_dCellLeft) / m_dCellSizeOfX);
	J	= (int)((dY - m_dCellBottom) / m_dCellSizeOfY);
	
	CDCR_TinVertex	*pNewVertex;
	pNewVertex	= NULL;

	bool			bFirstValue;
	double			dMinDistance;

	bFirstValue	= true;
	dMinDistance= 0.0;

	//循环的次数
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
							//进行判断
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

				//做标记
				m_pCells[iC].m_bUsed	= true;
			}
		}

		//判断跳出的
		if(pNewVertex != NULL)
		{	
			break;
		}
	}
		
	//找到了
	if(pNewVertex != NULL)
	{
		//使用了
		pNewVertex->m_bUsed	= true;

		int		iCount;
		iCount		= 0;
		pVertex		= m_pConvexHullHead;
		while(pVertex != NULL)
		{
			iCount++;
			pVertex	= pVertex->m_pNext;
		}

		//记录凸包中的顶点ID串
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

		//针对顶点串，进行两两循环
		for(int j=0;j<iCount;j++)
		{
			//根据顶点编号获取序号
			CDCR_TinVertex	*pVertex1,*pVertex2;
			pVertex1	= GetIndexFromVertex(iArray[j]);
			pVertex2	= GetIndexFromVertex(iArray[j+1]);

			if(pVertex1==NULL || pVertex2==NULL)
			{
				//这说明出问题，强行退出!
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

			//构建三角形
			CDCR_TinTriangle	*pTriangle;
			pTriangle	= new	CDCR_TinTriangle;
			pTriangle->m_nID	= m_iTriangleCount+1;
			pTriangle->m_nVertexID[0]	= iNewVertexID;
			pTriangle->m_nVertexID[1]	= iVertexID1;
			pTriangle->m_nVertexID[2]	= iVertexID2;
			pTriangle->SetVertexCoord(iNewVertexID,dNewX,dNewY,dNewZ,iVertexID1,dX1,dY1,dZ1,iVertexID2,dX2,dY2,dZ2);
			AddTriangle(pTriangle);

			CDCR_TinEdge		*pEdge;

			//增加第一条边
			pEdge		= GetEdgeFromCells((dNewX+dX1)/2,(dNewY+dY1)/2,iNewVertexID,iVertexID1);
			if(pEdge != NULL)
			{
				//这可能有点问题，2010-11-1
				pEdge->m_nTriangleID[1]		= pTriangle->m_nID;
				pEdge->m_pRightTriangle		= pTriangle;
			}
			else
			{
				//第一条边
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

				//实时绘制
				DrawEdge(pDC,pEdge);
			}

			//增加第二条边
			pEdge		= GetEdgeFromCells((dX1+dX2)/2,(dY1+dY2)/2,iVertexID2,iVertexID2);
			if(pEdge != NULL)
			{
				pEdge->m_nTriangleID[1]		= pTriangle->m_nID;
				pEdge->m_pRightTriangle		= pTriangle;
			}
			else
			{
				//第一条边
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

				//实时绘制
				DrawEdge(pDC,pEdge);
			}

			//增加第三条边
			pEdge		= GetEdgeFromCells((dX2+dNewX)/2,(dY2+dNewY)/2,iVertexID2,iNewVertexID);
			if(pEdge != NULL)
			{
				pEdge->m_nTriangleID[1]		= pTriangle->m_nID;

				pEdge->m_pRightTriangle		= pTriangle;
			}
			else
			{
				//第一条边
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

				//实时绘制
				DrawEdge(pDC,pEdge);
			}

			pVertex1->m_bUsed	= true;
			pVertex2->m_bUsed	= true;
		}

		delete	[]iArray;
	}
}

//根据顶点的坐标获取，该顶点所在的三角形
CDCR_TinTriangle* CDCR_TinPITACreator::GetTriangleFromPoint(double dX, double dY)
{
	//
	CDCR_TinTriangle	*pTriangle;

	pTriangle		= m_pTriangleHead;
	while(pTriangle != NULL)
	{
		if(false == pTriangle->m_bDelete)
		{
			//在某一三角形内部
			if(pTriangle->IsInTriangle(dX,dY))
			{
				return	pTriangle;
			}
		}

		pTriangle	= pTriangle->m_pNext;
	}
	
	return	NULL;
}

//往边地址列表中增加边
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

//直线段是否交叉，及交叉点
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
	//判断是否在延长线上
	if(fabs(dX2-dX1) > fabs(dY2-dY1))	//横大于纵
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
	//判断是否在延长线上
	if(fabs(dX4-dX3) > fabs(dY4-dY3))	//横大于纵
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
//交叉点(dXD,dYD)
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

	//在圆内，需要交换
	if(dPD*dD3 < dD1*dD2)	return	true;
	
	return	false;
}
