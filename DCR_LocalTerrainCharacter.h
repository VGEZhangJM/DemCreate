// DCR_LocalTerrainCharacter.h: interface for the CDCR_LocalTerrainCharacter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCR_LOCALTERRAINCHARACTER_H__1D43A273_7BAE_465B_BD95_6A2F93EC4861__INCLUDED_)
#define AFX_DCR_LOCALTERRAINCHARACTER_H__1D43A273_7BAE_465B_BD95_6A2F93EC4861__INCLUDED_

#include "DCR_GridVertexList.h"
#include "DCR_PointPattern_NN.h"
#include "DCR_TinTDTACreator.h"
#include "DCR_TinTriangleList.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//�ֲ���������
class CDCR_LocalTerrainCharacter  
{
public:
	CDCR_LocalTerrainCharacter();
	virtual ~CDCR_LocalTerrainCharacter();
public:
	//�������
	bool	CalculatePointPattern();

	//���ȴ���TIN����TINģ���м����¶Ⱥʹֲڶ�
	bool	CaculateTerrainFactor();

	//���ö�������
	void	SetGridVertexList(CDCR_GridVertexList *pAllVretexList,CDCR_GridVertexList *pVretexList,double dRadius,double dX,double dY);

	void	OnSave(FILE *fp);
	
	//
	double	GetPointPatternValue();
	void	SetPointPatternValue(double dValue);
	//
	double	GetCentroidValue();
	void	SetCentroidValue(double dValue);

	//
	double	GetMeanSlope();
	void	SetMeanSlope(double dValue);

	//
	double	GetMeanRoughness();
	void	SetMeanRoughness(double dValue);

	//
	double	GetMaxDistance();
	//
	double	GetMinDistance();
	
	//
	CDCR_TinTriangleList*	GetTriangleList();
	
protected:
	//���Χ�ڵļ��������
	CDCR_GridVertexList		*m_pVretexList;

	//���Χ�����в�����
	CDCR_GridVertexList		*m_pAllVretexList;

	//
	CDCR_TinTriangleList	m_pTriangleList;

	//����ڵ���
	CDCR_PointPattern_NN	m_pPointPatterNN;

	//TIN������
	CDCR_TinTDTACreator		m_pTinCreator;

	//��ǰ������
	double					m_dCenterX;
	double					m_dCenterY;

	//������
	double					m_dRadius;

	//��С����
	double					m_dMinDistance;

	//����ֵ
	double					m_dPointPatternValue;
	//����ֵ
	double					m_dCentroidValue;
	//ƽ���¶�ֵ
	double					m_dMeanSlope;
	//���δֲڶ�ֵ
	double					m_dMeanRoughness;
};

#endif // !defined(AFX_DCR_LOCALTERRAINCHARACTER_H__1D43A273_7BAE_465B_BD95_6A2F93EC4861__INCLUDED_)
