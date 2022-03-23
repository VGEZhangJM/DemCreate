#if !defined(AFX_SETCELLCOUNTDLG_H__ABD50C3A_2E0D_49B0_B292_7BEEDB9A5D79__INCLUDED_)
#define AFX_SETCELLCOUNTDLG_H__ABD50C3A_2E0D_49B0_B292_7BEEDB9A5D79__INCLUDED_

#include "Resource.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetCellCountDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetCellCountDlg dialog

class CSetCellCountDlg : public CDialog
{
// Construction
public:
	CSetCellCountDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetCellCountDlg)
	enum { IDD = IDD_SET_CELLCOUNT_DLG };
	int		m_iNumOfX;
	int		m_iNumOfY;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetCellCountDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetCellCountDlg)
	afx_msg void OnChangeEditX();
	afx_msg void OnChangeEditY();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETCELLCOUNTDLG_H__ABD50C3A_2E0D_49B0_B292_7BEEDB9A5D79__INCLUDED_)
