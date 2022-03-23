// DCR_TinTriangle.cpp: implementation of the CDCR_TinTriangle class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DCR_TinTriangle.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDCR_TinTriangle::CDCR_TinTriangle()
{
	m_nID		= -1;

	for(int i=0;i<3;i++)
	{
		m_nVertexID[i]	= -1;
		//�����ߣ����Ǵ���˳���
		m_bEdgeUsed[i]	= false;

		//���������
		m_dX[i]			= 0.0;
		m_dY[i]			= 0.0;
		m_dZ[i]			= 0.0;
	}

	//�����εķ�Χ
	m_dMinX	= m_dMaxX	= 0.0;
	m_dMinY	= m_dMaxY	= 0.0;

	//
	m_bUsed		= false;
	//
	m_bDelete	= false;
	//
	m_pNext		= NULL;
}

//
CDCR_TinTriangle::~CDCR_TinTriangle()
{

}

//////////////////////////////////////////////////////////////////////
// �Ƿ�Ϊ��û�д���Ķ����?
// �жϷ����ǣ�m_bUsed�д���false����Ϊû�б�������ġ�
//////////////////////////////////////////////////////////////////////
bool CDCR_TinTriangle::IsUsed()
{
	if(m_bEdgeUsed[0] == false)		return	true;
	if(m_bEdgeUsed[1] == false)		return	true;
	if(m_bEdgeUsed[2] == false)		return	true;

	return	false;
}

//////////////////////////////////////////////////////////////////////
// ��������
//////////////////////////////////////////////////////////////////////
void CDCR_TinTriangle::SetVertexCoord(int iVertexID1, double dX1, double dY1, double dZ1, 
									  int iVertexID2, double dX2, double dY2, double dZ2, 
									  int iVertexID3, double dX3, double dY3, double dZ3)
{
	for(int i=0;i<3;i++)
	{
		if(m_nVertexID[i] == iVertexID1)	
		{
			m_dX[i]	= dX1;	m_dY[i]	= dY1;	m_dZ[i]	= dZ1;
		}

		if(m_nVertexID[i] == iVertexID2)	
		{
			m_dX[i]	= dX2;	m_dY[i]	= dY2;	m_dZ[i]	= dZ2;
		}

		if(m_nVertexID[i] == iVertexID3)	
		{
			m_dX[i]	= dX3;	m_dY[i]	= dY3;	m_dZ[i]	= dZ3;
		}
	}

	//��һ��
	m_dMinX	= m_dMaxX	= dX1;
	m_dMinY	= m_dMaxY	= dY1;

	//�ڶ��㡢������
	if(m_dMinX > dX2)	m_dMinX	= dX2;
	if(m_dMaxX < dX2)	m_dMaxX	= dX2;
	if(m_dMinY > dY2)	m_dMinY	= dY2;
	if(m_dMaxY < dY2)	m_dMaxY	= dY2;

	if(m_dMinX > dX3)	m_dMinX	= dX3;
	if(m_dMaxX < dX3)	m_dMaxX	= dX3;
	if(m_dMinY > dY3)	m_dMinY	= dY3;
	if(m_dMaxY < dY3)	m_dMaxY	= dY3;
}

//////////////////////////////////////////////////////////////////////
// ��������
//////////////////////////////////////////////////////////////////////
bool CDCR_TinTriangle::GetVertexCoord(int iVertexID1, double& dX1, double& dY1, double& dZ1,
									  int iVertexID2, double& dX2, double& dY2, double& dZ2,
									  int& iVertexID3, double& dX3, double& dY3, double& dZ3
									  )
{
	int	iItem;
	iItem	= 0;

	for(int i=0;i<3;i++)
	{
		if(m_nVertexID[i] == iVertexID1)
		{
			dX1	= m_dX[i];	dY1	= m_dY[i];	dZ1	= m_dZ[i];

			iItem++;
		}

		if(m_nVertexID[i] == iVertexID2)
		{
			dX2	= m_dX[i];	dY2	= m_dY[i];	dZ2	= m_dZ[i];
			iItem++;
		}

		if(m_nVertexID[i]!=iVertexID1 && m_nVertexID[i]!=iVertexID2)
		{
			iVertexID3	= m_nVertexID[i];
			dX3	= m_dX[i];	dY3	= m_dY[i];	dZ3	= m_dZ[i];
		}
	}

	if(iItem == 2)	return	true;
	return	false;
}

bool CDCR_TinTriangle::GetVertexCoordEx(int& iVertexID1, double& dX1, double& dY1, double& dZ1,
										int& iVertexID2, double& dX2, double& dY2, double& dZ2,
										int& iVertexID3, double& dX3, double& dY3, double& dZ3 
										)
{
	iVertexID1	= m_nVertexID[0];
	dX1			= m_dX[0];	dY1			= m_dY[0];		dZ1			= m_dZ[0];

	iVertexID2	= m_nVertexID[1];
	dX2			= m_dX[1];	dY2			= m_dY[1];		dZ2			= m_dZ[1];

	iVertexID3	= m_nVertexID[2];
	dX3			= m_dX[2];	dY3			= m_dY[2];		dZ3			= m_dZ[2];

	return	true;
}

