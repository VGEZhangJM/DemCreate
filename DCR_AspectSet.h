// DCR_AspectSet.h: interface for the CDCR_AspectSet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCR_ASPECTSET_H__4501BB17_98A3_41F1_BAAF_FCB897AF83CF__INCLUDED_)
#define AFX_DCR_ASPECTSET_H__4501BB17_98A3_41F1_BAAF_FCB897AF83CF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDCR_AspectSet  
{
public:
	CDCR_AspectSet();
	virtual ~CDCR_AspectSet();

	static void UnInstance();
	static CDCR_AspectSet* Instance();
	
	//通过坡度值获取颜色值
	COLORREF	GetColor(double dAspect);
	
	//
	void		SetMinAspect(double dMinAspect);
	//
	void		SetMidAspect(double dMidAspect);
	//
	void		SetMaxAspect(double dMaxAspect);
	
public:
	static	CDCR_AspectSet* m_pInstance;
	
	//
	double		m_dMinAspect;
	//
	double		m_dMidAspect;
	//
	double		m_dMaxAspect;
	
	//最小坡度的颜色
	int			m_iMinR,m_iMinG,m_iMinB;
	//
	int			m_iMidR,m_iMidG,m_iMidB;
	//
	int			m_iMaxR,m_iMaxG,m_iMaxB;
};

#endif // !defined(AFX_DCR_ASPECTSET_H__4501BB17_98A3_41F1_BAAF_FCB897AF83CF__INCLUDED_)
