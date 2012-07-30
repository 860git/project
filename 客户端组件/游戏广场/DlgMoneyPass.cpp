#include "stdafx.h"
#include"resource.h"
#include "DlgMoneyPass.h"
#include "GameFrame.h"
#include "GlobalUnits.h"
#include "Zip.h"

////////////////////////////////////////////////////////////////////////////////////
//�궨��
#define DEF_INSIDEBORDER_COLOR	RGB(176,20,1)						//Ĭ����ɫ
#define BGROUND_COLOR			RGB(218,222,223)

////////////////////////////////////////////////////////////////////////////////////


BEGIN_MESSAGE_MAP(CDlgMoneyPass, CDialog)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_SHOWWINDOW()
	ON_WM_CLOSE()

	ON_BN_CLICKED(IDC_EDIT, OnBnClickedEdit)

END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgMoneyPass::CDlgMoneyPass(CWnd* pParent): CDialog(IDD_CHANGE_PASS, pParent)//, CSkinWndObject(this)
{
	//���ñ���

	m_pIClientSocket = NULL;
	m_enOperateStatus=enOperateMoneyPassStatus_NULL;
	
}

//�鹹����
CDlgMoneyPass::~CDlgMoneyPass()
{
	if(m_brBkground.GetSafeHandle()) m_brBkground.DeleteObject();
}

//��������
void CDlgMoneyPass::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT, m_btEdit);
}

//��ʼ����
BOOL CDlgMoneyPass::OnInitDialog()
{
	__super::OnInitDialog();

	//����ˢ��
	if(m_brBkground.GetSafeHandle()) m_brBkground.DeleteObject();
	m_brBkground.CreateSolidBrush(BGROUND_COLOR);

	//���ñ���
	SetWindowText(TEXT("���ֶ�����"));

	//���¿ؼ�
	UpdateControls();

	//��ʼ������
	//__super::InitSkinObject();

	return TRUE;  
}

//�滭����
void CDlgMoneyPass::OnPaint()
{
	CPaintDC dc(this);

	
	//�滭����
//	__super::DrawSkinView(&dc);
}


//��ʾ��Ϣ
void CDlgMoneyPass::ShowMessage(TCHAR *pMessage)
{
	ShowInformationEx(pMessage,0,MB_ICONQUESTION,TEXT("���ͻ��ֶ�"));

	return;
}

//��������
bool CDlgMoneyPass::ConnectServer()
{
	if ( m_pIClientSocket != NULL && m_pIClientSocket->GetSocketStatus() == SOCKET_STATUS_CONNECT && m_enOperateStatus != enOperateMoneyPassStatus_NULL )
	{
		ShowMessage(TEXT("�������ڽ��У����Ժ�"));
		return false;
	}

	if ( m_pIClientSocket != NULL && m_pIClientSocket->GetSocketStatus() == SOCKET_STATUS_CONNECT )//?GetSocketStatus
	{
		SendData();
		return true;
	}

	//��ȡ����
	CGameFrame *pGameFrame = (CGameFrame *)AfxGetMainWnd() ;
	CPlazaViewItem * pPlazaViewItem = &(pGameFrame->m_DlgGamePlaza) ;
	if ( m_pIClientSocket == NULL )
	{

		//�Ϸ��ж�
		if ( pPlazaViewItem->m_ClientSocket.GetInterface() == NULL ) 
		{
			ShowMessage(TEXT("���������û�д����������µ�¼��"));
			return false;
		}

		//���ñ���
		m_pIClientSocket = (ITCPSocket*)pPlazaViewItem->m_ClientSocket.GetInterface();
	}

	//��ַ����
	CRegKey RegServer;
	LPCTSTR pszServerIP=pPlazaViewItem->m_DlgLogon.GetLogonServer();
	TCHAR szRegServer[256]=TEXT(""),szServerAddr[64]=TEXT("");
	_snprintf(szRegServer,sizeof(szRegServer),TEXT("%s\\%s"),REG_LOGON_SERVER,pszServerIP);

	if (RegServer.Open(HKEY_CURRENT_USER,szRegServer,KEY_READ)==ERROR_SUCCESS)
	{
		TCHAR szReadData[1024]=TEXT("");
		DWORD dwReadData=0L,dwDataType=0L,dwDataSize=sizeof(szReadData);
		LONG lErrorCode=RegServer.QueryValue(TEXT("ServerAddr"),&dwDataType,szReadData,&dwDataSize);
		if (lErrorCode==ERROR_SUCCESS) 
		{
			CXOREncrypt::CrevasseData(szReadData,szServerAddr,sizeof(szServerAddr));
			pszServerIP=szServerAddr;
		}
	}

	//���ӷ�����
	try
	{
		//���ӷ�����
		m_pIClientSocket->CloseSocket();
		if (m_pIClientSocket->Connect(pszServerIP,PORT_LOGON_SERVER)!=ERROR_SUCCESS)
		{
			throw TEXT("���������Ӵ��󣬿��������ϵͳ��û�гɹ����������磡");
		}
	}
	catch (LPCTSTR pszError)
	{
		ShowMessage((TCHAR*)pszError);
		return false;
	}

	return true;
}


