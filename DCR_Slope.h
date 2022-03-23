// DCR_Slope.h: interface for the CDCR_Slope class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCR_SLOPE_H__B0A1032C_87B2_47A3_B768_EA4944F3C747__INCLUDED_)
#define AFX_DCR_SLOPE_H__B0A1032C_87B2_47A3_B768_EA4944F3C747__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDCR_Slope  
{
public:
	CDCR_Slope();
	virtual ~CDCR_Slope();
public:
	//48 设置/获取坡度显示状态
	void	SetShowStatus(bool bShowStatus);
	bool	GetShowStatus();

	//
	double* GetSlopeOfSlope();
	void	GetSlopeOfSlopeStatisticsInfo(double& dMin, double &dMax, double& dMean,double& dStd);

	//
	double* GetSlope();
	void	GetSlopeStatisticsInfo(double& dMin, double &dMax, double& dMean,double& dStd);
	
	//
	void	SetCoordMapping(double dXA,double dXB,double dYA,double dYB);
	//
	void	SetClientRect(CRect rRect);

	//
	bool	OnSave();
	//
	void	OnDraw(CDC *pDC);
	//
	void	OnRelease();
	//
	void	OnCalculate(int iType);
	//计算坡度变率
	void	OnCalculateSlopeOfSlope( );

	//
	void	SetGridDem(double *pData,int iNumOfX,int iNumOfY,double dSizeOfX,double dSizeOfY,double dRealSize,double dLeft,double dRight,double dBottom,double dTop,CString szPathName);

protected:
	//二阶差分模型求坡度
	void	OnCalculateWithSecondOrderDifferentialModel();
	
protected:
	//窗口尺寸
	CRect	m_rRect;
	double	m_dMapXA,m_dMapXB,m_dMapYA,m_dMapYB;

	//坡度信息
	double	*m_pSlopeObj;
	//最大、最小坡度
	double	m_dMinSlope,m_dMaxSlope;
	//平均坡度
	double	m_dMeanSlope;
	//坡度方差
	double	m_dStdSlope;

	//Slope
	bool	m_bShowStatus;

	//坡度变率
	double*	m_pSlopeOfSlopeObj;
	//
	double	m_dMinSlopeOfSlope,m_dMaxSlopeOfSlope;
	//平均坡度
	double	m_dMeanSlopeOfSlope;
	//坡度方差
	double	m_dStdSlopeOfSlope;

	//
	double	*m_pDemObj;

	//
	int		m_iNumOfX,m_iNumOfY;

	//
	double	m_dDemLeft,m_dDemRight,m_dDemBottom,m_dDemTop;

	//
	double	m_dSizeOfX,m_dSizeOfY;

	//
	double	m_dRealSize;

	//
	CString	m_szPathName;
};

#endif // !defined(AFX_DCR_SLOPE_H__B0A1032C_87B2_47A3_B768_EA4944F3C747__INCLUDED_)
