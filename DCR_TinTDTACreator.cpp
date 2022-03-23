// DCR_TinTDTACreator.cpp: implementation of the CDCR_TinTDTACreator class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DCR_TinTDTACreator.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDCR_TinTDTACreator::CDCR_TinTDTACreator()
{
	//���㴮
	m_pVertexHead	= NULL;
	m_pVertexLast	= NULL;
	m_iVertexCount	= 0;
	
	//�ߴ�
	m_pEdgeHead		= NULL;
	m_pEdgeLast		= NULL;
	m_iEdgeCount	= 0;

	//�����δ�
	m_pTriangleHead	= NULL;
	m_pTriangleLast	= NULL;
	m_iTriangleCount= 0;

	//��ͼ����
	m_dMapLeft		= 0.0;
	m_dMapRight		= 0.0;
	m_dMapBottom	= 0.0;
	m_dMapTop		= 0.0;

	//��ͼ������С
	m_dCellSizeOfY	= 0.0;
	m_dCellSizeOfX	= 0.0;
	
	//��ͼ�ֿ���
	m_nCellNumOfY	= 15;
	m_nCellNumOfX	= 15;

	//��������Χ
	m_dCellLeft		= m_dCellRight	= 0;
	m_dCellBottom	= m_dCellTop	= 0;

	m_pCells		= NULL;
}

CDCR_TinTDTACreator::~CDCR_TinTDTACreator()
{
	if(m_pCells != NULL)
	{
		delete	[]m_pCells;
		m_pCells	= NULL;
	}

	Release();
}

//////////////////////////////////////////////////////////////////////
// 01 ��ȡ�����ľ���
//////////////////////////////////////////////////////////////////////
double CDCR_TinTDTACreator::GetDistance(double x1, double y1, double z1, double x2, double y2, double z2)
{
	double	dDistance;

	//dDistance	= sqrt( (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) + (z1-z2)*(z1-z2) );
	dDistance	= sqrt( (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) );

	return	dDistance;
}

//////////////////////////////////////////////////////////////////////
// 02 ��������������
// �жϵ�ֱ�ߵ���һ����
// -1:��ʾ��ֱ
// 1:��ʾ���ⷽ��
//////////////////////////////////////////////////////////////////////
int CDCR_TinTDTACreator::CreateJudgeFunction(double dX1, double dY1, double dX2, double dY2, double &a, double &b)
{
	if(fabs(dX2-dX1) < 1e-10)	//x2==x1
	{
		a	= dX1;	
		b	= 0;
		return	-1;
	}
	else
	if(fabs(dY2-dY1) < 1e-10)	//y2=y1
	{
		a	= 0;
		b	= dY1;
		return	1;
	}
	else
	{
		a	= (dY2-dY1) / (dX2-dX1);
		b	= (dX1*dY2-dX2*dY1) / (dX2-dX1);

		return	0;
	}
}

//////////////////////////////////////////////////////////////////////
// 03 ��������������
// �жϽ��
//////////////////////////////////////////////////////////////////////
double CDCR_TinTDTACreator::GetJudgeValue(double dX, double dY, int bSign, double a, double b)
{
	double	dValue;

	if(bSign == -1)				//��ֱ
	{
		dValue	= dX-a;			//
	}
	else	
	if(bSign == 1)				//ˮƽ
	{
		dValue	= dY-b;			
	}
	else
	{
		dValue	= dY-(a*dX-b);	//
	}
	
	return	dValue;
}

//////////////////////////////////////////////////////////////////////
// 04 ��������������
// ��ȡ���ҽǶ�ֵ
//////////////////////////////////////////////////////////////////////
double CDCR_TinTDTACreator::GetCosValue(double dX, double dY, double dZ, double dX1, double dY1, double dZ1,double dX2, double dY2,double dZ2)
{
	double	a,b,c;

	a	= GetDistance(dX1,dY1,dZ1,dX2,dY2,dZ2);
	b	= GetDistance(dX,dY,dZ,dX1,dY1,dZ1);
	c	= GetDistance(dX,dY,dZ,dX2,dY2,dZ2);

	double	cosa;
	cosa	= (b*b+c*c-a*a)/(2*b*c);

	return	cosa;
}

//////////////////////////////////////////////////////////////////////
// 05 ͨ������ID��ȡ��������
// ����ֵ��bool��true��ʾ�ɹ���false��ʾʧ��
//////////////////////////////////////////////////////////////////////
bool CDCR_TinTDTACreator::GetVertexCoordFromID(int iVertexID, double &dX, double &dY, double &dZ)
{
	CDCR_TinVertex	*pVertex;
	pVertex	= m_pVertexHead;

	while(pVertex != NULL)
	{
		if(pVertex->m_nID == iVertexID)
		{
			dX	= pVertex->m_xx;
			dY	= pVertex->m_yy;
			dZ	= pVertex->m_zz;

			return	true;
		}

		pVertex	= pVertex->m_pNext;
	}

	return	false;
}

//////////////////////////////////////////////////////////////////////
// 06 �ӱ���Ż�ö������
//////////////////////////////////////////////////////////////////////
CDCR_TinEdge* CDCR_TinTDTACreator::GetEdgeFromEdgeID(int iEdgeID, int &iVertexID1, int &iVertexID2)
{	
	CDCR_TinEdge*	pEdge;

	pEdge	= m_pEdgeHead;
	while(pEdge != NULL)
	{
		if(pEdge->m_nID == iEdgeID)
		{
			iVertexID1	= pEdge->m_nVertexID[0];
			iVertexID2	= pEdge->m_nVertexID[1];		
			
			return	pEdge;
		}
		
		pEdge	= pEdge->m_pNext;
	}
	
	return	NULL;
}

//////////////////////////////////////////////////////////////////////
// 07 ͨ�������ñ�
//////////////////////////////////////////////////////////////////////
CDCR_TinEdge* CDCR_TinTDTACreator::GetEdgeWithVertex(int nVertexID1, int nVertexID2)
{
	CDCR_TinEdge*	pEdge;

	pEdge	= m_pEdgeHead;
	while(pEdge != NULL)
	{
		if((pEdge->m_nVertexID[0]==nVertexID1&&pEdge->m_nVertexID[1]==nVertexID2) || 
			(pEdge->m_nVertexID[0]==nVertexID2&&pEdge->m_nVertexID[1]==nVertexID1))
		{
			return	pEdge;
		}
	

		pEdge	= pEdge->m_pNext;
	}

	return	NULL;
}

void CDCR_TinTDTACreator::GetEdgeWithVertex(int iVertexID1, int iVertexID2, int iVertexID3,CDCR_TinEdge** pEdge1,CDCR_TinEdge** pEdge2,CDCR_TinEdge** pEdge3)
{
	CDCR_TinEdge*	pEdge;
	int				iItem;

	int				iEdgeVertexID1,iEdgeVertexID2;

	iItem	= 0;
	pEdge	= m_pEdgeHead;
	while(pEdge != NULL)
	{
		//�����Ѿ���ʹ�����Σ�������������
		//if(pEdge->m_iUsedCount < 2)
		{
			iEdgeVertexID1	= pEdge->m_nVertexID[0];
			iEdgeVertexID2	= pEdge->m_nVertexID[1]; 

			if((iEdgeVertexID1==iVertexID1&&iEdgeVertexID2==iVertexID2) || (iEdgeVertexID1==iVertexID2&&iEdgeVertexID2==iVertexID1))
			{
				(*pEdge1)	= pEdge;
				iItem++;
			}
		
			if((iEdgeVertexID1==iVertexID2&&iEdgeVertexID2==iVertexID3) || (iEdgeVertexID1==iVertexID3&&iEdgeVertexID2==iVertexID2))
			{
				(*pEdge2)	= pEdge;
				iItem++;
			}

			if((iEdgeVertexID1==iVertexID3&&iEdgeVertexID2==iVertexID1) || (iEdgeVertexID1==iVertexID1&&iEdgeVertexID2==iVertexID3))
			{
				(*pEdge2)	= pEdge;
				iItem++;
			}
			
			if(iItem == 3)	break;
		}

		pEdge	= pEdge->m_pNext;
	}
}

//////////////////////////////////////////////////////////////////////
// 08 ��ȡ��û�д���������ζ���
// ���ΪNULL�����ʾ���е������ζ��Ѿ��������
//////////////////////////////////////////////////////////////////////
CDCR_TinTriangle* CDCR_TinTDTACreator::GetUnUsedTriangle()
{
	CDCR_TinTriangle	*pTriangle;

	pTriangle	= m_pTriangleHead;
	while(pTriangle != NULL)
	{
		if(pTriangle->m_bUsed == false)
		{
			if(pTriangle->IsUsed())
				return	pTriangle;
			else
				pTriangle->m_bUsed	= true;
		}
		pTriangle	= pTriangle->m_pNext;
	}

	return	NULL;
}


//////////////////////////////////////////////////////////////////////
// 09 �жϱ��Ƿ�ʹ������
//////////////////////////////////////////////////////////////////////
bool CDCR_TinTDTACreator::IsEdgeUsedTwice(int iVertexID1, int iVertexID2)
{
	CDCR_TinEdge*	pEdge;
	
	pEdge	= m_pEdgeHead;
	while(pEdge != NULL)
	{
		//�ܹ��ҵ��ߣ�����Ҫ�ж�
		//if(pEdge->m_bUsed == false)
		{
			if((pEdge->m_nVertexID[0]==iVertexID1&&pEdge->m_nVertexID[1]==iVertexID2) || 
				(pEdge->m_nVertexID[1]==iVertexID1&&pEdge->m_nVertexID[0]==iVertexID2))
			{
				if(pEdge->m_iUsedCount < 2)
					return	false;
				else
				{
					//pEdge->m_bUsed	= true;
					return	true;
				}
			}		
		}

		pEdge	= pEdge->m_pNext;
	}
	
	//������µı�?	
	return	false;	
}

