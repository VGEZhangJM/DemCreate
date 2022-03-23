// DCR_LocalTerrainCharacter.cpp: implementation of the CDCR_LocalTerrainCharacter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "vr_demcreator.h"
#include "DCR_LocalTerrainCharacter.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDCR_LocalTerrainCharacter::CDCR_LocalTerrainCharacter()
{
	//���������
	m_pVretexList			= NULL;

	//
	m_pAllVretexList		= NULL;
	
	//���뾶
	m_dRadius				= 0;
	m_dMinDistance			= 0;

	//����ֵ,-32767��ȱʡֵ
	m_dPointPatternValue	= -32767;
	//
	m_dCentroidValue		= -32767;
	//ƽ���¶�ֵ
	m_dMeanSlope			= -32767;
	//���δֲڶ�ֵ
	m_dMeanRoughness		= -32767;

	m_dCenterX				= 0;
	m_dCenterY				= 0;
}

CDCR_LocalTerrainCharacter::~CDCR_LocalTerrainCharacter()
{

}

//���ö����б�
void CDCR_LocalTerrainCharacter::SetGridVertexList(CDCR_GridVertexList *pAllVretexList,CDCR_GridVertexList *pVretexList,double dRadius,double dX,double dY)
{
	//���в����㣬���ڼ���ƽ���¶�
	m_pAllVretexList	= pAllVretexList;
	//��������㣬���ڼ������ֵ
	m_pVretexList		= pVretexList;
	//	
	m_dRadius			= dRadius;

	//
	m_dCenterX			= dX;
	m_dCenterY			= dY;

	//������С����
	bool			bFirstValue;
	CDCR_GridVertex	*pVertex;

	bFirstValue	= true;
	pVertex		= m_pAllVretexList->GetGridVertex();
	while(pVertex != NULL)
	{
		if(bFirstValue)
		{
			m_dMinDistance	= pVertex->m_dDistance;
			bFirstValue		= false;
		}
		else
		{
			if(m_dMinDistance > pVertex->m_dDistance)
				m_dMinDistance	= pVertex->m_dDistance;
		}

		pVertex	= pVertex->m_pNext;
	}
}

//���ȴ���TIN����TINģ���м����¶Ⱥʹֲڶ�
bool CDCR_LocalTerrainCharacter::CaculateTerrainFactor()
{
	if(m_pAllVretexList == NULL)	return	false;

	//���ͷ�
	m_pTinCreator.Release();
	
	//{
	//��ȡ��ǰ����ķ�Χ
	double	dLeft,dRight,dBottom,dTop;
	
	bool			bFirstValue;
	CDCR_GridVertex	*pVertex;

	bFirstValue	= true;
	pVertex		= m_pAllVretexList->GetGridVertex();
	while(pVertex != NULL)
	{
		if(bFirstValue)
		{
			dLeft	= dRight	= pVertex->m_dX;
			dBottom	= dTop		= pVertex->m_dY;

			bFirstValue			= false;
		}
		else
		{
			if(dLeft > pVertex->m_dX)	dLeft	= pVertex->m_dX;
			if(dRight < pVertex->m_dX)	dRight	= pVertex->m_dX;

			if(dBottom > pVertex->m_dY)	dBottom	= pVertex->m_dY;
			if(dTop < pVertex->m_dY)	dTop	= pVertex->m_dY;
		}

		pVertex	= pVertex->m_pNext;
	}

	//
	m_pTinCreator.SetMapExt(dLeft,dRight,dBottom,dTop);
	//}

	//��������
	//{
	pVertex		= m_pAllVretexList->GetGridVertex();
	while(pVertex != NULL)
	{
		m_pTinCreator.AddVertex(pVertex->m_dX,pVertex->m_dY,pVertex->m_dZ);

		pVertex	= pVertex->m_pNext;
	}
	//}

	//�����ɹ�
	//{
	m_pTinCreator.Create();
	//}

	//
	m_pTinCreator.CaculateTerrainFactor(m_dMeanSlope,m_dMeanRoughness);

	//��ɾ��
	m_pTriangleList.Release();
	
	//������
	CDCR_TinTriangle	*pTriangle;
	pTriangle	= m_pTinCreator.GetTriangle();
	while(pTriangle != NULL)
	{
		m_pTriangleList.Add(
							pTriangle->m_nVertexID[0],pTriangle->m_dX[0],pTriangle->m_dY[0],
							pTriangle->m_nVertexID[1],pTriangle->m_dX[1],pTriangle->m_dY[1],
							pTriangle->m_nVertexID[2],pTriangle->m_dX[2],pTriangle->m_dY[2]
							);
	
		pTriangle	= pTriangle->m_pNext;
	}

	return	true;
}

//�������
bool CDCR_LocalTerrainCharacter::CalculatePointPattern()
{
	if(m_pVretexList->GetGridVertexCount() > 1)
	{
		m_pPointPatterNN.SetGridVertexList(m_pVretexList);

		//����ֵ
		m_dPointPatternValue	= m_pPointPatterNN.JudgePattern(PI * m_dRadius * m_dRadius);

		//����
		m_dCentroidValue		= m_pPointPatterNN.CalcCentroid(m_dCenterX,m_dCenterY) / m_dRadius;
	}

	return	true;
}

//
void CDCR_LocalTerrainCharacter::OnSave(FILE *fp)
{
	if(fp != NULL)
	{
		//����	����ֵ	ƽ���¶�	�ֲڶ�	������	��С����

		fprintf(fp,"%lf\t %lf\t %lf\t %lf\t %lf\t %lf\t",m_dPointPatternValue,m_dCentroidValue,m_dMeanSlope,m_dMeanRoughness,m_dRadius,m_dMinDistance);
	}
}

//
double CDCR_LocalTerrainCharacter::GetPointPatternValue()
{
	return	m_dPointPatternValue;
}

void CDCR_LocalTerrainCharacter::SetPointPatternValue(double dValue)
{
	m_dPointPatternValue	= dValue;
}

//
double CDCR_LocalTerrainCharacter::GetCentroidValue()
{
	return	m_dCentroidValue;
}

void CDCR_LocalTerrainCharacter::SetCentroidValue(double dValue)
{
	m_dCentroidValue	= dValue;	
}

//
double CDCR_LocalTerrainCharacter::GetMeanSlope()
{
	return	m_dMeanSlope;
}
void CDCR_LocalTerrainCharacter::SetMeanSlope(double dValue)
{
	m_dMeanSlope	= dValue;
}

//
double CDCR_LocalTerrainCharacter::GetMeanRoughness()
{
	return	m_dMeanRoughness;
}

void CDCR_LocalTerrainCharacter::SetMeanRoughness(double dValue)
{
	m_dMeanRoughness	= dValue;
}

//
CDCR_TinTriangleList* CDCR_LocalTerrainCharacter::GetTriangleList()
{
	return	&m_pTriangleList;
}

//
double CDCR_LocalTerrainCharacter::GetMaxDistance()
{
	return	m_dRadius;
}

//
double CDCR_LocalTerrainCharacter::GetMinDistance()
{
	return	m_dMinDistance;
}
