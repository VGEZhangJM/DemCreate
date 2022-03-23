// DCR_GridCreator.cpp: implementation of the CDCR_GridCreator class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DCR_GridCreator.h"

#include "DCR_GridVertexList.h"
#include "SetGridCreatorIDWDlg.h"
#include "SetGridCreatorRBFDlg.h"
#include "SetGridCreatorKRGDlg.h"
#include "SetGridCreatorPLYDlg.h"
#include "SetGridCreatorSPDDlg.h"

//#include "DrawTerrain.h"
#include "DCR_HeightSet.h"
#include "DCR_DemSet.h"

#include "matlib.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDCR_GridCreator::CDCR_GridCreator()
{
	//��ͼ����ķ�Χ
	m_dMapLeft		= m_dMapRight	= 0.0;
	m_dMapBottom	= m_dMapTop		= 0.0;	

	//�������DEM�ĸ�����Ŀ
	m_iDemNumOfX	= m_iDemNumOfY	= 100;

	//�������DEM�ĸ����ߴ�
	m_dDemSizeOfX	= m_dDemSizeOfY	= 0.0;

	//
	m_dMaxHeight	= m_dMinHeight	= 0.0;

	//�������DEM�ĸ߳�ֵ
	m_pHeight		= NULL;

	//�̵߳ķ���ֵ
	m_pHeightSD		= NULL;


	//����CDCR_WorkSpace�еķ�����������
	//������������
	m_pCells		= NULL;
	//
	//��ͼ������С
	m_dCellSizeOfX	= 0;
	m_dCellSizeOfY	= 0;
	
	//��ͼ�ֿ���
	m_iCellNumOfX	= 0;
	m_iCellNumOfY	= 0;

	//��������Χ
	m_dCellLeft		= m_dCellRight	= 0;
	m_dCellBottom	= m_dCellTop	= 0;

	//��Ϊtrueʱ��m_dSearchRadius�������ã�
	//��Ϊfalseʱ��m_dSearchRadius�����ã�
	m_bGlobalSearch			= true;
	//�����뾶
	m_dSearchRadius			= 0;

	//�����뾶��������
	m_bSearchRadiusUsed		= false;
	//�����뾶�����õĴ���
	m_iSearchRadiusUsed		= 0;

	//�������򣺷�Ϊ�޷����ķ��򡢰˷���
	//����һ��ʹ�õ����ķ���Ͱ˷���
	m_enumDirection			= FourDirection;
	
	//ÿһ�����������ڵ������������
	m_iPointNumPreDirection	= 1;

	//�������ܵ�������������������޷���ʱ�������ã�
	//���ܹ���Ҫ�������ٸ�����ĵ���
	m_iPointNumInTotal		= 10;

	//��ʾ����
	m_dMapXA				= 1.0;
	m_dMapXB				= 0.0;
	m_dMapYA				= -1.0;	
	m_dMapYB				= 0.0;

	//
	m_bCaclRemainsError		= true;

	//
	m_dwStartTime			= 0;
	m_dwFinishTime			= 0;
}

CDCR_GridCreator::~CDCR_GridCreator()
{
	if(m_pHeight != NULL)
	{
		delete	[]m_pHeight;
		m_pHeight	= NULL;
	}	

	if(m_pHeightSD != NULL)
	{
		delete	[]m_pHeightSD;
		m_pHeightSD	= NULL;
	}
}

//////////////////////////////////////////////////////////////////////
//���õ�ͼ��Χ�������ڽ�ģ��
//////////////////////////////////////////////////////////////////////
void CDCR_GridCreator::SetMapExt(double dLeft,double dRight,double dBottom,double dTop)
{
	//��ͼ����ķ�Χ
	m_dMapLeft		= dLeft;
	m_dMapRight		= dRight;

	m_dMapBottom	= dBottom;
	m_dMapTop		= dTop;	

	if(m_dMapRight-m_dMapLeft > m_dMapTop-m_dMapBottom)
		m_dSearchRadius	= m_dMapRight - m_dMapLeft;
	else
		m_dSearchRadius	= m_dMapTop - m_dMapBottom;
}

void CDCR_GridCreator::GetMapExt(double& dLeft,double& dRight,double& dBottom,double& dTop)
{
	dLeft	= m_dMapLeft;
	dRight	= m_dMapRight;
	dBottom	= m_dMapBottom;
	dTop	= m_dMapTop;
}

//////////////////////////////////////////////////////////////////////
//���ù������DEM�ĸ�����Ŀ
//////////////////////////////////////////////////////////////////////
void CDCR_GridCreator::SetDemCount(int iNumX,int iNumY)
{
	m_iDemNumOfX	= iNumX;
	m_iDemNumOfY	= iNumY;
	
	if(m_iDemNumOfX == 0 || m_iDemNumOfY == 0)	
		return;

	//��������ߴ�
	m_dDemSizeOfX	= (m_dMapRight-m_dMapLeft)/(m_iDemNumOfX-1);
	m_dDemSizeOfY	= (m_dMapTop-m_dMapBottom)/(m_iDemNumOfY-1);	
}

void CDCR_GridCreator::GetDemCount(int& iNumX,int& iNumY)
{
	iNumX	= m_iDemNumOfX;
	iNumY	= m_iDemNumOfY;
}

//////////////////////////////////////////////////////////////////////
//���ù������DEM�ĸ����ߴ�
//////////////////////////////////////////////////////////////////////
void CDCR_GridCreator::SetDemSize(double dSizeX,double dSizeY)
{
	m_dDemSizeOfX	= dSizeX;
	m_dDemSizeOfY	= dSizeY;	
	
	if(m_dDemSizeOfX == 0 || m_dDemSizeOfY == 0)
		return;

	//�����������Ŀ
	m_iDemNumOfX	= int((m_dMapRight-m_dMapLeft)/m_dDemSizeOfX)+1;
	m_iDemNumOfY	= int((m_dMapTop-m_dMapBottom)/m_dDemSizeOfY)+1;
}

void CDCR_GridCreator::GetDemSize(double& dSizeX,double& dSizeY)
{
	dSizeX	= m_dDemSizeOfX;
	dSizeY	= m_dDemSizeOfY;
}

//////////////////////////////////////////////////////////////////////
//ԭʼ���ݵķ�����������
//////////////////////////////////////////////////////////////////////
void CDCR_GridCreator::SetCell(CDCR_Cells *pCells)
{
	m_pCells	= pCells;
}

//////////////////////////////////////////////////////////////////////
//ԭʼ���ݵķ���������Ŀ
//////////////////////////////////////////////////////////////////////
void CDCR_GridCreator::SetCellCount(int iNumX,int iNumY)
{
	m_iCellNumOfX	= iNumX;
	m_iCellNumOfY	= iNumY;
}

//////////////////////////////////////////////////////////////////////
//ԭʼ���ݵķ��������ߴ�
//////////////////////////////////////////////////////////////////////
void CDCR_GridCreator::SetCellSize(double dSizeX,double dSizeY)
{
	m_dCellSizeOfX	= dSizeX;
	m_dCellSizeOfY	= dSizeY;
}

//////////////////////////////////////////////////////////////////////
//ԭʼ���ݵķ���������Χ����͵�ͼ��Χ���в��
//////////////////////////////////////////////////////////////////////
void CDCR_GridCreator::SetCellExt(double dLeft,double dRight,double dBottom,double dTop)
{
	m_dCellLeft		= dLeft;
	m_dCellRight	= dRight;
	m_dCellBottom	= dBottom;
	m_dCellTop		= dTop;
}

//////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////
void CDCR_GridCreator::SetSampleCount(int iSampleCount)
{
	m_iSampleCount	= 	iSampleCount;
}

//////////////////////////////////////////////////////////////////////
// 01 ��ȡ�����ľ���
//////////////////////////////////////////////////////////////////////
double CDCR_GridCreator::GetDistance(double x1, double y1, double z1, double x2, double y2, double z2)
{
	double	dDistance;

	//dDistance	= sqrt( (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) + (z1-z2)*(z1-z2) );
	dDistance	= sqrt( (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) );

	return	dDistance;
}

//////////////////////////////////////////////////////////////////////
// 02 ��ȡ�Ƕ�
//////////////////////////////////////////////////////////////////////
double CDCR_GridCreator::GetAngle(double dX, double dY)
{
	double	dAngle;

	if(fabs(dY) < 1e-10)
	{
		if(dX > 0.0)					dAngle	= 0.0;
		else							dAngle	= PI;
	}
	else	if(dY > 1e-10)
	{
		if(dX > 1e-10)					dAngle	= atan2(fabs(dY),fabs(dX));		
		else	if(fabs(dX) < 1e-10)	dAngle	= PI/2;
		else	if(dX < 1e-10)			dAngle	= PI-atan2(fabs(dY),fabs(dX));
	}
	else
	{
		if(dX > 1e-10)					dAngle	= PI*2-atan2(fabs(dY),fabs(dX));
		else	if(fabs(dX) < 1e-10)	dAngle	= PI*3/2;
		else	if(dX < 1e-10)			dAngle	= PI+atan2(fabs(dY),fabs(dX));
	}
	return	dAngle;
}

//////////////////////////////////////////////////////////////////////
//������־����
//////////////////////////////////////////////////////////////////////
void CDCR_GridCreator::CellReset()
{
	if(m_pCells != NULL)
	{
		for(int i=0;i<m_iCellNumOfX*m_iCellNumOfY;i++)
		{
			m_pCells[i].m_bUsed	= false;
		}
	}
}

//
void CDCR_GridCreator::Release()
{
	//����ɾ��
	m_pGridVertexList.Release();
	
	if(m_pHeight != NULL)
	{
		 delete	[]m_pHeight;

		 m_pHeight	= NULL;
	}

	if(m_pHeightSD != NULL)
	{
		delete	[]m_pHeightSD;
		m_pHeightSD	= NULL;
	}

	//
	InitialHeight();
}

//
void CDCR_GridCreator::SetCoordMapping(double dXA,double dXB,double dYA,double dYB)
{
	m_dMapXA	= dXA;
	m_dMapXB	= dXB;
	m_dMapYA	= dYA;
	m_dMapYB	= dYB;
}

void CDCR_GridCreator::DrawVertex(CDC *pDC)
{
	CDCR_GridVertex	*pVertex;

	pVertex		= m_pGridVertexList.GetGridVertex();

	while(pVertex != NULL)
	{
		int		iX,iY;
		
		iX	= (int)(pVertex->m_dX*m_dMapXA + m_dMapXB + 0.5);
		iY	= (int)(pVertex->m_dY*m_dMapYA + m_dMapYB + 0.5);
		
		pDC->Ellipse(iX-3,iY-3,iX+3,iY+3);

		pVertex	= pVertex->m_pNext;
	}
}

void CDCR_GridCreator::DrawCell(CDC *pDC, double dX, double dY,double dZ)
{
	COLORREF	crColor;
	crColor		= CDCR_DemSet::Instance()->GetColor(dZ);

	//����
	double	dLeft,dRight,dBottom,dTop;
	dLeft	= dX - m_dDemSizeOfX/2;
	dRight	= dLeft + m_dDemSizeOfX;
	dBottom	= dY - m_dDemSizeOfY/2;
	dTop	= dBottom + m_dDemSizeOfY;
	
	CRect	rectCell;
	rectCell.left	= (int)(dLeft*m_dMapXA + m_dMapXB + 0.5);
	rectCell.bottom	= (int)(dBottom*m_dMapYA + m_dMapYB + 0.5);
	rectCell.right	= (int)(dRight*m_dMapXA + m_dMapXB + 0.5);
	rectCell.top	= (int)(dTop*m_dMapYA + m_dMapYB + 0.5);
	
	CBrush	*pNewBrush,*pOldBrush;
	pNewBrush	= new	CBrush;
	pNewBrush->CreateSolidBrush(crColor);
	pOldBrush	= pDC->SelectObject(pNewBrush);

	pDC->FillRect(rectCell,pNewBrush);	

	pDC->SelectObject(pOldBrush);
	delete	pNewBrush;
}

//
void CDCR_GridCreator::Draw(CDC *pDC)
{
	//DrawVertex(pDC);
}

