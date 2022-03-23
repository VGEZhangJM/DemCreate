// DCR_LocalTerrainCreator.h: interface for the CDCR_LocalTerrainCreator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCR_LOCALTERRAINCREATOR_H__1960049E_AEF5_439F_A161_280E28D009DF__INCLUDED_)
#define AFX_DCR_LOCALTERRAINCREATOR_H__1960049E_AEF5_439F_A161_280E28D009DF__INCLUDED_

#include "DCR_Cells.h"
#include "DCR_GridVertexList.h"
#include "DCR_TinTriangleList.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDCR_LocalTerrainCreator  
{
public:
	CDCR_LocalTerrainCreator();
	virtual ~CDCR_LocalTerrainCreator();

public:
	//
	void	OnRelease();
	//原始数据的分区格网数据
	void	SetCell(CDCR_Cells *pCells,int iNumX,int iNumY,double dSizeX,double dSizeY,double dLeft,double dRight,double dBottom,double dTop);

	//
	void	CalculateLocalTerrainCharacter(	int iSampleCount, 
											CDCR_PointFeature *pSamplePointList,
											CString	szPathName,
											int		iSearchType,
											int		iPointNumPreDirection,
											int		iPointNumInTotal,
											double	dSearchRadius
											);

	//单点计算
	void	CalculateLocalTerrainCharacter(	int iSampleCount, 
											CDCR_PointFeature *pSamplePointList,
											CDCR_PointFeature *pSelectPoint,
											int		iSearchType,
											int		iPointNumPreDirection,
											int		iPointNumInTotal,
											double	dSearchRadius
											);
	
	//
	double	GetPointPatternValue();
	//
	double	GetCentroidValue();
	//
	double	GetMeanSlope();
	//
	double	GetMeanRoughness();
	//
	double	GetMaxDistance();
	//
	double	GetMinDistance();

	//
	CDCR_GridVertexList*	GetGridVertexList();
	//
	CDCR_GridVertexList*	GetAllGridVertexList();
	//
	CDCR_TinTriangleList*	GetTriangleList();

protected:
	//
	bool	CalculateSearchRadius(double dX, double dY, double& dMaxDistance);
		
	//
	bool	SearchSamplingPoint(CDCR_PointFeature *pDelPoint);

	//
	bool	ReSearchSamplingPoint(CDCR_PointFeature *pDelPoint,double dRadius);

	//
	void	CellReset();
	//
	double	GetDistance(double x1, double y1, double z1, double x2, double y2, double z2);
	//
	double	GetAngle(double dX, double dY);

protected:
	//参与插值计算的顶点列表
	CDCR_GridVertexList		m_pGridVertexList;

	//所有范围内的顶点列表
	CDCR_GridVertexList		m_pAllGridVertexList;

	//
	CDCR_TinTriangleList	m_pTriangleList;

	//导入CDCR_WorkSpace中的分区格网数据
	//分区格网数据
	CDCR_Cells				*m_pCells;
	//
	//分区格网大小
	double					m_dCellSizeOfX;
	double					m_dCellSizeOfY;
	
	//分区格网块数
	long					m_iCellNumOfX;
	long					m_iCellNumOfY;

	//分区格网范围
	double					m_dCellLeft,m_dCellRight;
	double					m_dCellBottom,m_dCellTop;

	//关于领域的搜索
	//全局搜索标志，

	//当为true时，m_dSearchRadius不起作用；
	//当为false时，m_dSearchRadius起作用；
	bool					m_bGlobalSearch;
	//搜索半径
	double					m_dSearchRadius;
	//搜索半径起作用了
	bool					m_bSearchRadiusUsed;
	//搜索半径起作用的次数
	int						m_iSearchRadiusUsed;

	//搜索方向：分为无方向、四方向、八方向，
	//但是一般使用的是四方向和八方向
	DCR_DIRECTION			m_enumDirection;
	
	//每一个搜索方向内的最多搜索点数
	int						m_iPointNumPreDirection;

	//搜索的总点数，这个参数仅仅在无方向时，起作用，
	//即总共需要搜索多少个最近的点数
	int						m_iPointNumInTotal;

	//点格局值
	double					m_dPointPatternValue;
	//
	double					m_dCentroidValue;
	//平均坡度值
	double					m_dMeanSlope;
	//地形粗糙度值
	double					m_dMeanRoughness;

	//
	double					m_dMaxDistance;

	//
	double					m_dMinDistance;
};

#endif // !defined(AFX_DCR_LOCALTERRAINCREATOR_H__1960049E_AEF5_439F_A161_280E28D009DF__INCLUDED_)
