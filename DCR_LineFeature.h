// DCR_LineFeature.h: interface for the CDCR_LineFeature class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCR_LINEFEATURE_H__DF53A740_C690_4779_ADC8_C9ACE94AFFB5__INCLUDED_)
#define AFX_DCR_LINEFEATURE_H__DF53A740_C690_4779_ADC8_C9ACE94AFFB5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDCR_LineFeature  
{
public:
	CDCR_LineFeature();
	virtual ~CDCR_LineFeature();

public:
	//
	void	OnDraw(CDC *pDC,double dXA,double dXB,double dYA,double dYB,COLORREF crColor);
	
	//
	void	AddLine(int iPointNum,double *dX,double *dY,double dZ);

	//缺省都是大于等于2
	int					m_iPointNum;
	//具体的三维坐标点
	SMap_3DPointCoord	*m_pPointCoord;

	//
	double				m_dLeft,m_dRight,m_dBottom,m_dTop;

	//
	CDCR_LineFeature	*m_pNext;
};

#endif // !defined(AFX_DCR_LINEFEATURE_H__DF53A740_C690_4779_ADC8_C9ACE94AFFB5__INCLUDED_)
