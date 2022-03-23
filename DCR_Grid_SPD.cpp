// DCR_Grid_SPD.cpp: implementation of the CDCR_Grid_SPD class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "vr_demcreator.h"
#include "DCR_Grid_SPD.h"

//#include "libLeastSquare.h"
#include "DCR_LeastSquare.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDCR_Grid_SPD::CDCR_Grid_SPD()
{
	//权指数
	m_iU			= 2;
	//光滑参数
	m_dT			= 0;

	//
	m_bCustom		= false;

	m_dRadius		= 10000;

	//采样点点数
	m_pVretexList	= NULL;

	//
	m_szPathName	= "D:\\Out(SPD).grd";

	//初始化
	//libLeastSquareInitialize();
}

CDCR_Grid_SPD::~CDCR_Grid_SPD()
{
	//停止
	//libLeastSquareTerminate();
}

//设置顶点列表
void CDCR_Grid_SPD::SetGridVertexList(CDCR_GridVertexList *pVretexList)
{
	m_pVretexList	= pVretexList;
}

//计算插值的高程值
bool CDCR_Grid_SPD::CalcHeight(double dX,double dY,double& dZ)
{
	if(m_pVretexList == NULL)	return	false;
	
	CDCR_GridVertex	*pVertex;
	
	//如果都大，则通过加权计算
	//2011-06-30：比较d和R，如果d》R，则认为权重为0，这是合理的，应用了改进谢别德的局部性。
	//所以，所有的R值都是指定的，
	double	dUpValue	= 0;
	double	dDownValue	= 0;

	double	dUpWeighted		= 0;
	double	dDownWeighted	= 0;

	bool	bNullUsed		= true;

	pVertex	= m_pVretexList->GetGridVertex();
	while(pVertex != NULL)
	{	
		//假如d>R，则删除不继续
		if(pVertex->m_dDistance > m_dRadius)
		{
			pVertex	= pVertex->m_pNext;
			continue;
		}

		bNullUsed	= false;

		//假如是靠近点，则返回，直接使用该点
		if(pVertex->m_dDistance < 1e-10)
		{
			dZ		= pVertex->m_dZ;

			return	true;
		}
		
		//距离
		double	dDistance;
		dDistance	= sqrt(pVertex->m_dDistance*pVertex->m_dDistance + m_dT*m_dT);

		double	dWeighted;
		//计算（(1/d-1/R)^u）
		dWeighted	= pow((1.0/dDistance - 1.0/m_dRadius),(double)m_iU);

		//计算分子
		dUpValue	= dUpValue + dWeighted * pVertex->m_dZ;	

		//计算分母
		dDownValue	= dDownValue + dWeighted;	
		
		pVertex	= pVertex->m_pNext;
	}
	
	if(bNullUsed)
		dZ	= -9999;
	else
		//计算具体的值
		dZ	= dUpValue / dDownValue;
	
	return	true;
}

