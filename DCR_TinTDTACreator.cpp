// DCR_TinTDTACreator.cpp: implementation of the CDCR_TinTDTACreator class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DCR_TinTDTACreator.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDCR_TinTDTACreator::CDCR_TinTDTACreator()
{
	//顶点串
	m_pVertexHead	= NULL;
	m_pVertexLast	= NULL;
	m_iVertexCount	= 0;
	
	//边串
	m_pEdgeHead		= NULL;
	m_pEdgeLast		= NULL;
	m_iEdgeCount	= 0;

	//三角形串
	m_pTriangleHead	= NULL;
	m_pTriangleLast	= NULL;
	m_iTriangleCount= 0;

	//地图区域
	m_dMapLeft		= 0.0;
	m_dMapRight		= 0.0;
	m_dMapBottom	= 0.0;
	m_dMapTop		= 0.0;

	//地图格网大小
	m_dCellSizeOfY	= 0.0;
	m_dCellSizeOfX	= 0.0;
	
	//地图分块数
	m_nCellNumOfY	= 15;
	m_nCellNumOfX	= 15;

	//划分网格范围
	m_dCellLeft		= m_dCellRight	= 0;
	m_dCellBottom	= m_dCellTop	= 0;

	m_pCells		= NULL;
}

CDCR_TinTDTACreator::~CDCR_TinTDTACreator()
{
	if(m_pCells != NULL)
	{
		delete	[]m_pCells;
		m_pCells	= NULL;
	}

	Release();
}

//////////////////////////////////////////////////////////////////////
// 01 获取两点间的距离
//////////////////////////////////////////////////////////////////////
double CDCR_TinTDTACreator::GetDistance(double x1, double y1, double z1, double x2, double y2, double z2)
{
	double	dDistance;

	//dDistance	= sqrt( (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) + (z1-z2)*(z1-z2) );
	dDistance	= sqrt( (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) );

	return	dDistance;
}

//////////////////////////////////////////////////////////////////////
// 02 三角形生长法：
// 判断点直线的哪一方？
// -1:表示垂直
// 1:表示任意方向
//////////////////////////////////////////////////////////////////////
int CDCR_TinTDTACreator::CreateJudgeFunction(double dX1, double dY1, double dX2, double dY2, double &a, double &b)
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
double CDCR_TinTDTACreator::GetJudgeValue(double dX, double dY, int bSign, double a, double b)
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

//////////////////////////////////////////////////////////////////////
// 04 三角形生长法：
// 获取余弦角度值
//////////////////////////////////////////////////////////////////////
double CDCR_TinTDTACreator::GetCosValue(double dX, double dY, double dZ, double dX1, double dY1, double dZ1,double dX2, double dY2,double dZ2)
{
	double	a,b,c;

	a	= GetDistance(dX1,dY1,dZ1,dX2,dY2,dZ2);
	b	= GetDistance(dX,dY,dZ,dX1,dY1,dZ1);
	c	= GetDistance(dX,dY,dZ,dX2,dY2,dZ2);

	double	cosa;
	cosa	= (b*b+c*c-a*a)/(2*b*c);

	return	cosa;
}

//////////////////////////////////////////////////////////////////////
// 05 通过顶点ID获取顶点坐标
// 返回值：bool，true表示成功，false表示失败
//////////////////////////////////////////////////////////////////////
bool CDCR_TinTDTACreator::GetVertexCoordFromID(int iVertexID, double &dX, double &dY, double &dZ)
{
	CDCR_TinVertex	*pVertex;
	pVertex	= m_pVertexHead;

	while(pVertex != NULL)
	{
		if(pVertex->m_nID == iVertexID)
		{
			dX	= pVertex->m_xx;
			dY	= pVertex->m_yy;
			dZ	= pVertex->m_zz;

			return	true;
		}

		pVertex	= pVertex->m_pNext;
	}

	return	false;
}

//////////////////////////////////////////////////////////////////////
// 06 从边序号获得顶点序号
//////////////////////////////////////////////////////////////////////
CDCR_TinEdge* CDCR_TinTDTACreator::GetEdgeFromEdgeID(int iEdgeID, int &iVertexID1, int &iVertexID2)
{	
	CDCR_TinEdge*	pEdge;

	pEdge	= m_pEdgeHead;
	while(pEdge != NULL)
	{
		if(pEdge->m_nID == iEdgeID)
		{
			iVertexID1	= pEdge->m_nVertexID[0];
			iVertexID2	= pEdge->m_nVertexID[1];		
			
			return	pEdge;
		}
		
		pEdge	= pEdge->m_pNext;
	}
	
	return	NULL;
}

//////////////////////////////////////////////////////////////////////
// 07 通过顶点获得边
//////////////////////////////////////////////////////////////////////
CDCR_TinEdge* CDCR_TinTDTACreator::GetEdgeWithVertex(int nVertexID1, int nVertexID2)
{
	CDCR_TinEdge*	pEdge;

	pEdge	= m_pEdgeHead;
	while(pEdge != NULL)
	{
		if((pEdge->m_nVertexID[0]==nVertexID1&&pEdge->m_nVertexID[1]==nVertexID2) || 
			(pEdge->m_nVertexID[0]==nVertexID2&&pEdge->m_nVertexID[1]==nVertexID1))
		{
			return	pEdge;
		}
	

		pEdge	= pEdge->m_pNext;
	}

	return	NULL;
}

void CDCR_TinTDTACreator::GetEdgeWithVertex(int iVertexID1, int iVertexID2, int iVertexID3,CDCR_TinEdge** pEdge1,CDCR_TinEdge** pEdge2,CDCR_TinEdge** pEdge3)
{
	CDCR_TinEdge*	pEdge;
	int				iItem;

	int				iEdgeVertexID1,iEdgeVertexID2;

	iItem	= 0;
	pEdge	= m_pEdgeHead;
	while(pEdge != NULL)
	{
		//假如已经被使用两次，就无需再用了
		//if(pEdge->m_iUsedCount < 2)
		{
			iEdgeVertexID1	= pEdge->m_nVertexID[0];
			iEdgeVertexID2	= pEdge->m_nVertexID[1]; 

			if((iEdgeVertexID1==iVertexID1&&iEdgeVertexID2==iVertexID2) || (iEdgeVertexID1==iVertexID2&&iEdgeVertexID2==iVertexID1))
			{
				(*pEdge1)	= pEdge;
				iItem++;
			}
		
			if((iEdgeVertexID1==iVertexID2&&iEdgeVertexID2==iVertexID3) || (iEdgeVertexID1==iVertexID3&&iEdgeVertexID2==iVertexID2))
			{
				(*pEdge2)	= pEdge;
				iItem++;
			}

			if((iEdgeVertexID1==iVertexID3&&iEdgeVertexID2==iVertexID1) || (iEdgeVertexID1==iVertexID1&&iEdgeVertexID2==iVertexID3))
			{
				(*pEdge2)	= pEdge;
				iItem++;
			}
			
			if(iItem == 3)	break;
		}

		pEdge	= pEdge->m_pNext;
	}
}

//////////////////////////////////////////////////////////////////////
// 08 获取还没有处理的三角形对象，
// 如果为NULL，则表示所有的三角形都已经处理完成
//////////////////////////////////////////////////////////////////////
CDCR_TinTriangle* CDCR_TinTDTACreator::GetUnUsedTriangle()
{
	CDCR_TinTriangle	*pTriangle;

	pTriangle	= m_pTriangleHead;
	while(pTriangle != NULL)
	{
		if(pTriangle->m_bUsed == false)
		{
			if(pTriangle->IsUsed())
				return	pTriangle;
			else
				pTriangle->m_bUsed	= true;
		}
		pTriangle	= pTriangle->m_pNext;
	}

	return	NULL;
}


//////////////////////////////////////////////////////////////////////
// 09 判断边是否被使用两次
//////////////////////////////////////////////////////////////////////
bool CDCR_TinTDTACreator::IsEdgeUsedTwice(int iVertexID1, int iVertexID2)
{
	CDCR_TinEdge*	pEdge;
	
	pEdge	= m_pEdgeHead;
	while(pEdge != NULL)
	{
		//能够找到边，则需要判断
		//if(pEdge->m_bUsed == false)
		{
			if((pEdge->m_nVertexID[0]==iVertexID1&&pEdge->m_nVertexID[1]==iVertexID2) || 
				(pEdge->m_nVertexID[1]==iVertexID1&&pEdge->m_nVertexID[0]==iVertexID2))
			{
				if(pEdge->m_iUsedCount < 2)
					return	false;
				else
				{
					//pEdge->m_bUsed	= true;
					return	true;
				}
			}		
		}

		pEdge	= pEdge->m_pNext;
	}
	
	//如果是新的边?	
	return	false;	
}

