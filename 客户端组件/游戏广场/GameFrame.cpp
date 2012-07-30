#include "Stdafx.h"
#include "GameFrame.h"
#include "DlgOption.h"
#include "GlobalUnits.h"
#include "DlgIndividualInfo.h"
#include "DlgPlayerInfo.h"
#include "TableOption.h"

//////////////////////////////////////////////////////////////////////////

//���ƫ��
#define FRAME_EXALTATION			24									//�����߶�

//��Ļλ��
#define LESS_SCREEN_X				1024								//��С���
#define LESS_SCREEN_Y				720									//��С�߶�

//��ť��ʾ
#define IDC_BT_MIN					1000								//��С��ť
#define IDC_BT_MAX					1001								//��С��ť
#define IDC_BT_CLOSE				1002								//�رհ�ť
#define IDC_BT_EXCHANGE_SINK		1003								//������ť
#define IDC_BT_BUTTON_1				1004								//���ܰ�ť
#define IDC_BT_BUTTON_2				1005								//���ܰ�ť
#define IDC_BT_BUTTON_3				1006								//���ܰ�ť
#define IDC_BT_BUTTON_4				1007								//���ܰ�ť
#define IDC_BT_BUTTON_5				1008								//���ܰ�ť
#define IDC_BT_BUTTON_6				11009								//���ܰ�ť
#define IDC_BT_BUTTON_7				11010								//���ܰ�ť
#define IDC_BT_MENU					11011								//���ܰ�ť
#define IDC_BT_HELP					11012								//���ܰ�ť

#define IDC_BT_SAFE					11013								//���ܰ�ť
#define IDC_BT_VIP				11014								//���ܰ�ť
#define IDC_BT_FANKUI					11015								//���ܰ�ť
#define IDC_BT_BBS					11016								//���ܰ�ť
#define IDC_BT_FONT					11017								//���ܰ�ť
//�ͻ��ؼ�
// #define IDC_LOCK				5001								//���ܰ�ť
// #define IDC_UNLOCK				5002								//���ܰ�ť
// #define IDC_VIDEO				5003								//���ܰ�ť
// #define IDC_MOBILE			    5004								//���ܰ�ť
// #define IDC_JIANGHU				5005								//���ܰ�ť
// #define IDC_HONGZUAN		    5006								//���ܰ�ť
// #define IDC_UNZUAN				5007								//���ܰ�ť
// #define IDC_SET				    5008								//���ܰ�ť

#define IDC_LOCKROOM				5009								//���ܰ�ť

//�ؼ���ʶ
#define IDC_BT_PLAZA				1038								//��Ϸ�㳡
#define IDC_BT_SWITCH_ACCOUNTS		1009								//�л��ʺ�
#define IDC_BT_SYSTEM_OPTION		1010								//ϵͳ����
#define IDC_BT_SELF_OPTION			1011								//��������
#define IDC_BT_QUIT_GAME			1012								//�˳���Ϸ
#define IDC_BT_VIEW_ITEM			1013								//����ؼ�
#define IDC_BT_LIST_CONTROL_1		21015								//���ư�ť
#define IDC_BT_LIST_CONTROL_2		21016								//���ư�ť
#define IDC_BROWER_AD				1017								//�������
#define IDC_SPLITTER				1018								//��������
#define IDC_SERVER_ITEM_VIEW		1019								//�б����
#define IDC_COMPANION_LIST			1020								//�����б�
#define CR_TEXT						RGB(255,255,255)					//������ɫ	

//��Ϣ����
#define WM_CLOSE_ROOM_VIEW_ITEM		WM_USER+10							//�ر���Ϣ
#define WM_SETUP_FINISH				WM_USER+100//��װ���
#define WM_BIND_PC					WM_USER+201							//�󶨻���

#define IDI_TIME_UPDATETIME			555									//ʱ�����

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameFrame, CFrameWnd)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_CLOSE()
	ON_WM_CREATE()
	ON_WM_NCHITTEST()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_SETTINGCHANGE()
	ON_WM_GETMINMAXINFO()
	ON_MESSAGE(WM_HOTKEY,OnHotKeyMessage)
	ON_MESSAGE(WM_SETUP_FINISH,OnMessageSetupFinish)
	ON_MESSAGE(WM_CLOSE_ROOM_VIEW_ITEM,OnCloseRoomViewItem)
	ON_MESSAGE(WM_BIND_PC, ShowBindPc)
	ON_WM_NCMOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_WM_ACTIVATE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////
CBitmap CGameFrame::m_bmScroll;
//���캯��
CGameFrame::CGameFrame():m_Splitter(VorSpliter)//,m_BankStorage(0)
{
	//���ñ���
	m_bRectify=false;
	m_bIsMax=true;
	m_bHideGameList=false;
	m_wShowListType=IDC_BT_LIST_CONTROL_1;
	m_pActiveViewItem=NULL;
	m_pRoomViewItemCreate=NULL;
	m_bMouseOnFace=false;
	m_bMouseOnAccount= false;
	m_bMouseOnUnderWrite= false;
	memset(m_pRoomViewItem,0,sizeof(m_pRoomViewItem));

	//����״̬
	m_bMaxShow=false;
	m_rcNormalSize.SetRect(0,0,0,0);

	if (m_bmScroll.GetSafeHandle()==NULL)
	{
		m_bmScroll.LoadBitmap(IDB_SCROLL);
	}
	OnLineCount=0;
	m_bIsLogBank = false;
	m_tmLogonTime = NULL;
	if(!m_PngImageTopAd.IsNull()) m_PngImageTopAd.DestroyImage();

	m_PngImageTopAd.LoadImage(szResPath("Frame\\Topad.png"));
	return;
}

//��������
CGameFrame::~CGameFrame()
{
	m_bIsLogBank = false;
	if(!m_PngImageTopAd.IsNull()) m_PngImageTopAd.DestroyImage();
}

//��ť��Ϣ
bool CGameFrame::OnSplitterButton(CSkinSplitter * pSkinSplitter, CSkinButton * pSplitterButton)
{
	//�б���Ϣ
	ControlServerList(ServerListControl_Turn);

	return true;
}

//�������Ϣ
bool CGameFrame::OnSplitterEvent(CSkinSplitter * pSkinSplitter, UINT uSplitID, int nXPos, int nYPos)
{
	//��ʾ�ؼ�
	if (m_bHideGameList==true)
	{
		//���ñ���
		m_bRectify=true;
		m_bHideGameList=false;

		//�������
		m_UserInfoView.ShowWindow(SW_SHOW);
		//m_ServerItmOpaint.ShowWindow(SW_SHOW);
		OnCommand(m_wShowListType,0);

		//���ð�ť
		m_Splitter.ShowSplitterButton(GetPlatformRes().uBtHideGameList,GetResInstanceHandle());
	}

	//�����ؼ�
	CRect rcClient;
	GetClientRect(&rcClient);

	RectifyControl(rcClient.Width(),rcClient.Height());

	return true;
}

//�������
void __cdecl CGameFrame::OnTreeLeftClick(CListItem *pListItem, HTREEITEM hTreeItem, CTreeCtrl * pTreeCtrl)
{
	//Ч�����
	if (pListItem==NULL) 
	{
		return;
	}

	//��Ϣ����
	switch (pListItem->GetItemGenre())
	{
	case ItemGenre_Inside:
		{
			//��������
			CListInside * pListInside=(CListInside *)pListItem;
			tagGameInside * pGameInside=pListInside->GetItemInfo();

			WebBrowse(TEXT("http://www.7x78.com/mini.asp"),true);

			return;
		}
	case ItemGenre_Kind:		//��Ϸ����
		{
			//��������
			CListKind * pListKind=(CListKind *)pListItem;
			tagGameKind * pGameKind=pListKind->GetItemInfo();

			TCHAR szRuleUrl[256]=TEXT("");
			//���ӹ���
			if(pGameKind->wKindID<3000)
			{

				_snprintf(szRuleUrl,sizeof(szRuleUrl),TEXT("http://www.7x78.com/GameRule.asp?KindID=%ld"),pGameKind->wKindID);
			}
			//////////////////////////////////////////////////////////////////////////
			if(pGameKind->wTypeID == 4)
			{
				_snprintf(szRuleUrl, sizeof(szRuleUrl), TEXT("http://www.7x78.com/WebGame/%s"), pGameKind->szProcessName);
			}

			//////////////////////////////////////////////////////////////////////////
			WebBrowse(szRuleUrl,true);

			return;
		}
	}

	return;
}

//�Ҽ�����
void __cdecl CGameFrame::OnTreeRightClick(CListItem *pListItem, HTREEITEM hTreeItem, CTreeCtrl *pTreeCtrl)
{
	return;
}

//���˫��
void __cdecl CGameFrame::OnTreeLeftDBClick(CListItem *pListItem, HTREEITEM hTreeItem, CTreeCtrl *pTreeCtrl)
{
	//Ч�����
	if(pListItem==NULL) return;

	//��Ϣ����
	switch (pListItem->GetItemGenre())
	{
	case ItemGenre_Kind:	//��Ϸ����
		{
			//��������
			CListKind * pListKind=(CListKind *)pListItem;
			tagGameKind * pGameKind=pListKind->GetItemInfo();

			//��װ�ж�
			if ((pListKind->m_bInstall==false)&&(pListKind->m_GameKind.dwMaxVersion!=0L))
			{
				//������ʾ
				TCHAR szBuffer[512]=TEXT("");
				_snprintf(szBuffer,sizeof(szBuffer),TEXT("��%s����û�а�װ�������Ƿ����أ�"),pGameKind->szKindName);

				//��ʾ��Ϣ
				if (ShowMessageBox(szBuffer,MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON1)==IDYES)
				{
					g_GlobalAttemper.DownLoadClient(pGameKind->szKindName,pGameKind->wKindID,true);
				}
			}

			return;
		}
	case ItemGenre_Server:	//��Ϸ����
		{
			CListServer * pListServer=(CListServer *)pListItem;
			CreateRoomViewItem(pListServer);
			return;
		}
	}

	return;
}

//�Ҽ�˫��
void __cdecl CGameFrame::OnTreeRightDBClick(CListItem *pListItem, HTREEITEM hTreeItem, CTreeCtrl *pTreeCtrl)
{
	return;
}

//ѡ��ı�
void __cdecl CGameFrame::OnTreeSelectChanged(CListItem *pListItem, HTREEITEM hTreeItem, CTreeCtrl *pTreeCtrl)
{
	return;
}

//����չ��
void __cdecl CGameFrame::OnTreeItemexpanded(CListItem *pListItem, HTREEITEM hTreeItem, CTreeCtrl * pTreeCtrl)
{
	return;
}

//��Ϣ����
BOOL CGameFrame::PreTranslateMessage(MSG * pMsg)
{
	m_ToolTipCtrl.RelayEvent(pMsg);
	return __super::PreTranslateMessage(pMsg);
}

