// DCR_CurvatureSet.h: interface for the CDCR_CurvatureSet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCR_CURVATURESET_H__9A182527_384C_43F6_87A4_E9E83C5FF3FC__INCLUDED_)
#define AFX_DCR_CURVATURESET_H__9A182527_384C_43F6_87A4_E9E83C5FF3FC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDCR_CurvatureSet  
{
public:
	CDCR_CurvatureSet();
	virtual ~CDCR_CurvatureSet();

	static void UnInstance();
	static CDCR_CurvatureSet* Instance();

	//通过坡度值获取颜色值
	COLORREF	GetColor(double dCurvature);
	
	//
	void		SetMinCurvature(double dMinCurvature);
	//
	void		SetMidCurvature(double dMidCurvaturee);
	//
	void		SetMaxCurvature(double dMaxCurvature);

public:
	static	CDCR_CurvatureSet* m_pInstance;

	//
	double		m_dMinCurvature;
	//
	double		m_dMidCurvature;
	//
	double		m_dMaxCurvature;

	//最小坡度的颜色
	int			m_iMinR,m_iMinG,m_iMinB;
	//
	int			m_iMidR,m_iMidG,m_iMidB;
	//
	int			m_iMaxR,m_iMaxG,m_iMaxB;
};

#endif // !defined(AFX_DCR_CURVATURESET_H__9A182527_384C_43F6_87A4_E9E83C5FF3FC__INCLUDED_)
