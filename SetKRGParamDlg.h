#if !defined(AFX_SETKRGPARAMDLG_H__294D027D_834C_47F2_85CC_13E0420F6258__INCLUDED_)
#define AFX_SETKRGPARAMDLG_H__294D027D_834C_47F2_85CC_13E0420F6258__INCLUDED_

#include "resource.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetKRGParamDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetKRGParamDlg dialog

class CSetKRGParamDlg : public CDialog
{
// Construction
public:
	CSetKRGParamDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetKRGParamDlg)
	enum { IDD = IDD_SET_KRGPARAM_DLG };
	int		m_iModelType;
	double	m_dA;
	double	m_dC;
	double	m_dC0;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetKRGParamDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetKRGParamDlg)
	afx_msg void OnChangeEditC0();
	afx_msg void OnChangeEditC();
	afx_msg void OnChangeEditA();
	afx_msg void OnRadioQx();
	afx_msg void OnRadioZs();
	afx_msg void OnRadioGs();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETKRGPARAMDLG_H__294D027D_834C_47F2_85CC_13E0420F6258__INCLUDED_)
