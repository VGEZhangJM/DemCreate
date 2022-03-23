#if !defined(AFX_SETTERRAINRELIFRESULTDLG_H__90D44748_354A_44E9_A48D_0E27CE05B9B8__INCLUDED_)
#define AFX_SETTERRAINRELIFRESULTDLG_H__90D44748_354A_44E9_A48D_0E27CE05B9B8__INCLUDED_

#include "Resource.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetTerrainRelifResultDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetTerrainRelifResultDlg dialog

class CSetTerrainRelifResultDlg : public CDialog
{
// Construction
public:
	CSetTerrainRelifResultDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetTerrainRelifResultDlg)
	enum { IDD = IDD_SET_TERRAINRELIFRESULT_DLG };
	int		m_iWindowSize;
	double	m_dWindowRange;
	double	m_dMaxValue;
	double	m_dMinValue;
	double	m_dMeanValue;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetTerrainRelifResultDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetTerrainRelifResultDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETTERRAINRELIFRESULTDLG_H__90D44748_354A_44E9_A48D_0E27CE05B9B8__INCLUDED_)
