// DCR_LeastSquare.h: interface for the CDCR_LeastSquare class.
// 最小二乘方法
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCR_LEASTSQUARE_H__4B48385F_206D_4F35_B04E_9427FB17AB18__INCLUDED_)
#define AFX_DCR_LEASTSQUARE_H__4B48385F_206D_4F35_B04E_9427FB17AB18__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "matlib.h"
#include "math.h"
#include "CalcOLSquation.h"

class CDCR_LeastSquare  
{
public:
	CDCR_LeastSquare();
	virtual ~CDCR_LeastSquare();
	
	//计算
	bool	GetZCoord(double dX,double dY,double& dZ);
	
	//计算矩阵,得到C矩阵
	bool	CalcMatrix(int iPointNum,double *dX,double *dY,double *dZ);

protected:
	
	//
	int		m_iIndex;

	//
	bool	m_bAlready;
	
	//矩阵A
	double	*A;

	//
	int		m_iCoff;
	double	C[50];
	int		CX[50];
	int		CY[50];
};

#endif // !defined(AFX_DCR_LEASTSQUARE_H__4B48385F_206D_4F35_B04E_9427FB17AB18__INCLUDED_)