//��������
bool CDlgMoneyPass::SendData()
{
	
	//״̬�ж�
	ASSERT(m_enOperateStatus != enOperateMoneyPassStatus_NULL);
	if ( m_enOperateStatus == enOperateMoneyPassStatus_NULL ) return false;

	if ( m_pIClientSocket == NULL || m_pIClientSocket->GetSocketStatus() != SOCKET_STATUS_CONNECT ) return false;

	if ( m_enOperateStatus == enOperateEditStatus_Save )
	{
		CMD_GP_EditBankPass EditBankPass;
		ZeroMemory(&EditBankPass, sizeof(EditBankPass));

		tagGlobalUserData &GlobalUserData = g_GlobalUnits.GetGolbalUserData();
		EditBankPass.dwUserID = GlobalUserData.dwUserID;
		
		CString strPass;
		GetDlgItem(IDC_OLD_PASSWORD)->GetWindowText(strPass);

		TCHAR szBankpass[33];
		CMD5Encrypt::EncryptData(strPass,szBankpass);

		strcpy(EditBankPass.szPassword,szBankpass);

		GetDlgItem(IDC_NEW_PASSWORD)->GetWindowText(strPass);

		CMD5Encrypt::EncryptData(strPass,szBankpass);

		strcpy(EditBankPass.szPassword1,szBankpass);
		//Ͷ����Ϣ
		m_pIClientSocket->SendData(MDM_GP_USER, SUB_GP_EDIT_BANK_PASS, &EditBankPass, sizeof(EditBankPass));        

		//���½���
		InvalidateRect(NULL);
	}
	
	
	////����״̬
	m_enOperateStatus = enOperateMoneyPassStatus_NULL ;

	return true;
}

//���¿ؼ�
void CDlgMoneyPass::UpdateControls()
{
	//��������
	tagGlobalUserData & GlobalUserInfo=g_GlobalUnits.GetGolbalUserData();
	
	UpdateData(false);

}

//��ɫ����
HBRUSH CDlgMoneyPass::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
void CDlgMoneyPass::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	//���¿ؼ�
	if ( bShow ) UpdateControls();
}

//������Ϣ
void CDlgMoneyPass::OnClose()
{
	if ( m_pIClientSocket != NULL && m_pIClientSocket->GetSocketStatus() == SOCKET_STATUS_CONNECT && m_enOperateStatus != enOperateMoneyStatus_NULL )
	{
		m_pIClientSocket->CloseSocket();
	}
	CDialog::OnClose();
	DestroyWindow();
}


//��ʼ������
bool CDlgMoneyPass::InitRoomName()
{
	

	return true;
}

void CDlgMoneyPass::OnBnClickedEdit()
{
	CString strBuffer;
	
	TCHAR		m_szPassword1[PASS_LEN];
	TCHAR		m_szPassword2[PASS_LEN];

	GetDlgItemText(IDC_NEW_PASSWORD,m_szPassword1,CountArray(m_szPassword1));
	GetDlgItemText(IDC_NEW_PASSWORD1,m_szPassword2,CountArray(m_szPassword2));

	if (lstrcmp(m_szPassword1,m_szPassword2)!=0)
	{
		ShowInformation(TEXT("��Ϸ������ȷ�����벻һ�£�����������ȷ�����룡"),0,MB_ICONQUESTION);
		GetDlgItem(IDC_NEW_PASSWORD1)->SetFocus();
		return;
	}

	m_enOperateStatus = enOperateEditStatus_Save;
	//��������
	if ( ConnectServer() == false )
	{
		//����״̬
		m_enOperateStatus = enOperateMoneyPassStatus_NULL;
		return;
	}
}


////////////////////////////////////////////////////////////////////////////////////