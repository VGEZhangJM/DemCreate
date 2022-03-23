#if !defined(AFX_SETIMPORTTXTDLG_H__99E42127_ECB6_4D48_A811_163416A4216A__INCLUDED_)
#define AFX_SETIMPORTTXTDLG_H__99E42127_ECB6_4D48_A811_163416A4216A__INCLUDED_

#include "resource.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetImportTxtDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetImportTxtDlg dialog

class CSetImportTxtDlg : public CDialog
{
// Construction
public:
	CSetImportTxtDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetImportTxtDlg)
	enum { IDD = IDD_SET_IMPORTTXT_DLG };
	CString	m_szTxtPathName;
	int		m_iX;
	int		m_iY;
	int		m_iZ;
	int		m_iNumOfAttrib;
	BOOL	m_bFirstLine;
	int		m_iSeparator;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetImportTxtDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetImportTxtDlg)
	afx_msg void OnChangeEditX();
	afx_msg void OnChangeEditY();
	afx_msg void OnChangeEditZ();
	afx_msg void OnChangeEditNumattrib();
	afx_msg void OnButtonTxt();
	afx_msg void OnCheckFirstline();
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	afx_msg void OnRadio3();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETIMPORTTXTDLG_H__99E42127_ECB6_4D48_A811_163416A4216A__INCLUDED_)
