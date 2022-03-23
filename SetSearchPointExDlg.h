#if !defined(AFX_SETSEARCHPOINTEXDLG_H__9234127B_C97B_4D83_AB77_4AB4BC5DDC86__INCLUDED_)
#define AFX_SETSEARCHPOINTEXDLG_H__9234127B_C97B_4D83_AB77_4AB4BC5DDC86__INCLUDED_

#include "resource.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetSearchPointExDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetSearchPointExDlg dialog

class CSetSearchPointExDlg : public CDialog
{
// Construction
public:
	CSetSearchPointExDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetSearchPointExDlg)
	enum { IDD = IDD_SET_SEARCHPOINTEX_DLG };
	double	m_dX;
	double	m_dY;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetSearchPointExDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetSearchPointExDlg)
	afx_msg void OnChangeEditX();
	afx_msg void OnChangeEditY();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETSEARCHPOINTEXDLG_H__9234127B_C97B_4D83_AB77_4AB4BC5DDC86__INCLUDED_)
