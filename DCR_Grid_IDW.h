// DCR_Grid_IDW.h: interface for the CDCR_Grid_IDW class.
// �����Ȩ��ֵ
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCR_GRID_IDW_H__F1FF6CB7_5D58_49BE_9909_3149988CCA57__INCLUDED_)
#define AFX_DCR_GRID_IDW_H__F1FF6CB7_5D58_49BE_9909_3149988CCA57__INCLUDED_

#include "DCR_GridVertexList.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDCR_Grid_IDW  
{
public:
	CDCR_Grid_IDW();
	virtual ~CDCR_Grid_IDW();
	
	//���ö����б�
	void	SetGridVertexList(CDCR_GridVertexList *pVretexList);

	//�����ֵ�ĸ߳�ֵ
	bool	CalcHeight(double dX,double dY,double& dZ);

	//
	void	SetU(int iU);
	int		GetU();

	//
	void	SetT(double dT);
	double	GetT();
	
	//
	void	SetPathName(CString szPathName);
	CString	GetPathName();

public:
	//Ȩָ��
	int					m_iU;
	//�⻬����
	double				m_dT;

	//���������
	CDCR_GridVertexList	*m_pVretexList;

	//
	CString				m_szPathName;
};

#endif // !defined(AFX_DCR_GRID_IDW_H__F1FF6CB7_5D58_49BE_9909_3149988CCA57__INCLUDED_)
