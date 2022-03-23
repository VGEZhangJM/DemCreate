// DCR_PointPattern_KFunction.cpp: implementation of the CDCR_PointPattern_KFunction class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "vr_demcreator.h"
#include "DCR_PointPattern_KFunction.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDCR_PointPattern_KFunction::CDCR_PointPattern_KFunction()
{
	m_pVretexList	= NULL;
	m_iCount		= 50;
	m_dSize			= 0;
	m_dArea			= 0;

	m_arrD			= NULL;
	m_arrK			= NULL;
}

CDCR_PointPattern_KFunction::~CDCR_PointPattern_KFunction()
{

}

//设置顶点列表
void	CDCR_PointPattern_KFunction::SetGridVertexList(CDCR_GridVertexList *pVretexList)
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

	if(dRight-dLeft > dTop-dBottom)		m_dSize	= dRight-dLeft;
	else								m_dSize	= dTop-dBottom;

	m_dArea	= (dRight-dLeft)*(dTop-dBottom);

	m_iSampleCount	= I;
}

//
void CDCR_PointPattern_KFunction::SetPointFeature(CDCR_PointFeature *pPointHead)
{
	m_pPointHead	= pPointHead;

	//计算面积，根据区域的最大最小值判断
	double	dLeft,dRight,dBottom,dTop;

	CDCR_PointFeature	*pPoint;
	int				I;

	dLeft		= 0;
	dRight		= 0;
	dBottom		= 0;
	dTop		= 0;

	I			= 0;
	pPoint		= m_pPointHead;
	while(pPoint != NULL)
	{
		if(I==0)
		{
			dLeft	= pPoint->m_pPointCoord.x;
			dRight	= pPoint->m_pPointCoord.x;
			dBottom	= pPoint->m_pPointCoord.y;
			dTop	= pPoint->m_pPointCoord.y;
		}
		else
		{
			if(dLeft > pPoint->m_pPointCoord.x)		dLeft	= pPoint->m_pPointCoord.x;
			if(dRight < pPoint->m_pPointCoord.x)	dRight	= pPoint->m_pPointCoord.x;
			if(dBottom > pPoint->m_pPointCoord.y)	dBottom	= pPoint->m_pPointCoord.y;
			if(dTop < pPoint->m_pPointCoord.y)		dTop	= pPoint->m_pPointCoord.y;
		}

		I++;

		pPoint	= pPoint->m_pNext;
	}

	if(dRight-dLeft > dTop-dBottom)		m_dSize	= dRight-dLeft;
	else								m_dSize	= dTop-dBottom;

	m_dArea	= (dRight-dLeft)*(dTop-dBottom);

	m_iSampleCount	= I;
}

//计算Ripley K函数
bool CDCR_PointPattern_KFunction::CalcRipleyK()
{
	if(m_pVretexList == NULL)	return	false;

	if(m_arrD != NULL)	{	delete	m_arrD;		m_arrD	= NULL;	}
	if(m_arrK != NULL)	{	delete	m_arrK;		m_arrK	= NULL;	}

	m_arrD	= new	double[m_iCount];
	m_arrK	= new	double[m_iCount];
	
	double	dDelta;
	int		i;

	dDelta	= m_dSize/m_iCount;

	for(i=0;i<m_iCount;i++)
	{
		m_arrD[i]	= (i+1)*dDelta;
	}
	
	for(i=0;i<m_iCount;i++)
	{
		int		iTempCount;
		iTempCount		= 0;

		double	dTempDistance;
		dTempDistance	= m_arrD[i]*m_arrD[i];

		CDCR_GridVertex	*pGridVertex;
		
		pGridVertex	= m_pVretexList->GetGridVertex();
		while(pGridVertex != NULL)
		{
			//
			CDCR_GridVertex	*pOtherGridVertex;

			pOtherGridVertex	= m_pVretexList->GetGridVertex();
			while(pOtherGridVertex != NULL)
			{
				//不相同
				if(pOtherGridVertex != pGridVertex)
				{
					//
					double	dDistance;
					dDistance	= (pGridVertex->m_dX-pOtherGridVertex->m_dX) * (pGridVertex->m_dX-pOtherGridVertex->m_dX) 
								+ (pGridVertex->m_dY-pOtherGridVertex->m_dY) * (pGridVertex->m_dY-pOtherGridVertex->m_dY);

					if(dDistance < dTempDistance)
					{
						iTempCount++;
					}
				}

				pOtherGridVertex	= pOtherGridVertex->m_pNext;
			}
			
			//
			pGridVertex	= pGridVertex->m_pNext;
		}

		m_arrK[i]	= m_dArea * iTempCount / (m_iSampleCount*m_iSampleCount);
	}
		
	//记录
	{
		CString	szPathName;

		szPathName	= m_szPathName.Left(m_szPathName.GetLength()-4) + "RipleyK.txt"; 

		FILE	*fp;
		fp	= fopen(szPathName,"w");
		if(fp != NULL)
		{
			for(i=0;i<m_iCount;i++)
			{
				double	dDistance;

				dDistance	= m_arrD[i];

				fprintf(fp,"%10.5lf ",dDistance);
			}
			fprintf(fp,"\n");

			for(i=0;i<m_iCount;i++)
			{
				double	dK;

				dK			= m_arrK[i];

				fprintf(fp,"%10.5lf ",dK);
			}
			fprintf(fp,"\n");

			fclose(fp);
		}
		else	return	false;
	}
	
	//
	if(m_arrD != NULL)	{	delete	m_arrD;		m_arrD	= NULL;	}
	if(m_arrK != NULL)	{	delete	m_arrK;		m_arrK	= NULL;	}

	return	true;
}

