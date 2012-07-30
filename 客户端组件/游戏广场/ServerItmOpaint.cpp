#include "Stdafx.h"
#include "Resource.h"
#include "GlobalUnits.h"
#include "ServerItmOpaint.h"

//////////////////////////////////////////////////////////////////////////

//��ɫ����
#define CR_TEXT						RGB(255,255,255)					//������ɫ
#define CR_BACKGROUP				RGB(255,232,160)					//������ɫ

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CServerItmOpaint, CDialog)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CServerItmOpaint::CServerItmOpaint() : CDialog(IDD_SERVER_INFO)
{
	//״̬����
	m_nTitleHeight=0;
	m_bShowInfo=false;
	m_bCreateFlag=false;

	return;
}

//��������
CServerItmOpaint::~CServerItmOpaint()
{
}

//�ؼ���
VOID CServerItmOpaint::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
}

//��ʼ������
BOOL CServerItmOpaint::OnInitDialog()
{
	__super::OnInitDialog();

	//������Դ
	UpdateSkinResource();

	//��ȡ��С
	CImageHandle HandleTitleM(&m_ImageTitleM);
	m_nTitleHeight=m_ImageTitleM.GetHeight();

	//�ƶ�����
	SetWindowPos(NULL,0,0,0,m_nTitleHeight,SWP_NOMOVE|SWP_NOZORDER);

	//���ñ���
	m_bCreateFlag=true;

	return TRUE;
}

//����߶�
UINT CServerItmOpaint::GetTitleHeight()
{
	return m_nTitleHeight;
}

//������Ϣ
VOID CServerItmOpaint::UpdateUserInfo()
{
	//���½���
	Invalidate();

	return;
}

//��ʾ��Ϣ
VOID CServerItmOpaint::ShowUserInfo(bool bShowInfo)
{
	//״̬�ж�
	if (m_bShowInfo==bShowInfo) return;

	//���ñ���
	m_bShowInfo=bShowInfo;

	//���½���
	Invalidate();

	return;
}

//������Դ
bool CServerItmOpaint::UpdateSkinResource()
{
	//��ȡ��Դ
	tagControlViewImage & ControlViewImage=g_GlobalUnits.m_ControlViewImage;
	HINSTANCE hInstance=g_GlobalUnits.m_PlatformResourceModule->GetResInstance();

	m_ImageTitleL.SetLoadInfo(ControlViewImage.uUserInfoL,hInstance);
	m_ImageTitleM.SetLoadInfo(ControlViewImage.uServerIfnoM,hInstance);
	m_ImageTitleR.SetLoadInfo(ControlViewImage.uServerIfnoB,hInstance);

	return true;
}

//�滭����
BOOL CServerItmOpaint::OnEraseBkgnd(CDC * pDC)
{
	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//��������
	CImage ImageBuffer;
	ImageBuffer.Create(rcClient.Width(),rcClient.Height(),24);

	//���� DC
	CDC BufferDC;
	BufferDC.CreateCompatibleDC(pDC);
	BufferDC.SelectObject(ImageBuffer);

	//������Դ
	CImageHandle HandleTitleL(&m_ImageTitleL);
	CImageHandle HandleTitleM(&m_ImageTitleM);
	CImageHandle HandleTitleR(&m_ImageTitleR);

	//�滭����
	//m_ImageTitleL.BitBlt(BufferDC,0,0);

	//����м�
	for (INT nXPos=0;nXPos<rcClient.Height()-m_ImageTitleR.GetHeight();nXPos+=m_ImageTitleM.GetHeight())
	{
		m_ImageTitleM.BitBlt(BufferDC,0,nXPos);
	}
	//�滭����
	m_ImageTitleR.BitBlt(BufferDC,0,rcClient.Height()-m_ImageTitleR.GetHeight());
    
	//���� DC
	BufferDC.SetTextColor(CR_TEXT);
	BufferDC.SetBkMode(TRANSPARENT);
	BufferDC.SelectObject(CSkinResourceManager::GetDefaultFont());

	//�û���Ϣ
	if (m_bShowInfo==true)
	{
		//��������
		tagGlobalUserData & GlobalUserInfo=g_GlobalUnits.GetGolbalUserData();

		//�û�ͷ��
		g_GlobalUnits.m_UserFaceRes->DrawNormalFace(&BufferDC,10,12,GlobalUserInfo.cbGender,GlobalUserInfo.dwUserID,GlobalUserInfo.dwCustomFaceVer,32,32);

		//����λ��
		TCHAR szBuffer[255]=TEXT("");
		CRect rcAccounts(50,15,rcClient.Width()-20,27);
		CRect rcUnderWrite(50,32,rcClient.Width()-20,44);

		//�û��ʺ�
		_sntprintf(szBuffer,CountArray(szBuffer),TEXT("�û��ʺţ�%s [ %ld ]"),GlobalUserInfo.szAccounts,GlobalUserInfo.dwGameID);
		BufferDC.DrawText(szBuffer,lstrlen(szBuffer),&rcAccounts,DT_VCENTER|DT_WORD_ELLIPSIS|DT_SINGLELINE);

		//��Ա��Ϣ
		LPCTSTR pszMemberOrder[]={TEXT("�ǻ�Ա"),TEXT("�����Ա"),TEXT("�����Ա"),TEXT("�����Ա"),TEXT("�����Ա")};
		if (GlobalUserInfo.cbMember<CountArray(pszMemberOrder))
		{
			_sntprintf(szBuffer,CountArray(szBuffer),TEXT("��Ա�ȼ���%s"),pszMemberOrder[GlobalUserInfo.cbMember]);
			BufferDC.DrawText(szBuffer,lstrlen(szBuffer),&rcUnderWrite,DT_VCENTER|DT_WORD_ELLIPSIS|DT_SINGLELINE);
		}
	}
    //�滭����
	pDC->BitBlt(0,0,rcClient.Width(),rcClient.Height(),&BufferDC,0,0,SRCCOPY);

	//������Դ
	BufferDC.DeleteDC();
	ImageBuffer.Destroy();

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