//////////////////////////////////////////////////////////////////////
//���������ֵ�Ĳ���������
//(dX,dY):Ϊ��ֵ��
//����ֵΪbool
//////////////////////////////////////////////////////////////////////
bool CDCR_GridCreator::SearchSamplingPoint(double dX,double dY)
{
	if(m_pCells == NULL)	return	false;

	//���ȣ�����(dX,dY)ȷ����ֵ�����ڵĸ���
	int		iCurrentI,iCurrentJ;
	iCurrentI	= (int)((dX - m_dCellLeft) / m_dCellSizeOfX);
	iCurrentJ	= (int)((dY - m_dCellBottom) / m_dCellSizeOfY);

	//����ɾ��
	m_pGridVertexList.Release();

	//
	m_bSearchRadiusUsed	= false;

	CellReset();

	//������ȫ������
	//Ψһ�����õ����ܵ�����m_iPointNumPreDirection
	if(m_bGlobalSearch)
	{
		//��ʱ����
		CDCR_GridVertexList	pTempGridVertexList;

		//�ж���ֹ�������ǵ����ﵽҪ��
		//������������һȦ
		int		iK			= 1;
		bool	bFinished	= true;

		do
		{
			//
			int		iBegin,iEnd;
			int		jBegin,jEnd;
			
			iBegin	= iCurrentI - iK;		iEnd	= iCurrentI + iK;
			jBegin	= iCurrentJ - iK;		jEnd	= iCurrentJ + iK;
			
			if(iBegin < 0)					iBegin	= 0;
			if(iBegin >= m_iCellNumOfX-1)	iBegin	= m_iCellNumOfX-1;
			if(iEnd < 0)					iEnd	= 0;
			if(iEnd >= m_iCellNumOfX-1)		iEnd	= m_iCellNumOfX-1;

			if(jBegin < 0)					jBegin	= 0;
			if(jBegin >= m_iCellNumOfY-1)	jBegin	= m_iCellNumOfY-1;
			if(jEnd < 0)					jEnd	= 0;
			if(jEnd >= m_iCellNumOfY-1)		jEnd	= m_iCellNumOfY-1;

			for(int j=jBegin;j<=jEnd;j++)
			{
				for(int i=iBegin;i<=iEnd;i++)
				{
					//
					int		iI,iJ,iC;

					iI	= i;	iJ	= j;					
					iC	= iJ * m_iCellNumOfX + iI;

					//�жϸø����Ƿ�ʹ��
					//û��ʹ�õĲ��ܽ���
					//���ų�һЩ
					if(false == m_pCells[iC].m_bUsed)
					{
						//�ù���
						m_pCells[iC].m_bUsed	= true;
							
						//��ȡ����Ϣ
						CDCR_PointFeatureAddress	*pPFD;
						pPFD		= m_pCells[iC].m_pPFDHead;
							
						while(pPFD != NULL)
						{
							CDCR_PointFeature	*pPoint;
							pPoint		= pPFD->m_pPointFeature;

							//û�б�ɾ���ĵ�
							if(false == pPoint->m_bUsed)
							{
								//
								double				dDistance;	
								dDistance	= GetDistance(pPoint->m_pPointCoord.x,pPoint->m_pPointCoord.y,0,dX,dY,0);
								
								//
								pTempGridVertexList.AddGridVertex(pPoint->m_pPointCoord.x,pPoint->m_pPointCoord.y,pPoint->m_pPointCoord.z,dDistance,m_iPointNumInTotal);
							}

							pPFD	= pPFD->m_pNext;
						}
					}
				
				}//for(int i=iBegin;i<=iEnd;i++)

			}//for(int j=jBegin;j<=jEnd;j++)
			
			bFinished	= false;

			//������ڹ涨�ĵ��������˳�
			if(pTempGridVertexList.GetGridVertexCount() >= m_iPointNumInTotal)
				bFinished	= true;
			
			//ͨ��iK�������
			//double	dMaxDistance;
			//if(m_dCellSizeOfX > m_dCellSizeOfY)
			//	dMaxDistance	= iK * m_dCellSizeOfY;
			//else	
			//	dMaxDistance	= iK * m_dCellSizeOfX;
			//��������뾶����ָ�������뾶�����˳�
			//if(dMaxDistance > m_dSearchRadius)
			//	bFinished	= true;

			iK++;
			
			if(m_iCellNumOfX > m_iCellNumOfY)
			{
				if(iK > m_iCellNumOfX)	
					bFinished	= true;
			}
			else
			{	
				if(iK > m_iCellNumOfY)	
					bFinished	= true;
			}
		}while(!bFinished);	

		//����
		//�Ѿ����ݹ̶���������Ŀ�Ѿ��������㹻�Ĳ�������

		//����
		//��pTempGridVertexList�еĲ�������ݡ������뾶�����޶����õ�m_pGridVertexList�У�
		//���м����һ��������ж�����
		CDCR_GridVertex	*pVertex;
		pVertex	= pTempGridVertexList.GetGridVertex();
		while(pVertex != NULL)
		{
			//С��ָ������������
			if(pVertex->m_dDistance < m_dSearchRadius)
			{
				m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);
			}
			else
			{
				//��������뾶��������
				//���ʹ�������뾶�޶��ı�־����
				m_bSearchRadiusUsed		= true;
			}

			pVertex	= pVertex->m_pNext;
		}

		//���
		//���磬�����õ�û�в������ˣ���ô�죿
		//���ڵĽ�����������°�ԭ�������ݲ������������ܴ�������
		if(m_pGridVertexList.GetGridVertexCount() == 0)
		{
			pVertex	= pTempGridVertexList.GetGridVertex();
			while(pVertex != NULL)
			{
				m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);

				pVertex	= pVertex->m_pNext;
			}
		}
		else
		{
			if(m_bSearchRadiusUsed)		m_iSearchRadiusUsed++;
		}
	}
	else
	{
		//�ķ�������
		if(m_enumDirection == FourDirection)
		{
			//��¼������Ϣ
			//0-90
			CDCR_GridVertexList	pGridVertexList1;
			//90-180
			CDCR_GridVertexList	pGridVertexList2;
			//180-270
			CDCR_GridVertexList	pGridVertexList3;
			//270-360
			CDCR_GridVertexList	pGridVertexList4;
			
			//�ж���ֹ�������ǵ����ﵽҪ��
			//������������һȦ
			int		iK			= 1;
			bool	bFinished	= true;

			do
			{
				//
				int		iBegin,iEnd;
				int		jBegin,jEnd;
				
				iBegin	= iCurrentI - iK;		iEnd	= iCurrentI + iK;
				jBegin	= iCurrentJ - iK;		jEnd	= iCurrentJ + iK;

				if(iBegin < 0)					iBegin	= 0;
				if(iBegin >= m_iCellNumOfX-1)	iBegin	= m_iCellNumOfX-1;
				if(iEnd < 0)					iEnd	= 0;
				if(iEnd >= m_iCellNumOfX-1)		iEnd	= m_iCellNumOfX-1;

				if(jBegin < 0)					jBegin	= 0;
				if(jBegin >= m_iCellNumOfY-1)	jBegin	= m_iCellNumOfY-1;
				if(jEnd < 0)					jEnd	= 0;
				if(jEnd >= m_iCellNumOfY-1)		jEnd	= m_iCellNumOfY-1;

				for(int j=jBegin;j<=jEnd;j++)
				{
					for(int i=iBegin;i<=iEnd;i++)
					{
						//
						int		iI,iJ,iC;

						iI	= i;	iJ	= j;
						iC	= iJ * m_iCellNumOfX + iI;

						//�жϸø����Ƿ�ʹ��
						//û��ʹ�õĲ��ܽ���
						if(false == m_pCells[iC].m_bUsed)
						{
							//�ù���
							m_pCells[iC].m_bUsed	= true;
								
							//��ȡ����Ϣ
							CDCR_PointFeatureAddress	*pPFD;
							pPFD		= m_pCells[iC].m_pPFDHead;
								
							while(pPFD != NULL)
							{
								CDCR_PointFeature	*pPoint;
								pPoint		= pPFD->m_pPointFeature;

								//û�б�ɾ���ĵ�
								if(false == pPoint->m_bUsed)
								{
									//�����жϽǶ�
									double				dAngle;
									dAngle		= GetAngle(pPoint->m_pPointCoord.x-dX,pPoint->m_pPointCoord.y-dY);

									double				dDistance;	
									dDistance	= GetDistance(pPoint->m_pPointCoord.x,pPoint->m_pPointCoord.y,0,dX,dY,0);
									
									//if(dAngle >=PI/4 && dAngle < 3*PI/4)
									if(dAngle >=0 && dAngle < PI/2)
										pGridVertexList1.AddGridVertex(pPoint->m_pPointCoord.x,pPoint->m_pPointCoord.y,pPoint->m_pPointCoord.z,dDistance,m_iPointNumPreDirection);
									//else	if(dAngle>=3*PI/4 && dAngle < 5*PI/4)
									else	if(dAngle>=PI/2 && dAngle < PI)
										pGridVertexList2.AddGridVertex(pPoint->m_pPointCoord.x,pPoint->m_pPointCoord.y,pPoint->m_pPointCoord.z,dDistance,m_iPointNumPreDirection);
									//else	if(dAngle>=5*PI/4 && dAngle < 7*PI/4)
									else	if(dAngle>=PI && dAngle < 3*PI/2)
										pGridVertexList3.AddGridVertex(pPoint->m_pPointCoord.x,pPoint->m_pPointCoord.y,pPoint->m_pPointCoord.z,dDistance,m_iPointNumPreDirection);
									else
										pGridVertexList4.AddGridVertex(pPoint->m_pPointCoord.x,pPoint->m_pPointCoord.y,pPoint->m_pPointCoord.z,dDistance,m_iPointNumPreDirection);
								}

								pPFD	= pPFD->m_pNext;
							}
						}
					
					}//for(int i=iBegin;i<=iEnd;i++)

				}//for(int j=jBegin;j<=jEnd;j++)
				
				bFinished	= true;
				if(pGridVertexList1.GetGridVertexCount() < m_iPointNumPreDirection)
				{
					bFinished	= false;
				}

				if(bFinished)
				{
					if(pGridVertexList2.GetGridVertexCount() < m_iPointNumPreDirection)
					{
						bFinished	= false;
					}
				}

				if(bFinished)
				{
					if(pGridVertexList3.GetGridVertexCount() < m_iPointNumPreDirection)
					{
						bFinished	= false;
					}
				}

				if(bFinished)
				{
					if(pGridVertexList4.GetGridVertexCount() < m_iPointNumPreDirection)
					{
						bFinished	= false;
					}
				}

				//ͨ��iK�������
				//double	dMaxDistance;
				//if(m_dCellSizeOfX > m_dCellSizeOfY)
				//	dMaxDistance	= iK * m_dCellSizeOfY;
				//else	
				//	dMaxDistance	= iK * m_dCellSizeOfX;
				//����һ������:�뾶
				//if(dMaxDistance > m_dSearchRadius)
				//	bFinished	= true;

				iK++;

				if(m_iCellNumOfX > m_iCellNumOfY)
				{
					if(iK > m_iCellNumOfX)	
						bFinished	= true;
				}
				else
				{	
					if(iK > m_iCellNumOfY)	
						bFinished	= true;
				}

			}while(!bFinished);	
			
			//��pTempGridVertexList�е����ݸ��������뾶���޶����õ�m_pGridVertexList�У�
			//���м����һ��������ж�����
			CDCR_GridVertex	*pVertex;

			//��һ������
			pVertex	= pGridVertexList1.GetGridVertex();
			while(pVertex != NULL)
			{
				if(pVertex->m_dDistance < m_dSearchRadius)
				{
					m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);
				}
				else
				{
					//�����뾶��������
					m_bSearchRadiusUsed		= true;
				}

				pVertex	= pVertex->m_pNext;
			}

			//�ڶ�������
			pVertex	= pGridVertexList2.GetGridVertex();
			while(pVertex != NULL)
			{
				if(pVertex->m_dDistance < m_dSearchRadius)
				{
					m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);
				}
				else
				{
					//�����뾶��������
					m_bSearchRadiusUsed		= true;
				}

				pVertex	= pVertex->m_pNext;
			}
			
			//����������
			pVertex	= pGridVertexList3.GetGridVertex();
			while(pVertex != NULL)
			{
				if(pVertex->m_dDistance < m_dSearchRadius)
				{
					m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);
				}
				else
				{
					//�����뾶��������
					m_bSearchRadiusUsed		= true;
				}

				pVertex	= pVertex->m_pNext;
			}

			//
			pVertex	= pGridVertexList4.GetGridVertex();
			while(pVertex != NULL)
			{
				if(pVertex->m_dDistance < m_dSearchRadius)
				{
					m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);
				}
				else
				{
					//�����뾶��������
					m_bSearchRadiusUsed		= true;
				}

				pVertex	= pVertex->m_pNext;
			}

			//���磬���еĲ����㶼���뾶�ˣ���ô�죿
			//������ͬ��
			if(m_pGridVertexList.GetGridVertexCount() == 0)
			{
				//��һ������
				pVertex	= pGridVertexList1.GetGridVertex();
				while(pVertex != NULL)
				{
					m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);

					pVertex	= pVertex->m_pNext;
				}

				//�ڶ�������
				pVertex	= pGridVertexList2.GetGridVertex();
				while(pVertex != NULL)
				{
					m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);

					pVertex	= pVertex->m_pNext;
				}
				
				//����������
				pVertex	= pGridVertexList3.GetGridVertex();
				while(pVertex != NULL)
				{
					m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);

					pVertex	= pVertex->m_pNext;
				}

				//
				pVertex	= pGridVertexList4.GetGridVertex();
				while(pVertex != NULL)
				{
					m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);

					pVertex	= pVertex->m_pNext;
				}
			}
			else
			{
				if(m_bSearchRadiusUsed)		m_iSearchRadiusUsed++;
			}
		}

		//�˷�������
		if(m_enumDirection == EightDirection)
		{
			//��¼������Ϣ
			//0-45
			CDCR_GridVertexList	pGridVertexList1;
			//45-90
			CDCR_GridVertexList	pGridVertexList2;
			//90-135
			CDCR_GridVertexList	pGridVertexList3;
			//135-180
			CDCR_GridVertexList	pGridVertexList4;
			//180-225
			CDCR_GridVertexList	pGridVertexList5;
			//225-270
			CDCR_GridVertexList	pGridVertexList6;
			//270-315
			CDCR_GridVertexList	pGridVertexList7;
			//315-360
			CDCR_GridVertexList	pGridVertexList8;
			
			//�ж���ֹ�������ǵ����ﵽҪ��
			//������������һȦ
			int		iK			= 1;
			
			bool	bFinished	= true;

			do
			{
				//
				int		iBegin,iEnd;
				int		jBegin,jEnd;
				
				iBegin	= iCurrentI - iK;		iEnd	= iCurrentI + iK;
				jBegin	= iCurrentJ - iK;		jEnd	= iCurrentJ + iK;

				if(iBegin < 0)					iBegin	= 0;
				if(iBegin >= m_iCellNumOfX-1)	iBegin	= m_iCellNumOfX-1;
				if(iEnd < 0)					iEnd	= 0;
				if(iEnd >= m_iCellNumOfX-1)		iEnd	= m_iCellNumOfX-1;

				if(jBegin < 0)					jBegin	= 0;
				if(jBegin >= m_iCellNumOfY-1)	jBegin	= m_iCellNumOfY-1;
				if(jEnd < 0)					jEnd	= 0;
				if(jEnd >= m_iCellNumOfY-1)		jEnd	= m_iCellNumOfY-1;

				for(int j=jBegin;j<=jEnd;j++)
				{
					for(int i=iBegin;i<=iEnd;i++)
					{
						//
						int		iI,iJ,iC;

						iI	= i;	iJ	= j;
						iC	= iJ * m_iCellNumOfX + iI;

						//�жϸø����Ƿ�ʹ��
						//û��ʹ�õĲ��ܽ���
						if(false == m_pCells[iC].m_bUsed)
						{
							//�ù���
							m_pCells[iC].m_bUsed	= true;

							//��ȡ����Ϣ
							CDCR_PointFeatureAddress	*pPFD;
							pPFD		= m_pCells[iC].m_pPFDHead;
								
							while(pPFD != NULL)
							{
								CDCR_PointFeature	*pPoint;
								pPoint		= pPFD->m_pPointFeature;

								//û�б�ɾ���ĵ�
								if(false == pPoint->m_bUsed)
								{
									//�����жϽǶ�
									double				dAngle;
									dAngle		= GetAngle(pPoint->m_pPointCoord.x-dX,pPoint->m_pPointCoord.y-dY);

									double				dDistance;	
									dDistance	= GetDistance(pPoint->m_pPointCoord.x,pPoint->m_pPointCoord.y,0,dX,dY,0);
									
									if(dAngle >=0 && dAngle < PI/4)
										pGridVertexList1.AddGridVertex(pPoint->m_pPointCoord.x,pPoint->m_pPointCoord.y,pPoint->m_pPointCoord.z,dDistance,m_iPointNumPreDirection);
									if(dAngle>=PI/4 && dAngle < PI/2)
										pGridVertexList2.AddGridVertex(pPoint->m_pPointCoord.x,pPoint->m_pPointCoord.y,pPoint->m_pPointCoord.z,dDistance,m_iPointNumPreDirection);
									if(dAngle>=PI/2 && dAngle < 3*PI/4)
										pGridVertexList3.AddGridVertex(pPoint->m_pPointCoord.x,pPoint->m_pPointCoord.y,pPoint->m_pPointCoord.z,dDistance,m_iPointNumPreDirection);
									if(dAngle>=3*PI/4 && dAngle < PI)
										pGridVertexList4.AddGridVertex(pPoint->m_pPointCoord.x,pPoint->m_pPointCoord.y,pPoint->m_pPointCoord.z,dDistance,m_iPointNumPreDirection);

									if(dAngle >=PI && dAngle < 5*PI/4)
										pGridVertexList5.AddGridVertex(pPoint->m_pPointCoord.x,pPoint->m_pPointCoord.y,pPoint->m_pPointCoord.z,dDistance,m_iPointNumPreDirection);
									if(dAngle>=5*PI/4 && dAngle < 3*PI/2)
										pGridVertexList6.AddGridVertex(pPoint->m_pPointCoord.x,pPoint->m_pPointCoord.y,pPoint->m_pPointCoord.z,dDistance,m_iPointNumPreDirection);
									if(dAngle>=3*PI/2 && dAngle < 7*PI/4)
										pGridVertexList7.AddGridVertex(pPoint->m_pPointCoord.x,pPoint->m_pPointCoord.y,pPoint->m_pPointCoord.z,dDistance,m_iPointNumPreDirection);
									if(dAngle>=7*PI/4 && dAngle < 2*PI)
										pGridVertexList8.AddGridVertex(pPoint->m_pPointCoord.x,pPoint->m_pPointCoord.y,pPoint->m_pPointCoord.z,dDistance,m_iPointNumPreDirection);
								}

								pPFD	= pPFD->m_pNext;
							}
						}
					
					}//for(int i=iBegin;i<=iEnd;i++)

				}//for(int j=jBegin;j<=jEnd;j++)
				
				bFinished	= true;
				if(pGridVertexList1.GetGridVertexCount() < m_iPointNumPreDirection)
				{
					bFinished	= false;
				}

				if(bFinished)
				{
					if(pGridVertexList2.GetGridVertexCount() < m_iPointNumPreDirection)
					{
						bFinished	= false;
					}
				}

				if(bFinished)
				{
					if(pGridVertexList3.GetGridVertexCount() < m_iPointNumPreDirection)
					{
						bFinished	= false;
					}
				}

				if(bFinished)
				{
					if(pGridVertexList4.GetGridVertexCount() < m_iPointNumPreDirection)
					{
						bFinished	= false;
					}
				}

				if(bFinished)
				{
					if(pGridVertexList5.GetGridVertexCount() < m_iPointNumPreDirection)
					{
						bFinished	= false;
					}
				}

				if(bFinished)
				{
					if(pGridVertexList6.GetGridVertexCount() < m_iPointNumPreDirection)
					{
						bFinished	= false;
					}
				}

				if(bFinished)
				{
					if(pGridVertexList7.GetGridVertexCount() < m_iPointNumPreDirection)
					{
						bFinished	= false;
					}
				}

				if(bFinished)
				{
					if(pGridVertexList8.GetGridVertexCount() < m_iPointNumPreDirection)
					{
						bFinished	= false;
					}
				}

				//ͨ��iK�������
				//double	dMaxDistance;
				//if(m_dCellSizeOfX > m_dCellSizeOfY)
				//	dMaxDistance	= iK * m_dCellSizeOfY;
				//else	
				//	dMaxDistance	= iK * m_dCellSizeOfX;
				//����һ������:�뾶
				//if(dMaxDistance > m_dSearchRadius)
				//	bFinished	= true;

				iK++;

				if(m_iCellNumOfX > m_iCellNumOfY)
				{
					if(iK > m_iCellNumOfX)	
						bFinished	= true;
				}
				else
				{	
					if(iK > m_iCellNumOfY)	
						bFinished	= true;
				}
			}while(!bFinished);	

			//��pTempGridVertexList�е����ݸ��������뾶���޶����õ�m_pGridVertexList�У�
			//���м����һ��������ж�����
			CDCR_GridVertex	*pVertex;

			//��һ������
			pVertex	= pGridVertexList1.GetGridVertex();
			while(pVertex != NULL)
			{
				if(pVertex->m_dDistance < m_dSearchRadius)
				{
					m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);
				}
				else
				{
					//�����뾶��������
					m_bSearchRadiusUsed		= true;
				}

				pVertex	= pVertex->m_pNext;
			}

			//�ڶ�������
			pVertex	= pGridVertexList2.GetGridVertex();
			while(pVertex != NULL)
			{
				if(pVertex->m_dDistance < m_dSearchRadius)
				{
					m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);
				}
				else
				{
					//�����뾶��������
					m_bSearchRadiusUsed		= true;
				}

				pVertex	= pVertex->m_pNext;
			}

			//����������
			pVertex	= pGridVertexList3.GetGridVertex();
			while(pVertex != NULL)
			{
				if(pVertex->m_dDistance < m_dSearchRadius)
				{
					m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);
				}
				else
				{
					//�����뾶��������
					m_bSearchRadiusUsed		= true;
				}

				pVertex	= pVertex->m_pNext;
			}

			//���ĸ�����
			pVertex	= pGridVertexList4.GetGridVertex();
			while(pVertex != NULL)
			{
				if(pVertex->m_dDistance < m_dSearchRadius)
				{
					m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);
				}
				else
				{
					//�����뾶��������
					m_bSearchRadiusUsed		= true;
				}

				pVertex	= pVertex->m_pNext;
			}

			//���������
			pVertex	= pGridVertexList5.GetGridVertex();
			while(pVertex != NULL)
			{
				if(pVertex->m_dDistance < m_dSearchRadius)
				{
					m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);
				}
				else
				{
					//�����뾶��������
					m_bSearchRadiusUsed		= true;
				}

				pVertex	= pVertex->m_pNext;
			}

			//����������
			pVertex	= pGridVertexList6.GetGridVertex();
			while(pVertex != NULL)
			{
				if(pVertex->m_dDistance < m_dSearchRadius)
				{
					m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);
				}
				else
				{
					//�����뾶��������
					m_bSearchRadiusUsed		= true;
				}

				pVertex	= pVertex->m_pNext;
			}

			//���߸�����
			pVertex	= pGridVertexList7.GetGridVertex();
			while(pVertex != NULL)
			{
				if(pVertex->m_dDistance < m_dSearchRadius)
				{
					m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);
				}
				else
				{
					//�����뾶��������
					m_bSearchRadiusUsed		= true;
				}

				pVertex	= pVertex->m_pNext;
			}

			//�ڰ˸�����
			pVertex	= pGridVertexList8.GetGridVertex();
			while(pVertex != NULL)
			{
				if(pVertex->m_dDistance < m_dSearchRadius)
				{
					m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);
				}
				else
				{
					//�����뾶��������
					m_bSearchRadiusUsed		= true;
				}

				pVertex	= pVertex->m_pNext;
			}

			//���磬���еĲ����㶼���뾶�ˣ���ô�죿
			if(m_pGridVertexList.GetGridVertexCount() == 0)
			{
				//��һ������
				pVertex	= pGridVertexList1.GetGridVertex();
				while(pVertex != NULL)
				{
					m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);

					pVertex	= pVertex->m_pNext;
				}

				//�ڶ�������
				pVertex	= pGridVertexList2.GetGridVertex();
				while(pVertex != NULL)
				{
					m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);

					pVertex	= pVertex->m_pNext;
				}
				
				//����������
				pVertex	= pGridVertexList3.GetGridVertex();
				while(pVertex != NULL)
				{
					m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);

					pVertex	= pVertex->m_pNext;
				}

				//
				pVertex	= pGridVertexList4.GetGridVertex();
				while(pVertex != NULL)
				{
					m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);

					pVertex	= pVertex->m_pNext;
				}
			}
			else
			{
				if(m_bSearchRadiusUsed)		m_iSearchRadiusUsed++;
			}
		}
	}

	//�ҵ��ˣ���ʵ����ͳһ����
	{
		//��������Ʊߣ�����Ҫ���Ʊ�
	}

	if(m_pGridVertexList.GetGridVertexCount() == 0)	return	false;

	return	true;
}

