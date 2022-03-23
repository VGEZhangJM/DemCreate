// DCR_GeoMorphological_HXY.h: interface for the CDCR_GeoMorphological_HXY class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCR_GEOMORPHOLOGICAL_HXY_H__56DEBEA2_D293_4A6A_BB77_7F0DCC989359__INCLUDED_)
#define AFX_DCR_GEOMORPHOLOGICAL_HXY_H__56DEBEA2_D293_4A6A_BB77_7F0DCC989359__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DCR_OppositeHeight.h"
#include "DCR_Slope.h"

class CDCR_GeoMorphological_HXY  
{
public:
	CDCR_GeoMorphological_HXY();
	virtual ~CDCR_GeoMorphological_HXY();

	//
	void	SetGridDem(double *pData,int iNumOfX,int iNumOfY,double dSizeOfX,double dSizeOfY,double dRealSize,double dLeft,double dRight,double dBottom,double dTop,CString szPathName);
	
	//
	bool	Judge();

	//
	void	SetCoordMapping(double dXA,double dXB,double dYA,double dYB);
	//
	void	SetClientRect(CRect rRect);

	//
	void	Draw(CDC *pDC);

	//
	void	Release();

	//���ô��ڳߴ�
	void	SetWindowSize(int iSize);
	int		GetWindowSize();

protected:
	//���ڳߴ�
	CRect	m_rRect;
	double	m_dMapXA,m_dMapXB,m_dMapYA,m_dMapYB;

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

	//����Ԫ��:
	//�¶�<3		: ƽ��
	//��Ը߶�<100  : 
	//<20		: ƽԭ
	//20-200	: ����
	//200-500	: ��ɽ
	//500-1500	: ��ɽ
	//>1500		: ��ɽ
	BYTE	*m_pTypeObj;

	//�ߴ�
	int		m_iWindowSize;

	//��Ը߼�����
	CDCR_OppositeHeight	pOHeight;

	//�¶ȼ�����
	CDCR_Slope			pSlope;
};

#endif // !defined(AFX_DCR_GEOMORPHOLOGICAL_HXY_H__56DEBEA2_D293_4A6A_BB77_7F0DCC989359__INCLUDED_)
