#include "stdafx.h"
#include"resource.h"
#include "DlgUserManager.h"
#include "GlobalUnits.h"


////////////////////////////////////////////////////////////////////////////////////
//�궨��
#define DEF_INSIDEBORDER_COLOR	RGB(176,20,1)						//Ĭ����ɫ
#define BGROUND_COLOR			RGB(218,222,223)

////////////////////////////////////////////////////////////////////////////////////


BEGIN_MESSAGE_MAP(CDlgUserManage, CSkinDialogEx1)
	ON_WM_PAINT()
	//ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_CLOSE, OnClose1)

	ON_BN_CLICKED(IDC_SAMPLE, OnSample)
	ON_BN_CLICKED(IDC_XX_INFO, OnXX)
	ON_BN_CLICKED(IDC_LOCK, OnLock)
	ON_BN_CLICKED(IDC_PASS, OnPass)
//	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE, OnSelchangedTree1)

END_MESSAGE_MAP()

//��������
void CDlgUserManage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_CLOSE, m_btOk);

	DDX_Control(pDX, IDC_SAMPLE, m_btSample);
	DDX_Control(pDX, IDC_XX_INFO, m_btXX);
	DDX_Control(pDX, IDC_LOCK, m_btLock);
	DDX_Control(pDX, IDC_PASS, m_btPass);
	
	//DDX_Control(pDX, IDC_TREE, m_tree);
	
}

////////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgUserManage::CDlgUserManage(): CSkinDialogEx1(IDD_USER_MANAGER)//, CSkinWndObject(this)
{
	if (!m_pngImageBack.IsNull())
	{
		m_pngImageBack.DestroyImage();
	}
	m_pngImageBack.LoadImage(szResPath("BankDlg\\BankDlgBack.png"));
}

//�鹹����
CDlgUserManage::~CDlgUserManage()
{
	if (m_brBkground.m_hObject != NULL ) m_brBkground.DeleteObject();
	if (!m_pngImageBack.IsNull())
	{
		m_pngImageBack.DestroyImage();
	}
}



//��ʼ����
BOOL CDlgUserManage::OnInitDialog()
{
	__super::OnInitDialog();

	//����ˢ��
	m_brBkground.CreateSolidBrush(BGROUND_COLOR);

	//��������
	SetWindowPos(NULL, 0,0,m_pngImageBack.GetWidth(), m_pngImageBack.GetHeight(),SWP_NOZORDER);
	CenterWindow();
	CRect rcClient;
	GetWindowRect(&rcClient);
	ScreenToClient(&rcClient);

	//�������
	CRect rcUserInfoIni;
	CGameFrame::GetRectFromIni(szResPath("UserInfo\\UserInfo.ini"), TEXT("UserInfo"), TEXT("ItemRect"), rcUserInfoIni, rcUserInfoIni);
	GetDlgItem(IDC_SHOW)->SetWindowPos(NULL, rcUserInfoIni.left, rcUserInfoIni.top, rcUserInfoIni.Width(),rcUserInfoIni.Height(),SWP_NOZORDER);

	m_btOk.SetButtonImage(szResPath("BankDlg\\IDB_DLG_BTN_CANCLE.bmp"), false);
	m_btOk.SetWindowPos(NULL, rcClient.Width()-30, 3, 0, 0, SWP_NOSIZE);
	m_btOk.SetWindowText("");

	static CRect rect;
	if(rect.IsRectNull())
	{
		//�ָ�������
		CRect rectH;  //ˮƽ
		CRect rectV;  //��ֱ
		GetDlgItem(IDC_SHOW)->GetWindowRect(&rectH);
		
		//GetDlgItem(IDC_SEP2)->GetWindowRect(&rectV);
		rect.left= rectH.left+10;
		rect.bottom=rectH.bottom;
		rect.right=rectH.right;
		rect.top=rectH.top+15;

		ScreenToClient(&rect);
	}

	UINT xPos=0;//GetXExcursionPos()+10;
	UINT yPox =32; //GetYExcursionPos()+10;
	CRect rcBtn;
	m_btSample.GetWindowRect(&rcBtn);
	UINT uStype = 37;
	m_btSample.SetWindowPos(NULL, xPos, yPox, 0, 0,SWP_NOSIZE);
	m_btXX.SetWindowPos(NULL, xPos, yPox+uStype, 0, 0,SWP_NOSIZE);
	m_btLock.SetWindowPos(NULL, xPos, yPox+uStype*2, 0, 0,SWP_NOSIZE);
	m_btPass.SetWindowPos(NULL, xPos, yPox+uStype*3, 0,0 ,SWP_NOSIZE);
	
	m_DlgUserSample.Create(IDD_USER_JD_INFO,this);
	m_DlgUserSample.MoveWindow(rect);
	m_DlgUserSample.ShowWindow(SW_SHOW);   //��ʼ��Ϊ��һ���Ի���

	m_DlgUserXX.Create(IDD_USER_XX_INFO,this);
	m_DlgUserXX.MoveWindow(rect);

	m_DlgUserLock.Create(IDD_USER_LOCK,this);
	m_DlgUserLock.MoveWindow(rect);

	m_DlgUserPass.Create(IDD_USER_PASS,this);
	m_DlgUserPass.MoveWindow(rect);
	
	GetDlgItem(IDC_NAME)->SetWindowText("������Ϣ");

	//HTREEITEM  hti1 = m_tree.InsertItem ( _T("���ֶ�����"),0,1);
	//HTREEITEM  hti2 = m_tree.InsertItem ( _T("������־"),0,1);
	//HTREEITEM  hti3 = m_tree.InsertItem ( _T("��Ϸ��ȡ"),0,1);
	//HTREEITEM  hti4 = m_tree.InsertItem ( _T("�����޸�"),0,1);
	//HTREEITEM  hti5 = m_tree.InsertItem ( _T("ѡ��"),0,1);

	//m_tree.SetItemHeight(20);
	//


	//���ñ���
	//SetWindowText(TEXT("���ֶ�����"));

	//���¿ؼ�
	//UpdateControls();

	//��ʼ������
	//__super::InitSkinObject();

	return TRUE;  
}

