// DCR_TinCreator.h: interface for the CDCR_TinCreator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCR_TINCREATOR_H__994D0100_0E88_456C_A259_CD4A9E97DCF5__INCLUDED_)
#define AFX_DCR_TINCREATOR_H__994D0100_0E88_456C_A259_CD4A9E97DCF5__INCLUDED_

#include "DCR_TinTDTACreator.h"
#include "DCR_TinPITACreator.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDCR_TinCreator  
{
public:
	CDCR_TinCreator();
	virtual ~CDCR_TinCreator();

	// 释放
	void	Release();

	// 002 增加数据
	void	AddVertex(int iType, double dX,double dY,double dZ);
	
	//
	void	SetClientRect(CRect rRect);
	//
	void	SetCoordMapping(double dXA,double dXB,double dYA,double dYB);

	//
	void	SetMapExt(double dLeft,double dRight,double dBottom,double dTop);
	void	GetMapExt(double& dLeft,double& dRight,double& dBottom,double& dTop);

	//
	void	Draw(CDC *pDC);

	//
	void	Create(CDC *pDC,int iType);

protected:
	//
	CDCR_TinTDTACreator	m_pTinTDTACreator;

	//
	CDCR_TinPITACreator	m_pTinPITACreator;

	//
	double	m_dMapLeft;
	double	m_dMapRight;
	double	m_dMapBottom;
	double	m_dMapTop;
};

#endif // !defined(AFX_DCR_TINCREATOR_H__994D0100_0E88_456C_A259_CD4A9E97DCF5__INCLUDED_)
