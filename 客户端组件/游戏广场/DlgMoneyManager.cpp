#include "stdafx.h"
#include"resource.h"
#include "DlgMoneyManager.h"
#include "GameFrame.h"
#include "GlobalUnits.h"
#include "Zip.h"

////////////////////////////////////////////////////////////////////////////////////
//�궨��
#define DEF_INSIDEBORDER_COLOR	RGB(176,20,1)						//Ĭ����ɫ
#define BGROUND_COLOR			RGB(218,222,223)

////////////////////////////////////////////////////////////////////////////////////


BEGIN_MESSAGE_MAP(CDlgMoneyManager, CDialog)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_SHOWWINDOW()
	ON_WM_CLOSE()

	ON_BN_CLICKED(IDC_SAVE, OnBnClickedSave)
	ON_BN_CLICKED(IDC_GET, OnBnClickedGet)
	ON_BN_CLICKED(IDC_REFRESH, OnBnClickedRefresh)
	ON_CBN_SELCHANGE(IDC_ROOM_SELECT, OnEnChangeRoom)
	ON_EN_CHANGE(IDC_NUM, OnEnChangeNum)
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgMoneyManager::CDlgMoneyManager(CWnd* pParent): CDialog(IDD_MONEY_MANAGER, pParent)//, CSkinWndObject(this)
{
	//���ñ���
	m_lTotalMoney = 0;
	m_lRoomMoney = 0;
	m_pIClientSocket = NULL;
	m_enOperateStatus=enOperateMoneyStatus_NULL;
	
}

//�鹹����
CDlgMoneyManager::~CDlgMoneyManager()
{
	if(m_brBkground.GetSafeHandle()) m_brBkground.DeleteObject();
}

//��������
void CDlgMoneyManager::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_ROOM_SELECT, m_RoomSelect);
	DDX_Control(pDX, IDC_SAVE, m_btSave);
	DDX_Control(pDX, IDC_GET, m_btGet);
	DDX_Control(pDX, IDC_REFRESH, m_btRefresh);
	//
	//DDX_Control(pDX, IDC_ROOM_SELECT, m_chkRoom);
	
}

//��ʼ����
BOOL CDlgMoneyManager::OnInitDialog()
{
	__super::OnInitDialog();

	if(m_brBkground.GetSafeHandle()) m_brBkground.DeleteObject();

	//����ˢ��
	m_brBkground.CreateSolidBrush(BGROUND_COLOR);

	//���ñ���
	SetWindowText(TEXT("�����书��"));

	//���¿ؼ�
	UpdateControls();

	//��ʼ������
	//__super::InitSkinObject();

	return TRUE;  
}

//�滭����
void CDlgMoneyManager::OnPaint()
{
	CPaintDC dc(this);

	
	//�滭����
//	__super::DrawSkinView(&dc);
}


//��ʾ��Ϣ
void CDlgMoneyManager::ShowMessage(TCHAR *pMessage)
{
	ShowInformationEx(pMessage,0,MB_ICONQUESTION,TEXT("�Զ���ͷ��"));

	return;
}

