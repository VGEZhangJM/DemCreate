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
	//数量长度
	m_iCount		= 0;
	//数据内容
	m_pData			= NULL;

	//统计数据的极值
	m_dDataMin		= m_dDataMax	= 0;

	//统计数据的均值
	m_dDataMean		= 0;

	//统计数据的方差,是无偏的
	m_dDataVar		= 0;

	//统计数据的标准差，是无偏的
	m_dStandardVar	= 0;

	//变异系数
	m_dDataCV		= 0;
	//偏度系数
	m_dDataSkewness	= 0;
	//峰度系数
	m_dDataKurtosis	= 0;

	//
	m_dDataRMSE		= 0;

	m_pData			= NULL;
}

CDCR_Statistics::~CDCR_Statistics()
{

}


//////////////////////////////////////////////////////////////////////
//输入统计数据
//////////////////////////////////////////////////////////////////////
void CDCR_Statistics::SetStatisticsData(int iCount,double *pData)
{
	m_iCount	= iCount;

	m_pData		= pData;
}

//////////////////////////////////////////////////////////////////////
//进行各种量的统计
//////////////////////////////////////////////////////////////////////
bool CDCR_Statistics::Statistics()
{
	//
	if(m_pData == NULL)	return	false;
	
	//初始化
	{
		//统计数据的极值
		m_dDataMin		= m_dDataMax	= 0;

		//统计数据的均值
		m_dDataMean		= 0;

		//统计数据的方差,是无偏的
		m_dDataVar		= 0;

		//统计数据的标准差，是无偏的
		m_dStandardVar	= 0;

		//变异系数
		m_dDataCV		= 0;
		//偏度系数
		m_dDataSkewness	= 0;
		//峰度系数
		m_dDataKurtosis	= 0;

		//
		m_dDataRMSE		= 0;
	}

	int		i;
	bool	bFirstValue;

	bFirstValue	= true;

	//极值
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
	//均值
	for(i=0;i<m_iCount;i++)
	{
		if(m_pData[i] == INVALID_VALUE)	continue;		
		if(m_pData[i] == INVALID_HEIGHT)	continue;

		m_dDataMean	= m_dDataMean + m_pData[i];
		iCount++;
	}

	if(iCount != 0)		m_dDataMean		= m_dDataMean/iCount;
	else				m_dDataMean		= 0;

	//样本方差
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

	//样本标准差
	m_dStandardVar	= sqrt(m_dDataVar);

	//变异系数
	m_dDataCV		= m_dStandardVar / m_dDataMean;

	//偏度系数
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
	
	//峰度系数
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
// 获取数据数目
//////////////////////////////////////////////////////////////////////
void CDCR_Statistics::GetDataCount(int& iCount)
{
	iCount	= m_iCount;
}

//////////////////////////////////////////////////////////////////////
// 获取数据的极值
//////////////////////////////////////////////////////////////////////
void CDCR_Statistics::GetDataMinMax(double& dMin,double& dMax)
{
	dMin	= m_dDataMin;
	dMax	= m_dDataMax;
}

//////////////////////////////////////////////////////////////////////
//获取数据的均值
//////////////////////////////////////////////////////////////////////
void CDCR_Statistics::GetDataMean(double& dMean)
{
	dMean	= m_dDataMean;
}

//////////////////////////////////////////////////////////////////////
//获取数据的样本方差
//////////////////////////////////////////////////////////////////////
void CDCR_Statistics::GetDataVar(double& dVar)
{
	dVar	= m_dDataVar;
}

//////////////////////////////////////////////////////////////////////
//获取数据的标准差
//////////////////////////////////////////////////////////////////////
void CDCR_Statistics::GetDataStandardVar(double& dStandardVar)
{
	dStandardVar	= m_dStandardVar;
}

//////////////////////////////////////////////////////////////////////
//获取数据的变异系数
//////////////////////////////////////////////////////////////////////
void CDCR_Statistics::GetDataCV(double& dCV)
{
	dCV	= m_dDataCV;
}

//////////////////////////////////////////////////////////////////////
//获取数据的偏度系数
//////////////////////////////////////////////////////////////////////
void CDCR_Statistics::GetDataSkewness(double& dSkewness)
{
	dSkewness	= m_dDataSkewness;
}

//////////////////////////////////////////////////////////////////////
//获取数据的峰度系数
//////////////////////////////////////////////////////////////////////
void CDCR_Statistics::GetDataKurtosis(double& dKurtosis)
{
	dKurtosis	= m_dDataKurtosis;
}

//////////////////////////////////////////////////////////////////////
//获取数据的中误差
//////////////////////////////////////////////////////////////////////
void CDCR_Statistics::GetDataRMSE(double& dRMSE)
{
	dRMSE	= m_dDataRMSE;
}

//////////////////////////////////////////////////////////////////////
//保存数据
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

		fprintf(fp,"点数:\t%ld\n",m_iCount);					//"点数"
		fprintf(fp,"最小值:\t%.8lf\n",m_dDataMin);				//"最小值"
		fprintf(fp,"最大值:\t%.8lf\n",m_dDataMax);				//"最大值",
		fprintf(fp,"均值:\t%.8lf\n",m_dDataMean);				//"均值",
		fprintf(fp,"方差:\t%.8lf\n",m_dDataVar);				//"方差",
		fprintf(fp,"标准差:\t%.8lf\n",m_dStandardVar);			//"标准差",
		fprintf(fp,"变异系数:\t%.8lf\n",m_dDataCV);				//"变异系数",
		fprintf(fp,"偏度系数:\t%.8lf\n",m_dDataSkewness);		//"偏度系数"
		fprintf(fp,"峰度系数:\t%.8lf\n",m_dDataKurtosis);		//"峰度系数",
		fprintf(fp,"中误差:\t%.8lf\n",m_dDataRMSE);				//"中误差",

		fclose(fp);
	}
}

