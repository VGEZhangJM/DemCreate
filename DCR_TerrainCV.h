// DCR_TerrainCV.h: interface for the CDCR_TerrainCV class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCR_TERRAINCV_H__4AFA7EC2_F521_43B1_9731_F4446A1F8835__INCLUDED_)
#define AFX_DCR_TERRAINCV_H__4AFA7EC2_F521_43B1_9731_F4446A1F8835__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDCR_TerrainCV  
{
public:
	CDCR_TerrainCV();
	virtual ~CDCR_TerrainCV();
public:
	//56 ����/��ȡ�����������ʾ״̬
	void	SetShowStatus(bool bShowStatus);
	bool	GetShowStatus();

	//
	double* GetCV();
	void	GetCVStatisticsInfo(double& dMin, double &dMax, double& dMean,double& dStd);

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

	//�̱߳�����Ϣ
	double	*m_pCVObj;
	//�����С�̱߳���
	double	m_dMinCV,m_dMaxCV;
	//ƽ���̱߳���
	double	m_dMeanCV;
	//�̱߳��췽��
	double	m_dStdCV;
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

#endif // !defined(AFX_DCR_TERRAINCV_H__4AFA7EC2_F521_43B1_9731_F4446A1F8835__INCLUDED_)
