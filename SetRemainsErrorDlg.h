#if !defined(AFX_SETREMAINSERRORDLG_H__A090D6F1_08F5_4B9F_A5A0_A32E52CB5897__INCLUDED_)
#define AFX_SETREMAINSERRORDLG_H__A090D6F1_08F5_4B9F_A5A0_A32E52CB5897__INCLUDED_

#include "Resource.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetRemainsErrorDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetRemainsErrorDlg dialog

class CSetRemainsErrorDlg : public CDialog
{
// Construction
public:
	CSetRemainsErrorDlg(CWnd* pParent = NULL);   // standard constructor
	
// Dialog Data
	//{{AFX_DATA(CSetRemainsErrorDlg)
	enum { IDD = IDD_SETREMAINSERROR_DLG };
	CListCtrl	m_pListStats;
	CListCtrl	m_pListCtrl;
	//}}AFX_DATA
	
	int		iSampleCount;
	double	*dSampleX;
	double	*dSampleY;
	double	*dSampleZ;
	double	*dSampleCZ;
	
	//
	CString	m_szPathName;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetRemainsErrorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetRemainsErrorDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETREMAINSERRORDLG_H__A090D6F1_08F5_4B9F_A5A0_A32E52CB5897__INCLUDED_)
