// DCR_Param_RBF.h: interface for the CDCR_Param_RBF class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCR_PARAM_RBF_H__F1286749_5651_4782_8D61_68A397AE595E__INCLUDED_)
#define AFX_DCR_PARAM_RBF_H__F1286749_5651_4782_8D61_68A397AE595E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDCR_Param_RBF  
{
public:
	CDCR_Param_RBF();
	virtual ~CDCR_Param_RBF();
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
	//1�����������ֵ��
	//2�������������ֵ��
	int		m_iModelType;

	//�����ĸ���
	int		m_iNumOfC;

	//��������20
	double	m_dC[20];
};

#endif // !defined(AFX_DCR_PARAM_RBF_H__F1286749_5651_4782_8D61_68A397AE595E__INCLUDED_)
