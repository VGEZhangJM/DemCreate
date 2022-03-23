// DCR_PointFeature.h: interface for the CDCR_PointFeature class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCR_POINTFEATURE_H__DF4634FA_AA87_4ADE_8AB8_847A55C210A6__INCLUDED_)
#define AFX_DCR_POINTFEATURE_H__DF4634FA_AA87_4ADE_8AB8_847A55C210A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDCR_PointFeature  
{
public:
	CDCR_PointFeature();
	virtual ~CDCR_PointFeature();

public:
	//
	void	OnDraw(CDC *pDC,double dXA,double dXB,double dYA,double dYB,COLORREF crColor);
	//
	void	AddPoint(double dX,double dY,double dZ);

	//缺省都是为1
	int					m_iPointNum;
	//具体的三维坐标点
	SMap_3DPointCoord	m_pPointCoord;
	//
	double				m_dLeft,m_dRight,m_dBottom,m_dTop;

	//
	bool				m_bUsed;

	CDCR_PointFeature	*m_pNext;
};

#endif // !defined(AFX_DCR_POINTFEATURE_H__DF4634FA_AA87_4ADE_8AB8_847A55C210A6__INCLUDED_)
