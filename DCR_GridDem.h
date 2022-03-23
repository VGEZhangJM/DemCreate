// DCR_GridDem.h: interface for the CDCR_GridDem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCR_GRIDDEM_H__54FB971B_58EA_471D_88DD_FB742C821692__INCLUDED_)
#define AFX_DCR_GRIDDEM_H__54FB971B_58EA_471D_88DD_FB742C821692__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDCR_GridDem  
{
public:
	CDCR_GridDem();
	virtual ~CDCR_GridDem();

	//01 读取DEM数据
	bool	OnRead(char *chPathName);
	//02 保存DEM数据
	bool	OnSave(char *chPathName);

	//03 导入DEM数据
	bool	OnImport(char *chPathName);
	//04 导出DEM数据
	bool	OnExport(char *chPathName);

	//11 释放数据，包括DEM、以及地形特征因子数据
	void	OnRelease();

	//12 复制DEM数据
	void	OnClone(CDCR_GridDem *pDEM);

	//13 设置显示窗口
	void	SetClientRect(CRect rRect);

	//14 坐标映射
	void	SetCoordMapping(double dXA,double dXB,double dYA,double dYB);

	//15 绘制各种信息，包括坡度图、坡向图、剖面曲率图、平面曲率图、切线曲率图等
	void	OnDraw(CDC *pDC);
	
	//16 判断是否为空
	bool	IsEmpty();

	//17 获取/设置横向点数
	int		GetNumOfX();
	void	SetNumOfX(int iNumX);

	//18 获取/设置纵向点数
	int		GetNumOfY();
	void	SetNumOfY(int iNumY);

	//19 设置/获取横向间隔
	double	GetSizeOfX();
	void	SetSizeOfX(double dSizeX);
	
	//20 设置/获取纵向间隔
	double	GetSizeOfY();
	void	SetSizeOfY(double dSizeY);

	//
	double	GetRealSize();

	//21 获取/设置DEM范围
	void	GetDemExt(double &dLeft,double &dRight,double &dBottom,double &dTop);
	void	SetDemExt(double dLeft,double dRight,double dBottom,double dTop);

	//22 获取最低、最高高程值
	double	GetMaxHeight();
	double	GetMinHeight();

	//23 通过DEM获取某一点的高程值
	double	GetHeight(double dX,double dY);
	//24 获取DEM的高程值
	double*	GetHeight();

	//25 设置DEM的高程值
	void	SetHeight(double *pHeight);

	//26 设置Z轴比例
	void	SetZScale(double dZScale);
	//27 撤销Z轴比例
	void	UnSetZScale(double dZScale);
		
	//44 计算表面积
	double	OnCalculateSurface(double& dV);

	//45 设置/获取DEM路径
	void	SetPathName(CString szPathName);
	CString	GetPathName();

	//46 设置/获取DEM显示状态
	void	SetShowStatus(bool bShowStatus);
	bool	GetShowStatus();

	//地形抹平
	bool	OnTerrainToZero();

	//提取DEM中的部分数据
	bool	OnPickUpGrid(double dLeft,double dRight,double dBottom,double dTop,CString szPathName);
	
protected:
	//DEM的范围
	double			m_dDemLeft;
	double			m_dDemRight;
	double			m_dDemBottom;
	double			m_dDemTop;
	
	//显示参数
	double			m_dMapXA;
	double			m_dMapXB;
	double			m_dMapYA;
	double			m_dMapYB;

	//
	CRect			m_rRect;

	//DEM的格网数目
	long			m_iNumOfX;
	long			m_iNumOfY;

	//DEM的格网尺寸
	double			m_dSizeOfX;
	double			m_dSizeOfY;

	//DEM的实际比高,一般为1
	double			m_dRealSize;

	//DEM值
	double*			m_pDemObj;
	//DEM的最高最低值
	double			m_dMinHeight;
	double			m_dMaxHeight;

	//DEM的方差值
	double*			m_pDemSD;
	double			m_dMinSD;
	double			m_dMaxSD;

	//路径
	CString			m_szPathName;
	
	//显示状态
	bool			m_bShowStatus;
};

#endif // !defined(AFX_DCR_GRIDDEM_H__54FB971B_58EA_471D_88DD_FB742C821692__INCLUDED_)