//�����
BOOL CGameFrame::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//��������
	UINT nCommandID=LOWORD(wParam);

	//�л���ť
	if ((nCommandID>=IDC_BT_VIEW_ITEM)&&(nCommandID<(IDC_BT_VIEW_ITEM+CountArray(m_btViewItem))))
	{
		ActiveRoomViewItem(m_pRoomViewItem[nCommandID-IDC_BT_VIEW_ITEM]);
		return TRUE;
	}

	//��Ϣ����
	switch(nCommandID)
	{
	case IDC_BT_MIN	:					//��С����
		{
			m_bIsMax = false;
			ShowWindow(SW_MINIMIZE);

			return TRUE;
		}
	case IDC_BT_MAX		:				//��󻯴�
		{
			m_bIsMax = true;
			if (m_bMaxShow==true) RestoreWindow();
			else MaxSizeWindow();

			return TRUE;
		}
	case  IDC_BT_BUTTON_1:			//��¼
		{
			//ShowSystemOption();;
			QieHuanZhangHao();
			return TRUE;
		}
	case  IDC_BT_BUTTON_2:			//��ҳ
		{
			ShellExecute(NULL,TEXT("open"),TEXT("http://www.7x78.com/"),NULL,NULL,SW_SHOWDEFAULT);
			return TRUE;
		}
	case  IDC_BT_BUTTON_3:			//��ֵ
		{
			WebBrowse(TEXT("http://www.7x78.com/PayIndex.asp"),true);
			return TRUE;
		}
	case  IDC_BT_BUTTON_4:			//����
		{
			tagGlobalUserData &GlobalUserData=g_GlobalUnits.GetGolbalUserData();

			if(GlobalUserData.dwUserID==0)
				return FALSE;

			//��������
// 			if(!m_bIsLogBank)
// 			{
// 				if(m_DlgBankPass.DoModal()!=IDOK)
// 					return true;
// 			}
// 			else
// 			{
// 				if(g_GlobalOption.m_BankCloseMode!=enBankRule_LK)
// 				{
					if(m_DlgBankPass.DoModal()!=IDOK)
						return true;
// 				}
// 			}
			m_bIsLogBank = true;
			if ( m_DlgBankManager.m_hWnd == NULL )
			{
				m_DlgBankManager.Create(IDD_BANK_MANAGER, this);
			}

			//m_DlgBankManager.InitRoomName();
			//��ʾ����
			m_DlgBankManager.CenterWindow();
			m_DlgBankManager.ShowWindow(SW_SHOW);
			m_DlgBankManager.SetActiveWindow();
			m_DlgBankManager.SetForegroundWindow();

			return TRUE;
		}
	case  IDC_BT_BUTTON_5:			//�̳�
		{
			WebBrowse(TEXT("http://www.7x78.com/ShopIndex.asp"),true);
			return TRUE;
		}
	case  IDC_BT_BUTTON_6:			//���
		{
			//��ȡ���
			tagGlobalUserData &GlobalUserData=g_GlobalUnits.GetGolbalUserData();

			//���ж�
			if (!GlobalUserData.JiQi)
			{
				m_DlgLockMachine.m_bLock=true;
				m_DlgLockMachine.m_strDescribe=TEXT("ע�⣺�󶨻����󣬴��ʺ�ֻ���ڴ˼�����Ͻ�����Ϸ���󶨻�������ڴ˻������н���󶨲�����");

				//��ѯ��
				if (m_DlgLockMachine.DoModal()==IDOK)
				{
					m_DlgGamePlaza.m_ClientSocket->CloseSocket();

					//�����ж�
					if (m_DlgGamePlaza.m_ClientSocket->GetSocketStatus()==SOCKET_STATUS_CONNECT)
					{
						//�������к�
						CMD_GP_SafeBind SafeBind;
						ZeroMemory(&SafeBind, sizeof(SafeBind));
						SafeBind.dwUserID = GlobalUserData.dwUserID;
						g_GlobalUnits.GetClientSerial(SafeBind.ClientSerial);
						TCHAR szPassword[33];
						CMD5Encrypt::EncryptData(m_DlgLockMachine.m_strPassword,szPassword);
						lstrcpyn(SafeBind.szPassWord,szPassword,CountArray(SafeBind.szPassWord));

						//��������
						m_DlgGamePlaza.m_ClientSocket->SendData(MDM_GP_USER,SUB_GP_SAFE_BIND,&SafeBind,sizeof(SafeBind));
					}
					else
					{
						//���ñ�ʶ
						m_DlgGamePlaza.m_enBindAction=enBindAction_Bind;

						//���ӷ�����
						m_DlgGamePlaza.SendConnectMessage();
					}
				}
			}
			else
			{
				m_DlgLockMachine.m_bLock=false;
				m_DlgLockMachine.m_strDescribe=TEXT("���˺��Ѿ�ʹ���˰󶨻������ܣ�������ȷ����������󣬵��ȷ����ť����󶨣�");

				//��ѯ��
				if (m_DlgLockMachine.DoModal()==IDOK)
				{
					m_DlgGamePlaza.m_ClientSocket->CloseSocket();
					//�����ж�
					if (m_DlgGamePlaza.m_ClientSocket->GetSocketStatus()==SOCKET_STATUS_CONNECT)
					{
						CMD_GP_SafUnbind SafUnbind;
						ZeroMemory(&SafUnbind, sizeof(SafUnbind));

						SafUnbind.dwUserID=GlobalUserData.dwUserID;
						TCHAR szPassword[33];
						CMD5Encrypt::EncryptData(m_DlgLockMachine.m_strPassword,szPassword);
						lstrcpyn(SafUnbind.szPassWord,szPassword,CountArray(SafUnbind.szPassWord));

						//��������
						m_DlgGamePlaza.m_ClientSocket->SendData(MDM_GP_USER,SUB_GP_SAFE_UNBIND,&SafUnbind,sizeof(SafUnbind));
					}
					else
					{
						//���ñ�ʶ
						m_DlgGamePlaza.m_enBindAction=enBindAction_UnBind;

						//���ӷ�����
						m_DlgGamePlaza.SendConnectMessage();
					}
				}
			}
			return TRUE;
		}
	case  IDC_BT_BUTTON_7:			//�˳�
		{
			CloseCurrentViewItem();
			return TRUE;
		}

	case IDC_BT_PLAZA	:				//������ť
		{
			ActivePlazaViewItem();

			return TRUE;
		}
	case IDC_BT_EXCHANGE_SINK:			//������ť
		{
			g_GlobalUnits.m_PlatformResourceModule->SetResourceType(RESOURCE_RED);
			UpdateSkinResource();
			m_DlgGamePlaza.UpdateSkinResource();
			m_UserInfoView.UpdateSkinResource();
			//m_ServerItmOpaint.UpdateSkinResource();
			for (int i=0;i<CountArray(m_pRoomViewItem);i++)
			{
				if (m_pRoomViewItem[i]==NULL) break;
				m_pRoomViewItem[i]->UpdateSkinResource();
			}

			return TRUE;
		}
	case IDC_BT_CLOSE	:				//�رշ���
	case IDC_BT_QUIT_GAME	:			//�˳���Ϸ
		{
			CloseCurrentViewItem();

			return TRUE;
		}
	case IDC_SET	:		//ϵͳ����
		{
			//ϵͳ����
			ShowSystemOption();

			return TRUE;
		}
	case IDC_BT_SYSTEM_OPTION	:		//ϵͳ����
		{
			//ϵͳ����
			ShowSystemOption();

			return TRUE;
		}
	case IDC_BT_SELF_OPTION:			//��������
		{
			CDlgIndividualInfo DlgIndividualInfo;
			DlgIndividualInfo.DoModal();

			return TRUE;
		}
	case IDC_BT_SWITCH_ACCOUNTS :		//�л���ť
		{
			//״̬�ж�
			tagGlobalUserData & GlobalUserData=g_GlobalUnits.GetGolbalUserData();
			if (GlobalUserData.dwUserID==0L) return TRUE;

			//�л�ѯ��
			if (m_pRoomViewItem[0]!=NULL)
			{
				const TCHAR szMessage[]=TEXT("�л��û��ʺţ�����ر�������Ϸ���䣬ȷʵҪ�л��û��ʺ��� ");
				int iResult=ShowMessageBox(szMessage,MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2);
				if (iResult!=IDYES) return TRUE;
			}
			else
			{
				const TCHAR szMessage[]=TEXT("ȷʵҪע����ǰ�û����л��û��ʺ��� ");
				int iResult=ShowMessageBox(szMessage,MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2);
				if (iResult!=IDYES) return TRUE;
			}

			//�رշ���
			ActivePlazaViewItem();
			CloseAllRoomViewItem();

			//ɾ����¼
			g_GlobalUnits.DeleteUserCookie();

			//���ñ���
			memset(&GlobalUserData,0,sizeof(GlobalUserData));

			//Ͷ����Ϣ
			m_DlgGamePlaza.SendLogonMessage();

			return TRUE;
		}
	case IDC_BT_LIST_CONTROL_1:			//�б�ť
	case IDC_BT_LIST_CONTROL_2:			//�б�ť
		{
			//�����б�
			if(m_bHideGameList)
			{
				//���ذ�ť
				//m_btListControl1.ShowWindow(SW_HIDE);
				//m_btListControl2.ShowWindow(SW_HIDE);
				m_ServerItemView.ShowWindow(SW_HIDE);
				m_UserCompanionList.ShowWindow(SW_HIDE);
			}
			//��ʾ�б�
			else
			{
				//���ñ���
				m_wShowListType = nCommandID;

				//���ð�ť
				//m_btListControl1.ShowWindow(SW_SHOW);
				//m_btListControl2.ShowWindow(SW_SHOW);
				UINT uControl1=((IDC_BT_LIST_CONTROL_1==m_wShowListType)?GetPlatformRes().uFrameServiceBtShow1:GetPlatformRes().uFrameServiceBtHide1);
				UINT uControl2=((IDC_BT_LIST_CONTROL_2==m_wShowListType)?GetPlatformRes().uFrameServiceBtShow2:GetPlatformRes().uFrameServiceBtHide2);
				//m_btListControl1.SetButtonImage(uControl1,GetResInstanceHandle(),false);
				//m_btListControl2.SetButtonImage(uControl2,GetResInstanceHandle(),false);

				//�ж���ʾ
				m_ServerItemView.ShowWindow((m_wShowListType==IDC_BT_LIST_CONTROL_1)?SW_SHOW:SW_HIDE);
				m_UserCompanionList.ShowWindow((m_wShowListType==IDC_BT_LIST_CONTROL_2)?SW_SHOW:SW_HIDE);
			}

			return TRUE;
		}
	case IDC_SHOW_TIP:				//���ܰ�ť
		{
			//��������
			if ( m_DlgUserManager.m_hWnd == NULL )
			{
				m_DlgUserManager.Create(IDD_USER_MANAGER, this);
			}
			m_DlgUserManager.CenterWindow();
			m_DlgUserManager.ShowWindow(SW_SHOW);
			m_DlgUserManager.SetActiveWindow();
			m_DlgUserManager.SetForegroundWindow();
			m_DlgUserManager.InitRoomName();
			return TRUE;
		}
	}

	return __super::OnCommand(wParam,lParam);
}
void CGameFrame::QieHuanZhangHao()
{
	//״̬�ж�
	tagGlobalUserData & GlobalUserData=g_GlobalUnits.GetGolbalUserData();
	if (GlobalUserData.dwUserID==0L) return;

	//�л�ѯ��
	if (m_pRoomViewItem[0]!=NULL)
	{
		const TCHAR szMessage[]=TEXT("�л��û��ʺţ�����ر�������Ϸ���䣬ȷʵҪ�л��û��ʺ��� ");
		int iResult=ShowMessageBox(szMessage,MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2);
		if (iResult!=IDYES) return;
	}
	else
	{

	}

	//�رշ���
	ActivePlazaViewItem();
	CloseAllRoomViewItem();

	//ɾ����¼
	g_GlobalUnits.DeleteUserCookie();

	//���ñ���
	memset(&GlobalUserData,0,sizeof(GlobalUserData));

	//Ͷ����Ϣ
	m_DlgGamePlaza.SendLogonMessage();
}
//��ʾ��ҳ
void CGameFrame::WebBrowse(LPCTSTR lpszUrl, bool bForceBrowse)
{
	//����㳡
	ActivePlazaViewItem();

	//����ҳ
	if (&m_DlgGamePlaza!=NULL) 
	{
		m_DlgGamePlaza.WebBrowse(lpszUrl,bForceBrowse);
	}

	return;
}

//ϵͳ����
void CGameFrame::ShowSystemOption()
{
	//��ȡ����
	CGameOption * pGameOption=NULL;
	CServerOption * pServerOption=NULL;
	if (m_pActiveViewItem!=&m_DlgGamePlaza)
	{
		for (int i=0;i<CountArray(m_pRoomViewItem);i++)
		{
			if (m_pRoomViewItem[i]==NULL) break;
			if (m_pActiveViewItem==m_pRoomViewItem[i])
			{
				pGameOption=m_pRoomViewItem[i]->GetGameOption();
				pServerOption=m_pRoomViewItem[i]->GetServerOption();
				break;
			}
		}
	}

	//��ʾ����
	CDlgOption DlgOption(pGameOption,pServerOption);
	INT_PTR iResult=DlgOption.DoModal();
	if (iResult==IDOK)
	{
		for (int i=0;i<CountArray(m_pRoomViewItem);i++)
		{
			if (m_pRoomViewItem[i]==NULL) break;
			m_pRoomViewItem[i]->SendUserRule();
		}
	}

	return;
}

//��ʾ��Ϣ
int CGameFrame::ShowMessageBox(LPCTSTR pszMessage, UINT nType)
{
	INT nResult=ShowInformationEx(pszMessage,0,nType,TEXT("��Ϸ����"));
	return nResult;
}

//�б����
bool CGameFrame::ControlServerList(enServerListControl ServerListControl)
{
	//λ�ÿ���
	bool bHideGameList=false;
	switch (ServerListControl)
	{
	case ServerListControl_Hide: { bHideGameList=true; break; }
	case ServerListControl_Show: { bHideGameList=false; break; }
	case ServerListControl_Turn: { bHideGameList=!m_bHideGameList; break; }
	}

	//�����ж�
	if (bHideGameList==m_bHideGameList) return false;

	//���ñ���
	m_bRectify=false;
	m_bHideGameList=bHideGameList;

	//�������
	//m_UserInfoView.ShowWindow((bHideGameList==false)?SW_SHOW:SW_HIDE);
	//m_ServerItmOpaint.ShowWindow((bHideGameList==false)?SW_SHOW:SW_HIDE);
	OnCommand(m_wShowListType,0);

	//���ð�ť
	UINT uControlId = ((m_bHideGameList)?GetPlatformRes().uBtShowGameList:GetPlatformRes().uBtHideGameList);
	m_Splitter.ShowSplitterButton(uControlId,GetResInstanceHandle());

	//��������
	CRect rcClient;
	GetClientRect(&rcClient);
	RectifyControl(rcClient.Width(),rcClient.Height());

	return true;
}

//��Դ���
HINSTANCE CGameFrame::GetResInstanceHandle()
{
	return g_GlobalUnits.m_PlatformResourceModule->GetResInstance();
}

//��ȡ��Դ
tagPlatformFrameImage CGameFrame::GetPlatformRes()
{
	return g_GlobalUnits.m_PlatformFrameImage;
}

