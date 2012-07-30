#include "Stdafx.h"
#include "Resource.h"
#include "DlgStatus.h"
#include "GlobalUnits.h"

//��ʱ�� ID
#define IDT_STATUS_IMANGE					1					//״̬ͼ��
#define STATUS_IMAGE_COUNT					8					//״̬��Ŀ

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgStatus, CDialog)
	ON_BN_CLICKED(IDC_CLOSE, OnBnClickedClose)
	ON_WM_PAINT()
	ON_WM_TIMER()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CDlgStatus::CDlgStatus() : CDialog(IDD_STATUS)
{
	m_wImagePos=0;
	m_pNotifyWnd=NULL;
	PosX=20;
	return;
}

//��������
CDlgStatus::~CDlgStatus()
{
}

//�ؼ���
void CDlgStatus::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_CLOSE, m_btCancel);
}

//�ػ���Ϣ
void CDlgStatus::OnPaint()
{
	CPaintDC dc(this);

	//��ȡλ��
	CRect rcClientRect;
	GetClientRect(&rcClientRect);

	//�滭����
	CImage ImageBuffer;
	ImageBuffer.Create(rcClientRect.Width(),rcClientRect.Height(),24);
	CDC * pDCBuffer=CDC::FromHandle(ImageBuffer.GetDC());
	pDCBuffer->FillSolidRect(0,0,rcClientRect.Width(),rcClientRect.Height(),RGB(30,178,239)/*CSkinDialog::m_SkinAttribute.m_crBackGround*/);

// 	//�滭״̬
// 	CImage ImageStatus;
// 	ImageStatus.LoadFromResource(AfxGetInstanceHandle(),IDB_CONNECT_STATUS);
// 	CImage ImageStatus1;
// 	ImageStatus1.LoadFromResource(AfxGetInstanceHandle(),IDB_CONNECT_TIAO);
// 	CDC * pDCImage=CDC::FromHandle(ImageStatus.GetDC());
// 	AlphaDrawImage(pDCBuffer,0,0,rcClientRect.Width(),68,pDCImage,0,0,rcClientRect.Width(),68,RGB(255,0,255));
    
    
// 	CDC * pDCImage1=CDC::FromHandle(ImageStatus1.GetDC());
// 	AlphaDrawImage(pDCBuffer,PosX,36,75,22,pDCImage1,0,0,75,22,RGB(255,0,255));
//     
	//�滭��Ϣ
	CFont DrawFont;
	DrawFont.CreateFont(-12,0,0,0,400,0,0,0,134,3,2,1,2,TEXT("����"));
	CFont * pOldFont=pDCBuffer->SelectObject(&DrawFont);
	pDCBuffer->SetTextColor(RGB(0,0,0));
	pDCBuffer->SetBkMode(TRANSPARENT);
	pDCBuffer->TextOut(10,7,m_strMessage);
	pDCBuffer->SelectObject(pOldFont);
	DrawFont.DeleteObject();

	//�滭����
	ImageBuffer.BitBlt(dc,0,0);

	//������Դ
	//ImageStatus.ReleaseDC();
	ImageBuffer.ReleaseDC();
	//ImageStatus.Destroy();
	ImageBuffer.Destroy();

	return;
}

//��ʱ����Ϣ
void CDlgStatus::OnTimer(UINT nIDEvent)
{
	if (nIDEvent==IDT_STATUS_IMANGE)
	{
		CRect rcImageRect;
		rcImageRect.SetRect(0,0,373,68);
		m_wImagePos=(m_wImagePos+1)%STATUS_IMAGE_COUNT;
			if(PosX<=280)
	 PosX+=8;
	 else
      PosX=20;
		InvalidateRect(&rcImageRect,FALSE);
		return;
	}
	__super::OnTimer(nIDEvent);
}

//������Ϣ
bool CDlgStatus::ShowStatusMessage(LPCTSTR pszMessage, CWnd * pNotifyWnd)
{
	//��������
	if (m_hWnd==NULL)
	{
		Create(IDD_STATUS);
		SetTimer(IDT_STATUS_IMANGE,80,NULL);
	}

	//������Ϣ
	m_pNotifyWnd=pNotifyWnd;
	m_strMessage=pszMessage;
	Invalidate(FALSE);

	//��ʾ����
	ShowWindow(SW_SHOW);
	SetForegroundWindow();

	return true;
}

//���ٴ���
bool CDlgStatus::DestroyStatusWnd(CWnd * pNotifyWnd)
{
	if ((m_pNotifyWnd==pNotifyWnd)||(pNotifyWnd==NULL))
	{
		DestroyWindow();
		return true;
	}
	return false;
}

//ȡ����ť
void CDlgStatus::OnBnClickedClose()
{
	if (m_pNotifyWnd==NULL) return;
	if (IsWindow(m_pNotifyWnd->m_hWnd)==FALSE) return;
	m_pNotifyWnd->PostMessage(WM_COMMAND,IDM_CANCEL_CONNECT,0);
	return;
}

//////////////////////////////////////////////////////////////////////////
