// Interface_DemCreator.h: interface for the CInterface_DemCreator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INTERFACE_DEMCREATOR_H__4F6ED25D_756F_4317_BE51_A8D0FEBACA7C__INCLUDED_)
#define AFX_INTERFACE_DEMCREATOR_H__4F6ED25D_756F_4317_BE51_A8D0FEBACA7C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IInterface_DemCreator.h"
#include "DCR_WorkSpace.h"
#include "DCR_SemiVariogram.h"
#include "DCR_GridDem.h"
#include "DCR_TinCreator.h"
#include "DCR_GridCreator.h"
#include "DCR_PointPattern_KFunction.h"

#include "DCR_Wavelets.h"
#include "DCR_Slope.h"
#include "DCR_Aspect.h"
#include "DCR_Curvature.h"
#include "DCR_TerrainRoughness.h"
#include "DCR_TerrainCV.h"
#include "DCR_TerrainRelif.h"
#include "DCR_TerrainIncision.h"
#include "DCR_Cluster_Analysis.h"
#include "DCR_LocalTerrain.h"

#include "SetLocalTCDlg.h"
#include "SetSearchPointDlg.h"

#include "DCR_GeoMorphological.h"

class CInterface_DemCreator : public IInterface_DemCreator  
{
public:
	CInterface_DemCreator();
	virtual ~CInterface_DemCreator();
	
	//01 ��ȡ������ָ��
	virtual	void	AttachMainWnd(CWnd *pWnd); 

	//02 ������ͼDC
	virtual	void	CreateMapDC(CDC *pDC,int iSx,int iSy);
	//03 �ͷŵ�ͼDC
	virtual	void	ReleaseMapDC();
	//04 ��ȡ��ͼDC
	virtual	CDC*	GetMapDC();

	//05 �����û�DC
	virtual	void	CreateUserDC(CDC *pDC,int iSx,int iSy);
	//06 �ͷ��û�DC
	virtual	void	ReleaseUserDC();
	//07 ��ȡ�û�DC
	virtual	CDC*	GetUserDC();
	
	//08 ����/��ȡ��ͼ����
	virtual	void	SetMapTool(int iMapTool);
	virtual	int		GetMapTool();

	//09 ȫͼ��ʾ
	virtual	void	OnFull();

	//10 ����/��ȡ��ͼ����
	virtual	void	SetMapExt(double dLeft,double dRight,double dBottom,double dTop);
	virtual	void	GetMapExt(double& dLeft,double& dRight,double& dBottom,double& dTop);

	//11 ����/��ȡ��ʾ����
	virtual	void	SetShowExt(double dLeft,double dRight,double dBottom,double dTop);
	virtual	void	GetShowExt(double& dLeft,double& dRight,double& dBottom,double& dTop);
	
	//12 ����/��ȡ�ϴ���ʾ����
	virtual	void	SetOldShowExt(double dLeft,double dRight,double dBottom,double dTop);
	virtual	void	GetOldShowExt(double& dLeft,double& dRight,double& dBottom,double& dTop);
	
	//13 �����ӿ�
	virtual	void	SetClientRect(CRect rRect);

	//14 ��������,����DEM��GRD��ʸ�����ݵȣ�ͨ����׺�Ĳ�ͬ����
	virtual	bool	OnReadAsVector(CString szPathName);
	virtual	bool	OnReadAsGrid(CString szPathName);
	virtual	bool	OnReadLocalTerrain(CString szPathName);
	
	//15 �ж��ļ��Ƿ���سɹ�
	virtual	bool	IsEmpty();

	//16 �ͷ�����
	virtual	void	OnRelease();

	//17 ��������
	virtual	bool	OnSave(CString szPathName);

	//18 ����
	virtual	void	OnDraw(CDC *pDC);
	
	//19 ����DEM��Map DC��
	virtual	void	DrawToDem();
	//20 ���Ƶ�User DC��
	virtual	void	DrawToUser();
	//21 ���Ƶ�Screen DC��
	virtual	void	DrawToScreen(CDC *pDC);

