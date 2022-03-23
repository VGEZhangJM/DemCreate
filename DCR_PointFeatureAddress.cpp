// DCR_PointFeatureAddress.cpp: implementation of the CDCR_PointFeatureAddress class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DCR_PointFeatureAddress.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDCR_PointFeatureAddress::CDCR_PointFeatureAddress()
{
	//
	m_pNext			= NULL;
	//
	m_pPointFeature	= NULL;
}

CDCR_PointFeatureAddress::~CDCR_PointFeatureAddress()
{

}

//
CDCR_PointFeature* CDCR_PointFeatureAddress::GetPointFeature()
{
	return	m_pPointFeature;
}

//
void CDCR_PointFeatureAddress::SetPointFeature(CDCR_PointFeature *pPointFeature)
{
	m_pPointFeature	= pPointFeature;
}