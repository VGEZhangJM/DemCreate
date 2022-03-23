// DCR_Grid_PLY.h: interface for the CDCR_Grid_PLY class.
// 多项式插值，包括双线性，最近邻、自然样条、V4
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCR_GRID_PLY_H__11038BFA_432A_4D75_ACB3_9BF9D5AB46B2__INCLUDED_)
#define AFX_DCR_GRID_PLY_H__11038BFA_432A_4D75_ACB3_9BF9D5AB46B2__INCLUDED_

#include "DCR_GridVertexList.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDCR_Grid_PLY  
{
public:
	CDCR_Grid_PLY();
	virtual ~CDCR_Grid_PLY();
	
	//设置顶点列表
	void	SetGridVertexList(CDCR_GridVertexList *pVretexList);

	//计算插值的高程值
	bool	CalcHeight(double dX,double dY,double& dZ);

public:
	//是拟合还是插值
	//0:拟合
	//1:插值
	int					m_iInterpolationMode;
	//模型类型
	// 0:双线性
	// 1:最近邻
	// 2:自然样条
	// 3:V4-Matlab
	int					m_iModelType;

	//采样点点数
	CDCR_GridVertexList	*m_pVretexList;

	//
	CString				m_szPathName;
};

#endif // !defined(AFX_DCR_GRID_PLY_H__11038BFA_432A_4D75_ACB3_9BF9D5AB46B2__INCLUDED_)
