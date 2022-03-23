// DCR_HeightSet.cpp: implementation of the CDCR_HeightSet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "vr_demcreator.h"
#include "DCR_HeightSet.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CDCR_HeightSet* CDCR_HeightSet::m_pInstance = NULL;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDCR_HeightSet::CDCR_HeightSet()
{
	//把高程设置为9层
	m_iLayerCount	= 19;
	

	//那么高程分级为20
	m_dLayerHeight[0]	= -4000;
	m_dLayerHeight[1]	= -2000;
	m_dLayerHeight[2]	= -1000;
	m_dLayerHeight[3]	= -200;
	m_dLayerHeight[4]	= -20;
	m_dLayerHeight[5]	= 0;
	m_dLayerHeight[6]	= 50;
	m_dLayerHeight[7]	= 100;
	m_dLayerHeight[8]	= 200;
	m_dLayerHeight[9]	= 500;
	m_dLayerHeight[10]	= 1000;
	m_dLayerHeight[11]	= 1500;
	m_dLayerHeight[12]	= 2500;
	m_dLayerHeight[13]	= 3000;
	m_dLayerHeight[14]	= 4000;
	m_dLayerHeight[15]	= 5000;
	m_dLayerHeight[16]	= 6000;
	m_dLayerHeight[17]	= 9000;
	
	//
	m_crLayerColor[0]	= RGB(76,142,191);	//-4000
	m_crLayerColor[1]	= RGB(100,159,204);	//-2000
	m_crLayerColor[2]	= RGB(126,177,217);	//-1000
	m_crLayerColor[3]	= RGB(154,197,230);	//-200
	m_crLayerColor[4]	= RGB(184,217,243);	//-20
	m_crLayerColor[5]	= RGB(160,204,126);	//-0
	m_crLayerColor[6]	= RGB(160,200,133);	//50
	m_crLayerColor[7]	= RGB(165,195,140);	//100
	m_crLayerColor[8]	= RGB(157,180,130);	//200
	m_crLayerColor[9]	= RGB(149,169,101);	//500
	m_crLayerColor[10]	= RGB(159,165,105);	//1000
	m_crLayerColor[11]	= RGB(200,205,150);	//1500
	m_crLayerColor[12]	= RGB(241,232,204);	//2500
	m_crLayerColor[13]	= RGB(221,198,142);	//3000
	m_crLayerColor[14]	= RGB(213,180,120);	//4000
	m_crLayerColor[15]	= RGB(229,213,186);	//5000
	m_crLayerColor[16]	= RGB(245,238,255);	//6000
	m_crLayerColor[17]	= RGB(231,227,255);	//9000
}

CDCR_HeightSet::~CDCR_HeightSet()
{

}

CDCR_HeightSet* CDCR_HeightSet::Instance()
{
	if(m_pInstance == NULL)
	{
		m_pInstance	= new	CDCR_HeightSet;
	}

	return	m_pInstance;
}

void CDCR_HeightSet::UnInstance()
{
	if(m_pInstance != NULL)
	{
		delete	m_pInstance;

		m_pInstance	= NULL;
	}
}

//////////////////////////////////////////////////////////////////////
// 根据高程信息获取相应的颜色信息
//////////////////////////////////////////////////////////////////////
COLORREF CDCR_HeightSet::GetColor(double dH)
{
	if(dH < m_dLayerHeight[0])		return	m_crLayerColor[0];

	if(dH > m_dLayerHeight[17])		return	m_crLayerColor[17];
	
	//
	double		dMinH,dMaxH;
	COLORREF	crMinColor,crMaxColor;
	int			iMinR,iMinG,iMinB;
	int			iMaxR,iMaxG,iMaxB;
	int			iR,iG,iB;
	
	for(int i=0;i<17;i++)
	{
		if(dH>=m_dLayerHeight[i] && dH<m_dLayerHeight[i+1])
		{
			//
			dMinH		= m_dLayerHeight[i];
			dMaxH		= m_dLayerHeight[i+1];

			crMinColor	= m_crLayerColor[i];
			crMaxColor	= m_crLayerColor[i+1];

			iMinR		= GetRValue(crMinColor);
			iMinG		= GetGValue(crMinColor);
			iMinB		= GetBValue(crMinColor);

			iMaxR		= GetRValue(crMaxColor);
			iMaxG		= GetGValue(crMaxColor);
			iMaxB		= GetBValue(crMaxColor);
		
			iR			= (int)((dH-dMinH)/(dMaxH-dMinH)*(iMaxR-iMinR)+iMinR +0.5);
			iG			= (int)((dH-dMinH)/(dMaxH-dMinH)*(iMaxG-iMinG)+iMinG +0.5);
			iB			= (int)((dH-dMinH)/(dMaxH-dMinH)*(iMaxB-iMinB)+iMinB +0.5);

			return	RGB(iR,iG,iB);
		}
	}
	
	return	RGB(0,0,0);
}
