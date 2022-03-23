// DCR_WorkSpace.cpp: implementation of the CDCR_WorkSpace class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DCR_WorkSpace.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDCR_WorkSpace::CDCR_WorkSpace()
{
	//
	m_pPointHead	= NULL;
	m_pPointLast	= NULL;

	//
	m_pLineHead		= NULL;
	m_pLineLast		= NULL;

	//
	m_pCurrentPoint	= NULL;

	//
	m_pCells		= NULL;

	//地图区域
	m_dMapLeft		= 0.0;
	m_dMapRight		= 0.0;
	m_dMapBottom	= 0.0;
	m_dMapTop		= 0.0;
	
	//缩放系数
	m_dMapXA		= 1.0;
	m_dMapXB		= 0.0;
	m_dMapYA		= -1.0;
	m_dMapYB		= 0.0;

	//地图格网大小
	m_dCellSizeOfY	= 0.0;
	m_dCellSizeOfX	= 0.0;
	
	//地图分块数
	m_nCellNumOfY	= 16;
	m_nCellNumOfX	= 24;

	//划分网格范围
	m_dCellLeft		= m_dCellRight	= 0;
	m_dCellBottom	= m_dCellTop	= 0;

	//
	m_dMaxH			= -1;
	m_dMidH			= -1;
	m_dMinH			= -1;

	//分层设色信息
	m_crMinColor	= RGB(95,165,111);
	m_crMidColor	= RGB(250,240,190);
	m_crMaxColor	= RGB(254,182,129);

	//最小距离
	m_dMinDistance	= 0.5;

	m_iVertexCount	= 0;

	//地图内容显示状态
	m_bShowStatusInContent	= true;
	//格网线显示状态
	m_bShowStatusInCell		= true;
}

CDCR_WorkSpace::~CDCR_WorkSpace()
{
	Release();
}

//
bool CDCR_WorkSpace::IsEmpty()
{
	if(m_pPointHead==NULL && m_pLineHead==NULL)		return	true;

	return	false;
}

//矩形和矩形相交判断函数
bool CDCR_WorkSpace::RectCrossWithRect(double dMinX, double dMaxX, double dMinY, double dMaxY, double dLeft, double dRight, double dBottom, double dTop)
{
	if(dMaxX < dLeft || dMinX > dRight || dMinY > dTop || dMaxY < dBottom)
		return	false;
	return	true;
}

void CDCR_WorkSpace::ScreenToMap(int iX, int iY, double &dX, double &dY)
{
	dX	= (iX - m_dMapXB) / m_dMapXA;
	dY	= (iY - m_dMapYB) / m_dMapYA;
}

void CDCR_WorkSpace::MapToScreen(double dX, double dY, int &iX, int &iY)
{
	iX	= (int)(dX*m_dMapXA + m_dMapXB + 0.5);
	iY	= (int)(dY*m_dMapYA + m_dMapYB + 0.5);
}

void CDCR_WorkSpace::DrawContent(CDC *pDC)
{
	if(m_pPointHead==NULL && m_pLineHead==NULL)	return;

	double	dShowLeft,dShowRight,dShowBottom,dShowTop;
	//计算实际的显示区域
	dShowLeft	= (m_rRect.left - m_dMapXB) / m_dMapXA;
	dShowRight	= (m_rRect.right - m_dMapXB) / m_dMapXA;
	dShowTop	= (m_rRect.top - m_dMapYB) / m_dMapYA;
	dShowBottom	= (m_rRect.bottom - m_dMapYB) / m_dMapYA;

	{
		CDCR_PointFeature	*pPF;
		
		pPF	= m_pPointHead;
		while(pPF != NULL)
		{
			if( (pPF->m_pPointCoord.x>=dShowLeft&&pPF->m_pPointCoord.x<=dShowRight) && 
				(pPF->m_pPointCoord.y>=dShowBottom&&pPF->m_pPointCoord.y<=dShowTop))
			{
				COLORREF	crColor;
				crColor	= GetColorFromH(pPF->m_pPointCoord.z);

				//
				pPF->OnDraw(pDC,m_dMapXA,m_dMapXB,m_dMapYA,m_dMapYB,crColor);
			}

			pPF	= pPF->m_pNext;
		}
	}

	if(m_pCurrentPoint != NULL)
	{
		int		iX,iY;
		MapToScreen(m_pCurrentPoint->m_pPointCoord.x,m_pCurrentPoint->m_pPointCoord.y,iX,iY);

		CPen	*pNewPen,*pOldPen;
		pNewPen		= new	CPen;
		pNewPen->CreatePen(PS_SOLID,1,RGB(255,0,0));
		pOldPen		= pDC->SelectObject(pNewPen);

		CGdiObject	*pBackup;
		pBackup		= pDC->SelectStockObject(NULL_BRUSH);

		pDC->Ellipse(iX-5,iY-5,iX+5,iY+5);

		pDC->SelectObject(pOldPen);
		delete	pNewPen;

		pDC->SelectObject(pBackup);	
	}

	//
	//{
	//CDCR_LineFeature	*pLF;

	//pLF	= m_pLineHead;
	//while(pLF != NULL)
	//{
	//	//
	//	if(RectCrossWithRect(dShowLeft,dShowRight,dShowBottom,dShowTop,
	//		pLF->m_dLeft,pLF->m_dRight,pLF->m_dBottom,pLF->m_dTop))
	//	{
	//		COLORREF	crColor;
	//		crColor	= GetColorFromH(pLF->m_pPointCoord.z);
	//
	//		//
	//		pLF->OnDraw(pDC,m_dMapXA,m_dMapXB,m_dMapYA,m_dMapYB,crColor);
	//	}
	//
	//	pLF	= pLF->m_pNext;
	//}
	//}
}

