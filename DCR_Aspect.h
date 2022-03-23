// DCR_Aspect.h: interface for the CDCR_Aspect class.
// 坡向
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCR_ASPECT_H__6A3FA5C1_AECB_4D9C_AD4A_FFCE63762F95__INCLUDED_)
#define AFX_DCR_ASPECT_H__6A3FA5C1_AECB_4D9C_AD4A_FFCE63762F95__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDCR_Aspect  
{
public:
	CDCR_Aspect();
	virtual ~CDCR_Aspect();

	//48 设置/获取坡向显示状态
	void	SetShowStatus(bool bShowStatus);
	bool	GetShowStatus();
	
	//
	double* GetAspect();
	void	GetAspectStatisticsInfo(double& dMin, double &dMax, double& dMean,double& dStd);
	
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
	//
	void	OnCalculateAspect();
	
	//
	void	SetGridDem(double *pData,int iNumOfX,int iNumOfY,double dSizeOfX,double dSizeOfY,double dRealSize,double dLeft,double dRight,double dBottom,double dTop,CString szPathName);
		
protected:
	//窗口尺寸
	CRect	m_rRect;
	double	m_dMapXA,m_dMapXB,m_dMapYA,m_dMapYB;
	
	//坡度信息
	double	*m_pAspectObj;
	//最大、最小坡度
	double	m_dMinAspect,m_dMaxAspect;
	//平均坡度
	double	m_dMeanAspect;
	//坡度方差
	double	m_dStdAspect;
	
	//Slope
	bool	m_bShowStatus;
		
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

#endif // !defined(AFX_DCR_ASPECT_H__6A3FA5C1_AECB_4D9C_AD4A_FFCE63762F95__INCLUDED_)
