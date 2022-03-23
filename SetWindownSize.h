#if !defined(AFX_SETWINDOWNSIZE_H__0F0F9B55_03DA_4F9A_9EE0_F7F902E632B0__INCLUDED_)
#define AFX_SETWINDOWNSIZE_H__0F0F9B55_03DA_4F9A_9EE0_F7F902E632B0__INCLUDED_

#include "resource.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetWindownSize.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetWindownSize dialog

class CSetWindownSize : public CDialog
{
// Construction
public:
	CSetWindownSize(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetWindownSize)
	enum { IDD = IDD_SET_WINDOWNSIZE };
	int		m_iSize;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetWindownSize)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetWindownSize)
	afx_msg void OnChangeEditSize();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETWINDOWNSIZE_H__0F0F9B55_03DA_4F9A_9EE0_F7F902E632B0__INCLUDED_)
