// DCR_GridVertexList.cpp: implementation of the CDCR_GridVertexList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DCR_GridVertexList.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDCR_GridVertexList::CDCR_GridVertexList()
{
	//�����б�ָ��
	m_pGridVertexHead	= NULL;
	m_pGridVertexLast	= NULL;

	//
	m_iGridVertexCount	= 0;

	//
}

CDCR_GridVertexList::~CDCR_GridVertexList()
{
	Release();
}

//////////////////////////////////////////////////////////////////////
// ��ȡ�����б��б�
//////////////////////////////////////////////////////////////////////
CDCR_GridVertex* CDCR_GridVertexList::GetGridVertex()
{
	return	m_pGridVertexHead;
}


//////////////////////////////////////////////////////////////////////
// ��ȡ�����б���Ŀ
//////////////////////////////////////////////////////////////////////
int CDCR_GridVertexList::GetGridVertexCount()
{
	return	m_iGridVertexCount;
}

//////////////////////////////////////////////////////////////////////
// ���Ӷ��㵽���б�
//////////////////////////////////////////////////////////////////////
bool CDCR_GridVertexList::AddGridVertex(double dX, double dY, double dZ, double dDistance,int iTotalVertexNum)
{
	if(m_pGridVertexHead == NULL)
	{
		CDCR_GridVertex	*pGridVertex;

		pGridVertex	= new	CDCR_GridVertex;
		if(pGridVertex == NULL)	return	false;

		pGridVertex->m_dX			= dX;
		pGridVertex->m_dY			= dY;
		pGridVertex->m_dZ			= dZ;
		pGridVertex->m_dDistance	= dDistance;

		m_pGridVertexHead	= pGridVertex;
		m_pGridVertexLast	= pGridVertex;

		m_iGridVertexCount++;
	}
	else
	{
		if(m_iGridVertexCount < iTotalVertexNum)
		{
			CDCR_GridVertex	*pGridVertex;

			pGridVertex	= new	CDCR_GridVertex;
			if(pGridVertex == NULL)	return	false;

			pGridVertex->m_dX			= dX;
			pGridVertex->m_dY			= dY;
			pGridVertex->m_dZ			= dZ;
			pGridVertex->m_dDistance	= dDistance;

			m_pGridVertexLast->m_pNext	= pGridVertex;
			m_pGridVertexLast			= pGridVertex;

			m_iGridVertexCount++;
		}
		else
		{
			//Ѱ������
			double			dMaxDistance;
			//�ȽϾ���Ȼ����뵽����
			CDCR_GridVertex	*pVertex;
			CDCR_GridVertex	*pTempVertex;
			
			dMaxDistance	= 0;
			pTempVertex		= NULL;
			pVertex			= m_pGridVertexHead;
			while(pVertex != NULL)
			{	
				if(pVertex == m_pGridVertexHead)
				{
					dMaxDistance	= pVertex->m_dDistance;
					pTempVertex		= pVertex;
				}
				else
				{
					if(dMaxDistance < pVertex->m_dDistance)
					{
						dMaxDistance	= pVertex->m_dDistance;
						pTempVertex		= pVertex;
					}
				}
				
				pVertex			= pVertex->m_pNext;	
			}			
			
			if(pTempVertex != NULL)
			{
				if(dMaxDistance > dDistance)
				{
					pTempVertex->m_dX			= dX;
					pTempVertex->m_dY			= dY;
					pTempVertex->m_dZ			= dZ;
					pTempVertex->m_dDistance	= dDistance;
				}
			}
		}
	}

	return	true;
}

//
bool CDCR_GridVertexList::AddGridVertexEx(double dX,double dY,double dZ,double dDistance)
{
	CDCR_GridVertex	*pGridVertex;

	pGridVertex	= new	CDCR_GridVertex;
	if(pGridVertex == NULL)	return	false;

	pGridVertex->m_dX			= dX;
	pGridVertex->m_dY			= dY;
	pGridVertex->m_dZ			= dZ;
	pGridVertex->m_dDistance	= dDistance;


	if(m_pGridVertexHead == NULL)
	{
		m_pGridVertexHead	= pGridVertex;
		m_pGridVertexLast	= pGridVertex;
	}
	else
	{
		m_pGridVertexLast->m_pNext	= pGridVertex;
		m_pGridVertexLast			= pGridVertex;
	}

	m_iGridVertexCount++;

	return	true;
}

//////////////////////////////////////////////////////////////////////
// �ͷ�
//////////////////////////////////////////////////////////////////////
void CDCR_GridVertexList::Release()
{
	CDCR_GridVertex	*pVertex1,*pVertex2;

	if(m_pGridVertexHead != NULL)
	{
		pVertex1	= pVertex2	= m_pGridVertexHead;
		do
		{
			pVertex1		= pVertex1->m_pNext;
			delete	pVertex2;
			pVertex2		= pVertex1;
		}while(pVertex1 != NULL);

		m_pGridVertexHead	= NULL;
		m_pGridVertexLast	= NULL;

		m_iGridVertexCount	= 0;
	}	
}
