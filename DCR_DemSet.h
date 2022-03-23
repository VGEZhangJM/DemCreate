// DCR_DemSet.h: interface for the CDCR_DemSet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCR_DEMSET_H__D776AFA3_9A9C_48B8_9B68_A5DCDF366325__INCLUDED_)
#define AFX_DCR_DEMSET_H__D776AFA3_9A9C_48B8_9B68_A5DCDF366325__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDCR_DemSet  
{
public:
	CDCR_DemSet();
	virtual ~CDCR_DemSet();

	static void UnInstance();
	static CDCR_DemSet* Instance();

	//通过坡度值获取颜色值
	COLORREF	GetColor(double dHeight);
	
	//
	void		SetMinHeight(double dMinHeight);
	//
	void		SetMidHeight(double dMidHeight);
	//
	void		SetMaxHeight(double dMaxHeight);

public:
	static	CDCR_DemSet* m_pInstance;

	//
	double		m_dMinHeight;
	//
	double		m_dMidHeight;
	//
	double		m_dMaxHeight;

	//最小坡度的颜色
	int			m_iMinR,m_iMinG,m_iMinB;
	//
	int			m_iMidR,m_iMidG,m_iMidB;
	//
	int			m_iMaxR,m_iMaxG,m_iMaxB;
};

#endif // !defined(AFX_DCR_DEMSET_H__D776AFA3_9A9C_48B8_9B68_A5DCDF366325__INCLUDED_)
