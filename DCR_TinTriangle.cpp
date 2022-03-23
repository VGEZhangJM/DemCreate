// DCR_TinTriangle.cpp: implementation of the CDCR_TinTriangle class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DCR_TinTriangle.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDCR_TinTriangle::CDCR_TinTriangle()
{
	m_nID		= -1;

	for(int i=0;i<3;i++)
	{
		m_nVertexID[i]	= -1;
		//三条边，这是存在顺序的
		m_bEdgeUsed[i]	= false;

		//具体的坐标
		m_dX[i]			= 0.0;
		m_dY[i]			= 0.0;
		m_dZ[i]			= 0.0;
	}

	//三角形的范围
	m_dMinX	= m_dMaxX	= 0.0;
	m_dMinY	= m_dMaxY	= 0.0;

	//
	m_bUsed		= false;
	//
	m_bDelete	= false;
	//
	m_pNext		= NULL;
}

//
CDCR_TinTriangle::~CDCR_TinTriangle()
{

}

//////////////////////////////////////////////////////////////////////
// 是否为还没有处理的多边形?
// 判断方法是：m_bUsed中存在false，即为没有被处理完的。
//////////////////////////////////////////////////////////////////////
bool CDCR_TinTriangle::IsUsed()
{
	if(m_bEdgeUsed[0] == false)		return	true;
	if(m_bEdgeUsed[1] == false)		return	true;
	if(m_bEdgeUsed[2] == false)		return	true;

	return	false;
}

//////////////////////////////////////////////////////////////////////
// 设置坐标
//////////////////////////////////////////////////////////////////////
void CDCR_TinTriangle::SetVertexCoord(int iVertexID1, double dX1, double dY1, double dZ1, 
									  int iVertexID2, double dX2, double dY2, double dZ2, 
									  int iVertexID3, double dX3, double dY3, double dZ3)
{
	for(int i=0;i<3;i++)
	{
		if(m_nVertexID[i] == iVertexID1)	
		{
			m_dX[i]	= dX1;	m_dY[i]	= dY1;	m_dZ[i]	= dZ1;
		}

		if(m_nVertexID[i] == iVertexID2)	
		{
			m_dX[i]	= dX2;	m_dY[i]	= dY2;	m_dZ[i]	= dZ2;
		}

		if(m_nVertexID[i] == iVertexID3)	
		{
			m_dX[i]	= dX3;	m_dY[i]	= dY3;	m_dZ[i]	= dZ3;
		}
	}

	//第一点
	m_dMinX	= m_dMaxX	= dX1;
	m_dMinY	= m_dMaxY	= dY1;

	//第二点、第三点
	if(m_dMinX > dX2)	m_dMinX	= dX2;
	if(m_dMaxX < dX2)	m_dMaxX	= dX2;
	if(m_dMinY > dY2)	m_dMinY	= dY2;
	if(m_dMaxY < dY2)	m_dMaxY	= dY2;

	if(m_dMinX > dX3)	m_dMinX	= dX3;
	if(m_dMaxX < dX3)	m_dMaxX	= dX3;
	if(m_dMinY > dY3)	m_dMinY	= dY3;
	if(m_dMaxY < dY3)	m_dMaxY	= dY3;
}

//////////////////////////////////////////////////////////////////////
// 设置坐标
//////////////////////////////////////////////////////////////////////
bool CDCR_TinTriangle::GetVertexCoord(int iVertexID1, double& dX1, double& dY1, double& dZ1,
									  int iVertexID2, double& dX2, double& dY2, double& dZ2,
									  int& iVertexID3, double& dX3, double& dY3, double& dZ3
									  )
{
	int	iItem;
	iItem	= 0;

	for(int i=0;i<3;i++)
	{
		if(m_nVertexID[i] == iVertexID1)
		{
			dX1	= m_dX[i];	dY1	= m_dY[i];	dZ1	= m_dZ[i];

			iItem++;
		}

		if(m_nVertexID[i] == iVertexID2)
		{
			dX2	= m_dX[i];	dY2	= m_dY[i];	dZ2	= m_dZ[i];
			iItem++;
		}

		if(m_nVertexID[i]!=iVertexID1 && m_nVertexID[i]!=iVertexID2)
		{
			iVertexID3	= m_nVertexID[i];
			dX3	= m_dX[i];	dY3	= m_dY[i];	dZ3	= m_dZ[i];
		}
	}

	if(iItem == 2)	return	true;
	return	false;
}

