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
	//Ȩָ��
	m_iU			= 2;
	//�⻬����
	m_dT			= 0;

	//
	m_bCustom		= false;

	m_dRadius		= 10000;

	//���������
	m_pVretexList	= NULL;

	//
	m_szPathName	= "D:\\Out(SPD).grd";

	//��ʼ��
	//libLeastSquareInitialize();
}

CDCR_Grid_SPD::~CDCR_Grid_SPD()
{
	//ֹͣ
	//libLeastSquareTerminate();
}

//���ö����б�
void CDCR_Grid_SPD::SetGridVertexList(CDCR_GridVertexList *pVretexList)
{
	m_pVretexList	= pVretexList;
}

//�����ֵ�ĸ߳�ֵ
bool CDCR_Grid_SPD::CalcHeight(double dX,double dY,double& dZ)
{
	if(m_pVretexList == NULL)	return	false;
	
	CDCR_GridVertex	*pVertex;
	
	//���������ͨ����Ȩ����
	//2011-06-30���Ƚ�d��R�����d��R������ΪȨ��Ϊ0�����Ǻ���ģ�Ӧ���˸Ľ�л��µľֲ��ԡ�
	//���ԣ����е�Rֵ����ָ���ģ�
	double	dUpValue	= 0;
	double	dDownValue	= 0;

	double	dUpWeighted		= 0;
	double	dDownWeighted	= 0;

	bool	bNullUsed		= true;

	pVertex	= m_pVretexList->GetGridVertex();
	while(pVertex != NULL)
	{	
		//����d>R����ɾ��������
		if(pVertex->m_dDistance > m_dRadius)
		{
			pVertex	= pVertex->m_pNext;
			continue;
		}

		bNullUsed	= false;

		//�����ǿ����㣬�򷵻أ�ֱ��ʹ�øõ�
		if(pVertex->m_dDistance < 1e-10)
		{
			dZ		= pVertex->m_dZ;

			return	true;
		}
		
		//����
		double	dDistance;
		dDistance	= sqrt(pVertex->m_dDistance*pVertex->m_dDistance + m_dT*m_dT);

		double	dWeighted;
		//���㣨(1/d-1/R)^u��
		dWeighted	= pow((1.0/dDistance - 1.0/m_dRadius),(double)m_iU);

		//�������
		dUpValue	= dUpValue + dWeighted * pVertex->m_dZ;	

		//�����ĸ
		dDownValue	= dDownValue + dWeighted;	
		
		pVertex	= pVertex->m_pNext;
	}
	
	if(bNullUsed)
		dZ	= -9999;
	else
		//��������ֵ
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
	
	//�����������
	CDCR_LeastSquare	pLeastSquare;

	//�������
	bool	bAlready;
	bAlready	= pLeastSquare.CalcMatrix(iPointNum,dSX,dSY,dSZ);

	//ʹ�ö��ζ���ʽ��ֵ����ԭʼֵ
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

	//����dZ
	//{
		//���������ͨ����Ȩ����
		double	dUpValue	= 0;
		double	dDownValue	= 0;

		double	dWeighted;

		pVertex	= m_pVretexList->GetGridVertex();
		while(pVertex != NULL)
		{	
			//����d*=d^2+t^2;
			dWeighted	= sqrt(pVertex->m_dDistance*pVertex->m_dDistance + m_dT*m_dT);
			//d*^u
			dWeighted	= pow(dWeighted,(double)m_iU);

			//
			if(dWeighted < 1e-10)	
			{
				dZ		= pVertex->m_dZ;

				return	true;
			}

			//���㣨1/(d*^u)��
			dWeighted	= 1.0/dWeighted;

			//�������
			dUpValue	= dUpValue + dWeighted * pVertex->m_dZ;	

			//�����ĸ
			dDownValue	= dDownValue + dWeighted;	
			
			pVertex	= pVertex->m_pNext;
		}	
		
		//if(dDownValue < 1e-10)
		//	dZ	= -9999;
		//else
			//��ֵ����õ�Zֵ
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
	
	//�����������
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
		
		//����ƽ��ֵ
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
		//�����������ŵ�������
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
		
		//ʹ�ö��ζ���ʽ��C0+C1*x+C2*y+C3*x*y+C4*x^2+C5*y^2+C6*x*y^2+C7*y*x^2+C8*x^2*y^2;
		//ͨ��m�ļ�����
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
			
			//����
			Mm maxtixResult;
			maxtixResult	= CalcOLSquation(maxtixInA,maxtixInZ);

			//�������õ���ÿһ��ֵ
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


	//����dZ,�������ʽ�޽⣬��ԭ�еĵ㣬��֮ʹ�ö���ʽ��
	//{
		//���������ͨ����Ȩ����
		double	dUpValue	= 0;
		double	dDownValue	= 0;

		double	dWeighted;
		
		i		= 0;
		pVertex	= m_pVretexList->GetGridVertex();
		while(pVertex != NULL)
		{	
			//����d*=d^2+t^2;
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

			//���㣨1/(d*^u)��
			dWeighted	= 1.0/dWeighted;

			//�������
			//dUpValue	= dUpValue + dWeighted * pVertex->m_dZ;	
			dUpValue	= dUpValue + dWeighted * dSZ[i];	

			//�����ĸ
			dDownValue	= dDownValue + dWeighted;	
			
			i++;

			pVertex	= pVertex->m_pNext;
		}	
		
		//if(dDownValue < 1e-10)
		//	dZ	= -9999;
		//else
			//��ֵ����õ�Zֵ
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

