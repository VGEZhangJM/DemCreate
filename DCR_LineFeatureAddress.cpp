// DCR_LineFeatureAddress.cpp: implementation of the CDCR_LineFeatureAddress class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DCR_LineFeatureAddress.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDCR_LineFeatureAddress::CDCR_LineFeatureAddress()
{
	//
	m_pNext			= NULL;
	//
	m_pLineFeature	= NULL;
}

CDCR_LineFeatureAddress::~CDCR_LineFeatureAddress()
{

}

//
CDCR_LineFeature* CDCR_LineFeatureAddress::GetLineFeature()
{
	return	m_pLineFeature;
}

//
void CDCR_LineFeatureAddress::SetLineFeature(CDCR_LineFeature *pLineFeature)
{
	m_pLineFeature	= pLineFeature;
}
