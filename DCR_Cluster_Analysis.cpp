// DCR_Cluster_Analysis.cpp: implementation of the CDCR_Cluster_Analysis class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "vr_demcreator.h"
#include "DCR_Cluster_Analysis.h"
#include "SetImportTxtDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDCR_Cluster_Analysis::CDCR_Cluster_Analysis()
{
	//这三个是必须存在的
	m_dFeature		= NULL;

	m_nFeatures		= 3;
	//离散点数
	m_iPointNum		= 0;

	//每一个点的聚类序号，初始可为0
	m_Cluster		= NULL;
	//类数目，一般为10
	m_nClusters		= 0;
	
	//距离值
	m_Variance		= NULL;
	//聚类中心,一般为3 × N
	m_Centroid		= NULL;
	//最小距离值
	m_SP			= -1.0;

	m_nMembers		= NULL;

	//
	m_Iteration		= 1;

	//
	m_dLeft			= 0;
	m_dRight		= 0;
	m_dBottom		= 0;
	m_dTop			= 0;

	m_crMaxColor	= RGB(255,0,0);
	m_crMinColor	= RGB(0,0,255);
}

CDCR_Cluster_Analysis::~CDCR_Cluster_Analysis()
{
	if(m_dFeature != NULL)		{	delete	[]m_dFeature;	m_dFeature		= NULL;		}

	if(m_Cluster != NULL)		{	delete	[]m_Cluster;	m_Cluster		= NULL;		}
	if(m_Variance != NULL)		{	delete	[]m_Variance;	m_Variance		= NULL;		}
	if(m_Centroid != NULL)		{	delete	[]m_Centroid;	m_Centroid		= NULL;		}

	if(m_nMembers != NULL)		{	delete	[]m_nMembers;	m_nMembers		= NULL;		}
}

//
void CDCR_Cluster_Analysis::GetMapExt(double& dLeft,double& dRight,double& dBottom,double& dTop)
{
	dLeft	= m_dLeft;
	dRight	= m_dRight;
	dBottom	= m_dBottom;
	dTop	= m_dTop;
}

//
bool CDCR_Cluster_Analysis::IsEmpty()
{
	if(m_dFeature == NULL)	return	true;

	return	false;
}

//
bool CDCR_Cluster_Analysis::OnImport(CString szPathName)
{
	//
	m_szPathName	= szPathName;
	
	//计算有多少个记录
	m_iPointNum	= 0;
	{
		FILE	*fp;
		fp	= fopen(m_szPathName,"r");
		if(fp == NULL)	return	false;

		//
		char	chName[32];
		fscanf(fp,"%s%s%s",chName,chName,chName);
		

		double	dX,dY,dZ;
		while(fscanf(fp,"%lf%lf%lf",&dX,&dY,&dZ) == 3)
		{
			//
			m_iPointNum++;
		}

		fclose(fp);
	}
	
	if(m_iPointNum > 0)
	{
		FILE	*fp;
		fp	= fopen(m_szPathName,"r");
		if(fp == NULL)	return	false;

		//
		char	chName[32];
		fscanf(fp,"%s%s%s",chName,chName,chName);

		//
		if(m_dFeature != NULL)	{	delete	[]m_dFeature;	m_dFeature	= NULL;		}
		
		m_dFeature	= new	double[m_iPointNum*m_nFeatures];
		
		double	dX,dY,dZ;
		int		i;
		i		= 0;
		while(fscanf(fp,"%lf%lf%lf",&dX,&dY,&dZ) == 3)
		{
			m_dFeature[i*m_nFeatures+0]		= dX;
			m_dFeature[i*m_nFeatures+1]		= dY;
			m_dFeature[i*m_nFeatures+2]		= dZ;

			if(i==0)
			{
				m_dLeft		= m_dRight	= dX;
				m_dBottom	= m_dTop	= dY;
			}
			else
			{
				if(m_dLeft > dX)	m_dLeft		= dX;
				if(m_dRight < dX)	m_dRight	= dX;

				if(m_dBottom > dY)	m_dBottom	= dY;
				if(m_dTop < dY)		m_dTop		= dY;
			}
			
			i++;
		}
		
		fclose(fp);
	}

	return	true;
}


//
void CDCR_Cluster_Analysis::SetClientRect(CRect rRect)
{
	m_rRect	= rRect;
}

//
void CDCR_Cluster_Analysis::SetCoordMapping(double dXA,double dXB,double dYA,double dYB)
{
	m_dMapXA	= dXA;
	m_dMapXB	= dXB;
	m_dMapYA	= dYA;
	m_dMapYB	= dYB;
}

