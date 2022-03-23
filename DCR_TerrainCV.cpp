// DCR_TerrainCV.cpp: implementation of the CDCR_TerrainCV class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "vr_demcreator.h"
#include "DCR_TerrainCV.h"

#include "DCR_Statistics.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDCR_TerrainCV::CDCR_TerrainCV()
{
	//���ڳߴ�
	m_dMapXA	= m_dMapXB	= 0;
	m_dMapYA	= m_dMapYB	= 0;

	//�ֲڶ���Ϣ
	m_pCVObj	= NULL;
	//�����С�ֲڶ�
	m_dMinCV	= 0;
	m_dMaxCV	= 0;
	//ƽ���ֲڶ�
	m_dMeanCV	= 0;
	//�ֲڶȷ���
	m_dStdCV	= 0;
	
	//
	m_pDemObj		= NULL;

	//
	m_iNumOfX		= m_iNumOfY		= 0;

	//
	m_dDemLeft		= m_dDemRight	= 0;
	m_dDemBottom	= m_dDemTop		= 0;

	//
	m_dSizeOfX		= m_dSizeOfY	= 0;

	//
	m_dRealSize		= 1.0;

	//
	m_szPathName.Empty();

	//11*11
	m_iWindowSize			= 5;

	m_bShowStatus			= false;
}

CDCR_TerrainCV::~CDCR_TerrainCV()
{
	OnRelease();
}

//
void CDCR_TerrainCV::SetGridDem(double *pData,int iNumOfX,int iNumOfY,double dSizeOfX,double dSizeOfY,double dRealSize,double dLeft,double dRight,double dBottom,double dTop,CString szPathName)
{
	m_pDemObj			= pData;

	m_iNumOfX			= iNumOfX;
	m_iNumOfY			= iNumOfY;

	//
	m_dDemLeft			= dLeft;
	m_dDemRight			= dRight;
	m_dDemBottom		= dBottom;
	m_dDemTop			= dTop;

	//
	m_dSizeOfX			= dSizeOfX;
	m_dSizeOfY			= dSizeOfY;

	//
	m_dRealSize			= dRealSize;

	//
	m_szPathName		= szPathName;
}

//
void CDCR_TerrainCV::OnCalculate(int iWindowSize)
{
	//������ڳߴ�û�и�ֵ����ȡĬ��ֵ
	if(iWindowSize == 0)	iWindowSize	= m_iWindowSize;	

	if(m_pCVObj != NULL)
	{
		delete	[]m_pCVObj;
		m_pCVObj	= NULL;
	}

	m_dMinCV	= 0;
	m_dMaxCV	= 0;

	m_pCVObj	= new	double[m_iNumOfX*m_iNumOfY];

	if(m_pCVObj == NULL)	return;
	
	int		i,j,ci;
	bool	bFirstValue;

	bFirstValue	= true;

	for(j=0;j<m_iNumOfY;j++)
	{
		for(i=0;i<m_iNumOfX;i++)
		{
			ci		= j*m_iNumOfX+i;
			//����ÿһ����˵������Ҫ�����
			int	iBegin,iEnd;
			int	jBegin,jEnd;

			iBegin	= i - iWindowSize;
			iEnd	= i + iWindowSize;

			jBegin	= j - iWindowSize;
			jEnd	= j + iWindowSize;

			//���Խ���ˣ��򲻲������
			if(iBegin < 0 || iEnd>m_iNumOfX-1 || jBegin<0 || jEnd>m_iNumOfY-1)
			{
				m_pCVObj[ci]	= -9999.0;
			}
			else
			{
				//��iBegin,iEnd��jBegin,jEnd���д�����֤������
				//if(iBegin<0)		iBegin	= 0;	if(iBegin>m_iNumOfX-1)	iBegin	= m_iNumOfX-1;
				//if(iEnd<0)		iEnd	= 0;	if(iEnd>m_iNumOfX-1)	iEnd	= m_iNumOfX-1;
				
				//if(jBegin<0)		jBegin	= 0;	if(jBegin>m_iNumOfY-1)	jBegin	= m_iNumOfY-1;
				//if(jEnd<0)		jEnd	= 0;	if(jEnd>m_iNumOfY-1)	jEnd	= m_iNumOfY-1;

				//Ȼ�����õ�ĸ̱߳���ϵ��
				int		iNum	= 0;
				double	dMeanZ	= 0;
				int		I,J;
				
				for(J=jBegin;J<=jEnd;J++)
				{
					for(I=iBegin;I<=iEnd;I++)
					{
						if(I==iBegin && J==jBegin)
						{
							dMeanZ	= m_pDemObj[I+J*m_iNumOfX];
						}
						else
						{
							dMeanZ	= dMeanZ + m_pDemObj[I+J*m_iNumOfX];
						}
						iNum++;
					}
				}

				dMeanZ	= dMeanZ/iNum;
				
				double	dStdZ	= 0;

				for(J=jBegin;J<=jEnd;J++)
				{
					for(I=iBegin;I<=iEnd;I++)
					{
						if(I==iBegin && J==jBegin)
						{
							dStdZ	= (m_pDemObj[I+J*m_iNumOfX] - dMeanZ)*(m_pDemObj[I+J*m_iNumOfX] - dMeanZ);
						}
						else
						{
							dStdZ	= dStdZ + (m_pDemObj[I+J*m_iNumOfX] - dMeanZ)*(m_pDemObj[I+J*m_iNumOfX] - dMeanZ);
						}
					}
				}
				
				//
				dStdZ	= sqrt(dStdZ/(iNum-1));
				
				if(fabs(dMeanZ) < 1e-10)	m_pCVObj[ci]	= -9999.0;
				else
					//
					m_pCVObj[ci]	= dStdZ/dMeanZ;
			}

			//���������С�¶�
			if(m_pCVObj[ci] != -9999.0)
			{
				if(bFirstValue)
				{
					m_dMinCV	= m_pCVObj[ci];
					m_dMaxCV	= m_pCVObj[ci];
					bFirstValue	= false;
				}
				else
				{
					if(m_dMinCV > m_pCVObj[ci])	m_dMinCV	= m_pCVObj[ci];
					if(m_dMaxCV < m_pCVObj[ci])	m_dMaxCV	= m_pCVObj[ci];
				}
			}

		}//for(i=0;i<m_iNumOfX;i++)

	}//for(j=0;j<m_iNumOfY;j++)

	//���֮��Ӧ����ֱ�Ӽ����¶ȵ�ͳ����Ϣ
	{
		double	*dTemp;
		int		iTempNum;
		int		I,J;

		iTempNum	= 0;
		for(I=0;I<m_iNumOfX*m_iNumOfY;I++)
		{
			if(m_pCVObj[I] != -9999.0)		iTempNum++;
		}

		J		= 0;
		dTemp	= new	double[iTempNum];
		for(I=0;I<m_iNumOfX*m_iNumOfY;I++)
		{
			if(m_pCVObj[I] != -9999.0)		
			{
				dTemp[J]	= m_pCVObj[I];
				J++;
			}
		}

		CDCR_Statistics	pStatistics;
		//
		pStatistics.SetStatisticsData(iTempNum,dTemp);
		//
		pStatistics.Statistics();
		
		pStatistics.GetDataMinMax(m_dMinCV,m_dMaxCV);
		pStatistics.GetDataMean(m_dMeanCV);
		pStatistics.GetDataStandardVar(m_dStdCV);

		//
		CString	szPathName;
		szPathName	= m_szPathName.Left(szPathName.GetLength()-4) + "_CV.txt";
		pStatistics.Save(szPathName);

		if(dTemp != NULL)	delete	[]dTemp;
	}
}

