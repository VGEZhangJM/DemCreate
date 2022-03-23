// DCR_GridDem.cpp: implementation of the CDCR_GridDem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "vr_demcreator.h"
#include "DCR_GridDem.h"

#include "DCR_SlopeSet.h"
#include "DCR_CurvatureSet.h"
#include "DCR_HeightSet.h"
#include "DCR_DemSet.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDCR_GridDem::CDCR_GridDem()
{
	m_dDemLeft				= 0.0;
	m_dDemRight				= 0.0;
	m_dDemBottom			= 0.0;
	m_dDemTop				= 0.0;

	//显示参数
	m_dMapXA				= 1.0;
	m_dMapXB				= 0.0;
	m_dMapYA				= -1.0;
	m_dMapYB				= 0.0;

	m_iNumOfX				= 0;
	m_iNumOfY				= 0;

	m_dSizeOfX				= 0.0;
	m_dSizeOfY				= 0.0;

	m_dRealSize				= 0.0;

	//
	m_pDemObj				= NULL;
	m_dMinHeight			= 0.0;
	m_dMaxHeight			= 0.0;
	
	//DEM的方差值
	m_pDemSD				= NULL;
	m_dMinSD				= 0;
	m_dMaxSD				= 0;

	//DEM
	m_bShowStatus			= true;
}

CDCR_GridDem::~CDCR_GridDem()
{
	OnRelease();
}


//////////////////////////////////////////////////////////////////////
// 01 读取数据
//////////////////////////////////////////////////////////////////////
bool CDCR_GridDem::OnRead(char *chPathName)
{
	//先释放
	OnRelease();

	//获取dem的最大最小值
	FILE	*fp;
	fp	= fopen(chPathName,"rb");

	if(fp == NULL)	return	false;

	fread(&m_dDemLeft,sizeof(double),1,fp);
	fread(&m_dDemBottom,sizeof(double),1,fp);
	fread(&m_dDemRight,sizeof(double),1,fp);
	fread(&m_dDemTop,sizeof(double),1,fp);
	fread(&m_iNumOfX,sizeof(long),1,fp);
	fread(&m_iNumOfY,sizeof(long),1,fp);
	fread(&m_dSizeOfX,sizeof(double),1,fp);
	fread(&m_dSizeOfY,sizeof(double),1,fp);
	fread(&m_dRealSize,sizeof(double),1,fp);
	float	fMinHeight;
	float	fMaxHeight;
	fread(&fMinHeight,sizeof(float),1,fp);
	fread(&fMaxHeight,sizeof(float),1,fp);

	m_dMinHeight	= fMinHeight;
	m_dMaxHeight	= fMaxHeight;
	
	m_pDemObj		= new	double[m_iNumOfY*m_iNumOfX];

	float	*pDemObj;
	pDemObj			= new	float[m_iNumOfY*m_iNumOfX];
	fread(pDemObj,sizeof(float),m_iNumOfY*m_iNumOfX,fp);
	
	for(int i=0;i<m_iNumOfY*m_iNumOfX;i++)
	{
		m_pDemObj[i]	= pDemObj[i];
	}
	
	delete	[]pDemObj;

	fclose(fp);

	m_szPathName	= chPathName;

	return	true;
}

