#if !defined(AFX_SETSPDPARAMDLG_H__7298E2CB_2CFC_4DE5_89F1_35C9173B395F__INCLUDED_)
#define AFX_SETSPDPARAMDLG_H__7298E2CB_2CFC_4DE5_89F1_35C9173B395F__INCLUDED_

#include "resource.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetSPDParamDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetSPDParamDlg dialog

class CSetSPDParamDlg : public CDialog
{
// Construction
public:
	CSetSPDParamDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetSPDParamDlg)
	enum { IDD = IDD_SET_SPDPARAM_DLG };
	double	m_dT;
	int		m_iU;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetSPDParamDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetSPDParamDlg)
	afx_msg void OnChangeEditU();
	afx_msg void OnChangeEditT();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETSPDPARAMDLG_H__7298E2CB_2CFC_4DE5_89F1_35C9173B395F__INCLUDED_)
