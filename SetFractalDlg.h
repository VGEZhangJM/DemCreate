#if !defined(AFX_SETFRACTALDLG_H__BDF2BB7D_0410_47CE_B941_33F18A31A522__INCLUDED_)
#define AFX_SETFRACTALDLG_H__BDF2BB7D_0410_47CE_B941_33F18A31A522__INCLUDED_

#include "Resource.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetFractalDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetFractalDlg dialog

class CSetFractalDlg : public CDialog
{
// Construction
public:
	CSetFractalDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetFractalDlg)
	enum { IDD = IDD_SET_FRACTAL_DLG };
	CListBox	m_pListScale;
	double	m_dSizeX;
	double	m_dSizeY;
	int		m_iNumX;
	int		m_iNumY;
	double	m_dZScale;
	double	m_dScale;
	CString	m_szDemName;
	//}}AFX_DATA
	
	//码尺数目
	int		m_iScaleCount;
	//具体码尺
	double	m_dScaleList[50];

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetFractalDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetFractalDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEditZscale();
	afx_msg void OnChangeEditScale();
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonDel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETFRACTALDLG_H__BDF2BB7D_0410_47CE_B941_33F18A31A522__INCLUDED_)
