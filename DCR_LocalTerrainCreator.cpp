// DCR_LocalTerrainCreator.cpp: implementation of the CDCR_LocalTerrainCreator class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "vr_demcreator.h"
#include "DCR_LocalTerrainCreator.h"

#include "DCR_LocalTerrainCharacter.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDCR_LocalTerrainCreator::CDCR_LocalTerrainCreator()
{

	//导入CDCR_WorkSpace中的分区格网数据
	//分区格网数据
	m_pCells				= NULL;
	//
	//分区格网大小
	m_dCellSizeOfX			= 0;
	m_dCellSizeOfY			= 0;
	
	//分区格网块数
	m_iCellNumOfX			= 0;
	m_iCellNumOfY			= 0;

	//分区格网范围
	m_dCellLeft				= 0;
	m_dCellRight			= 0;
	m_dCellBottom			= 0;
	m_dCellTop				= 0;

	//点格局值
	m_dPointPatternValue	= 0;
	//
	m_dCentroidValue		= 0;

	//平均坡度值
	m_dMeanSlope			= 0;
	//地形粗糙度值
	m_dMeanRoughness		= 0;
	//
	m_dMaxDistance			= 0;
	m_dMinDistance			= 0;
}

CDCR_LocalTerrainCreator::~CDCR_LocalTerrainCreator()
{	
	OnRelease();
}

//原始数据的分区格网数据
void CDCR_LocalTerrainCreator::SetCell(CDCR_Cells *pCells,int iNumX,int iNumY,double dSizeX,double dSizeY,double dLeft,double dRight,double dBottom,double dTop)
{
	//
	m_pCells				= pCells;

	//分区格网大小
	m_dCellSizeOfX			= dSizeX;
	m_dCellSizeOfY			= dSizeY;
	
	//分区格网块数
	m_iCellNumOfX			= iNumX;
	m_iCellNumOfY			= iNumY;

	//分区格网范围
	m_dCellLeft				= dLeft;
	m_dCellRight			= dRight;
	m_dCellBottom			= dBottom;
	m_dCellTop				= dTop;
}

//
void CDCR_LocalTerrainCreator::OnRelease()
{
	//
	m_pGridVertexList.Release();
	//
	m_pAllGridVertexList.Release();
	//
	m_pTriangleList.Release();
}

//
void CDCR_LocalTerrainCreator::CalculateLocalTerrainCharacter(	int iSampleCount, 
																CDCR_PointFeature *pSamplePointList,
																CString	szPathName,
																int		iSearchType,
																int		iPointNumPreDirection,
																int		iPointNumInTotal,
																double	dSearchRadius
																)
{
	//如果
	if(m_pCells == NULL)			return;

	if(pSamplePointList == NULL)	return;

	//接收搜索参数
	{
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
	}

	//写入文件
	FILE	*fw;
	CString	szTerrainCharacterPathName;

	szTerrainCharacterPathName.Format("D%ld_P%ld_TE.txt",iSearchType,m_iPointNumInTotal);
	szTerrainCharacterPathName	= szPathName + "\\" + szTerrainCharacterPathName;
	
	fw	= fopen(szTerrainCharacterPathName,"w");
	if(fw != NULL)
	{
		//写入文件头
		{
			fprintf(fw,"横坐标\t 纵坐标\t 点格局值\t 质心值\t 平均坡度\t 粗糙度 最大距离\t  最小距离\t \n");
		}

		//计算采样点数
		CDCR_PointFeature	*pPoint;
		
		pPoint	= pSamplePointList;
		while(pPoint != NULL)
		{
			if(pPoint->m_bUsed == false)
			{
				double	dX,dY;
				dX	= pPoint->m_pPointCoord.x;
				dY	= pPoint->m_pPointCoord.y;

				if(SearchSamplingPoint(pPoint))
				{
					//这是半径
					double	dMaxDistance;
					CalculateSearchRadius(dX,dY,dMaxDistance);

					//重新搜索
					ReSearchSamplingPoint(pPoint,dMaxDistance);
					
					//计算地形特征值
					//假如搜索得到的采样点存在，则计算
					if(m_pAllGridVertexList.GetGridVertexCount() > 0)
					{
						CDCR_LocalTerrainCharacter	pLocalTerrainCharacter;
						
						//还是需要用全部点
						//pLocalTerrainCharacter.SetGridVertexList(&m_pAllGridVertexList,&m_pGridVertexList,dMaxDistance,dX,dY);
						pLocalTerrainCharacter.SetGridVertexList(&m_pAllGridVertexList,&m_pAllGridVertexList,dMaxDistance,dX,dY);
						
						//计算点格局
						pLocalTerrainCharacter.CalculatePointPattern();

						//计算坡度和粗糙度
						pLocalTerrainCharacter.CaculateTerrainFactor();

						
						fprintf(fw,"%lf\t %lf\t",dX,dY);
						
						//假如没有搜索到有效的点数，则属于残次品，不能用于判断，可能会出现问题
						/*
						if(m_iPointNumInTotal != m_pGridVertexList.GetGridVertexCount())
						{
							pLocalTerrainCharacter.SetMeanSlope(-32767);
							pLocalTerrainCharacter.SetPointPatternValue(-32767);
							pLocalTerrainCharacter.SetMeanRoughness(-32767);
							pLocalTerrainCharacter.SetCentroidValue(-32767);
						}
						*/

						//
						pLocalTerrainCharacter.OnSave(fw);

						fprintf(fw,"\n");
					}
				}
			}

			//
			pPoint	= pPoint->m_pNext;
		}
		
		//
		fclose(fw);
	}
}