//////////////////////////////////////////////////////////////////////
// 10 ��������������
// Ѱ�ҵ�һ���������š�����������ġ�
//////////////////////////////////////////////////////////////////////
int CDCR_TinTDTACreator::CreateFirstVertex()
{	
	CDCR_TinVertex	*pVertex;
	//������������Ӿ��Σ�ͬʱ������������
	double	dMinX,dMaxX,dX;
	double	dMinY,dMaxY,dY;
	double	dMinZ,dMaxZ,dZ;

	//
	pVertex	= m_pVertexHead;
	while(pVertex != NULL)
	{
		if(pVertex==m_pVertexHead)
		{
			dMinX	= dMaxX	= pVertex->m_xx;
			dMinY	= dMaxY	= pVertex->m_yy;
			dMinZ	= dMaxZ	= pVertex->m_zz;
		}
		else
		{
			if(dMinX > pVertex->m_xx)	dMinX	= pVertex->m_xx;
			if(dMaxX < pVertex->m_xx)	dMaxX	= pVertex->m_xx;
			if(dMinY > pVertex->m_yy)	dMinY	= pVertex->m_yy;
			if(dMaxY < pVertex->m_yy)	dMaxY	= pVertex->m_yy;
			if(dMinZ > pVertex->m_zz)	dMinZ	= pVertex->m_zz;
			if(dMaxZ < pVertex->m_zz)	dMaxZ	= pVertex->m_zz;
		}

		pVertex	= pVertex->m_pNext;
	}
	
	dX	= (dMinX + dMaxX) / 2;
	dY	= (dMinY + dMaxY) / 2;
	dZ	= (dMinZ + dMaxZ) / 2;
	
	//������û���Ż��ĳ���
	/*
	//Ѱ����dX��dY��dZ����Ķ���
	double			dDistance;
	CDCR_TinVertex	*pTempVertex;

	pTempVertex	= NULL;
	pVertex		= m_pVertexHead;
	while(pVertex != NULL)
	{
		//�����ж�
		double	dTempValue;
		double	dTempXX,dTempYY,dTempZZ;

		dTempXX		= pVertex->m_xx;
		dTempYY		= pVertex->m_yy;
		dTempZZ		= pVertex->m_zz;

		dTempValue	= GetDistance(dX,dY,dZ,dTempXX,dTempYY,dTempZZ);

		if(pVertex == m_pVertexHead)
		{
			dDistance	= dTempValue;
			pTempVertex	= pVertex;
		}
		else
		{
			if(dDistance > dTempValue)
			{
				dDistance	= dTempValue;
				pTempVertex	= pVertex;
			}
		}

		pVertex	= pVertex->m_pNext;
	}
	
	if(pTempVertex != NULL)
	{
		return	pTempVertex->m_nID;
	}
	*/

	//
	CellReset();

	//ȷ��ѭ������������m_nCellNumOfX��m_nCellNumOfY�Ĵ���
	int	iMaxCount;

	iMaxCount	= m_nCellNumOfX;
	if(iMaxCount < m_nCellNumOfY)	iMaxCount	= m_nCellNumOfY;
	
	//�ҵ�dX��dY���ڵĸ���
	int		I,J;
	I	= (int)((dX - m_dCellLeft) / m_dCellSizeOfX);
	J	= (int)((dY - m_dCellBottom) / m_dCellSizeOfY);
	
	//ѭ���Ĵ���
	for(int k=1;k<=iMaxCount;k++)
	{
		int		iBegin,iEnd;
		int		jBegin,jEnd;
		
		iBegin	= I-k;	iEnd	= I+k;
		jBegin	= J-k;	jEnd	= J+k;
		
		bool	bFirstValue;
		double	dMinDistance;
		CDCR_TinVertex	*pTempVertex;

		pTempVertex	= NULL;
		bFirstValue	= true;
		dMinDistance= 0.0;

		int		iC;
		int		iI,iJ;
		for(int j=jBegin;j<=jEnd;j++)
		{
			for(int i=iBegin;i<=iEnd;i++)
			{
				iI	= i;	iJ	= j;

				if(iI < 0)				iI	= 0;
				if(iJ < 0)				iJ	= 0;
				if(iI >= m_nCellNumOfX)	iI	= m_nCellNumOfX-1;
				if(iJ >= m_nCellNumOfY)	iJ	= m_nCellNumOfY-1;
				
				//
				iC	= iJ * m_nCellNumOfX + iI;

				//
				if(m_pCells[iC].m_bUsed == false)
				{
					CDCR_TinVertexAddress	*pTVD;
					
					pTVD	= m_pCells[iC].m_pTVDHead;
					while(pTVD != NULL)
					{			
						//�����ж�
						double	dTempValue;
						double	dTempXX,dTempYY,dTempZZ;

						dTempXX		= pTVD->m_pVertex->m_xx;
						dTempYY		= pTVD->m_pVertex->m_yy;
						dTempZZ		= pTVD->m_pVertex->m_zz;

						dTempValue	= GetDistance(dX,dY,dZ,dTempXX,dTempYY,dTempZZ);

						if(bFirstValue)
						{
							dMinDistance= dTempValue;
							pTempVertex	= pTVD->m_pVertex;

							bFirstValue	= false;
						}
						else
						{
							if(dMinDistance > dTempValue)
							{
								dMinDistance= dTempValue;
								pTempVertex	= pTVD->m_pVertex;
							}
						}
						
						pTVD	= pTVD->m_pNext;
					}
				}

				//�����
				m_pCells[iC].m_bUsed	= true;
			}
		}

		//�ж�������
		if(pTempVertex != NULL)
		{	
			return	pTempVertex->m_nID;
		}
	}
	
	return	-1;
}

//////////////////////////////////////////////////////////////////////
// 11 ��������������
// ���ݵ�һ����������Ѱ�ҵڶ�����������
//////////////////////////////////////////////////////////////////////
int CDCR_TinTDTACreator::CreateSecondVertex(int iVertexID)
{
	//��ȡ��һ�����������
	double	dX1,dY1,dZ1;
	if(!GetVertexCoordFromID(iVertexID,dX1,dY1,dZ1))	return	-1;
	
	//������û�о����Ż��ĳ���
	/*
	//�����ж����������͵�һ����������������һ������
	double	dDistance;
	bool	bFirstValue;
	
	CDCR_TinVertex	*pVertex;
	CDCR_TinVertex	*pTempVertex;
	
	bFirstValue	= true;
	pTempVertex	= NULL;
	pVertex		= m_pVertexHead;
	while(pVertex != NULL)
	{
		if(pVertex->m_nID != iVertexID)
		{
			//�����ж�
			double	dTempValue;
			double	dTempXX,dTempYY,dTempZZ;

			dTempXX		= pVertex->m_xx;
			dTempYY		= pVertex->m_yy;
			dTempZZ		= pVertex->m_zz;

			dTempValue	= GetDistance(dX1,dY1,dZ1,dTempXX,dTempYY,dTempZZ);

			if(bFirstValue)
			{
				dDistance	= dTempValue;
				pTempVertex	= pVertex;
				bFirstValue	= false;
			}
			else
			{
				if(dDistance > dTempValue)
				{
					dDistance	= dTempValue;
					pTempVertex	= pVertex;
				}
			}
		}

		pVertex	= pVertex->m_pNext;
	}
	
	if(pTempVertex != NULL)
	{
		return	pTempVertex->m_nID;
	}
	*/

	//
	CellReset();

	//ȷ��ѭ������������m_nCellNumOfX��m_nCellNumOfY�Ĵ���
	int	iMaxCount;

	iMaxCount	= m_nCellNumOfX;
	if(iMaxCount < m_nCellNumOfY)	iMaxCount	= m_nCellNumOfY;
	
	//�ҵ�dX��dY���ڵĸ���
	int		I,J;
	I	= (int)((dX1 - m_dCellLeft) / m_dCellSizeOfX);
	J	= (int)((dY1 - m_dCellBottom) / m_dCellSizeOfY);
	
	//ѭ���Ĵ���
	for(int k=1;k<iMaxCount;k++)
	{
		int		iBegin,iEnd;
		int		jBegin,jEnd;
		
		iBegin	= I-k;	iEnd	= I+k;
		jBegin	= J-k;	jEnd	= J+k;
		
		bool	bFirstValue;
		double	dMinDistance;
		CDCR_TinVertex	*pTempVertex;

		pTempVertex	= NULL;
		bFirstValue	= true;
		dMinDistance= 0.0;

		int		iC;
		int		iI,iJ;
		for(int j=jBegin;j<=jEnd;j++)
		{
			for(int i=iBegin;i<=iEnd;i++)
			{
				iI	= i;	iJ	= j;

				if(iI < 0)				iI	= 0;
				if(iJ < 0)				iJ	= 0;
				if(iI >= m_nCellNumOfX)	iI	= m_nCellNumOfX-1;
				if(iJ >= m_nCellNumOfY)	iJ	= m_nCellNumOfY-1;
				
				//
				iC	= iJ * m_nCellNumOfX + iI;

				//
				if(m_pCells[iC].m_bUsed == false)
				{
					CDCR_TinVertexAddress	*pTVD;
					
					pTVD	= m_pCells[iC].m_pTVDHead;
					while(pTVD != NULL)
					{			
						if(pTVD->m_pVertex->m_nID != iVertexID)
						{
							//�����ж�
							double	dTempValue;
							double	dTempXX,dTempYY,dTempZZ;

							dTempXX		= pTVD->m_pVertex->m_xx;
							dTempYY		= pTVD->m_pVertex->m_yy;
							dTempZZ		= pTVD->m_pVertex->m_zz;

							dTempValue	= GetDistance(dX1,dY1,dZ1,dTempXX,dTempYY,dTempZZ);

							if(bFirstValue)
							{
								dMinDistance= dTempValue;
								pTempVertex	= pTVD->m_pVertex;

								bFirstValue	= false;
							}
							else
							{
								if(dMinDistance > dTempValue)
								{
									dMinDistance= dTempValue;
									pTempVertex	= pTVD->m_pVertex;
								}
							}
						}
						
						pTVD	= pTVD->m_pNext;
					}
				}

				//�����
				m_pCells[iC].m_bUsed	= true;
			}
		}

		//�ж�������
		if(pTempVertex != NULL)
		{	
			return	pTempVertex->m_nID;
		}
	}
	
	return	-1;
}

