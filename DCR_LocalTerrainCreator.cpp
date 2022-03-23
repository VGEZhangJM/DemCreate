// DCR_LocalTerrainCreator.cpp: implementation of the CDCR_LocalTerrainCreator class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "vr_demcreator.h"
#include "DCR_LocalTerrainCreator.h"

#include "DCR_LocalTerrainCharacter.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDCR_LocalTerrainCreator::CDCR_LocalTerrainCreator()
{

	//����CDCR_WorkSpace�еķ�����������
	//������������
	m_pCells				= NULL;
	//
	//����������С
	m_dCellSizeOfX			= 0;
	m_dCellSizeOfY			= 0;
	
	//������������
	m_iCellNumOfX			= 0;
	m_iCellNumOfY			= 0;

	//����������Χ
	m_dCellLeft				= 0;
	m_dCellRight			= 0;
	m_dCellBottom			= 0;
	m_dCellTop				= 0;

	//����ֵ
	m_dPointPatternValue	= 0;
	//
	m_dCentroidValue		= 0;

	//ƽ���¶�ֵ
	m_dMeanSlope			= 0;
	//���δֲڶ�ֵ
	m_dMeanRoughness		= 0;
	//
	m_dMaxDistance			= 0;
	m_dMinDistance			= 0;
}

CDCR_LocalTerrainCreator::~CDCR_LocalTerrainCreator()
{	
	OnRelease();
}

//ԭʼ���ݵķ�����������
void CDCR_LocalTerrainCreator::SetCell(CDCR_Cells *pCells,int iNumX,int iNumY,double dSizeX,double dSizeY,double dLeft,double dRight,double dBottom,double dTop)
{
	//
	m_pCells				= pCells;

	//����������С
	m_dCellSizeOfX			= dSizeX;
	m_dCellSizeOfY			= dSizeY;
	
	//������������
	m_iCellNumOfX			= iNumX;
	m_iCellNumOfY			= iNumY;

	//����������Χ
	m_dCellLeft				= dLeft;
	m_dCellRight			= dRight;
	m_dCellBottom			= dBottom;
	m_dCellTop				= dTop;
}

//
void CDCR_LocalTerrainCreator::OnRelease()
{
	//
	m_pGridVertexList.Release();
	//
	m_pAllGridVertexList.Release();
	//
	m_pTriangleList.Release();
}

//
void CDCR_LocalTerrainCreator::CalculateLocalTerrainCharacter(	int iSampleCount, 
																CDCR_PointFeature *pSamplePointList,
																CString	szPathName,
																int		iSearchType,
																int		iPointNumPreDirection,
																int		iPointNumInTotal,
																double	dSearchRadius
																)
{
	//���
	if(m_pCells == NULL)			return;

	if(pSamplePointList == NULL)	return;

	//������������
	{
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
	}

	//д���ļ�
	FILE	*fw;
	CString	szTerrainCharacterPathName;

	szTerrainCharacterPathName.Format("D%ld_P%ld_TE.txt",iSearchType,m_iPointNumInTotal);
	szTerrainCharacterPathName	= szPathName + "\\" + szTerrainCharacterPathName;
	
	fw	= fopen(szTerrainCharacterPathName,"w");
	if(fw != NULL)
	{
		//д���ļ�ͷ
		{
			fprintf(fw,"������\t ������\t ����ֵ\t ����ֵ\t ƽ���¶�\t �ֲڶ� ������\t  ��С����\t \n");
		}

		//�����������
		CDCR_PointFeature	*pPoint;
		
		pPoint	= pSamplePointList;
		while(pPoint != NULL)
		{
			if(pPoint->m_bUsed == false)
			{
				double	dX,dY;
				dX	= pPoint->m_pPointCoord.x;
				dY	= pPoint->m_pPointCoord.y;

				if(SearchSamplingPoint(pPoint))
				{
					//���ǰ뾶
					double	dMaxDistance;
					CalculateSearchRadius(dX,dY,dMaxDistance);

					//��������
					ReSearchSamplingPoint(pPoint,dMaxDistance);
					
					//�����������ֵ
					//���������õ��Ĳ�������ڣ������
					if(m_pAllGridVertexList.GetGridVertexCount() > 0)
					{
						CDCR_LocalTerrainCharacter	pLocalTerrainCharacter;
						
						//������Ҫ��ȫ����
						//pLocalTerrainCharacter.SetGridVertexList(&m_pAllGridVertexList,&m_pGridVertexList,dMaxDistance,dX,dY);
						pLocalTerrainCharacter.SetGridVertexList(&m_pAllGridVertexList,&m_pAllGridVertexList,dMaxDistance,dX,dY);
						
						//�������
						pLocalTerrainCharacter.CalculatePointPattern();

						//�����¶Ⱥʹֲڶ�
						pLocalTerrainCharacter.CaculateTerrainFactor();

						
						fprintf(fw,"%lf\t %lf\t",dX,dY);
						
						//����û����������Ч�ĵ����������ڲд�Ʒ�����������жϣ����ܻ��������
						/*
						if(m_iPointNumInTotal != m_pGridVertexList.GetGridVertexCount())
						{
							pLocalTerrainCharacter.SetMeanSlope(-32767);
							pLocalTerrainCharacter.SetPointPatternValue(-32767);
							pLocalTerrainCharacter.SetMeanRoughness(-32767);
							pLocalTerrainCharacter.SetCentroidValue(-32767);
						}
						*/

						//
						pLocalTerrainCharacter.OnSave(fw);

						fprintf(fw,"\n");
					}
				}
			}

			//
			pPoint	= pPoint->m_pNext;
		}
		
		//
		fclose(fw);
	}
}

