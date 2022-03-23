#if !defined(AFX_SETTERRAINFACTORDLG_H__5FA13C82_17BF_46BD_B06F_0D8F2AA5936D__INCLUDED_)
#define AFX_SETTERRAINFACTORDLG_H__5FA13C82_17BF_46BD_B06F_0D8F2AA5936D__INCLUDED_

#include "resource.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetTerrainFactorDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetTerrainFactorDlg dialog

class CSetTerrainFactorDlg : public CDialog
{
// Construction
public:
	CSetTerrainFactorDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetTerrainFactorDlg)
	enum { IDD = IDD_SET_TERRAINFACTOR_DLG };
	CString	m_szPathName;
	//}}AFX_DATA
	CString	m_szWindowName;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetTerrainFactorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetTerrainFactorDlg)
	afx_msg void OnBrown();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETTERRAINFACTORDLG_H__5FA13C82_17BF_46BD_B06F_0D8F2AA5936D__INCLUDED_)
