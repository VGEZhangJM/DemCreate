// SetLocalTCDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "vr_demcreator.h"
#include "SetLocalTCDlg.h"

#include "DCR_Grid_IDW.h"
#include "DCR_Grid_SPD.h"
#include "DCR_Grid_RBF.h"

#include "matlib.h"

#include "SetIDWParamDlg.h"
#include "SetSPDParamDlg.h"
#include "SetRBFParamDlg.h"
#include "SetKRGParamDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetLocalTCDlg dialog


CSetLocalTCDlg::CSetLocalTCDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetLocalTCDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetLocalTCDlg)
	m_iDirectionType = 0;
	m_iPrePointNum = 1;
	m_iTotalPointNum = 4;
	m_dPointPattern = 0.0;
	m_dMeanSlope = 0.0;
	m_dMeanRoughness = 0.0;
	m_dSearchRadius = 30000.0;
	m_dIDWZ = 0.0;
	m_dCurrentZ = 0.0;
	m_dSPDZ = 0.0;
	m_dRBFZ = 0.0;
	m_dKRGZ = 0.0;
	m_dMaxDistance = 0.0;
	m_dCentroid = 0.0;
	m_dMinDistance = 0.0;
	//}}AFX_DATA_INIT
	//
	m_pMainView		= NULL;

}


void CSetLocalTCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetLocalTCDlg)
	DDX_Radio(pDX, IDC_RADIO_NULLDIRECTION, m_iDirectionType);
	DDX_Text(pDX, IDC_EDIT_PRE_POINTNUM, m_iPrePointNum);
	DDX_Text(pDX, IDC_EDIT_TOTAL_POINTNUM, m_iTotalPointNum);
	DDX_Text(pDX, IDC_EDIT_POINTPATTERN, m_dPointPattern);
	DDX_Text(pDX, IDC_EDIT_MEANSLOPE, m_dMeanSlope);
	DDX_Text(pDX, IDC_EDIT_MEANROUGHNESS, m_dMeanRoughness);
	DDX_Text(pDX, IDC_EDIT_RADIUS, m_dSearchRadius);
	DDX_Text(pDX, IDC_EDIT_IDW_Z, m_dIDWZ);
	DDX_Text(pDX, IDC_EDIT_Z, m_dCurrentZ);
	DDX_Text(pDX, IDC_EDIT_SPD_Z, m_dSPDZ);
	DDX_Text(pDX, IDC_EDIT_RBF_Z, m_dRBFZ);
	DDX_Text(pDX, IDC_EDIT_KRG_Z, m_dKRGZ);
	DDX_Text(pDX, IDC_EDIT_MAXDISTANCE, m_dMaxDistance);
	DDX_Text(pDX, IDC_EDIT_CENTROD, m_dCentroid);
	DDX_Text(pDX, IDC_EDIT_MINDISTANCE, m_dMinDistance);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetLocalTCDlg, CDialog)
	//{{AFX_MSG_MAP(CSetLocalTCDlg)
	ON_BN_CLICKED(IDC_RADIO_NULLDIRECTION, OnRadioNulldirection)
	ON_BN_CLICKED(IDC_RADIO_FOURDIRECTION, OnRadioFourdirection)
	ON_BN_CLICKED(IDC_RADIO_EIGHTDIRECTION, OnRadioEightdirection)
	ON_EN_CHANGE(IDC_EDIT_PRE_POINTNUM, OnChangeEditPrePointnum)
	ON_EN_CHANGE(IDC_EDIT_TOTAL_POINTNUM, OnChangeEditTotalPointnum)
	ON_EN_CHANGE(IDC_EDIT_RADIUS, OnChangeEditRadius)
	ON_WM_PAINT()
	ON_BN_CLICKED(ID_CCUCLATE, OnCcuclate)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_IDW, OnButtonIdw)
	ON_BN_CLICKED(IDC_BUTTON_SPD, OnButtonSpd)
	ON_BN_CLICKED(IDC_BUTTON_RBF, OnButtonRbf)
	ON_BN_CLICKED(IDC_BUTTON_KRG, OnButtonKrg)
	ON_EN_CHANGE(IDC_EDIT_MAXDISTANCE, OnChangeEditMaxdistance)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetLocalTCDlg message handlers
