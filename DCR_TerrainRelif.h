// DCR_TerrainRelif.h: interface for the CDCR_TerrainRelif class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCR_TERRAINRELIF_H__B7C7D626_9E56_4844_B292_E47910411773__INCLUDED_)
#define AFX_DCR_TERRAINRELIF_H__B7C7D626_9E56_4844_B292_E47910411773__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDCR_TerrainRelif  
{
public:
	CDCR_TerrainRelif();
	virtual ~CDCR_TerrainRelif();

public:
	//56 设置/获取地形起伏度显示状态
	void	SetShowStatus(bool bShowStatus);
	bool	GetShowStatus();

	//
	double* GetRelif();
	void	GetRelifStatisticsInfo(double& dMin,double& dMax, double& dMean,double& dStd);

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
	double	*m_pRelifObj;
	//最大、最小粗糙度
	double	m_dMinRelif,m_dMaxRelif;
	//平均粗糙度
	double	m_dMeanRelif;
	//粗糙度方差
	double	m_dStdRelif;

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

#endif // !defined(AFX_DCR_TERRAINRELIF_H__B7C7D626_9E56_4844_B292_E47910411773__INCLUDED_)
