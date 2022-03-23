// DCR_Grid_RBF.cpp: implementation of the CDCR_Grid_RBF class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "vr_demcreator.h"
#include "DCR_Grid_RBF.h"

#include "matlib.h"
#include "math.h"
#include "CalcRBFEquation.h"

//#include "libCalcRBFEquation.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDCR_Grid_RBF::CDCR_Grid_RBF()
{
	//采样点点数
	m_pVretexList	= NULL;
	//
	m_iModelType	= 0;

	//参数
	m_dC			= 0;

	m_szPathName	= "D:\\Out(RBF).grd";

	//初始化
	//libCalcRBFEquationInitialize();
}

CDCR_Grid_RBF::~CDCR_Grid_RBF()
{
	//停止
	//libCalcRBFEquationTerminate();
}

//////////////////////////////////////////////////////////////////////
// 设置顶点列表
//////////////////////////////////////////////////////////////////////
void CDCR_Grid_RBF::SetGridVertexList(CDCR_GridVertexList *pVretexList)
{
	m_pVretexList	= pVretexList;
}

//////////////////////////////////////////////////////////////////////
// 计算距离
//////////////////////////////////////////////////////////////////////
double CDCR_Grid_RBF::GetDistance(double x1, double y1, double z1, double x2, double y2, double z2)
{
	double	dDistance;

	//dDistance	= sqrt( (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) + (z1-z2)*(z1-z2) );
	dDistance	= (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2);

	return	dDistance;
}

//////////////////////////////////////////////////////////////////////
// 计算插值的高程值
//////////////////////////////////////////////////////////////////////
bool CDCR_Grid_RBF::CalcHeight(double dX,double dY,double& dZ)
{
	if(m_pVretexList == NULL)	return	false;
	
	//多重曲面函数
	if(m_iModelType == 0)
	{
		return	RBFMQF(dX,dY,dZ);
	}

	//倒数多重曲面函数
	if(m_iModelType == 1)
	{
		return	RBFIMQF(dX,dY,dZ);
	}

	//薄板样条
	if(m_iModelType == 2)
	{
		return	RBFTPSF(dX,dY,dZ);
	}

	//多重对数
	if(m_iModelType == 3)
	{
		return	RBFMLF(dX,dY,dZ);
	}
	
	//自然三次样条函数
	if(m_iModelType == 4)
	{
		return	RBNCSF(dX,dY,dZ);
	}

	return	false;
}

