// DCR_GridDem.h: interface for the CDCR_GridDem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCR_GRIDDEM_H__54FB971B_58EA_471D_88DD_FB742C821692__INCLUDED_)
#define AFX_DCR_GRIDDEM_H__54FB971B_58EA_471D_88DD_FB742C821692__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDCR_GridDem  
{
public:
	CDCR_GridDem();
	virtual ~CDCR_GridDem();

	//01 ��ȡDEM����
	bool	OnRead(char *chPathName);
	//02 ����DEM����
	bool	OnSave(char *chPathName);

	//03 ����DEM����
	bool	OnImport(char *chPathName);
	//04 ����DEM����
	bool	OnExport(char *chPathName);

	//11 �ͷ����ݣ�����DEM���Լ�����������������
	void	OnRelease();

	//12 ����DEM����
	void	OnClone(CDCR_GridDem *pDEM);

	//13 ������ʾ����
	void	SetClientRect(CRect rRect);

	//14 ����ӳ��
	void	SetCoordMapping(double dXA,double dXB,double dYA,double dYB);

	//15 ���Ƹ�����Ϣ�������¶�ͼ������ͼ����������ͼ��ƽ������ͼ����������ͼ��
	void	OnDraw(CDC *pDC);
	
	//16 �ж��Ƿ�Ϊ��
	bool	IsEmpty();

	//17 ��ȡ/���ú������
	int		GetNumOfX();
	void	SetNumOfX(int iNumX);

	//18 ��ȡ/�����������
	int		GetNumOfY();
	void	SetNumOfY(int iNumY);

	//19 ����/��ȡ������
	double	GetSizeOfX();
	void	SetSizeOfX(double dSizeX);
	
	//20 ����/��ȡ������
	double	GetSizeOfY();
	void	SetSizeOfY(double dSizeY);

	//
	double	GetRealSize();

	//21 ��ȡ/����DEM��Χ
	void	GetDemExt(double &dLeft,double &dRight,double &dBottom,double &dTop);
	void	SetDemExt(double dLeft,double dRight,double dBottom,double dTop);

	//22 ��ȡ��͡���߸߳�ֵ
	double	GetMaxHeight();
	double	GetMinHeight();

	//23 ͨ��DEM��ȡĳһ��ĸ߳�ֵ
	double	GetHeight(double dX,double dY);
	//24 ��ȡDEM�ĸ߳�ֵ
	double*	GetHeight();

	//25 ����DEM�ĸ߳�ֵ
	void	SetHeight(double *pHeight);

	//26 ����Z�����
	void	SetZScale(double dZScale);
	//27 ����Z�����
	void	UnSetZScale(double dZScale);
		
	//44 ��������
	double	OnCalculateSurface(double& dV);

	//45 ����/��ȡDEM·��
	void	SetPathName(CString szPathName);
	CString	GetPathName();

	//46 ����/��ȡDEM��ʾ״̬
	void	SetShowStatus(bool bShowStatus);
	bool	GetShowStatus();

	//����Ĩƽ
	bool	OnTerrainToZero();

	//��ȡDEM�еĲ�������
	bool	OnPickUpGrid(double dLeft,double dRight,double dBottom,double dTop,CString szPathName);
	
protected:
	//DEM�ķ�Χ
	double			m_dDemLeft;
	double			m_dDemRight;
	double			m_dDemBottom;
	double			m_dDemTop;
	
	//��ʾ����
	double			m_dMapXA;
	double			m_dMapXB;
	double			m_dMapYA;
	double			m_dMapYB;

	//
	CRect			m_rRect;

	//DEM�ĸ�����Ŀ
	long			m_iNumOfX;
	long			m_iNumOfY;

	//DEM�ĸ����ߴ�
	double			m_dSizeOfX;
	double			m_dSizeOfY;

	//DEM��ʵ�ʱȸ�,һ��Ϊ1
	double			m_dRealSize;

	//DEMֵ
	double*			m_pDemObj;
	//DEM��������ֵ
	double			m_dMinHeight;
	double			m_dMaxHeight;

	//DEM�ķ���ֵ
	double*			m_pDemSD;
	double			m_dMinSD;
	double			m_dMaxSD;

	//·��
	CString			m_szPathName;
	
	//��ʾ״̬
	bool			m_bShowStatus;
};

#endif // !defined(AFX_DCR_GRIDDEM_H__54FB971B_58EA_471D_88DD_FB742C821692__INCLUDED_)
