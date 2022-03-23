// DCR_WorkSpace.h: interface for the CDCR_WorkSpace class.
// DCR的工作区,专门存储数据
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCR_WORKSPACE_H__23E90892_1415_4A7E_999A_D691AED48DC9__INCLUDED_)
#define AFX_DCR_WORKSPACE_H__23E90892_1415_4A7E_999A_D691AED48DC9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DCR_PointFeature.h"
#include "DCR_LineFeature.h"
#include "DCR_Cells.h"

class CDCR_WorkSpace  
{
public:
	CDCR_WorkSpace();
	virtual ~CDCR_WorkSpace();
	
	//
	bool					IsEmpty();

	//绘制
	void					Draw(CDC *pDC);

	void					SetClientRect(CRect rRect);
	//
	void					SetCoordMapping(double dXA,double dXB,double dYA,double dYB);
	void					GetCoordMapping(double& dXA,double& dXB,double& dYA,double& dYB);

	//
	void					SetMapExt(double dLeft,double dRight,double dBottom,double dTop);
	void					GetMapExt(double& dLeft,double& dRight,double& dBottom,double& dTop);
	
	//
	void					GetHeightExt(double& dMinH,double& dMaxH);

	//
	void					SetMinDistance(double dMinDistance);
	//
	double					GetMinDistance();

	//
	bool					Read(CString szPathName);

	//
	bool					Import(CString szPathName);
	
	//
	bool					ImportDEM(CString szPathName);

	//
	void					Release();

	//
	bool					Save(CString szPathName);

	//创建内存格网
	void					CellsBuild();

	//删除重点
	void					DeleteRePoint();
	
	//
	CDCR_Cells*				GetCell();
	//格网数目
	void					SetCellCount(int iCellNumOfX,int iCellNumOfY);
	void					GetCellCount(int& iCellNumOfX,int& iCellNumOfY);
	
	//
	void					GetCellSize(double& dCellSizeOfX,double& dCellSizeOfY);
	//
	void					GetCellExt(double& dLeft,double& dRight,double& dBottom,double& dTop);

	//获取采样点个数
	int						GetSamplePointCount();
	//获取采样点信息
	void					GetSamplePoint(double** dX,double** dY,double** dZ);
	
	//获取点数据
	CDCR_PointFeature*		GetPointFeature();

	//
	void					SetShowStatusInContent(bool bShowStatus);
	bool					GetShowStatusInContent();
		
	//
	void					SetShowStatusInCell(bool bShowStatus);
	bool					GetShowStatusInCell();

	//
	CDCR_PointFeature*		SelectPoint(double& dX,double& dY);
	//
	CDCR_PointFeature*		SelectPoint();
	//
	CString					GetPathName();
	
	//
	bool					PickupCheckPoint(int iCount);

protected:
	//
	void					DrawContent(CDC *pDC);

	void					DrawCell(CDC *pDC);

	//
	void					CellsRelease();

	//
	bool					AddPointToCells(CDCR_PointFeature *pf);
	//
	bool					AddLineToCells(CDCR_LineFeature *lf);
	
	//
	bool					RectCrossWithRect(double dMinX, double dMaxX, double dMinY, double dMaxY, double dLeft, double dRight, double dBottom, double dTop);

	//
	void					ScreenToMap(int iX, int iY, double &dX, double &dY);
	void					MapToScreen(double dX, double dY, int &iX, int &iY);

	//
	COLORREF				GetColorFromH(double dH);
	
protected:
	//实际数据
	CDCR_LineFeature*		m_pLineLast;
	CDCR_LineFeature*		m_pLineHead;

	CDCR_PointFeature*		m_pPointLast;
	CDCR_PointFeature*		m_pPointHead;

	CDCR_PointFeature*		m_pCurrentPoint;

	CString					m_szPathName;

	//内存索引
	CDCR_Cells*				m_pCells;
	int						m_iVertexCount;

	//地图格网大小
	double					m_dCellSizeOfY;
	double					m_dCellSizeOfX;
	
	//地图分块数
	long					m_nCellNumOfY;
	long					m_nCellNumOfX;
	
	//缩放系数
	double					m_dMapXA,m_dMapXB;
	double					m_dMapYA,m_dMapYB;

	//
	CRect					m_rRect;

	//地图区域
	double					m_dMapLeft,m_dMapRight;
	double					m_dMapBottom,m_dMapTop;

	//划分网格范围
	double					m_dCellLeft,m_dCellRight;
	double					m_dCellBottom,m_dCellTop;

	//最高、最低高程
	double					m_dMaxH;
	double					m_dMidH;
	double					m_dMinH;

	//分层设色信息
	COLORREF				m_crMinColor;
	COLORREF				m_crMidColor;
	COLORREF				m_crMaxColor;

	//最小距离
	double					m_dMinDistance;

	//地图内容显示状态
	bool					m_bShowStatusInContent;
	//格网线显示状态
	bool					m_bShowStatusInCell;
};

#endif // !defined(AFX_DCR_WORKSPACE_H__23E90892_1415_4A7E_999A_D691AED48DC9__INCLUDED_)