//////////////////////////////////////////////////////////////////////
// RBFMQF计算插值的高程值
//////////////////////////////////////////////////////////////////////
bool CDCR_Grid_RBF::RBFMQF(double dX, double dY, double &dZ)
{
	int		iCount;
	iCount	= m_pVretexList->GetGridVertexCount();
	
	//参数矩阵iCount行iCount列
	double	*dParaMatrix; 
	dParaMatrix		= new	double[iCount*iCount];

	//iCount行1列
	double	*dTempMatrix;
	dTempMatrix		= new	double[iCount];
	
	//1行iCount列
	double	*dTargetMatrix;
	dTargetMatrix	= new	double[iCount];

	//
	CDCR_GridVertex	*pGridVertex;
	CDCR_GridVertex	*pNextGridVertex;
	
	//填充数组
	int		I	= 0;
	int		J	= 0;
	pGridVertex	= m_pVretexList->GetGridVertex();
	while(pGridVertex != NULL)
	{
		pNextGridVertex	= m_pVretexList->GetGridVertex();
		
		while(pNextGridVertex != NULL)
		{
			//
			if(pGridVertex == pNextGridVertex)
				dParaMatrix[I]	= 0;
			else
			{
				//sqrt(d^2+C^2)
				dParaMatrix[I]	= sqrt(GetDistance(pGridVertex->m_dX,pGridVertex->m_dY,0,pNextGridVertex->m_dX,pNextGridVertex->m_dY,0) + m_dC*m_dC);
			}
			
			I++;
			pNextGridVertex	= pNextGridVertex->m_pNext;
		}
		
		//
		dTempMatrix[J]	= pGridVertex->m_dZ;
		//
		dTargetMatrix[J]= sqrt(GetDistance(pGridVertex->m_dX,pGridVertex->m_dY,0,dX,dY,0) + m_dC*m_dC);

		J++;

		pGridVertex	= pGridVertex->m_pNext;
	}
	
	bool	bResult;
	bResult	= false;
	//调用MatCom的方式实现
	{
		Mm	maxtixPara,maxtixTemp,maxtixTarget;
		int	i;

		maxtixPara	= zeros(iCount,iCount);
		maxtixTemp	= zeros(iCount,1);
		maxtixTarget= zeros(1,iCount);

		for(i=0;i<iCount*iCount;i++)
		{	
			maxtixPara.r(1+i)	= dParaMatrix[i];
		}
		
		for(i=0;i<iCount;i++)
		{	
			maxtixTemp.r(1+i)	= dTempMatrix[i];
			maxtixTarget.r(1+i)	= dTargetMatrix[i];
		}
		
		//
		Mm maxtixResult;
		maxtixResult	= CalcRBFEquation(maxtixPara,maxtixTemp,maxtixTarget);
		
		m_type resultx;
		
		resultx	= maxtixResult.r(1);
		if(mexIsInf(resultx) || mexIsNaN(resultx)) 
		{
			dZ		= -9999;
			bResult	= false;
		}
		else
		{
			dZ		= maxtixResult.r(1);
			bResult	= true;
		}
	}

	/*
	//调用m文件
	{
		int		i;
		mxArray	*pArrayOut	= NULL;
		mxArray	*pArrayInA	= NULL;
		mxArray	*pArrayInb	= NULL;
		mxArray	*pArrayInL	= NULL;

		pArrayInA	= mxCreateDoubleMatrix(iCount,iCount,mxREAL);

		for(i=0;i<iCount*iCount;i++)
		{	
			*(mxGetPr(pArrayInA) + i)	= dParaMatrix[i];
		}

		pArrayInb	= mxCreateDoubleMatrix(iCount,1,mxREAL);
		pArrayInL	= mxCreateDoubleMatrix(1,iCount,mxREAL);

		for(i=0;i<iCount;i++)
		{	
			*(mxGetPr(pArrayInb) + i)	= dTempMatrix[i];
			*(mxGetPr(pArrayInL) + i)	= dTargetMatrix[i];
		}
		
		//
		mlfLibCalcRBFEquation(1, &pArrayOut, pArrayInA, pArrayInb, pArrayInL);
		
		if(mxIsInf(*(mxGetPr(pArrayOut))) || mxIsNaN(*(mxGetPr(pArrayOut)))) 
		{
			dZ	= -9999;
		}
		else
		{	
			dZ	= *(mxGetPr(pArrayOut));
		}

		//释放
		if(pArrayInA)	mxDestroyArray(pArrayInA);
		if(pArrayInb)	mxDestroyArray(pArrayInb);
		if(pArrayInL)	mxDestroyArray(pArrayInL);
		if(pArrayOut)	mxDestroyArray(pArrayOut);
	}
	*/
	
	delete	[]dParaMatrix;
	delete	[]dTempMatrix;
	delete	[]dTargetMatrix;

	return	bResult;
}