void CDCR_Statistics::Save(FILE *fp)
{
	if(fp != NULL)
	{	
		fprintf(fp,"%ld\n",m_iCount);					//"点数"
		fprintf(fp,"%16.5lf\n",m_dDataMin);				//"最小值"
		fprintf(fp,"%16.5lf\n",m_dDataMax);				//"最大值",
		fprintf(fp,"%16.5lf\n",m_dDataMean);			//"均值",
		fprintf(fp,"%16.5lf\n",m_dDataVar);				//"方差",
		fprintf(fp,"%16.5lf\n",m_dStandardVar);			//"标准差",
		fprintf(fp,"%16.5lf\n",m_dDataCV);				//"变异系数",
		fprintf(fp,"%16.5lf\n",m_dDataSkewness);		//"偏度系数"
		fprintf(fp,"%16.5lf\n",m_dDataKurtosis);		//"峰度系数",
		fprintf(fp,"%16.5lf\n",m_dDataRMSE);			//"中误差",
	}
}

void CDCR_Statistics::SaveExInTile(FILE *fp)
{
	if(fp != NULL)
	{	
		fprintf(fp,"%s \t","点数");					//"点数"
		fprintf(fp,"%s \t","最小值");				//"最小值"
		fprintf(fp,"%s \t","最大值");				//"最大值",
		fprintf(fp,"%s \t","均值");					//"均值",
		fprintf(fp,"%s \t","方差");					//"方差",
		fprintf(fp,"%s \t","标准差");				//"标准差",
		fprintf(fp,"%s \t","变异系数");				//"变异系数",
		fprintf(fp,"%s \t","偏度系数");				//"偏度系数"
		fprintf(fp,"%s \t","峰度系数");				//"峰度系数",
		fprintf(fp,"%s \t","中误差");				//"中误差",
	}
}

void CDCR_Statistics::SaveEx(FILE *fp)
{
	if(fp != NULL)
	{	
		fprintf(fp,"%6ld \t",m_iCount);					//"点数"
		fprintf(fp,"%.5lf \t",m_dDataMin);				//"最小值"
		fprintf(fp,"%.5lf \t",m_dDataMax);				//"最大值",
		fprintf(fp,"%.5lf \t",m_dDataMean);				//"均值",
		fprintf(fp,"%.5lf \t",m_dDataVar);				//"方差",
		fprintf(fp,"%.5lf \t",m_dStandardVar);			//"标准差",
		fprintf(fp,"%.5lf \t",m_dDataCV);				//"变异系数",
		fprintf(fp,"%.5lf \t",m_dDataSkewness);			//"偏度系数"
		fprintf(fp,"%.5lf \t",m_dDataKurtosis);			//"峰度系数",
		fprintf(fp,"%.5lf \t",m_dDataRMSE);				//"中误差",
	}
}