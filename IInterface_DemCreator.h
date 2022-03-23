// IInterface_DemCreator.h: interface for the IInterface_DemCreator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IINTERFACE_DEMCREATOR_H__ABE0EEFD_43F1_45CF_9BCA_8E1DB3387214__INCLUDED_)
#define AFX_IINTERFACE_DEMCREATOR_H__ABE0EEFD_43F1_45CF_9BCA_8E1DB3387214__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class IInterface_DemCreator  
{
public:
	//01 ��ȡ������ָ��
	virtual	void	AttachMainWnd(CWnd *pWnd) = 0; 

	//02 ������ͼDC
	virtual	void	CreateMapDC(CDC *pDC,int iSx,int iSy) = 0;
	//03 �ͷŵ�ͼDC
	virtual	void	ReleaseMapDC() = 0;
	//04 ��ȡ��ͼDC
	virtual	CDC*	GetMapDC() = 0;

	//05 �����û�DC
	virtual	void	CreateUserDC(CDC *pDC,int iSx,int iSy) = 0;
	//06 �ͷ��û�DC
	virtual	void	ReleaseUserDC() = 0;
	//07 ��ȡ�û�DC
	virtual	CDC*	GetUserDC() = 0;
	
	//08 ����/��ȡ��ͼ����
	virtual	void	SetMapTool(int iMapTool) = 0;
	virtual	int		GetMapTool() = 0;

	//09 ȫͼ��ʾ
	virtual	void	OnFull() = 0;

	//10 ����/��ȡ��ͼ����
	virtual	void	SetMapExt(double dLeft,double dRight,double dBottom,double dTop) = 0;
	virtual	void	GetMapExt(double& dLeft,double& dRight,double& dBottom,double& dTop) = 0;

	//11 ����/��ȡ��ʾ����
	virtual	void	SetShowExt(double dLeft,double dRight,double dBottom,double dTop) = 0;
	virtual	void	GetShowExt(double& dLeft,double& dRight,double& dBottom,double& dTop) = 0;
	
	//12 ����/��ȡ�ϴ���ʾ����
	virtual	void	SetOldShowExt(double dLeft,double dRight,double dBottom,double dTop) = 0;
	virtual	void	GetOldShowExt(double& dLeft,double& dRight,double& dBottom,double& dTop) = 0;
	
	//13 �����ӿ�
	virtual	void	SetClientRect(CRect rRect) = 0;

	//14 ��������,����DEM��GRD��ʸ�����ݵȣ�ͨ����׺�Ĳ�ͬ����
	virtual	bool	OnReadAsVector(CString szPathName) = 0;
	virtual	bool	OnReadAsGrid(CString szPathName) = 0;
	virtual	bool	OnReadLocalTerrain(CString szPathName) = 0;
	
	//15 �ж��ļ��Ƿ���سɹ�
	virtual	bool	IsEmpty() = 0;

	//16 �ͷ�����
	virtual	void	OnRelease() = 0;

	//17 ��������
	virtual	bool	OnSave(CString szPathName) = 0;

	//18 ����
	virtual	void	OnDraw(CDC *pDC) = 0;
	
	//19 ����DEM��Map DC��
	virtual	void	DrawToDem() = 0;
	//20 ���Ƶ�User DC��
	virtual	void	DrawToUser() = 0;
	//21 ���Ƶ�Screen DC��
	virtual	void	DrawToScreen(CDC *pDC) = 0;

	//22 ����/��ȡ������ʾ״̬
	virtual void	SetShowStatusInContent(bool bShowStatus) = 0;
	virtual bool	GetShowStatusInContent() = 0;
		
	//23 ����/��ȡ������ʾ״̬
	virtual void	SetShowStatusInCell(bool bShowStatus) = 0;
	virtual bool	GetShowStatusInCell() = 0;

	//24 ����/��ȡDEM��ʾ״̬
	virtual void	SetShowStatusInDem(bool bShowStatus) = 0;
	virtual bool	GetShowStatusInDem() = 0;

	//25 ����/��ȡ�¶���ʾ״̬
	virtual void	SetShowStatusInSlope(bool bShowStatus) = 0;
	virtual bool	GetShowStatusInSlope() = 0;
	//25 ����/��ȡ�¶���ʾ״̬
	virtual void	SetShowStatusInAspect(bool bShowStatus) = 0;
	virtual bool	GetShowStatusInAspect() = 0;

	///26 ����/��ȡ������ʾ״̬
	virtual void	SetShowStatusInCurvature(bool bShowStatus) = 0;
	virtual bool	GetShowStatusInCurvature() = 0;

	//27 ����/��ȡ����ֲڶ���ʾ״̬
	virtual void	SetShowStatusInRoughness(bool bShowStatus) = 0;
	virtual bool	GetShowStatusInRoughness() = 0;

	//28 ����/��ȡ�����������ʾ״̬
	virtual void	SetShowStatusInRelif(bool bShowStatus) = 0;
	virtual bool	GetShowStatusInRelif() = 0;

	//28 ����/��ȡ�����������ʾ״̬
	virtual void	SetShowStatusInGeoMorphJudge(bool bShowStatus) = 0;
	virtual bool	GetShowStatusInGeoMorphJudge() = 0;

	//29 ����ƶ���ͨ������ͼ�����
	virtual	void	OnMouseMove(UINT nFlags, CPoint point) = 0;
	//30 ����������ͨ������ͼ�����
	virtual	void	OnLButtonUp(UINT nFlags, CPoint point) = 0;
	//31 ���������£�ͨ������ͼ�����
	virtual	void	OnLButtonDown(UINT nFlags, CPoint point) = 0;
	//32 ������˫����ͨ������ͼ�����
	virtual	void	OnLButtonDblClk(UINT nFlags, CPoint point) = 0;
	//33 ����������ͨ������ͼ�����
	virtual	void	OnRButtonUp(UINT nFlags, CPoint point) = 0;
	//34 ���������£�ͨ������ͼ�����
	virtual	void	OnRButtonDown(UINT nFlags, CPoint point) = 0;
	//35 ���ù����ʽ
	virtual	BOOL	OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) = 0;

	//36 ��ӦOnSize����
	virtual void	OnSize(UINT nType, int cx, int cy) = 0;

	//37 ���ø�����Ŀ
	virtual	void	SetCellCount(int iCellNumX,int iCellNumY) = 0;
	virtual	void	SetCellCount() = 0;

	//38 ������������
	virtual	void	CellBuild() = 0;
	
	//39 ɾ���ظ���
	virtual	void	DeleteRePoint() = 0;

	//40 ����/��ȡɾ���ظ����еĵ����֮�����С���
	virtual	void	SetMinDistance(double dMinDistance) = 0;
	virtual	double	GetMinDistance() = 0;

	//41 ����ʸ�����ݵĵ���ͳ����
	virtual	void	OnTerrainDescriptiveStatisticsInVector() = 0;
	//42 ���ڸ������ݵĵ���ͳ����
	virtual	void	OnTerrainDescriptiveStatisticsInGrd() = 0;

	//43 ͳ�ư���캯��,��������
	virtual	int		OnSemiVariogramStatistics(double **dX,double **dY) = 0;
	//44 ͳ�ư���캯��
	virtual	void	OnSemiVariogramStatisticsEx() = 0;

	//45 �����¶�
	virtual	void	OnCalculateSlope() = 0;
	//46 �����¶ȣ��������¶ȵ�ͳ����Ϣ
	virtual	void	OnSlope() = 0;

	//47 ��������
	virtual	void	OnCalculateAspect() = 0;
	//
	virtual	void	OnAspect() = 0;

	//48 ������������
	virtual	void	OnCalculateProfileCurvature() = 0;
	//49 ����ƽ������
	virtual	void	OnCalculatePlanCurvature() = 0;
	//50 ������������
	virtual	void	OnCalculateTgCurvature() = 0;
	//51 �������ʵ�ͳ����Ϣ
	virtual	void	OnCurvature(int iType) = 0;

	//52 ����ر�ֲڶ�
	virtual	void	OnCalculateTerrainRoughness() = 0;
	//46 �����¶ȣ��������¶ȵ�ͳ����Ϣ
	virtual	void	OnTerrainRoughness() = 0;

	//53 ������������
	virtual	void	OnCalculateTerrainRelif() = 0;
	//54 �����������������
	virtual	void	OnCalculateTerrainRelifInBat() = 0;

	//55 ����̱߳���ϵ��
	virtual	void	OnCalculateTerrainCV() = 0;
	//56 ���������̱߳���ϵ��
	virtual	void	OnCalculateTerrainCVInBat() = 0;

	//57 ��������
	virtual	void	OnCalculateSurface() = 0;

	//58 �������ά��
	virtual	void	OnCalculateFractal() = 0;

	//59 �������������������Ϣ
	virtual	void	OnCalculateTerrainFactorInBat() = 0;

	//59-1 ������Ϣ������Ϣ,��GRD��ʾ
	virtual	void	OnExportTerrainFactor() = 0;

	//60 ����в�
	virtual	void	OnCalculateRemainsError() = 0;

	//61 ����TIN
	virtual	void	OnTinCreator(CDC *pDC,int iType = 0) = 0;
	//62 ����GRID
	virtual	void	OnGridCreator(CDC *pDC,int iType = 0) = 0;	

	//63 IDW��������
	virtual	void	OnCreateInIDWInBat(CDC *pDC) = 0;
	//64 RBF��������
	virtual	void	OnCreateInRBFInBat(CDC *pDC) = 0;
	//65 KRG��������
	virtual	void	OnCreateInKRGInBat(CDC *pDC) = 0;
	//66 SPD��������
	virtual	void	OnCreateInSPDInBat(CDC *pDC) = 0;

	//67 IDW������֤
	virtual	void	OnCrossValidateInIDWInBat(CDC *pDC) = 0;
	//68 RBF������֤
	virtual	void	OnCrossValidateInRBFInBat(CDC *pDC) = 0;
	//69 KRG������֤
	virtual	void	OnCrossValidateInKRGInBat(CDC *pDC) = 0;
	//70 SPD������֤
	virtual	void	OnCrossValidateInSPDInBat(CDC *pDC) = 0;

	//���㽻����֤����µľֲ���������
	virtual	void	OnCalculateLocalTerrainCharacterInBat() = 0;

	//71 ��ֵ��������ָ����xy���꣬������Ӧ��Zֵ��������ָ�̣߳���������Ӧ��ͳ����Ϣ
	virtual	void	OnInterpolationValue() = 0;

	//72 ת��EGC����ΪDEM����
	virtual	bool	OnConversionEGCToGRD(CString szPathName) = 0;
	virtual	bool	OnConversionMATToGRD(CString szPathName) = 0;
	virtual	bool	OnConversionDVFToBLN(CString szPathName) = 0;

	//73 ����Ĩƽ�����Ѹ߳�С����Ĺ��㵽��
	virtual	bool	OnTerrainToZero() = 0;
	//74 ����Ripley K����
	virtual	void	OnCalcRipleyK()=0;

	//75 ��ȡ����
	virtual	void	OnPickUpGrid(CString szPathName) = 0;

	//76 ��ȡ����
	virtual	void	OnWavelets() = 0;

	//
	virtual	void	OnLocalTerrainCharacter() = 0;

	//
	virtual	void	OnCalcSubjectValue() = 0;

	//���в����;ֲ���������������ϣ������ڿռ�������
	virtual	void	OnCombineREAndLocalTerrainCharacter() = 0;

	//�ռ�������
	virtual	void	OnClusterAnalysis(CDC *pDC) = 0;

	//
	virtual	void	OnDimensionAnalyse() = 0;

	//
	virtual	void	OnSearchCurrentPoint() = 0;
	//
	virtual	bool	OnSearchCurrentPoint(double dX,double dY) = 0;

	//
	virtual	void	SwitchNewRegion(CRect rRect, bool bSame=false) = 0;

	//
	virtual	void	OnJudgeGeoMorphological(int iType) = 0;

	//�ٷֱȷ���
	virtual	void	OnPercentAnalyse() = 0;
	//�в�ٷֱȷ���������
	virtual	void	OnPercentAnalyseCombat() = 0;

	//���������Ⱦ��󣬼�����ɢ���������ֵ
	virtual	void	OnCalcLiShuDuValue() = 0;

	//�ڡ����š���ֵ����ʱ����в�ֵ������Ӧ�ľֲ�������������D2P64Ϊ׼������������ʡ�Ժܶ�Ĺ�����
	virtual	void	OnCalcLTCandDeltaH() = 0;

	//�����ȡ���㣬����*.dat�ļ��洢
	virtual	void	OnPickupCheckPoint() = 0;

	//99 �ص�����
	virtual	void	OnCallBackFunc_Process(void *pFunc) = 0;
};

_declspec(dllexport) IInterface_DemCreator* CreateDemCreator();
_declspec(dllexport) void ReleaseDemCreator(IInterface_DemCreator* pIDemCreator);

#endif // !defined(AFX_IINTERFACE_DEMCREATOR_H__ABE0EEFD_43F1_45CF_9BCA_8E1DB3387214__INCLUDED_)
