// DCR_DemSet.cpp: implementation of the CDCR_DemSet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "vr_demcreator.h"
#include "DCR_DemSet.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//
CDCR_DemSet* CDCR_DemSet::m_pInstance = NULL;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDCR_DemSet::CDCR_DemSet()
{
	//
	m_dMinHeight		= 0;
	//
	m_dMidHeight		= 0;
	//
	m_dMaxHeight		= 0;

	//
	m_iMaxR			= 254;
	m_iMaxG			= 182;
	m_iMaxB			= 129;

	//
	m_iMidR			= 250;
	m_iMidG			= 240;
	m_iMidB			= 190;

	//
	m_iMinR			= 95;
	m_iMinG			= 165;
	m_iMinB			= 111;
}

CDCR_DemSet::~CDCR_DemSet()
{

}


CDCR_DemSet* CDCR_DemSet::Instance()
{
	if(m_pInstance == NULL)
	{
		m_pInstance	= new	CDCR_DemSet;
	}

	return	m_pInstance;
}

void CDCR_DemSet::UnInstance()
{
	if(m_pInstance != NULL)
	{
		delete	m_pInstance;

		m_pInstance	= NULL;
	}
}

//通过坡度值获取颜色值
COLORREF CDCR_DemSet::GetColor(double dHeight)
{
	if(dHeight < m_dMinHeight)	return	RGB(m_iMinR,m_iMinG,m_iMinB);

	if(dHeight > m_dMaxHeight)	return	RGB(m_iMaxR,m_iMaxG,m_iMaxB);
	
	if(dHeight>=m_dMinHeight && dHeight<m_dMidHeight)	
	{
		int		iR,iG,iB;

		iR			= (int)((dHeight-m_dMinHeight)/(m_dMidHeight-m_dMinHeight)*(m_iMidR-m_iMinR)+m_iMinR +0.5);
		iG			= (int)((dHeight-m_dMinHeight)/(m_dMidHeight-m_dMinHeight)*(m_iMidG-m_iMinG)+m_iMinG +0.5);
		iB			= (int)((dHeight-m_dMinHeight)/(m_dMidHeight-m_dMinHeight)*(m_iMidB-m_iMinB)+m_iMinB +0.5);		

		return	RGB(iR,iG,iB);
	}
	else
	{
		int		iR,iG,iB;

		iR			= (int)((dHeight-m_dMidHeight)/(m_dMaxHeight-m_dMidHeight)*(m_iMaxR-m_iMidR)+m_iMidR +0.5);
		iG			= (int)((dHeight-m_dMidHeight)/(m_dMaxHeight-m_dMidHeight)*(m_iMaxG-m_iMidG)+m_iMidG +0.5);
		iB			= (int)((dHeight-m_dMidHeight)/(m_dMaxHeight-m_dMidHeight)*(m_iMaxB-m_iMidB)+m_iMidB +0.5);		
		
		return	RGB(iR,iG,iB);
	}
}

//
void CDCR_DemSet::SetMinHeight(double dMinHeight)
{
	//
	m_dMinHeight	= dMinHeight;
}

//
void CDCR_DemSet::SetMidHeight(double dMidHeight)
{
	m_dMidHeight	= dMidHeight;
}

//
void CDCR_DemSet::SetMaxHeight(double dMaxHeight)
{
	m_dMaxHeight	= dMaxHeight;
}