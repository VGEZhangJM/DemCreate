// DCR_DimensionAnalyse.cpp: implementation of the CDCR_DimensionAnalyse class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "vr_demcreator.h"
#include "DCR_DimensionAnalyse.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDCR_DimensionAnalyse::CDCR_DimensionAnalyse()
{
	//记录数
	m_iRecordNum	= 0;

	//字段数
	m_iFieldNum		= 0;
	
	//字段名称
	m_szFieldName	= NULL;
	m_dMaxValue		= NULL;
	m_dMinValue		= NULL;

	//内容
	m_dValue		= NULL;

	m_bFileOpened	= false;
}

CDCR_DimensionAnalyse::~CDCR_DimensionAnalyse()
{
	if(m_szFieldName != NULL)	{	delete	[]m_szFieldName;	m_szFieldName	= NULL;	}
	if(m_dMaxValue != NULL)		{	delete	[]m_dMaxValue;		m_dMaxValue		= NULL;	}
	if(m_dMinValue != NULL)		{	delete	[]m_dMinValue;		m_dMinValue		= NULL;	}
	if(m_dValue != NULL)		{	delete	[]m_dValue;			m_dValue		= NULL;	}
}

//
bool CDCR_DimensionAnalyse::OnRead(CString szPathName, int iFieldNum,char *chDelims)
{
	//
	m_iFieldNum		= iFieldNum;
	m_bFileOpened	= false;
	m_szPathName	= szPathName;
	//读取记录数目
	{
		m_iRecordNum	= 0;
		//
		CStdioFile		fTxt;
		CFileException	fe;
		
		//
		if( !fTxt.Open(szPathName, CFile::modeRead|CFile::shareDenyWrite|CFile::typeText,&fe))
			return false;

		CString	szInfo;
		while(fTxt.ReadString(szInfo))
		{
			m_iRecordNum++;
		}

		m_iRecordNum--;

		fTxt.Close();
	}

	//
	{
		CStdioFile		fTxt;
		CFileException	fe;
		
		//
		if( !fTxt.Open(szPathName, CFile::modeRead|CFile::shareDenyWrite|CFile::typeText,&fe))
			return false;
		
		//读取字段信息
		{
			CString	szInfo;
			fTxt.ReadString(szInfo);
			char	chInfo[2048];
			strcpy(chInfo,szInfo);

			//
			m_szFieldName	= new	CString[iFieldNum];

			m_dMaxValue		= new	double[iFieldNum];
			m_dMinValue		= new	double[iFieldNum];

			char	*result	= NULL;
			result	= strtok(chInfo,chDelims);

			for(int i=0;i<iFieldNum;i++)
			{
				m_szFieldName[i]	= result;

				result	= strtok(NULL,chDelims);
			}
		}

		//读取具体内容
		{
			int			I;

			I			= 0;
			m_dValue	= new	double[m_iRecordNum * m_iFieldNum];
			CString	szInfo;
			while(fTxt.ReadString(szInfo))
			{
				char	chInfo[2048];
				strcpy(chInfo,szInfo);

				char	*result	= NULL;
				result	= strtok(chInfo,chDelims);

				for(int i=0;i<iFieldNum;i++)
				{
					m_dValue[I*iFieldNum + i]	= atof(result);

					result	= strtok(NULL,chDelims);
				}

				I++;
			}
		}
		
		fTxt.Close();
		
		m_bFileOpened	= true;

		return	true;
	}	
}