//������Դ
bool CGameFrame::UpdateSkinResource()
{
	tagControlViewImage & ControlViewImage=g_GlobalUnits.m_ControlViewImage;
	HINSTANCE hInstance=g_GlobalUnits.m_PlatformResourceModule->GetResInstance();

// 	m_ImageServerT.SetLoadInfo(ControlViewImage.uServerIfnoT,hInstance);
// 	m_ImageServerM.SetLoadInfo(ControlViewImage.uServerIfnoM,hInstance);
// 	m_ImageServerB.SetLoadInfo(ControlViewImage.uServerIfnoB,hInstance);

	m_ImageServerT.SetLoadInfo(szResPath("Frame\\SERVER_LIST_T.BMP"));
	m_ImageServerM.SetLoadInfo(szResPath("Frame\\SERVER_LIST_M.BMP"));
	m_ImageServerB.SetLoadInfo(szResPath("Frame\\SERVER_LIST_B.BMP"));

	//��ȡ��Դ
	tagPlatformFrameImage & PlatformFrameImage=GetPlatformRes();
	HINSTANCE hResInstance=GetResInstanceHandle();

//	m_ImageFrameMidT.SetLoadInfo(IDB_FRAME_MID_JIANBIAN, AfxGetInstanceHandle());
//	m_ImageFrameMidB.SetLoadInfo(IDB_FRAME_MID_JIANBIAN, AfxGetInstanceHandle());

	m_ImageFrameMidT.SetLoadInfo(szResPath("Frame\\FrameMidT.bmp"));
	m_ImageFrameMidB.SetLoadInfo(szResPath("Frame\\FrameMidB.bmp"));

	//������Դ
// 	m_EncircleFrame.ImageML.SetLoadInfo(PlatformFrameImage.uFrameML,hResInstance);
// 	m_EncircleFrame.ImageMR.SetLoadInfo(PlatformFrameImage.uFrameMR,hResInstance);
// 	m_EncircleFrame.ImageTL.SetLoadInfo(PlatformFrameImage.uFrameTL,hResInstance);
// 	m_EncircleFrame.ImageTM.SetLoadInfo(PlatformFrameImage.uFrameTM,hResInstance);
// 	m_EncircleFrame.ImageTR.SetLoadInfo(PlatformFrameImage.uFrameTR,hResInstance);
// 	m_EncircleFrame.ImageBL.SetLoadInfo(PlatformFrameImage.uFrameBL,hResInstance);
// 	m_EncircleFrame.ImageBM.SetLoadInfo(PlatformFrameImage.uFrameBM,hResInstance);
// 	m_EncircleFrame.ImageBR.SetLoadInfo(PlatformFrameImage.uFrameBR,hResInstance);

	m_EncircleFrame.ImageML.SetLoadInfo(szResPath("Frame\\FRAME_ML.bmp"));
	m_EncircleFrame.ImageMR.SetLoadInfo(szResPath("Frame\\FRAME_MR.bmp"));
	m_EncircleFrame.ImageTL.SetLoadInfo(szResPath("Frame\\FRAME_TL.bmp"));
	m_EncircleFrame.ImageTM.SetLoadInfo(szResPath("Frame\\FRAME_TM.bmp"));
	m_EncircleFrame.ImageTR.SetLoadInfo(szResPath("Frame\\FRAME_TR.bmp"));
	m_EncircleFrame.ImageBL.SetLoadInfo(szResPath("Frame\\FRAME_BL.bmp"));
	m_EncircleFrame.ImageBM.SetLoadInfo(szResPath("Frame\\FRAME_BM.bmp"));
	m_EncircleFrame.ImageBR.SetLoadInfo(szResPath("Frame\\FRAME_BR.bmp"));

// 	//�������б�
// 	m_EncircleServer.ImageML.SetLoadInfo(PlatformFrameImage.uServerML,hResInstance);
// 	m_EncircleServer.ImageMR.SetLoadInfo(PlatformFrameImage.uServerMR,hResInstance);
// 	m_EncircleServer.ImageTL.SetLoadInfo(PlatformFrameImage.uServerTL,hResInstance);
// 	m_EncircleServer.ImageTM.SetLoadInfo(PlatformFrameImage.uServerTM,hResInstance);
// 	m_EncircleServer.ImageTR.SetLoadInfo(PlatformFrameImage.uServerTR,hResInstance);
// 	m_EncircleServer.ImageBL.SetLoadInfo(PlatformFrameImage.uServerBL,hResInstance);
// 	m_EncircleServer.ImageBM.SetLoadInfo(PlatformFrameImage.uServerBM,hResInstance);
// 	m_EncircleServer.ImageBR.SetLoadInfo(PlatformFrameImage.uServerBR,hResInstance);

	//��ȡ��Ϣ
	CSkinAide::GetEncircleInfo(m_ImageInfoFrame,m_EncircleFrame);
//	CSkinAide::GetEncircleInfo(m_ImageInfoServer,m_EncircleServer);

	//������ؼ�
	UINT uControlId = ((m_bHideGameList)?PlatformFrameImage.uBtShowGameList:PlatformFrameImage.uBtHideGameList);
	m_Splitter.ShowSplitterButton(uControlId,hResInstance);

	//m_btMin.SetButtonImage(szResPath("Frame\\btMin.png"),false);
	//����ͼƬ
// 	m_btMin.SetButtonImage(PlatformFrameImage.uBtFrameMin,hResInstance,false);
// 	m_btMax.SetButtonImage((m_bMaxShow)?PlatformFrameImage.uBtFrameResore:PlatformFrameImage.uBtFrameMax,hResInstance,false);
// 	m_btClose.SetButtonImage(PlatformFrameImage.uBtFrameClose,hResInstance,false);
	m_btExChangeSkin.SetButtonImage(PlatformFrameImage.uBtFrameClose,hResInstance,false);
// 	m_btButton1.SetButtonImage(PlatformFrameImage.uFrameNavigation[0],hResInstance,false);
// 	m_btButton2.SetButtonImage(PlatformFrameImage.uFrameNavigation[1],hResInstance,false);
// 	m_btButton3.SetButtonImage(PlatformFrameImage.uFrameNavigation[2],hResInstance,false);
// 	m_btButton4.SetButtonImage(PlatformFrameImage.uFrameNavigation[3],hResInstance,false);
// 	m_btButton5.SetButtonImage(PlatformFrameImage.uFrameNavigation[4],hResInstance,false);
// 	m_btButton6.SetButtonImage(PlatformFrameImage.uFrameNavigation[5],hResInstance,false);
// 	m_btButton7.SetButtonImage(PlatformFrameImage.uFrameNavigation[6],hResInstance,false);

// 	m_btButton1.SetButtonImage(TEXT(szResPath("Frame\\NavigatBtn1.png"),false);
// 	m_btButton2.SetButtonImage(TEXT(szResPath("Frame\\NavigatBtn2.png"),false);
// 	m_btButton3.SetButtonImage(TEXT(szResPath("Frame\\NavigatBtn3.png"),false);
// 	m_btButton4.SetButtonImage(TEXT(szResPath("Frame\\NavigatBtn4.png"),false);
// 	m_btButton5.SetButtonImage(TEXT(szResPath("Frame\\NavigatBtn5.png"),false);
// 	m_btButton6.SetButtonImage(TEXT(szResPath("Frame\\NavigatBtn6.png"),false);
// 	m_btButton7.SetButtonImage(TEXT(szResPath("Frame\\NavigatBtn7.png"),false);

	m_btButton1.SetButtonImage(szResPath("Frame\\NavigatBtn1.bmp"),false);
	m_btButton2.SetButtonImage(szResPath("Frame\\NavigatBtn2.bmp"),false);
	m_btButton3.SetButtonImage(szResPath("Frame\\NavigatBtn3.bmp"),false);
	m_btButton4.SetButtonImage(szResPath("Frame\\NavigatBtn4.bmp"),false);
	m_btButton5.SetButtonImage(szResPath("Frame\\NavigatBtn5.bmp"),false);
	m_btButton6.SetButtonImage(szResPath("Frame\\NavigatBtn6.bmp"),false);
	m_btButton7.SetButtonImage(szResPath("Frame\\NavigatBtn7.bmp"),false);

	//m_btButton1.LoadStdImage(szResPath("LogonDlg\\BtStart.png", true);

	m_btMenu.SetButtonImage(IDB_PLAZA_MENU, AfxGetInstanceHandle(), false);
	m_btMenu.SetButtonImage(szResPath("Frame\\FRAME_BT_MENU.bmp"),false);
	m_btMin.SetButtonImage(szResPath("Frame\\FRAME_BT_MIN.bmp"),false);
	if (m_bMaxShow)
		m_btMax.SetButtonImage(szResPath("Frame\\FRAME_BT_RESORE.bmp"),false);
	else
		m_btMax.SetButtonImage(szResPath("Frame\\FRAME_BT_MAX.bmp"),false);

	m_btClose.SetButtonImage(szResPath("Frame\\FRAME_BT_CLOSE.bmp"),false);

	//m_btHelp.SetButtonImage(IDB_PLAZA_HELP, AfxGetInstanceHandle(), false);

	//m_btSwitchUser.SetButtonImage(PlatformFrameImage.uBtFrameSwitch,hResInstance,false);
	m_btGlobalOption.SetButtonImage(PlatformFrameImage.uBtFrameOption,hResInstance,false);
	//m_btSelfOption.SetButtonImage(PlatformFrameImage.uBtFrameSelfSet,hResInstance,false);

	m_BrowerToolBarAd.ShowWindow(0);
	//�û���Ϣ
	m_btFanKui.LoadStdImage(szResPath("Frame\\fankui.png"), true);
	m_btMember.LoadStdImage(szResPath("Frame\\vip.png"), true);
	m_btSafe.LoadStdImage(szResPath("Frame\\safe.png"), true);
	m_btBbs.LoadStdImage(szResPath("Frame\\bbs.png"), true);
	m_btFont.LoadStdImage(szResPath("Frame\\font.png"), true);
	//�ͻ���ť;

	//�б�ť
	UINT uControl1=((IDC_BT_LIST_CONTROL_1==m_wShowListType)?PlatformFrameImage.uFrameServiceBtShow1:PlatformFrameImage.uFrameServiceBtHide1);
	UINT uControl2=((IDC_BT_LIST_CONTROL_2==m_wShowListType)?PlatformFrameImage.uFrameServiceBtShow2:PlatformFrameImage.uFrameServiceBtHide2);
	//m_btListControl1.SetButtonImage(uControl1,hResInstance,false);
	//m_btListControl2.SetButtonImage(uControl2,hResInstance,false);

	//����ͼƬ
	int i;
	for (i=0;i<CountArray(m_pRoomViewItem);i++)
	{
		if(m_pActiveViewItem==m_pRoomViewItem[i])break;
	}
	CSkinButton * pTempButton[]={&m_btViewItem[0],&m_btViewItem[1],&m_btViewItem[2],&m_btGamePlaza};
	ASSERT((CountArray(m_btViewItem)+1)==CountArray(pTempButton));
	for (WORD j=0;j<CountArray(pTempButton);j++)
	{
		if(i==j)
		{
			pTempButton[j]->SetTextColor(RGB(0,0,0),RGB(133,215,255),true);
			pTempButton[j]->SetButtonImage(PlatformFrameImage.uBtFrameItemActive,hResInstance,false);
		}
		else
		{
			pTempButton[j]->SetTextColor(RGB(245,245,245),RGB(20,20,20),true);
			pTempButton[j]->SetButtonImage(PlatformFrameImage.uBtFrameItemNormal,hResInstance,false);
		}
	}
	return true;
}

//����㳡
void CGameFrame::ActivePlazaViewItem()
{
	//��ȡ����
	HWND hWndActive=NULL;
	if (m_pActiveViewItem!=NULL) hWndActive=m_pActiveViewItem->GetWindowHandle();
	if (m_DlgGamePlaza.GetSafeHwnd()!=hWndActive)
	{
		m_pActiveViewItem=&m_DlgGamePlaza;
		m_DlgGamePlaza.ShowWindow(SW_SHOW);
		m_btGlobalOption.ShowWindow(SW_HIDE);
		if (hWndActive!=NULL) ::ShowWindow(hWndActive,SW_HIDE);

		//��ʾ�б�
		if(!ControlServerList(ServerListControl_Show))
		{
			//��������
			CRect rcClient;
			GetClientRect(&rcClient);

			RectifyControl(rcClient.Width(),rcClient.Height());
		}
	}

	return;
}

