// DCR_Grid_RBF.h: interface for the CDCR_Grid_RBF class.
// �����������ֵ�㷨
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCR_GRID_RBF_H__4FF03EF4_1212_4CB9_89C4_0D464D3351A3__INCLUDED_)
#define AFX_DCR_GRID_RBF_H__4FF03EF4_1212_4CB9_89C4_0D464D3351A3__INCLUDED_

#include "DCR_GridVertexList.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDCR_Grid_RBF  
{
public:
	CDCR_Grid_RBF();
	virtual ~CDCR_Grid_RBF();
	
	//���ö����б�
	void	SetGridVertexList(CDCR_GridVertexList *pVretexList);

	//�����ֵ�ĸ߳�ֵ
	bool	CalcHeight(double dX,double dY,double& dZ);

	//
	void	SetModelType(int iType);
	int		GetModelType();
	
	//
	void	SetC(double dC);
	double	GetC();

	//
	void	SetPathName(CString szPathName);
	CString	GetPathName();

protected:
	//
	bool	RBNCSF(double dX,double dY,double& dZ);
	//
	bool	RBFMLF(double dX, double dY, double &dZ);
	//
	bool	RBFTPSF(double dX, double dY, double &dZ);
	//
	bool	RBFIMQF(double dX,double dY,double& dZ);
	//
	bool	RBFMQF(double dX,double dY,double& dZ);
	//
	double	GetDistance(double x1, double y1, double z1, double x2, double y2, double z2);

public:
	//���������
	CDCR_GridVertexList	*m_pVretexList;
	
	//ģ������,
	//1�����������ֵ��
	//2�������������ֵ��
	int		m_iModelType;

	//����
	double	m_dC;
	
	//
	CString	m_szPathName;
};

#endif // !defined(AFX_DCR_GRID_RBF_H__4FF03EF4_1212_4CB9_89C4_0D464D3351A3__INCLUDED_)
