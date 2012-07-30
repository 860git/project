#include "stdafx.h"
#include"resource.h"
#include "DlgUserInfoPass.h"
#include "GameFrame.h"
#include "GlobalUnits.h"
#include "Zip.h"

////////////////////////////////////////////////////////////////////////////////////
//�궨��
#define DEF_INSIDEBORDER_COLOR	RGB(176,20,1)						//Ĭ����ɫ
#define BGROUND_COLOR			RGB(218,222,223)

////////////////////////////////////////////////////////////////////////////////////


BEGIN_MESSAGE_MAP(CDlgUserPass, CDialog)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_SHOWWINDOW()
	ON_WM_CLOSE()

	ON_BN_CLICKED(IDOK, OnOK)

END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgUserPass::CDlgUserPass(CWnd* pParent): CDialog(IDD_USER_PASS, pParent)//, CSkinWndObject(this)
{
	//���ñ���

	m_pIClientSocket = NULL;
	m_enOperateStatus=enOperateUserPassStatus_NULL;
	
}

//�鹹����
CDlgUserPass::~CDlgUserPass()
{
	if (m_brBkground.m_hObject != NULL ) m_brBkground.DeleteObject();
}

//��������
void CDlgUserPass::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btEdit);
	
}

//��ʼ����
BOOL CDlgUserPass::OnInitDialog()
{
	__super::OnInitDialog();

	//����ˢ��
	m_brBkground.CreateSolidBrush(BGROUND_COLOR);

	//���ñ���
//	SetWindowText(TEXT("���ֶ�����"));

	//���¿ؼ�
	UpdateControls();

	//��ʼ������
	//__super::InitSkinObject();

	return TRUE;  
}

//�滭����
void CDlgUserPass::OnPaint()
{
	CPaintDC dc(this);

	
	//�滭����
//	__super::DrawSkinView(&dc);
}


//��ʾ��Ϣ
void CDlgUserPass::ShowMessage(TCHAR *pMessage)
{
	ShowInformationEx(pMessage,0,MB_ICONQUESTION,TEXT("���ͻ��ֶ�"));

	return;
}

//��������
bool CDlgUserPass::ConnectServer()
{
	if ( m_pIClientSocket != NULL && m_pIClientSocket->GetSocketStatus() == SOCKET_STATUS_CONNECT && m_enOperateStatus != enOperateUserPassStatus_NULL )
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
bool CDlgUserPass::SendData()
{
	
	//״̬�ж�
	ASSERT(m_enOperateStatus != enOperateUserPassStatus_NULL);
	if ( m_enOperateStatus == enOperateUserPassStatus_NULL ) return false;

	if ( m_pIClientSocket == NULL || m_pIClientSocket->GetSocketStatus() != SOCKET_STATUS_CONNECT ) return false;

	if ( m_enOperateStatus == enOperateUserPassStatus_Save )
	{
		CMD_GP_ModifyUserPass EditUserPass;
		ZeroMemory(&EditUserPass, sizeof(EditUserPass));

		tagGlobalUserData &GlobalUserData = g_GlobalUnits.GetGolbalUserData();
		EditUserPass.dwUserID = GlobalUserData.dwUserID;
		
		CString strPass;
		GetDlgItem(IDC_PASSWORD)->GetWindowText(strPass);

		TCHAR szBankpass[33];
		CMD5Encrypt::EncryptData(strPass,szBankpass);

		strcpy(EditUserPass.szPassword,szBankpass);

		GetDlgItem(IDC_PASSWORD1)->GetWindowText(strPass);

		CMD5Encrypt::EncryptData(strPass,szBankpass);

		strcpy(EditUserPass.szPassword1,szBankpass);
		//Ͷ����Ϣ
		m_pIClientSocket->SendData(MDM_GP_USER, SUB_GP_EDIT_USER_PASS, &EditUserPass, sizeof(EditUserPass));       
		//���½���
		InvalidateRect(NULL);
	}
	
	//����״̬
	m_enOperateStatus = enOperateUserPassStatus_NULL ;

	return true;
}

//���¿ؼ�
void CDlgUserPass::UpdateControls()
{
	//��������
	tagGlobalUserData & GlobalUserInfo=g_GlobalUnits.GetGolbalUserData();
	UpdateData(false);
}

//��ɫ����
HBRUSH CDlgUserPass::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
void CDlgUserPass::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	//���¿ؼ�
	if ( bShow ) UpdateControls();
}

//������Ϣ
void CDlgUserPass::OnClose()
{


	CDialog::OnClose();
}


//��ʼ������
bool CDlgUserPass::InitRoomName()
{
	/*tagGlobalUserData &GlobalUserData = g_GlobalUnits.GetGolbalUserData();
	
	m_RoomSelect.ResetContent();

	int nIndex=0;
	CListKind * pListKind=NULL;
	do
	{
		pListKind=g_GlobalUnits.m_ServerListManager.EnumKindItem(nIndex++);
		if (pListKind==NULL) break;
		m_RoomSelect.AddString(pListKind->m_GameKind.szKindName);
	} while (true);*/

	return true;
}

void CDlgUserPass::OnOK()
{
	CString strBuffer;
	
	TCHAR		m_szPassword1[PASS_LEN];
	TCHAR		m_szPassword2[PASS_LEN];

	GetDlgItemText(IDC_PASSWORD1,m_szPassword1,CountArray(m_szPassword1));
	GetDlgItemText(IDC_PASSWORD2,m_szPassword2,CountArray(m_szPassword2));

	if (lstrcmp(m_szPassword1,m_szPassword2)!=0)
	{
		ShowInformation(TEXT("��Ϸ������ȷ�����벻һ�£�����������ȷ�����룡"),0,MB_ICONQUESTION);
		GetDlgItem(IDC_PASSWORD1)->SetFocus();
		return;
	}

	m_enOperateStatus = enOperateUserPassStatus_Save;
	//��������
	if ( ConnectServer() == false )
	{
		//����״̬
		m_enOperateStatus = enOperateUserPassStatus_NULL;
		return;
	}
}


////////////////////////////////////////////////////////////////////////////////////