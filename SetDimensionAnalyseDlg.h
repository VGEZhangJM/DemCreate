#if !defined(AFX_SETDIMENSIONANALYSEDLG_H__BED4D288_B8DB_4D60_A60B_410094CF19CD__INCLUDED_)
#define AFX_SETDIMENSIONANALYSEDLG_H__BED4D288_B8DB_4D60_A60B_410094CF19CD__INCLUDED_

#include "resource.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetDimensionAnalyseDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetDimensionAnalyseDlg dialog

class CSetDimensionAnalyseDlg : public CDialog
{
// Construction
public:
	CSetDimensionAnalyseDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetDimensionAnalyseDlg)
	enum { IDD = IDD_SET_DIMENSIONANALYSE_DLG };
	CString	m_szPathName;
	int		m_iMode;
	int		m_iFieldNum;
	int		m_iDelims;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetDimensionAnalyseDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetDimensionAnalyseDlg)
	afx_msg void OnButtonPathname();
	afx_msg void OnChangeEditFieldnum();
	afx_msg void OnRadioMode1();
	afx_msg void OnRadioMode2();
	afx_msg void OnRadioMode3();
	afx_msg void OnRadioMode4();
	virtual void OnOK();
	afx_msg void OnRadioDelims1();
	afx_msg void OnRadioDelims2();
	afx_msg void OnRadioDelims3();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETDIMENSIONANALYSEDLG_H__BED4D288_B8DB_4D60_A60B_410094CF19CD__INCLUDED_)
