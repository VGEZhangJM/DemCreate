#if !defined(AFX_SETGRIDCREATORRBFDLG_H__225AE2C3_2CC7_45D5_810E_A73F841038B5__INCLUDED_)
#define AFX_SETGRIDCREATORRBFDLG_H__225AE2C3_2CC7_45D5_810E_A73F841038B5__INCLUDED_

#include "Resource.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetGridCreatorRBFDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetGridCreatorRBFDlg dialog

class CSetGridCreatorRBFDlg : public CDialog
{
// Construction
public:
	CSetGridCreatorRBFDlg(CWnd* pParent = NULL);   // standard constructor
	double	m_dMapLeft;
	double	m_dMapRight;
	double	m_dMapBottom;
	double	m_dMapTop;
	int		m_iModelType;

// Dialog Data
	//{{AFX_DATA(CSetGridCreatorRBFDlg)
	enum { IDD = IDD_SET_GRIDCREATOR_RBF_DLG };
	CComboBox	m_pMethodCombo;
	int		m_iSearchType;
	double	m_dSearchRadius;
	double	m_dC;
	int		m_iDemNumX;
	int		m_iDemNumY;
	double	m_dDemSizeX;
	double	m_dDemSizeY;
	CString	m_szPathName;
	int		m_iPointCountPreDirection;
	int		m_iPointCountInTotal;
	BOOL	m_bCheckRE;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetGridCreatorRBFDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetGridCreatorRBFDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioGlobal();
	afx_msg void OnRadioFourdirection();
	afx_msg void OnRadioEightdirection();
	afx_msg void OnChangePointcountPredirection();
	afx_msg void OnChangePointcountTotal();
	afx_msg void OnChangeEditSearchradius();
	afx_msg void OnSelchangeComboMethod();
	afx_msg void OnChangeEditT();
	afx_msg void OnChangeEditNumx();
	afx_msg void OnChangeEditNumy();
	afx_msg void OnChangeEditSizex();
	afx_msg void OnChangeEditSizey();
	afx_msg void OnChangeEditPathanme();
	afx_msg void OnButtonPath();
	afx_msg void OnCheckRe();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETGRIDCREATORRBFDLG_H__225AE2C3_2CC7_45D5_810E_A73F841038B5__INCLUDED_)