BOOL CSetLocalTCDlg::Create(CWnd* pView)
{
	m_pMainView	= pView;

	return CDialog::Create(CSetLocalTCDlg::IDD);
}

//
void CSetLocalTCDlg::MoveWindowPos()
{
	if(m_pMainView != NULL)
	{
		RECT	rcRect;
		m_pMainView->GetClientRect(&rcRect);
		
		RECT	winRect;
		GetWindowRect(&winRect);

		m_pMainView->ClientToScreen(&rcRect);

		MoveWindow(rcRect.left, (rcRect.bottom)-(winRect.bottom-winRect.top), winRect.right-winRect.left, winRect.bottom-winRect.top);
	}
}

void CSetLocalTCDlg::OnRadioNulldirection() 
{
	m_iDirectionType	= 0;

	UpdateData(FALSE);
}

void CSetLocalTCDlg::OnRadioFourdirection() 
{
	m_iDirectionType	= 1;

	m_iTotalPointNum	= 4 * m_iPrePointNum;

	UpdateData(FALSE);
}

void CSetLocalTCDlg::OnRadioEightdirection() 
{
	m_iDirectionType	= 2;

	m_iTotalPointNum	= 8 * m_iPrePointNum;

	UpdateData(FALSE);
}

void CSetLocalTCDlg::OnChangeEditPrePointnum() 
{
	UpdateData(TRUE);

	if(m_iDirectionType == 1)
		m_iTotalPointNum	= 4 * m_iPrePointNum;

	if(m_iDirectionType == 2)
		m_iTotalPointNum	= 8 * m_iPrePointNum;
	
	UpdateData(FALSE);
}

void CSetLocalTCDlg::OnChangeEditTotalPointnum() 
{
	int		iOldValue;

	iOldValue	= m_iTotalPointNum;
	
	if(m_iDirectionType == 0)		UpdateData(TRUE);
	
	UpdateData(FALSE);
}

BOOL CSetLocalTCDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//
	CWnd	*pWnd;
	pWnd	= GetDlgItem(IDC_STATIC_DRAW);

	if(pWnd)
	{
		pWnd->GetClientRect(m_rectClient);
		pWnd->MapWindowPoints(this,m_rectClient);
	}

	//
	CString	szInfo;
	char	chInfo[32];

	szInfo.Format("%.4lf",m_dPointPattern);
	strcpy(chInfo,szInfo);
	m_dPointPattern	= atof(chInfo);

	szInfo.Format("%.4lf",m_dMeanSlope);
	strcpy(chInfo,szInfo);
	m_dMeanSlope	= atof(chInfo);

	szInfo.Format("%.4lf",m_dMeanRoughness);
	strcpy(chInfo,szInfo);
	m_dMeanRoughness	= atof(chInfo);

	szInfo.Format("%.4lf",m_dMaxDistance);
	strcpy(chInfo,szInfo);
	m_dMaxDistance	= atof(chInfo);

	szInfo.Format("%.4lf",m_dMinDistance);
	strcpy(chInfo,szInfo);
	m_dMinDistance	= atof(chInfo);

	szInfo.Format("%.4lf",m_dCentroid);
	strcpy(chInfo,szInfo);
	m_dCentroid	= atof(chInfo);

	//进入MatCom
	initM(MATCOM_VERSION);

	//
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


