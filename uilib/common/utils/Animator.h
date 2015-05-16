#pragma once

// �ȱ仯����
class UNIFORMANIMATOR
{
public:
	BOOL Init(int nBeginValue, int nEndValue, DWORD dwTotalCostTime) ;

	int Start() ;
	int Get() ;
	BOOL ReachEnd() ;

protected:
	int m_nBeginValue ;
	int m_nEndValue ;
	DWORD m_dwTotalCostTime ;

	DWORD m_dwBeginTime ;

};

// �ȼ��ٶ���
class CONSTACCANIMATOR
{
public:
	BOOL Init(int nBeginValue, int nEndValue, DWORD dwTotalCostTime, BOOL bSpeedUp) ;

	int Start() ;
	int Get() ;
	BOOL ReachEnd() ;

protected:
	int m_nBeginValue ;
	int m_nEndValue ;
	DWORD m_dwTotalCostTime ;
	BOOL m_bSpeedUp ;				// ��Ǽ��ٻ��Ǽ���

	DWORD m_dwBeginTime ;
	double m_dAcceleration ;

};
