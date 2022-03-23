#if !defined(AFX_SETTERRAINCVDLG_H__6BBFF5D7_EFDB_4F0E_A670_7C290BB3A66C__INCLUDED_)
#define AFX_SETTERRAINCVDLG_H__6BBFF5D7_EFDB_4F0E_A670_7C290BB3A66C__INCLUDED_

#include "Resource.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetTerrainCVDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetTerrainCVDlg dialog

class CSetTerrainCVDlg : public CDialog
{
// Construction
public:
	CSetTerrainCVDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetTerrainCVDlg)
	enum { IDD = IDD_SET_TERRAINCV_DLG };
	CString	m_szDemName;
	int		m_iDemNumX;
	int		m_iDemNumY;
	double	m_dDemSizeX;
	double	m_dDemSizeY;
	double	m_dWindowRange;
	int		m_iWindowSize;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetTerrainCVDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetTerrainCVDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEditWindowsize();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETTERRAINCVDLG_H__6BBFF5D7_EFDB_4F0E_A670_7C290BB3A66C__INCLUDED_)
