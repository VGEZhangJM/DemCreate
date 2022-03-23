// DCR_GridCreator.h: interface for the CDCR_GridCreator class.
// 规则格网建模
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCR_GRIDCREATOR_H__C49377C5_5A8A_4765_8937_2DF5DA154018__INCLUDED_)
#define AFX_DCR_GRIDCREATOR_H__C49377C5_5A8A_4765_8937_2DF5DA154018__INCLUDED_

#include "DCR_Cells.h"
#include "DCR_Grid_IDW.h"
#include "DCR_Grid_RBF.h"
#include "DCR_Grid_KRG.h"
#include "DCR_Grid_PLY.h"
#include "DCR_Grid_SPD.h"
#include "DCR_Grid_Height.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDCR_GridCreator  
{
public:
	CDCR_GridCreator();
	virtual ~CDCR_GridCreator();
	
	//设置地图范围，即用于建模的
	void			SetMapExt(double dLeft,double dRight,double dBottom,double dTop);
	void			GetMapExt(double& dLeft,double& dRight,double& dBottom,double& dTop);

	//设置规则格网DEM的格网数目
	void			SetDemCount(int iNumX,int iNumY);
	void			GetDemCount(int& iNumX,int& iNumY);

	//设置规则格网DEM的格网尺寸
	void			SetDemSize(double dSizeX,double dSizeY);
	void			GetDemSize(double& dSizeX,double& dSizeY);

	//原始数据的分区格网数据
	void			SetCell(CDCR_Cells *pCells);
	//原始数据的分区格网数目
	void			SetCellCount(int iNumX,int iNumY);
	//原始数据的分区格网尺寸
	void			SetCellSize(double dSizeX,double dSizeY);
	//原始数据的分区格网范围，这和地图范围稍有差别
	void			SetCellExt(double dLeft,double dRight,double dBottom,double dTop);

	void			SetSampleCount(int iSampleCount);
	
	//
	void			Release();

	//
	void			SetCoordMapping(double dXA,double dXB,double dYA,double dYB);
	void			Draw(CDC *pDC);


	//搜索参与插值的采样点数据
	//(dX,dY):为插值点
	//返回值为bool
	bool			SearchSamplingPoint(double dX,double dY);

	//
	//返回值为bool
	//使用交叉验证计算精度值
	bool			SearchSamplingPointCV(CDCR_PointFeature *pDelPoint);


	//距离加权法计算
	bool			DistanceWrighted(double dX,double dY,double& dZ);

	//
	void			Create(CDC *pDC,int iType);

	//
	void			CreateInIDW(CDC *pDC, int iDemNumX,int iDemNumY,CString szPathName,int iSearchType,int iPointNumPreDirection,int iPointNumInTotal,double dSearchRadius,int iU,double dT);
	//New
	void			CreateInIDW(CDC *pDC, int iDemNumX,int iDemNumY,CString szPathName,int iSearchType,int iPointNumPreDirection,int iPointNumInTotal,double dSearchRadius,int iNumOfU, int *iU,double dT);
	//IDW交叉验证
	void			CrossValidateInIDW(CDC *pDC, int iSampleCount, CDCR_PointFeature *pSamplePointList,CString szPathName,int iSearchType,int iPointNumPreDirection,int iPointNumInTotal,double dSearchRadius,int iNumOfU, int *iU,double dT);
	void			CrossValidateInIDW(CDC *pDC, int iSampleCount, CDCR_PointFeature *pSamplePointList,CDCR_PointFeature *pPoint,int iSearchType,int iPointNumPreDirection,int iPointNumInTotal,double dSearchRadius,int iU,double dT,double& dDZ);
	
	//
	void			CreateInRBF(CDC *pDC, int iDemNumX,int iDemNumY,CString szPathName,int iSearchType,int iPointNumPreDirection,int iPointNumInTotal,double dSearchRadius,int iModelType,double dC);
	//New
	void			CreateInRBF(CDC *pDC, int iDemNumX,int iDemNumY,CString szPathName,int iSearchType,int iPointNumPreDirection,int iPointNumInTotal,double dSearchRadius,int iModelType,int iNumOfC,double *dC);
	//RBF交叉验证
	void			CrossValidateInRBF(CDC *pDC, int iSampleCount, CDCR_PointFeature *pSamplePointList,CString szPathName,int iSearchType,int iPointNumPreDirection,int iPointNumInTotal,double dSearchRadius,int iModelType,int iNumOfC,double *dC);
	void			CrossValidateInRBF(CDC *pDC, int iSampleCount, CDCR_PointFeature *pSamplePointList,CDCR_PointFeature *pPoint,int iSearchType,int iPointNumPreDirection,int iPointNumInTotal,double dSearchRadius,int iModelType,double dC,double& dDZ);
	
	//
	void			CreateInKRG(CDC *pDC, int iDemNumX,int iDemNumY,CString szPathName,int iSearchType,int iPointNumPreDirection,int iPointNumInTotal,double dSearchRadius,int iModelType,double dC0,double dC,double dAlfa);
	//
	void			CrossValidateInKRG(CDC *pDC, int iSampleCount, CDCR_PointFeature *pSamplePointList,CString szPathName,int iSearchType,int iPointNumPreDirection,int iPointNumInTotal,double dSearchRadius,int iModelType,double dC0,double dC,double dAlfa);
	void			CrossValidateInKRG(CDC *pDC, int iSampleCount, CDCR_PointFeature *pSamplePointList,CDCR_PointFeature *pPoint,int iSearchType,int iPointNumPreDirection,int iPointNumInTotal,double dSearchRadius,int iModelType,double dC0,double dC,double dAlfa,double& dDZ);
	
	//非批处理谢别德建立GRD
	void			CreateInSPD(CDC *pDC, int iDemNumX,int iDemNumY,CString szPathName,int iSearchType,int iPointNumPreDirection,int iPointNumInTotal,double dSearchRadius,int iU,double dT,int iRadiusType ,double dMaxRadius,int iKernalType);
	//New
	void			CreateInSPD(CDC *pDC, int iDemNumX,int iDemNumY,CString szPathName,int iSearchType,int iPointNumPreDirection,int iPointNumInTotal,double dSearchRadius,int iNumOfU,int *iU,double dT,int iRadiusType ,double dMaxRadius,int iKernalType);
	//SPD交叉验证
	void			CrossValidateInSPD(CDC *pDC, int iSampleCount, CDCR_PointFeature *pSamplePointList,CString szPathName,int iSearchType,int iPointNumPreDirection,int iPointNumInTotal,double dSearchRadius,int iNumOfU,int *iU,double dT,int iRadiusType ,double dMaxRadius,int iKernalType);
	void			CrossValidateInSPD(CDC *pDC, int iSampleCount, CDCR_PointFeature *pSamplePointList,CDCR_PointFeature *pPoint,int iSearchType,int iPointNumPreDirection,int iPointNumInTotal,double dSearchRadius,int iU,double dT,int iRadiusType ,double dMaxRadius,int iKernalType,double& dDZ);
	
	//
	void			CreateInPLY(CDC *pDC, int iDemNumX,int iDemNumY,CString szPathName,int iSearchType,int iPointNumPreDirection,int iPointNumInTotal,double dSearchRadius,int iModelType,int iInterpolationMode);

	//获取DEM的高程数据
	double*			GetHeight();

	//计算残差的标志
	bool			IsCalcRemainsError();

	//获取GRD文件路径
	CString			GetGrdPathName();
	
	//
	int				GetUsedTime();

	//
	int				GetSearchRadiusUsed();
	
protected:
	//
	void			Save(CString szPathName);
	void			SaveEx(CString szPathName,double dMinHeight,double dMaxHeight,double *dHeight);
	void			SaveRE(CString szPathName,double dMinHeight,double dMaxHeight,int iCount,CDCR_PointFeature *pSamplePointList,double *pHeight);

	//
	void			InitialHeight();
	
	//计算距离
	double			GetDistance(double x1, double y1, double z1, double x2, double y2, double z2);
	//
	double			GetAngle(double dX, double dY);

	//格网标志归零
	void			CellReset();
	//
	void			DrawVertex(CDC *pDC);

	//
	void			DrawCell(CDC *pDC,double dX,double dY,double dZ);

public:
	//
	CDCR_Grid_Height	m_pGridHeight[100];

protected:
	//地图区域的范围
	double				m_dMapLeft,m_dMapRight;
	double				m_dMapBottom,m_dMapTop;	

	//规则格网DEM的格网数目
	int					m_iDemNumOfX,m_iDemNumOfY;

	//规则格网DEM的格网尺寸
	double				m_dDemSizeOfX,m_dDemSizeOfY;
	
	//规则格网DEM的高程值
	double				*m_pHeight;
	//最高、最低高程值
	double				m_dMaxHeight,m_dMinHeight;
	
	//高程的方差值
	double				*m_pHeightSD;

	//导入CDCR_WorkSpace中的分区格网数据
	//分区格网数据
	CDCR_Cells			*m_pCells;
	//
	//分区格网大小
	double				m_dCellSizeOfX;
	double				m_dCellSizeOfY;
	
	//分区格网块数
	long				m_iCellNumOfX;
	long				m_iCellNumOfY;

	//分区格网范围
	double				m_dCellLeft,m_dCellRight;
	double				m_dCellBottom,m_dCellTop;

	//关于领域的搜索
	//全局搜索标志，

	//当为true时，m_dSearchRadius不起作用；
	//当为false时，m_dSearchRadius起作用；
	bool				m_bGlobalSearch;
	//搜索半径
	double				m_dSearchRadius;
	//搜索半径起作用了
	bool				m_bSearchRadiusUsed;
	//搜索半径起作用的次数
	int					m_iSearchRadiusUsed;

	//搜索方向：分为无方向、四方向、八方向，
	//但是一般使用的是四方向和八方向
	DCR_DIRECTION		m_enumDirection;
	
	//每一个搜索方向内的最多搜索点数
	int					m_iPointNumPreDirection;

	//采样点总数目
	int					m_iSampleCount;

	//搜索的总点数，这个参数仅仅在无方向时，起作用，
	//即总共需要搜索多少个最近的点数
	int					m_iPointNumInTotal;
	
	//显示参数
	double				m_dMapXA;
	double				m_dMapXB;
	double				m_dMapYA;
	double				m_dMapYB;
	
	//计算残差的标志
	bool				m_bCaclRemainsError;
	//生成的GRD文件的路径
	CString				m_szGrdPathName;

	//
	DWORD				m_dwStartTime;
	DWORD				m_dwFinishTime;

	//顶点列表
	CDCR_GridVertexList	m_pGridVertexList;
	//距离加权平均
	CDCR_Grid_IDW		m_pGridIDW;

	//RBF插值
	CDCR_Grid_RBF		m_pGridRBF;

	//克里金插值
	CDCR_Grid_KRG		m_pGridKRG;

	//多项式插值
	CDCR_Grid_PLY		m_pGridPLY;

	//谢别德插值法
	CDCR_Grid_SPD		m_pGridSPD;
};

#endif // !defined(AFX_DCR_GRIDCREATOR_H__C49377C5_5A8A_4765_8937_2DF5DA154018__INCLUDED_)
