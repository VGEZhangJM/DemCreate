// DCR_TinTriangleLOP.h: interface for the CDCR_TinTriangleLOP class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCR_TINTRIANGLELOP_H__783BECAD_85A7_4EB7_87FB_860F6E7CDE57__INCLUDED_)
#define AFX_DCR_TINTRIANGLELOP_H__783BECAD_85A7_4EB7_87FB_860F6E7CDE57__INCLUDED_

#include "DCR_TinTriangle.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDCR_TinTriangleLOP  
{
public:
	CDCR_TinTriangleLOP();
	virtual ~CDCR_TinTriangleLOP();

public:
	bool				m_bUsed;
	//待处理三角形
	CDCR_TinTriangle	*m_pUnTriangle;
	
	//确定的三角形
	CDCR_TinTriangle	*m_pTriangle;

	//
	int					m_iVertexID1;
	int					m_iVertexID2;

	//
	CDCR_TinTriangleLOP	*m_pNext;
};

#endif // !defined(AFX_DCR_TINTRIANGLELOP_H__783BECAD_85A7_4EB7_87FB_860F6E7CDE57__INCLUDED_)