//////////////////////////////////////////////////////////////////////
// 02 保存DEM为DEM文件
//////////////////////////////////////////////////////////////////////
bool CDCR_GridDem::OnSave(char *chPathName)
{
	if(m_pDemObj == NULL)	return	false;

	//获取dem的最大最小值
	FILE	*fp;
	fp	= fopen(chPathName,"wb");

	if(fp != NULL)
	{
		fwrite(&m_dDemLeft,sizeof(double),1,fp);
		fwrite(&m_dDemBottom,sizeof(double),1,fp);
		fwrite(&m_dDemRight,sizeof(double),1,fp);
		fwrite(&m_dDemTop,sizeof(double),1,fp);
		fwrite(&m_iNumOfX,sizeof(long),1,fp);
		fwrite(&m_iNumOfY,sizeof(long),1,fp);
		fwrite(&m_dSizeOfX,sizeof(double),1,fp);
		fwrite(&m_dSizeOfY,sizeof(double),1,fp);
		fwrite(&m_dRealSize,sizeof(double),1,fp);
		float	fMinHeight;
		float	fMaxHeight;
		fMinHeight	= (float)m_dMinHeight;
		fMaxHeight	= (float)m_dMaxHeight;
		fwrite(&fMinHeight,sizeof(float),1,fp);
		fwrite(&fMaxHeight,sizeof(float),1,fp);
		
		float	*pDemObj;
		pDemObj			= new	float[m_iNumOfY*m_iNumOfX];
		
		for(int i=0;i<m_iNumOfY*m_iNumOfX;i++)
		{
			 pDemObj[i]	= (float)(m_pDemObj[i]);
		}

		//
		fwrite(pDemObj,sizeof(float),m_iNumOfY*m_iNumOfX,fp);

		delete	[]pDemObj;

		fclose(fp);
	}

	return	true;
}

//////////////////////////////////////////////////////////////////////
// 03 导入数据,主要Sufer中的GRD
//////////////////////////////////////////////////////////////////////
bool CDCR_GridDem::OnImport(char *chPathName)
{
	//先释放
	OnRelease();

	FILE	*fp;
	fp	= fopen(chPathName,"r");

	if(fp == NULL)	return	false;

	char	chInfo[64];
	
	//DSAA
	fscanf(fp,"%s",chInfo);

	//iCols,iRows,应该判断清楚什么是行和列
	//先列后行
	fscanf(fp,"%ld%ld",&m_iNumOfX,&m_iNumOfY);
	
	//dMinX,dMaxX
	fscanf(fp,"%lf%lf",&m_dDemLeft,&m_dDemRight);

	//dMinY,dMaxY
	fscanf(fp,"%lf%lf",&m_dDemBottom,&m_dDemTop);

	//dMinZ,dMaxZ
	fscanf(fp,"%lf%lf",&m_dMinHeight,&m_dMaxHeight);
	
	m_dSizeOfX		= (m_dDemRight-m_dDemLeft)/(m_iNumOfX-1);
	m_dSizeOfY		= (m_dDemTop-m_dDemBottom)/(m_iNumOfY-1);
	//
	//double	dRealDistX,dRealDistY;
	//dRealDistX		= 2*6378245*cos(m_dDemBottom*3.141592627/180.0)*sin(m_dSizeOfY*3.141592627/180.0/2);
	//dRealDistY		= 2*6378245*sin(m_dSizeOfX*3.141592627/180.0/2);
	//m_dRealSize		= (dRealDistX+dRealDistY)/2;
	m_dRealSize		= 1;

	//
	m_pDemObj		= new	double[m_iNumOfY*m_iNumOfX];

	for(int j=0;j<m_iNumOfY;j++)
	{
		for(int i=0;i<m_iNumOfX;i++)
		{
			double	dH;
			fscanf(fp,"%lf",&dH);

			m_pDemObj[j*m_iNumOfX+i]	= dH;
		}
	}
	
	//关闭
	fclose(fp);
		
	m_szPathName	= chPathName;

	return	true;
}

