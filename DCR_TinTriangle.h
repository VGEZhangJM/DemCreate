// DCR_TinTriangle.h: interface for the CDCR_TinTriangle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCR_TINTRIANGLE_H__2E2468D0_63C5_4DBD_B915_B92503202D9E__INCLUDED_)
#define AFX_DCR_TINTRIANGLE_H__2E2468D0_63C5_4DBD_B915_B92503202D9E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//三角形的存放是应该有顺序的
//
class CDCR_TinTriangle  
{
public:
	CDCR_TinTriangle();
	virtual ~CDCR_TinTriangle();

	//判断点是否在三角形中
	bool	IsInTriangle(double dX,double dY);

	//获取坐标
	bool	GetVertexCoord(int iVertexID1, double& dX1, double& dY1, double& dZ1,
							int iVertexID2, double& dX2, double& dY2, double& dZ2,
							int& iVertexID3, double& dX3, double& dY3, double& dZ3
							);

	bool	GetVertexCoordEx(int& iVertexID1, double& dX1, double& dY1, double& dZ1,
							int& iVertexID2, double& dX2, double& dY2, double& dZ2,
							int& iVertexID3, double& dX3, double& dY3, double& dZ3 
							);

	// 设置坐标
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

	//三角形序号
	int		m_nID;		

	//顶点序号
	int		m_nVertexID[3];

	//三条边，这是存在顺序的
	//m_bUsed[0]:	代表0，1
	//m_bUsed[1]:	代表0，2
	//m_bUsed[2]:	代表2，1
	bool	m_bEdgeUsed[3];

	//三角形本身是否已经被使用
	bool	m_bUsed;

	//是否已经删除
	bool	m_bDelete;

	//具体的坐标
	double	m_dX[3];
	double	m_dY[3];
	double	m_dZ[3];

	//三角形的范围
	double	m_dMinX,m_dMaxX;
	double	m_dMinY,m_dMaxY;
	
	//下一三角形
	CDCR_TinTriangle	*m_pNext;
};

#endif // !defined(AFX_DCR_TINTRIANGLE_H__2E2468D0_63C5_4DBD_B915_B92503202D9E__INCLUDED_)
