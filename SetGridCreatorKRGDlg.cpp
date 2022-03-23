// SetGridCreatorKRGDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "vr_demcreator.h"
#include "SetGridCreatorKRGDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetGridCreatorKRGDlg dialog


CSetGridCreatorKRGDlg::CSetGridCreatorKRGDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetGridCreatorKRGDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetGridCreatorKRGDlg)
	m_iSearchType = 1;
	m_iPointCountPreDirection = 2;
	m_iPointCountInTotal = 8;
	m_dSearchRadius = 0.0;
	m_dC0 = 1.0;
	m_dC = 1.0;
	m_dAlfa = 200.0;
	m_iDemNumX = 100;
	m_iDemNumY = 100;
	m_dDemSizeX = 0.0;
	m_dDemSizeY = 0.0;
	m_szPathName = _T("");
	m_bCheckRE = FALSE;
	//}}AFX_DATA_INIT

	m_dMapLeft		= 0;
	m_dMapRight		= 0;
	m_dMapBottom	= 0;
	m_dMapTop		= 0;
	m_iModelType	= 0;
}


void CSetGridCreatorKRGDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetGridCreatorKRGDlg)
	DDX_Control(pDX, IDC_COMBO_METHOD, m_pMethodCombo);
	DDX_Radio(pDX, IDC_RADIO_GLOBAL, m_iSearchType);
	DDX_Text(pDX, IDC_POINTCOUNT_PREDIRECTION, m_iPointCountPreDirection);
	DDX_Text(pDX, IDC_POINTCOUNT_TOTAL, m_iPointCountInTotal);
	DDX_Text(pDX, IDC_EDIT_SEARCHRADIUS, m_dSearchRadius);
	DDX_Text(pDX, IDC_EDIT_C0, m_dC0);
	DDX_Text(pDX, IDC_EDIT_C, m_dC);
	DDX_Text(pDX, IDC_EDIT_ALFA, m_dAlfa);
	DDX_Text(pDX, IDC_EDIT_NUMX, m_iDemNumX);
	DDX_Text(pDX, IDC_EDIT_NUMY, m_iDemNumY);
	DDX_Text(pDX, IDC_EDIT_SIZEX, m_dDemSizeX);
	DDX_Text(pDX, IDC_EDIT_SIZEY, m_dDemSizeY);
	DDX_Text(pDX, IDC_EDIT_PATHANME, m_szPathName);
	DDX_Check(pDX, IDC_CHECK_RE, m_bCheckRE);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetGridCreatorKRGDlg, CDialog)
	//{{AFX_MSG_MAP(CSetGridCreatorKRGDlg)
	ON_BN_CLICKED(IDC_RADIO_GLOBAL, OnRadioGlobal)
	ON_BN_CLICKED(IDC_RADIO_FOURDIRECTION, OnRadioFourdirection)
	ON_BN_CLICKED(IDC_RADIO_EIGHTDIRECTION, OnRadioEightdirection)
	ON_EN_CHANGE(IDC_POINTCOUNT_PREDIRECTION, OnChangePointcountPredirection)
	ON_EN_CHANGE(IDC_POINTCOUNT_TOTAL, OnChangePointcountTotal)
	ON_EN_CHANGE(IDC_EDIT_SEARCHRADIUS, OnChangeEditSearchradius)
	ON_CBN_SELCHANGE(IDC_COMBO_METHOD, OnSelchangeComboMethod)
	ON_EN_CHANGE(IDC_EDIT_C0, OnChangeEditC0)
	ON_EN_CHANGE(IDC_EDIT_C, OnChangeEditC)
	ON_EN_CHANGE(IDC_EDIT_ALFA, OnChangeEditAlfa)
	ON_BN_CLICKED(IDC_BUTTON_VARIO, OnButtonVario)
	ON_EN_CHANGE(IDC_EDIT_NUMX, OnChangeEditNumx)
	ON_EN_CHANGE(IDC_EDIT_NUMY, OnChangeEditNumy)
	ON_EN_CHANGE(IDC_EDIT_SIZEX, OnChangeEditSizex)
	ON_EN_CHANGE(IDC_EDIT_SIZEY, OnChangeEditSizey)
	ON_EN_CHANGE(IDC_EDIT_PATHANME, OnChangeEditPathanme)
	ON_BN_CLICKED(IDC_BUTTON_PATH, OnButtonPath)
	ON_BN_CLICKED(IDC_CHECK_RE, OnCheckRe)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetGridCreatorKRGDlg message handlers

BOOL CSetGridCreatorKRGDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//
	if(m_iSearchType == 0)
	{
		GetDlgItem(IDC_POINTCOUNT_PREDIRECTION)->EnableWindow(FALSE);
		GetDlgItem(IDC_POINTCOUNT_TOTAL)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_POINTCOUNT_PREDIRECTION)->EnableWindow(TRUE);
		GetDlgItem(IDC_POINTCOUNT_TOTAL)->EnableWindow(FALSE);
	}
	
	//
	if(m_dMapRight-m_dMapLeft > m_dMapTop-m_dMapBottom)
		m_dSearchRadius	= m_dMapRight-m_dMapLeft;
	else
		m_dSearchRadius	= m_dMapTop-m_dMapBottom;
	
	m_dDemSizeX	= (m_dMapRight-m_dMapLeft) / (m_iDemNumX-1);
	m_dDemSizeY	= (m_dMapTop-m_dMapBottom) / (m_iDemNumY-1);

	m_pMethodCombo.SetCurSel(m_iModelType);
	
	m_szPathName	= "D:\\Out(KRG).grd";
	
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSetGridCreatorKRGDlg::OnRadioGlobal() 
{
	m_iSearchType	= 0;

	GetDlgItem(IDC_POINTCOUNT_PREDIRECTION)->EnableWindow(FALSE);
	GetDlgItem(IDC_POINTCOUNT_TOTAL)->EnableWindow(TRUE);	
}

