// DCR_Param_LTC.cpp: implementation of the CDCR_Param_LTC class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "vr_demcreator.h"
#include "DCR_Param_LTC.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDCR_Param_LTC::CDCR_Param_LTC()
{
	m_iSearchType			= 0;
	
	//
	m_iPointNumPreDirection	= 1;
	
	//
	m_iPointNumInTotal		= 4;

	//
	m_dSearchRadius			= 30000;
}

CDCR_Param_LTC::~CDCR_Param_LTC()
{
	
}

//
bool CDCR_Param_LTC::Read(FILE *fp)
{
	if(fp == NULL)	return	false;

	//������ʽ
	//0:Ϊȫ�����������޷�������
	//1:Ϊ�ķ�������
	//2:Ϊ�˷�������
	fscanf(fp,"%ld",&m_iSearchType);
	
	//
	fscanf(fp,"%ld",&m_iPointNumPreDirection);
	
	//
	fscanf(fp,"%ld",&m_iPointNumInTotal);

	//
	fscanf(fp,"%lf",&m_dSearchRadius);

	return	true;	
}