//////////////////////////////////////////////////////////////////////
// 12 ��������������
// ��ȡ��������������
//////////////////////////////////////////////////////////////////////
int CDCR_TinTDTACreator::CreateThirdVertex(int iFirstVertexID, int iSecondVertexID)
{
	//��ȡ��һ�����������
	double	dX1,dY1,dZ1;
	if(!GetVertexCoordFromID(iFirstVertexID,dX1,dY1,dZ1))	return	-1;

	//��ȡ��һ�����������
	double	dX2,dY2,dZ2;
	if(!GetVertexCoordFromID(iSecondVertexID,dX2,dY2,dZ2))	return	-1;

	//��������ϵĵ����,�������ʷ�׼��Ѱ�ҵ�,������������̾���Ѱ�ҵ�
	bool	bCosValue;
	double	dCosValue;

	CDCR_TinVertex	*pVertex;
	CDCR_TinVertex	*pTempVertex;

	pTempVertex	= NULL;
	bCosValue	= true;
	pVertex		= m_pVertexHead;
	while(pVertex != NULL)
	{
		if(pVertex->m_nID!=iFirstVertexID && pVertex->m_nID!=iSecondVertexID)
		{
			//�����ж�
			double		dTempValue;
			double		dTempXX,dTempYY,dTempZZ;

			dTempXX		= pVertex->m_xx;
			dTempYY		= pVertex->m_yy;
			dTempZZ		= pVertex->m_zz;
			
			//����COSa
			dTempValue	= GetCosValue(dTempXX,dTempYY,dTempZZ,dX1,dY1,dZ1,dX2,dY2,dZ2);

			if(bCosValue)
			{
				dCosValue	= dTempValue;
				pTempVertex	= pVertex;
				bCosValue	= false;
			}
			else
			{
				if(dCosValue > dTempValue)
				{
					dCosValue	= dTempValue;
					pTempVertex	= pVertex;
				}
			}
		}

		pVertex	= pVertex->m_pNext;
	}

	if(pTempVertex != NULL)
	{
		return	pTempVertex->m_nID;
	}

	return	-1;
}