//
void CDCR_TerrainCV::OnRelease()
{
	if(m_pCVObj != NULL)		{	delete	[]m_pCVObj;	m_pCVObj	= NULL;		}
}

//
void CDCR_TerrainCV::OnDraw(CDC *pDC)
{

}

//
bool CDCR_TerrainCV::OnSave()
{
	//��������ΪNULL
	if(m_pCVObj == NULL)	return	false;
	
	//�¶���Ϣ
	CString	szPathName;
	szPathName	= m_szPathName.Left(m_szPathName.GetLength()-4) + "(CV).grd";
	
	FILE	*fp;
	fp	= fopen(szPathName,"w");

	if(fp == NULL)	return	false;
	
	//DSAA
	fprintf(fp,"DSAA\n");

	//iCols,iRows,Ӧ���ж����ʲô���к���
	//���к���
	fprintf(fp,"%ld %ld\n",m_iNumOfX,m_iNumOfY);
	
	//dMinX,dMaxX
	fprintf(fp,"%lf %lf\n",m_dDemLeft,m_dDemRight);

	//dMinY,dMaxY
	fprintf(fp,"%lf %lf\n",m_dDemBottom,m_dDemTop);

	//
	int	i,j;
	for(j=0;j<m_iNumOfY;j++)
	{
		for(i=0;i<m_iNumOfX;i++)
		{
			double	dCV;
			dCV	= m_pCVObj[j*m_iNumOfX+i];
			if(i==0 && j==0)
			{
				m_dMinCV	= m_dMaxCV	= dCV;
			}
			else
			{
				if(dCV < m_dMinCV)	m_dMinCV	= dCV;
				if(dCV > m_dMaxCV)	m_dMaxCV	= dCV;
			}
		}
	}
	
	//m_dMinCV,m_dMaxCV
	fprintf(fp,"%.6lf %.6lf\n",m_dMinCV,m_dMaxCV);
	
	//
	for(j=0;j<m_iNumOfY;j++)
	{
		for(i=0;i<m_iNumOfX;i++)
		{
			double	dCV;
			dCV	= m_pCVObj[j*m_iNumOfX+i];

			fprintf(fp,"%.6lf ",dCV);

		}
		fprintf(fp,"\n");
	}
	
	//�ر�
	fclose(fp);
		
	return	true;
}

//
double* CDCR_TerrainCV::GetCV()
{
	return	m_pCVObj;
}

void CDCR_TerrainCV::GetCVStatisticsInfo(double& dMin, double &dMax, double& dMean,double& dStd)
{
	dMin	= m_dMinCV;
	dMax	= m_dMaxCV;

	//ƽ���ֲڶ�
	dMean	= m_dMeanCV;
	//�ֲڶȷ���
	dStd	= m_dStdCV;
}

//
void CDCR_TerrainCV::SetClientRect(CRect rRect)
{
	m_rRect	= rRect;
}

//
void CDCR_TerrainCV::SetCoordMapping(double dXA,double dXB,double dYA,double dYB)
{
	m_dMapXA	= dXA;
	m_dMapXB	= dXB;
	m_dMapYA	= dYA;
	m_dMapYB	= dYB;
}

//�������ڳߴ�
void CDCR_TerrainCV::SetWindowSize(int iSize)
{
	m_iWindowSize	= iSize;
}

int CDCR_TerrainCV::GetWindowSize()
{
	return	m_iWindowSize;
}

//56 ����/��ȡ�����������ʾ״̬
void CDCR_TerrainCV::SetShowStatus(bool bShowStatus)
{
	m_bShowStatus	= bShowStatus;
}

bool CDCR_TerrainCV::GetShowStatus()
{
	return	m_bShowStatus;
}
