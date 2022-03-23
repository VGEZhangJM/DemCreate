// DCR_TerrainRoughness.h: interface for the CDCR_TerrainRoughness class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCR_TERRAINROUGHNESS_H__FB4A15DA_9FFB_425C_901C_B597BBC9923D__INCLUDED_)
#define AFX_DCR_TERRAINROUGHNESS_H__FB4A15DA_9FFB_425C_901C_B597BBC9923D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDCR_TerrainRoughness  
{
public:
	CDCR_TerrainRoughness();
	virtual ~CDCR_TerrainRoughness();

public:
	//54 设置/获取粗糙度显示状态
	void	SetShowStatus(bool bShowStatus);
	bool	GetShowStatus();

	//
	double* GetRoughness();
	void	GetRoughnessStatisticsInfo(double& dMin,double& dMax, double& dMean,double& dStd);

	//分析窗口尺寸
	void	SetWindowSize(int iSize);
	int		GetWindowSize();
	
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
	void	OnCalculate(int iWindowSize = 0);

	//
	void	SetGridDem(double *pData,int iNumOfX,int iNumOfY,double dSizeOfX,double dSizeOfY,double dRealSize,double dLeft,double dRight,double dBottom,double dTop,CString szPathName);

protected:
	//窗口尺寸
	CRect	m_rRect;
	double	m_dMapXA,m_dMapXB,m_dMapYA,m_dMapYB;

	//粗糙度信息
	double	*m_pRoughnessObj;
	//最大、最小粗糙度
	double	m_dMinRoughness,m_dMaxRoughness;
	//平均粗糙度
	double	m_dMeanRoughness;
	//粗糙度方差
	double	m_dStdRoughness;

	//
	bool	m_bShowStatus;

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
};

#endif // !defined(AFX_DCR_TERRAINROUGHNESS_H__FB4A15DA_9FFB_425C_901C_B597BBC9923D__INCLUDED_)
