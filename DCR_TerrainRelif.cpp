// DCR_TerrainRelif.cpp: implementation of the CDCR_TerrainRelif class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "vr_demcreator.h"
#include "DCR_TerrainRelif.h"

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

CDCR_TerrainRelif::CDCR_TerrainRelif()
{
	//���ڳߴ�
	m_dMapXA	= m_dMapXB	= 0;
	m_dMapYA	= m_dMapYB	= 0;

	//�ֲڶ���Ϣ
	m_pRelifObj	= NULL;
	//�����С�ֲڶ�
	m_dMinRelif	= 0;
	m_dMaxRelif	= 0;
	//ƽ���ֲڶ�
	m_dMeanRelif= 0;
	//�ֲڶȷ���
	m_dStdRelif	= 0;
	
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

CDCR_TerrainRelif::~CDCR_TerrainRelif()
{
	OnRelease();
}

//
void CDCR_TerrainRelif::SetGridDem(double *pData,int iNumOfX,int iNumOfY,double dSizeOfX,double dSizeOfY,double dRealSize,double dLeft,double dRight,double dBottom,double dTop,CString szPathName)
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
void CDCR_TerrainRelif::OnCalculate(int iWindowSize)
{
	//������ڳߴ�û�и�ֵ����ȡĬ��ֵ
	if(iWindowSize == 0)	iWindowSize	= m_iWindowSize;	

	if(m_pRelifObj != NULL)
	{
		delete	[]m_pRelifObj;
		m_pRelifObj	= NULL;
	}

	m_dMinRelif	= 0;
	m_dMaxRelif	= 0;

	m_pRelifObj	= new	double[m_iNumOfX*m_iNumOfY];

	if(m_pRelifObj == NULL)
	{
		CString	szInfo;

		szInfo.Format("�ڼ�����������ʱ�����ߴ�Ϊ %ld ʱ���ܴ����ڴ�!\n������ͣ�������˳�!\n",iWindowSize);

		MessageBox(NULL,szInfo,"��ʾ��Ϣ",MB_OK);

		return;
	}

	int		i,j,ci;
	bool	bFirstValue;

	bFirstValue	= true;

	for(j=0;j<m_iNumOfY;j++)
	{
		for(i=0;i<m_iNumOfX;i++)
		{
			ci		= j*m_iNumOfX+i;
		
			if(m_pDemObj[ci] != INVALID_HEIGHT)
			{
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
					m_pRelifObj[ci]	= INVALID_VALUE;
				}
				else
				{
					//��iBegin,iEnd��jBegin,jEnd���д�����֤������
					//if(iBegin<0)		iBegin	= 0;	if(iBegin>m_iNumOfX-1)	iBegin	= m_iNumOfX-1;
					//if(iEnd<0)		iEnd	= 0;	if(iEnd>m_iNumOfX-1)	iEnd	= m_iNumOfX-1;
					
					//if(jBegin<0)		jBegin	= 0;	if(jBegin>m_iNumOfY-1)	jBegin	= m_iNumOfY-1;
					//if(jEnd<0)		jEnd	= 0;	if(jEnd>m_iNumOfY-1)	jEnd	= m_iNumOfY-1;

					//Ȼ�����õ�������
					//���ֵ
					double	dMaxH		= 0.0;
					
					//���ֵ
					double	dMinH		= 0.0;

					bool	bFirstValue	= true;
					
					for(int J=jBegin;J<=jEnd;J++)
					{
						for(int I=iBegin;I<=iEnd;I++)
						{
							if(m_pDemObj[I+J*m_iNumOfX] != INVALID_HEIGHT)
							{
								if(bFirstValue)
								{
									dMaxH	= dMinH	= m_pDemObj[I+J*m_iNumOfX];

									bFirstValue	= false;
								}
								else
								{
									if(dMaxH < m_pDemObj[I+J*m_iNumOfX])	dMaxH	= m_pDemObj[I+J*m_iNumOfX];
									if(dMinH > m_pDemObj[I+J*m_iNumOfX])	dMinH	= m_pDemObj[I+J*m_iNumOfX];
								}

							}
						}
					}

					m_pRelifObj[ci]	= dMaxH-dMinH;
				}
			}
			else
			{
				m_pRelifObj[ci]	= INVALID_VALUE;
			}

			//���������С�¶�
			if(m_pRelifObj[ci] != -9999.0)
			{
				if(bFirstValue)
				{
					m_dMinRelif	= m_pRelifObj[ci];
					m_dMaxRelif	= m_pRelifObj[ci];
					bFirstValue	= false;
				}
				else
				{
					if(m_dMinRelif > m_pRelifObj[ci])	m_dMinRelif	= m_pRelifObj[ci];
					if(m_dMaxRelif < m_pRelifObj[ci])	m_dMaxRelif	= m_pRelifObj[ci];
				}
			}
			
		}//for(i=0;i<m_iNumOfX;i++)

	}//for(j=0;j<m_iNumOfY;j++)

	//
	//���֮��Ӧ����ֱ�Ӽ����¶ȵ�ͳ����Ϣ
	{
		double	*dTemp;
		int		iTempNum;
		int		I,J;

		iTempNum	= 0;
		for(I=0;I<m_iNumOfX*m_iNumOfY;I++)
		{
			if(m_pRelifObj[I] != INVALID_VALUE)		iTempNum++;
		}

		J		= 0;
		dTemp	= new	double[iTempNum];
		for(I=0;I<m_iNumOfX*m_iNumOfY;I++)
		{
			if(m_pRelifObj[I] != INVALID_VALUE)		
			{
				dTemp[J]	= m_pRelifObj[I];
				J++;
			}
		}

		CDCR_Statistics	pStatistics;
		//
		pStatistics.SetStatisticsData(iTempNum,dTemp);
		//
		pStatistics.Statistics();
		
		pStatistics.GetDataMinMax(m_dMinRelif,m_dMaxRelif);
		pStatistics.GetDataMean(m_dMeanRelif);
		pStatistics.GetDataStandardVar(m_dStdRelif);

		//
		CString	szPathName;
		szPathName	= m_szPathName.Left(szPathName.GetLength()-4) + "_Relif.txt";
		pStatistics.Save(szPathName);

		if(dTemp != NULL)	delete	[]dTemp;
	}
}

