#if !defined(AFX_SETGRIDCREATORIDWDLG_H__B1D24C17_0792_4D16_A295_30437FC4802F__INCLUDED_)
#define AFX_SETGRIDCREATORIDWDLG_H__B1D24C17_0792_4D16_A295_30437FC4802F__INCLUDED_

#include "Resource.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetGridCreatorIDWDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetGridCreatorIDWDlg dialog

class CSetGridCreatorIDWDlg : public CDialog
{
// Construction
public:
	CSetGridCreatorIDWDlg(CWnd* pParent = NULL);   // standard constructor
	
	double	m_dMapLeft;
	double	m_dMapRight;
	double	m_dMapBottom;
	double	m_dMapTop;

// Dialog Data
	//{{AFX_DATA(CSetGridCreatorIDWDlg)
	enum { IDD = IDD_SET_GRIDCREATOR_IDW_DLG };
	int		m_iSearchType;
	int		m_iPointCountPreDirection;
	int		m_iPointCountInTotal;
	double	m_dSearchRadius;
	int		m_iU;
	double	m_dT;
	int		m_iDemNumX;
	int		m_iDemNumY;
	double	m_dDemSizeX;
	double	m_dDemSizeY;
	CString	m_szPathName;
	BOOL	m_bCheckRE;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetGridCreatorIDWDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetGridCreatorIDWDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioGlobal();
	afx_msg void OnRadioFourdirection();
	afx_msg void OnRadioEightdirection();
	afx_msg void OnChangePointcountPredirection();
	afx_msg void OnChangePointcountTotal();
	afx_msg void OnChangeEditSearchradius();
	afx_msg void OnChangeEditU();
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

#endif // !defined(AFX_SETGRIDCREATORIDWDLG_H__B1D24C17_0792_4D16_A295_30437FC4802F__INCLUDED_)
