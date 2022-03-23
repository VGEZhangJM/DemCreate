// DCR_LocalTerrainCharacter.h: interface for the CDCR_LocalTerrainCharacter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCR_LOCALTERRAINCHARACTER_H__1D43A273_7BAE_465B_BD95_6A2F93EC4861__INCLUDED_)
#define AFX_DCR_LOCALTERRAINCHARACTER_H__1D43A273_7BAE_465B_BD95_6A2F93EC4861__INCLUDED_

#include "DCR_GridVertexList.h"
#include "DCR_PointPattern_NN.h"
#include "DCR_TinTDTACreator.h"
#include "DCR_TinTriangleList.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//局部地形特征
class CDCR_LocalTerrainCharacter  
{
public:
	CDCR_LocalTerrainCharacter();
	virtual ~CDCR_LocalTerrainCharacter();
public:
	//计算点格局
	bool	CalculatePointPattern();

	//首先创建TIN，从TIN模型中计算坡度和粗糙度
	bool	CaculateTerrainFactor();

	//设置顶点坐标
	void	SetGridVertexList(CDCR_GridVertexList *pAllVretexList,CDCR_GridVertexList *pVretexList,double dRadius,double dX,double dY);

	void	OnSave(FILE *fp);
	
	//
	double	GetPointPatternValue();
	void	SetPointPatternValue(double dValue);
	//
	double	GetCentroidValue();
	void	SetCentroidValue(double dValue);

	//
	double	GetMeanSlope();
	void	SetMeanSlope(double dValue);

	//
	double	GetMeanRoughness();
	void	SetMeanRoughness(double dValue);

	//
	double	GetMaxDistance();
	//
	double	GetMinDistance();
	
	//
	CDCR_TinTriangleList*	GetTriangleList();
	
protected:
	//最大范围内的计算采样点
	CDCR_GridVertexList		*m_pVretexList;

	//最大范围的所有采样点
	CDCR_GridVertexList		*m_pAllVretexList;

	//
	CDCR_TinTriangleList	m_pTriangleList;

	//最近邻点格局
	CDCR_PointPattern_NN	m_pPointPatterNN;

	//TIN创建者
	CDCR_TinTDTACreator		m_pTinCreator;

	//当前点坐标
	double					m_dCenterX;
	double					m_dCenterY;

	//最大距离
	double					m_dRadius;

	//最小距离
	double					m_dMinDistance;

	//点格局值
	double					m_dPointPatternValue;
	//质心值
	double					m_dCentroidValue;
	//平均坡度值
	double					m_dMeanSlope;
	//地形粗糙度值
	double					m_dMeanRoughness;
};

#endif // !defined(AFX_DCR_LOCALTERRAINCHARACTER_H__1D43A273_7BAE_465B_BD95_6A2F93EC4861__INCLUDED_)
