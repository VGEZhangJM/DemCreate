// DCR_OppositeHeight.h: interface for the CDCR_OppositeHeight class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCR_OPPOSITEHEIGHT_H__EF0F9CAF_0575_4D9D_9D6D_F7F618E399DE__INCLUDED_)
#define AFX_DCR_OPPOSITEHEIGHT_H__EF0F9CAF_0575_4D9D_9D6D_F7F618E399DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDCR_OppositeHeight  
{
public:
	CDCR_OppositeHeight();
	virtual ~CDCR_OppositeHeight();

	//
	double* GetOHeight();
	void	GetOHeightStatisticsInfo(double& dMin,double& dMax, double& dMean,double& dStd);

	//
	void	SetGridDem(double *pData,int iNumOfX,int iNumOfY,double dSizeOfX,double dSizeOfY,double dRealSize,double dLeft,double dRight,double dBottom,double dTop,CString szPathName);

	//分析窗口尺寸
	void	SetWindowSize(int iSize);
	int		GetWindowSize();
	
	//
	bool	OnSave();
	//
	void	OnDraw(CDC *pDC);
	//
	void	OnRelease();
	//
	void	OnCalculate(int iWindowSize = 0);

protected:
	//窗口尺寸
	CRect	m_rRect;
	double	m_dMapXA,m_dMapXB,m_dMapYA,m_dMapYB;
	//
	int		m_iWindowSize;

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

	//相对高信息
	double	*m_pHeightObj;
	//最大、最小相对高
	double	m_dMinHeight,m_dMaxHeight;
	//平均相对高
	double	m_dMeanHeight;
	//相对高方差
	double	m_dStdHeight;

};

#endif // !defined(AFX_DCR_OPPOSITEHEIGHT_H__EF0F9CAF_0575_4D9D_9D6D_F7F618E399DE__INCLUDED_)