void CSetGridCreatorKRGDlg::OnRadioFourdirection() 
{
	m_iSearchType	= 1;
	
	GetDlgItem(IDC_POINTCOUNT_PREDIRECTION)->EnableWindow(TRUE);
	GetDlgItem(IDC_POINTCOUNT_TOTAL)->EnableWindow(FALSE);		
}

void CSetGridCreatorKRGDlg::OnRadioEightdirection() 
{
	m_iSearchType	= 2;
	
	GetDlgItem(IDC_POINTCOUNT_PREDIRECTION)->EnableWindow(TRUE);
	GetDlgItem(IDC_POINTCOUNT_TOTAL)->EnableWindow(FALSE);		
}

void CSetGridCreatorKRGDlg::OnChangePointcountPredirection() 
{
	UpdateData(TRUE);
	
	if(m_iSearchType == 1)
	{
		m_iPointCountInTotal	= m_iPointCountPreDirection * 4;
		UpdateData(FALSE);
	}
	
	if(m_iSearchType == 2)
	{
		m_iPointCountInTotal	= m_iPointCountPreDirection * 8;
		UpdateData(FALSE);
	}		
}

void CSetGridCreatorKRGDlg::OnChangePointcountTotal() 
{
	UpdateData(TRUE);	
}

void CSetGridCreatorKRGDlg::OnChangeEditSearchradius() 
{
	UpdateData(TRUE);
}

void CSetGridCreatorKRGDlg::OnSelchangeComboMethod() 
{
	m_iModelType	= m_pMethodCombo.GetCurSel();

	if(m_iModelType < 0)	return;	
}

void CSetGridCreatorKRGDlg::OnChangeEditC0() 
{
	UpdateData(TRUE);	
}

void CSetGridCreatorKRGDlg::OnChangeEditC() 
{
	UpdateData(TRUE);
}

void CSetGridCreatorKRGDlg::OnChangeEditAlfa() 
{
	UpdateData(TRUE);
}

void CSetGridCreatorKRGDlg::OnButtonVario() 
{
	// TODO: Add your control notification handler code here
	
}

void CSetGridCreatorKRGDlg::OnChangeEditNumx() 
{
	UpdateData(TRUE);

	//关联尺寸
	if(m_iDemNumX > 1)
	{
		m_dDemSizeX	= (m_dMapRight-m_dMapLeft)/(m_iDemNumX-1);

		UpdateData(FALSE);
	}		
}

void CSetGridCreatorKRGDlg::OnChangeEditNumy() 
{
	UpdateData(TRUE);

	//关联尺寸
	if(m_iDemNumY > 1)
	{
		m_dDemSizeY	= (m_dMapTop-m_dMapBottom)/(m_iDemNumY-1);

		UpdateData(FALSE);
	}		
}

void CSetGridCreatorKRGDlg::OnChangeEditSizex() 
{
	UpdateData(TRUE);

	//关联尺寸
	if(m_dDemSizeX > 0)
	{
		m_iDemNumX	= int((m_dMapRight-m_dMapLeft)/m_dDemSizeX) + 1;

		if(m_iDemNumX > 1)
		{
			m_dDemSizeX	= (m_dMapRight-m_dMapLeft)/(m_iDemNumX-1);
		}

		UpdateData(FALSE);
	}			
}

void CSetGridCreatorKRGDlg::OnChangeEditSizey() 
{
	UpdateData(TRUE);

	//关联尺寸
	if(m_dDemSizeY > 0)
	{
		m_iDemNumY	= int((m_dMapTop-m_dMapBottom)/m_dDemSizeY) + 1;

		if(m_iDemNumY > 1)
		{
			m_dDemSizeY	= (m_dMapTop-m_dMapBottom)/(m_iDemNumY-1);
		}

		UpdateData(FALSE);
	}		
}

void CSetGridCreatorKRGDlg::OnChangeEditPathanme() 
{
	UpdateData(TRUE);	
}

void CSetGridCreatorKRGDlg::OnButtonPath() 
{
	CFileDialog	dlg(FALSE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		"Surfer7.0文本数据文件(*.grd)|*.grd||");
	dlg.m_ofn.lpstrTitle="保存Surfer7.0文本数据文件";
	
	CString	szPathName;

	if(dlg.DoModal()==IDOK)
	{
		m_szPathName	= dlg.GetPathName();
	}
	else
		return;
	
	UpdateData(FALSE);	
}

void CSetGridCreatorKRGDlg::OnCheckRe() 
{
	m_bCheckRE	= !m_bCheckRE;
}