//////////////////////////////////////////////////////////////////////
// 13 �����������㷨����TIN
//////////////////////////////////////////////////////////////////////
bool CDCR_TinTDTACreator::Create(CDC *pDC)
{
	//���Ƚ�����������
	CellBuild();


	//���ͷ�,�ߺ�������
	{
		//
		ReleaseEdge();

		//
		ReleaseTriangle();
	}

	//������һ��������
	{
		int	iFirstVertexID,iSecondVertexID,iThirdVertexID;
		
		iFirstVertexID	= CreateFirstVertex();
		
		iSecondVertexID	= CreateSecondVertex(iFirstVertexID);
		if(iSecondVertexID < 0)	
		{
			//MessageBox(NULL,"Tin�������ɹ�!","��ʾ��Ϣ",MB_OK);
			return	false;
		}

		iThirdVertexID	= CreateThirdVertex(iFirstVertexID,iSecondVertexID);
		if(iThirdVertexID < 0)	
		{
			//MessageBox(NULL,"Tin�������ɹ�!","��ʾ��Ϣ",MB_OK);
			return	false;
		}

		double	dX1,dY1,dZ1,dX2,dY2,dZ2,dX3,dY3,dZ3;

		GetVertexCoordFromID(iFirstVertexID,dX1,dY1,dZ1);
		GetVertexCoordFromID(iSecondVertexID,dX2,dY2,dZ2);
		GetVertexCoordFromID(iThirdVertexID,dX3,dY3,dZ3);

		//���Ƚ���������
		//{
			//��һ��������
			CDCR_TinTriangle*	pTriangle;
			pTriangle					= new	CDCR_TinTriangle;
			if(pTriangle == NULL)	return	false;

			pTriangle->m_nID			= 1;
			pTriangle->m_nVertexID[0]	= iFirstVertexID;
			pTriangle->m_nVertexID[1]	= iSecondVertexID;
			pTriangle->m_nVertexID[2]	= iThirdVertexID;
			AddTriangle(pTriangle);
			
			//��������
			pTriangle->SetVertexCoord(iFirstVertexID,dX1,dY1,dZ1,iSecondVertexID,dX2,dY2,dZ2,iThirdVertexID,dX3,dY3,dZ3);
		//}

		//������һ����
		{
			CDCR_TinEdge*	pEdge1;
			//��һ����
			pEdge1						= new	CDCR_TinEdge;
			pEdge1->m_nID				= 1;
			pEdge1->m_nVertexID[0]		= iFirstVertexID;
			pEdge1->m_nVertexID[1]		= iSecondVertexID;
			pEdge1->m_nTriangleID[0]	= 1;
			pEdge1->m_pLeftTriangle		= pTriangle;
			pEdge1->m_iUsedCount++;
			//
			AddEdge(pEdge1);
			//���ӵ�������
			AddEdgeToCells((dX1+dX2)/2,(dY1+dY2)/2,pEdge1);
			//ʵʱ����
			if(pDC != NULL)	DrawEdge(pDC,pEdge1);
		}

		
		//�����ڶ�����
		{
			CDCR_TinEdge*	pEdge2;
			//�ڶ�����
			pEdge2						= new	CDCR_TinEdge;
			pEdge2->m_nID				= 2;
			pEdge2->m_nVertexID[0]		= iFirstVertexID;
			pEdge2->m_nVertexID[1]		= iThirdVertexID;
			//��ź�ָ����ͬһ����˼
			pEdge2->m_nTriangleID[0]	= 1;
			pEdge2->m_pLeftTriangle		= pTriangle;
			pEdge2->m_iUsedCount++;

			//
			AddEdge(pEdge2);
			//���ӵ�������
			AddEdgeToCells((dX1+dX3)/2,(dY1+dY3)/2,pEdge2);

			//ʵʱ����
			if(pDC != NULL)	DrawEdge(pDC,pEdge2);
		}

		//������������
		{
			CDCR_TinEdge*	pEdge3;
			//��������
			pEdge3						= new	CDCR_TinEdge;
			pEdge3->m_nID				= 3;
			pEdge3->m_nVertexID[0]		= iThirdVertexID;
			pEdge3->m_nVertexID[1]		= iSecondVertexID;	
			pEdge3->m_nTriangleID[0]	= 1;
			pEdge3->m_pLeftTriangle		= pTriangle;
			pEdge3->m_iUsedCount++;
			
			//
			AddEdge(pEdge3);
			//���ӵ�������
			AddEdgeToCells((dX2+dX3)/2,(dY2+dY3)/2,pEdge3);

			//ʵʱ����
			if(pDC != NULL)	DrawEdge(pDC,pEdge3);
		}
	}

	//��ʼ��չ�����Σ��������γ����е�������
	//
	{
		CDCR_TinTriangle	*pNextTriangle;

		pNextTriangle	= m_pTriangleHead;

		if(pNextTriangle == NULL)	return	false;

		do
		{
			for(int I=0;I<3;I++)
			{
				if(pNextTriangle->m_bEdgeUsed[I] == false)
				{
					int		iVertexID1,iVertexID2,iVertexID3;
					//��ȡ��һ���ڶ�����������������
					double	dX1,dY1,dZ1,dX2,dY2,dZ2,dX3,dY3,dZ3;

					//�����һ����û�д�������ָ����0-1
					//0-1
					if(I==0)
					{
						//��ȡ���ߵĶ���ID
						iVertexID1	= pNextTriangle->m_nVertexID[0];
						dX1			= pNextTriangle->m_dX[0];
						dY1			= pNextTriangle->m_dY[0];
						dZ1			= pNextTriangle->m_dZ[0];

						iVertexID2	= pNextTriangle->m_nVertexID[1];
						dX2			= pNextTriangle->m_dX[1];
						dY2			= pNextTriangle->m_dY[1];
						dZ2			= pNextTriangle->m_dZ[1];

						//��ȡ�������еĵ���������
						iVertexID3	= pNextTriangle->m_nVertexID[2];
						dX3			= pNextTriangle->m_dX[2];
						dY3			= pNextTriangle->m_dY[2];
						dZ3			= pNextTriangle->m_dZ[2];
					}

					//0-2
					if(I==1)
					{
						//��ȡ���ߵĶ���ID
						iVertexID1	= pNextTriangle->m_nVertexID[0];
						dX1			= pNextTriangle->m_dX[0];
						dY1			= pNextTriangle->m_dY[0];
						dZ1			= pNextTriangle->m_dZ[0];

						iVertexID2	= pNextTriangle->m_nVertexID[2];
						dX2			= pNextTriangle->m_dX[2];
						dY2			= pNextTriangle->m_dY[2];
						dZ2			= pNextTriangle->m_dZ[2];

						//��ȡ�������еĵ���������
						iVertexID3	= pNextTriangle->m_nVertexID[1];
						dX3			= pNextTriangle->m_dX[1];
						dY3			= pNextTriangle->m_dY[1];
						dZ3			= pNextTriangle->m_dZ[1];
					}

					//2-1
					if(I==2)
					{
						//��ȡ���ߵĶ���ID
						iVertexID1	= pNextTriangle->m_nVertexID[2];
						dX1			= pNextTriangle->m_dX[2];
						dY1			= pNextTriangle->m_dY[2];
						dZ1			= pNextTriangle->m_dZ[2];

						iVertexID2	= pNextTriangle->m_nVertexID[1];
						dX2			= pNextTriangle->m_dX[1];
						dY2			= pNextTriangle->m_dY[1];
						dZ2			= pNextTriangle->m_dZ[1];

						//��ȡ�������еĵ���������
						iVertexID3	= pNextTriangle->m_nVertexID[0];
						dX3			= pNextTriangle->m_dX[0];
						dY3			= pNextTriangle->m_dY[0];
						dZ3			= pNextTriangle->m_dZ[0];
					}

					//��Ҫ�ٴ��ж�
					//�����Ǳ���Ҫ					
					{
						CDCR_TinEdge	*pEdge	= NULL;
						pEdge	= GetEdgeFromCells((dX1+dX2)/2,(dY1+dY2)/2,iVertexID1,iVertexID2);

						if(pEdge==NULL || pEdge->m_iUsedCount<2)
							;
						else
							continue;
					}

					//������Ѱ���µĶ��㣬Ѱ���Ķ���������ã��������ã���Ҫ�ж�
					//{
						//����(dX1,dY1)��(dX2,dY2)�����жϺ���
						double	a,b;
						int		nSign;

						nSign			= CreateJudgeFunction(dX1,dY1,dX2,dY2,a,b);
						
						//��ȡ�Ե��λ��
						double	dVertexValue;
						dVertexValue	= GetJudgeValue(dX3,dY3,nSign,a,b);
						
						//Ѱ���Ž����ĵ�
						bool			bFirstCosValue;
						double			dMaxCosValue;
						CDCR_TinVertex	*pNewVertex;
						
						bFirstCosValue	= true;
						dMaxCosValue	= 0.0;
						pNewVertex		= NULL;

						//���������������ų���ͬһ�ߣ��Ӳ���ͬһ�ߵĵ���Ѱ��
						int				iC;
						//����������Ľǵ�
						double			dCellX1,dCellY1;
						double			dCellX2,dCellY2;
						double			dCellX3,dCellY3;
						double			dCellX4,dCellY4;

						for(int j=0;j<m_nCellNumOfY;j++)
						{
							for(int	i=0;i<m_nCellNumOfX;i++)
							{
								iC	= j*m_nCellNumOfX + i;
																
								//����
								dCellX1	= m_dCellLeft + m_dCellSizeOfX*i;
								dCellY1	= m_dCellBottom + m_dCellSizeOfY*j;

								//����
								dCellX2	= dCellX1 + m_dCellSizeOfX;
								dCellY2	= dCellY1;
								
								//����
								dCellX3	= dCellX2;
								dCellY3	= dCellY1 + m_dCellSizeOfY;
								
								//����
								dCellX4	= dCellX1;
								dCellY4	= dCellY3;

								//����ĸ��ǵ㶼�ͣ�dX��dY������(xx1,yy1)��(xx2,yy2)��ͬһ�ߵģ����ü���
								bool	bDealWith;
								double	dTempVertexValue;

								bDealWith	= false;

								//��dX,dY���� (xx1,yy1)��(xx2,yy2)�����
								dTempVertexValue	= GetJudgeValue(dCellX1,dCellY1,nSign,a,b);
								if(dVertexValue>0&&dTempVertexValue<0 || dVertexValue<0&&dTempVertexValue>0)
								{
									bDealWith	= true;
								}

								//
								if(!bDealWith)
								{
									dTempVertexValue	= GetJudgeValue(dCellX2,dCellY2,nSign,a,b);
									if(dVertexValue>0&&dTempVertexValue<0 || dVertexValue<0&&dTempVertexValue>0)
									{
										bDealWith	= true;
									}
								}

								if(!bDealWith)
								{
									//
									dTempVertexValue	= GetJudgeValue(dCellX3,dCellY3,nSign,a,b);
									if(dVertexValue>0&&dTempVertexValue<0 || dVertexValue<0&&dTempVertexValue>0)
									{
										bDealWith	= true;
									}
								}

								if(!bDealWith)
								{
									//
									dTempVertexValue	= GetJudgeValue(dCellX4,dCellY4,nSign,a,b);
									if(dVertexValue>0&&dTempVertexValue<0 || dVertexValue<0&&dTempVertexValue>0)
									{
										bDealWith	= true;
									}
								}

								//��Ҫ����
								if(bDealWith)
								{
									CDCR_TinVertexAddress	*pTVD;
									
									pTVD	= m_pCells[iC].m_pTVDHead;
									while(pTVD != NULL)
									{
										//�����ų�iVertexID1��iVertexID2��iVertexID3��������
										//���Ǳ����!
										if(pTVD->m_pVertex->m_nID!=iVertexID1 && 
											pTVD->m_pVertex->m_nID!=iVertexID2 && 
											pTVD->m_pVertex->m_nID!=iVertexID3)
										{
											//�����ж�
											double	dTempCosValue;
											double	dTempXX,dTempYY,dTempZZ;

											dTempXX	= pTVD->m_pVertex->m_xx;
											dTempYY	= pTVD->m_pVertex->m_yy;
											dTempZZ	= pTVD->m_pVertex->m_zz;

											dTempVertexValue	= GetJudgeValue(dTempXX,dTempYY,nSign,a,b);

											//��nVertexID�� (xx1,yy1)��(xx2,yy2)�����
											//����0���������ߣ�ȡ��
											if(fabs(dTempVertexValue) < 1e-10)
												;
											else
											if(dVertexValue>0&&dTempVertexValue<0 || dVertexValue<0&&dTempVertexValue>0)
											{
												//
												dTempCosValue		= GetCosValue(dTempXX,dTempYY,dTempZZ,dX1,dY1,dZ1,dX2,dY2,dZ2);
												
												if(bFirstCosValue)
												{
													dMaxCosValue	= dTempCosValue;
													pNewVertex		= pTVD->m_pVertex;
													bFirstCosValue	= false;
												}
												else
												{
													if(dMaxCosValue > dTempCosValue)	//���ŽǴ��,��ֵС��
													{
														dMaxCosValue	= dTempCosValue;
														pNewVertex		= pTVD->m_pVertex;
													}
												}
											}
										}

										pTVD	= pTVD->m_pNext;
									}//while(pTVD != NULL)

								}//if(bDealWith)
							
							}//for(int	i=0;i<m_nCellNumOfX;i++)

						}//for(int j=0;j<m_nCellNumOfY;j++)

						//CString	szInfo;
						//szInfo.Format("%lf\n",dMaxCosValue);
						//TRACE(szInfo);

						if(fabs(dMaxCosValue-1) < 1e-5)	
						{
							dMaxCosValue	= 0.0;
							pNewVertex		= NULL;
						}
						
					//}
					
					{
						//�ں͵��������Ӧ��һ���ҵ��Ž����ĵ���
						//������������ĵ㣬�򹹽��µ������Ρ�
						if(pNewVertex != NULL)
						{
							//�µĶ����
							int		iNewVertexID;
							double	dNewX,dNewY,dNewZ;

							iNewVertexID	= pNewVertex->m_nID;
							dNewX			= pNewVertex->m_xx;
							dNewY			= pNewVertex->m_yy;
							dNewZ			= pNewVertex->m_zz;

							//��Ҫ�жϣ�iVertexID1,nNewVertexID���ͣ�nNewVertexID,iVertexID2����ɵı��Ѿ���ʹ������
							CDCR_TinEdge	*pEdge1	= NULL;
							CDCR_TinEdge	*pEdge2	= NULL;
							CDCR_TinEdge	*pEdge3	= NULL;

							//�����ж����¶�����ɵı��Ƿ���ڻ���������
							//�����ڣ��϶����±�
							//���ڵ��ǽ���ʹ��һ�Σ��������Լ���ʹ��
							//���ڵ����Ѿ�ʹ�����Σ����������������
							pEdge1	= GetEdgeFromCells((dX1+dNewX)/2,(dY1+dNewY)/2,iVertexID1,iNewVertexID);
							//if(!IsEdgeUsedTwice(iVertexID1,iNewVertexID))
							if(pEdge1==NULL || pEdge1->m_iUsedCount<2)
							{
								//ͬ��
								pEdge2	= GetEdgeFromCells((dX2+dNewX)/2,(dY2+dNewY)/2,iVertexID2,iNewVertexID);
								//if(!IsEdgeUsedTwice(iNewVertexID,iVertexID2))
								if(pEdge2==NULL || pEdge2->m_iUsedCount<2)
								{
									//�µ�������
									int					nNewTriangleID;
									CDCR_TinTriangle	*pNewTriangle;
									
									//�����µ�������
									{
										nNewTriangleID					= m_iTriangleCount+1;

										pNewTriangle					= new	CDCR_TinTriangle;
										pNewTriangle->m_nID				= nNewTriangleID;

										pNewTriangle->m_nVertexID[0]	= iVertexID1;
										pNewTriangle->m_nVertexID[1]	= iVertexID2;
										pNewTriangle->m_nVertexID[2]	= iNewVertexID;
										//����Ҫ�ж�pNewTriangle��iVertexID1��iVertexID2��ɵı��ǲ���Ҫ�жϵ�
										pNewTriangle->m_bEdgeUsed[0]	= true;

										AddTriangle(pNewTriangle);
									
										//���������ε�����
										pNewTriangle->SetVertexCoord(iVertexID1,dX1,dY1,dZ1,iVertexID2,dX2,dY2,dZ2,iNewVertexID,dNewX,dNewY,dNewZ);
									}

									//����ԭ���ı߽��в���
									pEdge3	= GetEdgeFromCells((dX1+dX2)/2,(dY1+dY2)/2,iVertexID1,iVertexID2);
									//pEdge	= GetEdgeWithVertex(iVertexID1,iVertexID2);	//pEdge1
									if(pEdge3 != NULL)
									{
										pEdge3->m_nTriangleID[1]	= nNewTriangleID;

										pEdge3->m_iUsedCount++;

										//����pEdge3����Ӧ�ıߵ������û������ģ���Ϊ��
										//pNextTriangle->m_bEdgeUsed[I] = true;�ḳ��ʹ�ñ�־
									}

									//���ж��Ƿ��Ѿ����������ı����Ѿ��������������
									//pEdge	= GetEdgeWithVertex(iVertexID1,iNewVertexID);//pEdge3
									if(pEdge1 == NULL)
									{
										pEdge1						= new	CDCR_TinEdge;
										pEdge1->m_nID				= m_iEdgeCount+1;

										pEdge1->m_nVertexID[0]		= iVertexID1;
										pEdge1->m_nVertexID[1]		= iNewVertexID;	
										pEdge1->m_nTriangleID[0]	= nNewTriangleID;
										pEdge1->m_pLeftTriangle		= pNewTriangle;
										pEdge1->m_iUsedCount++;
										
										//
										AddEdge(pEdge1);
										//
										AddEdgeToCells((dX1+dNewX)/2,(dY1+dNewY)/2,pEdge1);
					
										//ʵʱ����
										if(pDC != NULL)		DrawEdge(pDC,pEdge1);
									}
									else
									{
										pEdge1->m_nTriangleID[1]	= nNewTriangleID;
										pEdge1->m_pRightTriangle	= pNewTriangle;

										pEdge1->m_iUsedCount++;

										/*
										if(pEdge1->m_iUsedCount >= 2)
										{
											//����ܻ�������⡣��Ϊ��������//pNextTriangle->m_bEdgeUsed[I] = true;�ḳ��ʹ�ñ�־���Ĳ���
											//��˱���Ҫ�Լ��ж�
											if(pEdge1->m_pLeftTriangle != NULL)
											{
												//�����������ù�������������������������ɵ���һ��������Ӧ����Ҫ����Ϊ���õġ�
												pEdge1->m_pLeftTriangle->UpdateEdge(iVertexID1,iNewVertexID);
											}

											if(pEdge1->m_pRightTriangle != NULL)
											{
												pEdge1->m_pRightTriangle->UpdateEdge(iVertexID1,iNewVertexID);
											}
										}
										*/
									}

									//pEdge	= GetEdgeWithVertex(iVertexID2,iNewVertexID);//pEdge2
									if(pEdge2 == NULL)
									{
										pEdge2						= new	CDCR_TinEdge;
										pEdge2->m_nID				= m_iEdgeCount+1;

										pEdge2->m_nVertexID[0]		= iVertexID2;
										pEdge2->m_nVertexID[1]		= iNewVertexID;	
										pEdge2->m_nTriangleID[0]	= nNewTriangleID;
										pEdge2->m_pLeftTriangle		= pNewTriangle;
										pEdge2->m_iUsedCount++;

										//
										AddEdge(pEdge2);
										//
										AddEdgeToCells((dX2+dNewX)/2,(dY2+dNewY)/2,pEdge2);
					
										//ʵʱ����
										if(pDC != NULL)		DrawEdge(pDC,pEdge2);
									}
									else
									{
										pEdge2->m_nTriangleID[1]	= nNewTriangleID;
										pEdge2->m_pRightTriangle	= pNewTriangle;

										pEdge2->m_iUsedCount++;

										/*
										if(pEdge2->m_iUsedCount >= 2)
										{
											//����ܻ�������⡣��Ϊ��������//pNextTriangle->m_bEdgeUsed[I] = true;�ḳ��ʹ�ñ�־���Ĳ���
											//��˱���Ҫ�Լ��ж�
											if(pEdge2->m_pLeftTriangle != NULL)
											{
												//�����������ù�������������������������ɵ���һ��������Ӧ����Ҫ����Ϊ���õġ�
												pEdge2->m_pLeftTriangle->UpdateEdge(iVertexID2,iNewVertexID);
											}
											if(pEdge2->m_pRightTriangle != NULL)
											{
												pEdge2->m_pRightTriangle->UpdateEdge(iVertexID2,iNewVertexID);
											}
										}
										*/
									}
								
								}//�ڶ������Ƿ�ʹ�����ε��ж�

							}//��һ�����Ƿ�ʹ�����ε��ж�

						}//if(pTempVertex != NULL)
					}

					//�ù���
					pNextTriangle->m_bEdgeUsed[I] = true;

				}//if(pTempTriangle->m_bUsed[I] == false)

			}//for(int I=0;I<3;I++)
			
			//�����ж��������ε���Ϣ
			//pNextTriangle	= GetUnUsedTriangle();
			pNextTriangle	= pNextTriangle->m_pNext;
		}while(pNextTriangle != NULL);
	}
	
	return	true;	
}

