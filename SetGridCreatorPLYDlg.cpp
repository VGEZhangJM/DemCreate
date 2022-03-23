// SetGridCreatorPLYDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "vr_demcreator.h"
#include "SetGridCreatorPLYDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetGridCreatorPLYDlg dialog


CSetGridCreatorPLYDlg::CSetGridCreatorPLYDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetGridCreatorPLYDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetGridCreatorPLYDlg)
	m_iSearchType = 1;
	m_iPointCountPreDirection = 2;
	m_iPointCountInTotal = 8;
	m_dSearchRadius = 0.0;
	m_iMode = 0;
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


void CSetGridCreatorPLYDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetGridCreatorPLYDlg)
	DDX_Control(pDX, IDC_COMBO_INTERPOLATION, m_pComboModel);
	DDX_Radio(pDX, IDC_RADIO_GLOBAL, m_iSearchType);
	DDX_Text(pDX, IDC_POINTCOUNT_PREDIRECTION, m_iPointCountPreDirection);
	DDX_Text(pDX, IDC_POINTCOUNT_TOTAL, m_iPointCountInTotal);
	DDX_Text(pDX, IDC_EDIT_SEARCHRADIUS, m_dSearchRadius);
	DDX_Radio(pDX, IDC_RADIO_INTERPOLATION_NH, m_iMode);
	DDX_Text(pDX, IDC_EDIT_NUMX, m_iDemNumX);
	DDX_Text(pDX, IDC_EDIT_NUMY, m_iDemNumY);
	DDX_Text(pDX, IDC_EDIT_SIZEX, m_dDemSizeX);
	DDX_Text(pDX, IDC_EDIT_SIZEY, m_dDemSizeY);
	DDX_Text(pDX, IDC_EDIT_PATHANME, m_szPathName);
	DDX_Check(pDX, IDC_CHECK_RE, m_bCheckRE);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetGridCreatorPLYDlg, CDialog)
	//{{AFX_MSG_MAP(CSetGridCreatorPLYDlg)
	ON_BN_CLICKED(IDC_RADIO_GLOBAL, OnRadioGlobal)
	ON_BN_CLICKED(IDC_RADIO_FOURDIRECTION, OnRadioFourdirection)
	ON_BN_CLICKED(IDC_RADIO_EIGHTDIRECTION, OnRadioEightdirection)
	ON_EN_CHANGE(IDC_POINTCOUNT_PREDIRECTION, OnChangePointcountPredirection)
	ON_EN_CHANGE(IDC_POINTCOUNT_TOTAL, OnChangePointcountTotal)
	ON_EN_CHANGE(IDC_EDIT_SEARCHRADIUS, OnChangeEditSearchradius)
	ON_BN_CLICKED(IDC_RADIO_INTERPOLATION_NH, OnRadioInterpolationNh)
	ON_BN_CLICKED(IDC_RADIO_INTERPOLATION_CZ, OnRadioInterpolationCz)
	ON_CBN_SELCHANGE(IDC_COMBO_INTERPOLATION, OnSelchangeComboInterpolation)
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
// CSetGridCreatorPLYDlg message handlers

BOOL CSetGridCreatorPLYDlg::OnInitDialog() 
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

	m_szPathName	= "D:\\Out(PLY).grd";

	m_pComboModel.SetCurSel(m_iModelType);
	

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSetGridCreatorPLYDlg::OnRadioGlobal() 
{
	m_iSearchType	= 0;

	GetDlgItem(IDC_POINTCOUNT_PREDIRECTION)->EnableWindow(FALSE);
	GetDlgItem(IDC_POINTCOUNT_TOTAL)->EnableWindow(TRUE);
}

void CSetGridCreatorPLYDlg::OnRadioFourdirection() 
{
	m_iSearchType	= 1;
	
	GetDlgItem(IDC_POINTCOUNT_PREDIRECTION)->EnableWindow(TRUE);
	GetDlgItem(IDC_POINTCOUNT_TOTAL)->EnableWindow(FALSE);
}

void CSetGridCreatorPLYDlg::OnRadioEightdirection() 
{
	m_iSearchType	= 2;
	
	GetDlgItem(IDC_POINTCOUNT_PREDIRECTION)->EnableWindow(TRUE);
	GetDlgItem(IDC_POINTCOUNT_TOTAL)->EnableWindow(FALSE);
}

void CSetGridCreatorPLYDlg::OnChangePointcountPredirection() 
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

void CSetGridCreatorPLYDlg::OnChangePointcountTotal() 
{
	UpdateData(TRUE);
}

void CSetGridCreatorPLYDlg::OnChangeEditSearchradius() 
{
	UpdateData(TRUE);
}

void CSetGridCreatorPLYDlg::OnRadioInterpolationNh() 
{
	m_iMode	= 0;	
}

void CSetGridCreatorPLYDlg::OnRadioInterpolationCz() 
{
	m_iMode	= 1;	
}

void CSetGridCreatorPLYDlg::OnSelchangeComboInterpolation() 
{
	int		iItem;
	
	iItem	= m_pComboModel.GetCurSel();

	if(iItem < 0)	return;

	m_iModelType	= iItem;
}

void CSetGridCreatorPLYDlg::OnChangeEditNumx() 
{
	UpdateData(TRUE);

	//关联尺寸
	if(m_iDemNumX > 1)
	{
		m_dDemSizeX	= (m_dMapRight-m_dMapLeft)/(m_iDemNumX-1);

		UpdateData(FALSE);
	}	
}

void CSetGridCreatorPLYDlg::OnChangeEditNumy() 
{
	UpdateData(TRUE);

	//关联尺寸
	if(m_iDemNumY > 1)
	{
		m_dDemSizeY	= (m_dMapTop-m_dMapBottom)/(m_iDemNumY-1);

		UpdateData(FALSE);
	}
}

void CSetGridCreatorPLYDlg::OnChangeEditSizex() 
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

void CSetGridCreatorPLYDlg::OnChangeEditSizey() 
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

void CSetGridCreatorPLYDlg::OnChangeEditPathanme() 
{
	UpdateData(TRUE);
}

void CSetGridCreatorPLYDlg::OnButtonPath() 
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

void CSetGridCreatorPLYDlg::OnCheckRe() 
{
	m_bCheckRE	= !m_bCheckRE;
}