//
void CDCR_LocalTerrainCreator::CalculateLocalTerrainCharacter(	int iSampleCount, 
																CDCR_PointFeature *pSamplePointList,
																CDCR_PointFeature *pSelectPoint,																
																int		iSearchType,
																int		iPointNumPreDirection,
																int		iPointNumInTotal,
																double	dSearchRadius
																)
{
	//��ʼ��
	m_dPointPatternValue= -32767;
	m_dCentroidValue	= -32767;
	m_dMeanSlope		= -32767;
	m_dMeanRoughness	= -32767;
	m_dMaxDistance		= -32767;
	m_dMinDistance		= -32767;

	//���
	if(m_pCells == NULL)			return;

	if(pSamplePointList == NULL)	return;

	if(pSelectPoint == NULL)		return;

	//������������
	{
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
	}

	double	dX,dY;
	dX	= pSelectPoint->m_pPointCoord.x;
	dY	= pSelectPoint->m_pPointCoord.y;

	if(SearchSamplingPoint(pSelectPoint))
	{
		//���ǰ뾶
		double	dMaxDistance;
		CalculateSearchRadius(dX,dY,dMaxDistance);

		//��������
		ReSearchSamplingPoint(pSelectPoint,dMaxDistance);
		
		//�����������ֵ
		//���������õ��Ĳ�������ڣ������
		if(m_pAllGridVertexList.GetGridVertexCount() > 0)
		{
			if(iPointNumInTotal != m_pGridVertexList.GetGridVertexCount())
			{
				m_dPointPatternValue= -32767;
				m_dCentroidValue	= -32767;
				m_dMeanSlope		= -32767;
				m_dMeanRoughness	= -32767;
				m_dMaxDistance		= -32767;
				m_dMinDistance		= -32767;
			}
			else
			{
				CDCR_LocalTerrainCharacter	pLocalTerrainCharacter;
				
				//����Ҫ���������ڵ�ȫ����
				//pLocalTerrainCharacter.SetGridVertexList(&m_pAllGridVertexList,&m_pGridVertexList,dMaxDistance,dX,dY);
				pLocalTerrainCharacter.SetGridVertexList(&m_pAllGridVertexList,&m_pAllGridVertexList,dMaxDistance,dX,dY);
				
				//�������
				pLocalTerrainCharacter.CalculatePointPattern();

				//�����¶Ⱥʹֲڶ�
				pLocalTerrainCharacter.CaculateTerrainFactor();

				//��ȡ��������������Ϣ
				m_pTriangleList.Release();
				m_pTriangleList.Clone(pLocalTerrainCharacter.GetTriangleList());
				
				//
				m_dPointPatternValue= pLocalTerrainCharacter.GetPointPatternValue();
				m_dCentroidValue	= pLocalTerrainCharacter.GetCentroidValue();
				m_dMeanSlope		= pLocalTerrainCharacter.GetMeanSlope() * 180 /PI;
				m_dMeanRoughness	= pLocalTerrainCharacter.GetMeanRoughness();
				m_dMaxDistance		= pLocalTerrainCharacter.GetMaxDistance();
				m_dMinDistance		= pLocalTerrainCharacter.GetMinDistance();
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////
//ʹ�ý�����֤���㾫��ֵ
//CDCR_PointFeature *pDelPoint��	Ϊɾ������Ҫ���²�ֵ�ĵ�
//////////////////////////////////////////////////////////////////////
bool  CDCR_LocalTerrainCreator::SearchSamplingPoint(CDCR_PointFeature *pDelPoint)
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

										//
										if(fabs(dAngle-2*PI) < 1e-5)	dAngle	= 0;

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

										//
										if(fabs(dAngle-2*PI) < 1e-5)	dAngle	= 0;

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

//������������
bool  CDCR_LocalTerrainCreator::ReSearchSamplingPoint(CDCR_PointFeature *pDelPoint,double dRadius)
{
	if(pDelPoint == NULL)	return	false;
	
	if(m_pCells == NULL)	return	false;

	//����ɾ��
	m_pAllGridVertexList.Release();

	double	dX,dY;
	dX	= pDelPoint->m_pPointCoord.x;
	dY	= pDelPoint->m_pPointCoord.y;

	double	dLeft,dRight,dBottom,dTop;
	dLeft	= dX - dRadius;
	dRight	= dX + dRadius;
	dBottom	= dY - dRadius;
	dTop	= dY + dRadius;

	//���ȣ�����(dLeft,dRight,dBottom,dTop)ȷ����ֵ�����ڵĸ���
	int		iBegin,iEnd;
	int		jBegin,jEnd;

	iBegin		= (int)((dLeft - m_dCellLeft) / m_dCellSizeOfX) - 1;
	iEnd		= (int)((dRight - m_dCellLeft) / m_dCellSizeOfX) + 1;

	jBegin		= (int)((dBottom - m_dCellBottom) / m_dCellSizeOfY) - 1;
	jEnd		= (int)((dTop - m_dCellBottom) / m_dCellSizeOfY) + 1;

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
			
			//				
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
						
						if(dDistance <= dRadius+0.000001)
						{
							//
							m_pAllGridVertexList.AddGridVertexEx(pPoint->m_pPointCoord.x,pPoint->m_pPointCoord.y,pPoint->m_pPointCoord.z,dDistance);
						}
					}
				}

				pPFD	= pPFD->m_pNext;
			}
		}
	}
	
	return	true;
}