//�����
void CGameFrame::ActiveRoomViewItem(CRoomViewItem * pRoomViewItem)
{
	//Ч�����
	ASSERT(pRoomViewItem!=NULL);
	if (pRoomViewItem==NULL) return;

	//�жϴ���
	if (m_pRoomViewItemCreate==pRoomViewItem)
	{
		m_pRoomViewItemCreate=NULL;
		MoveMemory(m_pRoomViewItem+1,m_pRoomViewItem,sizeof(m_pRoomViewItem[0])*(CountArray(m_pRoomViewItem)-1));
		m_pRoomViewItem[0]=pRoomViewItem;
	}

	//�����
	HWND hWndActive=NULL;
	if (m_pActiveViewItem!=NULL) hWndActive=m_pActiveViewItem->GetWindowHandle();
	if (pRoomViewItem->GetSafeHwnd()!=hWndActive)
	{
		m_pActiveViewItem=pRoomViewItem;
		pRoomViewItem->ShowWindow(SW_SHOW);
		if (hWndActive!=NULL) ::ShowWindow(hWndActive,SW_HIDE);

		//�����б�
		if(!ControlServerList(ServerListControl_Hide))
		{
			//��������
			CRect rcClient;
			GetClientRect(&rcClient);

			RectifyControl(rcClient.Width(),rcClient.Height());
		}
	}

	//���¿���
	UpdateControlButton();

	return;
}

//���ҷ���
CRoomViewItem * CGameFrame::SearchRoomViewItem(WORD wKindID, WORD wServerID)
{
	tagGameServer * pGameServer=NULL;
	for (int i=0;i<CountArray(m_pRoomViewItem);i++)
	{
		if (m_pRoomViewItem[i]==NULL) return NULL;
		pGameServer=m_pRoomViewItem[i]->GetServerInfo();
		if ((pGameServer->wKindID==wKindID)&&(pGameServer->wServerID==wServerID)) return m_pRoomViewItem[i];
	}

	return NULL;
}

//���뷿��
CRoomViewItem * CGameFrame::CreateRoomViewItem(CListServer * pListServer)
{
	//�ж�״̬
	tagGlobalUserData & GlobalUserData=g_GlobalUnits.GetGolbalUserData();
	if (GlobalUserData.dwUserID==0L)
	{
		ShowMessageBox(TEXT("�㻹û�е�¼�����ȵ�¼��Ϸ�㳡��"),MB_ICONQUESTION);
		return NULL;
	}

	//Ч�����
	ASSERT(pListServer!=NULL);
	CListKind * pListKind=pListServer->GetListKind();
	tagGameKind * pGameKind=pListKind->GetItemInfo();
	tagGameServer * pGameServer=pListServer->GetItemInfo();

	//�ж�����
	if (m_pRoomViewItemCreate!=NULL)
	{
		tagGameServer * pGameServerCreate=m_pRoomViewItemCreate->GetServerInfo();
		if ((pGameServer->wKindID==pGameServerCreate->wKindID)&&
			(pGameServer->wServerID==pGameServerCreate->wServerID))
			return NULL;
	}

	//Ѱ�ҷ���
	CRoomViewItem * pRoomViewItem=SearchRoomViewItem(pGameServer->wKindID,pGameServer->wServerID);
	if (pRoomViewItem!=NULL) 
	{
		ActiveRoomViewItem(pRoomViewItem);
		return pRoomViewItem;
	}

	//���뷿����Ŀ
	if (m_pRoomViewItem[CountArray(m_pRoomViewItem)-1]!=NULL)
	{
		ShowMessageBox(TEXT("�������Ϸ����̫���ˣ���رղ�����Ϸ�������ԣ�"),MB_ICONINFORMATION);
		return NULL;
	}

	//��ȡ�汾
	CWinFileInfo WinFileInfo;
	if (WinFileInfo.OpenWinFile(pGameKind->szProcessName)==false)
	{
		TCHAR szBuffer[512]=TEXT("");
		_snprintf(szBuffer,sizeof(szBuffer),TEXT("��%s����û�а�װ�������Ƿ����أ�"),pGameKind->szKindName);
		int nResult=ShowMessageBox(szBuffer,MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON1);
		if (nResult==IDYES) 
			g_GlobalAttemper.DownLoadClient(pGameKind->szKindName,pGameKind->wKindID,true);
		return NULL;
	}

	//��ȡ�汾
	DWORD dwFileVerMS=0L,dwFileVerLS=0L;
	WinFileInfo.GetFileVersion(dwFileVerMS,dwFileVerLS);

	//�汾����
	BYTE cbFileVer1=(BYTE)(HIWORD(dwFileVerMS));
	BYTE cbFileVer2=(BYTE)(LOWORD(dwFileVerMS));
	BYTE cbFileVer3=(BYTE)(HIWORD(dwFileVerLS));
	BYTE cbFileVer4=(BYTE)(LOWORD(dwFileVerLS));
	BYTE cbListVer1=(BYTE)(LOWORD(LOWORD(pGameKind->dwMaxVersion)));
	BYTE cbListVer2=(BYTE)(HIBYTE(LOWORD(pGameKind->dwMaxVersion)));
	BYTE cbListVer3=(BYTE)(LOBYTE(HIWORD(pGameKind->dwMaxVersion)));
	BYTE cbListVer4=(BYTE)(HIBYTE(HIWORD(pGameKind->dwMaxVersion)));

	//�жϰ汾
	if ((cbFileVer1!=cbListVer1)||(cbFileVer2!=cbListVer2)||(cbFileVer3!=cbListVer3))
	{
		TCHAR szBuffer[512]=TEXT("");
		_snprintf(szBuffer,sizeof(szBuffer),TEXT("��%s���Ѿ�����Ϊ %ld.%ld.%ld.%ld �汾����İ汾���ܼ���ʹ�ã������Ƿ����أ�"),pGameKind->szKindName,
			cbListVer1,cbListVer2,cbListVer3,cbListVer4);
		int nResult=ShowMessageBox(szBuffer,MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON1);
		if (nResult==IDYES) 
			g_GlobalAttemper.DownLoadClient(pGameKind->szKindName,pGameKind->wKindID,true);
		return NULL;
	}

	//���ݰ汾
	if (cbFileVer4!=cbListVer4)
	{
		TCHAR szBuffer[512]=TEXT("");
		_snprintf(szBuffer,sizeof(szBuffer),TEXT("��%s���Ѿ�����Ϊ %ld.%ld.%ld.%ld �汾�������Ƿ�����������"),pGameKind->szKindName,
			cbListVer1,cbListVer2,cbListVer3,cbListVer4);
		int nResult=ShowMessageBox(szBuffer,MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON1);
		if (nResult==IDYES) 
		{
			g_GlobalAttemper.DownLoadClient(pGameKind->szKindName,pGameKind->wKindID,true);
			return NULL;
		}
	}

	//��������
	try
	{
		pRoomViewItem=new CRoomViewItem;
		if (pRoomViewItem==NULL) return NULL;
		pRoomViewItem->Create(IDD_GAME_ROOM,AfxGetMainWnd());
		if (pRoomViewItem->InitRoomViewItem(pListServer)==false) throw TEXT("��Ϸ�����ʼ��ʧ�ܣ�");
	}
	catch (...) { SafeDelete(pRoomViewItem); }
	if (pRoomViewItem==NULL)
	{
		ShowMessageBox(TEXT("��Ϸ���䴴��ʧ��"),MB_ICONQUESTION);
		return NULL;
	}

	//������Ϸ
	bool bSuccess=pRoomViewItem->ConnectGameServer();
	if (bSuccess==false)
	{
		pRoomViewItem->DestroyWindow();
		SafeDelete(pRoomViewItem);
		return NULL;
	}

	//���ñ���
	m_pRoomViewItemCreate=pRoomViewItem;

	//������ʾ
	CString strBuffer;
	strBuffer.Format(TEXT("�������ӷ��䣬���Ժ�..."));
	g_GlobalAttemper.ShowStatusMessage(strBuffer,pRoomViewItem);

	// 	m_btLock.ShowWindow(SW_HIDE);
	// 	m_btUnlock.ShowWindow(SW_HIDE);
	// 	m_btVideo.ShowWindow(SW_HIDE);
	// 	m_btMobile.ShowWindow(SW_HIDE);
	// 	m_btHongzuan.ShowWindow(SW_HIDE);
	// 	m_btUnzuan.ShowWindow(SW_HIDE);
	// 	m_btJianghu.ShowWindow(SW_HIDE);
	// 	m_btSet.ShowWindow(SW_HIDE);
	//m_btGlobalOption.ShowWindow(SW_SHOW);
	//m_btLockroom.ShowWindow(SW_SHOW);

	return m_pRoomViewItemCreate;
}

//�رշ���
void CGameFrame::CloseRoomViewItem(IRoomViewItem * pIRoomViewItem)
{
	//Ч�����
	ASSERT(pIRoomViewItem!=NULL);
	if (pIRoomViewItem==NULL) return;

	//���ñ���
	if (m_pRoomViewItemCreate!=pIRoomViewItem)
	{
		//�ر���ʾ
		if (pIRoomViewItem->QueryCloseRoom()==false) return;

		//Ѱ�ҷ���
		for (int i=0;i<CountArray(m_pRoomViewItem);i++)
		{
			if (m_pRoomViewItem[i]==NULL) break;
			if (m_pRoomViewItem[i]==pIRoomViewItem) 
			{
				//�ƶ�����
				if ((i+1)!=CountArray(m_pRoomViewItem))
				{
					int nMoveSize=(CountArray(m_pRoomViewItem)-i-1)*sizeof(m_pRoomViewItem[0]);
					MoveMemory(m_pRoomViewItem+i,m_pRoomViewItem+i+1,nMoveSize);
				}
				m_pRoomViewItem[CountArray(m_pRoomViewItem)-1]=NULL;

				//������ͼ
				if (m_pActiveViewItem==pIRoomViewItem)
				{
					int nActivePos=CountArray(m_pRoomViewItem);
					for (int i=CountArray(m_pRoomViewItem)-1;i>=0;i--)
					{
						if (m_pRoomViewItem[i]!=NULL)
						{
							nActivePos=i;
							break;
						}
					}
					if (nActivePos==CountArray(m_pRoomViewItem)) ActivePlazaViewItem();
					else ActiveRoomViewItem(m_pRoomViewItem[nActivePos]);
				}

				//���¿���
				UpdateControlButton();

				break;
			}
		}
	}
	else m_pRoomViewItemCreate=NULL;

	//�رշ���
	pIRoomViewItem->CloseRoomViewItem();
	PostMessage(WM_CLOSE_ROOM_VIEW_ITEM,(WPARAM)pIRoomViewItem,(LPARAM)pIRoomViewItem);

	// 		m_btLock.ShowWindow(SW_SHOW);
	// 	m_btUnlock.ShowWindow(SW_SHOW);
	// 	m_btVideo.ShowWindow(SW_SHOW);
	// 	m_btMobile.ShowWindow(SW_SHOW);
	// 	m_btHongzuan.ShowWindow(SW_SHOW);
	// 	m_btUnzuan.ShowWindow(SW_SHOW);
	// 	m_btJianghu.ShowWindow(SW_SHOW);
	// 	m_btSet.ShowWindow(SW_SHOW);
	m_btGlobalOption.ShowWindow(SW_HIDE);
	//m_btLockroom.ShowWindow(SW_HIDE);
	return;
}

//�رշ���
void CGameFrame::CloseAllRoomViewItem()
{
	//�رշ���
	IRoomViewItem * pIRoomViewItem=NULL;
	for (int i=0;i<CountArray(m_pRoomViewItem);i++)
	{
		if (m_pRoomViewItem[i]==NULL) break;

		//�رշ���
		pIRoomViewItem=QUERY_OBJECT_PTR_INTERFACE(m_pRoomViewItem[i],IRoomViewItem);
		pIRoomViewItem->CloseRoomViewItem();
		PostMessage(WM_CLOSE_ROOM_VIEW_ITEM,(WPARAM)pIRoomViewItem,(LPARAM)pIRoomViewItem);

		//���ñ���
		m_pRoomViewItem[i]=NULL;
	}

	//�رմ�������
	if (m_pRoomViewItemCreate!=NULL) 
	{
		//�رշ���
		g_GlobalAttemper.DestroyStatusWnd(NULL);
		m_pRoomViewItemCreate->CloseRoomViewItem();
		pIRoomViewItem=QUERY_OBJECT_PTR_INTERFACE(m_pRoomViewItemCreate,IRoomViewItem);
		PostMessage(WM_CLOSE_ROOM_VIEW_ITEM,(WPARAM)pIRoomViewItem,(LPARAM)pIRoomViewItem);

		//���ñ���
		m_pRoomViewItemCreate=NULL;
	}

	//���¿���
	UpdateControlButton();

	return;
}

//�رյ�ǰ
void CGameFrame::CloseCurrentViewItem()
{
	//�����ж�
	ASSERT(m_pActiveViewItem!=NULL);
	if (m_pActiveViewItem!=&m_DlgGamePlaza)
	{
		IRoomViewItem * pIRoomViewItem=QUERY_OBJECT_PTR_INTERFACE(m_pActiveViewItem,IRoomViewItem);
		ASSERT(pIRoomViewItem!=NULL);
		CloseRoomViewItem(pIRoomViewItem);
	}
	else 
	{
		 	 m_pTuiChu=new CDTuiChu(this);
		      m_pTuiChu->Create(IDD_BANK_STORAGE1,this);
		      m_pTuiChu->SetSendInfo(NULL,NULL);
		      m_pTuiChu->ShowWindow(SW_SHOW);

		// 		//��ʾ��Ϣ
		 		//int iRet=ShowInformation(TEXT("��Ϸ������Ҫ�رգ�ȷʵҪ�˳���"),0,MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2);
		 		//if(iRet==IDYES)
				//	AfxGetMainWnd()->PostMessage(WM_CLOSE,0,0);
	} 
	//
	return;
}