//////////////////////////////////////////////////////////////////////
//ʹ�ý�����֤���㾫��ֵ
//CDCR_PointFeature *pDelPoint��	Ϊɾ������Ҫ���²�ֵ�ĵ�
//////////////////////////////////////////////////////////////////////
bool CDCR_GridCreator::SearchSamplingPointCV(CDCR_PointFeature *pDelPoint)
{
	if(pDelPoint == NULL)	return	false;
	
	if(m_pCells == NULL)	return	false;

	double	dX,dY;
	dX	= pDelPoint->m_pPointCoord.x;
	dY	= pDelPoint->m_pPointCoord.y;

	//���ȣ�����(dX,dY)ȷ����ֵ�����ڵĸ���
	int		iCurrentI,iCurrentJ;
	iCurrentI	= (int)((dX - m_dCellLeft) / m_dCellSizeOfX);
	iCurrentJ	= (int)((dY - m_dCellBottom) / m_dCellSizeOfY);
	
	//����ɾ��
	m_pGridVertexList.Release();

	m_bSearchRadiusUsed	= false;

	CellReset();

	//������ȫ������
	//Ψһ�����õ����ܵ�����m_iPointNumPreDirection
	if(m_bGlobalSearch)
	{
		//��ʱ����
		CDCR_GridVertexList	pTempGridVertexList;

		//�ж���ֹ�������ǵ����ﵽҪ��
		//������������һȦ
		int		iK			= 1;
		bool	bFinished	= true;

		do
		{
			//
			int		iBegin,iEnd;
			int		jBegin,jEnd;
			
			iBegin	= iCurrentI - iK;		iEnd	= iCurrentI + iK;
			jBegin	= iCurrentJ - iK;		jEnd	= iCurrentJ + iK;
			
			if(iBegin < 0)					iBegin	= 0;
			if(iBegin >= m_iCellNumOfX-1)	iBegin	= m_iCellNumOfX-1;
			if(iEnd < 0)					iEnd	= 0;
			if(iEnd >= m_iCellNumOfX-1)		iEnd	= m_iCellNumOfX-1;

			if(jBegin < 0)					jBegin	= 0;
			if(jBegin >= m_iCellNumOfY-1)	jBegin	= m_iCellNumOfY-1;
			if(jEnd < 0)					jEnd	= 0;
			if(jEnd >= m_iCellNumOfY-1)		jEnd	= m_iCellNumOfY-1;

			for(int j=jBegin;j<=jEnd;j++)
			{
				for(int i=iBegin;i<=iEnd;i++)
				{
					//
					int		iI,iJ,iC;

					iI	= i;	iJ	= j;					
					iC	= iJ * m_iCellNumOfX + iI;

					//�жϸø����Ƿ�ʹ��
					//û��ʹ�õĲ��ܽ���
					//���ų�һЩ
					if(false == m_pCells[iC].m_bUsed)
					{
						//�ù���
						m_pCells[iC].m_bUsed	= true;
							
						//��ȡ����Ϣ
						CDCR_PointFeatureAddress	*pPFD;
						pPFD		= m_pCells[iC].m_pPFDHead;
							
						while(pPFD != NULL)
						{
							CDCR_PointFeature	*pPoint;
							pPoint		= pPFD->m_pPointFeature;

							//�ڼ��ϲ��ܺ�ɾ���ĵ��ظ���һ����
							if(pPoint != pDelPoint)
							{
								//��ɾ���ĵ㲻����
								if(false == pPoint->m_bUsed)
								{
									//
									double				dDistance;	
									dDistance	= GetDistance(pPoint->m_pPointCoord.x,pPoint->m_pPointCoord.y,0,dX,dY,0);
									
									//
									pTempGridVertexList.AddGridVertex(pPoint->m_pPointCoord.x,pPoint->m_pPointCoord.y,pPoint->m_pPointCoord.z,dDistance,m_iPointNumInTotal);
								}
							}

							pPFD	= pPFD->m_pNext;
						}
					}
				
				}//for(int i=iBegin;i<=iEnd;i++)

			}//for(int j=jBegin;j<=jEnd;j++)
			
			bFinished	= false;

			//������ڹ涨�ĵ��������˳�
			if(pTempGridVertexList.GetGridVertexCount() >= m_iPointNumInTotal)
				bFinished	= true;
			
			//ͨ��iK�������
			//double	dMaxDistance;
			//if(m_dCellSizeOfX > m_dCellSizeOfY)
			//	dMaxDistance	= iK * m_dCellSizeOfY;
			//else	
			//	dMaxDistance	= iK * m_dCellSizeOfX;
			//��������뾶����ָ�������뾶�����˳�
			//if(dMaxDistance > m_dSearchRadius)
			//	bFinished	= true;

			iK++;
			
			if(m_iCellNumOfX > m_iCellNumOfY)
			{
				if(iK > m_iCellNumOfX)	
					bFinished	= true;
			}
			else
			{	
				if(iK > m_iCellNumOfY)	
					bFinished	= true;
			}
		}while(!bFinished);	

		//��pTempGridVertexList�е����ݸ��������뾶���޶����õ�m_pGridVertexList�У�
		//���м����һ��������ж�����
		CDCR_GridVertex	*pVertex;
		pVertex	= pTempGridVertexList.GetGridVertex();
		while(pVertex != NULL)
		{
			if(pVertex->m_dDistance < m_dSearchRadius)
			{
				m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);
			}
			else
			{
				//�����뾶��������
				m_bSearchRadiusUsed		= true;
			}

			pVertex	= pVertex->m_pNext;
		}

		//���磬�����õ�û�в������ˣ���ô�죿
		//����ͬ��
		if(m_pGridVertexList.GetGridVertexCount() == 0)
		{
			pVertex	= pTempGridVertexList.GetGridVertex();
			while(pVertex != NULL)
			{
				m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);

				pVertex	= pVertex->m_pNext;
			}
		}
		else
		{
			if(m_bSearchRadiusUsed)		m_iSearchRadiusUsed++;
		}
	}
	else
	{
		//�ķ�������
		if(m_enumDirection == FourDirection)
		{
			//��¼������Ϣ
			//0-90
			CDCR_GridVertexList	pGridVertexList1;
			//90-180
			CDCR_GridVertexList	pGridVertexList2;
			//180-270
			CDCR_GridVertexList	pGridVertexList3;
			//270-360
			CDCR_GridVertexList	pGridVertexList4;
			
			//�ж���ֹ�������ǵ����ﵽҪ��
			//������������һȦ
			int		iK			= 1;
			bool	bFinished	= true;

			do
			{
				//
				int		iBegin,iEnd;
				int		jBegin,jEnd;
				
				iBegin	= iCurrentI - iK;		iEnd	= iCurrentI + iK;
				jBegin	= iCurrentJ - iK;		jEnd	= iCurrentJ + iK;

				if(iBegin < 0)					iBegin	= 0;
				if(iBegin >= m_iCellNumOfX-1)	iBegin	= m_iCellNumOfX-1;
				if(iEnd < 0)					iEnd	= 0;
				if(iEnd >= m_iCellNumOfX-1)		iEnd	= m_iCellNumOfX-1;

				if(jBegin < 0)					jBegin	= 0;
				if(jBegin >= m_iCellNumOfY-1)	jBegin	= m_iCellNumOfY-1;
				if(jEnd < 0)					jEnd	= 0;
				if(jEnd >= m_iCellNumOfY-1)		jEnd	= m_iCellNumOfY-1;

				for(int j=jBegin;j<=jEnd;j++)
				{
					for(int i=iBegin;i<=iEnd;i++)
					{
						//
						int		iI,iJ,iC;

						iI	= i;	iJ	= j;
						iC	= iJ * m_iCellNumOfX + iI;

						//�жϸø����Ƿ�ʹ��
						//û��ʹ�õĲ��ܽ���
						if(false == m_pCells[iC].m_bUsed)
						{
							//�ù���
							m_pCells[iC].m_bUsed	= true;
								
							//��ȡ����Ϣ
							CDCR_PointFeatureAddress	*pPFD;
							pPFD		= m_pCells[iC].m_pPFDHead;
								
							while(pPFD != NULL)
							{
								CDCR_PointFeature	*pPoint;
								pPoint		= pPFD->m_pPointFeature;

								//������֤���Ѿ�ɾ���ĵ㲻����
								if(pPoint != pDelPoint)
								{
									//û�б�ɾ���ĵ�
									if(false == pPoint->m_bUsed)
									{
										//�����жϽǶ�
										double				dAngle;
										dAngle		= GetAngle(pPoint->m_pPointCoord.x-dX,pPoint->m_pPointCoord.y-dY);

										double				dDistance;	
										dDistance	= GetDistance(pPoint->m_pPointCoord.x,pPoint->m_pPointCoord.y,0,dX,dY,0);
										
										//if(dAngle >=PI/4 && dAngle < 3*PI/4)
										if(dAngle >=0 && dAngle < PI/2)
											pGridVertexList1.AddGridVertex(pPoint->m_pPointCoord.x,pPoint->m_pPointCoord.y,pPoint->m_pPointCoord.z,dDistance,m_iPointNumPreDirection);
										//else	if(dAngle>=3*PI/4 && dAngle < 5*PI/4)
										else	if(dAngle>=PI/2 && dAngle < PI)
											pGridVertexList2.AddGridVertex(pPoint->m_pPointCoord.x,pPoint->m_pPointCoord.y,pPoint->m_pPointCoord.z,dDistance,m_iPointNumPreDirection);
										//else	if(dAngle>=5*PI/4 && dAngle < 7*PI/4)
										else	if(dAngle>=PI && dAngle < 3*PI/2)
											pGridVertexList3.AddGridVertex(pPoint->m_pPointCoord.x,pPoint->m_pPointCoord.y,pPoint->m_pPointCoord.z,dDistance,m_iPointNumPreDirection);
										else
											pGridVertexList4.AddGridVertex(pPoint->m_pPointCoord.x,pPoint->m_pPointCoord.y,pPoint->m_pPointCoord.z,dDistance,m_iPointNumPreDirection);
									}
								}

								pPFD	= pPFD->m_pNext;
							}
						}
					
					}//for(int i=iBegin;i<=iEnd;i++)

				}//for(int j=jBegin;j<=jEnd;j++)
				
				bFinished	= true;
				if(pGridVertexList1.GetGridVertexCount() < m_iPointNumPreDirection)
				{
					bFinished	= false;
				}

				if(bFinished)
				{
					if(pGridVertexList2.GetGridVertexCount() < m_iPointNumPreDirection)
					{
						bFinished	= false;
					}
				}

				if(bFinished)
				{
					if(pGridVertexList3.GetGridVertexCount() < m_iPointNumPreDirection)
					{
						bFinished	= false;
					}
				}

				if(bFinished)
				{
					if(pGridVertexList4.GetGridVertexCount() < m_iPointNumPreDirection)
					{
						bFinished	= false;
					}
				}

				//ͨ��iK�������
				//double	dMaxDistance;
				//if(m_dCellSizeOfX > m_dCellSizeOfY)
				//	dMaxDistance	= iK * m_dCellSizeOfY;
				//else	
				//	dMaxDistance	= iK * m_dCellSizeOfX;
				//����һ������:�뾶
				//if(dMaxDistance > m_dSearchRadius)
				//	bFinished	= true;

				iK++;

				if(m_iCellNumOfX > m_iCellNumOfY)
				{
					if(iK > m_iCellNumOfX)	
						bFinished	= true;
				}
				else
				{	
					if(iK > m_iCellNumOfY)	
						bFinished	= true;
				}

			}while(!bFinished);	
			
			//��pTempGridVertexList�е����ݸ��������뾶���޶����õ�m_pGridVertexList�У�
			//���м����һ��������ж�����
			CDCR_GridVertex	*pVertex;

			//��һ������
			pVertex	= pGridVertexList1.GetGridVertex();
			while(pVertex != NULL)
			{
				if(pVertex->m_dDistance < m_dSearchRadius)
				{
					m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);
				}
				else
				{
					//�����뾶��������
					m_bSearchRadiusUsed		= true;
				}

				pVertex	= pVertex->m_pNext;
			}

			//�ڶ�������
			pVertex	= pGridVertexList2.GetGridVertex();
			while(pVertex != NULL)
			{
				if(pVertex->m_dDistance < m_dSearchRadius)
				{
					m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);
				}
				else
				{
					//�����뾶��������
					m_bSearchRadiusUsed		= true;
				}

				pVertex	= pVertex->m_pNext;
			}
			
			//����������
			pVertex	= pGridVertexList3.GetGridVertex();
			while(pVertex != NULL)
			{
				if(pVertex->m_dDistance < m_dSearchRadius)
				{
					m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);
				}
				else
				{
					//�����뾶��������
					m_bSearchRadiusUsed		= true;
				}

				pVertex	= pVertex->m_pNext;
			}

			//
			pVertex	= pGridVertexList4.GetGridVertex();
			while(pVertex != NULL)
			{
				if(pVertex->m_dDistance < m_dSearchRadius)
				{
					m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);
				}
				else
				{
					//�����뾶��������
					m_bSearchRadiusUsed		= true;
				}

				pVertex	= pVertex->m_pNext;
			}

			//���磬���еĲ����㶼���뾶�ˣ���ô�죿
			if(m_pGridVertexList.GetGridVertexCount() == 0)
			{
				//��һ������
				pVertex	= pGridVertexList1.GetGridVertex();
				while(pVertex != NULL)
				{
					m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);

					pVertex	= pVertex->m_pNext;
				}

				//�ڶ�������
				pVertex	= pGridVertexList2.GetGridVertex();
				while(pVertex != NULL)
				{
					m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);

					pVertex	= pVertex->m_pNext;
				}
				
				//����������
				pVertex	= pGridVertexList3.GetGridVertex();
				while(pVertex != NULL)
				{
					m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);

					pVertex	= pVertex->m_pNext;
				}

				//
				pVertex	= pGridVertexList4.GetGridVertex();
				while(pVertex != NULL)
				{
					m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);

					pVertex	= pVertex->m_pNext;
				}
			}
			else
			{
				if(m_bSearchRadiusUsed)		m_iSearchRadiusUsed++;
			}
		}

		//�˷�������
		if(m_enumDirection == EightDirection)
		{
			//��¼������Ϣ
			//0-45
			CDCR_GridVertexList	pGridVertexList1;
			//45-90
			CDCR_GridVertexList	pGridVertexList2;
			//90-135
			CDCR_GridVertexList	pGridVertexList3;
			//135-180
			CDCR_GridVertexList	pGridVertexList4;
			//180-225
			CDCR_GridVertexList	pGridVertexList5;
			//225-270
			CDCR_GridVertexList	pGridVertexList6;
			//270-315
			CDCR_GridVertexList	pGridVertexList7;
			//315-360
			CDCR_GridVertexList	pGridVertexList8;
			
			//�ж���ֹ�������ǵ����ﵽҪ��
			//������������һȦ
			int		iK			= 1;
			
			bool	bFinished	= true;

			do
			{
				//
				int		iBegin,iEnd;
				int		jBegin,jEnd;
				
				iBegin	= iCurrentI - iK;		iEnd	= iCurrentI + iK;
				jBegin	= iCurrentJ - iK;		jEnd	= iCurrentJ + iK;

				if(iBegin < 0)					iBegin	= 0;
				if(iBegin >= m_iCellNumOfX-1)	iBegin	= m_iCellNumOfX-1;
				if(iEnd < 0)					iEnd	= 0;
				if(iEnd >= m_iCellNumOfX-1)		iEnd	= m_iCellNumOfX-1;

				if(jBegin < 0)					jBegin	= 0;
				if(jBegin >= m_iCellNumOfY-1)	jBegin	= m_iCellNumOfY-1;
				if(jEnd < 0)					jEnd	= 0;
				if(jEnd >= m_iCellNumOfY-1)		jEnd	= m_iCellNumOfY-1;

				for(int j=jBegin;j<=jEnd;j++)
				{
					for(int i=iBegin;i<=iEnd;i++)
					{
						//
						int		iI,iJ,iC;

						iI	= i;	iJ	= j;
						iC	= iJ * m_iCellNumOfX + iI;

						//�жϸø����Ƿ�ʹ��
						//û��ʹ�õĲ��ܽ���
						if(false == m_pCells[iC].m_bUsed)
						{
							//�ù���
							m_pCells[iC].m_bUsed	= true;

							//��ȡ����Ϣ
							CDCR_PointFeatureAddress	*pPFD;
							pPFD		= m_pCells[iC].m_pPFDHead;
								
							while(pPFD != NULL)
							{
								CDCR_PointFeature	*pPoint;
								pPoint		= pPFD->m_pPointFeature;

								//������֤���Ѿ���ɾ���ĵ㲻����
								if(pPoint != pDelPoint)
								{
									//û�б�ɾ���ĵ�
									if(false == pPoint->m_bUsed)
									{
										//�����жϽǶ�
										double				dAngle;
										dAngle		= GetAngle(pPoint->m_pPointCoord.x-dX,pPoint->m_pPointCoord.y-dY);

										double				dDistance;	
										dDistance	= GetDistance(pPoint->m_pPointCoord.x,pPoint->m_pPointCoord.y,0,dX,dY,0);
										
										if(dAngle >=0 && dAngle < PI/4)
											pGridVertexList1.AddGridVertex(pPoint->m_pPointCoord.x,pPoint->m_pPointCoord.y,pPoint->m_pPointCoord.z,dDistance,m_iPointNumPreDirection);
										if(dAngle>=PI/4 && dAngle < PI/2)
											pGridVertexList2.AddGridVertex(pPoint->m_pPointCoord.x,pPoint->m_pPointCoord.y,pPoint->m_pPointCoord.z,dDistance,m_iPointNumPreDirection);
										if(dAngle>=PI/2 && dAngle < 3*PI/4)
											pGridVertexList3.AddGridVertex(pPoint->m_pPointCoord.x,pPoint->m_pPointCoord.y,pPoint->m_pPointCoord.z,dDistance,m_iPointNumPreDirection);
										if(dAngle>=3*PI/4 && dAngle < PI)
											pGridVertexList4.AddGridVertex(pPoint->m_pPointCoord.x,pPoint->m_pPointCoord.y,pPoint->m_pPointCoord.z,dDistance,m_iPointNumPreDirection);

										if(dAngle >=PI && dAngle < 5*PI/4)
											pGridVertexList5.AddGridVertex(pPoint->m_pPointCoord.x,pPoint->m_pPointCoord.y,pPoint->m_pPointCoord.z,dDistance,m_iPointNumPreDirection);
										if(dAngle>=5*PI/4 && dAngle < 3*PI/2)
											pGridVertexList6.AddGridVertex(pPoint->m_pPointCoord.x,pPoint->m_pPointCoord.y,pPoint->m_pPointCoord.z,dDistance,m_iPointNumPreDirection);
										if(dAngle>=3*PI/2 && dAngle < 7*PI/4)
											pGridVertexList7.AddGridVertex(pPoint->m_pPointCoord.x,pPoint->m_pPointCoord.y,pPoint->m_pPointCoord.z,dDistance,m_iPointNumPreDirection);
										if(dAngle>=7*PI/4 && dAngle < 2*PI)
											pGridVertexList8.AddGridVertex(pPoint->m_pPointCoord.x,pPoint->m_pPointCoord.y,pPoint->m_pPointCoord.z,dDistance,m_iPointNumPreDirection);
									}
								}

								pPFD	= pPFD->m_pNext;
							}
						}
					
					}//for(int i=iBegin;i<=iEnd;i++)

				}//for(int j=jBegin;j<=jEnd;j++)
				
				bFinished	= true;
				if(pGridVertexList1.GetGridVertexCount() < m_iPointNumPreDirection)
				{
					bFinished	= false;
				}

				if(bFinished)
				{
					if(pGridVertexList2.GetGridVertexCount() < m_iPointNumPreDirection)
					{
						bFinished	= false;
					}
				}

				if(bFinished)
				{
					if(pGridVertexList3.GetGridVertexCount() < m_iPointNumPreDirection)
					{
						bFinished	= false;
					}
				}

				if(bFinished)
				{
					if(pGridVertexList4.GetGridVertexCount() < m_iPointNumPreDirection)
					{
						bFinished	= false;
					}
				}

				if(bFinished)
				{
					if(pGridVertexList5.GetGridVertexCount() < m_iPointNumPreDirection)
					{
						bFinished	= false;
					}
				}

				if(bFinished)
				{
					if(pGridVertexList6.GetGridVertexCount() < m_iPointNumPreDirection)
					{
						bFinished	= false;
					}
				}

				if(bFinished)
				{
					if(pGridVertexList7.GetGridVertexCount() < m_iPointNumPreDirection)
					{
						bFinished	= false;
					}
				}

				if(bFinished)
				{
					if(pGridVertexList8.GetGridVertexCount() < m_iPointNumPreDirection)
					{
						bFinished	= false;
					}
				}

				//ͨ��iK�������
				//double	dMaxDistance;
				//if(m_dCellSizeOfX > m_dCellSizeOfY)
				//	dMaxDistance	= iK * m_dCellSizeOfY;
				//else	
				//	dMaxDistance	= iK * m_dCellSizeOfX;
				//����һ������:�뾶
				//if(dMaxDistance > m_dSearchRadius)
				//	bFinished	= true;

				iK++;

				if(m_iCellNumOfX > m_iCellNumOfY)
				{
					if(iK > m_iCellNumOfX)	
						bFinished	= true;
				}
				else
				{	
					if(iK > m_iCellNumOfY)	
						bFinished	= true;
				}
			}while(!bFinished);	

			//��pTempGridVertexList�е����ݸ��������뾶���޶����õ�m_pGridVertexList�У�
			//���м����һ��������ж�����
			CDCR_GridVertex	*pVertex;

			//��һ������
			pVertex	= pGridVertexList1.GetGridVertex();
			while(pVertex != NULL)
			{
				if(pVertex->m_dDistance < m_dSearchRadius)
				{
					m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);
				}
				else
				{
					//�����뾶��������
					m_bSearchRadiusUsed		= true;
				}

				pVertex	= pVertex->m_pNext;
			}

			//�ڶ�������
			pVertex	= pGridVertexList2.GetGridVertex();
			while(pVertex != NULL)
			{
				if(pVertex->m_dDistance < m_dSearchRadius)
				{
					m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);
				}
				else
				{
					//�����뾶��������
					m_bSearchRadiusUsed		= true;
				}

				pVertex	= pVertex->m_pNext;
			}

			//����������
			pVertex	= pGridVertexList3.GetGridVertex();
			while(pVertex != NULL)
			{
				if(pVertex->m_dDistance < m_dSearchRadius)
				{
					m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);
				}
				else
				{
					//�����뾶��������
					m_bSearchRadiusUsed		= true;
				}

				pVertex	= pVertex->m_pNext;
			}

			//���ĸ�����
			pVertex	= pGridVertexList4.GetGridVertex();
			while(pVertex != NULL)
			{
				if(pVertex->m_dDistance < m_dSearchRadius)
				{
					m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);
				}
				else
				{
					//�����뾶��������
					m_bSearchRadiusUsed		= true;
				}

				pVertex	= pVertex->m_pNext;
			}

			//���������
			pVertex	= pGridVertexList5.GetGridVertex();
			while(pVertex != NULL)
			{
				if(pVertex->m_dDistance < m_dSearchRadius)
				{
					m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);
				}
				else
				{
					//�����뾶��������
					m_bSearchRadiusUsed		= true;
				}

				pVertex	= pVertex->m_pNext;
			}

			//����������
			pVertex	= pGridVertexList6.GetGridVertex();
			while(pVertex != NULL)
			{
				if(pVertex->m_dDistance < m_dSearchRadius)
				{
					m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);
				}
				else
				{
					//�����뾶��������
					m_bSearchRadiusUsed		= true;
				}

				pVertex	= pVertex->m_pNext;
			}

			//���߸�����
			pVertex	= pGridVertexList7.GetGridVertex();
			while(pVertex != NULL)
			{
				if(pVertex->m_dDistance < m_dSearchRadius)
				{
					m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);
				}
				else
				{
					//�����뾶��������
					m_bSearchRadiusUsed		= true;
				}

				pVertex	= pVertex->m_pNext;
			}

			//�ڰ˸�����
			pVertex	= pGridVertexList8.GetGridVertex();
			while(pVertex != NULL)
			{
				if(pVertex->m_dDistance < m_dSearchRadius)
				{
					m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);
				}
				else
				{
					//�����뾶��������
					m_bSearchRadiusUsed		= true;
				}

				pVertex	= pVertex->m_pNext;
			}

			//���磬���еĲ����㶼���뾶�ˣ���ô�죿
			if(m_pGridVertexList.GetGridVertexCount() == 0)
			{
				//��һ������
				pVertex	= pGridVertexList1.GetGridVertex();
				while(pVertex != NULL)
				{
					m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);

					pVertex	= pVertex->m_pNext;
				}

				//�ڶ�������
				pVertex	= pGridVertexList2.GetGridVertex();
				while(pVertex != NULL)
				{
					m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);

					pVertex	= pVertex->m_pNext;
				}
				
				//����������
				pVertex	= pGridVertexList3.GetGridVertex();
				while(pVertex != NULL)
				{
					m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);

					pVertex	= pVertex->m_pNext;
				}

				//
				pVertex	= pGridVertexList4.GetGridVertex();
				while(pVertex != NULL)
				{
					m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);

					pVertex	= pVertex->m_pNext;
				}
			}
			else
			{
				if(m_bSearchRadiusUsed)		m_iSearchRadiusUsed++;
			}
		}
	}

	//�ҵ��ˣ���ʵ����ͳһ����
	{
		//��������Ʊߣ�����Ҫ���Ʊ�
	}

	if(m_pGridVertexList.GetGridVertexCount() == 0)	return	false;

	return	true;
}

