#if !defined(AFX_SETLIMITDLG_H__2FD31788_CC6B_4EFD_8D3D_51D76FBE4CE4__INCLUDED_)
#define AFX_SETLIMITDLG_H__2FD31788_CC6B_4EFD_8D3D_51D76FBE4CE4__INCLUDED_

#include "Resource.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetLimitDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetLimitDlg dialog

class CSetLimitDlg : public CDialog
{
// Construction
public:
	CSetLimitDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetLimitDlg)
	enum { IDD = IDD_SET_LIMIT_DLG };
	double	m_dLimit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetLimitDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetLimitDlg)
	afx_msg void OnChangeEditLimit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETLIMITDLG_H__2FD31788_CC6B_4EFD_8D3D_51D76FBE4CE4__INCLUDED_)
