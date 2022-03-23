// DCR_Aspect.cpp: implementation of the CDCR_Aspect class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "vr_demcreator.h"
#include "DCR_Aspect.h"

#include "DCR_Statistics.h"
#include "DCR_AspectSet.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDCR_Aspect::CDCR_Aspect()
{
	//
	m_pDemObj			= NULL;
	
	//
	m_pAspectObj			= NULL;
	m_dMinAspect			= m_dMaxAspect	= 0;
	
	//
	m_iNumOfX			= 0;
	m_iNumOfY			= 0;
	
	//
	m_dDemLeft			= m_dDemRight	= 0;
	m_dDemBottom		= m_dDemTop		= 0;
	
	//
	m_dSizeOfX			= 0;
	m_dSizeOfY			= 0;
	
	//
	m_dRealSize			= 1.0;
	
	m_bShowStatus		= false;
}

CDCR_Aspect::~CDCR_Aspect()
{
	OnRelease();
}

//
void CDCR_Aspect::SetGridDem(double *pData, int iNumOfX, int iNumOfY, double dSizeOfX, double dSizeOfY, double dRealSize, double dLeft, double dRight, double dBottom, double dTop,CString szPathName)
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

void CDCR_Aspect::OnCalculateAspect()
{
	if(m_pAspectObj != NULL)		{	delete	[]m_pAspectObj;		m_pAspectObj	= NULL;		}
	
	m_dMinAspect	= 0.0;
	m_dMaxAspect	= 0.0;
	
	m_pAspectObj	= new	double[m_iNumOfX*m_iNumOfY];
	
	if(m_pAspectObj == NULL)		return;
	
	int		i,j,ci,I,J;
	double	e,e1,e2,e3,e4;
	double	dSlopeWE,dSlopeSN;
	
	for(j=0;j<m_iNumOfY;j++)
	{
		for(i=0;i<m_iNumOfX;i++)
		{
			m_pAspectObj[j*m_iNumOfX+i]	= INVALID_VALUE;
		}
	}
	
	for(j=0;j<m_iNumOfY;j++)
	{
		for(i=0;i<m_iNumOfX;i++)
		{
			ci	= j*m_iNumOfX+i;
			e	= m_pDemObj[ci];
			
			if(e == INVALID_HEIGHT)	continue;
			
			//e的排列情况
			//
			//	  	e2	  
			//	e1	e	e3
			//	  	e4	 
			//	
			//e1
			I	= i-1;	J	= j;
			if(I<0)	I	= 0;	if(I>m_iNumOfX-1)	I	= m_iNumOfX-1;
			if(J<0)	J	= 0;	if(J>m_iNumOfY-1)	J	= m_iNumOfY-1;
			e1	= m_pDemObj[J*m_iNumOfX + I];
			if(e1 == INVALID_HEIGHT)	continue;
			
			//e2
			I	= i;	J	= j+1;
			if(I<0)	I	= 0;	if(I>m_iNumOfX-1)	I	= m_iNumOfX-1;
			if(J<0)	J	= 0;	if(J>m_iNumOfY-1)	J	= m_iNumOfY-1;
			e2	= m_pDemObj[J*m_iNumOfX + I];
			if(e2 == INVALID_HEIGHT)	continue;
			
			//e3
			I	= i+1;	J	= j;
			if(I<0)	I	= 0;	if(I>m_iNumOfX-1)	I	= m_iNumOfX-1;
			if(J<0)	J	= 0;	if(J>m_iNumOfY-1)	J	= m_iNumOfY-1;
			e3	= m_pDemObj[J*m_iNumOfX + I];
			if(e3 == INVALID_HEIGHT)	continue;
			
			//e4
			I	= i;	J	= j-1;
			if(I<0)	I	= 0;	if(I>m_iNumOfX-1)	I	= m_iNumOfX-1;
			if(J<0)	J	= 0;	if(J>m_iNumOfY-1)	J	= m_iNumOfY-1;
			e4	= m_pDemObj[J*m_iNumOfX + I];
			if(e4 == INVALID_HEIGHT)	continue;
			
			dSlopeWE	= (e1-e3)/(2*m_dSizeOfX*m_dRealSize);
			dSlopeSN	= (e4-e2)/(2*m_dSizeOfY*m_dRealSize);
			
			// m_pAspectObj[ci]	= 270- 180 / PI * atan( dSlopeWE/dSlopeSN );
			
			if (fabs(dSlopeWE) < 1e-10)
			{
				m_pAspectObj[ci]	= 270 + 180 / PI * atan( dSlopeWE/dSlopeSN );
			}
			else
			{
				m_pAspectObj[ci]	= 270 + 180 / PI * atan( dSlopeWE/dSlopeSN ) - 90*dSlopeSN/(fabs(dSlopeSN));
			}

			if (m_pAspectObj[ci] > 360)
			{
				m_pAspectObj[ci]	= m_pAspectObj[ci]-360;
			}

			//计算最大最小坡度
			if(i==0 && j==0)
			{
				m_dMinAspect	= m_pAspectObj[ci];
				m_dMaxAspect	= m_pAspectObj[ci];
			}
			else
			{
				if(m_dMinAspect > m_pAspectObj[ci])	m_dMinAspect	= m_pAspectObj[ci];
				if(m_dMaxAspect < m_pAspectObj[ci])	m_dMaxAspect	= m_pAspectObj[ci];
			}
		}
	}	
}

