#include "stdafx.h"
#include"resource.h"
#include "DlgMoneySelect.h"
#include "GameFrame.h"
#include "GlobalUnits.h"
#include "Zip.h"

////////////////////////////////////////////////////////////////////////////////////
//�궨��
#define DEF_INSIDEBORDER_COLOR	RGB(176,20,1)						//Ĭ����ɫ
#define BGROUND_COLOR			RGB(218,222,223)

////////////////////////////////////////////////////////////////////////////////////


BEGIN_MESSAGE_MAP(CDlgMoneySelect, CDialog)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_SHOWWINDOW()
	ON_WM_CLOSE()

	ON_BN_CLICKED(IDC_EDIT, OnBnClickedEdit)
	ON_BN_CLICKED(IDC_QUERY, OnBnClickedQuery)
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgMoneySelect::CDlgMoneySelect(CWnd* pParent): CDialog(IDD_SELECT, pParent)//, CSkinWndObject(this)
{
	//���ñ���
//	m_pIClientSocket = NULL;
}

//�鹹����
CDlgMoneySelect::~CDlgMoneySelect()
{
	if(m_brBkground.GetSafeHandle()) m_brBkground.DeleteObject();
}

//��������
void CDlgMoneySelect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_SELECT, m_boxTimeSecect);
	DDX_Control(pDX, IDC_EDIT, m_btEdit);
	
}

//��ʼ����
BOOL CDlgMoneySelect::OnInitDialog()
{
	__super::OnInitDialog();

	//����ˢ��
	if(m_brBkground.GetSafeHandle()) m_brBkground.DeleteObject();
	m_brBkground.CreateSolidBrush(BGROUND_COLOR);

	//���ñ���
	SetWindowText(TEXT("���ֶ�����"));



	m_boxTimeSecect.Clear();
	m_boxTimeSecect.InsertString(0,"1");
	m_boxTimeSecect.InsertString(1,"3");
	m_boxTimeSecect.InsertString(2,"5");
	m_boxTimeSecect.InsertString(3,"10");
	m_boxTimeSecect.InsertString(4,"30");
	m_boxTimeSecect.InsertString(5,"60");
	m_boxTimeSecect.SetCurSel(0);

	SetButtonSelected(IDC_QUERY,g_GlobalOption.m_bCloseQuery);
	if(!g_GlobalOption.m_bCloseQuery)
	{
		GetDlgItem(IDC_LK)->EnableWindow(false);	
		GetDlgItem(IDC_ZD)->EnableWindow(false);	
		GetDlgItem(IDC_LEAVE)->EnableWindow(false);	
		GetDlgItem(IDC_SELECT)->EnableWindow(false);	
		GetDlgItem(IDC_DESC)->EnableWindow(false);	
	}
	else
	{
		if(g_GlobalOption.m_BankCloseMode==enBankRule_LK)
		{
			SetButtonSelected(IDC_LK,true);
		}
		else if(g_GlobalOption.m_BankCloseMode==enBankRule_ZD)
		{
			SetButtonSelected(IDC_ZD,true);
			//m_boxTimeSecect.SelectString(0, )
			CString strTime;
			strTime.Format("%d", g_GlobalOption.m_BankCloseTime);
			m_boxTimeSecect.SelectString(0, strTime);
			//g_GlobalOption.m_BankCloseTime

		}
		else
		{
			SetButtonSelected(IDC_LEAVE,true);
		}
		GetDlgItem(IDC_LK)->EnableWindow(TRUE);	
		GetDlgItem(IDC_ZD)->EnableWindow(TRUE);	
		GetDlgItem(IDC_LEAVE)->EnableWindow(TRUE);	
		GetDlgItem(IDC_SELECT)->EnableWindow(TRUE);	
		GetDlgItem(IDC_DESC)->EnableWindow(TRUE);	
	}

	//���¿ؼ�
	UpdateControls();

	//��ʼ������
	//__super::InitSkinObject();

	return TRUE;  
}

//�滭����
void CDlgMoneySelect::OnPaint()
{
	CPaintDC dc(this);

	
	//�滭����
//	__super::DrawSkinView(&dc);
}


//��ʾ��Ϣ
void CDlgMoneySelect::ShowMessage(TCHAR *pMessage)
{
	ShowInformationEx(pMessage,0,MB_ICONQUESTION,TEXT("���������"));
	return;
}

