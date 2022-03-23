// DCR_HeightSet.h: interface for the CDCR_HeightSet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCR_HEIGHTSET_H__E504EEDF_19A6_481F_A9D3_32C11A621AB7__INCLUDED_)
#define AFX_DCR_HEIGHTSET_H__E504EEDF_19A6_481F_A9D3_32C11A621AB7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDCR_HeightSet  
{
public:
	CDCR_HeightSet();
	virtual ~CDCR_HeightSet();

	static void UnInstance();
	static CDCR_HeightSet* Instance();

	//
	COLORREF	GetColor(double dH);

protected:
	static	CDCR_HeightSet* m_pInstance;

	//把高程设置为9层
	int			m_iLayerCount;
	
	//那么高程分级为10
	double		m_dLayerHeight[18];

	//
	COLORREF	m_crLayerColor[18];
};

#endif // !defined(AFX_DCR_HEIGHTSET_H__E504EEDF_19A6_481F_A9D3_32C11A621AB7__INCLUDED_)
