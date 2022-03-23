// DCR_LocalTerrainCreator.h: interface for the CDCR_LocalTerrainCreator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCR_LOCALTERRAINCREATOR_H__1960049E_AEF5_439F_A161_280E28D009DF__INCLUDED_)
#define AFX_DCR_LOCALTERRAINCREATOR_H__1960049E_AEF5_439F_A161_280E28D009DF__INCLUDED_

#include "DCR_Cells.h"
#include "DCR_GridVertexList.h"
#include "DCR_TinTriangleList.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDCR_LocalTerrainCreator  
{
public:
	CDCR_LocalTerrainCreator();
	virtual ~CDCR_LocalTerrainCreator();

public:
	//
	void	OnRelease();
	//ԭʼ���ݵķ�����������
	void	SetCell(CDCR_Cells *pCells,int iNumX,int iNumY,double dSizeX,double dSizeY,double dLeft,double dRight,double dBottom,double dTop);

	//
	void	CalculateLocalTerrainCharacter(	int iSampleCount, 
											CDCR_PointFeature *pSamplePointList,
											CString	szPathName,
											int		iSearchType,
											int		iPointNumPreDirection,
											int		iPointNumInTotal,
											double	dSearchRadius
											);

	//�������
	void	CalculateLocalTerrainCharacter(	int iSampleCount, 
											CDCR_PointFeature *pSamplePointList,
											CDCR_PointFeature *pSelectPoint,
											int		iSearchType,
											int		iPointNumPreDirection,
											int		iPointNumInTotal,
											double	dSearchRadius
											);
	
	//
	double	GetPointPatternValue();
	//
	double	GetCentroidValue();
	//
	double	GetMeanSlope();
	//
	double	GetMeanRoughness();
	//
	double	GetMaxDistance();
	//
	double	GetMinDistance();

	//
	CDCR_GridVertexList*	GetGridVertexList();
	//
	CDCR_GridVertexList*	GetAllGridVertexList();
	//
	CDCR_TinTriangleList*	GetTriangleList();

protected:
	//
	bool	CalculateSearchRadius(double dX, double dY, double& dMaxDistance);
		
	//
	bool	SearchSamplingPoint(CDCR_PointFeature *pDelPoint);

	//
	bool	ReSearchSamplingPoint(CDCR_PointFeature *pDelPoint,double dRadius);

	//
	void	CellReset();
	//
	double	GetDistance(double x1, double y1, double z1, double x2, double y2, double z2);
	//
	double	GetAngle(double dX, double dY);

protected:
	//�����ֵ����Ķ����б�
	CDCR_GridVertexList		m_pGridVertexList;

	//���з�Χ�ڵĶ����б�
	CDCR_GridVertexList		m_pAllGridVertexList;

	//
	CDCR_TinTriangleList	m_pTriangleList;

	//����CDCR_WorkSpace�еķ�����������
	//������������
	CDCR_Cells				*m_pCells;
	//
	//����������С
	double					m_dCellSizeOfX;
	double					m_dCellSizeOfY;
	
	//������������
	long					m_iCellNumOfX;
	long					m_iCellNumOfY;

	//����������Χ
	double					m_dCellLeft,m_dCellRight;
	double					m_dCellBottom,m_dCellTop;

	//�������������
	//ȫ��������־��

	//��Ϊtrueʱ��m_dSearchRadius�������ã�
	//��Ϊfalseʱ��m_dSearchRadius�����ã�
	bool					m_bGlobalSearch;
	//�����뾶
	double					m_dSearchRadius;
	//�����뾶��������
	bool					m_bSearchRadiusUsed;
	//�����뾶�����õĴ���
	int						m_iSearchRadiusUsed;

	//�������򣺷�Ϊ�޷����ķ��򡢰˷���
	//����һ��ʹ�õ����ķ���Ͱ˷���
	DCR_DIRECTION			m_enumDirection;
	
	//ÿһ�����������ڵ������������
	int						m_iPointNumPreDirection;

	//�������ܵ�������������������޷���ʱ�������ã�
	//���ܹ���Ҫ�������ٸ�����ĵ���
	int						m_iPointNumInTotal;

	//����ֵ
	double					m_dPointPatternValue;
	//
	double					m_dCentroidValue;
	//ƽ���¶�ֵ
	double					m_dMeanSlope;
	//���δֲڶ�ֵ
	double					m_dMeanRoughness;

	//
	double					m_dMaxDistance;

	//
	double					m_dMinDistance;
};

#endif // !defined(AFX_DCR_LOCALTERRAINCREATOR_H__1960049E_AEF5_439F_A161_280E28D009DF__INCLUDED_)