//////////////////////////////////////////////////////////////////////
//��ֵ
//////////////////////////////////////////////////////////////////////
void CDCR_GridCreator::Create(CDC *pDC, int iType)
{
	//�����Ȩ
	if(iType == 0)
	{
		//
		CSetGridCreatorIDWDlg	pDlg;
		
		pDlg.m_dMapLeft		= m_dMapLeft;
		pDlg.m_dMapRight	= m_dMapRight;
		pDlg.m_dMapBottom	= m_dMapBottom;
		pDlg.m_dMapTop		= m_dMapTop;

		pDlg.m_iDemNumX		= m_iDemNumOfX;
		pDlg.m_iDemNumY		= m_iDemNumOfY;

		//Ȩָ��
		pDlg.m_iU			= m_pGridIDW.m_iU;
		//ƽ��ָ��
		pDlg.m_dT			= m_pGridIDW.m_dT;
		//
		pDlg.m_szPathName	= m_pGridIDW.m_szPathName;

		if(m_bCaclRemainsError)		pDlg.m_bCheckRE		= TRUE;
		else						pDlg.m_bCheckRE		= FALSE;

		if(pDlg.DoModal() == IDOK)
		{

		}
		else	return;

		//
		if(pDlg.m_bCheckRE)	m_bCaclRemainsError	= true;
		else				m_bCaclRemainsError	= false;

		//���ô�������
		CreateInIDW(

			pDC,
			pDlg.m_iDemNumX,
			pDlg.m_iDemNumY,
			pDlg.m_szPathName,
			pDlg.m_iSearchType,
			pDlg.m_iPointCountPreDirection,
			pDlg.m_iPointCountInTotal,
			pDlg.m_dSearchRadius,
			pDlg.m_iU,
			pDlg.m_dT
			
			);

		CString	szInfo;
		szInfo.Format("�����Ȩ��ֵ�㷨���������������DEM��ϣ�����ʱ : %dms �����뾶������ %ld ",GetUsedTime(),GetSearchRadiusUsed());

		MessageBox(NULL,szInfo,"��ʾ��Ϣ",MB_OK);
	}

	//���������
	if(iType == 1)
	{
		//
		CSetGridCreatorRBFDlg	pDlg;
		
		pDlg.m_dMapLeft		= m_dMapLeft;
		pDlg.m_dMapRight	= m_dMapRight;
		pDlg.m_dMapBottom	= m_dMapBottom;
		pDlg.m_dMapTop		= m_dMapTop;

		pDlg.m_iDemNumX		= m_iDemNumOfX;
		pDlg.m_iDemNumY		= m_iDemNumOfY;

		//��ֵģ��
		pDlg.m_iModelType	= m_pGridRBF.m_iModelType;
		//ƽ��ָ��
		pDlg.m_dC			= m_pGridRBF.m_dC;
		//
		pDlg.m_szPathName	= m_pGridRBF.m_szPathName;

		if(m_bCaclRemainsError)		pDlg.m_bCheckRE		= TRUE;
		else						pDlg.m_bCheckRE		= FALSE;

		if(pDlg.DoModal() == IDOK)
		{

		}
		else	return;

		//
		if(pDlg.m_bCheckRE)	m_bCaclRemainsError	= true;
		else				m_bCaclRemainsError	= false;

		//��ֵ����
		CreateInRBF(
			
			pDC,
			pDlg.m_iDemNumX,
			pDlg.m_iDemNumY,
			pDlg.m_szPathName,
			pDlg.m_iSearchType,
			pDlg.m_iPointCountPreDirection,
			pDlg.m_iPointCountInTotal,
			pDlg.m_dSearchRadius,
			pDlg.m_iModelType,
			pDlg.m_dC
			
			);

		CString	szInfo;
		szInfo.Format("�����������ֵ�����������������DEM��ϣ�����ʱ : %dms �����뾶������ %ld ",GetUsedTime(),GetSearchRadiusUsed());

		MessageBox(NULL,szInfo,"��ʾ��Ϣ",MB_OK);
	}

	//�����
	if(iType == 2)
	{
		//
		CSetGridCreatorKRGDlg	pDlg;
		
		pDlg.m_dMapLeft		= m_dMapLeft;
		pDlg.m_dMapRight	= m_dMapRight;
		pDlg.m_dMapBottom	= m_dMapBottom;
		pDlg.m_dMapTop		= m_dMapTop;

		pDlg.m_iDemNumX		= m_iDemNumOfX;
		pDlg.m_iDemNumY		= m_iDemNumOfY;

		//��ֵģ��
		pDlg.m_iModelType	= m_pGridKRG.m_iModelType;
		//ģ�Ͳ���
		pDlg.m_dC0			= m_pGridKRG.m_dC0;
		pDlg.m_dC			= m_pGridKRG.m_dC;
		pDlg.m_dAlfa		= m_pGridKRG.m_dAlfa;
		//
		pDlg.m_szPathName	= m_pGridKRG.m_szPathName;

		if(m_bCaclRemainsError)		pDlg.m_bCheckRE		= TRUE;
		else						pDlg.m_bCheckRE		= FALSE;

		if(pDlg.DoModal() == IDOK)
		{

		}
		else	return;

		//
		if(pDlg.m_bCheckRE)	m_bCaclRemainsError	= true;
		else				m_bCaclRemainsError	= false;
		
		CreateInKRG(
			
			pDC,
			pDlg.m_iDemNumX,
			pDlg.m_iDemNumY,
			pDlg.m_szPathName,
			pDlg.m_iSearchType,
			pDlg.m_iPointCountPreDirection,
			pDlg.m_iPointCountInTotal,
			pDlg.m_dSearchRadius,
			pDlg.m_iModelType,
			pDlg.m_dC0,
			pDlg.m_dC,
			pDlg.m_dAlfa
			
			);

		CString	szInfo;
		szInfo.Format("��ͨ������ֵ�����������������DEM��ϣ�����ʱ : %dms �����뾶������ %ld ",GetUsedTime(),GetSearchRadiusUsed());

		MessageBox(NULL,szInfo,"��ʾ��Ϣ",MB_OK);
	}

	//л���
	if(iType == 3)
	{
		//
		CSetGridCreatorSPDDlg	pDlg;
				
		pDlg.m_dMapLeft		= m_dMapLeft;
		pDlg.m_dMapRight	= m_dMapRight;
		pDlg.m_dMapBottom	= m_dMapBottom;
		pDlg.m_dMapTop		= m_dMapTop;

		pDlg.m_iDemNumX		= m_iDemNumOfX;
		pDlg.m_iDemNumY		= m_iDemNumOfY;

		//Ȩָ��
		pDlg.m_iU			= m_pGridSPD.GetU();
		//ƽ��ָ��
		pDlg.m_dT			= m_pGridSPD.GetT();
		//
		pDlg.m_szPathName	= m_pGridSPD.GetPathName();

		if(m_bCaclRemainsError)		pDlg.m_bCheckRE		= TRUE;
		else						pDlg.m_bCheckRE		= FALSE;

		if(pDlg.DoModal() == IDOK)
		{

		}
		else	return;

		//
		if(pDlg.m_bCheckRE)	m_bCaclRemainsError	= true;
		else				m_bCaclRemainsError	= false;
		
		//����ȱʡ�ĵ�������dAdjustR
		double	dAdjustR;
		if(pDlg.m_bCustom == 0)
		{
			dAdjustR	= sqrt( (m_dMapRight-m_dMapLeft)*(m_dMapTop-m_dMapBottom)/m_iSampleCount * pDlg.m_iPointCountInTotal / 3.1415926 );
		}
		else
		{
			dAdjustR	= pDlg.m_dMaxDistance;
		}
		
		//��ֵ�˺�������ʽ
		int		iKernalType;
		iKernalType		= pDlg.m_iKernal;

		CreateInSPD(

			pDC,
			pDlg.m_iDemNumX,
			pDlg.m_iDemNumY,
			pDlg.m_szPathName,
			pDlg.m_iSearchType,
			pDlg.m_iPointCountPreDirection,
			pDlg.m_iPointCountInTotal,
			pDlg.m_dSearchRadius,
			pDlg.m_iU,
			pDlg.m_dT,
			pDlg.m_bCustom,
			dAdjustR,
			iKernalType

			);

		CString	szInfo;
		szInfo.Format("л��²�ֵ�����������������DEM��ϣ�����ʱ : %dms �����뾶������ %ld ",GetUsedTime(),GetSearchRadiusUsed());

		MessageBox(NULL,szInfo,"��ʾ��Ϣ",MB_OK);
	}

	//����ʽ
	if(iType == 4)
	{
		//
		CSetGridCreatorPLYDlg	pDlg;
		
		pDlg.m_dMapLeft		= m_dMapLeft;
		pDlg.m_dMapRight	= m_dMapRight;
		pDlg.m_dMapBottom	= m_dMapBottom;
		pDlg.m_dMapTop		= m_dMapTop;

		pDlg.m_iDemNumX		= m_iDemNumOfX;
		pDlg.m_iDemNumY		= m_iDemNumOfY;

		//��ֵģ��
		pDlg.m_iModelType	= m_pGridPLY.m_iModelType;
		//ģ�Ͳ���
		pDlg.m_iMode		= m_pGridPLY.m_iInterpolationMode;
		//
		pDlg.m_szPathName	= m_pGridPLY.m_szPathName;

		if(m_bCaclRemainsError)		pDlg.m_bCheckRE		= TRUE;
		else						pDlg.m_bCheckRE		= FALSE;

		if(pDlg.DoModal() == IDOK)
		{

		}
		else	return;

		//
		if(pDlg.m_bCheckRE)	m_bCaclRemainsError	= true;
		else				m_bCaclRemainsError	= false;

		CreateInPLY(
			
			pDC,
			pDlg.m_iDemNumX,
			pDlg.m_iDemNumY,
			pDlg.m_szPathName,
			pDlg.m_iSearchType,
			pDlg.m_iPointCountPreDirection,
			pDlg.m_iPointCountInTotal,
			pDlg.m_dSearchRadius,
			pDlg.m_iModelType,
			pDlg.m_iMode
			
			);

		CString	szInfo;
		szInfo.Format("����ʽ��ֵ�����������������DEM��ϣ�����ʱ : %dms �����뾶������ %ld ",GetUsedTime(),GetSearchRadiusUsed());

		MessageBox(NULL,szInfo,"��ʾ��Ϣ",MB_OK);
	}
}
	
