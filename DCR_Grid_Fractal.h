// DCR_Grid_Fractal.h: interface for the CDCR_Grid_Fractal class.
// ·ÖÐÎÎ¬Êý
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCR_GRID_FRACTAL_H__6EF5DC1D_92B1_48E4_8026_172FA92920D8__INCLUDED_)
#define AFX_DCR_GRID_FRACTAL_H__6EF5DC1D_92B1_48E4_8026_172FA92920D8__INCLUDED_

#include "DCR_GridDem.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDCR_Grid_Fractal  
{
public:
	//
	double	OnCalculateSurface(double& dV);
	//
	double	CalculateDem();
	//
	void	SetSize(double dSize);

	CDCR_Grid_Fractal();
	virtual ~CDCR_Grid_Fractal();

	//
	int		m_iDemNumX,m_iDemNumY;
	double	m_dDemSizeX,m_dDemSizeY;
	double	*m_pHeight;
	
	//
	CDCR_GridDem	m_pGridDem;
};

#endif // !defined(AFX_DCR_GRID_FRACTAL_H__6EF5DC1D_92B1_48E4_8026_172FA92920D8__INCLUDED_)