bool CDCR_TinTriangle::GetVertexCoordEx(int& iVertexID1, double& dX1, double& dY1, double& dZ1,
										int& iVertexID2, double& dX2, double& dY2, double& dZ2,
										int& iVertexID3, double& dX3, double& dY3, double& dZ3 
										)
{
	iVertexID1	= m_nVertexID[0];
	dX1			= m_dX[0];	dY1			= m_dY[0];		dZ1			= m_dZ[0];

	iVertexID2	= m_nVertexID[1];
	dX2			= m_dX[1];	dY2			= m_dY[1];		dZ2			= m_dZ[1];

	iVertexID3	= m_nVertexID[2];
	dX3			= m_dX[2];	dY3			= m_dY[2];		dZ3			= m_dZ[2];

	return	true;
}

//////////////////////////////////////////////////////////////////////
//面积公式
//////////////////////////////////////////////////////////////////////
double CDCR_TinTriangle::GetArea(double dX1, double dY1, double dX2, double dY2, double dX3, double dY3)
{
	//m_dArea	= ((x2-x1)*(y3-y1)-(x3-x1)*(y2-y1))/2;
	return	((dX2-dX1)*(dY3-dY1)-(dX3-dX1)*(dY2-dY1));
}

//////////////////////////////////////////////////////////////////////
//判断点是否在三角形中
//////////////////////////////////////////////////////////////////////
bool CDCR_TinTriangle::IsInTriangle(double dX,double dY)
{
	//首先判断点是否在外接矩形中
	if(dX<m_dMinX || dX>m_dMaxX || dY<m_dMinY || dY>m_dMaxY)
		return	false;

	//在外接矩形中，立即判断是否在三角形中
	double	dABCValue;
	dABCValue	= GetArea(m_dX[0],m_dY[0],m_dX[1],m_dY[1],m_dX[2],m_dY[2]);

	double	dABP,dBCP,dACP;
	dABP		= GetArea(dX,dY,m_dX[0],m_dY[0],m_dX[1],m_dY[1]);
	dBCP		= GetArea(dX,dY,m_dX[1],m_dY[1],m_dX[2],m_dY[2]);
	dACP		= GetArea(dX,dY,m_dX[2],m_dY[2],m_dX[0],m_dY[0]);
	
	double	dAllValue;
	dAllValue	= fabs(dABP)+fabs(dBCP)+fabs(dACP);

	dABCValue	= fabs(dABCValue);
		
	//如果三角形的面积足够小，可能在判断时发生错误，应当适当的放大
	if(dABCValue < 1e-5)
	{
		dAllValue	= dAllValue*100000;
		dABCValue	= dABCValue*100000;
	}

	if(fabs(dAllValue-dABCValue) < 1e-10)
		return	true;
	
	return	false;
}

//////////////////////////////////////////////////////////////////////
// 02 三角形生长法：
// 判断点直线的哪一方？
// -1:表示垂直
// 1:表示任意方向
//////////////////////////////////////////////////////////////////////
int CDCR_TinTriangle::CreateJudgeFunction(double dX1, double dY1, double dX2, double dY2, double &a, double &b)
{
	if(fabs(dX2-dX1) < 1e-10)	//x2==x1
	{
		a	= dX1;	
		b	= 0;
		return	-1;
	}
	else
	if(fabs(dY2-dY1) < 1e-10)	//y2=y1
	{
		a	= 0;
		b	= dY1;
		return	1;
	}
	else
	{
		a	= (dY2-dY1) / (dX2-dX1);
		b	= (dX1*dY2-dX2*dY1) / (dX2-dX1);

		return	0;
	}
}

//////////////////////////////////////////////////////////////////////
// 03 三角形生长法：
// 判断结果
//////////////////////////////////////////////////////////////////////
double CDCR_TinTriangle::GetJudgeValue(double dX, double dY, int bSign, double a, double b)
{
	double	dValue;

	if(bSign == -1)				//垂直
	{
		dValue	= dX-a;			//
	}
	else	
	if(bSign == 1)				//水平
	{
		dValue	= dY-b;			
	}
	else
	{
		dValue	= dY-(a*dX-b);	//
	}
	
	return	dValue;
}

//
void CDCR_TinTriangle::UpdateEdge(int iVertexID1,int iVertexID2)
{
	if(((m_nVertexID[0]==iVertexID1)&&(m_nVertexID[1]==iVertexID2)) || ((m_nVertexID[0]==iVertexID2)&&(m_nVertexID[1]==iVertexID1)))
		m_bEdgeUsed[0]	= true;

	if(((m_nVertexID[0]==iVertexID1)&&(m_nVertexID[2]==iVertexID2)) || ((m_nVertexID[0]==iVertexID2)&&(m_nVertexID[2]==iVertexID1)))
		m_bEdgeUsed[1]	= true;

	if(((m_nVertexID[2]==iVertexID1)&&(m_nVertexID[1]==iVertexID2)) || ((m_nVertexID[2]==iVertexID2)&&(m_nVertexID[1]==iVertexID1)))
		m_bEdgeUsed[2]	= true;
}

