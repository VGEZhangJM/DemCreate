// DCR_Grid_SPD.h: interface for the CDCR_Grid_SPD class.
// 改进谢别德法
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCR_GRID_SPD_H__82096AEF_00A2_480A_9464_6D381E1B4307__INCLUDED_)
#define AFX_DCR_GRID_SPD_H__82096AEF_00A2_480A_9464_6D381E1B4307__INCLUDED_

#include "DCR_GridVertexList.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDCR_Grid_SPD  
{
public:
	CDCR_Grid_SPD();
	virtual ~CDCR_Grid_SPD();
	
	//设置顶点列表
	void	SetGridVertexList(CDCR_GridVertexList *pVretexList);

	//计算插值的高程值
	bool	CalcHeight(double dX,double dY,double& dZ);

	//
	bool	CalcHeightEx(double dX,double dY,double& dZ);

	//
	void	SetU(int iU);
	int		GetU();

	//
	void	SetT(double dT);
	double	GetT();
	
	//
	void	SetPathName(CString szPathName);
	CString	GetPathName();

	//
	void	SetCustom(bool bCustom);
	bool	GetCustom();
	//
	void	SetRadius(double dRadius);
	double	GetRadius();
	
protected:
	//用户指定半径?
	bool				m_bCustom;
	//局部范围,可以是采样点的最远值
	//也可以是用户指定
	double				m_dRadius;

	//权指数
	int					m_iU;
	//光滑参数
	double				m_dT;

	//采样点点数
	CDCR_GridVertexList	*m_pVretexList;

	//
	CString				m_szPathName;
};

#endif // !defined(AFX_DCR_GRID_SPD_H__82096AEF_00A2_480A_9464_6D381E1B4307__INCLUDED_)
