// DCR_GridVertex.h: interface for the CDCR_GridVertex class.
// DEM顶点数据
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCR_GRIDVERTEX_H__D03A0B20_C519_42A5_8F8E_38042102BB60__INCLUDED_)
#define AFX_DCR_GRIDVERTEX_H__D03A0B20_C519_42A5_8F8E_38042102BB60__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDCR_GridVertex  
{
public:
	CDCR_GridVertex();
	virtual ~CDCR_GridVertex();
	
public:
	//点坐标
	double			m_dX;
	double			m_dY;
	double			m_dZ;

	//距离
	double			m_dDistance;
	
	//
	CDCR_GridVertex	*m_pNext;
};

#endif // !defined(AFX_DCR_GRIDVERTEX_H__D03A0B20_C519_42A5_8F8E_38042102BB60__INCLUDED_)
