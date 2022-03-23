// DCR_AspectSet.cpp: implementation of the CDCR_AspectSet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "vr_demcreator.h"
#include "DCR_AspectSet.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CDCR_AspectSet* CDCR_AspectSet::m_pInstance = NULL;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDCR_AspectSet::CDCR_AspectSet()
{
	//
	m_dMinAspect		= 0;
	//
	m_dMidAspect		= 0;
	//
	m_dMaxAspect		= 0;
	
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

CDCR_AspectSet::~CDCR_AspectSet()
{
	
}


CDCR_AspectSet* CDCR_AspectSet::Instance()
{
	if(m_pInstance == NULL)
	{
		m_pInstance	= new	CDCR_AspectSet;
	}
	
	return	m_pInstance;
}

void CDCR_AspectSet::UnInstance()
{
	if(m_pInstance != NULL)
	{
		delete	m_pInstance;
		
		m_pInstance	= NULL;
	}
}

//通过坡度值获取颜色值
COLORREF CDCR_AspectSet::GetColor(double dAspect)
{
	if(dAspect < m_dMinAspect)	return	RGB(m_iMinR,m_iMinG,m_iMinB);
	
	if(dAspect > m_dMaxAspect)	return	RGB(m_iMaxR,m_iMaxG,m_iMaxB);
	
	if(dAspect>=m_dMinAspect && dAspect<m_dMidAspect)	
	{
		int		iR,iG,iB;
		
		iR			= (int)((dAspect-m_dMinAspect)/(m_dMidAspect-m_dMinAspect)*(m_iMidR-m_iMinR)+m_iMinR +0.5);
		iG			= (int)((dAspect-m_dMinAspect)/(m_dMidAspect-m_dMinAspect)*(m_iMidG-m_iMinG)+m_iMinG +0.5);
		iB			= (int)((dAspect-m_dMinAspect)/(m_dMidAspect-m_dMinAspect)*(m_iMidB-m_iMinB)+m_iMinB +0.5);		
		
		return	RGB(iR,iG,iB);
	}
	else
	{
		int		iR,iG,iB;
		
		iR			= (int)((dAspect-m_dMidAspect)/(m_dMaxAspect-m_dMidAspect)*(m_iMaxR-m_iMidR)+m_iMidR +0.5);
		iG			= (int)((dAspect-m_dMidAspect)/(m_dMaxAspect-m_dMidAspect)*(m_iMaxG-m_iMidG)+m_iMidG +0.5);
		iB			= (int)((dAspect-m_dMidAspect)/(m_dMaxAspect-m_dMidAspect)*(m_iMaxB-m_iMidB)+m_iMidB +0.5);		
		
		return	RGB(iR,iG,iB);
	}
}

//
void CDCR_AspectSet::SetMinAspect(double dMinAspect)
{
	//
	m_dMinAspect	= dMinAspect;
}

//
void CDCR_AspectSet::SetMidAspect(double dMidAspect)
{
	m_dMidAspect	= dMidAspect;
}

//
void CDCR_AspectSet::SetMaxAspect(double dMaxAspect)
{
	m_dMaxAspect	= dMaxAspect;
}
