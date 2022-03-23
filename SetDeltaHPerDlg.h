#if !defined(AFX_SETDELTAHPERDLG_H__D683D36B_2292_45FB_92FF_5FB1DAB304DA__INCLUDED_)
#define AFX_SETDELTAHPERDLG_H__D683D36B_2292_45FB_92FF_5FB1DAB304DA__INCLUDED_

#include "Resource.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetDeltaHPerDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetDeltaHPerDlg dialog

class CSetDeltaHPerDlg : public CDialog
{
// Construction
public:
	void DrawCount(CDC *pDC);
	CSetDeltaHPerDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetDeltaHPerDlg)
	enum { IDD = IDD_SET_DELTAHPER_DLG };
	BOOL	m_bCheckSlope;
	BOOL	m_bCheckPattern;
	BOOL	m_bCheckDistance;
	BOOL	m_bCheckPingYuan;
	BOOL	m_bCheckQiuLing;
	BOOL	m_bCheckShanDi;
	double	m_dMinSlope;
	double	m_dMaxSlope;
	double	m_dMinPattern;
	double	m_dMaxPattern;
	double	m_dMinDistance;
	double	m_dMaxDistance;
	double	m_dMinPingYuan;
	double	m_dMaxPingYuan;
	double	m_dMinQiuLing;
	double	m_dMaxQiuLing;
	double	m_dMinShanDi;
	double	m_dMaxShanDi;
	CString	m_szIDWPathName;
	CString	m_szSPDPathName;
	CString	m_szMQFPathName;
	CString	m_szMLFPathName;
	CString	m_szTPSFPathName;
	CString	m_szNCSFPathName;
	CString	m_szEXPPathName;
	CString	m_szLINEPathName;
	int		m_iIDWCount;
	int		m_iSPDCount;
	int		m_iMQFCount;
	int		m_iMLFCount;
	int		m_iTPSFCount;
	int		m_iNCSFCount;
	int		m_iEXPCount;
	int		m_iLINECount;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetDeltaHPerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetDeltaHPerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckSlope();
	afx_msg void OnChangeEditMinSlope();
	afx_msg void OnChangeEditMaxSlope();
	afx_msg void OnCheckPattern();
	afx_msg void OnChangeEditMinPattern();
	afx_msg void OnChangeEditMaxPattern();
	afx_msg void OnCheckDistance();
	afx_msg void OnChangeEditMinDistance();
	afx_msg void OnChangeEditMaxDistance();
	afx_msg void OnCheckPingyuan();
	afx_msg void OnChangeEditMinPingyuan();
	afx_msg void OnChangeEditMaxPingyuan();
	afx_msg void OnCheckQiuling();
	afx_msg void OnChangeEditMinQiuling();
	afx_msg void OnChangeEditMaxQiuling();
	afx_msg void OnCheckShandi();
	afx_msg void OnChangeEditMinShandi();
	afx_msg void OnChangeEditMaxShandi();
	afx_msg void OnButtonIdw();
	afx_msg void OnChangeEditIdw();
	afx_msg void OnButtonSpd();
	afx_msg void OnChangeEditSpd();
	afx_msg void OnButtonMqf();
	afx_msg void OnChangeEditMqf();
	afx_msg void OnButtonMlf();
	afx_msg void OnChangeEditMlf();
	afx_msg void OnButtonTpsf();
	afx_msg void OnChangeEditTpsf();
	afx_msg void OnButtonNcsf();
	afx_msg void OnChangeEditNcsf();
	afx_msg void OnButtonExp();
	afx_msg void OnChangeEditExp();
	afx_msg void OnButtonLine();
	afx_msg void OnChangeEditLine();
	afx_msg void OnButtonClear();
	afx_msg void OnButtonStart();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETDELTAHPERDLG_H__D683D36B_2292_45FB_92FF_5FB1DAB304DA__INCLUDED_)
