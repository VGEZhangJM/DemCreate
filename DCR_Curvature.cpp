// DCR_Curvature.cpp: implementation of the CDCR_Curvature class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "vr_demcreator.h"
#include "DCR_Curvature.h"

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

CDCR_Curvature::CDCR_Curvature()
{
	//剖面曲率
	m_pProfileCurvatureObj	= NULL;
	m_dMinProfileCurvature	= m_dMaxProfileCurvature	= 0;
	m_dMeanProfileCurvature	= m_dStdProfileCurvature	= 0;

	//平面曲率
	m_pPlanCurvatureObj		= NULL;
	m_dMinPlanCurvature		= m_dMaxPlanCurvature		= 0;
	m_dMeanPlanCurvature	= m_dStdPlanCurvature		= 0;

	//平面曲率
	m_pTgCurvatureObj		= NULL;
	m_dMinTgCurvature		= m_dMaxTgCurvature			= 0;
	m_dMeanTgCurvature		= m_dStdTgCurvature			= 0;

	//窗口尺寸
	m_dMapXA				= m_dMapXB	= 0;
	m_dMapYA				= m_dMapYB	= 0;

	//坡度信息
	m_pSlopeObj				= NULL;
	//最大、最小坡度
	m_dMinSlope				= m_dMaxSlope	= 0;
	//平均坡度
	m_dMeanSlope			= 0;
	//坡度方差
	m_dStdSlope				= 0;

	//
	m_pDemObj				= NULL;

	//
	m_iNumOfX				= m_iNumOfY	= 0;

	//
	m_dDemLeft				= m_dDemRight	= 0;
	m_dDemBottom			= m_dDemTop		= 0;

	//
	m_dSizeOfX				= m_dSizeOfY	= 0;

	//
	m_dRealSize				= 1.0;

	m_bShowStatusInCurvature= false;
}

CDCR_Curvature::~CDCR_Curvature()
{
	OnRelease();
}

//
void CDCR_Curvature::SetGridDem(double *pData,int iNumOfX,int iNumOfY,double dSizeOfX,double dSizeOfY,double dRealSize,double dLeft,double dRight,double dBottom,double dTop,CString szPathName)
{
	//
	m_pDemObj				= pData;

	//
	m_iNumOfX				= iNumOfX;
	m_iNumOfY				= iNumOfY;

	//
	m_dDemLeft				= dLeft;
	m_dDemRight				= dRight;
	m_dDemBottom			= dBottom;
	m_dDemTop				= dTop;

	//
	m_dSizeOfX				= dSizeOfX;
	m_dSizeOfY				= dSizeOfY;

	//
	m_dRealSize				= dRealSize;	
	
	//
	m_szPathName			= szPathName;
}

