// DCR_Grid_KRG.h: interface for the CDCR_Grid_KRG class.
// 克里金插值算法
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCR_GRID_KRG_H__04831692_C43E_42CA_8DEC_1C3A1621A71A__INCLUDED_)
#define AFX_DCR_GRID_KRG_H__04831692_C43E_42CA_8DEC_1C3A1621A71A__INCLUDED_

#include "DCR_GridVertexList.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDCR_Grid_KRG  
{
public:
	CDCR_Grid_KRG();
	virtual ~CDCR_Grid_KRG();
	
	//设置顶点列表
	void	SetGridVertexList(CDCR_GridVertexList *pVretexList);

	//计算插值的高程值
	//dSDZ为Z的方差
	bool	CalcHeight(double dX,double dY,double& dZ,double& dSDZ);

	//
	void	SetModelType(int iModelType);
	int		GetModelType();

	//
	void	SetC(double	dC);
	double	GetC();

	//
	void	SetC0(double dC0);
	double	GetC0();

	//
	void	SetA(double dA);
	double	GetA();

protected:
	//高斯模型
	bool	KRGGaussModel(double dX,double dY,double& dZ,double& dSDZ);
	//指数模型
	bool	KRGExponentModel(double dX, double dY, double &dZ,double& dSDZ);
	//球状模型
	bool	KRGSphereModel(double dX,double dY,double& dZ,double& dSDZ);
	//线性模型
	bool	KRGLineModel(double dX,double dY,double& dZ,double& dSDZ);

	//
	double	GetDistance(double x1, double y1, double z1, double x2, double y2, double z2);

public:
	//采样点点数
	CDCR_GridVertexList	*m_pVretexList;
	
	//模型种类
	// 0：球状模型
	// 1：指数模型
	// 2：高斯模型
	int		m_iModelType;

	//块金常数
	double	m_dC0;
	//拱高
	double	m_dC;
	//变程
	double	m_dAlfa;

	CString	m_szPathName;
};

#endif // !defined(AFX_DCR_GRID_KRG_H__04831692_C43E_42CA_8DEC_1C3A1621A71A__INCLUDED_)
