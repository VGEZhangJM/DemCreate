// DCR_Param_IDW.h: interface for the CDCR_Param_IDW class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCR_PARAM_IDW_H__252232E9_B496_4689_BFC6_492D455B5F98__INCLUDED_)
#define AFX_DCR_PARAM_IDW_H__252232E9_B496_4689_BFC6_492D455B5F98__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDCR_Param_IDW  
{
public:
	CDCR_Param_IDW();
	virtual ~CDCR_Param_IDW();

	//
	bool	Read(FILE *fp);

public:
	//DEM格网数目
	int		m_iDemNumX;
	//
	int		m_iDemNumY;
	
	//搜索方式
	//0:为全局搜索，即无方向限制
	//1:为四方向搜索
	//2:为八方向搜索
	int		m_iSearchType;
	
	//
	int		m_iPointNumPreDirection;
	
	//
	int		m_iPointNumInTotal;

	//搜索半径
	double	m_dSearchRadius;
	
	//
	int		m_iNumOfU;
	//权指数
	int		m_iU[5];
	
	//平滑参数
	double	m_dT;
};

#endif // !defined(AFX_DCR_PARAM_IDW_H__252232E9_B496_4689_BFC6_492D455B5F98__INCLUDED_)
