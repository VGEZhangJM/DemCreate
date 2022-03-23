#if !defined(AFX_SETGRIDCREATORSPDDLG_H__B60B8CB2_D4DB_4C62_A8C4_438368795976__INCLUDED_)
#define AFX_SETGRIDCREATORSPDDLG_H__B60B8CB2_D4DB_4C62_A8C4_438368795976__INCLUDED_

#include "Resource.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetGridCreatorSPDDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetGridCreatorSPDDlg dialog

class CSetGridCreatorSPDDlg : public CDialog
{
// Construction
public:
	CSetGridCreatorSPDDlg(CWnd* pParent = NULL);   // standard constructor
	double	m_dMapLeft;
	double	m_dMapRight;
	double	m_dMapBottom;
	double	m_dMapTop;

// Dialog Data
	//{{AFX_DATA(CSetGridCreatorSPDDlg)
	enum { IDD = IDD_SET_GRIDCREATOR_SPD_DLG };
	int		m_iSearchType;
	int		m_iPointCountPreDirection;
	int		m_iPointCountInTotal;
	double	m_dSearchRadius;
	int		m_iU;
	double	m_dT;
	int		m_bCustom;
	double	m_dMaxDistance;
	int		m_iDemNumX;
	int		m_iDemNumY;
	double	m_dDemSizeX;
	double	m_dDemSizeY;
	CString	m_szPathName;
	BOOL	m_bCheckRE;
	int		m_iKernal;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetGridCreatorSPDDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetGridCreatorSPDDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioGlobal();
	afx_msg void OnRadioFourdirection();
	afx_msg void OnRadioEightdirection();
	afx_msg void OnChangePointcountPredirection();
	afx_msg void OnChangePointcountTotal();
	afx_msg void OnChangeEditSearchradius();
	afx_msg void OnChangeEditU();
	afx_msg void OnChangeEditT();
	afx_msg void OnRadioSampledistance();
	afx_msg void OnRadioCustomdistance();
	afx_msg void OnChangeEditMaxdistance();
	afx_msg void OnChangeEditNumx();
	afx_msg void OnChangeEditNumy();
	afx_msg void OnChangeEditSizex();
	afx_msg void OnChangeEditSizey();
	afx_msg void OnChangeEditPathanme();
	afx_msg void OnButtonPath();
	afx_msg void OnCheckRe();
	afx_msg void OnRadioKernal();
	afx_msg void OnRadioKernal2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETGRIDCREATORSPDDLG_H__B60B8CB2_D4DB_4C62_A8C4_438368795976__INCLUDED_)
