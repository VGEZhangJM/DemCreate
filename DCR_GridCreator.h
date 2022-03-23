// DCR_GridCreator.h: interface for the CDCR_GridCreator class.
// ���������ģ
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
	
	//���õ�ͼ��Χ�������ڽ�ģ��
	void			SetMapExt(double dLeft,double dRight,double dBottom,double dTop);
	void			GetMapExt(double& dLeft,double& dRight,double& dBottom,double& dTop);

	//���ù������DEM�ĸ�����Ŀ
	void			SetDemCount(int iNumX,int iNumY);
	void			GetDemCount(int& iNumX,int& iNumY);

	//���ù������DEM�ĸ����ߴ�
	void			SetDemSize(double dSizeX,double dSizeY);
	void			GetDemSize(double& dSizeX,double& dSizeY);

	//ԭʼ���ݵķ�����������
	void			SetCell(CDCR_Cells *pCells);
	//ԭʼ���ݵķ���������Ŀ
	void			SetCellCount(int iNumX,int iNumY);
	//ԭʼ���ݵķ��������ߴ�
	void			SetCellSize(double dSizeX,double dSizeY);
	//ԭʼ���ݵķ���������Χ����͵�ͼ��Χ���в��
	void			SetCellExt(double dLeft,double dRight,double dBottom,double dTop);

	void			SetSampleCount(int iSampleCount);
	
	//
	void			Release();

	//
	void			SetCoordMapping(double dXA,double dXB,double dYA,double dYB);
	void			Draw(CDC *pDC);


	//���������ֵ�Ĳ���������
	//(dX,dY):Ϊ��ֵ��
	//����ֵΪbool
	bool			SearchSamplingPoint(double dX,double dY);

	//
	//����ֵΪbool
	//ʹ�ý�����֤���㾫��ֵ
	bool			SearchSamplingPointCV(CDCR_PointFeature *pDelPoint);


	//�����Ȩ������
	bool			DistanceWrighted(double dX,double dY,double& dZ);

	//
	void			Create(CDC *pDC,int iType);

	//
	void			CreateInIDW(CDC *pDC, int iDemNumX,int iDemNumY,CString szPathName,int iSearchType,int iPointNumPreDirection,int iPointNumInTotal,double dSearchRadius,int iU,double dT);
	//New
	void			CreateInIDW(CDC *pDC, int iDemNumX,int iDemNumY,CString szPathName,int iSearchType,int iPointNumPreDirection,int iPointNumInTotal,double dSearchRadius,int iNumOfU, int *iU,double dT);
	//IDW������֤
	void			CrossValidateInIDW(CDC *pDC, int iSampleCount, CDCR_PointFeature *pSamplePointList,CString szPathName,int iSearchType,int iPointNumPreDirection,int iPointNumInTotal,double dSearchRadius,int iNumOfU, int *iU,double dT);
	void			CrossValidateInIDW(CDC *pDC, int iSampleCount, CDCR_PointFeature *pSamplePointList,CDCR_PointFeature *pPoint,int iSearchType,int iPointNumPreDirection,int iPointNumInTotal,double dSearchRadius,int iU,double dT,double& dDZ);
	
	//
	void			CreateInRBF(CDC *pDC, int iDemNumX,int iDemNumY,CString szPathName,int iSearchType,int iPointNumPreDirection,int iPointNumInTotal,double dSearchRadius,int iModelType,double dC);
	//New
	void			CreateInRBF(CDC *pDC, int iDemNumX,int iDemNumY,CString szPathName,int iSearchType,int iPointNumPreDirection,int iPointNumInTotal,double dSearchRadius,int iModelType,int iNumOfC,double *dC);
	//RBF������֤
	void			CrossValidateInRBF(CDC *pDC, int iSampleCount, CDCR_PointFeature *pSamplePointList,CString szPathName,int iSearchType,int iPointNumPreDirection,int iPointNumInTotal,double dSearchRadius,int iModelType,int iNumOfC,double *dC);
	void			CrossValidateInRBF(CDC *pDC, int iSampleCount, CDCR_PointFeature *pSamplePointList,CDCR_PointFeature *pPoint,int iSearchType,int iPointNumPreDirection,int iPointNumInTotal,double dSearchRadius,int iModelType,double dC,double& dDZ);
	
	//
	void			CreateInKRG(CDC *pDC, int iDemNumX,int iDemNumY,CString szPathName,int iSearchType,int iPointNumPreDirection,int iPointNumInTotal,double dSearchRadius,int iModelType,double dC0,double dC,double dAlfa);
	//
	void			CrossValidateInKRG(CDC *pDC, int iSampleCount, CDCR_PointFeature *pSamplePointList,CString szPathName,int iSearchType,int iPointNumPreDirection,int iPointNumInTotal,double dSearchRadius,int iModelType,double dC0,double dC,double dAlfa);
	void			CrossValidateInKRG(CDC *pDC, int iSampleCount, CDCR_PointFeature *pSamplePointList,CDCR_PointFeature *pPoint,int iSearchType,int iPointNumPreDirection,int iPointNumInTotal,double dSearchRadius,int iModelType,double dC0,double dC,double dAlfa,double& dDZ);
	
	//��������л��½���GRD
	void			CreateInSPD(CDC *pDC, int iDemNumX,int iDemNumY,CString szPathName,int iSearchType,int iPointNumPreDirection,int iPointNumInTotal,double dSearchRadius,int iU,double dT,int iRadiusType ,double dMaxRadius,int iKernalType);
	//New
	void			CreateInSPD(CDC *pDC, int iDemNumX,int iDemNumY,CString szPathName,int iSearchType,int iPointNumPreDirection,int iPointNumInTotal,double dSearchRadius,int iNumOfU,int *iU,double dT,int iRadiusType ,double dMaxRadius,int iKernalType);
	//SPD������֤
	void			CrossValidateInSPD(CDC *pDC, int iSampleCount, CDCR_PointFeature *pSamplePointList,CString szPathName,int iSearchType,int iPointNumPreDirection,int iPointNumInTotal,double dSearchRadius,int iNumOfU,int *iU,double dT,int iRadiusType ,double dMaxRadius,int iKernalType);
	void			CrossValidateInSPD(CDC *pDC, int iSampleCount, CDCR_PointFeature *pSamplePointList,CDCR_PointFeature *pPoint,int iSearchType,int iPointNumPreDirection,int iPointNumInTotal,double dSearchRadius,int iU,double dT,int iRadiusType ,double dMaxRadius,int iKernalType,double& dDZ);
	
	//
	void			CreateInPLY(CDC *pDC, int iDemNumX,int iDemNumY,CString szPathName,int iSearchType,int iPointNumPreDirection,int iPointNumInTotal,double dSearchRadius,int iModelType,int iInterpolationMode);

	//��ȡDEM�ĸ߳�����
	double*			GetHeight();

	//����в�ı�־
	bool			IsCalcRemainsError();

	//��ȡGRD�ļ�·��
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
	
	//�������
	double			GetDistance(double x1, double y1, double z1, double x2, double y2, double z2);
	//
	double			GetAngle(double dX, double dY);

	//������־����
	void			CellReset();
	//
	void			DrawVertex(CDC *pDC);

	//
	void			DrawCell(CDC *pDC,double dX,double dY,double dZ);