//////////////////////////////////////////////////////////////////////
// 10 三角形生长法：
// 寻找第一个顶点的序号【可以是任意的】
//////////////////////////////////////////////////////////////////////
int CDCR_TinTDTACreator::CreateFirstVertex()
{	
	CDCR_TinVertex	*pVertex;
	//计算采样点的外接矩形，同时计算中心坐标
	double	dMinX,dMaxX,dX;
	double	dMinY,dMaxY,dY;
	double	dMinZ,dMaxZ,dZ;

	//
	pVertex	= m_pVertexHead;
	while(pVertex != NULL)
	{
		if(pVertex==m_pVertexHead)
		{
			dMinX	= dMaxX	= pVertex->m_xx;
			dMinY	= dMaxY	= pVertex->m_yy;
			dMinZ	= dMaxZ	= pVertex->m_zz;
		}
		else
		{
			if(dMinX > pVertex->m_xx)	dMinX	= pVertex->m_xx;
			if(dMaxX < pVertex->m_xx)	dMaxX	= pVertex->m_xx;
			if(dMinY > pVertex->m_yy)	dMinY	= pVertex->m_yy;
			if(dMaxY < pVertex->m_yy)	dMaxY	= pVertex->m_yy;
			if(dMinZ > pVertex->m_zz)	dMinZ	= pVertex->m_zz;
			if(dMaxZ < pVertex->m_zz)	dMaxZ	= pVertex->m_zz;
		}

		pVertex	= pVertex->m_pNext;
	}
	
	dX	= (dMinX + dMaxX) / 2;
	dY	= (dMinY + dMaxY) / 2;
	dZ	= (dMinZ + dMaxZ) / 2;
	
	//以下是没有优化的程序
	/*
	//寻找离dX、dY、dZ最近的顶点
	double			dDistance;
	CDCR_TinVertex	*pTempVertex;

	pTempVertex	= NULL;
	pVertex		= m_pVertexHead;
	while(pVertex != NULL)
	{
		//进行判断
		double	dTempValue;
		double	dTempXX,dTempYY,dTempZZ;

		dTempXX		= pVertex->m_xx;
		dTempYY		= pVertex->m_yy;
		dTempZZ		= pVertex->m_zz;

		dTempValue	= GetDistance(dX,dY,dZ,dTempXX,dTempYY,dTempZZ);

		if(pVertex == m_pVertexHead)
		{
			dDistance	= dTempValue;
			pTempVertex	= pVertex;
		}
		else
		{
			if(dDistance > dTempValue)
			{
				dDistance	= dTempValue;
				pTempVertex	= pVertex;
			}
		}

		pVertex	= pVertex->m_pNext;
	}
	
	if(pTempVertex != NULL)
	{
		return	pTempVertex->m_nID;
	}
	*/

	//
	CellReset();

	//确定循环的最大次数，m_nCellNumOfX和m_nCellNumOfY的大者
	int	iMaxCount;

	iMaxCount	= m_nCellNumOfX;
	if(iMaxCount < m_nCellNumOfY)	iMaxCount	= m_nCellNumOfY;
	
	//找到dX、dY所在的格网
	int		I,J;
	I	= (int)((dX - m_dCellLeft) / m_dCellSizeOfX);
	J	= (int)((dY - m_dCellBottom) / m_dCellSizeOfY);
	
	//循环的次数
	for(int k=1;k<=iMaxCount;k++)
	{
		int		iBegin,iEnd;
		int		jBegin,jEnd;
		
		iBegin	= I-k;	iEnd	= I+k;
		jBegin	= J-k;	jEnd	= J+k;
		
		bool	bFirstValue;
		double	dMinDistance;
		CDCR_TinVertex	*pTempVertex;

		pTempVertex	= NULL;
		bFirstValue	= true;
		dMinDistance= 0.0;

		int		iC;
		int		iI,iJ;
		for(int j=jBegin;j<=jEnd;j++)
		{
			for(int i=iBegin;i<=iEnd;i++)
			{
				iI	= i;	iJ	= j;

				if(iI < 0)				iI	= 0;
				if(iJ < 0)				iJ	= 0;
				if(iI >= m_nCellNumOfX)	iI	= m_nCellNumOfX-1;
				if(iJ >= m_nCellNumOfY)	iJ	= m_nCellNumOfY-1;
				
				//
				iC	= iJ * m_nCellNumOfX + iI;

				//
				if(m_pCells[iC].m_bUsed == false)
				{
					CDCR_TinVertexAddress	*pTVD;
					
					pTVD	= m_pCells[iC].m_pTVDHead;
					while(pTVD != NULL)
					{			
						//进行判断
						double	dTempValue;
						double	dTempXX,dTempYY,dTempZZ;

						dTempXX		= pTVD->m_pVertex->m_xx;
						dTempYY		= pTVD->m_pVertex->m_yy;
						dTempZZ		= pTVD->m_pVertex->m_zz;

						dTempValue	= GetDistance(dX,dY,dZ,dTempXX,dTempYY,dTempZZ);

						if(bFirstValue)
						{
							dMinDistance= dTempValue;
							pTempVertex	= pTVD->m_pVertex;

							bFirstValue	= false;
						}
						else
						{
							if(dMinDistance > dTempValue)
							{
								dMinDistance= dTempValue;
								pTempVertex	= pTVD->m_pVertex;
							}
						}
						
						pTVD	= pTVD->m_pNext;
					}
				}

				//做标记
				m_pCells[iC].m_bUsed	= true;
			}
		}

		//判断跳出的
		if(pTempVertex != NULL)
		{	
			return	pTempVertex->m_nID;
		}
	}
	
	return	-1;
}

//////////////////////////////////////////////////////////////////////
// 11 三角形生长法：
// 根据第一个顶点坐标寻找第二个顶点的序号
//////////////////////////////////////////////////////////////////////
int CDCR_TinTDTACreator::CreateSecondVertex(int iVertexID)
{
	//获取第一个顶点的坐标
	double	dX1,dY1,dZ1;
	if(!GetVertexCoordFromID(iVertexID,dX1,dY1,dZ1))	return	-1;
	
	//以下是没有经过优化的程序
	/*
	//从所有顶点中搜索和第一个顶点距离最近的另一个顶点
	double	dDistance;
	bool	bFirstValue;
	
	CDCR_TinVertex	*pVertex;
	CDCR_TinVertex	*pTempVertex;
	
	bFirstValue	= true;
	pTempVertex	= NULL;
	pVertex		= m_pVertexHead;
	while(pVertex != NULL)
	{
		if(pVertex->m_nID != iVertexID)
		{
			//进行判断
			double	dTempValue;
			double	dTempXX,dTempYY,dTempZZ;

			dTempXX		= pVertex->m_xx;
			dTempYY		= pVertex->m_yy;
			dTempZZ		= pVertex->m_zz;

			dTempValue	= GetDistance(dX1,dY1,dZ1,dTempXX,dTempYY,dTempZZ);

			if(bFirstValue)
			{
				dDistance	= dTempValue;
				pTempVertex	= pVertex;
				bFirstValue	= false;
			}
			else
			{
				if(dDistance > dTempValue)
				{
					dDistance	= dTempValue;
					pTempVertex	= pVertex;
				}
			}
		}

		pVertex	= pVertex->m_pNext;
	}
	
	if(pTempVertex != NULL)
	{
		return	pTempVertex->m_nID;
	}
	*/

	//
	CellReset();

	//确定循环的最大次数，m_nCellNumOfX和m_nCellNumOfY的大者
	int	iMaxCount;

	iMaxCount	= m_nCellNumOfX;
	if(iMaxCount < m_nCellNumOfY)	iMaxCount	= m_nCellNumOfY;
	
	//找到dX、dY所在的格网
	int		I,J;
	I	= (int)((dX1 - m_dCellLeft) / m_dCellSizeOfX);
	J	= (int)((dY1 - m_dCellBottom) / m_dCellSizeOfY);
	
	//循环的次数
	for(int k=1;k<iMaxCount;k++)
	{
		int		iBegin,iEnd;
		int		jBegin,jEnd;
		
		iBegin	= I-k;	iEnd	= I+k;
		jBegin	= J-k;	jEnd	= J+k;
		
		bool	bFirstValue;
		double	dMinDistance;
		CDCR_TinVertex	*pTempVertex;

		pTempVertex	= NULL;
		bFirstValue	= true;
		dMinDistance= 0.0;

		int		iC;
		int		iI,iJ;
		for(int j=jBegin;j<=jEnd;j++)
		{
			for(int i=iBegin;i<=iEnd;i++)
			{
				iI	= i;	iJ	= j;

				if(iI < 0)				iI	= 0;
				if(iJ < 0)				iJ	= 0;
				if(iI >= m_nCellNumOfX)	iI	= m_nCellNumOfX-1;
				if(iJ >= m_nCellNumOfY)	iJ	= m_nCellNumOfY-1;
				
				//
				iC	= iJ * m_nCellNumOfX + iI;

				//
				if(m_pCells[iC].m_bUsed == false)
				{
					CDCR_TinVertexAddress	*pTVD;
					
					pTVD	= m_pCells[iC].m_pTVDHead;
					while(pTVD != NULL)
					{			
						if(pTVD->m_pVertex->m_nID != iVertexID)
						{
							//进行判断
							double	dTempValue;
							double	dTempXX,dTempYY,dTempZZ;

							dTempXX		= pTVD->m_pVertex->m_xx;
							dTempYY		= pTVD->m_pVertex->m_yy;
							dTempZZ		= pTVD->m_pVertex->m_zz;

							dTempValue	= GetDistance(dX1,dY1,dZ1,dTempXX,dTempYY,dTempZZ);

							if(bFirstValue)
							{
								dMinDistance= dTempValue;
								pTempVertex	= pTVD->m_pVertex;

								bFirstValue	= false;
							}
							else
							{
								if(dMinDistance > dTempValue)
								{
									dMinDistance= dTempValue;
									pTempVertex	= pTVD->m_pVertex;
								}
							}
						}
						
						pTVD	= pTVD->m_pNext;
					}
				}

				//做标记
				m_pCells[iC].m_bUsed	= true;
			}
		}

		//判断跳出的
		if(pTempVertex != NULL)
		{	
			return	pTempVertex->m_nID;
		}
	}
	
	return	-1;
}

