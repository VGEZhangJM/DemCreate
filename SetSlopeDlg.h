#if !defined(AFX_SETSLOPEDLG_H__986C7B71_D5C8_4AA2_8334_1225D84103D4__INCLUDED_)
#define AFX_SETSLOPEDLG_H__986C7B71_D5C8_4AA2_8334_1225D84103D4__INCLUDED_

#include "Resource.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetSlopeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetSlopeDlg dialog

class CSetSlopeDlg : public CDialog
{
// Construction
public:
	CSetSlopeDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetSlopeDlg)
	enum { IDD = IDD_SET_SLOPE_DLG };
	CListCtrl	m_pListCtrl;
	//}}AFX_DATA
	int		iNum;
	double	*pData;

	double	dMin,dMax;
	double	dMean;
	double	dVar;
	double	dStdVar;
	double	dCV;
	double	dSkewness;
	double	dKurtosis;
	double	dRMSE;

	CImageList			*m_pImageList;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetSlopeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void	FillStatContent();

	// Generated message map functions
	//{{AFX_MSG(CSetSlopeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETSLOPEDLG_H__986C7B71_D5C8_4AA2_8334_1225D84103D4__INCLUDED_)
