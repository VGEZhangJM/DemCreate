// DCR_Grid_KRG.cpp: implementation of the CDCR_Grid_KRG class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "vr_demcreator.h"
#include "DCR_Grid_KRG.h"

#include "matlib.h"
#include "math.h"
#include "CalcKRGEquation.h"

//#include "libCalcKRGEquation.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDCR_Grid_KRG::CDCR_Grid_KRG()
{
	//���������
	m_pVretexList	= NULL;

	//ģ������
	m_iModelType	= 0;

	//�����
	m_dC0			= 0;
	//����
	m_dC			= 20;
	//���
	m_dAlfa			= 200;

	m_szPathName	= "D:\\Out(KRG).grd";

	//��ʼ��
	//libCalcKRGEquationInitialize();
}

CDCR_Grid_KRG::~CDCR_Grid_KRG()
{
	//ֹͣ
	//libCalcKRGEquationTerminate();
}

//////////////////////////////////////////////////////////////////////
// ���ö����б�
//////////////////////////////////////////////////////////////////////
void CDCR_Grid_KRG::SetGridVertexList(CDCR_GridVertexList *pVretexList)
{
	m_pVretexList	= pVretexList;
}

//////////////////////////////////////////////////////////////////////
// �������
//////////////////////////////////////////////////////////////////////
double CDCR_Grid_KRG::GetDistance(double x1, double y1, double z1, double x2, double y2, double z2)
{
	double	dDistance;

	dDistance	= sqrt( (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) + (z1-z2)*(z1-z2) );
	//dDistance	= (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2);

	return	dDistance;
}

//////////////////////////////////////////////////////////////////////
// �����ֵ�ĸ߳�ֵ
//////////////////////////////////////////////////////////////////////
bool CDCR_Grid_KRG::CalcHeight(double dX,double dY,double& dZ,double& dSDZ)
{
	if(m_pVretexList == NULL)	return	false;
	
	//����
	if(m_iModelType == 0)
	{
		return	KRGSphereModel(dX,dY,dZ,dSDZ);
	}

	//ָ��
	if(m_iModelType == 1)
	{
		return	KRGExponentModel(dX,dY,dZ,dSDZ);
	}

	//��˹
	if(m_iModelType == 2)
	{
		return	KRGGaussModel(dX,dY,dZ,dSDZ);
	}

	//����
	if(m_iModelType == 3)
	{
		return	KRGLineModel(dX,dY,dZ,dSDZ);
	}

	return	false;
}