//////////////////////////////////////////////////////////////////////
// RBFIMQF计算插值的高程值
//////////////////////////////////////////////////////////////////////
bool CDCR_Grid_RBF::RBFIMQF(double dX, double dY, double &dZ)
{
	int		iCount;
	iCount	= m_pVretexList->GetGridVertexCount();
	
	//参数矩阵iCount行iCount列
	double	*dParaMatrix; 
	dParaMatrix		= new	double[iCount*iCount];

	//iCount行1列
	double	*dTempMatrix;
	dTempMatrix		= new	double[iCount];
	
	//1行iCount列
	double	*dTargetMatrix;
	dTargetMatrix	= new	double[iCount];

	//
	CDCR_GridVertex	*pGridVertex;
	CDCR_GridVertex	*pNextGridVertex;

	bool			bRePoint;
	double			dRePointZ;

	bRePoint	= false;
	dRePointZ	= 0;
	
	//填充数组
	int		I	= 0;
	int		J	= 0;
	pGridVertex	= m_pVretexList->GetGridVertex();
	while(pGridVertex != NULL)
	{
		pNextGridVertex	= m_pVretexList->GetGridVertex();
		
		while(pNextGridVertex != NULL)
		{
			//
			if(pGridVertex == pNextGridVertex)
				dParaMatrix[I]	= 0;
			else
			{
				//1/sqrt(d^2+C^2)
				//可以保证是非零的
				dParaMatrix[I]	= sqrt(GetDistance(pGridVertex->m_dX,pGridVertex->m_dY,0,pNextGridVertex->m_dX,pNextGridVertex->m_dY,0) + m_dC*m_dC);
				
				if(dParaMatrix[I] < 1e-10)
					dParaMatrix[I]	= 0;
				else
					dParaMatrix[I]	= 1 / dParaMatrix[I];
			}
			
			I++;
			pNextGridVertex	= pNextGridVertex->m_pNext;
		}
		
		//
		dTempMatrix[J]	= pGridVertex->m_dZ;
		//
		dTargetMatrix[J]= sqrt(GetDistance(pGridVertex->m_dX,pGridVertex->m_dY,0,dX,dY,0) + m_dC*m_dC);

		//这里可能存在重点
		if(dTargetMatrix[J] < 1e-10)
		{
			dTargetMatrix[J]	= 0;

			bRePoint			= true;
			dRePointZ			= pGridVertex->m_dZ;
		}
		else
			dTargetMatrix[J]	= 1.0 / dTargetMatrix[J];

		J++;

		pGridVertex	= pGridVertex->m_pNext;
	}

	bool	bResult;
	bResult	= false;
	
	if(!bRePoint)
	{
		{
			//调用MatCom
			Mm	maxtixPara,maxtixTemp,maxtixTarget;
			int	i;

			maxtixPara	= zeros(iCount,iCount);
			maxtixTemp	= zeros(iCount,1);
			maxtixTarget= zeros(1,iCount);

			for(i=0;i<iCount*iCount;i++)
			{	
				maxtixPara.r(1+i)	= dParaMatrix[i];
			}
			
			for(i=0;i<iCount;i++)
			{	
				maxtixTemp.r(1+i)	= dTempMatrix[i];
				maxtixTarget.r(1+i)	= dTargetMatrix[i];
			}
			
			//
			Mm maxtixResult;
			maxtixResult	= CalcRBFEquation(maxtixPara,maxtixTemp,maxtixTarget);
			
			m_type resultx;
			
			resultx	= maxtixResult.r(1);
			if(mexIsInf(resultx) || mexIsNaN(resultx)) 
			{
				dZ		= -9999;
				bResult	= false;
			}
			else
			{
				dZ		= maxtixResult.r(1);
				bResult	= true;
			}
		}
		
		/*
		//调用m文件
		{
			int		i;
			mxArray	*pArrayOut	= NULL;
			mxArray	*pArrayInA	= NULL;
			mxArray	*pArrayInb	= NULL;
			mxArray	*pArrayInL	= NULL;

			pArrayInA	= mxCreateDoubleMatrix(iCount,iCount,mxREAL);

			for(i=0;i<iCount*iCount;i++)
			{	
				*(mxGetPr(pArrayInA) + i)	= dParaMatrix[i];
			}

			pArrayInb	= mxCreateDoubleMatrix(iCount,1,mxREAL);
			pArrayInL	= mxCreateDoubleMatrix(1,iCount,mxREAL);

			for(i=0;i<iCount;i++)
			{	
				*(mxGetPr(pArrayInb) + i)	= dTempMatrix[i];
				*(mxGetPr(pArrayInL) + i)	= dTargetMatrix[i];
			}
			
			//
			mlfLibCalcRBFEquation(1, &pArrayOut, pArrayInA, pArrayInb, pArrayInL);
			
			//
			if(mxIsInf(*(mxGetPr(pArrayOut))) || mxIsNaN(*(mxGetPr(pArrayOut)))) 
			{
				dZ	= -9999;
			}
			else
			{	
				dZ	= *(mxGetPr(pArrayOut));
			}

			//释放
			if(pArrayInA)	mxDestroyArray(pArrayInA);
			if(pArrayInb)	mxDestroyArray(pArrayInb);
			if(pArrayInL)	mxDestroyArray(pArrayInL);
			if(pArrayOut)	mxDestroyArray(pArrayOut);
		}
		*/
	}
	else
	{
		dZ		= dRePointZ;
		bResult	= true;
	}

	delete	[]dParaMatrix;
	delete	[]dTempMatrix;
	delete	[]dTargetMatrix;

	return	bResult;
}

