// DCR_TerrainRoughness.cpp: implementation of the CDCR_TerrainRoughness class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "vr_demcreator.h"
#include "DCR_TerrainRoughness.h"

#include "DCR_Statistics.h"
#include "DCR_CurvatureSet.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDCR_TerrainRoughness::CDCR_TerrainRoughness()
{
	//窗口尺寸
	m_dMapXA	= m_dMapXB	= 0;
	m_dMapYA	= m_dMapYB	= 0;

	//粗糙度信息
	m_pRoughnessObj	= NULL;
	//最大、最小粗糙度
	m_dMinRoughness	= 0;
	m_dMaxRoughness	= 0;
	//平均粗糙度
	m_dMeanRoughness= 0;
	//粗糙度方差
	m_dStdRoughness	= 0;
	
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
	m_iWindowSize	= 5;

	m_bShowStatus	= false;
}

CDCR_TerrainRoughness::~CDCR_TerrainRoughness()
{
	OnRelease();
}

//
void CDCR_TerrainRoughness::SetGridDem(double *pData,int iNumOfX,int iNumOfY,double dSizeOfX,double dSizeOfY,double dRealSize,double dLeft,double dRight,double dBottom,double dTop,CString szPathName)
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
void CDCR_TerrainRoughness::OnCalculate(int iWindowSize)
{
	//如果窗口尺寸没有赋值，则取默认值
	if(iWindowSize == 0)	iWindowSize	= m_iWindowSize;	

	if(m_pRoughnessObj != NULL)
	{
		delete	[]m_pRoughnessObj;
		m_pRoughnessObj	= NULL;
	}

	m_dMinRoughness	= 0;
	m_dMaxRoughness	= 0;

	m_pRoughnessObj	= new	double[m_iNumOfX*m_iNumOfY];

	if(m_pRoughnessObj == NULL)
	{
		CString	szInfo;

		szInfo.Format("在计算地表粗糙度时，当尺寸为 %ld 时不能创建内存!\n程序暂停，即将退出!\n",iWindowSize);

		MessageBox(NULL,szInfo,"提示信息",MB_OK);

		return;
	}
	
	int		i,j,ci;
	//最大最小值判断
	bool	bFirstValue;

	bFirstValue	= true;
	for(j=0;j<m_iNumOfY;j++)
	{
		for(i=0;i<m_iNumOfX;i++)
		{
			ci		= j*m_iNumOfX+i;
			//对于每一点来说都是需要计算的
			int	iBegin,iEnd;
			int	jBegin,jEnd;

			iBegin	= i - iWindowSize;
			iEnd	= i + iWindowSize;

			jBegin	= j - iWindowSize;
			jEnd	= j + iWindowSize;

			//如果越界了，则不参与计算
			if(iBegin < 0 || iEnd>m_iNumOfX-1 || jBegin<0 || jEnd>m_iNumOfY-1)
			{
				m_pRoughnessObj[ci]	= -9999.0;
			}
			else
			{
				//对iBegin,iEnd，jBegin,jEnd进行处理，保证不错误
				//if(iBegin<0)		iBegin	= 0;	if(iBegin>m_iNumOfX-1)	iBegin	= m_iNumOfX-1;
				//if(iEnd<0)		iEnd	= 0;	if(iEnd>m_iNumOfX-1)	iEnd	= m_iNumOfX-1;
				
				//if(jBegin<0)		jBegin	= 0;	if(jBegin>m_iNumOfY-1)	jBegin	= m_iNumOfY-1;
				//if(jEnd<0)		jEnd	= 0;	if(jEnd>m_iNumOfY-1)	jEnd	= m_iNumOfY-1;

				//然后计算该点的粗糙度
				//表面积
				double	dSurferArea		= 0.0;
				
				//投影面积
				double	dProjectionArea	= 0.0;
				
				for(int J=jBegin;J<jEnd;J++)
				{
					for(int I=iBegin;I<iEnd;I++)
					{
						// 计算格网的表面积，一律按照(I,J+1)和(I+1,J)来进行切分
						// (I,J+1)	(I+1,J+1)
						// (I,J)	(I+1,J)
						
						double	dTempZ;
						double	dTempA,dTempB,dTempC;
						
						//第一个半三角形
						// (I,J)	(I+1,J)
						dTempZ	= m_pDemObj[I+J*m_iNumOfX] - m_pDemObj[(I+1)+J*m_iNumOfX];
						dTempA	= sqrt(m_dSizeOfX*m_dSizeOfX + dTempZ*dTempZ);
						
						// (I,J)	(I,J+1)
						dTempZ	= m_pDemObj[I+J*m_iNumOfX] - m_pDemObj[I+(J+1)*m_iNumOfX];
						dTempB	= sqrt(m_dSizeOfY*m_dSizeOfY + dTempZ*dTempZ);
						
						// (I,J+1) (I+1,J)
						dTempZ	= m_pDemObj[I+(J+1)*m_iNumOfX] - m_pDemObj[(I+1)+J*m_iNumOfX];
						dTempC	= sqrt(m_dSizeOfX*m_dSizeOfX + m_dSizeOfY*m_dSizeOfY + dTempZ*dTempZ);
						
						//
						double	dTempP;
						dTempP	= (dTempA+dTempB+dTempC)/2;
						
						double	dS;
						dS		= sqrt(dTempP * (dTempP - dTempA) * (dTempP - dTempB) * (dTempP - dTempC));

						dSurferArea	= dSurferArea + dS;
						
						//第二个半三角形
						// (I,J+1)	(I+1,J+1)
						dTempZ	= m_pDemObj[I+(J+1)*m_iNumOfX] - m_pDemObj[(I+1)+(J+1)*m_iNumOfX];
						dTempA	= sqrt(m_dSizeOfX*m_dSizeOfX + dTempZ*dTempZ);
						
						// (I+1,J)	(I+1,J+1)
						dTempZ	= m_pDemObj[(I+1)+J*m_iNumOfX] - m_pDemObj[(I+1)+(J+1)*m_iNumOfX];
						dTempB	= sqrt(m_dSizeOfY*m_dSizeOfY + dTempZ*dTempZ);
						
						// (I,J+1) (I+1,J)
						dTempZ	= m_pDemObj[I+(J+1)*m_iNumOfX] - m_pDemObj[(I+1)+J*m_iNumOfX];
						dTempC	= sqrt(m_dSizeOfX*m_dSizeOfX + m_dSizeOfY*m_dSizeOfY + dTempZ*dTempZ);
						
						//
						dTempP	= (dTempA+dTempB+dTempC)/2;
						
						dS		= sqrt(dTempP * (dTempP - dTempA) * (dTempP - dTempB) * (dTempP - dTempC));

						dSurferArea		= dSurferArea + dS;		
						
						dProjectionArea	= dProjectionArea + m_dSizeOfX * m_dSizeOfY;
					}
				}

				//
				if(dProjectionArea == 0)
					m_pRoughnessObj[ci]	= -9999.0;		
				else
					m_pRoughnessObj[ci]	= dSurferArea / dProjectionArea;
			}

			//计算最大最小坡度
			if(m_pRoughnessObj[ci] != -9999.0)
			{
				if(bFirstValue)
				{
					m_dMinRoughness	= m_pRoughnessObj[ci];
					m_dMaxRoughness	= m_pRoughnessObj[ci];

					bFirstValue	= false;
				}
				else
				{
					if(m_dMinRoughness > m_pRoughnessObj[ci])	m_dMinRoughness	= m_pRoughnessObj[ci];
					if(m_dMaxRoughness < m_pRoughnessObj[ci])	m_dMaxRoughness	= m_pRoughnessObj[ci];
				}
			}

		}//for(i=0;i<m_iNumOfX;i++)
	
	}//for(j=0;j<m_iNumOfY;j++)

	//
	//完成之后，应该是直接计算坡度的统计信息
	{
		double	*dTemp;
		int		iTempNum;
		int		I,J;

		iTempNum	= 0;
		for(I=0;I<m_iNumOfX*m_iNumOfY;I++)
		{
			if(m_pRoughnessObj[I] != -9999.0)		iTempNum++;
		}

		J		= 0;
		dTemp	= new	double[iTempNum];
		for(I=0;I<m_iNumOfX*m_iNumOfY;I++)
		{
			if(m_pRoughnessObj[I] != -9999.0)		
			{
				dTemp[J]	= m_pRoughnessObj[I];
				J++;
			}
		}

		CDCR_Statistics	pStatistics;
		//
		pStatistics.SetStatisticsData(iTempNum,dTemp);
		//
		pStatistics.Statistics();
		
		pStatistics.GetDataMinMax(m_dMinRoughness,m_dMaxRoughness);
		pStatistics.GetDataMean(m_dMeanRoughness);
		pStatistics.GetDataStandardVar(m_dStdRoughness);

		//
		CString	szPathName;
		szPathName	= m_szPathName.Left(szPathName.GetLength()-4) + "_Roughness.txt";
		pStatistics.Save(szPathName);

		if(dTemp != NULL)	delete	[]dTemp;
	}
}
//
void CDCR_TerrainRoughness::OnRelease()
{
	if(m_pRoughnessObj != NULL)		{	delete	[]m_pRoughnessObj;	m_pRoughnessObj	= NULL;		}
}

