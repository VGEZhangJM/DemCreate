// DCR_CurvatureSet.cpp: implementation of the CDCR_CurvatureSet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "vr_demcreator.h"
#include "DCR_CurvatureSet.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CDCR_CurvatureSet* CDCR_CurvatureSet::m_pInstance = NULL;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDCR_CurvatureSet::CDCR_CurvatureSet()
{
	//
	m_dMinCurvature	= 0;
	//
	m_dMidCurvature	= 0;
	//
	m_dMaxCurvature	= 0;

	//
	m_iMaxR			= 255;
	m_iMaxG			= 0;
	m_iMaxB			= 0;

	//
	m_iMidR			= 255;
	m_iMidG			= 255;
	m_iMidB			= 255;

	//
	m_iMinR			= 0;
	m_iMinG			= 0;
	m_iMinB			= 0;
}

CDCR_CurvatureSet::~CDCR_CurvatureSet()
{

}


CDCR_CurvatureSet* CDCR_CurvatureSet::Instance()
{
	if(m_pInstance == NULL)
	{
		m_pInstance	= new	CDCR_CurvatureSet;
	}

	return	m_pInstance;
}

void CDCR_CurvatureSet::UnInstance()
{
	if(m_pInstance != NULL)
	{
		delete	m_pInstance;

		m_pInstance	= NULL;
	}
}

//通过坡度值获取颜色值
COLORREF CDCR_CurvatureSet::GetColor(double dCurvature)
{
	if(dCurvature < m_dMinCurvature)	return	RGB(m_iMinR,m_iMinG,m_iMinB);

	if(dCurvature > m_dMaxCurvature)	return	RGB(m_iMaxR,m_iMaxG,m_iMaxB);
	
	if(dCurvature>=m_dMinCurvature && dCurvature<m_dMidCurvature)	
	{
		int		iR,iG,iB;

		iR			= (int)((dCurvature-m_dMinCurvature)/(m_dMidCurvature-m_dMinCurvature)*(m_iMidR-m_iMinR)+m_iMinR +0.5);
		iG			= (int)((dCurvature-m_dMinCurvature)/(m_dMidCurvature-m_dMinCurvature)*(m_iMidG-m_iMinG)+m_iMinG +0.5);
		iB			= (int)((dCurvature-m_dMinCurvature)/(m_dMidCurvature-m_dMinCurvature)*(m_iMidB-m_iMinB)+m_iMinB +0.5);		

		return	RGB(iR,iG,iB);
	}
	else
	{
		int		iR,iG,iB;

		iR			= (int)((dCurvature-m_dMidCurvature)/(m_dMaxCurvature-m_dMidCurvature)*(m_iMaxR-m_iMidR)+m_iMidR +0.5);
		iG			= (int)((dCurvature-m_dMidCurvature)/(m_dMaxCurvature-m_dMidCurvature)*(m_iMaxG-m_iMidG)+m_iMidG +0.5);
		iB			= (int)((dCurvature-m_dMidCurvature)/(m_dMaxCurvature-m_dMidCurvature)*(m_iMaxB-m_iMidB)+m_iMidB +0.5);		
		
		return	RGB(iR,iG,iB);
	}
}

//
void CDCR_CurvatureSet::SetMinCurvature(double dMinCurvature)
{
	//
	m_dMinCurvature	= dMinCurvature;
}

//
void CDCR_CurvatureSet::SetMidCurvature(double dMidCurvature)
{
	m_dMidCurvature	= dMidCurvature;
}

//
void CDCR_CurvatureSet::SetMaxCurvature(double dMaxCurvature)
{
	m_dMaxCurvature	= dMaxCurvature;
}