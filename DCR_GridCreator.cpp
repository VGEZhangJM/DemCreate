// DCR_GridCreator.cpp: implementation of the CDCR_GridCreator class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DCR_GridCreator.h"

#include "DCR_GridVertexList.h"
#include "SetGridCreatorIDWDlg.h"
#include "SetGridCreatorRBFDlg.h"
#include "SetGridCreatorKRGDlg.h"
#include "SetGridCreatorPLYDlg.h"
#include "SetGridCreatorSPDDlg.h"

//#include "DrawTerrain.h"
#include "DCR_HeightSet.h"
#include "DCR_DemSet.h"

#include "matlib.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDCR_GridCreator::CDCR_GridCreator()
{
	//地图区域的范围
	m_dMapLeft		= m_dMapRight	= 0.0;
	m_dMapBottom	= m_dMapTop		= 0.0;	

	//规则格网DEM的格网数目
	m_iDemNumOfX	= m_iDemNumOfY	= 100;

	//规则格网DEM的格网尺寸
	m_dDemSizeOfX	= m_dDemSizeOfY	= 0.0;

	//
	m_dMaxHeight	= m_dMinHeight	= 0.0;

	//规则格网DEM的高程值
	m_pHeight		= NULL;

	//高程的方差值
	m_pHeightSD		= NULL;


	//导入CDCR_WorkSpace中的分区格网数据
	//分区格网数据
	m_pCells		= NULL;
	//
	//地图格网大小
	m_dCellSizeOfX	= 0;
	m_dCellSizeOfY	= 0;
	
	//地图分块数
	m_iCellNumOfX	= 0;
	m_iCellNumOfY	= 0;

	//划分网格范围
	m_dCellLeft		= m_dCellRight	= 0;
	m_dCellBottom	= m_dCellTop	= 0;

	//当为true时，m_dSearchRadius不起作用；
	//当为false时，m_dSearchRadius起作用；
	m_bGlobalSearch			= true;
	//搜索半径
	m_dSearchRadius			= 0;

	//搜索半径起作用了
	m_bSearchRadiusUsed		= false;
	//搜索半径起作用的次数
	m_iSearchRadiusUsed		= 0;

	//搜索方向：分为无方向、四方向、八方向，
	//但是一般使用的是四方向和八方向
	m_enumDirection			= FourDirection;
	
	//每一个搜索方向内的最多搜索点数
	m_iPointNumPreDirection	= 1;

	//搜索的总点数，这个参数仅仅在无方向时，起作用，
	//即总共需要搜索多少个最近的点数
	m_iPointNumInTotal		= 10;

	//显示参数
	m_dMapXA				= 1.0;
	m_dMapXB				= 0.0;
	m_dMapYA				= -1.0;	
	m_dMapYB				= 0.0;

	//
	m_bCaclRemainsError		= true;

	//
	m_dwStartTime			= 0;
	m_dwFinishTime			= 0;
}

CDCR_GridCreator::~CDCR_GridCreator()
{
	if(m_pHeight != NULL)
	{
		delete	[]m_pHeight;
		m_pHeight	= NULL;
	}	

	if(m_pHeightSD != NULL)
	{
		delete	[]m_pHeightSD;
		m_pHeightSD	= NULL;
	}
}

//////////////////////////////////////////////////////////////////////
//设置地图范围，即用于建模的
//////////////////////////////////////////////////////////////////////
void CDCR_GridCreator::SetMapExt(double dLeft,double dRight,double dBottom,double dTop)
{
	//地图区域的范围
	m_dMapLeft		= dLeft;
	m_dMapRight		= dRight;

	m_dMapBottom	= dBottom;
	m_dMapTop		= dTop;	

	if(m_dMapRight-m_dMapLeft > m_dMapTop-m_dMapBottom)
		m_dSearchRadius	= m_dMapRight - m_dMapLeft;
	else
		m_dSearchRadius	= m_dMapTop - m_dMapBottom;
}

void CDCR_GridCreator::GetMapExt(double& dLeft,double& dRight,double& dBottom,double& dTop)
{
	dLeft	= m_dMapLeft;
	dRight	= m_dMapRight;
	dBottom	= m_dMapBottom;
	dTop	= m_dMapTop;
}

//////////////////////////////////////////////////////////////////////
//设置规则格网DEM的格网数目
//////////////////////////////////////////////////////////////////////
void CDCR_GridCreator::SetDemCount(int iNumX,int iNumY)
{
	m_iDemNumOfX	= iNumX;
	m_iDemNumOfY	= iNumY;
	
	if(m_iDemNumOfX == 0 || m_iDemNumOfY == 0)	
		return;

	//计算格网尺寸
	m_dDemSizeOfX	= (m_dMapRight-m_dMapLeft)/(m_iDemNumOfX-1);
	m_dDemSizeOfY	= (m_dMapTop-m_dMapBottom)/(m_iDemNumOfY-1);	
}

void CDCR_GridCreator::GetDemCount(int& iNumX,int& iNumY)
{
	iNumX	= m_iDemNumOfX;
	iNumY	= m_iDemNumOfY;
}

//////////////////////////////////////////////////////////////////////
//设置规则格网DEM的格网尺寸
//////////////////////////////////////////////////////////////////////
void CDCR_GridCreator::SetDemSize(double dSizeX,double dSizeY)
{
	m_dDemSizeOfX	= dSizeX;
	m_dDemSizeOfY	= dSizeY;	
	
	if(m_dDemSizeOfX == 0 || m_dDemSizeOfY == 0)
		return;

	//计算格网的数目
	m_iDemNumOfX	= int((m_dMapRight-m_dMapLeft)/m_dDemSizeOfX)+1;
	m_iDemNumOfY	= int((m_dMapTop-m_dMapBottom)/m_dDemSizeOfY)+1;
}

void CDCR_GridCreator::GetDemSize(double& dSizeX,double& dSizeY)
{
	dSizeX	= m_dDemSizeOfX;
	dSizeY	= m_dDemSizeOfY;
}

//////////////////////////////////////////////////////////////////////
//原始数据的分区格网数据
//////////////////////////////////////////////////////////////////////
void CDCR_GridCreator::SetCell(CDCR_Cells *pCells)
{
	m_pCells	= pCells;
}

//////////////////////////////////////////////////////////////////////
//原始数据的分区格网数目
//////////////////////////////////////////////////////////////////////
void CDCR_GridCreator::SetCellCount(int iNumX,int iNumY)
{
	m_iCellNumOfX	= iNumX;
	m_iCellNumOfY	= iNumY;
}

//////////////////////////////////////////////////////////////////////
//原始数据的分区格网尺寸
//////////////////////////////////////////////////////////////////////
void CDCR_GridCreator::SetCellSize(double dSizeX,double dSizeY)
{
	m_dCellSizeOfX	= dSizeX;
	m_dCellSizeOfY	= dSizeY;
}

//////////////////////////////////////////////////////////////////////
//原始数据的分区格网范围，这和地图范围稍有差别
//////////////////////////////////////////////////////////////////////
void CDCR_GridCreator::SetCellExt(double dLeft,double dRight,double dBottom,double dTop)
{
	m_dCellLeft		= dLeft;
	m_dCellRight	= dRight;
	m_dCellBottom	= dBottom;
	m_dCellTop		= dTop;
}

//////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////
void CDCR_GridCreator::SetSampleCount(int iSampleCount)
{
	m_iSampleCount	= 	iSampleCount;
}

//////////////////////////////////////////////////////////////////////
// 01 获取两点间的距离
//////////////////////////////////////////////////////////////////////
double CDCR_GridCreator::GetDistance(double x1, double y1, double z1, double x2, double y2, double z2)
{
	double	dDistance;

	//dDistance	= sqrt( (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) + (z1-z2)*(z1-z2) );
	dDistance	= sqrt( (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) );

	return	dDistance;
}

//////////////////////////////////////////////////////////////////////
// 02 获取角度
//////////////////////////////////////////////////////////////////////
double CDCR_GridCreator::GetAngle(double dX, double dY)
{
	double	dAngle;

	if(fabs(dY) < 1e-10)
	{
		if(dX > 0.0)					dAngle	= 0.0;
		else							dAngle	= PI;
	}
	else	if(dY > 1e-10)
	{
		if(dX > 1e-10)					dAngle	= atan2(fabs(dY),fabs(dX));		
		else	if(fabs(dX) < 1e-10)	dAngle	= PI/2;
		else	if(dX < 1e-10)			dAngle	= PI-atan2(fabs(dY),fabs(dX));
	}
	else
	{
		if(dX > 1e-10)					dAngle	= PI*2-atan2(fabs(dY),fabs(dX));
		else	if(fabs(dX) < 1e-10)	dAngle	= PI*3/2;
		else	if(dX < 1e-10)			dAngle	= PI+atan2(fabs(dY),fabs(dX));
	}
	return	dAngle;
}

//////////////////////////////////////////////////////////////////////
//格网标志归零
//////////////////////////////////////////////////////////////////////
void CDCR_GridCreator::CellReset()
{
	if(m_pCells != NULL)
	{
		for(int i=0;i<m_iCellNumOfX*m_iCellNumOfY;i++)
		{
			m_pCells[i].m_bUsed	= false;
		}
	}
}

//
void CDCR_GridCreator::Release()
{
	//首先删除
	m_pGridVertexList.Release();
	
	if(m_pHeight != NULL)
	{
		 delete	[]m_pHeight;

		 m_pHeight	= NULL;
	}

	if(m_pHeightSD != NULL)
	{
		delete	[]m_pHeightSD;
		m_pHeightSD	= NULL;
	}

	//
	InitialHeight();
}

//
void CDCR_GridCreator::SetCoordMapping(double dXA,double dXB,double dYA,double dYB)
{
	m_dMapXA	= dXA;
	m_dMapXB	= dXB;
	m_dMapYA	= dYA;
	m_dMapYB	= dYB;
}

void CDCR_GridCreator::DrawVertex(CDC *pDC)
{
	CDCR_GridVertex	*pVertex;

	pVertex		= m_pGridVertexList.GetGridVertex();

	while(pVertex != NULL)
	{
		int		iX,iY;
		
		iX	= (int)(pVertex->m_dX*m_dMapXA + m_dMapXB + 0.5);
		iY	= (int)(pVertex->m_dY*m_dMapYA + m_dMapYB + 0.5);
		
		pDC->Ellipse(iX-3,iY-3,iX+3,iY+3);

		pVertex	= pVertex->m_pNext;
	}
}

void CDCR_GridCreator::DrawCell(CDC *pDC, double dX, double dY,double dZ)
{
	COLORREF	crColor;
	crColor		= CDCR_DemSet::Instance()->GetColor(dZ);

	//绘制
	double	dLeft,dRight,dBottom,dTop;
	dLeft	= dX - m_dDemSizeOfX/2;
	dRight	= dLeft + m_dDemSizeOfX;
	dBottom	= dY - m_dDemSizeOfY/2;
	dTop	= dBottom + m_dDemSizeOfY;
	
	CRect	rectCell;
	rectCell.left	= (int)(dLeft*m_dMapXA + m_dMapXB + 0.5);
	rectCell.bottom	= (int)(dBottom*m_dMapYA + m_dMapYB + 0.5);
	rectCell.right	= (int)(dRight*m_dMapXA + m_dMapXB + 0.5);
	rectCell.top	= (int)(dTop*m_dMapYA + m_dMapYB + 0.5);
	
	CBrush	*pNewBrush,*pOldBrush;
	pNewBrush	= new	CBrush;
	pNewBrush->CreateSolidBrush(crColor);
	pOldBrush	= pDC->SelectObject(pNewBrush);

	pDC->FillRect(rectCell,pNewBrush);	

	pDC->SelectObject(pOldBrush);
	delete	pNewBrush;
}

//
void CDCR_GridCreator::Draw(CDC *pDC)
{
	//DrawVertex(pDC);
}

