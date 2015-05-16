#pragma once
#include "uilib/uilib_exports.h"

//SHELL ��ĳ�� CWndLessShellBase
template <class SHELL>  class CWndLessImpBase;


//IMPT ��ĳ�� CWndLessImpBase, ����Ĭ��ʵ��
template <class IMPT> 
UILIB_EXPORT IMPT *CreateDefaultImp( typename IMPT::BaseType *);


//��ģ��
template <class IT, class IMPT> 
class CWndLessShellBase: public IT
{
public: 
	typedef IT BaseType;
	typedef IMPT ImpType;

public:
	CWndLessShellBase ()
	{
		m_wlimp = CreateDefaultImp< IMPT >(this);
	}

	virtual ~CWndLessShellBase()
	{
		if (m_wlimp)
		{
			delete m_wlimp;
			m_wlimp = NULL;
		}
	}

protected:
	typename IMPT* m_wlimp;
};



//ʵ��ģ�ͣ� ÿ��ʵ�����Ͷ��̳�������һ���Ŀ�
template <class SHELL> 
class CWndLessImpBase: public SHELL
{
public:
	CWndLessImpBase () : m_wrapper(NULL)
	{
	}

	virtual ~CWndLessImpBase()
	{
	}

	virtual void SetWrapper(typename SHELL::BaseType * wrapper)
	{
		m_wrapper = wrapper;
		m_wlimp->SetWrapper(wrapper);
	}

protected:
	typename SHELL::BaseType * m_wrapper;

};

