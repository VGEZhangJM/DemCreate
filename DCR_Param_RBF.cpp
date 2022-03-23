// DCR_Param_RBF.cpp: implementation of the CDCR_Param_RBF class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "vr_demcreator.h"
#include "DCR_Param_RBF.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDCR_Param_RBF::CDCR_Param_RBF()
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
	m_iPointNumPreDirection	=1;
	
	//
	m_iPointNumInTotal		= 4;

	//�����뾶
	m_dSearchRadius			= 3000;

	//ģ������,
	//1�����������ֵ��
	//2�������������ֵ��
	m_iModelType			= 0;

	//����
	m_iNumOfC				= 0;
	
	for(int i=0;i<20;i++)
	{
		m_dC[i]				= 0;
	}
}

CDCR_Param_RBF::~CDCR_Param_RBF()
{

}

bool CDCR_Param_RBF::Read(FILE *fp)
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

	//Ȩָ��
	fscanf(fp,"%ld",&m_iModelType);
	
	//ƽ������
	fscanf(fp,"%ld",&m_iNumOfC);
	for(int i=0;i<m_iNumOfC;i++)
	{
		double	dC;
		fscanf(fp,"%lf",&dC);
		m_dC[i]	= dC;
	}

	return	true;
}