//////////////////////////////////////////////////////////////////////
//搜索参与插值的采样点数据
//(dX,dY):为插值点
//返回值为bool
//////////////////////////////////////////////////////////////////////
bool CDCR_GridCreator::SearchSamplingPoint(double dX,double dY)
{
	if(m_pCells == NULL)	return	false;

	//首先：根据(dX,dY)确定插值点所在的格网
	int		iCurrentI,iCurrentJ;
	iCurrentI	= (int)((dX - m_dCellLeft) / m_dCellSizeOfX);
	iCurrentJ	= (int)((dY - m_dCellBottom) / m_dCellSizeOfY);

	//首先删除
	m_pGridVertexList.Release();

	//
	m_bSearchRadiusUsed	= false;

	CellReset();

	//假如是全局搜索
	//唯一有作用的是总点数：m_iPointNumPreDirection
	if(m_bGlobalSearch)
	{
		//临时数组
		CDCR_GridVertexList	pTempGridVertexList;

		//判断终止的条件是点数达到要求
		//但是至少运行一圈
		int		iK			= 1;
		bool	bFinished	= true;

		do
		{
			//
			int		iBegin,iEnd;
			int		jBegin,jEnd;
			
			iBegin	= iCurrentI - iK;		iEnd	= iCurrentI + iK;
			jBegin	= iCurrentJ - iK;		jEnd	= iCurrentJ + iK;
			
			if(iBegin < 0)					iBegin	= 0;
			if(iBegin >= m_iCellNumOfX-1)	iBegin	= m_iCellNumOfX-1;
			if(iEnd < 0)					iEnd	= 0;
			if(iEnd >= m_iCellNumOfX-1)		iEnd	= m_iCellNumOfX-1;

			if(jBegin < 0)					jBegin	= 0;
			if(jBegin >= m_iCellNumOfY-1)	jBegin	= m_iCellNumOfY-1;
			if(jEnd < 0)					jEnd	= 0;
			if(jEnd >= m_iCellNumOfY-1)		jEnd	= m_iCellNumOfY-1;

			for(int j=jBegin;j<=jEnd;j++)
			{
				for(int i=iBegin;i<=iEnd;i++)
				{
					//
					int		iI,iJ,iC;

					iI	= i;	iJ	= j;					
					iC	= iJ * m_iCellNumOfX + iI;

					//判断该格网是否被使用
					//没有使用的才能进行
					//在排除一些
					if(false == m_pCells[iC].m_bUsed)
					{
						//用过了
						m_pCells[iC].m_bUsed	= true;
							
						//提取点信息
						CDCR_PointFeatureAddress	*pPFD;
						pPFD		= m_pCells[iC].m_pPFDHead;
							
						while(pPFD != NULL)
						{
							CDCR_PointFeature	*pPoint;
							pPoint		= pPFD->m_pPointFeature;

							//没有被删除的点
							if(false == pPoint->m_bUsed)
							{
								//
								double				dDistance;	
								dDistance	= GetDistance(pPoint->m_pPointCoord.x,pPoint->m_pPointCoord.y,0,dX,dY,0);
								
								//
								pTempGridVertexList.AddGridVertex(pPoint->m_pPointCoord.x,pPoint->m_pPointCoord.y,pPoint->m_pPointCoord.z,dDistance,m_iPointNumInTotal);
							}

							pPFD	= pPFD->m_pNext;
						}
					}
				
				}//for(int i=iBegin;i<=iEnd;i++)

			}//for(int j=jBegin;j<=jEnd;j++)
			
			bFinished	= false;

			//如果大于规定的点数，则退出
			if(pTempGridVertexList.GetGridVertexCount() >= m_iPointNumInTotal)
				bFinished	= true;
			
			//通过iK计算距离
			//double	dMaxDistance;
			//if(m_dCellSizeOfX > m_dCellSizeOfY)
			//	dMaxDistance	= iK * m_dCellSizeOfY;
			//else	
			//	dMaxDistance	= iK * m_dCellSizeOfX;
			//如果搜索半径大于指定的最大半径，则退出
			//if(dMaxDistance > m_dSearchRadius)
			//	bFinished	= true;

			iK++;
			
			if(m_iCellNumOfX > m_iCellNumOfY)
			{
				if(iK > m_iCellNumOfX)	
					bFinished	= true;
			}
			else
			{	
				if(iK > m_iCellNumOfY)	
					bFinished	= true;
			}
		}while(!bFinished);	

		//上面
		//已经根据固定点数的数目已经搜索到足够的采样点了

		//下面
		//把pTempGridVertexList中的采样点根据“搜索半径”的限定放置到m_pGridVertexList中，
		//这中间就有一个距离的判断问题
		CDCR_GridVertex	*pVertex;
		pVertex	= pTempGridVertexList.GetGridVertex();
		while(pVertex != NULL)
		{
			//小于指定的搜索点数
			if(pVertex->m_dDistance < m_dSearchRadius)
			{
				m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);
			}
			else
			{
				//如果搜索半径起作用了
				//则把使用搜索半径限定的标志加上
				m_bSearchRadiusUsed		= true;
			}

			pVertex	= pVertex->m_pNext;
		}

		//最后
		//假如，起作用的没有采样点了，怎么办？
		//现在的解决方法是重新把原来的数据补充过来，这可能存在问题
		if(m_pGridVertexList.GetGridVertexCount() == 0)
		{
			pVertex	= pTempGridVertexList.GetGridVertex();
			while(pVertex != NULL)
			{
				m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);

				pVertex	= pVertex->m_pNext;
			}
		}
		else
		{
			if(m_bSearchRadiusUsed)		m_iSearchRadiusUsed++;
		}
	}
	else
	{
		//四方向搜索
		if(m_enumDirection == FourDirection)
		{
			//记录顶点信息
			//0-90
			CDCR_GridVertexList	pGridVertexList1;
			//90-180
			CDCR_GridVertexList	pGridVertexList2;
			//180-270
			CDCR_GridVertexList	pGridVertexList3;
			//270-360
			CDCR_GridVertexList	pGridVertexList4;
			
			//判断终止的条件是点数达到要求
			//但是至少运行一圈
			int		iK			= 1;
			bool	bFinished	= true;

			do
			{
				//
				int		iBegin,iEnd;
				int		jBegin,jEnd;
				
				iBegin	= iCurrentI - iK;		iEnd	= iCurrentI + iK;
				jBegin	= iCurrentJ - iK;		jEnd	= iCurrentJ + iK;

				if(iBegin < 0)					iBegin	= 0;
				if(iBegin >= m_iCellNumOfX-1)	iBegin	= m_iCellNumOfX-1;
				if(iEnd < 0)					iEnd	= 0;
				if(iEnd >= m_iCellNumOfX-1)		iEnd	= m_iCellNumOfX-1;

				if(jBegin < 0)					jBegin	= 0;
				if(jBegin >= m_iCellNumOfY-1)	jBegin	= m_iCellNumOfY-1;
				if(jEnd < 0)					jEnd	= 0;
				if(jEnd >= m_iCellNumOfY-1)		jEnd	= m_iCellNumOfY-1;

				for(int j=jBegin;j<=jEnd;j++)
				{
					for(int i=iBegin;i<=iEnd;i++)
					{
						//
						int		iI,iJ,iC;

						iI	= i;	iJ	= j;
						iC	= iJ * m_iCellNumOfX + iI;

						//判断该格网是否被使用
						//没有使用的才能进行
						if(false == m_pCells[iC].m_bUsed)
						{
							//用过了
							m_pCells[iC].m_bUsed	= true;
								
							//提取点信息
							CDCR_PointFeatureAddress	*pPFD;
							pPFD		= m_pCells[iC].m_pPFDHead;
								
							while(pPFD != NULL)
							{
								CDCR_PointFeature	*pPoint;
								pPoint		= pPFD->m_pPointFeature;

								//没有被删除的点
								if(false == pPoint->m_bUsed)
								{
									//首先判断角度
									double				dAngle;
									dAngle		= GetAngle(pPoint->m_pPointCoord.x-dX,pPoint->m_pPointCoord.y-dY);

									double				dDistance;	
									dDistance	= GetDistance(pPoint->m_pPointCoord.x,pPoint->m_pPointCoord.y,0,dX,dY,0);
									
									//if(dAngle >=PI/4 && dAngle < 3*PI/4)
									if(dAngle >=0 && dAngle < PI/2)
										pGridVertexList1.AddGridVertex(pPoint->m_pPointCoord.x,pPoint->m_pPointCoord.y,pPoint->m_pPointCoord.z,dDistance,m_iPointNumPreDirection);
									//else	if(dAngle>=3*PI/4 && dAngle < 5*PI/4)
									else	if(dAngle>=PI/2 && dAngle < PI)
										pGridVertexList2.AddGridVertex(pPoint->m_pPointCoord.x,pPoint->m_pPointCoord.y,pPoint->m_pPointCoord.z,dDistance,m_iPointNumPreDirection);
									//else	if(dAngle>=5*PI/4 && dAngle < 7*PI/4)
									else	if(dAngle>=PI && dAngle < 3*PI/2)
										pGridVertexList3.AddGridVertex(pPoint->m_pPointCoord.x,pPoint->m_pPointCoord.y,pPoint->m_pPointCoord.z,dDistance,m_iPointNumPreDirection);
									else
										pGridVertexList4.AddGridVertex(pPoint->m_pPointCoord.x,pPoint->m_pPointCoord.y,pPoint->m_pPointCoord.z,dDistance,m_iPointNumPreDirection);
								}

								pPFD	= pPFD->m_pNext;
							}
						}
					
					}//for(int i=iBegin;i<=iEnd;i++)

				}//for(int j=jBegin;j<=jEnd;j++)
				
				bFinished	= true;
				if(pGridVertexList1.GetGridVertexCount() < m_iPointNumPreDirection)
				{
					bFinished	= false;
				}

				if(bFinished)
				{
					if(pGridVertexList2.GetGridVertexCount() < m_iPointNumPreDirection)
					{
						bFinished	= false;
					}
				}

				if(bFinished)
				{
					if(pGridVertexList3.GetGridVertexCount() < m_iPointNumPreDirection)
					{
						bFinished	= false;
					}
				}

				if(bFinished)
				{
					if(pGridVertexList4.GetGridVertexCount() < m_iPointNumPreDirection)
					{
						bFinished	= false;
					}
				}

				//通过iK计算距离
				//double	dMaxDistance;
				//if(m_dCellSizeOfX > m_dCellSizeOfY)
				//	dMaxDistance	= iK * m_dCellSizeOfY;
				//else	
				//	dMaxDistance	= iK * m_dCellSizeOfX;
				//还有一个条件:半径
				//if(dMaxDistance > m_dSearchRadius)
				//	bFinished	= true;

				iK++;

				if(m_iCellNumOfX > m_iCellNumOfY)
				{
					if(iK > m_iCellNumOfX)	
						bFinished	= true;
				}
				else
				{	
					if(iK > m_iCellNumOfY)	
						bFinished	= true;
				}

			}while(!bFinished);	
			
			//把pTempGridVertexList中的数据根据搜索半径的限定放置到m_pGridVertexList中，
			//这中间就有一个距离的判断问题
			CDCR_GridVertex	*pVertex;

			//第一个方向
			pVertex	= pGridVertexList1.GetGridVertex();
			while(pVertex != NULL)
			{
				if(pVertex->m_dDistance < m_dSearchRadius)
				{
					m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);
				}
				else
				{
					//搜索半径起作用了
					m_bSearchRadiusUsed		= true;
				}

				pVertex	= pVertex->m_pNext;
			}

			//第二个方向
			pVertex	= pGridVertexList2.GetGridVertex();
			while(pVertex != NULL)
			{
				if(pVertex->m_dDistance < m_dSearchRadius)
				{
					m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);
				}
				else
				{
					//搜索半径起作用了
					m_bSearchRadiusUsed		= true;
				}

				pVertex	= pVertex->m_pNext;
			}
			
			//第三个方向
			pVertex	= pGridVertexList3.GetGridVertex();
			while(pVertex != NULL)
			{
				if(pVertex->m_dDistance < m_dSearchRadius)
				{
					m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);
				}
				else
				{
					//搜索半径起作用了
					m_bSearchRadiusUsed		= true;
				}

				pVertex	= pVertex->m_pNext;
			}

			//
			pVertex	= pGridVertexList4.GetGridVertex();
			while(pVertex != NULL)
			{
				if(pVertex->m_dDistance < m_dSearchRadius)
				{
					m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);
				}
				else
				{
					//搜索半径起作用了
					m_bSearchRadiusUsed		= true;
				}

				pVertex	= pVertex->m_pNext;
			}

			//假如，所有的采样点都被半径了，怎么办？
			//处理方法同上
			if(m_pGridVertexList.GetGridVertexCount() == 0)
			{
				//第一个方向
				pVertex	= pGridVertexList1.GetGridVertex();
				while(pVertex != NULL)
				{
					m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);

					pVertex	= pVertex->m_pNext;
				}

				//第二个方向
				pVertex	= pGridVertexList2.GetGridVertex();
				while(pVertex != NULL)
				{
					m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);

					pVertex	= pVertex->m_pNext;
				}
				
				//第三个方向
				pVertex	= pGridVertexList3.GetGridVertex();
				while(pVertex != NULL)
				{
					m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);

					pVertex	= pVertex->m_pNext;
				}

				//
				pVertex	= pGridVertexList4.GetGridVertex();
				while(pVertex != NULL)
				{
					m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);

					pVertex	= pVertex->m_pNext;
				}
			}
			else
			{
				if(m_bSearchRadiusUsed)		m_iSearchRadiusUsed++;
			}
		}

		//八方向搜索
		if(m_enumDirection == EightDirection)
		{
			//记录顶点信息
			//0-45
			CDCR_GridVertexList	pGridVertexList1;
			//45-90
			CDCR_GridVertexList	pGridVertexList2;
			//90-135
			CDCR_GridVertexList	pGridVertexList3;
			//135-180
			CDCR_GridVertexList	pGridVertexList4;
			//180-225
			CDCR_GridVertexList	pGridVertexList5;
			//225-270
			CDCR_GridVertexList	pGridVertexList6;
			//270-315
			CDCR_GridVertexList	pGridVertexList7;
			//315-360
			CDCR_GridVertexList	pGridVertexList8;
			
			//判断终止的条件是点数达到要求
			//但是至少运行一圈
			int		iK			= 1;
			
			bool	bFinished	= true;

			do
			{
				//
				int		iBegin,iEnd;
				int		jBegin,jEnd;
				
				iBegin	= iCurrentI - iK;		iEnd	= iCurrentI + iK;
				jBegin	= iCurrentJ - iK;		jEnd	= iCurrentJ + iK;

				if(iBegin < 0)					iBegin	= 0;
				if(iBegin >= m_iCellNumOfX-1)	iBegin	= m_iCellNumOfX-1;
				if(iEnd < 0)					iEnd	= 0;
				if(iEnd >= m_iCellNumOfX-1)		iEnd	= m_iCellNumOfX-1;

				if(jBegin < 0)					jBegin	= 0;
				if(jBegin >= m_iCellNumOfY-1)	jBegin	= m_iCellNumOfY-1;
				if(jEnd < 0)					jEnd	= 0;
				if(jEnd >= m_iCellNumOfY-1)		jEnd	= m_iCellNumOfY-1;

				for(int j=jBegin;j<=jEnd;j++)
				{
					for(int i=iBegin;i<=iEnd;i++)
					{
						//
						int		iI,iJ,iC;

						iI	= i;	iJ	= j;
						iC	= iJ * m_iCellNumOfX + iI;

						//判断该格网是否被使用
						//没有使用的才能进行
						if(false == m_pCells[iC].m_bUsed)
						{
							//用过了
							m_pCells[iC].m_bUsed	= true;

							//提取点信息
							CDCR_PointFeatureAddress	*pPFD;
							pPFD		= m_pCells[iC].m_pPFDHead;
								
							while(pPFD != NULL)
							{
								CDCR_PointFeature	*pPoint;
								pPoint		= pPFD->m_pPointFeature;

								//没有被删除的点
								if(false == pPoint->m_bUsed)
								{
									//首先判断角度
									double				dAngle;
									dAngle		= GetAngle(pPoint->m_pPointCoord.x-dX,pPoint->m_pPointCoord.y-dY);

									double				dDistance;	
									dDistance	= GetDistance(pPoint->m_pPointCoord.x,pPoint->m_pPointCoord.y,0,dX,dY,0);
									
									if(dAngle >=0 && dAngle < PI/4)
										pGridVertexList1.AddGridVertex(pPoint->m_pPointCoord.x,pPoint->m_pPointCoord.y,pPoint->m_pPointCoord.z,dDistance,m_iPointNumPreDirection);
									if(dAngle>=PI/4 && dAngle < PI/2)
										pGridVertexList2.AddGridVertex(pPoint->m_pPointCoord.x,pPoint->m_pPointCoord.y,pPoint->m_pPointCoord.z,dDistance,m_iPointNumPreDirection);
									if(dAngle>=PI/2 && dAngle < 3*PI/4)
										pGridVertexList3.AddGridVertex(pPoint->m_pPointCoord.x,pPoint->m_pPointCoord.y,pPoint->m_pPointCoord.z,dDistance,m_iPointNumPreDirection);
									if(dAngle>=3*PI/4 && dAngle < PI)
										pGridVertexList4.AddGridVertex(pPoint->m_pPointCoord.x,pPoint->m_pPointCoord.y,pPoint->m_pPointCoord.z,dDistance,m_iPointNumPreDirection);

									if(dAngle >=PI && dAngle < 5*PI/4)
										pGridVertexList5.AddGridVertex(pPoint->m_pPointCoord.x,pPoint->m_pPointCoord.y,pPoint->m_pPointCoord.z,dDistance,m_iPointNumPreDirection);
									if(dAngle>=5*PI/4 && dAngle < 3*PI/2)
										pGridVertexList6.AddGridVertex(pPoint->m_pPointCoord.x,pPoint->m_pPointCoord.y,pPoint->m_pPointCoord.z,dDistance,m_iPointNumPreDirection);
									if(dAngle>=3*PI/2 && dAngle < 7*PI/4)
										pGridVertexList7.AddGridVertex(pPoint->m_pPointCoord.x,pPoint->m_pPointCoord.y,pPoint->m_pPointCoord.z,dDistance,m_iPointNumPreDirection);
									if(dAngle>=7*PI/4 && dAngle < 2*PI)
										pGridVertexList8.AddGridVertex(pPoint->m_pPointCoord.x,pPoint->m_pPointCoord.y,pPoint->m_pPointCoord.z,dDistance,m_iPointNumPreDirection);
								}

								pPFD	= pPFD->m_pNext;
							}
						}
					
					}//for(int i=iBegin;i<=iEnd;i++)

				}//for(int j=jBegin;j<=jEnd;j++)
				
				bFinished	= true;
				if(pGridVertexList1.GetGridVertexCount() < m_iPointNumPreDirection)
				{
					bFinished	= false;
				}

				if(bFinished)
				{
					if(pGridVertexList2.GetGridVertexCount() < m_iPointNumPreDirection)
					{
						bFinished	= false;
					}
				}

				if(bFinished)
				{
					if(pGridVertexList3.GetGridVertexCount() < m_iPointNumPreDirection)
					{
						bFinished	= false;
					}
				}

				if(bFinished)
				{
					if(pGridVertexList4.GetGridVertexCount() < m_iPointNumPreDirection)
					{
						bFinished	= false;
					}
				}

				if(bFinished)
				{
					if(pGridVertexList5.GetGridVertexCount() < m_iPointNumPreDirection)
					{
						bFinished	= false;
					}
				}

				if(bFinished)
				{
					if(pGridVertexList6.GetGridVertexCount() < m_iPointNumPreDirection)
					{
						bFinished	= false;
					}
				}

				if(bFinished)
				{
					if(pGridVertexList7.GetGridVertexCount() < m_iPointNumPreDirection)
					{
						bFinished	= false;
					}
				}

				if(bFinished)
				{
					if(pGridVertexList8.GetGridVertexCount() < m_iPointNumPreDirection)
					{
						bFinished	= false;
					}
				}

				//通过iK计算距离
				//double	dMaxDistance;
				//if(m_dCellSizeOfX > m_dCellSizeOfY)
				//	dMaxDistance	= iK * m_dCellSizeOfY;
				//else	
				//	dMaxDistance	= iK * m_dCellSizeOfX;
				//还有一个条件:半径
				//if(dMaxDistance > m_dSearchRadius)
				//	bFinished	= true;

				iK++;

				if(m_iCellNumOfX > m_iCellNumOfY)
				{
					if(iK > m_iCellNumOfX)	
						bFinished	= true;
				}
				else
				{	
					if(iK > m_iCellNumOfY)	
						bFinished	= true;
				}
			}while(!bFinished);	

			//把pTempGridVertexList中的数据根据搜索半径的限定放置到m_pGridVertexList中，
			//这中间就有一个距离的判断问题
			CDCR_GridVertex	*pVertex;

			//第一个方向
			pVertex	= pGridVertexList1.GetGridVertex();
			while(pVertex != NULL)
			{
				if(pVertex->m_dDistance < m_dSearchRadius)
				{
					m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);
				}
				else
				{
					//搜索半径起作用了
					m_bSearchRadiusUsed		= true;
				}

				pVertex	= pVertex->m_pNext;
			}

			//第二个方向
			pVertex	= pGridVertexList2.GetGridVertex();
			while(pVertex != NULL)
			{
				if(pVertex->m_dDistance < m_dSearchRadius)
				{
					m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);
				}
				else
				{
					//搜索半径起作用了
					m_bSearchRadiusUsed		= true;
				}

				pVertex	= pVertex->m_pNext;
			}

			//第三个方向
			pVertex	= pGridVertexList3.GetGridVertex();
			while(pVertex != NULL)
			{
				if(pVertex->m_dDistance < m_dSearchRadius)
				{
					m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);
				}
				else
				{
					//搜索半径起作用了
					m_bSearchRadiusUsed		= true;
				}

				pVertex	= pVertex->m_pNext;
			}

			//第四个方向
			pVertex	= pGridVertexList4.GetGridVertex();
			while(pVertex != NULL)
			{
				if(pVertex->m_dDistance < m_dSearchRadius)
				{
					m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);
				}
				else
				{
					//搜索半径起作用了
					m_bSearchRadiusUsed		= true;
				}

				pVertex	= pVertex->m_pNext;
			}

			//第五个方向
			pVertex	= pGridVertexList5.GetGridVertex();
			while(pVertex != NULL)
			{
				if(pVertex->m_dDistance < m_dSearchRadius)
				{
					m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);
				}
				else
				{
					//搜索半径起作用了
					m_bSearchRadiusUsed		= true;
				}

				pVertex	= pVertex->m_pNext;
			}

			//第六个方向
			pVertex	= pGridVertexList6.GetGridVertex();
			while(pVertex != NULL)
			{
				if(pVertex->m_dDistance < m_dSearchRadius)
				{
					m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);
				}
				else
				{
					//搜索半径起作用了
					m_bSearchRadiusUsed		= true;
				}

				pVertex	= pVertex->m_pNext;
			}

			//第七个方向
			pVertex	= pGridVertexList7.GetGridVertex();
			while(pVertex != NULL)
			{
				if(pVertex->m_dDistance < m_dSearchRadius)
				{
					m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);
				}
				else
				{
					//搜索半径起作用了
					m_bSearchRadiusUsed		= true;
				}

				pVertex	= pVertex->m_pNext;
			}

			//第八个方向
			pVertex	= pGridVertexList8.GetGridVertex();
			while(pVertex != NULL)
			{
				if(pVertex->m_dDistance < m_dSearchRadius)
				{
					m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);
				}
				else
				{
					//搜索半径起作用了
					m_bSearchRadiusUsed		= true;
				}

				pVertex	= pVertex->m_pNext;
			}

			//假如，所有的采样点都被半径了，怎么办？
			if(m_pGridVertexList.GetGridVertexCount() == 0)
			{
				//第一个方向
				pVertex	= pGridVertexList1.GetGridVertex();
				while(pVertex != NULL)
				{
					m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);

					pVertex	= pVertex->m_pNext;
				}

				//第二个方向
				pVertex	= pGridVertexList2.GetGridVertex();
				while(pVertex != NULL)
				{
					m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);

					pVertex	= pVertex->m_pNext;
				}
				
				//第三个方向
				pVertex	= pGridVertexList3.GetGridVertex();
				while(pVertex != NULL)
				{
					m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);

					pVertex	= pVertex->m_pNext;
				}

				//
				pVertex	= pGridVertexList4.GetGridVertex();
				while(pVertex != NULL)
				{
					m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);

					pVertex	= pVertex->m_pNext;
				}
			}
			else
			{
				if(m_bSearchRadiusUsed)		m_iSearchRadiusUsed++;
			}
		}
	}

	//找到了，其实可以统一处理
	{
		//如果有限制边，还需要限制边
	}

	if(m_pGridVertexList.GetGridVertexCount() == 0)	return	false;

	return	true;
}

