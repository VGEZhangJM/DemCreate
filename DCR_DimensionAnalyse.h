// DCR_DimensionAnalyse.h: interface for the CDCR_DimensionAnalyse class.
// 量纲分析
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
	//量纲分析
	bool	OnExecute(int iMode);
	//
	bool	OnRead(CString szPathName, int iFieldNum,char *chDelims);
protected:
	//记录数
	int		m_iRecordNum;

	//字段数
	int		m_iFieldNum;
	
	//字段名称
	CString	*m_szFieldName;

	//内容
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