//��������
bool CDlgMoneyManager::ConnectServer()
{
	if ( m_pIClientSocket != NULL && m_pIClientSocket->GetSocketStatus() == SOCKET_STATUS_CONNECT && m_enOperateStatus != enOperateMoneyStatus_NULL )
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
bool CDlgMoneyManager::SendData()
{
	
	//״̬�ж�
	//ASSERT(m_enOperateStatus != enOperateMoneyStatus_NULL);
	if ( m_enOperateStatus == enOperateMoneyStatus_NULL ) return false;

	if ( m_pIClientSocket == NULL || m_pIClientSocket->GetSocketStatus() != SOCKET_STATUS_CONNECT ) return false;

	if ( m_enOperateStatus == enOperateStatus_Save )
	{
		CMD_GP_MoneyOper SaveMoney;
		ZeroMemory(&SaveMoney, sizeof(SaveMoney));

		tagGlobalUserData &GlobalUserData = g_GlobalUnits.GetGolbalUserData();
		SaveMoney.dwUserID = GlobalUserData.dwUserID;
		SaveMoney.cbType = 0;
	//	SaveMoney.lScore = GetDlgItemInt(IDC_NUM);

		CString str;
		//GetDlgItem(IDC_NUM)->GetWindowTextA(str);
		SaveMoney.lScore =GetDlgItemInt64(IDC_NUM);
		CString strRoomName;
		m_RoomSelect.GetWindowText(strRoomName);
		SaveMoney.dwKindID = GetGameKind(strRoomName);

		SetDlgItemText(IDC_NUM, "");

		//Ͷ����Ϣ
		m_pIClientSocket->SendData(MDM_GP_USER, SUB_GP_OPEN_MONEY, &SaveMoney, sizeof(SaveMoney));        

		//���½���
		InvalidateRect(NULL);
	}
	else if ( m_enOperateStatus == enOperateStatus_Get )
	{
		CMD_GP_MoneyOper GetMoney;
		ZeroMemory(&GetMoney, sizeof(GetMoney));

		tagGlobalUserData &GlobalUserData = g_GlobalUnits.GetGolbalUserData();
		GetMoney.dwUserID = GlobalUserData.dwUserID;
		GetMoney.cbType = 1;
		//GetMoney.lScore = GetDlgItemInt(IDC_NUM);
		//CString str;
		//GetDlgItem(IDC_NUM)->GetWindowTextA(str);
		GetMoney.lScore = GetDlgItemInt64(IDC_NUM);//_atoi64(str);

		CString strRoomName;
		m_RoomSelect.GetWindowText(strRoomName);
		GetMoney.dwKindID = GetGameKind(strRoomName);

		SetDlgItemText(IDC_NUM, "");
		//Ͷ����Ϣ
		m_pIClientSocket->SendData(MDM_GP_USER, SUB_GP_OPEN_MONEY, &GetMoney, sizeof(GetMoney));        

		//���½���
		InvalidateRect(NULL);
	}
	else if ( m_enOperateStatus == enOperateStatus_Refresh )
	{
		CMD_GP_MoneyOper GetMoney;
		ZeroMemory(&GetMoney, sizeof(GetMoney));

		tagGlobalUserData &GlobalUserData = g_GlobalUnits.GetGolbalUserData();
		GetMoney.dwUserID = GlobalUserData.dwUserID;
		GetMoney.cbType = 2;
		//GetMoney.lScore = GetDlgItemInt(IDC_NUM);

// 		CString str;
// 		GetDlgItem(IDC_NUM)->GetWindowTextA(str);
		GetMoney.lScore = GetDlgItemInt64(IDC_NUM);//_atoi64(str);
		CString strRoomName;
		m_RoomSelect.GetWindowText(strRoomName);
		GetMoney.dwKindID = GetGameKind(strRoomName);

		SetDlgItemText(IDC_NUM, "");
		//Ͷ����Ϣ
		m_pIClientSocket->SendData(MDM_GP_USER, SUB_GP_OPEN_MONEY, &GetMoney, sizeof(GetMoney));        

		//���½���
		InvalidateRect(NULL);
	}

	//����״̬
	m_enOperateStatus = enOperateMoneyStatus_NULL ;

	return true;
}

//���¿ؼ�
void CDlgMoneyManager::UpdateControls()
{
	//��������
	tagGlobalUserData & GlobalUserInfo=g_GlobalUnits.GetGolbalUserData();
	//CString strTemp;
	//strTemp.Format("%I64d",m_lTotalMoney);
	//
	GetDlgItem(IDC_TOTAL_MONEY)->SetWindowText(LongToString(m_lTotalMoney));
	//strTemp.Format("%I64d",m_lRoomMoney);
	GetDlgItem(IDC_MONEY)->SetWindowText(LongToString(m_lRoomMoney));
;
	m_DlgStatus.DestroyStatusWnd(this);
	
	UpdateData(false);

}

//��ɫ����
HBRUSH CDlgMoneyManager::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
void CDlgMoneyManager::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	//���¿ؼ�
	if ( bShow ) UpdateControls();
}

//������Ϣ
void CDlgMoneyManager::OnClose()
{
	if ( m_pIClientSocket != NULL && m_pIClientSocket->GetSocketStatus() == SOCKET_STATUS_CONNECT && m_enOperateStatus != enOperateMoneyStatus_NULL )
	{
		m_pIClientSocket->CloseSocket();
	}
	CDialog::OnClose();
	DestroyWindow();
}


//��ʼ������
bool CDlgMoneyManager::InitRoomName()
{
	tagGlobalUserData &GlobalUserData = g_GlobalUnits.GetGolbalUserData();
	
	m_RoomSelect.Clear();
	m_RoomSelect.ResetContent();

	int nIndex=0;
	CListKind * pListKind=NULL;
	do
	{
		pListKind=g_GlobalUnits.m_ServerListManager.EnumKindItem(nIndex++);
		if (pListKind==NULL) break;
		m_RoomSelect.AddString(pListKind->m_GameKind.szKindName);
	} while (true);

	m_RoomSelect.SetCurSel(0);

	OnBnClickedRefresh();
	return true;
}

DWORD CDlgMoneyManager::GetGameKind(CString m_strKindName)
{
	tagGlobalUserData &GlobalUserData = g_GlobalUnits.GetGolbalUserData();
	
	DWORD m_dwGameKind=0;
	int nIndex=0;
	CListKind * pListKind=NULL;
	do
	{
		pListKind=g_GlobalUnits.m_ServerListManager.EnumKindItem(nIndex++);
		if (pListKind==NULL) break;
		if(pListKind->m_GameKind.szKindName==m_strKindName)
			{
			m_dwGameKind = pListKind->m_GameKind.wKindID;
			break;
			}
		//m_RoomSelect.AddString(pListKind->m_GameKind.szKindName);
	} while (true);

	return m_dwGameKind;
}