//////////////////////////////////////////////////////////////////////
// IDW��������
//////////////////////////////////////////////////////////////////////
void CDCR_GridCreator::CreateInIDW(
								   CDC		*pDC, 
								   int		iDemNumX,
								   int		iDemNumY,
								   CString	szPathName,
								   int		iSearchType,
								   int		iPointNumPreDirection,
								   int		iPointNumInTotal,
								   double	dSearchRadius,
								   int		iU,
								   double	dT
								   )
{
	//���
	if(m_pCells == NULL)						
		return;

	//����MatCom
	initM(MATCOM_VERSION);

	//ʹ�������뾶�Ĵ���
	m_iSearchRadiusUsed	= 0;

	//
	{
		//��ʼʱ��
		m_dwStartTime	= GetTickCount();

		//
		m_iDemNumOfX	= iDemNumX;
		m_iDemNumOfY	= iDemNumY;

		m_dDemSizeOfX	= (m_dMapRight-m_dMapLeft)/(m_iDemNumOfX-1);
		m_dDemSizeOfY	= (m_dMapTop-m_dMapBottom)/(m_iDemNumOfY-1);
		
		//Ȩָ��
		m_pGridIDW.m_iU	= iU;
		//ƽ��ָ��
		m_pGridIDW.m_dT	= dT;

		//������ʽ
		if(iSearchType == 0)
			m_bGlobalSearch	= true;
		else
		{
			m_bGlobalSearch	= false;

			if(iSearchType == 1)
			{
				m_enumDirection	= FourDirection;
			}
			if(iSearchType == 2)
			{
				m_enumDirection	= EightDirection;
			}
		}	

		//ÿ�������ڵĵ���
		m_iPointNumPreDirection	= iPointNumPreDirection;
		//�ܵ���
		m_iPointNumInTotal		= iPointNumInTotal;
		//�����뾶
		m_dSearchRadius			= dSearchRadius;

		if(m_iDemNumOfX == 0 || m_iDemNumOfY == 0)
			return;

		int		iC;
		double	dX,dY,dZ;
		
		if(m_pHeight != NULL)
		{
			delete	[]m_pHeight;
			m_pHeight	= NULL;
		}

		m_pHeight	= new	double[m_iDemNumOfX*m_iDemNumOfY];

		for(int j=0;j<m_iDemNumOfY;j++)
		{
			for(int i=0;i<m_iDemNumOfX;i++)
			{
				iC	= j*m_iDemNumOfX + i;
				dX	= m_dMapLeft + i*m_dDemSizeOfX;
				dY	= m_dMapBottom + j*m_dDemSizeOfY;
				
				//ȱֵ
				dZ	= -9999;

				//����������
				if(SearchSamplingPoint(dX,dY))
				{
					//���붥������
					m_pGridIDW.SetGridVertexList(&m_pGridVertexList);
					
					//��ʼ����
					//
					m_pGridIDW.CalcHeight(dX,dY,dZ);
				}

				m_pHeight[iC]	= dZ;

				//��ͼ
				{
					DrawCell(pDC,dX,dY,dZ);
				}

				if(i==0 && j==0)
				{
					m_dMaxHeight	= m_dMinHeight	= dZ;
				}
				else
				{
					if(m_dMaxHeight < dZ)	m_dMaxHeight	= dZ;
					if(m_dMinHeight > dZ)	m_dMinHeight	= dZ;
				}

			}//for(int i=0;i<m_iDemNumOfX;i++)

		}//for(int j=0;j<m_iDemNumOfY;j++)

		m_szGrdPathName	= szPathName;
		Save(m_szGrdPathName);

		//����ʱ��
		m_dwFinishTime	= GetTickCount();
	}
	//

	//�˳�MatCom
	exitM();	
}

//////////////////////////////////////////////////////////////////////
// IDW�������� ��
//////////////////////////////////////////////////////////////////////
void CDCR_GridCreator::CreateInIDW(
								   CDC		*pDC, 
								   int		iDemNumX,
								   int		iDemNumY,
								   CString	szPathName,
								   int		iSearchType,
								   int		iPointNumPreDirection,
								   int		iPointNumInTotal,
								   double	dSearchRadius,
								   int		iNumOfU, 
								   int		*iU,
								   double	dT
								   )
{
	//���
	if(m_pCells == NULL)						
		return;

	//����MatCom
	initM(MATCOM_VERSION);

	//ʹ�������뾶�Ĵ���
	m_iSearchRadiusUsed	= 0;

	//
	{
		//��ʼʱ��
		m_dwStartTime	= GetTickCount();

		//��ֵ�ĸ�����
		m_iDemNumOfX	= iDemNumX;
		m_iDemNumOfY	= iDemNumY;
		//������Ӧ�ĸ����ߴ�
		m_dDemSizeOfX	= (m_dMapRight-m_dMapLeft)/(m_iDemNumOfX-1);
		m_dDemSizeOfY	= (m_dMapTop-m_dMapBottom)/(m_iDemNumOfY-1);
		
		//ƽ��ָ������ȱʡ��0��
		m_pGridIDW.m_dT	= dT;

		//������ʽ
		if(iSearchType == 0)
			m_bGlobalSearch	= true;
		else
		{
			m_bGlobalSearch	= false;

			if(iSearchType == 1)
			{
				m_enumDirection	= FourDirection;
			}
			if(iSearchType == 2)
			{
				m_enumDirection	= EightDirection;
			}
		}	

		//ÿ�������ڵĵ���
		m_iPointNumPreDirection	= iPointNumPreDirection;
		//�ܵ���
		m_iPointNumInTotal		= iPointNumInTotal;
		//�����뾶
		m_dSearchRadius			= dSearchRadius;

		if(m_iDemNumOfX == 0 || m_iDemNumOfY == 0)
			return;

		int		iC;
		int		i,j,k;
		double	dX,dY,dZ;

		//
		InitialHeight();
		
		for(k=0;k<iNumOfU;k++)
		{
			m_pGridHeight[k].m_iNumOfX	= m_iDemNumOfX;
			m_pGridHeight[k].m_iNumOfY	= m_iDemNumOfY;

			m_pGridHeight[k].m_pHeight	= new	double[m_iDemNumOfX*m_iDemNumOfY];
		}
		
		for(j=0;j<m_iDemNumOfY;j++)
		{
			for(i=0;i<m_iDemNumOfX;i++)
			{
				iC	= j*m_iDemNumOfX + i;
				dX	= m_dMapLeft + i*m_dDemSizeOfX;
				dY	= m_dMapBottom + j*m_dDemSizeOfY;
				
				//ȱֵ
				dZ	= -9999;

				//����������
				if(SearchSamplingPoint(dX,dY))
				{
					
					for(k=0;k<iNumOfU;k++)
					{
						//����������iU
						//Ȩָ��
						m_pGridIDW.m_iU		= iU[k];

						//���붥������
						m_pGridIDW.SetGridVertexList(&m_pGridVertexList);

						//
						m_pGridIDW.CalcHeight(dX,dY,dZ);

						//
						m_pGridHeight[k].m_pHeight[iC]	= dZ;

						m_pGridHeight[k].SetHeight(dZ);
					}
				}
				else
				{
					for(k=0;k<iNumOfU;k++)
					{
						//
						m_pGridHeight[k].m_pHeight[iC]	= dZ;

						m_pGridHeight[k].SetHeight(dZ);
					}
				}

				//
				{
					DrawCell(pDC,dX,dY,dZ);
				}

			}//for(int i=0;i<m_iDemNumOfX;i++)

		}//for(int j=0;j<m_iDemNumOfY;j++)

		//m_szGrdPathName	= szPathName;
		//Save(m_szGrdPathName);
		for(k=0;k<iNumOfU;k++)
		{
			CString	szGrdPathName;
			//���ɵ�GRD�ļ���Ӧ�÷��ں�DVF��ͬ��Ŀ¼�£������Ҫ�����л�ȡ
			szGrdPathName.Format("D%ld_P%ld_U%ld(%ld).grd",iSearchType,iPointNumInTotal,iU[k],m_iDemNumOfX);
			szGrdPathName	= szPathName + "\\" + szGrdPathName;

			//
			m_pGridHeight[k].m_szPathName	= szGrdPathName;

			SaveEx(szGrdPathName,m_pGridHeight[k].m_dMinHeight,m_pGridHeight[k].m_dMaxHeight,m_pGridHeight[k].m_pHeight);
		}

		//����ʱ��
		m_dwFinishTime	= GetTickCount();
	}
	//

	//�˳�MatCom
	exitM();	
}

//////////////////////////////////////////////////////////////////////
// ������֤IDW
//////////////////////////////////////////////////////////////////////
void CDCR_GridCreator::CrossValidateInIDW(
										  CDC		*pDC, 
										  int		iSampleCount, 
										  CDCR_PointFeature *pSamplePointList,
										  CString	szPathName,
										  int		iSearchType,
										  int		iPointNumPreDirection,
										  int		iPointNumInTotal,
										  double	dSearchRadius,
										  int		iNumOfU, 
										  int		*iU,
										  double	dT
										  )
{
	//���
	if(m_pCells == NULL)						
		return;

	//
	if(pSamplePointList == NULL)
		return;

	//����MatCom
	initM(MATCOM_VERSION);

	//ʹ�������뾶�Ĵ���
	m_iSearchRadiusUsed	= 0;

	//
	{
		//��ʼʱ��
		m_dwStartTime	= GetTickCount();

		//��ֵ�ĸ�����
		//m_iDemNumOfX	= iDemNumX;
		//m_iDemNumOfY	= iDemNumY;
		//������Ӧ�ĸ����ߴ�
		//m_dDemSizeOfX	= (m_dMapRight-m_dMapLeft)/(m_iDemNumOfX-1);
		//m_dDemSizeOfY	= (m_dMapTop-m_dMapBottom)/(m_iDemNumOfY-1);
		
		//ƽ��ָ������ȱʡ��0��
		m_pGridIDW.m_dT	= dT;

		//������ʽ
		if(iSearchType == 0)
			m_bGlobalSearch	= true;
		else
		{
			m_bGlobalSearch	= false;

			if(iSearchType == 1)
			{
				m_enumDirection	= FourDirection;
			}
			if(iSearchType == 2)
			{
				m_enumDirection	= EightDirection;
			}
		}	

		//ÿ�������ڵĵ���
		m_iPointNumPreDirection	= iPointNumPreDirection;
		//�ܵ���
		m_iPointNumInTotal		= iPointNumInTotal;
		//�����뾶
		m_dSearchRadius			= dSearchRadius;

		//if(m_iDemNumOfX == 0 || m_iDemNumOfY == 0)
		//	return;

		int		k;
		double	dX,dY,dZ;
		
		//
		InitialHeight();

		for(k=0;k<iNumOfU;k++)
		{
			m_pGridHeight[k].m_iNumOfX	= 0;
			m_pGridHeight[k].m_iNumOfY	= 0;

			m_pGridHeight[k].m_pHeight	= new	double[iSampleCount];
		}
		
		int		iC;
		iC		= 0;

		//�����������
		CDCR_PointFeature	*pPoint;
		
		pPoint	= pSamplePointList;
		while(pPoint != NULL)
		{
			if(pPoint->m_bUsed == false)
			{
				dX	= pPoint->m_pPointCoord.x;
				dY	= pPoint->m_pPointCoord.y;

				//
				//ȱֵ
				dZ	= -9999;

				if(SearchSamplingPointCV(pPoint))
				{
					for(k=0;k<iNumOfU;k++)
					{
						//����������iU
						//Ȩָ��
						m_pGridIDW.m_iU		= iU[k];

						//���붥������
						m_pGridIDW.SetGridVertexList(&m_pGridVertexList);

						//
						m_pGridIDW.CalcHeight(dX,dY,dZ);

						//
						m_pGridHeight[k].m_pHeight[iC]	= pPoint->m_pPointCoord.z - dZ;

						m_pGridHeight[k].SetHeight(pPoint->m_pPointCoord.z - dZ);
					}
				}
				else
				{
					TRACE("============�������������?============\n");
				}

				///////////////////////////////////////////////////////////
				//��ͼ
				{
					DrawCell(pDC,dX,dY,dZ);
				}
				///////////////////////////////////////////////////////////

				iC++;
			}

			//��һ��������
			pPoint	= pPoint->m_pNext;
		}

		//m_szGrdPathName	= szPathName;
		//Save(m_szGrdPathName);
		for(k=0;k<iNumOfU;k++)
		{
			CString	szGrdPathName;
			//���ɵ�GRD�ļ���Ӧ�÷��ں�DVF��ͬ��Ŀ¼�£������Ҫ�����л�ȡ
			szGrdPathName.Format("D%ld_P%ld_U%ld(%ld)RE.txt",iSearchType,iPointNumInTotal,iU[k],m_iDemNumOfX);
			szGrdPathName	= szPathName + "\\" + szGrdPathName;

			//
			m_pGridHeight[k].m_szPathName	= szGrdPathName;

			SaveRE(szGrdPathName,m_pGridHeight[k].m_dMinHeight,m_pGridHeight[k].m_dMaxHeight,iSampleCount,pSamplePointList,m_pGridHeight[k].m_pHeight);
		}

		//����ʱ��
		m_dwFinishTime	= GetTickCount();
	}
	//

	//�˳�MatCom
	exitM();	
}

//////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////
void CDCR_GridCreator::CrossValidateInIDW(
										  CDC *pDC, 
										  int iSampleCount, 
										  CDCR_PointFeature *pSamplePointList,
										  CDCR_PointFeature *pPoint,
										  int iSearchType,
										  int iPointNumPreDirection,
										  int iPointNumInTotal,
										  double dSearchRadius,
										  int iU,
										  double dT,
										  double& dDZ
										  )
{
	dDZ	= -32767;

	//���
	if(m_pCells == NULL)						
		return;

	//
	if(pSamplePointList == NULL)
		return;

	//����MatCom
	//initM(MATCOM_VERSION);

	//ʹ�������뾶�Ĵ���
	m_iSearchRadiusUsed	= 0;

	//
	{
		//��ֵ�ĸ�����
		//m_iDemNumOfX	= iDemNumX;
		//m_iDemNumOfY	= iDemNumY;
		//������Ӧ�ĸ����ߴ�
		//m_dDemSizeOfX	= (m_dMapRight-m_dMapLeft)/(m_iDemNumOfX-1);
		//m_dDemSizeOfY	= (m_dMapTop-m_dMapBottom)/(m_iDemNumOfY-1);
		
		//ƽ��ָ������ȱʡ��0��
		m_pGridIDW.m_dT	= dT;

		//������ʽ
		if(iSearchType == 0)
			m_bGlobalSearch	= true;
		else
		{
			m_bGlobalSearch	= false;

			if(iSearchType == 1)
			{
				m_enumDirection	= FourDirection;
			}
			if(iSearchType == 2)
			{
				m_enumDirection	= EightDirection;
			}
		}	

		//ÿ�������ڵĵ���
		m_iPointNumPreDirection	= iPointNumPreDirection;
		//�ܵ���
		m_iPointNumInTotal		= iPointNumInTotal;
		//�����뾶
		m_dSearchRadius			= dSearchRadius;

		//if(m_iDemNumOfX == 0 || m_iDemNumOfY == 0)
		//	return;

		double	dX,dY,dZ;

		dX	= pPoint->m_pPointCoord.x;
		dY	= pPoint->m_pPointCoord.y;

		//
		//ȱֵ
		dZ	= -9999;

		if(SearchSamplingPointCV(pPoint))
		{
			{
				//����������iU
				//Ȩָ��
				m_pGridIDW.m_iU		= iU;

				//���붥������
				m_pGridIDW.SetGridVertexList(&m_pGridVertexList);

				//
				m_pGridIDW.CalcHeight(dX,dY,dZ);

				//
				dDZ	= pPoint->m_pPointCoord.z - dZ;
			}
		}
	}

	//�˳�MatCom
	//exitM();		
}

//////////////////////////////////////////////////////////////////////
// RBF��������
//////////////////////////////////////////////////////////////////////
void CDCR_GridCreator::CreateInRBF(
								   CDC		*pDC, 
								   int		iDemNumX,
								   int		iDemNumY,
								   CString	szPathName,
								   int		iSearchType,
								   int		iPointNumPreDirection,
								   int		iPointNumInTotal,
								   double	dSearchRadius,
								   int		iModelType,
								   double	dC
								   )
{
	//���
	if(m_pCells == NULL)						
		return;

	//����MatCom
	initM(MATCOM_VERSION);

	//ʹ�������뾶�Ĵ���
	m_iSearchRadiusUsed	= 0;

	//���������
	{
		//��ʼʱ��
		m_dwStartTime	= GetTickCount();

		//
		m_iDemNumOfX	= iDemNumX;
		m_iDemNumOfY	= iDemNumY;

		m_dDemSizeOfX	= (m_dMapRight-m_dMapLeft)/(m_iDemNumOfX-1);
		m_dDemSizeOfY	= (m_dMapTop-m_dMapBottom)/(m_iDemNumOfY-1);
		
		//��ֵģ��
		m_pGridRBF.m_iModelType	= iModelType;
		//ƽ��ָ��
		m_pGridRBF.m_dC			= dC;

		//������ʽ
		if(iSearchType == 0)
			m_bGlobalSearch	= true;
		else
		{
			m_bGlobalSearch	= false;

			if(iSearchType == 1)
			{
				m_enumDirection	= FourDirection;
			}
			if(iSearchType == 2)
			{
				m_enumDirection	= EightDirection;
			}
		}	
		//ÿ�������ڵĵ���
		m_iPointNumPreDirection	= iPointNumPreDirection;
		//�ܵ���
		m_iPointNumInTotal		= iPointNumInTotal;
		//�����뾶
		m_dSearchRadius			= dSearchRadius;

		if(m_iDemNumOfX == 0 || m_iDemNumOfY == 0)
			return;

		int		iC;
		double	dX,dY,dZ;
		
		if(m_pHeight != NULL)
		{
			delete	[]m_pHeight;
			m_pHeight	= NULL;
		}

		m_pHeight	= new	double[m_iDemNumOfX*m_iDemNumOfY];

		for(int j=0;j<m_iDemNumOfY;j++)
		{
			for(int i=0;i<m_iDemNumOfX;i++)
			{
				iC	= j*m_iDemNumOfX + i;
				dX	= m_dMapLeft + i*m_dDemSizeOfX;
				dY	= m_dMapBottom + j*m_dDemSizeOfY;
				
				//ȱֵ
				dZ	= -9999;

				//����������
				if(SearchSamplingPoint(dX,dY))
				{
					//���붥������
					m_pGridRBF.SetGridVertexList(&m_pGridVertexList);
					
					//��ʼ����
					if(m_pGridVertexList.GetGridVertexCount() > 1)
					{
						//
						m_pGridRBF.CalcHeight(dX,dY,dZ);
					}
					else
					{
						CDCR_GridVertex	*pGridVertex;
						pGridVertex	= m_pGridVertexList.GetGridVertex();

						dZ	= pGridVertex->m_dZ;
					}
				}
				
				//����û���������㣬��������Ϊ0
				m_pHeight[iC]	= dZ;

				//
				{
					DrawCell(pDC,dX,dY,dZ);
				}

				if(i==0 && j==0)
				{
					m_dMaxHeight	= m_dMinHeight	= dZ;
				}
				else
				{
					if(m_dMaxHeight < dZ)	m_dMaxHeight	= dZ;
					if(m_dMinHeight > dZ)	m_dMinHeight	= dZ;
				}
				
				//break;
			}//for(int i=0;i<m_iDemNumOfX;i++)
			//break;
		}//for(int j=0;j<m_iDemNumOfY;j++)

		m_szGrdPathName	= szPathName;
		Save(m_szGrdPathName);

		m_dwFinishTime	= GetTickCount();
	}

	//�˳�MatCom
	exitM();	
}