//////////////////////////////////////////////////////////////////////
// 32 计算剖面曲率
//////////////////////////////////////////////////////////////////////
void CDCR_Curvature::OnCalculateProfileCurvature()
{
	if(m_pProfileCurvatureObj != NULL)
	{
		delete	[]m_pProfileCurvatureObj;		m_pProfileCurvatureObj	= NULL;
	}

	m_dMinProfileCurvature	= 0.0;
	m_dMaxProfileCurvature	= 0.0;

	m_pProfileCurvatureObj	= new	double[m_iNumOfX*m_iNumOfY];

	if(m_pProfileCurvatureObj == NULL)	return;

	int		i,j,ci,I,J;
	double	e;
	double	e1,e2,e3,e4;
	double	e5,e6,e7,e8;

	for(j=0;j<m_iNumOfY;j++)
	{
		for(i=0;i<m_iNumOfX;i++)
		{
			ci	= j*m_iNumOfX+i;
			e	= m_pDemObj[ci];

			//e的排列情况
			//
			//	e5	e2	e6
			//	e1	e	e3
			//	e8	e4	e7
			//	

			//e1
			I	= i-1;	J	= j;
			if(I<0)	I	= 0;	if(I>m_iNumOfX-1)	I	= m_iNumOfX-1;
			if(J<0)	J	= 0;	if(J>m_iNumOfY-1)	J	= m_iNumOfY-1;
			e1	= m_pDemObj[J*m_iNumOfX + I];

			//e2
			I	= i;	J	= j+1;
			if(I<0)	I	= 0;	if(I>m_iNumOfX-1)	I	= m_iNumOfX-1;
			if(J<0)	J	= 0;	if(J>m_iNumOfY-1)	J	= m_iNumOfY-1;
			e2	= m_pDemObj[J*m_iNumOfX + I];

			//e3
			I	= i+1;	J	= j;
			if(I<0)	I	= 0;	if(I>m_iNumOfX-1)	I	= m_iNumOfX-1;
			if(J<0)	J	= 0;	if(J>m_iNumOfY-1)	J	= m_iNumOfY-1;
			e3	= m_pDemObj[J*m_iNumOfX + I];

			//e4
			I	= i;	J	= j-1;
			if(I<0)	I	= 0;	if(I>m_iNumOfX-1)	I	= m_iNumOfX-1;
			if(J<0)	J	= 0;	if(J>m_iNumOfY-1)	J	= m_iNumOfY-1;
			e4	= m_pDemObj[J*m_iNumOfX + I];

			//e5
			I	= i-1;	J	= j+1;
			if(I<0)	I	= 0;	if(I>m_iNumOfX-1)	I	= m_iNumOfX-1;
			if(J<0)	J	= 0;	if(J>m_iNumOfY-1)	J	= m_iNumOfY-1;
			e5	= m_pDemObj[J*m_iNumOfX + I];

			//e6
			I	= i+1;	J	= j+1;
			if(I<0)	I	= 0;	if(I>m_iNumOfX-1)	I	= m_iNumOfX-1;
			if(J<0)	J	= 0;	if(J>m_iNumOfY-1)	J	= m_iNumOfY-1;
			e6	= m_pDemObj[J*m_iNumOfX + I];

			//e7
			I	= i+1;	J	= j-1;
			if(I<0)	I	= 0;	if(I>m_iNumOfX-1)	I	= m_iNumOfX-1;
			if(J<0)	J	= 0;	if(J>m_iNumOfY-1)	J	= m_iNumOfY-1;
			e7	= m_pDemObj[J*m_iNumOfX + I];

			//e8
			I	= i-1;	J	= j-1;
			if(I<0)	I	= 0;	if(I>m_iNumOfX-1)	I	= m_iNumOfX-1;
			if(J<0)	J	= 0;	if(J>m_iNumOfY-1)	J	= m_iNumOfY-1;
			e8	= m_pDemObj[J*m_iNumOfX + I];
			
			//
			double	dZx,dZy;
			double	dZx2,dZy2,dZxy;

			dZx		= (e3-e1)/(2*m_dSizeOfX);
			dZy		= (e2-e4)/(2*m_dSizeOfY);
			dZx2	= (e3-2*e+e1)/(m_dSizeOfX*m_dSizeOfX);
			dZy2	= (e2-2*e+e4)/(m_dSizeOfY*m_dSizeOfY);
			dZxy	= (e6+e8-e5-e7)/(4*m_dSizeOfX*m_dSizeOfY);

			double	dP,dQ;
			dP		= dZx*dZx+dZy*dZy;
			dQ		= 1+dP;

			//剖面曲率为：
			m_pProfileCurvatureObj[ci]	= ( dZx2*(dZx*dZx)+2*dZxy*dZx*dZy+dZy2*(dZy*dZy) ) / ( dP * dQ *sqrt(dQ) ) * 100;
			
			if( fabs(m_pProfileCurvatureObj[ci]) < 1e-10)	m_pProfileCurvatureObj[ci]	= 0;
		}
	}

	//完成之后，应该是直接计算坡度的统计信息
	{
		CDCR_Statistics	pStatistics;
		//
		pStatistics.SetStatisticsData(m_iNumOfX*m_iNumOfY,m_pProfileCurvatureObj);
		//
		pStatistics.Statistics();
		
		double	dVar;
		double	dCV;
		double	dSkewness;
		double	dKurtosis;
		double	dRMSE;

		pStatistics.GetDataMinMax(m_dMinProfileCurvature,m_dMaxProfileCurvature);
		pStatistics.GetDataMean(m_dMeanProfileCurvature);
		pStatistics.GetDataVar(dVar);
		pStatistics.GetDataStandardVar(m_dStdProfileCurvature);
		pStatistics.GetDataCV(dCV);
		pStatistics.GetDataSkewness(dSkewness);
		pStatistics.GetDataKurtosis(dKurtosis);
		pStatistics.GetDataRMSE(dRMSE);

		//
		CString	szPathName;
		szPathName	= m_szPathName.Left(szPathName.GetLength()-4) + "_ProfileCurvature.txt";
		pStatistics.Save(szPathName);
	}
}

