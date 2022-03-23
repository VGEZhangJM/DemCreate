// DCR_Grid_Fractal.cpp: implementation of the CDCR_Grid_Fractal class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "vr_demcreator.h"
#include "DCR_Grid_Fractal.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDCR_Grid_Fractal::CDCR_Grid_Fractal()
{
	m_iDemNumX		= 501;
	m_iDemNumY		= 501;

	m_dDemSizeX		= 30;
	m_dDemSizeY		= 30;

	m_pHeight		= NULL;
}

CDCR_Grid_Fractal::~CDCR_Grid_Fractal()
{
	if(m_pHeight != NULL)
	{
		delete	[]m_pHeight;
		m_pHeight	= NULL;
	}
}

//2:设置码尺
void CDCR_Grid_Fractal::SetSize(double dSize)
{
	m_dDemSizeX	= m_dDemSizeY	= dSize;
}

//3:计算通过码尺提取的DEM
double CDCR_Grid_Fractal::CalculateDem()
{
	//if(m_pGridDem != NULL)
	{
		double	dLeft,dRight,dBottom,dTop;
		
		//
		m_pGridDem.GetDemExt(dLeft,dRight,dBottom,dTop);

		m_iDemNumX	= (int)((dRight-dLeft)/m_dDemSizeX) + 1;
		m_iDemNumY	= (int)((dTop-dBottom)/m_dDemSizeY) + 1;

		dRight		= dLeft + m_dDemSizeX * (m_iDemNumX-1);
		dTop		= dBottom + m_dDemSizeY * (m_iDemNumY-1);
		//m_dDemSizeX	= (dRight-dLeft)/(m_iDemNumX-1);
		//m_dDemSizeY	= (dTop-dBottom)/(m_iDemNumY-1);
		
		if(m_pHeight != NULL)
		{
			delete	[]m_pHeight;
			m_pHeight	= NULL;
		}

		m_pHeight	= new	double[m_iDemNumX*m_iDemNumY];

		int	I,J;
		for(J=0;J<m_iDemNumY;J++)
		{
			for(I=0;I<m_iDemNumX;I++)
			{
				double	dX,dY;
				dX	= dLeft + I*m_dDemSizeX;
				dY	= dBottom + J*m_dDemSizeY;
				
				m_pHeight[I+J*m_iDemNumX]	= m_pGridDem.GetHeight(dX,dY);
			}	
		}
		
		//修正m_pGridDem，按照新有的修正
		//重新设置DEM区域
		m_pGridDem.SetDemExt(dLeft,dRight,dBottom,dTop);
		//
		m_pGridDem.SetNumOfX(m_iDemNumX);
		m_pGridDem.SetNumOfY(m_iDemNumY);

		//
		m_pGridDem.SetSizeOfX(m_dDemSizeX);
		m_pGridDem.SetSizeOfY(m_dDemSizeY);
		
		//
		m_pGridDem.SetHeight(m_pHeight);
	}
	
	return	m_dDemSizeX;
}

//
double CDCR_Grid_Fractal::OnCalculateSurface(double& dV)
{
	if(m_pHeight == NULL)	return	0.0;

	//然后计算该点的粗糙度
	//表面积
	double	dSurferArea		= 0.0;
	
	//投影面积
	double	dProjectionArea	= 0.0;

	//
	dV		= 0;

	int	I,J;
	for(J=0;J<m_iDemNumY-1;J++)
	{
		for(I=0;I<m_iDemNumX-1;I++)
		{
			// 计算格网的表面积，一律按照(I,J)和(I+1,J+1)来进行切分
			
			double	dTempZ;
			double	dTempA,dTempB,dTempC;
			
			//第一个半三角形
			// (I,J)	(I+1,J)
			dTempZ	= m_pHeight[I+J*m_iDemNumX] - m_pHeight[(I+1)+J*m_iDemNumX];
			dTempA	= sqrt(m_dDemSizeX*m_dDemSizeX + dTempZ*dTempZ);
			
			// (I+1,J)	(I+1,J+1)
			dTempZ	= m_pHeight[(I+1)+J*m_iDemNumX] - m_pHeight[(I+1)+(J+1)*m_iDemNumX];
			dTempB	= sqrt(m_dDemSizeY*m_dDemSizeY + dTempZ*dTempZ);
			
			// (I+1,J+1) (I,J)
			dTempZ	= m_pHeight[(I+1)+(J+1)*m_iDemNumX] - m_pHeight[I+J*m_iDemNumX];
			dTempC	= sqrt(m_dDemSizeX*m_dDemSizeX + m_dDemSizeY*m_dDemSizeY + dTempZ*dTempZ);
			
			//
			double	dTempP;
			dTempP	= (dTempA+dTempB+dTempC)/2;
			
			double	dS;
			dS		= sqrt(dTempP * (dTempP - dTempA) * (dTempP - dTempB) * (dTempP - dTempC));

			dSurferArea	= dSurferArea + dS;
			
			//第二个半三角形
			// (I,J)	(I,J+1)
			dTempZ	= m_pHeight[I+J*m_iDemNumX] - m_pHeight[(I+1)+J*m_iDemNumX];
			dTempA	= sqrt(m_dDemSizeY*m_dDemSizeY + dTempZ*dTempZ);
			
			// (I+1,J)	(I+1,J+1)
			dTempZ	= m_pHeight[(I+1)+J*m_iDemNumX] - m_pHeight[(I+1)+(J+1)*m_iDemNumX];
			dTempB	= sqrt(m_dDemSizeX*m_dDemSizeX + dTempZ*dTempZ);
			
			// (I,J+1) (I+1,J)
			dTempZ	= m_pHeight[(I+1)+(J+1)*m_iDemNumX] - m_pHeight[I+J*m_iDemNumX];
			dTempC	= sqrt(m_dDemSizeX*m_dDemSizeX + m_dDemSizeY*m_dDemSizeY + dTempZ*dTempZ);
			
			//
			dTempP	= (dTempA+dTempB+dTempC)/2;
			
			dS		= sqrt(dTempP * (dTempP - dTempA) * (dTempP - dTempB) * (dTempP - dTempC));

			dSurferArea		= dSurferArea + dS;		
			
			dProjectionArea	= dProjectionArea + m_dDemSizeX * m_dDemSizeY;

			dV		= dV + (m_dDemSizeX * m_dDemSizeY)*(m_pHeight[I+J*m_iDemNumX] + m_pHeight[I+(J+1)*m_iDemNumX] + m_pHeight[(I+1)+J*m_iDemNumX] + m_pHeight[(I+1)+(J+1)*m_iDemNumX])/4;
		}
	}

	return	dSurferArea;
}