//
void CSetLocalTCDlg::DrawAndUpdate(CDC *pDC)
{		
	pDC->Rectangle(m_rectClient);	

	if(m_pGridVertexList.GetGridVertexCount() == 0)	return;
	
	//
	double	dLeft,dRight,dBottom,dTop;

	//计算范围
	{
		CDCR_GridVertex	*pVertex;
		bool			bFirst;
		double			dMaxDistance;
	
		bFirst	= true;
		pVertex	= m_pGridVertexList.GetGridVertex();

		while(pVertex != NULL)
		{
			double	dDistance;
			dDistance	= sqrt((pVertex->m_dX-m_dCurrentX)*(pVertex->m_dX-m_dCurrentX) + (pVertex->m_dY-m_dCurrentY)*(pVertex->m_dY-m_dCurrentY));
			if(bFirst)
			{
				dMaxDistance	= dDistance;

				bFirst	= false;
			}
			else
			{
				if(dMaxDistance < dDistance)	dMaxDistance	= dDistance;
			}
				
			pVertex	= pVertex->m_pNext;
		}

		//
		dLeft	= m_dCurrentX - dMaxDistance;
		dRight	= m_dCurrentX + dMaxDistance;
		dBottom	= m_dCurrentY - dMaxDistance;
		dTop	= m_dCurrentY + dMaxDistance;
	}

	InitShow(m_rectClient,dLeft,dRight,dBottom,dTop);

	//绘制坐标系
	{
		CPen	*pOldPen,*pNewPen;
		pNewPen	= new	CPen;
		pNewPen->CreatePen(PS_SOLID,1,RGB(255,0,0));
		pOldPen	= pDC->SelectObject(pNewPen);
		
		int		iX,iY;
		iX	= (long)(m_dMapXA*m_dCurrentX + m_dMapXB + 0.5);
		iY	= (long)(m_dMapYA*m_dCurrentY + m_dMapYB + 0.5);
		
		pDC->MoveTo(iX,m_rectClient.top);
		pDC->LineTo(iX,m_rectClient.bottom);

		pDC->MoveTo(m_rectClient.left,iY);
		pDC->LineTo(m_rectClient.right,iY);

		//计算八方向时的分界线
		if(m_iDirectionType == 2)
		{
			int		iLeft,iRight,iBottom,iTop;
			iLeft	= (long)(m_dMapXA*dLeft + m_dMapXB + 0.5);
			iRight	= (long)(m_dMapXA*dRight + m_dMapXB + 0.5);
			iBottom	= (long)(m_dMapYA*dBottom + m_dMapYB + 0.5);
			iTop	= (long)(m_dMapYA*dTop + m_dMapYB + 0.5);
			

			pDC->MoveTo(iLeft,iBottom);
			pDC->LineTo(iRight,iTop);

			pDC->MoveTo(iLeft,iTop);
			pDC->LineTo(iRight,iBottom);
		}

		pDC->SelectObject(pOldPen);
		delete	pNewPen;
	}
	
	//显示半径
	{
		CRect	rect;
		rect.left	= (long)(m_dMapXA*dLeft + m_dMapXB + 0.5);
		rect.top	= (long)(m_dMapYA*dTop + m_dMapYB + 0.5);
		
		rect.right	= (long)(m_dMapXA*dRight + m_dMapXB + 0.5);
		rect.bottom	= (long)(m_dMapYA*dBottom + m_dMapYB + 0.5);
		
		CPen	*pNewPen,*pOldPen;
		pNewPen		= new	CPen;
		pNewPen->CreatePen(PS_DOT,1,RGB(255,0,0));
		pOldPen		= pDC->SelectObject(pNewPen);

		CGdiObject	*pBackup;
		pBackup		= pDC->SelectStockObject(NULL_BRUSH);

		pDC->Ellipse(rect);

		pDC->SelectObject(pOldPen);
		delete	pNewPen;

		pDC->SelectObject(pBackup);	
	}

	//显示插值点
	{
		CPen	*pNewPen,*pOldPen;
		pNewPen		= new	CPen;
		pNewPen->CreatePen(PS_SOLID,1,RGB(0,0,255));
		pOldPen		= pDC->SelectObject(pNewPen);

		CBrush	*pNewBrush,*pOldBrush;
		pNewBrush	= new	CBrush;
		pNewBrush->CreateSolidBrush(RGB(120,120,120));
		pOldBrush	= pDC->SelectObject(pNewBrush);
	
		//
		int		iX,iY;
		iX	= (long)(m_dMapXA*m_dCurrentX + m_dMapXB + 0.5);
		iY	= (long)(m_dMapYA*m_dCurrentY + m_dMapYB + 0.5);

		pDC->Ellipse(iX-4,iY-4,iX+4,iY+4);


		pDC->SelectObject(pOldBrush);
		delete	pNewBrush;

		pDC->SelectObject(pOldPen);
		delete	pNewPen;
	}

	//显示三角网
	{
		m_pTriangleList.Draw(pDC,m_dMapXA,m_dMapXB,m_dMapYA,m_dMapYB);

	}
	//显示所有节点
	
	{
		CPen	*pNewPen,*pOldPen;
		pNewPen		= new	CPen;
		pNewPen->CreatePen(PS_SOLID,1,RGB(120,120,120));
		pOldPen		= pDC->SelectObject(pNewPen);

		CBrush	*pNewBrush,*pOldBrush;
		pNewBrush	= new	CBrush;
		pNewBrush->CreateSolidBrush(RGB(120,120,120));
		pOldBrush	= pDC->SelectObject(pNewBrush);

		CDCR_GridVertex	*pVertex;
	
		pVertex	= m_pAllGridVertexList.GetGridVertex();
		while(pVertex != NULL)
		{
			//
			int		iX,iY;
			iX	= (long)(m_dMapXA*pVertex->m_dX + m_dMapXB + 0.5);
			iY	= (long)(m_dMapYA*pVertex->m_dY + m_dMapYB + 0.5);

			pDC->Rectangle(iX-2,iY-2,iX+2,iY+2);

			pVertex	= pVertex->m_pNext;
		}

		pDC->SelectObject(pOldBrush);
		delete	pNewBrush;

		pDC->SelectObject(pOldPen);
		delete	pNewPen;
	}
	

	//显示插值节点
	{
		CPen	*pNewPen,*pOldPen;
		pNewPen		= new	CPen;
		pNewPen->CreatePen(PS_SOLID,1,RGB(255,0,0));
		pOldPen		= pDC->SelectObject(pNewPen);

		//CGdiObject	*pBackup;
		//pBackup		= pDC->SelectStockObject(NULL_BRUSH);

		CDCR_GridVertex	*pVertex;
	
		pVertex	= m_pGridVertexList.GetGridVertex();
		while(pVertex != NULL)
		{
			//
			int		iX,iY;
			iX	= (long)(m_dMapXA*pVertex->m_dX + m_dMapXB + 0.5);
			iY	= (long)(m_dMapYA*pVertex->m_dY + m_dMapYB + 0.5);

			pDC->Rectangle(iX-4,iY-4,iX+4,iY+4);

			pVertex	= pVertex->m_pNext;
		}

		pDC->SelectObject(pOldPen);
		delete	pNewPen;

		//pDC->SelectObject(pBackup);	
	}

}

