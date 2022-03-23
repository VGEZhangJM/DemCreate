// DCR_Param_KRG.h: interface for the CDCR_Param_KRG class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCR_PARAM_KRG_H__36CE7B33_8F14_4587_8F39_F2B2F2A1FD05__INCLUDED_)
#define AFX_DCR_PARAM_KRG_H__36CE7B33_8F14_4587_8F39_F2B2F2A1FD05__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDCR_Param_KRG  
{
public:
	CDCR_Param_KRG();
	virtual ~CDCR_Param_KRG();

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

	//模型种类,
	//0：球型模型
	//1：指数模型
	//2：高斯模型
	int		m_iModelType;

	//块金
	double	m_dC0;
	//基台值
	double	m_dC;
	//变程
	double	m_dAlfa;
};

#endif // !defined(AFX_DCR_PARAM_KRG_H__36CE7B33_8F14_4587_8F39_F2B2F2A1FD05__INCLUDED_)