//
bool CDCR_Grid_SPD::CalcHeightEx(double dX,double dY,double& dZ)
{
	/*
	if(m_pVretexList == NULL)	return	false;
	
	int				i;
	int				iPointNum;
	double			*dSX,*dSY,*dSZ;
	
	CDCR_GridVertex	*pVertex;
	
	iPointNum	= 0;
	pVertex		= m_pVretexList->GetGridVertex();
	while(pVertex != NULL)
	{	
		iPointNum++;
		pVertex	= pVertex->m_pNext;
	}
	
	dSX	= new	double[iPointNum];
	dSY	= new	double[iPointNum];
	dSZ	= new	double[iPointNum];
		
	i			= 0;
	pVertex		= m_pVretexList->GetGridVertex();
	while(pVertex != NULL)
	{	
		dSX[i]	= pVertex->m_dX;
		dSY[i]	= pVertex->m_dY;
		dSZ[i]	= pVertex->m_dZ;
		
		i++;
		pVertex	= pVertex->m_pNext;
	}
	
	//计算二次曲面
	CDCR_LeastSquare	pLeastSquare;

	//加入计算
	bool	bAlready;
	bAlready	= pLeastSquare.CalcMatrix(iPointNum,dSX,dSY,dSZ);

	//使用二次多项式的值代替原始值
	if(bAlready)
	{
		pVertex		= m_pVretexList->GetGridVertex();
		while(pVertex != NULL)
		{	
			double	dTempZ;
			pLeastSquare.GetZCoord(pVertex->m_dX,pVertex->m_dY,dTempZ);

			pVertex->m_dZ	= dTempZ;
			
			pVertex	= pVertex->m_pNext;
		}
	}
	else
	{
		delete	[]dSX;	dSX	= NULL;
		delete	[]dSY;	dSY	= NULL;
		delete	[]dSZ;	dSZ	= NULL;

		dZ	= -9999;
		
		return	false;
	}

	delete	[]dSX;	dSX	= NULL;
	delete	[]dSY;	dSY	= NULL;
	delete	[]dSZ;	dSZ	= NULL;

	//计算dZ
	//{
		//如果都大，则通过加权计算
		double	dUpValue	= 0;
		double	dDownValue	= 0;

		double	dWeighted;

		pVertex	= m_pVretexList->GetGridVertex();
		while(pVertex != NULL)
		{	
			//计算d*=d^2+t^2;
			dWeighted	= sqrt(pVertex->m_dDistance*pVertex->m_dDistance + m_dT*m_dT);
			//d*^u
			dWeighted	= pow(dWeighted,(double)m_iU);

			//
			if(dWeighted < 1e-10)	
			{
				dZ		= pVertex->m_dZ;

				return	true;
			}

			//计算（1/(d*^u)）
			dWeighted	= 1.0/dWeighted;

			//计算分子
			dUpValue	= dUpValue + dWeighted * pVertex->m_dZ;	

			//计算分母
			dDownValue	= dDownValue + dWeighted;	
			
			pVertex	= pVertex->m_pNext;
		}	
		
		//if(dDownValue < 1e-10)
		//	dZ	= -9999;
		//else
			//比值计算得到Z值
			dZ	= dUpValue / dDownValue;
		//}

	return	true;	
	*/

	if(m_pVretexList == NULL)	return	false;

	int				i;
	int				iCount;
	
	CDCR_GridVertex	*pVertex;	
	double			*dSX;
	double			*dSY;
	double			*dSZ;
	
	//计算采样点数
	//{
		iCount		= 0;
		pVertex		= m_pVretexList->GetGridVertex();
		while(pVertex != NULL)
		{	
			iCount++;
			pVertex	= pVertex->m_pNext;
		}

		dSX	= new	double[iCount];
		dSY	= new	double[iCount];
		dSZ	= new	double[iCount];
		
		//计算平均值
		double	dSumX,dSumY;
		dSumX	= 0;
		dSumY	= 0;

		i			= 0;
		pVertex		= m_pVretexList->GetGridVertex();
		while(pVertex != NULL)
		{	
			dSumX	= dSumX + pVertex->m_dX;
			dSumY	= dSumY + pVertex->m_dY;

			i++;
			pVertex	= pVertex->m_pNext;
		}
		dSumX	= dSumX/iCount;
		dSumY	= dSumY/iCount;

		i			= 0;
		pVertex		= m_pVretexList->GetGridVertex();
		while(pVertex != NULL)
		{	
			dSX[i]	= pVertex->m_dX - dSumX;
			dSY[i]	= pVertex->m_dY - dSumY;
			dSZ[i]	= pVertex->m_dZ;

			i++;
			pVertex	= pVertex->m_pNext;
		}

	//}
	
	int	iCoffCount	= 9;

	if(iCount >=iCoffCount)
	{
		//将采样点数放到数组中
		//{
			double	*mA;
			mA	= new	double[iCount*iCoffCount];
			double	*mZ;
			mZ	= new	double[iCount];

			for(i=0;i<iCount;i++)
			{
				mA[i*iCoffCount+0]	= 1;
				mA[i*iCoffCount+1]	= dSX[i];
				mA[i*iCoffCount+2]	= dSY[i];
				mA[i*iCoffCount+3]	= dSX[i] * dSY[i];
				mA[i*iCoffCount+4]	= dSX[i] * dSX[i];
				mA[i*iCoffCount+5]	= dSY[i] * dSY[i];

				mA[i*iCoffCount+6]	= dSX[i] * dSX[i] * dSY[i];
				mA[i*iCoffCount+7]	= dSY[i] * dSY[i] * dSX[i];
				mA[i*iCoffCount+8]	= dSX[i] * dSX[i] * dSY[i] * dSY[i];

				mZ[i]		= dSZ[i];
			}
		//}
		
		//使用二次多项式：C0+C1*x+C2*y+C3*x*y+C4*x^2+C5*y^2+C6*x*y^2+C7*y*x^2+C8*x^2*y^2;
		//通过m文件计算
		{
			Mm	maxtixOut,maxtixInA,maxtixInZ;

			maxtixInA	= zeros(iCount,iCoffCount);
			maxtixInZ	= zeros(iCount,1);
		
			int	k=0;
			for(int j=0;j<iCoffCount;j++)
			{
				for(i=0;i<iCount;i++)
				{
					maxtixInA.r(1+k)			= mA[i*iCoffCount+j];

					k++;
				}
			}

			for(i=0;i<iCount;i++)
			{
				maxtixInZ.r(1+i)				= mZ[i];
			}
			
			//计算
			Mm maxtixResult;
			maxtixResult	= CalcOLSquation(maxtixInA,maxtixInZ);

			//检验计算得到的每一个值
			m_type resultx;
			for(i=0;i<iCoffCount;i++)
			{
				resultx	= maxtixResult.r(1+i);
				if(mexIsInf(resultx) || mexIsNaN(resultx)) 
				{
					if(mA != NULL)	delete	[]mA;
					if(mZ != NULL)	delete	[]mZ;

					return	false;
				}
			}
			
			//
			for(i=0;i<iCount;i++)
			{
				double	dValue;

				dValue	= maxtixResult.r(1) + 
					maxtixResult.r(2) * dSX[i] + 
					maxtixResult.r(3) * dSY[i] + 
					maxtixResult.r(4) * dSX[i] * dSY[i] +
					maxtixResult.r(5) * dSX[i] * dSX[i] +
					maxtixResult.r(6) * dSY[i] * dSY[i] + 
					maxtixResult.r(7) * dSX[i] * dSX[i] * dSY[i] + 
					maxtixResult.r(8) * dSY[i] * dSY[i] * dSX[i] + 
					maxtixResult.r(9) * dSX[i] * dSX[i] * dSY[i] * dSY[i];

				//
				//CString	szInfo;
				//szInfo.Format("%lf %lf \n",dSZ[i], dValue);
				//TRACE(szInfo);

				dSZ[i]	= dValue;
			}
		}

		if(mA != NULL)	delete	[]mA;
		if(mZ != NULL)	delete	[]mZ;
	}


	//计算dZ,如果多项式无解，用原有的点，反之使用多项式点
	//{
		//如果都大，则通过加权计算
		double	dUpValue	= 0;
		double	dDownValue	= 0;

		double	dWeighted;
		
		i		= 0;
		pVertex	= m_pVretexList->GetGridVertex();
		while(pVertex != NULL)
		{	
			//计算d*=d^2+t^2;
			dWeighted	= sqrt(pVertex->m_dDistance*pVertex->m_dDistance + m_dT*m_dT);
			//d*^u
			dWeighted	= pow(dWeighted,(double)m_iU);

			//
			if(dWeighted < 1e-10)	
			{
				//dZ		= pVertex->m_dZ;
				dZ		= dSZ[i];

				if(dSX != NULL)	delete[]dSX;
				if(dSY != NULL)	delete[]dSY;
				if(dSZ != NULL)	delete[]dSZ;

				return	true;
			}

			//计算（1/(d*^u)）
			dWeighted	= 1.0/dWeighted;

			//计算分子
			//dUpValue	= dUpValue + dWeighted * pVertex->m_dZ;	
			dUpValue	= dUpValue + dWeighted * dSZ[i];	

			//计算分母
			dDownValue	= dDownValue + dWeighted;	
			
			i++;

			pVertex	= pVertex->m_pNext;
		}	
		
		//if(dDownValue < 1e-10)
		//	dZ	= -9999;
		//else
			//比值计算得到Z值
		dZ	= dUpValue / dDownValue;


		if(dSX != NULL)	delete[]dSX;
		if(dSY != NULL)	delete[]dSY;
		if(dSZ != NULL)	delete[]dSZ;

		return	true;
	//}
}

//
void CDCR_Grid_SPD::SetU(int iU)
{
	m_iU	= iU;
}

int CDCR_Grid_SPD::GetU()
{
	return	m_iU;
}

//
void CDCR_Grid_SPD::SetT(double dT)
{
	m_dT	= dT;
}

double CDCR_Grid_SPD::GetT()
{
	return	m_dT;
}

//
void CDCR_Grid_SPD::SetPathName(CString szPathName)
{
	m_szPathName	= szPathName;
}

CString CDCR_Grid_SPD::GetPathName()
{
	return	m_szPathName;
}

//
void CDCR_Grid_SPD::SetCustom(bool bCustom)
{
	m_bCustom	= bCustom;
}

bool CDCR_Grid_SPD::GetCustom()
{
	return	m_bCustom;
}

//
void CDCR_Grid_SPD::SetRadius(double dRadius)
{
	m_dRadius	= dRadius;
}

double CDCR_Grid_SPD::GetRadius()
{
	return	m_dRadius;
}