//
void CDCR_Aspect::OnCalculate(int iType)
{
	if(m_pDemObj == NULL)	return;
	
	//
	//if(iType == 0)	
	OnCalculateAspect();	
	
	//完成之后，应该是直接计算坡向的统计信息
	{
		CDCR_Statistics	pStatistics;
		//
		pStatistics.SetStatisticsData(m_iNumOfX*m_iNumOfY,m_pAspectObj);
		//
		pStatistics.Statistics();
		
		double	dMin,dMax;
		double	dVar;
		double	dCV;
		double	dSkewness;
		double	dKurtosis;
		double	dRMSE;
		
		pStatistics.GetDataMinMax(dMin,dMax);
		pStatistics.GetDataMean(m_dMeanAspect);
		pStatistics.GetDataVar(dVar);
		pStatistics.GetDataStandardVar(m_dStdAspect);
		pStatistics.GetDataCV(dCV);
		pStatistics.GetDataSkewness(dSkewness);
		pStatistics.GetDataKurtosis(dKurtosis);
		pStatistics.GetDataRMSE(dRMSE);
		
		//
		CString	szPathName;
		szPathName	= m_szPathName.Left(szPathName.GetLength()-4) + "_Aspect.txt";
		pStatistics.Save(szPathName);
	}
}

void CDCR_Aspect::OnRelease()
{
	//
	if(m_pAspectObj != NULL)				{	delete	[]m_pAspectObj;				m_pAspectObj			= NULL;		}
}

