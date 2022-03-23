// DCR_SlopeSet.h: interface for the CDCR_SlopeSet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCR_SLOPESET_H__2C4DF201_04B2_4FBF_B5A2_149E95DF1841__INCLUDED_)
#define AFX_DCR_SLOPESET_H__2C4DF201_04B2_4FBF_B5A2_149E95DF1841__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDCR_SlopeSet  
{
public:
	CDCR_SlopeSet();
	virtual ~CDCR_SlopeSet();
	
	static void UnInstance();
	static CDCR_SlopeSet* Instance();

	//通过坡度值获取颜色值
	COLORREF	GetColor(double dSlope);
	
	//
	void		SetMinSlope(double dMinSlope);
	//
	void		SetMidSlope(double dMidSlope);
	//
	void		SetMaxSlope(double dMaxSlope);

public:
	static	CDCR_SlopeSet* m_pInstance;

	//
	double		m_dMinSlope;
	//
	double		m_dMidSlope;
	//
	double		m_dMaxSlope;

	//最小坡度的颜色
	int			m_iMinR,m_iMinG,m_iMinB;
	//
	int			m_iMidR,m_iMidG,m_iMidB;
	//
	int			m_iMaxR,m_iMaxG,m_iMaxB;
};

#endif // !defined(AFX_DCR_SLOPESET_H__2C4DF201_04B2_4FBF_B5A2_149E95DF1841__INCLUDED_)
