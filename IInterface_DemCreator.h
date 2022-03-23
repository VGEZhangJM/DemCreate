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
	//01 获取主窗口指针
	virtual	void	AttachMainWnd(CWnd *pWnd) = 0; 

	//02 创建地图DC
	virtual	void	CreateMapDC(CDC *pDC,int iSx,int iSy) = 0;
	//03 释放地图DC
	virtual	void	ReleaseMapDC() = 0;
	//04 获取地图DC
	virtual	CDC*	GetMapDC() = 0;

	//05 创建用户DC
	virtual	void	CreateUserDC(CDC *pDC,int iSx,int iSy) = 0;
	//06 释放用户DC
	virtual	void	ReleaseUserDC() = 0;
	//07 获取用户DC
	virtual	CDC*	GetUserDC() = 0;
	
	//08 设置/获取地图工具
	virtual	void	SetMapTool(int iMapTool) = 0;
	virtual	int		GetMapTool() = 0;

	//09 全图显示
	virtual	void	OnFull() = 0;

	//10 设置/获取地图区域
	virtual	void	SetMapExt(double dLeft,double dRight,double dBottom,double dTop) = 0;
	virtual	void	GetMapExt(double& dLeft,double& dRight,double& dBottom,double& dTop) = 0;

	//11 设置/获取显示区域
	virtual	void	SetShowExt(double dLeft,double dRight,double dBottom,double dTop) = 0;
	virtual	void	GetShowExt(double& dLeft,double& dRight,double& dBottom,double& dTop) = 0;
	
	//12 设置/获取上次显示区域
	virtual	void	SetOldShowExt(double dLeft,double dRight,double dBottom,double dTop) = 0;
	virtual	void	GetOldShowExt(double& dLeft,double& dRight,double& dBottom,double& dTop) = 0;
	
	//13 设置视口
	virtual	void	SetClientRect(CRect rRect) = 0;

	//14 加载数据,包括DEM、GRD、矢量数据等，通过后缀的不同区别
	virtual	bool	OnReadAsVector(CString szPathName) = 0;
	virtual	bool	OnReadAsGrid(CString szPathName) = 0;
	virtual	bool	OnReadLocalTerrain(CString szPathName) = 0;
	
	//15 判断文件是否加载成功
	virtual	bool	IsEmpty() = 0;

	//16 释放数据
	virtual	void	OnRelease() = 0;

	//17 保存数据
	virtual	bool	OnSave(CString szPathName) = 0;

	//18 绘制
	virtual	void	OnDraw(CDC *pDC) = 0;
	
	//19 绘制DEM到Map DC上
	virtual	void	DrawToDem() = 0;
	//20 绘制到User DC上
	virtual	void	DrawToUser() = 0;
	//21 绘制到Screen DC上
	virtual	void	DrawToScreen(CDC *pDC) = 0;

	//22 设置/获取内容显示状态
	virtual void	SetShowStatusInContent(bool bShowStatus) = 0;
	virtual bool	GetShowStatusInContent() = 0;
		
	//23 设置/获取格网显示状态
	virtual void	SetShowStatusInCell(bool bShowStatus) = 0;
	virtual bool	GetShowStatusInCell() = 0;

	//24 设置/获取DEM显示状态
	virtual void	SetShowStatusInDem(bool bShowStatus) = 0;
	virtual bool	GetShowStatusInDem() = 0;

	//25 设置/获取坡度显示状态
	virtual void	SetShowStatusInSlope(bool bShowStatus) = 0;
	virtual bool	GetShowStatusInSlope() = 0;
	//25 设置/获取坡度显示状态
	virtual void	SetShowStatusInAspect(bool bShowStatus) = 0;
	virtual bool	GetShowStatusInAspect() = 0;

	///26 设置/获取曲率显示状态
	virtual void	SetShowStatusInCurvature(bool bShowStatus) = 0;
	virtual bool	GetShowStatusInCurvature() = 0;

	//27 设置/获取表面粗糙度显示状态
	virtual void	SetShowStatusInRoughness(bool bShowStatus) = 0;
	virtual bool	GetShowStatusInRoughness() = 0;

	//28 设置/获取地形起伏度显示状态
	virtual void	SetShowStatusInRelif(bool bShowStatus) = 0;
	virtual bool	GetShowStatusInRelif() = 0;

	//28 设置/获取地形起伏度显示状态
	virtual void	SetShowStatusInGeoMorphJudge(bool bShowStatus) = 0;
	virtual bool	GetShowStatusInGeoMorphJudge() = 0;

	//29 鼠标移动，通过主视图类调用
	virtual	void	OnMouseMove(UINT nFlags, CPoint point) = 0;
	//30 鼠标左键弹起，通过主视图类调用
	virtual	void	OnLButtonUp(UINT nFlags, CPoint point) = 0;
	//31 鼠标左键按下，通过主视图类调用
	virtual	void	OnLButtonDown(UINT nFlags, CPoint point) = 0;
	//32 鼠标左键双击，通过主视图类调用
	virtual	void	OnLButtonDblClk(UINT nFlags, CPoint point) = 0;
	//33 鼠标左键弹起，通过主视图类调用
	virtual	void	OnRButtonUp(UINT nFlags, CPoint point) = 0;
	//34 鼠标左键按下，通过主视图类调用
	virtual	void	OnRButtonDown(UINT nFlags, CPoint point) = 0;
	//35 设置光标样式
	virtual	BOOL	OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) = 0;

	//36 响应OnSize函数
	virtual void	OnSize(UINT nType, int cx, int cy) = 0;

	//37 设置格网数目
	virtual	void	SetCellCount(int iCellNumX,int iCellNumY) = 0;
	virtual	void	SetCellCount() = 0;

	//38 分区格网创建
	virtual	void	CellBuild() = 0;
	
	//39 删除重复点
	virtual	void	DeleteRePoint() = 0;

	//40 设置/获取删除重复点中的点与点之间的最小间距
	virtual	void	SetMinDistance(double dMinDistance) = 0;
	virtual	double	GetMinDistance() = 0;

	//41 基于矢量数据的地形统计量
	virtual	void	OnTerrainDescriptiveStatisticsInVector() = 0;
	//42 基于格网数据的地形统计量
	virtual	void	OnTerrainDescriptiveStatisticsInGrd() = 0;

	//43 统计半变异函数,返回数据
	virtual	int		OnSemiVariogramStatistics(double **dX,double **dY) = 0;
	//44 统计半变异函数
	virtual	void	OnSemiVariogramStatisticsEx() = 0;

	//45 计算坡度
	virtual	void	OnCalculateSlope() = 0;
	//46 计算坡度，并计算坡度的统计信息
	virtual	void	OnSlope() = 0;

	//47 计算坡向
	virtual	void	OnCalculateAspect() = 0;
	//
	virtual	void	OnAspect() = 0;

	//48 计算剖面曲率
	virtual	void	OnCalculateProfileCurvature() = 0;
	//49 计算平面曲率
	virtual	void	OnCalculatePlanCurvature() = 0;
	//50 计算切线曲率
	virtual	void	OnCalculateTgCurvature() = 0;
	//51 计算曲率的统计信息
	virtual	void	OnCurvature(int iType) = 0;

	//52 计算地表粗糙度
	virtual	void	OnCalculateTerrainRoughness() = 0;
	//46 计算坡度，并计算坡度的统计信息
	virtual	void	OnTerrainRoughness() = 0;

	//53 计算地形起伏度
	virtual	void	OnCalculateTerrainRelif() = 0;
	//54 批处理计算地形起伏度
	virtual	void	OnCalculateTerrainRelifInBat() = 0;

	//55 计算高程变异系数
	virtual	void	OnCalculateTerrainCV() = 0;
	//56 批处理计算高程变异系数
	virtual	void	OnCalculateTerrainCVInBat() = 0;

	//57 计算表面积
	virtual	void	OnCalculateSurface() = 0;

	//58 计算分形维数
	virtual	void	OnCalculateFractal() = 0;

	//59 批处理地形特征因子信息
	virtual	void	OnCalculateTerrainFactorInBat() = 0;

	//59-1 导出信息因子信息,以GRD表示
	virtual	void	OnExportTerrainFactor() = 0;

	//60 计算残差
	virtual	void	OnCalculateRemainsError() = 0;

	//61 创建TIN
	virtual	void	OnTinCreator(CDC *pDC,int iType = 0) = 0;
	//62 创建GRID
	virtual	void	OnGridCreator(CDC *pDC,int iType = 0) = 0;	

	//63 IDW批处理创建
	virtual	void	OnCreateInIDWInBat(CDC *pDC) = 0;
	//64 RBF批处理创建
	virtual	void	OnCreateInRBFInBat(CDC *pDC) = 0;
	//65 KRG批处理创建
	virtual	void	OnCreateInKRGInBat(CDC *pDC) = 0;
	//66 SPD批处理创建
	virtual	void	OnCreateInSPDInBat(CDC *pDC) = 0;

	//67 IDW交叉验证
	virtual	void	OnCrossValidateInIDWInBat(CDC *pDC) = 0;
	//68 RBF交叉验证
	virtual	void	OnCrossValidateInRBFInBat(CDC *pDC) = 0;
	//69 KRG交叉验证
	virtual	void	OnCrossValidateInKRGInBat(CDC *pDC) = 0;
	//70 SPD交叉验证
	virtual	void	OnCrossValidateInSPDInBat(CDC *pDC) = 0;

	//计算交叉验证情况下的局部地形特征
	virtual	void	OnCalculateLocalTerrainCharacterInBat() = 0;

	//71 插值，即根据指定的xy坐标，计算相应的Z值，并不单指高程，并计算相应的统计信息
	virtual	void	OnInterpolationValue() = 0;

	//72 转换EGC数据为DEM数据
	virtual	bool	OnConversionEGCToGRD(CString szPathName) = 0;
	virtual	bool	OnConversionMATToGRD(CString szPathName) = 0;
	virtual	bool	OnConversionDVFToBLN(CString szPathName) = 0;

	//73 地形抹平，即把高程小于零的归算到零
	virtual	bool	OnTerrainToZero() = 0;
	//74 计算Ripley K函数
	virtual	void	OnCalcRipleyK()=0;

	//75 提取数据
	virtual	void	OnPickUpGrid(CString szPathName) = 0;

	//76 提取数据
	virtual	void	OnWavelets() = 0;

	//
	virtual	void	OnLocalTerrainCharacter() = 0;

	//
	virtual	void	OnCalcSubjectValue() = 0;

	//将残差结果和局部地形特征进行组合，以用于空间聚类分析
	virtual	void	OnCombineREAndLocalTerrainCharacter() = 0;

	//空间聚类分析
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

	//百分比分析
	virtual	void	OnPercentAnalyse() = 0;
	//残差百分比分析批处理
	virtual	void	OnPercentAnalyseCombat() = 0;

	//根据隶属度矩阵，计算离散点的隶属度值
	virtual	void	OnCalcLiShuDuValue() = 0;

	//在“最优”插值参数时计算残差值，并相应的局部地形特征（以D2P64为准），这样可以省略很多的工作。
	virtual	void	OnCalcLTCandDeltaH() = 0;

	//随机提取检查点，并以*.dat文件存储
	virtual	void	OnPickupCheckPoint() = 0;

	//99 回调函数
	virtual	void	OnCallBackFunc_Process(void *pFunc) = 0;
};

_declspec(dllexport) IInterface_DemCreator* CreateDemCreator();
_declspec(dllexport) void ReleaseDemCreator(IInterface_DemCreator* pIDemCreator);

#endif // !defined(AFX_IINTERFACE_DEMCREATOR_H__ABE0EEFD_43F1_45CF_9BCA_8E1DB3387214__INCLUDED_)