//û�о����Ż��ĳ���
/*
bool CDCR_TinTDTACreator::Create(CDC *pDC)
{
	//���Ƚ�����������
	CellBuild();

	//���ͷ�,�ߺ�������
	{
		//
		ReleaseEdge();

		//
		ReleaseTriangle();
	}

	//������һ��������
	{
		int	iFirstVertexID,iSecondVertexID,iThirdVertexID;
		
		iFirstVertexID	= CreateFirstVertex();
		
		iSecondVertexID	= CreateSecondVertex(iFirstVertexID);
		if(iSecondVertexID < 0)	
		{
			MessageBox(NULL,"Tin�������ɹ�!","��ʾ��Ϣ",MB_OK);
			return	false;
		}

		iThirdVertexID	= CreateThirdVertex(iFirstVertexID,iSecondVertexID);
		if(iThirdVertexID < 0)	
		{
			MessageBox(NULL,"Tin�������ɹ�!","��ʾ��Ϣ",MB_OK);
			return	false;
		}

		double	dX1,dY1,dZ1,dX2,dY2,dZ2,dX3,dY3,dZ3;

		GetVertexCoordFromID(iFirstVertexID,dX1,dY1,dZ1);
		GetVertexCoordFromID(iSecondVertexID,dX2,dY2,dZ2);
		GetVertexCoordFromID(iThirdVertexID,dX3,dY3,dZ3);

		CDCR_TinEdge*	pEdge;
		//��һ����
		pEdge						= new	CDCR_TinEdge;
		pEdge->m_nID				= 1;
		pEdge->m_nVertexID[0]		= iFirstVertexID;
		pEdge->m_nVertexID[1]		= iSecondVertexID;
		pEdge->m_nTriangleID[0]		= 1;
		pEdge->m_iUsedCount++;
		
		//
		AddEdge(pEdge);
		//���ӵ�������
		AddEdgeToCells((dX1+dX2)/2,(dY1+dY2)/2,pEdge);

		//ʵʱ����
		DrawEdge(pDC,pEdge);
		
		//�ڶ�����
		pEdge						= new	CDCR_TinEdge;
		pEdge->m_nID				= 2;
		pEdge->m_nVertexID[0]		= iFirstVertexID;
		pEdge->m_nVertexID[1]		= iThirdVertexID;
		pEdge->m_nTriangleID[0]		= 1;
		pEdge->m_iUsedCount++;

		//
		AddEdge(pEdge);
		//���ӵ�������
		AddEdgeToCells((dX1+dX3)/2,(dY1+dY3)/2,pEdge);

		//ʵʱ����
		DrawEdge(pDC,pEdge);

		//��������
		pEdge						= new	CDCR_TinEdge;
		pEdge->m_nID				= 3;
		pEdge->m_nVertexID[0]		= iThirdVertexID;
		pEdge->m_nVertexID[1]		= iSecondVertexID;	
		pEdge->m_nTriangleID[0]		= 1;
		pEdge->m_iUsedCount++;
		
		//
		AddEdge(pEdge);
		//���ӵ�������
		AddEdgeToCells((dX2+dX3)/2,(dY2+dY3)/2,pEdge);

		//ʵʱ����
		DrawEdge(pDC,pEdge);
		
		//��һ��������
		CDCR_TinTriangle*	pTriangle;
		pTriangle					= new	CDCR_TinTriangle;
		pTriangle->m_nID			= 1;
		pTriangle->m_nVertexID[0]	= iFirstVertexID;
		pTriangle->m_nVertexID[1]	= iSecondVertexID;
		pTriangle->m_nVertexID[2]	= iThirdVertexID;
		AddTriangle(pTriangle);
		
		//��������
		pTriangle->SetVertexCoord(iFirstVertexID,dX1,dY1,dZ1,iSecondVertexID,dX2,dY2,dZ2,iThirdVertexID,dX3,dY3,dZ3);
	}

	//��ʼ��չ�����Σ��������γ����е�������
	//
	{
		//CDCR_TinEdge		*pEdge;
		CDCR_TinTriangle	*pNextTriangle;

		//pEdge			= NULL;
		//pNextTriangle	= GetUnUsedTriangle();
		pNextTriangle	= m_pTriangleHead;

		do
		{
			for(int I=0;I<3;I++)
			{
				if(pNextTriangle->m_bEdgeUsed[I] == false)
				{
					int		iVertexID1,iVertexID2,iVertexID3;
					//��ȡ��һ���ڶ�����������������
					double	dX1,dY1,dZ1,dX2,dY2,dZ2,dX3,dY3,dZ3;

					//�����һ����û�д�������ָ����0-1
					//0-1
					if(I==0)
					{
						//��ȡ���ߵĶ���ID
						iVertexID1	= pNextTriangle->m_nVertexID[0];
						dX1			= pNextTriangle->m_dX[0];
						dY1			= pNextTriangle->m_dY[0];
						dZ1			= pNextTriangle->m_dZ[0];

						iVertexID2	= pNextTriangle->m_nVertexID[1];
						dX2			= pNextTriangle->m_dX[1];
						dY2			= pNextTriangle->m_dY[1];
						dZ2			= pNextTriangle->m_dZ[1];

						//��ȡ�������еĵ���������
						iVertexID3	= pNextTriangle->m_nVertexID[2];
						dX3			= pNextTriangle->m_dX[2];
						dY3			= pNextTriangle->m_dY[2];
						dZ3			= pNextTriangle->m_dZ[2];
					}

					//0-2
					if(I==1)
					{
						//��ȡ���ߵĶ���ID
						iVertexID1	= pNextTriangle->m_nVertexID[0];
						dX1			= pNextTriangle->m_dX[0];
						dY1			= pNextTriangle->m_dY[0];
						dZ1			= pNextTriangle->m_dZ[0];

						iVertexID2	= pNextTriangle->m_nVertexID[2];
						dX2			= pNextTriangle->m_dX[2];
						dY2			= pNextTriangle->m_dY[2];
						dZ2			= pNextTriangle->m_dZ[2];

						//��ȡ�������еĵ���������
						iVertexID3	= pNextTriangle->m_nVertexID[1];
						dX3			= pNextTriangle->m_dX[1];
						dY3			= pNextTriangle->m_dY[1];
						dZ3			= pNextTriangle->m_dZ[1];
					}

					//2-1
					if(I==2)
					{
						//��ȡ���ߵĶ���ID
						iVertexID1	= pNextTriangle->m_nVertexID[2];
						dX1			= pNextTriangle->m_dX[2];
						dY1			= pNextTriangle->m_dY[2];
						dZ1			= pNextTriangle->m_dZ[2];

						iVertexID2	= pNextTriangle->m_nVertexID[1];
						dX2			= pNextTriangle->m_dX[1];
						dY2			= pNextTriangle->m_dY[1];
						dZ2			= pNextTriangle->m_dZ[1];

						//��ȡ�������еĵ���������
						iVertexID3	= pNextTriangle->m_nVertexID[0];
						dX3			= pNextTriangle->m_dX[0];
						dY3			= pNextTriangle->m_dY[0];
						dZ3			= pNextTriangle->m_dZ[0];
					}

					//����(dX1,dY1)��(dX2,dY2)�����жϺ���
					double	a,b;
					int		nSign;

					nSign			= CreateJudgeFunction(dX1,dY1,dX2,dY2,a,b);
					
					//��ȡ�Ե��λ��
					double	dVertexValue;
					dVertexValue	= GetJudgeValue(dX3,dY3,nSign,a,b);
					
					//����û���Ż��ĳ���
					
					//Ѱ���Ž����ĵ�
					bool	bCosValue;
					double	dCosValue;
					CDCR_TinVertex	*pVertex;
					CDCR_TinVertex	*pTempVertex;
					
					bCosValue	= true;
					dCosValue	= 0.0;
					pTempVertex	= NULL;

					pVertex		= m_pVertexHead;
					while(pVertex != NULL)
					{
						//�����ų�iVertexID��iVertexID1��iVertexID2��������
						//���Ǳ����!
						if(pVertex->m_nID!=iVertexID1 && 
							pVertex->m_nID!=iVertexID2 && 
							pVertex->m_nID!=iVertexID3)
						{
							//�����ж�
							double	dTempValue;
							double	dTempXX,dTempYY,dTempZZ;
							double	dTempVertexValue;

							dTempXX	= pVertex->m_xx;
							dTempYY	= pVertex->m_yy;
							dTempZZ	= pVertex->m_zz;

							dTempVertexValue	= GetJudgeValue(dTempXX,dTempYY,nSign,a,b);

							//��nVertexID�� (xx1,yy1)��(xx2,yy2)�����
							if(dVertexValue*dTempVertexValue < 0)
							{
								//
								dTempValue		= GetCosValue(dTempXX,dTempYY,dTempZZ,dX1,dY1,dZ1,dX2,dY2,dZ2);

								if(bCosValue)
								{
									dCosValue	= dTempValue;
									pTempVertex	= pVertex;
									bCosValue	= false;
								}
								else
								{
									if(dCosValue > dTempValue)	//���ŽǴ��,��ֵС��
									{
										dCosValue	= dTempValue;
										pTempVertex	= pVertex;
									}
								}
							}
						}

						pVertex	= pVertex->m_pNext;
					}//while(pVertex != NULL)
					
				
					{
						//�ں͵��������Ӧ��һ���ҵ��Ž����ĵ���
						//������������ĵ㣬�򹹽��µ������Ρ�
						if(pTempVertex != NULL)
						{
							//�µĶ����
							int		iNewVertexID;
							double	dNewX,dNewY,dNewZ;

							iNewVertexID	= pTempVertex->m_nID;
							dNewX			= pTempVertex->m_xx;
							dNewY			= pTempVertex->m_yy;
							dNewZ			= pTempVertex->m_zz;

							//��Ҫ�жϣ�iVertexID1,nNewVertexID���ͣ�nNewVertexID,iVertexID2����ɵı��Ѿ���ʹ������

							//���˵�һ���ж�
							//pEdge1	= GetEdgeFromCells((dX1+dNewX)/2,(dY1+dNewY)/2,iVertexID1,iNewVertexID);
							if(!IsEdgeUsedTwice(iVertexID1,iNewVertexID))
							//if(pEdge1==NULL || pEdge1->m_iUsedCount<2)
							{
								//pEdge2	= GetEdgeFromCells((dX2+dNewX)/2,(dY2+dNewY)/2,iVertexID2,iNewVertexID);
								if(!IsEdgeUsedTwice(iNewVertexID,iVertexID2))
								//if(pEdge2==NULL || pEdge2->m_iUsedCount<2)
								{
									CDCR_TinEdge	*pEdge;

									//�µ�������
									int	nNewTriangleID;
									nNewTriangleID	= m_iTriangleCount+1;

									//����ԭ���ı߽��в���
									//pEdge3	= GetEdgeFromCells((dX1+dX2)/2,(dY1+dY2)/2,iVertexID1,iVertexID2);
									pEdge	= GetEdgeWithVertex(iVertexID1,iVertexID2);	//pEdge1
									if(pEdge != NULL)
									{
										pEdge->m_nTriangleID[1]	= nNewTriangleID;

										pEdge->m_iUsedCount++;
									}

									//���ж��Ƿ��Ѿ����������ı����Ѿ��������������
									pEdge	= GetEdgeWithVertex(iVertexID1,iNewVertexID);//pEdge3
									if(pEdge == NULL)
									{
										pEdge						= new	CDCR_TinEdge;
										pEdge->m_nID				= m_iEdgeCount+1;

										pEdge->m_nVertexID[0]		= iVertexID1;
										pEdge->m_nVertexID[1]		= iNewVertexID;	
										pEdge->m_nTriangleID[0]	= nNewTriangleID;
										pEdge->m_iUsedCount++;
										
										//
										AddEdge(pEdge);
										//
										//AddEdgeToCells((dX1+dNewX)/2,(dY1+dNewY)/2,pEdge1);
					
										//ʵʱ����
										DrawEdge(pDC,pEdge);
									}
									else
									{
										pEdge->m_nTriangleID[1]	= nNewTriangleID;

										pEdge->m_iUsedCount++;
									}

									pEdge	= GetEdgeWithVertex(iVertexID2,iNewVertexID);//pEdge2
									if(pEdge == NULL)
									{
										pEdge						= new	CDCR_TinEdge;
										pEdge->m_nID				= m_iEdgeCount+1;

										pEdge->m_nVertexID[0]		= iVertexID2;
										pEdge->m_nVertexID[1]		= iNewVertexID;	
										pEdge->m_nTriangleID[0]	= nNewTriangleID;
										pEdge->m_iUsedCount++;

										//
										AddEdge(pEdge);
										//
										//AddEdgeToCells((dX2+dNewX)/2,(dY2+dNewY)/2,pEdge2);
					
										//ʵʱ����
										DrawEdge(pDC,pEdge);
									}
									else
									{
										pEdge->m_nTriangleID[1]	= nNewTriangleID;

										pEdge->m_iUsedCount++;
									}

									//�����µ�������
									CDCR_TinTriangle	*pNewTriangle;

									pNewTriangle				= new	CDCR_TinTriangle;
									pNewTriangle->m_nID			= nNewTriangleID;

									pNewTriangle->m_nVertexID[0]	= iVertexID1;
									pNewTriangle->m_nVertexID[1]	= iVertexID2;
									pNewTriangle->m_nVertexID[2]	= iNewVertexID;
									//����Ҫ�ж�pNewTriangle��iVertexID1��iVertexID2��ɵı��ǲ���Ҫ�жϵ�
									pNewTriangle->m_bEdgeUsed[0]	= true;

									AddTriangle(pNewTriangle);
								
									//���������ε�����
									pNewTriangle->SetVertexCoord(iVertexID1,dX1,dY1,dZ1,iVertexID2,dX2,dY2,dZ2,iNewVertexID,dNewX,dNewY,dNewZ);
								
								}//�ڶ������Ƿ�ʹ�����ε��ж�

							}//��һ�����Ƿ�ʹ�����ε��ж�

						}//if(pTempVertex != NULL)
					}

					//�ù���
					pNextTriangle->m_bEdgeUsed[I] = true;

				}//if(pTempTriangle->m_bUsed[I] == false)

			}//for(int I=0;I<3;I++)
			
			//�����ж��������ε���Ϣ
			//pNextTriangle	= GetUnUsedTriangle();
			pNextTriangle	= pNextTriangle->m_pNext;
		}while(pNextTriangle != NULL);
	}
	
	return	true;	
}
*/
//////////////////////////////////////////////////////////////////////
// 001 �ͷ�TIN���ݼ�����������
//////////////////////////////////////////////////////////////////////
void CDCR_TinTDTACreator::Release( )
{
	//
	ReleaseVertex();

	//
	ReleaseEdge();

	//
	ReleaseTriangle();
}

