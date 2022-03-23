#if !defined(AFX_SETSEARCHPOINTDLG_H__E4D34834_B3A6_41CF_953E_FDFEC6B97F1F__INCLUDED_)
#define AFX_SETSEARCHPOINTDLG_H__E4D34834_B3A6_41CF_953E_FDFEC6B97F1F__INCLUDED_

#include "REsource.h"
#include "DCR_LocalTerrain.h"
#include "DCR_WorkSpace.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetSearchPointDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetSearchPointDlg dialog

class CSetSearchPointDlg : public CDialog
{
// Construction
public:
	CSetSearchPointDlg(CWnd* pParent = NULL);   // standard constructor
	
	//
	CDCR_LocalTerrain	*m_pLocalTerrain;
	//原始数据
	CDCR_WorkSpace		*m_pWorkspace;

	//
	BOOL	Create(CWnd *pView);
	//
	void	MoveWindowPos();

	//
	CWnd	*m_pMainView;

	double	m_dWidth;
	double	m_dHeight;

// Dialog Data
	//{{AFX_DATA(CSetSearchPointDlg)
	enum { IDD = IDD_SET_SEARCHPOINT_DLG };
	CListBox	m_pCoord;
	double	m_dX;
	double	m_dY;
	double	m_dPP;
	double	m_dMS;
	double	m_dDH;
	int		m_iNum;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetSearchPointDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetSearchPointDlg)
	afx_msg void OnChangeEditX();
	afx_msg void OnChangeEditY();
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEditPointpattern();
	afx_msg void OnChangeEditSlope();
	afx_msg void OnSearch();
	afx_msg void OnSelchangeListCoord();
	afx_msg void OnChangeEditDh();
	afx_msg void OnGo();
	afx_msg void OnChangeEditNum();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETSEARCHPOINTDLG_H__E4D34834_B3A6_41CF_953E_FDFEC6B97F1F__INCLUDED_)
