// DCR_Statistics.h: interface for the CDCR_Statistics class.
// 统计变量类
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

	//输入统计数据
	void	SetStatisticsData(int iCount,double *pData);

	//进行各种量的统计
	bool	Statistics();
	
	//获取数据数目
	void	GetDataCount(int& iCount);

	//获取数据的极值
	void	GetDataMinMax(double& dMin,double& dMax);
		
	//获取数据的均值
	void	GetDataMean(double& dMean);

	//获取数据的方差
	void	GetDataVar(double& dVar);

	//获取数据的标准差
	void	GetDataStandardVar(double& dStandardVar);

	//获取数据的变异系数
	void	GetDataCV(double& dCV);

	//获取数据的偏度系数
	void	GetDataSkewness(double& dSkewness);

	//获取数据的峰度系数
	void	GetDataKurtosis(double& dKurtosis);

	//获取数据的中误差
	void	GetDataRMSE(double& dRMSE);

protected:
	//数量长度
	int		m_iCount;
	//数据内容
	double	*m_pData;

	//统计数据的极值
	double	m_dDataMin,m_dDataMax;

	//统计数据的均值
	double	m_dDataMean;

	//统计数据的方差,是无偏的
	double	m_dDataVar;

	//统计数据的标准差，是无偏的
	double	m_dStandardVar;

	//变异系数
	double	m_dDataCV;

	//偏度系数
	double	m_dDataSkewness;
	//峰度系数
	double	m_dDataKurtosis;

	//中误差
	double	m_dDataRMSE;
};

#endif // !defined(AFX_DCR_STATISTICS_H__0FAE69F2_39EA_4823_9A69_C1A6F4AC7E34__INCLUDED_)
