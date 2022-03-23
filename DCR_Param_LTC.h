// DCR_Param_LTC.h: interface for the CDCR_Param_LTC class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCR_PARAM_LTC_H__E9A3EAAF_433F_456D_B06C_385377EC63C6__INCLUDED_)
#define AFX_DCR_PARAM_LTC_H__E9A3EAAF_433F_456D_B06C_385377EC63C6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDCR_Param_LTC  
{
public:
	CDCR_Param_LTC();
	virtual ~CDCR_Param_LTC();

public:
	//
	bool	Read(FILE *fp);

public:
	//������ʽ
	//0:Ϊȫ�����������޷�������
	//1:Ϊ�ķ�������
	//2:Ϊ�˷�������
	int		m_iSearchType;
	
	//
	int		m_iPointNumPreDirection;
	
	//
	int		m_iPointNumInTotal;

	//
	double	m_dSearchRadius;
};

#endif // !defined(AFX_DCR_PARAM_LTC_H__E9A3EAAF_433F_456D_B06C_385377EC63C6__INCLUDED_)
