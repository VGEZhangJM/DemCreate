// SetRECombineTEDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "vr_demcreator.h"
#include "SetRECombineTEDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetRECombineTEDlg dialog


CSetRECombineTEDlg::CSetRECombineTEDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetRECombineTEDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetRECombineTEDlg)
	m_szREPathName = _T("");
	m_szTEPathName = _T("");
	m_szComPathName = _T("");
	m_szREPathName2 = _T("");
	m_szREPathName3 = _T("");
	m_szREPathName4 = _T("");
	m_szREPathName5 = _T("");
	m_szREPathName6 = _T("");
	m_szTEPathName2 = _T("");
	m_szTEPathName4 = _T("");
	m_szTEPathName3 = _T("");
	m_szTEPathName5 = _T("");
	m_szTEPathName6 = _T("");
	m_bCheckPingYuan = FALSE;
	m_bCheckQiuLing = FALSE;
	m_bCheckDiShan = FALSE;
	m_bCheckZhongShan = FALSE;
	m_bCheckGaoShan = FALSE;
	m_bCheckHunHe = FALSE;
	m_iCombineType = -1;
	m_szLSDPathName = _T("");
	m_szLSDPathName2 = _T("");
	m_szLSDPathName3 = _T("");
	m_szLSDPathName4 = _T("");
	m_szLSDPathName5 = _T("");
	m_szLSDPathName6 = _T("");
	//}}AFX_DATA_INIT
}


void CSetRECombineTEDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetRECombineTEDlg)
	DDX_Text(pDX, IDC_EDIT_REPATHNAME, m_szREPathName);
	DDX_Text(pDX, IDC_EDIT_TEPATHNAME, m_szTEPathName);
	DDX_Text(pDX, IDC_EDIT_COMPATHNAME, m_szComPathName);
	DDX_Text(pDX, IDC_EDIT_REPATHNAME2, m_szREPathName2);
	DDX_Text(pDX, IDC_EDIT_REPATHNAME3, m_szREPathName3);
	DDX_Text(pDX, IDC_EDIT_REPATHNAME4, m_szREPathName4);
	DDX_Text(pDX, IDC_EDIT_REPATHNAME5, m_szREPathName5);
	DDX_Text(pDX, IDC_EDIT_REPATHNAME6, m_szREPathName6);
	DDX_Text(pDX, IDC_EDIT_TEPATHNAME2, m_szTEPathName2);
	DDX_Text(pDX, IDC_EDIT_TEPATHNAME4, m_szTEPathName4);
	DDX_Text(pDX, IDC_EDIT_TEPATHNAME3, m_szTEPathName3);
	DDX_Text(pDX, IDC_EDIT_TEPATHNAME5, m_szTEPathName5);
	DDX_Text(pDX, IDC_EDIT_TEPATHNAME6, m_szTEPathName6);
	DDX_Check(pDX, IDC_CHECK_PINGYUAN, m_bCheckPingYuan);
	DDX_Check(pDX, IDC_CHECK_QIULING, m_bCheckQiuLing);
	DDX_Check(pDX, IDC_CHECK_DISHAN, m_bCheckDiShan);
	DDX_Check(pDX, IDC_CHECK_ZHONGSHAN, m_bCheckZhongShan);
	DDX_Check(pDX, IDC_CHECK_GAOSHAN, m_bCheckGaoShan);
	DDX_Check(pDX, IDC_CHECK_HUNHE, m_bCheckHunHe);
	DDX_Radio(pDX, IDC_RADIO_TE, m_iCombineType);
	DDX_Text(pDX, IDC_EDIT_LSD, m_szLSDPathName);
	DDX_Text(pDX, IDC_EDIT_LSD2, m_szLSDPathName2);
	DDX_Text(pDX, IDC_EDIT_LSD3, m_szLSDPathName3);
	DDX_Text(pDX, IDC_EDIT_LSD4, m_szLSDPathName4);
	DDX_Text(pDX, IDC_EDIT_LSD5, m_szLSDPathName5);
	DDX_Text(pDX, IDC_EDIT_LSD6, m_szLSDPathName6);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetRECombineTEDlg, CDialog)
	//{{AFX_MSG_MAP(CSetRECombineTEDlg)
	ON_BN_CLICKED(IDC_BUTTON_RE, OnButtonRe)
	ON_BN_CLICKED(IDC_BUTTON_TE, OnButtonTe)
	ON_BN_CLICKED(IDC_BUTTON_COM, OnButtonCom)
	ON_BN_CLICKED(IDC_BUTTON_RE2, OnButtonRe2)
	ON_BN_CLICKED(IDC_BUTTON_RE3, OnButtonRe3)
	ON_BN_CLICKED(IDC_BUTTON_RE4, OnButtonRe4)
	ON_BN_CLICKED(IDC_BUTTON_RE5, OnButtonRe5)
	ON_BN_CLICKED(IDC_BUTTON_RE6, OnButtonRe6)
	ON_BN_CLICKED(IDC_BUTTON_TE2, OnButtonTe2)
	ON_BN_CLICKED(IDC_BUTTON_TE3, OnButtonTe3)
	ON_BN_CLICKED(IDC_BUTTON_TE4, OnButtonTe4)
	ON_BN_CLICKED(IDC_BUTTON_TE5, OnButtonTe5)
	ON_BN_CLICKED(IDC_BUTTON_TE6, OnButtonTe6)
	ON_BN_CLICKED(IDC_CHECK_PINGYUAN, OnCheckPingyuan)
	ON_BN_CLICKED(IDC_BUTTON_LSD, OnButtonLsd)
	ON_BN_CLICKED(IDC_CHECK_QIULING, OnCheckQiuling)
	ON_BN_CLICKED(IDC_BUTTON_LSD2, OnButtonLsd2)
	ON_BN_CLICKED(IDC_CHECK_DISHAN, OnCheckDishan)
	ON_BN_CLICKED(IDC_BUTTON_LSD3, OnButtonLsd3)
	ON_BN_CLICKED(IDC_CHECK_ZHONGSHAN, OnCheckZhongshan)
	ON_BN_CLICKED(IDC_BUTTON_LSD4, OnButtonLsd4)
	ON_BN_CLICKED(IDC_CHECK_GAOSHAN, OnCheckGaoshan)
	ON_BN_CLICKED(IDC_BUTTON_LSD5, OnButtonLsd5)
	ON_BN_CLICKED(IDC_CHECK_HUNHE, OnCheckHunhe)
	ON_BN_CLICKED(IDC_BUTTON_LSD6, OnButtonLsd6)
	ON_BN_CLICKED(IDC_RADIO_TE, OnRadioTe)
	ON_BN_CLICKED(IDC_RADIO_LSD, OnRadioLsd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetRECombineTEDlg message handlers
BOOL CSetRECombineTEDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_iCombineType	= 0;

	if(m_iCombineType == 0)
	{
		GetDlgItem(IDC_BUTTON_TE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_TEPATHNAME)->EnableWindow(TRUE);

		GetDlgItem(IDC_BUTTON_TE2)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_TEPATHNAME2)->EnableWindow(TRUE);

		GetDlgItem(IDC_BUTTON_TE3)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_TEPATHNAME3)->EnableWindow(TRUE);

		GetDlgItem(IDC_BUTTON_TE4)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_TEPATHNAME4)->EnableWindow(TRUE);

		GetDlgItem(IDC_BUTTON_TE5)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_TEPATHNAME5)->EnableWindow(TRUE);

		GetDlgItem(IDC_BUTTON_TE6)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_TEPATHNAME6)->EnableWindow(TRUE);


		GetDlgItem(IDC_BUTTON_LSD)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_LSD)->EnableWindow(FALSE);

		GetDlgItem(IDC_BUTTON_LSD2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_LSD2)->EnableWindow(FALSE);

		GetDlgItem(IDC_BUTTON_LSD3)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_LSD3)->EnableWindow(FALSE);

		GetDlgItem(IDC_BUTTON_LSD4)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_LSD4)->EnableWindow(FALSE);

		GetDlgItem(IDC_BUTTON_LSD5)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_LSD5)->EnableWindow(FALSE);

		GetDlgItem(IDC_BUTTON_LSD6)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_LSD6)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_TE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TEPATHNAME)->EnableWindow(FALSE);

		GetDlgItem(IDC_BUTTON_TE2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TEPATHNAME2)->EnableWindow(FALSE);

		GetDlgItem(IDC_BUTTON_TE3)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TEPATHNAME3)->EnableWindow(FALSE);

		GetDlgItem(IDC_BUTTON_TE4)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TEPATHNAME4)->EnableWindow(FALSE);

		GetDlgItem(IDC_BUTTON_TE5)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TEPATHNAME5)->EnableWindow(FALSE);

		GetDlgItem(IDC_BUTTON_TE6)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TEPATHNAME6)->EnableWindow(FALSE);


		GetDlgItem(IDC_BUTTON_LSD)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_LSD)->EnableWindow(TRUE);

		GetDlgItem(IDC_BUTTON_LSD2)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_LSD2)->EnableWindow(TRUE);

		GetDlgItem(IDC_BUTTON_LSD3)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_LSD3)->EnableWindow(TRUE);

		GetDlgItem(IDC_BUTTON_LSD4)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_LSD4)->EnableWindow(TRUE);

		GetDlgItem(IDC_BUTTON_LSD5)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_LSD5)->EnableWindow(TRUE);

		GetDlgItem(IDC_BUTTON_LSD6)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_LSD6)->EnableWindow(TRUE);
	}

	UpdateData(FALSE);	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSetRECombineTEDlg::OnRadioTe() 
{
	m_iCombineType	= 0;	

	if(m_iCombineType == 0)
	{
		GetDlgItem(IDC_BUTTON_TE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_TEPATHNAME)->EnableWindow(TRUE);

		GetDlgItem(IDC_BUTTON_TE2)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_TEPATHNAME2)->EnableWindow(TRUE);

		GetDlgItem(IDC_BUTTON_TE3)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_TEPATHNAME3)->EnableWindow(TRUE);

		GetDlgItem(IDC_BUTTON_TE4)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_TEPATHNAME4)->EnableWindow(TRUE);

		GetDlgItem(IDC_BUTTON_TE5)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_TEPATHNAME5)->EnableWindow(TRUE);

		GetDlgItem(IDC_BUTTON_TE6)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_TEPATHNAME6)->EnableWindow(TRUE);


		GetDlgItem(IDC_BUTTON_LSD)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_LSD)->EnableWindow(FALSE);

		GetDlgItem(IDC_BUTTON_LSD2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_LSD2)->EnableWindow(FALSE);

		GetDlgItem(IDC_BUTTON_LSD3)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_LSD3)->EnableWindow(FALSE);

		GetDlgItem(IDC_BUTTON_LSD4)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_LSD4)->EnableWindow(FALSE);

		GetDlgItem(IDC_BUTTON_LSD5)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_LSD5)->EnableWindow(FALSE);

		GetDlgItem(IDC_BUTTON_LSD6)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_LSD6)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_TE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TEPATHNAME)->EnableWindow(FALSE);

		GetDlgItem(IDC_BUTTON_TE2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TEPATHNAME2)->EnableWindow(FALSE);

		GetDlgItem(IDC_BUTTON_TE3)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TEPATHNAME3)->EnableWindow(FALSE);

		GetDlgItem(IDC_BUTTON_TE4)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TEPATHNAME4)->EnableWindow(FALSE);

		GetDlgItem(IDC_BUTTON_TE5)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TEPATHNAME5)->EnableWindow(FALSE);

		GetDlgItem(IDC_BUTTON_TE6)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TEPATHNAME6)->EnableWindow(FALSE);


		GetDlgItem(IDC_BUTTON_LSD)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_LSD)->EnableWindow(TRUE);

		GetDlgItem(IDC_BUTTON_LSD2)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_LSD2)->EnableWindow(TRUE);

		GetDlgItem(IDC_BUTTON_LSD3)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_LSD3)->EnableWindow(TRUE);

		GetDlgItem(IDC_BUTTON_LSD4)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_LSD4)->EnableWindow(TRUE);

		GetDlgItem(IDC_BUTTON_LSD5)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_LSD5)->EnableWindow(TRUE);

		GetDlgItem(IDC_BUTTON_LSD6)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_LSD6)->EnableWindow(TRUE);
	}

	UpdateData(FALSE);	
}

