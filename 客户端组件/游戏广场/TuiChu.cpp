#include "Stdafx.h"
#include "resource.h"
#include "GlobalUnits.h"
#include "TuiChu.h"


//////////////////////////////////////////////////////////////////////////

//��ʱ��I D
#define IDI_CHARMVALUE_UPDATE_VIEW		1								//���½���
#define TIME_CHARMVALUE_UPDATE_VIEW		200								//���½���

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDTuiChu, CSkinDialogEx)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()	
	ON_BN_CLICKED(IDOK,OnBnClickedOk)   //ȷ��
	ON_BN_CLICKED(IDC_TUICHU_GOLG, OnChangOk1)
	
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CDTuiChu::CDTuiChu(CWnd* pParent):CSkinDialogEx(IDD_QUIT, pParent)
{

}

//��������
CDTuiChu::~CDTuiChu()
{
}
void CDTuiChu::OnSize(UINT nType, int cx, int cy) 
{
	__super::OnSize(nType, cx, cy);

//	CRgn m_rgn;
// CRect rc;
// GetWindowRect(&rc); //��ô��ھ���
// rc -= rc.TopLeft();
//m_rgn.CreateRoundRectRgn(rc.left, rc.top, rc.right+1, rc.bottom+1, 5, 5); //���ݴ��ھ��δ���һ��Բ�Ǿ���
//SetWindowRgn(m_rgn,true);

// 	CDC* dc=GetDC();
//// 
//// 	//��ȡλ��
//// 	CRect rcClient;
//// 	GetClientRect(&rcClient);
// 	//����ͼ��
// 	HICON hIcon=LoadIcon(NULL,IDI_QUESTION);
// 
//	//�滭ͼ��
// 	DrawIconEx(dc->m_hDC,50,50,hIcon,32,32,0,NULL,DI_NORMAL);
// 	AfxMessageBox("xxxx");

	return;
}
//�ؼ���
void CDTuiChu::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btOK);
	 DDX_Control(pDX, IDC_TUICHU_GOLG, m_btTQ);
	 DDX_Control(pDX, IDCANCLE, m_btCancel);

}
//��ȡ��ť
void CDTuiChu::OnBnClickedOk()
{
	
	AfxGetMainWnd()->PostMessage(WM_CLOSE,0,0);
    //AfxMessageBox("��������");
//	AfxGetMainWnd()->PostMessage(IDC_BT_SWITCH_ACCOUNTS,0,0);
	//CGameFrame *pGameFrame = (CGameFrame *)AfxGetMainWnd() ;
	//pGameFrame->QieHuanZhangHao();
	//ShowWindow(SW_HIDE);
   return;
}
void CDTuiChu::OnChangOk1()
{
	
	//AfxGetMainWnd()->PostMessage(WM_CLOSE,0,0);
//	AfxGetMainWnd()->PostMessage(IDC_BT_SWITCH_ACCOUNTS,0,0);
	CGameFrame *pGameFrame = (CGameFrame *)AfxGetMainWnd() ;
	pGameFrame->QieHuanZhangHao();
	ShowWindow(SW_HIDE);
   return;
}

void CDTuiChu::SetPurchaseInfoSink(IPurchaseInfo *pIPurchaseInfo)
{
   return;
}

	//��ȡ��Ť
 void CDTuiChu::OnBnTiQuOk()             
 {
   return;
 }
	//���밴ť 
 void CDTuiChu::OnBnCunRuOk()
 {  
   return;
 }
	//ת�˰�ť 
 void CDTuiChu::OnZhuanZhangOk()
 {
   return;
 }
	//��������
 void CDTuiChu::OnPassBankOk()
 {

 return;
 }

//��ʼ������
BOOL CDTuiChu::OnInitDialog()
{
	CSkinDialogEx::OnInitDialog();
	
	//m_btOK.SetButtonImage(IDB_GAME_TUICHUI,AfxGetInstanceHandle(),false);
	//m_btTQ.SetButtonImage(IDB_GAME_QIEHUAN  ,AfxGetInstanceHandle(),false);
	//m_btTQ.SetWindowText("xxxx");
	SetWindowText("�˳�ȷ��");
	return TRUE;
}

//���½���
void CDTuiChu::UpdateView()
{

}

//�����Ϣ
void CDTuiChu::OnLButtonDown(UINT nFlags, CPoint point)
{
	CSkinDialogEx::OnLButtonDown(nFlags, point);
}

//�����Ϣ
void CDTuiChu::OnMouseMove(UINT nFlags, CPoint point)
{
	CSkinDialogEx::OnMouseMove(nFlags, point);
}

//������Ϣ
BOOL CDTuiChu::PreTranslateMessage(MSG* pMsg)
{

	////���˻س�
	//if(pMsg->message==WM_KEYDOWN)
	//{
	//	BYTE bMesValue = (BYTE)(pMsg->wParam);
	//	if(bMesValue==VK_RETURN) return TRUE;
	//}

	return __super::PreTranslateMessage(pMsg);
}

//ʱ����Ϣ
void CDTuiChu::OnTimer(UINT nIDEvent)
{

	__super::OnTimer(nIDEvent);
}

//������Ϣ
void CDTuiChu::OnEnChangeInCount()
{


	return ;
}

void CDTuiChu::OnEnChangeInID()
{

	return ;
}

//�Ƿ�ѡ��
bool CDTuiChu::IsButtonSelected(UINT uButtonID)
{
	return ((CButton *)GetDlgItem(uButtonID))->GetCheck()==BST_CHECKED;
}

void CDTuiChu::DrawTextString()
{
	
	return;
}
//ѡ�а�ť
void CDTuiChu::SetButtonSelected(UINT uButtonID, bool bSelected)
{
	
	return;
}

//������Ϣ
void CDTuiChu::SetSendInfo(IClientKernel *pClientKernel,ITCPSocket *pClientSocket)
{
	
    
	return;
}

//������Ϣ
void CDTuiChu::SendData(WORD wMainCmdID, WORD wSubCmdID, void * pBuffer, WORD wDataSize)
{

	

	return ;
}

bool CDTuiChu::ConnectServer()
{
	


	return true;
}

void CDTuiChu::OnPaint()
{

	CPaintDC dc(this);

	//�滭����
	DrawCaption(&dc);
	DrawBackGround(&dc);
	DrawBorder(&dc);

	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);
	//����ͼ��
	HICON hIcon=LoadIcon(NULL,IDI_QUESTION);

	//�滭ͼ��
	DrawIconEx(dc,10,35,hIcon,32,32,0,NULL,DI_NORMAL);
	//DrawSkinView(&dc);
	 //CSkinDialogEx::OnPaint();
	DrawBackGround(&dc);

	CRect rcClient1;
	GetWindowRect(&rcClient1);
	int nWidth1=rcClient1.Width();
	int nHeight1=rcClient1.Height();
	//�����ж�
	if ((nWidth1!=0)&&(nHeight1!=0))
	{
		//���λ��
		CRect rcFrame;
		rcFrame.SetRect(0,0,nWidth1,nHeight1);

		//��������
		CRgn RegionWindow;
		RegionWindow.CreateRoundRectRgn(rcFrame.left,rcFrame.top,rcFrame.right+1,rcFrame.bottom+1,5,5);

		//��������
		SetWindowRgn(RegionWindow,TRUE);
	}

	return;
}

//////////////////////////////////////////////////////////////////////////

//�һ�����


//////////////////////////////////////////////////////////////////////////

