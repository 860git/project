//#include "Stdafx.h"
//#include "GamePlaza.h"
//#include "GameFrame.h"
//#include "GlobalUnits.h"
//
////////////////////////////////////////////////////////////////////////////
//
//BEGIN_MESSAGE_MAP(CGamePlazaApp, CWinApp)
//END_MESSAGE_MAP()
//
////////////////////////////////////////////////////////////////////////////
//
////Ӧ�ó������˵��
//CGamePlazaApp theApp;
//
////���캯��
//CGamePlazaApp::CGamePlazaApp()
//{
//}
//
////��ʼ������
//BOOL CGamePlazaApp::InitInstance()
//{
//	__super::InitInstance();
//
//	//�ж��Ƿ�����
//#ifndef ADMIN_PLAZA
//#ifndef _DEBUG
//	bool bExistIGame=false;
//	CMutex Mutex(FALSE,MAIN_DLG_CLASSNAME,NULL);
//	if (Mutex.Lock(0)==FALSE) bExistIGame=true;
//	CWnd * pIGameWnd=CWnd::FindWindow(MAIN_DLG_CLASSNAME,NULL);
//	if (pIGameWnd!=NULL) 
//	{
//		bExistIGame=true;
//		if (pIGameWnd->IsIconic()) pIGameWnd->ShowWindow(SW_RESTORE);
//		pIGameWnd->SetActiveWindow();
//		pIGameWnd->BringWindowToTop();
//		pIGameWnd->SetForegroundWindow();
//	}
//	if (bExistIGame==true) return FALSE;
//#endif
//#endif
//
//	//�����ʼ��
//	AfxOleInit();
//	AfxInitRichEdit();
//	AfxInitRichEdit2();
//	InitCommonControls();
//	AfxEnableControlContainer();
//
//	//����ע���
//	SetRegistryKey(szSystemRegKey);
//
//	////���ؽ����
//	//CSkinAttribute::LoadSkinAttribute(QUERY_OBJECT_INTERFACE(m_SkinRecordFile,IUnknownEx));
//
//	//��������
//	g_GlobalOption.LoadOptionParameter();
//
//	//ȫ����Ϣ
//	if (g_GlobalUnits.InitGlobalUnits()==false)
//	{
//		ShowInformation(TEXT("��Ϸ�㳡��ʼ��ʧ�ܣ����򼴽��˳���"),0,MB_ICONSTOP);
//		return FALSE;
//	}
//
//	//��������
//	WNDCLASS WndClasss;
//	ZeroMemory(&WndClasss,sizeof(WndClasss));
//
//	//ע�ᴰ��
//	//WndClasss.style=CS_DBLCLKS;
//	//WndClasss.lpfnWndProc=DefWindowProc;
//	//WndClasss.lpszClassName=MAIN_DLG_CLASSNAME;
//	//WndClasss.hInstance=AfxGetInstanceHandle();
//	//WndClasss.hIcon=LoadIcon(MAKEINTRESOURCE(IDR_MAINFRAME));
//	//WndClasss.hCursor=LoadStandardCursor(MAKEINTRESOURCE(IDC_ARROW));
//	//if (AfxRegisterClass(&WndClasss)==FALSE) AfxThrowResourceException();
//
//	////��������
//	//CGameFrame * pMainFrame=new CGameFrame();
//	////pMainFrame->Create(MAIN_DLG_CLASSNAME,TEXT(""),WS_SYSMENU|WS_MINIMIZEBOX|WS_MAXIMIZEBOX|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,CRect(0,0,0,0));
//	//	pMainFrame->CreateEx(NULL, MAIN_DLG_CLASSNAME,TEXT(""),
//	//	WS_SYSMENU|WS_MINIMIZEBOX|WS_MAXIMIZEBOX|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,
//	//	0,0,0,0, NULL, NULL);
//	//m_pMainWnd=pMainFrame;
//
//
//
//	WndClasss.style=CS_DBLCLKS;
//	WndClasss.lpfnWndProc=DefWindowProc;
//	WndClasss.lpszClassName=MAIN_DLG_CLASSNAME;
//	WndClasss.hInstance=AfxGetInstanceHandle();
//	WndClasss.hIcon=LoadIcon(MAKEINTRESOURCE(IDR_MAINFRAME));
//	WndClasss.hCursor=LoadStandardCursor(MAKEINTRESOURCE(IDC_ARROW));
//	if (AfxRegisterClass(&WndClasss)==FALSE) AfxThrowResourceException();
//
//	/*m_pDispOM = new CImpIDispatch;
//	AfxEnableControlContainer(new CCustomOccManager); */
//
//	//��������
//	CGameFrame * pMainFrame=new CGameFrame();
//	pMainFrame->CreateEx(NULL, MAIN_DLG_CLASSNAME,TEXT(""),
//		WS_SYSMENU|WS_MINIMIZEBOX|WS_MAXIMIZEBOX|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,
//		0,0,0,0, NULL, NULL);
//	m_pMainWnd=pMainFrame;
//	return TRUE;
//
//}
//
////////////////////////////////////////////////////////////////////////////
//
#include "Stdafx.h"
#include "GamePlaza.h"
#include "GameFrame.h"
#include "GlobalUnits.h"
#include "SplashWnd.h"