void CSetRECombineTEDlg::OnRadioLsd() 
{
	m_iCombineType	= 1;	

	if(m_iCombineType == 0)
	{
		GetDlgItem(IDC_BUTTON_TE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_TEPATHNAME)->EnableWindow(TRUE);

		GetDlgItem(IDC_BUTTON_TE2)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_TEPATHNAME2)->EnableWindow(TRUE);

		GetDlgItem(IDC_BUTTON_TE3)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_TEPATHNAME3)->EnableWindow(TRUE);

		GetDlgItem(IDC_BUTTON_TE4)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_TEPATHNAME4)->EnableWindow(TRUE);

		GetDlgItem(IDC_BUTTON_TE5)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_TEPATHNAME5)->EnableWindow(TRUE);

		GetDlgItem(IDC_BUTTON_TE6)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_TEPATHNAME6)->EnableWindow(TRUE);


		GetDlgItem(IDC_BUTTON_LSD)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_LSD)->EnableWindow(FALSE);

		GetDlgItem(IDC_BUTTON_LSD2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_LSD2)->EnableWindow(FALSE);

		GetDlgItem(IDC_BUTTON_LSD3)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_LSD3)->EnableWindow(FALSE);

		GetDlgItem(IDC_BUTTON_LSD4)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_LSD4)->EnableWindow(FALSE);

		GetDlgItem(IDC_BUTTON_LSD5)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_LSD5)->EnableWindow(FALSE);

		GetDlgItem(IDC_BUTTON_LSD6)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_LSD6)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_TE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TEPATHNAME)->EnableWindow(FALSE);

		GetDlgItem(IDC_BUTTON_TE2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TEPATHNAME2)->EnableWindow(FALSE);

		GetDlgItem(IDC_BUTTON_TE3)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TEPATHNAME3)->EnableWindow(FALSE);

		GetDlgItem(IDC_BUTTON_TE4)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TEPATHNAME4)->EnableWindow(FALSE);

		GetDlgItem(IDC_BUTTON_TE5)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TEPATHNAME5)->EnableWindow(FALSE);

		GetDlgItem(IDC_BUTTON_TE6)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TEPATHNAME6)->EnableWindow(FALSE);


		GetDlgItem(IDC_BUTTON_LSD)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_LSD)->EnableWindow(TRUE);

		GetDlgItem(IDC_BUTTON_LSD2)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_LSD2)->EnableWindow(TRUE);

		GetDlgItem(IDC_BUTTON_LSD3)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_LSD3)->EnableWindow(TRUE);

		GetDlgItem(IDC_BUTTON_LSD4)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_LSD4)->EnableWindow(TRUE);

		GetDlgItem(IDC_BUTTON_LSD5)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_LSD5)->EnableWindow(TRUE);

		GetDlgItem(IDC_BUTTON_LSD6)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_LSD6)->EnableWindow(TRUE);
	}

	UpdateData(FALSE);		
}

