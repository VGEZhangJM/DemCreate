// DCR_Param_SPD.h: interface for the CDCR_Param_SPD class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCR_PARAM_SPD_H__1FB0ED5D_8EFF_49D2_90E5_6F55F6967174__INCLUDED_)
#define AFX_DCR_PARAM_SPD_H__1FB0ED5D_8EFF_49D2_90E5_6F55F6967174__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDCR_Param_SPD  
{
public:
	CDCR_Param_SPD();
	virtual ~CDCR_Param_SPD();

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

	int		m_iNumOfU;
	//权指数
	int		m_iU[5];

	//光滑参数
	double	m_dT;

	//
	int		m_iRadiusType;
	//
	double	m_dMaxRadius;

	//
	int		m_iKernalType;
};

#endif // !defined(AFX_DCR_PARAM_SPD_H__1FB0ED5D_8EFF_49D2_90E5_6F55F6967174__INCLUDED_)