//////////////////////////////////////////////////////////////////////
//使用交叉验证计算精度值
//CDCR_PointFeature *pDelPoint：	为删除的需要重新插值的点
//////////////////////////////////////////////////////////////////////
bool CDCR_GridCreator::SearchSamplingPointCV(CDCR_PointFeature *pDelPoint)
{
	if(pDelPoint == NULL)	return	false;
	
	if(m_pCells == NULL)	return	false;

	double	dX,dY;
	dX	= pDelPoint->m_pPointCoord.x;
	dY	= pDelPoint->m_pPointCoord.y;

	//首先：根据(dX,dY)确定插值点所在的格网
	int		iCurrentI,iCurrentJ;
	iCurrentI	= (int)((dX - m_dCellLeft) / m_dCellSizeOfX);
	iCurrentJ	= (int)((dY - m_dCellBottom) / m_dCellSizeOfY);
	
	//首先删除
	m_pGridVertexList.Release();

	m_bSearchRadiusUsed	= false;

	CellReset();

	//假如是全局搜索
	//唯一有作用的是总点数：m_iPointNumPreDirection
	if(m_bGlobalSearch)
	{
		//临时数组
		CDCR_GridVertexList	pTempGridVertexList;

		//判断终止的条件是点数达到要求
		//但是至少运行一圈
		int		iK			= 1;
		bool	bFinished	= true;

		do
		{
			//
			int		iBegin,iEnd;
			int		jBegin,jEnd;
			
			iBegin	= iCurrentI - iK;		iEnd	= iCurrentI + iK;
			jBegin	= iCurrentJ - iK;		jEnd	= iCurrentJ + iK;
			
			if(iBegin < 0)					iBegin	= 0;
			if(iBegin >= m_iCellNumOfX-1)	iBegin	= m_iCellNumOfX-1;
			if(iEnd < 0)					iEnd	= 0;
			if(iEnd >= m_iCellNumOfX-1)		iEnd	= m_iCellNumOfX-1;

			if(jBegin < 0)					jBegin	= 0;
			if(jBegin >= m_iCellNumOfY-1)	jBegin	= m_iCellNumOfY-1;
			if(jEnd < 0)					jEnd	= 0;
			if(jEnd >= m_iCellNumOfY-1)		jEnd	= m_iCellNumOfY-1;

			for(int j=jBegin;j<=jEnd;j++)
			{
				for(int i=iBegin;i<=iEnd;i++)
				{
					//
					int		iI,iJ,iC;

					iI	= i;	iJ	= j;					
					iC	= iJ * m_iCellNumOfX + iI;

					//判断该格网是否被使用
					//没有使用的才能进行
					//在排除一些
					if(false == m_pCells[iC].m_bUsed)
					{
						//用过了
						m_pCells[iC].m_bUsed	= true;
							
						//提取点信息
						CDCR_PointFeatureAddress	*pPFD;
						pPFD		= m_pCells[iC].m_pPFDHead;
							
						while(pPFD != NULL)
						{
							CDCR_PointFeature	*pPoint;
							pPoint		= pPFD->m_pPointFeature;

							//在加上不能和删除的点重复这一条件
							if(pPoint != pDelPoint)
							{
								//被删除的点不能用
								if(false == pPoint->m_bUsed)
								{
									//
									double				dDistance;	
									dDistance	= GetDistance(pPoint->m_pPointCoord.x,pPoint->m_pPointCoord.y,0,dX,dY,0);
									
									//
									pTempGridVertexList.AddGridVertex(pPoint->m_pPointCoord.x,pPoint->m_pPointCoord.y,pPoint->m_pPointCoord.z,dDistance,m_iPointNumInTotal);
								}
							}

							pPFD	= pPFD->m_pNext;
						}
					}
				
				}//for(int i=iBegin;i<=iEnd;i++)

			}//for(int j=jBegin;j<=jEnd;j++)
			
			bFinished	= false;

			//如果大于规定的点数，则退出
			if(pTempGridVertexList.GetGridVertexCount() >= m_iPointNumInTotal)
				bFinished	= true;
			
			//通过iK计算距离
			//double	dMaxDistance;
			//if(m_dCellSizeOfX > m_dCellSizeOfY)
			//	dMaxDistance	= iK * m_dCellSizeOfY;
			//else	
			//	dMaxDistance	= iK * m_dCellSizeOfX;
			//如果搜索半径大于指定的最大半径，则退出
			//if(dMaxDistance > m_dSearchRadius)
			//	bFinished	= true;

			iK++;
			
			if(m_iCellNumOfX > m_iCellNumOfY)
			{
				if(iK > m_iCellNumOfX)	
					bFinished	= true;
			}
			else
			{	
				if(iK > m_iCellNumOfY)	
					bFinished	= true;
			}
		}while(!bFinished);	

		//把pTempGridVertexList中的数据根据搜索半径的限定放置到m_pGridVertexList中，
		//这中间就有一个距离的判断问题
		CDCR_GridVertex	*pVertex;
		pVertex	= pTempGridVertexList.GetGridVertex();
		while(pVertex != NULL)
		{
			if(pVertex->m_dDistance < m_dSearchRadius)
			{
				m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);
			}
			else
			{
				//搜索半径起作用了
				m_bSearchRadiusUsed		= true;
			}

			pVertex	= pVertex->m_pNext;
		}

		//假如，起作用的没有采样点了，怎么办？
		//处理同上
		if(m_pGridVertexList.GetGridVertexCount() == 0)
		{
			pVertex	= pTempGridVertexList.GetGridVertex();
			while(pVertex != NULL)
			{
				m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);

				pVertex	= pVertex->m_pNext;
			}
		}
		else
		{
			if(m_bSearchRadiusUsed)		m_iSearchRadiusUsed++;
		}
	}
	else
	{
		//四方向搜索
		if(m_enumDirection == FourDirection)
		{
			//记录顶点信息
			//0-90
			CDCR_GridVertexList	pGridVertexList1;
			//90-180
			CDCR_GridVertexList	pGridVertexList2;
			//180-270
			CDCR_GridVertexList	pGridVertexList3;
			//270-360
			CDCR_GridVertexList	pGridVertexList4;
			
			//判断终止的条件是点数达到要求
			//但是至少运行一圈
			int		iK			= 1;
			bool	bFinished	= true;

			do
			{
				//
				int		iBegin,iEnd;
				int		jBegin,jEnd;
				
				iBegin	= iCurrentI - iK;		iEnd	= iCurrentI + iK;
				jBegin	= iCurrentJ - iK;		jEnd	= iCurrentJ + iK;

				if(iBegin < 0)					iBegin	= 0;
				if(iBegin >= m_iCellNumOfX-1)	iBegin	= m_iCellNumOfX-1;
				if(iEnd < 0)					iEnd	= 0;
				if(iEnd >= m_iCellNumOfX-1)		iEnd	= m_iCellNumOfX-1;

				if(jBegin < 0)					jBegin	= 0;
				if(jBegin >= m_iCellNumOfY-1)	jBegin	= m_iCellNumOfY-1;
				if(jEnd < 0)					jEnd	= 0;
				if(jEnd >= m_iCellNumOfY-1)		jEnd	= m_iCellNumOfY-1;

				for(int j=jBegin;j<=jEnd;j++)
				{
					for(int i=iBegin;i<=iEnd;i++)
					{
						//
						int		iI,iJ,iC;

						iI	= i;	iJ	= j;
						iC	= iJ * m_iCellNumOfX + iI;

						//判断该格网是否被使用
						//没有使用的才能进行
						if(false == m_pCells[iC].m_bUsed)
						{
							//用过了
							m_pCells[iC].m_bUsed	= true;
								
							//提取点信息
							CDCR_PointFeatureAddress	*pPFD;
							pPFD		= m_pCells[iC].m_pPFDHead;
								
							while(pPFD != NULL)
							{
								CDCR_PointFeature	*pPoint;
								pPoint		= pPFD->m_pPointFeature;

								//交叉验证中已经删除的点不能用
								if(pPoint != pDelPoint)
								{
									//没有被删除的点
									if(false == pPoint->m_bUsed)
									{
										//首先判断角度
										double				dAngle;
										dAngle		= GetAngle(pPoint->m_pPointCoord.x-dX,pPoint->m_pPointCoord.y-dY);

										double				dDistance;	
										dDistance	= GetDistance(pPoint->m_pPointCoord.x,pPoint->m_pPointCoord.y,0,dX,dY,0);
										
										//if(dAngle >=PI/4 && dAngle < 3*PI/4)
										if(dAngle >=0 && dAngle < PI/2)
											pGridVertexList1.AddGridVertex(pPoint->m_pPointCoord.x,pPoint->m_pPointCoord.y,pPoint->m_pPointCoord.z,dDistance,m_iPointNumPreDirection);
										//else	if(dAngle>=3*PI/4 && dAngle < 5*PI/4)
										else	if(dAngle>=PI/2 && dAngle < PI)
											pGridVertexList2.AddGridVertex(pPoint->m_pPointCoord.x,pPoint->m_pPointCoord.y,pPoint->m_pPointCoord.z,dDistance,m_iPointNumPreDirection);
										//else	if(dAngle>=5*PI/4 && dAngle < 7*PI/4)
										else	if(dAngle>=PI && dAngle < 3*PI/2)
											pGridVertexList3.AddGridVertex(pPoint->m_pPointCoord.x,pPoint->m_pPointCoord.y,pPoint->m_pPointCoord.z,dDistance,m_iPointNumPreDirection);
										else
											pGridVertexList4.AddGridVertex(pPoint->m_pPointCoord.x,pPoint->m_pPointCoord.y,pPoint->m_pPointCoord.z,dDistance,m_iPointNumPreDirection);
									}
								}

								pPFD	= pPFD->m_pNext;
							}
						}
					
					}//for(int i=iBegin;i<=iEnd;i++)

				}//for(int j=jBegin;j<=jEnd;j++)
				
				bFinished	= true;
				if(pGridVertexList1.GetGridVertexCount() < m_iPointNumPreDirection)
				{
					bFinished	= false;
				}

				if(bFinished)
				{
					if(pGridVertexList2.GetGridVertexCount() < m_iPointNumPreDirection)
					{
						bFinished	= false;
					}
				}

				if(bFinished)
				{
					if(pGridVertexList3.GetGridVertexCount() < m_iPointNumPreDirection)
					{
						bFinished	= false;
					}
				}

				if(bFinished)
				{
					if(pGridVertexList4.GetGridVertexCount() < m_iPointNumPreDirection)
					{
						bFinished	= false;
					}
				}

				//通过iK计算距离
				//double	dMaxDistance;
				//if(m_dCellSizeOfX > m_dCellSizeOfY)
				//	dMaxDistance	= iK * m_dCellSizeOfY;
				//else	
				//	dMaxDistance	= iK * m_dCellSizeOfX;
				//还有一个条件:半径
				//if(dMaxDistance > m_dSearchRadius)
				//	bFinished	= true;

				iK++;

				if(m_iCellNumOfX > m_iCellNumOfY)
				{
					if(iK > m_iCellNumOfX)	
						bFinished	= true;
				}
				else
				{	
					if(iK > m_iCellNumOfY)	
						bFinished	= true;
				}

			}while(!bFinished);	
			
			//把pTempGridVertexList中的数据根据搜索半径的限定放置到m_pGridVertexList中，
			//这中间就有一个距离的判断问题
			CDCR_GridVertex	*pVertex;

			//第一个方向
			pVertex	= pGridVertexList1.GetGridVertex();
			while(pVertex != NULL)
			{
				if(pVertex->m_dDistance < m_dSearchRadius)
				{
					m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);
				}
				else
				{
					//搜索半径起作用了
					m_bSearchRadiusUsed		= true;
				}

				pVertex	= pVertex->m_pNext;
			}

			//第二个方向
			pVertex	= pGridVertexList2.GetGridVertex();
			while(pVertex != NULL)
			{
				if(pVertex->m_dDistance < m_dSearchRadius)
				{
					m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);
				}
				else
				{
					//搜索半径起作用了
					m_bSearchRadiusUsed		= true;
				}

				pVertex	= pVertex->m_pNext;
			}
			
			//第三个方向
			pVertex	= pGridVertexList3.GetGridVertex();
			while(pVertex != NULL)
			{
				if(pVertex->m_dDistance < m_dSearchRadius)
				{
					m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);
				}
				else
				{
					//搜索半径起作用了
					m_bSearchRadiusUsed		= true;
				}

				pVertex	= pVertex->m_pNext;
			}

			//
			pVertex	= pGridVertexList4.GetGridVertex();
			while(pVertex != NULL)
			{
				if(pVertex->m_dDistance < m_dSearchRadius)
				{
					m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);
				}
				else
				{
					//搜索半径起作用了
					m_bSearchRadiusUsed		= true;
				}

				pVertex	= pVertex->m_pNext;
			}

			//假如，所有的采样点都被半径了，怎么办？
			if(m_pGridVertexList.GetGridVertexCount() == 0)
			{
				//第一个方向
				pVertex	= pGridVertexList1.GetGridVertex();
				while(pVertex != NULL)
				{
					m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);

					pVertex	= pVertex->m_pNext;
				}

				//第二个方向
				pVertex	= pGridVertexList2.GetGridVertex();
				while(pVertex != NULL)
				{
					m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);

					pVertex	= pVertex->m_pNext;
				}
				
				//第三个方向
				pVertex	= pGridVertexList3.GetGridVertex();
				while(pVertex != NULL)
				{
					m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);

					pVertex	= pVertex->m_pNext;
				}

				//
				pVertex	= pGridVertexList4.GetGridVertex();
				while(pVertex != NULL)
				{
					m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);

					pVertex	= pVertex->m_pNext;
				}
			}
			else
			{
				if(m_bSearchRadiusUsed)		m_iSearchRadiusUsed++;
			}
		}

		//八方向搜索
		if(m_enumDirection == EightDirection)
		{
			//记录顶点信息
			//0-45
			CDCR_GridVertexList	pGridVertexList1;
			//45-90
			CDCR_GridVertexList	pGridVertexList2;
			//90-135
			CDCR_GridVertexList	pGridVertexList3;
			//135-180
			CDCR_GridVertexList	pGridVertexList4;
			//180-225
			CDCR_GridVertexList	pGridVertexList5;
			//225-270
			CDCR_GridVertexList	pGridVertexList6;
			//270-315
			CDCR_GridVertexList	pGridVertexList7;
			//315-360
			CDCR_GridVertexList	pGridVertexList8;
			
			//判断终止的条件是点数达到要求
			//但是至少运行一圈
			int		iK			= 1;
			
			bool	bFinished	= true;

			do
			{
				//
				int		iBegin,iEnd;
				int		jBegin,jEnd;
				
				iBegin	= iCurrentI - iK;		iEnd	= iCurrentI + iK;
				jBegin	= iCurrentJ - iK;		jEnd	= iCurrentJ + iK;

				if(iBegin < 0)					iBegin	= 0;
				if(iBegin >= m_iCellNumOfX-1)	iBegin	= m_iCellNumOfX-1;
				if(iEnd < 0)					iEnd	= 0;
				if(iEnd >= m_iCellNumOfX-1)		iEnd	= m_iCellNumOfX-1;

				if(jBegin < 0)					jBegin	= 0;
				if(jBegin >= m_iCellNumOfY-1)	jBegin	= m_iCellNumOfY-1;
				if(jEnd < 0)					jEnd	= 0;
				if(jEnd >= m_iCellNumOfY-1)		jEnd	= m_iCellNumOfY-1;

				for(int j=jBegin;j<=jEnd;j++)
				{
					for(int i=iBegin;i<=iEnd;i++)
					{
						//
						int		iI,iJ,iC;

						iI	= i;	iJ	= j;
						iC	= iJ * m_iCellNumOfX + iI;

						//判断该格网是否被使用
						//没有使用的才能进行
						if(false == m_pCells[iC].m_bUsed)
						{
							//用过了
							m_pCells[iC].m_bUsed	= true;

							//提取点信息
							CDCR_PointFeatureAddress	*pPFD;
							pPFD		= m_pCells[iC].m_pPFDHead;
								
							while(pPFD != NULL)
							{
								CDCR_PointFeature	*pPoint;
								pPoint		= pPFD->m_pPointFeature;

								//交叉验证中已经被删除的点不能用
								if(pPoint != pDelPoint)
								{
									//没有被删除的点
									if(false == pPoint->m_bUsed)
									{
										//首先判断角度
										double				dAngle;
										dAngle		= GetAngle(pPoint->m_pPointCoord.x-dX,pPoint->m_pPointCoord.y-dY);

										double				dDistance;	
										dDistance	= GetDistance(pPoint->m_pPointCoord.x,pPoint->m_pPointCoord.y,0,dX,dY,0);
										
										if(dAngle >=0 && dAngle < PI/4)
											pGridVertexList1.AddGridVertex(pPoint->m_pPointCoord.x,pPoint->m_pPointCoord.y,pPoint->m_pPointCoord.z,dDistance,m_iPointNumPreDirection);
										if(dAngle>=PI/4 && dAngle < PI/2)
											pGridVertexList2.AddGridVertex(pPoint->m_pPointCoord.x,pPoint->m_pPointCoord.y,pPoint->m_pPointCoord.z,dDistance,m_iPointNumPreDirection);
										if(dAngle>=PI/2 && dAngle < 3*PI/4)
											pGridVertexList3.AddGridVertex(pPoint->m_pPointCoord.x,pPoint->m_pPointCoord.y,pPoint->m_pPointCoord.z,dDistance,m_iPointNumPreDirection);
										if(dAngle>=3*PI/4 && dAngle < PI)
											pGridVertexList4.AddGridVertex(pPoint->m_pPointCoord.x,pPoint->m_pPointCoord.y,pPoint->m_pPointCoord.z,dDistance,m_iPointNumPreDirection);

										if(dAngle >=PI && dAngle < 5*PI/4)
											pGridVertexList5.AddGridVertex(pPoint->m_pPointCoord.x,pPoint->m_pPointCoord.y,pPoint->m_pPointCoord.z,dDistance,m_iPointNumPreDirection);
										if(dAngle>=5*PI/4 && dAngle < 3*PI/2)
											pGridVertexList6.AddGridVertex(pPoint->m_pPointCoord.x,pPoint->m_pPointCoord.y,pPoint->m_pPointCoord.z,dDistance,m_iPointNumPreDirection);
										if(dAngle>=3*PI/2 && dAngle < 7*PI/4)
											pGridVertexList7.AddGridVertex(pPoint->m_pPointCoord.x,pPoint->m_pPointCoord.y,pPoint->m_pPointCoord.z,dDistance,m_iPointNumPreDirection);
										if(dAngle>=7*PI/4 && dAngle < 2*PI)
											pGridVertexList8.AddGridVertex(pPoint->m_pPointCoord.x,pPoint->m_pPointCoord.y,pPoint->m_pPointCoord.z,dDistance,m_iPointNumPreDirection);
									}
								}

								pPFD	= pPFD->m_pNext;
							}
						}
					
					}//for(int i=iBegin;i<=iEnd;i++)

				}//for(int j=jBegin;j<=jEnd;j++)
				
				bFinished	= true;
				if(pGridVertexList1.GetGridVertexCount() < m_iPointNumPreDirection)
				{
					bFinished	= false;
				}

				if(bFinished)
				{
					if(pGridVertexList2.GetGridVertexCount() < m_iPointNumPreDirection)
					{
						bFinished	= false;
					}
				}

				if(bFinished)
				{
					if(pGridVertexList3.GetGridVertexCount() < m_iPointNumPreDirection)
					{
						bFinished	= false;
					}
				}

				if(bFinished)
				{
					if(pGridVertexList4.GetGridVertexCount() < m_iPointNumPreDirection)
					{
						bFinished	= false;
					}
				}

				if(bFinished)
				{
					if(pGridVertexList5.GetGridVertexCount() < m_iPointNumPreDirection)
					{
						bFinished	= false;
					}
				}

				if(bFinished)
				{
					if(pGridVertexList6.GetGridVertexCount() < m_iPointNumPreDirection)
					{
						bFinished	= false;
					}
				}

				if(bFinished)
				{
					if(pGridVertexList7.GetGridVertexCount() < m_iPointNumPreDirection)
					{
						bFinished	= false;
					}
				}

				if(bFinished)
				{
					if(pGridVertexList8.GetGridVertexCount() < m_iPointNumPreDirection)
					{
						bFinished	= false;
					}
				}

				//通过iK计算距离
				//double	dMaxDistance;
				//if(m_dCellSizeOfX > m_dCellSizeOfY)
				//	dMaxDistance	= iK * m_dCellSizeOfY;
				//else	
				//	dMaxDistance	= iK * m_dCellSizeOfX;
				//还有一个条件:半径
				//if(dMaxDistance > m_dSearchRadius)
				//	bFinished	= true;

				iK++;

				if(m_iCellNumOfX > m_iCellNumOfY)
				{
					if(iK > m_iCellNumOfX)	
						bFinished	= true;
				}
				else
				{	
					if(iK > m_iCellNumOfY)	
						bFinished	= true;
				}
			}while(!bFinished);	

			//把pTempGridVertexList中的数据根据搜索半径的限定放置到m_pGridVertexList中，
			//这中间就有一个距离的判断问题
			CDCR_GridVertex	*pVertex;

			//第一个方向
			pVertex	= pGridVertexList1.GetGridVertex();
			while(pVertex != NULL)
			{
				if(pVertex->m_dDistance < m_dSearchRadius)
				{
					m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);
				}
				else
				{
					//搜索半径起作用了
					m_bSearchRadiusUsed		= true;
				}

				pVertex	= pVertex->m_pNext;
			}

			//第二个方向
			pVertex	= pGridVertexList2.GetGridVertex();
			while(pVertex != NULL)
			{
				if(pVertex->m_dDistance < m_dSearchRadius)
				{
					m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);
				}
				else
				{
					//搜索半径起作用了
					m_bSearchRadiusUsed		= true;
				}

				pVertex	= pVertex->m_pNext;
			}

			//第三个方向
			pVertex	= pGridVertexList3.GetGridVertex();
			while(pVertex != NULL)
			{
				if(pVertex->m_dDistance < m_dSearchRadius)
				{
					m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);
				}
				else
				{
					//搜索半径起作用了
					m_bSearchRadiusUsed		= true;
				}

				pVertex	= pVertex->m_pNext;
			}

			//第四个方向
			pVertex	= pGridVertexList4.GetGridVertex();
			while(pVertex != NULL)
			{
				if(pVertex->m_dDistance < m_dSearchRadius)
				{
					m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);
				}
				else
				{
					//搜索半径起作用了
					m_bSearchRadiusUsed		= true;
				}

				pVertex	= pVertex->m_pNext;
			}

			//第五个方向
			pVertex	= pGridVertexList5.GetGridVertex();
			while(pVertex != NULL)
			{
				if(pVertex->m_dDistance < m_dSearchRadius)
				{
					m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);
				}
				else
				{
					//搜索半径起作用了
					m_bSearchRadiusUsed		= true;
				}

				pVertex	= pVertex->m_pNext;
			}

			//第六个方向
			pVertex	= pGridVertexList6.GetGridVertex();
			while(pVertex != NULL)
			{
				if(pVertex->m_dDistance < m_dSearchRadius)
				{
					m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);
				}
				else
				{
					//搜索半径起作用了
					m_bSearchRadiusUsed		= true;
				}

				pVertex	= pVertex->m_pNext;
			}

			//第七个方向
			pVertex	= pGridVertexList7.GetGridVertex();
			while(pVertex != NULL)
			{
				if(pVertex->m_dDistance < m_dSearchRadius)
				{
					m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);
				}
				else
				{
					//搜索半径起作用了
					m_bSearchRadiusUsed		= true;
				}

				pVertex	= pVertex->m_pNext;
			}

			//第八个方向
			pVertex	= pGridVertexList8.GetGridVertex();
			while(pVertex != NULL)
			{
				if(pVertex->m_dDistance < m_dSearchRadius)
				{
					m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);
				}
				else
				{
					//搜索半径起作用了
					m_bSearchRadiusUsed		= true;
				}

				pVertex	= pVertex->m_pNext;
			}

			//假如，所有的采样点都被半径了，怎么办？
			if(m_pGridVertexList.GetGridVertexCount() == 0)
			{
				//第一个方向
				pVertex	= pGridVertexList1.GetGridVertex();
				while(pVertex != NULL)
				{
					m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);

					pVertex	= pVertex->m_pNext;
				}

				//第二个方向
				pVertex	= pGridVertexList2.GetGridVertex();
				while(pVertex != NULL)
				{
					m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);

					pVertex	= pVertex->m_pNext;
				}
				
				//第三个方向
				pVertex	= pGridVertexList3.GetGridVertex();
				while(pVertex != NULL)
				{
					m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);

					pVertex	= pVertex->m_pNext;
				}

				//
				pVertex	= pGridVertexList4.GetGridVertex();
				while(pVertex != NULL)
				{
					m_pGridVertexList.AddGridVertexEx(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ,pVertex->m_dDistance);

					pVertex	= pVertex->m_pNext;
				}
			}
			else
			{
				if(m_bSearchRadiusUsed)		m_iSearchRadiusUsed++;
			}
		}
	}

	//找到了，其实可以统一处理
	{
		//如果有限制边，还需要限制边
	}

	if(m_pGridVertexList.GetGridVertexCount() == 0)	return	false;

	return	true;
}

