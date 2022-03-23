// DCR_DimensionAnalyse.h: interface for the CDCR_DimensionAnalyse class.
// ���ٷ���
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCR_DIMENSIONANALYSE_H__FFFB053E_DBD7_45E2_B222_EDA535E4A385__INCLUDED_)
#define AFX_DCR_DIMENSIONANALYSE_H__FFFB053E_DBD7_45E2_B222_EDA535E4A385__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDCR_DimensionAnalyse  
{
public:
	CDCR_DimensionAnalyse();
	virtual ~CDCR_DimensionAnalyse();
public:
	//���ٷ���
	bool	OnExecute(int iMode);
	//
	bool	OnRead(CString szPathName, int iFieldNum,char *chDelims);
protected:
	//��¼��
	int		m_iRecordNum;

	//�ֶ���
	int		m_iFieldNum;
	
	//�ֶ�����
	CString	*m_szFieldName;

	//����
	double	*m_dValue;

	//
	double	*m_dMaxValue;
	double	*m_dMinValue;
	
	//
	CString	m_szPathName;
	//
	bool	m_bFileOpened;
};

#endif // !defined(AFX_DCR_DIMENSIONANALYSE_H__FFFB053E_DBD7_45E2_B222_EDA535E4A385__INCLUDED_)