//
COLORREF CDCR_Cluster_Analysis::GetColor(int iCluster)
{
	/*
	double		dHR,dHG,dHB;
	double		dLR,dLG,dLB;
	int			iR,iG,iB;

	dHR	= GetRValue(m_crMaxColor);
	dHG	= GetGValue(m_crMaxColor);
	dHB	= GetBValue(m_crMaxColor);

	dLR	= GetRValue(m_crMinColor);
	dLG	= GetGValue(m_crMinColor);
	dLB	= GetBValue(m_crMinColor);

	iR	= (int)((dHR - dLR)/m_nClusters*iCluster);
	iG	= (int)((dHG - dLG)/m_nClusters*iCluster);
	iB	= (int)((dHB - dLB)/m_nClusters*iCluster);

	return	RGB(iR,iG,iB);
	*/

	if(iCluster == 0)	return	RGB(0,0,255);
	if(iCluster == 1)	return	RGB(255,0,255);
	if(iCluster == 2)	return	RGB(120,0,255);
	if(iCluster == 3)	return	RGB(0,255,255);
	if(iCluster == 4)	return	RGB(0,255,0);
	if(iCluster == 5)	return	RGB(120,255,255);
	if(iCluster == 6)	return	RGB(120,255,0);
	if(iCluster == 7)	return	RGB(0,0,120);
	if(iCluster == 8)	return	RGB(0,120,0);
	if(iCluster == 9)	return	RGB(255,0,0);
	return	RGB(0,0,255);
}

void CDCR_Cluster_Analysis::Draw(CDC *pDC,int iElement)
{
	if(m_dFeature == NULL)	return;
	
	double		dX,dY;
	COLORREF	crColor;
	int			iX,iY;

	dX	= m_dFeature[iElement*m_nFeatures + 0];
	dY	= m_dFeature[iElement*m_nFeatures + 1];

	//
	if(m_Cluster == NULL)	crColor	= RGB(0,0,255);
	else	crColor	= GetColor(m_Cluster[iElement]);
	
	iX	= (int)(dX*m_dMapXA + m_dMapXB + 0.5);
	iY	= (int)(dY*m_dMapYA + m_dMapYB + 0.5);
	
	pDC->SetPixel(iX,iY,crColor);
}

//
void CDCR_Cluster_Analysis::Draw(CDC *pDC)
{
	if(m_dFeature == NULL)	return;
	
	double	dShowLeft,dShowRight,dShowBottom,dShowTop;
	//计算实际的显示区域
	dShowLeft	= (m_rRect.left - m_dMapXB) / m_dMapXA;
	dShowRight	= (m_rRect.right - m_dMapXB) / m_dMapXA;
	dShowTop	= (m_rRect.top - m_dMapYB) / m_dMapYA;
	dShowBottom	= (m_rRect.bottom - m_dMapYB) / m_dMapYA;
	
	double		dX,dY;
	COLORREF	crColor;
	int			iX,iY;

	for(int i=0;i<m_iPointNum;i++)
	{
		dX	= m_dFeature[i*m_nFeatures + 0];
		dY	= m_dFeature[i*m_nFeatures + 1];

		if((dX>dShowLeft&&dX<dShowRight) && (dY>dShowBottom&&dY<dShowTop))
		{
			//
			if(m_Cluster == NULL)	crColor	= RGB(0,0,255);
			else	crColor	= GetColor(m_Cluster[i]);
			
			iX	= (int)(dX*m_dMapXA + m_dMapXB + 0.5);
			iY	= (int)(dY*m_dMapYA + m_dMapYB + 0.5);
			
			pDC->SetPixel(iX,iY,crColor);
		}
	}
}

//
bool CDCR_Cluster_Analysis::OnExecute(CDC *pDC, int Method, int nClusters)
{
	//初始化
	if(m_dFeature == NULL)	return	false;

	m_nClusters	= nClusters;
	
	if(m_Cluster != NULL)	{	delete	[]m_Cluster;	m_Cluster	= NULL;	}
	m_Cluster	= new	int[m_iPointNum];
	for(int i=0;i<m_iPointNum;i++)
	{
		m_Cluster[i]	= i;
	}

	//
	if(m_nMembers != NULL)	{	delete	[]m_nMembers;	m_nMembers	= NULL;	}
	m_nMembers	= new	int[m_nClusters];

	//
	if(m_Variance != NULL)	{	delete	[]m_Variance;	m_Variance	= NULL;	}
	m_Variance	= new	double[m_nClusters];
	
	//
	if(m_Centroid != NULL)	{	delete	[]m_Centroid;	m_Centroid	= NULL;	}
	m_Centroid	= new	double[m_nClusters * m_nFeatures];
	
	//
	Minimum_Distance(pDC, true);
	
	//显示结果
	CString	szBox;
	for( i=0;i<m_nClusters;i++)
	{
		CString	szInfo;
		szInfo.Format("类号%6ld  点数 %6ld 方差 %10.4lf ",i,m_nMembers[i],m_Variance[i]);
		
		for(int j=0;j<m_nFeatures;j++)
		{
			CString	szTemp;
			szTemp.Format(" %10.4lf",m_Centroid[i*m_nFeatures+j]);

			szInfo	= szInfo + szTemp;
		}
		
		szInfo	= szInfo + "\n";
		szBox	= szBox + szInfo;
	}

	AfxMessageBox(szBox);

	return	true;
}