//����GDI����
ULONG_PTR m_gdiplusToken;
GdiplusStartupInput  gdiplusStartupInput;

//////////////////////////////////////////////////////////////////////////
DETOUR_TRAMPOLINE(int   WINAPI SetScrollInfoT(HWND, int, LPCSCROLLINFO, BOOL), SetScrollInfo)
DETOUR_TRAMPOLINE(BOOL  WINAPI GetScrollInfoT(HWND, int, LPSCROLLINFO), GetScrollInfo)
DETOUR_TRAMPOLINE(int   WINAPI SetScrollPosT(HWND, int, int, BOOL), SetScrollPos)
DETOUR_TRAMPOLINE(int   WINAPI GetScrollPosT(HWND, int), GetScrollPos)
DETOUR_TRAMPOLINE(BOOL  WINAPI GetScrollRangeT(HWND, int, LPINT, LPINT), GetScrollRange)
DETOUR_TRAMPOLINE(BOOL  WINAPI SetScrollRangeT(HWND, int, int, int, BOOL), SetScrollRange)
DETOUR_TRAMPOLINE(BOOL  WINAPI ShowScrollBarT(HWND, int, BOOL), ShowScrollBar)
DETOUR_TRAMPOLINE(BOOL  WINAPI EnableScrollBarT(HWND, UINT, UINT), EnableScrollBar)

int WINAPI SetScrollInfoD(HWND hwnd, int fnBar, LPCSCROLLINFO lpsi, BOOL bRedraw)
{
	if( SkinSB_IsValid(hwnd) ) return SkinSB_SetScrollInfo(hwnd, fnBar, lpsi, bRedraw);
	else return SetScrollInfoT(hwnd, fnBar, lpsi, bRedraw);
}

BOOL WINAPI GetScrollInfoD(HWND hwnd, int fnBar, LPSCROLLINFO lpsi)
{
	if( SkinSB_IsValid(hwnd) ) return SkinSB_GetScrollInfo(hwnd, fnBar, lpsi);
	else return GetScrollInfoT(hwnd, fnBar, lpsi);
}

int WINAPI SetScrollPosD(HWND hwnd, int nBar, int nPos, BOOL bRedraw)
{
	if( SkinSB_IsValid(hwnd) ) return SkinSB_SetScrollPos(hwnd, nBar, nPos, bRedraw);
	else return SetScrollPosT(hwnd, nBar, nPos, bRedraw);
}

int WINAPI GetScrollPosD(HWND hwnd, int nBar)
{
	if( SkinSB_IsValid(hwnd) ) return SkinSB_GetScrollPos(hwnd, nBar);
	else return GetScrollPosT(hwnd, nBar);
}

BOOL WINAPI SetScrollRangeD(HWND hwnd, int nBar, int nMinPos, int nMaxPos, BOOL bRedraw)
{
	if( SkinSB_IsValid(hwnd) ) return SkinSB_SetScrollRange(hwnd, nBar, nMinPos, nMaxPos, bRedraw);
	else return SetScrollRangeT(hwnd, nBar, nMinPos, nMaxPos, bRedraw);
}

BOOL WINAPI GetScrollRangeD(HWND hwnd, int nBar, LPINT lpMinPos, LPINT lpMaxPos)
{
	if( SkinSB_IsValid(hwnd) ) return SkinSB_GetScrollRange(hwnd, nBar, lpMinPos, lpMaxPos);
	else return GetScrollRangeT(hwnd, nBar, lpMinPos, lpMaxPos);
}

BOOL WINAPI ShowScrollBarD(HWND hwnd, int nBar, BOOL bShow)
{
	if( SkinSB_IsValid(hwnd) ) return SkinSB_ShowScrollBar(hwnd, nBar, bShow);
	else return ShowScrollBarT(hwnd, nBar, bShow);
}

BOOL WINAPI EnableScrollBarD(HWND hwnd, UINT wSBflags, UINT wArrows)
{
	if( SkinSB_IsValid(hwnd) ) return SkinSB_EnableScrollBar(hwnd, wSBflags, wArrows);
	else return EnableScrollBarT(hwnd, wSBflags, wArrows);
}

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGamePlazaApp, CWinApp)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//Ӧ�ó������˵��
CGamePlazaApp theApp;

//���캯��
CGamePlazaApp::CGamePlazaApp()
{
	//_CrtSetBreakAlloc(123);
}

CGamePlazaApp::~CGamePlazaApp()
{
	SafeDelete(m_pDispOM);

	// ж�ع���
	DetourRemove((PBYTE)SetScrollInfoT, (PBYTE)SetScrollInfoD);
	DetourRemove((PBYTE)SetScrollPosT, (PBYTE)SetScrollPosD);
	DetourRemove((PBYTE)GetScrollInfoT, (PBYTE)GetScrollInfoD);
	DetourRemove((PBYTE)GetScrollPosT, (PBYTE)GetScrollPosD);
	DetourRemove((PBYTE)SetScrollRangeT, (PBYTE)SetScrollRangeD);
	DetourRemove((PBYTE)GetScrollRangeT, (PBYTE)GetScrollRangeD);
	DetourRemove((PBYTE)ShowScrollBarT, (PBYTE)ShowScrollBarD);
	DetourRemove((PBYTE)EnableScrollBarT, (PBYTE)EnableScrollBarD);
}

