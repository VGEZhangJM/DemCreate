// DCR_Grid_SPD.h: interface for the CDCR_Grid_SPD class.
// �Ľ�л��·�
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCR_GRID_SPD_H__82096AEF_00A2_480A_9464_6D381E1B4307__INCLUDED_)
#define AFX_DCR_GRID_SPD_H__82096AEF_00A2_480A_9464_6D381E1B4307__INCLUDED_

#include "DCR_GridVertexList.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDCR_Grid_SPD  
{
public:
	CDCR_Grid_SPD();
	virtual ~CDCR_Grid_SPD();
	
	//���ö����б�
	void	SetGridVertexList(CDCR_GridVertexList *pVretexList);

	//�����ֵ�ĸ߳�ֵ
	bool	CalcHeight(double dX,double dY,double& dZ);

	//
	bool	CalcHeightEx(double dX,double dY,double& dZ);

	//
	void	SetU(int iU);
	int		GetU();

	//
	void	SetT(double dT);
	double	GetT();
	
	//
	void	SetPathName(CString szPathName);
	CString	GetPathName();

	//
	void	SetCustom(bool bCustom);
	bool	GetCustom();
	//
	void	SetRadius(double dRadius);
	double	GetRadius();
	
protected:
	//�û�ָ���뾶?
	bool				m_bCustom;
	//�ֲ���Χ,�����ǲ��������Զֵ
	//Ҳ�������û�ָ��
	double				m_dRadius;

	//Ȩָ��
	int					m_iU;
	//�⻬����
	double				m_dT;

	//���������
	CDCR_GridVertexList	*m_pVretexList;

	//
	CString				m_szPathName;
};

#endif // !defined(AFX_DCR_GRID_SPD_H__82096AEF_00A2_480A_9464_6D381E1B4307__INCLUDED_)
