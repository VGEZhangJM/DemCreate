#if !defined(AFX_SETCALCHEIGHTDLG_H__9B9B316B_56C3_4088_8686_B45B684DDA57__INCLUDED_)
#define AFX_SETCALCHEIGHTDLG_H__9B9B316B_56C3_4088_8686_B45B684DDA57__INCLUDED_

#include "Resource.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetCalcHeightDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetCalcHeightDlg dialog

class CSetCalcHeightDlg : public CDialog
{
// Construction
public:
	CSetCalcHeightDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetCalcHeightDlg)
	enum { IDD = IDD_SET_CALCHEIGHT_DLG };
	BOOL	m_bOnly;
	CString	m_szPathName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetCalcHeightDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString			szFilePath;
	CString			szFileName;

	// Generated message map functions
	//{{AFX_MSG(CSetCalcHeightDlg)
	afx_msg void OnOnly();
	afx_msg void OnBtnPath();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETCALCHEIGHTDLG_H__9B9B316B_56C3_4088_8686_B45B684DDA57__INCLUDED_)