void CDCR_WorkSpace::DrawCell(CDC *pDC)
{
	if(m_pCells == NULL)	return;
	//先绘制纵线
	{
		double	dYT,dYB;
		double	dX;

		dYB	= m_dCellBottom;
		dYT	= m_dCellTop;

		POINT	ptPoint[2];
		int		iX,iY;
		
		for(int i=0;i<=m_nCellNumOfX;i++)
		{
			dX	= m_dCellLeft + i*m_dCellSizeOfX;
			
			MapToScreen(dX,dYB,iX,iY);
			ptPoint[0].x	= iX;
			ptPoint[0].y	= iY;

			MapToScreen(dX,dYT,iX,iY);
			ptPoint[1].x	= iX;
			ptPoint[1].y	= iY;

			pDC->Polyline(ptPoint,2);
		}
	}

	//先绘制横线
	{
		double	dXL,dXR;
		double	dY;

		dXL	= m_dCellLeft;
		dXR	= m_dCellRight;

		POINT	ptPoint[2];
		int		iX,iY;
		
		for(int i=0;i<=m_nCellNumOfY;i++)
		{
			dY	= m_dCellBottom + i*m_dCellSizeOfY;
			
			MapToScreen(dXL,dY,iX,iY);
			ptPoint[0].x	= iX;
			ptPoint[0].y	= iY;

			MapToScreen(dXR,dY,iX,iY);
			ptPoint[1].x	= iX;
			ptPoint[1].y	= iY;

			pDC->Polyline(ptPoint,2);
		}
	}
}

//绘制
void CDCR_WorkSpace::Draw(CDC *pDC)
{
	//计算中值
	m_dMidH	= (m_dMinH + m_dMaxH)/2;
	
	//
	if(m_bShowStatusInContent)
		DrawContent(pDC);

	//绘制格网
	if(m_bShowStatusInCell)
		DrawCell(pDC);
}

void CDCR_WorkSpace::SetClientRect(CRect rRect)
{	
	m_rRect	= rRect;
}

//
void CDCR_WorkSpace::SetCoordMapping(double dXA,double dXB,double dYA,double dYB)
{
	m_dMapXA	= dXA;
	m_dMapXB	= dXB;

	m_dMapYA	= dYA;
	m_dMapYB	= dYB;
}

//
void CDCR_WorkSpace::GetCoordMapping(double& dXA,double& dXB,double& dYA,double& dYB)
{
	dXA	= m_dMapXA;
	dXB	= m_dMapXB;

	dYA	= m_dMapYA;
	dYB	= m_dMapYB;
}

//
void CDCR_WorkSpace::SetMapExt(double dLeft,double dRight,double dBottom,double dTop)
{
	m_dMapLeft		= dLeft;
	m_dMapRight		= dRight;
	m_dMapBottom	= dBottom;
	m_dMapTop		= dTop;
}

//
void CDCR_WorkSpace::GetMapExt(double& dLeft,double& dRight,double& dBottom,double& dTop)
{
	dLeft	= m_dMapLeft;
	dRight	= m_dMapRight;
	dBottom	= m_dMapBottom;
	dTop	= m_dMapTop;
}

//
void CDCR_WorkSpace::GetHeightExt(double& dMinH,double& dMaxH)
{
	dMinH	= m_dMinH;
	dMaxH	= m_dMaxH;
}

//
void CDCR_WorkSpace::SetMinDistance(double dMinDistance)
{
	m_dMinDistance	= dMinDistance;
}

//
double CDCR_WorkSpace::GetMinDistance()
{
	return	m_dMinDistance;
}