//////////////////////////////////////////////////////////////////////
// ����ģ�ͼ����ֵ�ĸ߳�ֵ
//////////////////////////////////////////////////////////////////////
bool CDCR_Grid_KRG::KRGLineModel(double dX,double dY,double& dZ,double& dSDZ)
{
	int		iCount;
	iCount	= m_pVretexList->GetGridVertexCount();
	
	//��������iCount��iCount��
	double	*dKMatrix; 
	dKMatrix		= new	double[(iCount+1)*(iCount+1)];

	//iCount��1��
	double	*dCMatrix;
	dCMatrix		= new	double[iCount+1];

	double	*dZMatrix;
	dZMatrix		= new	double[iCount+1];
	
	//
	CDCR_GridVertex	*pGridVertex;
	CDCR_GridVertex	*pNextGridVertex;

	double			dH;
	
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
				dKMatrix[I]	= m_dC;
			else
			{
				//���Ա�֤�Ƿ����
				dH	= GetDistance(pGridVertex->m_dX,pGridVertex->m_dY,0,pNextGridVertex->m_dX,pNextGridVertex->m_dY,0);
				
				//
				if(dH > m_dAlfa)
					dKMatrix[I]	= 0;
				else
				{
					double	dTemp;
					dTemp	= dH/m_dAlfa;
					dKMatrix[I]	= m_dC*(1-dTemp);
				}
			}
			
			
			I++;
			pNextGridVertex	= pNextGridVertex->m_pNext;
		}
		
		//+1
		dKMatrix[I]	= 1;
		I++;

		//
		dH	= GetDistance(pGridVertex->m_dX,pGridVertex->m_dY,0,dX,dY,0);

		if(dH < 1e-10)
		{
			dCMatrix[J]	= m_dC;
		}
		else
		{
			//
			if(dH > m_dAlfa)
				dCMatrix[J]	= 0;
			else
			{
				double	dTemp;
				dTemp	= dH/m_dAlfa;
				dCMatrix[J]	= m_dC*(1-dTemp);
			}
		}
		
		//
		dZMatrix[J]	= pGridVertex->m_dZ;

		J++;

		pGridVertex	= pGridVertex->m_pNext;
	}
	int	i;
	for(i=0;i<iCount;i++)
	{
		//�������һ�в���
		dKMatrix[I]	= 1;
		I++;
	}
	dKMatrix[I]	= 0;


	//�������һ������
	dCMatrix[J]	= 1;
	
	//����MatCom�ⷽ��
	{
		Mm	maxtixK,maxtixC;

		maxtixK	= zeros(iCount+1,iCount+1);
		maxtixC	= zeros(iCount+1,1);

		for(i=0;i<(iCount+1)*(iCount+1);i++)
		{	
			maxtixK.r(1+i)	= dKMatrix[i];
		}
		
		for(i=0;i<iCount+1;i++)
		{	
			maxtixC.r(1+i)	= dCMatrix[i];
		}

		//
		Mm maxtixResult;
		maxtixResult	= CalcKRGEquation(maxtixK,maxtixC);
		
		int		iSize;
		iSize	= maxtixResult.size();
		
		bool	bWrong	= false;
		for(i=0;i<iCount;i++)
		{
			m_type resultx;
			
			resultx	= maxtixResult.r(i+1);
			if(mexIsInf(resultx) || mexIsNaN(resultx)) 
			{
				bWrong	= true;
				break;
			}
		}

		if(!bWrong)
		{
			//ѭ����ȡdZ����
			dZ	= 0;
			for(i=0;i<iCount;i++)
			{
				dZ	= dZ + maxtixResult.r(i+1) * dZMatrix[i];
			}
			
			//��������õ�ķ���
			dSDZ	= m_dC0+m_dC;

			for(i=0;i<iCount;i++)
			{
				dSDZ	= dSDZ - maxtixResult.r(i+1) * dCMatrix[i];
			}

			dSDZ	= dSDZ - maxtixResult.r(iCount+1);
		}
		else
		{
			dZ		= 10000;
			dSDZ	= 0;
		}
	}

	/*
	//����m�ļ��ⷽ��
	{
		int		i;
		mxArray	*pArrayOut	= NULL;
		mxArray	*pArrayInK	= NULL;
		mxArray	*pArrayInC	= NULL;

		pArrayInK	= mxCreateDoubleMatrix(iCount+1,iCount+1,mxREAL);

		for(i=0;i<(iCount+1)*(iCount+1);i++)
		{	
			*(mxGetPr(pArrayInK) + i)	= dKMatrix[i];
		}

		pArrayInC	= mxCreateDoubleMatrix(iCount+1,1,mxREAL);

		for(i=0;i<iCount+1;i++)
		{	
			*(mxGetPr(pArrayInC) + i)	= dCMatrix[i];
		}
		
		//����ⷽ��
		mlfLibCalcKRGEquation(1, &pArrayOut, pArrayInK, pArrayInC);
		
		if(pArrayOut != NULL)
		{
			//�ж��Ƿ���ڴ�������
			bool	bWrong	= false;
			for(i=0;i<iCount;i++)
			{
				if( mxIsInf(*(mxGetPr(pArrayOut)+i)) || mxIsNaN(*(mxGetPr(pArrayOut)+i)) )
				{
					bWrong	= true;
					break;
				}
			}

			if(!bWrong)
			{
				//ѭ����ȡdZ����
				dZ	= 0;
				for(i=0;i<iCount;i++)
				{
					dZ	= dZ + *(mxGetPr(pArrayOut)+i) * dZMatrix[i];
				}
				
				//��������õ�ķ���
				dSDZ	= m_dC0+m_dC;

				for(i=0;i<iCount;i++)
				{
					dSDZ	= dSDZ - *(mxGetPr(pArrayOut)+i) * dCMatrix[i];
				}

				dSDZ	= dSDZ - *(mxGetPr(pArrayOut)+iCount);
			}
			else
			{
				dZ		= -9999;
				dSDZ	= 0;
			}
		}
		else
		{
			dZ		= -9999;
			dSDZ	= 0;

			CString	szInfo;
			szInfo.Format("dX=%lf dY=%lf ���ڵ�ⷽ�̴���!\n",dX,dY);
			TRACE(szInfo);
		}

		//�ͷ�
		if(pArrayInK)	mxDestroyArray(pArrayInK);
		if(pArrayInC)	mxDestroyArray(pArrayInC);
		if(pArrayOut)	mxDestroyArray(pArrayOut);
	}
	*/

	delete	[]dKMatrix;
	delete	[]dCMatrix;
	delete	[]dZMatrix;
	
	return	true;	
}

