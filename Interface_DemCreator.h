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
	
	//01 获取主窗口指针
	virtual	void	AttachMainWnd(CWnd *pWnd); 

	//02 创建地图DC
	virtual	void	CreateMapDC(CDC *pDC,int iSx,int iSy);
	//03 释放地图DC
	virtual	void	ReleaseMapDC();
	//04 获取地图DC
	virtual	CDC*	GetMapDC();

	//05 创建用户DC
	virtual	void	CreateUserDC(CDC *pDC,int iSx,int iSy);
	//06 释放用户DC
	virtual	void	ReleaseUserDC();
	//07 获取用户DC
	virtual	CDC*	GetUserDC();
	
	//08 设置/获取地图工具
	virtual	void	SetMapTool(int iMapTool);
	virtual	int		GetMapTool();

	//09 全图显示
	virtual	void	OnFull();

	//10 设置/获取地图区域
	virtual	void	SetMapExt(double dLeft,double dRight,double dBottom,double dTop);
	virtual	void	GetMapExt(double& dLeft,double& dRight,double& dBottom,double& dTop);

	//11 设置/获取显示区域
	virtual	void	SetShowExt(double dLeft,double dRight,double dBottom,double dTop);
	virtual	void	GetShowExt(double& dLeft,double& dRight,double& dBottom,double& dTop);
	
	//12 设置/获取上次显示区域
	virtual	void	SetOldShowExt(double dLeft,double dRight,double dBottom,double dTop);
	virtual	void	GetOldShowExt(double& dLeft,double& dRight,double& dBottom,double& dTop);
	
	//13 设置视口
	virtual	void	SetClientRect(CRect rRect);

	//14 加载数据,包括DEM、GRD、矢量数据等，通过后缀的不同区别
	virtual	bool	OnReadAsVector(CString szPathName);
	virtual	bool	OnReadAsGrid(CString szPathName);
	virtual	bool	OnReadLocalTerrain(CString szPathName);
	
	//15 判断文件是否加载成功
	virtual	bool	IsEmpty();

	//16 释放数据
	virtual	void	OnRelease();

	//17 保存数据
	virtual	bool	OnSave(CString szPathName);

	//18 绘制
	virtual	void	OnDraw(CDC *pDC);
	
	//19 绘制DEM到Map DC上
	virtual	void	DrawToDem();
	//20 绘制到User DC上
	virtual	void	DrawToUser();
	//21 绘制到Screen DC上
	virtual	void	DrawToScreen(CDC *pDC);

	//22 设置/获取内容显示状态
	virtual void	SetShowStatusInContent(bool bShowStatus);
	virtual bool	GetShowStatusInContent();
		
	//23 设置/获取格网显示状态
	virtual void	SetShowStatusInCell(bool bShowStatus);
	virtual bool	GetShowStatusInCell();

	//24 设置/获取DEM显示状态
	virtual void	SetShowStatusInDem(bool bShowStatus);
	virtual bool	GetShowStatusInDem();

	//25 设置/获取坡度显示状态
	virtual void	SetShowStatusInSlope(bool bShowStatus);
	virtual bool	GetShowStatusInSlope();

	//25 设置/获取坡度显示状态
	virtual void	SetShowStatusInAspect(bool bShowStatus);
	virtual bool	GetShowStatusInAspect();

	///26 设置/获取曲率显示状态
	virtual void	SetShowStatusInCurvature(bool bShowStatus);
	virtual bool	GetShowStatusInCurvature();

	//27 设置/获取表面粗糙度显示状态
	virtual void	SetShowStatusInRoughness(bool bShowStatus);
	virtual bool	GetShowStatusInRoughness();

	//28 设置/获取地形起伏度显示状态
	virtual void	SetShowStatusInRelif(bool bShowStatus);
	virtual bool	GetShowStatusInRelif();

	//28 设置/获取地形起伏度显示状态
	virtual void	SetShowStatusInGeoMorphJudge(bool bShowStatus);
	virtual bool	GetShowStatusInGeoMorphJudge();

	//29 鼠标移动，通过主视图类调用
	virtual	void	OnMouseMove(UINT nFlags, CPoint point);
	//30 鼠标左键弹起，通过主视图类调用
	virtual	void	OnLButtonUp(UINT nFlags, CPoint point);
	//31 鼠标左键按下，通过主视图类调用
	virtual	void	OnLButtonDown(UINT nFlags, CPoint point);
	//32 鼠标左键双击，通过主视图类调用
	virtual	void	OnLButtonDblClk(UINT nFlags, CPoint point);
	//33 鼠标左键弹起，通过主视图类调用
	virtual	void	OnRButtonUp(UINT nFlags, CPoint point);
	//34 鼠标左键按下，通过主视图类调用
	virtual	void	OnRButtonDown(UINT nFlags, CPoint point);
	//35 设置光标样式
	virtual	BOOL	OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);

	//36 响应OnSize函数
	virtual void	OnSize(UINT nType, int cx, int cy);

	//37 设置格网数目
	virtual	void	SetCellCount(int iCellNumX,int iCellNumY);
	virtual	void	SetCellCount();

	//38 分区格网创建
	virtual	void	CellBuild();
	
	//39 删除重复点
	virtual	void	DeleteRePoint();

	//40 设置/获取删除重复点中的点与点之间的最小间距
	virtual	void	SetMinDistance(double dMinDistance);
	virtual	double	GetMinDistance();

	//41 基于矢量数据的地形统计量
	virtual	void	OnTerrainDescriptiveStatisticsInVector();
	//42 基于格网数据的地形统计量
	virtual	void	OnTerrainDescriptiveStatisticsInGrd();

	//43 统计半变异函数,返回数据
	virtual	int		OnSemiVariogramStatistics(double **dX,double **dY);
	//44 统计半变异函数
	virtual	void	OnSemiVariogramStatisticsEx();

	//45 计算坡度
	virtual	void	OnCalculateSlope();
	//46 计算坡度，并计算坡度的统计信息
	virtual	void	OnSlope();

	//47 计算坡向
	virtual	void	OnCalculateAspect();
	//
	virtual	void	OnAspect();

	//48 计算剖面曲率
	virtual	void	OnCalculateProfileCurvature();
	//49 计算平面曲率
	virtual	void	OnCalculatePlanCurvature();
	//50 计算切线曲率
	virtual	void	OnCalculateTgCurvature();
	//51 计算曲率的统计信息
	virtual	void	OnCurvature(int iType);

	//52 计算地表粗糙度
	virtual	void	OnCalculateTerrainRoughness();
	//46 计算坡度，并计算坡度的统计信息
	virtual	void	OnTerrainRoughness();

	//53 计算地形起伏度
	virtual	void	OnCalculateTerrainRelif();
	//54 批处理计算地形起伏度
	virtual	void	OnCalculateTerrainRelifInBat();

	//55 计算高程变异系数
	virtual	void	OnCalculateTerrainCV();
	//56 批处理计算高程变异系数
	virtual	void	OnCalculateTerrainCVInBat();

	//57 计算表面积
	virtual	void	OnCalculateSurface();

	//58 计算分形维数
	virtual	void	OnCalculateFractal();

	//59 批处理地形特征因子信息
	virtual	void	OnCalculateTerrainFactorInBat();

	//59-1 导出信息因子信息,以GRD表示
	virtual	void	OnExportTerrainFactor();

	//60 计算残差
	virtual	void	OnCalculateRemainsError();

	//61 创建TIN
	virtual	void	OnTinCreator(CDC *pDC,int iType = 0);
	//62 创建GRID
	virtual	void	OnGridCreator(CDC *pDC,int iType = 0);	

	//63 IDW批处理创建
	virtual	void	OnCreateInIDWInBat(CDC *pDC);
	//64 RBF批处理创建
	virtual	void	OnCreateInRBFInBat(CDC *pDC);
	//65 KRG批处理创建
	virtual	void	OnCreateInKRGInBat(CDC *pDC);
	//66 SPD批处理创建
	virtual	void	OnCreateInSPDInBat(CDC *pDC);

	//67 IDW交叉验证
	virtual	void	OnCrossValidateInIDWInBat(CDC *pDC);
	//68 RBF交叉验证
	virtual	void	OnCrossValidateInRBFInBat(CDC *pDC);
	//69 KRG交叉验证
	virtual	void	OnCrossValidateInKRGInBat(CDC *pDC);
	//70 SPD交叉验证
	virtual	void	OnCrossValidateInSPDInBat(CDC *pDC);

	//计算交叉验证情况下的局部地形特征
	virtual	void	OnCalculateLocalTerrainCharacterInBat();

	//71 插值，即根据指定的xy坐标，计算相应的Z值，并不单指高程，并计算相应的统计信息
	virtual	void	OnInterpolationValue();

	//72 转换EGC数据为DEM数据
	virtual	bool	OnConversionEGCToGRD(CString szPathName);
	virtual	bool	OnConversionMATToGRD(CString szPathName);
	virtual	bool	OnConversionDVFToBLN(CString szPathName);

	//73 地形抹平，即把高程小于零的归算到零
	virtual	bool	OnTerrainToZero();

	//74 计算Ripley K函数
	virtual	void	OnCalcRipleyK();

	//75 提取数据
	virtual	void	OnPickUpGrid(CString szPathName);

	//76 提取数据
	virtual	void	OnWavelets();

	//
	virtual	void	OnLocalTerrainCharacter();

	//
	virtual	void	OnCalcSubjectValue();

	//将残差结果和局部地形特征进行组合，以用于空间聚类分析
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
	//百分比分析
	virtual	void	OnPercentAnalyse();
	//残差百分比分析批处理
	virtual	void	OnPercentAnalyseCombat();

	//根据隶属度矩阵，计算离散点的隶属度值
	virtual	void	OnCalcLiShuDuValue();

	//在“最优”插值参数时计算残差值，并相应的局部地形特征（以D2P64为准），这样可以省略很多的工作。
	virtual	void	OnCalcLTCandDeltaH();

	//随机提取检查点，并以*.dat文件存储
	virtual	void	OnPickupCheckPoint();

	//99 回调函数
	virtual	void	OnCallBackFunc_Process(void *pFunc);
