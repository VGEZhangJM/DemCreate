// DCR_TinEdgeAddress.h: interface for the CDCR_TinEdgeAddress class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCR_TINEDGEADDRESS_H__6EFD811A_4F37_4FBE_97B9_D34AF475FAC8__INCLUDED_)
#define AFX_DCR_TINEDGEADDRESS_H__6EFD811A_4F37_4FBE_97B9_D34AF475FAC8__INCLUDED_

#include "DCR_TinEdge.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDCR_TinEdgeAddress  
{
public:
	CDCR_TinEdgeAddress();
	virtual ~CDCR_TinEdgeAddress();

	//
	void						SetEdge(CDCR_TinEdge *pEdge);
	//
	CDCR_TinEdge*				GetEdge();

public:
	bool						m_bUsed;
	//
	CDCR_TinEdgeAddress			*m_pNext;
	//
	CDCR_TinEdge				*m_pEdge;
};

#endif // !defined(AFX_DCR_TINEDGEADDRESS_H__6EFD811A_4F37_4FBE_97B9_D34AF475FAC8__INCLUDED_)
