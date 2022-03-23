// WzdDrDlg.h: interface for the CWzdDirDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WZDDRDLG_H__D52656D3_8A25_11D2_9C53_00AA003D8695__INCLUDED_)
#define AFX_WZDDRDLG_H__D52656D3_8A25_11D2_9C53_00AA003D8695__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CWzdDirDlg  
{
public:
	CWzdDirDlg();
	virtual ~CWzdDirDlg();

	CString GetDirectory(CWnd *pParent=NULL,LPCSTR lpszRoot="c:\\",LPCSTR lpszTitle="��ѡ���ļ���...");

	static CString m_sRootDir;

};
int CALLBACK BrowseCallbackProc(HWND hwnd,UINT uMsg,LPARAM lp, LPARAM pData);

#endif // !defined(AFX_WZDDRDLG_H__D52656D3_8A25_11D2_9C53_00AA003D8695__INCLUDED_)
