// DCR_SemiVariogram.cpp: implementation of the CDCR_SemiVariogram class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DCR_SemiVariogram.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDCR_SemiVariogram::CDCR_SemiVariogram()
{
	m_pPointHead	= NULL;

	//�ͺ����,һ��������
	m_dLagSize		= 0;
	//ȱʡΪ50
	m_iLagNum		= 50;

	//ÿһ�εĳ���
	m_dLagDistance	= NULL;
	//ÿһ�εĵ����Ŀ
	m_iLagCount		= NULL;

	//ÿһ�ͺ�����µİ���캯��ֵ
	m_dRPreLag		= NULL;
	//ÿһ�ͺ�����µ��ͺ����ֵ
	m_dHPreLag		= NULL;
}

CDCR_SemiVariogram::~CDCR_SemiVariogram()
{
	//��ʼ��
	if(m_dLagDistance != NULL)
	{
		delete	[]m_dLagDistance;
		m_dLagDistance	= NULL;
	}
	
	//
	if(m_iLagCount != NULL)
	{
		delete	[]m_iLagCount;
		m_iLagCount		= NULL;
	}

	//ÿһ�ͺ�����µİ���캯��ֵ
	if(m_dRPreLag != NULL)
	{
		delete	[]m_dRPreLag;
		m_dRPreLag		= NULL;
	}

	//ÿһ�ͺ�����µ��ͺ����ֵ
	if(m_dHPreLag != NULL)
	{
		delete	[]m_dHPreLag;
		m_dHPreLag		= NULL;
	}
}

//���������ľ���
double CDCR_SemiVariogram::CalcDistance(double dX1,double dY1,double dX2,double dY2)
{
	double	dDistance;
	
	dDistance	= sqrt( (dX1-dX2)*(dX1-dX2) + (dY1-dY2)*(dY1-dY2) );

	return	dDistance;
}

//��ȡ���ݵķ�Χ��һ��ȡ��Ⱥ͸߶ȵ���ֵ��һ�룬��������ȡ��ֵ
void CDCR_SemiVariogram::CalcRange(double &dLeft, double &dRight, double &dBottom, double &dTop)
{
	bool	bFirst		= true;

	dLeft	= dRight	= 0;
	dBottom	= dTop		= 0;

	if(m_pPointHead == NULL)	return;

	//����������ֵ
	CDCR_PointFeature	*pPF;
	
	pPF	= m_pPointHead;
	while(pPF != NULL)
	{
		if(bFirst)
		{
			dLeft	= dRight	= pPF->m_pPointCoord.x;
			dBottom	= dTop		= pPF->m_pPointCoord.y;
			
			bFirst	= false;
		}
		else
		{
			if(dLeft > pPF->m_pPointCoord.x)		dLeft	= pPF->m_pPointCoord.x;
			if(dRight < pPF->m_pPointCoord.x)	dRight	= pPF->m_pPointCoord.x;

			if(dBottom > pPF->m_pPointCoord.y)	dBottom	= pPF->m_pPointCoord.y;
			if(dTop < pPF->m_pPointCoord.y)		dTop	= pPF->m_pPointCoord.y;
		}
		
		pPF	= pPF->m_pNext;
	}
}

//������ɢ������
void CDCR_SemiVariogram::SetPointFeature(CDCR_PointFeature *pPointHead)
{
	m_pPointHead	= pPointHead;
}

//
void CDCR_SemiVariogram::SetPathName(CString	szPathName)
{
	m_szPathName	= szPathName;
}

