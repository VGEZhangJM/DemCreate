// Interface_DemCreator.cpp: implementation of the CInterface_DemCreator class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Interface_DemCreator.h"

#include "SetStatDlg.h"
#include "SetSlopeDlg.h"
#include "SetRemainsErrorDlg.h"
#include "SetLimitDlg.h"

#include "DCR_HeightSet.h"
#include "DCR_SlopeSet.h"
#include "DCR_CurvatureSet.h"

#include "DCR_Statistics.h"
#include "SetCurvatureDlg.h"
#include "SetSemiVariogramDlg.h"
#include "SetCalcHeightDlg.h"

#include "DCR_Param_IDW.h"
#include "DCR_Param_RBF.h"
#include "DCR_Param_KRG.h"
#include "DCR_Param_SPD.h"

#include "SetTerrainFactorDlg.h"

#include "DCR_Grid_Fractal.h"

#include "SetFractalDlg.h"
#include "SetFractalResultDlg.h"

#include "SetTerrainRelifDlg.h"
#include "SetTerrainRelifResultDlg.h"

#include "SetTerrainCVDlg.h"
#include "SetTerrainCVResultDlg.h"
#include "SetMatInfoDlg.h"

#include "DCR_DemSet.h"

#include "DCR_Param_LTC.h"
#include "DCR_LocalTerrainCreator.h"
#include "SetCALCLSD_Dlg.h"

#include "SetRECombineTEDlg.h"
#include "SetDimensionAnalyseDlg.h"
#include "DCR_DimensionAnalyse.h"

#include "SetSearchPointDlg.h"
#include "SetSearchPointExDlg.h"
#include "SetWindownSize.h"

#include "SetDeltaHPerDlg.h"
#include "SetCheckPointDlg.h"

#include "matlib.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// CreateDemCreator
//////////////////////////////////////////////////////////////////////
_declspec(dllexport) IInterface_DemCreator* CreateDemCreator()
{
	return	(IInterface_DemCreator*) new	CInterface_DemCreator();
}

//////////////////////////////////////////////////////////////////////
// ReleaseDemCreator
//////////////////////////////////////////////////////////////////////
_declspec(dllexport) void ReleaseDemCreator(IInterface_DemCreator* pIDemCreator)
{
	if(pIDemCreator)
	{
		delete	(CInterface_DemCreator*)pIDemCreator;
		pIDemCreator	= NULL;
	}
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CInterface_DemCreator::CInterface_DemCreator()
{
	//��ȡӦ�ó���·��
	//{
	CString	strPath;
	::GetModuleFileName(NULL,strPath.GetBufferSetLength(MAX_PATH+1),MAX_PATH);
	strPath.ReleaseBuffer();
	
	int	nPos;
	nPos			= strPath.ReverseFind('\\');
	m_szExePathName	= strPath.Left(nPos);
	//}	

	//��ͼ��
	m_pMapBitmap			= NULL;
	m_pMapDC				= NULL;
	
	//�û��ڴ�λͼ
	m_pUserBitmap			= NULL;
	m_pUserDC				= NULL;

	//
	m_iMapTool				= 0;

	//
	m_pMainWnd				= NULL;
	m_pDC					= NULL;
	

	//��ʾ����
	m_dShowLeft				= m_dShowRight		= -1.0;
	m_dShowBottom			= m_dShowTop		= -1.0;

	m_dOldShowLeft			= m_dOldShowRight	= -1.0;
	m_dOldShowBottom		= m_dOldShowTop		= -1.0;

	//��ʾ����
	m_dMapXA				= m_dMapXB			= -1.0;
	m_dMapYA				= m_dMapYB			= -1.0;

	//�ӿڴ�С
	m_nFullWidth			= m_nFullHeight		= 0;

	//�ص�������ʼ��
	m_OnProcess				= NULL;

	m_pLocalTCDlg			= NULL;
	m_pSearchPointDlg		= NULL;

	m_bGeoMorphShowStatus	= false;
}

CInterface_DemCreator::~CInterface_DemCreator()
{
	//
	CDCR_HeightSet::UnInstance();
	//
	CDCR_SlopeSet::UnInstance();
	//
	CDCR_CurvatureSet::UnInstance();
	//
	CDCR_DemSet::UnInstance();

	//
	m_pWorkspace.Release();

	//
	m_pGridDem.OnRelease();

	//
	m_pTinCreator.Release();
	
	//
	m_pGridCreator.Release();

	//��ͼ��
	if(m_pMapBitmap != NULL)		{	delete	m_pMapBitmap;			m_pMapBitmap		= NULL;	}
	if(m_pMapDC != NULL)			{	delete	m_pMapDC;				m_pMapDC			= NULL;	}

	//
	if(m_pUserBitmap != NULL)		{	delete	m_pUserBitmap;			m_pUserBitmap		= NULL;	}
	if(m_pUserDC != NULL)			{	delete	m_pUserDC;				m_pUserDC			= NULL;	}

	if(m_pLocalTCDlg != NULL)		{	delete	m_pLocalTCDlg;			m_pLocalTCDlg		= NULL;	}
	if(m_pSearchPointDlg != NULL)	{	delete	m_pSearchPointDlg;		m_pSearchPointDlg	= NULL;	}
}

//////////////////////////////////////////////////////////////////////
// 01 ��ȡ������ָ��
//////////////////////////////////////////////////////////////////////
void CInterface_DemCreator::AttachMainWnd(CWnd *pWnd)
{
	m_pMainWnd	= pWnd;
}

//////////////////////////////////////////////////////////////////////
// 02 ������ͼDC
//////////////////////////////////////////////////////////////////////
void CInterface_DemCreator::CreateMapDC(CDC *pDC,int iSx,int iSy)
{
	if(iSx!=0 && iSy!=0)
	{
		if(m_pMapBitmap == NULL)	
			m_pMapBitmap	= new	CBitmap;
		
		m_nFullWidth	= GetSystemMetrics(SM_CXSCREEN)*iSx;
		m_nFullHeight	= GetSystemMetrics(SM_CYSCREEN)*iSy;
		
		//******************************************************************//
		//�����ڴ�λͼ��DC
		if(m_pMapDC == NULL)
		{
			m_pMapDC	= new	CDC;
			m_pMapDC->CreateCompatibleDC(pDC);
		}
		m_pMapBitmap->CreateCompatibleBitmap(pDC,m_nFullWidth,m_nFullHeight);
		
		m_pMapDC->SelectObject(m_pMapBitmap);
		m_pMapDC->PatBlt(0,0,m_nFullWidth,m_nFullHeight,WHITENESS);
		//******************************************************************//
	}

	m_pDC		= pDC;
	
	//
	//CMap_DeviceInfo::Instance()->Device(pDC->m_hDC);
}

//////////////////////////////////////////////////////////////////////
// 03 �ͷŵ�ͼDC
//////////////////////////////////////////////////////////////////////
void CInterface_DemCreator::ReleaseMapDC()
{
	if(m_pMapBitmap != NULL)
	{
		delete	m_pMapBitmap;
		m_pMapBitmap	= NULL;
	}
}

//////////////////////////////////////////////////////////////////////
// 04 ��ȡ��ͼDC
//////////////////////////////////////////////////////////////////////
CDC* CInterface_DemCreator::GetMapDC()
{
	if(m_pMapDC != NULL)
		return	m_pMapDC;

	return	NULL;
}

//////////////////////////////////////////////////////////////////////
// 05 �����û�DC
//////////////////////////////////////////////////////////////////////
void CInterface_DemCreator::CreateUserDC(CDC *pDC,int iSx,int iSy)
{
	if(iSx!=0 && iSy!=0)
	{
		if(m_pUserBitmap == NULL)	
			m_pUserBitmap	= new	CBitmap;

		m_nFullWidth	= GetSystemMetrics(SM_CXSCREEN)*iSx;
		m_nFullHeight	= GetSystemMetrics(SM_CYSCREEN)*iSy;
		
		//******************************************************************//
		//�����ڴ�λͼ��DC
		if(m_pUserDC == NULL)
		{
			m_pUserDC	= new	CDC;

			m_pUserDC->CreateCompatibleDC(pDC);
		}
		m_pUserBitmap->CreateCompatibleBitmap(pDC,m_nFullWidth,m_nFullHeight);
		
		m_pUserDC->SelectObject(m_pUserBitmap);
		m_pUserDC->PatBlt(0,0,m_nFullWidth,m_nFullHeight,WHITENESS);	
		//******************************************************************//
	}
}

//////////////////////////////////////////////////////////////////////
// 06 �ͷ��û�DC
//////////////////////////////////////////////////////////////////////
void CInterface_DemCreator::ReleaseUserDC()
{
	if(m_pUserBitmap != NULL)
	{
		delete	m_pUserBitmap;
		m_pUserBitmap	= NULL;
	}
}

//////////////////////////////////////////////////////////////////////
// 07 ��ȡ�û�DC
//////////////////////////////////////////////////////////////////////
CDC* CInterface_DemCreator::GetUserDC()
{
	if(m_pUserDC != NULL)
		return	m_pUserDC;

	return	NULL;
}

//////////////////////////////////////////////////////////////////////
// 08 ����/��ȡ��ͼ����
//////////////////////////////////////////////////////////////////////
void CInterface_DemCreator::SetMapTool(int iMapTool)
{
	m_iMapTool	= iMapTool;
}
int CInterface_DemCreator::GetMapTool()
{
	return	m_iMapTool;
}

//////////////////////////////////////////////////////////////////////
// 09 ȫͼ��ʾ
//////////////////////////////////////////////////////////////////////
void CInterface_DemCreator::OnFull()
{	
	double	dLeft,dRight,dBottom,dTop;

	GetMapExt(dLeft,dRight,dBottom,dTop);

	//
	SetShowExt(dLeft,dRight,dBottom,dTop);

	//
	SetOldShowExt(-1.0,-1.0,-1.0,-1.0);

	if(m_pMainWnd != NULL)	m_pMainWnd->Invalidate(FALSE);
}

//////////////////////////////////////////////////////////////////////
// 10 ����/��ȡ��ͼ����
//////////////////////////////////////////////////////////////////////
void CInterface_DemCreator::SetMapExt(double dLeft,double dRight,double dBottom,double dTop)
{
	//m_pWorkspace.SetMapExt(dLeft,dRight,dBottom,dTop);	
}
void CInterface_DemCreator::GetMapExt(double& dLeft,double& dRight,double& dBottom,double& dTop)
{
	if(!m_pWorkspace.IsEmpty())
	{
		m_pWorkspace.GetMapExt(dLeft,dRight,dBottom,dTop);

		return;
	}
	
	if(!m_pGridDem.IsEmpty())
	{
		m_pGridDem.GetDemExt(dLeft,dRight,dBottom,dTop);

		return;
	}

	if(!m_pClusterAnalysis.IsEmpty())
	{
		m_pClusterAnalysis.GetMapExt(dLeft,dRight,dBottom,dTop);

		return;
	}


	dLeft	= -1.0;
	dRight	= -1.0;
	dBottom	= -1.0;
	dTop	= -1.0;
}

//////////////////////////////////////////////////////////////////////
// 11 ����/��ȡ��ʾ����
//////////////////////////////////////////////////////////////////////
void CInterface_DemCreator::SetShowExt(double dLeft,double dRight,double dBottom,double dTop)
{
	m_dShowLeft		= dLeft;
	m_dShowRight	= dRight;
	m_dShowBottom	= dBottom;
	m_dShowTop		= dTop;
}
void CInterface_DemCreator::GetShowExt(double& dLeft,double& dRight,double& dBottom,double& dTop)
{
	dLeft		= m_dShowLeft;
	dRight		= m_dShowRight;
	dBottom		= m_dShowBottom;
	dTop		= m_dShowTop;
}

//////////////////////////////////////////////////////////////////////
// 12 ����/��ȡ�ϴ���ʾ����
//////////////////////////////////////////////////////////////////////
void CInterface_DemCreator::SetOldShowExt(double dLeft,double dRight,double dBottom,double dTop)
{
	m_dOldShowLeft		= dLeft;
	m_dOldShowRight		= dRight;
	m_dOldShowBottom	= dBottom;
	m_dOldShowTop		= dTop;
}
void CInterface_DemCreator::GetOldShowExt(double& dLeft,double& dRight,double& dBottom,double& dTop)
{
	dLeft		= m_dOldShowLeft;
	dRight		= m_dOldShowRight;
	dBottom		= m_dOldShowBottom;
	dTop		= m_dOldShowTop;
}

//////////////////////////////////////////////////////////////////////
// 13 �����ӿ�
//////////////////////////////////////////////////////////////////////
void CInterface_DemCreator::SetClientRect(CRect rRect)
{
	m_rRect	= rRect;
}

//////////////////////////////////////////////////////////////////////
// 14 ��������,����DEM��GRD��ʸ�����ݵȣ�ͨ����׺�Ĳ�ͬ����
//////////////////////////////////////////////////////////////////////
bool CInterface_DemCreator::OnReadAsVector(CString szPathName)
{	
	char	chPathName[128];
	strcpy(chPathName,szPathName);

	//
	m_szPathName	= szPathName;

	//��ȡ��׺��
	int		iBreakID;
	iBreakID		= szPathName.ReverseFind('.');

	CString	szExtName;
	szExtName		= szPathName.Right(szPathName.GetLength()-iBreakID-1);

	//��ȡ��׼·��
	iBreakID		= szPathName.ReverseFind('\\');
	m_szBasePathName= szPathName.Left(iBreakID);

	
	m_dOldShowLeft		= m_dOldShowRight	= -1.0;
	m_dOldShowBottom	= m_dOldShowTop		= -1.0;

	//DEM
	if(szExtName.CompareNoCase("dem") == 0)
	{
		//
		if(m_pWorkspace.ImportDEM(chPathName))
		{
			m_pWorkspace.GetMapExt(m_dShowLeft,m_dShowRight,m_dShowBottom,m_dShowTop);
			
			double	dMinH,dMaxH;
			m_pWorkspace.GetHeightExt(dMinH,dMaxH);

			//
			CDCR_DemSet::Instance()->SetMinHeight(dMinH);
			CDCR_DemSet::Instance()->SetMidHeight((dMaxH+dMinH)/2);
			CDCR_DemSet::Instance()->SetMaxHeight(dMaxH);

			return	true;
		}
	}

	//
	if(szExtName.CompareNoCase("uDvf") == 0)
	{
		//
		if(m_pWorkspace.Read(chPathName))
		{
			m_pWorkspace.GetMapExt(m_dShowLeft,m_dShowRight,m_dShowBottom,m_dShowTop);
			
			double	dMinH,dMaxH;
			m_pWorkspace.GetHeightExt(dMinH,dMaxH);

			//
			CDCR_DemSet::Instance()->SetMinHeight(dMinH);
			CDCR_DemSet::Instance()->SetMidHeight((dMaxH+dMinH)/2);
			CDCR_DemSet::Instance()->SetMaxHeight(dMaxH);

			return	true;
		}
	}

	//
	if(szExtName.CompareNoCase("dat") == 0)
	{
		//
		if(m_pWorkspace.Import(chPathName))
		{
			m_pWorkspace.GetMapExt(m_dShowLeft,m_dShowRight,m_dShowBottom,m_dShowTop);
			
			double	dMinH,dMaxH;
			m_pWorkspace.GetHeightExt(dMinH,dMaxH);

			//
			CDCR_DemSet::Instance()->SetMinHeight(dMinH);
			CDCR_DemSet::Instance()->SetMidHeight((dMaxH+dMinH)/2);
			CDCR_DemSet::Instance()->SetMaxHeight(dMaxH);

			return	true;
		}
	}

	//
	if(szExtName.CompareNoCase("txt") == 0)
	{
		if(m_pClusterAnalysis.OnImport(chPathName))
		{
			m_pClusterAnalysis.GetMapExt(m_dShowLeft,m_dShowRight,m_dShowBottom,m_dShowTop);

			return	true;
		}
	}

	return	false;
}

//////////////////////////////////////////////////////////////////////
// 14 ��������,����DEM��GRD��ʸ�����ݵȣ�ͨ����׺�Ĳ�ͬ����
//////////////////////////////////////////////////////////////////////
bool CInterface_DemCreator::OnReadAsGrid(CString szPathName)
{	
	char	chPathName[128];
	strcpy(chPathName,szPathName);

	//
	m_szPathName	= szPathName;

	//��ȡ��׺��
	int		iBreakID;
	iBreakID		= szPathName.ReverseFind('.');

	CString	szExtName;
	szExtName		= szPathName.Right(szPathName.GetLength()-iBreakID-1);

	//��ȡ��׼·��
	iBreakID		= szPathName.ReverseFind('\\');
	m_szBasePathName= szPathName.Left(iBreakID);

	
	m_dOldShowLeft		= m_dOldShowRight	= -1.0;
	m_dOldShowBottom	= m_dOldShowTop		= -1.0;

	//DEM
	if(szExtName.CompareNoCase("dem") == 0)
	{
		if(m_pGridDem.OnRead(chPathName))
		{
			m_pGridDem.GetDemExt(m_dShowLeft,m_dShowRight,m_dShowBottom,m_dShowTop);
			
			double	dMinH,dMaxH;
			dMinH	= m_pGridDem.GetMinHeight();
			dMaxH	= m_pGridDem.GetMaxHeight();

			//
			CDCR_DemSet::Instance()->SetMinHeight(dMinH);
			CDCR_DemSet::Instance()->SetMidHeight((dMaxH+dMinH)/2);
			CDCR_DemSet::Instance()->SetMaxHeight(dMaxH);

			return	true;
		}		
	}
	
	//GRD
	if(szExtName.CompareNoCase("grd") == 0)
	{
		if(m_pGridDem.OnImport(chPathName))
		{
			m_pGridDem.GetDemExt(m_dShowLeft,m_dShowRight,m_dShowBottom,m_dShowTop);

			double	dMinH,dMaxH;
			dMinH	= m_pGridDem.GetMinHeight();
			dMaxH	= m_pGridDem.GetMaxHeight();

			//
			CDCR_DemSet::Instance()->SetMinHeight(dMinH);
			CDCR_DemSet::Instance()->SetMidHeight((dMaxH+dMinH)/2);
			CDCR_DemSet::Instance()->SetMaxHeight(dMaxH);

			return	true;
		}		
	}

	return	false;
}

bool CInterface_DemCreator::OnReadLocalTerrain(CString szPathName)
{
	m_szLocalTerrainPathName	= szPathName;

	m_pLocalTerrain.OnRead(szPathName);

	return	true;
}

//////////////////////////////////////////////////////////////////////
// 15 �ж��ļ��Ƿ���سɹ�
//////////////////////////////////////////////////////////////////////
bool CInterface_DemCreator::IsEmpty()
{
	if(m_pWorkspace.IsEmpty() && m_pGridDem.IsEmpty() && m_pClusterAnalysis.IsEmpty())
		return	true;

	return	false;
}

//////////////////////////////////////////////////////////////////////
// 16 �ͷ�����
//////////////////////////////////////////////////////////////////////
void CInterface_DemCreator::OnRelease()
{
	//
	m_pWorkspace.Release();

	//
	m_pGridDem.OnRelease();

	//
	m_pTinCreator.Release();
	
	//
	m_pGridCreator.Release();

	//�¶���
	m_pSlope.OnRelease();

	//������
	m_pCurvature.OnRelease();

	//���δֲڶ���
	m_pTerrainRoughness.OnRelease();

	//���δֲڶ���
	m_pTerrainCV.OnRelease();

	//�����������
	m_pTerrainRelif.OnRelease();
	//�����и������
	m_pTerrainIncision.OnRelease();

	//
	m_pClusterAnalysis.OnRelease();

	//
	m_pGeoMorphological.OnRelease();
}

//////////////////////////////////////////////////////////////////////
// 17 ��������
//////////////////////////////////////////////////////////////////////
bool CInterface_DemCreator::OnSave(CString szPathName)
{	
	//��ȡ��׺��
	int		iBreakID;
	iBreakID		= szPathName.ReverseFind('.');

	CString	szExtName;
	szExtName		= szPathName.Right(szPathName.GetLength()-iBreakID-1);
	
	//
	if(szExtName.CompareNoCase("dem") == 0)
	{
		char	chPathName[200];
		strcpy(chPathName,szPathName);
		m_pGridDem.OnSave(chPathName);
		
		return	true;
	}
	
	//
	if(szExtName.CompareNoCase("grd") == 0)
	{
		//
		char	chPathName[200];
		strcpy(chPathName,szPathName);
		m_pGridDem.OnExport(chPathName);

		return	true;
	}

	if(szExtName.CompareNoCase("dat") == 0)
	{
		m_pWorkspace.Save(szPathName);

		return	true;
	}
	
	//����С���任
	if(szExtName.CompareNoCase("mat") == 0)
	{
		FILE	*fp;
		fp	= fopen(szPathName,"w");

		if(fp != NULL)
		{
			int	iNumOfX;
			int	iNumOfY;
			double	*dHeight;

			iNumOfX	= m_pGridDem.GetNumOfX();
			iNumOfY	= m_pGridDem.GetNumOfY();
			
			dHeight	= m_pGridDem.GetHeight();
			
			double	dH;
			for(int j=iNumOfY-1;j>=0;j--)
			{
				for(int i=0;i<iNumOfX;i++)
				{
					dH	= dHeight[j*iNumOfX+i];

					fprintf(fp,"%.4lf ",dH);
				}

				fprintf(fp,"\n");
			}

			fclose(fp);
		}
		
		return	true;
	}


	return	false;
}

//////////////////////////////////////////////////////////////////////
// 18 ����
//////////////////////////////////////////////////////////////////////
void CInterface_DemCreator::OnDraw(CDC *pDC)
{

}

//////////////////////////////////////////////////////////////////////
// 19 ����DEM��Map DC��
//////////////////////////////////////////////////////////////////////
void CInterface_DemCreator::DrawToDem()
{
	//
	if(m_pMapDC == NULL)	return;

	//���DEM���ݻ�ʸ�����ݾ������ڣ����˳�
	if(m_pGridDem.IsEmpty() && m_pWorkspace.IsEmpty() && m_pClusterAnalysis.IsEmpty())
	{
		return;
	}
	
	//
	double	dShowLeft,dShowRight,dShowBottom,dShowTop;
	GetShowExt(dShowLeft,dShowRight,dShowBottom,dShowTop);

	double	dPreShowLeft,dPreShowRight,dPreShowBottom,dPreShowTop;
	GetOldShowExt(dPreShowLeft,dPreShowRight,dPreShowBottom,dPreShowTop);
		
	if((dShowLeft!=dPreShowLeft) || (dShowRight!=dPreShowRight) || (dShowBottom!=dPreShowBottom) || dShowTop!=dPreShowTop)
	{
		//////////////////////////////////////////////////////////////
		//ˢ�׵�ͼ����������ÿ�θ�����˵����һ���ģ�����Ҫ���Ĺ�����
		{
		m_pMapDC->SelectObject(m_pMapBitmap);
		m_pMapDC->PatBlt(0, 0, m_nFullWidth, m_nFullHeight, WHITENESS);
		}
		//////////////////////////////////////////////////////////////
	
		//
		if(InitShow(m_rRect,m_dShowLeft,m_dShowRight,m_dShowBottom,m_dShowTop))
		{
			//DEM��ʾ
			if(!m_pGridDem.IsEmpty())
			{
				//��ʾDEM
				{
					m_pGridDem.SetClientRect(m_rRect);
					m_pGridDem.SetCoordMapping(m_dMapXA,m_dMapXB,m_dMapYA,m_dMapYB);
					m_pGridDem.OnDraw(m_pMapDC);
				}

				//��ʾ�¶�
				{
					m_pSlope.SetClientRect(m_rRect);
					m_pSlope.SetCoordMapping(m_dMapXA,m_dMapXB,m_dMapYA,m_dMapYB);
					m_pSlope.OnDraw(m_pMapDC);
				}

				//��ʾ�¶�
				{
					m_pAspect.SetClientRect(m_rRect);
					m_pAspect.SetCoordMapping(m_dMapXA,m_dMapXB,m_dMapYA,m_dMapYB);
					m_pAspect.OnDraw(m_pMapDC);
				}

				//��ʾ���δֲڶ�
				{
					m_pTerrainRoughness.SetClientRect(m_rRect);
					m_pTerrainRoughness.SetCoordMapping(m_dMapXA,m_dMapXB,m_dMapYA,m_dMapYB);
					m_pTerrainRoughness.OnDraw(m_pMapDC);				
				}

				//��ʾ���δֲڶ�
				{
					m_pTerrainRelif.SetClientRect(m_rRect);
					m_pTerrainRelif.SetCoordMapping(m_dMapXA,m_dMapXB,m_dMapYA,m_dMapYB);
					m_pTerrainRelif.OnDraw(m_pMapDC);				
				}
			}
			
			//ʸ��������ʾ
			if(!m_pWorkspace.IsEmpty())
			{
				m_pWorkspace.SetClientRect(m_rRect);
				m_pWorkspace.SetCoordMapping(m_dMapXA,m_dMapXB,m_dMapYA,m_dMapYB);
				m_pWorkspace.Draw(m_pMapDC);
			}

			//
			if(!m_pClusterAnalysis.IsEmpty())
			{
				m_pClusterAnalysis.SetClientRect(m_rRect);
				m_pClusterAnalysis.SetCoordMapping(m_dMapXA,m_dMapXB,m_dMapYA,m_dMapYB);
				m_pClusterAnalysis.Draw(m_pMapDC);
			}
			
			//��������ʾ
			{
				m_pTinCreator.SetClientRect(m_rRect);
				m_pTinCreator.SetCoordMapping(m_dMapXA,m_dMapXB,m_dMapYA,m_dMapYB);
				m_pTinCreator.Draw(m_pMapDC);
			}


			//��ʾ������
			if(m_bGeoMorphShowStatus)
			{
				m_pGeoMorphological.SetClientRect(m_rRect);
				m_pGeoMorphological.SetCoordMapping(m_dMapXA,m_dMapXB,m_dMapYA,m_dMapYB);
				m_pGeoMorphological.OnDraw(m_pMapDC);
			}
		}

		//
		{
			CFont font;
			font.CreateFont(12, 0, 000, 000, 400, 0, 0, 0, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_TT_ALWAYS, PROOF_QUALITY, VARIABLE_PITCH|FF_ROMAN, "����");
		
			CFont* oldfont=m_pMapDC->SelectObject(&font);
			int oldtextalign=m_pMapDC->SetTextAlign(TA_TOP|TA_CENTER);
			COLORREF oldtextcolor=m_pMapDC->SetTextColor(RGB(255,0,0));
			int oldbkmode=m_pMapDC->SetBkMode(TRANSPARENT);
		
			m_pMapDC->TextOut(m_rRect.left,m_rRect.top+20,m_szLocalTerrainPathName);
		
			m_pMapDC->SelectObject(oldfont);
			m_pMapDC->SetTextAlign(oldtextalign);
			m_pMapDC->SetTextColor(oldtextcolor);
			m_pMapDC->SetBkMode(oldbkmode);
		}

		//
		SetOldShowExt(m_dShowLeft,m_dShowRight,m_dShowBottom,m_dShowTop);
	}
}

//////////////////////////////////////////////////////////////////////
// 20 ���Ƶ�User DC��
//////////////////////////////////////////////////////////////////////
void CInterface_DemCreator::DrawToUser()
{
	if(m_pUserDC!=NULL && m_pMapDC!=NULL)
		m_pUserDC->BitBlt(m_rRect.left,m_rRect.top,m_rRect.Width(),m_rRect.Height(),m_pMapDC,m_rRect.left,m_rRect.top,SRCCOPY);
}

//////////////////////////////////////////////////////////////////////
// 21 ���Ƶ�Screen DC��
//////////////////////////////////////////////////////////////////////
void CInterface_DemCreator::DrawToScreen(CDC *pDC)
{
	if(m_pUserDC != NULL)
		pDC->BitBlt(m_rRect.left,m_rRect.top,m_rRect.Width(),m_rRect.Height(),m_pUserDC,m_rRect.left,m_rRect.top,SRCCOPY);
}

//////////////////////////////////////////////////////////////////////
// 22 ����/��ȡ������ʾ״̬
//////////////////////////////////////////////////////////////////////
void CInterface_DemCreator::SetShowStatusInContent(bool bShowStatus)
{
	m_pWorkspace.SetShowStatusInContent(bShowStatus);
	
	SetOldShowExt(-1.0,-1.0,-1.0,-1.0);

	if(m_pMainWnd != NULL)	m_pMainWnd->Invalidate(FALSE);
}
bool CInterface_DemCreator::GetShowStatusInContent()
{
	return	m_pWorkspace.GetShowStatusInContent();
}

//////////////////////////////////////////////////////////////////////
// 23 ����/��ȡ������ʾ״̬
//////////////////////////////////////////////////////////////////////
void CInterface_DemCreator::SetShowStatusInCell(bool bShowStatus)
{
	m_pWorkspace.SetShowStatusInCell(bShowStatus);
	
	SetOldShowExt(-1.0,-1.0,-1.0,-1.0);

	if(m_pMainWnd != NULL)	m_pMainWnd->Invalidate(FALSE);
}
bool CInterface_DemCreator::GetShowStatusInCell()
{
	return	m_pWorkspace.GetShowStatusInCell();
}

//////////////////////////////////////////////////////////////////////
// 24 ����/��ȡDEM��ʾ״̬
//////////////////////////////////////////////////////////////////////
void CInterface_DemCreator::SetShowStatusInDem(bool bShowStatus)
{
	SetNullStatus();

	m_pGridDem.SetShowStatus(bShowStatus);
	
	SetOldShowExt(-1.0,-1.0,-1.0,-1.0);

	if(m_pMainWnd != NULL)	m_pMainWnd->Invalidate(FALSE);
}
bool CInterface_DemCreator::GetShowStatusInDem()
{
	return	m_pGridDem.GetShowStatus();
}

//////////////////////////////////////////////////////////////////////
// 25 ����/��ȡ�¶���ʾ״̬
//////////////////////////////////////////////////////////////////////
void CInterface_DemCreator::SetShowStatusInSlope(bool bShowStatus)
{
	SetNullStatus();

	m_pSlope.SetShowStatus(bShowStatus);
	
	SetOldShowExt(-1.0,-1.0,-1.0,-1.0);

	if(m_pMainWnd != NULL)	m_pMainWnd->Invalidate(FALSE);
}
bool CInterface_DemCreator::GetShowStatusInSlope()
{
	return	m_pSlope.GetShowStatus();
}

//25 ����/��ȡ�¶���ʾ״̬
void CInterface_DemCreator::SetShowStatusInAspect(bool bShowStatus)
{
	SetNullStatus();
	
	m_pAspect.SetShowStatus(bShowStatus);
	
	SetOldShowExt(-1.0,-1.0,-1.0,-1.0);
	
	if(m_pMainWnd != NULL)	m_pMainWnd->Invalidate(FALSE);
}

bool CInterface_DemCreator::GetShowStatusInAspect()
{
	return	m_pAspect.GetShowStatus();
}

//////////////////////////////////////////////////////////////////////
// 26 ����/��ȡ������ʾ״̬
//////////////////////////////////////////////////////////////////////
void CInterface_DemCreator::SetShowStatusInCurvature(bool bShowStatus)
{
	SetNullStatus();

	m_pCurvature.SetShowStatus(bShowStatus);
	
	SetOldShowExt(-1.0,-1.0,-1.0,-1.0);

	if(m_pMainWnd != NULL)	m_pMainWnd->Invalidate(FALSE);
}
bool CInterface_DemCreator::GetShowStatusInCurvature()
{
	return	m_pCurvature.GetShowStatus();
}

//////////////////////////////////////////////////////////////////////
// 27 ����/��ȡ����ֲڶ���ʾ״̬
//////////////////////////////////////////////////////////////////////
void CInterface_DemCreator::SetShowStatusInRoughness(bool bShowStatus)
{
	SetNullStatus();

	m_pTerrainRoughness.SetShowStatus(bShowStatus);
	
	SetOldShowExt(-1.0,-1.0,-1.0,-1.0);

	if(m_pMainWnd != NULL)	m_pMainWnd->Invalidate(FALSE);
}
bool CInterface_DemCreator::GetShowStatusInRoughness()
{
	return	m_pTerrainRoughness.GetShowStatus();
}

//////////////////////////////////////////////////////////////////////
// 28 ����/��ȡ�����������ʾ״̬
//////////////////////////////////////////////////////////////////////
void CInterface_DemCreator::SetShowStatusInRelif(bool bShowStatus)
{
	SetNullStatus();
	
	m_pTerrainRelif.SetShowStatus(bShowStatus);
	
	SetOldShowExt(-1.0,-1.0,-1.0,-1.0);

	if(m_pMainWnd != NULL)	m_pMainWnd->Invalidate(FALSE);
}
bool CInterface_DemCreator::GetShowStatusInRelif()
{
	return	m_pTerrainRelif.GetShowStatus();
}

//////////////////////////////////////////////////////////////////////
// 28 ����/��ȡ��ò�жϽ����ʾ״̬
//////////////////////////////////////////////////////////////////////
void CInterface_DemCreator::SetShowStatusInGeoMorphJudge(bool bShowStatus)
{
	m_bGeoMorphShowStatus	= bShowStatus;

	if(m_bGeoMorphShowStatus == true)
	{
		SetShowStatusInDem(false);
	}
	else
	{
		SetShowStatusInDem(true);		
	}

	if(m_pMainWnd != NULL)	m_pMainWnd->Invalidate(TRUE);
}

bool CInterface_DemCreator::GetShowStatusInGeoMorphJudge()
{
	return	m_bGeoMorphShowStatus;
}

//////////////////////////////////////////////////////////////////////
// 29 ����ƶ���ͨ������ͼ�����
//////////////////////////////////////////////////////////////////////
void CInterface_DemCreator::OnMouseMove(UINT nFlags, CPoint point)
{
	//////////////////////////////////////////////////////////////////
	//�Ŵ�
	if(m_iMapTool == 1)
	{
		if(nFlags==1 && (MK_LBUTTON))
		{
			m_pUserDC->BitBlt(m_rRect.left,m_rRect.top,m_rRect.Width(),m_rRect.Height(),m_pMapDC,m_rRect.left,m_rRect.top,SRCCOPY);

			m_ptUp	= point;

			CRect	rect;

			rect.left	= m_ptUp.x;
			rect.top	= m_ptUp.y;
			rect.right	= m_ptDown.x;
			rect.bottom	= m_ptDown.y;
			rect.NormalizeRect();

			//���ƾ���
			{
				Graphics graphics(m_pUserDC->m_hDC);

				graphics.SetCompositingMode(CompositingModeSourceOver);

				SolidBrush	alphaBrush(Color(50, 255, 255, 0));
				Pen			alphaPen(Color(150,255,255,0),2);	

				graphics.FillRectangle(&alphaBrush,rect.left,rect.top,rect.Width(),rect.Height());	
				graphics.DrawRectangle(&alphaPen,rect.left,rect.top,rect.Width(),rect.Height());
			}

			m_pDC->BitBlt(m_rRect.left,m_rRect.top,m_rRect.Width(),m_rRect.Height(),m_pUserDC,m_rRect.left,m_rRect.top,SRCCOPY);
		}
	}
	//////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////
	//����
	if(m_iMapTool == 3)
	{
		if(nFlags==1 && (MK_LBUTTON))
		{
			CClientDC	dc(m_pMainWnd);

			CBitmap* pOldBitmap	= m_pUserDC->SelectObject(m_pUserBitmap);
			CBrush backgroundBrush((COLORREF)::GetSysColor(COLOR_WINDOW));
			CBrush*	pOldBrush	= m_pUserDC->SelectObject(&backgroundBrush);

			m_ptUp		= point;
			
			if((m_ptUp.y-m_ptDown.y)>0)
				dc.PatBlt(m_rRect.left, m_rRect.top, m_rRect.right, m_ptUp.y-m_ptDown.y, PATCOPY);
			else
				dc.PatBlt(m_rRect.left, m_rRect.bottom+(m_ptUp.y-m_ptDown.y), m_rRect.right, m_rRect.bottom, PATCOPY);

			if((m_ptUp.x-m_ptDown.x)>0)
				dc.PatBlt(m_rRect.left, m_rRect.top, m_ptUp.x-m_ptDown.x, m_rRect.bottom, PATCOPY);
			else
				dc.PatBlt(m_rRect.right+(m_ptUp.x-m_ptDown.x), m_rRect.top, m_rRect.right, m_rRect.bottom, PATCOPY);

			//
			dc.BitBlt(m_ptUp.x-m_ptDown.x, m_ptUp.y-m_ptDown.y,m_rRect.Width(), m_rRect.Height(),m_pUserDC, m_rRect.left, m_rRect.top, SRCCOPY);

			m_pUserDC->SelectObject(pOldBitmap);
			m_pUserDC->SelectObject(pOldBrush);
		}
	}
	//////////////////////////////////////////////////////////////////
}

//////////////////////////////////////////////////////////////////////
// 30 ����������ͨ������ͼ�����
//////////////////////////////////////////////////////////////////////
void CInterface_DemCreator::OnLButtonUp(UINT nFlags, CPoint point)
{
	//
	ReleaseCapture();

	//////////////////////////////////////////////////////////////////
	//�Ŵ�
	if(m_iMapTool == 1)
	{
		if(abs(m_ptDown.x-m_ptUp.x)<=20 && abs(m_ptDown.y-m_ptUp.y)<=20)
		{
			m_ptDown	= m_ptUp;
		}

		if((m_ptDown.x==m_ptUp.x)&&(m_ptDown.y==m_ptUp.y))	//����Ŵ�
		{
			double	dX,dY;
			
			ScreenToMap(m_ptDown.x,m_ptDown.y,dX,dY);
			
			double	dLeft,dRight,dBottom,dTop;
			double	dPreLeft,dPreRight,dPreBottom,dPreTop;

			GetShowExt(dPreLeft,dPreRight,dPreBottom,dPreTop);

			double	dHor;
			double	dVer;

			dHor	= fabs(dPreRight - dPreLeft)/4;
			dVer	= fabs(dPreTop - dPreBottom)/4;
				
			dLeft	= dX - dHor;
			dRight	= dX + dHor;
			dBottom	= dY - dVer;
			dTop	= dY + dVer;
			
			SetShowExt(dLeft,dRight,dBottom,dTop);

			//
			if(m_pMainWnd != NULL)	m_pMainWnd->Invalidate(FALSE);
		}
		else				 					//�����Ŵ�
		{
			double	x1,x2;
			double	y1,y2;	

			ScreenToMap(m_ptDown.x,m_ptDown.y,x1,y1);
			ScreenToMap(m_ptUp.x,m_ptUp.y,x2,y2);
			
			double	dLeft,dRight,dBottom,dTop;
			double	dPreLeft,dPreRight,dPreBottom,dPreTop;

			GetShowExt(dPreLeft,dPreRight,dPreBottom,dPreTop);

			if(x1 > x2)	{	dRight	= x1;	dLeft		= x2;	}
			else		{	dRight	= x2;	dLeft		= x1;	}
			if(y1 > y2)	{	dTop	= y1;	dBottom		= y2;	}
			else		{	dTop	= y2;	dBottom		= y1;	}

			SetShowExt(dLeft,dRight,dBottom,dTop);
			//
			if(m_pMainWnd != NULL)	m_pMainWnd->Invalidate(FALSE);
		}
	}
	//////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////
	//��С
	if(m_iMapTool == 2)
	{
		double	dX,dY;
		ScreenToMap(m_ptDown.x,m_ptDown.y,dX,dY);

		double	dHor;
		double	dVer;

		double	dLeft,dRight,dBottom,dTop;
		double	dPreLeft,dPreRight,dPreBottom,dPreTop;

		GetShowExt(dPreLeft,dPreRight,dPreBottom,dPreTop);

		dHor	= fabs(dPreRight - dPreLeft);
		dVer	= fabs(dPreTop - dPreBottom);
			
		dLeft		= dX - dHor;
		dRight		= dX + dHor;
		dBottom		= dY - dVer;
		dTop		= dY + dVer;

		SetShowExt(dLeft,dRight,dBottom,dTop);

		//
		if(m_pMainWnd != NULL)	m_pMainWnd->Invalidate(FALSE);
	}
	//////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////
	//����
	if(m_iMapTool == 3)
	{
		double	dX1,dY1,dX2,dY2,dX,dY;
		
		ScreenToMap(m_ptDown.x,m_ptDown.y,dX1,dY1);
		ScreenToMap(m_ptUp.x,m_ptUp.y,dX2,dY2);

		dX	= dX2-dX1;
		dY	= dY2-dY1;

		double	dLeft,dRight,dBottom,dTop;
		double	dPreLeft,dPreRight,dPreBottom,dPreTop;
		GetShowExt(dPreLeft,dPreRight,dPreBottom,dPreTop);
		
		dLeft		= dPreLeft - dX;
		dRight		= dPreRight -dX;
		dBottom		= dPreBottom - dY;
		dTop		= dPreTop - dY;
		
		SetShowExt(dLeft,dRight,dBottom,dTop);

		//
		if(m_pMainWnd != NULL)	m_pMainWnd->Invalidate(FALSE);
	}
	//////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////
	//����
	if(m_iMapTool == 4)
	{
		if(m_pLocalTCDlg != NULL)
		{
			//�Ի����еĲ���
			int		iSearchType;
			iSearchType				= m_pLocalTCDlg->m_iDirectionType;

			int		iPointNumPreDirection;
			iPointNumPreDirection	= m_pLocalTCDlg->m_iPrePointNum;

			int		iPointNumInTotal;
			iPointNumInTotal		= m_pLocalTCDlg->m_iTotalPointNum;

			double	dSearchRadius;
			dSearchRadius			= m_pLocalTCDlg->m_dSearchRadius;

			//��ȡ��
			double	dX,dY;
			ScreenToMap(m_ptDown.x,m_ptDown.y,dX,dY);		
			CDCR_PointFeature	*pSelectPoint;

			pSelectPoint	= m_pWorkspace.SelectPoint(dX,dY);

			if(pSelectPoint != NULL)
			{
				//�ڶ��� : ��ȡ���в������� 
				int					iSampleCount;
				iSampleCount	= m_pWorkspace.GetSamplePointCount();
				CDCR_PointFeature	*pSamplePointList;
				pSamplePointList= m_pWorkspace.GetPointFeature();
				
				//����������������Ϣ���ݸ�CDCR_LocalTerrainCreator
				CDCR_LocalTerrainCreator	m_pLTCreator;
				{
					//1�����÷���������Ŀ
					int		iCellNumX,iCellNumY;
					m_pWorkspace.GetCellCount(iCellNumX,iCellNumY);

					//2�����÷����ĸ����ߴ�
					double	dCellSizeX,dCellSizeY;
					m_pWorkspace.GetCellSize(dCellSizeX,dCellSizeY);

					//3�����÷����ķ�Χ���ͽ�ģ��Χ��һ��
					double	dCellLeft,dCellRight,dCellBottom,dCellTop;
					m_pWorkspace.GetCellExt(dCellLeft,dCellRight,dCellBottom,dCellTop);

					//4�����÷���������Ϣ
					CDCR_Cells	*pCells;
					pCells	= m_pWorkspace.GetCell();

					//5����������Ϣ���ݵ�CDCR_LocalTerrainCreator��
					m_pLTCreator.SetCell(pCells,iCellNumX,iCellNumY,dCellSizeX,dCellSizeY,dCellLeft,dCellRight,dCellBottom,dCellTop);
				}

				//������ : ����ֲ�����������Ϣ
				{	
					m_pLTCreator.CalculateLocalTerrainCharacter(iSampleCount,
																pSamplePointList,
																pSelectPoint,
																iSearchType,
																iPointNumPreDirection,
																iPointNumInTotal,
																dSearchRadius
																);

					//��ʾ���Ի�����
					//
					m_pLocalTCDlg->m_dPointPattern		= m_pLTCreator.GetPointPatternValue();
					//
					m_pLocalTCDlg->m_dCentroid			= m_pLTCreator.GetCentroidValue();
					//
					m_pLocalTCDlg->m_dMeanSlope			= m_pLTCreator.GetMeanSlope();
					//
					m_pLocalTCDlg->m_dMeanRoughness		= m_pLTCreator.GetMeanRoughness();
					//
					m_pLocalTCDlg->m_dMaxDistance		= m_pLTCreator.GetMaxDistance();
					//
					m_pLocalTCDlg->m_dMinDistance		= m_pLTCreator.GetMinDistance();

					m_pLocalTCDlg->m_dCurrentX			= pSelectPoint->m_pPointCoord.x;
					m_pLocalTCDlg->m_dCurrentY			= pSelectPoint->m_pPointCoord.y;
					m_pLocalTCDlg->m_dCurrentZ			= pSelectPoint->m_pPointCoord.z;

					//�ͷ�
					m_pLocalTCDlg->OnRelease();

					//���Ի����д�����������
					{
						CDCR_GridVertexList	*pVertexList;

						pVertexList	= m_pLTCreator.GetGridVertexList();
						if(pVertexList != NULL)
						{
							CDCR_GridVertex	*pVertex;
							pVertex	= pVertexList->GetGridVertex();

							while(pVertex != NULL)
							{
								m_pLocalTCDlg->AddGridVertex(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);

								pVertex	= pVertex->m_pNext;
							}
						}

						pVertexList	= m_pLTCreator.GetAllGridVertexList();
						if(pVertexList != NULL)
						{
							CDCR_GridVertex	*pVertex;
							pVertex	= pVertexList->GetGridVertex();

							while(pVertex != NULL)
							{
								m_pLocalTCDlg->AddAllGridVretex(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);

								pVertex	= pVertex->m_pNext;
							}
						}
					}

					//���Ի����д�����������Ϣ
					{
						m_pLocalTCDlg->m_pTriangleList.Clone(m_pLTCreator.GetTriangleList());
					}

					m_pLocalTCDlg->OnVauleRefresh();

					m_pLocalTCDlg->Invalidate();
				}

				//ѡ��
				if(m_pMainWnd)	
				{
					SetOldShowExt(-1,-1,-1,-1);

					m_pMainWnd->Invalidate(FALSE);
				}
			}
		}
	}
	//////////////////////////////////////////////////////////////////

	if(m_iMapTool == 5)
	{
		if(m_pGridDem.IsEmpty())
		{
			return;
		}

		if(m_pMainWnd == NULL)	return;

		double	dX,dY;
		
		ScreenToMap(m_ptDown.x,m_ptDown.y,dX,dY);
		
		int		iWindowSize;
		iWindowSize	= 10;
		
		//��ȡDEM����
		double	*pData;
		pData	= m_pGridDem.GetHeight();

		//
		int		iNumOfX,iNumOfY;
		iNumOfX	= m_pGridDem.GetNumOfX();
		iNumOfY	= m_pGridDem.GetNumOfY();

		//
		double	dSizeOfX,dSizeOfY;
		dSizeOfX	= m_pGridDem.GetSizeOfX();
		dSizeOfY	= m_pGridDem.GetSizeOfY();

		//
		double	dRealSize;
		dRealSize	= m_pGridDem.GetRealSize();

		//
		double	dLeft,dRight,dBottom,dTop;
		m_pGridDem.GetDemExt(dLeft,dRight,dBottom,dTop);
		
		CString	szPathName;
		szPathName	= m_pGridDem.GetPathName();
		
		//
		m_pGeoMorphological.SetGridDem(pData,iNumOfX,iNumOfY,dSizeOfX,dSizeOfY,dRealSize,dLeft,dRight,dBottom,dTop,szPathName);

		//
		m_pGeoMorphological.SetWindowSize(iWindowSize);


		//
		{
			double	dLeft,dRight,dBottom,dTop;
			dLeft	= dX - 	dSizeOfX * iWindowSize;
			dRight	= dX+ 	dSizeOfX * iWindowSize;
			dBottom	= dY - 	dSizeOfY * iWindowSize;
			dTop	= dY+ 	dSizeOfY * iWindowSize;
			
			CRect	rRect;
			int		iX,iY;
			MapToScreen(dLeft,dBottom,iX,iY);

			rRect.left		= iX;
			rRect.bottom	= iY;
			MapToScreen(dRight,dTop,iX,iY);
			rRect.right		= iX;
			rRect.top		= iY;
			rRect.NormalizeRect();

		
			CClientDC	dc(m_pMainWnd);

			CGdiObject	*pBackup;
			pBackup		= dc.SelectStockObject(NULL_BRUSH);

			//
			dc.Rectangle(rRect);


			dc.SelectObject(pBackup);	
		}

		m_pGeoMorphological.OnCapture(dX,dY);
	}	
}

//////////////////////////////////////////////////////////////////////
// 31 ���������£�ͨ������ͼ�����
//////////////////////////////////////////////////////////////////////
void CInterface_DemCreator::OnLButtonDown(UINT nFlags, CPoint point)
{
	//////////////////////////////////////////////////////////////////
	//�Ŵ�
	if(m_iMapTool == 1)
	{
		m_ptDown	= m_ptUp	= point;
	}
	//////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////
	//��С
	if(m_iMapTool == 2)
	{
		m_ptDown	= m_ptUp	= point;
	}
	//////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////
	//����
	if(m_iMapTool == 3)
	{
		m_ptDown	= m_ptUp	= point;
	}
	//////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////
	//��׽
	if(m_iMapTool == 4 || m_iMapTool == 5)
	{
		m_ptDown	= m_ptUp	= point;
	}
	//////////////////////////////////////////////////////////////////	
}

//////////////////////////////////////////////////////////////////////
// 32 ������˫����ͨ������ͼ�����
//////////////////////////////////////////////////////////////////////
//������˫����ͨ������ͼ�����
void CInterface_DemCreator::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	
}

