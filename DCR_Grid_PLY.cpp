// DCR_Grid_PLY.cpp: implementation of the CDCR_Grid_PLY class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "vr_demcreator.h"
#include "DCR_Grid_PLY.h"

#include "math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDCR_Grid_PLY::CDCR_Grid_PLY()
{
	//是拟合还是插值
	m_iInterpolationMode	= 1;
	//模型类型
	// 1:双线性
	// 2:最近邻
	// 3:自然样条
	// 4:V4-Matlab
	m_iModelType	= 0;

	//采样点点数
	m_pVretexList	= NULL;

	m_szPathName	= "D:\\Out(PLY).grd";
}

CDCR_Grid_PLY::~CDCR_Grid_PLY()
{

}

//设置顶点列表
void CDCR_Grid_PLY::SetGridVertexList(CDCR_GridVertexList *pVretexList)
{	
	m_pVretexList	= pVretexList;
}

//计算插值的高程值
bool CDCR_Grid_PLY::CalcHeight(double dX,double dY,double& dZ)
{
	int		iCount;
	iCount	= m_pVretexList->GetGridVertexCount();
	
	if(iCount == 0)	return	false;

	//调用MathLib解方程
	/*
	//用图表显示	
	Mm	maxtixX,maxtixY,maxtixZ;
	Mm	maxtixDX,maxtixDY;
	Mm	maxtixT;

	maxtixX		= zeros(1,iCount);
	maxtixY		= zeros(1,iCount);
	maxtixZ		= zeros(1,iCount);

	maxtixDX	= zeros(1,1);
	maxtixDY	= zeros(1,1);
	maxtixT		= zeros(1,1);

	//填充数组
	int		I	= 0;
	CDCR_GridVertex	*pGridVertex;

	pGridVertex	= m_pVretexList->GetGridVertex();
	while(pGridVertex != NULL)
	{
		maxtixX.r(1+I)	= pGridVertex->m_dX;
		maxtixY.r(1+I)	= pGridVertex->m_dY;
		maxtixZ.r(1+I)	= pGridVertex->m_dZ;

		I++;

		pGridVertex	= pGridVertex->m_pNext;
	}

	maxtixDX.r(1)	= dX;
	maxtixDY.r(1)	= dY;
	maxtixT.r(1)	= m_iModelType;

	//
	Mm maxtixResult;
	
	//拟合
	if(m_iInterpolationMode == 0)
	{
		maxtixResult	= CalcPolyInterpolation(maxtixX,maxtixY,maxtixZ,maxtixDX,maxtixDY,maxtixT);
		
	}
	else
	//插值
	{
		maxtixResult	= CalcPolyInterpolation(maxtixX,maxtixY,maxtixZ,maxtixDX,maxtixDY,maxtixT);
	}
	
	dZ	= maxtixResult.r(1);
	*/
	return	true;	
}
