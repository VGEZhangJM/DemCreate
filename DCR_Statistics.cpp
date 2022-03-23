// DCR_Statistics.cpp: implementation of the CDCR_Statistics class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "vr_demcreator.h"
#include "DCR_Statistics.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDCR_Statistics::CDCR_Statistics()
{
	//��������
	m_iCount		= 0;
	//��������
	m_pData			= NULL;

	//ͳ�����ݵļ�ֵ
	m_dDataMin		= m_dDataMax	= 0;

	//ͳ�����ݵľ�ֵ
	m_dDataMean		= 0;

	//ͳ�����ݵķ���,����ƫ��
	m_dDataVar		= 0;

	//ͳ�����ݵı�׼�����ƫ��
	m_dStandardVar	= 0;

	//����ϵ��
	m_dDataCV		= 0;
	//ƫ��ϵ��
	m_dDataSkewness	= 0;
	//���ϵ��
	m_dDataKurtosis	= 0;

	//
	m_dDataRMSE		= 0;

	m_pData			= NULL;
}

CDCR_Statistics::~CDCR_Statistics()
{

}


//////////////////////////////////////////////////////////////////////
//����ͳ������
//////////////////////////////////////////////////////////////////////
void CDCR_Statistics::SetStatisticsData(int iCount,double *pData)
{
	m_iCount	= iCount;

	m_pData		= pData;
}

//////////////////////////////////////////////////////////////////////
//���и�������ͳ��
//////////////////////////////////////////////////////////////////////
bool CDCR_Statistics::Statistics()
{
	//
	if(m_pData == NULL)	return	false;
	
	//��ʼ��
	{
		//ͳ�����ݵļ�ֵ
		m_dDataMin		= m_dDataMax	= 0;

		//ͳ�����ݵľ�ֵ
		m_dDataMean		= 0;

		//ͳ�����ݵķ���,����ƫ��
		m_dDataVar		= 0;

		//ͳ�����ݵı�׼�����ƫ��
		m_dStandardVar	= 0;

		//����ϵ��
		m_dDataCV		= 0;
		//ƫ��ϵ��
		m_dDataSkewness	= 0;
		//���ϵ��
		m_dDataKurtosis	= 0;

		//
		m_dDataRMSE		= 0;
	}

	int		i;
	bool	bFirstValue;

	bFirstValue	= true;

	//��ֵ
	for(i=0;i<m_iCount;i++)
	{
		if(m_pData[i] == INVALID_VALUE)	continue;

		if(m_pData[i] == INVALID_HEIGHT)	continue;

		if(bFirstValue)
		{
			m_dDataMin		= m_dDataMax	= m_pData[i];

			bFirstValue		= false;
		}
		else
		{
			if(m_dDataMin > m_pData[i])		m_dDataMin	= m_pData[i];
			if(m_dDataMax < m_pData[i])		m_dDataMax	= m_pData[i];
		}
	}

	int		iCount;
	iCount	= 0;
	//��ֵ
	for(i=0;i<m_iCount;i++)
	{
		if(m_pData[i] == INVALID_VALUE)	continue;		
		if(m_pData[i] == INVALID_HEIGHT)	continue;

		m_dDataMean	= m_dDataMean + m_pData[i];
		iCount++;
	}

	if(iCount != 0)		m_dDataMean		= m_dDataMean/iCount;
	else				m_dDataMean		= 0;

	//��������
	iCount	= 0;
	for(i=0;i<m_iCount;i++)
	{
		if(m_pData[i] == INVALID_VALUE)	continue;		
		if(m_pData[i] == INVALID_HEIGHT)	continue;

		m_dDataVar	= m_dDataVar + (m_pData[i]-m_dDataMean)*(m_pData[i]-m_dDataMean);
		iCount++;
	}
	if(iCount != 0)		m_dDataVar		= m_dDataVar/(iCount-1);
	else				m_dDataVar		= 0;

	//������׼��
	m_dStandardVar	= sqrt(m_dDataVar);

	//����ϵ��
	m_dDataCV		= m_dStandardVar / m_dDataMean;

	//ƫ��ϵ��
	double	u3	= 0;
	iCount	= 0;
	for(i=0;i<m_iCount;i++)
	{
		if(m_pData[i] == INVALID_VALUE)	continue;
		if(m_pData[i] == INVALID_HEIGHT)	continue;
		
		u3	= u3 + (m_pData[i]-m_dDataMean)*(m_pData[i]-m_dDataMean)*(m_pData[i]-m_dDataMean);
		iCount++;
	}
	if(iCount != 0)		u3		= u3/iCount;
	else				u3		= 0;

	//m_dDataSkewness	= (m_iCount*m_iCount*u3) / ((m_iCount-1)*(m_iCount-2)*m_dStandardVar*m_dStandardVar*m_dStandardVar);
	m_dDataSkewness	= (iCount*iCount*u3) / ((iCount-1)*(iCount-2)*m_dStandardVar*m_dStandardVar*m_dStandardVar);
	
	//���ϵ��
	double	u4	= 0;
	iCount	= 0;
	for(i=0;i<m_iCount;i++)
	{
		if(m_pData[i] == INVALID_VALUE)	continue;
		if(m_pData[i] == INVALID_HEIGHT)	continue;

		u4	= u4 + (m_pData[i]-m_dDataMean)*(m_pData[i]-m_dDataMean)*(m_pData[i]-m_dDataMean)*(m_pData[i]-m_dDataMean);
		iCount++;
	}
	if(iCount != 0)		u4		= u3/iCount;
	else				u4		= 0;

	m_dDataKurtosis	= ((iCount*iCount*(iCount+1))*u4) / ((iCount-1)*(iCount-2)*(iCount-3)*m_dStandardVar*m_dStandardVar*m_dStandardVar*m_dStandardVar);

	m_dDataKurtosis	= m_dDataKurtosis - (3*(iCount-1)*(iCount-1)) / ((iCount-2)*(iCount-3));
	
	//
	iCount	= 0;
	for(i=0;i<m_iCount;i++)
	{
		if(m_pData[i] == INVALID_VALUE)	continue;
		if(m_pData[i] == INVALID_HEIGHT)	continue;

		m_dDataRMSE	= m_dDataRMSE + m_pData[i] * m_pData[i];
		iCount++;
	}

	m_dDataRMSE	= sqrt(m_dDataRMSE/iCount);

	return	true;
}