//////////////////////////////////////////////////////////////////////
//插值
//////////////////////////////////////////////////////////////////////
void CDCR_GridCreator::Create(CDC *pDC, int iType)
{
	//距离加权
	if(iType == 0)
	{
		//
		CSetGridCreatorIDWDlg	pDlg;
		
		pDlg.m_dMapLeft		= m_dMapLeft;
		pDlg.m_dMapRight	= m_dMapRight;
		pDlg.m_dMapBottom	= m_dMapBottom;
		pDlg.m_dMapTop		= m_dMapTop;

		pDlg.m_iDemNumX		= m_iDemNumOfX;
		pDlg.m_iDemNumY		= m_iDemNumOfY;

		//权指数
		pDlg.m_iU			= m_pGridIDW.m_iU;
		//平滑指数
		pDlg.m_dT			= m_pGridIDW.m_dT;
		//
		pDlg.m_szPathName	= m_pGridIDW.m_szPathName;

		if(m_bCaclRemainsError)		pDlg.m_bCheckRE		= TRUE;
		else						pDlg.m_bCheckRE		= FALSE;

		if(pDlg.DoModal() == IDOK)
		{

		}
		else	return;

		//
		if(pDlg.m_bCheckRE)	m_bCaclRemainsError	= true;
		else				m_bCaclRemainsError	= false;

		//调用创建函数
		CreateInIDW(

			pDC,
			pDlg.m_iDemNumX,
			pDlg.m_iDemNumY,
			pDlg.m_szPathName,
			pDlg.m_iSearchType,
			pDlg.m_iPointCountPreDirection,
			pDlg.m_iPointCountInTotal,
			pDlg.m_dSearchRadius,
			pDlg.m_iU,
			pDlg.m_dT
			
			);

		CString	szInfo;
		szInfo.Format("距离加权插值算法构建构建规则格网DEM完毕，共耗时 : %dms 搜索半径起作用 %ld ",GetUsedTime(),GetSearchRadiusUsed());

		MessageBox(NULL,szInfo,"提示信息",MB_OK);
	}

	//径向基函数
	if(iType == 1)
	{
		//
		CSetGridCreatorRBFDlg	pDlg;
		
		pDlg.m_dMapLeft		= m_dMapLeft;
		pDlg.m_dMapRight	= m_dMapRight;
		pDlg.m_dMapBottom	= m_dMapBottom;
		pDlg.m_dMapTop		= m_dMapTop;

		pDlg.m_iDemNumX		= m_iDemNumOfX;
		pDlg.m_iDemNumY		= m_iDemNumOfY;

		//插值模型
		pDlg.m_iModelType	= m_pGridRBF.m_iModelType;
		//平滑指数
		pDlg.m_dC			= m_pGridRBF.m_dC;
		//
		pDlg.m_szPathName	= m_pGridRBF.m_szPathName;

		if(m_bCaclRemainsError)		pDlg.m_bCheckRE		= TRUE;
		else						pDlg.m_bCheckRE		= FALSE;

		if(pDlg.DoModal() == IDOK)
		{

		}
		else	return;

		//
		if(pDlg.m_bCheckRE)	m_bCaclRemainsError	= true;
		else				m_bCaclRemainsError	= false;

		//插值计算
		CreateInRBF(
			
			pDC,
			pDlg.m_iDemNumX,
			pDlg.m_iDemNumY,
			pDlg.m_szPathName,
			pDlg.m_iSearchType,
			pDlg.m_iPointCountPreDirection,
			pDlg.m_iPointCountInTotal,
			pDlg.m_dSearchRadius,
			pDlg.m_iModelType,
			pDlg.m_dC
			
			);

		CString	szInfo;
		szInfo.Format("径向基函数插值法构建构建规则格网DEM完毕，共耗时 : %dms 搜索半径起作用 %ld ",GetUsedTime(),GetSearchRadiusUsed());

		MessageBox(NULL,szInfo,"提示信息",MB_OK);
	}

	//克里格
	if(iType == 2)
	{
		//
		CSetGridCreatorKRGDlg	pDlg;
		
		pDlg.m_dMapLeft		= m_dMapLeft;
		pDlg.m_dMapRight	= m_dMapRight;
		pDlg.m_dMapBottom	= m_dMapBottom;
		pDlg.m_dMapTop		= m_dMapTop;

		pDlg.m_iDemNumX		= m_iDemNumOfX;
		pDlg.m_iDemNumY		= m_iDemNumOfY;

		//插值模型
		pDlg.m_iModelType	= m_pGridKRG.m_iModelType;
		//模型参数
		pDlg.m_dC0			= m_pGridKRG.m_dC0;
		pDlg.m_dC			= m_pGridKRG.m_dC;
		pDlg.m_dAlfa		= m_pGridKRG.m_dAlfa;
		//
		pDlg.m_szPathName	= m_pGridKRG.m_szPathName;

		if(m_bCaclRemainsError)		pDlg.m_bCheckRE		= TRUE;
		else						pDlg.m_bCheckRE		= FALSE;

		if(pDlg.DoModal() == IDOK)
		{

		}
		else	return;

		//
		if(pDlg.m_bCheckRE)	m_bCaclRemainsError	= true;
		else				m_bCaclRemainsError	= false;
		
		CreateInKRG(
			
			pDC,
			pDlg.m_iDemNumX,
			pDlg.m_iDemNumY,
			pDlg.m_szPathName,
			pDlg.m_iSearchType,
			pDlg.m_iPointCountPreDirection,
			pDlg.m_iPointCountInTotal,
			pDlg.m_dSearchRadius,
			pDlg.m_iModelType,
			pDlg.m_dC0,
			pDlg.m_dC,
			pDlg.m_dAlfa
			
			);

		CString	szInfo;
		szInfo.Format("普通克里金插值法构建构建规则格网DEM完毕，共耗时 : %dms 搜索半径起作用 %ld ",GetUsedTime(),GetSearchRadiusUsed());

		MessageBox(NULL,szInfo,"提示信息",MB_OK);
	}

	//谢别德
	if(iType == 3)
	{
		//
		CSetGridCreatorSPDDlg	pDlg;
				
		pDlg.m_dMapLeft		= m_dMapLeft;
		pDlg.m_dMapRight	= m_dMapRight;
		pDlg.m_dMapBottom	= m_dMapBottom;
		pDlg.m_dMapTop		= m_dMapTop;

		pDlg.m_iDemNumX		= m_iDemNumOfX;
		pDlg.m_iDemNumY		= m_iDemNumOfY;

		//权指数
		pDlg.m_iU			= m_pGridSPD.GetU();
		//平滑指数
		pDlg.m_dT			= m_pGridSPD.GetT();
		//
		pDlg.m_szPathName	= m_pGridSPD.GetPathName();

		if(m_bCaclRemainsError)		pDlg.m_bCheckRE		= TRUE;
		else						pDlg.m_bCheckRE		= FALSE;

		if(pDlg.DoModal() == IDOK)
		{

		}
		else	return;

		//
		if(pDlg.m_bCheckRE)	m_bCaclRemainsError	= true;
		else				m_bCaclRemainsError	= false;
		
		//计算缺省的调整距离dAdjustR
		double	dAdjustR;
		if(pDlg.m_bCustom == 0)
		{
			dAdjustR	= sqrt( (m_dMapRight-m_dMapLeft)*(m_dMapTop-m_dMapBottom)/m_iSampleCount * pDlg.m_iPointCountInTotal / 3.1415926 );
		}
		else
		{
			dAdjustR	= pDlg.m_dMaxDistance;
		}
		
		//插值核函数的样式
		int		iKernalType;
		iKernalType		= pDlg.m_iKernal;

		CreateInSPD(

			pDC,
			pDlg.m_iDemNumX,
			pDlg.m_iDemNumY,
			pDlg.m_szPathName,
			pDlg.m_iSearchType,
			pDlg.m_iPointCountPreDirection,
			pDlg.m_iPointCountInTotal,
			pDlg.m_dSearchRadius,
			pDlg.m_iU,
			pDlg.m_dT,
			pDlg.m_bCustom,
			dAdjustR,
			iKernalType

			);

		CString	szInfo;
		szInfo.Format("谢别德插值法构建构建规则格网DEM完毕，共耗时 : %dms 搜索半径起作用 %ld ",GetUsedTime(),GetSearchRadiusUsed());

		MessageBox(NULL,szInfo,"提示信息",MB_OK);
	}

	//多项式
	if(iType == 4)
	{
		//
		CSetGridCreatorPLYDlg	pDlg;
		
		pDlg.m_dMapLeft		= m_dMapLeft;
		pDlg.m_dMapRight	= m_dMapRight;
		pDlg.m_dMapBottom	= m_dMapBottom;
		pDlg.m_dMapTop		= m_dMapTop;

		pDlg.m_iDemNumX		= m_iDemNumOfX;
		pDlg.m_iDemNumY		= m_iDemNumOfY;

		//插值模型
		pDlg.m_iModelType	= m_pGridPLY.m_iModelType;
		//模型参数
		pDlg.m_iMode		= m_pGridPLY.m_iInterpolationMode;
		//
		pDlg.m_szPathName	= m_pGridPLY.m_szPathName;

		if(m_bCaclRemainsError)		pDlg.m_bCheckRE		= TRUE;
		else						pDlg.m_bCheckRE		= FALSE;

		if(pDlg.DoModal() == IDOK)
		{

		}
		else	return;

		//
		if(pDlg.m_bCheckRE)	m_bCaclRemainsError	= true;
		else				m_bCaclRemainsError	= false;

		CreateInPLY(
			
			pDC,
			pDlg.m_iDemNumX,
			pDlg.m_iDemNumY,
			pDlg.m_szPathName,
			pDlg.m_iSearchType,
			pDlg.m_iPointCountPreDirection,
			pDlg.m_iPointCountInTotal,
			pDlg.m_dSearchRadius,
			pDlg.m_iModelType,
			pDlg.m_iMode
			
			);

		CString	szInfo;
		szInfo.Format("多项式插值法构建构建规则格网DEM完毕，共耗时 : %dms 搜索半径起作用 %ld ",GetUsedTime(),GetSearchRadiusUsed());

		MessageBox(NULL,szInfo,"提示信息",MB_OK);
	}
}
	