//////////////////////////////////////////////////////////////////////
// RBF�������� ��
//////////////////////////////////////////////////////////////////////
void CDCR_GridCreator::CreateInRBF(
								   CDC		*pDC, 
								   int		iDemNumX,
								   int		iDemNumY,
								   CString	szPathName,
								   int		iSearchType,
								   int		iPointNumPreDirection,
								   int		iPointNumInTotal,
								   double	dSearchRadius,
								   int		iModelType,
								   int		iNumOfC,
								   double	*dC
								   )
{
	//���
	if(m_pCells == NULL)						
		return;

	//����MatCom
	initM(MATCOM_VERSION);

	//ʹ�������뾶�Ĵ���
	m_iSearchRadiusUsed	= 0;

	//���������
	{
		//��ʼʱ��
		m_dwStartTime	= GetTickCount();

		//
		m_iDemNumOfX	= iDemNumX;
		m_iDemNumOfY	= iDemNumY;

		m_dDemSizeOfX	= (m_dMapRight-m_dMapLeft)/(m_iDemNumOfX-1);
		m_dDemSizeOfY	= (m_dMapTop-m_dMapBottom)/(m_iDemNumOfY-1);
		
		//��ֵģ��
		m_pGridRBF.m_iModelType	= iModelType;

		//������ʽ
		if(iSearchType == 0)
			m_bGlobalSearch	= true;
		else
		{
			m_bGlobalSearch	= false;

			if(iSearchType == 1)
			{
				m_enumDirection	= FourDirection;
			}
			if(iSearchType == 2)
			{
				m_enumDirection	= EightDirection;
			}
		}	
		//ÿ�������ڵĵ���
		m_iPointNumPreDirection	= iPointNumPreDirection;
		//�ܵ���
		m_iPointNumInTotal		= iPointNumInTotal;
		//�����뾶
		m_dSearchRadius			= dSearchRadius;

		if(m_iDemNumOfX == 0 || m_iDemNumOfY == 0)
			return;

		int		k;
		int		iC;
		double	dX,dY,dZ;

		//
		InitialHeight();
		
		for(k=0;k<iNumOfC;k++)
		{
			m_pGridHeight[k].m_iNumOfX	= m_iDemNumOfX;
			m_pGridHeight[k].m_iNumOfY	= m_iDemNumOfY;

			m_pGridHeight[k].m_pHeight	= new	double[m_iDemNumOfX*m_iDemNumOfY];
		}
		
		for(int j=0;j<m_iDemNumOfY;j++)
		{
			for(int i=0;i<m_iDemNumOfX;i++)
			{
				iC	= j*m_iDemNumOfX + i;
				dX	= m_dMapLeft + i*m_dDemSizeOfX;
				dY	= m_dMapBottom + j*m_dDemSizeOfY;
				
				//ȱֵ
				dZ	= -9999;

				//����������
				if(SearchSamplingPoint(dX,dY))
				{
					for(k=0;k<iNumOfC;k++)
					{
						//����������dC
						//ƽ��ָ��
						m_pGridRBF.m_dC			= dC[k];

						//���붥������
						m_pGridRBF.SetGridVertexList(&m_pGridVertexList);
						
						//��ʼ����
						if(m_pGridVertexList.GetGridVertexCount() > 1)
						{
							//
							m_pGridRBF.CalcHeight(dX,dY,dZ);
						}
						else
						{
							CDCR_GridVertex	*pGridVertex;
							pGridVertex	= m_pGridVertexList.GetGridVertex();

							dZ	= pGridVertex->m_dZ;
						}

						//
						m_pGridHeight[k].m_pHeight[iC]	= dZ;

						m_pGridHeight[k].SetHeight(dZ);
					}
				}
				else
				{
					//����û���������㣬��������Ϊ0
					for(k=0;k<iNumOfC;k++)
					{				
						m_pGridHeight[k].m_pHeight[iC]	= dZ;
						m_pGridHeight[k].SetHeight(dZ);
					}
				}

				//
				{
					DrawCell(pDC,dX,dY,dZ);
				}
				
				//break;
			}//for(int i=0;i<m_iDemNumOfX;i++)
			//break;
		}//for(int j=0;j<m_iDemNumOfY;j++)
		
		//m_szGrdPathName	= szPathName;
		
		for(k=0;k<iNumOfC;k++)
		{
			CString	szGrdPathName;
			//���ɵ�GRD�ļ���Ӧ�÷��ں�DVF��ͬ��Ŀ¼�£������Ҫ�����л�ȡ
			szGrdPathName.Format("D%ld_P%ld_R%ld_M%ld_C%ld.grd",iSearchType,iPointNumInTotal,(int)(dSearchRadius),iModelType,(int)(dC[k]));
			szGrdPathName	= szPathName + "\\" + szGrdPathName;

			//
			m_pGridHeight[k].m_szPathName	= szGrdPathName;

			SaveEx(szGrdPathName,m_pGridHeight[k].m_dMinHeight,m_pGridHeight[k].m_dMaxHeight,m_pGridHeight[k].m_pHeight);
		}
		
		//
		//InitialHeight();

		m_dwFinishTime	= GetTickCount();
	}

	//�˳�MatCom
	exitM();	
}

//////////////////////////////////////////////////////////////////////
// RBF������֤
//////////////////////////////////////////////////////////////////////
void CDCR_GridCreator::CrossValidateInRBF(
										  CDC		*pDC, 
										  int		iSampleCount, 
										  CDCR_PointFeature *pSamplePointList,
										  CString	szPathName,
										  int		iSearchType,
										  int		iPointNumPreDirection,
										  int		iPointNumInTotal,
										  double	dSearchRadius,
										  int		iModelType,
										  int		iNumOfC,
										  double	*dC
										  )
{
	//���
	if(m_pCells == NULL)						
		return;

	//����MatCom
	initM(MATCOM_VERSION);

	//ʹ�������뾶�Ĵ���
	m_iSearchRadiusUsed	= 0;

	//���������
	{
		//��ʼʱ��
		m_dwStartTime	= GetTickCount();

		//
		//m_iDemNumOfX	= iDemNumX;
		//m_iDemNumOfY	= iDemNumY;

		//m_dDemSizeOfX	= (m_dMapRight-m_dMapLeft)/(m_iDemNumOfX-1);
		//m_dDemSizeOfY	= (m_dMapTop-m_dMapBottom)/(m_iDemNumOfY-1);
		
		//��ֵģ��
		m_pGridRBF.m_iModelType	= iModelType;

		//������ʽ
		if(iSearchType == 0)
			m_bGlobalSearch	= true;
		else
		{
			m_bGlobalSearch	= false;

			if(iSearchType == 1)
			{
				m_enumDirection	= FourDirection;
			}
			if(iSearchType == 2)
			{
				m_enumDirection	= EightDirection;
			}
		}	
		//ÿ�������ڵĵ���
		m_iPointNumPreDirection	= iPointNumPreDirection;
		//�ܵ���
		m_iPointNumInTotal		= iPointNumInTotal;
		//�����뾶
		m_dSearchRadius			= dSearchRadius;

		//if(m_iDemNumOfX == 0 || m_iDemNumOfY == 0)
		//	return;

		int		k;
		double	dX,dY,dZ;

		//
		InitialHeight();
		
		for(k=0;k<iNumOfC;k++)
		{ 
			m_pGridHeight[k].m_iNumOfX	= 0;
			m_pGridHeight[k].m_iNumOfY	= 0;

			m_pGridHeight[k].m_pHeight	= new	double[iSampleCount];
		}
		
		int		iC;
		iC		= 0;

		//�����������
		CDCR_PointFeature	*pPoint;
		
		pPoint	= pSamplePointList;
		while(pPoint != NULL)
		{
			if(pPoint->m_bUsed == false)
			{
				dX	= pPoint->m_pPointCoord.x;
				dY	= pPoint->m_pPointCoord.y;
				
				//
				//ȱֵ
				dZ	= -9999;

				if(SearchSamplingPointCV(pPoint))
				{
					for(k=0;k<iNumOfC;k++)
					{
						//����������dC
						//ƽ��ָ��
						m_pGridRBF.m_dC			= dC[k];

						//���붥������
						m_pGridRBF.SetGridVertexList(&m_pGridVertexList);
						
						//��ʼ����
						if(m_pGridVertexList.GetGridVertexCount() > 1)
						{
							//
							m_pGridRBF.CalcHeight(dX,dY,dZ);
						}
						else
						{
							CDCR_GridVertex	*pGridVertex;
							pGridVertex	= m_pGridVertexList.GetGridVertex();

							dZ	= pGridVertex->m_dZ;
						}

						//
						m_pGridHeight[k].m_pHeight[iC]	= pPoint->m_pPointCoord.z - dZ;

						m_pGridHeight[k].SetHeight(pPoint->m_pPointCoord.z - dZ);
					}
				}
				else
				{
					TRACE("============�������������?============\n");
				}

				///////////////////////////////////////////////////////////
				//��ͼ
				{
					DrawCell(pDC,dX,dY,dZ);
				}
				///////////////////////////////////////////////////////////

				iC++;
			}	

			pPoint	= pPoint->m_pNext;
		}

		//m_szGrdPathName	= szPathName;
		
		for(k=0;k<iNumOfC;k++)
		{
			CString	szGrdPathName;
			//���ɵ�GRD�ļ���Ӧ�÷��ں�DVF��ͬ��Ŀ¼�£������Ҫ�����л�ȡ
			szGrdPathName.Format("D%ld_P%ld_R%ld_M%ld_C%ldRE.txt",iSearchType,iPointNumInTotal,(int)(dSearchRadius),iModelType,(int)(dC[k]));
			szGrdPathName	= szPathName + "\\" + szGrdPathName;

			//
			m_pGridHeight[k].m_szPathName	= szGrdPathName;

			SaveRE(szGrdPathName,m_pGridHeight[k].m_dMinHeight,m_pGridHeight[k].m_dMaxHeight,iSampleCount,pSamplePointList,m_pGridHeight[k].m_pHeight);
		}
		
		//
		//InitialHeight();

		m_dwFinishTime	= GetTickCount();
	}

	//�˳�MatCom
	exitM();	
}

//////////////////////////////////////////////////////////////////////
// RBF������֤
//////////////////////////////////////////////////////////////////////
void CDCR_GridCreator::CrossValidateInRBF(
										  CDC		*pDC, 
										  int		iSampleCount, 
										  CDCR_PointFeature *pSamplePointList,
										  CDCR_PointFeature *pPoint,
										  int		iSearchType,
										  int		iPointNumPreDirection,
										  int		iPointNumInTotal,
										  double	dSearchRadius,
										  int		iModelType,
										  double	dC,
										  double&	dDZ
										  )
{
	//���
	if(m_pCells == NULL)						
		return;

	//����MatCom
	//initM(MATCOM_VERSION);

	//ʹ�������뾶�Ĵ���
	m_iSearchRadiusUsed	= 0;

	//���������
	{
		//
		//m_iDemNumOfX	= iDemNumX;
		//m_iDemNumOfY	= iDemNumY;

		//m_dDemSizeOfX	= (m_dMapRight-m_dMapLeft)/(m_iDemNumOfX-1);
		//m_dDemSizeOfY	= (m_dMapTop-m_dMapBottom)/(m_iDemNumOfY-1);
		
		//��ֵģ��
		m_pGridRBF.m_iModelType	= iModelType;

		//������ʽ
		if(iSearchType == 0)
			m_bGlobalSearch	= true;
		else
		{
			m_bGlobalSearch	= false;

			if(iSearchType == 1)
			{
				m_enumDirection	= FourDirection;
			}
			if(iSearchType == 2)
			{
				m_enumDirection	= EightDirection;
			}
		}	
		//ÿ�������ڵĵ���
		m_iPointNumPreDirection	= iPointNumPreDirection;
		//�ܵ���
		m_iPointNumInTotal		= iPointNumInTotal;
		//�����뾶
		m_dSearchRadius			= dSearchRadius;

		//if(m_iDemNumOfX == 0 || m_iDemNumOfY == 0)
		//	return;

		double	dX,dY,dZ;

		dX	= pPoint->m_pPointCoord.x;
		dY	= pPoint->m_pPointCoord.y;
		
		//
		//ȱֵ
		dZ	= -9999;

		if(SearchSamplingPointCV(pPoint))
		{
			{
				//����������dC
				//ƽ��ָ��
				m_pGridRBF.m_dC			= dC;

				//���붥������
				m_pGridRBF.SetGridVertexList(&m_pGridVertexList);
				
				//��ʼ����
				if(m_pGridVertexList.GetGridVertexCount() > 1)
				{
					//
					if(!m_pGridRBF.CalcHeight(dX,dY,dZ))
					{
						dZ	= pPoint->m_pPointCoord.z;
					}
				}
				else
				{
					CDCR_GridVertex	*pGridVertex;
					pGridVertex	= m_pGridVertexList.GetGridVertex();

					dZ	= pGridVertex->m_dZ;
				}

				//
				dDZ	= pPoint->m_pPointCoord.z - dZ;
			}
		}
	}

	//�˳�MatCom
	//exitM();	
}

//////////////////////////////////////////////////////////////////////
// KRG��������
//////////////////////////////////////////////////////////////////////
void CDCR_GridCreator::CreateInKRG(
								   CDC		*pDC,					//��ͼDC
								   int		iDemNumX,				//�����������
								   int		iDemNumY,				//
								   CString	szPathName,				//
								   int		iSearchType,			//��������
								   int		iPointNumPreDirection,	//ÿһ�����ڵĵ���
								   int		iPointNumInTotal,		//�ܵ���
								   double	dSearchRadius,			//�����뾶
								   int		iModelType,				//�����ģ������
								   double	dC0,					//���ֵ
								   double	dC,						//��ֵ̨
								   double	dAlfa					//���ֵ
								   )
{
	//���
	if(m_pCells == NULL)						
		return;

	//����MatCom
	initM(MATCOM_VERSION);

	//ʹ�������뾶�Ĵ���
	m_iSearchRadiusUsed	= 0;

	//�����
	{
		//��ʼʱ��
		m_dwStartTime	= GetTickCount();

		//
		m_iDemNumOfX	= iDemNumX;
		m_iDemNumOfY	= iDemNumY;

		m_dDemSizeOfX	= (m_dMapRight-m_dMapLeft)/(m_iDemNumOfX-1);
		m_dDemSizeOfY	= (m_dMapTop-m_dMapBottom)/(m_iDemNumOfY-1);
		
		//��ֵģ��
		m_pGridKRG.m_iModelType	= iModelType;
		//ģ�Ͳ���
		m_pGridKRG.m_dC0		= dC0;
		m_pGridKRG.m_dC			= dC;
		m_pGridKRG.m_dAlfa		= dAlfa;

		//������ʽ
		if(iSearchType == 0)
			m_bGlobalSearch	= true;
		else
		{
			m_bGlobalSearch	= false;

			if(iSearchType == 1)
			{
				m_enumDirection	= FourDirection;
			}
			if(iSearchType == 2)
			{
				m_enumDirection	= EightDirection;
			}
		}	
		//ÿ�������ڵĵ���
		m_iPointNumPreDirection	= iPointNumPreDirection;
		//�ܵ���
		m_iPointNumInTotal		= iPointNumInTotal;
		//�����뾶
		m_dSearchRadius			= dSearchRadius;

		if(m_iDemNumOfX == 0 || m_iDemNumOfY == 0)
			return;

		int		iC;
		double	dX,dY,dZ;
		double	dSDZ;

		if(m_pHeight != NULL)
		{
			delete	[]m_pHeight;
			m_pHeight	= NULL;
		}
		if(m_pHeightSD != NULL)
		{
			delete	[]m_pHeightSD;
			m_pHeightSD	= NULL;
		}

		m_pHeight	= new	double[m_iDemNumOfX*m_iDemNumOfY];
		m_pHeightSD	= new	double[m_iDemNumOfX*m_iDemNumOfY];

		for(int j=0;j<m_iDemNumOfY;j++)
		{
			for(int i=0;i<m_iDemNumOfX;i++)
			{
				iC	= j*m_iDemNumOfX + i;
				dX	= m_dMapLeft + i*m_dDemSizeOfX;
				dY	= m_dMapBottom + j*m_dDemSizeOfY;
				
				//ȱֵ
				dZ		= -9999;
				dSDZ	= -9999;

				//����������
				if(SearchSamplingPoint(dX,dY))
				{					
					if(m_pGridVertexList.GetGridVertexCount() > 1)
					{
						//���붥������
						m_pGridKRG.SetGridVertexList(&m_pGridVertexList);
						
						//��ʼ����
						m_pGridKRG.CalcHeight(dX,dY,dZ,dSDZ);
					}
					else
					{
						CDCR_GridVertex	*pGridVertex;
						pGridVertex	= m_pGridVertexList.GetGridVertex();

						dZ		= pGridVertex->m_dZ;
						dSDZ	= 0;
					}
				}

				m_pHeight[iC]	= dZ;
				m_pHeightSD[iC]	= dSDZ;

				//��ͼ
				{
					DrawCell(pDC,dX,dY,dZ);
				}

				if(i==0 && j==0)
				{
					m_dMaxHeight	= m_dMinHeight	= dZ;
				}
				else
				{
					if(m_dMaxHeight < dZ)	m_dMaxHeight	= dZ;
					if(m_dMinHeight > dZ)	m_dMinHeight	= dZ;
				}
				
			}//for(int i=0;i<m_iDemNumOfX;i++)

		}//for(int j=0;j<m_iDemNumOfY;j++)

		m_szGrdPathName	= szPathName;
		Save(m_szGrdPathName);

		m_dwFinishTime	= GetTickCount();
	}

	//�˳�MatCom
	exitM();	
}

