#if !defined(AFX_SETTERRAINCVRESULTDLG_H__6BE3BEB3_5F5A_4752_AE57_3DB602BD1512__INCLUDED_)
#define AFX_SETTERRAINCVRESULTDLG_H__6BE3BEB3_5F5A_4752_AE57_3DB602BD1512__INCLUDED_

#include "Resource.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetTerrainCVResultDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetTerrainCVResultDlg dialog

class CSetTerrainCVResultDlg : public CDialog
{
// Construction
public:
	CSetTerrainCVResultDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetTerrainCVResultDlg)
	enum { IDD = IDD_SET_TERRAINCVRESULT_DLG };
	double	m_dMaxValue;
	double	m_dMeanValue;
	double	m_dMinValue;
	double	m_dWindowRange;
	int		m_iWindowSize;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetTerrainCVResultDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetTerrainCVResultDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETTERRAINCVRESULTDLG_H__6BE3BEB3_5F5A_4752_AE57_3DB602BD1512__INCLUDED_)
