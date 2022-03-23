// SetCalcHeightDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "vr_demcreator.h"
#include "SetCalcHeightDlg.h"

#include <direct.h>
#include "WzdDirDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetCalcHeightDlg dialog


CSetCalcHeightDlg::CSetCalcHeightDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetCalcHeightDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetCalcHeightDlg)
	m_bOnly = FALSE;
	m_szPathName = _T("");
	//}}AFX_DATA_INIT
}


void CSetCalcHeightDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetCalcHeightDlg)
	DDX_Check(pDX, IDC_ONLY, m_bOnly);
	DDX_Text(pDX, IDC_EDIT_PATHNAME, m_szPathName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetCalcHeightDlg, CDialog)
	//{{AFX_MSG_MAP(CSetCalcHeightDlg)
	ON_BN_CLICKED(IDC_ONLY, OnOnly)
	ON_BN_CLICKED(IDC_BTN_PATH, OnBtnPath)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetCalcHeightDlg message handlers

void CSetCalcHeightDlg::OnOnly() 
{
	m_bOnly	= !m_bOnly;			
}

void CSetCalcHeightDlg::OnBtnPath() 
{
	CWzdDirDlg dlg;
	
	char chBuffer[MAX_PATH];
	_getcwd(chBuffer,MAX_PATH);
	CString szDir	= dlg.GetDirectory(this,chBuffer);		

	if(szDir.IsEmpty())		return;

	//int		nLocate;

	//nLocate	= -1;
	//nLocate	= szDir.ReverseFind('\\');
	
	//if(nLocate == -1)		return;

	szFilePath	= szDir;
	//szFileName	= szDir.Right((szDir.GetLength() - nLocate - 1));
	
	m_szPathName		= szFilePath;

	UpdateData(FALSE);	
}

void CSetCalcHeightDlg::OnOK() 
{
	if(szFilePath.CompareNoCase("")==0)
	{
		MessageBox("请先输入路径!", "提示信息", MB_ICONEXCLAMATION  | MB_OK);
		return;
	}	

	int		nNumItem;
	CString	arrFileName[2048];

	nNumItem		= 0;
	
	//if(m_bOnly)
	{
		CString			strDirName;
		WIN32_FIND_DATA wfd;
		HANDLE			hFind;
		CString			strText;
		CString			strTitleName;

		strDirName	= szFilePath+"\\";
		strText		= strDirName + _T("*.grd");
		hFind		= FindFirstFile(strText, &wfd);
		
		if (hFind != INVALID_HANDLE_VALUE) 
		{
			do
			{
				if ((strcmp(wfd.cFileName,_T("."))) && 
				(wfd.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE)) 
				{
					strTitleName.Format("%s",wfd.cFileName);
					arrFileName[nNumItem]	= strTitleName;
					nNumItem++;
				}
			} while (FindNextFile(hFind, &wfd));
			FindClose(hFind);
		}
	}
	//else
	//{
	//	arrFileName[nNumItem]	= szFileName;
	//	nNumItem++;
	//}

	AfxGetApp()->DoWaitCursor(1);

	FILE	*fw;
	fw	= fopen(szFilePath+"\\Height.txt","w");
	
	for(int i=0;i<nNumItem;i++)
	{
		CString		szPathName;
		szPathName.Format("%s\\%s",szFilePath,arrFileName[i]);
		
		double	dMinHeight,dMaxHeight;
		FILE	*fp;
		fp	= fopen(szPathName,"r");
		if(fp != NULL)
		{
			//DSAA
			char	chInfo[64];
			fscanf(fp,"%s",chInfo);

			//iCols,iRows,应该判断清楚什么是行和列
			int		iNumOfX,iNumOfY;
			fscanf(fp,"%ld%ld",&iNumOfX,&iNumOfY);
			
			//dMinX,dMaxX
			double	dDemLeft,dDemRight;
			fscanf(fp,"%lf%lf",&dDemLeft,&dDemRight);

			//dMinY,dMaxY
			double	dDemBottom,dDemTop;
			fscanf(fp,"%lf%lf",&dDemBottom,&dDemTop);

			//dMinZ,dMaxZ
			fscanf(fp,"%lf%lf",&dMinHeight,&dMaxHeight);

			fclose(fp);
		}
		
		char	chInfo[32];
		strcpy(chInfo,arrFileName[i]);

		fprintf(fw,"%-32s ",chInfo);

		fprintf(fw,"%.5lf ",dMinHeight);
		fprintf(fw,"%.5lf\n",dMaxHeight);

		//m_pImport.ImportVersion(szPathName);
	}

	fclose(fw);

	AfxGetApp()->DoWaitCursor(-1);	
}