//////////////////////////////////////////////////////////////////////
// IDW批处理创建
//////////////////////////////////////////////////////////////////////
void CDCR_GridCreator::CreateInIDW(
								   CDC		*pDC, 
								   int		iDemNumX,
								   int		iDemNumY,
								   CString	szPathName,
								   int		iSearchType,
								   int		iPointNumPreDirection,
								   int		iPointNumInTotal,
								   double	dSearchRadius,
								   int		iU,
								   double	dT
								   )
{
	//如果
	if(m_pCells == NULL)						
		return;

	//进入MatCom
	initM(MATCOM_VERSION);

	//使用搜索半径的次数
	m_iSearchRadiusUsed	= 0;

	//
	{
		//开始时间
		m_dwStartTime	= GetTickCount();

		//
		m_iDemNumOfX	= iDemNumX;
		m_iDemNumOfY	= iDemNumY;

		m_dDemSizeOfX	= (m_dMapRight-m_dMapLeft)/(m_iDemNumOfX-1);
		m_dDemSizeOfY	= (m_dMapTop-m_dMapBottom)/(m_iDemNumOfY-1);
		
		//权指数
		m_pGridIDW.m_iU	= iU;
		//平滑指数
		m_pGridIDW.m_dT	= dT;

		//搜索方式
		if(iSearchType == 0)
			m_bGlobalSearch	= true;
		else
		{
			m_bGlobalSearch	= false;

			if(iSearchType == 1)
			{
				m_enumDirection	= FourDirection;
			}
			if(iSearchType == 2)
			{
				m_enumDirection	= EightDirection;
			}
		}	

		//每个方向内的点数
		m_iPointNumPreDirection	= iPointNumPreDirection;
		//总点数
		m_iPointNumInTotal		= iPointNumInTotal;
		//搜索半径
		m_dSearchRadius			= dSearchRadius;

		if(m_iDemNumOfX == 0 || m_iDemNumOfY == 0)
			return;

		int		iC;
		double	dX,dY,dZ;
		
		if(m_pHeight != NULL)
		{
			delete	[]m_pHeight;
			m_pHeight	= NULL;
		}

		m_pHeight	= new	double[m_iDemNumOfX*m_iDemNumOfY];

		for(int j=0;j<m_iDemNumOfY;j++)
		{
			for(int i=0;i<m_iDemNumOfX;i++)
			{
				iC	= j*m_iDemNumOfX + i;
				dX	= m_dMapLeft + i*m_dDemSizeOfX;
				dY	= m_dMapBottom + j*m_dDemSizeOfY;
				
				//缺值
				dZ	= -9999;

				//搜索采样点
				if(SearchSamplingPoint(dX,dY))
				{
					//输入顶点坐标
					m_pGridIDW.SetGridVertexList(&m_pGridVertexList);
					
					//开始计算
					//
					m_pGridIDW.CalcHeight(dX,dY,dZ);
				}

				m_pHeight[iC]	= dZ;

				//绘图
				{
					DrawCell(pDC,dX,dY,dZ);
				}

				if(i==0 && j==0)
				{
					m_dMaxHeight	= m_dMinHeight	= dZ;
				}
				else
				{
					if(m_dMaxHeight < dZ)	m_dMaxHeight	= dZ;
					if(m_dMinHeight > dZ)	m_dMinHeight	= dZ;
				}

			}//for(int i=0;i<m_iDemNumOfX;i++)

		}//for(int j=0;j<m_iDemNumOfY;j++)

		m_szGrdPathName	= szPathName;
		Save(m_szGrdPathName);

		//结束时间
		m_dwFinishTime	= GetTickCount();
	}
	//

	//退出MatCom
	exitM();	
}

//////////////////////////////////////////////////////////////////////
// IDW批处理创建 新
//////////////////////////////////////////////////////////////////////
void CDCR_GridCreator::CreateInIDW(
								   CDC		*pDC, 
								   int		iDemNumX,
								   int		iDemNumY,
								   CString	szPathName,
								   int		iSearchType,
								   int		iPointNumPreDirection,
								   int		iPointNumInTotal,
								   double	dSearchRadius,
								   int		iNumOfU, 
								   int		*iU,
								   double	dT
								   )
{
	//如果
	if(m_pCells == NULL)						
		return;

	//进入MatCom
	initM(MATCOM_VERSION);

	//使用搜索半径的次数
	m_iSearchRadiusUsed	= 0;

	//
	{
		//开始时间
		m_dwStartTime	= GetTickCount();

		//插值的格网数
		m_iDemNumOfX	= iDemNumX;
		m_iDemNumOfY	= iDemNumY;
		//计算相应的格网尺寸
		m_dDemSizeOfX	= (m_dMapRight-m_dMapLeft)/(m_iDemNumOfX-1);
		m_dDemSizeOfY	= (m_dMapTop-m_dMapBottom)/(m_iDemNumOfY-1);
		
		//平滑指数【用缺省的0】
		m_pGridIDW.m_dT	= dT;

		//搜索方式
		if(iSearchType == 0)
			m_bGlobalSearch	= true;
		else
		{
			m_bGlobalSearch	= false;

			if(iSearchType == 1)
			{
				m_enumDirection	= FourDirection;
			}
			if(iSearchType == 2)
			{
				m_enumDirection	= EightDirection;
			}
		}	

		//每个方向内的点数
		m_iPointNumPreDirection	= iPointNumPreDirection;
		//总点数
		m_iPointNumInTotal		= iPointNumInTotal;
		//搜索半径
		m_dSearchRadius			= dSearchRadius;

		if(m_iDemNumOfX == 0 || m_iDemNumOfY == 0)
			return;

		int		iC;
		int		i,j,k;
		double	dX,dY,dZ;

		//
		InitialHeight();
		
		for(k=0;k<iNumOfU;k++)
		{
			m_pGridHeight[k].m_iNumOfX	= m_iDemNumOfX;
			m_pGridHeight[k].m_iNumOfY	= m_iDemNumOfY;

			m_pGridHeight[k].m_pHeight	= new	double[m_iDemNumOfX*m_iDemNumOfY];
		}
		
		for(j=0;j<m_iDemNumOfY;j++)
		{
			for(i=0;i<m_iDemNumOfX;i++)
			{
				iC	= j*m_iDemNumOfX + i;
				dX	= m_dMapLeft + i*m_dDemSizeOfX;
				dY	= m_dMapBottom + j*m_dDemSizeOfY;
				
				//缺值
				dZ	= -9999;

				//搜索采样点
				if(SearchSamplingPoint(dX,dY))
				{
					
					for(k=0;k<iNumOfU;k++)
					{
						//在这里输入iU
						//权指数
						m_pGridIDW.m_iU		= iU[k];

						//输入顶点坐标
						m_pGridIDW.SetGridVertexList(&m_pGridVertexList);

						//
						m_pGridIDW.CalcHeight(dX,dY,dZ);

						//
						m_pGridHeight[k].m_pHeight[iC]	= dZ;

						m_pGridHeight[k].SetHeight(dZ);
					}
				}
				else
				{
					for(k=0;k<iNumOfU;k++)
					{
						//
						m_pGridHeight[k].m_pHeight[iC]	= dZ;

						m_pGridHeight[k].SetHeight(dZ);
					}
				}

				//
				{
					DrawCell(pDC,dX,dY,dZ);
				}

			}//for(int i=0;i<m_iDemNumOfX;i++)

		}//for(int j=0;j<m_iDemNumOfY;j++)

		//m_szGrdPathName	= szPathName;
		//Save(m_szGrdPathName);
		for(k=0;k<iNumOfU;k++)
		{
			CString	szGrdPathName;
			//生成的GRD文件，应该放在和DVF相同的目录下，因此需要本类中获取
			szGrdPathName.Format("D%ld_P%ld_U%ld(%ld).grd",iSearchType,iPointNumInTotal,iU[k],m_iDemNumOfX);
			szGrdPathName	= szPathName + "\\" + szGrdPathName;

			//
			m_pGridHeight[k].m_szPathName	= szGrdPathName;

			SaveEx(szGrdPathName,m_pGridHeight[k].m_dMinHeight,m_pGridHeight[k].m_dMaxHeight,m_pGridHeight[k].m_pHeight);
		}

		//结束时间
		m_dwFinishTime	= GetTickCount();
	}
	//

	//退出MatCom
	exitM();	
}

//////////////////////////////////////////////////////////////////////
// 交叉验证IDW
//////////////////////////////////////////////////////////////////////
void CDCR_GridCreator::CrossValidateInIDW(
										  CDC		*pDC, 
										  int		iSampleCount, 
										  CDCR_PointFeature *pSamplePointList,
										  CString	szPathName,
										  int		iSearchType,
										  int		iPointNumPreDirection,
										  int		iPointNumInTotal,
										  double	dSearchRadius,
										  int		iNumOfU, 
										  int		*iU,
										  double	dT
										  )
{
	//如果
	if(m_pCells == NULL)						
		return;

	//
	if(pSamplePointList == NULL)
		return;

	//进入MatCom
	initM(MATCOM_VERSION);

	//使用搜索半径的次数
	m_iSearchRadiusUsed	= 0;

	//
	{
		//开始时间
		m_dwStartTime	= GetTickCount();

		//插值的格网数
		//m_iDemNumOfX	= iDemNumX;
		//m_iDemNumOfY	= iDemNumY;
		//计算相应的格网尺寸
		//m_dDemSizeOfX	= (m_dMapRight-m_dMapLeft)/(m_iDemNumOfX-1);
		//m_dDemSizeOfY	= (m_dMapTop-m_dMapBottom)/(m_iDemNumOfY-1);
		
		//平滑指数【用缺省的0】
		m_pGridIDW.m_dT	= dT;

		//搜索方式
		if(iSearchType == 0)
			m_bGlobalSearch	= true;
		else
		{
			m_bGlobalSearch	= false;

			if(iSearchType == 1)
			{
				m_enumDirection	= FourDirection;
			}
			if(iSearchType == 2)
			{
				m_enumDirection	= EightDirection;
			}
		}	

		//每个方向内的点数
		m_iPointNumPreDirection	= iPointNumPreDirection;
		//总点数
		m_iPointNumInTotal		= iPointNumInTotal;
		//搜索半径
		m_dSearchRadius			= dSearchRadius;

		//if(m_iDemNumOfX == 0 || m_iDemNumOfY == 0)
		//	return;

		int		k;
		double	dX,dY,dZ;
		
		//
		InitialHeight();

		for(k=0;k<iNumOfU;k++)
		{
			m_pGridHeight[k].m_iNumOfX	= 0;
			m_pGridHeight[k].m_iNumOfY	= 0;

			m_pGridHeight[k].m_pHeight	= new	double[iSampleCount];
		}
		
		int		iC;
		iC		= 0;

		//计算采样点数
		CDCR_PointFeature	*pPoint;
		
		pPoint	= pSamplePointList;
		while(pPoint != NULL)
		{
			if(pPoint->m_bUsed == false)
			{
				dX	= pPoint->m_pPointCoord.x;
				dY	= pPoint->m_pPointCoord.y;

				//
				//缺值
				dZ	= -9999;

				if(SearchSamplingPointCV(pPoint))
				{
					for(k=0;k<iNumOfU;k++)
					{
						//在这里输入iU
						//权指数
						m_pGridIDW.m_iU		= iU[k];

						//输入顶点坐标
						m_pGridIDW.SetGridVertexList(&m_pGridVertexList);

						//
						m_pGridIDW.CalcHeight(dX,dY,dZ);

						//
						m_pGridHeight[k].m_pHeight[iC]	= pPoint->m_pPointCoord.z - dZ;

						m_pGridHeight[k].SetHeight(pPoint->m_pPointCoord.z - dZ);
					}
				}
				else
				{
					TRACE("============存在这种情况吗?============\n");
				}

				///////////////////////////////////////////////////////////
				//绘图
				{
					DrawCell(pDC,dX,dY,dZ);
				}
				///////////////////////////////////////////////////////////

				iC++;
			}

			//下一个采样点
			pPoint	= pPoint->m_pNext;
		}

		//m_szGrdPathName	= szPathName;
		//Save(m_szGrdPathName);
		for(k=0;k<iNumOfU;k++)
		{
			CString	szGrdPathName;
			//生成的GRD文件，应该放在和DVF相同的目录下，因此需要本类中获取
			szGrdPathName.Format("D%ld_P%ld_U%ld(%ld)RE.txt",iSearchType,iPointNumInTotal,iU[k],m_iDemNumOfX);
			szGrdPathName	= szPathName + "\\" + szGrdPathName;

			//
			m_pGridHeight[k].m_szPathName	= szGrdPathName;

			SaveRE(szGrdPathName,m_pGridHeight[k].m_dMinHeight,m_pGridHeight[k].m_dMaxHeight,iSampleCount,pSamplePointList,m_pGridHeight[k].m_pHeight);
		}

		//结束时间
		m_dwFinishTime	= GetTickCount();
	}
	//

	//退出MatCom
	exitM();	
}

//////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////
void CDCR_GridCreator::CrossValidateInIDW(
										  CDC *pDC, 
										  int iSampleCount, 
										  CDCR_PointFeature *pSamplePointList,
										  CDCR_PointFeature *pPoint,
										  int iSearchType,
										  int iPointNumPreDirection,
										  int iPointNumInTotal,
										  double dSearchRadius,
										  int iU,
										  double dT,
										  double& dDZ
										  )
{
	dDZ	= -32767;

	//如果
	if(m_pCells == NULL)						
		return;

	//
	if(pSamplePointList == NULL)
		return;

	//进入MatCom
	//initM(MATCOM_VERSION);

	//使用搜索半径的次数
	m_iSearchRadiusUsed	= 0;

	//
	{
		//插值的格网数
		//m_iDemNumOfX	= iDemNumX;
		//m_iDemNumOfY	= iDemNumY;
		//计算相应的格网尺寸
		//m_dDemSizeOfX	= (m_dMapRight-m_dMapLeft)/(m_iDemNumOfX-1);
		//m_dDemSizeOfY	= (m_dMapTop-m_dMapBottom)/(m_iDemNumOfY-1);
		
		//平滑指数【用缺省的0】
		m_pGridIDW.m_dT	= dT;

		//搜索方式
		if(iSearchType == 0)
			m_bGlobalSearch	= true;
		else
		{
			m_bGlobalSearch	= false;

			if(iSearchType == 1)
			{
				m_enumDirection	= FourDirection;
			}
			if(iSearchType == 2)
			{
				m_enumDirection	= EightDirection;
			}
		}	

		//每个方向内的点数
		m_iPointNumPreDirection	= iPointNumPreDirection;
		//总点数
		m_iPointNumInTotal		= iPointNumInTotal;
		//搜索半径
		m_dSearchRadius			= dSearchRadius;

		//if(m_iDemNumOfX == 0 || m_iDemNumOfY == 0)
		//	return;

		double	dX,dY,dZ;

		dX	= pPoint->m_pPointCoord.x;
		dY	= pPoint->m_pPointCoord.y;

		//
		//缺值
		dZ	= -9999;

		if(SearchSamplingPointCV(pPoint))
		{
			{
				//在这里输入iU
				//权指数
				m_pGridIDW.m_iU		= iU;

				//输入顶点坐标
				m_pGridIDW.SetGridVertexList(&m_pGridVertexList);

				//
				m_pGridIDW.CalcHeight(dX,dY,dZ);

				//
				dDZ	= pPoint->m_pPointCoord.z - dZ;
			}
		}
	}

	//退出MatCom
	//exitM();		
}

//////////////////////////////////////////////////////////////////////
// RBF批处理创建
//////////////////////////////////////////////////////////////////////
void CDCR_GridCreator::CreateInRBF(
								   CDC		*pDC, 
								   int		iDemNumX,
								   int		iDemNumY,
								   CString	szPathName,
								   int		iSearchType,
								   int		iPointNumPreDirection,
								   int		iPointNumInTotal,
								   double	dSearchRadius,
								   int		iModelType,
								   double	dC
								   )
{
	//如果
	if(m_pCells == NULL)						
		return;

	//进入MatCom
	initM(MATCOM_VERSION);

	//使用搜索半径的次数
	m_iSearchRadiusUsed	= 0;

	//径向基函数
	{
		//开始时间
		m_dwStartTime	= GetTickCount();

		//
		m_iDemNumOfX	= iDemNumX;
		m_iDemNumOfY	= iDemNumY;

		m_dDemSizeOfX	= (m_dMapRight-m_dMapLeft)/(m_iDemNumOfX-1);
		m_dDemSizeOfY	= (m_dMapTop-m_dMapBottom)/(m_iDemNumOfY-1);
		
		//插值模型
		m_pGridRBF.m_iModelType	= iModelType;
		//平滑指数
		m_pGridRBF.m_dC			= dC;

		//搜索方式
		if(iSearchType == 0)
			m_bGlobalSearch	= true;
		else
		{
			m_bGlobalSearch	= false;

			if(iSearchType == 1)
			{
				m_enumDirection	= FourDirection;
			}
			if(iSearchType == 2)
			{
				m_enumDirection	= EightDirection;
			}
		}	
		//每个方向内的点数
		m_iPointNumPreDirection	= iPointNumPreDirection;
		//总点数
		m_iPointNumInTotal		= iPointNumInTotal;
		//搜索半径
		m_dSearchRadius			= dSearchRadius;

		if(m_iDemNumOfX == 0 || m_iDemNumOfY == 0)
			return;

		int		iC;
		double	dX,dY,dZ;
		
		if(m_pHeight != NULL)
		{
			delete	[]m_pHeight;
			m_pHeight	= NULL;
		}

		m_pHeight	= new	double[m_iDemNumOfX*m_iDemNumOfY];

		for(int j=0;j<m_iDemNumOfY;j++)
		{
			for(int i=0;i<m_iDemNumOfX;i++)
			{
				iC	= j*m_iDemNumOfX + i;
				dX	= m_dMapLeft + i*m_dDemSizeOfX;
				dY	= m_dMapBottom + j*m_dDemSizeOfY;
				
				//缺值
				dZ	= -9999;

				//搜索采样点
				if(SearchSamplingPoint(dX,dY))
				{
					//输入顶点坐标
					m_pGridRBF.SetGridVertexList(&m_pGridVertexList);
					
					//开始计算
					if(m_pGridVertexList.GetGridVertexCount() > 1)
					{
						//
						m_pGridRBF.CalcHeight(dX,dY,dZ);
					}
					else
					{
						CDCR_GridVertex	*pGridVertex;
						pGridVertex	= m_pGridVertexList.GetGridVertex();

						dZ	= pGridVertex->m_dZ;
					}
				}
				
				//这是没有搜索到点，采样点数为0
				m_pHeight[iC]	= dZ;

				//
				{
					DrawCell(pDC,dX,dY,dZ);
				}

				if(i==0 && j==0)
				{
					m_dMaxHeight	= m_dMinHeight	= dZ;
				}
				else
				{
					if(m_dMaxHeight < dZ)	m_dMaxHeight	= dZ;
					if(m_dMinHeight > dZ)	m_dMinHeight	= dZ;
				}
				
				//break;
			}//for(int i=0;i<m_iDemNumOfX;i++)
			//break;
		}//for(int j=0;j<m_iDemNumOfY;j++)

		m_szGrdPathName	= szPathName;
		Save(m_szGrdPathName);

		m_dwFinishTime	= GetTickCount();
	}

	//退出MatCom
	exitM();	
}