//////////////////////////////////////////////////////////////////////
// 34 计算平面曲率
//////////////////////////////////////////////////////////////////////
void CDCR_Curvature::OnCalculatePlanCurvature()
{
	if(m_pPlanCurvatureObj != NULL)
	{
		delete	[]m_pPlanCurvatureObj;	m_pPlanCurvatureObj	= NULL;
	}

	m_dMinPlanCurvature	= 0.0;
	m_dMaxPlanCurvature	= 0.0;

	m_pPlanCurvatureObj	= new	double[m_iNumOfX*m_iNumOfY];

	if(m_pPlanCurvatureObj == NULL)	return;

	int		i,j,ci,I,J;
	double	e;
	double	e1,e2,e3,e4;
	double	e5,e6,e7,e8;

	for(j=0;j<m_iNumOfY;j++)
	{
		for(i=0;i<m_iNumOfX;i++)
		{
			ci	= j*m_iNumOfX+i;
			e	= m_pDemObj[ci];

			//e的排列情况
			//
			//	e5	e2	e6
			//	e1	e	e3
			//	e8	e4	e7
			//	

			//e1
			I	= i-1;	J	= j;
			if(I<0)	I	= 0;	if(I>m_iNumOfX-1)	I	= m_iNumOfX-1;
			if(J<0)	J	= 0;	if(J>m_iNumOfY-1)	J	= m_iNumOfY-1;
			e1	= m_pDemObj[J*m_iNumOfX + I];

			//e2
			I	= i;	J	= j+1;
			if(I<0)	I	= 0;	if(I>m_iNumOfX-1)	I	= m_iNumOfX-1;
			if(J<0)	J	= 0;	if(J>m_iNumOfY-1)	J	= m_iNumOfY-1;
			e2	= m_pDemObj[J*m_iNumOfX + I];

			//e3
			I	= i+1;	J	= j;
			if(I<0)	I	= 0;	if(I>m_iNumOfX-1)	I	= m_iNumOfX-1;
			if(J<0)	J	= 0;	if(J>m_iNumOfY-1)	J	= m_iNumOfY-1;
			e3	= m_pDemObj[J*m_iNumOfX + I];

			//e4
			I	= i;	J	= j-1;
			if(I<0)	I	= 0;	if(I>m_iNumOfX-1)	I	= m_iNumOfX-1;
			if(J<0)	J	= 0;	if(J>m_iNumOfY-1)	J	= m_iNumOfY-1;
			e4	= m_pDemObj[J*m_iNumOfX + I];

			//e5
			I	= i-1;	J	= j+1;
			if(I<0)	I	= 0;	if(I>m_iNumOfX-1)	I	= m_iNumOfX-1;
			if(J<0)	J	= 0;	if(J>m_iNumOfY-1)	J	= m_iNumOfY-1;
			e5	= m_pDemObj[J*m_iNumOfX + I];

			//e6
			I	= i+1;	J	= j+1;
			if(I<0)	I	= 0;	if(I>m_iNumOfX-1)	I	= m_iNumOfX-1;
			if(J<0)	J	= 0;	if(J>m_iNumOfY-1)	J	= m_iNumOfY-1;
			e6	= m_pDemObj[J*m_iNumOfX + I];

			//e7
			I	= i+1;	J	= j-1;
			if(I<0)	I	= 0;	if(I>m_iNumOfX-1)	I	= m_iNumOfX-1;
			if(J<0)	J	= 0;	if(J>m_iNumOfY-1)	J	= m_iNumOfY-1;
			e7	= m_pDemObj[J*m_iNumOfX + I];

			//e8
			I	= i-1;	J	= j-1;
			if(I<0)	I	= 0;	if(I>m_iNumOfX-1)	I	= m_iNumOfX-1;
			if(J<0)	J	= 0;	if(J>m_iNumOfY-1)	J	= m_iNumOfY-1;
			e8	= m_pDemObj[J*m_iNumOfX + I];
			
			//
			double	dZx,dZy;
			double	dZx2,dZy2,dZxy;

			dZx		= (e3-e1)/(2*m_dSizeOfX);
			dZy		= (e2-e4)/(2*m_dSizeOfY);
			dZx2	= (e3-2*e+e1)/(m_dSizeOfX*m_dSizeOfX);
			dZy2	= (e2-2*e+e4)/(m_dSizeOfY*m_dSizeOfY);
			dZxy	= (e6+e8-e5-e7)/(4*m_dSizeOfX*m_dSizeOfY);

			double	dP;
			dP		= dZx*dZx+dZy*dZy;
			
			if(fabs(dP) < 1e-10)	m_pPlanCurvatureObj[ci]	= 0;
			else
			{
				//剖面曲率为：
				m_pPlanCurvatureObj[ci]	= ( dZx2*(dZx*dZx)-2*dZxy*dZx*dZy+dZy2*(dZy*dZy) ) / ( dP *sqrt(dP) ) * 100;
				
				if( fabs(m_pPlanCurvatureObj[ci]) < 1e-10)	m_pPlanCurvatureObj[ci]	= 0;
			}
		}
	}


	//完成之后，应该是直接计算坡度的统计信息
	{
		CDCR_Statistics	pStatistics;
		//
		pStatistics.SetStatisticsData(m_iNumOfX*m_iNumOfY,m_pPlanCurvatureObj);
		//
		pStatistics.Statistics();
		
		double	dVar;
		double	dCV;
		double	dSkewness;
		double	dKurtosis;
		double	dRMSE;

		pStatistics.GetDataMinMax(m_dMinPlanCurvature,m_dMaxPlanCurvature);
		pStatistics.GetDataMean(m_dMeanPlanCurvature);
		pStatistics.GetDataVar(dVar);
		pStatistics.GetDataStandardVar(m_dStdPlanCurvature);
		pStatistics.GetDataCV(dCV);
		pStatistics.GetDataSkewness(dSkewness);
		pStatistics.GetDataKurtosis(dKurtosis);
		pStatistics.GetDataRMSE(dRMSE);

		//
		CString	szPathName;
		szPathName	= m_szPathName.Left(szPathName.GetLength()-4) + "_PlanCurvature.txt";
		pStatistics.Save(szPathName);
	}
}