//////////////////////////////////////////////////////////////////////
// RBFIMQF计算插值的高程值
//////////////////////////////////////////////////////////////////////
bool CDCR_Grid_RBF::RBFTPSF(double dX, double dY, double &dZ)
{
	int		iCount;
	iCount	= m_pVretexList->GetGridVertexCount();
	
	//参数矩阵iCount行iCount列
	double	*dParaMatrix; 
	dParaMatrix		= new	double[iCount*iCount];

	//iCount行1列
	double	*dTempMatrix;
	dTempMatrix		= new	double[iCount];
	
	//1行iCount列
	double	*dTargetMatrix;
	dTargetMatrix	= new	double[iCount];

	//
	CDCR_GridVertex	*pGridVertex;
	CDCR_GridVertex	*pNextGridVertex;
	double			dTemp;

	//填充数组
	int		I	= 0;
	int		J	= 0;
	pGridVertex	= m_pVretexList->GetGridVertex();
	while(pGridVertex != NULL)
	{
		pNextGridVertex	= m_pVretexList->GetGridVertex();
		
		while(pNextGridVertex != NULL)
		{
			//
			if(pGridVertex == pNextGridVertex)
				dParaMatrix[I]	= 0;
			else
			{
				//(d^2+C^2)ln(d^2+C^2)
				//可以保证是非零的
				dTemp			= GetDistance(pGridVertex->m_dX,pGridVertex->m_dY,0,pNextGridVertex->m_dX,pNextGridVertex->m_dY,0) + m_dC*m_dC;
				
				dParaMatrix[I]	= dTemp * log(dTemp);
			}
			
			I++;
			pNextGridVertex	= pNextGridVertex->m_pNext;
		}
		
		//
		dTempMatrix[J]	= pGridVertex->m_dZ;
		//
		dTemp			= GetDistance(pGridVertex->m_dX,pGridVertex->m_dY,0,dX,dY,0) + m_dC*m_dC;

		//这里可能存在重点
		dTargetMatrix[J]= dTemp * log(dTemp);

		J++;

		pGridVertex	= pGridVertex->m_pNext;
	}
	
	//调用MathLib解方程
	bool	bResult;
	bResult	= false;
	{
		//用图表显示	
		Mm	maxtixPara,maxtixTemp,maxtixTarget;
		int	i;

		maxtixPara	= zeros(iCount,iCount);
		maxtixTemp	= zeros(iCount,1);
		maxtixTarget= zeros(1,iCount);

		for(i=0;i<iCount*iCount;i++)
		{	
			maxtixPara.r(1+i)	= dParaMatrix[i];
		}
		
		for(i=0;i<iCount;i++)
		{	
			maxtixTemp.r(1+i)	= dTempMatrix[i];
			maxtixTarget.r(1+i)	= dTargetMatrix[i];
		}
		
		//
		Mm maxtixResult;
		maxtixResult	= CalcRBFEquation(maxtixPara,maxtixTemp,maxtixTarget);
		
		m_type resultx;
		
		resultx	= maxtixResult.r(1);
		if(mexIsInf(resultx) || mexIsNaN(resultx)) 
		{
			dZ				= -9999;
			bResult			= false;
		}
		else
		{
			dZ				= maxtixResult.r(1);
			bResult			= true;
		}
	}

	/*
	//调用m文件
	{
		int		i;
		mxArray	*pArrayOut	= NULL;
		mxArray	*pArrayInA	= NULL;
		mxArray	*pArrayInb	= NULL;
		mxArray	*pArrayInL	= NULL;

		pArrayInA	= mxCreateDoubleMatrix(iCount,iCount,mxREAL);

		for(i=0;i<iCount*iCount;i++)
		{	
			*(mxGetPr(pArrayInA) + i)	= dParaMatrix[i];
		}

		pArrayInb	= mxCreateDoubleMatrix(iCount,1,mxREAL);
		pArrayInL	= mxCreateDoubleMatrix(1,iCount,mxREAL);

		for(i=0;i<iCount;i++)
		{	
			*(mxGetPr(pArrayInb) + i)	= dTempMatrix[i];
			*(mxGetPr(pArrayInL) + i)	= dTargetMatrix[i];
		}
		
		//
		mlfLibCalcRBFEquation(1, &pArrayOut, pArrayInA, pArrayInb, pArrayInL);
		
		//
		if(mxIsInf(*(mxGetPr(pArrayOut))) || mxIsNaN(*(mxGetPr(pArrayOut)))) 
		{
			dZ	= -9999;
		}
		else
		{	
			dZ	= *(mxGetPr(pArrayOut));
		}

		//释放
		if(pArrayInA)	mxDestroyArray(pArrayInA);
		if(pArrayInb)	mxDestroyArray(pArrayInb);
		if(pArrayInL)	mxDestroyArray(pArrayInL);
		if(pArrayOut)	mxDestroyArray(pArrayOut);
	}
	*/

	delete	[]dParaMatrix;
	delete	[]dTempMatrix;
	delete	[]dTargetMatrix;

	return	bResult;
}

