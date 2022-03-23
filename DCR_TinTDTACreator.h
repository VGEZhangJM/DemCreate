// DCR_TinTDTACreator.h: interface for the CDCR_TinTDTACreator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCR_TINTDTACREATOR_H__C8D0B305_605A_4F78_9741_DCDD35FB414F__INCLUDED_)
#define AFX_DCR_TINTDTACREATOR_H__C8D0B305_605A_4F78_9741_DCDD35FB414F__INCLUDED_

#include "DCR_TinVertex.h"
#include "DCR_TinEdge.h"
#include "DCR_TinTriangle.h"
#include "DCR_Cells.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//三角形生长算法
class CDCR_TinTDTACreator  
{
public:
	CDCR_TinTDTACreator();
	virtual ~CDCR_TinTDTACreator();
	
public:
	// 001 释放TIN数据及采样点数据
	void				Release();

	// 002 增加数据
	void				AddVertex(double dX,double dY,double dZ);

	// 003 使用各种方法创建TIN
	bool				Create(CDC *pDC = NULL);

	// 004 设置显示参数
	void 				SetCoordMapping(double dXA,double dXB,double dYA,double dYB);
	//
	void				SetClientRect(CRect rRect);
	
	// 005 绘制边
	void				DrawEdge(CDC* pDC,CDCR_TinEdge *pEdge);
	// 006 绘制TIN
	void				Draw(CDC *pDC);
	//
	void				SetMapExt(double dLeft,double dRight,double dBottom,double dTop);

	//在构建TIN后计算平均坡度
	bool				CaculateTerrainFactor(double& dMeanSlope,double& dMeanRoughness);

	//
	double				CaculateSlope(double dX1,double dY1,double dZ1,double dX2,double dY2,double dZ2,double dX3,double dY3,double dZ3);
	//
	double				CaculateSurfaceArea(double dX1,double dY1,double dZ1,double dX2,double dY2,double dZ2,double dX3,double dY3,double dZ3);

	//
	CDCR_TinTriangle*	GetTriangle();

protected:
	//顶点列表
	//CArray<CDCR_TinVertex*,CDCR_TinVertex*>		m_arrVertex;
	CDCR_TinVertex		*m_pVertexHead;
	CDCR_TinVertex		*m_pVertexLast;
	int					m_iVertexCount;

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

protected:
	CDCR_TinEdge*		GetEdgeFromCells(double dX,double dY,int iVertexID1,int iVertexID2);
	//
	void				CellReset();
	//
	void				CellBuild();
	//
	void				AddTriangle(CDCR_TinTriangle *pTriangle);
	//
	void				AddEdge(CDCR_TinEdge *pEdge);
	//将顶点增加到分区格网中
	bool				AddVertexToCells(CDCR_TinVertex *pVertex);
	//将边增加到分区格网中
	bool				AddEdgeToCells(double dX,double dY,CDCR_TinEdge *pEdge);
	//
	void				ReleaseVertex();
	//
	void				ReleaseTriangle();
	//
	void				ReleaseEdge();
	// 01 获取两点间的距离
	double				GetDistance(double x1, double y1, double z1, double x2, double y2, double z2);

	// 02 判断点直线的哪一方？
	int					CreateJudgeFunction(double dX1, double dY1, double dX2, double dY2, double &a, double &b);

	// 03
	double				GetJudgeValue(double dX, double dY, int bSign, double a, double b);

	// 04 获取余弦角度值
	double				GetCosValue(double dX, double dY, double dZ, double dX1, double dY1, double dZ1,double dX2, double dY2,double dZ2);
	

	// 05 获取指定的顶点的坐标
	bool				GetVertexCoordFromID(int iVertexID, double &xx, double &yy, double &zz);
	
	// 06 从边序号获得顶点序号
	CDCR_TinEdge*		GetEdgeFromEdgeID(int iEdgeID, int &iVertexID1, int &iVertexID2);
	// 07 通过顶点序号获得边
	CDCR_TinEdge*		GetEdgeWithVertex(int nVertexID1, int nVertexID2);
	//
	void				GetEdgeWithVertex(int iVertexID1, int iVertexID2, int iVertexID3,CDCR_TinEdge** pEdge1,CDCR_TinEdge** pEdge2,CDCR_TinEdge** pEdge3);

	// 08 获取还没有处理的三角形对象，
	// 如果为NULL，则表示所有的三角形都已经处理完成
	CDCR_TinTriangle*	GetUnUsedTriangle();

	// 09 判断边是否被使用两次
	bool				IsEdgeUsedTwice(int iVertexID1,int iVertexID2);

	// 10 获取第一个顶点的序号【可以是任意的】
	int					CreateFirstVertex();

	// 11 根据第一个顶点坐标寻找第二个顶点的序号
	int					CreateSecondVertex(int iVertexID);

	// 12 获取第三个顶点的序号
	int					CreateThirdVertex(int iFirstVertexID, int iSecondVertexID);
};

#endif // !defined(AFX_DCR_TINTDTACREATOR_H__C8D0B305_605A_4F78_9741_DCDD35FB414F__INCLUDED_)