//////////////////////////////////////////////////////////////////////
// ��״ģ�ͼ����ֵ�ĸ߳�ֵ
//////////////////////////////////////////////////////////////////////
bool CDCR_Grid_KRG::KRGSphereModel(double dX, double dY, double &dZ,double &dSDZ)
{
	int		iCount;
	iCount	= m_pVretexList->GetGridVertexCount();
	
	//��������iCount��iCount��
	double	*dKMatrix; 
	dKMatrix		= new	double[(iCount+1)*(iCount+1)];

	//iCount��1��
	double	*dCMatrix;
	dCMatrix		= new	double[iCount+1];

	double	*dZMatrix;
	dZMatrix		= new	double[iCount+1];
	
	//
	CDCR_GridVertex	*pGridVertex;
	CDCR_GridVertex	*pNextGridVertex;

	double			dH;
	
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
				dKMatrix[I]	= m_dC0+m_dC;
			else
			{
				//���Ա�֤�Ƿ����
				dH	= GetDistance(pGridVertex->m_dX,pGridVertex->m_dY,0,pNextGridVertex->m_dX,pNextGridVertex->m_dY,0);
				
				//
				if(dH > m_dAlfa)
					dKMatrix[I]	= 0;
				else
				{
					double	dTemp;
					dTemp	= dH/m_dAlfa;
					dKMatrix[I]	= m_dC*(1-1.5*dTemp+0.5*dTemp*dTemp*dTemp);
				}
			}
			
			I++;
			pNextGridVertex	= pNextGridVertex->m_pNext;
		}
		
		//+1
		dKMatrix[I]	= 1;
		I++;

		//
		dH	= GetDistance(pGridVertex->m_dX,pGridVertex->m_dY,0,dX,dY,0);

		if(dH < 1e-10)
		{
			dCMatrix[J]	= m_dC0+m_dC;
		}
		else
		{
			//
			if(dH > m_dAlfa)
				dCMatrix[J]	= 0;
			else
			{
				double	dTemp;
				dTemp	= dH/m_dAlfa;
				dCMatrix[J]	= m_dC*(1-1.5*dTemp+0.5*dTemp*dTemp*dTemp);
			}
		}
		
		//
		dZMatrix[J]	= pGridVertex->m_dZ;

		J++;

		pGridVertex	= pGridVertex->m_pNext;
	}
	int	i;
	for(i=0;i<iCount;i++)
	{
		//�������һ�в���
		dKMatrix[I]	= 1;
		I++;
	}
	dKMatrix[I]	= 0;


	//�������һ������
	dCMatrix[J]	= 1;
	
	//����MatCom�ⷽ��
	{
		Mm	maxtixK,maxtixC;

		maxtixK	= zeros(iCount+1,iCount+1);
		maxtixC	= zeros(iCount+1,1);

		for(i=0;i<(iCount+1)*(iCount+1);i++)
		{	
			maxtixK.r(1+i)	= dKMatrix[i];
		}
		
		for(i=0;i<iCount+1;i++)
		{	
			maxtixC.r(1+i)	= dCMatrix[i];
		}

		//
		Mm maxtixResult;
		maxtixResult	= CalcKRGEquation(maxtixK,maxtixC);
		
		int		iSize;
		iSize	= maxtixResult.size();
		
		bool	bWrong	= false;
		for(i=0;i<iCount;i++)
		{
			m_type resultx;
			
			resultx	= maxtixResult.r(i+1);
			if(mexIsInf(resultx) || mexIsNaN(resultx)) 
			{
				bWrong	= true;
				break;
			}
		}

		if(!bWrong)
		{
			//ѭ����ȡdZ����
			dZ	= 0;
			for(i=0;i<iCount;i++)
			{
				dZ	= dZ + maxtixResult.r(i+1) * dZMatrix[i];
			}
			
			//��������õ�ķ���
			dSDZ	= m_dC0+m_dC;

			for(i=0;i<iCount;i++)
			{
				dSDZ	= dSDZ - maxtixResult.r(i+1) * dCMatrix[i];
			}

			dSDZ	= dSDZ - maxtixResult.r(iCount+1);
		}
		else
		{
			dZ		= 10000;
			dSDZ	= 0;
		}
	}

	/*
	//����m�ļ��ⷽ��
	{
		int		i;
		mxArray	*pArrayOut	= NULL;
		mxArray	*pArrayInK	= NULL;
		mxArray	*pArrayInC	= NULL;

		pArrayInK	= mxCreateDoubleMatrix(iCount+1,iCount+1,mxREAL);

		for(i=0;i<(iCount+1)*(iCount+1);i++)
		{	
			*(mxGetPr(pArrayInK) + i)	= dKMatrix[i];
		}

		pArrayInC	= mxCreateDoubleMatrix(iCount+1,1,mxREAL);

		for(i=0;i<iCount+1;i++)
		{	
			*(mxGetPr(pArrayInC) + i)	= dCMatrix[i];
		}
		
		//����ⷽ��
		mlfLibCalcKRGEquation(1, &pArrayOut, pArrayInK, pArrayInC);
		
		if(pArrayOut != NULL)
		{
			//�ж��Ƿ���ڴ�������
			bool	bWrong	= false;
			for(i=0;i<iCount;i++)
			{
				if( mxIsInf(*(mxGetPr(pArrayOut)+i)) || mxIsNaN(*(mxGetPr(pArrayOut)+i)) )
				{
					bWrong	= true;
					break;
				}
			}

			if(!bWrong)
			{
				//ѭ����ȡdZ����
				dZ	= 0;
				for(i=0;i<iCount;i++)
				{
					dZ	= dZ + *(mxGetPr(pArrayOut)+i) * dZMatrix[i];
				}
				
				//��������õ�ķ���
				dSDZ	= m_dC0+m_dC;

				for(i=0;i<iCount;i++)
				{
					dSDZ	= dSDZ - *(mxGetPr(pArrayOut)+i) * dCMatrix[i];
				}

				dSDZ	= dSDZ - *(mxGetPr(pArrayOut)+iCount);
			}
			else
			{
				dZ		= -9999;
				dSDZ	= 0;
			}
		}
		else
		{
			dZ		= -9999;
			dSDZ	= 0;

			CString	szInfo;
			szInfo.Format("dX=%lf dY=%lf ���ڵ�ⷽ�̴���!\n",dX,dY);
			TRACE(szInfo);
		}

		//�ͷ�
		if(pArrayInK)	mxDestroyArray(pArrayInK);
		if(pArrayInC)	mxDestroyArray(pArrayInC);
		if(pArrayOut)	mxDestroyArray(pArrayOut);
	}
	*/

	delete	[]dKMatrix;
	delete	[]dCMatrix;
	delete	[]dZMatrix;
	
	return	true;
}

