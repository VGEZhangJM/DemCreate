// DCR_GeoMorphological_LSD.cpp: implementation of the CDCR_GeoMorphological_LSD class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "vr_demcreator.h"
#include "DCR_GeoMorphological_LSD.h"

#include "DCR_CurvatureSet.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDCR_GeoMorphological_LSD::CDCR_GeoMorphological_LSD()
{
	m_pDemObj		= NULL;

	//11*11
	m_iWindowSize	= 5;

	//
	m_pTypeObj		= NULL;	
}

CDCR_GeoMorphological_LSD::~CDCR_GeoMorphological_LSD()
{
	Release();
}


//
void CDCR_GeoMorphological_LSD::SetGridDem(double *pData,int iNumOfX,int iNumOfY,double dSizeOfX,double dSizeOfY,double dRealSize,double dLeft,double dRight,double dBottom,double dTop,CString szPathName)
{
	//
	m_pDemObj		= pData;

	//
	m_iNumOfX		= iNumOfX;
	m_iNumOfY		= iNumOfY;

	//
	m_dDemLeft		= dLeft;
	m_dDemRight		= dRight;
	m_dDemBottom	= dBottom;
	m_dDemTop		= dTop;

	//
	m_dSizeOfX		= dSizeOfX;
	m_dSizeOfY		= dSizeOfY;

	//
	m_dRealSize		= dRealSize;

	//
	m_szPathName	= szPathName;	
}

