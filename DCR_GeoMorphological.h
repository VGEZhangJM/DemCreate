// DCR_GeoMorphological.h: interface for the CDCR_GeoMorphological class.
// 地貌类型判断
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCR_GEOMORPHOLOGICAL_H__99AFAFA1_90A2_4BE3_ABB8_52C32345A8A7__INCLUDED_)
#define AFX_DCR_GEOMORPHOLOGICAL_H__99AFAFA1_90A2_4BE3_ABB8_52C32345A8A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DCR_GeoMorphological_CZX.h"
#include "DCR_GeoMorphological_HXY.h"
#include "DCR_GeoMorphological_LSD.h"

class CDCR_GeoMorphological  
{
public:
	CDCR_GeoMorphological();
	virtual ~CDCR_GeoMorphological();

	//
	void	SetGridDem(double *pData,int iNumOfX,int iNumOfY,double dSizeOfX,double dSizeOfY,double dRealSize,double dLeft,double dRight,double dBottom,double dTop,CString szPathName);
	
	//设置窗口尺寸
	void	SetWindowSize(int iSize);
	int		GetWindowSize();

	//判断地貌类型
	bool	OnJudge(int iType);

	//
	void	SetCoordMapping(double dXA,double dXB,double dYA,double dYB);
	//
	void	SetClientRect(CRect rRect);

	//
	void	OnDraw(CDC *pDC);

	//
	void	OnRelease();

	//
	bool	OnCapture(double dX,double dY);

protected:
	//窗口尺寸
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

	//
	int		m_iWindowSize;
	
	//判别种类
	int		m_iGeoType;

	//
	CDCR_GeoMorphological_CZX	m_pGeoCZX;

	//
	CDCR_GeoMorphological_HXY	m_pGeoHXY;

	//
	CDCR_GeoMorphological_LSD	m_pGeoLSD;
};

#endif // !defined(AFX_DCR_GEOMORPHOLOGICAL_H__99AFAFA1_90A2_4BE3_ABB8_52C32345A8A7__INCLUDED_)