//uDVF
bool CDCR_WorkSpace::Read(CString szPathName)
{
	FILE	*fp;

	fp	= fopen(szPathName,"r");
	if(fp == NULL)	return	false;
	
	//
	m_szPathName	= szPathName;

	//
	double	dMapVersion;
	fscanf(fp,"%lf",&dMapVersion);

	//
	long	nMapScale;
	fscanf(fp,"%ld",&nMapScale);

	//
	fscanf(fp,"%lf%lf%lf%lf",&m_dMapLeft,&m_dMapBottom,&m_dMapRight,&m_dMapTop);

	//
	long	nMapUnite;
	fscanf(fp,"%ld",&nMapUnite);

	long	iSign;
	long	iPointNumber;
	long	iMainType;
	long	iSubType;
	char	chName[60];
	double	dHeight;
	int		iPointNum;
	
	m_dMinH	= m_dMaxH	= -1;

	while(fscanf(fp,"%ld%ld%ld%ld%s%lf%ld",&iSign,&iPointNumber,&iMainType,&iSubType,chName,&dHeight,&iPointNum) == 7)
	{
		if(m_dMinH==-1 && m_dMaxH==-1)
		{
			m_dMinH	= m_dMaxH	= dHeight;
		}
		else
		{
			if(m_dMinH > dHeight)	m_dMinH	= dHeight;
			if(m_dMaxH < dHeight)	m_dMaxH	= dHeight;
		}
		
		//点对象
		if(iSign == 100)
		{
			for(int i=0;i<iPointNum;i++)
			{
				double	dX,dY;
				fscanf(fp,"%lf%lf",&dX,&dY);
				
				CDCR_PointFeature	*pPF;
					
				pPF	= new	CDCR_PointFeature;
				pPF->AddPoint(dX,dY,dHeight);

				if(m_pPointHead == NULL)
				{
					m_pPointHead	= pPF;
					m_pPointLast	= pPF;
				}
				else
				{
					m_pPointLast->m_pNext	= pPF;
					m_pPointLast			= pPF;
				}
			}
		}
		else
		//线对象，现在按照点对象处理
		if(iSign == 200)
		{
			for(int i=0;i<iPointNum;i++)
			{
				double	dX,dY;

				fscanf(fp,"%lf%lf",&dX,&dY);
				
				CDCR_PointFeature	*pPF;
					
				pPF	= new	CDCR_PointFeature;
				pPF->AddPoint(dX,dY,dHeight);

				if(m_pPointHead == NULL)
				{
					m_pPointHead	= pPF;
					m_pPointLast	= pPF;
				}
				else
				{
					m_pPointLast->m_pNext	= pPF;
					m_pPointLast			= pPF;
				}
			}
		}
		else
		//面对象，现在暂时不处理
		if(iSign == 300)
		{
			//
			MessageBox(NULL,"Wrong","提示信息",MB_OK);
		}
		else
		{
			//
			MessageBox(NULL,"Wrong","提示信息",MB_OK);
		}
	}
	
	//创建网格
	CellsBuild();

	return	true;
}

//
bool CDCR_WorkSpace::Import(CString szPathName)
{
	FILE	*fp;

	fp	= fopen(szPathName,"r");
	if(fp == NULL)	return	false;
	
	m_szPathName	= szPathName;
	//char	chInfo[128];
	//fscanf(fp,"%s,%s,%s",chInfo,chInfo,chInfo);

	int		i;
	double	dX,dY,dZ;

	i		= 0;	
	while(fscanf(fp,"%lf%lf%lf",&dX,&dY,&dZ) == 3)
	{
		if(i==0)
		{
			m_dMapLeft		= m_dMapRight	= dX;
			m_dMapBottom	= m_dMapTop		= dY;

			m_dMinH			= m_dMaxH		= dZ;
		}
		else
		{	
			if(m_dMapLeft > dX)		m_dMapLeft		= dX;
			if(m_dMapRight < dX)	m_dMapRight		= dX;

			if(m_dMapBottom > dY)	m_dMapBottom	= dY;
			if(m_dMapTop < dY)		m_dMapTop		= dY;

			if(m_dMinH > dZ)		m_dMinH			= dZ;
			if(m_dMaxH < dZ)		m_dMaxH			= dZ;
		}

		//
		CDCR_PointFeature	*pPF;			
		pPF	= new	CDCR_PointFeature;
		pPF->AddPoint(dX,dY,dZ);

		if(m_pPointHead == NULL)
		{
			m_pPointHead	= pPF;
			m_pPointLast	= pPF;
		}
		else
		{
			m_pPointLast->m_pNext	= pPF;
			m_pPointLast			= pPF;
		}		
		i++;
	}
	
	//创建网格
	CellsBuild();

	return	true;
}

