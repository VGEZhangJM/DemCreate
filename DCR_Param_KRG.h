// DCR_Param_KRG.h: interface for the CDCR_Param_KRG class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCR_PARAM_KRG_H__36CE7B33_8F14_4587_8F39_F2B2F2A1FD05__INCLUDED_)
#define AFX_DCR_PARAM_KRG_H__36CE7B33_8F14_4587_8F39_F2B2F2A1FD05__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDCR_Param_KRG  
{
public:
	CDCR_Param_KRG();
	virtual ~CDCR_Param_KRG();

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

	//ģ������,
	//0������ģ��
	//1��ָ��ģ��
	//2����˹ģ��
	int		m_iModelType;

	//���
	double	m_dC0;
	//��ֵ̨
	double	m_dC;
	//���
	double	m_dAlfa;
};

#endif // !defined(AFX_DCR_PARAM_KRG_H__36CE7B33_8F14_4587_8F39_F2B2F2A1FD05__INCLUDED_)