//////////////////////////////////////////////////////////////////////
// RBF批处理创建 新
//////////////////////////////////////////////////////////////////////
void CDCR_GridCreator::CreateInRBF(
								   CDC		*pDC, 
								   int		iDemNumX,
								   int		iDemNumY,
								   CString	szPathName,
								   int		iSearchType,
								   int		iPointNumPreDirection,
								   int		iPointNumInTotal,
								   double	dSearchRadius,
								   int		iModelType,
								   int		iNumOfC,
								   double	*dC
								   )
{
	//如果
	if(m_pCells == NULL)						
		return;

	//进入MatCom
	initM(MATCOM_VERSION);

	//使用搜索半径的次数
	m_iSearchRadiusUsed	= 0;

	//径向基函数
	{
		//开始时间
		m_dwStartTime	= GetTickCount();

		//
		m_iDemNumOfX	= iDemNumX;
		m_iDemNumOfY	= iDemNumY;

		m_dDemSizeOfX	= (m_dMapRight-m_dMapLeft)/(m_iDemNumOfX-1);
		m_dDemSizeOfY	= (m_dMapTop-m_dMapBottom)/(m_iDemNumOfY-1);
		
		//插值模型
		m_pGridRBF.m_iModelType	= iModelType;

		//搜索方式
		if(iSearchType == 0)
			m_bGlobalSearch	= true;
		else
		{
			m_bGlobalSearch	= false;

			if(iSearchType == 1)
			{
				m_enumDirection	= FourDirection;
			}
			if(iSearchType == 2)
			{
				m_enumDirection	= EightDirection;
			}
		}	
		//每个方向内的点数
		m_iPointNumPreDirection	= iPointNumPreDirection;
		//总点数
		m_iPointNumInTotal		= iPointNumInTotal;
		//搜索半径
		m_dSearchRadius			= dSearchRadius;

		if(m_iDemNumOfX == 0 || m_iDemNumOfY == 0)
			return;

		int		k;
		int		iC;
		double	dX,dY,dZ;

		//
		InitialHeight();
		
		for(k=0;k<iNumOfC;k++)
		{
			m_pGridHeight[k].m_iNumOfX	= m_iDemNumOfX;
			m_pGridHeight[k].m_iNumOfY	= m_iDemNumOfY;

			m_pGridHeight[k].m_pHeight	= new	double[m_iDemNumOfX*m_iDemNumOfY];
		}
		
		for(int j=0;j<m_iDemNumOfY;j++)
		{
			for(int i=0;i<m_iDemNumOfX;i++)
			{
				iC	= j*m_iDemNumOfX + i;
				dX	= m_dMapLeft + i*m_dDemSizeOfX;
				dY	= m_dMapBottom + j*m_dDemSizeOfY;
				
				//缺值
				dZ	= -9999;

				//搜索采样点
				if(SearchSamplingPoint(dX,dY))
				{
					for(k=0;k<iNumOfC;k++)
					{
						//在这里输入dC
						//平滑指数
						m_pGridRBF.m_dC			= dC[k];

						//输入顶点坐标
						m_pGridRBF.SetGridVertexList(&m_pGridVertexList);
						
						//开始计算
						if(m_pGridVertexList.GetGridVertexCount() > 1)
						{
							//
							m_pGridRBF.CalcHeight(dX,dY,dZ);
						}
						else
						{
							CDCR_GridVertex	*pGridVertex;
							pGridVertex	= m_pGridVertexList.GetGridVertex();

							dZ	= pGridVertex->m_dZ;
						}

						//
						m_pGridHeight[k].m_pHeight[iC]	= dZ;

						m_pGridHeight[k].SetHeight(dZ);
					}
				}
				else
				{
					//这是没有搜索到点，采样点数为0
					for(k=0;k<iNumOfC;k++)
					{				
						m_pGridHeight[k].m_pHeight[iC]	= dZ;
						m_pGridHeight[k].SetHeight(dZ);
					}
				}

				//
				{
					DrawCell(pDC,dX,dY,dZ);
				}
				
				//break;
			}//for(int i=0;i<m_iDemNumOfX;i++)
			//break;
		}//for(int j=0;j<m_iDemNumOfY;j++)
		
		//m_szGrdPathName	= szPathName;
		
		for(k=0;k<iNumOfC;k++)
		{
			CString	szGrdPathName;
			//生成的GRD文件，应该放在和DVF相同的目录下，因此需要本类中获取
			szGrdPathName.Format("D%ld_P%ld_R%ld_M%ld_C%ld.grd",iSearchType,iPointNumInTotal,(int)(dSearchRadius),iModelType,(int)(dC[k]));
			szGrdPathName	= szPathName + "\\" + szGrdPathName;

			//
			m_pGridHeight[k].m_szPathName	= szGrdPathName;

			SaveEx(szGrdPathName,m_pGridHeight[k].m_dMinHeight,m_pGridHeight[k].m_dMaxHeight,m_pGridHeight[k].m_pHeight);
		}
		
		//
		//InitialHeight();

		m_dwFinishTime	= GetTickCount();
	}

	//退出MatCom
	exitM();	
}

//////////////////////////////////////////////////////////////////////
// RBF交叉验证
//////////////////////////////////////////////////////////////////////
void CDCR_GridCreator::CrossValidateInRBF(
										  CDC		*pDC, 
										  int		iSampleCount, 
										  CDCR_PointFeature *pSamplePointList,
										  CString	szPathName,
										  int		iSearchType,
										  int		iPointNumPreDirection,
										  int		iPointNumInTotal,
										  double	dSearchRadius,
										  int		iModelType,
										  int		iNumOfC,
										  double	*dC
										  )
{
	//如果
	if(m_pCells == NULL)						
		return;

	//进入MatCom
	initM(MATCOM_VERSION);

	//使用搜索半径的次数
	m_iSearchRadiusUsed	= 0;

	//径向基函数
	{
		//开始时间
		m_dwStartTime	= GetTickCount();

		//
		//m_iDemNumOfX	= iDemNumX;
		//m_iDemNumOfY	= iDemNumY;

		//m_dDemSizeOfX	= (m_dMapRight-m_dMapLeft)/(m_iDemNumOfX-1);
		//m_dDemSizeOfY	= (m_dMapTop-m_dMapBottom)/(m_iDemNumOfY-1);
		
		//插值模型
		m_pGridRBF.m_iModelType	= iModelType;

		//搜索方式
		if(iSearchType == 0)
			m_bGlobalSearch	= true;
		else
		{
			m_bGlobalSearch	= false;

			if(iSearchType == 1)
			{
				m_enumDirection	= FourDirection;
			}
			if(iSearchType == 2)
			{
				m_enumDirection	= EightDirection;
			}
		}	
		//每个方向内的点数
		m_iPointNumPreDirection	= iPointNumPreDirection;
		//总点数
		m_iPointNumInTotal		= iPointNumInTotal;
		//搜索半径
		m_dSearchRadius			= dSearchRadius;

		//if(m_iDemNumOfX == 0 || m_iDemNumOfY == 0)
		//	return;

		int		k;
		double	dX,dY,dZ;

		//
		InitialHeight();
		
		for(k=0;k<iNumOfC;k++)
		{ 
			m_pGridHeight[k].m_iNumOfX	= 0;
			m_pGridHeight[k].m_iNumOfY	= 0;

			m_pGridHeight[k].m_pHeight	= new	double[iSampleCount];
		}
		
		int		iC;
		iC		= 0;

		//计算采样点数
		CDCR_PointFeature	*pPoint;
		
		pPoint	= pSamplePointList;
		while(pPoint != NULL)
		{
			if(pPoint->m_bUsed == false)
			{
				dX	= pPoint->m_pPointCoord.x;
				dY	= pPoint->m_pPointCoord.y;
				
				//
				//缺值
				dZ	= -9999;

				if(SearchSamplingPointCV(pPoint))
				{
					for(k=0;k<iNumOfC;k++)
					{
						//在这里输入dC
						//平滑指数
						m_pGridRBF.m_dC			= dC[k];

						//输入顶点坐标
						m_pGridRBF.SetGridVertexList(&m_pGridVertexList);
						
						//开始计算
						if(m_pGridVertexList.GetGridVertexCount() > 1)
						{
							//
							m_pGridRBF.CalcHeight(dX,dY,dZ);
						}
						else
						{
							CDCR_GridVertex	*pGridVertex;
							pGridVertex	= m_pGridVertexList.GetGridVertex();

							dZ	= pGridVertex->m_dZ;
						}

						//
						m_pGridHeight[k].m_pHeight[iC]	= pPoint->m_pPointCoord.z - dZ;

						m_pGridHeight[k].SetHeight(pPoint->m_pPointCoord.z - dZ);
					}
				}
				else
				{
					TRACE("============存在这种情况吗?============\n");
				}

				///////////////////////////////////////////////////////////
				//绘图
				{
					DrawCell(pDC,dX,dY,dZ);
				}
				///////////////////////////////////////////////////////////

				iC++;
			}	

			pPoint	= pPoint->m_pNext;
		}

		//m_szGrdPathName	= szPathName;
		
		for(k=0;k<iNumOfC;k++)
		{
			CString	szGrdPathName;
			//生成的GRD文件，应该放在和DVF相同的目录下，因此需要本类中获取
			szGrdPathName.Format("D%ld_P%ld_R%ld_M%ld_C%ldRE.txt",iSearchType,iPointNumInTotal,(int)(dSearchRadius),iModelType,(int)(dC[k]));
			szGrdPathName	= szPathName + "\\" + szGrdPathName;

			//
			m_pGridHeight[k].m_szPathName	= szGrdPathName;

			SaveRE(szGrdPathName,m_pGridHeight[k].m_dMinHeight,m_pGridHeight[k].m_dMaxHeight,iSampleCount,pSamplePointList,m_pGridHeight[k].m_pHeight);
		}
		
		//
		//InitialHeight();

		m_dwFinishTime	= GetTickCount();
	}

	//退出MatCom
	exitM();	
}

//////////////////////////////////////////////////////////////////////
// RBF交叉验证
//////////////////////////////////////////////////////////////////////
void CDCR_GridCreator::CrossValidateInRBF(
										  CDC		*pDC, 
										  int		iSampleCount, 
										  CDCR_PointFeature *pSamplePointList,
										  CDCR_PointFeature *pPoint,
										  int		iSearchType,
										  int		iPointNumPreDirection,
										  int		iPointNumInTotal,
										  double	dSearchRadius,
										  int		iModelType,
										  double	dC,
										  double&	dDZ
										  )
{
	//如果
	if(m_pCells == NULL)						
		return;

	//进入MatCom
	//initM(MATCOM_VERSION);

	//使用搜索半径的次数
	m_iSearchRadiusUsed	= 0;

	//径向基函数
	{
		//
		//m_iDemNumOfX	= iDemNumX;
		//m_iDemNumOfY	= iDemNumY;

		//m_dDemSizeOfX	= (m_dMapRight-m_dMapLeft)/(m_iDemNumOfX-1);
		//m_dDemSizeOfY	= (m_dMapTop-m_dMapBottom)/(m_iDemNumOfY-1);
		
		//插值模型
		m_pGridRBF.m_iModelType	= iModelType;

		//搜索方式
		if(iSearchType == 0)
			m_bGlobalSearch	= true;
		else
		{
			m_bGlobalSearch	= false;

			if(iSearchType == 1)
			{
				m_enumDirection	= FourDirection;
			}
			if(iSearchType == 2)
			{
				m_enumDirection	= EightDirection;
			}
		}	
		//每个方向内的点数
		m_iPointNumPreDirection	= iPointNumPreDirection;
		//总点数
		m_iPointNumInTotal		= iPointNumInTotal;
		//搜索半径
		m_dSearchRadius			= dSearchRadius;

		//if(m_iDemNumOfX == 0 || m_iDemNumOfY == 0)
		//	return;

		double	dX,dY,dZ;

		dX	= pPoint->m_pPointCoord.x;
		dY	= pPoint->m_pPointCoord.y;
		
		//
		//缺值
		dZ	= -9999;

		if(SearchSamplingPointCV(pPoint))
		{
			{
				//在这里输入dC
				//平滑指数
				m_pGridRBF.m_dC			= dC;

				//输入顶点坐标
				m_pGridRBF.SetGridVertexList(&m_pGridVertexList);
				
				//开始计算
				if(m_pGridVertexList.GetGridVertexCount() > 1)
				{
					//
					if(!m_pGridRBF.CalcHeight(dX,dY,dZ))
					{
						dZ	= pPoint->m_pPointCoord.z;
					}
				}
				else
				{
					CDCR_GridVertex	*pGridVertex;
					pGridVertex	= m_pGridVertexList.GetGridVertex();

					dZ	= pGridVertex->m_dZ;
				}

				//
				dDZ	= pPoint->m_pPointCoord.z - dZ;
			}
		}
	}

	//退出MatCom
	//exitM();	
}

//////////////////////////////////////////////////////////////////////
// KRG批处理创建
//////////////////////////////////////////////////////////////////////
void CDCR_GridCreator::CreateInKRG(
								   CDC		*pDC,					//绘图DC
								   int		iDemNumX,				//横向格网点数
								   int		iDemNumY,				//
								   CString	szPathName,				//
								   int		iSearchType,			//搜索方向
								   int		iPointNumPreDirection,	//每一方向内的点数
								   int		iPointNumInTotal,		//总点数
								   double	dSearchRadius,			//搜索半径
								   int		iModelType,				//半变异模型种类
								   double	dC0,					//块金值
								   double	dC,						//基台值
								   double	dAlfa					//变程值
								   )
{
	//如果
	if(m_pCells == NULL)						
		return;

	//进入MatCom
	initM(MATCOM_VERSION);

	//使用搜索半径的次数
	m_iSearchRadiusUsed	= 0;

	//克里格
	{
		//开始时间
		m_dwStartTime	= GetTickCount();

		//
		m_iDemNumOfX	= iDemNumX;
		m_iDemNumOfY	= iDemNumY;

		m_dDemSizeOfX	= (m_dMapRight-m_dMapLeft)/(m_iDemNumOfX-1);
		m_dDemSizeOfY	= (m_dMapTop-m_dMapBottom)/(m_iDemNumOfY-1);
		
		//插值模型
		m_pGridKRG.m_iModelType	= iModelType;
		//模型参数
		m_pGridKRG.m_dC0		= dC0;
		m_pGridKRG.m_dC			= dC;
		m_pGridKRG.m_dAlfa		= dAlfa;

		//搜索方式
		if(iSearchType == 0)
			m_bGlobalSearch	= true;
		else
		{
			m_bGlobalSearch	= false;

			if(iSearchType == 1)
			{
				m_enumDirection	= FourDirection;
			}
			if(iSearchType == 2)
			{
				m_enumDirection	= EightDirection;
			}
		}	
		//每个方向内的点数
		m_iPointNumPreDirection	= iPointNumPreDirection;
		//总点数
		m_iPointNumInTotal		= iPointNumInTotal;
		//搜索半径
		m_dSearchRadius			= dSearchRadius;

		if(m_iDemNumOfX == 0 || m_iDemNumOfY == 0)
			return;

		int		iC;
		double	dX,dY,dZ;
		double	dSDZ;

		if(m_pHeight != NULL)
		{
			delete	[]m_pHeight;
			m_pHeight	= NULL;
		}
		if(m_pHeightSD != NULL)
		{
			delete	[]m_pHeightSD;
			m_pHeightSD	= NULL;
		}

		m_pHeight	= new	double[m_iDemNumOfX*m_iDemNumOfY];
		m_pHeightSD	= new	double[m_iDemNumOfX*m_iDemNumOfY];

		for(int j=0;j<m_iDemNumOfY;j++)
		{
			for(int i=0;i<m_iDemNumOfX;i++)
			{
				iC	= j*m_iDemNumOfX + i;
				dX	= m_dMapLeft + i*m_dDemSizeOfX;
				dY	= m_dMapBottom + j*m_dDemSizeOfY;
				
				//缺值
				dZ		= -9999;
				dSDZ	= -9999;

				//搜索采样点
				if(SearchSamplingPoint(dX,dY))
				{					
					if(m_pGridVertexList.GetGridVertexCount() > 1)
					{
						//输入顶点坐标
						m_pGridKRG.SetGridVertexList(&m_pGridVertexList);
						
						//开始计算
						m_pGridKRG.CalcHeight(dX,dY,dZ,dSDZ);
					}
					else
					{
						CDCR_GridVertex	*pGridVertex;
						pGridVertex	= m_pGridVertexList.GetGridVertex();

						dZ		= pGridVertex->m_dZ;
						dSDZ	= 0;
					}
				}

				m_pHeight[iC]	= dZ;
				m_pHeightSD[iC]	= dSDZ;

				//绘图
				{
					DrawCell(pDC,dX,dY,dZ);
				}

				if(i==0 && j==0)
				{
					m_dMaxHeight	= m_dMinHeight	= dZ;
				}
				else
				{
					if(m_dMaxHeight < dZ)	m_dMaxHeight	= dZ;
					if(m_dMinHeight > dZ)	m_dMinHeight	= dZ;
				}
				
			}//for(int i=0;i<m_iDemNumOfX;i++)

		}//for(int j=0;j<m_iDemNumOfY;j++)

		m_szGrdPathName	= szPathName;
		Save(m_szGrdPathName);

		m_dwFinishTime	= GetTickCount();
	}

	//退出MatCom
	exitM();	
}

