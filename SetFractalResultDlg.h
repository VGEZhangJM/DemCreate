#if !defined(AFX_SETFRACTALRESULTDLG_H__D8D34CCE_62A6_44E6_8D91_ECE2E343B632__INCLUDED_)
#define AFX_SETFRACTALRESULTDLG_H__D8D34CCE_62A6_44E6_8D91_ECE2E343B632__INCLUDED_

#include "Resource.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetFractalResultDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetFractalResultDlg dialog

class CSetFractalResultDlg : public CDialog
{
// Construction
public:
	CSetFractalResultDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetFractalResultDlg)
	enum { IDD = IDD_SET_FRACTALRESULT_DLG };
	CString	m_szPathName;
	double	m_dResult1;
	double	m_dResult2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetFractalResultDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetFractalResultDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETFRACTALRESULTDLG_H__D8D34CCE_62A6_44E6_8D91_ECE2E343B632__INCLUDED_)
