// DCR_TinTriangle.h: interface for the CDCR_TinTriangle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCR_TINTRIANGLE_H__2E2468D0_63C5_4DBD_B915_B92503202D9E__INCLUDED_)
#define AFX_DCR_TINTRIANGLE_H__2E2468D0_63C5_4DBD_B915_B92503202D9E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//�����εĴ����Ӧ����˳���
//
class CDCR_TinTriangle  
{
public:
	CDCR_TinTriangle();
	virtual ~CDCR_TinTriangle();

	//�жϵ��Ƿ�����������
	bool	IsInTriangle(double dX,double dY);

	//��ȡ����
	bool	GetVertexCoord(int iVertexID1, double& dX1, double& dY1, double& dZ1,
							int iVertexID2, double& dX2, double& dY2, double& dZ2,
							int& iVertexID3, double& dX3, double& dY3, double& dZ3
							);

	bool	GetVertexCoordEx(int& iVertexID1, double& dX1, double& dY1, double& dZ1,
							int& iVertexID2, double& dX2, double& dY2, double& dZ2,
							int& iVertexID3, double& dX3, double& dY3, double& dZ3 
							);

	// ��������
	void	SetVertexCoord(int iVertexID1, double dX1, double dY1, double dZ1, 
						int iVertexID2, double dX2, double dY2, double dZ2, 
						int iVertexID3, double dX3, double dY3, double dZ3);

	int		CreateJudgeFunction(double dX1, double dY1, double dX2, double dY2, double &a, double &b);
	double	GetJudgeValue(double dX, double dY, int bSign, double a, double b);
	//
	bool	IsUsed();

	//
	double	GetArea(double dX1, double dY1, double dX2, double dY2, double dX3, double dY3);
	
	//
	void	UpdateEdge(int iVertexID1,int iVertexID2);

	//���������
	int		m_nID;		

	//�������
	int		m_nVertexID[3];

	//�����ߣ����Ǵ���˳���
	//m_bUsed[0]:	����0��1
	//m_bUsed[1]:	����0��2
	//m_bUsed[2]:	����2��1
	bool	m_bEdgeUsed[3];

	//�����α����Ƿ��Ѿ���ʹ��
	bool	m_bUsed;

	//�Ƿ��Ѿ�ɾ��
	bool	m_bDelete;

	//���������
	double	m_dX[3];
	double	m_dY[3];
	double	m_dZ[3];

	//�����εķ�Χ
	double	m_dMinX,m_dMaxX;
	double	m_dMinY,m_dMaxY;
	
	//��һ������
	CDCR_TinTriangle	*m_pNext;
};

#endif // !defined(AFX_DCR_TINTRIANGLE_H__2E2468D0_63C5_4DBD_B915_B92503202D9E__INCLUDED_)