//
void CDCR_LocalTerrainCreator::CalculateLocalTerrainCharacter(	int iSampleCount, 
																CDCR_PointFeature *pSamplePointList,
																CDCR_PointFeature *pSelectPoint,																
																int		iSearchType,
																int		iPointNumPreDirection,
																int		iPointNumInTotal,
																double	dSearchRadius
																)
{
	//初始化
	m_dPointPatternValue= -32767;
	m_dCentroidValue	= -32767;
	m_dMeanSlope		= -32767;
	m_dMeanRoughness	= -32767;
	m_dMaxDistance		= -32767;
	m_dMinDistance		= -32767;

	//如果
	if(m_pCells == NULL)			return;

	if(pSamplePointList == NULL)	return;

	if(pSelectPoint == NULL)		return;

	//接收搜索参数
	{
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
	}

	double	dX,dY;
	dX	= pSelectPoint->m_pPointCoord.x;
	dY	= pSelectPoint->m_pPointCoord.y;

	if(SearchSamplingPoint(pSelectPoint))
	{
		//这是半径
		double	dMaxDistance;
		CalculateSearchRadius(dX,dY,dMaxDistance);

		//重新搜索
		ReSearchSamplingPoint(pSelectPoint,dMaxDistance);
		
		//计算地形特征值
		//假如搜索得到的采样点存在，则计算
		if(m_pAllGridVertexList.GetGridVertexCount() > 0)
		{
			if(iPointNumInTotal != m_pGridVertexList.GetGridVertexCount())
			{
				m_dPointPatternValue= -32767;
				m_dCentroidValue	= -32767;
				m_dMeanSlope		= -32767;
				m_dMeanRoughness	= -32767;
				m_dMaxDistance		= -32767;
				m_dMinDistance		= -32767;
			}
			else
			{
				CDCR_LocalTerrainCharacter	pLocalTerrainCharacter;
				
				//还是要用最大距离内的全部点
				//pLocalTerrainCharacter.SetGridVertexList(&m_pAllGridVertexList,&m_pGridVertexList,dMaxDistance,dX,dY);
				pLocalTerrainCharacter.SetGridVertexList(&m_pAllGridVertexList,&m_pAllGridVertexList,dMaxDistance,dX,dY);
				
				//计算点格局
				pLocalTerrainCharacter.CalculatePointPattern();

				//计算坡度和粗糙度
				pLocalTerrainCharacter.CaculateTerrainFactor();

				//获取构建的三角网信息
				m_pTriangleList.Release();
				m_pTriangleList.Clone(pLocalTerrainCharacter.GetTriangleList());
				
				//
				m_dPointPatternValue= pLocalTerrainCharacter.GetPointPatternValue();
				m_dCentroidValue	= pLocalTerrainCharacter.GetCentroidValue();
				m_dMeanSlope		= pLocalTerrainCharacter.GetMeanSlope() * 180 /PI;
				m_dMeanRoughness	= pLocalTerrainCharacter.GetMeanRoughness();
				m_dMaxDistance		= pLocalTerrainCharacter.GetMaxDistance();
				m_dMinDistance		= pLocalTerrainCharacter.GetMinDistance();
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////
//使用交叉验证计算精度值
//CDCR_PointFeature *pDelPoint：	为删除的需要重新插值的点
//////////////////////////////////////////////////////////////////////
bool  CDCR_LocalTerrainCreator::SearchSamplingPoint(CDCR_PointFeature *pDelPoint)
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

										//
										if(fabs(dAngle-2*PI) < 1e-5)	dAngle	= 0;

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

										//
										if(fabs(dAngle-2*PI) < 1e-5)	dAngle	= 0;

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

//重新搜索点数
bool  CDCR_LocalTerrainCreator::ReSearchSamplingPoint(CDCR_PointFeature *pDelPoint,double dRadius)
{
	if(pDelPoint == NULL)	return	false;
	
	if(m_pCells == NULL)	return	false;

	//首先删除
	m_pAllGridVertexList.Release();

	double	dX,dY;
	dX	= pDelPoint->m_pPointCoord.x;
	dY	= pDelPoint->m_pPointCoord.y;

	double	dLeft,dRight,dBottom,dTop;
	dLeft	= dX - dRadius;
	dRight	= dX + dRadius;
	dBottom	= dY - dRadius;
	dTop	= dY + dRadius;

	//首先：根据(dLeft,dRight,dBottom,dTop)确定插值点所在的格网
	int		iBegin,iEnd;
	int		jBegin,jEnd;

	iBegin		= (int)((dLeft - m_dCellLeft) / m_dCellSizeOfX) - 1;
	iEnd		= (int)((dRight - m_dCellLeft) / m_dCellSizeOfX) + 1;

	jBegin		= (int)((dBottom - m_dCellBottom) / m_dCellSizeOfY) - 1;
	jEnd		= (int)((dTop - m_dCellBottom) / m_dCellSizeOfY) + 1;

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
			
			//				
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
						
						if(dDistance <= dRadius+0.000001)
						{
							//
							m_pAllGridVertexList.AddGridVertexEx(pPoint->m_pPointCoord.x,pPoint->m_pPointCoord.y,pPoint->m_pPointCoord.z,dDistance);
						}
					}
				}

				pPFD	= pPFD->m_pNext;
			}
		}
	}
	
	return	true;
}

