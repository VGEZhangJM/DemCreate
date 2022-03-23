// DCR_GeoMorphological_LSD.h: interface for the CDCR_GeoMorphological_LSD class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCR_GEOMORPHOLOGICAL_LSD_H__A527563B_B517_451A_877B_45600D3B6A99__INCLUDED_)
#define AFX_DCR_GEOMORPHOLOGICAL_LSD_H__A527563B_B517_451A_877B_45600D3B6A99__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DCR_Slope.h"

class CDCR_GeoMorphological_LSD  
{
public:
	double TH(double dX);
	double CH(double dX);
	CDCR_GeoMorphological_LSD();
	virtual ~CDCR_GeoMorphological_LSD();

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

	//ÉèÖÃ´°¿Ú³ß´ç
	void	SetWindowSize(int iSize);
	int		GetWindowSize();
	//
	bool	OnCapture(double dX,double dY);

protected:
	double SH(double dX);
	//´°¿Ú³ß´ç
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

	double	*m_pTypeObj;

	//³ß´ç
	int		m_iWindowSize;

	//ÆÂ¶È¼ÆËãÀà
	CDCR_Slope			pSlope;

};

#endif // !defined(AFX_DCR_GEOMORPHOLOGICAL_LSD_H__A527563B_B517_451A_877B_45600D3B6A99__INCLUDED_)