//
bool CDCR_GeoMorphological_LSD::Judge()
{
	//
	if(m_pDemObj == NULL)	return	false;
	
	//{
		//首先按照最佳分析区域计算坡度信息
		pSlope.SetGridDem(m_pDemObj,m_iNumOfX,m_iNumOfY,m_dSizeOfX,m_dSizeOfY,m_dRealSize,
							m_dDemLeft,m_dDemRight,m_dDemBottom,m_dDemTop,m_szPathName);
		
		//计算坡度
		pSlope.OnCalculate(0);
		
		//获取坡度信息
		double	*dSlope;
		dSlope		= pSlope.GetSlope();
		if(dSlope == NULL)		return	false;
	//}
	
	//创建对象
	//
	if(m_pTypeObj != NULL)	{	delete	[]m_pTypeObj;	m_pTypeObj	= NULL;		}
	//
	m_pTypeObj	= new	double[m_iNumOfX*m_iNumOfY];
	
	//
	if(m_pTypeObj == NULL)
	{
		CString	szInfo;
		szInfo.Format("在判断地貌类型时，当尺寸为 %ld 时不能创建内存!\n程序暂停，即将退出!\n",m_iWindowSize);
		MessageBox(NULL,szInfo,"提示信息",MB_OK);

		return	false;
	}
	
	double	*pPingYuan;
	double	*pQiuLing;
	double	*pShanDi;

	pPingYuan	= new	double[m_iNumOfX*m_iNumOfY];
	pQiuLing	= new	double[m_iNumOfX*m_iNumOfY];
	pShanDi		= new	double[m_iNumOfX*m_iNumOfY];

	//初始化
	int		i,j,ci;
	for(j=0;j<m_iNumOfY;j++)
	{
		for(i=0;i<m_iNumOfX;i++)
		{	
			ci				= j*m_iNumOfX+i;
			m_pTypeObj[ci]	= INVALID_HEIGHT;

			pPingYuan[ci]	= INVALID_VALUE;
			pQiuLing[ci]	= INVALID_VALUE;
			pShanDi[ci]		= INVALID_VALUE;
		}
	}

	//开始循环计算每一点所属的地貌类型隶属度值
	for(j=0;j<m_iNumOfY;j++)
	{
		for(i=0;i<m_iNumOfX;i++)
		{	
			ci			= j*m_iNumOfX+i;
			
			//假如高程是非法值，则退出
			if(m_pDemObj[ci] == INVALID_HEIGHT)	
			{
				m_pTypeObj[ci]	= 0;

				pPingYuan[ci]	= INVALID_VALUE;
				pQiuLing[ci]	= INVALID_VALUE;
				pShanDi[ci]		= INVALID_VALUE;

				continue;
			}

			//对于每一点来说都是需要计算的,即确定区域
			int	iBegin,iEnd;
			int	jBegin,jEnd;

			iBegin	= i - m_iWindowSize;
			iEnd	= i + m_iWindowSize;

			jBegin	= j - m_iWindowSize;
			jEnd	= j + m_iWindowSize;
			
			//如果越界了，则不参与计算
			if(iBegin < 0 || iEnd>m_iNumOfX-1 || jBegin<0 || jEnd>m_iNumOfY-1)
			{
				m_pTypeObj[ci]	= 0;

				pPingYuan[ci]	= INVALID_VALUE;
				pQiuLing[ci]	= INVALID_VALUE;
				pShanDi[ci]		= INVALID_VALUE;
			}
			else
			{		
				int		I,J;

				///////////////////////////////////////////////////////
				//提取平均坡度、提取边缘高差、提取高差
				//{
					double	dMeanSlope	= 0;
					int		iMeanSlope	= 0;

					for(I=iBegin;I<=iEnd;I++)
					{
						for(J=jBegin;J<=jEnd;J++)
						{
							if(dSlope[J*m_iNumOfX+I] == INVALID_VALUE)				continue;

							if( sqrt((I-i)*(I-i) + (J-j)*(J-j)) > m_iWindowSize)	continue;

							dMeanSlope	= dMeanSlope + dSlope[J*m_iNumOfX+I];
							iMeanSlope++;
						}
					}

					if(iMeanSlope == 0)		dMeanSlope	= 0;
					else					dMeanSlope	= dMeanSlope/iMeanSlope;
				//}
				///////////////////////////////////////////////////////

				///////////////////////////////////////////////////////
				//提取高差
				//{
					double	dDeltaHeight;
					double	dDeltaMarginHeight;

					double	dMinHeight	= 0;
					double	dMaxHeight	= 0;

					bool	bFirstValue	= true;
					
					//总的
					for(I=iBegin;I<=iEnd;I++)
					{
						for(J=jBegin;J<=jEnd;J++)
						{
							if(m_pDemObj[J*m_iNumOfX+I] == INVALID_HEIGHT)			continue;

							if( sqrt((I-i)*(I-i) + (J-j)*(J-j)) > m_iWindowSize)	continue;

							if(bFirstValue)
							{
								dMinHeight	= dMaxHeight	= m_pDemObj[J*m_iNumOfX+I];
								bFirstValue	= false;
							}
							else
							{
								if(m_pDemObj[J*m_iNumOfX+I] < dMinHeight)	dMinHeight	= m_pDemObj[J*m_iNumOfX+I];
								if(m_pDemObj[J*m_iNumOfX+I] > dMaxHeight)	dMaxHeight	= m_pDemObj[J*m_iNumOfX+I]; 
							}
						}
					}
					//dDeltaHeight	= dMaxHeight-dMinHeight;

					//边缘高差
					double	dMaxMarginH	= 0;
					double	dMinMarginH	= 0;
					double	dMeanMarginH= 0;
					int		iMeanMarginC= 0;
					bFirstValue		= true;

					for(I=iBegin;I<=iEnd;I++)
					{
						for(J=jBegin;J<=jEnd;J++)
						{
							if(m_pDemObj[J*m_iNumOfX+I] == INVALID_HEIGHT)			continue;

							//
							if( sqrt((I-i)*(I-i) + (J-j)*(J-j)) < m_iWindowSize-2)	continue;
							if( sqrt((I-i)*(I-i) + (J-j)*(J-j)) > m_iWindowSize)	continue;

							if(bFirstValue)
							{
								dMaxMarginH		= dMinMarginH	= m_pDemObj[J*m_iNumOfX+I];

								dMeanMarginH	= m_pDemObj[J*m_iNumOfX+I];
								iMeanMarginC++;
								bFirstValue		= false;
							}
							else
							{
								if(m_pDemObj[J*m_iNumOfX+I] < dMinMarginH)	dMinMarginH	= m_pDemObj[J*m_iNumOfX+I];
								if(m_pDemObj[J*m_iNumOfX+I] > dMaxMarginH)	dMaxMarginH	= m_pDemObj[J*m_iNumOfX+I]; 

								dMeanMarginH	= dMeanMarginH + m_pDemObj[J*m_iNumOfX+I];
								iMeanMarginC++;
							}
						}
					}

					dDeltaHeight		= fabs(dMaxHeight-dMeanMarginH/iMeanMarginC);
					dDeltaMarginHeight	= fabs(dMaxMarginH-dMeanMarginH/iMeanMarginC);
				//}
				///////////////////////////////////////////////////////
				
				//已经提取参数信息
				
				//台地隶属度
				double	dTaiDi;
				{
					double	dTaiDi1,dTaiDi2,dTaiDi3;
					
					//
					dTaiDi1	= 1./( 1 + 0.004 * pow(SH(10*dMeanSlope/180*PI),4) );
					//
					dTaiDi2	= 1./( 1 + pow(1.3 * TH(dDeltaHeight/200),10) );
					//
					dTaiDi3	= 1./( 1 + pow(0.4 * log(dDeltaMarginHeight + 1),10) );

					//dTaiDi	= pow( dTaiDi1*dTaiDi1*dTaiDi2*dTaiDi3 , 0.25);
					dTaiDi		= 0;
				}

				//平原隶属度(这是最简单的)
				double	dPingYuan;
				{
					double	dPingYuan1,dPingYuan2;
					
					//小于3度是平原，但是不能排除其他的情况，例如坡度较小，但是高差较大的情况
					//if(dMeanSlope<3)	dPingYuan1	= 1;
					//else				dPingYuan1	= 0;
					dPingYuan1	= 1. / ( 1 + exp(2*(dMeanSlope-4)) );

					//边缘高差和整体高差的差距在20米之内
					//dPingYuan2	= 1. / ( 1 + pow(0.3 * SH( fabs(dDeltaHeight-dDeltaMarginHeight)/10 ) , 3) );

					//整体高差在100米以内
					//dPingYuan2	= 1. / ( 1 + pow(1.3 * TH(dDeltaHeight/150) , 10) );
					dPingYuan2	= 1. / ( 1 + exp(0.05*(dDeltaHeight-100)) );

					//
					dPingYuan	= ( 2*dPingYuan2 + dPingYuan1 )/3;
				}
				
				//丘陵隶属度函数
				double	dQiuLing;
				{
					double	dQiuLing1,dQiuLing2,dQiuLing3,dQiuLing4;
					
					//高差在20-200之间
					//dQiuLing1	= 1. / ( 1 + 0.39 * pow( log( SH(dDeltaHeight/25) )-1.55,2 ) );
					dQiuLing1	= 1. / ( 1 + pow((dDeltaHeight-200)/100,6) );;

					//边缘高差在150左右
					dQiuLing2	= 1. / ( 1 + pow( 0.2 * log( dDeltaMarginHeight+1 ) , 10) );
					//高差比为0为绝对为丘陵，0.5为界线
					dQiuLing3	= 1. / ( 1 + exp(10*((dDeltaMarginHeight/dDeltaHeight)-0.5)) );
					//
					dQiuLing4	= 1. / ( 1 + pow((dMeanSlope-6)/2,4) );
					
					//
					dQiuLing	= sqrt( (1-dPingYuan) * ( (2*dQiuLing1 + dQiuLing2 + dQiuLing3 + 2*dQiuLing4)/6) );
				}

				//山地隶属度函数
				double	dShanDi;
				{
					double	dShanDi1,dShanDi2,dShanDi3;
					
					//高差大于200米
					//dShanDi1	= 1. / ( 1 + pow(400/dDeltaHeight,4));
					dShanDi1	= 1. / ( 1 + exp(-0.03*(dDeltaHeight-300)) );

					//dShanDi2	= 1. / ( 1 + pow( log(dDeltaMarginHeight/dDeltaHeight)+0.1, 2) );
					//高差比为1时，绝对是山地，0.5为界线
					dShanDi2	= 1. / ( 1 + exp(-10*((dDeltaMarginHeight/dDeltaHeight)-0.5)) );
					//
					dShanDi3	= 1. / ( 1 + exp(-2*(dMeanSlope-8)) );
					
					//
					dShanDi		= sqrt( (1-dPingYuan) * ( (2*dShanDi1+dShanDi2+2*dShanDi3)/5 ) );
				}

				//寻找最大值
				int		iSign[4];
				double	dLSD[4];
				
				iSign[0]	= 1;	dLSD[0]	= dTaiDi;
				iSign[1]	= 2;	dLSD[1]	= dPingYuan;
				iSign[2]	= 3;	dLSD[2]	= dQiuLing;
				iSign[3]	= 4;	dLSD[3]	= dShanDi;

				//排序
				for(I=0;I<4;I++)
				{
					for(J=I+1;J<4;J++)
					{
						//
						if(dLSD[I]<dLSD[J])	
						{
							double	dTemp;
							dTemp	= dLSD[J];	dLSD[J]		= dLSD[I];	dLSD[I]		= dTemp;

							int		iTemp;
							iTemp	= iSign[J];	iSign[J]	= iSign[I];	iSign[I]	= iTemp;
						}
					}
				}

				
				//台地				
				if(iSign[0] == 1)
				{
					//if(1-dLSD[1]/dLSD[0] < 0.1)			m_pTypeObj[ci]	= 5;
					//else								m_pTypeObj[ci]	= 1;
					//m_pTypeObj[ci]	= 1;
				}

				//平原
				if(iSign[0] == 2)
				{
					//丘陵
					if(iSign[1] == 3)
					{
						m_pTypeObj[ci]	= 2+dLSD[1]/(dLSD[1]+dLSD[0]);
					}
					else
					{
						m_pTypeObj[ci]	= 2;
					}
				}
				
				//丘陵
				if(iSign[0] == 3)
				{
					if(iSign[1] == 2)
					{
						m_pTypeObj[ci]	= 3-dLSD[1]/(dLSD[1]+dLSD[0]);
					}
					else	if(iSign[1] == 4)
					{
						m_pTypeObj[ci]	= 3+dLSD[1]/(dLSD[1]+dLSD[0]);
					}
					else
						m_pTypeObj[ci]	= 3;
				}
				
				//山地
				if(iSign[0] == 4)
				{
					if(iSign[1] == 3)
					{
						m_pTypeObj[ci]	= 4-dLSD[1]/(dLSD[1]+dLSD[0]);
					}
					else
						m_pTypeObj[ci]	= 4;
				}

				pPingYuan[ci]	= dPingYuan;
				pQiuLing[ci]	= dQiuLing;
				pShanDi[ci]		= dShanDi;
			}
		}
	}

	//保存为grid格式
	{
		CString	szPathName;
		szPathName	= m_szPathName.Left(m_szPathName.GetLength()-4)+"G.grd";

		FILE	*fp;
		fp	= fopen(szPathName,"w");

		if(fp == NULL)	return	false;
		
		//DSAA
		fprintf(fp,"DSAA\n");

		//iCols,iRows,应该判断清楚什么是行和列
		//先列后行
		fprintf(fp,"%ld %ld\n",m_iNumOfX,m_iNumOfY);
		
		//dMinX,dMaxX
		fprintf(fp,"%lf %lf\n",m_dDemLeft,m_dDemRight);

		//dMinY,dMaxY
		fprintf(fp,"%lf %lf\n",m_dDemBottom,m_dDemTop);

		//
		double	dMinH,dMaxH;
		int	i,j;
		for(j=0;j<m_iNumOfY;j++)
		{
			for(i=0;i<m_iNumOfX;i++)
			{
				double	dH;
				dH	= m_pTypeObj[j*m_iNumOfX+i];
				if(i==0 && j==0)
				{
					dMinH	= dMaxH	= dH;
				}
				else
				{
					if(dH < dMinH)	dMinH	= dH;
					if(dH > dMaxH)	dMaxH	= dH;
				}
			}
		}
		
		//dMinZ,dMaxZ
		fprintf(fp,"%lf %lf\n",dMinH,dMaxH);
		
		//
		int		iCount;
		for(j=0;j<m_iNumOfY;j++)
		{
			iCount	= 0;
			for(i=0;i<m_iNumOfX;i++)
			{
				double	dH;
				dH	= m_pTypeObj[j*m_iNumOfX+i];

				fprintf(fp,"%lf ",dH);

				iCount++;

				if(iCount == 10)	
				{
					fprintf(fp,"\n");

					iCount	= 0;
				}
			}
			fprintf(fp,"\n");
			fprintf(fp,"\n");
		}
		
		//关闭
		fclose(fp);
	}

	//保存为grid格式
	{
		CString	szPathName;
		szPathName	= m_szPathName.Left(m_szPathName.GetLength()-4)+"PingYuan.grd";

		FILE	*fp;
		fp	= fopen(szPathName,"w");

		if(fp == NULL)	return	false;
		
		//DSAA
		fprintf(fp,"DSAA\n");

		//iCols,iRows,应该判断清楚什么是行和列
		//先列后行
		fprintf(fp,"%ld %ld\n",m_iNumOfX,m_iNumOfY);
		
		//dMinX,dMaxX
		fprintf(fp,"%lf %lf\n",m_dDemLeft,m_dDemRight);

		//dMinY,dMaxY
		fprintf(fp,"%lf %lf\n",m_dDemBottom,m_dDemTop);
		
		//dMinZ,dMaxZ
		fprintf(fp,"%lf %lf\n",0.0,1.0);
		
		//
		int		iCount;
		for(j=0;j<m_iNumOfY;j++)
		{
			iCount	= 0;
			for(i=0;i<m_iNumOfX;i++)
			{
				double	dH;
				dH	= pPingYuan[j*m_iNumOfX+i];

				fprintf(fp,"%lf ",dH);

				iCount++;

				if(iCount == 10)	
				{
					fprintf(fp,"\n");

					iCount	= 0;
				}
			}
			fprintf(fp,"\n");
			fprintf(fp,"\n");
		}
		
		//关闭
		fclose(fp);
	}

	//保存为grid格式
	{
		CString	szPathName;
		szPathName	= m_szPathName.Left(m_szPathName.GetLength()-4)+"QiuLing.grd";

		FILE	*fp;
		fp	= fopen(szPathName,"w");

		if(fp == NULL)	return	false;
		
		//DSAA
		fprintf(fp,"DSAA\n");

		//iCols,iRows,应该判断清楚什么是行和列
		//先列后行
		fprintf(fp,"%ld %ld\n",m_iNumOfX,m_iNumOfY);
		
		//dMinX,dMaxX
		fprintf(fp,"%lf %lf\n",m_dDemLeft,m_dDemRight);

		//dMinY,dMaxY
		fprintf(fp,"%lf %lf\n",m_dDemBottom,m_dDemTop);
		
		//dMinZ,dMaxZ
		fprintf(fp,"%lf %lf\n",0.0,1.0);
		
		//
		int		iCount;
		for(j=0;j<m_iNumOfY;j++)
		{
			iCount	= 0;
			for(i=0;i<m_iNumOfX;i++)
			{
				double	dH;
				dH	= pQiuLing[j*m_iNumOfX+i];

				fprintf(fp,"%lf ",dH);

				iCount++;

				if(iCount == 10)	
				{
					fprintf(fp,"\n");

					iCount	= 0;
				}
			}
			fprintf(fp,"\n");
			fprintf(fp,"\n");
		}
		
		//关闭
		fclose(fp);
	}
	
	//保存为grid格式
	{
		CString	szPathName;
		szPathName	= m_szPathName.Left(m_szPathName.GetLength()-4)+"ShanDi.grd";

		FILE	*fp;
		fp	= fopen(szPathName,"w");

		if(fp == NULL)	return	false;
		
		//DSAA
		fprintf(fp,"DSAA\n");

		//iCols,iRows,应该判断清楚什么是行和列
		//先列后行
		fprintf(fp,"%ld %ld\n",m_iNumOfX,m_iNumOfY);
		
		//dMinX,dMaxX
		fprintf(fp,"%lf %lf\n",m_dDemLeft,m_dDemRight);

		//dMinY,dMaxY
		fprintf(fp,"%lf %lf\n",m_dDemBottom,m_dDemTop);
		
		//dMinZ,dMaxZ
		fprintf(fp,"%lf %lf\n",0.0,1.0);
		
		//
		int		iCount;
		for(j=0;j<m_iNumOfY;j++)
		{
			iCount	= 0;
			for(i=0;i<m_iNumOfX;i++)
			{
				double	dH;
				dH	= pShanDi[j*m_iNumOfX+i];

				fprintf(fp,"%lf ",dH);

				iCount++;

				if(iCount == 10)	
				{
					fprintf(fp,"\n");

					iCount	= 0;
				}
			}
			fprintf(fp,"\n");
			fprintf(fp,"\n");
		}
		
		//关闭
		fclose(fp);
	}
	
	if(pPingYuan != NULL)	{	delete	[]pPingYuan;	pPingYuan	= NULL;	}
	if(pQiuLing != NULL)	{	delete	[]pQiuLing;		pQiuLing	= NULL;	}
	if(pShanDi != NULL)		{	delete	[]pShanDi;		pShanDi	= NULL;	}

	return	true;
}

