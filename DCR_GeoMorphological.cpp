// DCR_GeoMorphological.cpp: implementation of the CDCR_GeoMorphological class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "vr_demcreator.h"
#include "DCR_GeoMorphological.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDCR_GeoMorphological::CDCR_GeoMorphological()
{
	m_pDemObj		= NULL;

	m_iWindowSize	= 5;
}

CDCR_GeoMorphological::~CDCR_GeoMorphological()
{

}

//传入数据
void CDCR_GeoMorphological::SetGridDem(double *pData,int iNumOfX,int iNumOfY,double dSizeOfX,double dSizeOfY,double dRealSize,double dLeft,double dRight,double dBottom,double dTop,CString szPathName)
{
	//
	m_pDemObj		= pData;

	//
	m_iNumOfX		= iNumOfX;
	m_iNumOfY		= iNumOfY;

	//
	m_dDemLeft		= dLeft;
	m_dDemRight		= dRight;
	m_dDemBottom	= dBottom;
	m_dDemTop		= dTop;

	//
	m_dSizeOfX		= dSizeOfX;
	m_dSizeOfY		= dSizeOfY;

	//
	m_dRealSize		= dRealSize;

	//
	m_szPathName	= szPathName;	
}


//分析窗口尺寸
void CDCR_GeoMorphological::SetWindowSize(int iSize)
{
	m_iWindowSize	= iSize;
}

int CDCR_GeoMorphological::GetWindowSize()
{
	return	m_iWindowSize;
}

//判断地貌类型
bool CDCR_GeoMorphological::OnJudge(int iType)
{
	m_iGeoType	= 0;

	if(iType == 1)
	{
		m_iGeoType	= 1;

		//
		m_pGeoCZX.SetGridDem(m_pDemObj,m_iNumOfX,m_iNumOfY,m_dSizeOfX,m_dSizeOfY,m_dRealSize,
			m_dDemLeft,m_dDemRight,m_dDemBottom,m_dDemTop,m_szPathName);

		//
		m_pGeoCZX.SetWindowSize(m_iWindowSize);

		//
		return	m_pGeoCZX.Judge();
	}
	
	if(iType == 2)
	{
		m_iGeoType	= 2;

		//
		m_pGeoHXY.SetGridDem(m_pDemObj,m_iNumOfX,m_iNumOfY,m_dSizeOfX,m_dSizeOfY,m_dRealSize,
			m_dDemLeft,m_dDemRight,m_dDemBottom,m_dDemTop,m_szPathName);

		//
		m_pGeoHXY.SetWindowSize(m_iWindowSize);

		//
		return	m_pGeoHXY.Judge();
	}

	if(iType == 3)
	{
		m_iGeoType	= 3;

		//
		m_pGeoLSD.SetGridDem(m_pDemObj,m_iNumOfX,m_iNumOfY,m_dSizeOfX,m_dSizeOfY,m_dRealSize,
			m_dDemLeft,m_dDemRight,m_dDemBottom,m_dDemTop,m_szPathName);

		//
		m_pGeoLSD.SetWindowSize(m_iWindowSize);

		//
		return	m_pGeoLSD.Judge();
	}

	return	false;
}

//
void CDCR_GeoMorphological::SetCoordMapping(double dXA,double dXB,double dYA,double dYB)
{
	m_dMapXA	= dXA;
	m_dMapXB	= dXB;
	m_dMapYA	= dYA;
	m_dMapYB	= dYB;
}

//
void CDCR_GeoMorphological::SetClientRect(CRect rRect)
{
	m_rRect	= rRect;
}

//
void CDCR_GeoMorphological::OnDraw(CDC *pDC)
{
	if(m_iGeoType == 1)
	{
		//
		m_pGeoCZX.SetClientRect(m_rRect);
		m_pGeoCZX.SetCoordMapping(m_dMapXA,m_dMapXB,m_dMapYA,m_dMapYB);
		
		m_pGeoCZX.Draw(pDC);
	}

	if(m_iGeoType == 2)
	{
		//
		m_pGeoHXY.SetClientRect(m_rRect);
		m_pGeoHXY.SetCoordMapping(m_dMapXA,m_dMapXB,m_dMapYA,m_dMapYB);
		
		m_pGeoHXY.Draw(pDC);
	}

	if(m_iGeoType == 3)
	{
		//
		m_pGeoLSD.SetClientRect(m_rRect);
		m_pGeoLSD.SetCoordMapping(m_dMapXA,m_dMapXB,m_dMapYA,m_dMapYB);
		
		m_pGeoLSD.Draw(pDC);
	}
}


//
void CDCR_GeoMorphological::OnRelease()
{
	m_pGeoCZX.Release();
	m_pGeoHXY.Release();
	m_pGeoLSD.Release();
}

//
bool CDCR_GeoMorphological::OnCapture(double dX,double dY)
{
	//m_pDemObj.
	m_pGeoLSD.SetGridDem(m_pDemObj,m_iNumOfX,m_iNumOfY,m_dSizeOfX,m_dSizeOfY,m_dRealSize,
		m_dDemLeft,m_dDemRight,m_dDemBottom,m_dDemTop,m_szPathName);

	m_pGeoLSD.OnCapture(dX,dY);

	return	true;
}