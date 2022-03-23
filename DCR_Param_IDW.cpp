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
	//DEM������Ŀ
	m_iDemNumX				= 301;
	//
	m_iDemNumY				= 301;
	
	//������ʽ
	//0:Ϊȫ�����������޷�������
	//1:Ϊ�ķ�������
	//2:Ϊ�˷�������
	m_iSearchType			= 0;
	
	//
	m_iPointNumPreDirection	= 1;
	
	//
	m_iPointNumInTotal		= 4;

	//�����뾶
	m_dSearchRadius			= 30000;

	//Ȩָ��
	//m_iU					= 2;
	
	//ƽ������
	m_dT					= 0;

	//����
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
	
	//DEM������Ŀ
	fscanf(fp,"%ld",&m_iDemNumX);
	fscanf(fp,"%ld",&m_iDemNumY);
	
	//������ʽ
	//0:Ϊȫ�����������޷�������
	//1:Ϊ�ķ�������
	//2:Ϊ�˷�������
	fscanf(fp,"%ld",&m_iSearchType);
	
	//
	fscanf(fp,"%ld",&m_iPointNumPreDirection);
	
	//
	fscanf(fp,"%ld",&m_iPointNumInTotal);

	//�����뾶
	fscanf(fp,"%lf",&m_dSearchRadius);
	
	//ƽ������
	fscanf(fp,"%lf",&m_dT);
	
	//Ȩָ��
	fscanf(fp,"%ld",&m_iNumOfU);
	for(int i=0;i<m_iNumOfU;i++)
	{
		long	iU;
		fscanf(fp,"%ld",&iU);
		m_iU[i]	= iU;
	}
	
	return	true;
}