//////////////////////////////////////////////////////////////////////
// 12 三角形生长法：
// 获取第三个顶点的序号
//////////////////////////////////////////////////////////////////////
int CDCR_TinTDTACreator::CreateThirdVertex(int iFirstVertexID, int iSecondVertexID)
{
	//获取第一个顶点的坐标
	double	dX1,dY1,dZ1;
	if(!GetVertexCoordFromID(iFirstVertexID,dX1,dY1,dZ1))	return	-1;

	//获取第一个顶点的坐标
	double	dX2,dY2,dZ2;
	if(!GetVertexCoordFromID(iSecondVertexID,dX2,dY2,dZ2))	return	-1;

	//搜索最符合的点序号,是利用剖分准则寻找的,而不是利用最短距离寻找的
	bool	bCosValue;
	double	dCosValue;

	CDCR_TinVertex	*pVertex;
	CDCR_TinVertex	*pTempVertex;

	pTempVertex	= NULL;
	bCosValue	= true;
	pVertex		= m_pVertexHead;
	while(pVertex != NULL)
	{
		if(pVertex->m_nID!=iFirstVertexID && pVertex->m_nID!=iSecondVertexID)
		{
			//进行判断
			double		dTempValue;
			double		dTempXX,dTempYY,dTempZZ;

			dTempXX		= pVertex->m_xx;
			dTempYY		= pVertex->m_yy;
			dTempZZ		= pVertex->m_zz;
			
			//计算COSa
			dTempValue	= GetCosValue(dTempXX,dTempYY,dTempZZ,dX1,dY1,dZ1,dX2,dY2,dZ2);

			if(bCosValue)
			{
				dCosValue	= dTempValue;
				pTempVertex	= pVertex;
				bCosValue	= false;
			}
			else
			{
				if(dCosValue > dTempValue)
				{
					dCosValue	= dTempValue;
					pTempVertex	= pVertex;
				}
			}
		}

		pVertex	= pVertex->m_pNext;
	}

	if(pTempVertex != NULL)
	{
		return	pTempVertex->m_nID;
	}

	return	-1;
}