//
void CDCR_TerrainRoughness::OnDraw(CDC *pDC)
{
	if(m_pRoughnessObj == NULL)		return;

	if(m_bShowStatus == false)		return;

	int		i,j,ci;
	double	dX,dY,dTerrainRoughness;

	CDCR_CurvatureSet::Instance()->SetMinCurvature(m_dMinRoughness);
	CDCR_CurvatureSet::Instance()->SetMidCurvature((m_dMinRoughness+m_dMaxRoughness)/2);
	CDCR_CurvatureSet::Instance()->SetMaxCurvature(m_dMaxRoughness);


	for(j=0;j<m_iNumOfY;j++)
	{
		for(i=0;i<m_iNumOfX;i++)
		{
			ci		= j*m_iNumOfX+i;
			dX		= m_dDemLeft + i*m_dSizeOfX;
			dY		= m_dDemBottom + j*m_dSizeOfY;
			
			dTerrainRoughness	= m_pRoughnessObj[ci];
			
			if(dTerrainRoughness != -9999.0)
			{
				//
				double	dLeft,dRight,dBottom,dTop;
				dLeft	= dX - m_dSizeOfX/2;
				dRight	= dLeft + m_dSizeOfX;
				dBottom	= dY - m_dSizeOfY/2;
				dTop	= dBottom + m_dSizeOfY;

				//
				CRect	rectCell;
				rectCell.left	= (int)(dLeft*m_dMapXA + m_dMapXB + 0.5);
				rectCell.bottom	= (int)(dBottom*m_dMapYA + m_dMapYB + 0.5);
				rectCell.right	= (int)(dRight*m_dMapXA + m_dMapXB + 0.5);
				rectCell.top	= (int)(dTop*m_dMapYA + m_dMapYB + 0.5);
				
				COLORREF	crColor;
				
				crColor	= CDCR_CurvatureSet::Instance()->GetColor(dTerrainRoughness);

				//
				CBrush	*pNewBrush,*pOldBrush;
				pNewBrush	= new	CBrush;
				pNewBrush->CreateSolidBrush(crColor);
				pOldBrush	= pDC->SelectObject(pNewBrush);

				pDC->FillRect(rectCell,pNewBrush);	

				pDC->SelectObject(pOldBrush);
				delete	pNewBrush;
			}
		}
	}
}