//////////////////////////////////////////////////////////////////////
// 002 ���Ӷ�������
//////////////////////////////////////////////////////////////////////
void CDCR_TinTDTACreator::AddVertex(double dX,double dY,double dZ)
{
	CDCR_TinVertex*	pVertex;

	m_iVertexCount++;

	pVertex	= new	CDCR_TinVertex;
	pVertex->Add(m_iVertexCount,dX,dY,dZ);
		
	if(m_pVertexHead == NULL)
	{
		m_pVertexHead	= pVertex;
		m_pVertexLast	= pVertex;
	}
	else
	{	
		m_pVertexLast->m_pNext	= pVertex;
		m_pVertexLast			= pVertex;
	}
}

//////////////////////////////////////////////////////////////////////
// �����������Ӷ���
//////////////////////////////////////////////////////////////////////
bool CDCR_TinTDTACreator::AddVertexToCells(CDCR_TinVertex *pVertex)
{
	if(pVertex == NULL)				return	false;

	int							ci;	
	int							xi,yi;
	CDCR_TinVertexAddress		*tvd;
		
	//��������һ����
	xi	= (int)((pVertex->m_xx - m_dCellLeft) / m_dCellSizeOfX);
	yi	= (int)((pVertex->m_yy - m_dCellBottom) / m_dCellSizeOfY);

	if(xi < 0)				xi	= 0;
	if(yi < 0)				yi	= 0;
	if(xi >= m_nCellNumOfX)	xi	= m_nCellNumOfX-1;
	if(yi >= m_nCellNumOfY)	yi	= m_nCellNumOfY-1;

	ci	= yi * m_nCellNumOfX + xi;

	if(m_pCells[ci].m_pTVDHead == NULL)
	{
		tvd							= new	CDCR_TinVertexAddress;	
		tvd->m_pVertex				= pVertex;

		m_pCells[ci].m_pTVDHead		= tvd;
		m_pCells[ci].m_pTVDLast		= tvd;
	}
	else
	{
		tvd							= new	CDCR_TinVertexAddress;	
		tvd->m_pVertex				= pVertex;

		m_pCells[ci].m_pTVDLast->m_pNext	= tvd;
		m_pCells[ci].m_pTVDLast				= tvd;
	}

	return	true;
}