//�滭����
void CDlgUserManage::OnPaint()
{

	CPaintDC dc(this);
	CRect rcClient;
	GetClientRect(&rcClient);
	//��������
	CBitmap ImageBuffer;
	ImageBuffer.CreateCompatibleBitmap(&dc,rcClient.Width(),rcClient.Height());

	//���� DC
	CDC BufferDC;
	BufferDC.CreateCompatibleDC(&dc);
	BufferDC.SelectObject(&ImageBuffer);

	CDC* pDc = GetDC();
	m_pngImageBack.DrawImage(&BufferDC, 0,0);
	//int iPox=m_pngStatus.GetHeight();
	dc.BitBlt(0,0,rcClient.Width(),rcClient.Height(),&BufferDC,0,0,SRCCOPY);
	//�滭����
	BufferDC.DeleteDC();
	ImageBuffer.DeleteObject();
	return;
	//�滭����
//	__super::DrawSkinView(&dc);
}

//��ɫ����
HBRUSH CDlgUserManage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor)
	{
	case CTLCOLOR_DLG:
	case CTLCOLOR_STATIC:
		{
		
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetBkColor(BGROUND_COLOR);
			return m_brBkground;

			//RGB(239,247,255)
		}
	}
	return __super::OnCtlColor(pDC,pWnd,nCtlColor);
}


//������Ϣ
void CDlgUserManage::OnClose()
{
	

	CDialog::OnClose();
}


//������Ϣ
void CDlgUserManage::OnClose1()
{
	CDialog::OnCancel();
}

//void CDlgMoneyBank::OnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult) 
//{
//	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
//	// TODO: Add your control notification handler code here
//	//Invalidate();
//	//SetRedraw (TRUE);
//	
//	HTREEITEM hItem = m_tree.GetSelectedItem();
//    //�õ���������
//    CString Text = m_tree.GetItemText(hItem);
//	if(Text=="���ֶ�����")
//	{
//		m_DlgMoneyGive.ShowWindow(SW_SHOW);
//		m_DlgMoneyManager.ShowWindow(SW_HIDE);
//	}
//	else if(Text=="��Ϸ��ȡ")
//	{
//		m_DlgMoneyGive.ShowWindow(SW_HIDE);
//		m_DlgMoneyManager.ShowWindow(SW_SHOW);
//	}
//
//
//	*pResult = 0;
//}


