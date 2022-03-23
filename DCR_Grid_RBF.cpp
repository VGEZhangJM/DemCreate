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
	//���������
	m_pVretexList	= NULL;
	//
	m_iModelType	= 0;

	//����
	m_dC			= 0;

	m_szPathName	= "D:\\Out(RBF).grd";

	//��ʼ��
	//libCalcRBFEquationInitialize();
}

CDCR_Grid_RBF::~CDCR_Grid_RBF()
{
	//ֹͣ
	//libCalcRBFEquationTerminate();
}

//////////////////////////////////////////////////////////////////////
// ���ö����б�
//////////////////////////////////////////////////////////////////////
void CDCR_Grid_RBF::SetGridVertexList(CDCR_GridVertexList *pVretexList)
{
	m_pVretexList	= pVretexList;
}

//////////////////////////////////////////////////////////////////////
// �������
//////////////////////////////////////////////////////////////////////
double CDCR_Grid_RBF::GetDistance(double x1, double y1, double z1, double x2, double y2, double z2)
{
	double	dDistance;

	//dDistance	= sqrt( (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) + (z1-z2)*(z1-z2) );
	dDistance	= (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2);

	return	dDistance;
}

//////////////////////////////////////////////////////////////////////
// �����ֵ�ĸ߳�ֵ
//////////////////////////////////////////////////////////////////////
bool CDCR_Grid_RBF::CalcHeight(double dX,double dY,double& dZ)
{
	if(m_pVretexList == NULL)	return	false;
	
	//�������溯��
	if(m_iModelType == 0)
	{
		return	RBFMQF(dX,dY,dZ);
	}

	//�����������溯��
	if(m_iModelType == 1)
	{
		return	RBFIMQF(dX,dY,dZ);
	}

	//��������
	if(m_iModelType == 2)
	{
		return	RBFTPSF(dX,dY,dZ);
	}

	//���ض���
	if(m_iModelType == 3)
	{
		return	RBFMLF(dX,dY,dZ);
	}
	
	//��Ȼ������������
	if(m_iModelType == 4)
	{
		return	RBNCSF(dX,dY,dZ);
	}

	return	false;
}

//////////////////////////////////////////////////////////////////////
// RBFMQF�����ֵ�ĸ߳�ֵ
//////////////////////////////////////////////////////////////////////
bool CDCR_Grid_RBF::RBFMQF(double dX, double dY, double &dZ)
{
	int		iCount;
	iCount	= m_pVretexList->GetGridVertexCount();
	
	//��������iCount��iCount��
	double	*dParaMatrix; 
	dParaMatrix		= new	double[iCount*iCount];

	//iCount��1��
	double	*dTempMatrix;
	dTempMatrix		= new	double[iCount];
	
	//1��iCount��
	double	*dTargetMatrix;
	dTargetMatrix	= new	double[iCount];

	//
	CDCR_GridVertex	*pGridVertex;
	CDCR_GridVertex	*pNextGridVertex;
	
	//�������
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
	//����MatCom�ķ�ʽʵ��
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
	//����m�ļ�
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

		//�ͷ�
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
// RBFIMQF�����ֵ�ĸ߳�ֵ
//////////////////////////////////////////////////////////////////////
bool CDCR_Grid_RBF::RBFIMQF(double dX, double dY, double &dZ)
{
	int		iCount;
	iCount	= m_pVretexList->GetGridVertexCount();
	
	//��������iCount��iCount��
	double	*dParaMatrix; 
	dParaMatrix		= new	double[iCount*iCount];

	//iCount��1��
	double	*dTempMatrix;
	dTempMatrix		= new	double[iCount];
	
	//1��iCount��
	double	*dTargetMatrix;
	dTargetMatrix	= new	double[iCount];

	//
	CDCR_GridVertex	*pGridVertex;
	CDCR_GridVertex	*pNextGridVertex;

	bool			bRePoint;
	double			dRePointZ;

	bRePoint	= false;
	dRePointZ	= 0;
	
	//�������
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
				//���Ա�֤�Ƿ����
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

		//������ܴ����ص�
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
			//����MatCom
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
		//����m�ļ�
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

			//�ͷ�
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
// RBFIMQF�����ֵ�ĸ߳�ֵ
//////////////////////////////////////////////////////////////////////
bool CDCR_Grid_RBF::RBFTPSF(double dX, double dY, double &dZ)
{
	int		iCount;
	iCount	= m_pVretexList->GetGridVertexCount();
	
	//��������iCount��iCount��
	double	*dParaMatrix; 
	dParaMatrix		= new	double[iCount*iCount];

	//iCount��1��
	double	*dTempMatrix;
	dTempMatrix		= new	double[iCount];
	
	//1��iCount��
	double	*dTargetMatrix;
	dTargetMatrix	= new	double[iCount];

	//
	CDCR_GridVertex	*pGridVertex;
	CDCR_GridVertex	*pNextGridVertex;
	double			dTemp;

	//�������
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
				//���Ա�֤�Ƿ����
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

		//������ܴ����ص�
		dTargetMatrix[J]= dTemp * log(dTemp);

		J++;

		pGridVertex	= pGridVertex->m_pNext;
	}
	
	//����MathLib�ⷽ��
	bool	bResult;
	bResult	= false;
	{
		//��ͼ����ʾ	
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
	//����m�ļ�
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

		//�ͷ�
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
// RBFMLF�����ֵ�ĸ߳�ֵ
//////////////////////////////////////////////////////////////////////
bool CDCR_Grid_RBF::RBFMLF(double dX, double dY, double &dZ)
{
	int		iCount;
	iCount	= m_pVretexList->GetGridVertexCount();
	
	//��������iCount��iCount��
	double	*dParaMatrix; 
	dParaMatrix		= new	double[iCount*iCount];

	//iCount��1��
	double	*dTempMatrix;
	dTempMatrix		= new	double[iCount];
	
	//1��iCount��
	double	*dTargetMatrix;
	dTargetMatrix	= new	double[iCount];

	//
	CDCR_GridVertex	*pGridVertex;
	CDCR_GridVertex	*pNextGridVertex;
	double			dTemp;

	//�������
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
				//���Ա�֤�Ƿ����
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

		//������ܴ����ص�
		dTargetMatrix[J]= log(dTemp);

		J++;

		pGridVertex	= pGridVertex->m_pNext;
	}
	
	bool	bResult;
	bResult	= false;
	//����MatCom�ⷽ��
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
	//����m�ļ�
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

		//�ͷ�
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
// RBNCSF�����ֵ�ĸ߳�ֵ
//////////////////////////////////////////////////////////////////////
bool CDCR_Grid_RBF::RBNCSF(double dX, double dY, double &dZ)
{
	int		iCount;
	iCount	= m_pVretexList->GetGridVertexCount();
	
	//��������iCount��iCount��
	double	*dParaMatrix; 
	dParaMatrix		= new	double[iCount*iCount];

	//iCount��1��
	double	*dTempMatrix;
	dTempMatrix		= new	double[iCount];
	
	//1��iCount��
	double	*dTargetMatrix;
	dTargetMatrix	= new	double[iCount];

	//
	CDCR_GridVertex	*pGridVertex;
	CDCR_GridVertex	*pNextGridVertex;
	double			dTemp;

	//�������
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
				//���Ա�֤�Ƿ����
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

		//������ܴ����ص�
		dTargetMatrix[J]= dTemp*sqrt(dTemp);

		J++;

		pGridVertex	= pGridVertex->m_pNext;
	}

	bool	bResult;
	bResult	= false;
	
	//����MathCom�ⷽ��
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
	//����m�ļ�
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

		//�ͷ�
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