//////////////////////////////////////////////////////////////////////
// 13 三角网生长算法生成TIN
//////////////////////////////////////////////////////////////////////
bool CDCR_TinTDTACreator::Create(CDC *pDC)
{
	//首先建立分区网格
	CellBuild();


	//先释放,边和三角形
	{
		//
		ReleaseEdge();

		//
		ReleaseTriangle();
	}

	//创建第一个三角形
	{
		int	iFirstVertexID,iSecondVertexID,iThirdVertexID;
		
		iFirstVertexID	= CreateFirstVertex();
		
		iSecondVertexID	= CreateSecondVertex(iFirstVertexID);
		if(iSecondVertexID < 0)	
		{
			//MessageBox(NULL,"Tin构建不成功!","提示信息",MB_OK);
			return	false;
		}

		iThirdVertexID	= CreateThirdVertex(iFirstVertexID,iSecondVertexID);
		if(iThirdVertexID < 0)	
		{
			//MessageBox(NULL,"Tin构建不成功!","提示信息",MB_OK);
			return	false;
		}

		double	dX1,dY1,dZ1,dX2,dY2,dZ2,dX3,dY3,dZ3;

		GetVertexCoordFromID(iFirstVertexID,dX1,dY1,dZ1);
		GetVertexCoordFromID(iSecondVertexID,dX2,dY2,dZ2);
		GetVertexCoordFromID(iThirdVertexID,dX3,dY3,dZ3);

		//首先建立三角形
		//{
			//第一个三角形
			CDCR_TinTriangle*	pTriangle;
			pTriangle					= new	CDCR_TinTriangle;
			if(pTriangle == NULL)	return	false;

			pTriangle->m_nID			= 1;
			pTriangle->m_nVertexID[0]	= iFirstVertexID;
			pTriangle->m_nVertexID[1]	= iSecondVertexID;
			pTriangle->m_nVertexID[2]	= iThirdVertexID;
			AddTriangle(pTriangle);
			
			//设置坐标
			pTriangle->SetVertexCoord(iFirstVertexID,dX1,dY1,dZ1,iSecondVertexID,dX2,dY2,dZ2,iThirdVertexID,dX3,dY3,dZ3);
		//}

		//建立第一条边
		{
			CDCR_TinEdge*	pEdge1;
			//第一条边
			pEdge1						= new	CDCR_TinEdge;
			pEdge1->m_nID				= 1;
			pEdge1->m_nVertexID[0]		= iFirstVertexID;
			pEdge1->m_nVertexID[1]		= iSecondVertexID;
			pEdge1->m_nTriangleID[0]	= 1;
			pEdge1->m_pLeftTriangle		= pTriangle;
			pEdge1->m_iUsedCount++;
			//
			AddEdge(pEdge1);
			//增加到网格中
			AddEdgeToCells((dX1+dX2)/2,(dY1+dY2)/2,pEdge1);
			//实时绘制
			if(pDC != NULL)	DrawEdge(pDC,pEdge1);
		}

		
		//建立第二条边
		{
			CDCR_TinEdge*	pEdge2;
			//第二条边
			pEdge2						= new	CDCR_TinEdge;
			pEdge2->m_nID				= 2;
			pEdge2->m_nVertexID[0]		= iFirstVertexID;
			pEdge2->m_nVertexID[1]		= iThirdVertexID;
			//序号和指针是同一个意思
			pEdge2->m_nTriangleID[0]	= 1;
			pEdge2->m_pLeftTriangle		= pTriangle;
			pEdge2->m_iUsedCount++;

			//
			AddEdge(pEdge2);
			//增加到网格中
			AddEdgeToCells((dX1+dX3)/2,(dY1+dY3)/2,pEdge2);

			//实时绘制
			if(pDC != NULL)	DrawEdge(pDC,pEdge2);
		}

		//建立第三条边
		{
			CDCR_TinEdge*	pEdge3;
			//第三条边
			pEdge3						= new	CDCR_TinEdge;
			pEdge3->m_nID				= 3;
			pEdge3->m_nVertexID[0]		= iThirdVertexID;
			pEdge3->m_nVertexID[1]		= iSecondVertexID;	
			pEdge3->m_nTriangleID[0]	= 1;
			pEdge3->m_pLeftTriangle		= pTriangle;
			pEdge3->m_iUsedCount++;
			
			//
			AddEdge(pEdge3);
			//增加到网格中
			AddEdgeToCells((dX2+dX3)/2,(dY2+dY3)/2,pEdge3);

			//实时绘制
			if(pDC != NULL)	DrawEdge(pDC,pEdge3);
		}
	}

	//开始扩展三角形，以至于形成所有的三角形
	//
	{
		CDCR_TinTriangle	*pNextTriangle;

		pNextTriangle	= m_pTriangleHead;

		if(pNextTriangle == NULL)	return	false;

		do
		{
			for(int I=0;I<3;I++)
			{
				if(pNextTriangle->m_bEdgeUsed[I] == false)
				{
					int		iVertexID1,iVertexID2,iVertexID3;
					//获取第一、第二个、三个顶点坐标
					double	dX1,dY1,dZ1,dX2,dY2,dZ2,dX3,dY3,dZ3;

					//如果第一条边没有处理：顶点指的是0-1
					//0-1
					if(I==0)
					{
						//获取基边的顶点ID
						iVertexID1	= pNextTriangle->m_nVertexID[0];
						dX1			= pNextTriangle->m_dX[0];
						dY1			= pNextTriangle->m_dY[0];
						dZ1			= pNextTriangle->m_dZ[0];

						iVertexID2	= pNextTriangle->m_nVertexID[1];
						dX2			= pNextTriangle->m_dX[1];
						dY2			= pNextTriangle->m_dY[1];
						dZ2			= pNextTriangle->m_dZ[1];

						//获取三角形中的第三个顶点
						iVertexID3	= pNextTriangle->m_nVertexID[2];
						dX3			= pNextTriangle->m_dX[2];
						dY3			= pNextTriangle->m_dY[2];
						dZ3			= pNextTriangle->m_dZ[2];
					}

					//0-2
					if(I==1)
					{
						//获取基边的顶点ID
						iVertexID1	= pNextTriangle->m_nVertexID[0];
						dX1			= pNextTriangle->m_dX[0];
						dY1			= pNextTriangle->m_dY[0];
						dZ1			= pNextTriangle->m_dZ[0];

						iVertexID2	= pNextTriangle->m_nVertexID[2];
						dX2			= pNextTriangle->m_dX[2];
						dY2			= pNextTriangle->m_dY[2];
						dZ2			= pNextTriangle->m_dZ[2];

						//获取三角形中的第三个顶点
						iVertexID3	= pNextTriangle->m_nVertexID[1];
						dX3			= pNextTriangle->m_dX[1];
						dY3			= pNextTriangle->m_dY[1];
						dZ3			= pNextTriangle->m_dZ[1];
					}

					//2-1
					if(I==2)
					{
						//获取基边的顶点ID
						iVertexID1	= pNextTriangle->m_nVertexID[2];
						dX1			= pNextTriangle->m_dX[2];
						dY1			= pNextTriangle->m_dY[2];
						dZ1			= pNextTriangle->m_dZ[2];

						iVertexID2	= pNextTriangle->m_nVertexID[1];
						dX2			= pNextTriangle->m_dX[1];
						dY2			= pNextTriangle->m_dY[1];
						dZ2			= pNextTriangle->m_dZ[1];

						//获取三角形中的第三个顶点
						iVertexID3	= pNextTriangle->m_nVertexID[0];
						dX3			= pNextTriangle->m_dX[0];
						dY3			= pNextTriangle->m_dY[0];
						dZ3			= pNextTriangle->m_dZ[0];
					}

					//需要再次判断
					//结论是必须要					
					{
						CDCR_TinEdge	*pEdge	= NULL;
						pEdge	= GetEdgeFromCells((dX1+dX2)/2,(dY1+dY2)/2,iVertexID1,iVertexID2);

						if(pEdge==NULL || pEdge->m_iUsedCount<2)
							;
						else
							continue;
					}

					//下面是寻找新的顶点，寻到的顶点可能有用，可能无用，需要判断
					//{
						//根据(dX1,dY1)和(dX2,dY2)创建判断函数
						double	a,b;
						int		nSign;

						nSign			= CreateJudgeFunction(dX1,dY1,dX2,dY2,a,b);
						
						//获取对点的位置
						double	dVertexValue;
						dVertexValue	= GetJudgeValue(dX3,dY3,nSign,a,b);
						
						//寻找张角最大的点
						bool			bFirstCosValue;
						double			dMaxCosValue;
						CDCR_TinVertex	*pNewVertex;
						
						bFirstCosValue	= true;
						dMaxCosValue	= 0.0;
						pNewVertex		= NULL;

						//遍历格网，首先排除在同一边，从不是同一边的点中寻找
						int				iC;
						//计算格网的四角点
						double			dCellX1,dCellY1;
						double			dCellX2,dCellY2;
						double			dCellX3,dCellY3;
						double			dCellX4,dCellY4;

						for(int j=0;j<m_nCellNumOfY;j++)
						{
							for(int	i=0;i<m_nCellNumOfX;i++)
							{
								iC	= j*m_nCellNumOfX + i;
																
								//左下
								dCellX1	= m_dCellLeft + m_dCellSizeOfX*i;
								dCellY1	= m_dCellBottom + m_dCellSizeOfY*j;

								//右下
								dCellX2	= dCellX1 + m_dCellSizeOfX;
								dCellY2	= dCellY1;
								
								//右上
								dCellX3	= dCellX2;
								dCellY3	= dCellY1 + m_dCellSizeOfY;
								
								//左下
								dCellX4	= dCellX1;
								dCellY4	= dCellY3;

								//如果四个角点都和（dX，dY）是在(xx1,yy1)、(xx2,yy2)的同一边的，则不用计算
								bool	bDealWith;
								double	dTempVertexValue;

								bDealWith	= false;

								//和dX,dY比在 (xx1,yy1)、(xx2,yy2)对面的
								dTempVertexValue	= GetJudgeValue(dCellX1,dCellY1,nSign,a,b);
								if(dVertexValue>0&&dTempVertexValue<0 || dVertexValue<0&&dTempVertexValue>0)
								{
									bDealWith	= true;
								}

								//
								if(!bDealWith)
								{
									dTempVertexValue	= GetJudgeValue(dCellX2,dCellY2,nSign,a,b);
									if(dVertexValue>0&&dTempVertexValue<0 || dVertexValue<0&&dTempVertexValue>0)
									{
										bDealWith	= true;
									}
								}

								if(!bDealWith)
								{
									//
									dTempVertexValue	= GetJudgeValue(dCellX3,dCellY3,nSign,a,b);
									if(dVertexValue>0&&dTempVertexValue<0 || dVertexValue<0&&dTempVertexValue>0)
									{
										bDealWith	= true;
									}
								}

								if(!bDealWith)
								{
									//
									dTempVertexValue	= GetJudgeValue(dCellX4,dCellY4,nSign,a,b);
									if(dVertexValue>0&&dTempVertexValue<0 || dVertexValue<0&&dTempVertexValue>0)
									{
										bDealWith	= true;
									}
								}

								//需要处理
								if(bDealWith)
								{
									CDCR_TinVertexAddress	*pTVD;
									
									pTVD	= m_pCells[iC].m_pTVDHead;
									while(pTVD != NULL)
									{
										//首先排除iVertexID1，iVertexID2，iVertexID3三个顶点
										//这是必须的!
										if(pTVD->m_pVertex->m_nID!=iVertexID1 && 
											pTVD->m_pVertex->m_nID!=iVertexID2 && 
											pTVD->m_pVertex->m_nID!=iVertexID3)
										{
											//进行判断
											double	dTempCosValue;
											double	dTempXX,dTempYY,dTempZZ;

											dTempXX	= pTVD->m_pVertex->m_xx;
											dTempYY	= pTVD->m_pVertex->m_yy;
											dTempZZ	= pTVD->m_pVertex->m_zz;

											dTempVertexValue	= GetJudgeValue(dTempXX,dTempYY,nSign,a,b);

											//和nVertexID在 (xx1,yy1)、(xx2,yy2)对面的
											//等于0，表明共线，取消
											if(fabs(dTempVertexValue) < 1e-10)
												;
											else
											if(dVertexValue>0&&dTempVertexValue<0 || dVertexValue<0&&dTempVertexValue>0)
											{
												//
												dTempCosValue		= GetCosValue(dTempXX,dTempYY,dTempZZ,dX1,dY1,dZ1,dX2,dY2,dZ2);
												
												if(bFirstCosValue)
												{
													dMaxCosValue	= dTempCosValue;
													pNewVertex		= pTVD->m_pVertex;
													bFirstCosValue	= false;
												}
												else
												{
													if(dMaxCosValue > dTempCosValue)	//求张角大的,即值小的
													{
														dMaxCosValue	= dTempCosValue;
														pNewVertex		= pTVD->m_pVertex;
													}
												}
											}
										}

										pTVD	= pTVD->m_pNext;
									}//while(pTVD != NULL)

								}//if(bDealWith)
							
							}//for(int	i=0;i<m_nCellNumOfX;i++)

						}//for(int j=0;j<m_nCellNumOfY;j++)

						//CString	szInfo;
						//szInfo.Format("%lf\n",dMaxCosValue);
						//TRACE(szInfo);

						if(fabs(dMaxCosValue-1) < 1e-5)	
						{
							dMaxCosValue	= 0.0;
							pNewVertex		= NULL;
						}
						
					//}
					
					{
						//在和第三个点对应的一侧找到张角最大的点了
						//如果存在这样的点，则构建新的三角形。
						if(pNewVertex != NULL)
						{
							//新的顶点号
							int		iNewVertexID;
							double	dNewX,dNewY,dNewZ;

							iNewVertexID	= pNewVertex->m_nID;
							dNewX			= pNewVertex->m_xx;
							dNewY			= pNewVertex->m_yy;
							dNewZ			= pNewVertex->m_zz;

							//需要判断（iVertexID1,nNewVertexID）和（nNewVertexID,iVertexID2）组成的边已经被使用两次
							CDCR_TinEdge	*pEdge1	= NULL;
							CDCR_TinEdge	*pEdge2	= NULL;
							CDCR_TinEdge	*pEdge3	= NULL;

							//首先判断由新顶点组成的边是否存在或已用两次
							//不存在，肯定是新边
							//存在但是仅仅使用一次，表明可以继续使用
							//存在但是已经使用两次，则表明不能在用了
							pEdge1	= GetEdgeFromCells((dX1+dNewX)/2,(dY1+dNewY)/2,iVertexID1,iNewVertexID);
							//if(!IsEdgeUsedTwice(iVertexID1,iNewVertexID))
							if(pEdge1==NULL || pEdge1->m_iUsedCount<2)
							{
								//同理
								pEdge2	= GetEdgeFromCells((dX2+dNewX)/2,(dY2+dNewY)/2,iVertexID2,iNewVertexID);
								//if(!IsEdgeUsedTwice(iNewVertexID,iVertexID2))
								if(pEdge2==NULL || pEdge2->m_iUsedCount<2)
								{
									//新的三角形
									int					nNewTriangleID;
									CDCR_TinTriangle	*pNewTriangle;
									
									//创建新的三角形
									{
										nNewTriangleID					= m_iTriangleCount+1;

										pNewTriangle					= new	CDCR_TinTriangle;
										pNewTriangle->m_nID				= nNewTriangleID;

										pNewTriangle->m_nVertexID[0]	= iVertexID1;
										pNewTriangle->m_nVertexID[1]	= iVertexID2;
										pNewTriangle->m_nVertexID[2]	= iNewVertexID;
										//还需要判断pNewTriangle的iVertexID1和iVertexID2组成的边是不需要判断的
										pNewTriangle->m_bEdgeUsed[0]	= true;

										AddTriangle(pNewTriangle);
									
										//修正三角形的坐标
										pNewTriangle->SetVertexCoord(iVertexID1,dX1,dY1,dZ1,iVertexID2,dX2,dY2,dZ2,iNewVertexID,dNewX,dNewY,dNewZ);
									}

									//对于原来的边进行操作
									pEdge3	= GetEdgeFromCells((dX1+dX2)/2,(dY1+dY2)/2,iVertexID1,iVertexID2);
									//pEdge	= GetEdgeWithVertex(iVertexID1,iVertexID2);	//pEdge1
									if(pEdge3 != NULL)
									{
										pEdge3->m_nTriangleID[1]	= nNewTriangleID;

										pEdge3->m_iUsedCount++;

										//关于pEdge3所对应的边的情况是没有问题的，因为在
										//pNextTriangle->m_bEdgeUsed[I] = true;会赋予使用标志
									}

									//先判断是否已经存在这样的边它已经包含两个顶点号
									//pEdge	= GetEdgeWithVertex(iVertexID1,iNewVertexID);//pEdge3
									if(pEdge1 == NULL)
									{
										pEdge1						= new	CDCR_TinEdge;
										pEdge1->m_nID				= m_iEdgeCount+1;

										pEdge1->m_nVertexID[0]		= iVertexID1;
										pEdge1->m_nVertexID[1]		= iNewVertexID;	
										pEdge1->m_nTriangleID[0]	= nNewTriangleID;
										pEdge1->m_pLeftTriangle		= pNewTriangle;
										pEdge1->m_iUsedCount++;
										
										//
										AddEdge(pEdge1);
										//
										AddEdgeToCells((dX1+dNewX)/2,(dY1+dNewY)/2,pEdge1);
					
										//实时绘制
										if(pDC != NULL)		DrawEdge(pDC,pEdge1);
									}
									else
									{
										pEdge1->m_nTriangleID[1]	= nNewTriangleID;
										pEdge1->m_pRightTriangle	= pNewTriangle;

										pEdge1->m_iUsedCount++;

										/*
										if(pEdge1->m_iUsedCount >= 2)
										{
											//这可能会存在问题。因为不会有象“//pNextTriangle->m_bEdgeUsed[I] = true;会赋予使用标志”的操作
											//因此必须要自己判断
											if(pEdge1->m_pLeftTriangle != NULL)
											{
												//把两个顶点拿过来，用于设置由两个顶点组成的另一个三角形应该是要设置为已用的。
												pEdge1->m_pLeftTriangle->UpdateEdge(iVertexID1,iNewVertexID);
											}

											if(pEdge1->m_pRightTriangle != NULL)
											{
												pEdge1->m_pRightTriangle->UpdateEdge(iVertexID1,iNewVertexID);
											}
										}
										*/
									}

									//pEdge	= GetEdgeWithVertex(iVertexID2,iNewVertexID);//pEdge2
									if(pEdge2 == NULL)
									{
										pEdge2						= new	CDCR_TinEdge;
										pEdge2->m_nID				= m_iEdgeCount+1;

										pEdge2->m_nVertexID[0]		= iVertexID2;
										pEdge2->m_nVertexID[1]		= iNewVertexID;	
										pEdge2->m_nTriangleID[0]	= nNewTriangleID;
										pEdge2->m_pLeftTriangle		= pNewTriangle;
										pEdge2->m_iUsedCount++;

										//
										AddEdge(pEdge2);
										//
										AddEdgeToCells((dX2+dNewX)/2,(dY2+dNewY)/2,pEdge2);
					
										//实时绘制
										if(pDC != NULL)		DrawEdge(pDC,pEdge2);
									}
									else
									{
										pEdge2->m_nTriangleID[1]	= nNewTriangleID;
										pEdge2->m_pRightTriangle	= pNewTriangle;

										pEdge2->m_iUsedCount++;

										/*
										if(pEdge2->m_iUsedCount >= 2)
										{
											//这可能会存在问题。因为不会有象“//pNextTriangle->m_bEdgeUsed[I] = true;会赋予使用标志”的操作
											//因此必须要自己判断
											if(pEdge2->m_pLeftTriangle != NULL)
											{
												//把两个顶点拿过来，用于设置由两个顶点组成的另一个三角形应该是要设置为已用的。
												pEdge2->m_pLeftTriangle->UpdateEdge(iVertexID2,iNewVertexID);
											}
											if(pEdge2->m_pRightTriangle != NULL)
											{
												pEdge2->m_pRightTriangle->UpdateEdge(iVertexID2,iNewVertexID);
											}
										}
										*/
									}
								
								}//第二条边是否使用两次的判断

							}//第一条边是否使用两次的判断

						}//if(pTempVertex != NULL)
					}

					//用过了
					pNextTriangle->m_bEdgeUsed[I] = true;

				}//if(pTempTriangle->m_bUsed[I] == false)

			}//for(int I=0;I<3;I++)
			
			//重新判断下三角形的信息
			//pNextTriangle	= GetUnUsedTriangle();
			pNextTriangle	= pNextTriangle->m_pNext;
		}while(pNextTriangle != NULL);
	}
	
	return	true;	
}

