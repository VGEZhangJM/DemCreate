// DCR_Grid_KRG.h: interface for the CDCR_Grid_KRG class.
// ������ֵ�㷨
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCR_GRID_KRG_H__04831692_C43E_42CA_8DEC_1C3A1621A71A__INCLUDED_)
#define AFX_DCR_GRID_KRG_H__04831692_C43E_42CA_8DEC_1C3A1621A71A__INCLUDED_

#include "DCR_GridVertexList.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDCR_Grid_KRG  
{
public:
	CDCR_Grid_KRG();
	virtual ~CDCR_Grid_KRG();
	
	//���ö����б�
	void	SetGridVertexList(CDCR_GridVertexList *pVretexList);

	//�����ֵ�ĸ߳�ֵ
	//dSDZΪZ�ķ���
	bool	CalcHeight(double dX,double dY,double& dZ,double& dSDZ);

	//
	void	SetModelType(int iModelType);
	int		GetModelType();

	//
	void	SetC(double	dC);
	double	GetC();

	//
	void	SetC0(double dC0);
	double	GetC0();

	//
	void	SetA(double dA);
	double	GetA();

protected:
	//��˹ģ��
	bool	KRGGaussModel(double dX,double dY,double& dZ,double& dSDZ);
	//ָ��ģ��
	bool	KRGExponentModel(double dX, double dY, double &dZ,double& dSDZ);
	//��״ģ��
	bool	KRGSphereModel(double dX,double dY,double& dZ,double& dSDZ);
	//����ģ��
	bool	KRGLineModel(double dX,double dY,double& dZ,double& dSDZ);

	//
	double	GetDistance(double x1, double y1, double z1, double x2, double y2, double z2);

public:
	//���������
	CDCR_GridVertexList	*m_pVretexList;
	
	//ģ������
	// 0����״ģ��
	// 1��ָ��ģ��
	// 2����˹ģ��
	int		m_iModelType;

	//�����
	double	m_dC0;
	//����
	double	m_dC;
	//���
	double	m_dAlfa;

	CString	m_szPathName;
};

#endif // !defined(AFX_DCR_GRID_KRG_H__04831692_C43E_42CA_8DEC_1C3A1621A71A__INCLUDED_)
