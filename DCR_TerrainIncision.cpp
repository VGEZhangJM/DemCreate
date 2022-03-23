// DCR_TerrainIncision.cpp: implementation of the CDCR_TerrainIncision class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "vr_demcreator.h"
#include "DCR_TerrainIncision.h"

#include "DCR_Statistics.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDCR_TerrainIncision::CDCR_TerrainIncision()
{
	//窗口尺寸
	m_dMapXA	= m_dMapXB	= 0;
	m_dMapYA	= m_dMapYB	= 0;

	//粗糙度信息
	m_pIncisionObj	= NULL;
	//最大、最小粗糙度
	m_dMinIncision	= 0;
	m_dMaxIncision	= 0;
	//平均粗糙度
	m_dMeanIncision= 0;
	//粗糙度方差
	m_dStdIncision	= 0;
	
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

CDCR_TerrainIncision::~CDCR_TerrainIncision()
{
	OnRelease();
}

//
void CDCR_TerrainIncision::SetGridDem(double *pData,int iNumOfX,int iNumOfY,double dSizeOfX,double dSizeOfY,double dRealSize,double dLeft,double dRight,double dBottom,double dTop,CString szPathName)
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
void CDCR_TerrainIncision::OnCalculate(int iWindowSize)
{
	//如果窗口尺寸没有赋值，则取默认值
	if(iWindowSize == 0)	iWindowSize	= m_iWindowSize;	

	if(m_pIncisionObj != NULL)
	{
		delete	[]m_pIncisionObj;
		m_pIncisionObj	= NULL;
	}

	m_dMinIncision	= 0;
	m_dMaxIncision	= 0;

	m_pIncisionObj	= new	double[m_iNumOfX*m_iNumOfY];

	if(m_pIncisionObj == NULL)	return;
	
	int		i,j,ci;
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
				m_pIncisionObj[ci]	= -9999.0;
			}
			else
			{
				//对iBegin,iEnd，jBegin,jEnd进行处理，保证不错误
				//if(iBegin<0)		iBegin	= 0;	if(iBegin>m_iNumOfX-1)	iBegin	= m_iNumOfX-1;
				//if(iEnd<0)		iEnd	= 0;	if(iEnd>m_iNumOfX-1)	iEnd	= m_iNumOfX-1;
				
				//if(jBegin<0)		jBegin	= 0;	if(jBegin>m_iNumOfY-1)	jBegin	= m_iNumOfY-1;
				//if(jEnd<0)		jEnd	= 0;	if(jEnd>m_iNumOfY-1)	jEnd	= m_iNumOfY-1;

				//然后计算该点的高程变异系数
				int		iNum	= 0;
				double	dMeanZ	= 0;
				double	dMinZ	= 0;
				int		I,J;
				
				for(J=jBegin;J<=jEnd;J++)
				{
					for(I=iBegin;I<=iEnd;I++)
					{
						if(I==iBegin && J==jBegin)
						{
							dMeanZ	= m_pDemObj[I+J*m_iNumOfX];
							dMinZ	= dMeanZ;
						}
						else
						{
							dMeanZ	= dMeanZ + m_pDemObj[I+J*m_iNumOfX];
							if(dMinZ > m_pDemObj[I+J*m_iNumOfX])
								dMinZ	= m_pDemObj[I+J*m_iNumOfX];
						}
						iNum++;
					}
				}

				dMeanZ	= dMeanZ/iNum;
				
				if(fabs(dMeanZ) < 1e-10)	m_pIncisionObj[ci]	= -9999.0;
				else
					//
					m_pIncisionObj[ci]	= dMeanZ-dMinZ;
			}

			//计算最大最小坡度
			if(m_pIncisionObj[ci] != -9999.0)
			{
				if(bFirstValue)
				{
					m_dMinIncision	= m_pIncisionObj[ci];
					m_dMaxIncision	= m_pIncisionObj[ci];
					bFirstValue	= false;
				}
				else
				{
					if(m_dMinIncision > m_pIncisionObj[ci])	m_dMinIncision	= m_pIncisionObj[ci];
					if(m_dMaxIncision < m_pIncisionObj[ci])	m_dMaxIncision	= m_pIncisionObj[ci];
				}
			}

		}//for(i=0;i<m_iNumOfX;i++)

	}//for(j=0;j<m_iNumOfY;j++)	

	//完成之后，应该是直接计算坡度的统计信息
	{
		double	*dTemp;
		int		iTempNum;
		int		I,J;

		iTempNum	= 0;
		for(I=0;I<m_iNumOfX*m_iNumOfY;I++)
		{
			if(m_pIncisionObj[I] != -9999.0)		iTempNum++;
		}

		J		= 0;
		dTemp	= new	double[iTempNum];
		for(I=0;I<m_iNumOfX*m_iNumOfY;I++)
		{
			if(m_pIncisionObj[I] != -9999.0)		
			{
				dTemp[J]	= m_pIncisionObj[I];
				J++;
			}
		}

		CDCR_Statistics	pStatistics;
		//
		pStatistics.SetStatisticsData(iTempNum,dTemp);
		//
		pStatistics.Statistics();
		
		pStatistics.GetDataMinMax(m_dMinIncision,m_dMaxIncision);
		pStatistics.GetDataMean(m_dMeanIncision);
		pStatistics.GetDataStandardVar(m_dStdIncision);

		//
		CString	szPathName;
		szPathName	= m_szPathName.Left(szPathName.GetLength()-4) + "_Incision.txt";
		pStatistics.Save(szPathName);

		if(dTemp != NULL)	delete	[]dTemp;
	}
}

//
void CDCR_TerrainIncision::OnRelease()
{
	if(m_pIncisionObj != NULL)		{	delete	[]m_pIncisionObj;	m_pIncisionObj	= NULL;		}
}

//
void CDCR_TerrainIncision::OnDraw(CDC *pDC)
{

}

//
bool CDCR_TerrainIncision::OnSave()
{

	return	true;
}

//
double* CDCR_TerrainIncision::GetIncision()
{
	return	m_pIncisionObj;
}

void CDCR_TerrainIncision::GetIncisionStatisticsInfo(double& dMin, double &dMax, double& dMean,double& dStd)
{
	dMin		= m_dMinIncision;
	dMax		= m_dMaxIncision;

	//平均粗糙度
	dMean		= m_dMeanIncision;
	//粗糙度方差
	dStd		= m_dStdIncision;
}

//
void CDCR_TerrainIncision::SetClientRect(CRect rRect)
{
	m_rRect	= rRect;
}

//
void CDCR_TerrainIncision::SetCoordMapping(double dXA,double dXB,double dYA,double dYB)
{
	m_dMapXA	= dXA;
	m_dMapXB	= dXB;
	m_dMapYA	= dYA;
	m_dMapYB	= dYB;
}

//分析窗口尺寸
void CDCR_TerrainIncision::SetWindowSize(int iSize)
{
	m_iWindowSize	= iSize;
}

int CDCR_TerrainIncision::GetWindowSize()
{
	return	m_iWindowSize;
}

//56 设置/获取地形起伏度显示状态
void CDCR_TerrainIncision::SetShowStatus(bool bShowStatus)
{
	m_bShowStatus	= bShowStatus;
}

bool CDCR_TerrainIncision::GetShowStatus()
{
	return	m_bShowStatus;
}