/////////////////////////////////////////////////////////////////////////////
//平原
void CSetRECombineTEDlg::OnCheckPingyuan() 
{
	m_bCheckPingYuan	= !m_bCheckPingYuan;
}

void CSetRECombineTEDlg::OnButtonRe() 
{
	CFileDialog	dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		"残差文本数据文件(*.txt)|*.txt||");
	dlg.m_ofn.lpstrTitle="打开残差文本数据文件";
	
	CString	szPathName;

	if(dlg.DoModal()==IDOK)
	{
		m_szREPathName	= dlg.GetPathName();
	}
	else
		return;
	
	UpdateData(FALSE);		
}

void CSetRECombineTEDlg::OnButtonTe() 
{
	CFileDialog	dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		"局部地形特征文本数据文件(*.txt)|*.txt||");
	dlg.m_ofn.lpstrTitle="打开局部地形特征文本数据文件";
	
	CString	szPathName;

	if(dlg.DoModal()==IDOK)
	{
		m_szTEPathName	= dlg.GetPathName();
	}
	else
		return;
	
	UpdateData(FALSE);			
}

void CSetRECombineTEDlg::OnButtonLsd() 
{
	CFileDialog	dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		"隶属度值文本数据文件(*.txt)|*.txt||");
	dlg.m_ofn.lpstrTitle="打开隶属度值文本数据文件";
	
	CString	szPathName;

	if(dlg.DoModal()==IDOK)
	{
		m_szLSDPathName	= dlg.GetPathName();
	}
	else
		return;
	
	UpdateData(FALSE);				
}
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
//丘陵
void CSetRECombineTEDlg::OnCheckQiuling() 
{
	m_bCheckQiuLing	= !m_bCheckQiuLing;
}

void CSetRECombineTEDlg::OnButtonRe2() 
{
	CFileDialog	dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		"残差文本数据文件(*.txt)|*.txt||");
	dlg.m_ofn.lpstrTitle="打开残差文本数据文件";
	
	CString	szPathName;

	if(dlg.DoModal()==IDOK)
	{
		m_szREPathName2	= dlg.GetPathName();
	}
	else
		return;
	
	UpdateData(FALSE);			
}

void CSetRECombineTEDlg::OnButtonTe2() 
{
	CFileDialog	dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		"局部地形特征文本数据文件(*.txt)|*.txt||");
	dlg.m_ofn.lpstrTitle="打开局部地形特征文本数据文件";
	
	CString	szPathName;

	if(dlg.DoModal()==IDOK)
	{
		m_szTEPathName2	= dlg.GetPathName();
	}
	else
		return;
	
	UpdateData(FALSE);				
}

void CSetRECombineTEDlg::OnButtonLsd2() 
{
	CFileDialog	dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		"隶属度值文本数据文件(*.txt)|*.txt||");
	dlg.m_ofn.lpstrTitle="打开隶属度值文本数据文件";
	
	CString	szPathName;

	if(dlg.DoModal()==IDOK)
	{
		m_szLSDPathName2	= dlg.GetPathName();
	}
	else
		return;
	
	UpdateData(FALSE);				
}
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
//低山
void CSetRECombineTEDlg::OnCheckDishan() 
{
	m_bCheckDiShan	= !m_bCheckDiShan;	
}

void CSetRECombineTEDlg::OnButtonRe3() 
{
	CFileDialog	dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		"残差文本数据文件(*.txt)|*.txt||");
	dlg.m_ofn.lpstrTitle="打开残差文本数据文件";
	
	CString	szPathName;

	if(dlg.DoModal()==IDOK)
	{
		m_szREPathName3	= dlg.GetPathName();
	}
	else
		return;
	
	UpdateData(FALSE);			
}

void CSetRECombineTEDlg::OnButtonTe3() 
{
	CFileDialog	dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		"局部地形特征文本数据文件(*.txt)|*.txt||");
	dlg.m_ofn.lpstrTitle="打开局部地形特征文本数据文件";
	
	CString	szPathName;

	if(dlg.DoModal()==IDOK)
	{
		m_szTEPathName3	= dlg.GetPathName();
	}
	else
		return;
	
	UpdateData(FALSE);				
	
}

void CSetRECombineTEDlg::OnButtonLsd3() 
{
	CFileDialog	dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		"隶属度值文本数据文件(*.txt)|*.txt||");
	dlg.m_ofn.lpstrTitle="打开隶属度值文本数据文件";
	
	CString	szPathName;

	if(dlg.DoModal()==IDOK)
	{
		m_szLSDPathName3	= dlg.GetPathName();
	}
	else
		return;
	
	UpdateData(FALSE);					
}
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
//中山
void CSetRECombineTEDlg::OnCheckZhongshan() 
{
	m_bCheckZhongShan	= !m_bCheckZhongShan;	
}