//��ʼ������
BOOL CGamePlazaApp::InitInstance()
{
	__super::InitInstance();
	VERIFY(GdiplusStartup(&m_gdiplusToken,&gdiplusStartupInput,NULL) == Ok);
	//�ж��Ƿ�����
#ifndef _DEBUG
// 	bool bExistIGame=false;
// 	CMutex Mutex(FALSE,MAIN_DLG_CLASSNAME,NULL);
// 	if (Mutex.Lock(0)==FALSE) bExistIGame=true;
// 	CWnd * pIGameWnd=CWnd::FindWindow(MAIN_DLG_CLASSNAME,NULL);
// 	if (pIGameWnd!=NULL) 
// 	{ 
// 		bExistIGame=true;
// 		if (pIGameWnd->IsIconic()) pIGameWnd->ShowWindow(SW_RESTORE);
// 		pIGameWnd->SetActiveWindow();
// 		pIGameWnd->BringWindowToTop();
// 		pIGameWnd->SetForegroundWindow();
// 	}
// 	if (bExistIGame==true) return FALSE;
#endif


	CFont TitleFont;
	CSkinMenu::SetSelectDisableMode(GetProfileInt(_T("SkinMenu"),_T("Selection"),1));
	CSkinMenu::SetXpBlending(GetProfileInt(_T("SkinMenu"),_T("Gloom"),1));
	if(TitleFont.CreatePointFont(140,_T("Comic Sans MS")))
		CSkinMenu::SetMenuTitleFont(&TitleFont);

	CSplashWnd::EnableSplashScreen(TRUE);

	// ���ع���
	DetourFunctionWithTrampoline((PBYTE)SetScrollInfoT, (PBYTE)SetScrollInfoD);
	DetourFunctionWithTrampoline((PBYTE)GetScrollInfoT, (PBYTE)GetScrollInfoD);
	DetourFunctionWithTrampoline((PBYTE)SetScrollPosT, (PBYTE)SetScrollPosD);
	DetourFunctionWithTrampoline((PBYTE)GetScrollPosT, (PBYTE)GetScrollPosD);
	DetourFunctionWithTrampoline((PBYTE)SetScrollRangeT, (PBYTE)SetScrollRangeD);
	DetourFunctionWithTrampoline((PBYTE)GetScrollRangeT, (PBYTE)GetScrollRangeD);
	DetourFunctionWithTrampoline((PBYTE)ShowScrollBarT, (PBYTE)ShowScrollBarD);
	DetourFunctionWithTrampoline((PBYTE)EnableScrollBarT, (PBYTE)EnableScrollBarD);


	//�����ʼ��
	AfxOleInit();
	AfxInitRichEdit();
	AfxInitRichEdit2();
	InitCommonControls();
	AfxEnableControlContainer();

	//����ע���
	SetRegistryKey(szSystemRegKey);

	////���ؽ����
	//CSkinAttribute::LoadSkinAttribute(QUERY_OBJECT_INTERFACE(m_SkinRecordFile,IUnknownEx));

	//��������
	g_GlobalOption.LoadOptionParameter();

	//ȫ����Ϣ
	if (g_GlobalUnits.InitGlobalUnits()==false)
	{
		ShowInformation(TEXT("��Ϸ�㳡��ʼ��ʧ�ܣ����򼴽��˳���"),0,MB_ICONSTOP);
		return FALSE;
	}

	//��������
	WNDCLASS WndClasss;
	ZeroMemory(&WndClasss,sizeof(WndClasss));

	//ע�ᴰ��
	WndClasss.style=CS_DBLCLKS;
	WndClasss.lpfnWndProc=DefWindowProc;
	WndClasss.lpszClassName=MAIN_DLG_CLASSNAME;
	WndClasss.hInstance=AfxGetInstanceHandle();
	WndClasss.hIcon=LoadIcon(MAKEINTRESOURCE(IDR_MAINFRAME));
	WndClasss.hCursor=LoadStandardCursor(MAKEINTRESOURCE(IDC_ARROW));
	if (AfxRegisterClass(&WndClasss)==FALSE) AfxThrowResourceException();

	m_pDispOM = new CImpIDispatch;
	AfxEnableControlContainer(new CCustomOccManager); 

	//��������
	CGameFrame * pMainFrame=new CGameFrame();
	pMainFrame->CreateEx(NULL, MAIN_DLG_CLASSNAME,TEXT(""),
		WS_SYSMENU|WS_MINIMIZEBOX|WS_MAXIMIZEBOX|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,
		0,0,0,0, NULL, NULL);
	m_pMainWnd=pMainFrame;

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

