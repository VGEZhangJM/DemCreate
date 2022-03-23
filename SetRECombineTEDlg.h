#if !defined(AFX_SETRECOMBINETEDLG_H__C129078D_5F2B_45B4_9C9D_59DC3C429C5D__INCLUDED_)
#define AFX_SETRECOMBINETEDLG_H__C129078D_5F2B_45B4_9C9D_59DC3C429C5D__INCLUDED_

#include "resource.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetRECombineTEDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetRECombineTEDlg dialog

class CSetRECombineTEDlg : public CDialog
{
// Construction
public:
	CSetRECombineTEDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetRECombineTEDlg)
	enum { IDD = IDD_SET_RECOMBINETE_DLG };
	CString	m_szREPathName;
	CString	m_szTEPathName;
	CString	m_szComPathName;
	CString	m_szREPathName2;
	CString	m_szREPathName3;
	CString	m_szREPathName4;
	CString	m_szREPathName5;
	CString	m_szREPathName6;
	CString	m_szTEPathName2;
	CString	m_szTEPathName4;
	CString	m_szTEPathName3;
	CString	m_szTEPathName5;
	CString	m_szTEPathName6;
	BOOL	m_bCheckPingYuan;
	BOOL	m_bCheckQiuLing;
	BOOL	m_bCheckDiShan;
	BOOL	m_bCheckZhongShan;
	BOOL	m_bCheckGaoShan;
	BOOL	m_bCheckHunHe;
	int		m_iCombineType;
	CString	m_szLSDPathName;
	CString	m_szLSDPathName2;
	CString	m_szLSDPathName3;
	CString	m_szLSDPathName4;
	CString	m_szLSDPathName5;
	CString	m_szLSDPathName6;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetRECombineTEDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetRECombineTEDlg)
	afx_msg void OnButtonRe();
	afx_msg void OnButtonTe();
	afx_msg void OnButtonCom();
	virtual void OnOK();
	afx_msg void OnButtonRe2();
	afx_msg void OnButtonRe3();
	afx_msg void OnButtonRe4();
	afx_msg void OnButtonRe5();
	afx_msg void OnButtonRe6();
	afx_msg void OnButtonTe2();
	afx_msg void OnButtonTe3();
	afx_msg void OnButtonTe4();
	afx_msg void OnButtonTe5();
	afx_msg void OnButtonTe6();
	afx_msg void OnCheckPingyuan();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonLsd();
	afx_msg void OnCheckQiuling();
	afx_msg void OnButtonLsd2();
	afx_msg void OnCheckDishan();
	afx_msg void OnButtonLsd3();
	afx_msg void OnCheckZhongshan();
	afx_msg void OnButtonLsd4();
	afx_msg void OnCheckGaoshan();
	afx_msg void OnButtonLsd5();
	afx_msg void OnCheckHunhe();
	afx_msg void OnButtonLsd6();
	afx_msg void OnRadioTe();
	afx_msg void OnRadioLsd();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETRECOMBINETEDLG_H__C129078D_5F2B_45B4_9C9D_59DC3C429C5D__INCLUDED_)
