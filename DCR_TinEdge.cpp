// DCR_TinEdge.cpp: implementation of the CDCR_TinEdge class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DCR_TinEdge.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDCR_TinEdge::CDCR_TinEdge()
{
	m_nID			= -1;
	m_bUsed			= false;
	m_iUsedCount	= 0;
	
	for(int i=0;i<2;i++)
	{
		m_nVertexID[i]	= -1;
		m_nTriangleID[i]= -1;
	}

	//�����ڵ�������ָ��
	m_pLeftTriangle	= NULL;
	m_pRightTriangle= NULL;

	m_bDelete		= false;

	m_pNext			= NULL;
}

CDCR_TinEdge::~CDCR_TinEdge()
{

}

//////////////////////////////////////////////////////////////////////
// ���ö���ID��
//////////////////////////////////////////////////////////////////////
void CDCR_TinEdge::SetVertexID(int iVertexID1, int iVertexID2)
{
	//��һ������
	m_nVertexID[0]	= iVertexID1;
	//�ڶ�������
	m_nVertexID[1]	= iVertexID2;
}

//////////////////////////////////////////////////////////////////////
// ���µ��ڽ��������滻�ɵ��ڽ�������
//////////////////////////////////////////////////////////////////////
void CDCR_TinEdge::UpdateTriangle(CDCR_TinTriangle *pOldTriangle,CDCR_TinTriangle *pNewTriangle)
{
	if(m_nTriangleID[0] == pOldTriangle->m_nID)
	{
		m_nTriangleID[0]	= pNewTriangle->m_nID;
	}

	if(m_nTriangleID[1] == pOldTriangle->m_nID)
	{
		m_nTriangleID[1]	= pNewTriangle->m_nID;
	}

	//
	if(m_pLeftTriangle == pOldTriangle)
	{
		m_pLeftTriangle		= 	pNewTriangle;
	}

	if(m_pRightTriangle == pOldTriangle)
	{
		m_pRightTriangle	= 	pNewTriangle;
	}
}