//////////////////////////////////////////////////////////////////////
// 36 计算切线曲率曲率
//////////////////////////////////////////////////////////////////////
void CDCR_Curvature::OnCalculateTgCurvature()
{
	if(m_pTgCurvatureObj != NULL)
	{
		delete	[]m_pTgCurvatureObj;		m_pTgCurvatureObj	= NULL;
	}

	m_dMinTgCurvature	= 0.0;
	m_dMaxTgCurvature	= 0.0;

	m_pTgCurvatureObj	= new	double[m_iNumOfX*m_iNumOfY];

	if(m_pTgCurvatureObj == NULL)	return;

	int		i,j,ci,I,J;
	double	e;
	double	e1,e2,e3,e4;
	double	e5,e6,e7,e8;

	for(j=0;j<m_iNumOfY;j++)
	{
		for(i=0;i<m_iNumOfX;i++)
		{
			ci	= j*m_iNumOfX+i;
			e	= m_pDemObj[ci];

			//e的排列情况
			//
			//	e5	e2	e6
			//	e1	e	e3
			//	e8	e4	e7
			//	

			//e1
			I	= i-1;	J	= j;
			if(I<0)	I	= 0;	if(I>m_iNumOfX-1)	I	= m_iNumOfX-1;
			if(J<0)	J	= 0;	if(J>m_iNumOfY-1)	J	= m_iNumOfY-1;
			e1	= m_pDemObj[J*m_iNumOfX + I];

			//e2
			I	= i;	J	= j+1;
			if(I<0)	I	= 0;	if(I>m_iNumOfX-1)	I	= m_iNumOfX-1;
			if(J<0)	J	= 0;	if(J>m_iNumOfY-1)	J	= m_iNumOfY-1;
			e2	= m_pDemObj[J*m_iNumOfX + I];

			//e3
			I	= i+1;	J	= j;
			if(I<0)	I	= 0;	if(I>m_iNumOfX-1)	I	= m_iNumOfX-1;
			if(J<0)	J	= 0;	if(J>m_iNumOfY-1)	J	= m_iNumOfY-1;
			e3	= m_pDemObj[J*m_iNumOfX + I];

			//e4
			I	= i;	J	= j-1;
			if(I<0)	I	= 0;	if(I>m_iNumOfX-1)	I	= m_iNumOfX-1;
			if(J<0)	J	= 0;	if(J>m_iNumOfY-1)	J	= m_iNumOfY-1;
			e4	= m_pDemObj[J*m_iNumOfX + I];

			//e5
			I	= i-1;	J	= j+1;
			if(I<0)	I	= 0;	if(I>m_iNumOfX-1)	I	= m_iNumOfX-1;
			if(J<0)	J	= 0;	if(J>m_iNumOfY-1)	J	= m_iNumOfY-1;
			e5	= m_pDemObj[J*m_iNumOfX + I];

			//e6
			I	= i+1;	J	= j+1;
			if(I<0)	I	= 0;	if(I>m_iNumOfX-1)	I	= m_iNumOfX-1;
			if(J<0)	J	= 0;	if(J>m_iNumOfY-1)	J	= m_iNumOfY-1;
			e6	= m_pDemObj[J*m_iNumOfX + I];

			//e7
			I	= i+1;	J	= j-1;
			if(I<0)	I	= 0;	if(I>m_iNumOfX-1)	I	= m_iNumOfX-1;
			if(J<0)	J	= 0;	if(J>m_iNumOfY-1)	J	= m_iNumOfY-1;
			e7	= m_pDemObj[J*m_iNumOfX + I];

			//e8
			I	= i-1;	J	= j-1;
			if(I<0)	I	= 0;	if(I>m_iNumOfX-1)	I	= m_iNumOfX-1;
			if(J<0)	J	= 0;	if(J>m_iNumOfY-1)	J	= m_iNumOfY-1;
			e8	= m_pDemObj[J*m_iNumOfX + I];
			
			//
			double	dZx,dZy;
			double	dZx2,dZy2,dZxy;

			dZx		= (e3-e1)/(2*m_dSizeOfX);
			dZy		= (e2-e4)/(2*m_dSizeOfY);
			dZx2	= (e3-2*e+e1)/(m_dSizeOfX*m_dSizeOfX);
			dZy2	= (e2-2*e+e4)/(m_dSizeOfY*m_dSizeOfY);
			dZxy	= (e6+e8-e5-e7)/(4*m_dSizeOfX*m_dSizeOfY);

			double	dP,dQ;
			dP		= dZx*dZx+dZy*dZy;
			dQ		= 1+dP;

			//剖面曲率为：
			m_pTgCurvatureObj[ci]	= ( dZx2*(dZx*dZx)-2*dZxy*dZx*dZy+dZy2*(dZy*dZy) ) / ( dP *sqrt(dQ) ) * 100;
			
			if( fabs(m_pTgCurvatureObj[ci]) < 1e-10)	m_pTgCurvatureObj[ci]	= 0;
		}
	}

	//完成之后，应该是直接计算坡度的统计信息
	{
		CDCR_Statistics	pStatistics;
		//
		pStatistics.SetStatisticsData(m_iNumOfX*m_iNumOfY,m_pTgCurvatureObj);
		//
		pStatistics.Statistics();
		
		double	dVar;
		double	dCV;
		double	dSkewness;
		double	dKurtosis;
		double	dRMSE;

		pStatistics.GetDataMinMax(m_dMinTgCurvature,m_dMaxTgCurvature);
		pStatistics.GetDataMean(m_dMeanTgCurvature);
		pStatistics.GetDataVar(dVar);
		pStatistics.GetDataStandardVar(m_dStdTgCurvature);
		pStatistics.GetDataCV(dCV);
		pStatistics.GetDataSkewness(dSkewness);
		pStatistics.GetDataKurtosis(dKurtosis);
		pStatistics.GetDataRMSE(dRMSE);

		//
		CString	szPathName;
		szPathName	= m_szPathName.Left(szPathName.GetLength()-4) + "_TgCurvature.txt";
		pStatistics.Save(szPathName);
	}
}