//
bool CDCR_WorkSpace::ImportDEM(CString szPathName)
{
	FILE	*fp;

	fp	= fopen(szPathName,"rb");
	if(fp == NULL)	return	false;
	
	fread(&m_dMapLeft,sizeof(double),1,fp);
	fread(&m_dMapBottom,sizeof(double),1,fp);
	fread(&m_dMapRight,sizeof(double),1,fp);
	fread(&m_dMapTop,sizeof(double),1,fp);
	
	int	iNumOfX,iNumOfY;
	fread(&iNumOfX,sizeof(long),1,fp);
	fread(&iNumOfY,sizeof(long),1,fp);
	
	double	dSizeOfX,dSizeOfY;
	fread(&dSizeOfX,sizeof(double),1,fp);
	fread(&dSizeOfY,sizeof(double),1,fp);
	
	double	dRealSize;
	fread(&dRealSize,sizeof(double),1,fp);
	
	float	fMinHeight;
	float	fMaxHeight;
	fread(&fMinHeight,sizeof(float),1,fp);
	fread(&fMaxHeight,sizeof(float),1,fp);

	m_dMinH			= fMinHeight;
	m_dMaxH			= fMaxHeight;
	
	
	float	*pDemObj;
	pDemObj			= new	float[iNumOfY*iNumOfX];
	fread(pDemObj,sizeof(float),iNumOfY*iNumOfX,fp);
	
	int		i,j,ci;
	double	dX,dY,dZ;
	for(j=0;j<iNumOfY;j++)
	{
		for(i=0;i<iNumOfX;i++)
		{
			ci		= j*iNumOfX+i;
			dX		= m_dMapLeft + i*dSizeOfX;
			dY		= m_dMapBottom + j*dSizeOfY;
			dZ		= pDemObj[ci];

			//
			CDCR_PointFeature	*pPF;			
			pPF	= new	CDCR_PointFeature;
			pPF->AddPoint(dX,dY,dZ);

			if(m_pPointHead == NULL)
			{
				m_pPointHead	= pPF;
				m_pPointLast	= pPF;
			}
			else
			{
				m_pPointLast->m_pNext	= pPF;
				m_pPointLast			= pPF;
			}		
		}
	}
	
	delete	[]pDemObj;

	//
	fclose(fp);
	
	//创建网格
	CellsBuild();

	return	true;	
}

//
void CDCR_WorkSpace::Release()
{
	CDCR_PointFeature	*pf1,*pf2;
	CDCR_LineFeature	*lf1,*lf2;

	if(m_pPointHead != NULL)
	{
		pf1	= pf2	= m_pPointHead;
		do
		{
			pf1		= pf1->m_pNext;
			delete	pf2;
			pf2		= pf1;
		}while(pf1 != NULL);

		m_pPointHead	= NULL;
		m_pPointLast	= NULL;
	}

	if(m_pLineHead != NULL)
	{
		lf1	=lf2	= m_pLineHead;
		do
		{
			lf1		= lf1->m_pNext;
			delete	lf2;
			lf2		= lf1;
		}while(lf1 != NULL);

		m_pLineHead		= NULL;
		m_pLineLast		= NULL;
	}	

	CellsRelease();

	m_pCurrentPoint	= NULL;
}

//
bool CDCR_WorkSpace::Save(CString szPathName)
{
	FILE	*fp;

	fp	= fopen(szPathName,"w");

	if(fp == NULL)	return	false;

	CDCR_PointFeature		*pPF;

	fprintf(fp,"\"Easting\",\"Northing\",\"Elevation\"\n");

	pPF	= m_pPointHead;
	while(pPF != NULL)
	{
		if(false == pPF->m_bUsed)
		{
			fprintf(fp,"%10.4lf,%10.4lf,%10.4lf\n",pPF->m_pPointCoord.x,pPF->m_pPointCoord.y,pPF->m_pPointCoord.z);
		}

		pPF	= pPF->m_pNext;
	}

	fclose(fp);
	
	return	true;
}

//
CDCR_Cells* CDCR_WorkSpace::GetCell()
{
	return	m_pCells;
}

//
void CDCR_WorkSpace::SetCellCount(int iCellNumOfX,int iCellNumOfY)
{
	m_nCellNumOfX	= iCellNumOfX;
	m_nCellNumOfY	= iCellNumOfY;
}

//
void CDCR_WorkSpace::GetCellCount(int& iCellNumOfX,int& iCellNumOfY)
{
	iCellNumOfX		= m_nCellNumOfX;
	iCellNumOfY		= m_nCellNumOfY;
}

//
void CDCR_WorkSpace::GetCellSize(double& dCellSizeOfX,double& dCellSizeOfY)
{
	dCellSizeOfX	= m_dCellSizeOfX;
	dCellSizeOfY	= m_dCellSizeOfY;
}

//
void CDCR_WorkSpace::GetCellExt(double& dLeft,double& dRight,double& dBottom,double& dTop)
{
	dLeft	= m_dCellLeft;
	dRight	= m_dCellRight;
	dBottom	= m_dCellBottom;
	dTop	= m_dCellTop;
}