//////////////////////////////////////////////////////////////////////
// RBFMLF计算插值的高程值
//////////////////////////////////////////////////////////////////////
bool CDCR_Grid_RBF::RBFMLF(double dX, double dY, double &dZ)
{
	int		iCount;
	iCount	= m_pVretexList->GetGridVertexCount();
	
	//参数矩阵iCount行iCount列
	double	*dParaMatrix; 
	dParaMatrix		= new	double[iCount*iCount];

	//iCount行1列
	double	*dTempMatrix;
	dTempMatrix		= new	double[iCount];
	
	//1行iCount列
	double	*dTargetMatrix;
	dTargetMatrix	= new	double[iCount];

	//
	CDCR_GridVertex	*pGridVertex;
	CDCR_GridVertex	*pNextGridVertex;
	double			dTemp;

	//填充数组
	int		I	= 0;
	int		J	= 0;
	pGridVertex	= m_pVretexList->GetGridVertex();
	while(pGridVertex != NULL)
	{
		pNextGridVertex	= m_pVretexList->GetGridVertex();
		
		while(pNextGridVertex != NULL)
		{
			//
			if(pGridVertex == pNextGridVertex)
				dParaMatrix[I]	= 0;
			else
			{
				//ln(d^2+C^2)
				//可以保证是非零的
				dTemp			= GetDistance(pGridVertex->m_dX,pGridVertex->m_dY,0,pNextGridVertex->m_dX,pNextGridVertex->m_dY,0) + m_dC*m_dC;
				
				dParaMatrix[I]	= log(dTemp);
			}
			
			I++;
			pNextGridVertex	= pNextGridVertex->m_pNext;
		}
		
		//
		dTempMatrix[J]	= pGridVertex->m_dZ;
		//
		dTemp			= GetDistance(pGridVertex->m_dX,pGridVertex->m_dY,0,dX,dY,0) + m_dC*m_dC;

		//这里可能存在重点
		dTargetMatrix[J]= log(dTemp);

		J++;

		pGridVertex	= pGridVertex->m_pNext;
	}
	
	bool	bResult;
	bResult	= false;
	//调用MatCom解方程
	{
		Mm	maxtixPara,maxtixTemp,maxtixTarget;
		int	i;

		maxtixPara	= zeros(iCount,iCount);
		maxtixTemp	= zeros(iCount,1);
		maxtixTarget= zeros(1,iCount);

		for(i=0;i<iCount*iCount;i++)
		{	
			maxtixPara.r(1+i)	= dParaMatrix[i];
		}
		
		for(i=0;i<iCount;i++)
		{	
			maxtixTemp.r(1+i)	= dTempMatrix[i];
			maxtixTarget.r(1+i)	= dTargetMatrix[i];
		}
		
		//
		Mm maxtixResult;
		maxtixResult	= CalcRBFEquation(maxtixPara,maxtixTemp,maxtixTarget);
		
		m_type resultx;
		
		resultx	= maxtixResult.r(1);
		if(mexIsInf(resultx) || mexIsNaN(resultx)) 
		{
			dZ		= -9999;
			bResult	= false;
		}
		else
		{
			dZ		= maxtixResult.r(1);
			bResult	= true;
		}
	}

	/*
	//调用m文件
	{
		int		i;
		mxArray	*pArrayOut	= NULL;
		mxArray	*pArrayInA	= NULL;
		mxArray	*pArrayInb	= NULL;
		mxArray	*pArrayInL	= NULL;

		pArrayInA	= mxCreateDoubleMatrix(iCount,iCount,mxREAL);

		for(i=0;i<iCount*iCount;i++)
		{	
			*(mxGetPr(pArrayInA) + i)	= dParaMatrix[i];
		}

		pArrayInb	= mxCreateDoubleMatrix(iCount,1,mxREAL);
		pArrayInL	= mxCreateDoubleMatrix(1,iCount,mxREAL);

		for(i=0;i<iCount;i++)
		{	
			*(mxGetPr(pArrayInb) + i)	= dTempMatrix[i];
			*(mxGetPr(pArrayInL) + i)	= dTargetMatrix[i];
		}
		
		//
		mlfLibCalcRBFEquation(1, &pArrayOut, pArrayInA, pArrayInb, pArrayInL);
		
		//
		if(mxIsInf(*(mxGetPr(pArrayOut))) || mxIsNaN(*(mxGetPr(pArrayOut)))) 
		{
			dZ	= -9999;
		}
		else
		{	
			dZ	= *(mxGetPr(pArrayOut));
		}

		//释放
		if(pArrayInA)	mxDestroyArray(pArrayInA);
		if(pArrayInb)	mxDestroyArray(pArrayInb);
		if(pArrayInL)	mxDestroyArray(pArrayInL);
		if(pArrayOut)	mxDestroyArray(pArrayOut);
	}
	*/

	delete	[]dParaMatrix;
	delete	[]dTempMatrix;
	delete	[]dTargetMatrix;

	return	bResult;
}