void CSetLocalTCDlg::AddGridVertex(double dX,double dY,double dZ,double dDistance)
{
	m_pGridVertexList.AddGridVertexEx(dX,dY,dZ,dDistance);
}

void CSetLocalTCDlg::AddAllGridVretex(double dX,double dY,double dZ,double dDistance)
{
	m_pAllGridVertexList.AddGridVertexEx(dX,dY,dZ,dDistance);
}

void CSetLocalTCDlg::OnRelease()
{
	m_pGridVertexList.Release();
	m_pAllGridVertexList.Release();
	m_pTriangleList.Release();
}

bool CSetLocalTCDlg::InitShow(CRect rRect, double dLeft, double dRight, double dBottom, double dTop)
{
	if(fabs(dRight-dLeft) < 1e-10)	return	false;
	if(fabs(dTop-dBottom) < 1e-10)	return	false;

	double	dScalex = fabs(rRect.Width() / (dRight-dLeft));
	double	dScaley = fabs(rRect.Height() / (dTop-dBottom));
	
	double	dMapScale;

	dMapScale		= (dScalex<dScaley) ? dScalex : dScaley;

	//缩小
	dMapScale		= dMapScale * 0.96;

	double dWinOrgDistX = (rRect.Width() - fabs(dRight-dLeft)*dMapScale) /2.0;
	double dWinOrgDistY = (rRect.Height() - fabs(dTop-dBottom)*dMapScale) /2.0;

	m_dMapXA		= dMapScale;
	m_dMapXB		= dWinOrgDistX - dLeft * dMapScale + rRect.left;
	m_dMapYA		= -1.0 * dMapScale;
	m_dMapYB		= dBottom * dMapScale - dWinOrgDistY + rRect.bottom;
	
	return	true;
}