	//22 ����/��ȡ������ʾ״̬
	virtual void	SetShowStatusInContent(bool bShowStatus);
	virtual bool	GetShowStatusInContent();
		
	//23 ����/��ȡ������ʾ״̬
	virtual void	SetShowStatusInCell(bool bShowStatus);
	virtual bool	GetShowStatusInCell();

	//24 ����/��ȡDEM��ʾ״̬
	virtual void	SetShowStatusInDem(bool bShowStatus);
	virtual bool	GetShowStatusInDem();

	//25 ����/��ȡ�¶���ʾ״̬
	virtual void	SetShowStatusInSlope(bool bShowStatus);
	virtual bool	GetShowStatusInSlope();

	//25 ����/��ȡ�¶���ʾ״̬
	virtual void	SetShowStatusInAspect(bool bShowStatus);
	virtual bool	GetShowStatusInAspect();

	///26 ����/��ȡ������ʾ״̬
	virtual void	SetShowStatusInCurvature(bool bShowStatus);
	virtual bool	GetShowStatusInCurvature();

	//27 ����/��ȡ����ֲڶ���ʾ״̬
	virtual void	SetShowStatusInRoughness(bool bShowStatus);
	virtual bool	GetShowStatusInRoughness();

	//28 ����/��ȡ�����������ʾ״̬
	virtual void	SetShowStatusInRelif(bool bShowStatus);
	virtual bool	GetShowStatusInRelif();

	//28 ����/��ȡ�����������ʾ״̬
	virtual void	SetShowStatusInGeoMorphJudge(bool bShowStatus);
	virtual bool	GetShowStatusInGeoMorphJudge();