void CDlgMoneyManager::OnBnClickedSave()
{
	if(GetDlgItemInt64(IDC_NUM) <=0)
	{
		ShowMessage("���ֶ���������0��");
		return;
	}


	m_enOperateStatus = enOperateStatus_Save;
	//��������
	if ( ConnectServer() == false )
	{
		//����״̬
		m_enOperateStatus = enOperateMoneyStatus_NULL;
		return;
	}
}
void CDlgMoneyManager::OnBnClickedGet()
{
	if(GetDlgItemInt64(IDC_NUM) <=0)
	{
		ShowMessage("���ֶ���������0��");
		return;
	}

	m_enOperateStatus = enOperateStatus_Get;

	//��������
	if ( ConnectServer() == false )
	{
		//����״̬
		m_enOperateStatus = enOperateMoneyStatus_NULL;
		return;
	}	
}
void CDlgMoneyManager::OnBnClickedRefresh()
{
	CString strBuffer;
	strBuffer.Format(TEXT("���ڻ���û������䣬���Ժ�..."));
	m_DlgStatus.ShowStatusMessage(strBuffer,this);
	m_DlgStatus.SetWindowPos(&wndTopMost,0,0,0, 0,SWP_NOMOVE|SWP_NOSIZE);
	m_enOperateStatus = enOperateStatus_Refresh;
	//��������
	if ( ConnectServer() == false )
	{
		//����״̬
		m_enOperateStatus = enOperateMoneyStatus_NULL;
		return ;
	}		

}

void CDlgMoneyManager::SetMoney(LONGLONG TotalMoney,LONGLONG RoomMoney)
{
	m_lTotalMoney = TotalMoney;
	m_lRoomMoney = RoomMoney;
}
//���뷿��
void CDlgMoneyManager::OnEnChangeRoom()
{
	m_lTotalMoney = 0;
	m_lRoomMoney = 0;

	UpdateControls();
	CString strBuffer;
	strBuffer.Format(TEXT("���ڻ���û������䣬���Ժ�..."));
//	g_GlobalAttemper.ShowStatusMessage(strBuffer,this);
//	g_GlobalAttemper.m_DlgStatus.BringWindowToTop();
	m_DlgStatus.ShowStatusMessage(strBuffer,this);
//	m_DlgStatus.BringWindowToTop();
	m_DlgStatus.SetWindowPos(&wndTopMost,0,0,0, 0,SWP_NOMOVE|SWP_NOSIZE   ); 

	m_enOperateStatus = enOperateStatus_Refresh;
	//��������
	if ( ConnectServer() == false )
	{
		//����״̬
		m_enOperateStatus = enOperateMoneyStatus_NULL;
		return ;
	}		
}

CString CDlgMoneyManager::LongToString( LONGLONG lScore )
{
	CString strText,strTmpText;
	strText = TEXT("");
	INT64 lGold;
	lGold = lScore;
	int NumCount=0;
	int lenCount=0;
	if(lGold==0){strText.Format("0");return "0";}
	if(lGold>0)
	{
		strTmpText.Format(TEXT("%I64d"),lGold);
		if(strTmpText.GetLength() > 3)
		{
			for(int i=0;i<strTmpText.GetLength();i++)
			{
				if(i%4==0)
				{
					strTmpText.Insert(strTmpText.GetLength()-i,',');
				}
			}
		}
		int nFind = strTmpText.ReverseFind(',');
		if(-1==nFind)
		{
			strText=strTmpText;
		}
		else
		{
			strText = strTmpText.Left(strTmpText.ReverseFind(','));
		}
	}
	return strText;
}

void CDlgMoneyManager::OnEnChangeNum()
{
	CString strText,strTmpText;
	strText = TEXT("");
	INT64 lGold;
	lGold = GetDlgItemInt64(IDC_NUM);
	int NumCount=0;
	int lenCount=0;
	if(lGold==0) strText=TEXT("0");
	if(lGold>0)
	{
		strTmpText.Format(TEXT("%I64d"),lGold);
		if(strTmpText.GetLength() > 3)
		{
			for(int i=0;i<strTmpText.GetLength();i++)
			{
				if(i%4==0)
				{
					strTmpText.Insert(strTmpText.GetLength()-i,',');
				}
			}
		}
		int nFind = strTmpText.ReverseFind(',');
		if(-1==nFind)
		{
			strText=strTmpText;
		}
		else
		{
			strText = strTmpText.Left(strTmpText.ReverseFind(','));
		}
		GetDlgItem(IDC_NUM)->SetWindowText(strText);
		((CEdit*)GetDlgItem(IDC_NUM))->SetSel(strText.GetLength(),-1);
	}
}

INT64 CDlgMoneyManager::GetDlgItemInt64(UINT nID)
{
	CString strText;
	if (GetDlgItemText(nID,strText)==0)
		return 0;
	strText.Remove(',');

	return _tstoi64(strText);
}


////////////////////////////////////////////////////////////////////////////////////