//没有经过优化的程序
/*
bool CDCR_TinTDTACreator::Create(CDC *pDC)
{
	//首先建立分区网格
	CellBuild();

	//先释放,边和三角形
	{
		//
		ReleaseEdge();

		//
		ReleaseTriangle();
	}

	//创建第一个三角形
	{
		int	iFirstVertexID,iSecondVertexID,iThirdVertexID;
		
		iFirstVertexID	= CreateFirstVertex();
		
		iSecondVertexID	= CreateSecondVertex(iFirstVertexID);
		if(iSecondVertexID < 0)	
		{
			MessageBox(NULL,"Tin构建不成功!","提示信息",MB_OK);
			return	false;
		}

		iThirdVertexID	= CreateThirdVertex(iFirstVertexID,iSecondVertexID);
		if(iThirdVertexID < 0)	
		{
			MessageBox(NULL,"Tin构建不成功!","提示信息",MB_OK);
			return	false;
		}

		double	dX1,dY1,dZ1,dX2,dY2,dZ2,dX3,dY3,dZ3;

		GetVertexCoordFromID(iFirstVertexID,dX1,dY1,dZ1);
		GetVertexCoordFromID(iSecondVertexID,dX2,dY2,dZ2);
		GetVertexCoordFromID(iThirdVertexID,dX3,dY3,dZ3);

		CDCR_TinEdge*	pEdge;
		//第一条边
		pEdge						= new	CDCR_TinEdge;
		pEdge->m_nID				= 1;
		pEdge->m_nVertexID[0]		= iFirstVertexID;
		pEdge->m_nVertexID[1]		= iSecondVertexID;
		pEdge->m_nTriangleID[0]		= 1;
		pEdge->m_iUsedCount++;
		
		//
		AddEdge(pEdge);
		//增加到网格中
		AddEdgeToCells((dX1+dX2)/2,(dY1+dY2)/2,pEdge);

		//实时绘制
		DrawEdge(pDC,pEdge);
		
		//第二条边
		pEdge						= new	CDCR_TinEdge;
		pEdge->m_nID				= 2;
		pEdge->m_nVertexID[0]		= iFirstVertexID;
		pEdge->m_nVertexID[1]		= iThirdVertexID;
		pEdge->m_nTriangleID[0]		= 1;
		pEdge->m_iUsedCount++;

		//
		AddEdge(pEdge);
		//增加到网格中
		AddEdgeToCells((dX1+dX3)/2,(dY1+dY3)/2,pEdge);

		//实时绘制
		DrawEdge(pDC,pEdge);

		//第三条边
		pEdge						= new	CDCR_TinEdge;
		pEdge->m_nID				= 3;
		pEdge->m_nVertexID[0]		= iThirdVertexID;
		pEdge->m_nVertexID[1]		= iSecondVertexID;	
		pEdge->m_nTriangleID[0]		= 1;
		pEdge->m_iUsedCount++;
		
		//
		AddEdge(pEdge);
		//增加到网格中
		AddEdgeToCells((dX2+dX3)/2,(dY2+dY3)/2,pEdge);

		//实时绘制
		DrawEdge(pDC,pEdge);
		
		//第一个三角形
		CDCR_TinTriangle*	pTriangle;
		pTriangle					= new	CDCR_TinTriangle;
		pTriangle->m_nID			= 1;
		pTriangle->m_nVertexID[0]	= iFirstVertexID;
		pTriangle->m_nVertexID[1]	= iSecondVertexID;
		pTriangle->m_nVertexID[2]	= iThirdVertexID;
		AddTriangle(pTriangle);
		
		//设置坐标
		pTriangle->SetVertexCoord(iFirstVertexID,dX1,dY1,dZ1,iSecondVertexID,dX2,dY2,dZ2,iThirdVertexID,dX3,dY3,dZ3);
	}

	//开始扩展三角形，以至于形成所有的三角形
	//
	{
		//CDCR_TinEdge		*pEdge;
		CDCR_TinTriangle	*pNextTriangle;

		//pEdge			= NULL;
		//pNextTriangle	= GetUnUsedTriangle();
		pNextTriangle	= m_pTriangleHead;

		do
		{
			for(int I=0;I<3;I++)
			{
				if(pNextTriangle->m_bEdgeUsed[I] == false)
				{
					int		iVertexID1,iVertexID2,iVertexID3;
					//获取第一、第二个、三个顶点坐标
					double	dX1,dY1,dZ1,dX2,dY2,dZ2,dX3,dY3,dZ3;

					//如果第一条边没有处理：顶点指的是0-1
					//0-1
					if(I==0)
					{
						//获取基边的顶点ID
						iVertexID1	= pNextTriangle->m_nVertexID[0];
						dX1			= pNextTriangle->m_dX[0];
						dY1			= pNextTriangle->m_dY[0];
						dZ1			= pNextTriangle->m_dZ[0];

						iVertexID2	= pNextTriangle->m_nVertexID[1];
						dX2			= pNextTriangle->m_dX[1];
						dY2			= pNextTriangle->m_dY[1];
						dZ2			= pNextTriangle->m_dZ[1];

						//获取三角形中的第三个顶点
						iVertexID3	= pNextTriangle->m_nVertexID[2];
						dX3			= pNextTriangle->m_dX[2];
						dY3			= pNextTriangle->m_dY[2];
						dZ3			= pNextTriangle->m_dZ[2];
					}

					//0-2
					if(I==1)
					{
						//获取基边的顶点ID
						iVertexID1	= pNextTriangle->m_nVertexID[0];
						dX1			= pNextTriangle->m_dX[0];
						dY1			= pNextTriangle->m_dY[0];
						dZ1			= pNextTriangle->m_dZ[0];

						iVertexID2	= pNextTriangle->m_nVertexID[2];
						dX2			= pNextTriangle->m_dX[2];
						dY2			= pNextTriangle->m_dY[2];
						dZ2			= pNextTriangle->m_dZ[2];

						//获取三角形中的第三个顶点
						iVertexID3	= pNextTriangle->m_nVertexID[1];
						dX3			= pNextTriangle->m_dX[1];
						dY3			= pNextTriangle->m_dY[1];
						dZ3			= pNextTriangle->m_dZ[1];
					}

					//2-1
					if(I==2)
					{
						//获取基边的顶点ID
						iVertexID1	= pNextTriangle->m_nVertexID[2];
						dX1			= pNextTriangle->m_dX[2];
						dY1			= pNextTriangle->m_dY[2];
						dZ1			= pNextTriangle->m_dZ[2];

						iVertexID2	= pNextTriangle->m_nVertexID[1];
						dX2			= pNextTriangle->m_dX[1];
						dY2			= pNextTriangle->m_dY[1];
						dZ2			= pNextTriangle->m_dZ[1];

						//获取三角形中的第三个顶点
						iVertexID3	= pNextTriangle->m_nVertexID[0];
						dX3			= pNextTriangle->m_dX[0];
						dY3			= pNextTriangle->m_dY[0];
						dZ3			= pNextTriangle->m_dZ[0];
					}

					//根据(dX1,dY1)和(dX2,dY2)创建判断函数
					double	a,b;
					int		nSign;

					nSign			= CreateJudgeFunction(dX1,dY1,dX2,dY2,a,b);
					
					//获取对点的位置
					double	dVertexValue;
					dVertexValue	= GetJudgeValue(dX3,dY3,nSign,a,b);
					
					//这是没有优化的程序
					
					//寻找张角最大的点
					bool	bCosValue;
					double	dCosValue;
					CDCR_TinVertex	*pVertex;
					CDCR_TinVertex	*pTempVertex;
					
					bCosValue	= true;
					dCosValue	= 0.0;
					pTempVertex	= NULL;

					pVertex		= m_pVertexHead;
					while(pVertex != NULL)
					{
						//首先排除iVertexID，iVertexID1，iVertexID2三个顶点
						//这是必须的!
						if(pVertex->m_nID!=iVertexID1 && 
							pVertex->m_nID!=iVertexID2 && 
							pVertex->m_nID!=iVertexID3)
						{
							//进行判断
							double	dTempValue;
							double	dTempXX,dTempYY,dTempZZ;
							double	dTempVertexValue;

							dTempXX	= pVertex->m_xx;
							dTempYY	= pVertex->m_yy;
							dTempZZ	= pVertex->m_zz;

							dTempVertexValue	= GetJudgeValue(dTempXX,dTempYY,nSign,a,b);

							//和nVertexID在 (xx1,yy1)、(xx2,yy2)对面的
							if(dVertexValue*dTempVertexValue < 0)
							{
								//
								dTempValue		= GetCosValue(dTempXX,dTempYY,dTempZZ,dX1,dY1,dZ1,dX2,dY2,dZ2);

								if(bCosValue)
								{
									dCosValue	= dTempValue;
									pTempVertex	= pVertex;
									bCosValue	= false;
								}
								else
								{
									if(dCosValue > dTempValue)	//求张角大的,即值小的
									{
										dCosValue	= dTempValue;
										pTempVertex	= pVertex;
									}
								}
							}
						}

						pVertex	= pVertex->m_pNext;
					}//while(pVertex != NULL)
					
				
					{
						//在和第三个点对应的一侧找到张角最大的点了
						//如果存在这样的点，则构建新的三角形。
						if(pTempVertex != NULL)
						{
							//新的顶点号
							int		iNewVertexID;
							double	dNewX,dNewY,dNewZ;

							iNewVertexID	= pTempVertex->m_nID;
							dNewX			= pTempVertex->m_xx;
							dNewY			= pTempVertex->m_yy;
							dNewZ			= pTempVertex->m_zz;

							//需要判断（iVertexID1,nNewVertexID）和（nNewVertexID,iVertexID2）组成的边已经被使用两次

							//过了第一个判读
							//pEdge1	= GetEdgeFromCells((dX1+dNewX)/2,(dY1+dNewY)/2,iVertexID1,iNewVertexID);
							if(!IsEdgeUsedTwice(iVertexID1,iNewVertexID))
							//if(pEdge1==NULL || pEdge1->m_iUsedCount<2)
							{
								//pEdge2	= GetEdgeFromCells((dX2+dNewX)/2,(dY2+dNewY)/2,iVertexID2,iNewVertexID);
								if(!IsEdgeUsedTwice(iNewVertexID,iVertexID2))
								//if(pEdge2==NULL || pEdge2->m_iUsedCount<2)
								{
									CDCR_TinEdge	*pEdge;

									//新的三角形
									int	nNewTriangleID;
									nNewTriangleID	= m_iTriangleCount+1;

									//对于原来的边进行操作
									//pEdge3	= GetEdgeFromCells((dX1+dX2)/2,(dY1+dY2)/2,iVertexID1,iVertexID2);
									pEdge	= GetEdgeWithVertex(iVertexID1,iVertexID2);	//pEdge1
									if(pEdge != NULL)
									{
										pEdge->m_nTriangleID[1]	= nNewTriangleID;

										pEdge->m_iUsedCount++;
									}

									//先判断是否已经存在这样的边它已经包含两个顶点号
									pEdge	= GetEdgeWithVertex(iVertexID1,iNewVertexID);//pEdge3
									if(pEdge == NULL)
									{
										pEdge						= new	CDCR_TinEdge;
										pEdge->m_nID				= m_iEdgeCount+1;

										pEdge->m_nVertexID[0]		= iVertexID1;
										pEdge->m_nVertexID[1]		= iNewVertexID;	
										pEdge->m_nTriangleID[0]	= nNewTriangleID;
										pEdge->m_iUsedCount++;
										
										//
										AddEdge(pEdge);
										//
										//AddEdgeToCells((dX1+dNewX)/2,(dY1+dNewY)/2,pEdge1);
					
										//实时绘制
										DrawEdge(pDC,pEdge);
									}
									else
									{
										pEdge->m_nTriangleID[1]	= nNewTriangleID;

										pEdge->m_iUsedCount++;
									}

									pEdge	= GetEdgeWithVertex(iVertexID2,iNewVertexID);//pEdge2
									if(pEdge == NULL)
									{
										pEdge						= new	CDCR_TinEdge;
										pEdge->m_nID				= m_iEdgeCount+1;

										pEdge->m_nVertexID[0]		= iVertexID2;
										pEdge->m_nVertexID[1]		= iNewVertexID;	
										pEdge->m_nTriangleID[0]	= nNewTriangleID;
										pEdge->m_iUsedCount++;

										//
										AddEdge(pEdge);
										//
										//AddEdgeToCells((dX2+dNewX)/2,(dY2+dNewY)/2,pEdge2);
					
										//实时绘制
										DrawEdge(pDC,pEdge);
									}
									else
									{
										pEdge->m_nTriangleID[1]	= nNewTriangleID;

										pEdge->m_iUsedCount++;
									}

									//创建新的三角形
									CDCR_TinTriangle	*pNewTriangle;

									pNewTriangle				= new	CDCR_TinTriangle;
									pNewTriangle->m_nID			= nNewTriangleID;

									pNewTriangle->m_nVertexID[0]	= iVertexID1;
									pNewTriangle->m_nVertexID[1]	= iVertexID2;
									pNewTriangle->m_nVertexID[2]	= iNewVertexID;
									//还需要判断pNewTriangle的iVertexID1和iVertexID2组成的边是不需要判断的
									pNewTriangle->m_bEdgeUsed[0]	= true;

									AddTriangle(pNewTriangle);
								
									//修正三角形的坐标
									pNewTriangle->SetVertexCoord(iVertexID1,dX1,dY1,dZ1,iVertexID2,dX2,dY2,dZ2,iNewVertexID,dNewX,dNewY,dNewZ);
								
								}//第二条边是否使用两次的判断

							}//第一条边是否使用两次的判断

						}//if(pTempVertex != NULL)
					}

					//用过了
					pNextTriangle->m_bEdgeUsed[I] = true;

				}//if(pTempTriangle->m_bUsed[I] == false)

			}//for(int I=0;I<3;I++)
			
			//重新判断下三角形的信息
			//pNextTriangle	= GetUnUsedTriangle();
			pNextTriangle	= pNextTriangle->m_pNext;
		}while(pNextTriangle != NULL);
	}
	
	return	true;	
}
*/
//////////////////////////////////////////////////////////////////////
// 001 释放TIN数据及采样点数据
//////////////////////////////////////////////////////////////////////
void CDCR_TinTDTACreator::Release( )
{
	//
	ReleaseVertex();

	//
	ReleaseEdge();

	//
	ReleaseTriangle();
}