//�������ӵ�����������
bool CDCR_TinTDTACreator::AddEdgeToCells(double dX,double dY,CDCR_TinEdge *pEdge)
{
	if(pEdge == NULL)				return	false;

	int							ci;	
	int							xi,yi;
	CDCR_TinEdgeAddress			*pTED;
		
	//��������һ����
	xi	= (int)((dX - m_dCellLeft) / m_dCellSizeOfX);
	yi	= (int)((dY - m_dCellBottom) / m_dCellSizeOfY);

	if(xi < 0)				xi	= 0;
	if(yi < 0)				yi	= 0;
	if(xi >= m_nCellNumOfX)	xi	= m_nCellNumOfX-1;
	if(yi >= m_nCellNumOfY)	yi	= m_nCellNumOfY-1;

	ci	= yi * m_nCellNumOfX + xi;

	if(m_pCells[ci].m_pTEDHead == NULL)
	{
		pTED						= new	CDCR_TinEdgeAddress;	
		pTED->m_pEdge				= pEdge;

		m_pCells[ci].m_pTEDHead		= pTED;
		m_pCells[ci].m_pTEDLast		= pTED;
	}
	else
	{
		//bool	bFinded;
		//bFinded	= false;

		//pTED	= m_pCells[ci].m_pTEDHead;
		//while(pTED != NULL)
		//{
		//	if(pTED->m_pEdge == pEdge)
		//	{
		//		bFinded	= true;
		//		break;
		//	}

		//	pTED	= pTED->m_pNext;
		//}

		//if(bFinded)
		{
			pTED						= new	CDCR_TinEdgeAddress;	
			pTED->m_pEdge				= pEdge;

			m_pCells[ci].m_pTEDLast->m_pNext	= pTED;
			m_pCells[ci].m_pTEDLast				= pTED;
		}
	}

	return	true;	
}

//////////////////////////////////////////////////////////////////////
// 004 ������ʾ����
//////////////////////////////////////////////////////////////////////
void CDCR_TinTDTACreator::SetCoordMapping(double dXA,double dXB,double dYA,double dYB)
{
	m_dMapXA	= dXA;
	m_dMapXB	= dXB;

	m_dMapYA	= dYA;
	m_dMapYB	= dYB;
}

//
void CDCR_TinTDTACreator::SetClientRect(CRect rRect)
{
	m_rRect	= rRect;
}
//////////////////////////////////////////////////////////////////////
// 005 ���Ʊ�
//////////////////////////////////////////////////////////////////////
void CDCR_TinTDTACreator::DrawEdge(CDC* pDC,CDCR_TinEdge *pEdge)
{
	if(pEdge != NULL)
	{
		int	nVertexID1,nVertexID2;
		nVertexID1	= pEdge->m_nVertexID[0];
		nVertexID2	= pEdge->m_nVertexID[1];

		double	dX1,dY1,dZ1;
		double	dX2,dY2,dZ2;

		GetVertexCoordFromID(nVertexID1,dX1,dY1,dZ1);
		GetVertexCoordFromID(nVertexID2,dX2,dY2,dZ2);
		
		POINT	ptPoints[2];

		ptPoints[0].x	= (int)(dX1*m_dMapXA + m_dMapXB + 0.5);
		ptPoints[0].y	= (int)(dY1*m_dMapYA + m_dMapYB + 0.5);

		ptPoints[1].x	= (int)(dX2*m_dMapXA + m_dMapXB + 0.5);
		ptPoints[1].y	= (int)(dY2*m_dMapYA + m_dMapYB + 0.5);
		
		pDC->Polyline(ptPoints,2);
	}
}

//////////////////////////////////////////////////////////////////////
// 006 ����TIN
//////////////////////////////////////////////////////////////////////
void CDCR_TinTDTACreator::Draw(CDC *pDC)
{
	CDCR_TinTriangle	*pTriangle;

	pTriangle	= m_pTriangleHead;

	while(pTriangle != NULL)
	{
		POINT	ptPoints[4];
		for(int j=0;j<3;j++)
		{
			ptPoints[j].x	= (int)(pTriangle->m_dX[j]*m_dMapXA + m_dMapXB + 0.5);
			ptPoints[j].y	= (int)(pTriangle->m_dY[j]*m_dMapYA + m_dMapYB + 0.5);
		}
		ptPoints[3]	= ptPoints[0];

		pDC->Polyline(ptPoints,4);

		pTriangle	= pTriangle->m_pNext;
	}
}

//////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////
void CDCR_TinTDTACreator::ReleaseEdge()
{
	CDCR_TinEdge	*pEdge1,*pEdge2;

	if(m_pEdgeHead != NULL)
	{
		pEdge1	= pEdge2	= m_pEdgeHead;
		do
		{
			pEdge1		= pEdge1->m_pNext;
			delete	pEdge2;
			pEdge2		= pEdge1;
		}while(pEdge1 != NULL);

		m_pEdgeHead	= NULL;
		m_pEdgeLast	= NULL;
	}	
}

//////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////
void CDCR_TinTDTACreator::ReleaseTriangle()
{
	CDCR_TinTriangle	*pTriangle1,*pTriangle2;

	if(m_pTriangleHead != NULL)
	{
		pTriangle1	= pTriangle2	= m_pTriangleHead;
		do
		{
			pTriangle1		= pTriangle1->m_pNext;
			delete	pTriangle2;
			pTriangle2		= pTriangle1;
		}while(pTriangle1 != NULL);

		m_pTriangleHead	= NULL;
		m_pTriangleLast	= NULL;
	}	
}

//////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////
void CDCR_TinTDTACreator::ReleaseVertex()
{
	CDCR_TinVertex	*pVertex1,*pVertex2;

	if(m_pVertexHead != NULL)
	{
		pVertex1	= pVertex2	= m_pVertexHead;
		do
		{
			pVertex1		= pVertex1->m_pNext;
			delete	pVertex2;
			pVertex2		= pVertex1;
		}while(pVertex1 != NULL);

		m_pVertexHead	= NULL;
		m_pVertexLast	= NULL;
	}	
}

