// DCR_GridVertexList.h: interface for the CDCR_GridVertexList class.
// 顶点列表串
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCR_GRIDVERTEXLIST_H__77C34E5A_02EF_4FAD_B24B_D7426EBF5379__INCLUDED_)
#define AFX_DCR_GRIDVERTEXLIST_H__77C34E5A_02EF_4FAD_B24B_D7426EBF5379__INCLUDED_

#include "DCR_GridVertex.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDCR_GridVertexList  
{
public:
	//
	void				Release();
	//
	bool				AddGridVertex(double dX,double dY,double dZ,double dDistance,int iTotalVertexNum);
	//
	bool				AddGridVertexEx(double dX,double dY,double dZ,double dDistance);
	//
	int					GetGridVertexCount();
	//
	CDCR_GridVertex*	GetGridVertex();

	CDCR_GridVertexList();
	virtual ~CDCR_GridVertexList();
	
protected:	
	//顶点列表串指针
	CDCR_GridVertex	*m_pGridVertexHead;
	CDCR_GridVertex	*m_pGridVertexLast;

	//
	int				m_iGridVertexCount;
};

#endif // !defined(AFX_DCR_GRIDVERTEXLIST_H__77C34E5A_02EF_4FAD_B24B_D7426EBF5379__INCLUDED_)