	//29 ����ƶ���ͨ������ͼ�����
	virtual	void	OnMouseMove(UINT nFlags, CPoint point);
	//30 ����������ͨ������ͼ�����
	virtual	void	OnLButtonUp(UINT nFlags, CPoint point);
	//31 ���������£�ͨ������ͼ�����
	virtual	void	OnLButtonDown(UINT nFlags, CPoint point);
	//32 ������˫����ͨ������ͼ�����
	virtual	void	OnLButtonDblClk(UINT nFlags, CPoint point);
	//33 ����������ͨ������ͼ�����
	virtual	void	OnRButtonUp(UINT nFlags, CPoint point);
	//34 ���������£�ͨ������ͼ�����
	virtual	void	OnRButtonDown(UINT nFlags, CPoint point);
	//35 ���ù����ʽ
	virtual	BOOL	OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);

	//36 ��ӦOnSize����
	virtual void	OnSize(UINT nType, int cx, int cy);

	//37 ���ø�����Ŀ
	virtual	void	SetCellCount(int iCellNumX,int iCellNumY);
	virtual	void	SetCellCount();

	//38 ������������
	virtual	void	CellBuild();
	
	//39 ɾ���ظ���
	virtual	void	DeleteRePoint();

	//40 ����/��ȡɾ���ظ����еĵ����֮�����С���
	virtual	void	SetMinDistance(double dMinDistance);
	virtual	double	GetMinDistance();

	//41 ����ʸ�����ݵĵ���ͳ����
	virtual	void	OnTerrainDescriptiveStatisticsInVector();
	//42 ���ڸ������ݵĵ���ͳ����
	virtual	void	OnTerrainDescriptiveStatisticsInGrd();

	//43 ͳ�ư���캯��,��������
	virtual	int		OnSemiVariogramStatistics(double **dX,double **dY);
	//44 ͳ�ư���캯��
	virtual	void	OnSemiVariogramStatisticsEx();

	//45 �����¶�
	virtual	void	OnCalculateSlope();
	//46 �����¶ȣ��������¶ȵ�ͳ����Ϣ
	virtual	void	OnSlope();

	//47 ��������
	virtual	void	OnCalculateAspect();
	//
	virtual	void	OnAspect();

	//48 ������������
	virtual	void	OnCalculateProfileCurvature();
	//49 ����ƽ������
	virtual	void	OnCalculatePlanCurvature();
	//50 ������������
	virtual	void	OnCalculateTgCurvature();
	//51 �������ʵ�ͳ����Ϣ
	virtual	void	OnCurvature(int iType);

	//52 ����ر�ֲڶ�
	virtual	void	OnCalculateTerrainRoughness();
	//46 �����¶ȣ��������¶ȵ�ͳ����Ϣ
	virtual	void	OnTerrainRoughness();

	//53 ������������
	virtual	void	OnCalculateTerrainRelif();
	//54 �����������������
	virtual	void	OnCalculateTerrainRelifInBat();

	//55 ����̱߳���ϵ��
	virtual	void	OnCalculateTerrainCV();
	//56 ���������̱߳���ϵ��
	virtual	void	OnCalculateTerrainCVInBat();

	//57 ��������
	virtual	void	OnCalculateSurface();

	//58 �������ά��
	virtual	void	OnCalculateFractal();

	//59 �������������������Ϣ
	virtual	void	OnCalculateTerrainFactorInBat();

	//59-1 ������Ϣ������Ϣ,��GRD��ʾ
	virtual	void	OnExportTerrainFactor();

	//60 ����в�
	virtual	void	OnCalculateRemainsError();

	//61 ����TIN
	virtual	void	OnTinCreator(CDC *pDC,int iType = 0);
	//62 ����GRID
	virtual	void	OnGridCreator(CDC *pDC,int iType = 0);	

	//63 IDW��������
	virtual	void	OnCreateInIDWInBat(CDC *pDC);
	//64 RBF��������
	virtual	void	OnCreateInRBFInBat(CDC *pDC);
	//65 KRG��������
	virtual	void	OnCreateInKRGInBat(CDC *pDC);
	//66 SPD��������
	virtual	void	OnCreateInSPDInBat(CDC *pDC);

	//67 IDW������֤
	virtual	void	OnCrossValidateInIDWInBat(CDC *pDC);
	//68 RBF������֤
	virtual	void	OnCrossValidateInRBFInBat(CDC *pDC);
	//69 KRG������֤
	virtual	void	OnCrossValidateInKRGInBat(CDC *pDC);
	//70 SPD������֤
	virtual	void	OnCrossValidateInSPDInBat(CDC *pDC);

	//���㽻����֤����µľֲ���������
	virtual	void	OnCalculateLocalTerrainCharacterInBat();

	//71 ��ֵ��������ָ����xy���꣬������Ӧ��Zֵ��������ָ�̣߳���������Ӧ��ͳ����Ϣ
	virtual	void	OnInterpolationValue();

	//72 ת��EGC����ΪDEM����
	virtual	bool	OnConversionEGCToGRD(CString szPathName);
	virtual	bool	OnConversionMATToGRD(CString szPathName);
	virtual	bool	OnConversionDVFToBLN(CString szPathName);

	//73 ����Ĩƽ�����Ѹ߳�С����Ĺ��㵽��
	virtual	bool	OnTerrainToZero();

	//74 ����Ripley K����
	virtual	void	OnCalcRipleyK();

	//75 ��ȡ����
	virtual	void	OnPickUpGrid(CString szPathName);

	//76 ��ȡ����
	virtual	void	OnWavelets();

	//
	virtual	void	OnLocalTerrainCharacter();

	//
	virtual	void	OnCalcSubjectValue();

	//���в����;ֲ���������������ϣ������ڿռ�������
	virtual	void	OnCombineREAndLocalTerrainCharacter();

	//
	virtual	void	OnClusterAnalysis(CDC *pDC);

	//
	virtual	void	OnDimensionAnalyse();

	//
	virtual	void	OnSearchCurrentPoint();
	//
	virtual	bool	OnSearchCurrentPoint(double dX,double dY);

	//
	virtual	void	SwitchNewRegion(CRect rRect, bool bSame=false);

	//
	virtual	void	OnJudgeGeoMorphological(int iType);
	//�ٷֱȷ���
	virtual	void	OnPercentAnalyse();
	//�в�ٷֱȷ���������
	virtual	void	OnPercentAnalyseCombat();

	//���������Ⱦ��󣬼�����ɢ���������ֵ
	virtual	void	OnCalcLiShuDuValue();

	//�ڡ����š���ֵ����ʱ����в�ֵ������Ӧ�ľֲ�������������D2P64Ϊ׼������������ʡ�Ժܶ�Ĺ�����
	virtual	void	OnCalcLTCandDeltaH();

	//�����ȡ���㣬����*.dat�ļ��洢
	virtual	void	OnPickupCheckPoint();

	//99 �ص�����
	virtual	void	OnCallBackFunc_Process(void *pFunc);
