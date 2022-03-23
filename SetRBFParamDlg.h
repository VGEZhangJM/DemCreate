#if !defined(AFX_SETRBFPARAMDLG_H__EA90A7C1_9AE8_4A44_9AC9_C664E65AF769__INCLUDED_)
#define AFX_SETRBFPARAMDLG_H__EA90A7C1_9AE8_4A44_9AC9_C664E65AF769__INCLUDED_

#include "resource.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetRBFParamDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetRBFParamDlg dialog

class CSetRBFParamDlg : public CDialog
{
// Construction
public:
	CSetRBFParamDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetRBFParamDlg)
	enum { IDD = IDD_SET_RBFPARAM_DLG };
	int		m_iModelType;
	double	m_dC;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetRBFParamDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetRBFParamDlg)
	afx_msg void OnChangeEditC();
	afx_msg void OnRadioMqf();
	afx_msg void OnRadioIqmf();
	afx_msg void OnRadioTpsf();
	afx_msg void OnRadioMlf();
	afx_msg void OnRadioNcsf();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETRBFPARAMDLG_H__EA90A7C1_9AE8_4A44_9AC9_C664E65AF769__INCLUDED_)