//////////////////////////////////////////////////////////////////////
// 002 增加顶点坐标
//////////////////////////////////////////////////////////////////////
void CDCR_TinTDTACreator::AddVertex(double dX,double dY,double dZ)
{
	CDCR_TinVertex*	pVertex;

	m_iVertexCount++;

	pVertex	= new	CDCR_TinVertex;
	pVertex->Add(m_iVertexCount,dX,dY,dZ);
		
	if(m_pVertexHead == NULL)
	{
		m_pVertexHead	= pVertex;
		m_pVertexLast	= pVertex;
	}
	else
	{	
		m_pVertexLast->m_pNext	= pVertex;
		m_pVertexLast			= pVertex;
	}
}

//////////////////////////////////////////////////////////////////////
// 往格网中增加顶点
//////////////////////////////////////////////////////////////////////
bool CDCR_TinTDTACreator::AddVertexToCells(CDCR_TinVertex *pVertex)
{
	if(pVertex == NULL)				return	false;

	int							ci;	
	int							xi,yi;
	CDCR_TinVertexAddress		*tvd;
		
	//仅仅考虑一个点
	xi	= (int)((pVertex->m_xx - m_dCellLeft) / m_dCellSizeOfX);
	yi	= (int)((pVertex->m_yy - m_dCellBottom) / m_dCellSizeOfY);

	if(xi < 0)				xi	= 0;
	if(yi < 0)				yi	= 0;
	if(xi >= m_nCellNumOfX)	xi	= m_nCellNumOfX-1;
	if(yi >= m_nCellNumOfY)	yi	= m_nCellNumOfY-1;

	ci	= yi * m_nCellNumOfX + xi;

	if(m_pCells[ci].m_pTVDHead == NULL)
	{
		tvd							= new	CDCR_TinVertexAddress;	
		tvd->m_pVertex				= pVertex;

		m_pCells[ci].m_pTVDHead		= tvd;
		m_pCells[ci].m_pTVDLast		= tvd;
	}
	else
	{
		tvd							= new	CDCR_TinVertexAddress;	
		tvd->m_pVertex				= pVertex;

		m_pCells[ci].m_pTVDLast->m_pNext	= tvd;
		m_pCells[ci].m_pTVDLast				= tvd;
	}

	return	true;
}

