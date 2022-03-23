// DCR_TinCreator.cpp: implementation of the CDCR_TinCreator class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DCR_TinCreator.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDCR_TinCreator::CDCR_TinCreator()
{
	m_dMapLeft		= 0;
	m_dMapRight		= 0;
	m_dMapBottom	= 0;
	m_dMapTop		= 0;
}

CDCR_TinCreator::~CDCR_TinCreator()
{

}

// 释放
void CDCR_TinCreator::Release()
{
	m_pTinTDTACreator.Release();

	m_pTinPITACreator.Release();
}

// 002 增加数据
void CDCR_TinCreator::AddVertex(int iType, double dX,double dY,double dZ)
{	
	//三角网生长法
	if(iType == 0)
		m_pTinTDTACreator.AddVertex(dX,dY,dZ);
	
	//逐点插入法
	if(iType == 1)
		m_pTinPITACreator.AddVertex(dX,dY,dZ);
}

//
void CDCR_TinCreator::SetCoordMapping(double dXA,double dXB,double dYA,double dYB)
{
	//
	m_pTinTDTACreator.SetCoordMapping(dXA,dXB,dYA,dYB);
	//
	m_pTinPITACreator.SetCoordMapping(dXA,dXB,dYA,dYB);
}

//
void CDCR_TinCreator::SetClientRect(CRect rRect)
{
	//
	m_pTinTDTACreator.SetClientRect(rRect);
	//
	m_pTinPITACreator.SetClientRect(rRect);
}

//
void CDCR_TinCreator::Draw(CDC *pDC)
{
	//
	m_pTinTDTACreator.Draw(pDC);
	//
	m_pTinPITACreator.Draw(pDC);
}

//
void CDCR_TinCreator::Create(CDC *pDC,int iType)
{
	//三角网生长法
	if(iType == 0)
	{
		DWORD dwStartTime = GetTickCount();

		m_pTinTDTACreator.Create(pDC);

		DWORD dwFinishTime = GetTickCount();

		CString	szInfo;
		szInfo.Format("三角网生长法构建三角网完毕，共耗时 : %dms",(int)(dwFinishTime-dwStartTime));

		MessageBox(NULL,szInfo,"提示信息",MB_OK);
	}

	//逐点插入法
	if(iType == 1)
	{
		DWORD dwStartTime = GetTickCount();

		m_pTinPITACreator.Create(pDC);

		DWORD dwFinishTime = GetTickCount();

		CString	szInfo;
		szInfo.Format("逐点插入法构建三角网完毕，共耗时 : %dms",(int)(dwFinishTime-dwStartTime));

		MessageBox(NULL,szInfo,"提示信息",MB_OK);
	}

	//
}

//
void CDCR_TinCreator::SetMapExt(double dLeft,double dRight,double dBottom,double dTop)
{
	//
	m_pTinTDTACreator.SetMapExt(dLeft,dRight,dBottom,dTop);
	//
	m_pTinPITACreator.SetMapExt(dLeft,dRight,dBottom,dTop);
}

void CDCR_TinCreator::GetMapExt(double& dLeft,double& dRight,double& dBottom,double& dTop)
{
	dLeft	= m_dMapLeft;
	dRight	= m_dMapRight;
	dBottom	= m_dMapBottom;
	dTop	= m_dMapTop;
}