//
void CDCR_TerrainRelif::OnRelease()
{
	if(m_pRelifObj != NULL)		{	delete	[]m_pRelifObj;	m_pRelifObj	= NULL;		}
}

//
void CDCR_TerrainRelif::OnDraw(CDC *pDC)
{
	if(m_pRelifObj == NULL)			return;

	if(m_bShowStatus == false)		return;

	int		i,j,ci;
	double	dX,dY,dTerrainRelif;

	CDCR_CurvatureSet::Instance()->SetMinCurvature(m_dMinRelif);
	CDCR_CurvatureSet::Instance()->SetMidCurvature((m_dMinRelif+m_dMaxRelif)/2);
	CDCR_CurvatureSet::Instance()->SetMaxCurvature(m_dMaxRelif);

	//
	for(j=0;j<m_iNumOfY;j++)
	{
		for(i=0;i<m_iNumOfX;i++)
		{
			ci		= j*m_iNumOfX+i;
			dX		= m_dDemLeft + i*m_dSizeOfX;
			dY		= m_dDemBottom + j*m_dSizeOfY;
			
			dTerrainRelif	= m_pRelifObj[ci];
			
			if(dTerrainRelif != INVALID_VALUE)
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
				
				crColor	= CDCR_CurvatureSet::Instance()->GetColor(dTerrainRelif);

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
					szInfo.Format("%.2lf",dTerrainRelif);
					
					CFont font;
					font.CreateFont(12, 0, 000, 000, 400, 0, 0, 0, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_TT_ALWAYS, PROOF_QUALITY, VARIABLE_PITCH|FF_ROMAN, "����");
					
					CFont* oldfont=pDC->SelectObject(&font);
					int oldtextalign=pDC->SetTextAlign(TA_TOP|TA_CENTER);
					COLORREF oldtextcolor=pDC->SetTextColor(RGB(255,0,0));
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
bool CDCR_TerrainRelif::OnSave()
{
	//��������ΪNULL
	if(m_pRelifObj == NULL)	return	false;
	
	//�¶���Ϣ
	CString	szPathName;
	szPathName	= m_szPathName.Left(m_szPathName.GetLength()-4) + "(Relif).grd";
	
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
			double	dRelif;
			dRelif	= m_pRelifObj[j*m_iNumOfX+i];
			if(i==0 && j==0)
			{
				m_dMinRelif	= m_dMaxRelif	= dRelif;
			}
			else
			{
				if(dRelif < m_dMinRelif)	m_dMinRelif	= dRelif;
				if(dRelif > m_dMaxRelif)	m_dMaxRelif	= dRelif;
			}
		}
	}
	
	//m_dMinRelif,m_dMaxRelif
	fprintf(fp,"%.6lf %.6lf\n",m_dMinRelif,m_dMaxRelif);
	
	//
	for(j=0;j<m_iNumOfY;j++)
	{
		for(i=0;i<m_iNumOfX;i++)
		{
			double	dRelif;
			dRelif	= m_pRelifObj[j*m_iNumOfX+i];

			fprintf(fp,"%.6lf ",dRelif);

		}
		fprintf(fp,"\n");
	}
	
	//�ر�
	fclose(fp);
		
	return	true;
}

//
double* CDCR_TerrainRelif::GetRelif()
{
	return	m_pRelifObj;
}

void CDCR_TerrainRelif::GetRelifStatisticsInfo(double& dMin,double& dMax, double& dMean,double& dStd)
{	
	dMin	= m_dMinRelif;
	dMax	= m_dMaxRelif;
	//ƽ���ֲڶ�
	dMean	= m_dMeanRelif;
	//�ֲڶȷ���
	dStd	= m_dStdRelif;
}

//
void CDCR_TerrainRelif::SetClientRect(CRect rRect)
{
	m_rRect	= rRect;
}

//
void CDCR_TerrainRelif::SetCoordMapping(double dXA,double dXB,double dYA,double dYB)
{
	m_dMapXA	= dXA;
	m_dMapXB	= dXB;
	m_dMapYA	= dYA;
	m_dMapYB	= dYB;
}

//�������ڳߴ�
void CDCR_TerrainRelif::SetWindowSize(int iSize)
{
	m_iWindowSize	= iSize;
}

int CDCR_TerrainRelif::GetWindowSize()
{
	return	m_iWindowSize;
}

//54 ����/��ȡ�ֲڶ���ʾ״̬
void CDCR_TerrainRelif::SetShowStatus(bool bShowStatus)
{
	m_bShowStatus	= bShowStatus;
}

bool CDCR_TerrainRelif::GetShowStatus()
{
	return	m_bShowStatus;
}