//////////////////////////////////////////////////////////////////////
// ��ȡ������Ŀ
//////////////////////////////////////////////////////////////////////
void CDCR_Statistics::GetDataCount(int& iCount)
{
	iCount	= m_iCount;
}

//////////////////////////////////////////////////////////////////////
// ��ȡ���ݵļ�ֵ
//////////////////////////////////////////////////////////////////////
void CDCR_Statistics::GetDataMinMax(double& dMin,double& dMax)
{
	dMin	= m_dDataMin;
	dMax	= m_dDataMax;
}

//////////////////////////////////////////////////////////////////////
//��ȡ���ݵľ�ֵ
//////////////////////////////////////////////////////////////////////
void CDCR_Statistics::GetDataMean(double& dMean)
{
	dMean	= m_dDataMean;
}

//////////////////////////////////////////////////////////////////////
//��ȡ���ݵ���������
//////////////////////////////////////////////////////////////////////
void CDCR_Statistics::GetDataVar(double& dVar)
{
	dVar	= m_dDataVar;
}

//////////////////////////////////////////////////////////////////////
//��ȡ���ݵı�׼��
//////////////////////////////////////////////////////////////////////
void CDCR_Statistics::GetDataStandardVar(double& dStandardVar)
{
	dStandardVar	= m_dStandardVar;
}

//////////////////////////////////////////////////////////////////////
//��ȡ���ݵı���ϵ��
//////////////////////////////////////////////////////////////////////
void CDCR_Statistics::GetDataCV(double& dCV)
{
	dCV	= m_dDataCV;
}

//////////////////////////////////////////////////////////////////////
//��ȡ���ݵ�ƫ��ϵ��
//////////////////////////////////////////////////////////////////////
void CDCR_Statistics::GetDataSkewness(double& dSkewness)
{
	dSkewness	= m_dDataSkewness;
}

//////////////////////////////////////////////////////////////////////
//��ȡ���ݵķ��ϵ��
//////////////////////////////////////////////////////////////////////
void CDCR_Statistics::GetDataKurtosis(double& dKurtosis)
{
	dKurtosis	= m_dDataKurtosis;
}

//////////////////////////////////////////////////////////////////////
//��ȡ���ݵ������
//////////////////////////////////////////////////////////////////////
void CDCR_Statistics::GetDataRMSE(double& dRMSE)
{
	dRMSE	= m_dDataRMSE;
}

