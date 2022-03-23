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

	//ģ������,
	//0������ģ��
	//1��ָ��ģ��
	//2����˹ģ��
	m_iModelType			= 0;

	//���
	m_dC0					= 0;
	//��ֵ̨
	m_dC					= 1;
	//���
	m_dAlfa					= 100;
}

CDCR_Param_KRG::~CDCR_Param_KRG()
{

}

bool CDCR_Param_KRG::Read(FILE *fp)
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

	//ģ������
	fscanf(fp,"%ld",&m_iModelType);

	//���
	fscanf(fp,"%lf",&m_dC0);
	
	//��̨
	fscanf(fp,"%lf",&m_dC);

	//���
	fscanf(fp,"%lf",&m_dAlfa);

	return	true;
}