//����ʵ�����캯��
int CDCR_SemiVariogram::SemiVariogram(double **dX,double **dY)
{
	//���ȼ������캯�����ͺ����,һ��ȥ����
	double	dLeft,dRight,dBottom,dTop;
	CalcRange(dLeft,dRight,dBottom,dTop);
	
	//
	double	dWidth;
	dWidth		= ((dRight-dLeft) + (dTop-dBottom))/2;
	
	m_dLagSize	= dWidth / m_iLagNum;
	
	//��֤��5�ı���
	//m_dLagSize	= (int)( m_dLagSize / 5.0 ) * 5;
	
	//��ʼ��
	if(m_dLagDistance != NULL)
	{
		delete	[]m_dLagDistance;
		m_dLagDistance	= NULL;
	}
	
	//
	if(m_iLagCount != NULL)
	{
		delete	[]m_iLagCount;
		m_iLagCount		= NULL;
	}

	//ÿһ�ͺ�����µİ���캯��ֵ
	if(m_dRPreLag != NULL)
	{
		delete	[]m_dRPreLag;
		m_dRPreLag		= NULL;
	}

	//ÿһ�ͺ�����µ��ͺ����ֵ
	if(m_dHPreLag != NULL)
	{
		delete	[]m_dHPreLag;
		m_dHPreLag		= NULL;
	}
	
	m_dLagDistance	= new	double[m_iLagNum];
	m_iLagCount		= new	int[m_iLagNum];

	m_dRPreLag		= new	double[m_iLagNum];
	m_dHPreLag		= new	double[m_iLagNum];

	for(int i=0;i<m_iLagNum;i++)
	{
		m_dLagDistance[i]	= 0.0;
		m_iLagCount[i]		= 0;

		m_dRPreLag[i]		= 0.0;
		m_dHPreLag[i]		= 0.0;
	}

	//����������ֵ
	CDCR_PointFeature	*pPF;
	CDCR_PointFeature	*pNextPF;
		
	pPF	= m_pPointHead;
	while(pPF != NULL)
	{
		pNextPF	= m_pPointHead;

		while(pNextPF != NULL)
		{
			//���߲���Ȳſ��Ե�
			if(pNextPF != pPF)
			{
				//�������
				double	dDistance;
				dDistance	= CalcDistance(pPF->m_pPointCoord.x,pPF->m_pPointCoord.y,pNextPF->m_pPointCoord.x,pNextPF->m_pPointCoord.y);
				
				int		I;
				I	= (int)( dDistance / m_dLagSize );

				if(I>=0 && I < m_iLagNum)
				{
					//�����¼���Ǿ���,���ڼ��������
					m_dLagDistance[I]	= m_dLagDistance[I] + dDistance;

					//
					m_iLagCount[I]++;

					//���ڼ�¼ (Z(x)-Z(x+h))^2
					m_dHPreLag[I]		= m_dHPreLag[I] + (pPF->m_pPointCoord.z - pNextPF->m_pPointCoord.z)*(pPF->m_pPointCoord.z - pNextPF->m_pPointCoord.z);
				}
			}
			
			pNextPF	= pNextPF->m_pNext;
		}

		pPF	= pPF->m_pNext;
	}
	
	for(i=0;i<m_iLagNum;i++)
	{
		//������
		if(m_iLagCount[i] != 0)
			m_dLagDistance[i]	= m_dLagDistance[i] / m_iLagCount[i];
		else
			m_dLagDistance[i]	= i * m_dLagSize;

		//������
		if(m_iLagCount[i] != 0)
			m_dRPreLag[i]		= m_dHPreLag[i] / (2 * m_iLagCount[i]);
		else
			m_dRPreLag[i]		= 0;
		
		if(dX != NULL)		(*dX)[i]			= m_dLagDistance[i];
		if(dY != NULL)		(*dY)[i]			= m_dRPreLag[i];
	}

	CString	szPathName;

	szPathName	= m_szPathName.Left(m_szPathName.GetLength()-4) + "Semi.txt"; 

	FILE	*fp;
	fp	= fopen(szPathName,"w");
	if(fp != NULL)
	{
		for(i=0;i<m_iLagNum;i++)
		{
			double	dLag;

			dLag	= m_dLagDistance[i];

			fprintf(fp,"%10.5lf ",dLag);
		}
		fprintf(fp,"\n");

		for(i=0;i<m_iLagNum;i++)
		{
			double	dLag;

			dLag	= m_dRPreLag[i];

			fprintf(fp,"%10.5lf ",dLag);
		}
		fprintf(fp,"\n");

		fclose(fp);
	}

	return	m_iLagNum;
}
