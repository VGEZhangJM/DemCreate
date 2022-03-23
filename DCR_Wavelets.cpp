// DCR_Wavelets.cpp: implementation of the CDCR_Wavelets class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "vr_demcreator.h"
#include "DCR_Wavelets.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDCR_Wavelets::CDCR_Wavelets()
{
	//
	m_pOrignBits	= NULL;
	//
	m_pCurrentBits	= NULL;

	//
	m_iNumOfX		= 0;
	m_iNumOfY		= 0;

	//高程极值
	m_dMinHeight	= 0;
	m_dMaxHeight	= 0;
}

CDCR_Wavelets::~CDCR_Wavelets()
{
	if(m_pCurrentBits)	{	delete	[]m_pCurrentBits;	m_pCurrentBits	= NULL;	}
}

//
void CDCR_Wavelets::SetOrignDem(double *pData,int iNumOfX,int iNumOfY,double dMinH,double dMaxH,double dLeft,double dRight,double dBottom,double dTop)
{
	m_pOrignBits	= pData;

	//
	m_iNumOfX		= iNumOfX;
	m_iNumOfY		= iNumOfY;

	//高程极值
	m_dMinHeight	= dMinH;
	m_dMaxHeight	= dMaxH;

	//
	m_dDemLeft		= dLeft;
	m_dDemRight		= dRight;
	m_dDemBottom	= dBottom;
	m_dDemTop		= dTop;
}

//
//pOrign为原始值，n为半数值
void CDCR_Wavelets::Calculate(double *pOrign, int n,double **pCurrent)
{
	double	*pCTemp,*pDTemp;

	pCTemp	= new	double[n+2];
	pDTemp	= new	double[n+2];

	//赋值
	pCTemp[0]	= 0;
	pDTemp[0]	= 0;
	for(int i=1;i<=n;i++)
	{
		pCTemp[i]	= pOrign[2*(i-1)];
		pDTemp[i]	= pOrign[2*(i-1)+1];
	}
	pCTemp[n+1]	= 0;
	pDTemp[n+1]	= 0;

	//第一步计算
	for(i=1;i<=n;i++)
	{
		pCTemp[i]	= pCTemp[i] + sqrt(3)*pDTemp[i];
	}

	//第二步
	for(i=1;i<=n;i++)
	{
		pDTemp[i]	= pDTemp[i] - sqrt(3)/4*pCTemp[i] - (sqrt(3)-2)/4*pCTemp[i-1];
	}

	//第三步
	for(i=1;i<=n;i++)
	{
		pCTemp[i]	= pCTemp[i] - pDTemp[i+1];
	}
	
	for(i=1;i<=n;i++)
		(*pCurrent)[i-1]	= ((sqrt(3)-1)/sqrt(2))*pCTemp[i];
	for(i=1;i<=n;i++)
		(*pCurrent)[n+i-1]	= ((sqrt(3)+1)/sqrt(2))*pDTemp[i];

	delete	[]pCTemp;	pCTemp	= NULL;
	delete	[]pDTemp;	pDTemp	= NULL;
}