//量纲分析
bool CDCR_DimensionAnalyse::OnExecute(int iMode)
{
	if(m_bFileOpened)
	{
		//
		double	*dTemp;
		dTemp	= new	double[m_iRecordNum];

		for(int i=0;i<m_iFieldNum;i++)
		{
			for(int j=0;j<m_iRecordNum;j++)
			{
				dTemp[j]	= m_dValue[j*m_iFieldNum + i];
			}

			//中性化变换
			if(iMode == 0)
			{
				double	dMean;

				dMean	= 0;
				for(int k=0;k<m_iRecordNum;k++)
				{
					dMean	= dMean + dTemp[k];
				}
				dMean	= dMean/m_iRecordNum;
				
				for(k=0;k<m_iRecordNum;k++)
				{
					m_dValue[k*m_iFieldNum + i]		= m_dValue[k*m_iFieldNum + i] - dMean;
				}
			}

			//极差规格化变换
			if(iMode == 1)
			{
				double dMax,dMin;
				dMax	= dMin	= 0;
				for(int k=0;k<m_iRecordNum;k++)
				{
					if(k==0)	{	dMin	= dMax	= dTemp[k];	}
					else
					{
						if(dMin > dTemp[k])	dMin	= dTemp[k];
						if(dMax < dTemp[k])	dMax	= dTemp[k];
					}
				}
				
				if(dMax-dMin != 0)
				{
					for(int k=0;k<m_iRecordNum;k++)
					{
						m_dValue[k*m_iFieldNum + i]	= (dTemp[k]-dMin)/(dMax-dMin);
					}
				}
				
				//
				m_dMaxValue[i]	= dMax;
				m_dMinValue[i]	= dMin;
			}
		}
		
		delete	[]dTemp;

		//写入
		{
			CString	szPathName;
			szPathName	= m_szPathName.Left(m_szPathName.GetLength()-4) + "_Normail.ltf";
			
			CStdioFile	fTxt;
			CFileException	fe;
			//
			if( !fTxt.Open(szPathName, CFile::modeCreate|CFile::modeWrite|CFile::typeText,&fe))
				return false;
			
			CString	szInfo;
			
			//文件头
			{
				for(int i=0;i<m_iFieldNum-1;i++)
				{
					CString	szTemp;
					szTemp.Format("%s\t",m_szFieldName[i]);

					szInfo	= szInfo + szTemp;
				}
				szInfo	= szInfo + m_szFieldName[m_iFieldNum-1] + "\n";
				fTxt.WriteString(szInfo);
			}
		
			for(int j=0;j<m_iRecordNum;j++)
			{
				szInfo.Empty();
				CString	szTemp;
				for(int i=0;i<m_iFieldNum-1;i++)
				{
					szTemp.Format("%.4lf\t",m_dValue[j*m_iFieldNum + i]);

					szInfo	= szInfo + szTemp;
				}

				szTemp.Format("%.4lf\n",m_dValue[j*m_iFieldNum + (m_iFieldNum-1)]);
				szInfo	= szInfo + szTemp;
				
				fTxt.WriteString(szInfo);
			}
			
			fTxt.Close();
		}

		//
		{
			CString	szPathName;
			szPathName	= m_szPathName.Left(m_szPathName.GetLength()-4) + "_Range.txt";
			
			CStdioFile	fTxt;
			CFileException	fe;
			//
			if( !fTxt.Open(szPathName, CFile::modeCreate|CFile::modeWrite|CFile::typeText,&fe))
				return false;
			
			CString	szInfo;
			
			//文件头
			{
				for(int i=0;i<m_iFieldNum-1;i++)
				{
					CString	szTemp;
					szTemp.Format("%s\t",m_szFieldName[i]);

					szInfo	= szInfo + szTemp;
				}
				szInfo	= szInfo + m_szFieldName[m_iFieldNum-1] + "\n";
				fTxt.WriteString(szInfo);
			}

			//最小值
			{
				szInfo.Empty();
				CString	szTemp;

				for(int i=0;i<m_iFieldNum-1;i++)
				{
					CString	szTemp;
					szTemp.Format("%.4lf\t",m_dMinValue[i]);

					szInfo	= szInfo + szTemp;
				}
				szTemp.Format("%.4lf\n",m_dMinValue[m_iFieldNum-1]);
				szInfo	= szInfo + szTemp;

				fTxt.WriteString(szInfo);
			}

			//最大值
			{
				szInfo.Empty();
				CString	szTemp;

				for(int i=0;i<m_iFieldNum-1;i++)
				{
					CString	szTemp;
					szTemp.Format("%.4lf\t",m_dMaxValue[i]);

					szInfo	= szInfo + szTemp;
				}
				szTemp.Format("%.4lf\n",m_dMaxValue[m_iFieldNum-1]);
				szInfo	= szInfo + szTemp;

				fTxt.WriteString(szInfo);
			}

			
			fTxt.Close();
		}
		
		return	true;
	}

	return	false;
}