//////////////////////////////////////////////////////////////////////
//�����ʽ
//////////////////////////////////////////////////////////////////////
double CDCR_TinTriangle::GetArea(double dX1, double dY1, double dX2, double dY2, double dX3, double dY3)
{
	//m_dArea	= ((x2-x1)*(y3-y1)-(x3-x1)*(y2-y1))/2;
	return	((dX2-dX1)*(dY3-dY1)-(dX3-dX1)*(dY2-dY1));
}

//////////////////////////////////////////////////////////////////////
//�жϵ��Ƿ�����������
//////////////////////////////////////////////////////////////////////
bool CDCR_TinTriangle::IsInTriangle(double dX,double dY)
{
	//�����жϵ��Ƿ�����Ӿ�����
	if(dX<m_dMinX || dX>m_dMaxX || dY<m_dMinY || dY>m_dMaxY)
		return	false;

	//����Ӿ����У������ж��Ƿ�����������
	double	dABCValue;
	dABCValue	= GetArea(m_dX[0],m_dY[0],m_dX[1],m_dY[1],m_dX[2],m_dY[2]);

	double	dABP,dBCP,dACP;
	dABP		= GetArea(dX,dY,m_dX[0],m_dY[0],m_dX[1],m_dY[1]);
	dBCP		= GetArea(dX,dY,m_dX[1],m_dY[1],m_dX[2],m_dY[2]);
	dACP		= GetArea(dX,dY,m_dX[2],m_dY[2],m_dX[0],m_dY[0]);
	
	double	dAllValue;
	dAllValue	= fabs(dABP)+fabs(dBCP)+fabs(dACP);

	dABCValue	= fabs(dABCValue);
		
	//��������ε�����㹻С���������ж�ʱ��������Ӧ���ʵ��ķŴ�
	if(dABCValue < 1e-5)
	{
		dAllValue	= dAllValue*100000;
		dABCValue	= dABCValue*100000;
	}

	if(fabs(dAllValue-dABCValue) < 1e-10)
		return	true;
	
	return	false;
}

//////////////////////////////////////////////////////////////////////
// 02 ��������������
// �жϵ�ֱ�ߵ���һ����
// -1:��ʾ��ֱ
// 1:��ʾ���ⷽ��
//////////////////////////////////////////////////////////////////////
int CDCR_TinTriangle::CreateJudgeFunction(double dX1, double dY1, double dX2, double dY2, double &a, double &b)
{
	if(fabs(dX2-dX1) < 1e-10)	//x2==x1
	{
		a	= dX1;	
		b	= 0;
		return	-1;
	}
	else
	if(fabs(dY2-dY1) < 1e-10)	//y2=y1
	{
		a	= 0;
		b	= dY1;
		return	1;
	}
	else
	{
		a	= (dY2-dY1) / (dX2-dX1);
		b	= (dX1*dY2-dX2*dY1) / (dX2-dX1);

		return	0;
	}
}

//////////////////////////////////////////////////////////////////////
// 03 ��������������
// �жϽ��
//////////////////////////////////////////////////////////////////////
double CDCR_TinTriangle::GetJudgeValue(double dX, double dY, int bSign, double a, double b)
{
	double	dValue;

	if(bSign == -1)				//��ֱ
	{
		dValue	= dX-a;			//
	}
	else	
	if(bSign == 1)				//ˮƽ
	{
		dValue	= dY-b;			
	}
	else
	{
		dValue	= dY-(a*dX-b);	//
	}
	
	return	dValue;
}

//
void CDCR_TinTriangle::UpdateEdge(int iVertexID1,int iVertexID2)
{
	if(((m_nVertexID[0]==iVertexID1)&&(m_nVertexID[1]==iVertexID2)) || ((m_nVertexID[0]==iVertexID2)&&(m_nVertexID[1]==iVertexID1)))
		m_bEdgeUsed[0]	= true;

	if(((m_nVertexID[0]==iVertexID1)&&(m_nVertexID[2]==iVertexID2)) || ((m_nVertexID[0]==iVertexID2)&&(m_nVertexID[2]==iVertexID1)))
		m_bEdgeUsed[1]	= true;

	if(((m_nVertexID[2]==iVertexID1)&&(m_nVertexID[1]==iVertexID2)) || ((m_nVertexID[2]==iVertexID2)&&(m_nVertexID[1]==iVertexID1)))
		m_bEdgeUsed[2]	= true;
}