//////////////////////////////////////////////////////////////////////
// 33 ����������ͨ������ͼ�����
//////////////////////////////////////////////////////////////////////
void CInterface_DemCreator::OnRButtonUp(UINT nFlags, CPoint point)
{
}

//////////////////////////////////////////////////////////////////////
// 34 ���������£�ͨ������ͼ�����
//////////////////////////////////////////////////////////////////////
void CInterface_DemCreator::OnRButtonDown(UINT nFlags, CPoint point)
{

}

//////////////////////////////////////////////////////////////////////
// 35 ���ù����ʽ
//////////////////////////////////////////////////////////////////////
BOOL CInterface_DemCreator::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	//////////////////////////////////////////////////////////////////
	//�Ŵ�
	if(m_iMapTool == 1)
	{
		SetCursor(AfxGetApp()->LoadCursor(IDC_ZOOMIN));

		return	TRUE;
	}
	//////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////
	//��С
	if(m_iMapTool == 2)
	{
		SetCursor(AfxGetApp()->LoadCursor(IDC_ZOOMOUT));

		return	TRUE;
	}
	//////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////
	//����
	if(m_iMapTool == 3)
	{
		SetCursor(AfxGetApp()->LoadCursor(IDC_PAN));

		return	TRUE;
	}
	//////////////////////////////////////////////////////////////////

	return	FALSE;
}

//////////////////////////////////////////////////////////////////////
// 36 ��ӦOnSize����
//////////////////////////////////////////////////////////////////////
void CInterface_DemCreator::OnSize(UINT nType, int cx, int cy)
{

}

//////////////////////////////////////////////////////////////////////
// 37 ���ø�����Ŀ
//////////////////////////////////////////////////////////////////////
void CInterface_DemCreator::SetCellCount(int iCellNumX,int iCellNumY)
{
	m_pWorkspace.SetCellCount(iCellNumX,iCellNumY);
}
void CInterface_DemCreator::SetCellCount()
{
	
}

//////////////////////////////////////////////////////////////////////
// 38 ������������
//////////////////////////////////////////////////////////////////////
void CInterface_DemCreator::CellBuild()
{
	m_pWorkspace.CellsBuild();
}

//////////////////////////////////////////////////////////////////////
// 39 ɾ���ظ���
//////////////////////////////////////////////////////////////////////
void CInterface_DemCreator::DeleteRePoint()
{
	//
	CSetLimitDlg	pDlg;
	
	//
	pDlg.m_dLimit	= m_pWorkspace.GetMinDistance();

	if(pDlg.DoModal() == IDOK)
	{
		double	dLimit;
		dLimit	= pDlg.m_dLimit;
	
		m_pWorkspace.SetMinDistance(dLimit);
	}
	else	return;
	
	m_pWorkspace.DeleteRePoint();
}

//////////////////////////////////////////////////////////////////////
// 40 ����/��ȡɾ���ظ����еĵ����֮�����С���
//////////////////////////////////////////////////////////////////////
void CInterface_DemCreator::SetMinDistance(double dMinDistance)
{
	m_pWorkspace.SetMinDistance(dMinDistance);
}
double CInterface_DemCreator::GetMinDistance()
{
	return	m_pWorkspace.GetMinDistance();
}

//////////////////////////////////////////////////////////////////////
// 41 ����ʸ�����ݵĵ���ͳ����
//////////////////////////////////////////////////////////////////////
void CInterface_DemCreator::OnTerrainDescriptiveStatisticsInVector()
{
	HINSTANCE hOld	= ::AfxGetResourceHandle();

	#ifdef _DEBUG
		HINSTANCE hin	= ::GetModuleHandle("VR_DemCreatorD.dll");
	#else
		HINSTANCE hin	= ::GetModuleHandle("VR_DemCreator.dll");
	#endif

	int		iSampleCount;
	double	*dSampleX;
	double	*dSampleY;
	double	*dSampleZ;

	iSampleCount	= m_pWorkspace.GetSamplePointCount();

	if(iSampleCount == 0)
	{
		MessageBox(NULL,"û�в�������,���ܽ��в���ͳ��!","��ʾ��Ϣ",MB_OK);

		return;
	}

	dSampleX	= new	double[iSampleCount];
	dSampleY	= new	double[iSampleCount];
	dSampleZ	= new	double[iSampleCount];
	
	//��ȡ��������
	m_pWorkspace.GetSamplePoint(&dSampleX,&dSampleY,&dSampleZ);

	//ͳ����
	CDCR_Statistics	pStatistics;
	CString	szPathName;

	//X������ͳ��
	pStatistics.SetStatisticsData(iSampleCount,dSampleX);
	pStatistics.Statistics();
	//
	double	dXMin,dXMax;
	double	dXMean;
	double	dXVar;
	double	dXStdVar;
	double	dXCV;
	pStatistics.GetDataMinMax(dXMin,dXMax);
	pStatistics.GetDataMean(dXMean);
	pStatistics.GetDataVar(dXVar);
	pStatistics.GetDataStandardVar(dXStdVar);
	pStatistics.GetDataCV(dXCV);

	szPathName	= m_szPathName.Left(m_szPathName.GetLength()-4) + "X.txt";
	pStatistics.Save(szPathName);

	//Y������ͳ��
	pStatistics.SetStatisticsData(iSampleCount,dSampleY);
	pStatistics.Statistics();
	//
	double	dYMin,dYMax;
	double	dYMean;
	double	dYVar;
	double	dYStdVar;
	double	dYCV;
	pStatistics.GetDataMinMax(dYMin,dYMax);
	pStatistics.GetDataMean(dYMean);
	pStatistics.GetDataVar(dYVar);
	pStatistics.GetDataStandardVar(dYStdVar);
	pStatistics.GetDataCV(dYCV);
	szPathName	= m_szPathName.Left(m_szPathName.GetLength()-4) + "Y.txt";
	pStatistics.Save(szPathName);

	//Z������ͳ��
	pStatistics.SetStatisticsData(iSampleCount,dSampleZ);
	pStatistics.Statistics();
	//
	double	dZMin,dZMax;
	double	dZMean;
	double	dZVar;
	double	dZStdVar;
	double	dZCV;
	pStatistics.GetDataMinMax(dZMin,dZMax);
	pStatistics.GetDataMean(dZMean);
	pStatistics.GetDataVar(dZVar);
	pStatistics.GetDataStandardVar(dZStdVar);
	pStatistics.GetDataCV(dZCV);
	szPathName	= m_szPathName.Left(m_szPathName.GetLength()-4) + "Z.txt";
	pStatistics.Save(szPathName);

	delete	[]dSampleX;
	delete	[]dSampleY;
	delete	[]dSampleZ;

	CSetStatDlg	pDlg;
	
	//����Χ
	pDlg.dLeft		= dXMin;
	pDlg.dRight		= dXMax;
	pDlg.dBottom	= dYMin;
	pDlg.dTop		= dYMax;
	//�̷߳�Χ
	pDlg.dMinH		= dZMin;
	pDlg.dMaxH		= dZMax;
	//������ֵ
	pDlg.dXMean		= dXMean;
	pDlg.dYMean		= dYMean;
	pDlg.dZMean		= dZMean;
	//��������
	pDlg.dXVar		= dXVar;
	pDlg.dYVar		= dYVar;
	pDlg.dZVar		= dZVar;
	//������׼����
	pDlg.dXStdVar	= dXStdVar;
	pDlg.dYStdVar	= dYStdVar;
	pDlg.dZStdVar	= dZStdVar;
	//��������ϵ��
	pDlg.dZCV		= dZCV;
	
	pDlg.DoModal();
	
	::AfxSetResourceHandle(hOld);
}

//////////////////////////////////////////////////////////////////////
// 42 ���ڸ������ݵĵ���ͳ����
//////////////////////////////////////////////////////////////////////
void CInterface_DemCreator::OnTerrainDescriptiveStatisticsInGrd()
{
	//���벻���ڣ����˳�
	if(m_pGridDem.IsEmpty())	return;

	int		iSampleCount;
	double	*dSample;

	iSampleCount	= m_pGridDem.GetNumOfX() * m_pGridDem.GetNumOfY();

	if(iSampleCount == 0)
	{
		MessageBox(NULL,"û�и�������,���ܽ���ͳ��!","��ʾ��Ϣ",MB_OK);

		return;
	}

	dSample	= m_pGridDem.GetHeight();
	
	//ͳ����
	CDCR_Statistics	pStatistics;
	CString	szPathName;

	//Z������ͳ��
	pStatistics.SetStatisticsData(iSampleCount,dSample);
	pStatistics.Statistics();
	
	//
	szPathName	= m_szPathName.Left(m_szPathName.GetLength()-4) + ".txt";
	pStatistics.Save(szPathName);
}