public:
	//
	CDCR_Grid_Height	m_pGridHeight[100];

protected:
	//��ͼ����ķ�Χ
	double				m_dMapLeft,m_dMapRight;
	double				m_dMapBottom,m_dMapTop;	

	//�������DEM�ĸ�����Ŀ
	int					m_iDemNumOfX,m_iDemNumOfY;

	//�������DEM�ĸ����ߴ�
	double				m_dDemSizeOfX,m_dDemSizeOfY;
	
	//�������DEM�ĸ߳�ֵ
	double				*m_pHeight;
	//��ߡ���͸߳�ֵ
	double				m_dMaxHeight,m_dMinHeight;
	
	//�̵߳ķ���ֵ
	double				*m_pHeightSD;

	//����CDCR_WorkSpace�еķ�����������
	//������������
	CDCR_Cells			*m_pCells;
	//
	//����������С
	double				m_dCellSizeOfX;
	double				m_dCellSizeOfY;
	
	//������������
	long				m_iCellNumOfX;
	long				m_iCellNumOfY;

	//����������Χ
	double				m_dCellLeft,m_dCellRight;
	double				m_dCellBottom,m_dCellTop;

	//�������������
	//ȫ��������־��

	//��Ϊtrueʱ��m_dSearchRadius�������ã�
	//��Ϊfalseʱ��m_dSearchRadius�����ã�
	bool				m_bGlobalSearch;
	//�����뾶
	double				m_dSearchRadius;
	//�����뾶��������
	bool				m_bSearchRadiusUsed;
	//�����뾶�����õĴ���
	int					m_iSearchRadiusUsed;

	//�������򣺷�Ϊ�޷����ķ��򡢰˷���
	//����һ��ʹ�õ����ķ���Ͱ˷���
	DCR_DIRECTION		m_enumDirection;
	
	//ÿһ�����������ڵ������������
	int					m_iPointNumPreDirection;

	//����������Ŀ
	int					m_iSampleCount;

	//�������ܵ�������������������޷���ʱ�������ã�
	//���ܹ���Ҫ�������ٸ�����ĵ���
	int					m_iPointNumInTotal;
	
	//��ʾ����
	double				m_dMapXA;
	double				m_dMapXB;
	double				m_dMapYA;
	double				m_dMapYB;
	
	//����в�ı�־
	bool				m_bCaclRemainsError;
	//���ɵ�GRD�ļ���·��
	CString				m_szGrdPathName;

	//
	DWORD				m_dwStartTime;
	DWORD				m_dwFinishTime;

	//�����б�
	CDCR_GridVertexList	m_pGridVertexList;
	//�����Ȩƽ��
	CDCR_Grid_IDW		m_pGridIDW;

	//RBF��ֵ
	CDCR_Grid_RBF		m_pGridRBF;

	//������ֵ
	CDCR_Grid_KRG		m_pGridKRG;

	//����ʽ��ֵ
	CDCR_Grid_PLY		m_pGridPLY;

	//л��²�ֵ��
	CDCR_Grid_SPD		m_pGridSPD;
};

#endif // !defined(AFX_DCR_GRIDCREATOR_H__C49377C5_5A8A_4765_8937_2DF5DA154018__INCLUDED_)
