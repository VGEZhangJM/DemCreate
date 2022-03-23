// DCR_Wavelets.h: interface for the CDCR_Wavelets class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCR_WAVELETS_H__82C96D9B_70E5_48EA_B6E7_CCD623CBB372__INCLUDED_)
#define AFX_DCR_WAVELETS_H__82C96D9B_70E5_48EA_B6E7_CCD623CBB372__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDCR_Wavelets  
{
public:
	CDCR_Wavelets();
	virtual ~CDCR_Wavelets();

	//��ֵ
	void	SetOrignDem(double *pData,int iNumOfX,int iNumOfY,double dMinH,double dMaxH,double dLeft,double dRight,double dBottom,double dTop);

	//
	void	WaveletsD4();

	//��������
	void	SaveCurrentDem(CString szPathName);

protected:
	//
	void	Calculate(double *pOrign, int n,double **pCurrent);

protected:
	//ԭ����DEM�߳�ֵ
	double	*m_pOrignBits;

	//��֮���DEM�߳�ֵ
	double	*m_pCurrentBits;
	
	//ԭʼDEM�ĸ�ʽ��,һ����2�ı���
	int		m_iNumOfX;
	int		m_iNumOfY;

	//�̼߳�ֵ
	double	m_dMinHeight;
	double	m_dMaxHeight;

	//DEM��Χ
	double	m_dDemLeft,m_dDemRight,m_dDemBottom,m_dDemTop;

};

#endif // !defined(AFX_DCR_WAVELETS_H__82C96D9B_70E5_48EA_B6E7_CCD623CBB372__INCLUDED_)