//
void CDCR_GeoMorphological_LSD::SetCoordMapping(double dXA,double dXB,double dYA,double dYB)
{
	m_dMapXA	= dXA;
	m_dMapXB	= dXB;
	m_dMapYA	= dYA;
	m_dMapYB	= dYB;
}

//
void CDCR_GeoMorphological_LSD::SetClientRect(CRect rRect)
{
	m_rRect	= rRect;
}

//
void CDCR_GeoMorphological_LSD::Draw(CDC *pDC)
{
	if(m_pTypeObj == NULL)			return;

	//if(m_bShowStatus == false)		return;

	CDCR_CurvatureSet::Instance()->SetMinCurvature(0);
	CDCR_CurvatureSet::Instance()->SetMidCurvature(0.5);
	CDCR_CurvatureSet::Instance()->SetMaxCurvature(1);

	double	dShowLeft,dShowRight,dShowBottom,dShowTop;
	//计算实际的显示区域
	dShowLeft	= (m_rRect.left - m_dMapXB) / m_dMapXA;
	dShowRight	= (m_rRect.right - m_dMapXB) / m_dMapXA;
	dShowTop	= (m_rRect.top - m_dMapYB) / m_dMapYA;
	dShowBottom	= (m_rRect.bottom - m_dMapYB) / m_dMapYA;

	int		i,j,ci;
	double	dX,dY;
	double		dValue;

	//
	for(j=0;j<m_iNumOfY;j++)
	{
		for(i=0;i<m_iNumOfX;i++)
		{	
			ci			= j*m_iNumOfX+i;
		
			if(m_pTypeObj[ci] == 0)	continue;

			dX			= m_dDemLeft + i*m_dSizeOfX;
			dY			= m_dDemBottom + j*m_dSizeOfY;
			
			if((dX>=dShowLeft-m_dSizeOfX&&dX<=dShowRight+m_dSizeOfX) && (dY>=dShowBottom-m_dSizeOfY&&dY<=dShowTop+m_dSizeOfY))
			{
				dValue	= m_pTypeObj[ci];
				
				if(dValue != 0)
				{
					//
					double	dLeft,dRight,dBottom,dTop;
					dLeft	= dX - m_dSizeOfX/2;
					dRight	= dLeft + m_dSizeOfX;
					dBottom	= dY - m_dSizeOfY/2;
					dTop	= dBottom + m_dSizeOfY;

					//
					CRect	rectCell;
					rectCell.left	= (int)(dLeft*m_dMapXA + m_dMapXB + 0.5);
					rectCell.bottom	= (int)(dBottom*m_dMapYA + m_dMapYB + 0.5);
					rectCell.right	= (int)(dRight*m_dMapXA + m_dMapXB + 0.5);
					rectCell.top	= (int)(dTop*m_dMapYA + m_dMapYB + 0.5);
							
					//COLORREF	crColor;
					//crColor	= CDCR_CurvatureSet::Instance()->GetColor(dValue);

					COLORREF	crColor;
					//if(dValue == 1)	crColor	= RGB(255,0,255);
					if(dValue == 2)	crColor	= RGB(132,207,66);
					if(dValue == 3)	crColor	= RGB(231,223,74);
					if(dValue == 4)	crColor	= RGB(231,154,90);
					//if(dValue == 5)	crColor	= RGB(255,255,255);
					
					//
					CBrush	*pNewBrush,*pOldBrush;
					pNewBrush	= new	CBrush;
					pNewBrush->CreateSolidBrush(crColor);
					pOldBrush	= pDC->SelectObject(pNewBrush);

					pDC->FillRect(rectCell,pNewBrush);	

					pDC->SelectObject(pOldBrush);
					delete	pNewBrush;
				}
			}
		}
	}
}