//
void CDCR_WorkSpace::CellsBuild()
{
	if(m_nCellNumOfX==0 || m_nCellNumOfY==0)	return;

	if(m_pPointHead==NULL && m_pLineHead==NULL)	return;

	CDCR_PointFeature	*pPF;
	CDCR_LineFeature	*pLF;

	int i,j,ci;

	//初始化
	CellsRelease();

	//重新计算网格尺寸
	//double	dSize;
	//dSize	= sqrt( (m_dMapRight-m_dMapLeft)*(m_dMapTop-m_dMapBottom)/m_iVertexCount );
	//m_dCellSizeOfX	= m_dCellSizeOfY	= 4*dSize/3;

	//先计算格网尺寸
	m_dCellSizeOfX	= (m_dMapRight-m_dMapLeft)/m_nCellNumOfX;
	m_dCellSizeOfY	= (m_dMapTop-m_dMapBottom)/m_nCellNumOfY;

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
		}
	}

	//往格网增加点要素
	pPF		= m_pPointHead;
	while(pPF != NULL)
	{
		AddPointToCells(pPF);
		pPF	= pPF->m_pNext;
	}

	//往
	pLF		= m_pLineHead;
	while(pLF != NULL)
	{
		AddLineToCells(pLF);
		pLF	= pLF->m_pNext;
	}
}

//删除重点
void CDCR_WorkSpace::DeleteRePoint()
{
	if(m_pCells == NULL)
	{
		MessageBox(NULL,"为了提高搜索速度,请先建立采样点格网!","提示信息",MB_OK);

		return;
	}

	//
	DWORD dwStartTime = GetTickCount();

	//计算多少个重复点被删除
	CDCR_PointFeature	*pPF;

	pPF		= m_pPointHead;
	while(pPF != NULL)
	{
		pPF->m_bUsed	= false;

		pPF	= pPF->m_pNext;
	}

	pPF	= m_pPointHead;
	//遍历顶点肯定是没有问题的，问题在于循环方面。
	while(pPF != NULL)
	{	
		//
		if(pPF->m_bUsed == false)
		{
			//首先获取pPF1点所在的格网
			int	cI,cJ;
			//仅仅考虑一个点
			cI	= (int)((pPF->m_pPointCoord.x - m_dCellLeft) / m_dCellSizeOfX);
			cJ	= (int)((pPF->m_pPointCoord.y - m_dCellBottom) / m_dCellSizeOfY);
			
			int	i,j;
			int	iBegin,iEnd;
			int	jBegin,jEnd;
			
			//
			iBegin	= cI-1;
			if(iBegin < 0)				iBegin	= 0;
			if(iBegin >= m_nCellNumOfX)	iBegin	= m_nCellNumOfX-1;
			//
			iEnd	= cI+1;
			if(iEnd < 0)				iEnd	= 0;
			if(iEnd >= m_nCellNumOfX)	iEnd	= m_nCellNumOfX-1;

			//
			jBegin	= cJ-1;
			if(jBegin < 0)				jBegin	= 0;
			if(jBegin >= m_nCellNumOfY)	jBegin	= m_nCellNumOfY-1;
			//
			jEnd	= cJ+1;
			if(jEnd < 0)				jEnd	= 0;
			if(jEnd >= m_nCellNumOfY)	jEnd	= m_nCellNumOfY-1;

			int	cc;
			//分别外廓一个格网搜索重复点
			for(i=iBegin;i<=iEnd;i++)
			{
				for(j=jBegin;j<=jEnd;j++)
				{	
					//好开始搜索
					cc	= j * m_nCellNumOfX + i;
					
					CDCR_PointFeatureAddress	*pPFA;
					pPFA	= m_pCells[cc].m_pPFDHead;
					while(pPFA != NULL)
					{
						if(pPFA->m_pPointFeature!=NULL && pPF!=pPFA->m_pPointFeature)
						{
							if(pPFA->m_pPointFeature->m_bUsed == false)
							{
								double	dMinX,dMinY;

								dMinX	= pPF->m_pPointCoord.x - pPFA->m_pPointFeature->m_pPointCoord.x;
								dMinY	= pPF->m_pPointCoord.y - pPFA->m_pPointFeature->m_pPointCoord.y;

								double	dMinDistance;

								dMinDistance	= sqrt(dMinX*dMinX + dMinY*dMinY);

								if(dMinDistance < m_dMinDistance)
									pPFA->m_pPointFeature->m_bUsed	= true;
							}
						}

						pPFA	= pPFA->m_pNext;
					}
				}
			}
		}
		
		pPF	= pPF->m_pNext;
	}

	//计算多少个重复点被删除
	int					iCount;
	int					iDelCount;

	iCount		= 0;
	iDelCount	= 0;
	
	pPF		= m_pPointHead;
	while(pPF != NULL)
	{
		if(pPF->m_bUsed == true)
		{
			iDelCount++;
		}

		iCount++;

		pPF	= pPF->m_pNext;
	}

	DWORD dwFinishTime = GetTickCount();

	CString	szInfo;
	szInfo.Format("总共存在 %ld 个采样点，其中需要删除重复采样点 %ld 个! 耗时 : %dms",iCount,iDelCount,(int)(dwFinishTime-dwStartTime));

	MessageBox(NULL,szInfo,"提示信息",MB_OK);
}

