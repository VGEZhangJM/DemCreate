// DCR_Param_IDW.cpp: implementation of the CDCR_Param_IDW class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "vr_demcreator.h"
#include "DCR_Param_IDW.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDCR_Param_IDW::CDCR_Param_IDW()
{
	//DEM格网数目
	m_iDemNumX				= 301;
	//
	m_iDemNumY				= 301;
	
	//搜索方式
	//0:为全局搜索，即无方向限制
	//1:为四方向搜索
	//2:为八方向搜索
	m_iSearchType			= 0;
	
	//
	m_iPointNumPreDirection	= 1;
	
	//
	m_iPointNumInTotal		= 4;

	//搜索半径
	m_dSearchRadius			= 30000;

	//权指数
	//m_iU					= 2;
	
	//平滑参数
	m_dT					= 0;

	//参数
	m_iNumOfU				= 0;
	for(int i=0;i<5;i++)
	{
		m_iU[i]				= 0;
	}
}

CDCR_Param_IDW::~CDCR_Param_IDW()
{

}

bool CDCR_Param_IDW::Read(FILE *fp)
{
	if(fp == NULL)	return	false;
	
	//DEM格网数目
	fscanf(fp,"%ld",&m_iDemNumX);
	fscanf(fp,"%ld",&m_iDemNumY);
	
	//搜索方式
	//0:为全局搜索，即无方向限制
	//1:为四方向搜索
	//2:为八方向搜索
	fscanf(fp,"%ld",&m_iSearchType);
	
	//
	fscanf(fp,"%ld",&m_iPointNumPreDirection);
	
	//
	fscanf(fp,"%ld",&m_iPointNumInTotal);

	//搜索半径
	fscanf(fp,"%lf",&m_dSearchRadius);
	
	//平滑参数
	fscanf(fp,"%lf",&m_dT);
	
	//权指数
	fscanf(fp,"%ld",&m_iNumOfU);
	for(int i=0;i<m_iNumOfU;i++)
	{
		long	iU;
		fscanf(fp,"%ld",&iU);
		m_iU[i]	= iU;
	}
	
	return	true;
}