void CDlgUserManage::OnSample()
{
	m_DlgUserSample.ShowWindow(SW_SHOW);
	m_DlgUserXX.ShowWindow(SW_HIDE);
	m_DlgUserLock.ShowWindow(SW_HIDE);
	m_DlgUserPass.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_NAME)->SetWindowText("������Ϣ");
	m_btSample.SetButtonImage(szResPath("BankDlg\\IDB_DLG_BTN_ACTIVE.BMP"), false);
	m_btXX.SetButtonImage(szResPath("BankDlg\\IDB_DLG_BTN_NORMAL.BMP"), false);
	m_btLock.SetButtonImage(szResPath("BankDlg\\IDB_DLG_BTN_NORMAL.BMP"), false);
	m_btPass.SetButtonImage(szResPath("BankDlg\\IDB_DLG_BTN_NORMAL.BMP"), false);
}

void CDlgUserManage::OnXX()
{
	m_DlgUserSample.ShowWindow(SW_HIDE);
	m_DlgUserXX.ShowWindow(SW_SHOW);
	m_DlgUserLock.ShowWindow(SW_HIDE);
	m_DlgUserPass.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_NAME)->SetWindowText("��ϸ��Ϣ");
	m_btSample.SetButtonImage(szResPath("BankDlg\\IDB_DLG_BTN_NORMAL.BMP"), false);
	m_btXX.SetButtonImage(szResPath("BankDlg\\IDB_DLG_BTN_ACTIVE.BMP"), false);
	m_btLock.SetButtonImage(szResPath("BankDlg\\IDB_DLG_BTN_NORMAL.BMP"), false);
	m_btPass.SetButtonImage(szResPath("BankDlg\\IDB_DLG_BTN_NORMAL.BMP"), false);
}

void CDlgUserManage::OnLock()
{
	m_DlgUserSample.ShowWindow(SW_HIDE);
	m_DlgUserXX.ShowWindow(SW_HIDE);
	m_DlgUserLock.ShowWindow(SW_SHOW);
	m_DlgUserPass.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_NAME)->SetWindowText("������");
	m_btSample.SetButtonImage(szResPath("BankDlg\\IDB_DLG_BTN_NORMAL.BMP"), false);
	m_btXX.SetButtonImage(szResPath("BankDlg\\IDB_DLG_BTN_NORMAL.BMP"), false);
	m_btLock.SetButtonImage(szResPath("BankDlg\\IDB_DLG_BTN_ACTIVE.BMP"), false);
	m_btPass.SetButtonImage(szResPath("BankDlg\\IDB_DLG_BTN_NORMAL.BMP"), false);
}

void CDlgUserManage::OnPass()
{
	m_DlgUserSample.ShowWindow(SW_HIDE);
	m_DlgUserXX.ShowWindow(SW_HIDE);
	m_DlgUserLock.ShowWindow(SW_HIDE);
	m_DlgUserPass.ShowWindow(SW_SHOW);
	GetDlgItem(IDC_NAME)->SetWindowText("��½����");
	m_btSample.SetButtonImage(szResPath("BankDlg\\IDB_DLG_BTN_NORMAL.BMP"), false);
	m_btXX.SetButtonImage(szResPath("BankDlg\\IDB_DLG_BTN_NORMAL.BMP"), false);
	m_btLock.SetButtonImage(szResPath("BankDlg\\IDB_DLG_BTN_NORMAL.BMP"), false);
	m_btPass.SetButtonImage(szResPath("BankDlg\\IDB_DLG_BTN_ACTIVE.BMP"), false);

}

void CDlgUserManage::InitRoomName()
{
	m_DlgUserSample.ShowWindow(SW_HIDE);
	m_DlgUserXX.ShowWindow(SW_HIDE);
	m_DlgUserLock.ShowWindow(SW_SHOW);
	m_DlgUserPass.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_NAME)->SetWindowText("������");
	m_btSample.SetButtonImage(szResPath("BankDlg\\IDB_DLG_BTN_NORMAL.BMP"), false);
	m_btXX.SetButtonImage(szResPath("BankDlg\\IDB_DLG_BTN_NORMAL.BMP"), false);
	m_btLock.SetButtonImage(szResPath("BankDlg\\IDB_DLG_BTN_ACTIVE.BMP"), false);
	m_btPass.SetButtonImage(szResPath("BankDlg\\IDB_DLG_BTN_NORMAL.BMP"), false);
}

BOOL CDlgUserManage::OnEraseBkgnd( CDC* pDC )
{
	Invalidate(FALSE);
	UpdateWindow();
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////