//计算已找到点的半径，也就是最远距离
bool  CDCR_LocalTerrainCreator::CalculateSearchRadius(double dX, double dY, double& dMaxDistance)
{
	if(m_pGridVertexList.GetGridVertexCount() == 0)	return	false;

	CDCR_GridVertex	*pVertex;
	bool			bFirstPoint;

	bFirstPoint	= true;
	pVertex		= m_pGridVertexList.GetGridVertex();
	while(pVertex != NULL)
	{
		if(bFirstPoint)
		{
			dMaxDistance	= pVertex->m_dDistance;

			bFirstPoint		= false;
		}
		else
		{
			if(dMaxDistance < pVertex->m_dDistance)	
				dMaxDistance	= pVertex->m_dDistance;
		}

		pVertex	= pVertex->m_pNext;
	}
	
	double	dTempDistance;
	dTempDistance	= 0;

	bFirstPoint		= true;
	pVertex			= m_pGridVertexList.GetGridVertex();
	while(pVertex != NULL)
	{
		double	dDistance;
		dDistance	= GetDistance(dX,dY,0,pVertex->m_dX,pVertex->m_dY,0);

		if(bFirstPoint)
		{
			dTempDistance	= dDistance;
			bFirstPoint		= false;
		}
		else
		{
			if(dTempDistance < dDistance)	dTempDistance	= dDistance;
		}

		pVertex	= pVertex->m_pNext;	
	}

	return	true;
}

//////////////////////////////////////////////////////////////////////
//格网标志归零
//////////////////////////////////////////////////////////////////////
void CDCR_LocalTerrainCreator::CellReset()
{
	if(m_pCells != NULL)
	{
		for(int i=0;i<m_iCellNumOfX*m_iCellNumOfY;i++)
		{
			m_pCells[i].m_bUsed	= false;
		}
	}
}

//////////////////////////////////////////////////////////////////////
// 01 获取两点间的距离
//////////////////////////////////////////////////////////////////////
double CDCR_LocalTerrainCreator::GetDistance(double x1, double y1, double z1, double x2, double y2, double z2)
{
	double	dDistance;

	//dDistance	= sqrt( (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) + (z1-z2)*(z1-z2) );
	dDistance	= sqrt( (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) );

	return	dDistance;
}

//////////////////////////////////////////////////////////////////////
// 02 获取角度
//////////////////////////////////////////////////////////////////////
double CDCR_LocalTerrainCreator::GetAngle(double dX, double dY)
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

//
double CDCR_LocalTerrainCreator::GetPointPatternValue()
{
	return	m_dPointPatternValue;
}

//
double CDCR_LocalTerrainCreator::GetCentroidValue()
{
	return	m_dCentroidValue;
}

//
double CDCR_LocalTerrainCreator::GetMeanSlope()
{
	return	m_dMeanSlope;
}

//
double CDCR_LocalTerrainCreator::GetMeanRoughness()
{
	return	m_dMeanRoughness;
}

//
double CDCR_LocalTerrainCreator::GetMaxDistance()
{
	return	m_dMaxDistance;
}

//
double CDCR_LocalTerrainCreator::GetMinDistance()
{
	return	m_dMinDistance;
}

//
CDCR_GridVertexList* CDCR_LocalTerrainCreator::GetGridVertexList()
{
	return	&m_pGridVertexList;
}

CDCR_GridVertexList* CDCR_LocalTerrainCreator::GetAllGridVertexList()
{
	return	&m_pAllGridVertexList;
}

//
CDCR_TinTriangleList* CDCR_LocalTerrainCreator::GetTriangleList()
{
	return	&m_pTriangleList;
}