//���¿���
void CGameFrame::UpdateControlButton()
{
	//���°�ť
	for (int i=0;i<CountArray(m_btViewItem);i++)
	{
		if (m_pRoomViewItem[i]!=NULL)
		{
			tagGameKind * pGameKind=m_pRoomViewItem[i]->GetKindInfo();
			m_btViewItem[i].ShowWindow(SW_SHOW);
			//m_btViewItem[i].SetTextColor(RGB(255,0,0));
			m_btViewItem[i].SetWindowText(pGameKind->szKindName);
		}
		else m_btViewItem[i].ShowWindow(SW_HIDE);
	}
	return;
}

//�����ؼ�
void CGameFrame::RectifyControl(int nWidth, int nHeight)
{

	if (m_DlgGamePlaza.m_bInitDialog==false) return;
	if ((nWidth==0)||(nHeight==0)) return;

	int nFullWidth=GetSystemMetrics(SM_CXSCREEN);
	int nFullHeigh=GetSystemMetrics(SM_CYSCREEN);

	int FullSize;
	FullSize=43;
	//��������
	const int nSplitterWidth=__max(6,m_Splitter.GetButtonSize().cx);

	//��ȡλ��
	CRect rcSplitter;
	m_Splitter.GetWindowRect(&rcSplitter);
	ScreenToClient(&rcSplitter);

	UINT iJL=4;
	UINT iJM = 3;
	UINT iJR = 4;
	//
	CImageHandle HandleServerT(&m_ImageServerT);

	//λ�õ���
	rcSplitter.top=m_ImageInfoFrame.nTBorder-FRAME_EXALTATION;
	rcSplitter.bottom=nHeight-m_ImageInfoFrame.nBBorder;

	//�����ؼ�
	HDWP hDwp=BeginDeferWindowPos(32);
	UINT uUserTitleHeight=m_UserInfoView.GetTitleHeight();
	//UINT uGameListTitleHeight=m_UserGameListTop.GetTitleHeight();
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS;
	DeferWindowPos(hDwp,m_Splitter,NULL,0,0,0,0,uFlags);

	//�б�ؼ�
	if(!m_bHideGameList)
	{
		CRect rcServerViewIni(m_ImageInfoFrame.nLBorder+7,m_ImageInfoFrame.nTBorder+m_ImageServerT.GetHeight(),
			m_ImageServerT.GetWidth()-9+iJL,/*nHeight - */(m_ImageInfoFrame.nBBorder+m_ImageServerT.GetHeight()+m_ImageInfoFrame.nTBorder+iJL));

		GetRectFromIni(szResPath("GameFrame.ini"), TEXT("Frame"), TEXT("ServerView"), rcServerViewIni, rcServerViewIni);
		rcServerViewIni.bottom = nHeight - rcServerViewIni.bottom;
		m_rcServerListView = rcServerViewIni;
		DeferWindowPos(hDwp,m_ServerItemView,NULL,rcServerViewIni.left, rcServerViewIni.top, rcServerViewIni.right, rcServerViewIni.bottom,uFlags);

		CPoint ptUserInfoIni;
		UINT uInfoStype = 40;
		GetPointFromIni(szResPath("GameFrame.ini"), TEXT("Frame"), TEXT("UserInfoBtn"), ptUserInfoIni,ptUserInfoIni);
		DeferWindowPos(hDwp, m_btSafe,NULL, ptUserInfoIni.x, ptUserInfoIni.y, 0,0, uFlags|SWP_NOSIZE);
		DeferWindowPos(hDwp, m_btMember, NULL,ptUserInfoIni.x+uInfoStype*1, ptUserInfoIni.y, 0,0, uFlags|SWP_NOSIZE);
		DeferWindowPos(hDwp, m_btFont,NULL, ptUserInfoIni.x+uInfoStype*2, ptUserInfoIni.y, 0,0, uFlags|SWP_NOSIZE);
		DeferWindowPos(hDwp, m_btBbs,NULL, ptUserInfoIni.x+uInfoStype*3, ptUserInfoIni.y, 0,0, uFlags|SWP_NOSIZE);
		DeferWindowPos(hDwp, m_btFanKui,NULL, ptUserInfoIni.x+uInfoStype*4, ptUserInfoIni.y, 0,0, uFlags|SWP_NOSIZE);
		m_btSafe.ShowWindow(1);m_btMember.ShowWindow(1);m_btFanKui.ShowWindow(1);m_btBbs.ShowWindow(1);m_btFont.ShowWindow(1);

	}else{
		m_btSafe.ShowWindow(0);m_btMember.ShowWindow(0);m_btFanKui.ShowWindow(0);m_btBbs.ShowWindow(0);m_btFont.ShowWindow(0);
	}
	//��ȡλ��
	CRect rcItemRect;
	m_btGamePlaza.GetWindowRect(&rcItemRect);

	//�ƶ��ؼ�
	INT nYPos = m_ImageInfoFrame.nTBorder;
	DeferWindowPos(hDwp,m_btGamePlaza,NULL,8/*rcSplitter.right-62-m_ImageInfoFrame.nLBorder*/,nHeight-30,0,0,uFlags|SWP_NOSIZE);
	for (int i=0;i<CountArray(m_btViewItem);i++) 
		DeferWindowPos(hDwp,m_btViewItem[i],NULL,/*rcSplitter.right-62-m_ImageInfoFrame.nLBorder*/8+rcItemRect.Width()*i+rcItemRect.Width(),nHeight-30,0,0,uFlags|SWP_NOSIZE);

	//�в������λ��
	
	HWND hWndActive=m_pActiveViewItem->GetWindowHandle();
	if(!m_bHideGameList)//�����
	{
		CRect rcBrowseIni(m_ImageInfoFrame.nLBorder+m_ImageServerT.GetWidth()+iJL+iJM,nYPos,nWidth-m_ImageInfoFrame.nRBorder-m_ImageInfoFrame.nLBorder-m_ImageServerT.GetWidth()-iJL-iJM-iJR,nHeight-m_ImageInfoFrame.nTBorder-m_ImageInfoFrame.nBBorder);
		int x = 0-m_ImageInfoFrame.nRBorder-m_ImageInfoFrame.nLBorder-m_ImageServerT.GetWidth()-iJL-iJM-iJR;
		int y = 0-m_ImageInfoFrame.nTBorder-m_ImageInfoFrame.nBBorder;
		GetRectFromIni(szResPath("GameFrame.ini"), TEXT("Frame"), TEXT("BrowseView"), rcBrowseIni, rcBrowseIni);
		rcBrowseIni.right = nWidth - rcBrowseIni.right;
		rcBrowseIni.bottom = nHeight - rcBrowseIni.bottom;
		m_rcBrowser= rcBrowseIni;
		DeferWindowPos(hDwp,hWndActive,NULL,rcBrowseIni.left, rcBrowseIni.top, rcBrowseIni.right, rcBrowseIni.bottom, uFlags);
	}
	else//����
	{
		CRect rcRoomViewIni(m_ImageInfoFrame.nLBorder+iJL,nYPos,0-m_ImageInfoFrame.nRBorder-rcSplitter.right+64+m_ImageInfoFrame.nLBorder+5,0-m_ImageInfoFrame.nTBorder-m_ImageInfoFrame.nBBorder);
		int x = 0-m_ImageInfoFrame.nRBorder-rcSplitter.right+64+m_ImageInfoFrame.nLBorder+5;
		int y=0-m_ImageInfoFrame.nTBorder-m_ImageInfoFrame.nBBorder;
		GetRectFromIni(szResPath("GameFrame.ini"), TEXT("Frame"), TEXT("RoomView"), rcRoomViewIni, rcRoomViewIni);
		rcRoomViewIni.right = nWidth-rcRoomViewIni.right;
		rcRoomViewIni.bottom = nHeight-rcRoomViewIni.bottom;
		m_rcRoomView = rcRoomViewIni;
		DeferWindowPos(hDwp,hWndActive,NULL,rcRoomViewIni.left, rcRoomViewIni.top, rcRoomViewIni.right, rcRoomViewIni.bottom,uFlags);
	}

	//�����ؼ�
	CPoint ptMaxIni,ptMinIni,ptCloseIni,ptMenuIni;
	GetPointFromIni(szResPath("GameFrame.ini"), TEXT("Frame"), TEXT("btMax"), ptMaxIni, ptMaxIni);
	GetPointFromIni(szResPath("GameFrame.ini"), TEXT("Frame"), TEXT("btMin"), ptMinIni, ptMinIni);
	GetPointFromIni(szResPath("GameFrame.ini"), TEXT("Frame"), TEXT("btClose"), ptCloseIni, ptCloseIni);
	GetPointFromIni(szResPath("GameFrame.ini"), TEXT("Frame"), TEXT("btMenu"), ptMenuIni, ptMenuIni);
	ptMinIni.x = nWidth-ptMinIni.x;ptMaxIni.x = nWidth-ptMaxIni.x;ptCloseIni.x = nWidth-ptCloseIni.x;ptMenuIni.x = nWidth-ptMenuIni.x;

	DeferWindowPos(hDwp,m_btMin,NULL,ptMinIni.x,ptMinIni.y,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btMax,NULL,ptMaxIni.x, ptMaxIni.y,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btClose,NULL,ptCloseIni.x, ptCloseIni.y,0,0,uFlags|SWP_NOSIZE);
	//DeferWindowPos(hDwp,m_btExChangeSkin,NULL,nWidth-153,4,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btMenu,NULL,ptMenuIni.x,ptMenuIni.y,0,0,uFlags|SWP_NOSIZE);
	//DeferWindowPos(hDwp,m_btHelp,NULL,nWidth-170+6,0,0,0,uFlags|SWP_NOSIZE);
	//�б�ť
	//m_btListControl1.GetWindowRect(&rcItemRect);
	INT nY = m_ImageInfoFrame.nTBorder+uUserTitleHeight+5;
	//DeferWindowPos(hDwp,//m_btListControl1,NULL,m_ImageInfoFrame.nLBorder+5,nY,0,0,uFlags|SWP_NOSIZE);
	//DeferWindowPos(hDwp,m_btListControl2,NULL,m_ImageInfoFrame.nLBorder+5,nY+rcItemRect.Height()+1,0,0,uFlags|SWP_NOSIZE);

	//��ťλ��
	CRect rcButton;
	m_btButton1.GetWindowRect(&rcButton);

	//�������
	int nEndPos=305;
	int nBeginPos=560;
	int nWindth=nWidth-nBeginPos-rcButton.Width()*5-nEndPos;
	int nButtonSpace=__min((nWidth-nBeginPos-rcButton.Width()*5-nEndPos)/4,30);

	//UINT uFlagsAd=uFlags|(m_bIsMax?SWP_SHOWWINDOW:SWP_HIDEWINDOW);
	//���ؼ�

// 	if(m_bMaxShow)
// 	{
// 		//CRect rcAdIni;
// 		//GetRectFromIni(szResPath("GameFrame.ini"), TEXT("Frame"), TEXT("TopBarAd"), rcAdIni, rcAdIni);
// 		//rcAdIni.left = nWidth - rcAdIni.left;
// 		//rcAdIni.right = nWidth - rcAdIni.right;
// 		int nWidthAd = 
// 		
// 		DeferWindowPos(hDwp,m_BrowerAD,NULL,rcAdIni.left,rcAdIni.top,rcAdIni.Width(),rcAdIni.Height(),uFlags);
// 	}else
		DeferWindowPos(hDwp,m_BrowerAD,NULL,0,0,0,0,uFlags);

	UINT uBtYpos = 2;
	CPoint ptNavigatBtnIni;
	UINT uBtStype = 70;
	GetPointFromIni(szResPath("GameFrame.ini"), TEXT("Frame"), TEXT("NavigatBtn"), ptNavigatBtnIni, ptNavigatBtnIni);
	DeferWindowPos(hDwp,m_btButton1,NULL,ptNavigatBtnIni.x,ptNavigatBtnIni.y,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btButton2,NULL,ptNavigatBtnIni.x+uBtStype*1,ptNavigatBtnIni.y,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btButton3,NULL,ptNavigatBtnIni.x+uBtStype*2,ptNavigatBtnIni.y,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btButton4,NULL,ptNavigatBtnIni.x+uBtStype*3,ptNavigatBtnIni.y,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btButton5,NULL,ptNavigatBtnIni.x+uBtStype*4,ptNavigatBtnIni.y,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btButton6,NULL,ptNavigatBtnIni.x+uBtStype*5,ptNavigatBtnIni.y,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btButton7,NULL,ptNavigatBtnIni.x+uBtStype*6,ptNavigatBtnIni.y,0,0,uFlags|SWP_NOSIZE);


	//DeferWindowPos(hDwp,m_btGlobalOption,NULL,nWidth-393,nYPos+10,0,0,uFlags|SWP_NOSIZE);

	//��������
	LockWindowUpdate();
	EndDeferWindowPos(hDwp);
	UnlockWindowUpdate();

	//����Ƥ��
	int i;
	for (i=0;i<CountArray(m_btViewItem);i++)
	{
		if(m_pActiveViewItem==m_pRoomViewItem[i])break;
	}
	CSkinButton * pTempButton[]={&m_btViewItem[0],&m_btViewItem[1],&m_btViewItem[2],&m_btGamePlaza};
	ASSERT((CountArray(m_btViewItem)+1)==CountArray(pTempButton));
	for (WORD j=0;j<CountArray(pTempButton);j++)
	{
		if(i==j)
		{
			pTempButton[j]->SetTextColor(RGB(245,245,245), RGB(0,0,0), true);
			pTempButton[j]->SetButtonImage(GetPlatformRes().uBtFrameItemActive,GetResInstanceHandle(),false);
		}
		else
		{
			pTempButton[j]->SetTextColor(RGB(0,0,0));
			pTempButton[j]->SetButtonImage(GetPlatformRes().uBtFrameItemNormal,GetResInstanceHandle(),false);
		}
	}

	return;
}
//��󴰿�
bool CGameFrame::MaxSizeWindow()
{
	//״̬�ж�
	if (m_bMaxShow==false)
	{
		//Ĭ��λ��
		GetWindowRect(&m_rcNormalSize);

		//���ð�ť
		//m_btMax.SetButtonImage(GetPlatformRes().uBtFrameResore,GetResInstanceHandle(),false);
		m_btMax.SetButtonImage(szResPath("Frame\\FRAME_BT_RESORE.bmp"),false);

		//m_btMax.LoadStdImage(PNG_FRAME_BT_MAX);
		//��ȡλ��
		CRect rcArce;
		SystemParametersInfo(SPI_GETWORKAREA,0,&rcArce,SPIF_SENDCHANGE);

		//�ƶ�����
		m_bMaxShow=true;
		LockWindowUpdate();
		SetWindowPos(NULL,rcArce.left-2,rcArce.top-2,rcArce.Width()+4,rcArce.Height()+4,SWP_NOZORDER);
		UnlockWindowUpdate();
	}

	return true;
}