//////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////
void CDCR_TinTDTACreator::AddEdge(CDCR_TinEdge *pEdge)
{
	if(m_pEdgeHead == NULL)
	{
		m_pEdgeHead	= pEdge;
		m_pEdgeLast	= pEdge;
	}
	else
	{
		m_pEdgeLast->m_pNext	= pEdge;
		m_pEdgeLast				= pEdge;
	}

	m_iEdgeCount++;
}

//////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////
void CDCR_TinTDTACreator::AddTriangle(CDCR_TinTriangle *pTriangle)
{
	if(m_pTriangleHead == NULL)
	{
		m_pTriangleHead	= pTriangle;
		m_pTriangleLast	= pTriangle;
	}
	else
	{
		m_pTriangleLast->m_pNext	= pTriangle;
		m_pTriangleLast				= pTriangle;
	}

	m_iTriangleCount++;
}

//////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////
void CDCR_TinTDTACreator::CellBuild()
{
	int i,j,ci;

	//��ʼ��
	if(m_pCells != NULL)
	{
		delete	[]m_pCells;
		m_pCells	= NULL;
	}
	
	//���¼�������ߴ�
	double	dSize;
	dSize	= sqrt( (m_dMapRight-m_dMapLeft)*(m_dMapTop-m_dMapBottom)/m_iVertexCount );
	m_dCellSizeOfX	= m_dCellSizeOfY	= 4*dSize/3;

	//�ȼ�������ߴ�
	m_nCellNumOfX	= (int)((m_dMapRight-m_dMapLeft)/m_dCellSizeOfX) + 1;
	m_nCellNumOfY	= (int)((m_dMapTop-m_dMapBottom)/m_dCellSizeOfY) + 1;

	//Ȼ��������Χ�����»������񻮷�����Χ
	m_dCellLeft		= m_dMapLeft - m_dCellSizeOfX/2;
	m_dCellRight	= m_dMapRight + m_dCellSizeOfX/2;
	m_dCellBottom	= m_dMapBottom - m_dCellSizeOfY/2;
	m_dCellTop		= m_dMapTop + m_dCellSizeOfY/2;

	//���¼�������ߴ�
	m_dCellSizeOfX	= (m_dCellRight-m_dCellLeft)/m_nCellNumOfX;
	m_dCellSizeOfY	= (m_dCellTop-m_dCellBottom)/m_nCellNumOfY;

	m_pCells		= new	CDCR_Cells[(m_nCellNumOfX*m_nCellNumOfY)];

	for(i=0;i<m_nCellNumOfY;i++)
	{
		for(j=0;j<m_nCellNumOfX;j++)
		{
			ci	= i*m_nCellNumOfX + j;
						
			m_pCells[ci].m_pPFDHead	= NULL;
			m_pCells[ci].m_pPFDLast	= NULL;
			m_pCells[ci].m_pLFDHead	= NULL;
			m_pCells[ci].m_pLFDLast	= NULL;
			m_pCells[ci].m_pTVDHead	= NULL;
			m_pCells[ci].m_pTVDLast	= NULL;
		}
	}

	//
	CDCR_TinVertex	*pVertex;

	pVertex	= m_pVertexHead;
	while(pVertex != NULL)
	{		
		//��������
		AddVertexToCells(pVertex);

		pVertex	= pVertex->m_pNext;
	}
}

//////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////
void CDCR_TinTDTACreator::SetMapExt(double dLeft,double dRight,double dBottom,double dTop)
{
	m_dMapLeft		= dLeft;
	m_dMapRight		= dRight;
	m_dMapBottom	= dBottom;
	m_dMapTop		= dTop;
}

//////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////
void CDCR_TinTDTACreator::CellReset()
{
	if(m_pCells != NULL)
	{
		int	iC;
		for(int j=0;j<m_nCellNumOfY;j++)
		{
			for(int	i=0;i<m_nCellNumOfX;i++)
			{
				iC	= j*m_nCellNumOfX + i;
							
				m_pCells[iC].m_bUsed	= false;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////
CDCR_TinEdge* CDCR_TinTDTACreator::GetEdgeFromCells(double dX, double dY, int iVertexID1, int iVertexID2)
{
	//�ҵ�dX��dY���ڵĸ���
	int		I,J,iC;
	I	= (int)((dX - m_dCellLeft) / m_dCellSizeOfX);
	J	= (int)((dY - m_dCellBottom) / m_dCellSizeOfY);

	if(I < 0)				I	= 0;
	if(J < 0)				J	= 0;
	if(I >= m_nCellNumOfX)	I	= m_nCellNumOfX-1;
	if(J >= m_nCellNumOfY)	J	= m_nCellNumOfY-1;
	
	//
	iC	= J * m_nCellNumOfX + I;

	//
	CDCR_TinEdgeAddress	*pTED;

	pTED	= m_pCells[iC].m_pTEDHead;
	while(pTED != NULL)
	{
		if((pTED->m_pEdge->m_nVertexID[0]==iVertexID1&&pTED->m_pEdge->m_nVertexID[1]==iVertexID2) || (pTED->m_pEdge->m_nVertexID[0]==iVertexID2&&pTED->m_pEdge->m_nVertexID[1]==iVertexID1))
			return	pTED->m_pEdge;

		pTED	= pTED->m_pNext;
	}
	
	return	NULL;
}

//////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////
bool CDCR_TinTDTACreator::CaculateTerrainFactor(double& dMeanSlope,double& dMeanRoughness)
{
	if(m_pTriangleHead == NULL)	return	false;

	dMeanSlope		= 0;
	dMeanRoughness	= 0;

	int				iTriangleCount;

	iTriangleCount	= m_iTriangleCount;

	CDCR_TinTriangle	*pTriangle;

	pTriangle	= m_pTriangleHead;
	while(pTriangle != NULL)
	{
		int		iID1;
		double	dX1,dY1,dZ1;

		int		iID2;
		double	dX2,dY2,dZ2;

		int		iID3;
		double	dX3,dY3,dZ3;

		pTriangle->GetVertexCoordEx(iID1,dX1,dY1,dZ1,
									iID2,dX2,dY2,dZ2,
									iID3,dX3,dY3,dZ3);

		//һ�㲻����Ϊ��
		double	dSlope;
		dSlope			= CaculateSlope(dX1,dY1,dZ1,dX2,dY2,dZ2,dX3,dY3,dZ3);
		
		double	dSurfaceArea;
		dSurfaceArea	= CaculateSurfaceArea(dX1,dY1,dZ1,dX2,dY2,dZ2,dX3,dY3,dZ3);
		
		double	dProjectionArea;
		dProjectionArea	= dSurfaceArea * cos(dSlope);

		//
		if(fabs(dProjectionArea) < 1e-5)
		{
			iTriangleCount--;
		}
		else
		{
			double	dRoughness;
			//����ͶӰ���Ϊ0��������Ǵ�ֱ����ֲڶ�Ϊ0.
			if(fabs(dProjectionArea) < 1e-5)
				dRoughness		= 1;
			else
				dRoughness		= dSurfaceArea / dProjectionArea;
			
			//
			dMeanSlope		= dMeanSlope + dSlope;

			dMeanRoughness	= dMeanRoughness + dRoughness;
		}

		pTriangle	= pTriangle->m_pNext;
	}
	
	if(iTriangleCount == 0)
	{
		dMeanSlope		= 0;
		dMeanRoughness	= 1;
	}
	else
	{
		dMeanSlope		= dMeanSlope / m_iTriangleCount;

		dMeanRoughness	= dMeanRoughness / m_iTriangleCount;
	}

	return	true;
}

double CDCR_TinTDTACreator::CaculateSlope(double dX1,double dY1,double dZ1,double dX2,double dY2,double dZ2,double dX3,double dY3,double dZ3)
{
	double	dX12,dY12,dZ12;

	dX12	= dX2-dX1;
	dY12	= dY2-dY1;
	dZ12	= dZ2-dZ1;

	double	dX13,dY13,dZ13;
	dX13	= dX3-dX1;
	dY13	= dY3-dY1;
	dZ13	= dZ3-dZ1;

	double	nX,nY,nZ;
	//
	nX		=   dY12*dZ13 - dY13*dZ12;
	//
	nY		= -(dX12*dZ13 - dX13*dZ12);
	//
	nZ		=   dX12*dY13 - dX13*dY12;
	
	//���Ϊ0��������ֱ
	if(fabs(nZ) < 1e-10)	return	90;
	
	//��һ��
	nX		= nX / nZ;
	nY		= nY / nZ;

	double	dAngle;
	
	dAngle	= acos(1.0/(sqrt(nX*nX + nY*nY + 1)));
	
	return	dAngle;
}

double CDCR_TinTDTACreator::CaculateSurfaceArea(double dX1,double dY1,double dZ1,double dX2,double dY2,double dZ2,double dX3,double dY3,double dZ3)
{
	//
	double	dA,dB,dC,dP;
	dA			= sqrt((dX2-dX3)*(dX2-dX3) + (dY2-dY3)*(dY2-dY3) + (dZ2-dZ3)*(dZ2-dZ3));
	dB			= sqrt((dX1-dX3)*(dX1-dX3) + (dY1-dY3)*(dY1-dY3) + (dZ1-dZ3)*(dZ1-dZ3));
	dC			= sqrt((dX2-dX1)*(dX2-dX1) + (dY2-dY1)*(dY2-dY1) + (dZ2-dZ1)*(dZ2-dZ1));
	dP			= (dA+dB+dC)/2;
	
	return	sqrt(dP*(dP-dA)*(dP-dB)*(dP-dC));
}

//
CDCR_TinTriangle* CDCR_TinTDTACreator::GetTriangle()
{
	return	m_pTriangleHead;
}