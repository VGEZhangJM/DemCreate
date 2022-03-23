// DCR_Wavelets.h: interface for the CDCR_Wavelets class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCR_WAVELETS_H__82C96D9B_70E5_48EA_B6E7_CCD623CBB372__INCLUDED_)
#define AFX_DCR_WAVELETS_H__82C96D9B_70E5_48EA_B6E7_CCD623CBB372__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDCR_Wavelets  
{
public:
	CDCR_Wavelets();
	virtual ~CDCR_Wavelets();

	//赋值
	void	SetOrignDem(double *pData,int iNumOfX,int iNumOfY,double dMinH,double dMaxH,double dLeft,double dRight,double dBottom,double dTop);

	//
	void	WaveletsD4();

	//保存数据
	void	SaveCurrentDem(CString szPathName);

protected:
	//
	void	Calculate(double *pOrign, int n,double **pCurrent);

protected:
	//原来的DEM高程值
	double	*m_pOrignBits;

	//简化之后的DEM高程值
	double	*m_pCurrentBits;
	
	//原始DEM的格式数,一般是2的倍数
	int		m_iNumOfX;
	int		m_iNumOfY;

	//高程极值
	double	m_dMinHeight;
	double	m_dMaxHeight;

	//DEM范围
	double	m_dDemLeft,m_dDemRight,m_dDemBottom,m_dDemTop;

};

#endif // !defined(AFX_DCR_WAVELETS_H__82C96D9B_70E5_48EA_B6E7_CCD623CBB372__INCLUDED_)
