#if !defined(AFX_SETSTATDLG_H__0D0A28FF_E9B3_404C_8658_82528BC5BAA8__INCLUDED_)
#define AFX_SETSTATDLG_H__0D0A28FF_E9B3_404C_8658_82528BC5BAA8__INCLUDED_

#include "Resource.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetStatDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetStatDlg dialog

class CSetStatDlg : public CDialog
{
// Construction
public:
	void FillStatContent();
	CSetStatDlg(CWnd* pParent = NULL);   // standard constructor
	
	//����Χ
	double	dLeft,dRight,dBottom,dTop;
	//�̷߳�Χ
	double	dMinH,dMaxH;
	//������ֵ
	double	dXMean,dYMean,dZMean;
	//��������
	double	dXVar,dYVar,dZVar;
	//������׼����
	double	dXStdVar,dYStdVar,dZStdVar;
	//��������ϵ��
	double	dZCV;

// Dialog Data
	//{{AFX_DATA(CSetStatDlg)
	enum { IDD = IDD_SET_STAT_DLG };
	CListCtrl	m_pListCtrl;
	//}}AFX_DATA
	


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetStatDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CImageList			*m_pImageList;

	// Generated message map functions
	//{{AFX_MSG(CSetStatDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETSTATDLG_H__0D0A28FF_E9B3_404C_8658_82528BC5BAA8__INCLUDED_)
