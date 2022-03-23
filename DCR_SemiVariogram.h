// DCR_SemiVariogram.h: interface for the CDCR_SemiVariogram class.
// ����캯��
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
	//������ɢ������
	void	SetPointFeature(CDCR_PointFeature *pPointHead);

	//
	void	SetPathName(CString	szPathName);

	//�������캯��
	int		SemiVariogram(double **dX,double **dY);

	CDCR_SemiVariogram();
	virtual ~CDCR_SemiVariogram();

protected:
	//
	double	CalcDistance(double dX1,double dY1,double dX2,double dY2);
	//
	void	CalcRange(double& dLeft,double& dRight ,double& dBottom,double& dTop);

protected:
	//��ɢ������
	CDCR_PointFeature	*m_pPointHead;

	//֮�����
	double				m_dLagSize;
	//
	int					m_iLagNum;

	//ÿһ�εĳ���
	double				*m_dLagDistance;
	//ÿһ�εĵ����Ŀ
	int					*m_iLagCount;

	//ÿһ�ͺ�����µİ���캯��ֵ
	double				*m_dRPreLag;
	//ÿһ�ͺ�����µ��ͺ����ֵ
	double				*m_dHPreLag;

	//
	CString				m_szPathName;
};

#endif // !defined(AFX_DCR_SEMIVARIOGRAM_H__70AF99EA_10CB_40E3_B3E7_135961BCB7E7__INCLUDED_)