//将边增加到分区格网中
bool CDCR_TinTDTACreator::AddEdgeToCells(double dX,double dY,CDCR_TinEdge *pEdge)
{
	if(pEdge == NULL)				return	false;

	int							ci;	
	int							xi,yi;
	CDCR_TinEdgeAddress			*pTED;
		
	//仅仅考虑一个点
	xi	= (int)((dX - m_dCellLeft) / m_dCellSizeOfX);
	yi	= (int)((dY - m_dCellBottom) / m_dCellSizeOfY);

	if(xi < 0)				xi	= 0;
	if(yi < 0)				yi	= 0;
	if(xi >= m_nCellNumOfX)	xi	= m_nCellNumOfX-1;
	if(yi >= m_nCellNumOfY)	yi	= m_nCellNumOfY-1;

	ci	= yi * m_nCellNumOfX + xi;

	if(m_pCells[ci].m_pTEDHead == NULL)
	{
		pTED						= new	CDCR_TinEdgeAddress;	
		pTED->m_pEdge				= pEdge;

		m_pCells[ci].m_pTEDHead		= pTED;
		m_pCells[ci].m_pTEDLast		= pTED;
	}
	else
	{
		//bool	bFinded;
		//bFinded	= false;

		//pTED	= m_pCells[ci].m_pTEDHead;
		//while(pTED != NULL)
		//{
		//	if(pTED->m_pEdge == pEdge)
		//	{
		//		bFinded	= true;
		//		break;
		//	}

		//	pTED	= pTED->m_pNext;
		//}

		//if(bFinded)
		{
			pTED						= new	CDCR_TinEdgeAddress;	
			pTED->m_pEdge				= pEdge;

			m_pCells[ci].m_pTEDLast->m_pNext	= pTED;
			m_pCells[ci].m_pTEDLast				= pTED;
		}
	}

	return	true;	
}

//////////////////////////////////////////////////////////////////////
// 004 设置显示参数
//////////////////////////////////////////////////////////////////////
void CDCR_TinTDTACreator::SetCoordMapping(double dXA,double dXB,double dYA,double dYB)
{
	m_dMapXA	= dXA;
	m_dMapXB	= dXB;

	m_dMapYA	= dYA;
	m_dMapYB	= dYB;
}

//
void CDCR_TinTDTACreator::SetClientRect(CRect rRect)
{
	m_rRect	= rRect;
}
//////////////////////////////////////////////////////////////////////
// 005 绘制边
//////////////////////////////////////////////////////////////////////
void CDCR_TinTDTACreator::DrawEdge(CDC* pDC,CDCR_TinEdge *pEdge)
{
	if(pEdge != NULL)
	{
		int	nVertexID1,nVertexID2;
		nVertexID1	= pEdge->m_nVertexID[0];
		nVertexID2	= pEdge->m_nVertexID[1];

		double	dX1,dY1,dZ1;
		double	dX2,dY2,dZ2;

		GetVertexCoordFromID(nVertexID1,dX1,dY1,dZ1);
		GetVertexCoordFromID(nVertexID2,dX2,dY2,dZ2);
		
		POINT	ptPoints[2];

		ptPoints[0].x	= (int)(dX1*m_dMapXA + m_dMapXB + 0.5);
		ptPoints[0].y	= (int)(dY1*m_dMapYA + m_dMapYB + 0.5);

		ptPoints[1].x	= (int)(dX2*m_dMapXA + m_dMapXB + 0.5);
		ptPoints[1].y	= (int)(dY2*m_dMapYA + m_dMapYB + 0.5);
		
		pDC->Polyline(ptPoints,2);
	}
}

//////////////////////////////////////////////////////////////////////
// 006 绘制TIN
//////////////////////////////////////////////////////////////////////
void CDCR_TinTDTACreator::Draw(CDC *pDC)
{
	CDCR_TinTriangle	*pTriangle;

	pTriangle	= m_pTriangleHead;

	while(pTriangle != NULL)
	{
		POINT	ptPoints[4];
		for(int j=0;j<3;j++)
		{
			ptPoints[j].x	= (int)(pTriangle->m_dX[j]*m_dMapXA + m_dMapXB + 0.5);
			ptPoints[j].y	= (int)(pTriangle->m_dY[j]*m_dMapYA + m_dMapYB + 0.5);
		}
		ptPoints[3]	= ptPoints[0];

		pDC->Polyline(ptPoints,4);

		pTriangle	= pTriangle->m_pNext;
	}
}

//////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////
void CDCR_TinTDTACreator::ReleaseEdge()
{
	CDCR_TinEdge	*pEdge1,*pEdge2;

	if(m_pEdgeHead != NULL)
	{
		pEdge1	= pEdge2	= m_pEdgeHead;
		do
		{
			pEdge1		= pEdge1->m_pNext;
			delete	pEdge2;
			pEdge2		= pEdge1;
		}while(pEdge1 != NULL);

		m_pEdgeHead	= NULL;
		m_pEdgeLast	= NULL;
	}	
}

//////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////
void CDCR_TinTDTACreator::ReleaseTriangle()
{
	CDCR_TinTriangle	*pTriangle1,*pTriangle2;

	if(m_pTriangleHead != NULL)
	{
		pTriangle1	= pTriangle2	= m_pTriangleHead;
		do
		{
			pTriangle1		= pTriangle1->m_pNext;
			delete	pTriangle2;
			pTriangle2		= pTriangle1;
		}while(pTriangle1 != NULL);

		m_pTriangleHead	= NULL;
		m_pTriangleLast	= NULL;
	}	
}

//////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////
void CDCR_TinTDTACreator::ReleaseVertex()
{
	CDCR_TinVertex	*pVertex1,*pVertex2;

	if(m_pVertexHead != NULL)
	{
		pVertex1	= pVertex2	= m_pVertexHead;
		do
		{
			pVertex1		= pVertex1->m_pNext;
			delete	pVertex2;
			pVertex2		= pVertex1;
		}while(pVertex1 != NULL);

		m_pVertexHead	= NULL;
		m_pVertexLast	= NULL;
	}	
}

//////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////
void CDCR_TinTDTACreator::AddEdge(CDCR_TinEdge *pEdge)
{
	if(m_pEdgeHead == NULL)
	{
		m_pEdgeHead	= pEdge;
		m_pEdgeLast	= pEdge;
	}
	else
	{
		m_pEdgeLast->m_pNext	= pEdge;
		m_pEdgeLast				= pEdge;
	}

	m_iEdgeCount++;
}