//��ԭ����
bool CGameFrame::RestoreWindow()
{
	//״̬�ж�
	if (m_bMaxShow==true)
	{
		//���ð�ť
		//m_btMax.SetButtonImage(GetPlatformRes().uBtFrameMax,GetResInstanceHandle(),false);
		m_btMax.SetButtonImage(szResPath("Frame\\FRAME_BT_MAX.bmp"),false);

		//m_btMax.LoadStdImage(PNG_FRAME_BT_MAX);

		//�ƶ�����
		m_bMaxShow=false;
		LockWindowUpdate();
		SetWindowPos(NULL,m_rcNormalSize.left,m_rcNormalSize.top,m_rcNormalSize.Width(),m_rcNormalSize.Height(),SWP_NOZORDER);
		UnlockWindowUpdate();
	}

	return true;
}
//������
bool CGameFrame::StartUpdateApp(bool bStart)
{

	////qweasdasd

	//qweasdasd
	///asdasdasdasdasd
	//�������³���
	TCHAR szModuleFile[MAX_PATH];
	TCHAR szUpdateFile[MAX_PATH];

	GetModuleFileName(AfxGetInstanceHandle(), szModuleFile, MAX_PATH);
	PathRemoveFileSpec(szModuleFile);
	strcpy(szUpdateFile, szModuleFile);
	strcat(szModuleFile, _T("\\AutoUpdate.EX_"));
	strcat(szUpdateFile, _T("\\AutoUpdate.exe"));

	//������ļ����ھ��滻���ɵ��ļ�
	if (PathFileExists(szModuleFile))
	{

		MoveFileEx(szModuleFile, szUpdateFile, MOVEFILE_REPLACE_EXISTING|MOVEFILE_COPY_ALLOWED);

	}

	if( bStart )
	{
		if (WinExec(szUpdateFile, SW_SHOWDEFAULT) > 31)
		{ 
			return true;
		}
		else
		{
			return false;
		}
	}

	return true;
}


//������Ϣ
int CGameFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//��������
	ModifyStyle(WS_CAPTION|WS_BORDER,0,0);
	ModifyStyleEx(WS_EX_CLIENTEDGE|WS_EX_WINDOWEDGE,0,0);

	//����ͼ��
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);
	//�Զ����´���
	////////////////////////

	//��������
	bool bCheckUpdate = true;
	LPWSTR *szArglist = NULL;
	int nArgs = 0;

	szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);
	for (int i = 1; i < nArgs; i++)
	{
		if (_wcsicmp(szArglist[i], L"-noupdate") == 0)
			bCheckUpdate = false;
	}

	StartUpdateApp(bCheckUpdate);
	///////////////////////
	//���ñ���
	CString strTitle;
	strTitle.LoadString(AfxGetInstanceHandle(),IDS_MAIN_TITLE);
	SetWindowText(strTitle);

	//���ؼ�
	/*m_BrowerAD.Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CRect(0,0,0,0),this,IDC_BROWER_AD,NULL);
	//m_BrowerAD.Navigate(TEXT("http://www.7x78.com/AD/GamePlazaAD.asp"),NULL,NULL,NULL,NULL);*/

	m_BrowerAD.Create(NULL,NULL,WS_VISIBLE,CRect(0,0,0,0),this,NULL,NULL);
	m_BrowerAD.Navigate(TEXT("http://www.7x78.com/AD/PlazaAD1.html"),NULL,NULL,NULL,NULL);

	m_BrowerToolBarAd.Create(NULL,NULL,WS_VISIBLE,CRect(0,0,0,0),this,NULL,NULL);
	m_BrowerToolBarAd.Navigate(TEXT("http://www.7x78.com/AD/ToolBarAd.html"),NULL,NULL,NULL,NULL);


	//������ؼ�
	m_Splitter.Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CRect(0,0,0,0),this,IDC_SPLITTER,NULL);
	m_Splitter.SetSplitterSink(this);

	//�û���ͼ
	m_UserInfoView.Create(IDD_USER_INFO,this);
	//m_ServerItmOpaint.Create(IDD_SERVER_INFO,this);
	m_UserInfoView.ShowWindow(SW_SHOW);
	//m_ServerItmOpaint.ShowWindow(SW_SHOW);

	//��������ͼ
	m_ServerItemView.Create(WS_VISIBLE | WS_CHILD  ,CRect(0,0,0,0),this,IDC_SERVER_ITEM_VIEW);
	m_ServerItemView.InitServerItemView(this);
	m_ServerItemView.SetSkinBkColor(RGB(46,198,248));
	m_ServerItemView.SetBkImage(IDB_TREE_BK);
	m_ServerItemView.ChangeTreeType(TREE_TYPE_DEF);	
	SkinSB_Init(m_ServerItemView,CGameFrame::m_bmScroll);
	g_GlobalUnits.m_ServerListManager.InitServerListManager(&m_ServerItemView);


	//�����б�
	m_wShowListType = IDC_BT_LIST_CONTROL_1;
	m_UserCompanionList.Create(WS_CHILD,CRect(0,0,0,0),this,IDC_COMPANION_LIST);

	//������ť
	m_btMin.Create(NULL,WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,IDC_BT_MIN);
	m_btMax.Create(NULL,WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,IDC_BT_MAX);
	m_btClose.Create(NULL,WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,IDC_BT_CLOSE);
	m_btMenu.Create(NULL,WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,IDC_BT_MENU);
	m_btExChangeSkin.Create(NULL,WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,IDC_BT_EXCHANGE_SINK);
	m_btExChangeSkin.ShowWindow(SW_HIDE);
	m_btButton1.Create(TEXT(""),WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,IDC_BT_BUTTON_1);
	m_btButton2.Create(TEXT(""),WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,IDC_BT_BUTTON_2);
	m_btButton5.Create(TEXT(""),WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,IDC_BT_BUTTON_5);
	m_btButton3.Create(TEXT(""),WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,IDC_BT_BUTTON_3);
	m_btButton4.Create(TEXT(""),WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,IDC_BT_BUTTON_4);
	m_btButton6.Create(TEXT(""),WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,IDC_BT_BUTTON_6);
	m_btButton7.Create(TEXT(""),WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,IDC_BT_BUTTON_7);

	m_btSafe.Create(TEXT(""),WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,IDC_BT_SAFE);
	m_btFanKui.Create(TEXT(""),WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,IDC_BT_FANKUI);
	m_btFont.Create(TEXT(""),WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,IDC_BT_FONT);
	m_btBbs.Create(TEXT(""),WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,IDC_BT_BBS);
	m_btMember.Create(TEXT(""),WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,IDC_BT_VIP);

	m_btGamePlaza.Create(TEXT("��Ϸ����"),WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,IDC_BT_PLAZA);
	//m_btSelfOption.Create(TEXT(""),WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,IDC_BT_SELF_OPTION);
	//m_btSwitchUser.Create(TEXT(""),WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,IDC_BT_SWITCH_ACCOUNTS);
	m_btGlobalOption.Create(TEXT(""),WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,IDC_BT_SYSTEM_OPTION);
	//m_btListControl1.Create(NULL,WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,IDC_BT_LIST_CONTROL_1);
	//m_btListControl2.Create(NULL,WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,IDC_BT_LIST_CONTROL_2);


	//�ͻ��ؼ�
	// 	m_btLock.Create(TEXT(""),WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,IDC_LOCK);
	// 	m_btLockroom.Create(TEXT(""),WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,IDC_LOCKROOM);
	// 	
	// 	m_btUnlock.Create(TEXT(""),WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,IDC_UNLOCK);
	// 	m_btVideo.Create(TEXT(""),WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,IDC_VIDEO);
	// 	m_btMobile.Create(TEXT(""),WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,IDC_MOBILE);
	// 	m_btHongzuan.Create(TEXT(""),WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,IDC_HONGZUAN);
	// 	m_btUnzuan.Create(TEXT(""),WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,IDC_UNZUAN);
	// 	m_btSet.Create(TEXT(""),WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,IDC_SET);
	// 	m_btJianghu.Create(TEXT(""),WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,IDC_JIANGHU);

	for (WORD i=0;i<CountArray(m_btViewItem);i++)
	{
		CRect rcCreate(0,0,0,0);
		m_btViewItem[i].Create(NULL,WS_CHILD,rcCreate,this,IDC_BT_VIEW_ITEM+i);
	}
	//Ƥ����Դ
	UpdateSkinResource();
	//������ʾ�ؼ�
	m_ToolTipCtrl.Create(this);
	m_ToolTipCtrl.Activate(TRUE);
	m_ToolTipCtrl.AddTool(GetDlgItem(IDC_BT_MIN),TEXT("��С����Ϸ�㳡"));
	m_ToolTipCtrl.AddTool(GetDlgItem(IDC_BT_CLOSE),TEXT("�˳���Ϸ�㳡"));
	//�����ؼ�
	m_DlgGamePlaza.Create(IDD_GAME_PLAZA,this);
	ActivePlazaViewItem();
	//ע���ȼ�
	g_GlobalUnits.RegisterHotKey(m_hWnd,IDI_HOT_KEY_BOSS,g_GlobalOption.m_wBossHotKey);
	//����λ��
	CRect rcArce;
	SystemParametersInfo(SPI_GETWORKAREA,0,&rcArce,SPIF_SENDCHANGE);
	//��ʾ����
	//MaxSizeWindow();
	RestoreWindow();
	CenterWindow();
	ShowWindow(SW_SHOW);

	//Ĭ��λ��
	m_rcNormalSize.left=(rcArce.Width()-LESS_SCREEN_X)/2;
	m_rcNormalSize.top=(rcArce.Height()-LESS_SCREEN_Y)/2;
	m_rcNormalSize.right=(rcArce.Width()+LESS_SCREEN_X)/2;
	m_rcNormalSize.bottom=(rcArce.Height()+LESS_SCREEN_Y)/2;

	SetTimer(IDI_TIME_UPDATETIME,60*1000,0);

	return 0;
}