//////////////////////////////////////////////////////////////////////
// ָ��ģ�ͼ����ֵ�ĸ߳�ֵ
//////////////////////////////////////////////////////////////////////
bool CDCR_Grid_KRG::KRGExponentModel(double dX, double dY, double &dZ,double &dSDZ)
{
	int		iCount;
	iCount	= m_pVretexList->GetGridVertexCount();
	
	//��������iCount��iCount��
	double	*dKMatrix; 
	dKMatrix		= new	double[(iCount+1)*(iCount+1)];

	//iCount��1��
	double	*dCMatrix;
	dCMatrix		= new	double[iCount+1];

	double	*dZMatrix;
	dZMatrix		= new	double[iCount+1];
	
	//
	CDCR_GridVertex	*pGridVertex;
	CDCR_GridVertex	*pNextGridVertex;

	double			dH;
	
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
				dKMatrix[I]	= m_dC0+m_dC;
			else
			{
				//���Ա�֤�Ƿ����
				dH	= GetDistance(pGridVertex->m_dX,pGridVertex->m_dY,0,pNextGridVertex->m_dX,pNextGridVertex->m_dY,0);
					
				//==0
				if(dH < 1e-10)	
					dKMatrix[I]	= m_dC0+m_dC;
				else
				{
					//
					double	dTemp;
					dTemp	= dH/m_dAlfa;
					dKMatrix[I]	= m_dC/(exp(dTemp));
				}
			}
			
			I++;
			pNextGridVertex	= pNextGridVertex->m_pNext;
		}
		
		//+1
		dKMatrix[I]	= 1;
		I++;

		//
		dH	= GetDistance(pGridVertex->m_dX,pGridVertex->m_dY,0,dX,dY,0);

		if(dH < 1e-10)
		{
			dCMatrix[J]	= m_dC0+m_dC;
		}
		else
		{
			double	dTemp;
			dTemp	= dH/m_dAlfa;

			dCMatrix[J]	= m_dC/(exp(dTemp));
		}
		
		//
		dZMatrix[J]	= pGridVertex->m_dZ;

		J++;

		pGridVertex	= pGridVertex->m_pNext;
	}
	int	i;
	for(i=0;i<iCount;i++)
	{
		//�������һ�в���
		dKMatrix[I]	= 1;
		I++;
	}
	dKMatrix[I]	= 0;

	//�������һ������
	dCMatrix[J]	= 1;
	
	//����MatCom�ⷽ��
	{
		Mm	maxtixK,maxtixC;

		maxtixK	= zeros(iCount+1,iCount+1);
		maxtixC	= zeros(iCount+1,1);

		for(i=0;i<(iCount+1)*(iCount+1);i++)
		{	
			maxtixK.r(1+i)	= dKMatrix[i];
		}
		
		for(i=0;i<iCount+1;i++)
		{	
			maxtixC.r(1+i)	= dCMatrix[i];
		}
		
		//
		Mm maxtixResult;
		maxtixResult	= CalcKRGEquation(maxtixK,maxtixC);
		
		int		iSize;
		iSize	= maxtixResult.size();
		
		bool	bWrong	= false;
		for(i=0;i<iCount;i++)
		{
			m_type resultx;
			
			resultx	= maxtixResult.r(i+1);
			if(mexIsInf(resultx) || mexIsNaN(resultx)) 
			{
				bWrong	= true;
				break;
			}
		}

		if(!bWrong)
		{
			//ѭ����ȡdZ����
			dZ	= 0;
			for(i=0;i<iCount;i++)
			{
				dZ	= dZ + maxtixResult.r(i+1) * dZMatrix[i];
			}
			
			//��������õ�ķ���
			dSDZ	= m_dC0+m_dC;

			for(i=0;i<iCount;i++)
			{
				dSDZ	= dSDZ - maxtixResult.r(i+1) * dCMatrix[i];
			}

			dSDZ	= dSDZ - maxtixResult.r(iCount+1);
		}
		else
		{
			dZ		= 10000;
			dSDZ	= 0;
		}
	}

	/*
	//����m�ļ��ⷽ��
	{
		int		i;
		mxArray	*pArrayOut	= NULL;
		mxArray	*pArrayInK	= NULL;
		mxArray	*pArrayInC	= NULL;

		pArrayInK	= mxCreateDoubleMatrix(iCount+1,iCount+1,mxREAL);

		for(i=0;i<(iCount+1)*(iCount+1);i++)
		{	
			*(mxGetPr(pArrayInK) + i)	= dKMatrix[i];
		}

		pArrayInC	= mxCreateDoubleMatrix(iCount+1,1,mxREAL);

		for(i=0;i<iCount+1;i++)
		{	
			*(mxGetPr(pArrayInC) + i)	= dCMatrix[i];
		}
		
		//����ⷽ��
		mlfLibCalcKRGEquation(1, &pArrayOut, pArrayInK, pArrayInC);
		
		if(pArrayOut != NULL)
		{
			//�ж��Ƿ���ڴ�������
			bool	bWrong	= false;
			for(i=0;i<iCount;i++)
			{
				if( mxIsInf(*(mxGetPr(pArrayOut)+i)) || mxIsNaN(*(mxGetPr(pArrayOut)+i)) )
				{
					bWrong	= true;
					break;
				}
			}

			if(!bWrong)
			{
				//ѭ����ȡdZ����
				dZ	= 0;
				for(i=0;i<iCount;i++)
				{
					dZ	= dZ + *(mxGetPr(pArrayOut)+i) * dZMatrix[i];
				}
				
				//��������õ�ķ���
				dSDZ	= m_dC0+m_dC;

				for(i=0;i<iCount;i++)
				{
					dSDZ	= dSDZ - *(mxGetPr(pArrayOut)+i) * dCMatrix[i];
				}

				dSDZ	= dSDZ - *(mxGetPr(pArrayOut)+iCount);
			}
			else
			{
				dZ		= -9999;
				dSDZ	= 0;
			}
		}
		else
		{
			dZ		= -9999;
			dSDZ	= 0;

			CString	szInfo;
			szInfo.Format("dX=%lf dY=%lf ���ڵ�ⷽ�̴���!\n",dX,dY);
			TRACE(szInfo);
		}

		//�ͷ�
		if(pArrayInK)	mxDestroyArray(pArrayInK);
		if(pArrayInC)	mxDestroyArray(pArrayInC);
		if(pArrayOut)	mxDestroyArray(pArrayOut);
	}
	*/

	delete	[]dKMatrix;
	delete	[]dCMatrix;
	delete	[]dZMatrix;
	
	return	true;
}

