// DCR_TinPITACreator.h: interface for the CDCR_TinPITACreator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCR_TINPITACREATOR_H__F9C9C34E_1247_4432_B066_89F60D8C3844__INCLUDED_)
#define AFX_DCR_TINPITACREATOR_H__F9C9C34E_1247_4432_B066_89F60D8C3844__INCLUDED_

#include "DCR_TinVertex.h"
#include "DCR_TinEdge.h"
#include "DCR_TinTriangle.h"
#include "DCR_Cells.h"
#include "DCR_TinTriangleLOP.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//逐点插入算法
class CDCR_TinPITACreator  
{
public:
	CDCR_TinPITACreator();
	virtual ~CDCR_TinPITACreator();
	
	// 001 释放TIN数据及采样点数据
	void				Release();

	// 002 增加数据
	void				AddVertex(double dX,double dY,double dZ);

	// 003 使用各种方法创建TIN
	bool				Create(CDC *pDC);

	// 004 设置显示参数
	void 				SetCoordMapping(double dXA,double dXB,double dYA,double dYB);
	void				SetClientRect(CRect rRect);
	
	// 005 绘制边
	void				DrawEdge(CDC* pDC,CDCR_TinEdge *pEdge);
	//
	void				DrawTriangle(CDC* pDC,CDCR_TinTriangle *pTriangle);
	// 006 绘制TIN
	void				Draw(CDC *pDC);
	//
	void				SetMapExt(double dLeft,double dRight,double dBottom,double dTop);

protected:
	//
	double				GetAngle(double dx, double dy);
	//
	double				GetArea(double dX1, double dY1, double dX2, double dY2, double dX3, double dY3);
	//
	double				GetCosValue(double dX, double dY, double dZ, double dX1, double dY1, double dZ1,double dX2, double dY2,double dZ2);
	//
	double				GetDistance(double x1, double y1, double z1, double x2, double y2, double z2);
	//
	void				GetMaxMinVextex();

	// 05 获取指定的顶点的坐标
	bool				GetVertexCoordFromID(int iVertexID, double &xx, double &yy, double &zz);
	
	//
	void				AddVertexToConvexHull(CDCR_TinVertex *pVertex);
	//
	void				AddVertexToConvexHull(CDCR_TinVertex *pInsertAtVertex, CDCR_TinVertex *pVertex);

	//
	CDCR_TinVertex*		GetIndexFromConvexHull(int nVertexID);
	CDCR_TinVertex*		GetIndexFromVertex(int nVertexID);

	//
	CDCR_TinEdge*		GetEdgeWithVertex(int nVertexID1, int nVertexID2);

	// 创建凸包
	void				CreateConvexHull(CDC* pDC);

	// 绘制凸包
	void				DrawConvexHull(CDC* pDC);

	// TIN的Lop局部优化
	void				LopTin(CDC* pDC);
	
	//
	bool				GetTriangleFromID(int iLeftTriangleID, int iRightTriangleID, CDCR_TinTriangle **pLeftTriangle, CDCR_TinTriangle **pRightTriangle);
	
	//插入其他点
	void				InsertOrtherVertex(CDC *pDC);

	//
	void				ReleaseVertex();
	//
	void				ReleaseTriangle();
	//
	void				ReleaseEdge();
	//
	void				ReleaseConverHull();

	//插入第一点
	void				InsertFirstVertex(CDC *pDC);

	//
	bool				AddEdgeToCells(double dX,double dY,CDCR_TinEdge *pEdge);

	//将顶点增加到分区格网中
	bool				AddVertexToCells(CDCR_TinVertex *pVertex);
	//
	void				CellBuild();
	//
	CDCR_TinEdge*		GetEdgeFromCells(double dX, double dY, int iVertexID1, int iVertexID2);

	//
	void				AddTriangle(CDCR_TinTriangle *pTriangle);

	//
	void				AddEdge(CDCR_TinEdge *pEdge);

	//
	void				AddTriangleToLOP(CDCR_TinTriangle *pUnTriangle,CDCR_TinTriangle *pTriangle,int iVertexID1,int iVertexID2);

protected:
	bool				IsInCircle(double dX1,double dY1,double dX2,double dY2,double dX3,double dY3,double dXP,double dYP,double dXD,double dYD);
	bool				IntersectLineLine(double dX1,double dY1,double dX2,double dY2,double dX3,double dY3,double dX4,double dY4,double& dX,double& dY);
	//
	void				ReleaseTriangleLOP();
	//
	CDCR_TinTriangle*	GetTriangleFromPoint(double dX,double dY);

	//顶点列表
	//CArray<CDCR_TinVertex*,CDCR_TinVertex*>		m_arrVertex;
	CDCR_TinVertex		*m_pVertexHead;
	CDCR_TinVertex		*m_pVertexLast;
	int					m_iVertexCount;

	//凸包列表
	//CArray<CDCR_TinVertex*,CDCR_TinVertex*>		m_arrConvexHull;
	CDCR_TinVertex		*m_pConvexHullHead;
	CDCR_TinVertex		*m_pConvexHullLast;

	//边列表
	//CArray<CDCR_TinEdge*,CDCR_TinEdge*>			m_arrEdge;
	CDCR_TinEdge		*m_pEdgeHead;
	CDCR_TinEdge		*m_pEdgeLast;
	int					m_iEdgeCount;

	//三角形列表
	//CArray<CDCR_TinTriangle*,CDCR_TinTriangle*>	m_arrTriangle;
	CDCR_TinTriangle	*m_pTriangleHead;
	CDCR_TinTriangle	*m_pTriangleLast;
	int					m_iTriangleCount;

	//问题出这，应为三角形
	CDCR_TinTriangleLOP	*m_pTriangleLOPHead;
	CDCR_TinTriangleLOP	*m_pTriangleLOPLast;

	//缩放系数
	double				m_dMapXA,m_dMapXB;
	double				m_dMapYA,m_dMapYB;

	//内存索引
	CDCR_Cells*			m_pCells;

	//地图格网大小
	double				m_dCellSizeOfY;
	double				m_dCellSizeOfX;
	
	//地图分块数
	long				m_nCellNumOfY;
	long				m_nCellNumOfX;
	//划分网格范围
	double				m_dCellLeft,m_dCellRight;
	double				m_dCellBottom,m_dCellTop;
	//地图区域
	double				m_dMapLeft,m_dMapRight;
	double				m_dMapBottom,m_dMapTop;

	//
	CRect				m_rRect;
};

#endif // !defined(AFX_DCR_TINPITACREATOR_H__F9C9C34E_1247_4432_B066_89F60D8C3844__INCLUDED_)