//�滭��Ϣ
void CGameFrame::OnPaint()
{
	CPaintDC dc(this);

	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//��������
	CBitmap ImageBuffer;
	ImageBuffer.CreateCompatibleBitmap(&dc,rcClient.Width(),rcClient.Height());

	//���� DC
	CDC BufferDC;
	BufferDC.CreateCompatibleDC(&dc);
	BufferDC.SelectObject(&ImageBuffer);

	CFont GlodFont;
	GlodFont.CreateFont(-12,0,0,0,400,0,0,0,134,3,2,1,2,TEXT("����"));

	CFont * pOldFont=BufferDC.SelectObject(&GlodFont);
	BufferDC.SetBkMode(TRANSPARENT);
	BufferDC.SetTextColor(RGB(0,0,0));
	//������Դ
	CImageHandle HandleTitleL(&m_ImageServerT);
	CImageHandle HandleTitleM(&m_ImageServerM);
	CImageHandle HandleTitleR(&m_ImageServerB);

	CImageHandle HandleFrameMidT(&m_ImageFrameMidT);
	CImageHandle HandleFrameMidB(&m_ImageFrameMidB);

	//��Ϸ�б�
	CRect /*rcServer,*/ rcServerIni;
// 	rcServer.left=m_ImageInfoFrame.nLBorder+4;
// 	rcServer.right=rcServer.left+m_ImageServerT.GetWidth();
// 	rcServer.bottom=rcClient.Height()-m_ImageInfoFrame.nBBorder+8;
// 	rcServer.top=m_ImageInfoFrame.nTBorder;
	GetRectFromIni(szResPath("GameFrame.ini"), TEXT("Frame"), TEXT("ServerList"), rcServerIni, rcServerIni);
	rcServerIni.bottom = rcClient.Height()-rcServerIni.bottom;
	m_rcServerBroder=rcServerIni;

	//�滭����
	//LEFT
	int iLeft = m_ImageInfoFrame.nLBorder;
	int iYPos = m_ImageInfoFrame.nTBorder;
	int ix = m_ImageFrameMidT.GetWidth();
	for(int iL=iLeft; iL <= rcClient.Width(); iL =iL +m_ImageFrameMidT.GetWidth())
	{
		if(iL>rcServerIni.left && iL < rcServerIni.right ) iL++;
		
		if(!m_bHideGameList)
		{
			if(iL>m_rcBrowser.left && iL < (rcClient.Width()-m_rcBrowser.right)) iL++;
		}
		else
		{
			if(iL>m_rcRoomView.left && iL < (m_rcRoomView.right)) iL++;
		}

		m_ImageFrameMidT.BitBlt(BufferDC, iL , iYPos);
		for (int iB = iYPos+m_ImageFrameMidT.GetHeight();iB < (rcClient.Height() - m_ImageInfoFrame.nBBorder); iB+=m_ImageFrameMidB.GetHeight())
		{
			m_ImageFrameMidB.BitBlt(BufferDC, iL , iB);
		}
	}

	//��Ϸ�б�
	if (!m_bHideGameList)
	{
		//�滭�б�
		m_ImageServerT.BitBlt(BufferDC,rcServerIni.left,rcServerIni.top);
		for (INT nXPos=rcServerIni.top+m_ImageServerT.GetHeight();nXPos<rcServerIni.bottom-m_ImageServerB.GetHeight();nXPos+=m_ImageServerM.GetHeight())
		{
			m_ImageServerM.BitBlt(BufferDC,rcServerIni.left,nXPos);
		}
 		//�滭����
 		m_ImageServerB.BitBlt(BufferDC,rcServerIni.left,rcServerIni.bottom-m_ImageServerB.GetHeight()-8);
		// TCHAR szBuffer[128];
		//sprintf(szBuffer,TEXT("ͬʱ����������%d"),OnLineCount);
		//BufferDC.TextOut(40,rcServer.top+5,szBuffer,lstrlen(szBuffer));
	}

	//�滭���
	CSkinAide::DrawEncircleFrame(&BufferDC,rcClient,m_EncircleFrame);

	//�����ʾ ADD 0513ygs
	int nWidth = 80;
	CRect rcLastBarBtn;
	m_btButton7.GetWindowRect(&rcLastBarBtn);
	ScreenToClient(&rcLastBarBtn);
	if((rcClient.right - rcLastBarBtn.right)>(m_PngImageTopAd.GetWidth()+10))
		m_PngImageTopAd.DrawImage(&BufferDC, rcLastBarBtn.right+10, 0);

	//��¼ʱ��
	CRect rcLogonTimeIni;
	GetRectFromIni(szResPath("GameFrame.ini"), TEXT("Frame"), TEXT("LogonTime"), rcLogonTimeIni, rcLogonTimeIni);
	rcLogonTimeIni.left = rcClient.Width()-rcLogonTimeIni.left;
	rcLogonTimeIni.right = rcClient.Width()-rcLogonTimeIni.right;
	rcLogonTimeIni.bottom = rcClient.Height() - rcLogonTimeIni.bottom;
	rcLogonTimeIni.top = rcClient.Height() -rcLogonTimeIni.top;
	m_rcLogonTime = rcLogonTimeIni;
	//BufferDC.Draw3dRect(rcLogonTimeIni,0,0);
	CString strLogonTime;
	if(m_tmLogonTime != NULL)
	{
		CTimeSpan tSpan = CTime::GetCurrentTime() - m_tmLogonTime;
		strLogonTime.Format("�����ۼ�����ʱ��:%ld����",tSpan.GetSeconds()/*.GetMinutes()*/);
	}
	else
		strLogonTime.Format("�����ۼ�����ʱ��:0����");

	BufferDC.DrawText(strLogonTime, rcLogonTimeIni, DT_VCENTER|DT_WORD_ELLIPSIS|DT_SINGLELINE);

	BufferDC.SetTextColor(RGB(0,0,0));
// 	SYSTEMTIME SystemTime;
// 	TCHAR szTimeBuffer[64];
// 	GetLocalTime(&SystemTime);
// 	_snprintf(szTimeBuffer,sizeof(szTimeBuffer),TEXT("��½ʱ�䣺%02d��%02d��%02d�� %02d:%02d:%02d"),SystemTime.wYear,SystemTime.wMonth,SystemTime.wDay,SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond);
//     BufferDC.TextOut(20,100,szTimeBuffer,lstrlen(szTimeBuffer));

	tagGlobalUserData & GlobalUserInfo=g_GlobalUnits.GetGolbalUserData();
	if( GlobalUserInfo.dwUserID != 0 && !m_bHideGameList)
	{
		//��ȡͷ��λ��
		CRect rcFace;
		GetRectFromIni(szResPath("GameFrame.ini"), TEXT("Frame"), TEXT("rcFace"), m_rcFace, rcFace);
		CBrush BackBrush(RGB(255,255,255));
		CPen LinePen(PS_SOLID,1,RGB(82,201,245));
		BufferDC.SelectObject(&LinePen);
		BufferDC.SelectObject(&BackBrush);
		CBrush BackBrush2(RGB(82,201,245));


		//�滭�Ͽ�
		BufferDC.SelectObject(&BackBrush2);
		CRect rcTemp = m_rcFace;
		//if(m_bMouseOnFace) rcTemp.DeflateRect(-2,-2,-2,-2);
			//else	rcTemp.DeflateRect(-3,-3,-3,-3);
		
 //		BufferDC.RoundRect(rcTemp,CPoint(10,10));
 		BufferDC.SelectObject(&BackBrush);

		//�û�ͷ��
		g_GlobalUnits.m_UserFaceRes->DrawNormalFace(&BufferDC,m_rcFace.left,m_rcFace.top,GlobalUserInfo.wFaceID,GlobalUserInfo.dwUserID,GlobalUserInfo.dwCustomFaceVer,48,48);

		//����λ��
		TCHAR szBuffer[255]=TEXT("");

		CRect rcUnderWriteIni;
		GetRectFromIni(szResPath("GameFrame.ini"), TEXT("Frame"), TEXT("rcUnderWrite"), rcUnderWriteIni, rcUnderWriteIni);
		m_rcUserUnderWrite = rcUnderWriteIni;

		CRect rcAccountsIni;
		GetRectFromIni(szResPath("GameFrame.ini"), TEXT("Frame"), TEXT("rcAccounts"), rcAccountsIni, rcAccountsIni);

		if(m_bMouseOnUnderWrite)
		{
			BufferDC.SelectObject(&BackBrush2);
			rcTemp.DeflateRect(-2,-2,-2,-2);
			BufferDC.RoundRect(rcUnderWriteIni,CPoint(5.5));
		}

//  		BufferDC.Draw3dRect(rcAccountsIni, RGB(255,0,255),0);
//  		BufferDC.Draw3dRect(rcUnderWriteIni,0,0);

		//�û��ʺ�
		//CFont GlodFont;
		//GlodFont.CreateFont(-14,0,0,0,500,0,0,0,134,3,2,1,2,TEXT("����"));
		//CFont * pOldFont=BufferDC.SelectObject(&GlodFont);
		//BufferDC.SetTextColor(RGB(0,87,233));
		//�û��ǳ�
		_sntprintf(szBuffer,CountArray(szBuffer),TEXT("%s"),GlobalUserInfo.szAccounts);
		CSize szStr;
		int iLen=lstrlen(szBuffer);
		GetTextExtentPoint(BufferDC, szBuffer, iLen, &szStr);
		rcAccountsIni.right= rcAccountsIni.left+szStr.cx;
		m_rcUserAccount= rcAccountsIni;

		// 		BufferDC.FillRect(rcAccounts, &BackBrush2);
		BufferDC.DrawText(szBuffer,lstrlen(szBuffer),&m_rcUserAccount,DT_LEFT|DT_WORD_ELLIPSIS|DT_SINGLELINE);
		if(m_bMouseOnAccount)
		{
			CPen LinePen2(PS_SOLID,1,RGB(0,0,0));
			BufferDC.SelectObject(&LinePen2);
			BufferDC.MoveTo(m_rcUserAccount.left, m_rcUserAccount.bottom-8);
			BufferDC.LineTo(m_rcUserAccount.right, m_rcUserAccount.bottom-8);
		}

		if (m_bMouseOnUnderWrite)
		{
			_sntprintf(szBuffer,CountArray(szBuffer),TEXT("%d        �������"),GlobalUserInfo.dwGameID);
		}else
			_sntprintf(szBuffer,CountArray(szBuffer),TEXT("%d"),GlobalUserInfo.dwGameID);

		BufferDC.DrawText(szBuffer,lstrlen(szBuffer),&m_rcUserUnderWrite,DT_VCENTER|DT_WORD_ELLIPSIS|DT_SINGLELINE);
	}

//////////////////////////////////////////////////////////////////////////
	//PNG anniu
	m_btFanKui.SetBkGnd(&BufferDC);
	m_btMember.SetBkGnd(&BufferDC);
	m_btSafe.SetBkGnd(&BufferDC);
	m_btBbs.SetBkGnd(&BufferDC);
	m_btFont.SetBkGnd(&BufferDC);

	dc.BitBlt(0,0,rcClient.Width(),rcClient.Height(),&BufferDC,0,0,SRCCOPY);

	//�滭����
	BufferDC.DeleteDC();
	ImageBuffer.DeleteObject();
	return;
}

//�滭����
BOOL CGameFrame::OnEraseBkgnd(CDC * pDC)
{
	Invalidate(FALSE);
	UpdateWindow();

	return TRUE;
}

//���øı�
void CGameFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	__super::OnSettingChange(uFlags, lpszSection);

	//������ܴ�С
	CRect rcClient;
	SystemParametersInfo(SPI_GETWORKAREA,0,&rcClient,SPIF_SENDCHANGE);
	MoveWindow(&rcClient,TRUE);

	return;
}

//�����Ϣ
void CGameFrame::OnNcMouseMove(UINT nHitTest, CPoint point)
{
	//ȥ���˵�
	LONG lStyle = GetWindowLong(m_hWnd,GWL_STYLE);
	if((lStyle&WS_SYSMENU)!=0)ModifyStyle(WS_SYSMENU,0,0);

	__super::OnNcMouseMove(nHitTest, point);
}

//�����¼�
void CGameFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	//���Ӳ˵�
	LONG lStyle = GetWindowLong(m_hWnd,GWL_STYLE);
	if((lStyle&WS_SYSMENU)==0)ModifyStyle(0,WS_SYSMENU,0);

	__super::OnActivate(nState, pWndOther, bMinimized);
}

//�ر���Ϣ
void CGameFrame::OnClose()
{

	//���ؽ���
	ShowWindow(SW_HIDE);

	//�رշ���
	CloseAllRoomViewItem();

	//��������
	g_GlobalOption.SaveOptionParameter();
	g_GlobalUnits.m_CompanionManager->SaveCompanion();

	__super::OnClose();
}

//��ȡ���λ��
void CGameFrame::OnGetMinMaxInfo(MINMAXINFO FAR * lpMMI) 
{
	//���ñ���
	lpMMI->ptMinTrackSize.x=LESS_SCREEN_X;
	lpMMI->ptMinTrackSize.y=LESS_SCREEN_Y;

	__super::OnGetMinMaxInfo(lpMMI);
}

//λ����Ϣ
void CGameFrame::OnSize(UINT nType, int cx, int cy) 
{
	__super::OnSize(nType, cx, cy);

	//���������
	RectifyControl(cx,cy);
	CRgn m_rgn;
	CRect rc;
	GetWindowRect(&rc); //��ô��ھ���
	rc -= rc.TopLeft();
	m_rgn.CreateRoundRectRgn(rc.left, rc.top, rc.right+1, rc.bottom+1, 5, 5); //���ݴ��ھ��δ���һ��Բ�Ǿ���
	SetWindowRgn(m_rgn,true);
	return;
}