//////////////////////////////////////////////////////////////////////
// RBF交叉验证
//////////////////////////////////////////////////////////////////////
void CDCR_GridCreator::CrossValidateInKRG(
										  CDC		*pDC,							//
										  int		iSampleCount,					//
										  CDCR_PointFeature *pSamplePointList,		//
										  CString	szPathName,						//
										  int		iSearchType,					//
										  int		iPointNumPreDirection,			//
										  int		iPointNumInTotal,				//
										  double	dSearchRadius,					//
										  int		iModelType,						//
										  double	dC0,							//
										  double	dC,								//
										  double	dAlfa							//
										  )
{
	//如果
	if(m_pCells == NULL)						
		return;

	//进入MatCom
	initM(MATCOM_VERSION);

	//使用搜索半径的次数
	m_iSearchRadiusUsed	= 0;

	//克里格函数
	{
		//开始时间
		m_dwStartTime	= GetTickCount();

		//
		//m_iDemNumOfX	= iDemNumX;
		//m_iDemNumOfY	= iDemNumY;

		//m_dDemSizeOfX	= (m_dMapRight-m_dMapLeft)/(m_iDemNumOfX-1);
		//m_dDemSizeOfY	= (m_dMapTop-m_dMapBottom)/(m_iDemNumOfY-1);
		
		//插值模型
		m_pGridKRG.m_iModelType	= iModelType;
		//模型参数
		m_pGridKRG.m_dC0		= dC0;
		m_pGridKRG.m_dC			= dC;
		m_pGridKRG.m_dAlfa		= dAlfa;

		//搜索方式
		if(iSearchType == 0)
			m_bGlobalSearch	= true;
		else
		{
			m_bGlobalSearch	= false;

			if(iSearchType == 1)
			{
				m_enumDirection	= FourDirection;
			}
			if(iSearchType == 2)
			{
				m_enumDirection	= EightDirection;
			}
		}	
		//每个方向内的点数
		m_iPointNumPreDirection	= iPointNumPreDirection;
		//总点数
		m_iPointNumInTotal		= iPointNumInTotal;
		//搜索半径
		m_dSearchRadius			= dSearchRadius;

		//if(m_iDemNumOfX == 0 || m_iDemNumOfY == 0)
		//	return;

		double	dX,dY,dZ;
		double	dSDZ;

		//
		InitialHeight();
		{
			m_pGridHeight[0].m_iNumOfX	= 0;
			m_pGridHeight[0].m_iNumOfY	= 0;

			m_pGridHeight[0].m_pHeight	= new	double[iSampleCount];
		}
		
		int		iC;
		iC		= 0;

		//计算采样点数
		CDCR_PointFeature	*pPoint;
		
		pPoint	= pSamplePointList;
		while(pPoint != NULL)
		{
			if(pPoint->m_bUsed == false)
			{
				dX	= pPoint->m_pPointCoord.x;
				dY	= pPoint->m_pPointCoord.y;
				
				//
				//缺值
				dZ	= -9999;

				if(SearchSamplingPointCV(pPoint))
				{
					if(m_pGridVertexList.GetGridVertexCount() > 1)
					{
						//输入顶点坐标
						m_pGridKRG.SetGridVertexList(&m_pGridVertexList);
						
						//开始计算
						m_pGridKRG.CalcHeight(dX,dY,dZ,dSDZ);
					}
					else
					{
						CDCR_GridVertex	*pGridVertex;
						pGridVertex	= m_pGridVertexList.GetGridVertex();

						dZ		= pGridVertex->m_dZ;
						dSDZ	= 0;
					}

					//
					m_pGridHeight[0].m_pHeight[iC]	= pPoint->m_pPointCoord.z - dZ;

					m_pGridHeight[0].SetHeight(pPoint->m_pPointCoord.z - dZ);

				}
				else
				{
					TRACE("============存在这种情况吗?============\n");
				}

				///////////////////////////////////////////////////////////
				//绘图
				{
					DrawCell(pDC,dX,dY,dZ);
				}
				///////////////////////////////////////////////////////////
					
				iC++;
			}

			pPoint	= pPoint->m_pNext;
		}

		//保存
		{
			CString	szGrdPathName;
			//生成的GRD文件，应该放在和DVF相同的目录下，因此需要本类中获取
			szGrdPathName.Format("D%ld_P%ld_R%ld_M%ldRE.txt",iSearchType,iPointNumInTotal,(int)(dSearchRadius),iModelType);
			szGrdPathName	= szPathName + "\\" + szGrdPathName;

			//
			m_pGridHeight[0].m_szPathName	= szGrdPathName;

			SaveRE(szGrdPathName,m_pGridHeight[0].m_dMinHeight,m_pGridHeight[0].m_dMaxHeight,iSampleCount,pSamplePointList,m_pGridHeight[0].m_pHeight);
		}
		
		//
		//InitialHeight();

		m_dwFinishTime	= GetTickCount();
	}

	//退出MatCom
	exitM();	
}

//////////////////////////////////////////////////////////////////////
// RBF交叉验证
//////////////////////////////////////////////////////////////////////
void CDCR_GridCreator::CrossValidateInKRG(
										  CDC		*pDC,							//
										  int		iSampleCount,					//
										  CDCR_PointFeature *pSamplePointList,		//
										  CDCR_PointFeature	*pPoint,				//
										  int		iSearchType,					//
										  int		iPointNumPreDirection,			//
										  int		iPointNumInTotal,				//
										  double	dSearchRadius,					//
										  int		iModelType,						//
										  double	dC0,							//
										  double	dC,								//
										  double	dAlfa,							//
										  double&	dDZ
										  )
{
	//如果
	if(m_pCells == NULL)						
		return;
		
	//假如不存在模型，则返回
	if(iModelType == -1)	return;

	//进入MatCom
	//initM(MATCOM_VERSION);

	//使用搜索半径的次数
	m_iSearchRadiusUsed	= 0;

	//克里格函数
	{
		//
		//m_iDemNumOfX	= iDemNumX;
		//m_iDemNumOfY	= iDemNumY;

		//m_dDemSizeOfX	= (m_dMapRight-m_dMapLeft)/(m_iDemNumOfX-1);
		//m_dDemSizeOfY	= (m_dMapTop-m_dMapBottom)/(m_iDemNumOfY-1);
		
		//插值模型
		m_pGridKRG.m_iModelType	= iModelType;
		//模型参数
		m_pGridKRG.m_dC0		= dC0;
		m_pGridKRG.m_dC			= dC;
		m_pGridKRG.m_dAlfa		= dAlfa;

		//搜索方式
		if(iSearchType == 0)
			m_bGlobalSearch	= true;
		else
		{
			m_bGlobalSearch	= false;

			if(iSearchType == 1)
			{
				m_enumDirection	= FourDirection;
			}
			if(iSearchType == 2)
			{
				m_enumDirection	= EightDirection;
			}
		}	
		//每个方向内的点数
		m_iPointNumPreDirection	= iPointNumPreDirection;
		//总点数
		m_iPointNumInTotal		= iPointNumInTotal;
		//搜索半径
		m_dSearchRadius			= dSearchRadius;

		//if(m_iDemNumOfX == 0 || m_iDemNumOfY == 0)
		//	return;

		double	dX,dY,dZ;
		double	dSDZ;

		dX	= pPoint->m_pPointCoord.x;
		dY	= pPoint->m_pPointCoord.y;
		
		//
		//缺值
		dZ	= -9999;

		if(SearchSamplingPointCV(pPoint))
		{
			if(m_pGridVertexList.GetGridVertexCount() > 1)
			{
				//输入顶点坐标
				m_pGridKRG.SetGridVertexList(&m_pGridVertexList);
				
				//开始计算
				m_pGridKRG.CalcHeight(dX,dY,dZ,dSDZ);
			}
			else
			{
				CDCR_GridVertex	*pGridVertex;
				pGridVertex	= m_pGridVertexList.GetGridVertex();

				dZ		= pGridVertex->m_dZ;
				dSDZ	= 0;
			}

			//
			dDZ	= pPoint->m_pPointCoord.z - dZ;
		}
	}

	//退出MatCom
	//exitM();	
}

//////////////////////////////////////////////////////////////////////
// 非批处理谢别德建立GRD
//////////////////////////////////////////////////////////////////////
void CDCR_GridCreator::CreateInSPD(
								   CDC		*pDC, 
								   int		iDemNumX,
								   int		iDemNumY,
								   CString	szPathName,
								   int		iSearchType,
								   int		iPointNumPreDirection,
								   int		iPointNumInTotal,
								   double	dSearchRadius,
								   int		iU,
								   double	dT,
								   int		iRadiusType,
								   double	dMaxRadius,
								   int		iKernalType
								   )
{
	//如果
	if(m_pCells == NULL)						
		return;

	//进入MatCom
	initM(MATCOM_VERSION);

	//使用搜索半径的次数
	m_iSearchRadiusUsed	= 0;
	
	//谢别德
	{
		//开始时间
		m_dwStartTime	= GetTickCount();

		//
		m_iDemNumOfX	= iDemNumX;
		m_iDemNumOfY	= iDemNumY;

		m_dDemSizeOfX	= (m_dMapRight-m_dMapLeft)/(m_iDemNumOfX-1);
		m_dDemSizeOfY	= (m_dMapTop-m_dMapBottom)/(m_iDemNumOfY-1);
		
		//权指数
		m_pGridSPD.SetU(iU);
		//平滑指数
		m_pGridSPD.SetT(dT);
		//
		if(iRadiusType == 0)	m_pGridSPD.SetCustom(false);
		else					m_pGridSPD.SetCustom(true);

		m_pGridSPD.SetRadius(dMaxRadius);

		//搜索方式
		if(iSearchType == 0)
			m_bGlobalSearch	= true;
		else
		{
			m_bGlobalSearch	= false;

			if(iSearchType == 1)
			{
				m_enumDirection	= FourDirection;
			}
			if(iSearchType == 2)
			{
				m_enumDirection	= EightDirection;
			}
		}	
		//每个方向内的点数
		m_iPointNumPreDirection	= iPointNumPreDirection;
		//总点数
		m_iPointNumInTotal		= iPointNumInTotal;
		//搜索半径
		m_dSearchRadius			= dSearchRadius;

		if(m_iDemNumOfX == 0 || m_iDemNumOfY == 0)
			return;

		int		iC;
		double	dX,dY,dZ;
		
		if(m_pHeight != NULL)
		{
			delete	[]m_pHeight;
			m_pHeight	= NULL;
		}

		m_pHeight	= new	double[m_iDemNumOfX*m_iDemNumOfY];

		for(int j=0;j<m_iDemNumOfY;j++)
		{
			for(int i=0;i<m_iDemNumOfX;i++)
			{
				iC	= j*m_iDemNumOfX + i;
				dX	= m_dMapLeft + i*m_dDemSizeOfX;
				dY	= m_dMapBottom + j*m_dDemSizeOfY;
				
				//缺值
				dZ	= -9999;

				//搜索采样点
				if(SearchSamplingPoint(dX,dY))
				{
					//输入顶点坐标
					m_pGridSPD.SetGridVertexList(&m_pGridVertexList);
					
					//开始计算
					//
					if(iKernalType == 0)		m_pGridSPD.CalcHeight(dX,dY,dZ);
					else						m_pGridSPD.CalcHeightEx(dX,dY,dZ);
				}

				m_pHeight[iC]	= dZ;

				//绘图
				{
					DrawCell(pDC,dX,dY,dZ);
				}

				if(i==0 && j==0)
				{
					m_dMaxHeight	= m_dMinHeight	= dZ;
				}
				else
				{
					if(m_dMaxHeight < dZ)	m_dMaxHeight	= dZ;
					if(m_dMinHeight > dZ)	m_dMinHeight	= dZ;
				}

			}//for(int i=0;i<m_iDemNumOfX;i++)

		}//for(int j=0;j<m_iDemNumOfY;j++)

		m_szGrdPathName	= szPathName;
		Save(m_szGrdPathName);

		m_dwFinishTime	= GetTickCount();
	}
	
	//退出MatCom
	exitM();	
}

//////////////////////////////////////////////////////////////////////
// SPD批处理创建
//////////////////////////////////////////////////////////////////////
void CDCR_GridCreator::CreateInSPD(
								   CDC		*pDC, 
								   int		iDemNumX,
								   int		iDemNumY,
								   CString	szPathName,
								   int		iSearchType,
								   int		iPointNumPreDirection,
								   int		iPointNumInTotal,
								   double	dSearchRadius,
								   int		iNumOfU,
								   int		*iU,
								   double	dT,
								   int		iRadiusType,
								   double	dMaxRadius,
								   int		iKernalType
								   )
{
	//如果
	if(m_pCells == NULL)						
		return;

	//进入MatCom
	initM(MATCOM_VERSION);

	//使用搜索半径的次数
	m_iSearchRadiusUsed	= 0;
	
	//谢别德
	{
		//开始时间
		m_dwStartTime	= GetTickCount();

		//
		m_iDemNumOfX	= iDemNumX;
		m_iDemNumOfY	= iDemNumY;

		m_dDemSizeOfX	= (m_dMapRight-m_dMapLeft)/(m_iDemNumOfX-1);
		m_dDemSizeOfY	= (m_dMapTop-m_dMapBottom)/(m_iDemNumOfY-1);
		
		//权指数
		//m_pGridSPD.m_iU	= iU;
		//平滑指数
		m_pGridSPD.SetT(dT);
		//
		if(iRadiusType == 0)	m_pGridSPD.SetCustom(false);
		else					m_pGridSPD.SetCustom(true);

		m_pGridSPD.SetRadius(dMaxRadius);

		//搜索方式
		if(iSearchType == 0)
			m_bGlobalSearch	= true;
		else
		{
			m_bGlobalSearch	= false;

			if(iSearchType == 1)
			{
				m_enumDirection	= FourDirection;
			}
			if(iSearchType == 2)
			{
				m_enumDirection	= EightDirection;
			}
		}	
		//每个方向内的点数
		m_iPointNumPreDirection	= iPointNumPreDirection;
		//总点数
		m_iPointNumInTotal		= iPointNumInTotal;
		//搜索半径
		m_dSearchRadius			= dSearchRadius;

		if(m_iDemNumOfX == 0 || m_iDemNumOfY == 0)
			return;

		int		iC;
		int		i,j,k;
		double	dX,dY,dZ;
		
		//
		InitialHeight();

		for(k=0;k<iNumOfU;k++)
		{
			m_pGridHeight[k].m_iNumOfX	= m_iDemNumOfX;
			m_pGridHeight[k].m_iNumOfY	= m_iDemNumOfY;

			m_pGridHeight[k].m_pHeight	= new	double[m_iDemNumOfX*m_iDemNumOfY];
		}

		for(j=0;j<m_iDemNumOfY;j++)
		{
			for(i=0;i<m_iDemNumOfX;i++)
			{
				iC	= j*m_iDemNumOfX + i;
				dX	= m_dMapLeft + i*m_dDemSizeOfX;
				dY	= m_dMapBottom + j*m_dDemSizeOfY;
				
				//缺值
				dZ	= -9999;

				//搜索采样点
				if(SearchSamplingPoint(dX,dY))
				{
					for(k=0;k<iNumOfU;k++)
					{
						//在这里输入iU
						//权指数
						m_pGridSPD.SetU(iU[k]);

						//输入顶点坐标
						m_pGridSPD.SetGridVertexList(&m_pGridVertexList);

						//
						if(iKernalType == 0)	m_pGridSPD.CalcHeight(dX,dY,dZ);
						else					m_pGridSPD.CalcHeightEx(dX,dY,dZ);

						//
						m_pGridHeight[k].m_pHeight[iC]	= dZ;

						m_pGridHeight[k].SetHeight(dZ);
					}
				}
				else
				{
					for(k=0;k<iNumOfU;k++)
					{
						//
						m_pGridHeight[k].m_pHeight[iC]	= dZ;

						m_pGridHeight[k].SetHeight(dZ);
					}
				}

				//绘图
				{
					DrawCell(pDC,dX,dY,dZ);
				}

			}//for(int i=0;i<m_iDemNumOfX;i++)

		}//for(int j=0;j<m_iDemNumOfY;j++)

		//m_szGrdPathName	= szPathName;
		//Save(m_szGrdPathName);

		for(k=0;k<iNumOfU;k++)
		{
			CString	szGrdPathName;
			//生成的GRD文件，应该放在和DVF相同的目录下，因此需要本类中获取
			szGrdPathName.Format("D%ld_P%ld_U%ld(%ld).grd",iSearchType,iPointNumInTotal,iU[k],m_iDemNumOfX);
			szGrdPathName	= szPathName + "\\" + szGrdPathName;

			//
			m_pGridHeight[k].m_szPathName	= szGrdPathName;

			SaveEx(szGrdPathName,m_pGridHeight[k].m_dMinHeight,m_pGridHeight[k].m_dMaxHeight,m_pGridHeight[k].m_pHeight);
		}

		m_dwFinishTime	= GetTickCount();
	}
	
	//退出MatCom
	exitM();	
}

