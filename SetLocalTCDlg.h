#if !defined(AFX_SETLOCALTCDLG_H__721E5D51_C2E0_4F58_8D67_06D675072CC6__INCLUDED_)
#define AFX_SETLOCALTCDLG_H__721E5D51_C2E0_4F58_8D67_06D675072CC6__INCLUDED_

#include "resource.h"
#include "DCR_GridVertexList.h"
#include "DCR_TinTriangleList.h"
#include "DCR_Grid_IDW.h"
#include "DCR_Grid_SPD.h"
#include "DCR_Grid_RBF.h"
#include "DCR_Grid_KRG.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetLocalTCDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetLocalTCDlg dialog

class CSetLocalTCDlg : public CDialog
{
// Construction
public:
	CSetLocalTCDlg(CWnd* pParent = NULL);   // standard constructor
	//
	BOOL	Create(CWnd *pView);
	//
	void	MoveWindowPos();

	//
	void	DrawAndUpdate(CDC *pDC);

	void	AddGridVertex(double dX,double dY,double dZ,double dDistance);

	void	AddAllGridVretex(double dX,double dY,double dZ,double dDistance);

	void	OnRelease();

	void	OnVauleRefresh();

	//参与插值计算的顶点列表
	CDCR_GridVertexList		m_pGridVertexList;

	//所有范围内的顶点列表
	CDCR_GridVertexList		m_pAllGridVertexList;

	//
	CDCR_TinTriangleList	m_pTriangleList;

	double	m_dCurrentX;
	double	m_dCurrentY;


// Dialog Data
	//{{AFX_DATA(CSetLocalTCDlg)
	enum { IDD = IDD_SET_LOCALTC_DLG };
	int		m_iDirectionType;
	int		m_iPrePointNum;
	int		m_iTotalPointNum;
	double	m_dPointPattern;
	double	m_dMeanSlope;
	double	m_dMeanRoughness;
	double	m_dSearchRadius;
	double	m_dIDWZ;
	double	m_dCurrentZ;
	double	m_dSPDZ;
	double	m_dRBFZ;
	double	m_dKRGZ;
	double	m_dMaxDistance;
	double	m_dCentroid;
	double	m_dMinDistance;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetLocalTCDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//
	bool	InitShow(CRect rRect, double dLeft, double dRight, double dBottom, double dTop);
	//
	CWnd	*m_pMainView;

	CRect	m_rectClient;

	double	m_dMapXA;
	double	m_dMapXB;
	double	m_dMapYA;
	double	m_dMapYB;

	CDCR_Grid_IDW	m_pIDW;
	CDCR_Grid_SPD	m_pSPD;
	CDCR_Grid_RBF	m_pRBF;

	CDCR_Grid_KRG	m_pKRG;

	// Generated message map functions
	//{{AFX_MSG(CSetLocalTCDlg)
	afx_msg void OnRadioNulldirection();
	afx_msg void OnRadioFourdirection();
	afx_msg void OnRadioEightdirection();
	afx_msg void OnChangeEditPrePointnum();
	afx_msg void OnChangeEditTotalPointnum();
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEditRadius();
	afx_msg void OnPaint();
	afx_msg void OnCcuclate();
	afx_msg void OnDestroy();
	afx_msg void OnButtonIdw();
	afx_msg void OnButtonSpd();
	afx_msg void OnButtonRbf();
	afx_msg void OnButtonKrg();
	afx_msg void OnChangeEditMaxdistance();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETLOCALTCDLG_H__721E5D51_C2E0_4F58_8D67_06D675072CC6__INCLUDED_)
