// DCR_PointPattern_NN.cpp: implementation of the CDCR_PointPattern_NN class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "vr_demcreator.h"
#include "DCR_PointPattern_NN.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDCR_PointPattern_NN::CDCR_PointPattern_NN()
{
	m_pVretexList	= NULL;
	m_dArea			= 0;
}

CDCR_PointPattern_NN::~CDCR_PointPattern_NN()
{

}

//设置顶点列表
void CDCR_PointPattern_NN::SetGridVertexList(CDCR_GridVertexList *pVretexList)
{
	m_pVretexList	= pVretexList;

	//计算面积，根据区域的最大最小值判断
	double	dLeft,dRight,dBottom,dTop;

	CDCR_GridVertex	*pGridVertex;
	int				I;

	dLeft		= 0;
	dRight		= 0;
	dBottom		= 0;
	dTop		= 0;

	I			= 0;
	pGridVertex	= m_pVretexList->GetGridVertex();
	while(pGridVertex != NULL)
	{
		if(I==0)
		{
			dLeft	= pGridVertex->m_dX;
			dRight	= pGridVertex->m_dX;
			dBottom	= pGridVertex->m_dY;
			dTop	= pGridVertex->m_dY;
		}
		else
		{
			if(dLeft > pGridVertex->m_dX)	dLeft	= pGridVertex->m_dX;
			if(dRight < pGridVertex->m_dX)	dRight	= pGridVertex->m_dX;
			if(dBottom > pGridVertex->m_dY)	dBottom	= pGridVertex->m_dY;
			if(dTop < pGridVertex->m_dY)	dTop	= pGridVertex->m_dY;
		}

		I++;

		pGridVertex	= pGridVertex->m_pNext;
	}

	m_dArea	= (dRight-dLeft)*(dTop-dBottom);
}

//////////////////////////////////////////////////////////////////////
// 01 获取两点间的距离
//////////////////////////////////////////////////////////////////////
double CDCR_PointPattern_NN::GetDistance(double x1, double y1, double z1, double x2, double y2, double z2)
{
	double	dDistance;

	//dDistance	= sqrt( (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) + (z1-z2)*(z1-z2) );
	dDistance	= sqrt( (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) );

	return	dDistance;
}
//////////////////////////////////////////////////////////////////////
// 点格局判断,需要验证
//////////////////////////////////////////////////////////////////////
double CDCR_PointPattern_NN::JudgePattern(double dA = 0)
{
	if(dA == 0)		dA	= m_dArea;

	int		iCount;
	iCount	= m_pVretexList->GetGridVertexCount();

	if(iCount < 0)	return	-1;

	double	*dMinDistance;
	dMinDistance	= new	double[iCount];
	
	CDCR_GridVertex	*pGridVertex;
	int				I	=0;

	pGridVertex	= m_pVretexList->GetGridVertex();
	while(pGridVertex != NULL)
	{
		CDCR_GridVertex	*pNextGridVertex;
		bool			bFirstValue	= true;

		pNextGridVertex	= m_pVretexList->GetGridVertex();
		while(pNextGridVertex != NULL)
		{
			if(pNextGridVertex != pGridVertex)
			{
				double	dTempDistance;

				dTempDistance	= GetDistance(pGridVertex->m_dX,pGridVertex->m_dY,0,pNextGridVertex->m_dX,pNextGridVertex->m_dY,0);

				if(bFirstValue)
				{
					dMinDistance[I]	= dTempDistance;
					bFirstValue		= false;
				}
				else
				{
					if(dMinDistance[I]>dTempDistance)
						dMinDistance[I]	= dTempDistance;
				}
			}

			pNextGridVertex	= pNextGridVertex->m_pNext;
		}
		
		I++;
		pGridVertex	= pGridVertex->m_pNext;
	}
	
	//已经求取所有点对应的最小距离点对
	double	dNND	= 0.0;
	for(int i=0;i<iCount;i++)
	{
		dNND	= dNND + dMinDistance[i];
	}
	dNND	= dNND/iCount;

	double	dNNDR;
	dNNDR	= sqrt(dA/iCount)/2;

	//
	if(dMinDistance != NULL)	{	delete	[]dMinDistance;	dMinDistance	= NULL;		}
	
	return	dNND/dNNDR;
}

//
double CDCR_PointPattern_NN::CalcCentroid(double dX,double dY)
{
	CDCR_GridVertex	*pGridVertex;
	double			dCenterX,dCenterY;
	
	dCenterX	= 0;
	dCenterY	= 0;
	pGridVertex	= m_pVretexList->GetGridVertex();
	while(pGridVertex != NULL)
	{
		dCenterX	= dCenterX + pGridVertex->m_dX;
		dCenterY	= dCenterY + pGridVertex->m_dY;

		pGridVertex	= pGridVertex->m_pNext;
	}

	dCenterX	= dCenterX/m_pVretexList->GetGridVertexCount();
	dCenterY	= dCenterY/m_pVretexList->GetGridVertexCount();
	
	return	sqrt((dCenterX-dX)*(dCenterX-dX) + (dCenterY-dY)*(dCenterY-dY));
}
