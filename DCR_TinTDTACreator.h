// DCR_TinTDTACreator.h: interface for the CDCR_TinTDTACreator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCR_TINTDTACREATOR_H__C8D0B305_605A_4F78_9741_DCDD35FB414F__INCLUDED_)
#define AFX_DCR_TINTDTACREATOR_H__C8D0B305_605A_4F78_9741_DCDD35FB414F__INCLUDED_

#include "DCR_TinVertex.h"
#include "DCR_TinEdge.h"
#include "DCR_TinTriangle.h"
#include "DCR_Cells.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//�����������㷨
class CDCR_TinTDTACreator  
{
public:
	CDCR_TinTDTACreator();
	virtual ~CDCR_TinTDTACreator();
	
public:
	// 001 �ͷ�TIN���ݼ�����������
	void				Release();

	// 002 ��������
	void				AddVertex(double dX,double dY,double dZ);

	// 003 ʹ�ø��ַ�������TIN
	bool				Create(CDC *pDC = NULL);

	// 004 ������ʾ����
	void 				SetCoordMapping(double dXA,double dXB,double dYA,double dYB);
	//
	void				SetClientRect(CRect rRect);
	
	// 005 ���Ʊ�
	void				DrawEdge(CDC* pDC,CDCR_TinEdge *pEdge);
	// 006 ����TIN
	void				Draw(CDC *pDC);
	//
	void				SetMapExt(double dLeft,double dRight,double dBottom,double dTop);

	//�ڹ���TIN�����ƽ���¶�
	bool				CaculateTerrainFactor(double& dMeanSlope,double& dMeanRoughness);

	//
	double				CaculateSlope(double dX1,double dY1,double dZ1,double dX2,double dY2,double dZ2,double dX3,double dY3,double dZ3);
	//
	double				CaculateSurfaceArea(double dX1,double dY1,double dZ1,double dX2,double dY2,double dZ2,double dX3,double dY3,double dZ3);

	//
	CDCR_TinTriangle*	GetTriangle();

protected:
	//�����б�
	//CArray<CDCR_TinVertex*,CDCR_TinVertex*>		m_arrVertex;
	CDCR_TinVertex		*m_pVertexHead;
	CDCR_TinVertex		*m_pVertexLast;
	int					m_iVertexCount;

	//���б�
	//CArray<CDCR_TinEdge*,CDCR_TinEdge*>			m_arrEdge;
	CDCR_TinEdge		*m_pEdgeHead;
	CDCR_TinEdge		*m_pEdgeLast;
	int					m_iEdgeCount;

	//�������б�
	//CArray<CDCR_TinTriangle*,CDCR_TinTriangle*>	m_arrTriangle;
	CDCR_TinTriangle	*m_pTriangleHead;
	CDCR_TinTriangle	*m_pTriangleLast;
	int					m_iTriangleCount;

	//����ϵ��
	double				m_dMapXA,m_dMapXB;
	double				m_dMapYA,m_dMapYB;
	
	//�ڴ�����
	CDCR_Cells*			m_pCells;

	//��ͼ������С
	double				m_dCellSizeOfY;
	double				m_dCellSizeOfX;
	
	//��ͼ�ֿ���
	long				m_nCellNumOfY;
	long				m_nCellNumOfX;
	//��������Χ
	double				m_dCellLeft,m_dCellRight;
	double				m_dCellBottom,m_dCellTop;
	//��ͼ����
	double				m_dMapLeft,m_dMapRight;
	double				m_dMapBottom,m_dMapTop;

	//
	CRect				m_rRect;

protected:
	CDCR_TinEdge*		GetEdgeFromCells(double dX,double dY,int iVertexID1,int iVertexID2);
	//
	void				CellReset();
	//
	void				CellBuild();
	//
	void				AddTriangle(CDCR_TinTriangle *pTriangle);
	//
	void				AddEdge(CDCR_TinEdge *pEdge);
	//���������ӵ�����������
	bool				AddVertexToCells(CDCR_TinVertex *pVertex);
	//�������ӵ�����������
	bool				AddEdgeToCells(double dX,double dY,CDCR_TinEdge *pEdge);
	//
	void				ReleaseVertex();
	//
	void				ReleaseTriangle();
	//
	void				ReleaseEdge();
	// 01 ��ȡ�����ľ���
	double				GetDistance(double x1, double y1, double z1, double x2, double y2, double z2);

	// 02 �жϵ�ֱ�ߵ���һ����
	int					CreateJudgeFunction(double dX1, double dY1, double dX2, double dY2, double &a, double &b);

	// 03
	double				GetJudgeValue(double dX, double dY, int bSign, double a, double b);

	// 04 ��ȡ���ҽǶ�ֵ
	double				GetCosValue(double dX, double dY, double dZ, double dX1, double dY1, double dZ1,double dX2, double dY2,double dZ2);
	

	// 05 ��ȡָ���Ķ��������
	bool				GetVertexCoordFromID(int iVertexID, double &xx, double &yy, double &zz);
	
	// 06 �ӱ���Ż�ö������
	CDCR_TinEdge*		GetEdgeFromEdgeID(int iEdgeID, int &iVertexID1, int &iVertexID2);
	// 07 ͨ��������Ż�ñ�
	CDCR_TinEdge*		GetEdgeWithVertex(int nVertexID1, int nVertexID2);
	//
	void				GetEdgeWithVertex(int iVertexID1, int iVertexID2, int iVertexID3,CDCR_TinEdge** pEdge1,CDCR_TinEdge** pEdge2,CDCR_TinEdge** pEdge3);

	// 08 ��ȡ��û�д���������ζ���
	// ���ΪNULL�����ʾ���е������ζ��Ѿ��������
	CDCR_TinTriangle*	GetUnUsedTriangle();

	// 09 �жϱ��Ƿ�ʹ������
	bool				IsEdgeUsedTwice(int iVertexID1,int iVertexID2);

	// 10 ��ȡ��һ���������š�����������ġ�
	int					CreateFirstVertex();

	// 11 ���ݵ�һ����������Ѱ�ҵڶ�����������
	int					CreateSecondVertex(int iVertexID);

	// 12 ��ȡ��������������
	int					CreateThirdVertex(int iFirstVertexID, int iSecondVertexID);
};

#endif // !defined(AFX_DCR_TINTDTACREATOR_H__C8D0B305_605A_4F78_9741_DCDD35FB414F__INCLUDED_)
