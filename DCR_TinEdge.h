// DCR_TinEdge.h: interface for the CDCR_TinEdge class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCR_TINEDGE_H__D863B872_4E70_4CF8_BE33_EA4BEC327B53__INCLUDED_)
#define AFX_DCR_TINEDGE_H__D863B872_4E70_4CF8_BE33_EA4BEC327B53__INCLUDED_

#include "DCR_TinTriangle.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDCR_TinEdge  
{
public:
	CDCR_TinEdge();
	virtual ~CDCR_TinEdge();

	//
	void				SetVertexID(int iVertexID1,int iVertexID2);

	//
	void				UpdateTriangle(CDCR_TinTriangle *pOldTriangle,CDCR_TinTriangle *pNewTriangle);

	//�ߵ����
	int					m_nID;

	//ʹ�ñ�־
	bool				m_bUsed;

	//
	bool				m_bDelete;
	
	//
	int					m_iUsedCount;

	//�������
	int					m_nVertexID[2];

	//�ߵ����������κ�
	int					m_nTriangleID[2];

	//�ߵ����ڵ�������ָ��
	CDCR_TinTriangle	*m_pLeftTriangle;
	CDCR_TinTriangle	*m_pRightTriangle;
	
	//
	CDCR_TinEdge		*m_pNext;
};

#endif // !defined(AFX_DCR_TINEDGE_H__D863B872_4E70_4CF8_BE33_EA4BEC327B53__INCLUDED_)
