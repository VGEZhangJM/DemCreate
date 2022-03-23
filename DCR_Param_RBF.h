// DCR_Param_RBF.h: interface for the CDCR_Param_RBF class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCR_PARAM_RBF_H__F1286749_5651_4782_8D61_68A397AE595E__INCLUDED_)
#define AFX_DCR_PARAM_RBF_H__F1286749_5651_4782_8D61_68A397AE595E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDCR_Param_RBF  
{
public:
	CDCR_Param_RBF();
	virtual ~CDCR_Param_RBF();
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
	//1：多重曲面插值法
	//2：反多重曲面插值法
	int		m_iModelType;

	//参数的个数
	int		m_iNumOfC;

	//参数做多20
	double	m_dC[20];
};

#endif // !defined(AFX_DCR_PARAM_RBF_H__F1286749_5651_4782_8D61_68A397AE595E__INCLUDED_)