//
bool CDCR_TerrainRoughness::OnSave()
{
	//假如数据为NULL
	if(m_pRoughnessObj == NULL)	return	false;
	
	//坡度信息
	CString	szPathName;
	szPathName	= m_szPathName.Left(m_szPathName.GetLength()-4) + "(Roughness).grd";
	
	FILE	*fp;
	fp	= fopen(szPathName,"w");

	if(fp == NULL)	return	false;
	
	//DSAA
	fprintf(fp,"DSAA\n");

	//iCols,iRows,应该判断清楚什么是行和列
	//先列后行
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
			double	dRoughness;
			dRoughness	= m_pRoughnessObj[j*m_iNumOfX+i];
			if(i==0 && j==0)
			{
				m_dMinRoughness	= m_dMaxRoughness	= dRoughness;
			}
			else
			{
				if(dRoughness < m_dMinRoughness)	m_dMinRoughness	= dRoughness;
				if(dRoughness > m_dMaxRoughness)	m_dMaxRoughness	= dRoughness;
			}
		}
	}
	
	//m_dMinRoughness,m_dMaxRoughness
	fprintf(fp,"%.6lf %.6lf\n",m_dMinRoughness,m_dMaxRoughness);
	
	//
	for(j=0;j<m_iNumOfY;j++)
	{
		for(i=0;i<m_iNumOfX;i++)
		{
			double	dRoughness;
			dRoughness	= m_pRoughnessObj[j*m_iNumOfX+i];

			fprintf(fp,"%.6lf ",dRoughness);

		}
		fprintf(fp,"\n");
	}
	
	//关闭
	fclose(fp);
		
	return	true;
}

//
double* CDCR_TerrainRoughness::GetRoughness()
{
	return	m_pRoughnessObj;
}

void CDCR_TerrainRoughness::GetRoughnessStatisticsInfo(double& dMin,double& dMax, double& dMean,double& dStd)
{
	dMin	= m_dMinRoughness;
	dMax	= m_dMaxRoughness;
	//平均粗糙度
	dMean	= m_dMeanRoughness;
	//粗糙度方差
	dStd	= m_dStdRoughness;
}

//
void CDCR_TerrainRoughness::SetClientRect(CRect rRect)
{
	m_rRect	= rRect;
}

//
void CDCR_TerrainRoughness::SetCoordMapping(double dXA,double dXB,double dYA,double dYB)
{
	m_dMapXA	= dXA;
	m_dMapXB	= dXB;
	m_dMapYA	= dYA;
	m_dMapYB	= dYB;
}

//分析窗口尺寸
void CDCR_TerrainRoughness::SetWindowSize(int iSize)
{
	m_iWindowSize	= iSize;
}

int CDCR_TerrainRoughness::GetWindowSize()
{
	return	m_iWindowSize;
}

//54 设置/获取粗糙度显示状态
void CDCR_TerrainRoughness::SetShowStatus(bool bShowStatus)
{
	m_bShowStatus	= bShowStatus;
}

bool CDCR_TerrainRoughness::GetShowStatus()
{
	return	m_bShowStatus;
}