//计算Ripley K函数
bool CDCR_PointPattern_KFunction::CalcRipleyKWithPoint()
{
	if(m_pPointHead == NULL)	return	false;

	if(m_arrD != NULL)	{	delete	m_arrD;		m_arrD	= NULL;	}
	if(m_arrK != NULL)	{	delete	m_arrK;		m_arrK	= NULL;	}

	m_arrD	= new	double[m_iCount];
	m_arrK	= new	double[m_iCount];
	
	double	dDelta;
	int		i;

	dDelta	= m_dSize/m_iCount;

	for(i=0;i<m_iCount;i++)
	{
		m_arrD[i]	= (i+1)*dDelta;
	}
	
	for(i=0;i<m_iCount;i++)
	{
		int		iTempCount;
		iTempCount		= 0;

		double	dTempDistance;
		dTempDistance	= m_arrD[i]*m_arrD[i];

		CDCR_PointFeature	*pPoint;
		
		pPoint	= m_pPointHead;
		while(pPoint != NULL)
		{
			//
			CDCR_PointFeature	*pOtherPoint;

			pOtherPoint	= m_pPointHead;
			while(pOtherPoint != NULL)
			{
				//不相同
				if(pOtherPoint != pPoint)
				{
					//
					double	dDistance;
					dDistance	= (pPoint->m_pPointCoord.x-pOtherPoint->m_pPointCoord.x) * (pPoint->m_pPointCoord.x-pOtherPoint->m_pPointCoord.x) 
								+ (pPoint->m_pPointCoord.y-pOtherPoint->m_pPointCoord.y) * (pPoint->m_pPointCoord.y-pOtherPoint->m_pPointCoord.y);

					if(dDistance < dTempDistance)
					{
						iTempCount++;
					}
				}

				pOtherPoint	= pOtherPoint->m_pNext;
			}
			
			//
			pPoint	= pPoint->m_pNext;
		}

		m_arrK[i]	= m_dArea * iTempCount / (m_iSampleCount*m_iSampleCount);
	}
		
	//记录
	{
		CString	szPathName;

		szPathName	= m_szPathName.Left(m_szPathName.GetLength()-4) + "RipleyK.txt"; 

		FILE	*fp;
		fp	= fopen(szPathName,"w");
		if(fp != NULL)
		{
			for(i=0;i<m_iCount;i++)
			{
				double	dDistance;

				dDistance	= m_arrD[i];

				fprintf(fp,"%10.5lf ",dDistance);
			}
			fprintf(fp,"\n");

			for(i=0;i<m_iCount;i++)
			{
				double	dK;

				dK			= sqrt(m_arrK[i]/PI) - m_arrD[i];

				fprintf(fp,"%10.5lf ",dK);
			}
			fprintf(fp,"\n");

			fclose(fp);
		}
		else	
		{
			if(m_arrD != NULL)	{	delete	m_arrD;		m_arrD	= NULL;	}
			if(m_arrK != NULL)	{	delete	m_arrK;		m_arrK	= NULL;	}
			
			return	false;
		}
	}
	
	//
	if(m_arrD != NULL)	{	delete	m_arrD;		m_arrD	= NULL;	}
	if(m_arrK != NULL)	{	delete	m_arrK;		m_arrK	= NULL;	}

	return	true;
}

//
void CDCR_PointPattern_KFunction::SetPathName(CString	szPathName)
{
	m_szPathName	= szPathName;
}