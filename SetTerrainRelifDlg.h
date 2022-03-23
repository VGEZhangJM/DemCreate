#if !defined(AFX_SETTERRAINRELIFDLG_H__42D04D8B_6051_4412_9689_47AD8738E7AF__INCLUDED_)
#define AFX_SETTERRAINRELIFDLG_H__42D04D8B_6051_4412_9689_47AD8738E7AF__INCLUDED_

#include "Resource.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetTerrainRelifDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetTerrainRelifDlg dialog

class CSetTerrainRelifDlg : public CDialog
{
// Construction
public:
	CSetTerrainRelifDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetTerrainRelifDlg)
	enum { IDD = IDD_SET_TERRAINRELIF_DLG };
	CString	m_szDemName;
	int		m_iDemNumX;
	int		m_iDemNumY;
	double	m_dDemSizeX;
	double	m_dDemSizeY;
	int		m_iWindowSize;
	double	m_dWindowRange;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetTerrainRelifDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetTerrainRelifDlg)
	afx_msg void OnChangeEditWindowsize();
	afx_msg void OnChangeEditWindowrange();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETTERRAINRELIFDLG_H__42D04D8B_6051_4412_9689_47AD8738E7AF__INCLUDED_)
