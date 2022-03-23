// SetWindownSize.cpp : implementation file
//

#include "stdafx.h"
//#include "vr_demcreator.h"
#include "SetWindownSize.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetWindownSize dialog


CSetWindownSize::CSetWindownSize(CWnd* pParent /*=NULL*/)
	: CDialog(CSetWindownSize::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetWindownSize)
	m_iSize = 5;
	//}}AFX_DATA_INIT
}


void CSetWindownSize::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetWindownSize)
	DDX_Text(pDX, IDC_EDIT_SIZE, m_iSize);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetWindownSize, CDialog)
	//{{AFX_MSG_MAP(CSetWindownSize)
	ON_EN_CHANGE(IDC_EDIT_SIZE, OnChangeEditSize)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetWindownSize message handlers

void CSetWindownSize::OnChangeEditSize() 
{
	UpdateData(TRUE);	
}