void CDCR_WorkSpace::CellsRelease()
{
	if(m_pCells != NULL)
	{
		delete	[]m_pCells;
		m_pCells	= NULL;
	}
}

//
bool CDCR_WorkSpace::AddPointToCells(CDCR_PointFeature *pf)
{
	if(pf == NULL)				return	false;

	int			iPointNum;
	iPointNum	= pf->m_iPointNum;
	
	if(iPointNum == 0)			return	false;

	int							ci;	
	int							xi,yi;
	CDCR_PointFeatureAddress	*pfd;
		
	//仅仅考虑一个点
	xi	= (int)((pf->m_pPointCoord.x - m_dCellLeft) / m_dCellSizeOfX);
	yi	= (int)((pf->m_pPointCoord.y - m_dCellBottom) / m_dCellSizeOfY);

	if(xi < 0)				xi	= 0;
	if(yi < 0)				yi	= 0;
	if(xi >= m_nCellNumOfX)	xi	= m_nCellNumOfX-1;
	if(yi >= m_nCellNumOfY)	yi	= m_nCellNumOfY-1;

	ci	= yi * m_nCellNumOfX + xi;

	if(m_pCells[ci].m_pPFDHead == NULL)
	{
		pfd							= new	CDCR_PointFeatureAddress;	
		pfd->m_pPointFeature		= pf;

		m_pCells[ci].m_pPFDHead		= pfd;
		m_pCells[ci].m_pPFDLast		= pfd;
	}
	else
	{
		pfd							= new	CDCR_PointFeatureAddress;	
		pfd->m_pPointFeature		= pf;

		m_pCells[ci].m_pPFDLast->m_pNext	= pfd;
		m_pCells[ci].m_pPFDLast				= pfd;
	}

	return	true;
}

//
bool CDCR_WorkSpace::AddLineToCells(CDCR_LineFeature *lf)
{
	if(lf == NULL)			return	false;

	int		iPointNum;
	iPointNum	= lf->m_iPointNum;

	if(iPointNum == 0)		return	false;

	int		I,i,j,ci;	
	int		xB,yB,xE,yE;
	double	dMinX,dMaxX,dMinY,dMaxY;

	CDCR_LineFeatureAddress	*lfd;
	SMap_3DPointCoord		*pc;
		
	pc			= lf->m_pPointCoord;

	for(I=0;I<iPointNum-1;I++)
	{
		dMinX	= (pc[I].x < pc[I+1].x) ? pc[I].x : pc[I+1].x;
		dMaxX	= (pc[I].x > pc[I+1].x) ? pc[I].x : pc[I+1].x;
		dMinY	= (pc[I].y < pc[I+1].y) ? pc[I].y : pc[I+1].y;
		dMaxY	= (pc[I].y > pc[I+1].y) ? pc[I].y : pc[I+1].y;

		xB		= int((dMinX-m_dCellLeft) / m_dCellSizeOfX);
		xE		= int((dMaxX-m_dCellLeft) / m_dCellSizeOfX);
		yB		= int((dMinY-m_dCellBottom) / m_dCellSizeOfY);
		yE		= int((dMaxY-m_dCellBottom) / m_dCellSizeOfY);

		if(xB < 0)				xB	= 0;
		if(yB < 0)				yB	= 0;
		if(xE < 0)				xE	= 0;
		if(yE < 0)				yE	= 0;
		if(xB >= m_nCellNumOfX)	xB	= m_nCellNumOfX-1;
		if(yB >= m_nCellNumOfY)	yB	= m_nCellNumOfY-1;
		if(xE >= m_nCellNumOfX)	xE	= m_nCellNumOfX-1;
		if(yE >= m_nCellNumOfY)	yE	= m_nCellNumOfY-1;

		for(j=yB;j<=yE;j++)
		{
			for(i=xB;i<=xE;i++)
			{
				ci	= j*m_nCellNumOfX+i;

				if(m_pCells[ci].m_pLFDHead == NULL)
				{
					lfd						= new	CDCR_LineFeatureAddress;
					lfd->m_pLineFeature		= lf;

					m_pCells[ci].m_pLFDHead	= lfd;
					m_pCells[ci].m_pLFDLast	= lfd;
				}
				else
				{
					CDCR_LineFeatureAddress	*thelfd;

					bool		bFlag;
					bFlag	= true;
					thelfd	= m_pCells[ci].m_pLFDHead;
					while(thelfd != NULL)
					{
						if(thelfd->m_pLineFeature == lf)
						{
							bFlag	= false;
							break;
						}
						thelfd	= thelfd->m_pNext;
					}
					if(bFlag)
					{
						lfd							= new	CDCR_LineFeatureAddress;
						lfd->m_pLineFeature			= lf;

						m_pCells[ci].m_pLFDLast->m_pNext	= lfd;
						m_pCells[ci].m_pLFDLast				= lfd;
					}
				}
			}//i
		}//j	
	}//I

	return	true;
}