//
double* CDCR_Curvature::GetProfileCurvature()
{
	return	m_pProfileCurvatureObj;
}

void CDCR_Curvature::GetProfileCurvatureStatisticsInfo(double& dMin,double& dMax, double& dMean,double& dStd)
{
	//
	dMin	= m_dMinProfileCurvature;
	dMax	= m_dMaxProfileCurvature;

	dMean	= m_dMeanProfileCurvature;
	dStd	= m_dStdProfileCurvature;
}

//
double* CDCR_Curvature::GetPlanCurvature()
{
	return	m_pPlanCurvatureObj;
}

void CDCR_Curvature::GetPlanCurvatureStatisticsInfo(double& dMin,double& dMax, double& dMean,double& dStd)
{
	dMin	= m_dMinPlanCurvature;
	dMax	= m_dMaxPlanCurvature;

	dMean	= m_dMeanPlanCurvature;
	dStd	= m_dStdPlanCurvature;
}

//
double* CDCR_Curvature::GetTgCurvature()
{
	return	m_pTgCurvatureObj;
}
	
void CDCR_Curvature::GetTgCurvatureStatisticsInfo(double& dMin,double& dMax, double& dMean,double& dStd)
{
	dMin	= m_dMinTgCurvature;
	dMax	= m_dMaxTgCurvature;

	dMean	= m_dMeanTgCurvature;
	dStd	= m_dStdTgCurvature;
}

