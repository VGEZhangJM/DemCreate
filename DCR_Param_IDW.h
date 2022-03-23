// DCR_Param_IDW.h: interface for the CDCR_Param_IDW class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCR_PARAM_IDW_H__252232E9_B496_4689_BFC6_492D455B5F98__INCLUDED_)
#define AFX_DCR_PARAM_IDW_H__252232E9_B496_4689_BFC6_492D455B5F98__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDCR_Param_IDW  
{
public:
	CDCR_Param_IDW();
	virtual ~CDCR_Param_IDW();

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
	
	//
	int		m_iNumOfU;
	//Ȩָ��
	int		m_iU[5];
	
	//ƽ������
	double	m_dT;
};

#endif // !defined(AFX_DCR_PARAM_IDW_H__252232E9_B496_4689_BFC6_492D455B5F98__INCLUDED_)
