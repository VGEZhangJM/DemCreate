#if !defined(AFX_SETCALCLSD_DLG_H__E391D79D_D3BE_40C7_981B_A954167443FC__INCLUDED_)
#define AFX_SETCALCLSD_DLG_H__E391D79D_D3BE_40C7_981B_A954167443FC__INCLUDED_

#include "resource.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetCALCLSD_Dlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetCALCLSD_Dlg dialog

class CSetCALCLSD_Dlg : public CDialog
{
// Construction
public:
	CSetCALCLSD_Dlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetCALCLSD_Dlg)
	enum { IDD = IDD_SET_CACLLSD_DLG };
	double	m_dD;
	double	m_dValue;
	double	m_dValue2;
	double	m_dValue3;
	double	m_dValue4;
	double	m_dValue5;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetCALCLSD_Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetCALCLSD_Dlg)
	afx_msg void OnChangeEditD();
	virtual BOOL OnInitDialog();
	afx_msg void OnCalc();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETCALCLSD_DLG_H__E391D79D_D3BE_40C7_981B_A954167443FC__INCLUDED_)