//////////////////////////////////////////////////////////////////////
//��������
//////////////////////////////////////////////////////////////////////
void CDCR_Statistics::Save(CString szPathName)
{
	FILE	*fp;

	fp	= fopen(szPathName,"w");

	if(fp != NULL)
	{	
		char	chPathName[128];
		strcpy(chPathName,szPathName);
		fprintf(fp,"%s\n\n",chPathName);

		fprintf(fp,"����:\t%ld\n",m_iCount);					//"����"
		fprintf(fp,"��Сֵ:\t%.8lf\n",m_dDataMin);				//"��Сֵ"
		fprintf(fp,"���ֵ:\t%.8lf\n",m_dDataMax);				//"���ֵ",
		fprintf(fp,"��ֵ:\t%.8lf\n",m_dDataMean);				//"��ֵ",
		fprintf(fp,"����:\t%.8lf\n",m_dDataVar);				//"����",
		fprintf(fp,"��׼��:\t%.8lf\n",m_dStandardVar);			//"��׼��",
		fprintf(fp,"����ϵ��:\t%.8lf\n",m_dDataCV);				//"����ϵ��",
		fprintf(fp,"ƫ��ϵ��:\t%.8lf\n",m_dDataSkewness);		//"ƫ��ϵ��"
		fprintf(fp,"���ϵ��:\t%.8lf\n",m_dDataKurtosis);		//"���ϵ��",
		fprintf(fp,"�����:\t%.8lf\n",m_dDataRMSE);				//"�����",

		fclose(fp);
	}
}

void CDCR_Statistics::Save(FILE *fp)
{
	if(fp != NULL)
	{	
		fprintf(fp,"%ld\n",m_iCount);					//"����"
		fprintf(fp,"%16.5lf\n",m_dDataMin);				//"��Сֵ"
		fprintf(fp,"%16.5lf\n",m_dDataMax);				//"���ֵ",
		fprintf(fp,"%16.5lf\n",m_dDataMean);			//"��ֵ",
		fprintf(fp,"%16.5lf\n",m_dDataVar);				//"����",
		fprintf(fp,"%16.5lf\n",m_dStandardVar);			//"��׼��",
		fprintf(fp,"%16.5lf\n",m_dDataCV);				//"����ϵ��",
		fprintf(fp,"%16.5lf\n",m_dDataSkewness);		//"ƫ��ϵ��"
		fprintf(fp,"%16.5lf\n",m_dDataKurtosis);		//"���ϵ��",
		fprintf(fp,"%16.5lf\n",m_dDataRMSE);			//"�����",
	}
}

void CDCR_Statistics::SaveExInTile(FILE *fp)
{
	if(fp != NULL)
	{	
		fprintf(fp,"%s \t","����");					//"����"
		fprintf(fp,"%s \t","��Сֵ");				//"��Сֵ"
		fprintf(fp,"%s \t","���ֵ");				//"���ֵ",
		fprintf(fp,"%s \t","��ֵ");					//"��ֵ",
		fprintf(fp,"%s \t","����");					//"����",
		fprintf(fp,"%s \t","��׼��");				//"��׼��",
		fprintf(fp,"%s \t","����ϵ��");				//"����ϵ��",
		fprintf(fp,"%s \t","ƫ��ϵ��");				//"ƫ��ϵ��"
		fprintf(fp,"%s \t","���ϵ��");				//"���ϵ��",
		fprintf(fp,"%s \t","�����");				//"�����",
	}
}

void CDCR_Statistics::SaveEx(FILE *fp)
{
	if(fp != NULL)
	{	
		fprintf(fp,"%6ld \t",m_iCount);					//"����"
		fprintf(fp,"%.5lf \t",m_dDataMin);				//"��Сֵ"
		fprintf(fp,"%.5lf \t",m_dDataMax);				//"���ֵ",
		fprintf(fp,"%.5lf \t",m_dDataMean);				//"��ֵ",
		fprintf(fp,"%.5lf \t",m_dDataVar);				//"����",
		fprintf(fp,"%.5lf \t",m_dStandardVar);			//"��׼��",
		fprintf(fp,"%.5lf \t",m_dDataCV);				//"����ϵ��",
		fprintf(fp,"%.5lf \t",m_dDataSkewness);			//"ƫ��ϵ��"
		fprintf(fp,"%.5lf \t",m_dDataKurtosis);			//"���ϵ��",
		fprintf(fp,"%.5lf \t",m_dDataRMSE);				//"�����",
	}
}