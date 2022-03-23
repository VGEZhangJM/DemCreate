// DCR_SlopeSet.cpp: implementation of the CDCR_SlopeSet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DCR_SlopeSet.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CDCR_SlopeSet* CDCR_SlopeSet::m_pInstance = NULL;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDCR_SlopeSet::CDCR_SlopeSet()
{
	//
	m_dMinSlope		= 0;
	//
	m_dMidSlope		= 0;
	//
	m_dMaxSlope		= 0;

	//
	m_iMaxR			= 255;
	m_iMaxG			= 255;
	m_iMaxB			= 255;

	//
	m_iMidR			= 122;
	m_iMidG			= 122;
	m_iMidB			= 122;

	//
	m_iMinR			= 0;
	m_iMinG			= 0;
	m_iMinB			= 0;
}

CDCR_SlopeSet::~CDCR_SlopeSet()
{

}


CDCR_SlopeSet* CDCR_SlopeSet::Instance()
{
	if(m_pInstance == NULL)
	{
		m_pInstance	= new	CDCR_SlopeSet;
	}

	return	m_pInstance;
}

void CDCR_SlopeSet::UnInstance()
{
	if(m_pInstance != NULL)
	{
		delete	m_pInstance;

		m_pInstance	= NULL;
	}
}

//通过坡度值获取颜色值
COLORREF CDCR_SlopeSet::GetColor(double dSlope)
{
	if(dSlope < m_dMinSlope)	return	RGB(m_iMinR,m_iMinG,m_iMinB);

	if(dSlope > m_dMaxSlope)	return	RGB(m_iMaxR,m_iMaxG,m_iMaxB);
	
	if(dSlope>=m_dMinSlope && dSlope<m_dMidSlope)	
	{
		int		iR,iG,iB;

		iR			= (int)((dSlope-m_dMinSlope)/(m_dMidSlope-m_dMinSlope)*(m_iMidR-m_iMinR)+m_iMinR +0.5);
		iG			= (int)((dSlope-m_dMinSlope)/(m_dMidSlope-m_dMinSlope)*(m_iMidG-m_iMinG)+m_iMinG +0.5);
		iB			= (int)((dSlope-m_dMinSlope)/(m_dMidSlope-m_dMinSlope)*(m_iMidB-m_iMinB)+m_iMinB +0.5);		

		return	RGB(iR,iG,iB);
	}
	else
	{
		int		iR,iG,iB;

		iR			= (int)((dSlope-m_dMidSlope)/(m_dMaxSlope-m_dMidSlope)*(m_iMaxR-m_iMidR)+m_iMidR +0.5);
		iG			= (int)((dSlope-m_dMidSlope)/(m_dMaxSlope-m_dMidSlope)*(m_iMaxG-m_iMidG)+m_iMidG +0.5);
		iB			= (int)((dSlope-m_dMidSlope)/(m_dMaxSlope-m_dMidSlope)*(m_iMaxB-m_iMidB)+m_iMidB +0.5);		
		
		return	RGB(iR,iG,iB);
	}
}

//
void CDCR_SlopeSet::SetMinSlope(double dMinSlope)
{
	//
	m_dMinSlope	= dMinSlope;
}

//
void CDCR_SlopeSet::SetMidSlope(double dMidSlope)
{
	m_dMidSlope	= dMidSlope;
}

//
void CDCR_SlopeSet::SetMaxSlope(double dMaxSlope)
{
	m_dMaxSlope	= dMaxSlope;
}