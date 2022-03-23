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
	//48 ����/��ȡ�¶���ʾ״̬
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
	//�����¶ȱ���
	void	OnCalculateSlopeOfSlope( );

	//
	void	SetGridDem(double *pData,int iNumOfX,int iNumOfY,double dSizeOfX,double dSizeOfY,double dRealSize,double dLeft,double dRight,double dBottom,double dTop,CString szPathName);

protected:
	//���ײ��ģ�����¶�
	void	OnCalculateWithSecondOrderDifferentialModel();
	
protected:
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

	//Slope
	bool	m_bShowStatus;

	//�¶ȱ���
	double*	m_pSlopeOfSlopeObj;
	//
	double	m_dMinSlopeOfSlope,m_dMaxSlopeOfSlope;
	//ƽ���¶�
	double	m_dMeanSlopeOfSlope;
	//�¶ȷ���
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