//
void CDCR_GeoMorphological_LSD::Release()
{
	if(m_pTypeObj != NULL)	{	delete	[]m_pTypeObj;	m_pTypeObj	= NULL;	}
}

//设置窗口尺寸
void CDCR_GeoMorphological_LSD::SetWindowSize(int iSize)
{
	m_iWindowSize	= iSize;
}

int CDCR_GeoMorphological_LSD::GetWindowSize()
{
	return	m_iWindowSize;
}

double CDCR_GeoMorphological_LSD::SH(double dX)
{
	return	( exp(dX) - exp(-dX) ) / 2;
}

double CDCR_GeoMorphological_LSD::CH(double dX)
{
	return	( exp(dX) + exp(-dX) ) / 2;
}

double CDCR_GeoMorphological_LSD::TH(double dX)
{
	return	( exp(dX) - exp(-dX) ) / ( exp(dX) + exp(-dX) );
}

//
bool CDCR_GeoMorphological_LSD::OnCapture(double dX,double dY)
{
	if(dX<m_dDemLeft || dX>m_dDemRight || dY<m_dDemBottom || dY>m_dDemTop)	return	false;
	
	int		iI,iJ;
	
	iI	= (int)((dX-m_dDemLeft)/m_dSizeOfX+0.5);
	iJ	= (int)((dY-m_dDemBottom)/m_dSizeOfY+0.5);
	
	//{
		//首先按照最佳分析区域计算坡度信息
		pSlope.SetGridDem(m_pDemObj,m_iNumOfX,m_iNumOfY,m_dSizeOfX,m_dSizeOfY,m_dRealSize,
							m_dDemLeft,m_dDemRight,m_dDemBottom,m_dDemTop,m_szPathName);
		
		//计算坡度
		pSlope.OnCalculate(0);
		
		//获取坡度信息
		double	*dSlope;
		dSlope		= pSlope.GetSlope();
		if(dSlope == NULL)		return	false;
	//}
	

	//开始循环计算每一点所属的地貌类型隶属度值
	int	ci;
	ci			= iJ*m_iNumOfX+iI;
	
	//假如高程是非法值，则退出
	if(m_pDemObj[ci] == INVALID_HEIGHT)	
	{
		return	false;
	}

	//对于每一点来说都是需要计算的,即确定区域
	int	iBegin,iEnd;
	int	jBegin,jEnd;

	iBegin	= iI - m_iWindowSize;
	iEnd	= iI + m_iWindowSize;

	jBegin	= iJ - m_iWindowSize;
	jEnd	= iJ + m_iWindowSize;
	
	//如果越界了，则不参与计算
	if(iBegin < 0 || iEnd>m_iNumOfX-1 || jBegin<0 || jEnd>m_iNumOfY-1)
	{
		pSlope.OnRelease();
		return	false;
	}
	else
	{		
		int		I,J;

		///////////////////////////////////////////////////////
		//提取平均坡度、提取边缘高差、提取高差
		//{
			double	dMeanSlope	= 0;
			int		iMeanSlope	= 0;

			for(I=iBegin;I<=iEnd;I++)
			{
				for(J=jBegin;J<=jEnd;J++)
				{
					if(dSlope[J*m_iNumOfX+I] == INVALID_VALUE)				continue;

					if( sqrt((I-iI)*(I-iI) + (J-iJ)*(J-iJ)) > m_iWindowSize)	continue;

					dMeanSlope	= dMeanSlope + dSlope[J*m_iNumOfX+I];
					iMeanSlope++;
				}
			}

			if(iMeanSlope == 0)		dMeanSlope	= 0;
			else					dMeanSlope	= dMeanSlope/iMeanSlope;
		//}
		///////////////////////////////////////////////////////

		///////////////////////////////////////////////////////
		//提取高差
		//{
			double	dDeltaHeight;
			double	dDeltaMarginHeight;

			double	dMinHeight	= 0;
			double	dMaxHeight	= 0;

			bool	bFirstValue	= true;
			
			//总的
			for(I=iBegin;I<=iEnd;I++)
			{
				for(J=jBegin;J<=jEnd;J++)
				{
					if(m_pDemObj[J*m_iNumOfX+I] == INVALID_HEIGHT)			continue;

					if( sqrt((I-iI)*(I-iI) + (J-iJ)*(J-iJ)) > m_iWindowSize)	continue;

					if(bFirstValue)
					{
						dMinHeight	= dMaxHeight	= m_pDemObj[J*m_iNumOfX+I];
						bFirstValue	= false;
					}
					else
					{
						if(m_pDemObj[J*m_iNumOfX+I] < dMinHeight)	dMinHeight	= m_pDemObj[J*m_iNumOfX+I];
						if(m_pDemObj[J*m_iNumOfX+I] > dMaxHeight)	dMaxHeight	= m_pDemObj[J*m_iNumOfX+I]; 
					}
				}
			}
			//dDeltaHeight	= dMaxHeight-dMinHeight;

			//边缘高差
			double	dMaxMarginH	= 0;
			double	dMinMarginH	= 0;
			double	dMeanMarginH= 0;
			int		iMeanMarginC= 0;
			bFirstValue		= true;

			for(I=iBegin;I<=iEnd;I++)
			{
				for(J=jBegin;J<=jEnd;J++)
				{
					if(m_pDemObj[J*m_iNumOfX+I] == INVALID_HEIGHT)			continue;

					//
					if( sqrt((I-iI)*(I-iI) + (J-iJ)*(J-iJ)) < m_iWindowSize-2)	continue;
					if( sqrt((I-iI)*(I-iI) + (J-iJ)*(J-iJ)) > m_iWindowSize)	continue;

					if(bFirstValue)
					{
						dMaxMarginH		= dMinMarginH	= m_pDemObj[J*m_iNumOfX+I];

						dMeanMarginH	= m_pDemObj[J*m_iNumOfX+I];
						iMeanMarginC++;
						bFirstValue		= false;
					}
					else
					{
						if(m_pDemObj[J*m_iNumOfX+I] < dMinMarginH)	dMinMarginH	= m_pDemObj[J*m_iNumOfX+I];
						if(m_pDemObj[J*m_iNumOfX+I] > dMaxMarginH)	dMaxMarginH	= m_pDemObj[J*m_iNumOfX+I]; 

						dMeanMarginH	= dMeanMarginH + m_pDemObj[J*m_iNumOfX+I];
						iMeanMarginC++;
					}
				}
			}

			dDeltaHeight		= fabs(dMaxHeight-dMeanMarginH/iMeanMarginC);
			dDeltaMarginHeight	= fabs(dMaxMarginH-dMeanMarginH/iMeanMarginC);
		//}
		///////////////////////////////////////////////////////
		
		//已经提取参数信息
		
		//台地隶属度
		double	dTaiDi;
		{
			double	dTaiDi1,dTaiDi2,dTaiDi3;
			
			dTaiDi1	= 1. / ( 1 + exp(dMeanSlope-12.5) );

			dTaiDi2	= 1./( 1 + pow(1.3 * TH(dDeltaHeight/200),10) );
			dTaiDi3	= 1./( 1 + pow(0.4 * log(dDeltaMarginHeight + 1),10) );

			//dTaiDi	= pow( dTaiDi1*dTaiDi1*dTaiDi2*dTaiDi3 , 0.25);
			dTaiDi	= 0;
		}
				//平原隶属度(这是最简单的)
				double	dPingYuan;
				{
					double	dPingYuan1,dPingYuan2;
					
					//小于3度是平原，但是不能排除其他的情况，例如坡度较小，但是高差较大的情况
					//if(dMeanSlope<3)	dPingYuan1	= 1;
					//else				dPingYuan1	= 0;
					dPingYuan1	= 1. / ( 1 + exp(2*(dMeanSlope-4)) );

					//边缘高差和整体高差的差距在20米之内
					//dPingYuan2	= 1. / ( 1 + pow(0.3 * SH( fabs(dDeltaHeight-dDeltaMarginHeight)/10 ) , 3) );

					//整体高差在100米以内
					//dPingYuan2	= 1. / ( 1 + pow(1.3 * TH(dDeltaHeight/150) , 10) );
					dPingYuan2	= 1. / ( 1 + exp(0.05*(dDeltaHeight-100)) );

					//
					dPingYuan	= ( 2*dPingYuan2 + dPingYuan1 )/3;
				}
				
				//丘陵隶属度函数
				double	dQiuLing;
				{
					double	dQiuLing1,dQiuLing2,dQiuLing3,dQiuLing4;
					
					//高差在20-200之间
					//dQiuLing1	= 1. / ( 1 + 0.39 * pow( log( SH(dDeltaHeight/25) )-1.55,2 ) );
					dQiuLing1	= 1. / ( 1 + pow((dDeltaHeight-250)/150,6) );;

					//边缘高差在150左右
					dQiuLing2	= 1. / ( 1 + pow( 0.2 * log( dDeltaMarginHeight+1 ) , 10) );
					//高差比为0为绝对为丘陵，0.5为界线
					dQiuLing3	= 1. / ( 1 + exp(10*((dDeltaMarginHeight/dDeltaHeight)-0.5)) );
					//
					dQiuLing4	= 1. / ( 1 + pow((dMeanSlope-6)/2,4) );
					
					//
					dQiuLing	= sqrt( (1-dPingYuan) * ( (2*dQiuLing1 + dQiuLing2 + dQiuLing3 + 2*dQiuLing4)/6) );
				}

				//山地隶属度函数
				double	dShanDi;
				{
					double	dShanDi1,dShanDi2,dShanDi3;
					
					//高差大于200米
					//dShanDi1	= 1. / ( 1 + pow(400/dDeltaHeight,4));
					dShanDi1	= 1. / ( 1 + exp(-0.05*(dDeltaHeight-400)) );

					//dShanDi2	= 1. / ( 1 + pow( log(dDeltaMarginHeight/dDeltaHeight)+0.1, 2) );
					//高差比为1时，绝对是山地，0.5为界线
					dShanDi2	= 1. / ( 1 + exp(-10*((dDeltaMarginHeight/dDeltaHeight)-0.5)) );
					//
					dShanDi3	= 1. / ( 1 + exp(-2*(dMeanSlope-8)) );
					
					//
					dShanDi		= sqrt( (1-dPingYuan) * ( (2*dShanDi1+dShanDi2+2*dShanDi3)/5 ) );
				}


		CString	szInfo;
		szInfo.Format("坡度 = %lf 高差 %lf 边缘高差 %lf \n 台地 = %lf 平原 = %lf 丘陵 = %lf 山地 = %lf ",dMeanSlope,dDeltaHeight,dDeltaMarginHeight,dTaiDi,dPingYuan,dQiuLing,dShanDi);
		AfxMessageBox(szInfo);
	}
	
	pSlope.OnRelease();

	return	true;
}