COLORREF CDCR_WorkSpace::GetColorFromH(double dH)
{
	int		hr,hg,hb;
	int		mr,mg,mb;
	int		lr,lg,lb;
	int		r,g,b;

	hr	= GetRValue(m_crMaxColor);
	hg	= GetGValue(m_crMaxColor);
	hb	= GetBValue(m_crMaxColor);

	mr	= GetRValue(m_crMidColor);
	mg	= GetGValue(m_crMidColor);
	mb	= GetBValue(m_crMidColor);

	lr	= GetRValue(m_crMinColor);
	lg	= GetGValue(m_crMinColor);
	lb	= GetBValue(m_crMinColor);

	if(dH > m_dMidH)	
	{
		if(m_dMaxH == m_dMidH)
		{
			r	= hr;
			g	= hg;
			b	= hb;
		}
		else
		{
			r	= (int)((dH-m_dMidH)*(hr-mr)/(m_dMaxH-m_dMidH)+mr);
			g	= (int)((dH-m_dMidH)*(hg-mg)/(m_dMaxH-m_dMidH)+mg);
			b	= (int)((dH-m_dMidH)*(hb-mb)/(m_dMaxH-m_dMidH)+mb);			
		}
	}
	else
	{
		if(m_dMidH == m_dMinH)
		{
			r	= lr;
			g	= lg;
			b	= lb;
		}
		else
		{
			r	= (int)((dH-m_dMinH)*(mr-lr)/(m_dMidH-m_dMinH)+lr);
			g	= (int)((dH-m_dMinH)*(mg-lg)/(m_dMidH-m_dMinH)+lg);
			b	= (int)((dH-m_dMinH)*(mb-lb)/(m_dMidH-m_dMinH)+lb);				
		}
	}

	return	RGB(r,g,b);
}

//
CDCR_PointFeature* CDCR_WorkSpace::GetPointFeature()
{
	return	m_pPointHead;
}

//获取采样点个数
int CDCR_WorkSpace::GetSamplePointCount()
{	
	CDCR_PointFeature	*pPF;
	int					iCount;
	
	iCount	= 0;
	pPF		= m_pPointHead;
	while(pPF != NULL)
	{		
		if(pPF->m_bUsed == false)
		{
			iCount++;
		}

		pPF	= pPF->m_pNext;
	}
	
	return	iCount;
}

//获取采样点信息
void CDCR_WorkSpace::GetSamplePoint(double** dX,double** dY,double** dZ)
{
	CDCR_PointFeature	*pPF;
	int					iCount;
	
	iCount	= 0;
	pPF		= m_pPointHead;
	while(pPF != NULL)
	{		
		if(pPF->m_bUsed == false)
		{
			(*dX)[iCount]	= pPF->m_pPointCoord.x;
			(*dY)[iCount]	= pPF->m_pPointCoord.y;
			(*dZ)[iCount]	= pPF->m_pPointCoord.z;

			iCount++;
		}

		pPF	= pPF->m_pNext;
	}
}

//
void CDCR_WorkSpace::SetShowStatusInContent(bool bShowStatus)
{
	m_bShowStatusInContent	= bShowStatus;
}

bool CDCR_WorkSpace::GetShowStatusInContent()
{
	return	m_bShowStatusInContent;
}

//
void CDCR_WorkSpace::SetShowStatusInCell(bool bShowStatus)
{
	m_bShowStatusInCell	= bShowStatus;
}

bool CDCR_WorkSpace::GetShowStatusInCell()
{
	return	m_bShowStatusInCell;
}