//////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////
void CDCR_TinTDTACreator::AddTriangle(CDCR_TinTriangle *pTriangle)
{
	if(m_pTriangleHead == NULL)
	{
		m_pTriangleHead	= pTriangle;
		m_pTriangleLast	= pTriangle;
	}
	else
	{
		m_pTriangleLast->m_pNext	= pTriangle;
		m_pTriangleLast				= pTriangle;
	}

	m_iTriangleCount++;
}

//////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////
void CDCR_TinTDTACreator::CellBuild()
{
	int i,j,ci;

	//初始化
	if(m_pCells != NULL)
	{
		delete	[]m_pCells;
		m_pCells	= NULL;
	}
	
	//重新计算网格尺寸
	double	dSize;
	dSize	= sqrt( (m_dMapRight-m_dMapLeft)*(m_dMapTop-m_dMapBottom)/m_iVertexCount );
	m_dCellSizeOfX	= m_dCellSizeOfY	= 4*dSize/3;

	//先计算格网尺寸
	m_nCellNumOfX	= (int)((m_dMapRight-m_dMapLeft)/m_dCellSizeOfX) + 1;
	m_nCellNumOfY	= (int)((m_dMapTop-m_dMapBottom)/m_dCellSizeOfY) + 1;

	//然后外扩范围，重新划分网格划分网格范围
	m_dCellLeft		= m_dMapLeft - m_dCellSizeOfX/2;
	m_dCellRight	= m_dMapRight + m_dCellSizeOfX/2;
	m_dCellBottom	= m_dMapBottom - m_dCellSizeOfY/2;
	m_dCellTop		= m_dMapTop + m_dCellSizeOfY/2;

	//重新计算网格尺寸
	m_dCellSizeOfX	= (m_dCellRight-m_dCellLeft)/m_nCellNumOfX;
	m_dCellSizeOfY	= (m_dCellTop-m_dCellBottom)/m_nCellNumOfY;

	m_pCells		= new	CDCR_Cells[(m_nCellNumOfX*m_nCellNumOfY)];

	for(i=0;i<m_nCellNumOfY;i++)
	{
		for(j=0;j<m_nCellNumOfX;j++)
		{
			ci	= i*m_nCellNumOfX + j;
						
			m_pCells[ci].m_pPFDHead	= NULL;
			m_pCells[ci].m_pPFDLast	= NULL;
			m_pCells[ci].m_pLFDHead	= NULL;
			m_pCells[ci].m_pLFDLast	= NULL;
			m_pCells[ci].m_pTVDHead	= NULL;
			m_pCells[ci].m_pTVDLast	= NULL;
		}
	}

	//
	CDCR_TinVertex	*pVertex;

	pVertex	= m_pVertexHead;
	while(pVertex != NULL)
	{		
		//建立网格
		AddVertexToCells(pVertex);

		pVertex	= pVertex->m_pNext;
	}
}

//////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////
void CDCR_TinTDTACreator::SetMapExt(double dLeft,double dRight,double dBottom,double dTop)
{
	m_dMapLeft		= dLeft;
	m_dMapRight		= dRight;
	m_dMapBottom	= dBottom;
	m_dMapTop		= dTop;
}

//////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////
void CDCR_TinTDTACreator::CellReset()
{
	if(m_pCells != NULL)
	{
		int	iC;
		for(int j=0;j<m_nCellNumOfY;j++)
		{
			for(int	i=0;i<m_nCellNumOfX;i++)
			{
				iC	= j*m_nCellNumOfX + i;
							
				m_pCells[iC].m_bUsed	= false;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////
CDCR_TinEdge* CDCR_TinTDTACreator::GetEdgeFromCells(double dX, double dY, int iVertexID1, int iVertexID2)
{
	//找到dX、dY所在的格网
	int		I,J,iC;
	I	= (int)((dX - m_dCellLeft) / m_dCellSizeOfX);
	J	= (int)((dY - m_dCellBottom) / m_dCellSizeOfY);

	if(I < 0)				I	= 0;
	if(J < 0)				J	= 0;
	if(I >= m_nCellNumOfX)	I	= m_nCellNumOfX-1;
	if(J >= m_nCellNumOfY)	J	= m_nCellNumOfY-1;
	
	//
	iC	= J * m_nCellNumOfX + I;

	//
	CDCR_TinEdgeAddress	*pTED;

	pTED	= m_pCells[iC].m_pTEDHead;
	while(pTED != NULL)
	{
		if((pTED->m_pEdge->m_nVertexID[0]==iVertexID1&&pTED->m_pEdge->m_nVertexID[1]==iVertexID2) || (pTED->m_pEdge->m_nVertexID[0]==iVertexID2&&pTED->m_pEdge->m_nVertexID[1]==iVertexID1))
			return	pTED->m_pEdge;

		pTED	= pTED->m_pNext;
	}
	
	return	NULL;
}

//////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////
bool CDCR_TinTDTACreator::CaculateTerrainFactor(double& dMeanSlope,double& dMeanRoughness)
{
	if(m_pTriangleHead == NULL)	return	false;

	dMeanSlope		= 0;
	dMeanRoughness	= 0;

	int				iTriangleCount;

	iTriangleCount	= m_iTriangleCount;

	CDCR_TinTriangle	*pTriangle;

	pTriangle	= m_pTriangleHead;
	while(pTriangle != NULL)
	{
		int		iID1;
		double	dX1,dY1,dZ1;

		int		iID2;
		double	dX2,dY2,dZ2;

		int		iID3;
		double	dX3,dY3,dZ3;

		pTriangle->GetVertexCoordEx(iID1,dX1,dY1,dZ1,
									iID2,dX2,dY2,dZ2,
									iID3,dX3,dY3,dZ3);

		//一般不可能为〇
		double	dSlope;
		dSlope			= CaculateSlope(dX1,dY1,dZ1,dX2,dY2,dZ2,dX3,dY3,dZ3);
		
		double	dSurfaceArea;
		dSurfaceArea	= CaculateSurfaceArea(dX1,dY1,dZ1,dX2,dY2,dZ2,dX3,dY3,dZ3);
		
		double	dProjectionArea;
		dProjectionArea	= dSurfaceArea * cos(dSlope);

		//
		if(fabs(dProjectionArea) < 1e-5)
		{
			iTriangleCount--;
		}
		else
		{
			double	dRoughness;
			//假如投影面积为0，则表明是垂直，其粗糙度为0.
			if(fabs(dProjectionArea) < 1e-5)
				dRoughness		= 1;
			else
				dRoughness		= dSurfaceArea / dProjectionArea;
			
			//
			dMeanSlope		= dMeanSlope + dSlope;

			dMeanRoughness	= dMeanRoughness + dRoughness;
		}

		pTriangle	= pTriangle->m_pNext;
	}
	
	if(iTriangleCount == 0)
	{
		dMeanSlope		= 0;
		dMeanRoughness	= 1;
	}
	else
	{
		dMeanSlope		= dMeanSlope / m_iTriangleCount;

		dMeanRoughness	= dMeanRoughness / m_iTriangleCount;
	}

	return	true;
}

double CDCR_TinTDTACreator::CaculateSlope(double dX1,double dY1,double dZ1,double dX2,double dY2,double dZ2,double dX3,double dY3,double dZ3)
{
	double	dX12,dY12,dZ12;

	dX12	= dX2-dX1;
	dY12	= dY2-dY1;
	dZ12	= dZ2-dZ1;

	double	dX13,dY13,dZ13;
	dX13	= dX3-dX1;
	dY13	= dY3-dY1;
	dZ13	= dZ3-dZ1;

	double	nX,nY,nZ;
	//
	nX		=   dY12*dZ13 - dY13*dZ12;
	//
	nY		= -(dX12*dZ13 - dX13*dZ12);
	//
	nZ		=   dX12*dY13 - dX13*dY12;
	
	//如果为0，表明垂直
	if(fabs(nZ) < 1e-10)	return	90;
	
	//归一化
	nX		= nX / nZ;
	nY		= nY / nZ;

	double	dAngle;
	
	dAngle	= acos(1.0/(sqrt(nX*nX + nY*nY + 1)));
	
	return	dAngle;
}

double CDCR_TinTDTACreator::CaculateSurfaceArea(double dX1,double dY1,double dZ1,double dX2,double dY2,double dZ2,double dX3,double dY3,double dZ3)
{
	//
	double	dA,dB,dC,dP;
	dA			= sqrt((dX2-dX3)*(dX2-dX3) + (dY2-dY3)*(dY2-dY3) + (dZ2-dZ3)*(dZ2-dZ3));
	dB			= sqrt((dX1-dX3)*(dX1-dX3) + (dY1-dY3)*(dY1-dY3) + (dZ1-dZ3)*(dZ1-dZ3));
	dC			= sqrt((dX2-dX1)*(dX2-dX1) + (dY2-dY1)*(dY2-dY1) + (dZ2-dZ1)*(dZ2-dZ1));
	dP			= (dA+dB+dC)/2;
	
	return	sqrt(dP*(dP-dA)*(dP-dB)*(dP-dC));
}

//
CDCR_TinTriangle* CDCR_TinTDTACreator::GetTriangle()
{
	return	m_pTriangleHead;
}