//////////////////////////////////////////////////////////////////////
//SPD交叉验证
//////////////////////////////////////////////////////////////////////
void CDCR_GridCreator::CrossValidateInSPD(
										  CDC		*pDC, 
										  int		iSampleCount, 
										  CDCR_PointFeature *pSamplePointList,
										  CString	szPathName,
										  int		iSearchType,
										  int		iPointNumPreDirection,
										  int		iPointNumInTotal,
										  double	dSearchRadius,
										  int		iNumOfU,
										  int		*iU,
										  double	dT,
										  int		iRadiusType,
										  double	dMaxRadius,
										  int		iKernalType
										  )
{
	//如果
	if(m_pCells == NULL)						
		return;

	//
	if(pSamplePointList == NULL)
		return;

	//进入MatCom
	initM(MATCOM_VERSION);

	//使用搜索半径的次数
	m_iSearchRadiusUsed	= 0;
	
	//谢别德
	{
		//开始时间
		m_dwStartTime	= GetTickCount();
		
		//平滑指数
		m_pGridSPD.SetT(dT);
		//
		if(iRadiusType == 0)	m_pGridSPD.SetCustom(false);
		else					m_pGridSPD.SetCustom(true);

		m_pGridSPD.SetRadius(dMaxRadius);

		//搜索方式
		if(iSearchType == 0)
			m_bGlobalSearch	= true;
		else
		{
			m_bGlobalSearch	= false;

			if(iSearchType == 1)
			{
				m_enumDirection	= FourDirection;
			}
			if(iSearchType == 2)
			{
				m_enumDirection	= EightDirection;
			}
		}	
		//每个方向内的点数
		m_iPointNumPreDirection	= iPointNumPreDirection;
		//总点数
		m_iPointNumInTotal		= iPointNumInTotal;
		//搜索半径
		m_dSearchRadius			= dSearchRadius;

		//if(m_iDemNumOfX == 0 || m_iDemNumOfY == 0)
		//	return;

		int		k;
		double	dX,dY,dZ;
		
		//
		InitialHeight();

		for(k=0;k<iNumOfU;k++)
		{
			m_pGridHeight[k].m_iNumOfX	= 0;
			m_pGridHeight[k].m_iNumOfY	= 0;

			m_pGridHeight[k].m_pHeight	= new	double[iSampleCount];
		}

		int		iC;
		iC		= 0;

		//计算采样点数
		CDCR_PointFeature	*pPoint;
		
		pPoint	= pSamplePointList;
		while(pPoint != NULL)
		{
			if(pPoint->m_bUsed == false)
			{
				dX	= pPoint->m_pPointCoord.x;
				dY	= pPoint->m_pPointCoord.y;

				//缺值
				dZ	= -9999;

				//搜索采样点
				if(SearchSamplingPointCV(pPoint))
				{
					for(k=0;k<iNumOfU;k++)
					{
						//在这里输入iU
						//权指数
						m_pGridSPD.SetU(iU[k]);

						//输入顶点坐标
						m_pGridSPD.SetGridVertexList(&m_pGridVertexList);

						//开始计算
						if(iKernalType == 0)		m_pGridSPD.CalcHeight(dX,dY,dZ);
						else						m_pGridSPD.CalcHeightEx(dX,dY,dZ);

						//
						m_pGridHeight[k].m_pHeight[iC]	= pPoint->m_pPointCoord.z - dZ;

						m_pGridHeight[k].SetHeight(pPoint->m_pPointCoord.z - dZ);
					}
				}
				else
				{
					TRACE("============存在这种情况吗?============\n");
				}

				///////////////////////////////////////////////////////////
				//绘图
				{
					DrawCell(pDC,dX,dY,dZ);
				}
				///////////////////////////////////////////////////////////

				iC++;
			}

			pPoint	= pPoint->m_pNext;
		}

		//m_szGrdPathName	= szPathName;
		//Save(m_szGrdPathName);

		for(k=0;k<iNumOfU;k++)
		{
			CString	szGrdPathName;
			//生成的GRD文件，应该放在和DVF相同的目录下，因此需要本类中获取
			szGrdPathName.Format("D%ld_P%ld_U%ld(%ld)RE.txt",iSearchType,iPointNumInTotal,iU[k],m_iDemNumOfX);
			szGrdPathName	= szPathName + "\\" + szGrdPathName;

			//
			m_pGridHeight[k].m_szPathName	= szGrdPathName;

			SaveRE(szGrdPathName,m_pGridHeight[k].m_dMinHeight,m_pGridHeight[k].m_dMaxHeight,iSampleCount,pSamplePointList,m_pGridHeight[k].m_pHeight);
		}

		m_dwFinishTime	= GetTickCount();
	}
	
	//退出MatCom
	exitM();	
}

//////////////////////////////////////////////////////////////////////
//SPD交叉验证
//////////////////////////////////////////////////////////////////////
void CDCR_GridCreator::CrossValidateInSPD(
										  CDC		*pDC, 
										  int		iSampleCount, 
										  CDCR_PointFeature *pSamplePointList,
										  CDCR_PointFeature	*pPoint,
										  int		iSearchType,
										  int		iPointNumPreDirection,
										  int		iPointNumInTotal,
										  double	dSearchRadius,
										  int		iU,
										  double	dT,
										  int		iRadiusType,
										  double	dMaxRadius,
										  int		iKernalType,
										  double&	dDZ
										  )
{
	//如果
	if(m_pCells == NULL)						
		return;

	//
	if(pSamplePointList == NULL)
		return;

	//进入MatCom
	//initM(MATCOM_VERSION);

	//使用搜索半径的次数
	m_iSearchRadiusUsed	= 0;
	
	//谢别德
	{
		//开始时间
		m_dwStartTime	= GetTickCount();
		
		//平滑指数
		m_pGridSPD.SetT(dT);
		//
		if(iRadiusType == 0)	m_pGridSPD.SetCustom(false);
		else					m_pGridSPD.SetCustom(true);

		m_pGridSPD.SetRadius(dMaxRadius);

		//搜索方式
		if(iSearchType == 0)
			m_bGlobalSearch	= true;
		else
		{
			m_bGlobalSearch	= false;

			if(iSearchType == 1)
			{
				m_enumDirection	= FourDirection;
			}
			if(iSearchType == 2)
			{
				m_enumDirection	= EightDirection;
			}
		}	
		//每个方向内的点数
		m_iPointNumPreDirection	= iPointNumPreDirection;
		//总点数
		m_iPointNumInTotal		= iPointNumInTotal;
		//搜索半径
		m_dSearchRadius			= dSearchRadius;

		//if(m_iDemNumOfX == 0 || m_iDemNumOfY == 0)
		//	return;
		
		double	dX,dY,dZ;

		dX	= pPoint->m_pPointCoord.x;
		dY	= pPoint->m_pPointCoord.y;

		//缺值
		dZ	= -9999;

		//搜索采样点
		if(SearchSamplingPointCV(pPoint))
		{
			//在这里输入iU
			//权指数
			m_pGridSPD.SetU(iU);

			//输入顶点坐标
			m_pGridSPD.SetGridVertexList(&m_pGridVertexList);

			//开始计算
			if(iKernalType == 0)		m_pGridSPD.CalcHeight(dX,dY,dZ);
			else						m_pGridSPD.CalcHeightEx(dX,dY,dZ);

			//
			dDZ	= pPoint->m_pPointCoord.z - dZ;
		}
	}
	
	//退出MatCom
	//exitM();	
}

//////////////////////////////////////////////////////////////////////
// PLY批处理创建
//////////////////////////////////////////////////////////////////////
void CDCR_GridCreator::CreateInPLY(
								   CDC		*pDC,						//绘图DC
								   int		iDemNumX,					//横向格网点数
								   int		iDemNumY,					//
								   CString	szPathName,					//
								   int		iSearchType,				//搜索方向行驶
								   int		iPointNumPreDirection,		//每一方向内的点数
								   int		iPointNumInTotal,			//总点数
								   double	dSearchRadius,				//搜索半径
								   int		iModelType,					//模型种类
								   int		iInterpolationMode			//
								   )
{
	//如果
	if(m_pCells == NULL)						
		return;

	//进入MatCom
	initM(MATCOM_VERSION);

	//使用搜索半径的次数
	m_iSearchRadiusUsed	= 0;

	//多项式
	{
		//开始时间
		m_dwStartTime	= GetTickCount();

		//
		m_iDemNumOfX	= iDemNumX;
		m_iDemNumOfY	= iDemNumY;

		m_dDemSizeOfX	= (m_dMapRight-m_dMapLeft)/(m_iDemNumOfX-1);
		m_dDemSizeOfY	= (m_dMapTop-m_dMapBottom)/(m_iDemNumOfY-1);
		
		//插值模型
		m_pGridPLY.m_iModelType			= iModelType;
		//模型参数
		m_pGridPLY.m_iInterpolationMode	= iInterpolationMode;

		//搜索方式
		if(iSearchType == 0)
			m_bGlobalSearch	= true;
		else
		{
			m_bGlobalSearch	= false;

			if(iSearchType == 1)
			{
				m_enumDirection	= FourDirection;
			}
			if(iSearchType == 2)
			{
				m_enumDirection	= EightDirection;
			}
		}	
		//每个方向内的点数
		m_iPointNumPreDirection	= iPointNumPreDirection;
		//总点数
		m_iPointNumInTotal		= iPointNumInTotal;
		//搜索半径
		m_dSearchRadius			= dSearchRadius;

		if(m_iDemNumOfX == 0 || m_iDemNumOfY == 0)
			return;

		int		iC;
		double	dX,dY,dZ;
		
		if(m_pHeight != NULL)
		{
			delete	[]m_pHeight;
			m_pHeight	= NULL;
		}

		m_pHeight	= new	double[m_iDemNumOfX*m_iDemNumOfY];

		for(int j=0;j<m_iDemNumOfY;j++)
		{
			for(int i=0;i<m_iDemNumOfX;i++)
			{
				iC	= j*m_iDemNumOfX + i;
				dX	= m_dMapLeft + i*m_dDemSizeOfX;
				dY	= m_dMapBottom + j*m_dDemSizeOfY;
				
				//缺值
				dZ	= -9999;

				//搜索采样点
				if(SearchSamplingPoint(dX,dY))
				{					
					//输入顶点坐标
					m_pGridPLY.SetGridVertexList(&m_pGridVertexList);
					
					//开始计算
					m_pGridPLY.CalcHeight(dX,dY,dZ);
				}

				m_pHeight[iC]	= dZ;

				//绘制
				{
					DrawCell(pDC,dX,dY,dZ);
				}

				if(i==0 && j==0)
				{
					m_dMaxHeight	= m_dMinHeight	= dZ;
				}
				else
				{
					if(m_dMaxHeight < dZ)	m_dMaxHeight	= dZ;
					if(m_dMinHeight > dZ)	m_dMinHeight	= dZ;
				}

			}//for(int i=0;i<m_iDemNumOfX;i++)

		}//for(int j=0;j<m_iDemNumOfY;j++)

		m_szGrdPathName	= szPathName;
		Save(m_szGrdPathName);

		m_dwFinishTime	= GetTickCount();
	}

	//退出MatCom
	exitM();	
}

//////////////////////////////////////////////////////////////////////
// 保存
//////////////////////////////////////////////////////////////////////
void CDCR_GridCreator::Save(CString szPathName)
{
	if(m_pHeight == NULL)	return;

	FILE	*fp;
	fp	= fopen(szPathName,"w");

	if(fp == NULL)	return;
	
	//DSAA
	fprintf(fp,"%s\n","DSAA");

	//iCols,iRows,应该判断清楚什么是行和列
	fprintf(fp,"%6ld %6ld\n",m_iDemNumOfX,m_iDemNumOfY);
	
	//dMinX,dMaxX
	fprintf(fp,"%10.6lf %10.6lf\n",m_dMapLeft,m_dMapRight);

	//dMinY,dMaxY
	fprintf(fp,"%10.6lf %10.6lf\n",m_dMapBottom,m_dMapTop);

	//dMinZ,dMaxZ
	fprintf(fp,"%10.6lf %10.6lf\n",m_dMinHeight,m_dMaxHeight);
	
	for(int j=0;j<m_iDemNumOfY;j++)
	{
		for(int i=0;i<m_iDemNumOfX;i++)
		{
			double	dH;
			dH	= m_pHeight[j*m_iDemNumOfX+i];

			fprintf(fp,"%10.6lf ",dH);

			if((i+1) % 10 == 0)
				printf("\n");
		}

		printf("\n");
	}
	
	//关闭
	fclose(fp);

	//保存方差数据
	if(m_pHeightSD != NULL)
	{
		CString	szSDPathName;
		szSDPathName	= szPathName.Left(szPathName.GetLength()-4) + "SD.grd";
		FILE	*fp;
		fp	= fopen(szSDPathName,"w");

		if(fp == NULL)	return;
		
		//DSAA
		fprintf(fp,"%s\n","DSAA");

		//iCols,iRows,应该判断清楚什么是行和列
		fprintf(fp,"%6ld %6ld\n",m_iDemNumOfX,m_iDemNumOfY);
		
		//dMinX,dMaxX
		fprintf(fp,"%10.6lf %10.6lf\n",m_dMapLeft,m_dMapRight);

		//dMinY,dMaxY
		fprintf(fp,"%10.6lf %10.6lf\n",m_dMapBottom,m_dMapTop);

		//dMinZ,dMaxZ
		fprintf(fp,"%10.6lf %10.6lf\n",m_dMinHeight,m_dMaxHeight);
		
		for(int j=0;j<m_iDemNumOfY;j++)
		{
			for(int i=0;i<m_iDemNumOfX;i++)
			{
				double	dH;
				dH	= m_pHeightSD[j*m_iDemNumOfX+i];

				fprintf(fp,"%10.6lf ",dH);

				if((i+1) % 10 == 0)
					printf("\n");
			}

			printf("\n");
		}
		
		//关闭
		fclose(fp);		
	}
}

//////////////////////////////////////////////////////////////////////
// 保存
//////////////////////////////////////////////////////////////////////
void CDCR_GridCreator::SaveEx(CString szPathName,double dMinHeight,double dMaxHeight,double *pHeight)
{
	if(pHeight == NULL)	return;

	FILE	*fp;
	fp	= fopen(szPathName,"w");

	if(fp == NULL)	return;
	
	//DSAA
	fprintf(fp,"%s\n","DSAA");

	//iCols,iRows,应该判断清楚什么是行和列
	fprintf(fp,"%6ld %6ld\n",m_iDemNumOfX,m_iDemNumOfY);
	
	//dMinX,dMaxX
	fprintf(fp,"%10.6lf %10.6lf\n",m_dMapLeft,m_dMapRight);

	//dMinY,dMaxY
	fprintf(fp,"%10.6lf %10.6lf\n",m_dMapBottom,m_dMapTop);

	//dMinZ,dMaxZ
	fprintf(fp,"%10.6lf %10.6lf\n",dMinHeight,dMaxHeight);
	
	for(int j=0;j<m_iDemNumOfY;j++)
	{
		for(int i=0;i<m_iDemNumOfX;i++)
		{
			double	dH;
			dH	= pHeight[j*m_iDemNumOfX+i];

			fprintf(fp,"%10.6lf ",dH);

			if((i+1) % 10 == 0)
				printf("\n");
		}

		printf("\n");
	}
	
	//关闭
	fclose(fp);
}

//////////////////////////////////////////////////////////////////////
// 保存残差
//////////////////////////////////////////////////////////////////////
void CDCR_GridCreator::SaveRE(CString szPathName,double dMinHeight,double dMaxHeight,int iCount,CDCR_PointFeature *pSamplePointList,double *pHeight)
{
	//
	if(pHeight == NULL)				return;
	//
	if(pSamplePointList == NULL)	return;

	FILE	*fp;
	fp	= fopen(szPathName,"w");

	if(fp == NULL)	return;
	
	CDCR_PointFeature	*pPoint;
	int					i;

	i		= 0;
	pPoint	= pSamplePointList;
	while(pPoint != NULL)
	{
		if(pPoint->m_bUsed == false)
		{
			double	dX,dY,dZ,dRE;

			dX	= pPoint->m_pPointCoord.x;
			dY	= pPoint->m_pPointCoord.y;
			dZ	= pPoint->m_pPointCoord.z;

			dRE	= pHeight[i];
			fprintf(fp,"%10.6lf \t%10.6lf \t%10.6lf \t%10.6lf \n",dX,dY,dZ,dRE);

			i++;
		}

		pPoint	= pPoint->m_pNext;
	}
	
	//关闭
	fclose(fp);
}

//////////////////////////////////////////////////////////////////////
// 获取DEM的高程数据
//////////////////////////////////////////////////////////////////////
double* CDCR_GridCreator::GetHeight()
{
	if(m_pHeight == NULL)	return	NULL;
	
	return	m_pHeight;
}

//////////////////////////////////////////////////////////////////////
//计算残差
//////////////////////////////////////////////////////////////////////
bool CDCR_GridCreator::IsCalcRemainsError()
{
	return	m_bCaclRemainsError;
}

//////////////////////////////////////////////////////////////////////
// 获取GRD文件路径
//////////////////////////////////////////////////////////////////////
CString CDCR_GridCreator::GetGrdPathName()
{
	return	m_szGrdPathName;
}

//////////////////////////////////////////////////////////////////////
// 获取使用时间
//////////////////////////////////////////////////////////////////////
int CDCR_GridCreator::GetUsedTime()
{
	return	(int)(m_dwFinishTime-m_dwStartTime);
}

//////////////////////////////////////////////////////////////////////
// 获取使用使用了搜索半径的限制
//////////////////////////////////////////////////////////////////////
int CDCR_GridCreator::GetSearchRadiusUsed()
{
	return	m_iSearchRadiusUsed;
}

//////////////////////////////////////////////////////////////////////
// 初始化Height
//////////////////////////////////////////////////////////////////////
void CDCR_GridCreator::InitialHeight()
{
	for(int i=0;i<100;i++)
	{
		m_pGridHeight[i].Release();
	}
}
