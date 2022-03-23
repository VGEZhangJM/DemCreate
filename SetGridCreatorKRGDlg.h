#if !defined(AFX_SETGRIDCREATORKRGDLG_H__7A4BDC99_2054_4837_A9E4_9ED4FF860C41__INCLUDED_)
#define AFX_SETGRIDCREATORKRGDLG_H__7A4BDC99_2054_4837_A9E4_9ED4FF860C41__INCLUDED_

#include "Resource.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetGridCreatorKRGDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetGridCreatorKRGDlg dialog

class CSetGridCreatorKRGDlg : public CDialog
{
// Construction
public:
	CSetGridCreatorKRGDlg(CWnd* pParent = NULL);   // standard constructor
	double	m_dMapLeft;
	double	m_dMapRight;
	double	m_dMapBottom;
	double	m_dMapTop;

	int		m_iModelType;

// Dialog Data
	//{{AFX_DATA(CSetGridCreatorKRGDlg)
	enum { IDD = IDD_SET_GRIDCREATOR_KRG_DLG };
	CComboBox	m_pMethodCombo;
	int		m_iSearchType;
	int		m_iPointCountPreDirection;
	int		m_iPointCountInTotal;
	double	m_dSearchRadius;
	double	m_dC0;
	double	m_dC;
	double	m_dAlfa;
	int		m_iDemNumX;
	int		m_iDemNumY;
	double	m_dDemSizeX;
	double	m_dDemSizeY;
	CString	m_szPathName;
	BOOL	m_bCheckRE;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetGridCreatorKRGDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetGridCreatorKRGDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioGlobal();
	afx_msg void OnRadioFourdirection();
	afx_msg void OnRadioEightdirection();
	afx_msg void OnChangePointcountPredirection();
	afx_msg void OnChangePointcountTotal();
	afx_msg void OnChangeEditSearchradius();
	afx_msg void OnSelchangeComboMethod();
	afx_msg void OnChangeEditC0();
	afx_msg void OnChangeEditC();
	afx_msg void OnChangeEditAlfa();
	afx_msg void OnButtonVario();
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

#endif // !defined(AFX_SETGRIDCREATORKRGDLG_H__7A4BDC99_2054_4837_A9E4_9ED4FF860C41__INCLUDED_)