void CSetLocalTCDlg::OnChangeEditRadius() 
{
	//
	UpdateData(TRUE);
}

void CSetLocalTCDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	DrawAndUpdate(&dc);
		
	// Do not call CDialog::OnPaint() for painting messages
}

//
void CSetLocalTCDlg::OnVauleRefresh()
{
	//
	CString	szInfo;
	char	chInfo[32];

	szInfo.Format("%.4lf",m_dPointPattern);
	strcpy(chInfo,szInfo);
	m_dPointPattern	= atof(chInfo);

	szInfo.Format("%.4lf",m_dMeanSlope);
	strcpy(chInfo,szInfo);
	m_dMeanSlope	= atof(chInfo);

	szInfo.Format("%.4lf",m_dMeanRoughness);
	strcpy(chInfo,szInfo);
	m_dMeanRoughness	= atof(chInfo);

	szInfo.Format("%.4lf",m_dMaxDistance);
	strcpy(chInfo,szInfo);
	m_dMaxDistance	= atof(chInfo);

	szInfo.Format("%.4lf",m_dMinDistance);
	strcpy(chInfo,szInfo);
	m_dMinDistance	= atof(chInfo);

	szInfo.Format("%.4lf",m_dCentroid);
	strcpy(chInfo,szInfo);
	m_dCentroid	= atof(chInfo);

	UpdateData(FALSE);
}

void CSetLocalTCDlg::OnCcuclate() 
{
	double			dZ;

	//使用IDW计算
	{
		dZ				= 9999;

		m_pIDW.SetGridVertexList(&m_pGridVertexList);
		
		m_pIDW.CalcHeight(m_dCurrentX,m_dCurrentY,dZ);
		
		m_dIDWZ	= dZ-m_dCurrentZ;

		CString	szInfo;
		char	chInfo[32];

		szInfo.Format("%.5lf",m_dIDWZ);
		strcpy(chInfo,szInfo);
		m_dIDWZ	= atof(chInfo);
	}

	//使用SPD计算
	{		
		dZ				= 9999;

		m_pSPD.SetGridVertexList(&m_pGridVertexList);

		m_pSPD.CalcHeightEx(m_dCurrentX,m_dCurrentY,dZ);

		m_dSPDZ	= dZ-m_dCurrentZ;

		CString	szInfo;
		char	chInfo[32];

		szInfo.Format("%.5lf",m_dSPDZ);
		strcpy(chInfo,szInfo);
		m_dSPDZ	= atof(chInfo);
	}

	//使用RBF计算
	{
		dZ				= 9999;

		m_pRBF.SetGridVertexList(&m_pGridVertexList);

		m_pRBF.CalcHeight(m_dCurrentX,m_dCurrentY,dZ);

		m_dRBFZ	= dZ-m_dCurrentZ;

		CString	szInfo;
		char	chInfo[32];

		szInfo.Format("%.5lf",m_dRBFZ);
		strcpy(chInfo,szInfo);
		m_dRBFZ	= atof(chInfo);
	}

	//
	{
		dZ				= 9999;
		double	dZSD;

		m_pKRG.SetGridVertexList(&m_pGridVertexList);

		m_pKRG.CalcHeight(m_dCurrentX,m_dCurrentY,dZ,dZSD);

		m_dKRGZ	= dZ-m_dCurrentZ;

		CString	szInfo;
		char	chInfo[32];

		szInfo.Format("%.5lf",m_dKRGZ);
		strcpy(chInfo,szInfo);
		m_dKRGZ	= atof(chInfo);
	}


	UpdateData(FALSE);
}

void CSetLocalTCDlg::OnDestroy() 
{
	CDialog::OnDestroy();

	//退出MatCom
	exitM();	
}

