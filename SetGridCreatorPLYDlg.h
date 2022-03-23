#if !defined(AFX_SETGRIDCREATORPLYDLG_H__78E8673B_00BA_42CC_85C6_76B3091B8673__INCLUDED_)
#define AFX_SETGRIDCREATORPLYDLG_H__78E8673B_00BA_42CC_85C6_76B3091B8673__INCLUDED_

#include	"Resource.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetGridCreatorPLYDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetGridCreatorPLYDlg dialog

class CSetGridCreatorPLYDlg : public CDialog
{
// Construction
public:
	CSetGridCreatorPLYDlg(CWnd* pParent = NULL);   // standard constructor

	int		m_iModelType;

	double	m_dMapLeft;
	double	m_dMapRight;
	double	m_dMapBottom;
	double	m_dMapTop;

// Dialog Data
	//{{AFX_DATA(CSetGridCreatorPLYDlg)
	enum { IDD = IDD_SET_GRIDCREATOR_PLY_DLG };
	CComboBox	m_pComboModel;
	int		m_iSearchType;
	int		m_iPointCountPreDirection;
	int		m_iPointCountInTotal;
	double	m_dSearchRadius;
	int		m_iMode;
	int		m_iDemNumX;
	int		m_iDemNumY;
	double	m_dDemSizeX;
	double	m_dDemSizeY;
	CString	m_szPathName;
	BOOL	m_bCheckRE;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetGridCreatorPLYDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetGridCreatorPLYDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioGlobal();
	afx_msg void OnRadioFourdirection();
	afx_msg void OnRadioEightdirection();
	afx_msg void OnChangePointcountPredirection();
	afx_msg void OnChangePointcountTotal();
	afx_msg void OnChangeEditSearchradius();
	afx_msg void OnRadioInterpolationNh();
	afx_msg void OnRadioInterpolationCz();
	afx_msg void OnSelchangeComboInterpolation();
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

#endif // !defined(AFX_SETGRIDCREATORPLYDLG_H__78E8673B_00BA_42CC_85C6_76B3091B8673__INCLUDED_)