protected:
	//101 ��ʾ��������
	bool	InitShow(CRect rRect, double dLeft, double dRight, double dBottom, double dTop);
	//102 ��Ļ����͵�ͼ����ת��
	void	ScreenToMap(int cx, int cy, double &xx, double &yy);
	void	MapToScreen(double xx, double yy,int& cx, int& cy);

protected:
	void SetNullStatus();
	//С��������
	CDCR_Wavelets			m_pWavelets;

	//�¶���
	CDCR_Slope				m_pSlope;

	//�¶���
	CDCR_Aspect				m_pAspect;

	//������
	CDCR_Curvature			m_pCurvature;

	//���δֲڶ���
	CDCR_TerrainRoughness	m_pTerrainRoughness;

	//���δֲڶ���
	CDCR_TerrainCV			m_pTerrainCV;

	//�����������
	CDCR_TerrainRelif		m_pTerrainRelif;
	//�����и������
	CDCR_TerrainIncision	m_pTerrainIncision;

	//���Ǹ�༭�Ի���
	CSetLocalTCDlg			*m_pLocalTCDlg;
	CSetSearchPointDlg		*m_pSearchPointDlg;

	//��ò�����б�
	CDCR_GeoMorphological	m_pGeoMorphological;
	//
	bool					m_bGeoMorphShowStatus;


	//��Ϣ��ȡ�ص�����
	ONPROCESS			m_OnProcess;	

	//ԭʼ����
	CDCR_WorkSpace		m_pWorkspace;
	
	//����캯����
	CDCR_SemiVariogram	m_pSemiVariogram;

	//
	CDCR_PointPattern_KFunction	m_pPointPatternK;

	//
	CDCR_Cluster_Analysis		m_pClusterAnalysis;

	//�����������ݣ�����������
	CDCR_LocalTerrain			m_pLocalTerrain;
	CString						m_szLocalTerrainPathName;

	//DEM��
	CDCR_GridDem		m_pGridDem;

	//TIN����
	CDCR_TinCreator		m_pTinCreator;

	//GRID����
	CDCR_GridCreator	m_pGridCreator;

	//��Ҫ��ָDVF��DEM�ļ���Ŀ¼
	CString				m_szPathName;

	//DVF���ڵ�Ŀ¼��������ָĿ¼����
	CString				m_szBasePathName;

	//ϵͳ·��
	CString				m_szExePathName;

	//�ӿڳߴ�
	CRect				m_rRect;

	//��ʾ����
	double				m_dShowLeft,m_dShowRight;
	double				m_dShowBottom,m_dShowTop;

	double				m_dOldShowLeft,m_dOldShowRight;
	double				m_dOldShowBottom,m_dOldShowTop;

	//��ʾ����
	double				m_dMapXA,m_dMapXB;
	double				m_dMapYA,m_dMapYB;

	//�ӿڴ�С
	int					m_nFullWidth, m_nFullHeight;

	//
	CDC*				m_pDC;

	//��ͼ�ڴ�λͼ
	CBitmap*			m_pMapBitmap;	
	//��ͼ�ڴ�DC	
	CDC*				m_pMapDC;	

	//
	//�û��ڴ�λͼ
	CBitmap*			m_pUserBitmap;	
	//��ͼ�ڴ�DC	
	CDC*				m_pUserDC;	

	//
	CPoint				m_ptDown;
	CPoint				m_ptUp;

	//
	int					m_iMapTool;

	//
	CWnd				*m_pMainWnd;
};

#endif // !defined(AFX_INTERFACE_DEMCREATOR_H__4F6ED25D_756F_4317_BE51_A8D0FEBACA7C__INCLUDED_)
