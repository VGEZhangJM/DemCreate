// DCR_TinTriangleList.h: interface for the CDCR_TinTriangleList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCR_TINTRIANGLELIST_H__7FDD6B35_ABE2_47EF_BC84_3FA99A7DEA24__INCLUDED_)
#define AFX_DCR_TINTRIANGLELIST_H__7FDD6B35_ABE2_47EF_BC84_3FA99A7DEA24__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDCR_TinTriangleList  
{
public:
	CDCR_TinTriangleList();
	virtual ~CDCR_TinTriangleList();
	
	//
	void	Add(int iID1,double dX1,double dY1,int iID2,double dX2,double dY2,int iID3,double dX3,double dY3);

	//
	void	Clone(CDCR_TinTriangleList *pList);

	//
	void	Release();

	//
	void	Draw(CDC *pDC,double dXA,double dXB,double dYA,double dYB);

public:
	//
	CArray<tagTRIANGLE,tagTRIANGLE>	m_pTriangleList;
};

#endif // !defined(AFX_DCR_TINTRIANGLELIST_H__7FDD6B35_ABE2_47EF_BC84_3FA99A7DEA24__INCLUDED_)