void CSetRECombineTEDlg::OnButtonRe4() 
{
	CFileDialog	dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		"残差文本数据文件(*.txt)|*.txt||");
	dlg.m_ofn.lpstrTitle="打开残差文本数据文件";
	
	CString	szPathName;

	if(dlg.DoModal()==IDOK)
	{
		m_szREPathName4	= dlg.GetPathName();
	}
	else
		return;
	
	UpdateData(FALSE);			
}

void CSetRECombineTEDlg::OnButtonTe4() 
{
	CFileDialog	dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		"局部地形特征文本数据文件(*.txt)|*.txt||");
	dlg.m_ofn.lpstrTitle="打开局部地形特征文本数据文件";
	
	CString	szPathName;

	if(dlg.DoModal()==IDOK)
	{
		m_szTEPathName4	= dlg.GetPathName();
	}
	else
		return;
	
	UpdateData(FALSE);				
	
}

void CSetRECombineTEDlg::OnButtonLsd4() 
{
	CFileDialog	dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		"隶属度值文本数据文件(*.txt)|*.txt||");
	dlg.m_ofn.lpstrTitle="打开隶属度值文本数据文件";
	
	CString	szPathName;

	if(dlg.DoModal()==IDOK)
	{
		m_szLSDPathName4	= dlg.GetPathName();
	}
	else
		return;
	
	UpdateData(FALSE);				
}
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
//高山
void CSetRECombineTEDlg::OnCheckGaoshan() 
{
	m_bCheckGaoShan	= !m_bCheckGaoShan;	
}

void CSetRECombineTEDlg::OnButtonRe5() 
{
	CFileDialog	dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		"残差文本数据文件(*.txt)|*.txt||");
	dlg.m_ofn.lpstrTitle="打开残差文本数据文件";
	
	CString	szPathName;

	if(dlg.DoModal()==IDOK)
	{
		m_szREPathName5	= dlg.GetPathName();
	}
	else
		return;
	
	UpdateData(FALSE);			
}

void CSetRECombineTEDlg::OnButtonTe5() 
{
	CFileDialog	dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		"局部地形特征文本数据文件(*.txt)|*.txt||");
	dlg.m_ofn.lpstrTitle="打开局部地形特征文本数据文件";
	
	CString	szPathName;

	if(dlg.DoModal()==IDOK)
	{
		m_szTEPathName5	= dlg.GetPathName();
	}
	else
		return;
	
	UpdateData(FALSE);				
	
}

void CSetRECombineTEDlg::OnButtonLsd5() 
{
	CFileDialog	dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		"隶属度值文本数据文件(*.txt)|*.txt||");
	dlg.m_ofn.lpstrTitle="打开隶属度值文本数据文件";
	
	CString	szPathName;

	if(dlg.DoModal()==IDOK)
	{
		m_szLSDPathName5	= dlg.GetPathName();
	}
	else
		return;
	
	UpdateData(FALSE);					
}
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
//混合
void CSetRECombineTEDlg::OnCheckHunhe() 
{
	m_bCheckHunHe	= !m_bCheckHunHe;	
}

void CSetRECombineTEDlg::OnButtonRe6() 
{
	CFileDialog	dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		"残差文本数据文件(*.txt)|*.txt||");
	dlg.m_ofn.lpstrTitle="打开残差文本数据文件";
	
	CString	szPathName;

	if(dlg.DoModal()==IDOK)
	{
		m_szREPathName6	= dlg.GetPathName();
	}
	else
		return;
	
	UpdateData(FALSE);			
}

void CSetRECombineTEDlg::OnButtonTe6() 
{
	CFileDialog	dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		"局部地形特征文本数据文件(*.txt)|*.txt||");
	dlg.m_ofn.lpstrTitle="打开局部地形特征文本数据文件";
	
	CString	szPathName;

	if(dlg.DoModal()==IDOK)
	{
		m_szTEPathName6	= dlg.GetPathName();
	}
	else
		return;
	
	UpdateData(FALSE);				
	
}

void CSetRECombineTEDlg::OnButtonLsd6() 
{
	CFileDialog	dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		"隶属度值文本数据文件(*.txt)|*.txt||");
	dlg.m_ofn.lpstrTitle="打开隶属度值文本数据文件";
	
	CString	szPathName;

	if(dlg.DoModal()==IDOK)
	{
		m_szLSDPathName6	= dlg.GetPathName();
	}
	else
		return;
	
	UpdateData(FALSE);				
}
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
//
void CSetRECombineTEDlg::OnButtonCom() 
{
	CFileDialog	dlg(FALSE,_T("ltf"),NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		"局部地形特征数据文件(*.ltf)|*.ltf||");
	dlg.m_ofn.lpstrTitle="保存局部地形特征数据文件";
	
	CString	szPathName;

	if(dlg.DoModal()==IDOK)
	{
		m_szComPathName	= dlg.GetPathName();
	}
	else
		return;
	
	UpdateData(FALSE);		
}


