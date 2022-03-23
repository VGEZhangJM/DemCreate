// DCR_OppositeHeight.cpp: implementation of the CDCR_OppositeHeight class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "vr_demcreator.h"
#include "DCR_OppositeHeight.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDCR_OppositeHeight::CDCR_OppositeHeight()
{
	m_pDemObj		= NULL;

	//��Ը���Ϣ
	m_pHeightObj	= NULL;
	//�����С��Ը�
	m_dMinHeight	= 0;
	m_dMaxHeight	= 0;
	//ƽ����Ը�
	m_dMeanHeight	= 0;
	//��Ը߷���
	m_dStdHeight	= 0;

	//11*11
	m_iWindowSize	= 5;

	m_pHeightObj	= NULL;
}

CDCR_OppositeHeight::~CDCR_OppositeHeight()
{

}

//��������
void CDCR_OppositeHeight::SetGridDem(double *pData,int iNumOfX,int iNumOfY,double dSizeOfX,double dSizeOfY,double dRealSize,double dLeft,double dRight,double dBottom,double dTop,CString szPathName)
{
	//
	m_pDemObj		= pData;

	//
	m_iNumOfX		= iNumOfX;
	m_iNumOfY		= iNumOfY;

	//
	m_dDemLeft		= dLeft;
	m_dDemRight		= dRight;
	m_dDemBottom	= dBottom;
	m_dDemTop		= dTop;

	//
	m_dSizeOfX		= dSizeOfX;
	m_dSizeOfY		= dSizeOfY;

	//
	m_dRealSize		= dRealSize;

	//
	m_szPathName	= szPathName;	
}

//�������ڳߴ�
void CDCR_OppositeHeight::SetWindowSize(int iSize)
{
	m_iWindowSize	= iSize;
}

int CDCR_OppositeHeight::GetWindowSize()
{
	return	m_iWindowSize;
}

//
bool CDCR_OppositeHeight::OnSave()
{
	return	false;
}

//
void CDCR_OppositeHeight::OnDraw(CDC *pDC)
{
	
}

//
void CDCR_OppositeHeight::OnRelease()
{
	if(m_pHeightObj != NULL)		{	delete	[]m_pHeightObj;		m_pHeightObj	= NULL;		}	
}

//
void CDCR_OppositeHeight::OnCalculate(int iWindowSize)
{
	//������ڳߴ�û�и�ֵ����ȡĬ��ֵ
	if(iWindowSize == 0)	iWindowSize	= m_iWindowSize;	

	if(m_pHeightObj != NULL)
	{
		delete	[]m_pHeightObj;
		m_pHeightObj	= NULL;
	}

	m_dMinHeight	= 0;
	m_dMaxHeight	= 0;

	m_pHeightObj	= new	double[m_iNumOfX*m_iNumOfY];

	if(m_pHeightObj == NULL)
	{
		CString	szInfo;

		szInfo.Format("�ڼ�����Ը�ʱ�����ߴ�Ϊ %ld ʱ���ܴ����ڴ�!\n������ͣ�������˳�!\n",iWindowSize);

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
				m_pHeightObj[ci]	= -9999.0;
			}
			else
			{
				//��iBegin,iEnd��jBegin,jEnd���д�����֤������
				//if(iBegin<0)		iBegin	= 0;	if(iBegin>m_iNumOfX-1)	iBegin	= m_iNumOfX-1;
				//if(iEnd<0)		iEnd	= 0;	if(iEnd>m_iNumOfX-1)	iEnd	= m_iNumOfX-1;
				
				//if(jBegin<0)		jBegin	= 0;	if(jBegin>m_iNumOfY-1)	jBegin	= m_iNumOfY-1;
				//if(jEnd<0)		jEnd	= 0;	if(jEnd>m_iNumOfY-1)	jEnd	= m_iNumOfY-1;

				double	dDeltaH		= 0.0;
								
				for(int J=jBegin;J<=jEnd;J++)
				{
					for(int I=iBegin;I<=iEnd;I++)
					{
						if(I==iBegin && J==jBegin)
						{
							dDeltaH	= fabs(m_pDemObj[I+J*m_iNumOfX] - m_pDemObj[i+j*m_iNumOfX]);
						}
						else
						{
							if(dDeltaH < fabs(m_pDemObj[I+J*m_iNumOfX] - m_pDemObj[i+j*m_iNumOfX]))
								dDeltaH	= fabs(m_pDemObj[I+J*m_iNumOfX] - m_pDemObj[i+j*m_iNumOfX]);
						}
					}
				}

				m_pHeightObj[ci]	= dDeltaH;
			}

			//���������С��Ը�
			if(m_pHeightObj[ci] != -9999.0)
			{
				if(bFirstValue)
				{
					m_dMinHeight	= m_pHeightObj[ci];
					m_dMaxHeight	= m_pHeightObj[ci];
					bFirstValue	= false;
				}
				else
				{
					if(m_dMinHeight > m_pHeightObj[ci])	m_dMinHeight	= m_pHeightObj[ci];
					if(m_dMaxHeight < m_pHeightObj[ci])	m_dMaxHeight	= m_pHeightObj[ci];
				}
			}
		}//for(i=0;i<m_iNumOfX;i++)

	}//for(j=0;j<m_iNumOfY;j++)
}

//
double* CDCR_OppositeHeight::GetOHeight()
{
	return	m_pHeightObj;
}

void CDCR_OppositeHeight::GetOHeightStatisticsInfo(double& dMin,double& dMax, double& dMean,double& dStd)
{
	dMin	= 0;
	dMax	= 0;
	dMean	= 0;
	dStd	= 0;
}