//////////////////////////////////////////////////////////////////////
// 43 ͳ�ư���캯��,��������
//////////////////////////////////////////////////////////////////////
int CInterface_DemCreator::OnSemiVariogramStatistics(double **dX,double **dY)
{
	//��������
	m_pSemiVariogram.SetPointFeature(m_pWorkspace.GetPointFeature());

	//
	m_pSemiVariogram.SetPathName(m_szPathName);
	
	int	iLagNum;
	//
	iLagNum	= m_pSemiVariogram.SemiVariogram(dX,dY);

	if(iLagNum)
	{
		//
		CSetSemiVariogramDlg	pDlg;

		//
		pDlg.m_iLagNum	= iLagNum;

		for(int i=0;i<iLagNum;i++)
		{
			pDlg.m_dLagDistance[i]	= (*dX)[i];
			pDlg.m_dRPreLag[i]		= (*dY)[i];
		}

		pDlg.DoModal();
	}

	return	iLagNum;
}

//////////////////////////////////////////////////////////////////////
// 44 ͳ�ư���캯��
//////////////////////////////////////////////////////////////////////
void CInterface_DemCreator::OnSemiVariogramStatisticsEx()
{
	//��������
	m_pSemiVariogram.SetPointFeature(m_pWorkspace.GetPointFeature());

	//
	m_pSemiVariogram.SetPathName(m_szPathName);
	
	//
	m_pSemiVariogram.SemiVariogram(NULL,NULL);
}

//////////////////////////////////////////////////////////////////////
// 45 �����¶�
//////////////////////////////////////////////////////////////////////
void CInterface_DemCreator::OnCalculateSlope()
{
	//m_pGridDem.OnCalculateSlope();
	if(m_pGridDem.IsEmpty())
	{
		return;
	}

	//��ȡDEM����
	double	*pData;
	pData	= m_pGridDem.GetHeight();

	//
	int		iNumOfX,iNumOfY;
	iNumOfX	= m_pGridDem.GetNumOfX();
	iNumOfY	= m_pGridDem.GetNumOfY();

	double	dSizeOfX,dSizeOfY;
	dSizeOfX= m_pGridDem.GetSizeOfX();
	dSizeOfY= m_pGridDem.GetSizeOfY();

	//
	double	dLeft,dRight,dBottom,dTop;
	m_pGridDem.GetDemExt(dLeft,dRight,dBottom,dTop);
	
	//
	double	dRealSize;
	dRealSize= m_pGridDem.GetRealSize();

	//
	CString	szPathName;
	szPathName= m_pGridDem.GetPathName();
	
	//
	m_pSlope.SetGridDem(pData,iNumOfX,iNumOfY,dSizeOfX,dSizeOfY,dRealSize,dLeft,dRight,dBottom,dTop,szPathName);

	//����
	m_pSlope.OnCalculate(0);
}

//////////////////////////////////////////////////////////////////////
// 46 �����¶ȣ��������¶ȵ�ͳ����Ϣ
//////////////////////////////////////////////////////////////////////
void CInterface_DemCreator::OnSlope()
{
	if(m_pGridDem.IsEmpty())
	{
		return;
	}

	int		iNumX,iNumY;
	iNumX			= m_pGridDem.GetNumOfX();
	iNumY			= m_pGridDem.GetNumOfY();

	//
	HINSTANCE hOld	= ::AfxGetResourceHandle();

	#ifdef _DEBUG
		HINSTANCE hin	= ::GetModuleHandle("VR_DemCreatorD.dll");
	#else
		HINSTANCE hin	= ::GetModuleHandle("VR_DemCreator.dll");
	#endif

	//
	double	*dSlope;
	dSlope			= m_pSlope.GetSlope();
	
	if(dSlope != NULL)
	{
		CDCR_Statistics	pStatistics;
		//
		pStatistics.SetStatisticsData(iNumX*iNumY,dSlope);
		//
		pStatistics.Statistics();
		
		double	dMin,dMax;
		double	dMean;
		double	dVar;
		double	dStdVar;
		double	dCV;
		double	dSkewness;
		double	dKurtosis;
		double	dRMSE;

		pStatistics.GetDataMinMax(dMin,dMax);
		pStatistics.GetDataMean(dMean);
		pStatistics.GetDataVar(dVar);
		pStatistics.GetDataStandardVar(dStdVar);
		pStatistics.GetDataCV(dCV);
		pStatistics.GetDataSkewness(dSkewness);
		pStatistics.GetDataKurtosis(dKurtosis);
		pStatistics.GetDataRMSE(dRMSE);

		//
		CSetSlopeDlg	pDlg;

		pDlg.iNum				= iNumX*iNumY;
		pDlg.pData				= dSlope;

		pDlg.dMin				= dMin;
		pDlg.dMax				= dMax;
		pDlg.dMean				= dMean;
		pDlg.dVar				= dVar;
		pDlg.dStdVar			= dStdVar;
		pDlg.dCV				= dCV;
		pDlg.dSkewness			= dSkewness;
		pDlg.dKurtosis			= dKurtosis;
		pDlg.dRMSE				= dRMSE;

		pDlg.DoModal();
	}

	::AfxSetResourceHandle(hOld);
}

//////////////////////////////////////////////////////////////////////
// 47 ��������
//////////////////////////////////////////////////////////////////////
void CInterface_DemCreator::OnCalculateAspect()
{
	//m_pGridDem.OnCalculateSlope();
	if(m_pGridDem.IsEmpty())
	{
		return;
	}
	
	//��ȡDEM����
	double	*pData;
	pData	= m_pGridDem.GetHeight();
	
	//
	int		iNumOfX,iNumOfY;
	iNumOfX	= m_pGridDem.GetNumOfX();
	iNumOfY	= m_pGridDem.GetNumOfY();
	
	double	dSizeOfX,dSizeOfY;
	dSizeOfX= m_pGridDem.GetSizeOfX();
	dSizeOfY= m_pGridDem.GetSizeOfY();
	
	//
	double	dLeft,dRight,dBottom,dTop;
	m_pGridDem.GetDemExt(dLeft,dRight,dBottom,dTop);
	
	//
	double	dRealSize;
	dRealSize= m_pGridDem.GetRealSize();
	
	//
	CString	szPathName;
	szPathName= m_pGridDem.GetPathName();
	
	//
	m_pAspect.SetGridDem(pData,iNumOfX,iNumOfY,dSizeOfX,dSizeOfY,dRealSize,dLeft,dRight,dBottom,dTop,szPathName);
	
	//����
	m_pAspect.OnCalculate(0);	
}

//////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////
void CInterface_DemCreator::OnAspect()
{
	if(m_pGridDem.IsEmpty())
	{
		return;
	}
	
	int		iNumX,iNumY;
	iNumX			= m_pGridDem.GetNumOfX();
	iNumY			= m_pGridDem.GetNumOfY();
	
	//
	HINSTANCE hOld	= ::AfxGetResourceHandle();
	
#ifdef _DEBUG
	HINSTANCE hin	= ::GetModuleHandle("VR_DemCreatorD.dll");
#else
	HINSTANCE hin	= ::GetModuleHandle("VR_DemCreator.dll");
#endif
	
	//
	double	*dSlope;
	dSlope			= m_pAspect.GetAspect();
	
	if(dSlope != NULL)
	{
		CDCR_Statistics	pStatistics;
		//
		pStatistics.SetStatisticsData(iNumX*iNumY,dSlope);
		//
		pStatistics.Statistics();
		
		double	dMin,dMax;
		double	dMean;
		double	dVar;
		double	dStdVar;
		double	dCV;
		double	dSkewness;
		double	dKurtosis;
		double	dRMSE;
		
		pStatistics.GetDataMinMax(dMin,dMax);
		pStatistics.GetDataMean(dMean);
		pStatistics.GetDataVar(dVar);
		pStatistics.GetDataStandardVar(dStdVar);
		pStatistics.GetDataCV(dCV);
		pStatistics.GetDataSkewness(dSkewness);
		pStatistics.GetDataKurtosis(dKurtosis);
		pStatistics.GetDataRMSE(dRMSE);
		
		//
		CSetSlopeDlg	pDlg;
		
		pDlg.iNum				= iNumX*iNumY;
		pDlg.pData				= dSlope;
		
		pDlg.dMin				= dMin;
		pDlg.dMax				= dMax;
		pDlg.dMean				= dMean;
		pDlg.dVar				= dVar;
		pDlg.dStdVar			= dStdVar;
		pDlg.dCV				= dCV;
		pDlg.dSkewness			= dSkewness;
		pDlg.dKurtosis			= dKurtosis;
		pDlg.dRMSE				= dRMSE;
		
		pDlg.DoModal();
	}
	
	::AfxSetResourceHandle(hOld);
}

//////////////////////////////////////////////////////////////////////
// 48 ������������
//////////////////////////////////////////////////////////////////////
void CInterface_DemCreator::OnCalculateProfileCurvature()
{
	//m_pGridDem.OnCalculateProfileCurvature();
	
	if(m_pGridDem.IsEmpty())
	{
		return;
	}

	//��ȡDEM����
	double	*pData;
	pData	= m_pGridDem.GetHeight();

	//
	int		iNumOfX,iNumOfY;
	iNumOfX	= m_pGridDem.GetNumOfX();
	iNumOfY	= m_pGridDem.GetNumOfY();

	double	dSizeOfX,dSizeOfY;
	dSizeOfX= m_pGridDem.GetSizeOfX();
	dSizeOfY= m_pGridDem.GetSizeOfY();

	//
	double	dLeft,dRight,dBottom,dTop;
	m_pGridDem.GetDemExt(dLeft,dRight,dBottom,dTop);
	
	//
	double	dRealSize;
	dRealSize= m_pGridDem.GetRealSize();
	
	//
	//
	CString	szPathName;
	szPathName= m_pGridDem.GetPathName();

	m_pCurvature.SetGridDem(pData,iNumOfX,iNumOfY,dSizeOfX,dSizeOfY,dRealSize,dLeft,dRight,dBottom,dTop,szPathName);

	//����
	m_pCurvature.OnCalculateProfileCurvature();
}

//////////////////////////////////////////////////////////////////////
// 49 ����ƽ������
//////////////////////////////////////////////////////////////////////
void CInterface_DemCreator::OnCalculatePlanCurvature()
{
	//m_pGridDem.OnCalculatePlanCurvature();	
	
	if(m_pGridDem.IsEmpty())
	{
		return;
	}

	//��ȡDEM����
	double	*pData;
	pData	= m_pGridDem.GetHeight();

	//
	int		iNumOfX,iNumOfY;
	iNumOfX	= m_pGridDem.GetNumOfX();
	iNumOfY	= m_pGridDem.GetNumOfY();

	double	dSizeOfX,dSizeOfY;
	dSizeOfX= m_pGridDem.GetSizeOfX();
	dSizeOfY= m_pGridDem.GetSizeOfY();

	//
	double	dLeft,dRight,dBottom,dTop;
	m_pGridDem.GetDemExt(dLeft,dRight,dBottom,dTop);
	
	//
	double	dRealSize;
	dRealSize= m_pGridDem.GetRealSize();
	
	//
	CString	szPathName;
	szPathName= m_pGridDem.GetPathName();

	//
	m_pCurvature.SetGridDem(pData,iNumOfX,iNumOfY,dSizeOfX,dSizeOfY,dRealSize,dLeft,dRight,dBottom,dTop,szPathName);

	//����
	m_pCurvature.OnCalculateProfileCurvature();
}

//////////////////////////////////////////////////////////////////////
// 50 ������������
//////////////////////////////////////////////////////////////////////
void CInterface_DemCreator::OnCalculateTgCurvature()
{
	//m_pGridDem.OnCalculateTgCurvature();	
	
	if(m_pGridDem.IsEmpty())
	{
		return;
	}

	//��ȡDEM����
	double	*pData;
	pData	= m_pGridDem.GetHeight();

	//
	int		iNumOfX,iNumOfY;
	iNumOfX	= m_pGridDem.GetNumOfX();
	iNumOfY	= m_pGridDem.GetNumOfY();

	double	dSizeOfX,dSizeOfY;
	dSizeOfX= m_pGridDem.GetSizeOfX();
	dSizeOfY= m_pGridDem.GetSizeOfY();

	//
	double	dLeft,dRight,dBottom,dTop;
	m_pGridDem.GetDemExt(dLeft,dRight,dBottom,dTop);
	
	//
	double	dRealSize;
	dRealSize= m_pGridDem.GetRealSize();
	
	//
	CString	szPathName;
	szPathName= m_pGridDem.GetPathName();
	//
	m_pCurvature.SetGridDem(pData,iNumOfX,iNumOfY,dSizeOfX,dSizeOfY,dRealSize,dLeft,dRight,dBottom,dTop,szPathName);

	//����
	m_pCurvature.OnCalculateProfileCurvature();
}

//////////////////////////////////////////////////////////////////////
// 51 �������ʵ�ͳ����Ϣ
//////////////////////////////////////////////////////////////////////
void CInterface_DemCreator::OnCurvature(int iType)
{
	/*
	HINSTANCE hOld	= ::AfxGetResourceHandle();

	#ifdef _DEBUG
		HINSTANCE hin	= ::GetModuleHandle("VR_DemCreatorD.dll");
	#else
		HINSTANCE hin	= ::GetModuleHandle("VR_DemCreator.dll");
	#endif

	//
	double	*dCurvature;
	
	if(iType == 1)	dCurvature		= m_pGridDem.GetProfileCurvature();
	if(iType == 2)	dCurvature		= m_pGridDem.GetPlanCurvature();
	if(iType == 3)	dCurvature		= m_pGridDem.GetTgCurvature();

	int		iNumX,iNumY;
	iNumX			= m_pGridDem.GetNumOfX();
	iNumY			= m_pGridDem.GetNumOfY();
	
	if(dCurvature == NULL)	return;

	CDCR_Statistics	pStatistics;
	//
	pStatistics.SetStatisticsData(iNumX*iNumY,dCurvature);
	//
	pStatistics.Statistics();
	
	double	dMin,dMax;
	double	dMean;
	double	dVar;
	double	dStdVar;
	double	dCV;
	double	dSkewness;
	double	dKurtosis;

	pStatistics.GetDataMinMax(dMin,dMax);
	pStatistics.GetDataMean(dMean);
	pStatistics.GetDataVar(dVar);
	pStatistics.GetDataStandardVar(dStdVar);
	pStatistics.GetDataCV(dCV);
	pStatistics.GetDataSkewness(dSkewness);
	pStatistics.GetDataKurtosis(dKurtosis);
	pStatistics.Save(m_szBasePathName + "\\Lib\\Creator\\StaticsCurvature.txt");

	//
	CSetCurvatureDlg	pDlg;

	pDlg.iNumX				= iNumX;
	pDlg.iNumY				= iNumY;
	pDlg.dCurvature			= dCurvature;

	pDlg.dMin				= dMin;
	pDlg.dMax				= dMax;
	pDlg.dMean				= dMean;
	pDlg.dVar				= dVar;
	pDlg.dStdVar			= dStdVar;
	pDlg.dCV				= dCV;
	pDlg.dSkewness			= dSkewness;
	pDlg.dKurtosis			= dKurtosis;

	pDlg.DoModal();

	::AfxSetResourceHandle(hOld);
	*/
}

//////////////////////////////////////////////////////////////////////
// 52 ����ر�ֲڶ�
//////////////////////////////////////////////////////////////////////
void CInterface_DemCreator::OnCalculateTerrainRoughness()
{
	//m_pGridDem.OnCalculateTerrainRoughness();
	if(m_pGridDem.IsEmpty())
	{
		return;
	}

	//��ȡDEM����
	double	*pData;
	pData	= m_pGridDem.GetHeight();

	//
	int		iNumOfX,iNumOfY;
	iNumOfX	= m_pGridDem.GetNumOfX();
	iNumOfY	= m_pGridDem.GetNumOfY();

	double	dSizeOfX,dSizeOfY;
	dSizeOfX= m_pGridDem.GetSizeOfX();
	dSizeOfY= m_pGridDem.GetSizeOfY();

	//
	double	dLeft,dRight,dBottom,dTop;
	m_pGridDem.GetDemExt(dLeft,dRight,dBottom,dTop);
	
	//
	double	dRealSize;
	dRealSize= m_pGridDem.GetRealSize();
	
	//
	CString	szPathName;
	szPathName= m_pGridDem.GetPathName();
	//
	m_pTerrainRoughness.SetGridDem(pData,iNumOfX,iNumOfY,dSizeOfX,dSizeOfY,dRealSize,dLeft,dRight,dBottom,dTop,szPathName);

	//����
	m_pTerrainRoughness.OnCalculate();
}

//46 �����¶ȣ��������¶ȵ�ͳ����Ϣ
void CInterface_DemCreator::OnTerrainRoughness()
{
	if(m_pGridDem.IsEmpty())
	{
		return;
	}

	int		iNumX,iNumY;
	iNumX			= m_pGridDem.GetNumOfX();
	iNumY			= m_pGridDem.GetNumOfY();

	//
	HINSTANCE hOld	= ::AfxGetResourceHandle();

	#ifdef _DEBUG
		HINSTANCE hin	= ::GetModuleHandle("VR_DemCreatorD.dll");
	#else
		HINSTANCE hin	= ::GetModuleHandle("VR_DemCreator.dll");
	#endif

	//
	double	*dRoughness;
	dRoughness			= m_pTerrainRoughness.GetRoughness();

	int		iNum;
	iNum	= 0;
	for(int i=0;i<iNumX*iNumY;i++)
	{
		if(dRoughness[i] != -9999)
			iNum++;
	}

	int		I;
	double	*pTemp;
	pTemp	= new	double[iNum];
	for(i=0,I=0;i<iNumX*iNumY;i++)
	{
		if(dRoughness[i] != -9999)
		{
			pTemp[I]	= dRoughness[i];
			I++;
		}
	}

	if(pTemp != NULL)
	{
		CDCR_Statistics	pStatistics;
		//
		pStatistics.SetStatisticsData(iNum,pTemp);
		//
		pStatistics.Statistics();
		
		double	dMin,dMax;
		double	dMean;
		double	dVar;
		double	dStdVar;
		double	dCV;
		double	dSkewness;
		double	dKurtosis;
		double	dRMSE;

		pStatistics.GetDataMinMax(dMin,dMax);
		pStatistics.GetDataMean(dMean);
		pStatistics.GetDataVar(dVar);
		pStatistics.GetDataStandardVar(dStdVar);
		pStatistics.GetDataCV(dCV);
		pStatistics.GetDataSkewness(dSkewness);
		pStatistics.GetDataKurtosis(dKurtosis);
		pStatistics.GetDataRMSE(dRMSE);

		//
		CSetSlopeDlg	pDlg;

		pDlg.iNum				= iNum;
		pDlg.pData				= pTemp;

		pDlg.dMin				= dMin;
		pDlg.dMax				= dMax;
		pDlg.dMean				= dMean;
		pDlg.dVar				= dVar;
		pDlg.dStdVar			= dStdVar;
		pDlg.dCV				= dCV;
		pDlg.dSkewness			= dSkewness;
		pDlg.dKurtosis			= dKurtosis;
		pDlg.dRMSE				= dRMSE;

		pDlg.DoModal();
	}

	if(pTemp != NULL)	delete[]pTemp;

	::AfxSetResourceHandle(hOld);
}

//////////////////////////////////////////////////////////////////////
// 53 ������������
//////////////////////////////////////////////////////////////////////
void CInterface_DemCreator::OnCalculateTerrainRelif()
{
	if(m_pGridDem.IsEmpty())	
	{
		MessageBox(NULL,"��������DEM����!","��ʾ��Ϣ",MB_OK);	
		return;
	}

	//��ȡDEM����
	double	*pData;
	pData	= m_pGridDem.GetHeight();

	//
	int		iNumOfX,iNumOfY;
	iNumOfX	= m_pGridDem.GetNumOfX();
	iNumOfY	= m_pGridDem.GetNumOfY();

	double	dSizeOfX,dSizeOfY;
	dSizeOfX= m_pGridDem.GetSizeOfX();
	dSizeOfY= m_pGridDem.GetSizeOfY();

	//
	double	dLeft,dRight,dBottom,dTop;
	m_pGridDem.GetDemExt(dLeft,dRight,dBottom,dTop);
	
	//
	double	dRealSize;
	dRealSize= m_pGridDem.GetRealSize();
	
	//
	CString	szPathName;
	szPathName= m_pGridDem.GetPathName();
	
	CSetTerrainRelifDlg	pDlg;
	
	pDlg.m_szDemName	= szPathName;
	pDlg.m_iDemNumX		= iNumOfX;
	pDlg.m_iDemNumY		= iNumOfY;
	pDlg.m_dDemSizeX	= dSizeOfX;
	pDlg.m_dDemSizeY	= dSizeOfY;
	
	if(pDlg.DoModal() == IDOK)
	{
	
	}
	else	return;
	
	int		iWindowSize;
	double	dWindowRange;
	iWindowSize	= pDlg.m_iWindowSize;
	dWindowRange= pDlg.m_dWindowRange;

	//������������
	m_pTerrainRelif.SetGridDem(pData,iNumOfX,iNumOfY,dSizeOfX,dSizeOfY,dRealSize,dLeft,dRight,dBottom,dTop,szPathName);
	m_pTerrainRelif.OnCalculate(iWindowSize);
	
	//��ȡ�����������Ϣ���ֱ���ȡ���ֵ����Сֵ�;�ֵ
	double	dMaxTerrainRelif,dMinTerrainRelif,dMeanTerrainRelif;
	dMaxTerrainRelif	= -9999.0;
	dMinTerrainRelif	= -9999.0;
	dMeanTerrainRelif	= -9999.0;

	double	*pTerrainRelif;
	pTerrainRelif		= m_pTerrainRelif.GetRelif();
		
	if(pTerrainRelif != NULL)
	{
		//
		bool	bFirstValue	= true;
		int		iCount		= 0;
		for(int i=0;i<pDlg.m_iDemNumX*pDlg.m_iDemNumY;i++)
		{
			if(pTerrainRelif[i]!=-9999.0)
			{
				if(bFirstValue == true)
				{
					dMaxTerrainRelif	= pTerrainRelif[i];
					dMinTerrainRelif	= pTerrainRelif[i];
					dMeanTerrainRelif	= pTerrainRelif[i];

					bFirstValue	= false;
					iCount++;
				}
				else
				{
					if(dMaxTerrainRelif < pTerrainRelif[i])	dMaxTerrainRelif	= pTerrainRelif[i];
					if(dMinTerrainRelif > pTerrainRelif[i])	dMinTerrainRelif	= pTerrainRelif[i];

					dMeanTerrainRelif	= dMeanTerrainRelif + pTerrainRelif[i];
					iCount++;
				}
			}
		}
		
		//-9999Ϊ��Чֵ
		if(iCount != 0)		dMeanTerrainRelif	= dMeanTerrainRelif / iCount;
		
		//��ʾ
		{
			CSetTerrainRelifResultDlg	pDlg;

			pDlg.m_iWindowSize	= iWindowSize;
			pDlg.m_dWindowRange	= dWindowRange;

			pDlg.m_dMaxValue	= dMaxTerrainRelif;
			pDlg.m_dMinValue	= dMinTerrainRelif;
			pDlg.m_dMeanValue	= dMeanTerrainRelif;

			pDlg.DoModal();
		}
	}
}