//////////////////////////////////////////////////////////////////////
// RBNCSF计算插值的高程值
//////////////////////////////////////////////////////////////////////
bool CDCR_Grid_RBF::RBNCSF(double dX, double dY, double &dZ)
{
	int		iCount;
	iCount	= m_pVretexList->GetGridVertexCount();
	
	//参数矩阵iCount行iCount列
	double	*dParaMatrix; 
	dParaMatrix		= new	double[iCount*iCount];

	//iCount行1列
	double	*dTempMatrix;
	dTempMatrix		= new	double[iCount];
	
	//1行iCount列
	double	*dTargetMatrix;
	dTargetMatrix	= new	double[iCount];

	//
	CDCR_GridVertex	*pGridVertex;
	CDCR_GridVertex	*pNextGridVertex;
	double			dTemp;

	//填充数组
	int		I	= 0;
	int		J	= 0;
	pGridVertex	= m_pVretexList->GetGridVertex();
	while(pGridVertex != NULL)
	{
		pNextGridVertex	= m_pVretexList->GetGridVertex();
		
		while(pNextGridVertex != NULL)
		{
			//
			if(pGridVertex == pNextGridVertex)
				dParaMatrix[I]	= 0;
			else
			{
				//(d^2+C^2)^(3/2)
				//可以保证是非零的
				dTemp			= GetDistance(pGridVertex->m_dX,pGridVertex->m_dY,0,pNextGridVertex->m_dX,pNextGridVertex->m_dY,0) + m_dC*m_dC;
				
				dParaMatrix[I]	= dTemp*sqrt(dTemp);
			}
			
			I++;
			pNextGridVertex	= pNextGridVertex->m_pNext;
		}
		
		//
		dTempMatrix[J]	= pGridVertex->m_dZ;
		//
		dTemp			= GetDistance(pGridVertex->m_dX,pGridVertex->m_dY,0,dX,dY,0) + m_dC*m_dC;

		//这里可能存在重点
		dTargetMatrix[J]= dTemp*sqrt(dTemp);

		J++;

		pGridVertex	= pGridVertex->m_pNext;
	}

	bool	bResult;
	bResult	= false;
	
	//调用MathCom解方程
	{
		Mm	maxtixPara,maxtixTemp,maxtixTarget;
		int	i;

		maxtixPara	= zeros(iCount,iCount);
		maxtixTemp	= zeros(iCount,1);
		maxtixTarget= zeros(1,iCount);

		for(i=0;i<iCount*iCount;i++)
		{	
			maxtixPara.r(1+i)	= dParaMatrix[i];
		}
		
		for(i=0;i<iCount;i++)
		{	
			maxtixTemp.r(1+i)	= dTempMatrix[i];
			maxtixTarget.r(1+i)	= dTargetMatrix[i];
		}
		
		//
		Mm maxtixResult;
		maxtixResult	= CalcRBFEquation(maxtixPara,maxtixTemp,maxtixTarget);

		m_type resultx;
		
		resultx	= maxtixResult.r(1);
		if(mexIsInf(resultx) || mexIsNaN(resultx)) 
		{
			dZ		= -9999;
			bResult	= false;
		}
		else
		{	
			dZ		= maxtixResult.r(1);
			bResult	= true;
		}
	}
	
	/*
	//调用m文件
	{
		int		i;
		mxArray	*pArrayOut	= NULL;
		mxArray	*pArrayInA	= NULL;
		mxArray	*pArrayInb	= NULL;
		mxArray	*pArrayInL	= NULL;

		pArrayInA	= mxCreateDoubleMatrix(iCount,iCount,mxREAL);

		for(i=0;i<iCount*iCount;i++)
		{	
			*(mxGetPr(pArrayInA) + i)	= dParaMatrix[i];
		}

		pArrayInb	= mxCreateDoubleMatrix(iCount,1,mxREAL);
		pArrayInL	= mxCreateDoubleMatrix(1,iCount,mxREAL);

		for(i=0;i<iCount;i++)
		{	
			*(mxGetPr(pArrayInb) + i)	= dTempMatrix[i];
			*(mxGetPr(pArrayInL) + i)	= dTargetMatrix[i];
		}
		
		//
		mlfLibCalcRBFEquation(1, &pArrayOut, pArrayInA, pArrayInb, pArrayInL);
		
		//
		if(mxIsInf(*(mxGetPr(pArrayOut))) || mxIsNaN(*(mxGetPr(pArrayOut)))) 
		{
			dZ	= -9999;
		}
		else
		{	
			dZ	= *(mxGetPr(pArrayOut));
		}

		//释放
		if(pArrayInA)	mxDestroyArray(pArrayInA);
		if(pArrayInb)	mxDestroyArray(pArrayInb);
		if(pArrayInL)	mxDestroyArray(pArrayInL);
		if(pArrayOut)	mxDestroyArray(pArrayOut);
	}
	*/

	delete	[]dParaMatrix;
	delete	[]dTempMatrix;
	delete	[]dTargetMatrix;

	return	bResult;
}

//
void CDCR_Grid_RBF::SetModelType(int iType)
{
	m_iModelType	= iType;
}

int CDCR_Grid_RBF::GetModelType()
{
	return	m_iModelType;
}

//
void CDCR_Grid_RBF::SetC(double dC)
{
	m_dC	= dC;
}

double CDCR_Grid_RBF::GetC()
{
	return	m_dC;
}

//
void CDCR_Grid_RBF::SetPathName(CString szPathName)
{
	m_szPathName	= szPathName;
}

CString CDCR_Grid_RBF::GetPathName()
{
	return	m_szPathName;
}
