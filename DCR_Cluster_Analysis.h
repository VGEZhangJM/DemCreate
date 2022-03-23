// DCR_Cluster_Analysis.h: interface for the CDCR_Cluster_Analysis class.
// �ռ�������
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCR_CLUSTER_ANALYSIS_H__1CCAEE5D_CF69_4F69_AA3B_FF73E4298A36__INCLUDED_)
#define AFX_DCR_CLUSTER_ANALYSIS_H__1CCAEE5D_CF69_4F69_AA3B_FF73E4298A36__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDCR_Cluster_Analysis  
{
public:
	CDCR_Cluster_Analysis();
	virtual ~CDCR_Cluster_Analysis();

public:
	//
	void	OnRelease();
	//
	void	GetMapExt(double& dLeft,double& dRight,double& dBottom,double& dTop);

	//����TXT�ı�����
	bool	OnImport(CString szPathName);
	
	//
	bool	IsEmpty();

	//
	bool	OnExecute(CDC *pDC, int Method, int nClusters);

	//
	bool	Minimum_Distance(CDC *pDC, bool bInitialize);

	//
	void	SetClientRect(CRect rRect);

	//
	void	SetCoordMapping(double dXA,double dXB,double dYA,double dYB);

	//
	void	Draw(CDC *pDC);

	//
	COLORREF	GetColor(int iCluster);

	void	Draw(CDC *pDC,int iElement);
	
protected:
	double	Get_Square(double x)
	{
		return( x * x );
	};

protected:
	//
	CString	m_szPathName;

	//�������Ǳ�����ڵ�
	double	*m_dFeature;
	int		m_nFeatures;
		
	//��ɢ����
	int		m_iPointNum;

	//ÿһ����ľ�����ţ���ʼ��Ϊ0
	int		*m_Cluster;
	//����Ŀ��һ��Ϊ10
	int		m_nClusters;
		
	//
	int		*m_nMembers;

	//����ֵ
	double	*m_Variance;
	//��������,һ��Ϊ3 �� N
	double	*m_Centroid;
	//��С����ֵ
	double	m_SP;
	//
	int		m_Iteration;

	//
	double	m_dLeft,m_dRight;
	double	m_dBottom,m_dTop;

	//
	CRect	m_rRect;

	double	m_dMapXA,m_dMapXB,m_dMapYA,m_dMapYB;

	//
	COLORREF	m_crMaxColor;
	COLORREF	m_crMinColor;
};

#endif // !defined(AFX_DCR_CLUSTER_ANALYSIS_H__1CCAEE5D_CF69_4F69_AA3B_FF73E4298A36__INCLUDED_)