//////////////////////////////////////////////////////////////////////
// 04 导出DEM信息为GRD文件,可供Surfer使用
//////////////////////////////////////////////////////////////////////
bool CDCR_GridDem::OnExport(char *chPathName)
{
	//假如数据为NULL
	if(m_pDemObj == NULL)	return	false;

	FILE	*fp;
	fp	= fopen(chPathName,"w");

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
	int	i,j;
	for(j=0;j<m_iNumOfY;j++)
	{
		for(i=0;i<m_iNumOfX;i++)
		{
			double	dH;
			dH	= m_pDemObj[j*m_iNumOfX+i];
			if(i==0 && j==0)
			{
				m_dMinHeight	= m_dMaxHeight	= dH;
			}
			else
			{
				if(dH < m_dMinHeight)	m_dMinHeight	= dH;
				if(dH > m_dMaxHeight)	m_dMaxHeight	= dH;
			}
		}
	}
	
	//dMinZ,dMaxZ
	fprintf(fp,"%lf %lf\n",m_dMinHeight,m_dMaxHeight);
	
	//
	int		iCount;
	for(j=0;j<m_iNumOfY;j++)
	{
		iCount	= 0;
		for(i=0;i<m_iNumOfX;i++)
		{
			double	dH;
			dH	= m_pDemObj[j*m_iNumOfX+i];

			fprintf(fp,"%.0lf ",dH);

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
		
	return	true;
}

//////////////////////////////////////////////////////////////////////
// 11 释放数据
//////////////////////////////////////////////////////////////////////
void CDCR_GridDem::OnRelease()
{
	//
	if(m_pDemObj != NULL)
	{
		delete	[]m_pDemObj;
		m_pDemObj				= NULL;
	}
}

//////////////////////////////////////////////////////////////////////
// 12 DEM数据克隆
//////////////////////////////////////////////////////////////////////
void CDCR_GridDem::OnClone(CDCR_GridDem *pDEM)
{
	if(pDEM != NULL)
	{
		double	dLeft,dRight,dBottom,dTop;
		//
		pDEM->GetDemExt(dLeft,dRight,dBottom,dTop);

		//
		m_dDemLeft		= dLeft;
		m_dDemRight		= dRight;
		m_dDemBottom	= dBottom;
		m_dDemTop		= dTop;
		
		//
		m_iNumOfX		= pDEM->GetNumOfX();
		m_iNumOfY		= pDEM->GetNumOfY();

		//
		m_dSizeOfX		= pDEM->GetSizeOfX();
		m_dSizeOfY		= pDEM->GetSizeOfY();

		//
		SetHeight(pDEM->GetHeight());		
	}
}

//////////////////////////////////////////////////////////////////////
// 13 设置显示窗口
//////////////////////////////////////////////////////////////////////
void CDCR_GridDem::SetClientRect(CRect rRect)
{
	m_rRect	= rRect;
}

//////////////////////////////////////////////////////////////////////
// 14 坐标映射
//////////////////////////////////////////////////////////////////////
void CDCR_GridDem::SetCoordMapping(double dXA,double dXB,double dYA,double dYB)
{
	m_dMapXA	= dXA;
	m_dMapXB	= dXB;
	m_dMapYA	= dYA;
	m_dMapYB	= dYB;
}

//////////////////////////////////////////////////////////////////////
// 15 绘图
//////////////////////////////////////////////////////////////////////
void CDCR_GridDem::OnDraw(CDC *pDC)
{	
	//
	if(m_pDemObj == NULL)			return;

	//
	if(m_bShowStatus == false)		return;
	
	double	dShowLeft,dShowRight,dShowBottom,dShowTop;
	//计算实际的显示区域
	dShowLeft	= (m_rRect.left - m_dMapXB) / m_dMapXA;
	dShowRight	= (m_rRect.right - m_dMapXB) / m_dMapXA;
	dShowTop	= (m_rRect.top - m_dMapYB) / m_dMapYA;
	dShowBottom	= (m_rRect.bottom - m_dMapYB) / m_dMapYA;

	int		i,j,ci;
	double	dX,dY,dH;

	for(j=0;j<m_iNumOfY;j++)
	{
		for(i=0;i<m_iNumOfX;i++)
		{
			ci		= j*m_iNumOfX+i;
			dX		= m_dDemLeft + i*m_dSizeOfX;
			dY		= m_dDemBottom + j*m_dSizeOfY;
			
			if((dX>=dShowLeft-m_dSizeOfX&&dX<=dShowRight+m_dSizeOfX) && (dY>=dShowBottom-m_dSizeOfY&&dY<=dShowTop+m_dSizeOfY))
			{
				dH		= m_pDemObj[ci];

				//
				if(INVALID_HEIGHT != dH && INVALID_VALUE != dH)
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
					
					COLORREF	crColor;
					
					//crColor	= CDCR_HeightSet::Instance()->GetColor(dH);
					crColor		= CDCR_DemSet::Instance()->GetColor(dH);

					//
					CBrush	*pNewBrush,*pOldBrush;
					pNewBrush	= new	CBrush;
					pNewBrush->CreateSolidBrush(crColor);
					pOldBrush	= pDC->SelectObject(pNewBrush);

					pDC->FillRect(rectCell,pNewBrush);	

					pDC->SelectObject(pOldBrush);
					delete	pNewBrush;			
					
					//
					if(rectCell.Width() > 30)
					{
						CString	szInfo;
						szInfo.Format("%.2lf",dH);

						CFont font;
						font.CreateFont(12, 0, 000, 000, 400, 0, 0, 0, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_TT_ALWAYS, PROOF_QUALITY, VARIABLE_PITCH|FF_ROMAN, "宋体");
					
						CFont* oldfont=pDC->SelectObject(&font);
						int oldtextalign=pDC->SetTextAlign(TA_TOP|TA_CENTER);
						COLORREF oldtextcolor=pDC->SetTextColor(RGB(0,0,0));
						int oldbkmode=pDC->SetBkMode(TRANSPARENT);
					
						pDC->TextOut((rectCell.left+rectCell.right)/2,(rectCell.bottom+rectCell.top)/2-7,szInfo);
					
						pDC->SelectObject(oldfont);
						pDC->SetTextAlign(oldtextalign);
						pDC->SetTextColor(oldtextcolor);
						pDC->SetBkMode(oldbkmode);
					}
				}
			}
		}
	}		
}

//////////////////////////////////////////////////////////////////////
// 16 判断是否为空
//////////////////////////////////////////////////////////////////////
bool CDCR_GridDem::IsEmpty()
{
	if(m_pDemObj == NULL)	return	true;

	return	false;
}

//////////////////////////////////////////////////////////////////////
// 17 获取/设置横向点数
//////////////////////////////////////////////////////////////////////
int CDCR_GridDem::GetNumOfX()
{
	return	m_iNumOfX;
}
void CDCR_GridDem::SetNumOfX(int iNumX)
{
	m_iNumOfX	= iNumX;
}

//////////////////////////////////////////////////////////////////////
// 18 获取/设置纵向点数
//////////////////////////////////////////////////////////////////////
int CDCR_GridDem::GetNumOfY()
{
	return	m_iNumOfY;
}
void CDCR_GridDem::SetNumOfY(int iNumY)
{
	m_iNumOfY	= iNumY;
}

//////////////////////////////////////////////////////////////////////
// 19 设置/获取横向间隔
//////////////////////////////////////////////////////////////////////
double CDCR_GridDem::GetSizeOfX()
{
	return	m_dSizeOfX;
}
void CDCR_GridDem::SetSizeOfX(double dSizeX)
{
	m_dSizeOfX	= dSizeX;
}

//////////////////////////////////////////////////////////////////////
// 20 设置/获取纵向间隔
//////////////////////////////////////////////////////////////////////
double CDCR_GridDem::GetSizeOfY()
{
	return	m_dSizeOfY;
}
void CDCR_GridDem::SetSizeOfY(double dSizeY)
{
	m_dSizeOfY	= dSizeY;
}

//////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////
double CDCR_GridDem::GetRealSize()
{
	return	m_dRealSize;
}

//////////////////////////////////////////////////////////////////////
// 21 获取、设置DEM范围
//////////////////////////////////////////////////////////////////////
void CDCR_GridDem::GetDemExt(double &dLeft,double &dRight,double &dBottom,double &dTop)
{
	dLeft	= m_dDemLeft;
	dRight	= m_dDemRight;
	dBottom	= m_dDemBottom;
	dTop	= m_dDemTop;
}
void CDCR_GridDem::SetDemExt(double dLeft,double dRight,double dBottom,double dTop)
{
	m_dDemLeft		= dLeft;
	m_dDemRight		= dRight;
	m_dDemBottom	= dBottom;
	m_dDemTop		= dTop;
}

//////////////////////////////////////////////////////////////////////
// 22 获取最低、最高高程值
//////////////////////////////////////////////////////////////////////
double CDCR_GridDem::GetMaxHeight()
{
	return	m_dMaxHeight;
}
double CDCR_GridDem::GetMinHeight()
{
	return	m_dMinHeight;
}

//////////////////////////////////////////////////////////////////////
// 23 通过DEM获取某一点的高程值
//////////////////////////////////////////////////////////////////////
double CDCR_GridDem::GetHeight(double dX,double dY)
{
	if(m_pDemObj == NULL)			return	INVALID_VALUE;
	
	//
	if(dX < m_dDemLeft)				return	INVALID_VALUE;
	if(dX > m_dDemRight)			return	INVALID_VALUE;
	if(dY < m_dDemBottom)			return	INVALID_VALUE;
	if(dY > m_dDemTop)				return	INVALID_VALUE;

	double	dH;
	int		II,JJ;
	II=(int)((dX-m_dDemLeft)/m_dSizeOfX);
	JJ=(int)((dY-m_dDemBottom)/m_dSizeOfY);

	if(II < 0 || II > m_iNumOfX-1)	return INVALID_VALUE;
	if(JJ < 0 || JJ > m_iNumOfY-1)	return INVALID_VALUE;
	
	//西南到东北的划分方式

	double	xp,yp;

	xp	= (dX-(II*m_dSizeOfX+m_dDemLeft)) / m_dSizeOfX;
	yp	= (dY-(JJ*m_dSizeOfY+m_dDemBottom)) / m_dSizeOfY;

	if(fabs(xp) < 1e-10 && fabs(yp) < 1e-10)
	{
		dH	= m_pDemObj[II+JJ*m_iNumOfX];
		return	dH;
	}
	else
	{
		double	z1,z2,z3,z4;

		//在垂线上
		if(fabs(xp) < 1e-10)
		{
			z1	= m_pDemObj[II+JJ*m_iNumOfX];
			z2	= m_pDemObj[II+(JJ+1)*m_iNumOfX];

			if(z1==INVALID_VALUE)	return	INVALID_VALUE;
			if(z2==INVALID_VALUE)	return	INVALID_VALUE;
			
			dH	= z1+(z2-z1)*yp;	return	dH;
		}
		//在横线上
		else	if(fabs(yp) < 1e-10)
		{
			z1	= m_pDemObj[II+JJ*m_iNumOfX];
			z4	= m_pDemObj[(II+1)+JJ*m_iNumOfX];	

			if(z1==INVALID_VALUE)	return	INVALID_VALUE;
			if(z4==INVALID_VALUE)	return	INVALID_VALUE;

			dH	= z1+(z4-z1)*xp;	return	dH;
		}
		else
		{
			z1	= m_pDemObj[II+JJ*m_iNumOfX];
			z2	= m_pDemObj[II+(JJ+1)*m_iNumOfX];
			z3	= m_pDemObj[(II+1)+(JJ+1)*m_iNumOfX];
			z4	= m_pDemObj[(II+1)+JJ*m_iNumOfX];	
			if(fabs(xp-yp) < 1e-10)
			{
				dH	= z1+(z3-z1)*(sqrt(xp*xp+yp*yp));
			}
			else
			{
				//线性插值			
				double	k;
				//上三角
				if(xp<yp)	k	= 1;
				//下三角
				if(xp>yp)	k	= 0;
				
				dH	= k * ( z1 + (z3-z2)*xp + (z2-z1)*yp ) + (1-k) * (z1 + (z4-z1)*xp + (z3-z4)*yp );
			}

			if(z1==INVALID_VALUE)	return	INVALID_VALUE;
			if(z2==INVALID_VALUE)	return	INVALID_VALUE;
			if(z3==INVALID_VALUE)	return	INVALID_VALUE;
			if(z4==INVALID_VALUE)	return	INVALID_VALUE;
			
			return	dH;
		}
	}
}

//////////////////////////////////////////////////////////////////////
// 24 获取DEM的高程值
//////////////////////////////////////////////////////////////////////
double* CDCR_GridDem::GetHeight()
{
	return	m_pDemObj;
}

//////////////////////////////////////////////////////////////////////
// 25 设置DEM的高程值
//////////////////////////////////////////////////////////////////////
void CDCR_GridDem::SetHeight(double *pHeight)
{
	if(pHeight == NULL)	return;

	if(m_iNumOfX == 0 || m_iNumOfY == 0)	return;
	
	if(m_pDemObj != NULL)
	{
		delete	[]m_pDemObj;
		m_pDemObj	= NULL;
	}

	m_pDemObj	= new	double[m_iNumOfX*m_iNumOfY];
	
	::memcpy(m_pDemObj,pHeight,sizeof(double)*m_iNumOfX*m_iNumOfY);
}

//////////////////////////////////////////////////////////////////////
// 26 设置Z轴比例
//////////////////////////////////////////////////////////////////////
void CDCR_GridDem::SetZScale(double dZScale)
{
	if(m_pDemObj == NULL)	return;

	for(int j=0;j<m_iNumOfY;j++)
	{
		for(int i=0;i<m_iNumOfX;i++)
		{
			m_pDemObj[j*m_iNumOfX+i]	= m_pDemObj[j*m_iNumOfX+i] * dZScale;
		}
	}
}

//////////////////////////////////////////////////////////////////////
// 27 撤销Z轴比例
//////////////////////////////////////////////////////////////////////
void CDCR_GridDem::UnSetZScale(double dZScale)
{
	if(m_pDemObj == NULL)	return;

	for(int j=0;j<m_iNumOfY;j++)
	{
		for(int i=0;i<m_iNumOfX;i++)
		{
			m_pDemObj[j*m_iNumOfX+i]	= m_pDemObj[j*m_iNumOfX+i] / dZScale;
		}
	}
}

//////////////////////////////////////////////////////////////////////
// 44 计算表面积
//////////////////////////////////////////////////////////////////////
double CDCR_GridDem::OnCalculateSurface(double& dV)
{
	//然后计算该点的粗糙度
	//表面积
	double	dSurferArea		= 0.0;
	
	//投影面积
	double	dProjectionArea	= 0.0;

	dV	= 0;

	int	I,J;
	for(J=0;J<m_iNumOfY-1;J++)
	{
		for(I=0;I<m_iNumOfX-1;I++)
		{
			// 计算格网的表面积，一律按照(I,J)和(I+1,J+1)来进行切分
			
			double	dTempZ;
			double	dTempA,dTempB,dTempC;
			
			//第一个半三角形
			// (I,J)	(I+1,J)
			dTempZ	= m_pDemObj[I+J*m_iNumOfX] - m_pDemObj[(I+1)+J*m_iNumOfX];
			dTempA	= sqrt(m_dSizeOfX*m_dSizeOfX + dTempZ*dTempZ);
			
			// (I+1,J)	(I+1,J+1)
			dTempZ	= m_pDemObj[(I+1)+J*m_iNumOfX] - m_pDemObj[(I+1)+(J+1)*m_iNumOfX];
			dTempB	= sqrt(m_dSizeOfY*m_dSizeOfY + dTempZ*dTempZ);
			
			// (I+1,J+1) (I,J)
			dTempZ	= m_pDemObj[(I+1)+(J+1)*m_iNumOfX] - m_pDemObj[I+J*m_iNumOfX];
			dTempC	= sqrt(m_dSizeOfX*m_dSizeOfX + m_dSizeOfY*m_dSizeOfY + dTempZ*dTempZ);
			
			//
			double	dTempP;
			dTempP	= (dTempA+dTempB+dTempC)/2;
			
			double	dS;
			dS		= sqrt(dTempP * (dTempP - dTempA) * (dTempP - dTempB) * (dTempP - dTempC));

			dSurferArea	= dSurferArea + dS;
			
			//第二个半三角形
			// (I,J)	(I,J+1)
			dTempZ	= m_pDemObj[I+J*m_iNumOfX] - m_pDemObj[(I+1)+J*m_iNumOfX];
			dTempA	= sqrt(m_dSizeOfY*m_dSizeOfY + dTempZ*dTempZ);
			
			// (I,J+1)	(I+1,J+1)
			dTempZ	= m_pDemObj[(I+1)+J*m_iNumOfX] - m_pDemObj[(I+1)+(J+1)*m_iNumOfX];
			dTempB	= sqrt(m_dSizeOfX*m_dSizeOfX + dTempZ*dTempZ);
			
			// (I,J+1) (I+1,J)
			dTempZ	= m_pDemObj[(I+1)+(J+1)*m_iNumOfX] - m_pDemObj[I+J*m_iNumOfX];
			dTempC	= sqrt(m_dSizeOfX*m_dSizeOfX + m_dSizeOfY*m_dSizeOfY + dTempZ*dTempZ);
			
			//
			dTempP	= (dTempA+dTempB+dTempC)/2;
			
			dS		= sqrt(dTempP * (dTempP - dTempA) * (dTempP - dTempB) * (dTempP - dTempC));

			dSurferArea		= dSurferArea + dS;		
			
			dProjectionArea	= dProjectionArea + m_dSizeOfX * m_dSizeOfY;

			dV		= dV + (m_dSizeOfX * m_dSizeOfY)*(m_pDemObj[I+J*m_iNumOfX] + m_pDemObj[I+(J+1)*m_iNumOfX] + m_pDemObj[(I+1)+J*m_iNumOfX] + m_pDemObj[(I+1)+(J+1)*m_iNumOfX])/4;
		}
	}

	return	dSurferArea;
}

//////////////////////////////////////////////////////////////////////
// 45 设置/获取DEM路径
//////////////////////////////////////////////////////////////////////
void CDCR_GridDem::SetPathName(CString szPathName)
{
	m_szPathName	= szPathName;
}
CString CDCR_GridDem::GetPathName()
{
	return	m_szPathName;
}

//////////////////////////////////////////////////////////////////////
// 46 设置/获取DEM显示状态
//////////////////////////////////////////////////////////////////////
void CDCR_GridDem::SetShowStatus(bool bShowStatus)
{
	m_bShowStatus	= bShowStatus;
}
bool CDCR_GridDem::GetShowStatus()
{
	return	m_bShowStatus;
}

//////////////////////////////////////////////////////////////////////
//地形抹平
//////////////////////////////////////////////////////////////////////
bool CDCR_GridDem::OnTerrainToZero()
{
	if(m_pDemObj == NULL)	return	false;
	
	int	i,j;
	for(j=0;j<m_iNumOfY;j++)
	{
		for(i=0;i<m_iNumOfX;i++)
		{
			double	dH;
			dH	= m_pDemObj[j*m_iNumOfX+i];
			
			if(dH < 0)	
			{
				m_pDemObj[j*m_iNumOfX+i]	= 0;
			}
		}
	}

	return	true;
}

//提取DEM中的部分数据
bool CDCR_GridDem::OnPickUpGrid(double dLeft,double dRight,double dBottom,double dTop,CString szPathName)
{
	if(m_pDemObj == NULL)	return	false;
	
	int		iXB,iXE,iYB,iYE;
	//
	iXB	= (int)( (dLeft-m_dDemLeft)/m_dSizeOfX ) -1;
	iXE	= (int)( (dRight-m_dDemLeft)/m_dSizeOfX ) + 1;

	iYB	= (int)( (dBottom-m_dDemBottom)/m_dSizeOfX ) -1;
	iYE	= (int)( (dTop-m_dDemBottom)/m_dSizeOfX ) + 1;

	//计算压盖了多少的DEM格网
	if(iXB < 0)				iXB = 0;
	if(iYB < 0)				iYB = 0;
	if(iXE < 0)				iXE = 0;
	if(iYE < 0)				iYE = 0;
	if(iXB >= m_iNumOfX)	iXB = m_iNumOfX;
	if(iYB >= m_iNumOfY)	iYB = m_iNumOfY;
	if(iXE >= m_iNumOfX)	iXE = m_iNumOfX;
	if(iYE >= m_iNumOfY)	iYE = m_iNumOfY;		
	
	//局部dem的值，先初始化
	double	*pSmallDemObj;
	int		iSmallNumOfX,iSmallNumOfY;
	double	dSmallLeft,dSmallRight;
	double	dSmallBottom,dSmallTop;

	iSmallNumOfX			= (iXE - iXB );
	iSmallNumOfY			= (iYE - iYB );

	if(iSmallNumOfX == 0 || iSmallNumOfY == 0)	return	false;
	
	dSmallLeft				= m_dDemLeft + iXB * m_dSizeOfX;
	dSmallRight				= dSmallLeft + (iSmallNumOfX-1) * m_dSizeOfX;
	dSmallBottom			= m_dDemBottom + iYB * m_dSizeOfY;
	dSmallTop				= dSmallBottom + (iSmallNumOfY-1) * m_dSizeOfY;

	pSmallDemObj			= new	double[iSmallNumOfX*iSmallNumOfY];

	int	i,j;
	int	II;
	double	dSmallMinH,dSmallMaxH;
	II		= 0;
	dSmallMinH	= dSmallMaxH	= 0;
	for(j=iYB;j<iYE;j++)
	{
		for(i=iXB;i<iXE;i++)
		{
			double	dH;
			dH	= m_pDemObj[j*m_iNumOfX+i];
			
			pSmallDemObj[II]	= dH;
			if(II==0)
			{
				dSmallMinH	= dSmallMaxH	= dH;
			}
			else
			{
				if(dSmallMinH > dH)	dSmallMinH	= dH;
				if(dSmallMaxH < dH)	dSmallMaxH	= dH;
			}

			II++;
		}
	}
	
	//保存
	{
		//假如数据为NULL
		if(pSmallDemObj == NULL)	return	false;

		FILE	*fp;
		fp	= fopen(szPathName,"w");

		if(fp != NULL)
		{
			//DSAA
			fprintf(fp,"DSAA\n");

			//iCols,iRows,应该判断清楚什么是行和列
			//先列后行
			fprintf(fp,"%ld %ld\n",iSmallNumOfX,iSmallNumOfY);
			
			//dMinX,dMaxX
			fprintf(fp,"%lf %lf\n",dSmallLeft,dSmallRight);

			//dMinY,dMaxY
			fprintf(fp,"%lf %lf\n",dSmallBottom,dSmallTop);
			
			//dMinZ,dMaxZ
			fprintf(fp,"%lf %lf\n",dSmallMinH,dSmallMaxH);
			
			//
			int		iCount;
			for(j=0;j<iSmallNumOfY;j++)
			{
				iCount	= 0;
				for(i=0;i<iSmallNumOfX;i++)
				{
					double	dH;
					dH	= pSmallDemObj[j*iSmallNumOfX+i];

					fprintf(fp,"%.0lf ",dH);

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
	}

	delete	[]pSmallDemObj;

	return	true;
}