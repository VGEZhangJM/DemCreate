// DCR_Grid_IDW.cpp: implementation of the CDCR_Grid_IDW class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DCR_Grid_IDW.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDCR_Grid_IDW::CDCR_Grid_IDW()
{
	//权指数
	m_iU			= 2;
	//光滑参数
	m_dT			= 0;

	//采样点点数
	m_pVretexList	= NULL;

	m_szPathName	= "D:\\Out(IDW).grd";
}

CDCR_Grid_IDW::~CDCR_Grid_IDW()
{

}

//设置顶点列表
void CDCR_Grid_IDW::SetGridVertexList(CDCR_GridVertexList *pVretexList)
{
	m_pVretexList	= pVretexList;
}

//计算插值的高程值
bool CDCR_Grid_IDW::CalcHeight(double dX,double dY,double& dZ)
{
	if(m_pVretexList == NULL)	return	false;
	
	CDCR_GridVertex	*pVertex;
	
	//如果都大，则通过加权计算
	double	dUpValue	= 0;
	double	dDownValue	= 0;

	double	dWeighted;

	pVertex	= m_pVretexList->GetGridVertex();
	while(pVertex != NULL)
	{	
		//计算d*=d^2+t^2;
		dWeighted	= sqrt(pVertex->m_dDistance*pVertex->m_dDistance + m_dT*m_dT);
		//d*^u
		dWeighted	= pow(dWeighted,(double)m_iU);

		//
		if(dWeighted < 1e-10)	
		{
			dZ		= pVertex->m_dZ;

			return	true;
		}

		//计算（1/(d*^u)）
		dWeighted	= 1.0/dWeighted;

		//计算分子
		dUpValue	= dUpValue + dWeighted * pVertex->m_dZ;	

		//计算分母
		dDownValue	= dDownValue + dWeighted;	
		
		pVertex	= pVertex->m_pNext;
	}	
	
	//if(dDownValue < 1e-10)
	//	dZ	= -9999;
	//else
		//比值计算得到Z值
		dZ	= dUpValue / dDownValue;
	
	return	true;
}

//
void CDCR_Grid_IDW::SetU(int iU)
{
	m_iU	= iU;
}

int CDCR_Grid_IDW::GetU()
{
	return	m_iU;
}

//
void CDCR_Grid_IDW::SetT(double dT)
{
	m_dT	= dT;
}

double CDCR_Grid_IDW::GetT()
{
	return	m_dT;
}

//
void CDCR_Grid_IDW::SetPathName(CString szPathName)
{
	m_szPathName	= szPathName;
}

CString CDCR_Grid_IDW::GetPathName()
{
	return	m_szPathName;
}
