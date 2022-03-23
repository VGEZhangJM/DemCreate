// DCR_PointPattern_KFunction.h: interface for the CDCR_PointPattern_KFunction class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCR_POINTPATTERN_KFUNCTION_H__10E55A3C_31D3_4621_B962_8D4D96A87C3F__INCLUDED_)
#define AFX_DCR_POINTPATTERN_KFUNCTION_H__10E55A3C_31D3_4621_B962_8D4D96A87C3F__INCLUDED_

#include "DCR_GridVertexList.h"
#include "DCR_PointFeature.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDCR_PointPattern_KFunction  
{
public:
	CDCR_PointPattern_KFunction();
	virtual ~CDCR_PointPattern_KFunction();

	//设置顶点列表
	void	SetGridVertexList(CDCR_GridVertexList *pVretexList);

	//
	void	SetPointFeature(CDCR_PointFeature *pPointHead);

	//
	bool	CalcRipleyK();
	bool	CalcRipleyKWithPoint();

	//
	void	SetPathName(CString	szPathName);
public:
	//采样点点数
	CDCR_GridVertexList	*m_pVretexList;
	//离散点数据
	CDCR_PointFeature	*m_pPointHead;

	//面积
	double				m_dArea;
	//尺度,取最大值
	double				m_dSize;

	//分割数，缺省为50
	int					m_iCount;

	//
	int					m_iSampleCount;

	//
	double				*m_arrD;
	double				*m_arrK;

	CString				m_szPathName;
};

#endif // !defined(AFX_DCR_POINTPATTERN_KFUNCTION_H__10E55A3C_31D3_4621_B962_8D4D96A87C3F__INCLUDED_)
