#pragma once
#include <string>
#include <map>
#include "uilib\uilib_exports.h"

namespace hdutils
{

//helper function

// ���¿���Ĭ��������д�ĳ����ַ
UILIB_EXPORT void OpenURL(const std::wstring & url);

//��explorer�д�һ���ļ���
UILIB_EXPORT void OpenFolder(const std::wstring &path);

// ���һ���û�ID�Ƿ�Ϸ�, ����ID���滯(trim����to_lower)
UILIB_EXPORT bool CheckUserID(std::wstring & wstrID);

// ���һ���û������Ƿ�Ϸ�
UILIB_EXPORT bool CheckPwd(const std::wstring & wstrPwd);

// ���һ���ַ��Ƿ���ڱ༭���б���ʾ
UILIB_EXPORT bool IsEditPrint(TCHAR c) ;

// ��ȡ��ǰ����·��
UILIB_EXPORT bool GetModuleFileName( std::wstring& fileName );

// ��ȡ��ǰ��������Ŀ¼
UILIB_EXPORT bool GetAppDirectory( std::wstring& moudleDir );


// �����ֵ������������
class UILIB_EXPORT DictionarySorter
{
public:
	bool operator () ( const std::wstring& lc, const std::wstring& rc);

} ;

// �ж�һ�������ַ���str�Ƿ�ƥ����ĳ�������ַ���match������֧��ƥ������ĸ
// ���� IsStringMarch(L"cs", L"����") = true
UILIB_EXPORT bool IsStringMarch( const std::wstring& match, const std::wstring& originString );

UILIB_EXPORT void SearchTarget(LPCWSTR lpszFilePath);
UILIB_EXPORT void SearchTarget2(LPCWSTR lpszFilePath);
UILIB_EXPORT void CopyToClipBoard(const std::wstring &wstrID); //copy��ճ����



//���ĳ��ϵͳĿ¼��ַ������:
//GetWindowsDir(CSIDL_LOCAL_APPDATA, dir);// dir = &appdata&
UILIB_EXPORT bool GetWindowsDir(int dirid,std::wstring &wstrPath);


// std::wstring GetStatusStr(LibIM::ContactStatus status);

UILIB_EXPORT std::wstring GetTimeString(time_t t);


class UILIB_EXPORT CPUPerformance
{
public:
	CPUPerformance(LPCTSTR lpszTag) ;
	~CPUPerformance() ;

	BOOL Start() ;
	BOOL Tick() ;

protected:
	LPCTSTR m_sTag ;

	LARGE_INTEGER m_liFrequency ;

	LARGE_INTEGER m_liStart ;

private:
};


//�û���Ϊͳ�ƽӿ�
void UILIB_EXPORT TrackUIOperation(HWND wnd, UINT message, WPARAM wParam, LPARAM lParam);
void UILIB_EXPORT GetUIOperationCounter( std::map<std::string , unsigned int> &counts);

} ;
