// DCR_TinVertexAddress.h: interface for the CDCR_TinVertexAddress class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCR_TINVERTEXADDRESS_H__3419158E_C673_4AE1_AC00_640A520DCB7B__INCLUDED_)
#define AFX_DCR_TINVERTEXADDRESS_H__3419158E_C673_4AE1_AC00_640A520DCB7B__INCLUDED_

#include "DCR_TinVertex.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDCR_TinVertexAddress  
{
public:
	CDCR_TinVertexAddress();
	virtual ~CDCR_TinVertexAddress();

	//
	void						SetVertex(CDCR_TinVertex *pVertex);
	//
	CDCR_TinVertex*				GetVertex();

public:
	//
	CDCR_TinVertexAddress		*m_pNext;
	//
	CDCR_TinVertex				*m_pVertex;
};

#endif // !defined(AFX_DCR_TINVERTEXADDRESS_H__3419158E_C673_4AE1_AC00_640A520DCB7B__INCLUDED_)
