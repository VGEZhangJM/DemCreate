// DCR_SemiVariogram.h: interface for the CDCR_SemiVariogram class.
// 半变异函数
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCR_SEMIVARIOGRAM_H__70AF99EA_10CB_40E3_B3E7_135961BCB7E7__INCLUDED_)
#define AFX_DCR_SEMIVARIOGRAM_H__70AF99EA_10CB_40E3_B3E7_135961BCB7E7__INCLUDED_

#include "DCR_PointFeature.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDCR_SemiVariogram  
{
public:
	//接受离散点数据
	void	SetPointFeature(CDCR_PointFeature *pPointHead);

	//
	void	SetPathName(CString	szPathName);

	//计算半变异函数
	int		SemiVariogram(double **dX,double **dY);

	CDCR_SemiVariogram();
	virtual ~CDCR_SemiVariogram();

protected:
	//
	double	CalcDistance(double dX1,double dY1,double dX2,double dY2);
	//
	void	CalcRange(double& dLeft,double& dRight ,double& dBottom,double& dTop);

protected:
	//离散点数据
	CDCR_PointFeature	*m_pPointHead;

	//之后距离
	double				m_dLagSize;
	//
	int					m_iLagNum;

	//每一段的长度
	double				*m_dLagDistance;
	//每一段的点对数目
	int					*m_iLagCount;

	//每一滞后距离下的半变异函数值
	double				*m_dRPreLag;
	//每一滞后距离下的滞后距离值
	double				*m_dHPreLag;

	//
	CString				m_szPathName;
};

#endif // !defined(AFX_DCR_SEMIVARIOGRAM_H__70AF99EA_10CB_40E3_B3E7_135961BCB7E7__INCLUDED_)
