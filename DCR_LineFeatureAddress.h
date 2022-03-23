// DCR_LineFeatureAddress.h: interface for the CDCR_LineFeatureAddress class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCR_LINEFEATUREADDRESS_H__7CF54CA1_B5A6_4D40_9D9E_56435E123523__INCLUDED_)
#define AFX_DCR_LINEFEATUREADDRESS_H__7CF54CA1_B5A6_4D40_9D9E_56435E123523__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DCR_LineFeature.h"

class CDCR_LineFeatureAddress  
{
public:
	CDCR_LineFeatureAddress();
	virtual ~CDCR_LineFeatureAddress();
	
	//
	void					SetLineFeature(CDCR_LineFeature *pLineFeature);
	//
	CDCR_LineFeature*		GetLineFeature();

public:
	//
	CDCR_LineFeatureAddress		*m_pNext;
	//
	CDCR_LineFeature			*m_pLineFeature;
};

#endif // !defined(AFX_DCR_LINEFEATUREADDRESS_H__7CF54CA1_B5A6_4D40_9D9E_56435E123523__INCLUDED_)
