// DCR_Curvature.h: interface for the CDCR_Curvature class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCR_CURVATURE_H__63A7E35B_78AA_4B99_997B_AE80D13308A0__INCLUDED_)
#define AFX_DCR_CURVATURE_H__63A7E35B_78AA_4B99_997B_AE80D13308A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDCR_Curvature  
{
public:
	CDCR_Curvature();
	virtual ~CDCR_Curvature();

public:
	//50 ����/��ȡ������ʾ״̬
	void	SetShowStatus(bool bShowStatus);
	bool	GetShowStatus();

	//
	void	SetGridDem(double *pData,int iNumOfX,int iNumOfY,double dSizeOfX,double dSizeOfY,double dRealSize,double dLeft,double dRight,double dBottom,double dTop,CString szPathName);

	//��������
	void	OnCalculateProfileCurvature();
	//ƽ������
	void	OnCalculatePlanCurvature();
	//��������
	void	OnCalculateTgCurvature();

	//
	bool	OnSave( );
	//
	void	OnDraw(CDC *pDC);
	//
	void	OnRelease();

	//
	double* GetProfileCurvature();
	void	GetProfileCurvatureStatisticsInfo(double& dMin,double& dMax, double& dMean,double& dStd);

	//
	double* GetPlanCurvature();
	void	GetPlanCurvatureStatisticsInfo(double& dMin,double& dMax, double& dMean,double& dStd);

	//
	double* GetTgCurvature();
	void	GetTgCurvatureStatisticsInfo(double& dMin,double& dMax, double& dMean,double& dStd);

	//
	void	SetCoordMapping(double dXA,double dXB,double dYA,double dYB);
	//
	void	SetClientRect(CRect rRect);

protected:
	//
	void	OnDrawProfileCurvature(CDC *pDC);
	//
	void	OnDrawPlanCurvature(CDC *pDC);
	//
	void	OnDrawTgCurvature(CDC *pDC);

	//
	bool	OnSavePlanCurvature();
	//
	bool	OnSaveProfileCurvature();

protected:
	//��������
	double*			m_pProfileCurvatureObj;
	double			m_dMinProfileCurvature,m_dMaxProfileCurvature;
	double			m_dMeanProfileCurvature,m_dStdProfileCurvature;

	//ƽ������
	double*			m_pPlanCurvatureObj;
	double			m_dMinPlanCurvature,m_dMaxPlanCurvature;
	double			m_dMeanPlanCurvature,m_dStdPlanCurvature;

	//ƽ������
	double*			m_pTgCurvatureObj;
	double			m_dMinTgCurvature,m_dMaxTgCurvature;
	double			m_dMeanTgCurvature,m_dStdTgCurvature;

	//Curvatires
	bool			m_bShowStatusInCurvature;

	//���ڳߴ�
	CRect	m_rRect;
	double	m_dMapXA,m_dMapXB,m_dMapYA,m_dMapYB;

	//�¶���Ϣ
	double	*m_pSlopeObj;
	//�����С�¶�
	double	m_dMinSlope,m_dMaxSlope;
	//ƽ���¶�
	double	m_dMeanSlope;
	//�¶ȷ���
	double	m_dStdSlope;

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

#endif // !defined(AFX_DCR_CURVATURE_H__63A7E35B_78AA_4B99_997B_AE80D13308A0__INCLUDED_)