void CSetRECombineTEDlg::OnOK() 
{
	//if(m_szREPathName.IsEmpty())	return;
	//if(m_szTEPathName.IsEmpty())	return;
	
	AfxGetApp()->DoWaitCursor(1);

	if(m_szComPathName.IsEmpty())
	{
		MessageBox("请先输入目标文件名!","提示信息",MB_OK);

		return;
	}

	//地形特征组合的
	if(m_iCombineType == 0)
	{
		//首先记录文件名,有六个
		//{
			CString szREPathName[6];
			CString szTCPathName[6];
			bool	bCheckType[6];

			//平原
			szREPathName[0]	= m_szREPathName;
			szTCPathName[0]	= m_szTEPathName;
			if(m_bCheckPingYuan == TRUE)
				bCheckType[0]	= true;
			else	
				bCheckType[0]	= false;

			//丘陵
			szREPathName[1]	= m_szREPathName2;
			szTCPathName[1]	= m_szTEPathName2;
			if(m_bCheckQiuLing == TRUE)
				bCheckType[1]	= true;
			else	
				bCheckType[1]	= false;

			//低山
			szREPathName[2]	= m_szREPathName3;
			szTCPathName[2]	= m_szTEPathName3;
			if(m_bCheckDiShan == TRUE)
				bCheckType[2]	= true;
			else	
				bCheckType[2]	= false;

			//中山
			szREPathName[3]	= m_szREPathName4;
			szTCPathName[3]	= m_szTEPathName4;
			if(m_bCheckZhongShan == TRUE)
				bCheckType[3]	= true;
			else	
				bCheckType[3]	= false;

			//高山
			szREPathName[4]	= m_szREPathName5;
			szTCPathName[4]	= m_szTEPathName5;
			if(m_bCheckGaoShan == TRUE)
				bCheckType[4]	= true;
			else	
				bCheckType[4]	= false;

			//混合
			szREPathName[5]	= m_szREPathName6;
			szTCPathName[5]	= m_szTEPathName6;
			if(m_bCheckHunHe == TRUE)
				bCheckType[5]	= true;
			else	
				bCheckType[5]	= false;
			
			//组合文件
			CString szPathName;
			szPathName		= m_szComPathName;
		//}


		//最大残差
		double	dMaxDH;
		//最大最小距离
		double	dMaxMinDistance;

		//
		FILE	*fw;

		//首先写入头信息
		fw	= fopen(szPathName,"w");
		if(fw == NULL)		return;
		fprintf(fw," %s\t %s\t %s\t %s\t %s\t %s\n","平均坡度","点格局","最小距离","残差","X坐标","Y坐标");
		
		//
		bool	bFirstMaxDH			= true;
		bool	bFirstMinDistance	= true;

		for(int i=0;i<6;i++)
		{
			//这里的残差值是没有意义的，应该继续下一步
			if(bCheckType[i] == true)	continue;

			//验证文件中药是的数目是否相等
			{
				//读取残差数据
				FILE	*frre;
				//读取TE数据
				FILE	*frtc;
				
				//没有文件，也是返回继续下一步的
				if(szREPathName[i].IsEmpty()) continue;
				frre	= fopen(szREPathName[i],"r");
				if(frre == NULL)	continue;


				//没有文件，也是返回继续下一步的
				if(szTCPathName[i].IsEmpty()) continue;
				frtc	= fopen(szTCPathName[i],"r");
				if(frtc == NULL)	continue;

				//计算残差文件中的数目
				int	iRENum;
				iRENum	= 0;
				
				//
				{
					double	dX,dY,dH,dDH;
					while(fscanf(frre,"%lf%lf%lf%lf",&dX,&dY,&dH,&dDH) == 4)
					{
						
						if(bFirstMaxDH == true)
						{
							dMaxDH	= fabs(dDH);

							bFirstMaxDH	= false;
						}
						else
						{
							if(dMaxDH < fabs(dDH))	dMaxDH	= fabs(dDH);
						}
						

						iRENum++;
					}		
					fclose(frre);
				}

				//
				int	iTENum;
				iTENum	= 0;
				
				//计算地形特征文件中的数目
				{
					//
					char	chName[64];
					fscanf(frtc,"%s%s%s%s%s%s%s%s",chName,chName,chName,chName,chName,chName,chName,chName);
				
					//横坐标	 纵坐标	 点格局值	 质心值	 平均坡度	 粗糙度		最大距离	最小距离

					double	dX,dY;
					double	dMaxDistance,dPointPattern,dCentroid,dSlope,dRoughness,dMinDistance;
					while(fscanf(frtc,"%lf%lf%lf%lf%lf%lf%lf%lf",&dX,&dY,&dPointPattern,&dCentroid,&dSlope,&dRoughness,&dMaxDistance,&dMinDistance) == 8)
					{
						
						if(bFirstMinDistance == true)
						{
							dMaxMinDistance	= dMinDistance;

							bFirstMinDistance	= false;
						}
						else
						{
							if(dMaxMinDistance < dMinDistance)		dMaxMinDistance	= dMinDistance;
						}
						

						iTENum++;
					}
					fclose(frtc);
				}

				if(iRENum != iTENum)
				{
					MessageBox("数目不符","提示信息",MB_OK);
					continue;
				}
			}//
		}

		//写入信息
		for(i=0;i<6;i++)		
		{
			//读取残差数据
			FILE	*frre;
			//读取TE数据
			FILE	*frtc;

			//
			if(szREPathName[i].IsEmpty()) continue;
			frre	= fopen(szREPathName[i],"r");
			if(frre == NULL)		continue;

			//
			if(szTCPathName[i].IsEmpty()) continue;
			frtc	= fopen(szTCPathName[i],"r");
			if(frtc == NULL)		continue;

			//
			double	dX,dY,dH,dDH;
			char	chName[64];

			//
			//横坐标	 纵坐标	 点格局值	 质心值	 平均坡度	 粗糙度		最大距离	最小距离
			fscanf(frtc,"%s%s%s%s%s%s%s%s",chName,chName,chName,chName,chName,chName,chName,chName);

			//
			while(fscanf(frre,"%lf%lf%lf%lf",&dX,&dY,&dH,&dDH) == 4)
			{
				double	dX2,dY2;
				double	dMaxDistance,dMinDistance,dPointPattern,dCentroid,dSlope,dRoughness;
				fscanf(frtc,"%lf%lf%lf%lf%lf%lf%lf%lf",&dX2,&dY2,&dPointPattern,&dCentroid,&dSlope,&dRoughness,&dMaxDistance,&dMinDistance);
				
				if(dSlope != -32767.0)
				{
					if(bCheckType[i] == true)
					{
						fprintf(fw," %lf\t %lf\t %lf\t %lf\t %lf\t %lf\t\n",dSlope/(PI/2),dPointPattern/2.149,dMinDistance/dMaxDistance,INVALID_VALUE,dX,dY);
					}
					else
					{
						//"平均坡度","点格局","最小距离","残差","X坐标","Y坐标"
						//dSlope	= dSlope * 180 /PI;
						//fprintf(fw," %lf\t %lf\t %lf\t %lf\t %lf\t %lf\t\n",dSlope/(PI/2),dPointPattern/2.149,dMinDistance/dMaxMinDistance,fabs(dDH)/dMaxDH,dX,dY);
						//第一种距离方式
						//fprintf(fw," %lf\t %lf\t %lf\t %lf\t %lf\t %lf\t\n",dSlope/(PI/2),dPointPattern/2.149,dMinDistance/dMaxDistance,fabs(dDH)/dMaxDH,dX,dY);
						//应使用残差绝对值的形式，因为这是横向比较
						fprintf(fw," %lf\t %lf\t %lf\t %lf\t %lf\t %lf\t\n",dSlope/(PI/2),dPointPattern/2.149,dMinDistance/dMaxDistance,fabs(dDH),dX,dY);
						//第二种距离方式
						//fprintf(fw," %lf\t %lf\t %lf\t %lf\t %lf\t %lf\t\n",dSlope/(PI/2),dPointPattern/2.149,dMinDistance/dMaxMinDistance,dDH/dMaxDH,dX,dY);
						//第三种距离方式
						//fprintf(fw," %lf\t %lf\t %lf\t %lf\t %lf\t %lf\t\n",dSlope/(PI/2),dPointPattern/2.149,sqrt((dMinDistance/dMaxMinDistance)*(dMinDistance/dMaxDistance)),fabs(dDH)/dMaxDH,dX,dY);
					}
				}
			}
			
			fclose(frre);
			
			fclose(frtc);
		}

		fclose(fw);

		CString	szInfo;
		szInfo.Format("最大残差值=%.2lf 米,处理完毕!",dMaxDH);
		MessageBox(szInfo,"提示信息",MB_OK);
	}

	//隶属度值
	if(m_iCombineType == 1)
	{
		//首先记录文件名,有六个
		//{
			CString szREPathName[6];
			CString szLSDPathName[6];
			bool	bCheckType[6];

			//平原
			szREPathName[0]		= m_szREPathName;
			szLSDPathName[0]	= m_szLSDPathName;
			if(m_bCheckPingYuan == TRUE)
				bCheckType[0]	= true;
			else	
				bCheckType[0]	= false;

			//丘陵
			szREPathName[1]		= m_szREPathName2;
			szLSDPathName[1]	= m_szLSDPathName2;
			if(m_bCheckQiuLing == TRUE)
				bCheckType[1]	= true;
			else	
				bCheckType[1]	= false;

			//低山
			szREPathName[2]		= m_szREPathName3;
			szLSDPathName[2]	= m_szLSDPathName3;
			if(m_bCheckDiShan == TRUE)
				bCheckType[2]	= true;
			else	
				bCheckType[2]	= false;

			//中山
			szREPathName[3]		= m_szREPathName4;
			szLSDPathName[3]	= m_szLSDPathName4;
			if(m_bCheckZhongShan == TRUE)
				bCheckType[3]	= true;
			else	
				bCheckType[3]	= false;

			//高山
			szREPathName[4]		= m_szREPathName5;
			szLSDPathName[4]	= m_szLSDPathName5;
			if(m_bCheckGaoShan == TRUE)
				bCheckType[4]	= true;
			else	
				bCheckType[4]	= false;

			//混合
			szREPathName[5]		= m_szREPathName6;
			szLSDPathName[5]	= m_szLSDPathName6;
			if(m_bCheckHunHe == TRUE)
				bCheckType[5]	= true;
			else	
				bCheckType[5]	= false;
			
			//组合文件
			CString szPathName;
			szPathName		= m_szComPathName;
		//}


		//最大残差
		double	dMaxDH;

		//
		FILE	*fw;

		//首先写入头信息
		fw	= fopen(szPathName,"w");
		if(fw == NULL)		return;
		fprintf(fw," %s\t %s\t %s\t %s\t %s\t %s\n","平原隶属度","丘陵隶属度","山地隶属度","残差","X坐标","Y坐标");
		
		//
		bool	bFirstMaxDH			= true;
		bool	bFirstMinDistance	= true;

		for(int i=0;i<6;i++)
		{
			//这里的残差值是没有意义的，应该继续下一步
			if(bCheckType[i] == true)	continue;

			//验证文件中药是的数目是否相等
			{
				//读取残差数据
				FILE	*frre;
				//读取TE数据
				FILE	*frtc;
				
				//没有文件，也是返回继续下一步的
				if(szREPathName[i].IsEmpty()) continue;
				frre	= fopen(szREPathName[i],"r");
				if(frre == NULL)	continue;


				//没有文件，也是返回继续下一步的
				if(szLSDPathName[i].IsEmpty()) continue;
				frtc	= fopen(szLSDPathName[i],"r");
				if(frtc == NULL)	continue;

				//计算残差文件中的数目
				int	iRENum;
				iRENum	= 0;
				
				//
				{
					double	dX,dY,dH,dDH;
					while(fscanf(frre,"%lf%lf%lf%lf",&dX,&dY,&dH,&dDH) == 4)
					{
						
						if(bFirstMaxDH == true)
						{
							dMaxDH	= fabs(dDH);

							bFirstMaxDH	= false;
						}
						else
						{
							if(dMaxDH < fabs(dDH))	dMaxDH	= fabs(dDH);
						}
						

						iRENum++;
					}		
					fclose(frre);
				}

				//
				int	iTENum;
				iTENum	= 0;
				
				//计算地形特征文件中的数目
				{
					//
					char	chName[64];
					fscanf(frtc,"%s%s%s%s%s",chName,chName,chName,chName,chName);
				
					//横坐标	 纵坐标	 点格局值	 质心值	 平均坡度	 粗糙度		最大距离	最小距离

					double	dX,dY;
					double	dLSDPingYuan,dLSDQiuLing,dLSDShanDi;
					while(fscanf(frtc,"%lf%lf%lf%lf%lf",&dX,&dY,&dLSDPingYuan,&dLSDQiuLing,&dLSDShanDi) == 5)
					{
						iTENum++;
					}
					fclose(frtc);
				}

				if(iRENum != iTENum)
				{
					MessageBox("数目不符","提示信息",MB_OK);
					continue;
				}
			}//
		}

		//写入信息
		for(i=0;i<6;i++)		
		{
			//读取残差数据
			FILE	*frre;
			//读取TE数据
			FILE	*frtc;

			//
			if(szREPathName[i].IsEmpty()) continue;
			frre	= fopen(szREPathName[i],"r");
			if(frre == NULL)		continue;

			//
			if(szLSDPathName[i].IsEmpty()) continue;
			frtc	= fopen(szLSDPathName[i],"r");
			if(frtc == NULL)		continue;

			//
			double	dX,dY,dH,dDH;
			char	chName[64];

			//
			//横坐标	 纵坐标	 点格局值	 质心值	 平均坡度	 粗糙度		最大距离	最小距离
			fscanf(frtc,"%s%s%s%s%s",chName,chName,chName,chName,chName);

			//
			while(fscanf(frre,"%lf%lf%lf%lf",&dX,&dY,&dH,&dDH) == 4)
			{
				double	dX2,dY2;
				double	dLSDPingYuan,dLSDQiuLing,dLSDShanDi;
				fscanf(frtc,"%lf%lf%lf%lf%lf",&dX2,&dY2,&dLSDPingYuan,&dLSDQiuLing,&dLSDShanDi);
				
				//if(dSlope != -32767.0)
				{
					if(bCheckType[i] == true)
					{
						fprintf(fw," %lf\t %lf\t %lf\t %lf\t %lf\t %lf\t\n",dLSDPingYuan,dLSDQiuLing,dLSDShanDi,INVALID_VALUE,dX,dY);
					}
					else
					{
						//"平原隶属度","丘陵隶属度","山地隶属度","残差","X坐标","Y坐标"
						fprintf(fw," %lf\t %lf\t %lf\t %lf\t %lf\t %lf\t\n",dLSDPingYuan,dLSDQiuLing,dLSDShanDi,fabs(dDH),dX,dY);
					}
				}
			}
			
			fclose(frre);
			
			fclose(frtc);
		}

		fclose(fw);

		CString	szInfo;
		szInfo.Format("最大残差值=%.2lf 米,处理完毕!",dMaxDH);
		MessageBox(szInfo,"提示信息",MB_OK);		
	}
	
	AfxGetApp()->DoWaitCursor(-1);
	//CDialog::OnOK();
}
