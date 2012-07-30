#include "stdafx.h"
#include"resource.h"
#include "DlgUserInfoSample.h"
#include "GameFrame.h"
#include "GlobalUnits.h"
#include "Zip.h"

////////////////////////////////////////////////////////////////////////////////////
//�궨��
#define DEF_INSIDEBORDER_COLOR	RGB(176,20,1)						//Ĭ����ɫ
#define BGROUND_COLOR			RGB(218,222,223)

////////////////////////////////////////////////////////////////////////////////////


BEGIN_MESSAGE_MAP(CDlgUserSample, CDialog)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_SHOWWINDOW()
	ON_WM_CLOSE()

	ON_BN_CLICKED(IDOK, OnOK)

END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgUserSample::CDlgUserSample(CWnd* pParent): CDialog(IDD_USER_JD_INFO, pParent)//, CSkinWndObject(this)
{
	//���ñ���

	m_pIClientSocket = NULL;
	m_enOperateStatus=enOperateUserSampleStatus_NULL;
	
}

//�鹹����
CDlgUserSample::~CDlgUserSample()
{
	if (m_brBkground.m_hObject != NULL ) m_brBkground.DeleteObject();
}

//��������
void CDlgUserSample::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_GENDER, m_UserSelect);
	DDX_Control(pDX, IDOK, m_btEdit);
	
}

//��ʼ����
BOOL CDlgUserSample::OnInitDialog()
{
	__super::OnInitDialog();

	//����ˢ��
	m_brBkground.CreateSolidBrush(BGROUND_COLOR);

	//���ñ���
//	SetWindowText(TEXT("���ֶ�����"));

	m_UserSelect.InsertString(0,"��");
	m_UserSelect.InsertString(1,"Ů");
	m_UserSelect.InsertString(2,"δ֪");

	tagGlobalUserData & GlobalUserInfo=g_GlobalUnits.GetGolbalUserData();

	GetDlgItem(IDC_ACCOUNTS)->SetWindowText(GlobalUserInfo.szAccounts);

	CString strTemp;
	strTemp.Format("%ld",GlobalUserInfo.dwGameID);
	GetDlgItem(IDC_GAME_ID)->SetWindowText(strTemp);

	if(GlobalUserInfo.cbGender==0)
		m_UserSelect.SetCurSel(0);
	else if(GlobalUserInfo.cbGender==1)
		m_UserSelect.SetCurSel(1);
	else
		m_UserSelect.SetCurSel(2);

	GetDlgItem(IDC_DESC)->SetWindowText(GlobalUserInfo.szUnderWrite);

	//���¿ؼ�
	UpdateControls();

	//��ʼ������
	//__super::InitSkinObject();

	return TRUE;  
}

//�滭����
void CDlgUserSample::OnPaint()
{
	CPaintDC dc(this);

	
	//�滭����
//	__super::DrawSkinView(&dc);
}


//��ʾ��Ϣ
void CDlgUserSample::ShowMessage(TCHAR *pMessage)
{
	ShowInformationEx(pMessage,0,MB_ICONQUESTION,TEXT("�޸��û���Ϣ"));

	return;
}

//��������
bool CDlgUserSample::ConnectServer()
{
	if ( m_pIClientSocket != NULL && m_pIClientSocket->GetSocketStatus() == SOCKET_STATUS_CONNECT && m_enOperateStatus != enOperateUserSampleStatus_NULL )
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
bool CDlgUserSample::SendData()
{
	
	//״̬�ж�
	ASSERT(m_enOperateStatus != enOperateUserSampleStatus_NULL);
	if ( m_enOperateStatus == enOperateUserSampleStatus_NULL ) return false;

	if ( m_pIClientSocket == NULL || m_pIClientSocket->GetSocketStatus() != SOCKET_STATUS_CONNECT ) return false;

	if ( m_enOperateStatus == enOperateUserSampleStatus_Save )
	{
		CMD_GP_ModifyUserWrite ModifyWrite;
		ZeroMemory(&ModifyWrite, sizeof(ModifyWrite));

		tagGlobalUserData &GlobalUserData = g_GlobalUnits.GetGolbalUserData();
		ModifyWrite.dwUserID = GlobalUserData.dwUserID;
		
		CString strDesc;
		GetDlgItem(IDC_DESC)->GetWindowText(strDesc);
		lstrcpyn(ModifyWrite.szUnderWrite,strDesc, CountArray(ModifyWrite.szUnderWrite));
		
		//Ͷ����Ϣ
		m_pIClientSocket->SendData(MDM_GP_USER, SUB_GP_MODIFY_WRITE, &ModifyWrite, sizeof(ModifyWrite));        

		//���½���
		InvalidateRect(NULL);
	}
	
	
	//����״̬
	m_enOperateStatus = enOperateUserSampleStatus_NULL ;

	return true;
}

//���¿ؼ�
void CDlgUserSample::UpdateControls()
{
	//��������
	tagGlobalUserData & GlobalUserInfo=g_GlobalUnits.GetGolbalUserData();
	UpdateData(false);
}

//��ɫ����
HBRUSH CDlgUserSample::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
void CDlgUserSample::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	//���¿ؼ�
	if ( bShow ) UpdateControls();
}

//������Ϣ
void CDlgUserSample::OnClose()
{


	CDialog::OnClose();
}


//��ʼ������
bool CDlgUserSample::InitRoomName()
{
	

	return true;
}

void CDlgUserSample::OnOK()
{
	m_enOperateStatus = enOperateUserSampleStatus_Save;
	//��������
	if ( ConnectServer() == false )
	{
		//����״̬
		m_enOperateStatus = enOperateUserSampleStatus_NULL;
		return;
	}
}

////////////////////////////////////////////////////////////////////////////////////