//////////////////////////////////////////////////////////////////////
// 54 �����������������
//////////////////////////////////////////////////////////////////////
void CInterface_DemCreator::OnCalculateTerrainRelifInBat()
{
	CSetTerrainFactorDlg	pDlg;
	CString					szBasePathName;
	
	pDlg.m_szWindowName	= "�����������������";
	if(pDlg.DoModal() == IDOK)
	{
		
	}
	else
	{
		//
		return;
	}
	szBasePathName	= pDlg.m_szPathName;

	//��ȡ����
	if(szBasePathName.CompareNoCase("")==0)
	{
		MessageBox(NULL,"��������·��!", "��ʾ��Ϣ", MB_ICONEXCLAMATION  | MB_OK);
		return;
	}	

	int		nNumItem;
	CString	arrFileName[2048];

	nNumItem		= 0;
	
	////////////////////////////////////////////////////////////////
	//��ȡ�ļ���Ϣ
	{
		CString			strDirName;
		WIN32_FIND_DATA wfd;
		HANDLE			hFind;
		CString			strText;
		CString			strTitleName;

		strDirName	= szBasePathName+"\\";
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
	////////////////////////////////////////////////////////////////

	int		iWindowSizeCount;
	int		iWindowSize[1024];

	iWindowSizeCount	= 0;
	//��ȡ��ģ��������
	//��ģ�������ڷ���ϵͳĿ¼�¡�
	FILE			*fr;
	fr	= fopen(m_szExePathName + "\\Lib\\Creator\\TerrainRelifBat.txt","r");
	if(fr != NULL)
	{
		fscanf(fr,"%ld",&iWindowSizeCount);
		
		for(int j=0;j<iWindowSizeCount;j++)
		{
			int	iSize;
			fscanf(fr,"%ld",&iSize);

			iWindowSize[j]	= iSize;
		}
		
		fclose(fr);		
	}

	if(iWindowSizeCount == 0)	return;

	//
	CString	szSavePathName;
	szSavePathName	= szBasePathName + "\\TerrainRelif_Result.txt";
	FILE	*fw;
	fw	= fopen(szSavePathName,"w");
	if(fw != NULL)
	{
		//��ʼ������
		//
		if(m_OnProcess != NULL)
			m_OnProcess(NULL,nNumItem * iWindowSizeCount,0);
		
		int	iSerial	= 0;

		//ѭ��GRD�ļ�
		for(int i=0;i<nNumItem;i++)
		{
			//************************************//
			//д���ļ���
			fprintf(fw,"%s\n",arrFileName[i]);
			//************************************//

			//
			//��һ������������
			char	chGrdPathName[128];
			strcpy(chGrdPathName,szBasePathName + "\\" + arrFileName[i]);
			m_pGridDem.OnImport(chGrdPathName);
			
			//�ڶ��������м��������
			//�ֱ�������е������ֵ
			for(int I=0;I<iWindowSizeCount;I++)
			{
				//��ȡDEM����
				double	*pData;
				pData	= m_pGridDem.GetHeight();

				//
				int		iNumOfX,iNumOfY;
				iNumOfX	= m_pGridDem.GetNumOfX();
				iNumOfY	= m_pGridDem.GetNumOfY();

				double	dSizeOfX,dSizeOfY;
				dSizeOfX= m_pGridDem.GetSizeOfX();
				dSizeOfY= m_pGridDem.GetSizeOfY();

				//
				double	dLeft,dRight,dBottom,dTop;
				m_pGridDem.GetDemExt(dLeft,dRight,dBottom,dTop);
				
				//
				double	dRealSize;
				dRealSize= m_pGridDem.GetRealSize();
				
				//
				CString	szPathName;
				szPathName= m_pGridDem.GetPathName();

				//������������
				{
					//������������
					m_pTerrainRelif.SetGridDem(pData,iNumOfX,iNumOfY,dSizeOfX,dSizeOfY,dRealSize,dLeft,dRight,dBottom,dTop,szPathName);
					m_pTerrainRelif.OnCalculate(iWindowSize[I]);
					
					double	dMaxTerrainRelif,dMinTerrainRelif,dMeanTerrainRelif;
					dMaxTerrainRelif	= -9999.0;
					dMinTerrainRelif	= -9999.0;
					dMeanTerrainRelif	= -9999.0;

					double	*pTerrainRelif;
					pTerrainRelif		= m_pTerrainRelif.GetRelif();
					
					if(pTerrainRelif != NULL)
					{
						//
						bool	bFirstValue	= true;
						int		iCountNum	= 0;
						for(int j=0;j<iNumOfX*iNumOfY;j++)
						{
							if(pTerrainRelif[j]!=-9999.0)
							{
								if(bFirstValue == true)
								{
									dMaxTerrainRelif	= pTerrainRelif[j];
									dMinTerrainRelif	= pTerrainRelif[j];
									dMeanTerrainRelif	= pTerrainRelif[j];

									bFirstValue	= false;
									iCountNum++;
								}
								else
								{
									if(dMaxTerrainRelif < pTerrainRelif[j])	dMaxTerrainRelif	= pTerrainRelif[j];
									if(dMinTerrainRelif > pTerrainRelif[j])	dMinTerrainRelif	= pTerrainRelif[j];

									dMeanTerrainRelif	= dMeanTerrainRelif + pTerrainRelif[j];
									iCountNum++;
								}
							}
						}
						
						//-9999Ϊ��Чֵ
						if(iCountNum != 0)		dMeanTerrainRelif	= dMeanTerrainRelif / iCountNum;

						//************************************//
						//���棬��������
						if(fw != NULL)
						{
							//�ߴ� ���ֵ ƽ��ֵ
							double	dRange;
							dRange	= (dSizeOfX+dSizeOfY)*iWindowSize[I];
							
							fprintf(fw,"%10.0lf %10.6lf %10.6lf\n",dRange,dMaxTerrainRelif,dMeanTerrainRelif);
						}
						//************************************//
					}

					//
					m_pTerrainRelif.OnRelease();
				}

				iSerial++;
				if(m_OnProcess != NULL)
					m_OnProcess(NULL,nNumItem * iWindowSizeCount,iSerial);
			}

			//ɾ��DEM����
			m_pGridDem.OnRelease();
		}

		fclose(fw);
	}
	
	if(m_OnProcess != NULL)
		m_OnProcess(NULL,1,-1);

	//MessageBox(NULL,"��������ȴ������!","��ʾ��Ϣ",MB_OK);
}

//////////////////////////////////////////////////////////////////////
// 55 ����̱߳���ϵ��
//////////////////////////////////////////////////////////////////////
void CInterface_DemCreator::OnCalculateTerrainCV()
{
	if(m_pGridDem.IsEmpty())	
	{
		MessageBox(NULL,"��������DEM����!","��ʾ��Ϣ",MB_OK);	
		return;
	}
	
	//��ȡDEM����
	double	*pData;
	pData	= m_pGridDem.GetHeight();

	//
	int		iNumOfX,iNumOfY;
	iNumOfX	= m_pGridDem.GetNumOfX();
	iNumOfY	= m_pGridDem.GetNumOfY();

	double	dSizeOfX,dSizeOfY;
	dSizeOfX= m_pGridDem.GetSizeOfX();
	dSizeOfY= m_pGridDem.GetSizeOfY();

	//
	double	dLeft,dRight,dBottom,dTop;
	m_pGridDem.GetDemExt(dLeft,dRight,dBottom,dTop);
	
	//
	double	dRealSize;
	dRealSize= m_pGridDem.GetRealSize();
	
	//
	CString	szPathName;
	szPathName= m_pGridDem.GetPathName();
	
	CSetTerrainCVDlg	pDlg;
	
	pDlg.m_szDemName	= szPathName;
	pDlg.m_iDemNumX		= iNumOfX;
	pDlg.m_iDemNumY		= iNumOfY;
	pDlg.m_dDemSizeX	= dSizeOfX;
	pDlg.m_dDemSizeY	= dSizeOfY;
	
	if(pDlg.DoModal() == IDOK)
	{
	
	}
	else	return;
	
	int		iWindowSize;
	double	dWindowRange;
	iWindowSize	= pDlg.m_iWindowSize;
	dWindowRange= pDlg.m_dWindowRange;
	
	//������������
	m_pTerrainCV.SetGridDem(pData, iNumOfX, iNumOfY, dSizeOfX, dSizeOfY, dRealSize, dLeft, dRight, dBottom, dTop, szPathName);

	m_pTerrainCV.OnCalculate(iWindowSize);

	//��ȡ�����������Ϣ���ֱ���ȡ���ֵ����Сֵ�;�ֵ
	double	dMaxTerrainCV,dMinTerrainCV,dMeanTerrainCV;
	dMaxTerrainCV	= -9999.0;
	dMinTerrainCV	= -9999.0;
	dMeanTerrainCV	= -9999.0;

	double	*pTerrainCV;
	pTerrainCV		= m_pTerrainCV.GetCV();
	
	//
	bool	bFirstValue	= true;
	int		iCount		= 0;
	for(int i=0;i<iNumOfX*iNumOfY;i++)
	{
		if(pTerrainCV[i]!=-9999.0)
		{
			if(bFirstValue == true)
			{
				dMaxTerrainCV	= pTerrainCV[i];
				dMinTerrainCV	= pTerrainCV[i];
				dMeanTerrainCV	= pTerrainCV[i];

				bFirstValue	= false;
				iCount++;
			}
			else
			{
				if(dMaxTerrainCV < pTerrainCV[i])	dMaxTerrainCV	= pTerrainCV[i];
				if(dMinTerrainCV > pTerrainCV[i])	dMinTerrainCV	= pTerrainCV[i];

				dMeanTerrainCV	= dMeanTerrainCV + pTerrainCV[i];
				iCount++;
			}
		}
	}
	
	//-9999Ϊ��Чֵ
	if(iCount != 0)		dMeanTerrainCV	= dMeanTerrainCV / iCount;
	
	//��ʾ
	{
		CSetTerrainCVResultDlg	pDlg;

		pDlg.m_iWindowSize	= iWindowSize;
		pDlg.m_dWindowRange	= dWindowRange;

		pDlg.m_dMaxValue	= dMaxTerrainCV;
		pDlg.m_dMinValue	= dMinTerrainCV;
		pDlg.m_dMeanValue	= dMeanTerrainCV;

		pDlg.DoModal();
	}	
}

//////////////////////////////////////////////////////////////////////
// 56 ���������̱߳���ϵ��
//////////////////////////////////////////////////////////////////////
void CInterface_DemCreator::OnCalculateTerrainCVInBat()
{
	CSetTerrainFactorDlg	pDlg;
	CString					szBasePathName;
	
	pDlg.m_szWindowName	= "���θ̱߳���ϵ����������";
	if(pDlg.DoModal() == IDOK)
	{
		
	}
	else
	{
		//
		return;
	}
	szBasePathName	= pDlg.m_szPathName;

	//��ȡ����
	if(szBasePathName.CompareNoCase("")==0)
	{
		MessageBox(NULL,"��������·��!", "��ʾ��Ϣ", MB_ICONEXCLAMATION  | MB_OK);
		return;
	}	

	int		nNumItem;
	CString	arrFileName[2048];

	nNumItem		= 0;
	
	////////////////////////////////////////////////////////////////
	//��ȡ�ļ���Ϣ
	{
		CString			strDirName;
		WIN32_FIND_DATA wfd;
		HANDLE			hFind;
		CString			strText;
		CString			strTitleName;

		strDirName	= szBasePathName+"\\";
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
	////////////////////////////////////////////////////////////////

	int		iWindowSizeCount;
	int		iWindowSize[1024];

	iWindowSizeCount	= 0;
	//��ȡ��ģ��������
	//��ģ�������ڷ���ϵͳĿ¼�¡�
	FILE			*fr;
	fr	= fopen(m_szExePathName + "\\Lib\\Creator\\TerrainCVBat.txt","r");
	if(fr != NULL)
	{
		fscanf(fr,"%ld",&iWindowSizeCount);
		
		for(int j=0;j<iWindowSizeCount;j++)
		{
			int	iSize;
			fscanf(fr,"%ld",&iSize);

			iWindowSize[j]	= iSize;
		}
		
		fclose(fr);		
	}

	if(iWindowSizeCount == 0)	return;

	//
	CString	szSavePathName;
	szSavePathName	= szBasePathName + "\\TerrainCV_Result.txt";
	FILE	*fw;
	fw	= fopen(szSavePathName,"w");
	if(fw != NULL)
	{
		//��ʼ������
		//ѭ��GRD�ļ�
		for(int i=0;i<nNumItem;i++)
		{
			//************************************//
			//д���ļ���
			fprintf(fw,"%s\n",arrFileName[i]);
			//************************************//

			//
			//��һ������������
			char	chGrdPathName[128];
			strcpy(chGrdPathName,szBasePathName + "\\" + arrFileName[i]);
			m_pGridDem.OnImport(chGrdPathName);
			
			//�ڶ��������м��������
			//�ֱ�������е������ֵ
			for(int I=0;I<iWindowSizeCount;I++)
			{
				//��ȡDEM����
				double	*pData;
				pData	= m_pGridDem.GetHeight();

				//
				int		iNumOfX,iNumOfY;
				iNumOfX	= m_pGridDem.GetNumOfX();
				iNumOfY	= m_pGridDem.GetNumOfY();

				double	dSizeOfX,dSizeOfY;
				dSizeOfX= m_pGridDem.GetSizeOfX();
				dSizeOfY= m_pGridDem.GetSizeOfY();

				//
				double	dLeft,dRight,dBottom,dTop;
				m_pGridDem.GetDemExt(dLeft,dRight,dBottom,dTop);
				
				//
				double	dRealSize;
				dRealSize= m_pGridDem.GetRealSize();
				
				//
				CString	szPathName;
				szPathName= m_pGridDem.GetPathName();

				//������������
				{
					m_pTerrainCV.SetGridDem(pData, iNumOfX, iNumOfY, dSizeOfX, dSizeOfY, dRealSize, dLeft, dRight, dBottom, dTop, szPathName);

					m_pTerrainCV.OnCalculate(iWindowSize[I]);
					
					//��ȡ���θ̱߳���ϵ����Ϣ���ֱ���ȡ���ֵ����Сֵ�;�ֵ
					double	dMaxTerrainCV,dMinTerrainCV,dMeanTerrainCV;
					dMaxTerrainCV	= -9999.0;
					dMinTerrainCV	= -9999.0;
					dMeanTerrainCV	= -9999.0;

					double	*pTerrainCV;
					pTerrainCV		= m_pTerrainCV.GetCV();
					
					//
					bool	bFirstValue	= true;
					int		iCVCount	= 0;
					for(int j=0;j<iNumOfX*iNumOfY;j++)
					{
						if(pTerrainCV[j]!=-9999.0)
						{
							if(bFirstValue == true)
							{
								dMaxTerrainCV	= pTerrainCV[j];
								dMinTerrainCV	= pTerrainCV[j];
								dMeanTerrainCV	= pTerrainCV[j];

								bFirstValue	= false;
								iCVCount++;
							}
							else
							{
								if(dMaxTerrainCV < pTerrainCV[j])	dMaxTerrainCV	= pTerrainCV[j];
								if(dMinTerrainCV > pTerrainCV[j])	dMinTerrainCV	= pTerrainCV[j];

								dMeanTerrainCV	= dMeanTerrainCV + pTerrainCV[j];
								iCVCount++;
							}
						}
					}
					
					//-9999Ϊ��Чֵ
					if(iCVCount != 0)		dMeanTerrainCV	= dMeanTerrainCV / iCVCount;

					/////////////////////////////////////////////
					//���棬��������
					if(fw != NULL)
					{
						//�ߴ� ���ֵ ƽ��ֵ
						double	dRange;
						dRange	= (dSizeOfX+dSizeOfY)*iWindowSize[I];
						
						fprintf(fw,"%10.0lf %10.6lf %10.6lf\n",dRange,dMaxTerrainCV,dMeanTerrainCV);
					}
					/////////////////////////////////////////////

					//
					m_pTerrainCV.OnRelease();
				}
			}

			//ɾ��DEM����
			m_pGridDem.OnRelease();
		}

		fclose(fw);
	}

	//MessageBox(NULL,"���θ̱߳���ϵ���������!","��ʾ��Ϣ",MB_OK);
}

//////////////////////////////////////////////////////////////////////
// 57 ��������
//////////////////////////////////////////////////////////////////////
void CInterface_DemCreator::OnCalculateSurface()
{
	if(m_pGridDem.IsEmpty())	
	{
		MessageBox(NULL,"��������DEM����!","��ʾ��Ϣ",MB_OK);	
		return;
	}

	double	dS;	
	double	dV;
	dS	= m_pGridDem.OnCalculateSurface(dV);

	CString	szInfo;
	szInfo.Format("��%s�� �ı����Ϊ %lf",m_pGridDem.GetPathName(), dS);
	
	MessageBox(NULL,szInfo,"��ʾ��Ϣ",MB_OK);
}

//////////////////////////////////////////////////////////////////////
// 58 �������ά��
//////////////////////////////////////////////////////////////////////
void CInterface_DemCreator::OnCalculateFractal()
{
	if(m_pGridDem.IsEmpty())	
	{
		MessageBox(NULL,"��������DEM����!","��ʾ��Ϣ",MB_OK);	
		return;
	}
	
	CSetFractalDlg	pDlg;
	
	pDlg.m_szDemName= m_pGridDem.GetPathName();
	pDlg.m_dSizeX	= m_pGridDem.GetSizeOfX();
	pDlg.m_dSizeY	= m_pGridDem.GetSizeOfY();
	pDlg.m_iNumX	= m_pGridDem.GetNumOfX();
	pDlg.m_iNumY	= m_pGridDem.GetNumOfY();

	if(pDlg.DoModal() == IDOK)
	{

	}
	else	return;

	//
	double				dZScale;
	dZScale		= pDlg.m_dZScale;
	
	//
	m_pGridDem.SetZScale(dZScale);

	//���
	int					iScaleCount;
	double				dScaleSize[50];
	iScaleCount	= pDlg.m_iScaleCount;
	for(int i=0;i<iScaleCount;i++)
	{
		dScaleSize[i]	= pDlg.m_dScaleList[i];
	}
	
	double				dSurface[50];
	double				dV[50];
	double				dTemp;
	
	//���ȼ��㱾�����Ϣ
	for(i=0;i<iScaleCount;i++)
	{
		CDCR_Grid_Fractal	pFractal;
		pFractal.m_pGridDem.OnClone(&m_pGridDem);
		
		//
		pFractal.SetSize(dScaleSize[i]);
		pFractal.CalculateDem();

		dSurface[i]	= pFractal.OnCalculateSurface(dTemp);
		dV[i]		= dTemp;
	}

	double	lnAr[50];
	double	lnr[50];

	double	lnVr[50];

	for(i=0;i<iScaleCount;i++)
	{
		//lnAr[i]	= log10((dSurface[i]/(iSize[i]*iSize[i])));
		//lnCr[i]	= log10((pow(dV[i],1.0/3)/iSize[i]));

		lnAr[i]	= log((dSurface[i]/(dScaleSize[i]*dScaleSize[i])));
		lnr[i]	= log(dScaleSize[i]);

		lnVr[i]	= log(pow(dV[i],1.0/3)/dScaleSize[i]);
	}

	//���Ժ������
	//double	dFractal1,dFractal2;
	{
		/*
		//����MatCom
		initM(MATCOM_VERSION);
			
		Mm	maxtixLnAr,maxtixLnr,maxtixlnVr;

		maxtixLnAr		= zeros(1,iScaleCount);
		maxtixLnr		= zeros(1,iScaleCount);
		maxtixlnVr		= zeros(1,iScaleCount);

		for(i=0;i<iScaleCount;i++)
		{
			maxtixLnAr.r(i+1)	= lnAr[i];
			maxtixLnr.r(i+1)	= lnr[i];
			maxtixlnVr.r(i+1)	= lnVr[i];
		} 
		
		//
		Mm maxtixResult1;
		maxtixResult1	= FractalLineFit(maxtixLnr,maxtixLnAr);
		//�������߶ȵķ���ά��
		dFractal1		= fabs(maxtixResult1.r(1));

		Mm maxtixResult2;
		maxtixResult2	= FractalLineFit(maxtixlnVr,maxtixLnAr);
		//��������ἰ�ķ���ά��
		dFractal2		= fabs(maxtixResult2.r(1));

		//�˳�MatCom
		//exitM();	
		*/
	}
	
	//�Ѹ߳�ֵ����
	m_pGridDem.UnSetZScale(dZScale);

	CSetFractalResultDlg	pResultDlg;

	pResultDlg.m_szPathName	= m_pGridDem.GetPathName();
	//pResultDlg.m_dResult1	= dFractal1;
	//pResultDlg.m_dResult2	= dFractal2;

	pResultDlg.DoModal();
}

//////////////////////////////////////////////////////////////////////
// 59 �������������������Ϣ
//////////////////////////////////////////////////////////////////////
void CInterface_DemCreator::OnCalculateTerrainFactorInBat()
{
	//�������������������Ϣ��
	//�������̡߳���С�̡߳�ƽ���̡߳��̱߳���ϵ�����¶ȡ�ƽ�����ʡ��������ʡ��ر�ֲڶȡ��ر������
	
	CSetTerrainFactorDlg	pDlg;
	CString					szPathName;
	
	if(pDlg.DoModal() == IDOK)
	{

	}
	else
	{
		//
		return;
	}
	szPathName	= pDlg.m_szPathName;

	//��ȡ����
	if(szPathName.CompareNoCase("")==0)
	{
		MessageBox(NULL,"��������·��!", "��ʾ��Ϣ", MB_ICONEXCLAMATION  | MB_OK);
		return;
	}	

	int		nNumItem;
	CString	arrFileName[2048];

	nNumItem		= 0;

	//��ȡ�ļ���Ϣ
	{
		CString			strDirName;
		WIN32_FIND_DATA wfd;
		HANDLE			hFind;
		CString			strText;
		CString			strTitleName;

		strDirName	= szPathName+"\\";
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
	
	CString	szSavePathName;
	szSavePathName	= szPathName + "\\TerrainFactor.txt";
	
	FILE	*fp;
	fp	= fopen(szSavePathName,"w");

	if(fp != NULL)
	{
		//д���ļ�ͷ��Ϣ
		{
			fprintf(fp,"�ļ���\t ");

			fprintf(fp,"��͸߳�\t ");
			fprintf(fp,"��߸߳�\t ");
			fprintf(fp,"ƽ���߳�\t ");
			fprintf(fp,"�߲�\t ");
			fprintf(fp,"�̱߳�׼��\t ");

			fprintf(fp,"��С�¶�\t ");
			fprintf(fp,"����¶�\t ");
			fprintf(fp,"ƽ���¶�\t ");
			fprintf(fp,"ƽ���¶ȱ�׼��\t ");

			fprintf(fp,"��С�¶ȱ���\t ");
			fprintf(fp,"����¶ȱ���\t ");
			fprintf(fp,"ƽ���¶ȱ���\t ");
			fprintf(fp,"ƽ���¶ȱ��ʱ�׼��\t ");

			fprintf(fp,"��Сƽ������\t ");
			fprintf(fp,"���ƽ������\t ");
			fprintf(fp,"ƽ������\t ");
			fprintf(fp,"ƽ�����ʱ�׼��\t ");

			fprintf(fp,"��С��������\t ");
			fprintf(fp,"�����������\t ");
			fprintf(fp,"��������\t ");
			fprintf(fp,"�������ʱ�׼��\t ");

			fprintf(fp,"��С�ر�ֲڶ�\t ");
			fprintf(fp,"���ر�ֲڶ�\t ");
			fprintf(fp,"�ر�ֲڶ�\t ");
			fprintf(fp,"�ر�ֲڶȱ�׼��\t ");

			fprintf(fp,"��С���������\t ");
			fprintf(fp,"�����������\t ");
			fprintf(fp,"���������\t ");
			fprintf(fp,"��������ȱ�׼��\t ");

			fprintf(fp,"��С�̱߳���ϵ��\t ");
			fprintf(fp,"���̱߳���ϵ��\t ");
			fprintf(fp,"�̱߳���ϵ��\t ");
			fprintf(fp,"�̱߳���ϵ����׼��\t ");

			fprintf(fp,"��С�ر��и����\t ");
			fprintf(fp,"���ر��и����\t ");
			fprintf(fp,"�ر��и����\t ");
			fprintf(fp,"�ر��и���ȱ�׼��\t ");

			fprintf(fp,"����ά��\n");
		}

		for(int i=0;i<nNumItem;i++)
		{
			//��͸߳�
			double	dMinHeight			= -32767;
			double	dMaxHeight			= -32767;
			double	dMeanHeight			= -32767;
			double	dDeltaHeight		= -32767;
			double	dStdHeight			= -32767;

			//ƽ���¶�
			double	dMinSlope			= -32767;
			double	dMaxSlope			= -32767;
			double	dMeanSlope			= -32767;
			double	dStdSlope			= -32767;

			//ƽ���¶ȱ���
			double	dMinSlopeOfSlope	= -32767;
			double	dMaxSlopeOfSlope	= -32767;
			double	dMeanSlopeOfSlope	= -32767;
			double	dStdSlopeOfSlope	= -32767;

			//ƽ������
			double	dMinPlan			= 0;
			double	dMaxPlan			= 0;
			double	dMeanPlan			= 0;
			double	dStdPlan			= 0;

			//��������
			double	dMinProfile			= 0;
			double	dMaxProfile			= 0;
			double	dMeanProfile		= 0;
			double	dStdProfile			= 0;

			//�ر�ֲڶ�
			double	dMinRoughness		= 0;
			double	dMaxRoughness		= 0;
			double	dMeanRoughness		= 0;
			double	dStdRoughness		= 0;

			//���������
			double	dMinRelif			= 0;
			double	dMaxRelif			= 0;
			double	dMeanRelif			= 0;
			double	dStdRelif			= 0;
			
			//�̱߳���ϵ��	
			double	dMinCV				= 0;
			double	dMaxCV				= 0;
			double	dMeanCV				= 0;
			double	dStdCV				= 0;

			//�ر��и����
			double	dMinIncision		= 0;
			double	dMaxIncision		= 0;
			double	dMeanIncision		= 0;
			double	dStdIncision		= 0;

			//����ά��
			double	dFractal			= 0;

			//��һ������������
			char	chGrdPathName[128];
			strcpy(chGrdPathName,szPathName + "\\" + arrFileName[i]);
			m_pGridDem.OnImport(chGrdPathName);
			
			//
			int		iNumOfX,iNumOfY;
			iNumOfX		= m_pGridDem.GetNumOfX();
			iNumOfY		= m_pGridDem.GetNumOfY();

			double	dSizeOfX,dSizeOfY;
			dSizeOfX	= m_pGridDem.GetSizeOfX();
			dSizeOfY	= m_pGridDem.GetSizeOfY();

			double	dRealSize;
			dRealSize	= m_pGridDem.GetRealSize();

			double	dLeft,dRight,dBottom,dTop;
			m_pGridDem.GetDemExt(dLeft,dRight,dBottom,dTop);

			CString	szPathName;
			szPathName	= m_pGridDem.GetPathName();

			//�ڶ������߳���Ϣ����
			double	*pDemHeight;
			pDemHeight	= m_pGridDem.GetHeight();
			
			//��ȡ��Ҫ����Ϣ
			{
				//
				CDCR_Statistics	pStatistics;
				//
				pStatistics.SetStatisticsData(iNumOfX*iNumOfY,pDemHeight);
				//
				pStatistics.Statistics();

				pStatistics.GetDataMinMax(dMinHeight,dMaxHeight);
				dDeltaHeight	= dMaxHeight-dMinHeight;
				pStatistics.GetDataMean(dMeanHeight);
				pStatistics.GetDataStandardVar(dStdHeight);
			}

			//���������¶���Ϣ����
			{
				//
				m_pSlope.SetGridDem(pDemHeight,iNumOfX,iNumOfY,dSizeOfX,dSizeOfY,dRealSize,dLeft,dRight,dBottom,dTop,szPathName);

				//�����¶���Ϣ
				m_pSlope.OnCalculate(0);
				m_pSlope.GetSlopeStatisticsInfo(dMinSlope,dMaxSlope,dMeanSlope,dStdSlope);
			}

			//���Ĳ����¶ȱ�����Ϣ����
			{
				//
				m_pSlope.SetGridDem(pDemHeight,iNumOfX,iNumOfY,dSizeOfX,dSizeOfY,dRealSize,dLeft,dRight,dBottom,dTop,szPathName);
				//
				m_pSlope.OnCalculateSlopeOfSlope();
				m_pSlope.GetSlopeOfSlopeStatisticsInfo(dMinSlopeOfSlope,dMaxSlopeOfSlope,dMeanSlopeOfSlope,dStdSlopeOfSlope);
			}
			
			/*
			//���Ĳ���ƽ��������Ϣ����
			{
				m_pCurvature.SetGridDem(pDemHeight,iNumOfX,iNumOfY,dSizeOfX,dSizeOfY,dRealSize,dLeft,dRight,dBottom,dTop,szPathName);
				//
				m_pCurvature.OnCalculatePlanCurvature();
				m_pCurvature.GetPlanCurvatureStatisticsInfo(dMinPlan,dMaxPlan,dMeanPlan,dStdPlan);
			}

			//���岽������������Ϣ����
			{
				m_pCurvature.SetGridDem(pDemHeight,iNumOfX,iNumOfY,dSizeOfX,dSizeOfY,dRealSize,dLeft,dRight,dBottom,dTop,szPathName);
				//
				m_pCurvature.OnCalculateProfileCurvature();
				m_pCurvature.GetProfileCurvatureStatisticsInfo(dMinProfile,dMaxProfile,dMeanProfile,dStdProfile);
			}

			//���������ر�ֲڶ���Ϣ����,�Ž�����2012-01-23 ok��
			{
				m_pTerrainRoughness.SetGridDem(pDemHeight,iNumOfX,iNumOfY,dSizeOfX,dSizeOfY,dRealSize,dLeft,dRight,dBottom,dTop,szPathName);
				m_pTerrainRoughness.OnCalculate();
				m_pTerrainRoughness.GetRoughnessStatisticsInfo(dMinRoughness,dMaxRoughness,dMeanRoughness,dStdRoughness);
			}

			//���߲����ر��������Ϣ����
			{
				m_pTerrainRelif.SetGridDem(pDemHeight,iNumOfX,iNumOfY,dSizeOfX,dSizeOfY,dRealSize,dLeft,dRight,dBottom,dTop,szPathName);
				m_pTerrainRelif.OnCalculate();
				m_pTerrainRelif.GetRelifStatisticsInfo(dMinRelif,dMaxRelif,dMeanRelif,dStdRelif);
			}

			//�ڰ˲����̱߳���ϵ����Ϣ����
			{
				m_pTerrainCV.SetGridDem(pDemHeight,iNumOfX,iNumOfY,dSizeOfX,dSizeOfY,dRealSize,dLeft,dRight,dBottom,dTop,szPathName);
				m_pTerrainCV.OnCalculate();
				m_pTerrainCV.GetCVStatisticsInfo(dMinCV,dMaxCV,dMeanCV,dStdCV);
			}


			//�ھŲ����ر��и������Ϣ����
			{
				m_pTerrainIncision.SetGridDem(pDemHeight,iNumOfX,iNumOfY,dSizeOfX,dSizeOfY,dRealSize,dLeft,dRight,dBottom,dTop,szPathName);
				m_pTerrainIncision.OnCalculate();
				m_pTerrainIncision.GetIncisionStatisticsInfo(dMinIncision,dMaxIncision,dMeanIncision,dStdIncision);
			}

			//�ھŲ����������ά��
			//�Ȳ������Ժ���˵
			{

			}
			*/
			
			//��ʮ�����ͷ�����
			m_pGridDem.OnRelease();

			//��ʮһ����д������
			{
				//���ȱ����ļ���
				char	chFileName[128];
				strcpy(chFileName,arrFileName[i]);

				fprintf(fp,"%s\t",chFileName);

				//��͸߳�
				fprintf(fp,"%12.6lf\t %12.6lf\t %12.6lf\t %12.6lf\t %12.6lf\t ",dMinHeight,dMaxHeight,dMeanHeight,dDeltaHeight,dStdHeight);

				//ƽ���¶�
				fprintf(fp,"%12.6lf\t %12.6lf\t %12.6lf\t %12.6lf\t ",dMinSlope,dMaxSlope,dMeanSlope,dStdSlope);

				//ƽ���¶ȱ���
				fprintf(fp,"%12.6lf\t %12.6lf\t %12.6lf\t %12.6lf\t ",dMinSlopeOfSlope,dMaxSlopeOfSlope,dMeanSlopeOfSlope,dStdSlopeOfSlope);

				//ƽ������
				fprintf(fp,"%12.6lf\t %12.6lf\t %12.6lf\t %12.6lf\t ",dMinPlan,dMaxPlan,dMeanPlan,dStdPlan);

				//��������
				fprintf(fp,"%12.6lf\t %12.6lf\t %12.6lf\t %12.6lf\t ",dMinProfile,dMaxProfile,dMeanProfile,dStdProfile);

				//�ر�ֲڶ�
				fprintf(fp,"%12.6lf\t %12.6lf\t %12.6lf\t %12.6lf\t ",dMinRoughness,dMaxRoughness,dMeanRoughness,dStdRoughness);

				//���������
				fprintf(fp,"%12.6lf\t %12.6lf\t %12.6lf\t %12.6lf\t ",dMinRelif,dMaxRelif,dMeanRelif,dStdRelif);

				//�̱߳���ϵ��
				fprintf(fp,"%12.6lf\t %12.6lf\t %12.6lf\t %12.6lf\t ",dMinCV,dMaxCV,dMeanCV,dStdCV);

				//�ر��и����
				fprintf(fp,"%12.6lf\t %12.6lf\t %12.6lf\t %12.6lf\t ",dMinIncision,dMaxIncision,dMeanIncision,dStdIncision);

				//����ά��
				fprintf(fp,"%12.6lf\n",dFractal);
			}
		}

		fclose(fp);
	}
}

//////////////////////////////////////////////////////////////////////
//59-1 ������Ϣ������Ϣ,��GRD��ʾ
//////////////////////////////////////////////////////////////////////
void CInterface_DemCreator::OnExportTerrainFactor()
{
	//
	if(!m_pGridDem.IsEmpty())
	{

	}
}

//////////////////////////////////////////////////////////////////////
// 60 ����в�
//////////////////////////////////////////////////////////////////////
void CInterface_DemCreator::OnCalculateRemainsError()
{
	if(m_pGridDem.IsEmpty())	
	{
		MessageBox(NULL,"û��DEM����,���ܽ��вв����!","��ʾ��Ϣ",MB_OK);

		return;
	}

	int		iSampleCount;
	double	*dSampleX;
	double	*dSampleY;
	double	*dSampleZ;
	double	*dSampleCZ;

	iSampleCount	= m_pWorkspace.GetSamplePointCount();

	if(iSampleCount == 0)
	{
		MessageBox(NULL,"û�в�������,���ܽ��вв����!","��ʾ��Ϣ",MB_OK);

		return;
	}

	dSampleX	= new	double[iSampleCount];
	dSampleY	= new	double[iSampleCount];
	dSampleZ	= new	double[iSampleCount];
	dSampleCZ	= new	double[iSampleCount];
	
	//��ȡ��������
	m_pWorkspace.GetSamplePoint(&dSampleX,&dSampleY,&dSampleZ);
	
	//����������ݵ��ڲ�ֵ
	for(int i=0;i<iSampleCount;i++)
	{
		dSampleCZ[i]	= m_pGridDem.GetHeight(dSampleX[i],dSampleY[i]);
	}
	
	//
	CSetRemainsErrorDlg	pDlg;

	pDlg.iSampleCount	= iSampleCount;
	pDlg.dSampleX		= dSampleX;
	pDlg.dSampleY		= dSampleY;
	pDlg.dSampleZ		= dSampleZ;
	pDlg.dSampleCZ		= dSampleCZ;
	pDlg.m_szPathName	= m_szBasePathName;
	
	pDlg.DoModal();

	delete	[]dSampleX;
	delete	[]dSampleY;
	delete	[]dSampleZ;
	delete	[]dSampleCZ;
}

//////////////////////////////////////////////////////////////////////
// 61 ����TIN
//////////////////////////////////////////////////////////////////////
void CInterface_DemCreator::OnTinCreator(CDC *pDC,int iType)
{
	//�����ͷ�����
	m_pTinCreator.Release();

	//
	double	dLeft,dRight,dBottom,dTop;
	m_pWorkspace.GetMapExt(dLeft,dRight,dBottom,dTop);

	m_pTinCreator.SetMapExt(dLeft,dRight,dBottom,dTop);

	//��һ����������
	CDCR_PointFeature	*pPoint;
	pPoint	= m_pWorkspace.GetPointFeature();

	while(pPoint != NULL)
	{
		if(pPoint->m_bUsed == false)
		{
			m_pTinCreator.AddVertex(iType,pPoint->m_pPointCoord.x,pPoint->m_pPointCoord.y,pPoint->m_pPointCoord.z);	
		}

		pPoint	= pPoint->m_pNext;
	}

	//�ڶ�������DEM
	m_pTinCreator.SetCoordMapping(m_dMapXA,m_dMapXB,m_dMapYA,m_dMapYB);
	m_pTinCreator.Create(pDC,iType);
}

//////////////////////////////////////////////////////////////////////
// 62 ����GRID
//////////////////////////////////////////////////////////////////////
void CInterface_DemCreator::OnGridCreator(CDC *pDC,int iType)
{
	if(m_pWorkspace.IsEmpty())	return;

	//�����ͷ�����
	m_pGridCreator.Release();
	
	//���ý�ģ�ķ�Χ
	double	dLeft,dRight,dBottom,dTop;
	m_pWorkspace.GetMapExt(dLeft,dRight,dBottom,dTop);
	m_pGridCreator.SetMapExt(dLeft,dRight,dBottom,dTop);
	
	//���÷���������Ŀ
	int		iCellNumX,iCellNumY;
	m_pWorkspace.GetCellCount(iCellNumX,iCellNumY);
	m_pGridCreator.SetCellCount(iCellNumX,iCellNumY);
	
	//���÷����ĸ����ߴ�
	double	dCellSizeX,dCellSizeY;
	m_pWorkspace.GetCellSize(dCellSizeX,dCellSizeY);
	m_pGridCreator.SetCellSize(dCellSizeX,dCellSizeY);
	
	//���÷����ķ�Χ���ͽ�ģ��Χ��һ��
	double	dCellLeft,dCellRight,dCellBottom,dCellTop;
	m_pWorkspace.GetCellExt(dCellLeft,dCellRight,dCellBottom,dCellTop);
	m_pGridCreator.SetCellExt(dCellLeft,dCellRight,dCellBottom,dCellTop);

	//���÷�������
	m_pGridCreator.SetCell(m_pWorkspace.GetCell());

	//����Grid DEM
	m_pGridCreator.SetCoordMapping(m_dMapXA,m_dMapXB,m_dMapYA,m_dMapYB);

	//��ȡ�����������Ŀ
	m_pGridCreator.SetSampleCount(m_pWorkspace.GetSamplePointCount());

	m_pGridCreator.Create(pDC,iType);

	//��������ˣ�������Ҫ������߳���Ϣת�ŵ�CDCR_GridDem��
	
	CString	szPathName;
	//����DEM��·��
	szPathName	= m_pGridCreator.GetGrdPathName();
	m_pGridDem.SetPathName(szPathName);

	//����DEM�ķ�Χ
	m_pGridDem.SetDemExt(dLeft,dRight,dBottom,dTop);
	
	//���ø����ĵ���
	int	iDemNumX,iDemNumY;
	m_pGridCreator.GetDemCount(iDemNumX,iDemNumY);
	m_pGridDem.SetNumOfX(iDemNumX);
	m_pGridDem.SetNumOfY(iDemNumY);

	//���ø����ĳߴ�
	double	dDemSizeX,dDemSizeY;
	m_pGridCreator.GetDemSize(dDemSizeX,dDemSizeY);
	m_pGridDem.SetSizeOfX(dDemSizeX);
	m_pGridDem.SetSizeOfY(dDemSizeY);
	
	//
	m_pGridDem.SetHeight(m_pGridCreator.GetHeight());

	//����в�
	if(m_pGridCreator.IsCalcRemainsError())
	{
		int		iSampleCount;
		double	*dSampleX;
		double	*dSampleY;
		double	*dSampleZ;

		iSampleCount	= m_pWorkspace.GetSamplePointCount();

		if(iSampleCount == 0)
		{
			MessageBox(NULL,"û�в�������,���ܽ��вв����!","��ʾ��Ϣ",MB_OK);

			return;
		}

		dSampleX	= new	double[iSampleCount];
		dSampleY	= new	double[iSampleCount];
		dSampleZ	= new	double[iSampleCount];
		
		//��ȡ��������
		m_pWorkspace.GetSamplePoint(&dSampleX,&dSampleY,&dSampleZ);
		
		//����������ݵ��ڲ�ֵ
		for(int i=0;i<iSampleCount;i++)
		{
			dSampleZ[i]	= dSampleZ[i]-m_pGridDem.GetHeight(dSampleX[i],dSampleY[i]);
		}
		
		//
		CDCR_Statistics	pStatistics;
		//
		pStatistics.SetStatisticsData(iSampleCount,dSampleZ);
		pStatistics.Statistics();
		
		//����
		szPathName	= szPathName.Left(szPathName.GetLength()-4) + "_RE.txt";

		pStatistics.Save(szPathName);

		delete	[]dSampleX;
		delete	[]dSampleY;
		delete	[]dSampleZ;
	}
}

//////////////////////////////////////////////////////////////////////
// 63 IDW��������
//////////////////////////////////////////////////////////////////////
void CInterface_DemCreator::OnCreateInIDWInBat(CDC *pDC)
{
	//���ȶ�ȡ����
	int				i;
	FILE			*fr;
	CString			szPathName;

	CDCR_Param_IDW	*m_pParaList;
	//CArray<CDCR_Param_IDW*,CDCR_Param_IDW*>	m_pParaList;
	
	//��ģ�������ڷ���ϵͳĿ¼�¡�
	szPathName	= m_szExePathName + "\\Lib\\Creator\\IDW.txt";

	fr	= fopen(szPathName,"r");
	if(fr == NULL)	return;

	int		iCount;
	fscanf(fr,"%ld",&iCount);

	m_pParaList	= new	CDCR_Param_IDW[iCount];
	for(i=0;i<iCount;i++)
	{
		m_pParaList[i].Read(fr);
	}
	
	fclose(fr);
	
	//MessageBox(NULL,"��ֵ������ȡ���!","��ʾ��Ϣ",MB_OK);

	//��Ųв���Ϣ������
	FILE	*fw;
	fw	= fopen(m_szBasePathName + "\\(IDW_RE).txt","w");
	if(fw == NULL)
	{
		if(m_pParaList != NULL)
		{
			delete	[]m_pParaList;
			m_pParaList	= NULL;
		}

		return;
	}

	//д�ñ���
	{
		fprintf(fw,"%s \t","��������");

		fprintf(fw,"%s \t","��������");

		fprintf(fw,"%s \t","Ȩָ��");

		CDCR_Statistics	pStatistics;

		pStatistics.SaveExInTile(fw);

		fprintf(fw,"%s \t","�����뾶ʹ��");

		fprintf(fw,"%s \n","����ʱ��");
	}
		
	for(i=0;i<iCount;i++)
	{
		//�����ͷ�����
		m_pGridCreator.Release();

		//���ý�ģ�ķ�Χ
		double	dLeft,dRight,dBottom,dTop;
		m_pWorkspace.GetMapExt(dLeft,dRight,dBottom,dTop);
		m_pGridCreator.SetMapExt(dLeft,dRight,dBottom,dTop);
	
		//���÷���������Ŀ
		int		iCellNumX,iCellNumY;
		m_pWorkspace.GetCellCount(iCellNumX,iCellNumY);
		m_pGridCreator.SetCellCount(iCellNumX,iCellNumY);
	
		//���÷����ĸ����ߴ�
		double	dCellSizeX,dCellSizeY;
		m_pWorkspace.GetCellSize(dCellSizeX,dCellSizeY);
		m_pGridCreator.SetCellSize(dCellSizeX,dCellSizeY);
	
		//���÷����ķ�Χ���ͽ�ģ��Χ��һ��
		double	dCellLeft,dCellRight,dCellBottom,dCellTop;
		m_pWorkspace.GetCellExt(dCellLeft,dCellRight,dCellBottom,dCellTop);
		m_pGridCreator.SetCellExt(dCellLeft,dCellRight,dCellBottom,dCellTop);

		//���÷�������
		m_pGridCreator.SetCell(m_pWorkspace.GetCell());

		//
		m_pGridCreator.SetCoordMapping(m_dMapXA,m_dMapXB,m_dMapYA,m_dMapYB);

		//����Grid DEM
		m_pGridCreator.CreateInIDW(pDC, m_pParaList[i].m_iDemNumX, m_pParaList[i].m_iDemNumY, m_szBasePathName, 
			m_pParaList[i].m_iSearchType, m_pParaList[i].m_iPointNumPreDirection, m_pParaList[i].m_iPointNumInTotal, 
			m_pParaList[i].m_dSearchRadius, m_pParaList[i].m_iNumOfU,m_pParaList[i].m_iU, 0);

		//��������ˣ�������Ҫ������߳���Ϣת�ŵ�CDCR_GridDem��
		//����DEM�ķ�Χ
		m_pGridDem.SetDemExt(dLeft,dRight,dBottom,dTop);
	
		//���ø����ĵ���
		int		iDemNumX,iDemNumY;
		m_pGridCreator.GetDemCount(iDemNumX,iDemNumY);
		m_pGridDem.SetNumOfX(iDemNumX);
		m_pGridDem.SetNumOfY(iDemNumY);

		//���ø����ĳߴ�
		double	dDemSizeX,dDemSizeY;
		m_pGridCreator.GetDemSize(dDemSizeX,dDemSizeY);
		m_pGridDem.SetSizeOfX(dDemSizeX);
		m_pGridDem.SetSizeOfY(dDemSizeY);
	
		//
		for(int k=0;k<m_pParaList[i].m_iNumOfU;k++)
		{
			//
			m_pGridDem.SetHeight(m_pGridCreator.m_pGridHeight[k].m_pHeight);
			
			//����в�
			if(m_pGridCreator.IsCalcRemainsError())
			{
				int		iSampleCount;
				double	*dSampleX;
				double	*dSampleY;
				double	*dSampleZ;

				iSampleCount	= m_pWorkspace.GetSamplePointCount();

				if(iSampleCount != 0)
				{
					dSampleX	= new	double[iSampleCount];
					dSampleY	= new	double[iSampleCount];
					dSampleZ	= new	double[iSampleCount];
					
					//��ȡ��������
					m_pWorkspace.GetSamplePoint(&dSampleX,&dSampleY,&dSampleZ);
					
					//����������ݵ��ڲ�ֵ
					for(int j=0;j<iSampleCount;j++)
					{
						dSampleZ[j]	= dSampleZ[j]-m_pGridDem.GetHeight(dSampleX[j],dSampleY[j]);
					}
					
					//
					CDCR_Statistics	pStatistics;
					//
					pStatistics.SetStatisticsData(iSampleCount,dSampleZ);
					pStatistics.Statistics();
					
					//����"������/����/U/..../�����뾶ʹ��/ʱ��"��˳����д
					int		iSearchType;
					iSearchType			= m_pParaList[i].m_iSearchType;
					fprintf(fw,"%6ld \t",iSearchType);
					
					//����
					int		iPointNumInTotal;
					iPointNumInTotal	= m_pParaList[i].m_iPointNumInTotal;
					fprintf(fw,"%6ld \t",iPointNumInTotal);
					
					int		iU;
					iU					= m_pParaList[i].m_iU[k];
					fprintf(fw,"%6ld \t",iU);
						
					pStatistics.SaveEx(fw);
					
					//
					delete	[]dSampleX;
					delete	[]dSampleY;
					delete	[]dSampleZ;
				}
			}
			
			//
			int	iSearchRadiusUsed;
			iSearchRadiusUsed	= m_pGridCreator.GetSearchRadiusUsed();
			fprintf(fw,"%6ld \t",iSearchRadiusUsed);
			
			int	iUsedTime;
			iUsedTime			= m_pGridCreator.GetUsedTime();
			fprintf(fw,"%6ld \n",iUsedTime);
		}
	}
	

	fclose(fw);

	if(m_pParaList != NULL)
	{
		delete	[]m_pParaList;
		m_pParaList	= NULL;
	}
}

//////////////////////////////////////////////////////////////////////
// 64 RBF��������
//////////////////////////////////////////////////////////////////////
void CInterface_DemCreator::OnCreateInRBFInBat(CDC *pDC)
{
	//���ȶ�ȡ����
	int				i;
	FILE			*fr;
	CString			szPathName;
	CDCR_Param_RBF	*m_pParaList;
	
	//��ģ�������ڷ���ϵͳĿ¼�¡�
	szPathName	= m_szExePathName + "\\Lib\\Creator\\RBF.txt";

	fr	= fopen(szPathName,"r");
	if(fr == NULL)	return;

	int		iCount;
	fscanf(fr,"%ld",&iCount);

	m_pParaList	= new	CDCR_Param_RBF[iCount];
	for(i=0;i<iCount;i++)
	{
		m_pParaList[i].Read(fr);
	}
	
	fclose(fr);
	
	//��Ųв���Ϣ������
	FILE	*fw;
	fw	= fopen(m_szBasePathName + "\\(RBF_RE).txt","w");
	if(fw == NULL)
	{
		if(m_pParaList != NULL)
		{
			delete	[]m_pParaList;
			m_pParaList	= NULL;
		}
		return;
	}

	//д�ñ���
	{
		fprintf(fw,"%s \t","��������");

		fprintf(fw,"%s \t","��������");

		fprintf(fw,"%s \t","Ȩָ��");

		CDCR_Statistics	pStatistics;

		pStatistics.SaveExInTile(fw);

		fprintf(fw,"%s \t","�����뾶ʹ��");

		fprintf(fw,"%s \n","����ʱ��");
	}		

	for(i=0;i<iCount;i++)
	{
		//�����ͷ�����
		m_pGridCreator.Release();

		//���ý�ģ�ķ�Χ
		double	dLeft,dRight,dBottom,dTop;
		m_pWorkspace.GetMapExt(dLeft,dRight,dBottom,dTop);
		m_pGridCreator.SetMapExt(dLeft,dRight,dBottom,dTop);
	
		//���÷���������Ŀ
		int		iCellNumX,iCellNumY;
		m_pWorkspace.GetCellCount(iCellNumX,iCellNumY);
		m_pGridCreator.SetCellCount(iCellNumX,iCellNumY);
	
		//���÷����ĸ����ߴ�
		double	dCellSizeX,dCellSizeY;
		m_pWorkspace.GetCellSize(dCellSizeX,dCellSizeY);
		m_pGridCreator.SetCellSize(dCellSizeX,dCellSizeY);
	
		//���÷����ķ�Χ���ͽ�ģ��Χ��һ��
		double	dCellLeft,dCellRight,dCellBottom,dCellTop;
		m_pWorkspace.GetCellExt(dCellLeft,dCellRight,dCellBottom,dCellTop);
		m_pGridCreator.SetCellExt(dCellLeft,dCellRight,dCellBottom,dCellTop);

		//���÷�������
		m_pGridCreator.SetCell(m_pWorkspace.GetCell());

		//
		m_pGridCreator.SetCoordMapping(m_dMapXA,m_dMapXB,m_dMapYA,m_dMapYB);

		//����Grid DEM
		m_pGridCreator.CreateInRBF(pDC, m_pParaList[i].m_iDemNumX, m_pParaList[i].m_iDemNumY, m_szBasePathName, 
			m_pParaList[i].m_iSearchType, m_pParaList[i].m_iPointNumPreDirection, m_pParaList[i].m_iPointNumInTotal, 
			m_pParaList[i].m_dSearchRadius, m_pParaList[i].m_iModelType, m_pParaList[i].m_iNumOfC,m_pParaList[i].m_dC);

		//��������ˣ�������Ҫ������߳���Ϣת�ŵ�CDCR_GridDem��
		//����DEM�ķ�Χ
		m_pGridDem.SetDemExt(dLeft,dRight,dBottom,dTop);
	
		//���ø����ĵ���
		int		iDemNumX,iDemNumY;
		m_pGridCreator.GetDemCount(iDemNumX,iDemNumY);
		m_pGridDem.SetNumOfX(iDemNumX);
		m_pGridDem.SetNumOfY(iDemNumY);

		//���ø����ĳߴ�
		double	dDemSizeX,dDemSizeY;
		m_pGridCreator.GetDemSize(dDemSizeX,dDemSizeY);
		m_pGridDem.SetSizeOfX(dDemSizeX);
		m_pGridDem.SetSizeOfY(dDemSizeY);
	
		//
		for(int k=0;k<m_pParaList[i].m_iNumOfC;k++)
		{
			//
			m_pGridDem.SetHeight(m_pGridCreator.m_pGridHeight[k].m_pHeight);
			
			//����в�
			if(m_pGridCreator.IsCalcRemainsError())
			{
				int		iSampleCount;
				double	*dSampleX;
				double	*dSampleY;
				double	*dSampleZ;

				iSampleCount	= m_pWorkspace.GetSamplePointCount();

				if(iSampleCount != 0)
				{
					dSampleX	= new	double[iSampleCount];
					dSampleY	= new	double[iSampleCount];
					dSampleZ	= new	double[iSampleCount];
					
					//��ȡ��������
					m_pWorkspace.GetSamplePoint(&dSampleX,&dSampleY,&dSampleZ);
					
					//����������ݵ��ڲ�ֵ
					for(int j=0;j<iSampleCount;j++)
					{
						dSampleZ[j]	= dSampleZ[j]-m_pGridDem.GetHeight(dSampleX[j],dSampleY[j]);
					}
					
					//
					CDCR_Statistics	pStatistics;
					//
					pStatistics.SetStatisticsData(iSampleCount,dSampleZ);
					pStatistics.Statistics();
					

					//����"������/����/U/..../�����뾶ʹ��/ʱ��"��˳����д
					int		iSearchType;
					iSearchType			= m_pParaList[i].m_iSearchType;
					fprintf(fw,"%6ld \t",iSearchType);
					
					//����
					int		iPointNumInTotal;
					iPointNumInTotal	= m_pParaList[i].m_iPointNumInTotal;
					fprintf(fw,"%6ld \t",iPointNumInTotal);
					
					double		dC;
					dC					= m_pParaList[i].m_dC[k];
					fprintf(fw,"%10.5lf \t",dC);
						
					pStatistics.SaveEx(fw);					

					//
					delete	[]dSampleX;
					delete	[]dSampleY;
					delete	[]dSampleZ;
				}
			}
			//
			int	iSearchRadiusUsed;
			iSearchRadiusUsed	= m_pGridCreator.GetSearchRadiusUsed();
			fprintf(fw,"%6ld \t",iSearchRadiusUsed);
			
			int	iUsedTime;
			iUsedTime			= m_pGridCreator.GetUsedTime();
			fprintf(fw,"%6ld \n",iUsedTime);
		}
	}

	fclose(fw);

	if(m_pParaList != NULL)
	{
		delete	[]m_pParaList;
		m_pParaList	= NULL;
	}
}

//////////////////////////////////////////////////////////////////////
// 65 KRG��������
//////////////////////////////////////////////////////////////////////
void CInterface_DemCreator::OnCreateInKRGInBat(CDC *pDC)
{
	//���ȶ�ȡ����
	int			i;
	FILE		*fr;
	CString		szPathName;
	CArray<CDCR_Param_KRG*,CDCR_Param_KRG*>	m_pParaList;
	
	//��ģ�������ڷ���ϵͳĿ¼�¡�
	szPathName	= m_szExePathName + "\\Lib\\Creator\\KRG.txt";

	fr	= fopen(szPathName,"r");
	if(fr == NULL)	return;

	int		iCount;
	fscanf(fr,"%ld",&iCount);

	for(i=0;i<iCount;i++)
	{
		CDCR_Param_KRG	*pKRG;

		pKRG	= new	CDCR_Param_KRG;

		pKRG->Read(fr);
		
		m_pParaList.Add(pKRG);
	}
	
	fclose(fr);
	
	//��Ųв���Ϣ������
	FILE	*fw;
	fw	= fopen(m_szBasePathName + "\\(KRG_RE).txt","w");
	if(fw == NULL)	return;

	for(i=0;i<iCount;i++)
	{
		CString	szGrdPathName;
		//���ɵ�GRD�ļ���Ӧ�÷��ں�DVF��ͬ��Ŀ¼�£������Ҫ�����л�ȡ
		szGrdPathName.Format("D%ld_P%ld_R%ld_M%ld.grd",m_pParaList[i]->m_iSearchType,m_pParaList[i]->m_iPointNumInTotal,(int)(m_pParaList[i]->m_dSearchRadius),m_pParaList[i]->m_iModelType);
		szGrdPathName	= m_szBasePathName + "\\" + szGrdPathName;

		//�����ͷ�����
		m_pGridCreator.Release();

		//���ý�ģ�ķ�Χ
		double	dLeft,dRight,dBottom,dTop;
		m_pWorkspace.GetMapExt(dLeft,dRight,dBottom,dTop);
		m_pGridCreator.SetMapExt(dLeft,dRight,dBottom,dTop);
	
		//���÷���������Ŀ
		int		iCellNumX,iCellNumY;
		m_pWorkspace.GetCellCount(iCellNumX,iCellNumY);
		m_pGridCreator.SetCellCount(iCellNumX,iCellNumY);
	
		//���÷����ĸ����ߴ�
		double	dCellSizeX,dCellSizeY;
		m_pWorkspace.GetCellSize(dCellSizeX,dCellSizeY);
		m_pGridCreator.SetCellSize(dCellSizeX,dCellSizeY);
	
		//���÷����ķ�Χ���ͽ�ģ��Χ��һ��
		double	dCellLeft,dCellRight,dCellBottom,dCellTop;
		m_pWorkspace.GetCellExt(dCellLeft,dCellRight,dCellBottom,dCellTop);
		m_pGridCreator.SetCellExt(dCellLeft,dCellRight,dCellBottom,dCellTop);

		//���÷�������
		m_pGridCreator.SetCell(m_pWorkspace.GetCell());

		//
		m_pGridCreator.SetCoordMapping(m_dMapXA,m_dMapXB,m_dMapYA,m_dMapYB);

		//����Grid DEM
		m_pGridCreator.CreateInKRG(
			pDC, 
			m_pParaList[i]->m_iDemNumX, 
			m_pParaList[i]->m_iDemNumY, 
			szGrdPathName, 
			m_pParaList[i]->m_iSearchType, 
			m_pParaList[i]->m_iPointNumPreDirection, 
			m_pParaList[i]->m_iPointNumInTotal, 
			m_pParaList[i]->m_dSearchRadius, 
			m_pParaList[i]->m_iModelType, 
			m_pParaList[i]->m_dC0,
			m_pParaList[i]->m_dC,
			m_pParaList[i]->m_dAlfa
			);

		//��������ˣ�������Ҫ������߳���Ϣת�ŵ�CDCR_GridDem��
		//����DEM�ķ�Χ
		m_pGridDem.SetDemExt(dLeft,dRight,dBottom,dTop);
	
		//���ø����ĵ���
		int		iDemNumX,iDemNumY;
		m_pGridCreator.GetDemCount(iDemNumX,iDemNumY);
		m_pGridDem.SetNumOfX(iDemNumX);
		m_pGridDem.SetNumOfY(iDemNumY);

		//���ø����ĳߴ�
		double	dDemSizeX,dDemSizeY;
		m_pGridCreator.GetDemSize(dDemSizeX,dDemSizeY);
		m_pGridDem.SetSizeOfX(dDemSizeX);
		m_pGridDem.SetSizeOfY(dDemSizeY);
	
		//
		m_pGridDem.SetHeight(m_pGridCreator.GetHeight());
		
		//����в�
		if(m_pGridCreator.IsCalcRemainsError())
		{
			int		iSampleCount;
			double	*dSampleX;
			double	*dSampleY;
			double	*dSampleZ;

			iSampleCount	= m_pWorkspace.GetSamplePointCount();

			if(iSampleCount != 0)
			{
				dSampleX	= new	double[iSampleCount];
				dSampleY	= new	double[iSampleCount];
				dSampleZ	= new	double[iSampleCount];
				
				//��ȡ��������
				m_pWorkspace.GetSamplePoint(&dSampleX,&dSampleY,&dSampleZ);
				
				//����������ݵ��ڲ�ֵ
				for(int j=0;j<iSampleCount;j++)
				{
					dSampleZ[j]	= dSampleZ[j]-m_pGridDem.GetHeight(dSampleX[j],dSampleY[j]);
				}
				
				//
				CDCR_Statistics	pStatistics;
				//
				pStatistics.SetStatisticsData(iSampleCount,dSampleZ);
				pStatistics.Statistics();
				
				//
				char	chName[1024];
				strcpy(chName,szGrdPathName);
				fprintf(fw,"%s\n",chName);
					
				pStatistics.Save(fw);
				
				//
				delete	[]dSampleX;
				delete	[]dSampleY;
				delete	[]dSampleZ;
			}
		}
	}

	fclose(fw);

	for(i=0;i<iCount;i++)
	{
		delete	m_pParaList[i];
	}
	m_pParaList.RemoveAll();
}

//////////////////////////////////////////////////////////////////////
// 66 SPD��������
//////////////////////////////////////////////////////////////////////
void CInterface_DemCreator::OnCreateInSPDInBat(CDC *pDC)
{
	//���ȶ�ȡ����
	int				i;
	CDCR_Param_SPD	*m_pParaList;
	
	//{
	//��ȡ��ģ������Ϣ
	//��ģ�������ڷ���ϵͳĿ¼�¡�
		FILE		*fr;
		CString		szPathName;
		szPathName	= m_szExePathName + "\\Lib\\Creator\\SPD.txt";

		fr	= fopen(szPathName,"r");
		if(fr == NULL)	return;

		int		iCount;
		fscanf(fr,"%ld",&iCount);

		m_pParaList	= new	CDCR_Param_SPD[iCount];
		for(i=0;i<iCount;i++)
		{
			m_pParaList[i].Read(fr);
		}
		
		fclose(fr);
	//}

	//
	//��Ųв���Ϣ������
	FILE	*fw;
	fw	= fopen(m_szBasePathName + "\\(SPD_RE).txt","w");
	if(fw == NULL)
	{
		if(m_pParaList != NULL)
		{
			delete	[]m_pParaList;
			m_pParaList	= NULL;
		}

		return;		
	}

	//д�ñ���
	{
		fprintf(fw,"%s \t","��������");

		fprintf(fw,"%s \t","��������");

		fprintf(fw,"%s \t","Ȩָ��");

		CDCR_Statistics	pStatistics;

		pStatistics.SaveExInTile(fw);

		fprintf(fw,"%s \t","�����뾶ʹ��");

		fprintf(fw,"%s \n","����ʱ��");
	}

	int					iSampleCount;
	iSampleCount	= m_pWorkspace.GetSamplePointCount();

	for(i=0;i<iCount;i++)
	{
		//�����ͷ�����
		m_pGridCreator.Release();

		//���ý�ģ�ķ�Χ
		double	dLeft,dRight,dBottom,dTop;
		m_pWorkspace.GetMapExt(dLeft,dRight,dBottom,dTop);
		m_pGridCreator.SetMapExt(dLeft,dRight,dBottom,dTop);
	
		//���÷���������Ŀ
		int		iCellNumX,iCellNumY;
		m_pWorkspace.GetCellCount(iCellNumX,iCellNumY);
		m_pGridCreator.SetCellCount(iCellNumX,iCellNumY);
	
		//���÷����ĸ����ߴ�
		double	dCellSizeX,dCellSizeY;
		m_pWorkspace.GetCellSize(dCellSizeX,dCellSizeY);
		m_pGridCreator.SetCellSize(dCellSizeX,dCellSizeY);
	
		//���÷����ķ�Χ���ͽ�ģ��Χ��һ��
		double	dCellLeft,dCellRight,dCellBottom,dCellTop;
		m_pWorkspace.GetCellExt(dCellLeft,dCellRight,dCellBottom,dCellTop);
		m_pGridCreator.SetCellExt(dCellLeft,dCellRight,dCellBottom,dCellTop);

		//���÷�������
		m_pGridCreator.SetCell(m_pWorkspace.GetCell());

		//
		m_pGridCreator.SetCoordMapping(m_dMapXA,m_dMapXB,m_dMapYA,m_dMapYB);

		//����ȱʡ�ĵ�������dAdjustR
		double	dAdjustR;
		if(m_pParaList[i].m_iRadiusType == 0)
		{
			dAdjustR	= sqrt( (dRight-dLeft)*(dTop-dBottom)/iSampleCount * m_pParaList[i].m_iPointNumInTotal / 3.1415926 );
		}
		else
		{
			dAdjustR	= m_pParaList[i].m_dMaxRadius;
		}

		//����Grid DEM
		m_pGridCreator.CreateInSPD
			(
			pDC,										//pDC
			m_pParaList[i].m_iDemNumX,					//�����������	
			m_pParaList[i].m_iDemNumY,					//�����������
			m_szBasePathName,							//��׼·��
			m_pParaList[i].m_iSearchType,				//��������
			m_pParaList[i].m_iPointNumPreDirection,		//ÿ����������ĵ���
			m_pParaList[i].m_iPointNumInTotal,			//���������ܵ���
			m_pParaList[i].m_dSearchRadius,				//�����뾶
			m_pParaList[i].m_iNumOfU,					//Ȩָ��U����
			m_pParaList[i].m_iU,						//Ȩָ��U
			m_pParaList[i].m_dT,						//ƽ������T
			m_pParaList[i].m_iRadiusType,				//�����������ͣ���ȱʡ������ָ��
			dAdjustR,									//��������
			m_pParaList[i].m_iKernalType				//��ֵ�˺���
			);

		//��������ˣ�������Ҫ������߳���Ϣת�ŵ�CDCR_GridDem��
		//����DEM�ķ�Χ
		m_pGridDem.SetDemExt(dLeft,dRight,dBottom,dTop);
	
		//���ø����ĵ���
		int		iDemNumX,iDemNumY;
		m_pGridCreator.GetDemCount(iDemNumX,iDemNumY);
		m_pGridDem.SetNumOfX(iDemNumX);
		m_pGridDem.SetNumOfY(iDemNumY);

		//���ø����ĳߴ�
		double	dDemSizeX,dDemSizeY;
		m_pGridCreator.GetDemSize(dDemSizeX,dDemSizeY);
		m_pGridDem.SetSizeOfX(dDemSizeX);
		m_pGridDem.SetSizeOfY(dDemSizeY);
	
		//
		for(int k=0;k<m_pParaList[i].m_iNumOfU;k++)
		{
			//
			m_pGridDem.SetHeight(m_pGridCreator.m_pGridHeight[k].m_pHeight);
			
			//����в�
			if(m_pGridCreator.IsCalcRemainsError())
			{
				int		iSampleCount;
				double	*dSampleX;
				double	*dSampleY;
				double	*dSampleZ;

				iSampleCount	= m_pWorkspace.GetSamplePointCount();

				if(iSampleCount != 0)
				{
					dSampleX	= new	double[iSampleCount];
					dSampleY	= new	double[iSampleCount];
					dSampleZ	= new	double[iSampleCount];
					
					//��ȡ��������
					m_pWorkspace.GetSamplePoint(&dSampleX,&dSampleY,&dSampleZ);
					
					//����������ݵ��ڲ�ֵ
					for(int j=0;j<iSampleCount;j++)
					{
						dSampleZ[j]	= dSampleZ[j]-m_pGridDem.GetHeight(dSampleX[j],dSampleY[j]);
					}
					
					//
					CDCR_Statistics	pStatistics;
					//
					pStatistics.SetStatisticsData(iSampleCount,dSampleZ);
					pStatistics.Statistics();
					
					//����"������/����/U/..../�����뾶ʹ��/ʱ��"��˳����д
					int		iSearchType;
					iSearchType			= m_pParaList[i].m_iSearchType;
					fprintf(fw,"%6ld \t",iSearchType);
					
					//����
					int		iPointNumInTotal;
					iPointNumInTotal	= m_pParaList[i].m_iPointNumInTotal;
					fprintf(fw,"%6ld \t",iPointNumInTotal);
					
					int		iU;
					iU					= m_pParaList[i].m_iU[k];
					fprintf(fw,"%6ld \t",iU);
						
					pStatistics.SaveEx(fw);
				
					//
					delete	[]dSampleX;
					delete	[]dSampleY;
					delete	[]dSampleZ;
				}
			}
			
			//
			//
			int	iSearchRadiusUsed;
			iSearchRadiusUsed	= m_pGridCreator.GetSearchRadiusUsed();
			fprintf(fw,"%6ld \t",iSearchRadiusUsed);
			
			int	iUsedTime;
			iUsedTime			= m_pGridCreator.GetUsedTime();
			fprintf(fw,"%6ld \n",iUsedTime);
		}
	}

	fclose(fw);

	if(m_pParaList != NULL)
	{
		delete	[]m_pParaList;
		m_pParaList	= NULL;
	}
}

//////////////////////////////////////////////////////////////////////
// 67 IDW������֤
//////////////////////////////////////////////////////////////////////
void CInterface_DemCreator::OnCrossValidateInIDWInBat(CDC *pDC)
{
	int				iParamCount;
	CDCR_Param_IDW	*m_pParaList;

	iParamCount	= 0;
	m_pParaList	= NULL;

	//���ȶ�ȡ����
	{
		int				i;
		FILE			*fr;
		CString			szPathName;

		
		//��ģ�������ڷ���ϵͳĿ¼�¡�
		szPathName	= m_szExePathName + "\\Lib\\Creator\\IDW.txt";

		fr	= fopen(szPathName,"r");
		if(fr != NULL)
		{
			fscanf(fr,"%ld",&iParamCount);

			m_pParaList	= new	CDCR_Param_IDW[iParamCount];
			for(i=0;i<iParamCount;i++)
			{
				m_pParaList[i].Read(fr);
			}
			
			fclose(fr);
		}
	}
	
	//MessageBox(NULL,"��ֵ������ȡ���!","��ʾ��Ϣ",MB_OK);

	//��Ųв���Ϣ������
	FILE	*fw;
	fw	= fopen(m_szBasePathName + "\\(IDWCV_RE).txt","w");
	if(fw != NULL)
	{
		int		i;

		//д�ñ���
		{
			fprintf(fw,"%s \t","��������");

			fprintf(fw,"%s \t","��������");

			fprintf(fw,"%s \t","Ȩָ��");

			CDCR_Statistics	pStatistics;

			pStatistics.SaveExInTile(fw);

			fprintf(fw,"%s \t","�����뾶ʹ��");

			fprintf(fw,"%s \n","����ʱ��");
		}
		
		//����������
		int					iSampleCount;
		iSampleCount	= m_pWorkspace.GetSamplePointCount();
		CDCR_PointFeature	*pSamplePointList;
		pSamplePointList= m_pWorkspace.GetPointFeature();
		
		for(i=0;i<iParamCount;i++)
		{
			//�����ͷ�����
			m_pGridCreator.Release();

			//���ý�ģ�ķ�Χ
			double	dLeft,dRight,dBottom,dTop;
			m_pWorkspace.GetMapExt(dLeft,dRight,dBottom,dTop);
			m_pGridCreator.SetMapExt(dLeft,dRight,dBottom,dTop);
		
			//���÷���������Ŀ
			int		iCellNumX,iCellNumY;
			m_pWorkspace.GetCellCount(iCellNumX,iCellNumY);
			m_pGridCreator.SetCellCount(iCellNumX,iCellNumY);
		
			//���÷����ĸ����ߴ�
			double	dCellSizeX,dCellSizeY;
			m_pWorkspace.GetCellSize(dCellSizeX,dCellSizeY);
			m_pGridCreator.SetCellSize(dCellSizeX,dCellSizeY);
		
			//���÷����ķ�Χ���ͽ�ģ��Χ��һ��
			double	dCellLeft,dCellRight,dCellBottom,dCellTop;
			m_pWorkspace.GetCellExt(dCellLeft,dCellRight,dCellBottom,dCellTop);
			m_pGridCreator.SetCellExt(dCellLeft,dCellRight,dCellBottom,dCellTop);

			//���÷�������
			m_pGridCreator.SetCell(m_pWorkspace.GetCell());
			
			//
			m_pGridCreator.SetCoordMapping(m_dMapXA,m_dMapXB,m_dMapYA,m_dMapYB);
			
			//������֤
			m_pGridCreator.CrossValidateInIDW(	pDC, 
												iSampleCount,
												pSamplePointList, 
												m_szBasePathName, 
												m_pParaList[i].m_iSearchType, 
												m_pParaList[i].m_iPointNumPreDirection, 
												m_pParaList[i].m_iPointNumInTotal, 
												m_pParaList[i].m_dSearchRadius, 
												m_pParaList[i].m_iNumOfU,
												m_pParaList[i].m_iU, 
												0
												);

			//��������ˣ�������Ҫ������߳���Ϣת�ŵ�CDCR_GridDem��
			//����DEM�ķ�Χ
			m_pGridDem.SetDemExt(dLeft,dRight,dBottom,dTop);
		
			//���ø����ĵ���
			int		iDemNumX,iDemNumY;
			m_pGridCreator.GetDemCount(iDemNumX,iDemNumY);
			m_pGridDem.SetNumOfX(iDemNumX);
			m_pGridDem.SetNumOfY(iDemNumY);

			//���ø����ĳߴ�
			double	dDemSizeX,dDemSizeY;
			m_pGridCreator.GetDemSize(dDemSizeX,dDemSizeY);
			m_pGridDem.SetSizeOfX(dDemSizeX);
			m_pGridDem.SetSizeOfY(dDemSizeY);
		
			//
			for(int k=0;k<m_pParaList[i].m_iNumOfU;k++)
			{
				//����Ҫ����в��
				double	*dSampleZ;

				dSampleZ	= m_pGridCreator.m_pGridHeight[k].m_pHeight;

				//
				CDCR_Statistics	pStatistics;
				//
				pStatistics.SetStatisticsData(iSampleCount,dSampleZ);
				pStatistics.Statistics();
				

				//����"������/����/U/..../�����뾶ʹ��/ʱ��"��˳����д
				int		iSearchType;
				iSearchType			= m_pParaList[i].m_iSearchType;
				fprintf(fw,"%6ld \t",iSearchType);
				
				//����
				int		iPointNumInTotal;
				iPointNumInTotal	= m_pParaList[i].m_iPointNumInTotal;
				fprintf(fw,"%6ld \t",iPointNumInTotal);
				
				int		iU;
				iU					= m_pParaList[i].m_iU[k];
				fprintf(fw,"%6ld \t",iU);
					
				pStatistics.SaveEx(fw);

				//
				int	iSearchRadiusUsed;
				iSearchRadiusUsed	= m_pGridCreator.GetSearchRadiusUsed();
				fprintf(fw,"%6ld \t",iSearchRadiusUsed);
				
				int	iUsedTime;
				iUsedTime			= m_pGridCreator.GetUsedTime();
				fprintf(fw,"%6ld \n",iUsedTime);
			}
		}
		
		fclose(fw);

		if(m_pParaList != NULL)
		{
			delete	[]m_pParaList;
			m_pParaList	= NULL;
		}
	}
}

//////////////////////////////////////////////////////////////////////
//68 RBF������֤
//////////////////////////////////////////////////////////////////////
void CInterface_DemCreator::OnCrossValidateInRBFInBat(CDC *pDC)
{
	int				iParamCount;
	CDCR_Param_RBF	*m_pParaList;

	iParamCount	= 0;
	m_pParaList	= NULL;

	//���ȶ�ȡ����
	{
		int				i;
		FILE			*fr;
		CString			szPathName;
		
		//��ģ�������ڷ���ϵͳĿ¼�¡�
		szPathName	= m_szExePathName + "\\Lib\\Creator\\RBF.txt";

		fr	= fopen(szPathName,"r");
		if(fr != NULL)
		{
			fscanf(fr,"%ld",&iParamCount);

			m_pParaList	= new	CDCR_Param_RBF[iParamCount];
			for(i=0;i<iParamCount;i++)
			{
				m_pParaList[i].Read(fr);
			}
			
			fclose(fr);
		}
	}
	
	//��Ųв���Ϣ������
	FILE	*fw;
	fw	= fopen(m_szBasePathName + "\\(RBFCV_RE).txt","w");
	if(fw != NULL)
	{
		int		i;
		//д�ñ���
		{
			fprintf(fw,"%s \t","��������");

			fprintf(fw,"%s \t","��������");

			fprintf(fw,"%s \t","�⻬����");

			CDCR_Statistics	pStatistics;

			pStatistics.SaveExInTile(fw);

			fprintf(fw,"%s \t","�����뾶ʹ��");

			fprintf(fw,"%s \n","����ʱ��");
		}	

		//����������
		int					iSampleCount;
		iSampleCount	= m_pWorkspace.GetSamplePointCount();
		CDCR_PointFeature	*pSamplePointList;
		pSamplePointList= m_pWorkspace.GetPointFeature();

		for(i=0;i<iParamCount;i++)
		{
			//�����ͷ�����
			m_pGridCreator.Release();

			//���ý�ģ�ķ�Χ
			double	dLeft,dRight,dBottom,dTop;
			m_pWorkspace.GetMapExt(dLeft,dRight,dBottom,dTop);
			m_pGridCreator.SetMapExt(dLeft,dRight,dBottom,dTop);
		
			//���÷���������Ŀ
			int		iCellNumX,iCellNumY;
			m_pWorkspace.GetCellCount(iCellNumX,iCellNumY);
			m_pGridCreator.SetCellCount(iCellNumX,iCellNumY);
		
			//���÷����ĸ����ߴ�
			double	dCellSizeX,dCellSizeY;
			m_pWorkspace.GetCellSize(dCellSizeX,dCellSizeY);
			m_pGridCreator.SetCellSize(dCellSizeX,dCellSizeY);
		
			//���÷����ķ�Χ���ͽ�ģ��Χ��һ��
			double	dCellLeft,dCellRight,dCellBottom,dCellTop;
			m_pWorkspace.GetCellExt(dCellLeft,dCellRight,dCellBottom,dCellTop);
			m_pGridCreator.SetCellExt(dCellLeft,dCellRight,dCellBottom,dCellTop);

			//���÷�������
			m_pGridCreator.SetCell(m_pWorkspace.GetCell());

			//
			m_pGridCreator.SetCoordMapping(m_dMapXA,m_dMapXB,m_dMapYA,m_dMapYB);

			//������֤
			m_pGridCreator.CrossValidateInRBF(
				pDC,
				iSampleCount,
				pSamplePointList,
				m_szBasePathName,
				m_pParaList[i].m_iSearchType, 
				m_pParaList[i].m_iPointNumPreDirection,
				m_pParaList[i].m_iPointNumInTotal, 
				m_pParaList[i].m_dSearchRadius,
				m_pParaList[i].m_iModelType, 
				m_pParaList[i].m_iNumOfC,
				m_pParaList[i].m_dC
				);
			
			//��������ˣ�������Ҫ������߳���Ϣת�ŵ�CDCR_GridDem��
			//����DEM�ķ�Χ
			m_pGridDem.SetDemExt(dLeft,dRight,dBottom,dTop);
		
			//���ø����ĵ���
			int		iDemNumX,iDemNumY;
			m_pGridCreator.GetDemCount(iDemNumX,iDemNumY);
			m_pGridDem.SetNumOfX(iDemNumX);
			m_pGridDem.SetNumOfY(iDemNumY);

			//���ø����ĳߴ�
			double	dDemSizeX,dDemSizeY;
			m_pGridCreator.GetDemSize(dDemSizeX,dDemSizeY);
			m_pGridDem.SetSizeOfX(dDemSizeX);
			m_pGridDem.SetSizeOfY(dDemSizeY);
		
			//
			for(int k=0;k<m_pParaList[i].m_iNumOfC;k++)
			{
				//����Ҫ����в��
				double	*dSampleZ;

				dSampleZ	= m_pGridCreator.m_pGridHeight[k].m_pHeight;
								
				//
				CDCR_Statistics	pStatistics;
				//
				pStatistics.SetStatisticsData(iSampleCount,dSampleZ);
				pStatistics.Statistics();
				

				//����"������/����/U/..../�����뾶ʹ��/ʱ��"��˳����д
				int		iSearchType;
				iSearchType			= m_pParaList[i].m_iSearchType;
				fprintf(fw,"%6ld \t",iSearchType);
				
				//����
				int		iPointNumInTotal;
				iPointNumInTotal	= m_pParaList[i].m_iPointNumInTotal;
				fprintf(fw,"%6ld \t",iPointNumInTotal);
				
				double		dC;
				dC					= m_pParaList[i].m_dC[k];
				fprintf(fw,"%10.5lf \t",dC);
					
				pStatistics.SaveEx(fw);

				//
				int	iSearchRadiusUsed;
				iSearchRadiusUsed	= m_pGridCreator.GetSearchRadiusUsed();
				fprintf(fw,"%6ld \t",iSearchRadiusUsed);
				
				int	iUsedTime;
				iUsedTime			= m_pGridCreator.GetUsedTime();
				fprintf(fw,"%6ld \n",iUsedTime);
			}
		}

		fclose(fw);

		if(m_pParaList != NULL)
		{
			delete	[]m_pParaList;
			m_pParaList	= NULL;
		}
	}
}

//////////////////////////////////////////////////////////////////////
//69 KRG������֤
//////////////////////////////////////////////////////////////////////
void CInterface_DemCreator::OnCrossValidateInKRGInBat(CDC *pDC)
{
	int				iParamCount;
	CDCR_Param_KRG	*m_pParaList;

	iParamCount	= 0;
	m_pParaList	= NULL;

	//���ȶ�ȡ����
	{
		int				i;
		FILE			*fr;
		CString			szPathName;
		
		//��ģ�������ڷ���ϵͳĿ¼�¡�
		szPathName	= m_szExePathName + "\\Lib\\Creator\\KRG.txt";

		fr	= fopen(szPathName,"r");
		if(fr != NULL)
		{
			fscanf(fr,"%ld",&iParamCount);

			m_pParaList	= new	CDCR_Param_KRG[iParamCount];
			for(i=0;i<iParamCount;i++)
			{
				m_pParaList[i].Read(fr);
			}
			
			fclose(fr);
		}
	}
	
	//��Ųв���Ϣ������
	FILE	*fw;
	fw	= fopen(m_szBasePathName + "\\(KRGCV_RE).txt","w");
	if(fw != NULL)
	{
		int		i;
		//д�ñ���
		{
			fprintf(fw,"%s \t","��������");

			fprintf(fw,"%s \t","��������");

			fprintf(fw,"%s \t","ģ������");

			fprintf(fw,"%s \t","���ֵ");

			fprintf(fw,"%s \t","��ֵ̨");
			
			fprintf(fw,"%s \t","���");

			CDCR_Statistics	pStatistics;

			pStatistics.SaveExInTile(fw);

			fprintf(fw,"%s \t","�����뾶ʹ��");

			fprintf(fw,"%s \n","����ʱ��");
		}	

		//����������
		int					iSampleCount;
		iSampleCount	= m_pWorkspace.GetSamplePointCount();
		CDCR_PointFeature	*pSamplePointList;
		pSamplePointList= m_pWorkspace.GetPointFeature();

		for(i=0;i<iParamCount;i++)
		{
			//�����ͷ�����
			m_pGridCreator.Release();

			//���ý�ģ�ķ�Χ
			double	dLeft,dRight,dBottom,dTop;
			m_pWorkspace.GetMapExt(dLeft,dRight,dBottom,dTop);
			m_pGridCreator.SetMapExt(dLeft,dRight,dBottom,dTop);
		
			//���÷���������Ŀ
			int		iCellNumX,iCellNumY;
			m_pWorkspace.GetCellCount(iCellNumX,iCellNumY);
			m_pGridCreator.SetCellCount(iCellNumX,iCellNumY);
		
			//���÷����ĸ����ߴ�
			double	dCellSizeX,dCellSizeY;
			m_pWorkspace.GetCellSize(dCellSizeX,dCellSizeY);
			m_pGridCreator.SetCellSize(dCellSizeX,dCellSizeY);
		
			//���÷����ķ�Χ���ͽ�ģ��Χ��һ��
			double	dCellLeft,dCellRight,dCellBottom,dCellTop;
			m_pWorkspace.GetCellExt(dCellLeft,dCellRight,dCellBottom,dCellTop);
			m_pGridCreator.SetCellExt(dCellLeft,dCellRight,dCellBottom,dCellTop);

			//���÷�������
			m_pGridCreator.SetCell(m_pWorkspace.GetCell());

			//
			m_pGridCreator.SetCoordMapping(m_dMapXA,m_dMapXB,m_dMapYA,m_dMapYB);

			//������֤
			m_pGridCreator.CrossValidateInKRG(
				pDC,
				iSampleCount,
				pSamplePointList,
				m_szBasePathName,
				m_pParaList[i].m_iSearchType, 
				m_pParaList[i].m_iPointNumPreDirection,
				m_pParaList[i].m_iPointNumInTotal, 
				m_pParaList[i].m_dSearchRadius,
				m_pParaList[i].m_iModelType, 
				m_pParaList[i].m_dC0,
				m_pParaList[i].m_dC,
				m_pParaList[i].m_dAlfa
				);
			
			//��������ˣ�������Ҫ������߳���Ϣת�ŵ�CDCR_GridDem��
			//����DEM�ķ�Χ
			m_pGridDem.SetDemExt(dLeft,dRight,dBottom,dTop);
		
			//���ø����ĵ���
			int		iDemNumX,iDemNumY;
			m_pGridCreator.GetDemCount(iDemNumX,iDemNumY);
			m_pGridDem.SetNumOfX(iDemNumX);
			m_pGridDem.SetNumOfY(iDemNumY);

			//���ø����ĳߴ�
			double	dDemSizeX,dDemSizeY;
			m_pGridCreator.GetDemSize(dDemSizeX,dDemSizeY);
			m_pGridDem.SetSizeOfX(dDemSizeX);
			m_pGridDem.SetSizeOfY(dDemSizeY);
		
			//
			//for(int k=0;k<m_pParaList[i].m_iNumOfC;k++)
			{
				//����Ҫ����в��
				double	*dSampleZ;

				dSampleZ	= m_pGridCreator.m_pGridHeight[0].m_pHeight;
								
				//
				CDCR_Statistics	pStatistics;
				//
				pStatistics.SetStatisticsData(iSampleCount,dSampleZ);
				pStatistics.Statistics();
				

				//����"������/����/ģ������/���ֵ/��ֵ̨/���/..../�����뾶ʹ��/ʱ��"��˳����д
				int		iSearchType;
				iSearchType			= m_pParaList[i].m_iSearchType;
				fprintf(fw,"%6ld \t",iSearchType);
				
				//����
				int		iPointNumInTotal;
				iPointNumInTotal	= m_pParaList[i].m_iPointNumInTotal;
				fprintf(fw,"%6ld \t",iPointNumInTotal);
				
				int		iModelType;
				iModelType			= m_pParaList[i].m_iModelType;
				fprintf(fw,"%6ld \t",iModelType);

				double	dC0;
				dC0					= m_pParaList[i].m_dC0;
				fprintf(fw,"%lf \t",dC0);

				double	dC;
				dC					= m_pParaList[i].m_dC;
				fprintf(fw,"%lf \t",dC);

				double	dAlfa;
				dAlfa				= m_pParaList[i].m_dAlfa;
				fprintf(fw,"%lf \t",dAlfa);
					
				pStatistics.SaveEx(fw);

				//
				int	iSearchRadiusUsed;
				iSearchRadiusUsed	= m_pGridCreator.GetSearchRadiusUsed();
				fprintf(fw,"%6ld \t",iSearchRadiusUsed);
				
				int	iUsedTime;
				iUsedTime			= m_pGridCreator.GetUsedTime();
				fprintf(fw,"%6ld \n",iUsedTime);
			}
		}

		fclose(fw);

		if(m_pParaList != NULL)
		{
			delete	[]m_pParaList;
			m_pParaList	= NULL;
		}
	}
}

//////////////////////////////////////////////////////////////////////
// 70 SPD������֤
//////////////////////////////////////////////////////////////////////
void CInterface_DemCreator::OnCrossValidateInSPDInBat(CDC *pDC)
{
	int				iParamCount;
	CDCR_Param_SPD	*m_pParaList;

	iParamCount	= 0;
	m_pParaList	= NULL;

	//���ȶ�ȡ����
	{
		int				i;
		FILE			*fr;
		CString			szPathName;
		
		//��ģ�������ڷ���ϵͳĿ¼�¡�
		szPathName	= m_szExePathName + "\\Lib\\Creator\\SPD.txt";

		fr	= fopen(szPathName,"r");
		if(fr != NULL)
		{
			fscanf(fr,"%ld",&iParamCount);

			m_pParaList	= new	CDCR_Param_SPD[iParamCount];
			for(i=0;i<iParamCount;i++)
			{
				m_pParaList[i].Read(fr);
			}
			
			fclose(fr);		
		}
	}
	
	//��Ųв���Ϣ������
	FILE	*fw;
	fw	= fopen(m_szBasePathName + "\\(SPDCV_RE).txt","w");
	if(fw != NULL)
	{
		int		i;
		//д�ñ���
		{
			fprintf(fw,"%s \t","��������");

			fprintf(fw,"%s \t","��������");

			fprintf(fw,"%s \t","Ȩָ��");

			CDCR_Statistics	pStatistics;

			pStatistics.SaveExInTile(fw);

			fprintf(fw,"%s \t","�����뾶ʹ��");

			fprintf(fw,"%s \n","����ʱ��");
		}

		//����������
		int					iSampleCount;
		iSampleCount	= m_pWorkspace.GetSamplePointCount();
		CDCR_PointFeature	*pSamplePointList;
		pSamplePointList= m_pWorkspace.GetPointFeature();

		for(i=0;i<iParamCount;i++)
		{
			//��һ���������ͷ�����
			m_pGridCreator.Release();

			//�ڶ��������ý�ģ�ķ�Χ
			double	dLeft,dRight,dBottom,dTop;
			m_pWorkspace.GetMapExt(dLeft,dRight,dBottom,dTop);
			m_pGridCreator.SetMapExt(dLeft,dRight,dBottom,dTop);
		
			//�����������÷���������Ŀ
			int		iCellNumX,iCellNumY;
			m_pWorkspace.GetCellCount(iCellNumX,iCellNumY);
			m_pGridCreator.SetCellCount(iCellNumX,iCellNumY);
		
			//���Ĳ������÷����ĸ����ߴ�
			double	dCellSizeX,dCellSizeY;
			m_pWorkspace.GetCellSize(dCellSizeX,dCellSizeY);
			m_pGridCreator.SetCellSize(dCellSizeX,dCellSizeY);
		
			//���岽�����÷����ķ�Χ���ͽ�ģ��Χ��һ��
			double	dCellLeft,dCellRight,dCellBottom,dCellTop;
			m_pWorkspace.GetCellExt(dCellLeft,dCellRight,dCellBottom,dCellTop);
			m_pGridCreator.SetCellExt(dCellLeft,dCellRight,dCellBottom,dCellTop);

			//�����������÷���������Ϣ
			m_pGridCreator.SetCell(m_pWorkspace.GetCell());

			//���߲���������ʾ����
			m_pGridCreator.SetCoordMapping(m_dMapXA,m_dMapXB,m_dMapYA,m_dMapYB);

			//����ȱʡ�ĵ�������dAdjustR
			double	dAdjustR;
			if(m_pParaList[i].m_iRadiusType == 0)
			{
				dAdjustR	= sqrt( (dRight-dLeft)*(dTop-dBottom)/iSampleCount * m_pParaList[i].m_iPointNumInTotal / 3.1415926 );
			}
			else
			{
				dAdjustR	= m_pParaList[i].m_dMaxRadius;
			}

			//����Grid DEM
			m_pGridCreator.CrossValidateInSPD
				(
				pDC,										//pDC
				iSampleCount,								//��������Ŀ
				pSamplePointList,							//����������
				m_szBasePathName,							//��׼·��
				m_pParaList[i].m_iSearchType,				//��������
				m_pParaList[i].m_iPointNumPreDirection,		//ÿ���������������
				m_pParaList[i].m_iPointNumInTotal,			//�������ܵ���
				m_pParaList[i].m_dSearchRadius,				//�����뾶
				m_pParaList[i].m_iNumOfU,					//Ȩָ��U��Ŀ
				m_pParaList[i].m_iU,						//Ȩָ��U
				m_pParaList[i].m_dT,						//ƽ������T
				m_pParaList[i].m_iRadiusType,				//�����������ͣ���ȱʡ������ָ��
				dAdjustR,									//��������
				m_pParaList[i].m_iKernalType				//��ֵ�˺���
				);

			//��������ˣ�������Ҫ������߳���Ϣת�ŵ�CDCR_GridDem��
			//����DEM�ķ�Χ
			m_pGridDem.SetDemExt(dLeft,dRight,dBottom,dTop);
		
			//���ø����ĵ���
			int		iDemNumX,iDemNumY;
			m_pGridCreator.GetDemCount(iDemNumX,iDemNumY);
			m_pGridDem.SetNumOfX(iDemNumX);
			m_pGridDem.SetNumOfY(iDemNumY);

			//���ø����ĳߴ�
			double	dDemSizeX,dDemSizeY;
			m_pGridCreator.GetDemSize(dDemSizeX,dDemSizeY);
			m_pGridDem.SetSizeOfX(dDemSizeX);
			m_pGridDem.SetSizeOfY(dDemSizeY);
		
			//
			for(int k=0;k<m_pParaList[i].m_iNumOfU;k++)
			{
				//����Ҫ����в��
				double	*dSampleZ;

				dSampleZ	= m_pGridCreator.m_pGridHeight[k].m_pHeight;

				//
				CDCR_Statistics	pStatistics;
				//
				pStatistics.SetStatisticsData(iSampleCount,dSampleZ);
				pStatistics.Statistics();
				

				//����"������/����/U/..../�����뾶ʹ��/ʱ��"��˳����д
				int		iSearchType;
				iSearchType			= m_pParaList[i].m_iSearchType;
				fprintf(fw,"%6ld \t",iSearchType);
				
				//����
				int		iPointNumInTotal;
				iPointNumInTotal	= m_pParaList[i].m_iPointNumInTotal;
				fprintf(fw,"%6ld \t",iPointNumInTotal);
				
				int		iU;
				iU					= m_pParaList[i].m_iU[k];
				fprintf(fw,"%6ld \t",iU);
					
				pStatistics.SaveEx(fw);

				//
				int	iSearchRadiusUsed;
				iSearchRadiusUsed	= m_pGridCreator.GetSearchRadiusUsed();
				fprintf(fw,"%6ld \t",iSearchRadiusUsed);
				
				int	iUsedTime;
				iUsedTime			= m_pGridCreator.GetUsedTime();
				fprintf(fw,"%6ld \n",iUsedTime);
			}
		}

		fclose(fw);

		if(m_pParaList != NULL)
		{
			delete	[]m_pParaList;
			m_pParaList	= NULL;
		}	
	}
}

//////////////////////////////////////////////////////////////////////
//���㽻����֤����µľֲ���������
//////////////////////////////////////////////////////////////////////
void CInterface_DemCreator::OnCalculateLocalTerrainCharacterInBat()
{
	int				iParamCount;
	CDCR_Param_LTC	*pParaList;

	iParamCount	= 0;
	pParaList	= NULL;

	// ��һ�� : ���ȶ�ȡ����
	{
		int				i;
		FILE			*fr;
		CString			szPathName;
		
		//��ģ�������ڷ���ϵͳĿ¼�¡�
		szPathName	= m_szExePathName + "\\Lib\\Creator\\LocalTerrainCharacter.txt";

		fr	= fopen(szPathName,"r");
		if(fr != NULL)
		{
			fscanf(fr,"%ld",&iParamCount);

			pParaList	= new	CDCR_Param_LTC[iParamCount];
			for(i=0;i<iParamCount;i++)
			{
				pParaList[i].Read(fr);
			}
			
			fclose(fr);
		}
	}
	
	//�ڶ��� : ��ȡ���в������� 
	int					iSampleCount;
	iSampleCount	= m_pWorkspace.GetSamplePointCount();
	CDCR_PointFeature	*pSamplePointList;
	pSamplePointList= m_pWorkspace.GetPointFeature();
	
	//����������������Ϣ���ݸ�CDCR_LocalTerrainCreator
	CDCR_LocalTerrainCreator	m_pLTCreator;
	{
		//1�����÷���������Ŀ
		int		iCellNumX,iCellNumY;
		m_pWorkspace.GetCellCount(iCellNumX,iCellNumY);

		//2�����÷����ĸ����ߴ�
		double	dCellSizeX,dCellSizeY;
		m_pWorkspace.GetCellSize(dCellSizeX,dCellSizeY);

		//3�����÷����ķ�Χ���ͽ�ģ��Χ��һ��
		double	dCellLeft,dCellRight,dCellBottom,dCellTop;
		m_pWorkspace.GetCellExt(dCellLeft,dCellRight,dCellBottom,dCellTop);

		//4�����÷���������Ϣ
		CDCR_Cells	*pCells;
		pCells	= m_pWorkspace.GetCell();

		//5����������Ϣ���ݵ�CDCR_LocalTerrainCreator��
		m_pLTCreator.SetCell(pCells,iCellNumX,iCellNumY,dCellSizeX,dCellSizeY,dCellLeft,dCellRight,dCellBottom,dCellTop);
	}

	//������ : ����ֲ�����������Ϣ
	{	
		//ÿһ������µľֲ���������
		for(int i=0;i<iParamCount;i++)
		{
			//����һ��ͼ�����е����Ϣ
			m_pLTCreator.CalculateLocalTerrainCharacter(iSampleCount,
														pSamplePointList,
														m_szBasePathName,
														pParaList[i].m_iSearchType,
														pParaList[i].m_iPointNumPreDirection,
														pParaList[i].m_iPointNumInTotal,
														pParaList[i].m_dSearchRadius
														);
		}
	}

}

//////////////////////////////////////////////////////////////////////
//71 ��ֵ��������ָ����xy���꣬������Ӧ��Zֵ��������ָ�̣߳���������Ӧ��ͳ����Ϣ
//////////////////////////////////////////////////////////////////////
void CInterface_DemCreator::OnInterpolationValue()
{
	if(m_pWorkspace.IsEmpty())	
	{
		return;
	}

	if(m_pGridDem.IsEmpty())
	{
		return;
	}

	CDCR_PointFeature	*pPoint;
	int					iPointCount;
	double				*dValue;
	int					i;

	pPoint		= m_pWorkspace.GetPointFeature();
	iPointCount	= m_pWorkspace.GetSamplePointCount();

	dValue		= new	double[iPointCount];
		
	i			= 0;
	while(pPoint != NULL)
	{
		if(false == pPoint->m_bUsed)
		{
			dValue[i]	= m_pGridDem.GetHeight(pPoint->m_pPointCoord.x,pPoint->m_pPointCoord.y);

			i++;
		}

		pPoint	= pPoint->m_pNext;
	}
	
	//
	CString			szPathName;

	szPathName		= m_szBasePathName + "\\ValueStatistics.txt";
	
	FILE			*fp;
	fp				= fopen(szPathName,"w");

	if(fp != NULL)
	{
		//
		CDCR_Statistics	pStatistics;
		//
		pStatistics.SetStatisticsData(iPointCount,dValue);
		//
		pStatistics.Statistics();

		//
		pStatistics.SaveExInTile(fp);
		//
		pStatistics.SaveEx(fp);

		fclose(fp);
	}

	delete	[]dValue;
}

//////////////////////////////////////////////////////////////////////
// 72 ת��EGC����ΪDEM����
//////////////////////////////////////////////////////////////////////
bool CInterface_DemCreator::OnConversionEGCToGRD(CString szPathName)
{
	//�����ļ���¼Ԫ������Ϣ
	CString	szOGYPathName;
	szOGYPathName	= szPathName.Left(szPathName.GetLength()-3) + "ogy";

	double	dLeft,dRight,dBottom,dTop;

	//���ȶ�ȡ������Ϣ
	FILE	*fp;

	fp	= fopen(szOGYPathName,"r");
	if(fp != NULL)
	{
		//�ļ���
		char	chFileName[64];
		fscanf(fp,"%s",chFileName);

		//ͼ��
		char	chMapNumber[64];
		fscanf(fp,"%s",chMapNumber);

		//ͼ��
		char	chMapName[64];
		fscanf(fp,"%s",chMapName);

		//���½ǵ�����
		fscanf(fp,"%lf%lf",&dLeft,&dBottom);
		
		//ԭͼ��������
		char	chProduceDate[64];
		fscanf(fp,"%s",chProduceDate);

		//
		char	chGatherDate[64];
		fscanf(fp,"%s",chGatherDate);

		//ԭͼ������λ
		char	chProduceUnite[64];
		fscanf(fp,"%s",chProduceUnite);

		//
		char	chGatherUnite[64];
		fscanf(fp,"%s",chGatherUnite);

		//������Դ
		char	chOrign[32];
		fscanf(fp,"%s",chOrign);
		
		//��������
		char	chHangSheDate[64];
		fscanf(fp,"%s",chHangSheDate);

		//��������
		char	chDiaoHuiDate[64];
		fscanf(fp,"%s",chDiaoHuiDate);

		//��������
		char	chUpdataDate[64];
		fscanf(fp,"%s",chUpdataDate);

		//���µ�λ
		char	chUpdateUnite[64];
		fscanf(fp,"%s",chUpdateUnite);

		//����ϵ
		char	cCoord;
		fscanf(fp,"%c",&cCoord);

		//�̻߳�׼
		char	cHeightNormal;
		fscanf(fp,"%c",&cHeightNormal);

		//�ȸ߾�
		double	dInterval;
		fscanf(fp,"%lf",&dInterval);

		//�����
		double	dRMSE;
		fscanf(fp,"%lf",&dRMSE);

		//������Ϣ
		char	chInfo[128];
		fscanf(fp,"%s",chInfo);

		fclose(fp);
	}

	//����ͼ����Χ,��5�������Ϊ׼
	dRight	= dLeft + 15/60.0;
	dTop	= dBottom + 10/60.0;
	
	int		iNumOfX,iNumOfY;
	double	dMinHeight,dMaxHeight;

	//��ȡ��������
	fp	= fopen(szPathName,"r");
	if(fp != NULL)
	{
		//�ļ���
		char	chFileName[64];
		fscanf(fp,"%s",chFileName);

		//
		fscanf(fp,"%ld",&iNumOfX);
		
		for(int i=0;i<iNumOfX;i++)
		{
			//���
			int	iID;
			fscanf(fp,"%ld",&iID);

			//����
			int	iNum;
			fscanf(fp,"%ld",&iNum);

			//���̡߳���С�߳�
			double	dMinH,dMaxH;
			fscanf(fp,"%lf%lf",&dMaxH,&dMinH);

			//
			double	dH;
			for(int j=0;j<iNum;j++)
			{
				fscanf(fp,"%lf",&dH);
			}

			//�ж�
			if(i==0)
			{
				iNumOfY		= iNum;
				dMinHeight	= dMinH;
				dMaxHeight	= dMaxH;
			}
			else
			{
				if(iNumOfY < iNum)		iNumOfY		= iNum;
				if(dMinHeight > dMinH)	dMinHeight	= dMinH;
				if(dMaxHeight < dMaxH)	dMaxHeight	= dMaxH;
			}
		}

		fclose(fp);
	}
	else	return	false;

	double	*dHeight;
	dHeight	= new	double[iNumOfX*iNumOfY];

	for(int i=0;i<iNumOfX*iNumOfY;i++)
	{
		dHeight[i]	= -9999.0;
	}

	//��ȡ��������
	fp	= fopen(szPathName,"r");
	if(fp != NULL)
	{
		//�ļ���
		char	chFileName[64];
		fscanf(fp,"%s",chFileName);

		//
		fscanf(fp,"%ld",&iNumOfX);
		
		for(int i=0;i<iNumOfX;i++)
		{
			//���
			int	iID;
			fscanf(fp,"%ld",&iID);

			//����
			int	iNum;
			fscanf(fp,"%ld",&iNum);

			//���̡߳���С�߳�
			double	dMinH,dMaxH;
			fscanf(fp,"%lf%lf",&dMaxH,&dMinH);

			//
			double	dH;
			for(int j=0;j<iNum;j++)
			{
				fscanf(fp,"%lf",&dH);

				dHeight[j*iNumOfX+i]	= dH;
			}
		}

		fclose(fp);
	}
	
	//д��DEM�ļ�
	{
		CString	szDEMPathName;
		szDEMPathName	= szPathName.Left(szPathName.GetLength()-3) + "grd";

		FILE	*fr;
		fr	= fopen(szDEMPathName,"w");

		if(fr != NULL)
		{
			//DSAA
			fprintf(fr,"DSAA\n");

			//iCols,iRows,Ӧ���ж����ʲô���к���
			//���к���
			fprintf(fr,"%ld %ld\n",iNumOfX,iNumOfY);
			
			//dMinX,dMaxX
			fprintf(fr,"%lf %lf\n",dLeft,dRight);

			//dMinY,dMaxY
			fprintf(fr,"%lf %lf\n",dBottom,dTop);
			
			//dMinZ,dMaxZ
			fprintf(fr,"%lf %lf\n",dMinHeight,dMaxHeight);
			
			//
			int		i,j;
			double	dH;
			for(j=0;j<iNumOfY;j++)
			{
				for(i=0;i<iNumOfX;i++)
				{
					dH	= dHeight[j*iNumOfX+i];

					fprintf(fr,"%.0lf ",dH);

				}
				fprintf(fr,"\n");
			}
			
			//�ر�
			fclose(fr);
		}
	}

	if(dHeight != NULL)
	{
		delete	[]dHeight;
		dHeight	= NULL;
	}
		
	return	true;	
}

//////////////////////////////////////////////////////////////////////
// 72 ת��Mat����ΪDEM����
//////////////////////////////////////////////////////////////////////
bool CInterface_DemCreator::OnConversionMATToGRD(CString szPathName)
{
	//ȷ����Χ
	CSetMatInfoDlg	pDlg;
	
	if(pDlg.DoModal() == IDOK)
	{
	
	}
	else	return	false;

	double	dLeft,dRight,dBottom,dTop;
	int		iMatNumOfX,iMatNumOfY;
	int		iNumOfX,iNumOfY;
	double	dMinH,dMaxH;
	
	dLeft		= pDlg.m_dLeft;
	dRight		= pDlg.m_dRight;
	dBottom		= pDlg.m_dBottom;
	dTop		= pDlg.m_dTop;

	iMatNumOfX	= pDlg.m_iMatWidth;
	iMatNumOfY	= pDlg.m_iMatHeight;

	iNumOfX		= pDlg.m_iWidth;
	iNumOfY		= pDlg.m_iHeight;


	//��ȡ�����߸߳�
	{
		FILE	*fp;
		fp	= fopen(szPathName,"r");
		if(fp == NULL)	return	false;
		
		int		i,j;
		double	dH;
		for(j=0;j<iMatNumOfY;j++)
		{
			for(i=0;i<iMatNumOfX;i++)
			{
				fscanf(fp,"%lf",&dH);

				if(i==0 && j==0)
				{
					dMinH	= dMaxH	= dH;
				}
				else
				{
					if(dMinH > dH)	dMinH	= dH;
					if(dMaxH < dH)	dMaxH	= dH;
				}
			}
		}
		
		fclose(fp);
	}
	
	//�����ڴ�
	double	*dHeight;
	dHeight	= new	double[iNumOfX*iNumOfY];

	//���¶�ȡ�ļ�����ȡ��Ҫ��
	{
		FILE	*fp;
		fp	= fopen(szPathName,"r");
		if(fp == NULL)	return	false;
		
		int	iOrign;
		iOrign	= (int)(floor((iMatNumOfX-iNumOfX)/2.0));
		iOrign	= iMatNumOfX-iNumOfX-iOrign;
		
		int		i,j,I,J;
		double	dH;

		for(j=0;j<iMatNumOfY;j++)
		{
			for(i=0;i<iMatNumOfX;i++)
			{
				fscanf(fp,"%lf",&dH);
					
				if(i>=iOrign && i<iNumOfX+iOrign)
				{
					if(j>=iOrign && j<iNumOfY+iOrign)
					{
						//
						I	= i-iOrign;
						J	= (iNumOfY-1)-(j-iOrign);
						dHeight[J*iNumOfX+I]	= dH;
					}
				}
			}
		}
		
		fclose(fp);
	}
	
	//����
	{
		CString	szGrdPathName;
		szGrdPathName	= szPathName.Left(szPathName.GetLength()-3)+"grd";
			
		FILE	*fr;
		fr	= fopen(szGrdPathName,"w");

		if(fr != NULL)
		{
			//DSAA
			fprintf(fr,"DSAA\n");

			//iCols,iRows,Ӧ���ж����ʲô���к���
			//���к���
			fprintf(fr,"%ld %ld\n",iNumOfX,iNumOfY);
			
			//dMinX,dMaxX
			fprintf(fr,"%lf %lf\n",dLeft,dRight);

			//dMinY,dMaxY
			fprintf(fr,"%lf %lf\n",dBottom,dTop);
			
			//dMinZ,dMaxZ
			fprintf(fr,"%lf %lf\n",dMinH,dMaxH);
			
			//
			int		i,j;
			double	dH;
			for(j=0;j<iNumOfY;j++)
			{
				for(i=0;i<iNumOfX;i++)
				{
					dH	= dHeight[j*iNumOfX+i];

					fprintf(fr,"%.0lf ",dH);

				}
				fprintf(fr,"\n");
			}
			
			//�ر�
			fclose(fr);
		}
	}


	//
	if(dHeight != NULL)
	{
		delete	[]dHeight;
		dHeight	= NULL;
	}
	
	return	true;
}

//////////////////////////////////////////////////////////////////////
// 72 ת��Mat����ΪDEM����
//////////////////////////////////////////////////////////////////////
bool CInterface_DemCreator::OnConversionDVFToBLN(CString szPathName)
{
	FILE	*fr;
	fr	= fopen(szPathName,"r");
	if(fr == NULL)	return	false;
	
	CString	szBLNPathName;
	szBLNPathName	= szPathName.Left(szPathName.GetLength()-4) + "bln";

	FILE	*fw;
	fw	= fopen(szBLNPathName,"w");
	if(fw == NULL)	return	false;
	
	//
	double	dMapVersion;
	fscanf(fr,"%lf",&dMapVersion);
	
	//
	long	nMapScale;
	fscanf(fr,"%ld",&nMapScale);
	
	//
	double	dLeft,dRight,dBottom,dTop;
	fscanf(fr,"%lf%lf%lf%lf",&dLeft,&dBottom,&dRight,&dTop);

	//
	long	nMapUnite;
	fscanf(fr,"%ld",&nMapUnite);

	long	iSign;
	long	iPointNumber;
	long	iMainType;
	long	iSubType;
	char	chName[60];
	double	dHeight;
	int		iPointNum;
	
	while(fscanf(fr,"%ld%ld%ld%ld%s%lf%ld",&iSign,&iPointNumber,&iMainType,&iSubType,chName,&dHeight,&iPointNum) == 7)
	{	
		//�����
		if(iSign == 100)
		{
			for(int i=0;i<iPointNum;i++)
			{
				double	dX,dY;
				fscanf(fr,"%lf%lf",&dX,&dY);
			}
		}
		else
		//�߶������ڰ��յ������
		if(iSign == 200)
		{
			fprintf(fw,"%ld,0\n",iPointNum);

			for(int i=0;i<iPointNum;i++)
			{
				double	dX,dY;

				fscanf(fr,"%lf%lf",&dX,&dY);
				
				fprintf(fw,"%lf, %lf, %lf\n",dX,dY,dHeight);			
			}
		}
	}

	fclose(fr);

	fclose(fw);

	return	true;
}

//////////////////////////////////////////////////////////////////////
// 73 ����Ĩƽ�����Ѹ߳�С����Ĺ��㵽��
//////////////////////////////////////////////////////////////////////
bool CInterface_DemCreator::OnTerrainToZero()
{
	//
	return	m_pGridDem.OnTerrainToZero();
}

//////////////////////////////////////////////////////////////////////
// 74 ����Ripley K����
//////////////////////////////////////////////////////////////////////
void CInterface_DemCreator::OnCalcRipleyK()
{
	//��������
	m_pPointPatternK.SetPointFeature(m_pWorkspace.GetPointFeature());

	//
	m_pPointPatternK.SetPathName(m_szPathName);
	
	m_pPointPatternK.CalcRipleyKWithPoint();
}

//////////////////////////////////////////////////////////////////////
// 75 ��ȡ����
//////////////////////////////////////////////////////////////////////
void CInterface_DemCreator::OnPickUpGrid(CString szPathName)
{
	if(m_pGridDem.IsEmpty())
	{
		return;
	}

	//���ȶ�ȡ����
	CArray<tagRECTANGLERANGE,tagRECTANGLERANGE>	pRange;	
	
	FILE	*fp;
	fp	=fopen(szPathName,"r");

	if(fp != NULL)
	{
		//
		double	dMapVersion;
		fscanf(fp,"%lf",&dMapVersion);

		//
		long	nMapScale;
		fscanf(fp,"%ld",&nMapScale);

		//
		double	dMapLeft,dMapBottom,dMapRight,dMapTop;
		fscanf(fp,"%lf%lf%lf%lf",&dMapLeft,&dMapBottom,&dMapRight,&dMapTop);

		//
		long	nMapUnite;
		fscanf(fp,"%ld",&nMapUnite);

		long	iSign;
		long	iPointNumber;
		long	iMainType;
		long	iSubType;
		char	chName[60];
		double	dHeight;
		int		iPointNum;
		
		while(fscanf(fp,"%ld%ld%ld%ld%s%lf%ld",&iSign,&iPointNumber,&iMainType,&iSubType,chName,&dHeight,&iPointNum) == 7)
		{

			//�����
			if(iSign == 100)
			{
				for(int i=0;i<iPointNum;i++)
				{
					double	dX,dY;
					fscanf(fp,"%lf%lf",&dX,&dY);
				}
			}
			else
			//�߶������ڰ��յ������
			if(iSign == 200)
			{
				for(int i=0;i<iPointNum;i++)
				{
					double	dX,dY;

					fscanf(fp,"%lf%lf",&dX,&dY);

					if(i==0)	
					{
						dMapLeft	= dMapRight	= dX;
						dMapBottom	= dMapTop	= dY;
					}
					else
					{
						if(dMapLeft > dX)	dMapLeft	= dX;
						if(dMapRight < dX)	dMapRight	= dX;
						if(dMapBottom > dY)	dMapBottom	= dY;
						if(dMapTop < dY)	dMapTop		= dY;
					}
				}
				
				tagRECTANGLERANGE	dRectRange;
				dRectRange.iID		= pRange.GetSize() + 1;
				dRectRange.dLeft	= dMapLeft;
				dRectRange.dRight	= dMapRight;
				dRectRange.dBottom	= dMapBottom;
				dRectRange.dTop		= dMapTop;

				pRange.Add(dRectRange);				
			}
			else
			//�����������ʱ������
			if(iSign == 300)
			{
				//
				MessageBox(NULL,"Wrong","��ʾ��Ϣ",MB_OK);
			}
			else
			{
				//
				MessageBox(NULL,"Wrong","��ʾ��Ϣ",MB_OK);
			}
		}
	}

	for(int i=0;i<pRange.GetSize();i++)
	{
		CString	szDemPathName;
		CString	szSmallPathName;

		szDemPathName	= m_pGridDem.GetPathName();

		szSmallPathName.Format("%s_%ld.grd",szDemPathName.Left(szDemPathName.GetLength()-4),pRange[i].iID);
	
		m_pGridDem.OnPickUpGrid(pRange[i].dLeft,pRange[i].dRight,pRange[i].dBottom,pRange[i].dTop,szSmallPathName);
	}

	pRange.RemoveAll();
}

//////////////////////////////////////////////////////////////////////
// 76 ��ȡ����
//////////////////////////////////////////////////////////////////////
void CInterface_DemCreator::OnWavelets()
{
	if(m_pGridDem.IsEmpty())
	{
		return;
	}

	//��ȡDEM����
	double	*pData;
	pData	= m_pGridDem.GetHeight();

	//
	int		iNumOfX,iNumOfY;
	iNumOfX	= m_pGridDem.GetNumOfX();
	iNumOfY	= m_pGridDem.GetNumOfY();

	//
	double	dMinH,dMaxH;
	dMinH	= m_pGridDem.GetMinHeight();
	dMaxH	= m_pGridDem.GetMaxHeight();

	//
	double	dLeft,dRight,dBottom,dTop;
	m_pGridDem.GetDemExt(dLeft,dRight,dBottom,dTop);
	
	CString	szPathName;
	szPathName	= m_pGridDem.GetPathName();

	//
	CString	szWaveletsPathName;
	szWaveletsPathName	= szPathName.Left(szPathName.GetLength()-4) + "-2.grd";
	
	int		iNewNumOfX,iNewNumOfY;
	double	*pNewData;

	if(iNumOfX % 2 == 1)
	{
		iNewNumOfX	= iNumOfX + 1;
		iNewNumOfY	= iNumOfY + 1;

		pNewData	= new	double[iNewNumOfX * iNewNumOfY];
		
		//����
		for(int j=0;j<iNewNumOfY-1;j++)
		{
			for(int i=0;i<iNewNumOfX-1;i++)
			{
				pNewData[j*iNewNumOfX+i]	= pData[j*iNumOfX+i];
			}
		}

		//
		j	= iNewNumOfY-1;
		{
			for(int i=0;i<iNewNumOfX-1;i++)
			{
				pNewData[j*iNewNumOfX+i]	= pNewData[(j-1)*iNewNumOfX+i];
			}
		}

		//
		for(j=0;j<iNewNumOfY-1;j++)
		{
			int	i	= iNewNumOfX-1;
			{
				pNewData[j*iNewNumOfX+i]	= pNewData[j*iNewNumOfX+i-1];
			}
		}

		//���һ��
		pNewData[(iNewNumOfY-1)*iNewNumOfX+iNewNumOfX-1]	= pNewData[(iNewNumOfY-2)*iNewNumOfX+iNewNumOfX-2];
	}
	else
	{
		iNewNumOfX	= iNumOfX;
		iNewNumOfY	= iNumOfY;

		pNewData	= new	double[iNewNumOfX * iNewNumOfY];
		
		//����
		for(int j=0;j<iNewNumOfY;j++)
		{
			for(int i=0;i<iNewNumOfX;i++)
			{
				pNewData[j*iNewNumOfX+i]	= pData[j*iNewNumOfX+i];
			}
		}
	}

	//��������
	m_pWavelets.SetOrignDem(pNewData,iNewNumOfX,iNewNumOfY,dMinH,dMaxH,dLeft,dRight,dBottom,dTop);
	
	//С���任
	m_pWavelets.WaveletsD4();

	m_pWavelets.SaveCurrentDem(szWaveletsPathName);

	delete	[]pNewData;
	pNewData	= NULL;
}

//////////////////////////////////////////////////////////////////////
// 99 �ص�����
//////////////////////////////////////////////////////////////////////
void CInterface_DemCreator::OnCallBackFunc_Process(void *pFunc)
{
	m_OnProcess	= (ONPROCESS)pFunc;
}


//////////////////////////////////////////////////////////////////////
// 101 ��ʾ��������[protected]
//////////////////////////////////////////////////////////////////////
bool CInterface_DemCreator::InitShow(CRect rRect, double dLeft, double dRight, double dBottom, double dTop)
{
	if(fabs(dRight-dLeft) < 1e-10)	return	false;
	if(fabs(dTop-dBottom) < 1e-10)	return	false;

	double	dScalex = fabs(rRect.Width() / (dRight-dLeft));
	double	dScaley = fabs(rRect.Height() / (dTop-dBottom));
	
	double	dMapScale;

	dMapScale		= (dScalex<dScaley) ? dScalex : dScaley;

	//��С
	dMapScale		= dMapScale * 0.96;

	double dWinOrgDistX = (rRect.Width() - fabs(dRight-dLeft)*dMapScale) /2.0;
	double dWinOrgDistY = (rRect.Height() - fabs(dTop-dBottom)*dMapScale) /2.0;

	m_dMapXA		= dMapScale;
	m_dMapXB		= dWinOrgDistX - dLeft * dMapScale + rRect.left;
	m_dMapYA		= -1.0 * dMapScale;
	m_dMapYB		= dBottom * dMapScale - dWinOrgDistY + rRect.bottom;
	
	//������ʾ����
	SetShowExt(dLeft,dRight,dBottom,dTop);

	return	true;
}

//////////////////////////////////////////////////////////////////////
// 102 ��Ļ����͵�ͼ����ת��[protected]
//////////////////////////////////////////////////////////////////////
void CInterface_DemCreator::ScreenToMap(int cx, int cy, double &xx, double &yy)
{
	xx	= (cx - m_dMapXB) / m_dMapXA;
	yy	= (cy - m_dMapYB) / m_dMapYA;
}
	
void CInterface_DemCreator::MapToScreen(double xx, double yy,int& cx, int& cy)
{
	cx	= (int)(xx * m_dMapXA + m_dMapXB +0.5);
	cy	= (int)(yy * m_dMapYA + m_dMapYB +0.5);
}

//////////////////////////////////////////////////////////////////////
// 102 ��Ļ����͵�ͼ����ת��[protected]
//////////////////////////////////////////////////////////////////////
void CInterface_DemCreator::OnLocalTerrainCharacter()
{
	m_iMapTool	= 4;

	//������ʾ�Ի���
	//���Ǹ�ѡ��		
	if(m_pLocalTCDlg == NULL)		m_pLocalTCDlg	= new	CSetLocalTCDlg(m_pMainWnd);
	
	if(m_pLocalTCDlg->GetSafeHwnd() == 0)
	{
		if(!m_pLocalTCDlg->Create(m_pMainWnd))	
		{
			MessageBox(NULL,_T("�������ڴ���!"),_T("��ʾ��Ϣ"),MB_ICONEXCLAMATION);
	
			delete	m_pLocalTCDlg;
			m_pLocalTCDlg	= NULL;

			return;
		}
		m_pLocalTCDlg->MoveWindowPos();
	}
	if(m_pLocalTCDlg)
	{
		m_pLocalTCDlg->ShowWindow(SW_SHOW);
		m_pLocalTCDlg->UpdateData(FALSE);	
	}
}

//////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////
void CInterface_DemCreator::OnCalcSubjectValue()
{
	CSetCALCLSD_Dlg	pDlg;

	pDlg.DoModal();
}

//////////////////////////////////////////////////////////////////////
// ���в����;ֲ���������������ϣ������ڿռ�������
//////////////////////////////////////////////////////////////////////
void CInterface_DemCreator::OnCombineREAndLocalTerrainCharacter()
{
	CSetRECombineTEDlg	pDlg;

	pDlg.DoModal();
}

//
void CInterface_DemCreator::OnClusterAnalysis(CDC *pDC)
{
	m_pClusterAnalysis.OnExecute(pDC, 0, 10);
}

//
void CInterface_DemCreator::OnDimensionAnalyse()
{
	CSetDimensionAnalyseDlg	pDlg;

	if(pDlg.DoModal() == IDOK)
	{

	}
	else	return;

	CDCR_DimensionAnalyse	pAnalyse;
	char					chDelims[16];

	if(pDlg.m_iDelims == 0)		strcpy(chDelims,"\t");
	if(pDlg.m_iDelims == 1)		strcpy(chDelims," ");
	if(pDlg.m_iDelims == 2)		strcpy(chDelims,",");

	if(pAnalyse.OnRead(pDlg.m_szPathName,pDlg.m_iFieldNum,chDelims))
	{
		pAnalyse.OnExecute(pDlg.m_iMode);
	}
}

//
void CInterface_DemCreator::OnSearchCurrentPoint()
{
	if(m_pWorkspace.IsEmpty())	return;
	
	//������ʾ�Ի���
	if(m_pSearchPointDlg == NULL)		m_pSearchPointDlg	= new	CSetSearchPointDlg(m_pMainWnd);
	
	if(m_pSearchPointDlg->GetSafeHwnd() == 0)
	{
		if(!m_pSearchPointDlg->Create(m_pMainWnd))	
		{
			MessageBox(NULL,_T("�������ڴ���!"),_T("��ʾ��Ϣ"),MB_ICONEXCLAMATION);
	
			delete	m_pSearchPointDlg;
			m_pSearchPointDlg	= NULL;

			return;
		}
		m_pSearchPointDlg->MoveWindowPos();
	}

	if(m_pSearchPointDlg)
	{
		m_pSearchPointDlg->ShowWindow(SW_SHOW);
		m_pSearchPointDlg->MoveWindowPos();
		m_pSearchPointDlg->UpdateData(FALSE);	
	}

	if(m_pSearchPointDlg != NULL)
	{
		//
		m_pSearchPointDlg->m_pLocalTerrain	= &m_pLocalTerrain;
		m_pSearchPointDlg->m_pWorkspace		= &m_pWorkspace;

		double	dLeft,dRight,dBottom,dTop;
		GetShowExt(dLeft,dRight,dBottom,dTop);
		m_pSearchPointDlg->m_dWidth			= dRight-dLeft;
		m_pSearchPointDlg->m_dHeight		= dTop-dBottom;
	}
}

//
bool CInterface_DemCreator::OnSearchCurrentPoint(double dX,double dY)
{	
	if(m_pWorkspace.IsEmpty())	return	false;
	
	//
	CSetSearchPointExDlg	pDlg;

	if(pDlg.DoModal() == IDOK)
	{
		
	}
	else	return	false;

	//
	dX	= pDlg.m_dX;
	dY	= pDlg.m_dY;
	if(m_pWorkspace.SelectPoint(dX,dY))
	{
		double	dLeft,dRight,dBottom,dTop;
		GetShowExt(dLeft,dRight,dBottom,dTop);

		double	dWidth,dHeight;
		dWidth	= (dRight-dLeft);
		dHeight	= (dTop-dBottom);
		
		//
		dLeft	= dX-dWidth/2;
		dRight	= dLeft + dWidth;

		dBottom	= dY-dHeight/2;
		dTop	= dBottom + dHeight;

		SetShowExt(dLeft,dRight,dBottom,dTop);

		if(m_pMainWnd != NULL)	m_pMainWnd->Invalidate(FALSE);

		return	true;
	}

	return	false;
}	

//
void CInterface_DemCreator::SwitchNewRegion(CRect rRect, bool bSame)
{
	double	dLeft,dRight,dBottom,dTop;

	//��γ��
	dLeft	= rRect.left;
	dRight	= rRect.right;
	dBottom	= rRect.bottom;
	dTop	= rRect.top;
	
	SetShowExt(dLeft,dRight,dBottom,dTop);

	if(m_pMainWnd != NULL)	m_pMainWnd->Invalidate(FALSE);
}

//
void CInterface_DemCreator::OnJudgeGeoMorphological(int iType)
{
	if(m_pGridDem.IsEmpty())
	{
		return;
	}

	CSetWindownSize	pDlg;

	if(pDlg.DoModal() == IDOK)
	{
		
	}
	else	return;
	
	AfxGetApp()->DoWaitCursor(1);

	int		iWindowSize;
	iWindowSize	= pDlg.m_iSize;

	//��ȡDEM����
	double	*pData;
	pData	= m_pGridDem.GetHeight();

	//
	int		iNumOfX,iNumOfY;
	iNumOfX	= m_pGridDem.GetNumOfX();
	iNumOfY	= m_pGridDem.GetNumOfY();

	//
	double	dSizeOfX,dSizeOfY;
	dSizeOfX	= m_pGridDem.GetSizeOfX();
	dSizeOfY	= m_pGridDem.GetSizeOfY();

	//
	double	dRealSize;
	dRealSize	= m_pGridDem.GetRealSize();

	//
	double	dLeft,dRight,dBottom,dTop;
	m_pGridDem.GetDemExt(dLeft,dRight,dBottom,dTop);
	
	CString	szPathName;
	szPathName	= m_pGridDem.GetPathName();
	
	//
	m_pGeoMorphological.SetGridDem(pData,iNumOfX,iNumOfY,dSizeOfX,dSizeOfY,dRealSize,dLeft,dRight,dBottom,dTop,szPathName);


	//
	m_pGeoMorphological.SetWindowSize(iWindowSize);
	
	//
	if(m_pGeoMorphological.OnJudge(iType))
	{
		MessageBox(NULL,"���õ�ò�����Զ��������!","��ʾ��Ϣ",MB_OK);	
	}

	AfxGetApp()->DoWaitCursor(-1);
}

///////////////////////////////////////////////////////////////
//�ٷֱȷ���
///////////////////////////////////////////////////////////////
void CInterface_DemCreator::OnPercentAnalyse()
{
	CSetDeltaHPerDlg	pDlg;

	pDlg.DoModal();
}

//�в�ٷֱȷ���������
void CInterface_DemCreator::OnPercentAnalyseCombat()
{
	AfxGetApp()->DoWaitCursor(1);

	//�ļ���Ϣ
	CString		m_szIPathName;

	//�޶�����
	int		iCount;
	
	typedef	struct	tagCondition
	{
		bool	bCheckSlope;
		double	dMinSlope;
		double	dMaxSlope;

		bool	bCheckPattern;
		double	dMinPattern;
		double	dMaxPattern;

		bool	bCheckDistance;
		double	dMinDistance;
		double	dMaxDistance;

		bool	bCheckPingYuan;
		double	dMinPingYuan;
		double	dMaxPingYuan;

		bool	bCheckQiuLing;
		double	dMinQiuLing;
		double	dMaxQiuLing;

		bool	bCheckShanDi;
		double	dMinShanDi;
		double	dMaxShanDi;
	}CONDITION;

	CONDITION	*strCondition;
	
	iCount			= 0;
	strCondition	= NULL;
	
	{
		CString	szPathName;
		szPathName	= m_szExePathName + "\\Lib\\Creator\\PercentAnalyse.txt";
		//
		FILE	*fp;
		//
		fp	= fopen(szPathName,"r");
		
		//������LTF�ļ�
		//{
		char	chPathName[200];
		fscanf(fp,"%s",chPathName);	m_szPathName	= chPathName;
		//}

		//
		fscanf(fp,"%ld",&iCount);
		strCondition	= new	CONDITION[iCount];

		//
		for(int i=0;i<iCount;i++)
		{
			double	dMinSlope;
			double	dMaxSlope;

			double	dMinPattern;
			double	dMaxPattern;

			double	dMinDistance;
			double	dMaxDistance;

			double	dMinPingYuan;
			double	dMaxPingYuan;

			double	dMinQiuLing;
			double	dMaxQiuLing;

			double	dMinShanDi;
			double	dMaxShanDi;
				
			int		iSign;
			
			//
			fscanf(fp,"%ld%lf%lf",&iSign,&dMinSlope,&dMaxSlope);
			if(iSign == 1)	strCondition[i].bCheckSlope	= true;
			else			strCondition[i].bCheckSlope	= false;
			strCondition[i].dMinSlope	= dMinSlope;
			strCondition[i].dMaxSlope	= dMaxSlope;

			//
			fscanf(fp,"%ld%lf%lf",&iSign,&dMinPattern,&dMaxPattern);
			if(iSign == 1)	strCondition[i].bCheckPattern	= true;
			else			strCondition[i].bCheckPattern	= false;
			strCondition[i].dMinPattern	= dMinPattern;
			strCondition[i].dMaxPattern	= dMaxPattern;
			
			//
			fscanf(fp,"%ld%lf%lf",&iSign,&dMinDistance,&dMaxDistance);
			if(iSign == 1)	strCondition[i].bCheckDistance	= true;
			else			strCondition[i].bCheckDistance	= false;
			strCondition[i].dMinDistance	= dMinDistance;
			strCondition[i].dMaxDistance	= dMaxDistance;

			//
			fscanf(fp,"%ld%lf%lf",&iSign,&dMinPingYuan,&dMaxPingYuan);
			if(iSign == 1)	strCondition[i].bCheckPingYuan	= true;
			else			strCondition[i].bCheckPingYuan	= false;
			strCondition[i].dMinPingYuan	= dMinPingYuan;
			strCondition[i].dMaxPingYuan	= dMaxPingYuan;

			//
			fscanf(fp,"%ld%lf%lf",&iSign,&dMinQiuLing,&dMaxQiuLing);
			if(iSign == 1)	strCondition[i].bCheckQiuLing	= true;
			else			strCondition[i].bCheckQiuLing	= false;
			strCondition[i].dMinQiuLing	= dMinQiuLing;
			strCondition[i].dMaxQiuLing	= dMaxQiuLing;

			//
			fscanf(fp,"%ld%lf%lf",&iSign,&dMinShanDi,&dMaxShanDi);
			if(iSign == 1)	strCondition[i].bCheckShanDi	= true;
			else			strCondition[i].bCheckShanDi	= false;
			strCondition[i].dMinShanDi	= dMinShanDi;
			strCondition[i].dMaxShanDi	= dMaxShanDi;
		}

		fclose(fp);
	}

	CString	szSavePathName;
	FILE	*fw;

	szSavePathName	= m_szExePathName + "\\Lib\\Creator\\PercentAnalyseResult.txt";
	
	//
	fw		= fopen(szSavePathName,"w");
	if(fw == NULL)	
	{
		if(strCondition != NULL)
		{
			delete	[]strCondition;
			strCondition	= NULL;
		}	

		return;
	}

	//̧ͷ
	fprintf(fw,"Slope_Sign\t Slope_Range\t Pattern_Sign\t Pattern_Range\t IDW_COUNT\t SPD_COUNT\t MQF_COUNT\t MLF_COUNT\t TPSF_COUNT\t NCSF_COUNT\t EXP_COUNT\t LINE_COUNT\t COUNT\t\n");

	//���з���
	for(int i=0;i<iCount;i++)
	{
		//////////////////////////////////////////////////////////////////
		//��ʼ�ж�
		int		iTotalNum	= 0;
		int		iIDWCount	= 0;
		int		iSPDCount	= 0;
		int		iMQFCount	= 0;
		int		iMLFCount	= 0;
		int		iTPSFCount	= 0;
		int		iNCSFCount	= 0;
		int		iEXPCount	= 0;
		int		iLINECount	= 0;
		
		int		iPointCount	= 0;

		//
		{
			FILE	*fp;
			
			//
			fp		= fopen(m_szPathName,"r");
			
			//��IDWΪ��׼,ѭ��!
			double	dX,dY,dZ;
			double	dSlope,dPattern;
			double	dIDWZ,dSPDZ,dMQFZ,dMLFZ,dTPSFZ,dNCSFZ,dEXPZ,dLINEZ;
			
			while( fscanf(fp,"%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf",&dX,&dY,&dZ,&dSlope,&dPattern,&dIDWZ,&dSPDZ,&dMQFZ,&dMLFZ,&dTPSFZ,&dNCSFZ,&dEXPZ,&dLINEZ) == 13)
			{
				//�Ƿ�����������ķ�Χ֮��
				bool	bInSlopeCondition	= false;
				dSlope	= dSlope/90.0;
				if(strCondition[i].bCheckSlope) 
				{
					if(dSlope>=strCondition[i].dMinSlope && dSlope<=strCondition[i].dMaxSlope)
						bInSlopeCondition	= true;
				}
	
				bool	bInPatternCondition	= false;
				dPattern	= dPattern/2.149;

				if(strCondition[i].bCheckPattern) 
				{
					if(dPattern>=strCondition[i].dMinPattern && dPattern<=strCondition[i].dMaxPattern)
						bInPatternCondition	= true;
				}

				if(bInSlopeCondition && bInPatternCondition)
				{	
					//����
					dIDWZ	= fabs(dIDWZ);
					dSPDZ	= fabs(dSPDZ);
					dMQFZ	= fabs(dMQFZ);
					dMLFZ	= fabs(dMLFZ);
					dTPSFZ	= fabs(dTPSFZ);
					dNCSFZ	= fabs(dNCSFZ);
					dEXPZ	= fabs(dEXPZ);
					dLINEZ	= fabs(dLINEZ);

					//����
					iTotalNum++;

					double	dDeltaH[8];
					dDeltaH[0]	= dIDWZ;
					dDeltaH[1]	= dSPDZ;
					dDeltaH[2]	= dMQFZ;
					dDeltaH[3]	= dMLFZ;
					dDeltaH[4]	= dTPSFZ;
					dDeltaH[5]	= dNCSFZ;
					dDeltaH[6]	= dEXPZ;
					dDeltaH[7]	= dLINEZ;
					
					//����
					for(int I=0;I<8;I++)
					{
						for(int J=I+1;J<8;J++)
						{
							if(dDeltaH[I] > dDeltaH[J])
							{
								double	dTemp;
								dTemp		= dDeltaH[I];
								dDeltaH[I]	= dDeltaH[J];
								dDeltaH[J]	= dTemp;
							}
						}
					}

					//Ѱ����С�в�
					double	dMinH;
					for(I=0;I<8;I++)
					{
						if(dDeltaH[I] != INVALID_VALUE)
						{
							dMinH	= dDeltaH[I];
							break;
						}
					}
						
					//�����Щ��ֵ�㷨����Ĳв�ֵ����Сֵ���					
					//���С��0.01�ף���1����
					if( fabs(dMinH - dIDWZ) < 0.01)		iIDWCount++;
					if( fabs(dMinH - dSPDZ) < 0.01)		iSPDCount++;
					if( fabs(dMinH - dMQFZ) < 0.01)		iMQFCount++;
					if( fabs(dMinH - dMLFZ) < 0.01)		iMLFCount++;
					if( fabs(dMinH - dTPSFZ) < 0.01)	iTPSFCount++;
					if( fabs(dMinH - dNCSFZ) < 0.01)	iNCSFCount++;
					if( fabs(dMinH - dEXPZ) < 0.01)		iEXPCount++;
					if( fabs(dMinH - dLINEZ) < 0.01)	iLINECount++;
				}

				iPointCount++;
			}
		}

		//�¶�
		if(strCondition[i].bCheckSlope)		fprintf(fw,"%ld\t ",1);
		else								fprintf(fw,"%ld\t ",0);
		fprintf(fw,"[%.2lf-%.2lf]\t ",strCondition[i].dMinSlope,strCondition[i].dMaxSlope);

		//����
		if(strCondition[i].bCheckPattern)	fprintf(fw,"%ld\t ",1);
		else								fprintf(fw,"%ld\t ",0);
		fprintf(fw,"[%.2lf-%.2lf]\t ",strCondition[i].dMinPattern,strCondition[i].dMaxPattern);
		 
		fprintf(fw,"%ld\t %ld\t %ld\t %ld\t %ld\t %ld\t %ld\t %ld\t %ld\t\n",iIDWCount,iSPDCount,iMQFCount,iMLFCount,iTPSFCount,iNCSFCount,iEXPCount,iLINECount,iTotalNum);
	}
	
	fclose(fw);

	if(strCondition != NULL)
	{
		delete	[]strCondition;
		strCondition	= NULL;
	}

	MessageBox(NULL,"�������!","��ʾ��Ϣ",MB_OK);

	AfxGetApp()->DoWaitCursor(-1);
}

//���������Ⱦ��󣬼�����ɢ���������ֵ
void CInterface_DemCreator::OnCalcLiShuDuValue()
{
	AfxGetApp()->DoWaitCursor(1);

	if(m_pGridDem.IsEmpty())
	{
		MessageBox(NULL,"���������¼��������Ϣ��GRD","��ʾ��Ϣ",MB_OK);

		return;
	}
	
	if(m_pWorkspace.IsEmpty())
	{
		MessageBox(NULL,"����������ɢ����Ϣ","��ʾ��Ϣ",MB_OK);

		return;
	}

	CString	szPathName;
	szPathName	= m_pWorkspace.GetPathName();
	
	//
	CString	szSavePathName;
	int		iBreakID;
	iBreakID		= szPathName.ReverseFind('.');
	szSavePathName	= szPathName.Left(iBreakID)+".txt";

	FILE	*fw;
	fw	= fopen(szSavePathName,"w");
	if(fw == NULL)		return;
	
	//
	fprintf(fw,"������\t������\t������ֵ\n");
	
	//������
	CDCR_PointFeature	*pPointHead;
	CDCR_PointFeature	*pPointFeature;

	pPointHead		= m_pWorkspace.GetPointFeature();
	if(pPointHead == NULL)	return;
		
	
	pPointFeature	= pPointHead;
	while(pPointFeature != NULL)
	{
		//
		if(pPointFeature->m_bUsed == false)
		{
			//
			double	dX,dY;
			dX	= pPointFeature->m_pPointCoord.x;
			dY	= pPointFeature->m_pPointCoord.y;
			
			double	dValue;
			dValue	= m_pGridDem.GetHeight(dX,dY);

			//���ֵ�����壬�����ʵ��
			//if(dValue != INVALID_VALUE)
			{
				//
				fprintf(fw,"%lf\t%lf\t%lf\n",dX,dY,dValue);
			}
		}

		pPointFeature	= pPointFeature->m_pNext;
	}

	fclose(fw);

	AfxGetApp()->DoWaitCursor(1);
}

//////////////////////////////////////////////////////////////////////
// �ڡ����š���ֵ����ʱ����в�ֵ������Ӧ�ľֲ�������������D2P64Ϊ׼������������ʡ�Ժܶ�Ĺ�����
//////////////////////////////////////////////////////////////////////
void CInterface_DemCreator::OnCalcLTCandDeltaH()
{
	if(m_pWorkspace.IsEmpty())	
	{
		MessageBox(NULL,"���ȶ�������!","��ʾ��Ϣ",MB_OK);

		return	;
	}

	//��һ������ȡ��ֵ����
	CDCR_Param_IDW	pIDW;
	CDCR_Param_SPD	pSPD;
	CDCR_Param_RBF	pMQF;
	CDCR_Param_RBF	pMLF;
	CDCR_Param_RBF	pTPSF;
	CDCR_Param_RBF	pNCSF;
	CDCR_Param_KRG	pEXP;
	CDCR_Param_KRG	pLINE;

	{
		FILE	*fr;
		//
		CString			szPathName;
		szPathName		= m_szBasePathName + "\\LocalTerrainBat.txt";
		
		fr				= fopen(szPathName,"r");
		
		if(fr == NULL)	
		{
			MessageBox(NULL,szPathName + "\n������!","��ʾ��Ϣ",MB_OK);

			return	;
		}
		
		//
		pIDW.Read(fr);
		//
		pSPD.Read(fr);
		//
		pMQF.Read(fr);
		//
		pMLF.Read(fr);
		//
		pTPSF.Read(fr);
		//
		pNCSF.Read(fr);
		//
		pEXP.Read(fr);
		//
		pLINE.Read(fr);

		fclose(fr);
	}

	//��һ�� : ��ȡ���в�������(�����ظ���)
	int					iSampleCount;
	iSampleCount	= m_pWorkspace.GetSamplePointCount();
	CDCR_PointFeature	*pSamplePointList;
	pSamplePointList= m_pWorkspace.GetPointFeature();
		
	FILE	*fw;
	CString	szPathName;
	szPathName		= m_pWorkspace.GetPathName();
	CString	szSavePathName;
	szSavePathName	= szPathName.Left(szPathName.GetLength()-4) + "ltb";
	
	fw	= fopen(szSavePathName,"w");
	if(fw == NULL)	
	{
		MessageBox(NULL,"�洢·��������!","��ʾ��Ϣ",MB_OK);

		return	;
	}
	
	//����������������Ϣ���ݸ�CDCR_LocalTerrainCreator
	CDCR_LocalTerrainCreator	m_pLTCreator;
	{
		//1�����÷���������Ŀ
		int		iCellNumX,iCellNumY;
		m_pWorkspace.GetCellCount(iCellNumX,iCellNumY);

		//2�����÷����ĸ����ߴ�
		double	dCellSizeX,dCellSizeY;
		m_pWorkspace.GetCellSize(dCellSizeX,dCellSizeY);

		//3�����÷����ķ�Χ���ͽ�ģ��Χ��һ��
		double	dCellLeft,dCellRight,dCellBottom,dCellTop;
		m_pWorkspace.GetCellExt(dCellLeft,dCellRight,dCellBottom,dCellTop);

		//4�����÷���������Ϣ
		CDCR_Cells	*pCells;
		pCells	= m_pWorkspace.GetCell();

		//5����������Ϣ���ݵ�CDCR_LocalTerrainCreator��
		m_pLTCreator.SetCell(pCells,iCellNumX,iCellNumY,dCellSizeX,dCellSizeY,dCellLeft,dCellRight,dCellBottom,dCellTop);
	}

	//���Ĳ�����������Ϣ���ݸ�m_pGridCreator
	{
		//�����ͷ�����
		m_pGridCreator.Release();

		//���ý�ģ�ķ�Χ
		double	dLeft,dRight,dBottom,dTop;
		m_pWorkspace.GetMapExt(dLeft,dRight,dBottom,dTop);
		m_pGridCreator.SetMapExt(dLeft,dRight,dBottom,dTop);

		//���÷���������Ŀ
		int		iCellNumX,iCellNumY;
		m_pWorkspace.GetCellCount(iCellNumX,iCellNumY);
		m_pGridCreator.SetCellCount(iCellNumX,iCellNumY);

		//���÷����ĸ����ߴ�
		double	dCellSizeX,dCellSizeY;
		m_pWorkspace.GetCellSize(dCellSizeX,dCellSizeY);
		m_pGridCreator.SetCellSize(dCellSizeX,dCellSizeY);

		//���÷����ķ�Χ���ͽ�ģ��Χ��һ��
		double	dCellLeft,dCellRight,dCellBottom,dCellTop;
		m_pWorkspace.GetCellExt(dCellLeft,dCellRight,dCellBottom,dCellTop);
		m_pGridCreator.SetCellExt(dCellLeft,dCellRight,dCellBottom,dCellTop);

		//���÷�������
		m_pGridCreator.SetCell(m_pWorkspace.GetCell());
		
		//
		m_pGridCreator.SetCoordMapping(m_dMapXA,m_dMapXB,m_dMapYA,m_dMapYB);
	}

	//����MatCom
	initM(MATCOM_VERSION);

	//���岽������
	CDCR_PointFeature	*pPoint;

	pPoint	= pSamplePointList;
	while(pPoint != NULL)
	{
		//
		m_pLTCreator.CalculateLocalTerrainCharacter(iSampleCount,pSamplePointList,pPoint,2,8,64,30000);
		
		//�õ��ֲ���������ֵ
		double	dMeanSlope;
		double	dPointPattern;
		//
		dMeanSlope		= m_pLTCreator.GetMeanSlope();
		dPointPattern	= m_pLTCreator.GetPointPatternValue();

		//����ÿһ��ֵ�㷨�Ĳв�ֵ
		{
			//IDW
			double	dIDWZ	= -32767;
			m_pGridCreator.CrossValidateInIDW(NULL,iSampleCount,pSamplePointList,pPoint,
												pIDW.m_iSearchType,
												pIDW.m_iPointNumPreDirection,
												pIDW.m_iPointNumInTotal,
												pIDW.m_dSearchRadius,
												pIDW.m_iU[0],
												pIDW.m_dT,
												dIDWZ);

			//SPD
			double	dSPDZ	= -32767;
			m_pGridCreator.CrossValidateInSPD(NULL,iSampleCount,pSamplePointList,pPoint,
												pSPD.m_iSearchType,
												pSPD.m_iPointNumPreDirection,
												pSPD.m_iPointNumInTotal,
												pSPD.m_dSearchRadius,
												pSPD.m_iU[0],
												pSPD.m_dT,
												pSPD.m_iRadiusType,
												pSPD.m_dMaxRadius,
												pSPD.m_iKernalType,
												dSPDZ);

			//MQF
			double	dMQFZ	= -32767;
			m_pGridCreator.CrossValidateInRBF(NULL,iSampleCount,pSamplePointList,pPoint,pMQF.m_iSearchType,
												pMQF.m_iPointNumPreDirection,
												pMQF.m_iPointNumInTotal,
												pMQF.m_dSearchRadius,
												pMQF.m_iModelType,
												pMQF.m_dC[0],
												dMQFZ);

			//MLF
			double	dMLFZ	= -32767;
			m_pGridCreator.CrossValidateInRBF(NULL,iSampleCount,pSamplePointList,pPoint,
												pMLF.m_iSearchType,
												pMLF.m_iPointNumPreDirection,
												pMLF.m_iPointNumInTotal,
												pMLF.m_dSearchRadius,
												pMLF.m_iModelType,
												pMLF.m_dC[0],
												dMLFZ);

			//TPSF
			double	dTPSFZ	= -32767;
			m_pGridCreator.CrossValidateInRBF(NULL,iSampleCount,pSamplePointList,pPoint,
												pTPSF.m_iSearchType,
												pTPSF.m_iPointNumPreDirection,
												pTPSF.m_iPointNumInTotal,
												pTPSF.m_dSearchRadius,
												pTPSF.m_iModelType,
												pTPSF.m_dC[0],
												dTPSFZ);

			//NCSF
			double	dNCSFZ	= -32767;
			m_pGridCreator.CrossValidateInRBF(NULL,iSampleCount,pSamplePointList,pPoint,
												pNCSF.m_iSearchType,
												pNCSF.m_iPointNumPreDirection,
												pNCSF.m_iPointNumInTotal,
												pNCSF.m_dSearchRadius,
												pNCSF.m_iModelType,
												pNCSF.m_dC[0],
												dNCSFZ);
			
			//EXP
			double	dEXPZ	= -32767;
			m_pGridCreator.CrossValidateInKRG(NULL,iSampleCount,pSamplePointList,pPoint,
												pEXP.m_iSearchType,
												pEXP.m_iPointNumPreDirection,
												pEXP.m_iPointNumInTotal,
												pEXP.m_dSearchRadius,
												pEXP.m_iModelType,
												pEXP.m_dC0,
												pEXP.m_dC,
												pEXP.m_dAlfa,
												dEXPZ);

			//LINE
			double	dLINEZ	= -32767;
			m_pGridCreator.CrossValidateInKRG(NULL,iSampleCount,pSamplePointList,pPoint,
												pLINE.m_iSearchType,
												pLINE.m_iPointNumPreDirection,
												pLINE.m_iPointNumInTotal,
												pLINE.m_dSearchRadius,
												pLINE.m_iModelType,
												pLINE.m_dC0,
												pLINE.m_dC,
												pLINE.m_dAlfa,
												dLINEZ);

			double	dX,dY,dZ;

			dX	= pPoint->m_pPointCoord.x;
			dY	= pPoint->m_pPointCoord.y;
			dZ	= pPoint->m_pPointCoord.z;

			fprintf(fw,"%10.6lf %10.6lf %10.6lf %10.6lf %10.6lf %10.6lf %10.6lf %10.6lf %10.6lf %10.6lf %10.6lf %10.6lf %10.6lf\n",
				dX,dY,dZ,dMeanSlope,dPointPattern,
				dIDWZ,dSPDZ,dMQFZ,dMLFZ,dTPSFZ,dNCSFZ,dEXPZ,dLINEZ);
		}
		
		pPoint	= pPoint->m_pNext;
	}
	
	//�˳�MatCom
	exitM();	

	//
	fclose(fw);

	MessageBox(NULL,"ϵͳ�������!","��ʾ��Ϣ",MB_OK);
}

//�����ȡ���㣬����*.dat�ļ��洢
void CInterface_DemCreator::OnPickupCheckPoint()
{
	if(m_pWorkspace.IsEmpty())	
	{
		MessageBox(NULL,"���ȶ�������!","��ʾ��Ϣ",MB_OK);

		return	;
	}
	
	CSetCheckPointDlg	pDlg;
	if(pDlg.DoModal() == IDOK)
	{
		
	}
	else	return;

	//
	
	m_pWorkspace.PickupCheckPoint(pDlg.m_iPointNum);
}

void CInterface_DemCreator::SetNullStatus()
{
	//��ʼ��Ϊfalse
	m_pGridDem.SetShowStatus(false);
	m_pSlope.SetShowStatus(false);
	m_pAspect.SetShowStatus(false);
	m_pCurvature.SetShowStatus(false);
	m_pTerrainRoughness.SetShowStatus(false);
	m_pTerrainCV.SetShowStatus(false);
	m_pTerrainRelif.SetShowStatus(false);
	m_pTerrainIncision.SetShowStatus(false);
}
