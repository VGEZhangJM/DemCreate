// DCR_Statistics.h: interface for the CDCR_Statistics class.
// ͳ�Ʊ�����
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCR_STATISTICS_H__0FAE69F2_39EA_4823_9A69_C1A6F4AC7E34__INCLUDED_)
#define AFX_DCR_STATISTICS_H__0FAE69F2_39EA_4823_9A69_C1A6F4AC7E34__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDCR_Statistics  
{
public:
	CDCR_Statistics();
	virtual ~CDCR_Statistics();
	
	//
	void	Save(CString szPathName);
	void	Save(FILE *fp);
	void	SaveEx(FILE *fp);
	void	SaveExInTile(FILE *fp);

	//����ͳ������
	void	SetStatisticsData(int iCount,double *pData);

	//���и�������ͳ��
	bool	Statistics();
	
	//��ȡ������Ŀ
	void	GetDataCount(int& iCount);

	//��ȡ���ݵļ�ֵ
	void	GetDataMinMax(double& dMin,double& dMax);
		
	//��ȡ���ݵľ�ֵ
	void	GetDataMean(double& dMean);

	//��ȡ���ݵķ���
	void	GetDataVar(double& dVar);

	//��ȡ���ݵı�׼��
	void	GetDataStandardVar(double& dStandardVar);

	//��ȡ���ݵı���ϵ��
	void	GetDataCV(double& dCV);

	//��ȡ���ݵ�ƫ��ϵ��
	void	GetDataSkewness(double& dSkewness);

	//��ȡ���ݵķ��ϵ��
	void	GetDataKurtosis(double& dKurtosis);

	//��ȡ���ݵ������
	void	GetDataRMSE(double& dRMSE);

protected:
	//��������
	int		m_iCount;
	//��������
	double	*m_pData;

	//ͳ�����ݵļ�ֵ
	double	m_dDataMin,m_dDataMax;

	//ͳ�����ݵľ�ֵ
	double	m_dDataMean;

	//ͳ�����ݵķ���,����ƫ��
	double	m_dDataVar;

	//ͳ�����ݵı�׼�����ƫ��
	double	m_dStandardVar;

	//����ϵ��
	double	m_dDataCV;

	//ƫ��ϵ��
	double	m_dDataSkewness;
	//���ϵ��
	double	m_dDataKurtosis;

	//�����
	double	m_dDataRMSE;
};

#endif // !defined(AFX_DCR_STATISTICS_H__0FAE69F2_39EA_4823_9A69_C1A6F4AC7E34__INCLUDED_)
