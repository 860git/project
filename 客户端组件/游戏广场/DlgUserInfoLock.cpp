#include "stdafx.h"
#include"resource.h"
#include "DlgUserInfoLock.h"
#include "GameFrame.h"
#include "GlobalUnits.h"
#include "Zip.h"
#include "DlgBankPass.h"

////////////////////////////////////////////////////////////////////////////////////
//�궨��
#define DEF_INSIDEBORDER_COLOR	RGB(176,20,1)						//Ĭ����ɫ
#define BGROUND_COLOR			RGB(218,222,223)

////////////////////////////////////////////////////////////////////////////////////


BEGIN_MESSAGE_MAP(CDlgUserLock, CDialog)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_SHOWWINDOW()
	ON_WM_CLOSE()

	ON_BN_CLICKED(IDC_BIND, OnOK)

END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgUserLock::CDlgUserLock(CWnd* pParent): CDialog(IDD_USER_LOCK, pParent)//, CSkinWndObject(this)
{
	//���ñ���

	m_pIClientSocket = NULL;
	m_enOperateStatus=enOperateUserLockStatus_NULL;
	
}

//�鹹����
CDlgUserLock::~CDlgUserLock()
{
	if (m_brBkground.m_hObject != NULL ) m_brBkground.DeleteObject();
}

//��������
void CDlgUserLock::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_BIND, m_btEdit);
}

//��ʼ����
BOOL CDlgUserLock::OnInitDialog()
{
	__super::OnInitDialog();

	//����ˢ��
	m_brBkground.CreateSolidBrush(BGROUND_COLOR);

	//���ñ���
//	SetWindowText(TEXT("���ֶ�����"));

	tagGlobalUserData &GlobalUserData = g_GlobalUnits.GetGolbalUserData();
	if(GlobalUserData.JiQi==1)
	{
		GetDlgItem(IDC_BIND)->SetWindowTextA("����");
		GetDlgItem(IDC_STATUS)->SetWindowTextA("��ǰ״̬: ����״̬");
	}
	else
	{
		GetDlgItem(IDC_BIND)->SetWindowTextA("����");
		GetDlgItem(IDC_STATUS)->SetWindowTextA("��ǰ״̬: ����״̬");
	}

	//���¿ؼ�
	UpdateControls();

	//��ʼ������
	//__super::InitSkinObject();

	return TRUE;  
}

//�滭����
void CDlgUserLock::OnPaint()
{
	CPaintDC dc(this);

	
	//�滭����
//	__super::DrawSkinView(&dc);
}


//��ʾ��Ϣ
void CDlgUserLock::ShowMessage(TCHAR *pMessage)
{
	ShowInformationEx(pMessage,0,MB_ICONQUESTION,TEXT("���ͻ��ֶ�"));

	return;
}

//��������
bool CDlgUserLock::ConnectServer()
{
	if ( m_pIClientSocket != NULL && m_pIClientSocket->GetSocketStatus() == SOCKET_STATUS_CONNECT && m_enOperateStatus != enOperateUserLockStatus_NULL )
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
bool CDlgUserLock::SendData()
{
	
	////״̬�ж�
	ASSERT(m_enOperateStatus != enOperateUserLockStatus_NULL);
	if ( m_enOperateStatus == enOperateUserLockStatus_NULL ) return false;

	if ( m_pIClientSocket == NULL || m_pIClientSocket->GetSocketStatus() != SOCKET_STATUS_CONNECT ) return false;

	if ( m_enOperateStatus == enOperateUserLockStatus_Save )
	{
		tagGlobalUserData &GlobalUserData = g_GlobalUnits.GetGolbalUserData();
		if(GlobalUserData.JiQi==1)
		{
			CMD_GP_SafUnbind UnBind;
			ZeroMemory(&UnBind, sizeof(UnBind));

			tagGlobalUserData &GlobalUserData = g_GlobalUnits.GetGolbalUserData();
			UnBind.dwUserID = GlobalUserData.dwUserID;
			lstrcpy(UnBind.szPassWord, GlobalUserData.szPassWord);

			//Ͷ����Ϣ
			m_pIClientSocket->SendData(MDM_GP_USER, SUB_GP_SAFE_UNBIND, &UnBind, sizeof(UnBind)); 
		}
		else
		{
			CMD_GP_SafeBind SafeBind;
			ZeroMemory(&SafeBind, sizeof(SafeBind));

			tagGlobalUserData &GlobalUserData = g_GlobalUnits.GetGolbalUserData();
			SafeBind.dwUserID = GlobalUserData.dwUserID;
			lstrcpy(SafeBind.szPassWord, GlobalUserData.szPassWord);
			g_GlobalUnits.GetClientSerial(SafeBind.ClientSerial);
					
			//Ͷ����Ϣ
			m_pIClientSocket->SendData(MDM_GP_USER, SUB_GP_SAFE_BIND, &SafeBind, sizeof(SafeBind)); 
		}


		//���½���
		InvalidateRect(NULL);
	}
	
	//����״̬
	m_enOperateStatus = enOperateUserLockStatus_NULL ;

	return true;
}

//���¿ؼ�
void CDlgUserLock::UpdateControls()
{
	//��������
	tagGlobalUserData & GlobalUserInfo=g_GlobalUnits.GetGolbalUserData();
	UpdateData(false);
}

//��ɫ����
HBRUSH CDlgUserLock::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
void CDlgUserLock::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	//���¿ؼ�
	if ( bShow ) UpdateControls();
}

//������Ϣ
void CDlgUserLock::OnClose()
{


	CDialog::OnClose();
}


//��ʼ������
bool CDlgUserLock::InitRoomName()
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

	tagGlobalUserData &GlobalUserData = g_GlobalUnits.GetGolbalUserData();
	if(GlobalUserData.JiQi==1)
	{
		GetDlgItem(IDC_BIND)->SetWindowTextA("����");
		GetDlgItem(IDC_STATUS)->SetWindowTextA("��ǰ״̬: ����״̬");
	}
	else
	{
		GetDlgItem(IDC_BIND)->SetWindowTextA("����");
		GetDlgItem(IDC_STATUS)->SetWindowTextA("��ǰ״̬: ����״̬");
	}

	return true;
}

void CDlgUserLock::OnOK()
{
	CDlgBankPass DlgBankPass;

	if( DlgBankPass.DoModal()==IDOK)
	{
		m_enOperateStatus = enOperateUserLockStatus_Save;
		//��������
		if ( ConnectServer() == false )
		{
			//����״̬
			m_enOperateStatus = enOperateUserLockStatus_NULL;
			return;
		}
	}

}


////////////////////////////////////////////////////////////////////////////////////