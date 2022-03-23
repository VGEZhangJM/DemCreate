#if !defined(AFX_SETCHECKPOINTDLG_H__CCDD2109_8878_4BE1_B9C2_F3BB21933057__INCLUDED_)
#define AFX_SETCHECKPOINTDLG_H__CCDD2109_8878_4BE1_B9C2_F3BB21933057__INCLUDED_

#include "resource.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetCheckPointDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetCheckPointDlg dialog

class CSetCheckPointDlg : public CDialog
{
// Construction
public:
	CSetCheckPointDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetCheckPointDlg)
	enum { IDD = IDD_SET_CHECKPOINT_DLG };
	int		m_iPointNum;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetCheckPointDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetCheckPointDlg)
	afx_msg void OnChangeEditNum();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETCHECKPOINTDLG_H__CCDD2109_8878_4BE1_B9C2_F3BB21933057__INCLUDED_)
