#if !defined(AFX_SETCURVATUREDLG_H__AF44B3AF_E7E1_4466_A94A_596FE6C7107A__INCLUDED_)
#define AFX_SETCURVATUREDLG_H__AF44B3AF_E7E1_4466_A94A_596FE6C7107A__INCLUDED_

#include "Resource.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetCurvatureDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetCurvatureDlg dialog

class CSetCurvatureDlg : public CDialog
{
// Construction
public:
	CSetCurvatureDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetCurvatureDlg)
	enum { IDD = IDD_SET_CURVATURE_DLG };
	CListCtrl	m_pListCtrl;
	//}}AFX_DATA

	int		iNumX,iNumY;
	double	*dCurvature;

	double	dMin,dMax;
	double	dMean;
	double	dVar;
	double	dStdVar;
	double	dCV;
	double	dSkewness;
	double	dKurtosis;

	CImageList			*m_pImageList;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetCurvatureDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void	FillStatContent();

	// Generated message map functions
	//{{AFX_MSG(CSetCurvatureDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETCURVATUREDLG_H__AF44B3AF_E7E1_4466_A94A_596FE6C7107A__INCLUDED_)
