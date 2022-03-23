// SetGridCreatorSPDDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "vr_demcreator.h"
#include "SetGridCreatorSPDDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetGridCreatorSPDDlg dialog


CSetGridCreatorSPDDlg::CSetGridCreatorSPDDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetGridCreatorSPDDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetGridCreatorSPDDlg)
	m_iSearchType = 1;
	m_iPointCountPreDirection = 2;
	m_iPointCountInTotal = 8;
	m_dSearchRadius = 0.0;
	m_iU = 1;
	m_dT = 0.0;
	m_bCustom = 0;
	m_dMaxDistance = 0.0;
	m_iDemNumX = 100;
	m_iDemNumY = 100;
	m_dDemSizeX = 0.0;
	m_dDemSizeY = 0.0;
	m_szPathName = _T("");
	m_bCheckRE = FALSE;
	m_iKernal = 0;
	//}}AFX_DATA_INIT
	m_dMapLeft		= 0;
	m_dMapRight		= 0;
	m_dMapBottom	= 0;
	m_dMapTop		= 0;
}


void CSetGridCreatorSPDDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetGridCreatorSPDDlg)
	DDX_Radio(pDX, IDC_RADIO_GLOBAL, m_iSearchType);
	DDX_Text(pDX, IDC_POINTCOUNT_PREDIRECTION, m_iPointCountPreDirection);
	DDX_Text(pDX, IDC_POINTCOUNT_TOTAL, m_iPointCountInTotal);
	DDX_Text(pDX, IDC_EDIT_SEARCHRADIUS, m_dSearchRadius);
	DDX_Text(pDX, IDC_EDIT_U, m_iU);
	DDX_Text(pDX, IDC_EDIT_T, m_dT);
	DDX_Radio(pDX, IDC_RADIO_SAMPLEDISTANCE, m_bCustom);
	DDX_Text(pDX, IDC_EDIT_MAXDISTANCE, m_dMaxDistance);
	DDX_Text(pDX, IDC_EDIT_NUMX, m_iDemNumX);
	DDX_Text(pDX, IDC_EDIT_NUMY, m_iDemNumY);
	DDX_Text(pDX, IDC_EDIT_SIZEX, m_dDemSizeX);
	DDX_Text(pDX, IDC_EDIT_SIZEY, m_dDemSizeY);
	DDX_Text(pDX, IDC_EDIT_PATHANME, m_szPathName);
	DDX_Check(pDX, IDC_CHECK_RE, m_bCheckRE);
	DDX_Radio(pDX, IDC_RADIO_KERNAL, m_iKernal);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetGridCreatorSPDDlg, CDialog)
	//{{AFX_MSG_MAP(CSetGridCreatorSPDDlg)
	ON_BN_CLICKED(IDC_RADIO_GLOBAL, OnRadioGlobal)
	ON_BN_CLICKED(IDC_RADIO_FOURDIRECTION, OnRadioFourdirection)
	ON_BN_CLICKED(IDC_RADIO_EIGHTDIRECTION, OnRadioEightdirection)
	ON_EN_CHANGE(IDC_POINTCOUNT_PREDIRECTION, OnChangePointcountPredirection)
	ON_EN_CHANGE(IDC_POINTCOUNT_TOTAL, OnChangePointcountTotal)
	ON_EN_CHANGE(IDC_EDIT_SEARCHRADIUS, OnChangeEditSearchradius)
	ON_EN_CHANGE(IDC_EDIT_U, OnChangeEditU)
	ON_EN_CHANGE(IDC_EDIT_T, OnChangeEditT)
	ON_BN_CLICKED(IDC_RADIO_SAMPLEDISTANCE, OnRadioSampledistance)
	ON_BN_CLICKED(IDC_RADIO_CUSTOMDISTANCE, OnRadioCustomdistance)
	ON_EN_CHANGE(IDC_EDIT_MAXDISTANCE, OnChangeEditMaxdistance)
	ON_EN_CHANGE(IDC_EDIT_NUMX, OnChangeEditNumx)
	ON_EN_CHANGE(IDC_EDIT_NUMY, OnChangeEditNumy)
	ON_EN_CHANGE(IDC_EDIT_SIZEX, OnChangeEditSizex)
	ON_EN_CHANGE(IDC_EDIT_SIZEY, OnChangeEditSizey)
	ON_EN_CHANGE(IDC_EDIT_PATHANME, OnChangeEditPathanme)
	ON_BN_CLICKED(IDC_BUTTON_PATH, OnButtonPath)
	ON_BN_CLICKED(IDC_CHECK_RE, OnCheckRe)
	ON_BN_CLICKED(IDC_RADIO_KERNAL, OnRadioKernal)
	ON_BN_CLICKED(IDC_RADIO_KERNAL2, OnRadioKernal2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetGridCreatorSPDDlg message handlers

BOOL CSetGridCreatorSPDDlg::OnInitDialog() 
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

	m_dMaxDistance	= m_dSearchRadius;
	
	m_dDemSizeX	= (m_dMapRight-m_dMapLeft) / (m_iDemNumX-1);
	m_dDemSizeY	= (m_dMapTop-m_dMapBottom) / (m_iDemNumY-1);

	m_szPathName	= "D:\\Out(SPD).grd";

	if(m_bCustom == 0)	GetDlgItem(IDC_EDIT_MAXDISTANCE)->EnableWindow(FALSE);
	if(m_bCustom == 1)	GetDlgItem(IDC_EDIT_MAXDISTANCE)->EnableWindow(TRUE);

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSetGridCreatorSPDDlg::OnRadioGlobal() 
{
	m_iSearchType	= 0;

	GetDlgItem(IDC_POINTCOUNT_PREDIRECTION)->EnableWindow(FALSE);
	GetDlgItem(IDC_POINTCOUNT_TOTAL)->EnableWindow(TRUE);	
}

void CSetGridCreatorSPDDlg::OnRadioFourdirection() 
{
	m_iSearchType	= 1;
	
	GetDlgItem(IDC_POINTCOUNT_PREDIRECTION)->EnableWindow(TRUE);
	GetDlgItem(IDC_POINTCOUNT_TOTAL)->EnableWindow(FALSE);
}

void CSetGridCreatorSPDDlg::OnRadioEightdirection() 
{
	m_iSearchType	= 2;
	
	GetDlgItem(IDC_POINTCOUNT_PREDIRECTION)->EnableWindow(TRUE);
	GetDlgItem(IDC_POINTCOUNT_TOTAL)->EnableWindow(FALSE);
}

void CSetGridCreatorSPDDlg::OnChangePointcountPredirection() 
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

void CSetGridCreatorSPDDlg::OnChangePointcountTotal() 
{
	UpdateData(TRUE);
}

void CSetGridCreatorSPDDlg::OnChangeEditSearchradius() 
{
	UpdateData(TRUE);
}

void CSetGridCreatorSPDDlg::OnChangeEditU() 
{
	UpdateData(TRUE);
}

void CSetGridCreatorSPDDlg::OnChangeEditT() 
{
	UpdateData(TRUE);
}

void CSetGridCreatorSPDDlg::OnRadioSampledistance() 
{
	m_bCustom	= 0;	

	if(m_bCustom == 0)	GetDlgItem(IDC_EDIT_MAXDISTANCE)->EnableWindow(FALSE);
	if(m_bCustom == 1)	GetDlgItem(IDC_EDIT_MAXDISTANCE)->EnableWindow(TRUE);
}

void CSetGridCreatorSPDDlg::OnRadioCustomdistance() 
{
	m_bCustom	= 1;	

	if(m_bCustom == 0)	GetDlgItem(IDC_EDIT_MAXDISTANCE)->EnableWindow(FALSE);
	if(m_bCustom == 1)	GetDlgItem(IDC_EDIT_MAXDISTANCE)->EnableWindow(TRUE);
}

void CSetGridCreatorSPDDlg::OnChangeEditMaxdistance() 
{
	UpdateData(TRUE);
}

void CSetGridCreatorSPDDlg::OnChangeEditNumx() 
{
	UpdateData(TRUE);

	//关联尺寸
	if(m_iDemNumX > 1)
	{
		m_dDemSizeX	= (m_dMapRight-m_dMapLeft)/(m_iDemNumX-1);

		UpdateData(FALSE);
	}
}

void CSetGridCreatorSPDDlg::OnChangeEditNumy() 
{
	UpdateData(TRUE);

	//关联尺寸
	if(m_iDemNumY > 1)
	{
		m_dDemSizeY	= (m_dMapTop-m_dMapBottom)/(m_iDemNumY-1);

		UpdateData(FALSE);
	}	
}

void CSetGridCreatorSPDDlg::OnChangeEditSizex() 
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

void CSetGridCreatorSPDDlg::OnChangeEditSizey() 
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

void CSetGridCreatorSPDDlg::OnChangeEditPathanme() 
{
	UpdateData(TRUE);
}

void CSetGridCreatorSPDDlg::OnButtonPath() 
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

void CSetGridCreatorSPDDlg::OnCheckRe() 
{
	m_bCheckRE	= !m_bCheckRE;	
}

void CSetGridCreatorSPDDlg::OnRadioKernal() 
{
	m_iKernal	= 0;
}

void CSetGridCreatorSPDDlg::OnRadioKernal2() 
{
	m_iKernal	= 1;
}
