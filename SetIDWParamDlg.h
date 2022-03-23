#if !defined(AFX_SETIDWPARAMDLG_H__A4591E18_793C_4AFB_A165_C5EE9E2E70B8__INCLUDED_)
#define AFX_SETIDWPARAMDLG_H__A4591E18_793C_4AFB_A165_C5EE9E2E70B8__INCLUDED_

#include "resource.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetIDWParamDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetIDWParamDlg dialog

class CSetIDWParamDlg : public CDialog
{
// Construction
public:
	CSetIDWParamDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetIDWParamDlg)
	enum { IDD = IDD_SET_IDWPARAM_DLG };
	double	m_dT;
	int		m_iU;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetIDWParamDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetIDWParamDlg)
	afx_msg void OnChangeEditU();
	afx_msg void OnChangeEditT();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETIDWPARAMDLG_H__A4591E18_793C_4AFB_A165_C5EE9E2E70B8__INCLUDED_)
