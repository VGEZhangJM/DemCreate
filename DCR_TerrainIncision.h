// DCR_TerrainIncision.h: interface for the CDCR_TerrainIncision class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCR_TERRAININCISION_H__876E52B4_15BE_41B7_A8C7_5CA86D943C75__INCLUDED_)
#define AFX_DCR_TERRAININCISION_H__876E52B4_15BE_41B7_A8C7_5CA86D943C75__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDCR_TerrainIncision  
{
public:
	CDCR_TerrainIncision();
	virtual ~CDCR_TerrainIncision();

public:
	//56 ����/��ȡ�����������ʾ״̬
	void	SetShowStatus(bool bShowStatus);
	bool	GetShowStatus();

	//
	double* GetIncision();
	void	GetIncisionStatisticsInfo(double& dMin,double& dMax, double& dMean,double& dStd);

	//�������ڳߴ�
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
	//���ڳߴ�
	CRect	m_rRect;
	double	m_dMapXA,m_dMapXB,m_dMapYA,m_dMapYB;

	//�ֲڶ���Ϣ
	double	*m_pIncisionObj;
	//�����С�ֲڶ�
	double	m_dMinIncision,m_dMaxIncision;
	//ƽ���ֲڶ�
	double	m_dMeanIncision;
	//�ֲڶȷ���
	double	m_dStdIncision;
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

#endif // !defined(AFX_DCR_TERRAININCISION_H__876E52B4_15BE_41B7_A8C7_5CA86D943C75__INCLUDED_)