//��������
bool CDlgMoneySelect::ConnectServer()
{
	//if ( m_pIClientSocket != NULL && m_pIClientSocket->GetSocketStatus() == SOCKET_STATUS_CONNECT && m_enOperateStatus != enOperateMoneyStatus_NULL )
	//{
	//	ShowMessage(TEXT("�������ڽ��У����Ժ�"));
	//	return false;
	//}

	//if ( m_pIClientSocket != NULL && m_pIClientSocket->GetSocketStatus() == SOCKET_STATUS_CONNECT )//?GetSocketStatus
	//{
	//	SendData();
	//	return true;
	//}

	////��ȡ����
	//CGameFrame *pGameFrame = (CGameFrame *)AfxGetMainWnd() ;
	//CPlazaViewItem * pPlazaViewItem = &(pGameFrame->m_DlgGamePlaza) ;
	//if ( m_pIClientSocket == NULL )
	//{

	//	//�Ϸ��ж�
	//	if ( pPlazaViewItem->m_ClientSocket.GetInterface() == NULL ) 
	//	{
	//		ShowMessage(TEXT("���������û�д����������µ�¼��"));
	//		return false;
	//	}

	//	//���ñ���
	//	m_pIClientSocket = (ITCPSocket*)pPlazaViewItem->m_ClientSocket.GetInterface();
	//}

	////��ַ����
	//CRegKey RegServer;
	//LPCTSTR pszServerIP=pPlazaViewItem->m_DlgLogon.GetLogonServer();
	//TCHAR szRegServer[256]=TEXT(""),szServerAddr[64]=TEXT("");
	//_snprintf(szRegServer,sizeof(szRegServer),TEXT("%s\\%s"),REG_LOGON_SERVER,pszServerIP);

	//if (RegServer.Open(HKEY_CURRENT_USER,szRegServer,KEY_READ)==ERROR_SUCCESS)
	//{
	//	TCHAR szReadData[1024]=TEXT("");
	//	DWORD dwReadData=0L,dwDataType=0L,dwDataSize=sizeof(szReadData);
	//	LONG lErrorCode=RegServer.QueryValue(TEXT("ServerAddr"),&dwDataType,szReadData,&dwDataSize);
	//	if (lErrorCode==ERROR_SUCCESS) 
	//	{
	//		CXOREncrypt::CrevasseData(szReadData,szServerAddr,sizeof(szServerAddr));
	//		pszServerIP=szServerAddr;
	//	}
	//}

	////���ӷ�����
	//try
	//{
	//	//���ӷ�����
	//	m_pIClientSocket->CloseSocket();
	//	if (m_pIClientSocket->Connect(pszServerIP,PORT_LOGON_SERVER)!=ERROR_SUCCESS)
	//	{
	//		throw TEXT("���������Ӵ��󣬿��������ϵͳ��û�гɹ����������磡");
	//	}
	//}
	//catch (LPCTSTR pszError)
	//{
	//	ShowMessage((TCHAR*)pszError);
	//	return false;
	//}

	return true;
}


//��������
bool CDlgMoneySelect::SendData()
{
	
	////״̬�ж�
	//ASSERT(m_enOperateStatus != enOperateMoneyGiveStatus_NULL);
	//if ( m_enOperateStatus == enOperateMoneyGiveStatus_NULL ) return false;

	//if ( m_pIClientSocket == NULL || m_pIClientSocket->GetSocketStatus() != SOCKET_STATUS_CONNECT ) return false;

	//if ( m_enOperateStatus == enOperateGiveStatus_Save )
	//{
	//	CMD_GP_MoneyGive GiveMoney;
	//	ZeroMemory(&GiveMoney, sizeof(GiveMoney));

	//	tagGlobalUserData &GlobalUserData = g_GlobalUnits.GetGolbalUserData();
	//	GiveMoney.dwUserID = GlobalUserData.dwUserID;
	//	GiveMoney.lScore = GetDlgItemInt(IDC_SCORE);
	//	CString strNickName;
	//	GetDlgItem(IDC_USER)->GetWindowText(strNickName);
	//	strcpy(GiveMoney.szNickname,strNickName);
	//	GiveMoney.cbType = m_UserSelect.GetCurSel();

	//	//Ͷ����Ϣ
	//	m_pIClientSocket->SendData(MDM_GP_USER, SUB_GP_GIVE_MONEY, &GiveMoney, sizeof(GiveMoney));        

	//	//���½���
	//	InvalidateRect(NULL);
	//}
	//
	//else if ( m_enOperateStatus == enOperateGiveStatus_Refresh )
	//{
	//	CMD_GP_MoneyOper GetMoney;
	//	ZeroMemory(&GetMoney, sizeof(GetMoney));

	//	tagGlobalUserData &GlobalUserData = g_GlobalUnits.GetGolbalUserData();
	//	GetMoney.dwUserID = GlobalUserData.dwUserID;
	//	GetMoney.cbType = 2;
	//	GetMoney.lScore = 0;
	//	GetMoney.dwKindID = 0;
	//	//Ͷ����Ϣ
	//	m_pIClientSocket->SendData(MDM_GP_USER, SUB_GP_OPEN_MONEY, &GetMoney, sizeof(GetMoney));        

	//	//���½���
	//	InvalidateRect(NULL);
	//}

	////����״̬
	//m_enOperateStatus = enOperateMoneyGiveStatus_NULL ;

	return true;
}

