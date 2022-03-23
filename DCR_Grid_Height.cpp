// DCR_Grid_Height.cpp: implementation of the CDCR_Grid_Height class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "vr_demcreator.h"
#include "DCR_Grid_Height.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDCR_Grid_Height::CDCR_Grid_Height()
{
	//
	m_iNumOfX		= 0;
	m_iNumOfY		= 0;

	//
	m_bNewValue		= true;

	//
	m_dMaxHeight	= 0;
	m_dMinHeight	= 0;

	//
	m_pHeight		= NULL;
}

CDCR_Grid_Height::~CDCR_Grid_Height()
{
	Release();
}

//
void CDCR_Grid_Height::SetHeight(double dH)
{
	if(m_bNewValue)
	{
		m_dMaxHeight	= dH;
		m_dMinHeight	= dH;

		m_bNewValue		= false;
	}
	else
	{
		if(m_dMinHeight > dH)	m_dMinHeight	= dH;
		if(m_dMaxHeight < dH)	m_dMaxHeight	= dH;
	}
}

//
void CDCR_Grid_Height::Release()
{
	if(m_pHeight != NULL)
	{
		delete	[]m_pHeight;
		m_pHeight	= NULL;
	}

	//
	m_iNumOfX		= 0;
	m_iNumOfY		= 0;

	//
	m_bNewValue		= true;

	//
	m_dMaxHeight	= 0;
	m_dMinHeight	= 0;

	m_szPathName.Empty();
}
