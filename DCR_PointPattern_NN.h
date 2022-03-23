// DCR_PointPattern_NN.h: interface for the CDCR_PointPattern_NN class.
// 通过最邻近距离统计判断点格局
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCR_POINTPATTERN_NN_H__2F916D70_BB14_45D4_9B33_BE94A300578D__INCLUDED_)
#define AFX_DCR_POINTPATTERN_NN_H__2F916D70_BB14_45D4_9B33_BE94A300578D__INCLUDED_

#include "DCR_GridVertexList.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDCR_PointPattern_NN  
{
public:
	CDCR_PointPattern_NN();
	virtual ~CDCR_PointPattern_NN();

	//设置顶点列表
	void	SetGridVertexList(CDCR_GridVertexList *pVretexList);
	//
	double	JudgePattern(double dA);
	//
	double	GetDistance(double x1, double y1, double z1, double x2, double y2, double z2);

	//计算质心
	double	CalcCentroid(double dX,double dY);
	
protected:
	//采样点点数
	CDCR_GridVertexList	*m_pVretexList;

	//
	double				m_dArea;
};

#endif // !defined(AFX_DCR_POINTPATTERN_NN_H__2F916D70_BB14_45D4_9B33_BE94A300578D__INCLUDED_)