//
bool CDCR_Curvature::OnSave( )
{
	//保存平面曲率
	OnSavePlanCurvature();

	//保存剖面曲率
	OnSaveProfileCurvature();

	return	true;
}

//
void CDCR_Curvature::OnDraw(CDC *pDC)
{
	
}

//
void CDCR_Curvature::OnRelease()
{
	if(m_pProfileCurvatureObj != NULL)
	{
		delete	[]m_pProfileCurvatureObj;	m_pProfileCurvatureObj	= NULL;
	}

	if(m_pPlanCurvatureObj != NULL)
	{
		delete	[]m_pPlanCurvatureObj;		m_pPlanCurvatureObj		= NULL;
	}

	if(m_pTgCurvatureObj != NULL)
	{
		delete	[]m_pTgCurvatureObj;		m_pTgCurvatureObj		= NULL;
	}
}

//
void CDCR_Curvature::SetClientRect(CRect rRect)
{
	m_rRect	= rRect;
}

//
void CDCR_Curvature::SetCoordMapping(double dXA, double dXB, double dYA, double dYB)
{
	m_dMapXA	= dXA;
	m_dMapXB	= dXB;
	m_dMapYA	= dYA;
	m_dMapYB	= dYB;
}

//////////////////////////////////////////////////////////////////////
// 51 绘制剖面曲率
//////////////////////////////////////////////////////////////////////
void CDCR_Curvature::OnDrawProfileCurvature(CDC *pDC)
{	
	if(m_pProfileCurvatureObj == NULL)		return;

	int		i,j,ci;
	double	dX,dY,dProfileCurvature;

	CDCR_CurvatureSet::Instance()->SetMinCurvature(m_dMinProfileCurvature);
	CDCR_CurvatureSet::Instance()->SetMidCurvature((m_dMinProfileCurvature+m_dMaxProfileCurvature)/2);
	CDCR_CurvatureSet::Instance()->SetMaxCurvature(m_dMaxProfileCurvature);


	for(j=0;j<m_iNumOfY;j++)
	{
		for(i=0;i<m_iNumOfX;i++)
		{
			ci		= j*m_iNumOfX+i;
			dX		= m_dDemLeft + i*m_dSizeOfX;
			dY		= m_dDemBottom + j*m_dSizeOfY;
			
			dProfileCurvature	= m_pProfileCurvatureObj[ci];
			
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
			
			crColor	= CDCR_CurvatureSet::Instance()->GetColor(dProfileCurvature);

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

//////////////////////////////////////////////////////////////////////
// 52 绘制剖面曲率
//////////////////////////////////////////////////////////////////////
void CDCR_Curvature::OnDrawPlanCurvature(CDC *pDC)
{
	if(m_pPlanCurvatureObj == NULL)		return;

	int		i,j,ci;
	double	dX,dY,dPlanCurvature;

	CDCR_CurvatureSet::Instance()->SetMinCurvature(m_dMinPlanCurvature);
	CDCR_CurvatureSet::Instance()->SetMidCurvature((m_dMinPlanCurvature+m_dMaxPlanCurvature)/2);
	CDCR_CurvatureSet::Instance()->SetMaxCurvature(m_dMaxPlanCurvature);


	for(j=0;j<m_iNumOfY;j++)
	{
		for(i=0;i<m_iNumOfX;i++)
		{
			ci		= j*m_iNumOfX+i;
			dX		= m_dDemLeft + i*m_dSizeOfX;
			dY		= m_dDemBottom + j*m_dSizeOfY;
			
			dPlanCurvature	= m_pPlanCurvatureObj[ci];
			
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
			
			crColor	= CDCR_CurvatureSet::Instance()->GetColor(dPlanCurvature);

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

//////////////////////////////////////////////////////////////////////
// 53 绘制切线曲率
//////////////////////////////////////////////////////////////////////
void CDCR_Curvature::OnDrawTgCurvature(CDC *pDC)
{
	if(m_pTgCurvatureObj == NULL)		return;

	int		i,j,ci;
	double	dX,dY,dTgCurvature;

	CDCR_CurvatureSet::Instance()->SetMinCurvature(m_dMinTgCurvature);
	CDCR_CurvatureSet::Instance()->SetMidCurvature((m_dMinTgCurvature+m_dMaxTgCurvature)/2);
	CDCR_CurvatureSet::Instance()->SetMaxCurvature(m_dMaxTgCurvature);

	for(j=0;j<m_iNumOfY;j++)
	{
		for(i=0;i<m_iNumOfX;i++)
		{
			ci		= j*m_iNumOfX+i;
			dX		= m_dDemLeft + i*m_dSizeOfX;
			dY		= m_dDemBottom + j*m_dSizeOfY;
			
			dTgCurvature	= m_pTgCurvatureObj[ci];
			
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
			
			crColor	= CDCR_CurvatureSet::Instance()->GetColor(dTgCurvature);

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

//////////////////////////////////////////////////////////////////////
// 06 保存平面曲率信息为GRD文件
//////////////////////////////////////////////////////////////////////
bool CDCR_Curvature::OnSavePlanCurvature()
{
	//假如数据为NULL
	if(m_pPlanCurvatureObj == NULL)	return	false;
	
	//坡度信息
	CString	szPathName;
	szPathName	= m_szPathName.Left(m_szPathName.GetLength()-4) + "(Plan).grd";
	
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
			double	dCurvature;
			dCurvature	= m_pPlanCurvatureObj[j*m_iNumOfX+i];
			if(i==0 && j==0)
			{
				m_dMinPlanCurvature	= m_dMaxPlanCurvature	= dCurvature;
			}
			else
			{
				if(dCurvature < m_dMinPlanCurvature)	m_dMinPlanCurvature	= dCurvature;
				if(dCurvature > m_dMaxPlanCurvature)	m_dMaxPlanCurvature	= dCurvature;
			}
		}
	}
	
	//m_dMinPlanCurvature,m_dMaxPlanCurvature
	fprintf(fp,"%.6lf %.6lf\n",m_dMinPlanCurvature,m_dMaxPlanCurvature);
	
	//
	for(j=0;j<m_iNumOfY;j++)
	{
		for(i=0;i<m_iNumOfX;i++)
		{
			double	dCurvature;
			dCurvature	= m_pPlanCurvatureObj[j*m_iNumOfX+i];

			fprintf(fp,"%.6lf ",dCurvature);

		}
		fprintf(fp,"\n");
	}
	
	//关闭
	fclose(fp);
		
	return	true;
}

//////////////////////////////////////////////////////////////////////
// 07 保存剖面曲率信息为GRD文件
//////////////////////////////////////////////////////////////////////
bool CDCR_Curvature::OnSaveProfileCurvature()
{
	//假如数据为NULL
	if(m_pProfileCurvatureObj == NULL)	return	false;
	
	//坡度信息
	CString	szPathName;
	szPathName	= m_szPathName.Left(m_szPathName.GetLength()-4) + "(Profile).grd";
	
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
			double	dCurvature;
			dCurvature	= m_pProfileCurvatureObj[j*m_iNumOfX+i];
			if(i==0 && j==0)
			{
				m_dMinProfileCurvature	= m_dMaxProfileCurvature	= dCurvature;
			}
			else
			{
				if(dCurvature < m_dMinProfileCurvature)	m_dMinProfileCurvature	= dCurvature;
				if(dCurvature > m_dMaxProfileCurvature)	m_dMaxProfileCurvature	= dCurvature;
			}
		}
	}
	
	//m_dMinProfileCurvature,m_dMaxProfileCurvature
	fprintf(fp,"%.6lf %.6lf\n",m_dMinProfileCurvature,m_dMaxProfileCurvature);
	
	//
	for(j=0;j<m_iNumOfY;j++)
	{
		for(i=0;i<m_iNumOfX;i++)
		{
			double	dCurvature;
			dCurvature	= m_pProfileCurvatureObj[j*m_iNumOfX+i];

			fprintf(fp,"%.6lf ",dCurvature);

		}
		fprintf(fp,"\n");
	}
	
	//关闭
	fclose(fp);
		
	return	true;
}

//50 设置/获取曲率显示状态
void CDCR_Curvature::SetShowStatus(bool bShowStatus)
{
	m_bShowStatusInCurvature	= bShowStatus;
}

bool CDCR_Curvature::GetShowStatus()
{
	return	m_bShowStatusInCurvature;
}