//�������ҵ���İ뾶��Ҳ������Զ����
bool  CDCR_LocalTerrainCreator::CalculateSearchRadius(double dX, double dY, double& dMaxDistance)
{
	if(m_pGridVertexList.GetGridVertexCount() == 0)	return	false;

	CDCR_GridVertex	*pVertex;
	bool			bFirstPoint;

	bFirstPoint	= true;
	pVertex		= m_pGridVertexList.GetGridVertex();
	while(pVertex != NULL)
	{
		if(bFirstPoint)
		{
			dMaxDistance	= pVertex->m_dDistance;

			bFirstPoint		= false;
		}
		else
		{
			if(dMaxDistance < pVertex->m_dDistance)	
				dMaxDistance	= pVertex->m_dDistance;
		}

		pVertex	= pVertex->m_pNext;
	}
	
	double	dTempDistance;
	dTempDistance	= 0;

	bFirstPoint		= true;
	pVertex			= m_pGridVertexList.GetGridVertex();
	while(pVertex != NULL)
	{
		double	dDistance;
		dDistance	= GetDistance(dX,dY,0,pVertex->m_dX,pVertex->m_dY,0);

		if(bFirstPoint)
		{
			dTempDistance	= dDistance;
			bFirstPoint		= false;
		}
		else
		{
			if(dTempDistance < dDistance)	dTempDistance	= dDistance;
		}

		pVertex	= pVertex->m_pNext;	
	}

	return	true;
}

//////////////////////////////////////////////////////////////////////
//������־����
//////////////////////////////////////////////////////////////////////
void CDCR_LocalTerrainCreator::CellReset()
{
	if(m_pCells != NULL)
	{
		for(int i=0;i<m_iCellNumOfX*m_iCellNumOfY;i++)
		{
			m_pCells[i].m_bUsed	= false;
		}
	}
}

//////////////////////////////////////////////////////////////////////
// 01 ��ȡ�����ľ���
//////////////////////////////////////////////////////////////////////
double CDCR_LocalTerrainCreator::GetDistance(double x1, double y1, double z1, double x2, double y2, double z2)
{
	double	dDistance;

	//dDistance	= sqrt( (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) + (z1-z2)*(z1-z2) );
	dDistance	= sqrt( (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) );

	return	dDistance;
}

//////////////////////////////////////////////////////////////////////
// 02 ��ȡ�Ƕ�
//////////////////////////////////////////////////////////////////////
double CDCR_LocalTerrainCreator::GetAngle(double dX, double dY)
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

//
double CDCR_LocalTerrainCreator::GetPointPatternValue()
{
	return	m_dPointPatternValue;
}

//
double CDCR_LocalTerrainCreator::GetCentroidValue()
{
	return	m_dCentroidValue;
}

//
double CDCR_LocalTerrainCreator::GetMeanSlope()
{
	return	m_dMeanSlope;
}

//
double CDCR_LocalTerrainCreator::GetMeanRoughness()
{
	return	m_dMeanRoughness;
}

//
double CDCR_LocalTerrainCreator::GetMaxDistance()
{
	return	m_dMaxDistance;
}

//
double CDCR_LocalTerrainCreator::GetMinDistance()
{
	return	m_dMinDistance;
}

//
CDCR_GridVertexList* CDCR_LocalTerrainCreator::GetGridVertexList()
{
	return	&m_pGridVertexList;
}

CDCR_GridVertexList* CDCR_LocalTerrainCreator::GetAllGridVertexList()
{
	return	&m_pAllGridVertexList;
}

//
CDCR_TinTriangleList* CDCR_LocalTerrainCreator::GetTriangleList()
{
	return	&m_pTriangleList;
}