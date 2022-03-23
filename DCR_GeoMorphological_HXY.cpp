// DCR_GeoMorphological_HXY.cpp: implementation of the CDCR_GeoMorphological_HXY class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "vr_demcreator.h"
#include "DCR_GeoMorphological_HXY.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDCR_GeoMorphological_HXY::CDCR_GeoMorphological_HXY()
{
	m_pDemObj		= NULL;

	//11*11
	m_iWindowSize	= 5;

	//
	m_pTypeObj		= NULL;
}

CDCR_GeoMorphological_HXY::~CDCR_GeoMorphological_HXY()
{
	Release();
}

//��������
void CDCR_GeoMorphological_HXY::SetGridDem(double *pData,int iNumOfX,int iNumOfY,double dSizeOfX,double dSizeOfY,double dRealSize,double dLeft,double dRight,double dBottom,double dTop,CString szPathName)
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

//�жϵ�ò����
bool CDCR_GeoMorphological_HXY::Judge()
{
	//
	if(m_pDemObj == NULL)	return	false;

	//{
	//	//���Ȱ�����ѷ������������Ը߶�
	//	pOHeight.SetGridDem(m_pDemObj,m_iNumOfX,m_iNumOfY,m_dSizeOfX,m_dSizeOfY,m_dRealSize,
	//						m_dDemLeft,m_dDemRight,m_dDemBottom,m_dDemTop,m_szPathName);
	//	
	//	//������ѷ�������ߴ�
	//	pOHeight.SetWindowSize(m_iWindowSize);
	//	
	//	//������Ը���Ϣ
	//	pOHeight.OnCalculate();
	//	
	//	//��ȡ��Ը���Ϣ
	//	double	*dOHeight;
	//	dOHeight	= pOHeight.GetOHeight();
	//	if(dOHeight == NULL)	return	false;
	//}

	//{
		//���Ȱ�����ѷ������������Ը߶�
		pSlope.SetGridDem(m_pDemObj,m_iNumOfX,m_iNumOfY,m_dSizeOfX,m_dSizeOfY,m_dRealSize,
							m_dDemLeft,m_dDemRight,m_dDemBottom,m_dDemTop,m_szPathName);
		
		//�����¶�
		pSlope.OnCalculate(0);
		
		//��ȡ�¶���Ϣ
		double	*dSlope;
		dSlope		= pSlope.GetSlope();
		if(dSlope == NULL)		return	false;
	//}

	//��������
	if(m_pTypeObj != NULL)	{	delete	[]m_pTypeObj;	m_pTypeObj	= NULL;		}
	//
	m_pTypeObj	= new	BYTE[m_iNumOfX*m_iNumOfY];
	//
	if(m_pTypeObj == NULL)
	{
		CString	szInfo;
		szInfo.Format("���жϵ�ò����ʱ�����ߴ�Ϊ %ld ʱ���ܴ����ڴ�!\n������ͣ�������˳�!\n",m_iWindowSize);
		MessageBox(NULL,szInfo,"��ʾ��Ϣ",MB_OK);

		return	false;
	}

	int		i,j,ci;
	for(j=0;j<m_iNumOfY;j++)
	{
		for(i=0;i<m_iNumOfX;i++)
		{
			ci		= j*m_iNumOfX+i;	
			
			//����߳��ǷǷ�ֵ�����˳�
			if(m_pDemObj[ci] == INVALID_HEIGHT)	
			{
				m_pTypeObj[ci]	= 0;
				continue;
			}
			
			//����ÿһ����˵������Ҫ�����,��ȷ������
			int	iBegin,iEnd;
			int	jBegin,jEnd;

			iBegin	= i - m_iWindowSize;
			iEnd	= i + m_iWindowSize;

			jBegin	= j - m_iWindowSize;
			jEnd	= j + m_iWindowSize;
			
			//���Խ���ˣ��򲻲������
			if(iBegin < 0 || iEnd>m_iNumOfX-1 || jBegin<0 || jEnd>m_iNumOfY-1)
			{
				m_pTypeObj[ci]	= 0;
			}
			else
			{		
				int		I,J;

				///////////////////////////////////////////////////////
				//��ȡƽ���¶ȡ���ȡ��Ե�߲��ȡ�߲�
				//{
					double	dMeanSlope	= 0;
					int		iMeanSlope	= 0;

					for(I=iBegin;I<=iEnd;I++)
					{
						for(J=jBegin;J<=jEnd;J++)
						{
							if(dSlope[J*m_iNumOfX+I] == INVALID_VALUE)	continue;

							dMeanSlope	= dMeanSlope + dSlope[J*m_iNumOfX+I];
							iMeanSlope++;
						}
					}

					if(iMeanSlope == 0)		dMeanSlope	= 0;
					else					dMeanSlope	= dMeanSlope/iMeanSlope;
				//}
				///////////////////////////////////////////////////////
					
				///////////////////////////////////////////////////////
				//��ȡ�߲�
				//{
					double	dDeltaHeight;

					double	dMinHeight	= 0;
					double	dMaxHeight	= 0;

					bool	bFirstValue	= true;
					
					//�ܵ�
					for(I=iBegin;I<=iEnd;I++)
					{
						for(J=jBegin;J<=jEnd;J++)
						{
							if(m_pDemObj[J*m_iNumOfX+I] == INVALID_HEIGHT)	continue;

							if(bFirstValue)
							{
								dMinHeight	= dMaxHeight	= m_pDemObj[J*m_iNumOfX+I];
								bFirstValue	= false;
							}
							else
							{
								if(m_pDemObj[J*m_iNumOfX+I] < dMinHeight)	dMinHeight	= m_pDemObj[J*m_iNumOfX+I];
								if(m_pDemObj[J*m_iNumOfX+I] > dMaxHeight)	dMaxHeight	= m_pDemObj[J*m_iNumOfX+I]; 
							}
						}
					}
					dDeltaHeight	= dMaxHeight-dMinHeight;
				//}

				//s<3	ƽ��
				if(dMeanSlope <= 3)				m_pTypeObj[ci]	= 1;
				else
				{
					//dH<=100	����
					if(dDeltaHeight<=100)		
						m_pTypeObj[ci]	= 2;
					else
					{
						//h<400 && <100dH<200	����
						if((dDeltaHeight>100 && dDeltaHeight<=200) && m_pDemObj[ci]<=400)		m_pTypeObj[ci]	= 3;
						if( dDeltaHeight>200 && (m_pDemObj[ci]>400 && m_pDemObj[ci]<=800))		m_pTypeObj[ci]	= 4;
						if( dDeltaHeight>200 && m_pDemObj[ci]>800)								m_pTypeObj[ci]	= 5;
					}
				}
			}
		}
	}

	//�ͷ���Ը���Ϣ
	//pOHeight.OnRelease();
	//�ͷ��¶���Ϣ
	pSlope.OnRelease();

	return	true;
}

