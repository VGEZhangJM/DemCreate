// DCR_PointFeatureAddress.h: interface for the CDCR_PointFeatureAddress class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCR_POINTFEATUREADDRESS_H__1300FCC3_E4D4_43C2_AA46_2CAA5A1DA559__INCLUDED_)
#define AFX_DCR_POINTFEATUREADDRESS_H__1300FCC3_E4D4_43C2_AA46_2CAA5A1DA559__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DCR_PointFeature.h"

class CDCR_PointFeatureAddress  
{
public:
	CDCR_PointFeatureAddress();
	virtual ~CDCR_PointFeatureAddress();
	
	//
	void						SetPointFeature(CDCR_PointFeature *pPointFeature);
	//
	CDCR_PointFeature*			GetPointFeature();

public:
	//
	CDCR_PointFeatureAddress	*m_pNext;
	//
	CDCR_PointFeature			*m_pPointFeature;
};

#endif // !defined(AFX_DCR_POINTFEATUREADDRESS_H__1300FCC3_E4D4_43C2_AA46_2CAA5A1DA559__INCLUDED_)
