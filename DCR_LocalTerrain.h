// DCR_LocalTerrain.h: interface for the CDCR_LocalTerrain class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCR_LOCALTERRAIN_H__8EA8DB66_1558_48A2_B38B_BB4FCED5C960__INCLUDED_)
#define AFX_DCR_LOCALTERRAIN_H__8EA8DB66_1558_48A2_B38B_BB4FCED5C960__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDCR_LocalTerrain  
{
public:
	//
	void	OnRead(CString szPathName);
	
	//
	bool	OnSearchPoint(double dPointPattern,double dMeanSlope,double dH,int iNum = 5);

	//
	int		GetSelectNum();
	//
	bool	GetSelectPoint(int iSerial,double& dX,double& dY,double& dDH,double& dMaxDistance,double& dMinDistance);


	CDCR_LocalTerrain();
	virtual ~CDCR_LocalTerrain();

protected:
	//
	CString	m_szPathName;

	//
	CArray<tagLOCALTERRAIN,tagLOCALTERRAIN>	m_pData;
	//
	CArray<tagLOCALTERRAIN,tagLOCALTERRAIN>	m_pSelectData;
};

#endif // !defined(AFX_DCR_LOCALTERRAIN_H__8EA8DB66_1558_48A2_B38B_BB4FCED5C960__INCLUDED_)
