// DCR_Param_SPD.h: interface for the CDCR_Param_SPD class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCR_PARAM_SPD_H__1FB0ED5D_8EFF_49D2_90E5_6F55F6967174__INCLUDED_)
#define AFX_DCR_PARAM_SPD_H__1FB0ED5D_8EFF_49D2_90E5_6F55F6967174__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDCR_Param_SPD  
{
public:
	CDCR_Param_SPD();
	virtual ~CDCR_Param_SPD();

	//
	bool	Read(FILE *fp);

public:
	//DEM������Ŀ
	int		m_iDemNumX;
	//
	int		m_iDemNumY;
	
	//������ʽ
	//0:Ϊȫ�����������޷�������
	//1:Ϊ�ķ�������
	//2:Ϊ�˷�������
	int		m_iSearchType;
	
	//
	int		m_iPointNumPreDirection;
	
	//
	int		m_iPointNumInTotal;

	//�����뾶
	double	m_dSearchRadius;

	int		m_iNumOfU;
	//Ȩָ��
	int		m_iU[5];

	//�⻬����
	double	m_dT;

	//
	int		m_iRadiusType;
	//
	double	m_dMaxRadius;

	//
	int		m_iKernalType;
};

#endif // !defined(AFX_DCR_PARAM_SPD_H__1FB0ED5D_8EFF_49D2_90E5_6F55F6967174__INCLUDED_)
