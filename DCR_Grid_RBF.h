// DCR_Grid_RBF.h: interface for the CDCR_Grid_RBF class.
// 径向基函数插值算法
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCR_GRID_RBF_H__4FF03EF4_1212_4CB9_89C4_0D464D3351A3__INCLUDED_)
#define AFX_DCR_GRID_RBF_H__4FF03EF4_1212_4CB9_89C4_0D464D3351A3__INCLUDED_

#include "DCR_GridVertexList.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDCR_Grid_RBF  
{
public:
	CDCR_Grid_RBF();
	virtual ~CDCR_Grid_RBF();
	
	//设置顶点列表
	void	SetGridVertexList(CDCR_GridVertexList *pVretexList);

	//计算插值的高程值
	bool	CalcHeight(double dX,double dY,double& dZ);

	//
	void	SetModelType(int iType);
	int		GetModelType();
	
	//
	void	SetC(double dC);
	double	GetC();

	//
	void	SetPathName(CString szPathName);
	CString	GetPathName();

protected:
	//
	bool	RBNCSF(double dX,double dY,double& dZ);
	//
	bool	RBFMLF(double dX, double dY, double &dZ);
	//
	bool	RBFTPSF(double dX, double dY, double &dZ);
	//
	bool	RBFIMQF(double dX,double dY,double& dZ);
	//
	bool	RBFMQF(double dX,double dY,double& dZ);
	//
	double	GetDistance(double x1, double y1, double z1, double x2, double y2, double z2);

public:
	//采样点点数
	CDCR_GridVertexList	*m_pVretexList;
	
	//模型种类,
	//1：多重曲面插值法
	//2：反多重曲面插值法
	int		m_iModelType;

	//参数
	double	m_dC;
	
	//
	CString	m_szPathName;
};

#endif // !defined(AFX_DCR_GRID_RBF_H__4FF03EF4_1212_4CB9_89C4_0D464D3351A3__INCLUDED_)