//
void CDCR_GeoMorphological_HXY::SetCoordMapping(double dXA,double dXB,double dYA,double dYB)
{
	m_dMapXA	= dXA;
	m_dMapXB	= dXB;
	m_dMapYA	= dYA;
	m_dMapYB	= dYB;
}

//
void CDCR_GeoMorphological_HXY::SetClientRect(CRect rRect)
{
	m_rRect	= rRect;
}

//
void CDCR_GeoMorphological_HXY::Draw(CDC *pDC)
{
	if(m_pTypeObj == NULL)			return;

	//if(m_bShowStatus == false)		return;

	double	dShowLeft,dShowRight,dShowBottom,dShowTop;
	//����ʵ�ʵ���ʾ����
	dShowLeft	= (m_rRect.left - m_dMapXB) / m_dMapXA;
	dShowRight	= (m_rRect.right - m_dMapXB) / m_dMapXA;
	dShowTop	= (m_rRect.top - m_dMapYB) / m_dMapYA;
	dShowBottom	= (m_rRect.bottom - m_dMapYB) / m_dMapYA;

	int		i,j,ci;
	double	dX,dY;
	BYTE	byHeight;

	//
	for(j=0;j<m_iNumOfY;j++)
	{
		for(i=0;i<m_iNumOfX;i++)
		{	
			ci			= j*m_iNumOfX+i;

			if(m_pTypeObj[ci] == 0)	continue;

			dX			= m_dDemLeft + i*m_dSizeOfX;
			dY			= m_dDemBottom + j*m_dSizeOfY;
			
			if((dX>=dShowLeft-m_dSizeOfX&&dX<=dShowRight+m_dSizeOfX) && (dY>=dShowBottom-m_dSizeOfY&&dY<=dShowTop+m_dSizeOfY))
			{
				byHeight	= m_pTypeObj[ci];
				
				//{
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
					//ƽ��
					if(byHeight == 1)	crColor	= RGB(132,207,66);
					//����
					if(byHeight == 2)	crColor	= RGB(0,0,0);
					//����
					if(byHeight == 3)	crColor	= RGB(231,223,74);
					//��ɽ
					if(byHeight == 4)	crColor	= RGB(255,255,66);
					//��ɽ
					if(byHeight == 5)	crColor	= RGB(231,154,90);
				

					//
					CBrush	*pNewBrush,*pOldBrush;
					pNewBrush	= new	CBrush;
					pNewBrush->CreateSolidBrush(crColor);
					pOldBrush	= pDC->SelectObject(pNewBrush);

					pDC->FillRect(rectCell,pNewBrush);	

					pDC->SelectObject(pOldBrush);
					delete	pNewBrush;
				//}
			}
		}
	}
}

//���ô��ڳߴ�
void CDCR_GeoMorphological_HXY::SetWindowSize(int iSize)
{
	m_iWindowSize	= iSize;
}

int CDCR_GeoMorphological_HXY::GetWindowSize()
{
	return	m_iWindowSize;
}

//
void CDCR_GeoMorphological_HXY::Release()
{
	if(m_pTypeObj != NULL)	{	delete	[]m_pTypeObj;	m_pTypeObj	= NULL;	}
}