//���¿ؼ�
void CDlgMoneySelect::UpdateControls()
{
	//��������
	tagGlobalUserData & GlobalUserInfo=g_GlobalUnits.GetGolbalUserData();
	
	UpdateData(false);

}

//��ɫ����
HBRUSH CDlgMoneySelect::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor)
	{
	case CTLCOLOR_DLG:
	case CTLCOLOR_STATIC:
		{
		
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetBkColor(BGROUND_COLOR);
			return m_brBkground;
		}
	}
	return __super::OnCtlColor(pDC,pWnd,nCtlColor);
}

//��ʾ��Ϣ
void CDlgMoneySelect::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	//���¿ؼ�
	if ( bShow ) UpdateControls();
}

//������Ϣ
void CDlgMoneySelect::OnClose()
{
// 	if ( m_pIClientSocket != NULL && m_pIClientSocket->GetSocketStatus() == SOCKET_STATUS_CONNECT && m_enOperateStatus != enOperateMoneyStatus_NULL )
// 	{
// 		m_pIClientSocket->CloseSocket();
// 	}

	CDialog::OnClose();
		DestroyWindow();
}


//��ʼ������
bool CDlgMoneySelect::InitRoomName()
{
	

	return true;
}


////////////////////////////////////////////////////////////////////////////////////
//�Ƿ�ѡ��
bool CDlgMoneySelect::IsButtonSelected(UINT uButtonID)
{
	return ((CButton *)GetDlgItem(uButtonID))->GetCheck()==BST_CHECKED;
}

//ѡ�а�ť
void CDlgMoneySelect::SetButtonSelected(UINT uButtonID, bool bSelected)
{
	CButton * pButton=(CButton *)GetDlgItem(uButtonID);
	if (bSelected) pButton->SetCheck(BST_CHECKED);
	else pButton->SetCheck(BST_UNCHECKED);
	return;
}


void CDlgMoneySelect::OnBnClickedEdit()
{
	g_GlobalOption.m_bCloseQuery=IsButtonSelected(IDC_QUERY);

	//����ģʽ
	if (IsButtonSelected(IDC_LK)) 
	{
		g_GlobalOption.m_BankCloseMode=enBankRule_LK;
	}
	else if (IsButtonSelected(IDC_ZD)) 
	{
		g_GlobalOption.m_BankCloseMode=enBankRule_ZD;
		CString strTime;
		GetDlgItemText(IDC_SELECT,strTime);
		g_GlobalOption.m_BankCloseTime = atoi(strTime.GetBuffer(0));
	}
	else 
		g_GlobalOption.m_BankCloseMode=enBankRule_LEAVE;

	ShowMessage(TEXT("����ɹ�"));
}

void CDlgMoneySelect::OnBnClickedQuery()
{
	if(IsButtonSelected(IDC_QUERY))
	{
		GetDlgItem(IDC_LK)->EnableWindow(true);	
		GetDlgItem(IDC_ZD)->EnableWindow(true);	
		GetDlgItem(IDC_LEAVE)->EnableWindow(true);	
		GetDlgItem(IDC_SELECT)->EnableWindow(true);	
		GetDlgItem(IDC_DESC)->EnableWindow(true);		
	}
	else
	{
		GetDlgItem(IDC_LK)->EnableWindow(false);	
		GetDlgItem(IDC_ZD)->EnableWindow(false);	
		GetDlgItem(IDC_LEAVE)->EnableWindow(false);	
		GetDlgItem(IDC_SELECT)->EnableWindow(false);	
		GetDlgItem(IDC_DESC)->EnableWindow(false);		
	}
}