//点选捕捉点，计算该点的信息
CDCR_PointFeature* CDCR_WorkSpace::SelectPoint(double& dX,double& dY)
{
	if(m_pPointHead == NULL)	return	NULL;

	if(m_pCells == NULL)		return	NULL;
	
	//
	int		iX,iY;
	iX	= (int)((dX - m_dCellLeft) / m_dCellSizeOfX);
	iY	= (int)((dY - m_dCellBottom) / m_dCellSizeOfY);

	if(iX < 0)				iX	= 0;
	if(iY < 0)				iY	= 0;
	if(iX >= m_nCellNumOfX)	iX	= m_nCellNumOfX-1;
	if(iY >= m_nCellNumOfY)	iY	= m_nCellNumOfY-1;
	
	int		iC;
	iC	= iY * m_nCellNumOfX + iX;
	
	double				dMinDistance;
	CDCR_PointFeature	*pCurrentPF;
	bool				bFirst;
	
	//
	pCurrentPF	= NULL;
	bFirst		= true;

	//
	CDCR_PointFeatureAddress	*pPFD;
	pPFD		= m_pCells[iC].m_pPFDHead;

	while(pPFD != NULL)
	{
		CDCR_PointFeature	*pPF;
		pPF		= pPFD->GetPointFeature();
		
		if(pPF != NULL)
		{
			double	dDistance;
			dDistance	= sqrt( (pPF->m_pPointCoord.x-dX)*(pPF->m_pPointCoord.x-dX) + (pPF->m_pPointCoord.y-dY)*(pPF->m_pPointCoord.y-dY) ); 

			if(bFirst)
			{
				dMinDistance	= dDistance;
				pCurrentPF		= pPF;

				bFirst			= false;
			}
			else
			{
				if(dMinDistance > dDistance)
				{
					dMinDistance	= dDistance;
					pCurrentPF		= pPF;
				}
			}
		}

		pPFD	= pPFD->m_pNext;
	}

	if(pCurrentPF != NULL)
	{
		dX	= pCurrentPF->m_pPointCoord.x;
		dY	= pCurrentPF->m_pPointCoord.y;

		m_pCurrentPoint	= pCurrentPF;
		return	pCurrentPF;
	}

	return	NULL;
}

//
CDCR_PointFeature* CDCR_WorkSpace::SelectPoint()
{
	return	m_pCurrentPoint;
}

CString CDCR_WorkSpace::GetPathName()
{
	return	m_szPathName;
}

//提取检查点的数据
bool CDCR_WorkSpace::PickupCheckPoint(int iCount)
{
	//
	if(iCount <=0 )			return	false;
	
	//统计存在多少数据点
	CDCR_PointFeature*		pPoint;
	int						iPointNum;
	
	iPointNum	= 0;
	pPoint		= m_pPointHead;
	while(pPoint != NULL)
	{
		iPointNum++;
	
		pPoint	= pPoint->m_pNext;
	}
	
	//如果提取点数大于iPointNum，则提取
	if(iPointNum < iCount)	return	false;
	
	int		i;
	int		*iPointSerial;
	iPointSerial	= new	int[iCount];
	for(i=0;i<iCount;i++)	iPointSerial[i]	= -1;	
	
	//随机提取数据
	srand((unsigned)time(0));
	for(i=0;i<iCount;i++)
	{
		iPointSerial[i]	= rand() % iPointNum;
	}
	
	//提取
	CDCR_PointFeature	*pMyPoint;
	CDCR_PointFeature	*pMyPointHead;
	CDCR_PointFeature	*pMyPointLast;
	int					iSerial;

	pMyPointHead	= NULL;
	pMyPointLast	= NULL;

	for(i=0;i<iCount;i++)
	{
		iSerial	= 0;
		pPoint		= m_pPointHead;
		while(pPoint != NULL)
		{
			if(iPointSerial[i] == iSerial)
			{
				//
				pMyPoint	= new	CDCR_PointFeature;
				
				pMyPoint->AddPoint(pPoint->m_pPointCoord.x,pPoint->m_pPointCoord.y,pPoint->m_pPointCoord.z);

				//
				if(pMyPointHead == NULL)
				{
					pMyPointHead	= pMyPoint;
					pMyPointLast	= pMyPoint;
				}
				else
				{
					pMyPointLast->m_pNext	= pMyPoint;
					pMyPointLast			= pMyPoint;
				}
				
				break;
			}			
			
			iSerial++;
			
			pPoint	= pPoint->m_pNext;
		}
	}

	//保存
	if(pMyPointHead != NULL)
	{
		CString	szPathName;
		szPathName	= m_szPathName.Left(m_szPathName.GetLength()-4) + "P.dat";
		
		FILE	*fp;
		fp	= fopen(szPathName,"w");
		if(fp != NULL)
		{
			pMyPoint	= pMyPointHead;
			while(pMyPoint != NULL)
			{
				fprintf(fp,"%lf %lf %lf\n",pMyPoint->m_pPointCoord.x,pMyPoint->m_pPointCoord.y,pMyPoint->m_pPointCoord.z);

				pMyPoint	= pMyPoint->m_pNext;
			}

			fclose(fp);
		}
	}
	
	//删除
	if(pMyPointHead != NULL)
	{
		CDCR_PointFeature	*pf1,*pf2;

		pf1	= pf2	= pMyPointHead;
		do
		{
			pf1		= pf1->m_pNext;
			delete	pf2;
			pf2		= pf1;
		}while(pf1 != NULL);

		pMyPointHead	= NULL;
		pMyPointLast	= NULL;		
	}

	if(iPointSerial != NULL)
	{
		delete[]iPointSerial;
		iPointSerial	= NULL;
	}
	
	return	true;
}
