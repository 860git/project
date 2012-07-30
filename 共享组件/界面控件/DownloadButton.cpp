// DownloadButton.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Resource.h"
#include "DownloadButton.h"


// CDownloadButton

IMPLEMENT_DYNAMIC(CDownloadButton, CSkinButton)
CDownloadButton::CDownloadButton( HWND hWnd ) : m_hWndMsg( hWnd) 
{
}

CDownloadButton::~CDownloadButton()
{
}


BEGIN_MESSAGE_MAP(CDownloadButton, CSkinButton)
END_MESSAGE_MAP()



// CDownloadButton ��Ϣ�������


LRESULT CDownloadButton::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: �ڴ����ר�ô����/����û���

	if ( message == WM_LBUTTONUP )
	{

		UINT id;
		id = GetDlgCtrlID();
	//	m_hWnd=GetParent()->GetSafeHwnd();

		if ( ::IsWindow(m_hWndMsg) == TRUE)
		{
			::SendMessage(m_hWndMsg,WM_LBUTTONUPU,id,0);
		}
	}

	return CSkinButton::DefWindowProc(message, wParam, lParam);
}