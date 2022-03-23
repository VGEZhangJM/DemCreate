// DCR_LeastSquare.cpp: implementation of the CDCR_LeastSquare class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "vr_demcreator.h"
#include "DCR_LeastSquare.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDCR_LeastSquare::CDCR_LeastSquare()
{
	//���ζ���ʽ
	m_iIndex	= 2;

	//
	m_bAlready	= false;
	
	//����A
	A			= NULL;

	m_iCoff		= 6;
	//
	CX[0]		= 0;	CY[0]		= 0;
	//
	CX[1]		= 1;	CY[1]		= 0;
	//
	CX[2]		= 0;	CY[2]		= 1;
	//
	CX[3]		= 1;	CY[3]		= 1;
	//
	CX[4]		= 2;	CY[4]		= 0;
	//
	CX[5]		= 0;	CY[5]		= 2;
	
}

CDCR_LeastSquare::~CDCR_LeastSquare()
{
	if(A != NULL)	{	delete	A;	A	= NULL;	}
}

//////////////////////////////////////////////////////////////////////
// ����ϵ������
//////////////////////////////////////////////////////////////////////
bool CDCR_LeastSquare::GetZCoord(double dX,double dY,double& dZ)
{
	if(m_bAlready)
	{
		int	i	=0;
		dZ		= 0;
		
		for(i=0;i<m_iCoff;i++)
		{
			dZ	= dZ + pow(dX,CX[i])*pow(dY,CY[i])*C[i];
		}
		
		return	true;
	}
	
	//��Чֵ
	dZ	= -9999;

	return	false;
}

//////////////////////////////////////////////////////////////////////
// ����ϵ������
//////////////////////////////////////////////////////////////////////
bool CDCR_LeastSquare::CalcMatrix(int iPointNum,double *dX,double *dY,double *dZ)
{
	//
	m_bAlready	= false;

	if(iPointNum == 0)	return	false;
			
	//���ϵ�����ڲ������������޽�
	if(m_iCoff > iPointNum)	return	false;
	
	if(A != NULL)	{	delete	[]A;	A	= NULL;	}

	A	= new	double[iPointNum*m_iCoff];

	double	*Z;
	Z	= new	double[iPointNum];

	for(int i=0;i<iPointNum;i++)
	{
		int	iI	= 0;
		for(int j=0;j<m_iCoff;j++)
		{
			A[i*m_iCoff + j]	= pow(dX[i],CX[j]) * pow(dY[i],CY[j]);
		}

		Z[i]	= dZ[i];
	}
		
	//����MathLib�ⷽ��
	Mm	maxtixA,maxtixZ;
	
	maxtixA	= zeros(iPointNum,m_iCoff);
	maxtixZ	= zeros(iPointNum,1);
	
	//����A����
	for(i=0;i<iPointNum*m_iCoff;i++)
	{
		maxtixA.r(1+i)	= A[i];
	}
	//����Z����
	for(i=0;i<iPointNum;i++)
	{
		maxtixZ.r(1+i)	= Z[i];
	}

	Mm maxtixResult;
	maxtixResult	= CalcOLSquation(maxtixA,maxtixZ);
		
	//�������õ���ÿһ��ֵ
	m_type resultx;
	for(i=0;i<m_iCoff;i++)
	{
		resultx	= maxtixResult.r(1+i);
		if(mexIsInf(resultx) || mexIsNaN(resultx)) 
		{
			return	false;
		}

		C[i]	= resultx;
	}

	//
	m_bAlready	= true;

	return	true;
}