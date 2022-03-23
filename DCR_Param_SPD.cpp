// DCR_Param_SPD.cpp: implementation of the CDCR_Param_SPD class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "vr_demcreator.h"
#include "DCR_Param_SPD.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDCR_Param_SPD::CDCR_Param_SPD()
{
	//DEM������Ŀ
	m_iDemNumX			= 301;
	//
	m_iDemNumY			= 301;
	
	//������ʽ
	//0:Ϊȫ�����������޷�������
	//1:Ϊ�ķ�������
	//2:Ϊ�˷�������
	m_iSearchType		= 0;
	
	//
	m_iPointNumPreDirection	= 1;
	
	//
	m_iPointNumInTotal		= 4;

	//�����뾶
	m_dSearchRadius			= 30000;

	//Ȩָ��
	m_iNumOfU				= 0;
	for(int i=0;i<5;i++)
		m_iU[i]				= 1;

	//�⻬����
	m_dT					= 0;

	//
	m_iRadiusType			= 0;
	//
	m_dMaxRadius			= 0;

	//
	m_iKernalType			= 0;
}

CDCR_Param_SPD::~CDCR_Param_SPD()
{

}

bool CDCR_Param_SPD::Read(FILE *fp)
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

	//
	fscanf(fp,"%ld",&m_iRadiusType);

	//
	fscanf(fp,"%lf",&m_dMaxRadius);

	//��ֵ�˺���
	fscanf(fp,"%ld",&m_iKernalType);

	return	true;
}
