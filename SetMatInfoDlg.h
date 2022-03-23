#if !defined(AFX_SETMATINFODLG_H__D33A50A6_7E12_4975_AB65_E9907394C6B3__INCLUDED_)
#define AFX_SETMATINFODLG_H__D33A50A6_7E12_4975_AB65_E9907394C6B3__INCLUDED_

#include "resource.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetMatInfoDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetMatInfoDlg dialog

class CSetMatInfoDlg : public CDialog
{
// Construction
public:
	CSetMatInfoDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetMatInfoDlg)
	enum { IDD = IDD_SET_MATINFO_DLG };
	double	m_dLeft;
	double	m_dRight;
	double	m_dBottom;
	double	m_dTop;
	int		m_iMatWidth;
	int		m_iMatHeight;
	int		m_iWidth;
	int		m_iHeight;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetMatInfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetMatInfoDlg)
	afx_msg void OnChangeEditLeft();
	afx_msg void OnChangeEditRight();
	afx_msg void OnChangeEditBottom();
	afx_msg void OnChangeEditTop();
	afx_msg void OnChangeEditMatwidth();
	afx_msg void OnChangeEditMatheight();
	afx_msg void OnChangeEditWidth();
	afx_msg void OnChangeEditHeight();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETMATINFODLG_H__D33A50A6_7E12_4975_AB65_E9907394C6B3__INCLUDED_)