//////////////////////////////////////////////////////////////////////
// RBF������֤
//////////////////////////////////////////////////////////////////////
void CDCR_GridCreator::CrossValidateInKRG(
										  CDC		*pDC,							//
										  int		iSampleCount,					//
										  CDCR_PointFeature *pSamplePointList,		//
										  CString	szPathName,						//
										  int		iSearchType,					//
										  int		iPointNumPreDirection,			//
										  int		iPointNumInTotal,				//
										  double	dSearchRadius,					//
										  int		iModelType,						//
										  double	dC0,							//
										  double	dC,								//
										  double	dAlfa							//
										  )
{
	//���
	if(m_pCells == NULL)						
		return;

	//����MatCom
	initM(MATCOM_VERSION);

	//ʹ�������뾶�Ĵ���
	m_iSearchRadiusUsed	= 0;

	//�������
	{
		//��ʼʱ��
		m_dwStartTime	= GetTickCount();

		//
		//m_iDemNumOfX	= iDemNumX;
		//m_iDemNumOfY	= iDemNumY;

		//m_dDemSizeOfX	= (m_dMapRight-m_dMapLeft)/(m_iDemNumOfX-1);
		//m_dDemSizeOfY	= (m_dMapTop-m_dMapBottom)/(m_iDemNumOfY-1);
		
		//��ֵģ��
		m_pGridKRG.m_iModelType	= iModelType;
		//ģ�Ͳ���
		m_pGridKRG.m_dC0		= dC0;
		m_pGridKRG.m_dC			= dC;
		m_pGridKRG.m_dAlfa		= dAlfa;

		//������ʽ
		if(iSearchType == 0)
			m_bGlobalSearch	= true;
		else
		{
			m_bGlobalSearch	= false;

			if(iSearchType == 1)
			{
				m_enumDirection	= FourDirection;
			}
			if(iSearchType == 2)
			{
				m_enumDirection	= EightDirection;
			}
		}	
		//ÿ�������ڵĵ���
		m_iPointNumPreDirection	= iPointNumPreDirection;
		//�ܵ���
		m_iPointNumInTotal		= iPointNumInTotal;
		//�����뾶
		m_dSearchRadius			= dSearchRadius;

		//if(m_iDemNumOfX == 0 || m_iDemNumOfY == 0)
		//	return;

		double	dX,dY,dZ;
		double	dSDZ;

		//
		InitialHeight();
		{
			m_pGridHeight[0].m_iNumOfX	= 0;
			m_pGridHeight[0].m_iNumOfY	= 0;

			m_pGridHeight[0].m_pHeight	= new	double[iSampleCount];
		}
		
		int		iC;
		iC		= 0;

		//�����������
		CDCR_PointFeature	*pPoint;
		
		pPoint	= pSamplePointList;
		while(pPoint != NULL)
		{
			if(pPoint->m_bUsed == false)
			{
				dX	= pPoint->m_pPointCoord.x;
				dY	= pPoint->m_pPointCoord.y;
				
				//
				//ȱֵ
				dZ	= -9999;

				if(SearchSamplingPointCV(pPoint))
				{
					if(m_pGridVertexList.GetGridVertexCount() > 1)
					{
						//���붥������
						m_pGridKRG.SetGridVertexList(&m_pGridVertexList);
						
						//��ʼ����
						m_pGridKRG.CalcHeight(dX,dY,dZ,dSDZ);
					}
					else
					{
						CDCR_GridVertex	*pGridVertex;
						pGridVertex	= m_pGridVertexList.GetGridVertex();

						dZ		= pGridVertex->m_dZ;
						dSDZ	= 0;
					}

					//
					m_pGridHeight[0].m_pHeight[iC]	= pPoint->m_pPointCoord.z - dZ;

					m_pGridHeight[0].SetHeight(pPoint->m_pPointCoord.z - dZ);

				}
				else
				{
					TRACE("============�������������?============\n");
				}

				///////////////////////////////////////////////////////////
				//��ͼ
				{
					DrawCell(pDC,dX,dY,dZ);
				}
				///////////////////////////////////////////////////////////
					
				iC++;
			}

			pPoint	= pPoint->m_pNext;
		}

		//����
		{
			CString	szGrdPathName;
			//���ɵ�GRD�ļ���Ӧ�÷��ں�DVF��ͬ��Ŀ¼�£������Ҫ�����л�ȡ
			szGrdPathName.Format("D%ld_P%ld_R%ld_M%ldRE.txt",iSearchType,iPointNumInTotal,(int)(dSearchRadius),iModelType);
			szGrdPathName	= szPathName + "\\" + szGrdPathName;

			//
			m_pGridHeight[0].m_szPathName	= szGrdPathName;

			SaveRE(szGrdPathName,m_pGridHeight[0].m_dMinHeight,m_pGridHeight[0].m_dMaxHeight,iSampleCount,pSamplePointList,m_pGridHeight[0].m_pHeight);
		}
		
		//
		//InitialHeight();

		m_dwFinishTime	= GetTickCount();
	}

	//�˳�MatCom
	exitM();	
}

//////////////////////////////////////////////////////////////////////
// RBF������֤
//////////////////////////////////////////////////////////////////////
void CDCR_GridCreator::CrossValidateInKRG(
										  CDC		*pDC,							//
										  int		iSampleCount,					//
										  CDCR_PointFeature *pSamplePointList,		//
										  CDCR_PointFeature	*pPoint,				//
										  int		iSearchType,					//
										  int		iPointNumPreDirection,			//
										  int		iPointNumInTotal,				//
										  double	dSearchRadius,					//
										  int		iModelType,						//
										  double	dC0,							//
										  double	dC,								//
										  double	dAlfa,							//
										  double&	dDZ
										  )
{
	//���
	if(m_pCells == NULL)						
		return;
		
	//���粻����ģ�ͣ��򷵻�
	if(iModelType == -1)	return;

	//����MatCom
	//initM(MATCOM_VERSION);

	//ʹ�������뾶�Ĵ���
	m_iSearchRadiusUsed	= 0;

	//�������
	{
		//
		//m_iDemNumOfX	= iDemNumX;
		//m_iDemNumOfY	= iDemNumY;

		//m_dDemSizeOfX	= (m_dMapRight-m_dMapLeft)/(m_iDemNumOfX-1);
		//m_dDemSizeOfY	= (m_dMapTop-m_dMapBottom)/(m_iDemNumOfY-1);
		
		//��ֵģ��
		m_pGridKRG.m_iModelType	= iModelType;
		//ģ�Ͳ���
		m_pGridKRG.m_dC0		= dC0;
		m_pGridKRG.m_dC			= dC;
		m_pGridKRG.m_dAlfa		= dAlfa;

		//������ʽ
		if(iSearchType == 0)
			m_bGlobalSearch	= true;
		else
		{
			m_bGlobalSearch	= false;

			if(iSearchType == 1)
			{
				m_enumDirection	= FourDirection;
			}
			if(iSearchType == 2)
			{
				m_enumDirection	= EightDirection;
			}
		}	
		//ÿ�������ڵĵ���
		m_iPointNumPreDirection	= iPointNumPreDirection;
		//�ܵ���
		m_iPointNumInTotal		= iPointNumInTotal;
		//�����뾶
		m_dSearchRadius			= dSearchRadius;

		//if(m_iDemNumOfX == 0 || m_iDemNumOfY == 0)
		//	return;

		double	dX,dY,dZ;
		double	dSDZ;

		dX	= pPoint->m_pPointCoord.x;
		dY	= pPoint->m_pPointCoord.y;
		
		//
		//ȱֵ
		dZ	= -9999;

		if(SearchSamplingPointCV(pPoint))
		{
			if(m_pGridVertexList.GetGridVertexCount() > 1)
			{
				//���붥������
				m_pGridKRG.SetGridVertexList(&m_pGridVertexList);
				
				//��ʼ����
				m_pGridKRG.CalcHeight(dX,dY,dZ,dSDZ);
			}
			else
			{
				CDCR_GridVertex	*pGridVertex;
				pGridVertex	= m_pGridVertexList.GetGridVertex();

				dZ		= pGridVertex->m_dZ;
				dSDZ	= 0;
			}

			//
			dDZ	= pPoint->m_pPointCoord.z - dZ;
		}
	}

	//�˳�MatCom
	//exitM();	
}

//////////////////////////////////////////////////////////////////////
// ��������л��½���GRD
//////////////////////////////////////////////////////////////////////
void CDCR_GridCreator::CreateInSPD(
								   CDC		*pDC, 
								   int		iDemNumX,
								   int		iDemNumY,
								   CString	szPathName,
								   int		iSearchType,
								   int		iPointNumPreDirection,
								   int		iPointNumInTotal,
								   double	dSearchRadius,
								   int		iU,
								   double	dT,
								   int		iRadiusType,
								   double	dMaxRadius,
								   int		iKernalType
								   )
{
	//���
	if(m_pCells == NULL)						
		return;

	//����MatCom
	initM(MATCOM_VERSION);

	//ʹ�������뾶�Ĵ���
	m_iSearchRadiusUsed	= 0;
	
	//л���
	{
		//��ʼʱ��
		m_dwStartTime	= GetTickCount();

		//
		m_iDemNumOfX	= iDemNumX;
		m_iDemNumOfY	= iDemNumY;

		m_dDemSizeOfX	= (m_dMapRight-m_dMapLeft)/(m_iDemNumOfX-1);
		m_dDemSizeOfY	= (m_dMapTop-m_dMapBottom)/(m_iDemNumOfY-1);
		
		//Ȩָ��
		m_pGridSPD.SetU(iU);
		//ƽ��ָ��
		m_pGridSPD.SetT(dT);
		//
		if(iRadiusType == 0)	m_pGridSPD.SetCustom(false);
		else					m_pGridSPD.SetCustom(true);

		m_pGridSPD.SetRadius(dMaxRadius);

		//������ʽ
		if(iSearchType == 0)
			m_bGlobalSearch	= true;
		else
		{
			m_bGlobalSearch	= false;

			if(iSearchType == 1)
			{
				m_enumDirection	= FourDirection;
			}
			if(iSearchType == 2)
			{
				m_enumDirection	= EightDirection;
			}
		}	
		//ÿ�������ڵĵ���
		m_iPointNumPreDirection	= iPointNumPreDirection;
		//�ܵ���
		m_iPointNumInTotal		= iPointNumInTotal;
		//�����뾶
		m_dSearchRadius			= dSearchRadius;

		if(m_iDemNumOfX == 0 || m_iDemNumOfY == 0)
			return;

		int		iC;
		double	dX,dY,dZ;
		
		if(m_pHeight != NULL)
		{
			delete	[]m_pHeight;
			m_pHeight	= NULL;
		}

		m_pHeight	= new	double[m_iDemNumOfX*m_iDemNumOfY];

		for(int j=0;j<m_iDemNumOfY;j++)
		{
			for(int i=0;i<m_iDemNumOfX;i++)
			{
				iC	= j*m_iDemNumOfX + i;
				dX	= m_dMapLeft + i*m_dDemSizeOfX;
				dY	= m_dMapBottom + j*m_dDemSizeOfY;
				
				//ȱֵ
				dZ	= -9999;

				//����������
				if(SearchSamplingPoint(dX,dY))
				{
					//���붥������
					m_pGridSPD.SetGridVertexList(&m_pGridVertexList);
					
					//��ʼ����
					//
					if(iKernalType == 0)		m_pGridSPD.CalcHeight(dX,dY,dZ);
					else						m_pGridSPD.CalcHeightEx(dX,dY,dZ);
				}

				m_pHeight[iC]	= dZ;

				//��ͼ
				{
					DrawCell(pDC,dX,dY,dZ);
				}

				if(i==0 && j==0)
				{
					m_dMaxHeight	= m_dMinHeight	= dZ;
				}
				else
				{
					if(m_dMaxHeight < dZ)	m_dMaxHeight	= dZ;
					if(m_dMinHeight > dZ)	m_dMinHeight	= dZ;
				}

			}//for(int i=0;i<m_iDemNumOfX;i++)

		}//for(int j=0;j<m_iDemNumOfY;j++)

		m_szGrdPathName	= szPathName;
		Save(m_szGrdPathName);

		m_dwFinishTime	= GetTickCount();
	}
	
	//�˳�MatCom
	exitM();	
}

//////////////////////////////////////////////////////////////////////
// SPD��������
//////////////////////////////////////////////////////////////////////
void CDCR_GridCreator::CreateInSPD(
								   CDC		*pDC, 
								   int		iDemNumX,
								   int		iDemNumY,
								   CString	szPathName,
								   int		iSearchType,
								   int		iPointNumPreDirection,
								   int		iPointNumInTotal,
								   double	dSearchRadius,
								   int		iNumOfU,
								   int		*iU,
								   double	dT,
								   int		iRadiusType,
								   double	dMaxRadius,
								   int		iKernalType
								   )
{
	//���
	if(m_pCells == NULL)						
		return;

	//����MatCom
	initM(MATCOM_VERSION);

	//ʹ�������뾶�Ĵ���
	m_iSearchRadiusUsed	= 0;
	
	//л���
	{
		//��ʼʱ��
		m_dwStartTime	= GetTickCount();

		//
		m_iDemNumOfX	= iDemNumX;
		m_iDemNumOfY	= iDemNumY;

		m_dDemSizeOfX	= (m_dMapRight-m_dMapLeft)/(m_iDemNumOfX-1);
		m_dDemSizeOfY	= (m_dMapTop-m_dMapBottom)/(m_iDemNumOfY-1);
		
		//Ȩָ��
		//m_pGridSPD.m_iU	= iU;
		//ƽ��ָ��
		m_pGridSPD.SetT(dT);
		//
		if(iRadiusType == 0)	m_pGridSPD.SetCustom(false);
		else					m_pGridSPD.SetCustom(true);

		m_pGridSPD.SetRadius(dMaxRadius);

		//������ʽ
		if(iSearchType == 0)
			m_bGlobalSearch	= true;
		else
		{
			m_bGlobalSearch	= false;

			if(iSearchType == 1)
			{
				m_enumDirection	= FourDirection;
			}
			if(iSearchType == 2)
			{
				m_enumDirection	= EightDirection;
			}
		}	
		//ÿ�������ڵĵ���
		m_iPointNumPreDirection	= iPointNumPreDirection;
		//�ܵ���
		m_iPointNumInTotal		= iPointNumInTotal;
		//�����뾶
		m_dSearchRadius			= dSearchRadius;

		if(m_iDemNumOfX == 0 || m_iDemNumOfY == 0)
			return;

		int		iC;
		int		i,j,k;
		double	dX,dY,dZ;
		
		//
		InitialHeight();

		for(k=0;k<iNumOfU;k++)
		{
			m_pGridHeight[k].m_iNumOfX	= m_iDemNumOfX;
			m_pGridHeight[k].m_iNumOfY	= m_iDemNumOfY;

			m_pGridHeight[k].m_pHeight	= new	double[m_iDemNumOfX*m_iDemNumOfY];
		}

		for(j=0;j<m_iDemNumOfY;j++)
		{
			for(i=0;i<m_iDemNumOfX;i++)
			{
				iC	= j*m_iDemNumOfX + i;
				dX	= m_dMapLeft + i*m_dDemSizeOfX;
				dY	= m_dMapBottom + j*m_dDemSizeOfY;
				
				//ȱֵ
				dZ	= -9999;

				//����������
				if(SearchSamplingPoint(dX,dY))
				{
					for(k=0;k<iNumOfU;k++)
					{
						//����������iU
						//Ȩָ��
						m_pGridSPD.SetU(iU[k]);

						//���붥������
						m_pGridSPD.SetGridVertexList(&m_pGridVertexList);

						//
						if(iKernalType == 0)	m_pGridSPD.CalcHeight(dX,dY,dZ);
						else					m_pGridSPD.CalcHeightEx(dX,dY,dZ);

						//
						m_pGridHeight[k].m_pHeight[iC]	= dZ;

						m_pGridHeight[k].SetHeight(dZ);
					}
				}
				else
				{
					for(k=0;k<iNumOfU;k++)
					{
						//
						m_pGridHeight[k].m_pHeight[iC]	= dZ;

						m_pGridHeight[k].SetHeight(dZ);
					}
				}

				//��ͼ
				{
					DrawCell(pDC,dX,dY,dZ);
				}

			}//for(int i=0;i<m_iDemNumOfX;i++)

		}//for(int j=0;j<m_iDemNumOfY;j++)

		//m_szGrdPathName	= szPathName;
		//Save(m_szGrdPathName);

		for(k=0;k<iNumOfU;k++)
		{
			CString	szGrdPathName;
			//���ɵ�GRD�ļ���Ӧ�÷��ں�DVF��ͬ��Ŀ¼�£������Ҫ�����л�ȡ
			szGrdPathName.Format("D%ld_P%ld_U%ld(%ld).grd",iSearchType,iPointNumInTotal,iU[k],m_iDemNumOfX);
			szGrdPathName	= szPathName + "\\" + szGrdPathName;

			//
			m_pGridHeight[k].m_szPathName	= szGrdPathName;

			SaveEx(szGrdPathName,m_pGridHeight[k].m_dMinHeight,m_pGridHeight[k].m_dMaxHeight,m_pGridHeight[k].m_pHeight);
		}

		m_dwFinishTime	= GetTickCount();
	}
	
	//�˳�MatCom
	exitM();	
}