//使用D4简化小波
void CDCR_Wavelets::WaveletsD4()
{
	if(m_pOrignBits == NULL)	return;
	
	if(m_pCurrentBits)	{	delete	[]m_pCurrentBits;	m_pCurrentBits	= NULL;	}

	m_pCurrentBits	= new	double[m_iNumOfX*m_iNumOfY];
		
	//行变换
	for(int i=0;i<m_iNumOfY;i++)
	{
		double *pOrign,*pCurrent;

		pOrign		= new	double[m_iNumOfX];
		pCurrent	= new	double[m_iNumOfX];
		
		//红变量
		for(int j=0;j<m_iNumOfX;j++)
			//pOrign[j]	= m_pOrignBits[(m_iNumOfY-1-i)*m_iNumOfX+j];
			pOrign[j]	= m_pOrignBits[i*m_iNumOfX+j];

		Calculate(pOrign, m_iNumOfX/2,&pCurrent);
	
		for(j=0;j<m_iNumOfX;j++)
			//m_pCurrentBits[(m_iNumOfY-1-i)*m_iNumOfX+j]	= pCurrent[j];
			m_pCurrentBits[i*m_iNumOfX+j]	= pCurrent[j];

		delete	[]pOrign;
		delete	[]pCurrent;
	}

	//列变换
	for(i=0;i<m_iNumOfX;i++)
	{
		double *pOrign,*pCurrent;

		pOrign		= new	double[m_iNumOfY];
		pCurrent	= new	double[m_iNumOfY];
		
		//红变量
		for(int j=0;j<m_iNumOfY;j++)
			//pOrign[j]	= m_pCurrentBits[(m_iNumOfY-1-j)*m_iNumOfX+i];
			pOrign[j]	= m_pCurrentBits[j*m_iNumOfX+i];

		Calculate(pOrign, m_iNumOfY/2,&pCurrent);
	
		for(j=0;j<m_iNumOfY;j++)
			//m_pCurrentBits[(m_iNumOfY-1-j)*m_iNumOfX+i]	= pCurrent[j];
			m_pCurrentBits[j*m_iNumOfX+i]	= pCurrent[j];

		delete	[]pOrign;
		delete	[]pCurrent;
	}
}

//保存数据
void CDCR_Wavelets::SaveCurrentDem(CString szPathName)
{
	//假如数据为NULL
	if(m_pCurrentBits == NULL)	return;

	//求取最大最小高程值
	double	dMinH,dMaxH;
	int		i,j;
	for(j=0;j<m_iNumOfY/2;j++)
	{
		for(i=0;i<m_iNumOfX/2;i++)
		{
			double	dH;
			dH	= m_pCurrentBits[j*m_iNumOfX+i];
			
			if(i==0 && j==0)
			{
				dMinH	= dMaxH	= dH;
			}
			else
			{	
				if(dMinH > dH)	dMinH	= dH;
				if(dMaxH < dH)	dMaxH	= dH;
			}
		}
	}

	//
	/*
	for(j=0;j<m_iNumOfY/2;j++)
	{
		for(i=0;i<m_iNumOfX/2;i++)
		{
			double	dH;
			dH	= m_pCurrentBits[j*m_iNumOfX+i];
			m_pCurrentBits[j*m_iNumOfX+i]	= (dH-dMinH)/(dMaxH-dMinH) * (m_dMaxHeight-m_dMinHeight)+m_dMinHeight;
			
			if(i==0 && j==0)
			{
				dMinH	= dMaxH	= m_pCurrentBits[j*m_iNumOfX+i];
			}
			else
			{	
				if(dMinH > m_pCurrentBits[j*m_iNumOfX+i])	dMinH	= m_pCurrentBits[j*m_iNumOfX+i];
				if(dMaxH < m_pCurrentBits[j*m_iNumOfX+i])	dMaxH	= m_pCurrentBits[j*m_iNumOfX+i];
			}
		}
	}
	*/

	FILE	*fp;
	fp	= fopen(szPathName,"w");

	if(fp != NULL)
	{
		//DSAA
		fprintf(fp,"DSAA\n");

		//iCols,iRows,应该判断清楚什么是行和列
		//先列后行
		fprintf(fp,"%ld %ld\n",m_iNumOfX/2,m_iNumOfY/2);
		
		//dMinX,dMaxX
		fprintf(fp,"%lf %lf\n",m_dDemLeft,m_dDemRight);

		//dMinY,dMaxY
		fprintf(fp,"%lf %lf\n",m_dDemBottom,m_dDemTop);
		
		//dMinZ,dMaxZ
		fprintf(fp,"%lf %lf\n",dMinH/2,dMaxH/2);
		
		//
		int		iCount;
		for(j=0;j<m_iNumOfY/2;j++)
		{
			iCount	= 0;
			for(i=0;i<m_iNumOfX/2;i++)
			{
				double	dH;
				dH	= m_pCurrentBits[j*m_iNumOfX+i];

				fprintf(fp,"%.4lf ",dH/2);

				iCount++;

				if(iCount == 10)	
				{
					fprintf(fp,"\n");

					iCount	= 0;
				}
			}
			fprintf(fp,"\n");
			fprintf(fp,"\n");
		}
		
		//关闭
		fclose(fp);
	}
}
