// DCR_Cells.h: interface for the CDCR_Cells class.
// 内存索引
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCR_CELLS_H__59DC2DB4_CA76_483B_97C6_EB4C848ABDB2__INCLUDED_)
#define AFX_DCR_CELLS_H__59DC2DB4_CA76_483B_97C6_EB4C848ABDB2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DCR_PointFeatureAddress.h"
#include "DCR_LineFeatureAddress.h"
#include "DCR_TinVertexAddress.h"
#include "DCR_TinEdgeAddress.h"

class CDCR_Cells  
{
public:
	CDCR_Cells();
	virtual ~CDCR_Cells();
	
	bool	m_bUsed;

	//
	CDCR_PointFeatureAddress	*m_pPFDHead;
	CDCR_PointFeatureAddress	*m_pPFDLast;
	
	//
	CDCR_LineFeatureAddress		*m_pLFDHead;
	CDCR_LineFeatureAddress		*m_pLFDLast;

	//顶点信息
	CDCR_TinVertexAddress		*m_pTVDHead;
	CDCR_TinVertexAddress		*m_pTVDLast;

	//边信息
	CDCR_TinEdgeAddress			*m_pTEDHead;
	CDCR_TinEdgeAddress			*m_pTEDLast;
};

#endif // !defined(AFX_DCR_CELLS_H__59DC2DB4_CA76_483B_97C6_EB4C848ABDB2__INCLUDED_)
