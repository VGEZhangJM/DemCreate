// WzdDrDlg.cpp: implementation of the CWzdDirDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WzdDirDlg.h"
#include "Shlobj.h"

CString CWzdDirDlg::m_sRootDir;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWzdDirDlg::CWzdDirDlg()
{

}

CWzdDirDlg::~CWzdDirDlg()
{

}

CString CWzdDirDlg::GetDirectory(CWnd *pParent,LPCSTR lpszRoot,LPCSTR lpszTitle)
{
	CString str;
	BROWSEINFO bi;
    bi.hwndOwner=pParent->m_hWnd;	//owner of created dialog box
    bi.pidlRoot=0;					//unused
    bi.pszDisplayName=0;			//buffer to receive name displayed by folder (not a valid path)
    bi.lpszTitle=lpszTitle;			//title is "Browse for Folder", this is an instruction
	bi.lpfn = BrowseCallbackProc;	//callback routine called when dialog has been initialized
    bi.lParam=0;					//passed to callback routine
    bi.ulFlags=
		BIF_RETURNONLYFSDIRS |		//only allow user to select a directory
		BIF_STATUSTEXT |			//create status text field we will be writing to in callback
//		BIF_BROWSEFORCOMPUTER|		//only allow user to select a computer
//		BIF_BROWSEFORPRINTER |		//only allow user to select a printer
//		BIF_BROWSEINCLUDEFILES|		//displays files too which user can pick
//		BIF_DONTGOBELOWDOMAIN|		//when user is exploring the "Entire Network" they
									// are not allowed into any domain
		0; 
	m_sRootDir=lpszRoot;

	LPITEMIDLIST lpItemId=::SHBrowseForFolder(&bi); 
	if (lpItemId)
	{
		LPTSTR szBuf=str.GetBuffer(MAX_PATH);
		::SHGetPathFromIDList(lpItemId, szBuf);
		::GlobalFree(lpItemId);
		str.ReleaseBuffer();
	}

	return str;
}

int CALLBACK BrowseCallbackProc(HWND hwnd,UINT msg,LPARAM lp, LPARAM pData)
{
	TCHAR buf[MAX_PATH];

	switch(msg) 
	{
	// when dialog is first initialized, change directory to one chosen above
		case BFFM_INITIALIZED: 
			strcpy(buf,CWzdDirDlg::m_sRootDir);
			::SendMessage(hwnd,BFFM_SETSELECTION,TRUE,(LPARAM)buf);
			break;

	// if you picked BIF_STATUSTEXT above, you can fill status here
		case BFFM_SELCHANGED:
			if (::SHGetPathFromIDList((LPITEMIDLIST) lp ,buf)) 
				SendMessage(hwnd,BFFM_SETSTATUSTEXT,0,(LPARAM)buf);
			break;
	}
	return 0;
}