//---------------------------------------------------------
bool CDCR_Cluster_Analysis::Minimum_Distance(CDC *pDC, bool bInitialize)
{
	bool	bContinue;
	int		iElement, iFeature, iCluster, nShifts;
	double	SP_Last	= -1.0;

	//-----------------------------------------------------
	for(iElement=0; iElement<m_iPointNum; iElement++)
	{
		iCluster	= m_Cluster[iElement];

		if( bInitialize || iCluster < 0 || iCluster >= m_nClusters )
		{
			m_Cluster[iElement]	= iCluster = iElement % m_nClusters;

			//
			Draw(pDC,iElement);
		}
	}

	//-----------------------------------------------------
	//判断循环终止的条件，等会仔细看看
	for(m_Iteration=1, bContinue=true; bContinue; m_Iteration++)
	{
		//初始化
		for(iCluster=0; iCluster<m_nClusters; iCluster++)
		{
			m_Variance[iCluster]	= 0.0;
			m_nMembers[iCluster]	= 0;

			//属性值也要初始化，相当于是聚类中心，是每一属性的
			for(iFeature=0; iFeature<m_nFeatures; iFeature++)
			{
				m_Centroid[iCluster*m_nFeatures + iFeature]	= 0.0;
			}
		}

		//-------------------------------------------------
		for(iElement=0; iElement<m_iPointNum; iElement++)
		{
			if( (iCluster = m_Cluster[iElement]) >= 0 )
			{
				m_nMembers[iCluster]++;

				for(iFeature=0; iFeature<m_nFeatures; iFeature++)
				{
					m_Centroid[iCluster*m_nFeatures + iFeature]	+= m_dFeature[iElement*m_nFeatures + iFeature];
				}
			}
		}

		//-------------------------------------------------
		for(iCluster=0; iCluster<m_nClusters; iCluster++)
		{
			double	d	= m_nMembers[iCluster] > 0 ? 1.0 / m_nMembers[iCluster] : 0.0;

			for(iFeature=0; iFeature<m_nFeatures; iFeature++)
			{
				m_Centroid[iCluster*m_nFeatures + iFeature]	*= d;
			}
		}

		//-------------------------------------------------
		for(iElement=0, m_SP=0.0, nShifts=0; iElement<m_iPointNum && bContinue; iElement++)
		{
			double	minVariance	= -1.0;
			int		minCluster	= -1;

			for(iCluster=0; iCluster<m_nClusters; iCluster++)
			{
				double	iVariance	= 0.0;

				for(iFeature=0; iFeature<m_nFeatures; iFeature++)
				{
					iVariance	+= Get_Square(m_Centroid[iCluster*m_nFeatures + iFeature] - m_dFeature[iElement*m_nFeatures + iFeature]);
				}

				if( minVariance < 0.0 || iVariance < minVariance )
				{
					minVariance	= iVariance;
					minCluster	= iCluster;
				}
			}

			if( m_Cluster[iElement] != minCluster )
			{
				m_Cluster[iElement]	= minCluster;
				nShifts++;

				Draw(pDC,iElement);
			}

			m_SP					+= minVariance;
			m_Variance[minCluster]	+= minVariance;
		}

		//-------------------------------------------------
		if( nShifts == 0 || (SP_Last >= 0 && m_SP >= SP_Last) )
		{
			bContinue	= false;
		}
		
		CString	szInfo;
		szInfo.Format("%s: %d >> %s %f","pass",m_Iteration,"change",m_Iteration <= 1 ? m_SP : SP_Last - m_SP);
	
		pDC->TextOut(0,0,szInfo);

		SP_Last		= m_SP;
	}

	return	 true;
}

void CDCR_Cluster_Analysis::OnRelease()
{
	if(m_dFeature != NULL)		{	delete	[]m_dFeature;	m_dFeature		= NULL;		}

	if(m_Cluster != NULL)		{	delete	[]m_Cluster;	m_Cluster		= NULL;		}
	if(m_Variance != NULL)		{	delete	[]m_Variance;	m_Variance		= NULL;		}
	if(m_Centroid != NULL)		{	delete	[]m_Centroid;	m_Centroid		= NULL;		}

	if(m_nMembers != NULL)		{	delete	[]m_nMembers;	m_nMembers		= NULL;		}
}