//////////////////////////////////////////////////////////////////////
// ��˹ģ�ͼ����ֵ�ĸ߳�ֵ
//////////////////////////////////////////////////////////////////////
bool CDCR_Grid_KRG::KRGGaussModel(double dX, double dY, double &dZ,double &dSDZ)
{
	int		iCount;
	iCount	= m_pVretexList->GetGridVertexCount();
	
	//��������iCount��iCount��
	double	*dKMatrix; 
	dKMatrix		= new	double[(iCount+1)*(iCount+1)];

	//iCount��1��
	double	*dCMatrix;
	dCMatrix		= new	double[iCount+1];

	double	*dZMatrix;
	dZMatrix		= new	double[iCount+1];
	
	//
	CDCR_GridVertex	*pGridVertex;
	CDCR_GridVertex	*pNextGridVertex;

	double			dH;
	
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
				dKMatrix[I]	= m_dC0+m_dC;
			else
			{
				//���Ա�֤�Ƿ����
				dH	= GetDistance(pGridVertex->m_dX,pGridVertex->m_dY,0,pNextGridVertex->m_dX,pNextGridVertex->m_dY,0);
				
				if(dH < 1e-10)
					dKMatrix[I]	= m_dC0+m_dC;
				else
				{
					//
					double	dTemp;
					dTemp	= dH/m_dAlfa;
					dKMatrix[I]	= m_dC/(exp(dTemp*dTemp));
				}
			}
			
			I++;
			pNextGridVertex	= pNextGridVertex->m_pNext;
		}
		
		//+1
		dKMatrix[I]	= 1;
		I++;

		//
		dH	= GetDistance(pGridVertex->m_dX,pGridVertex->m_dY,0,dX,dY,0);

		if(dH < 1e-10)
		{
			dCMatrix[J]	= m_dC0+m_dC;
		}
		else
		{
			double	dTemp;
			dTemp	= dH/m_dAlfa;

			dCMatrix[J]	= m_dC/(exp(dTemp*dTemp));
		}
		
		//
		dZMatrix[J]	= pGridVertex->m_dZ;

		J++;

		pGridVertex	= pGridVertex->m_pNext;
	}
	int	i;
	for(i=0;i<iCount;i++)
	{
		//�������һ�в���
		dKMatrix[I]	= 1;
		I++;
	}
	dKMatrix[I]	= 0;

	//�������һ������
	dCMatrix[J]	= 1;
	
	
	//����MatCom�ⷽ��
	{
		Mm	maxtixK,maxtixC;

		maxtixK	= zeros(iCount+1,iCount+1);
		maxtixC	= zeros(iCount+1,1);

		for(i=0;i<(iCount+1)*(iCount+1);i++)
		{	
			maxtixK.r(1+i)	= dKMatrix[i];
		}
		
		for(i=0;i<iCount+1;i++)
		{	
			maxtixC.r(1+i)	= dCMatrix[i];
		}
		
		//
		Mm maxtixResult;
		maxtixResult	= CalcKRGEquation(maxtixK,maxtixC);
		
		int		iSize;
		iSize	= maxtixResult.size();
		
		bool	bWrong	= false;
		for(i=0;i<iCount;i++)
		{
			m_type resultx;
			
			resultx	= maxtixResult.r(i+1);
			if(mexIsInf(resultx) || mexIsNaN(resultx)) 
			{
				bWrong	= true;
				break;
			}
		}
		
		if(!bWrong)
		{
			//ѭ����ȡdZ����
			dZ	= 0;
			for(i=0;i<iCount;i++)
			{
				dZ	= dZ + maxtixResult.r(i+1) * dZMatrix[i];
			}
			
			//��������õ�ķ���
			dSDZ	= m_dC0+m_dC;

			for(i=0;i<iCount;i++)
			{
				dSDZ	= dSDZ - maxtixResult.r(i+1) * dCMatrix[i];
			}

			dSDZ	= dSDZ - maxtixResult.r(iCount+1);
		}
		else
		{
			dZ		= 10000;
			dSDZ	= 0;
		}
	}
	
	/*
	//����m�ļ��ⷽ��
	{
		int		i;
		mxArray	*pArrayOut	= NULL;
		mxArray	*pArrayInK	= NULL;
		mxArray	*pArrayInC	= NULL;

		pArrayInK	= mxCreateDoubleMatrix(iCount+1,iCount+1,mxREAL);

		for(i=0;i<(iCount+1)*(iCount+1);i++)
		{	
			*(mxGetPr(pArrayInK) + i)	= dKMatrix[i];
		}

		pArrayInC	= mxCreateDoubleMatrix(iCount+1,1,mxREAL);

		for(i=0;i<iCount+1;i++)
		{	
			*(mxGetPr(pArrayInC) + i)	= dCMatrix[i];
		}
		
		//����ⷽ��
		mlfLibCalcKRGEquation(1, &pArrayOut, pArrayInK, pArrayInC);
		
		if(pArrayOut != NULL)
		{
			//�ж��Ƿ���ڴ�������
			bool	bWrong	= false;
			for(i=0;i<iCount;i++)
			{
				if( mxIsInf(*(mxGetPr(pArrayOut)+i)) || mxIsNaN(*(mxGetPr(pArrayOut)+i)) )
				{
					bWrong	= true;
					break;
				}
			}

			if(!bWrong)
			{
				//ѭ����ȡdZ����
				dZ	= 0;
				for(i=0;i<iCount;i++)
				{
					dZ	= dZ + *(mxGetPr(pArrayOut)+i) * dZMatrix[i];
				}
				
				//��������õ�ķ���
				dSDZ	= m_dC0+m_dC;

				for(i=0;i<iCount;i++)
				{
					dSDZ	= dSDZ - *(mxGetPr(pArrayOut)+i) * dCMatrix[i];
				}

				dSDZ	= dSDZ - *(mxGetPr(pArrayOut)+iCount);
			}
			else
			{
				dZ		= -9999;
				dSDZ	= 0;
			}
		}
		else
		{
			dZ		= -9999;
			dSDZ	= 0;

			CString	szInfo;
			szInfo.Format("dX=%lf dY=%lf ���ڵ�ⷽ�̴���!\n",dX,dY);
			TRACE(szInfo);
		}

		//�ͷ�
		if(pArrayInK)	mxDestroyArray(pArrayInK);
		if(pArrayInC)	mxDestroyArray(pArrayInC);
		if(pArrayOut)	mxDestroyArray(pArrayOut);
	}
	*/

	delete	[]dKMatrix;
	delete	[]dCMatrix;
	delete	[]dZMatrix;
	
	return	true;
}

//
void CDCR_Grid_KRG::SetModelType(int iModelType)
{
	m_iModelType	= iModelType;
}

int CDCR_Grid_KRG::GetModelType()
{
	return	m_iModelType;
}

//
void CDCR_Grid_KRG::SetC(double	dC)
{
	m_dC	= dC;
}

double CDCR_Grid_KRG::GetC()
{
	return	m_dC;
}

//
void CDCR_Grid_KRG::SetC0(double dC0)
{
	m_dC0	= dC0;
}

double CDCR_Grid_KRG::GetC0()
{
	return	m_dC0;
}

//
void CDCR_Grid_KRG::SetA(double dA)
{
	m_dAlfa	= dA;
}

double CDCR_Grid_KRG::GetA()
{
	return	m_dAlfa;
}
