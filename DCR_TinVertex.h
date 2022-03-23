// DCR_TinVertex.h: interface for the CDCR_TinVertex class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCR_TINVERTEX_H__31AD0C77_0707_4FFE_B930_563933E3D3B4__INCLUDED_)
#define AFX_DCR_TINVERTEX_H__31AD0C77_0707_4FFE_B930_563933E3D3B4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDCR_TinVertex  
{
public:
	CDCR_TinVertex();
	virtual ~CDCR_TinVertex();

public:
	//
	void	Add(int iID,double dX,double dY,double dZ);
	
	//顶点坐标
	double	m_zz;
	double	m_yy;
	double	m_xx;
	
	//
	bool	m_bUsed;

	//顶点序号
	int		m_nID;

	//下一顶点
	CDCR_TinVertex	*m_pNext;
};

#endif // !defined(AFX_DCR_TINVERTEX_H__31AD0C77_0707_4FFE_B930_563933E3D3B4__INCLUDED_)