//
void CDCR_Aspect::OnDraw(CDC *pDC)
{
	if(m_pAspectObj == NULL)				return;
	
	if(m_bShowStatus == false)				return;
	
	int		i,j,ci;
	double	dX,dY,dAspect;
	
	CDCR_AspectSet::Instance()->SetMinAspect(m_dMinAspect);
	CDCR_AspectSet::Instance()->SetMidAspect((m_dMinAspect+m_dMaxAspect)/2);
	CDCR_AspectSet::Instance()->SetMaxAspect(m_dMaxAspect);
	
	double	dShowLeft,dShowRight,dShowBottom,dShowTop;
	//计算实际的显示区域
	dShowLeft	= (m_rRect.left - m_dMapXB) / m_dMapXA;
	dShowRight	= (m_rRect.right - m_dMapXB) / m_dMapXA;
	dShowTop	= (m_rRect.top - m_dMapYB) / m_dMapYA;
	dShowBottom	= (m_rRect.bottom - m_dMapYB) / m_dMapYA;
	
	for(j=0;j<m_iNumOfY;j++)
	{
		for(i=0;i<m_iNumOfX;i++)
		{
			ci		= j*m_iNumOfX+i;
			dX		= m_dDemLeft + i*m_dSizeOfX;
			dY		= m_dDemBottom + j*m_dSizeOfY;
			
			if(m_pAspectObj[ci] == INVALID_VALUE)	continue;
			
			if((dX>=dShowLeft-m_dSizeOfX&&dX<=dShowRight+m_dSizeOfX) && (dY>=dShowBottom-m_dSizeOfY&&dY<=dShowTop+m_dSizeOfY))
			{
				dAspect	= m_pAspectObj[ci];
				
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
				
				crColor	= CDCR_AspectSet::Instance()->GetColor(dAspect);
				
				//
				CBrush	*pNewBrush,*pOldBrush;
				pNewBrush	= new	CBrush;
				pNewBrush->CreateSolidBrush(crColor);
				pOldBrush	= pDC->SelectObject(pNewBrush);
				
				pDC->FillRect(rectCell,pNewBrush);	
				
				pDC->SelectObject(pOldBrush);
				delete	pNewBrush;	
				
				//
				if(rectCell.Width() > 30)
				{
					CString	szInfo;
					szInfo.Format("%.2lf",dAspect);
					
					CFont font;
					font.CreateFont(12, 0, 000, 000, 400, 0, 0, 0, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_TT_ALWAYS, PROOF_QUALITY, VARIABLE_PITCH|FF_ROMAN, "宋体");
					
					CFont* oldfont=pDC->SelectObject(&font);
					int oldtextalign=pDC->SetTextAlign(TA_TOP|TA_CENTER);
					COLORREF oldtextcolor=pDC->SetTextColor(RGB(0,0,0));
					int oldbkmode=pDC->SetBkMode(TRANSPARENT);
					
					pDC->TextOut((rectCell.left+rectCell.right)/2,(rectCell.bottom+rectCell.top)/2-7,szInfo);
					
					pDC->SelectObject(oldfont);
					pDC->SetTextAlign(oldtextalign);
					pDC->SetTextColor(oldtextcolor);
					pDC->SetBkMode(oldbkmode);
				}
			}
		}
	}
}

//
void CDCR_Aspect::SetClientRect(CRect rRect)
{
	m_rRect	= rRect;
}

//
void CDCR_Aspect::SetCoordMapping(double dXA, double dXB, double dYA, double dYB)
{
	m_dMapXA	= dXA;
	m_dMapXB	= dXB;
	m_dMapYA	= dYA;
	m_dMapYB	= dYB;
}

//
double* CDCR_Aspect::GetAspect()
{
	return	m_pAspectObj;
}

void CDCR_Aspect::GetAspectStatisticsInfo(double& dMin, double &dMax, double& dMean,double& dStd)
{
	dMin		= m_dMinAspect;
	dMax		= m_dMaxAspect;
	
	dMean		= m_dMeanAspect;
	dStd		= m_dStdAspect;
}

//
bool CDCR_Aspect::OnSave()
{
	//假如数据为NULL
	if(m_pAspectObj == NULL)	return	false;
	
	//坡度信息
	CString	szPathName;
	szPathName	= m_szPathName.Left(m_szPathName.GetLength()-4) + "(Aspect).grd";
	
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
			double	dAspect;
			dAspect	= m_pAspectObj[j*m_iNumOfX+i];
			if(i==0 && j==0)
			{
				m_dMinAspect	= m_dMaxAspect	= dAspect;
			}
			else
			{
				if(dAspect < m_dMinAspect)	m_dMinAspect	= dAspect;
				if(dAspect > m_dMaxAspect)	m_dMaxAspect	= dAspect;
			}
		}
	}
	
	//dMinSlope,dMaxSlope
	fprintf(fp,"%.6lf %.6lf\n",m_dMinAspect,m_dMaxAspect);
	
	//
	for(j=0;j<m_iNumOfY;j++)
	{
		for(i=0;i<m_iNumOfX;i++)
		{
			double	dAspect;
			dAspect	= m_pAspectObj[j*m_iNumOfX+i];
			
			fprintf(fp,"%.6lf ",dAspect);
			
		}
		fprintf(fp,"\n");
	}
	
	//关闭
	fclose(fp);
	
	return	true;
}

//48 设置/获取坡度显示状态
void CDCR_Aspect::SetShowStatus(bool bShowStatus)
{
	m_bShowStatus	= bShowStatus;
}

bool CDCR_Aspect::GetShowStatus()
{
	return	m_bShowStatus;
}