//��������
LRESULT CGameFrame::OnNcHitTest(CPoint Point)
{
	//��ť����
	if (m_bMaxShow==false)
	{
		//��ȡλ��
		CRect rcClient;
		GetClientRect(&rcClient);

		//ת��λ��
		CPoint ClientPoint=Point;
		ScreenToClient(&ClientPoint);

		//����λ��
		if (ClientPoint.x<=m_ImageInfoFrame.nRBorder)
		{
			if (ClientPoint.y<=m_ImageInfoFrame.nTBorder-FRAME_EXALTATION) return HTTOPLEFT;
			if (ClientPoint.y>=(rcClient.Height()-m_ImageInfoFrame.nRBorder)) return HTBOTTOMLEFT;
			return HTLEFT;
		}

		//����λ��
		if (ClientPoint.x>=(rcClient.Width()-m_ImageInfoFrame.nRBorder))
		{
			if (ClientPoint.y<=m_ImageInfoFrame.nTBorder-FRAME_EXALTATION) return HTTOPRIGHT;
			if (ClientPoint.y>=(rcClient.Height()-m_ImageInfoFrame.nRBorder)) return HTBOTTOMRIGHT;
			return HTRIGHT;
		}

		//����λ��
		if (ClientPoint.y<=m_ImageInfoFrame.nRBorder) return HTTOP;
		if (ClientPoint.y>=(rcClient.Height()-m_ImageInfoFrame.nRBorder)) return HTBOTTOM;
	}

	return __super::OnNcHitTest(Point);
}

//�����Ϣ
void CGameFrame::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags,Point);

	//ģ�ⰴ����
	if ((m_bMaxShow==false)&&(Point.y<=m_ImageInfoFrame.nTBorder-FRAME_EXALTATION))
	{
		PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(Point.x,Point.y));
	}

	//���ͷ��
	if(m_bMouseOnFace==true)
	{
		//CGameFrame *pGameFrame=(CGameFrame *)AfxGetMainWnd();
		//pGameFrame->WebBrowse("http://www.7x78.com/MemberModifyFace.asp",true);
		::SendMessage(AfxGetMainWnd()->m_hWnd,WM_COMMAND,IDC_SHOW_TIP,0);
	}

	//����û���
	if(m_bMouseOnAccount)
	{
		//CGameFrame *pGameFrame=(CGameFrame *)AfxGetMainWnd();
		//pGameFrame->WebBrowse("http://www.7x78.com/MemberIndex.asp",true);
		::SendMessage(AfxGetMainWnd()->m_hWnd,WM_COMMAND,IDC_SHOW_TIP,0);
	}

	//����ƹ�����
	if(m_bMouseOnUnderWrite==true)
	{
		//CGameFrame *pGameFrame=(CGameFrame *)AfxGetMainWnd();
		//pGameFrame->WebBrowse("http://www.7x78.com/MemberModifyUserInfo.asp",true);
		TCHAR szTuiGuangUrl[256]={0};
		tagGlobalUserData & GlobalUserInfo=g_GlobalUnits.GetGolbalUserData();
		_sntprintf(szTuiGuangUrl, CountArray(szTuiGuangUrl), TEXT("http://www.7x78.com/Register.asp?Spreader=%d"),GlobalUserInfo.dwGameID);

		//�򿪼��а�
		if (OpenClipboard()==FALSE) return ;
		if (EmptyClipboard()==FALSE) 
		{
			CloseClipboard();
			return ;
		}

		//��������
		HANDLE hData=GlobalAlloc(GMEM_MOVEABLE|GMEM_ZEROINIT,256);
		if (hData==NULL) 
		{
			CloseClipboard();
			return ;
		}
		LPTSTR szMemName=(LPTSTR)GlobalLock(hData);
		lstrcpy(szMemName,szTuiGuangUrl);
		SetClipboardData(CF_TEXT,hData);
		GlobalUnlock(hData);
		CloseClipboard();

		ShowMessageBox(TEXT("�����ƹ������Ѹ��Ƶ�ճ���壡"), MB_ICONQUESTION);

	}

	return;
}

//�����Ϣ
void CGameFrame::OnLButtonDblClk(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDblClk(nFlags,Point);

	//״̬�ж�
	if (Point.y>m_ImageInfoFrame.nTBorder-FRAME_EXALTATION) return;

	//���ƴ���
	if (m_bMaxShow==true) RestoreWindow();
	else MaxSizeWindow();

	return;
}

//��װ���
LRESULT CGameFrame::OnMessageSetupFinish(WPARAM wParam, LPARAM lParam)
{
	g_GlobalUnits.m_ServerListManager.UpdateGameKind((WORD)wParam);
	return 0;
}

//�ȼ���Ϣ
LRESULT CGameFrame::OnHotKeyMessage(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case IDI_HOT_KEY_BOSS:		//�ϰ��ȼ�
		{
			//��������
			bool bBossCome=(IsWindowVisible()==FALSE)?false:true;

			//���ش���
			if (bBossCome==false)
			{
				//��ԭ����
				ShowWindow(SW_RESTORE);
				ShowWindow(SW_SHOW);

				//�ö�����
				SetActiveWindow();
				BringWindowToTop();
				SetForegroundWindow();
			}
			else
			{
				//���ش���
				ShowWindow(SW_MINIMIZE);
				ShowWindow(SW_HIDE);
			}

			//������Ϣ
			for (INT_PTR i=0;i<MAX_SERVER;i++)
			{
				if (m_pRoomViewItem[i]!=NULL)
				{
					m_pRoomViewItem[i]->NotifyBossCome(bBossCome);
				}
			}

			return 0;
		}
	}

	return 0;
}

//�رշ���
LRESULT CGameFrame::OnCloseRoomViewItem(WPARAM wParam, LPARAM lParam)
{
	//�ͷŷ���
	IRoomViewItem * pIRoomViewItem=(IRoomViewItem *)wParam;
	if (pIRoomViewItem!=NULL) pIRoomViewItem->Release();

	return 0;
}

LRESULT CGameFrame::ShowBindPc(WPARAM wParam, LPARAM lParam)
{

	//��ȡ���
	tagGlobalUserData &GlobalUserData=g_GlobalUnits.GetGolbalUserData();

	if(GlobalUserData.dwUserID==0)
	{
		ShowMessageBox(_T("�Բ�������û�е�¼���������ܴ򿪴����"),MB_ICONINFORMATION);
		return 0;
	}

	//���ж�
	if (!GlobalUserData.JiQi)
	{
		m_DlgLockMachine.m_bLock=true;
		m_DlgLockMachine.m_strDescribe=TEXT("ע�⣺�󶨻����󣬴��ʺ�ֻ���ڴ˼�����Ͻ�����Ϸ���󶨻�������ڴ˻������н���󶨲�����");

		//��ѯ��
		if (m_DlgLockMachine.DoModal()==IDOK)
		{
			m_DlgGamePlaza.m_ClientSocket->CloseSocket();

			//�����ж�
			if (m_DlgGamePlaza.m_ClientSocket->GetSocketStatus()==SOCKET_STATUS_CONNECT)
			{
				//�������к�
				CMD_GP_SafeBind SafeBind;
				ZeroMemory(&SafeBind, sizeof(SafeBind));
				SafeBind.dwUserID = GlobalUserData.dwUserID;
				g_GlobalUnits.GetClientSerial(SafeBind.ClientSerial);
				TCHAR szPassword[33];
				CMD5Encrypt::EncryptData(m_DlgLockMachine.m_strPassword,szPassword);
				lstrcpyn(SafeBind.szPassWord,szPassword,CountArray(SafeBind.szPassWord));

				//��������
				m_DlgGamePlaza.m_ClientSocket->SendData(MDM_GP_USER,SUB_GP_SAFE_BIND,&SafeBind,sizeof(SafeBind));
			}
			else
			{
				//���ñ�ʶ
				m_DlgGamePlaza.m_enBindAction=enBindAction_Bind;

				//���ӷ�����
				m_DlgGamePlaza.SendConnectMessage();
			}
		}
	}
	else
	{
		m_DlgLockMachine.m_bLock=false;
		m_DlgLockMachine.m_strDescribe=TEXT("���˺��Ѿ�ʹ���˰󶨻������ܣ�������ȷ����������󣬵��ȷ����ť����󶨣�");

		//��ѯ��
		if (m_DlgLockMachine.DoModal()==IDOK)
		{
			m_DlgGamePlaza.m_ClientSocket->CloseSocket();
			//�����ж�
			if (m_DlgGamePlaza.m_ClientSocket->GetSocketStatus()==SOCKET_STATUS_CONNECT)
			{
				CMD_GP_SafUnbind SafUnbind;
				ZeroMemory(&SafUnbind, sizeof(SafUnbind));

				SafUnbind.dwUserID=GlobalUserData.dwUserID;
				TCHAR szPassword[33];
				CMD5Encrypt::EncryptData(m_DlgLockMachine.m_strPassword,szPassword);
				lstrcpyn(SafUnbind.szPassWord,szPassword,CountArray(SafUnbind.szPassWord));

				//��������
				m_DlgGamePlaza.m_ClientSocket->SendData(MDM_GP_USER,SUB_GP_SAFE_UNBIND,&SafUnbind,sizeof(SafUnbind));
			}
			else
			{
				//���ñ�ʶ
				m_DlgGamePlaza.m_enBindAction=enBindAction_UnBind;

				//���ӷ�����
				m_DlgGamePlaza.SendConnectMessage();
			}
		}
	}

	return 0;
}

BOOL CGameFrame::OnSetCursor( CWnd * pWnd, UINT nHitTest, UINT uMessage )
{
	//��ȡ���
	CPoint MousePoint;
	GetCursorPos(&MousePoint);
	ScreenToClient(&MousePoint);

	//��������
	bool bMouseEvent=false;
	bool bRedrawWindow=false;

	//�����ж�
	if ((m_bMouseOnFace==false)&&(m_rcFace.PtInRect(MousePoint)==TRUE))
	{
		//���ñ���
		bMouseEvent=true;
		bRedrawWindow=true;

		//���ñ���
		m_bMouseOnFace=true;
	}
	//�뿪�ж�
	if ((m_bMouseOnFace==true)&&(m_rcFace.PtInRect(MousePoint)==FALSE))
	{
		//���ñ���
		bRedrawWindow=true;
		//���ñ���
		m_bMouseOnFace=false;
	}

	//�����ж�
	if ((m_bMouseOnUnderWrite==false)&&(m_rcUserUnderWrite.PtInRect(MousePoint)==TRUE))
	{
		//���ñ���
		bMouseEvent=true;
		bRedrawWindow=true;

		//���ñ���
		m_bMouseOnUnderWrite=true;
	}
	//�뿪�ж�
	if ((m_bMouseOnUnderWrite==true)&&(m_rcUserUnderWrite.PtInRect(MousePoint)==FALSE))
	{
		//���ñ���
		bRedrawWindow=true;
		//���ñ���
		m_bMouseOnUnderWrite=false;
	}

	//�����ж�
	if ((m_bMouseOnAccount==false)&&(m_rcUserAccount.PtInRect(MousePoint)==TRUE))
	{
		//���ñ���
		bMouseEvent=true;
		bRedrawWindow=true;

		//���ñ���
		m_bMouseOnAccount=true;
	}
	//�뿪�ж�
	if ((m_bMouseOnAccount==true)&&(m_rcUserAccount.PtInRect(MousePoint)==FALSE))
	{
		//���ñ���
		bRedrawWindow=true;
		//���ñ���
		m_bMouseOnAccount=false;
	}

	//���½���
	if (bRedrawWindow==true)
	{
		//CRect(0, 0, 200, 200);
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	//���ù��
	if (!m_bHideGameList && (m_bMouseOnFace==true || m_bMouseOnUnderWrite == true || m_bMouseOnAccount) )
	{
		SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_HAND_CUR)));
		return true;
	}

	return __super::OnSetCursor(pWnd,nHitTest,uMessage);
}

bool CGameFrame::GetRectFromIni(LPCTSTR lpFileName, LPCTSTR lpAppName,LPCTSTR lpKeyName,CRect& rcControl, CRect rcDefault)
{
	if(lpFileName == NULL) return false;
	CString strKey;

	strKey.Format("%s_top", lpKeyName);
	rcControl.top = GetPrivateProfileInt(lpAppName, strKey, rcDefault.top, lpFileName);

	strKey.Format("%s_left", lpKeyName);
	rcControl.left = GetPrivateProfileInt(lpAppName, strKey, rcDefault.left, lpFileName);

	strKey.Format("%s_right", lpKeyName);
	rcControl.right = GetPrivateProfileInt(lpAppName, strKey, rcDefault.right, lpFileName);

	strKey.Format("%s_bottom", lpKeyName);
	rcControl.bottom = GetPrivateProfileInt(lpAppName, strKey, rcDefault.bottom, lpFileName);

	return true;
}

bool CGameFrame::GetPointFromIni( LPCTSTR lpFileName, LPCTSTR lpAppName,LPCTSTR lpKeyName,CPoint& ptControl, CPoint ptDefault )
{
	if(lpFileName == NULL) return false;
	CString strKey;

	strKey.Format("%s_x", lpKeyName);
	ptControl.x = GetPrivateProfileInt(lpAppName, strKey, ptDefault.x, lpFileName);

	strKey.Format("%s_y", lpKeyName);
	ptControl.y = GetPrivateProfileInt(lpAppName, strKey, ptDefault.y, lpFileName);

	return true;
}

void CGameFrame::OnTimer( UINT nIDEvent )
{
	if(nIDEvent == IDI_TIME_UPDATETIME)
	{
		InvalidateRect(m_rcLogonTime, FALSE);
		return ;
	}
	__super::OnTimer(nIDEvent);
}


//////////////////////////////////////////////////////////////////////////