//////////////////////////////////////////////////////////////////////
//SPD������֤
//////////////////////////////////////////////////////////////////////
void CDCR_GridCreator::CrossValidateInSPD(
										  CDC		*pDC, 
										  int		iSampleCount, 
										  CDCR_PointFeature *pSamplePointList,
										  CString	szPathName,
										  int		iSearchType,
										  int		iPointNumPreDirection,
										  int		iPointNumInTotal,
										  double	dSearchRadius,
										  int		iNumOfU,
										  int		*iU,
										  double	dT,
										  int		iRadiusType,
										  double	dMaxRadius,
										  int		iKernalType
										  )
{
	//���
	if(m_pCells == NULL)						
		return;

	//
	if(pSamplePointList == NULL)
		return;

	//����MatCom
	initM(MATCOM_VERSION);

	//ʹ�������뾶�Ĵ���
	m_iSearchRadiusUsed	= 0;
	
	//л���
	{
		//��ʼʱ��
		m_dwStartTime	= GetTickCount();
		
		//ƽ��ָ��
		m_pGridSPD.SetT(dT);
		//
		if(iRadiusType == 0)	m_pGridSPD.SetCustom(false);
		else					m_pGridSPD.SetCustom(true);

		m_pGridSPD.SetRadius(dMaxRadius);

		//������ʽ
		if(iSearchType == 0)
			m_bGlobalSearch	= true;
		else
		{
			m_bGlobalSearch	= false;

			if(iSearchType == 1)
			{
				m_enumDirection	= FourDirection;
			}
			if(iSearchType == 2)
			{
				m_enumDirection	= EightDirection;
			}
		}	
		//ÿ�������ڵĵ���
		m_iPointNumPreDirection	= iPointNumPreDirection;
		//�ܵ���
		m_iPointNumInTotal		= iPointNumInTotal;
		//�����뾶
		m_dSearchRadius			= dSearchRadius;

		//if(m_iDemNumOfX == 0 || m_iDemNumOfY == 0)
		//	return;

		int		k;
		double	dX,dY,dZ;
		
		//
		InitialHeight();

		for(k=0;k<iNumOfU;k++)
		{
			m_pGridHeight[k].m_iNumOfX	= 0;
			m_pGridHeight[k].m_iNumOfY	= 0;

			m_pGridHeight[k].m_pHeight	= new	double[iSampleCount];
		}

		int		iC;
		iC		= 0;

		//�����������
		CDCR_PointFeature	*pPoint;
		
		pPoint	= pSamplePointList;
		while(pPoint != NULL)
		{
			if(pPoint->m_bUsed == false)
			{
				dX	= pPoint->m_pPointCoord.x;
				dY	= pPoint->m_pPointCoord.y;

				//ȱֵ
				dZ	= -9999;

				//����������
				if(SearchSamplingPointCV(pPoint))
				{
					for(k=0;k<iNumOfU;k++)
					{
						//����������iU
						//Ȩָ��
						m_pGridSPD.SetU(iU[k]);

						//���붥������
						m_pGridSPD.SetGridVertexList(&m_pGridVertexList);

						//��ʼ����
						if(iKernalType == 0)		m_pGridSPD.CalcHeight(dX,dY,dZ);
						else						m_pGridSPD.CalcHeightEx(dX,dY,dZ);

						//
						m_pGridHeight[k].m_pHeight[iC]	= pPoint->m_pPointCoord.z - dZ;

						m_pGridHeight[k].SetHeight(pPoint->m_pPointCoord.z - dZ);
					}
				}
				else
				{
					TRACE("============�������������?============\n");
				}

				///////////////////////////////////////////////////////////
				//��ͼ
				{
					DrawCell(pDC,dX,dY,dZ);
				}
				///////////////////////////////////////////////////////////

				iC++;
			}

			pPoint	= pPoint->m_pNext;
		}

		//m_szGrdPathName	= szPathName;
		//Save(m_szGrdPathName);

		for(k=0;k<iNumOfU;k++)
		{
			CString	szGrdPathName;
			//���ɵ�GRD�ļ���Ӧ�÷��ں�DVF��ͬ��Ŀ¼�£������Ҫ�����л�ȡ
			szGrdPathName.Format("D%ld_P%ld_U%ld(%ld)RE.txt",iSearchType,iPointNumInTotal,iU[k],m_iDemNumOfX);
			szGrdPathName	= szPathName + "\\" + szGrdPathName;

			//
			m_pGridHeight[k].m_szPathName	= szGrdPathName;

			SaveRE(szGrdPathName,m_pGridHeight[k].m_dMinHeight,m_pGridHeight[k].m_dMaxHeight,iSampleCount,pSamplePointList,m_pGridHeight[k].m_pHeight);
		}

		m_dwFinishTime	= GetTickCount();
	}
	
	//�˳�MatCom
	exitM();	
}

//////////////////////////////////////////////////////////////////////
//SPD������֤
//////////////////////////////////////////////////////////////////////
void CDCR_GridCreator::CrossValidateInSPD(
										  CDC		*pDC, 
										  int		iSampleCount, 
										  CDCR_PointFeature *pSamplePointList,
										  CDCR_PointFeature	*pPoint,
										  int		iSearchType,
										  int		iPointNumPreDirection,
										  int		iPointNumInTotal,
										  double	dSearchRadius,
										  int		iU,
										  double	dT,
										  int		iRadiusType,
										  double	dMaxRadius,
										  int		iKernalType,
										  double&	dDZ
										  )
{
	//���
	if(m_pCells == NULL)						
		return;

	//
	if(pSamplePointList == NULL)
		return;

	//����MatCom
	//initM(MATCOM_VERSION);

	//ʹ�������뾶�Ĵ���
	m_iSearchRadiusUsed	= 0;
	
	//л���
	{
		//��ʼʱ��
		m_dwStartTime	= GetTickCount();
		
		//ƽ��ָ��
		m_pGridSPD.SetT(dT);
		//
		if(iRadiusType == 0)	m_pGridSPD.SetCustom(false);
		else					m_pGridSPD.SetCustom(true);

		m_pGridSPD.SetRadius(dMaxRadius);

		//������ʽ
		if(iSearchType == 0)
			m_bGlobalSearch	= true;
		else
		{
			m_bGlobalSearch	= false;

			if(iSearchType == 1)
			{
				m_enumDirection	= FourDirection;
			}
			if(iSearchType == 2)
			{
				m_enumDirection	= EightDirection;
			}
		}	
		//ÿ�������ڵĵ���
		m_iPointNumPreDirection	= iPointNumPreDirection;
		//�ܵ���
		m_iPointNumInTotal		= iPointNumInTotal;
		//�����뾶
		m_dSearchRadius			= dSearchRadius;

		//if(m_iDemNumOfX == 0 || m_iDemNumOfY == 0)
		//	return;
		
		double	dX,dY,dZ;

		dX	= pPoint->m_pPointCoord.x;
		dY	= pPoint->m_pPointCoord.y;

		//ȱֵ
		dZ	= -9999;

		//����������
		if(SearchSamplingPointCV(pPoint))
		{
			//����������iU
			//Ȩָ��
			m_pGridSPD.SetU(iU);

			//���붥������
			m_pGridSPD.SetGridVertexList(&m_pGridVertexList);

			//��ʼ����
			if(iKernalType == 0)		m_pGridSPD.CalcHeight(dX,dY,dZ);
			else						m_pGridSPD.CalcHeightEx(dX,dY,dZ);

			//
			dDZ	= pPoint->m_pPointCoord.z - dZ;
		}
	}
	
	//�˳�MatCom
	//exitM();	
}

//////////////////////////////////////////////////////////////////////
// PLY��������
//////////////////////////////////////////////////////////////////////
void CDCR_GridCreator::CreateInPLY(
								   CDC		*pDC,						//��ͼDC
								   int		iDemNumX,					//�����������
								   int		iDemNumY,					//
								   CString	szPathName,					//
								   int		iSearchType,				//����������ʻ
								   int		iPointNumPreDirection,		//ÿһ�����ڵĵ���
								   int		iPointNumInTotal,			//�ܵ���
								   double	dSearchRadius,				//�����뾶
								   int		iModelType,					//ģ������
								   int		iInterpolationMode			//
								   )
{
	//���
	if(m_pCells == NULL)						
		return;

	//����MatCom
	initM(MATCOM_VERSION);

	//ʹ�������뾶�Ĵ���
	m_iSearchRadiusUsed	= 0;

	//����ʽ
	{
		//��ʼʱ��
		m_dwStartTime	= GetTickCount();

		//
		m_iDemNumOfX	= iDemNumX;
		m_iDemNumOfY	= iDemNumY;

		m_dDemSizeOfX	= (m_dMapRight-m_dMapLeft)/(m_iDemNumOfX-1);
		m_dDemSizeOfY	= (m_dMapTop-m_dMapBottom)/(m_iDemNumOfY-1);
		
		//��ֵģ��
		m_pGridPLY.m_iModelType			= iModelType;
		//ģ�Ͳ���
		m_pGridPLY.m_iInterpolationMode	= iInterpolationMode;

		//������ʽ
		if(iSearchType == 0)
			m_bGlobalSearch	= true;
		else
		{
			m_bGlobalSearch	= false;

			if(iSearchType == 1)
			{
				m_enumDirection	= FourDirection;
			}
			if(iSearchType == 2)
			{
				m_enumDirection	= EightDirection;
			}
		}	
		//ÿ�������ڵĵ���
		m_iPointNumPreDirection	= iPointNumPreDirection;
		//�ܵ���
		m_iPointNumInTotal		= iPointNumInTotal;
		//�����뾶
		m_dSearchRadius			= dSearchRadius;

		if(m_iDemNumOfX == 0 || m_iDemNumOfY == 0)
			return;

		int		iC;
		double	dX,dY,dZ;
		
		if(m_pHeight != NULL)
		{
			delete	[]m_pHeight;
			m_pHeight	= NULL;
		}

		m_pHeight	= new	double[m_iDemNumOfX*m_iDemNumOfY];

		for(int j=0;j<m_iDemNumOfY;j++)
		{
			for(int i=0;i<m_iDemNumOfX;i++)
			{
				iC	= j*m_iDemNumOfX + i;
				dX	= m_dMapLeft + i*m_dDemSizeOfX;
				dY	= m_dMapBottom + j*m_dDemSizeOfY;
				
				//ȱֵ
				dZ	= -9999;

				//����������
				if(SearchSamplingPoint(dX,dY))
				{					
					//���붥������
					m_pGridPLY.SetGridVertexList(&m_pGridVertexList);
					
					//��ʼ����
					m_pGridPLY.CalcHeight(dX,dY,dZ);
				}

				m_pHeight[iC]	= dZ;

				//����
				{
					DrawCell(pDC,dX,dY,dZ);
				}

				if(i==0 && j==0)
				{
					m_dMaxHeight	= m_dMinHeight	= dZ;
				}
				else
				{
					if(m_dMaxHeight < dZ)	m_dMaxHeight	= dZ;
					if(m_dMinHeight > dZ)	m_dMinHeight	= dZ;
				}

			}//for(int i=0;i<m_iDemNumOfX;i++)

		}//for(int j=0;j<m_iDemNumOfY;j++)

		m_szGrdPathName	= szPathName;
		Save(m_szGrdPathName);

		m_dwFinishTime	= GetTickCount();
	}

	//�˳�MatCom
	exitM();	
}

//////////////////////////////////////////////////////////////////////
// ����
//////////////////////////////////////////////////////////////////////
void CDCR_GridCreator::Save(CString szPathName)
{
	if(m_pHeight == NULL)	return;

	FILE	*fp;
	fp	= fopen(szPathName,"w");

	if(fp == NULL)	return;
	
	//DSAA
	fprintf(fp,"%s\n","DSAA");

	//iCols,iRows,Ӧ���ж����ʲô���к���
	fprintf(fp,"%6ld %6ld\n",m_iDemNumOfX,m_iDemNumOfY);
	
	//dMinX,dMaxX
	fprintf(fp,"%10.6lf %10.6lf\n",m_dMapLeft,m_dMapRight);

	//dMinY,dMaxY
	fprintf(fp,"%10.6lf %10.6lf\n",m_dMapBottom,m_dMapTop);

	//dMinZ,dMaxZ
	fprintf(fp,"%10.6lf %10.6lf\n",m_dMinHeight,m_dMaxHeight);
	
	for(int j=0;j<m_iDemNumOfY;j++)
	{
		for(int i=0;i<m_iDemNumOfX;i++)
		{
			double	dH;
			dH	= m_pHeight[j*m_iDemNumOfX+i];

			fprintf(fp,"%10.6lf ",dH);

			if((i+1) % 10 == 0)
				printf("\n");
		}

		printf("\n");
	}
	
	//�ر�
	fclose(fp);

	//���淽������
	if(m_pHeightSD != NULL)
	{
		CString	szSDPathName;
		szSDPathName	= szPathName.Left(szPathName.GetLength()-4) + "SD.grd";
		FILE	*fp;
		fp	= fopen(szSDPathName,"w");

		if(fp == NULL)	return;
		
		//DSAA
		fprintf(fp,"%s\n","DSAA");

		//iCols,iRows,Ӧ���ж����ʲô���к���
		fprintf(fp,"%6ld %6ld\n",m_iDemNumOfX,m_iDemNumOfY);
		
		//dMinX,dMaxX
		fprintf(fp,"%10.6lf %10.6lf\n",m_dMapLeft,m_dMapRight);

		//dMinY,dMaxY
		fprintf(fp,"%10.6lf %10.6lf\n",m_dMapBottom,m_dMapTop);

		//dMinZ,dMaxZ
		fprintf(fp,"%10.6lf %10.6lf\n",m_dMinHeight,m_dMaxHeight);
		
		for(int j=0;j<m_iDemNumOfY;j++)
		{
			for(int i=0;i<m_iDemNumOfX;i++)
			{
				double	dH;
				dH	= m_pHeightSD[j*m_iDemNumOfX+i];

				fprintf(fp,"%10.6lf ",dH);

				if((i+1) % 10 == 0)
					printf("\n");
			}

			printf("\n");
		}
		
		//�ر�
		fclose(fp);		
	}
}

//////////////////////////////////////////////////////////////////////
// ����
//////////////////////////////////////////////////////////////////////
void CDCR_GridCreator::SaveEx(CString szPathName,double dMinHeight,double dMaxHeight,double *pHeight)
{
	if(pHeight == NULL)	return;

	FILE	*fp;
	fp	= fopen(szPathName,"w");

	if(fp == NULL)	return;
	
	//DSAA
	fprintf(fp,"%s\n","DSAA");

	//iCols,iRows,Ӧ���ж����ʲô���к���
	fprintf(fp,"%6ld %6ld\n",m_iDemNumOfX,m_iDemNumOfY);
	
	//dMinX,dMaxX
	fprintf(fp,"%10.6lf %10.6lf\n",m_dMapLeft,m_dMapRight);

	//dMinY,dMaxY
	fprintf(fp,"%10.6lf %10.6lf\n",m_dMapBottom,m_dMapTop);

	//dMinZ,dMaxZ
	fprintf(fp,"%10.6lf %10.6lf\n",dMinHeight,dMaxHeight);
	
	for(int j=0;j<m_iDemNumOfY;j++)
	{
		for(int i=0;i<m_iDemNumOfX;i++)
		{
			double	dH;
			dH	= pHeight[j*m_iDemNumOfX+i];

			fprintf(fp,"%10.6lf ",dH);

			if((i+1) % 10 == 0)
				printf("\n");
		}

		printf("\n");
	}
	
	//�ر�
	fclose(fp);
}

//////////////////////////////////////////////////////////////////////
// ����в�
//////////////////////////////////////////////////////////////////////
void CDCR_GridCreator::SaveRE(CString szPathName,double dMinHeight,double dMaxHeight,int iCount,CDCR_PointFeature *pSamplePointList,double *pHeight)
{
	//
	if(pHeight == NULL)				return;
	//
	if(pSamplePointList == NULL)	return;

	FILE	*fp;
	fp	= fopen(szPathName,"w");

	if(fp == NULL)	return;
	
	CDCR_PointFeature	*pPoint;
	int					i;

	i		= 0;
	pPoint	= pSamplePointList;
	while(pPoint != NULL)
	{
		if(pPoint->m_bUsed == false)
		{
			double	dX,dY,dZ,dRE;

			dX	= pPoint->m_pPointCoord.x;
			dY	= pPoint->m_pPointCoord.y;
			dZ	= pPoint->m_pPointCoord.z;

			dRE	= pHeight[i];
			fprintf(fp,"%10.6lf \t%10.6lf \t%10.6lf \t%10.6lf \n",dX,dY,dZ,dRE);

			i++;
		}

		pPoint	= pPoint->m_pNext;
	}
	
	//�ر�
	fclose(fp);
}

//////////////////////////////////////////////////////////////////////
// ��ȡDEM�ĸ߳�����
//////////////////////////////////////////////////////////////////////
double* CDCR_GridCreator::GetHeight()
{
	if(m_pHeight == NULL)	return	NULL;
	
	return	m_pHeight;
}

//////////////////////////////////////////////////////////////////////
//����в�
//////////////////////////////////////////////////////////////////////
bool CDCR_GridCreator::IsCalcRemainsError()
{
	return	m_bCaclRemainsError;
}

//////////////////////////////////////////////////////////////////////
// ��ȡGRD�ļ�·��
//////////////////////////////////////////////////////////////////////
CString CDCR_GridCreator::GetGrdPathName()
{
	return	m_szGrdPathName;
}

//////////////////////////////////////////////////////////////////////
// ��ȡʹ��ʱ��
//////////////////////////////////////////////////////////////////////
int CDCR_GridCreator::GetUsedTime()
{
	return	(int)(m_dwFinishTime-m_dwStartTime);
}

//////////////////////////////////////////////////////////////////////
// ��ȡʹ��ʹ���������뾶������
//////////////////////////////////////////////////////////////////////
int CDCR_GridCreator::GetSearchRadiusUsed()
{
	return	m_iSearchRadiusUsed;
}

//////////////////////////////////////////////////////////////////////
// ��ʼ��Height
//////////////////////////////////////////////////////////////////////
void CDCR_GridCreator::InitialHeight()
{
	for(int i=0;i<100;i++)
	{
		m_pGridHeight[i].Release();
	}
}
