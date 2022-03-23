// DCR_Grid_PLY.h: interface for the CDCR_Grid_PLY class.
// ����ʽ��ֵ������˫���ԣ�����ڡ���Ȼ������V4
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCR_GRID_PLY_H__11038BFA_432A_4D75_ACB3_9BF9D5AB46B2__INCLUDED_)
#define AFX_DCR_GRID_PLY_H__11038BFA_432A_4D75_ACB3_9BF9D5AB46B2__INCLUDED_

#include "DCR_GridVertexList.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDCR_Grid_PLY  
{
public:
	CDCR_Grid_PLY();
	virtual ~CDCR_Grid_PLY();
	
	//���ö����б�
	void	SetGridVertexList(CDCR_GridVertexList *pVretexList);

	//�����ֵ�ĸ߳�ֵ
	bool	CalcHeight(double dX,double dY,double& dZ);

public:
	//����ϻ��ǲ�ֵ
	//0:���
	//1:��ֵ
	int					m_iInterpolationMode;
	//ģ������
	// 0:˫����
	// 1:�����
	// 2:��Ȼ����
	// 3:V4-Matlab
	int					m_iModelType;

	//���������
	CDCR_GridVertexList	*m_pVretexList;

	//
	CString				m_szPathName;
};

#endif // !defined(AFX_DCR_GRID_PLY_H__11038BFA_432A_4D75_ACB3_9BF9D5AB46B2__INCLUDED_)
