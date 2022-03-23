// DCR_LocalTerrain.cpp: implementation of the CDCR_LocalTerrain class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "vr_demcreator.h"
#include "DCR_LocalTerrain.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDCR_LocalTerrain::CDCR_LocalTerrain()
{

}

CDCR_LocalTerrain::~CDCR_LocalTerrain()
{
	m_pData.RemoveAll();
}

//
void CDCR_LocalTerrain::OnRead(CString szPathName)
{
	m_szPathName	= szPathName;

	FILE	*fp;

	fp	= fopen(szPathName,"r");

	if(fp != NULL)
	{
		char	chName[32];
		//"点格局","平均坡度","残差","最小距离","X坐标","Y坐标"
		fscanf(fp,"%s%s%s%s%s%s",chName,chName,chName,chName,chName,chName);

		double	dPointPattern,dMeanSlope,dH,dCenriod,dMaxDistance,dMinDistance,dRough,dX,dY;
		dPointPattern	= 0;
		dMeanSlope		= 0;
		dH				= 0;
		dCenriod		= 0;
		dMaxDistance	= 0;
		dMinDistance	= 0;
		dRough			= 0;

		while(fscanf(fp,"%lf%lf%lf%lf%lf%lf",&dPointPattern,&dMeanSlope,&dH,&dMinDistance,&dX,&dY) == 6)
		{
			tagLOCALTERRAIN	pTerrain;

			pTerrain.dPointPattern	= dPointPattern;
			pTerrain.dMeanSlope		= dMeanSlope;
			pTerrain.dH				= dH;

			pTerrain.dCenriod		= dCenriod;
			pTerrain.dMaxDistance	= dMaxDistance;
			pTerrain.dMinDistance	= dMinDistance;
			pTerrain.dMeanRough		= dRough;
			pTerrain.dX				= dX;
			pTerrain.dY				= dY;

			pTerrain.dValue			= -1.0;

			//
			m_pData.Add(pTerrain);
		}

		fclose(fp);
	}
}

//
bool CDCR_LocalTerrain::OnSearchPoint(double dPointPattern,double dMeanSlope,double dH, int iNum)
{
	if(m_pData.GetSize() == 0)	return	false;
	
	//
	m_pSelectData.RemoveAll();


	for(int i=0;i<iNum;i++)
	{
		double	dPP,dMS,dDH,dCenriod,dMaxDistance,dMinDistance,dRough,dX,dY;

		dPP				= m_pData[i].dPointPattern;
		dMS				= m_pData[i].dMeanSlope;
		dDH				= m_pData[i].dH;

		dCenriod		= m_pData[i].dCenriod;
		dMaxDistance	= m_pData[i].dMaxDistance;
		dMinDistance	= m_pData[i].dMinDistance;
		dRough			= m_pData[i].dMeanRough;
		dX				= m_pData[i].dX;
		dY				= m_pData[i].dY;


		tagLOCALTERRAIN	pTerrain;
		pTerrain.dPointPattern	= dPP;
		pTerrain.dMeanSlope		= dMS;
		pTerrain.dH				= dDH;

		pTerrain.dCenriod		= dCenriod;
		pTerrain.dMaxDistance	= dMaxDistance;
		pTerrain.dMinDistance	= dMinDistance;
		pTerrain.dMeanRough		= dRough;
		pTerrain.dX				= dX;
		pTerrain.dY				= dY;

		pTerrain.dValue			= (dPP-dPointPattern)*(dPP-dPointPattern)+(dMS-dMeanSlope)*(dMS-dMeanSlope)+(dDH-dH)*(dDH-dH);

		//
		m_pSelectData.Add(pTerrain);
	}

	for(i=iNum;i<m_pData.GetSize();i++)
	{
		double	dMyD;

		dMyD	= (m_pData[i].dPointPattern-dPointPattern)*(m_pData[i].dPointPattern-dPointPattern)
				+(m_pData[i].dMeanSlope-dMeanSlope)*(m_pData[i].dMeanSlope-dMeanSlope)
				+(m_pData[i].dH-dH)*(m_pData[i].dH-dH);
		
		double	dMaxD;
		int		iCurrentI;
		for(int j=0;j<m_pSelectData.GetSize();j++)
		{
			if(j==0)	
			{
				dMaxD		= m_pSelectData[j].dValue;
				iCurrentI	= j;
			}
			else
			{
				if(dMaxD < m_pSelectData[j].dValue)
				{
					dMaxD		= m_pSelectData[j].dValue;
					iCurrentI	= j;
				}
			}
		}

		//更新
		if(dMaxD > dMyD)
		{
			double	dPP,dMS,dDH,dCenriod,dMaxDistance,dMinDistance,dRough,dX,dY;

			dPP				= m_pData[i].dPointPattern;
			dMS				= m_pData[i].dMeanSlope;
			dDH				= m_pData[i].dH;

			dCenriod		= m_pData[i].dCenriod;
			dMaxDistance	= m_pData[i].dMaxDistance;
			dMinDistance	= m_pData[i].dMinDistance;
			dRough			= m_pData[i].dMeanRough;
			dX				= m_pData[i].dX;
			dY				= m_pData[i].dY;

			m_pSelectData[iCurrentI].dPointPattern	= dPP;
			m_pSelectData[iCurrentI].dMeanSlope		= dMS;
			m_pSelectData[iCurrentI].dH				= dDH;

			m_pSelectData[iCurrentI].dCenriod		= dCenriod;
			m_pSelectData[iCurrentI].dMaxDistance	= dMaxDistance;
			m_pSelectData[iCurrentI].dMinDistance	= dMinDistance;
			m_pSelectData[iCurrentI].dMeanRough		= dRough;
			m_pSelectData[iCurrentI].dX				= dX;
			m_pSelectData[iCurrentI].dY				= dY;
			m_pSelectData[iCurrentI].dValue			= dMyD;
		}
	}

	return	true;
}

//
int CDCR_LocalTerrain::GetSelectNum()
{
	return	m_pSelectData.GetSize();
}

//
bool CDCR_LocalTerrain::GetSelectPoint(int iSerial,double& dX,double& dY,double& dDH,double& dMaxDistance,double& dMinDistance)
{
	if(iSerial<0 || iSerial>=m_pSelectData.GetSize())	return	false;
	
	dX				= m_pSelectData[iSerial].dX;
	dY				= m_pSelectData[iSerial].dY;
	dDH				= m_pSelectData[iSerial].dH;
	dMaxDistance	= m_pSelectData[iSerial].dMaxDistance;
	dMinDistance	= m_pSelectData[iSerial].dMinDistance;

	return	true;
}