protected:
	//101 显示参数计算
	bool	InitShow(CRect rRect, double dLeft, double dRight, double dBottom, double dTop);
	//102 屏幕坐标和地图坐标转换
	void	ScreenToMap(int cx, int cy, double &xx, double &yy);
	void	MapToScreen(double xx, double yy,int& cx, int& cy);

protected:
	void SetNullStatus();
	//小波分析类
	CDCR_Wavelets			m_pWavelets;

	//坡度类
	CDCR_Slope				m_pSlope;

	//坡度类
	CDCR_Aspect				m_pAspect;

	//曲率类
	CDCR_Curvature			m_pCurvature;

	//地形粗糙度类
	CDCR_TerrainRoughness	m_pTerrainRoughness;

	//地形粗糙度类
	CDCR_TerrainCV			m_pTerrainCV;

	//地形起伏度类
	CDCR_TerrainRelif		m_pTerrainRelif;
	//地形切割深度类
	CDCR_TerrainIncision	m_pTerrainIncision;

	//六角格编辑对话框
	CSetLocalTCDlg			*m_pLocalTCDlg;
	CSetSearchPointDlg		*m_pSearchPointDlg;

	//地貌类型判别
	CDCR_GeoMorphological	m_pGeoMorphological;
	//
	bool					m_bGeoMorphShowStatus;


	//消息读取回调函数
	ONPROCESS			m_OnProcess;	

	//原始数据
	CDCR_WorkSpace		m_pWorkspace;
	
	//半变异函数类
	CDCR_SemiVariogram	m_pSemiVariogram;

	//
	CDCR_PointPattern_KFunction	m_pPointPatternK;

	//
	CDCR_Cluster_Analysis		m_pClusterAnalysis;

	//地形特征数据，经过整理后的
	CDCR_LocalTerrain			m_pLocalTerrain;
	CString						m_szLocalTerrainPathName;

	//DEM类
	CDCR_GridDem		m_pGridDem;

	//TIN创建
	CDCR_TinCreator		m_pTinCreator;

	//GRID创建
	CDCR_GridCreator	m_pGridCreator;

	//主要是指DVF、DEM文件的目录
	CString				m_szPathName;

	//DVF所在的目录，仅仅是指目录而已
	CString				m_szBasePathName;

	//系统路径
	CString				m_szExePathName;

	//视口尺寸
	CRect				m_rRect;

	//显示区域
	double				m_dShowLeft,m_dShowRight;
	double				m_dShowBottom,m_dShowTop;

	double				m_dOldShowLeft,m_dOldShowRight;
	double				m_dOldShowBottom,m_dOldShowTop;

	//显示参数
	double				m_dMapXA,m_dMapXB;
	double				m_dMapYA,m_dMapYB;

	//视口大小
	int					m_nFullWidth, m_nFullHeight;

	//
	CDC*				m_pDC;

	//地图内存位图
	CBitmap*			m_pMapBitmap;	
	//地图内存DC	
	CDC*				m_pMapDC;	

	//
	//用户内存位图
	CBitmap*			m_pUserBitmap;	
	//地图内存DC	
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
