#if !defined(AFX_SETSEMIVARIOGRAMDLG_H__2D5E028B_F957_4F05_BBA9_0AEEC4DA3FA7__INCLUDED_)
#define AFX_SETSEMIVARIOGRAMDLG_H__2D5E028B_F957_4F05_BBA9_0AEEC4DA3FA7__INCLUDED_

#include "Resource.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetSemiVariogramDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetSemiVariogramDlg dialog

class CSetSemiVariogramDlg : public CDialog
{
// Construction
public:
	CSetSemiVariogramDlg(CWnd* pParent = NULL);   // standard constructor
	
	int		m_iLagNum;
	double	m_dLagDistance[100];
	double	m_dRPreLag[100];

// Dialog Data
	//{{AFX_DATA(CSetSemiVariogramDlg)
	enum { IDD = IDD_SET_SEMIVARIOGRAM_DLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetSemiVariogramDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetSemiVariogramDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETSEMIVARIOGRAMDLG_H__2D5E028B_F957_4F05_BBA9_0AEEC4DA3FA7__INCLUDED_)
