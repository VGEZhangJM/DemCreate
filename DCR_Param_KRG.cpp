// DCR_Param_KRG.cpp: implementation of the CDCR_Param_KRG class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "vr_demcreator.h"
#include "DCR_Param_KRG.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDCR_Param_KRG::CDCR_Param_KRG()
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

	//模型种类,
	//0：球型模型
	//1：指数模型
	//2：高斯模型
	m_iModelType			= 0;

	//块金
	m_dC0					= 0;
	//基台值
	m_dC					= 1;
	//变程
	m_dAlfa					= 100;
}

CDCR_Param_KRG::~CDCR_Param_KRG()
{

}

bool CDCR_Param_KRG::Read(FILE *fp)
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

	//模型种类
	fscanf(fp,"%ld",&m_iModelType);

	//块金
	fscanf(fp,"%lf",&m_dC0);
	
	//基台
	fscanf(fp,"%lf",&m_dC);

	//变程
	fscanf(fp,"%lf",&m_dAlfa);

	return	true;
}

