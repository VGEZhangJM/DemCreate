// DCR_Grid_Height.h: interface for the CDCR_Grid_Height class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCR_GRID_HEIGHT_H__54351529_5537_4C6D_8E79_CC851D4D13C8__INCLUDED_)
#define AFX_DCR_GRID_HEIGHT_H__54351529_5537_4C6D_8E79_CC851D4D13C8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDCR_Grid_Height  
{
public:
	void Release();
	//
	void SetHeight(double dH);

	CDCR_Grid_Height();
	virtual ~CDCR_Grid_Height();
	
	//
	int					m_iNumOfX;
	int					m_iNumOfY;
	
	//
	bool				m_bNewValue;

	//
	double				m_dMaxHeight;
	double				m_dMinHeight;

	//规则格网DEM的高程值
	double				*m_pHeight;

	//
	CString				m_szPathName;
};

#endif // !defined(AFX_DCR_GRID_HEIGHT_H__54351529_5537_4C6D_8E79_CC851D4D13C8__INCLUDED_)