void CSetLocalTCDlg::OnButtonIdw() 
{
	CSetIDWParamDlg	pDlg;
	
	pDlg.m_iU	= m_pIDW.GetU();
	pDlg.m_dT	= m_pIDW.GetT();

	if(pDlg.DoModal() == IDOK)
	{
		
	}
	else	return;

	//计算
	
	if(m_pGridVertexList.GetGridVertexCount() > 0)
	{
		m_pIDW.SetU(pDlg.m_iU);
		m_pIDW.SetT(pDlg.m_dT);
		
		double	dZ;
		dZ				= 9999;

		m_pIDW.SetGridVertexList(&m_pGridVertexList);

		m_pIDW.CalcHeight(m_dCurrentX,m_dCurrentY,dZ);

		m_dIDWZ	= dZ-m_dCurrentZ;

		//
		CString	szInfo;
		char	chInfo[32];

		szInfo.Format("%.5lf",m_dIDWZ);
		strcpy(chInfo,szInfo);
		m_dIDWZ	= atof(chInfo);

		UpdateData(FALSE);
	}
}

void CSetLocalTCDlg::OnButtonSpd() 
{
	CSetSPDParamDlg		pDlg;

	pDlg.m_iU	= m_pSPD.GetU();
	pDlg.m_dT	= m_pSPD.GetT();

	if(pDlg.DoModal() == IDOK)
	{

	}
	else	return;

	if(m_pGridVertexList.GetGridVertexCount() > 0)
	{
		m_pSPD.SetU(pDlg.m_iU);
		m_pSPD.SetT(pDlg.m_dT);
		
		double	dZ;
		dZ				= 9999;

		m_pSPD.SetGridVertexList(&m_pGridVertexList);

		m_pSPD.CalcHeightEx(m_dCurrentX,m_dCurrentY,dZ);

		m_dSPDZ	= dZ-m_dCurrentZ;

		CString	szInfo;
		char	chInfo[32];

		szInfo.Format("%.5lf",m_dSPDZ);
		strcpy(chInfo,szInfo);
		m_dSPDZ	= atof(chInfo);

		UpdateData(FALSE);
	}
}

void CSetLocalTCDlg::OnButtonRbf() 
{
	CSetRBFParamDlg	pDlg;

	pDlg.m_iModelType	= m_pRBF.GetModelType();
	pDlg.m_dC			= m_pRBF.GetC();

	if(pDlg.DoModal() == IDOK)
	{

	}
	else	return;

	if(m_pGridVertexList.GetGridVertexCount() > 0)
	{
		m_pRBF.SetModelType(pDlg.m_iModelType);
		m_pRBF.SetC(pDlg.m_dC);
		
		double	dZ;
		dZ				= 9999;

		m_pRBF.SetGridVertexList(&m_pGridVertexList);

		m_pRBF.CalcHeight(m_dCurrentX,m_dCurrentY,dZ);

		m_dRBFZ	= dZ-m_dCurrentZ;

		CString	szInfo;
		char	chInfo[32];

		szInfo.Format("%.5lf",m_dRBFZ);
		strcpy(chInfo,szInfo);
		m_dRBFZ	= atof(chInfo);

		UpdateData(FALSE);
	}

}

void CSetLocalTCDlg::OnButtonKrg() 
{
	CSetKRGParamDlg	pDlg;
	
	pDlg.m_iModelType	= m_pKRG.GetModelType();
	pDlg.m_dC			= m_pKRG.GetC();
	pDlg.m_dC0			= m_pKRG.GetC0();
	pDlg.m_dA			= m_pKRG.GetA();

	if(pDlg.DoModal() == IDOK)
	{

	}
	else	return;

	if(m_pGridVertexList.GetGridVertexCount() > 0)
	{
		m_pKRG.SetModelType(pDlg.m_iModelType);
		m_pKRG.SetC(pDlg.m_dC);
		m_pKRG.SetC0(pDlg.m_dC0);
		m_pKRG.SetA(pDlg.m_dA);

		double	dZ;
		dZ				= 9999;
		double	dZSD;

		m_pKRG.SetGridVertexList(&m_pGridVertexList);

		m_pKRG.CalcHeight(m_dCurrentX,m_dCurrentY,dZ,dZSD);

		m_dKRGZ	= dZ-m_dCurrentZ;

		CString	szInfo;
		char	chInfo[32];

		szInfo.Format("%.5lf",m_dKRGZ);
		strcpy(chInfo,szInfo);
		m_dKRGZ	= atof(chInfo);

		UpdateData(FALSE);
	}
}

void CSetLocalTCDlg::OnChangeEditMaxdistance() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
}
