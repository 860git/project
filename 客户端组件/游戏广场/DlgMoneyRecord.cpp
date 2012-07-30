#include "stdafx.h"
#include"resource.h"
#include "DlgMoneyRecord.h"
#include "GameFrame.h"
#include "GlobalUnits.h"
#include "Zip.h"

#define PageNum   9
////////////////////////////////////////////////////////////////////////////////////
//�궨��
#define DEF_INSIDEBORDER_COLOR	RGB(176,20,1)						//Ĭ����ɫ
#define BGROUND_COLOR			RGB(218,222,223)

////////////////////////////////////////////////////////////////////////////////////


BEGIN_MESSAGE_MAP(CDlgMoneyRecord, CDialog)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_SHOWWINDOW()
	ON_WM_CLOSE()

	ON_BN_CLICKED(IDC_QUERY, OnBnClickedGet)
	ON_BN_CLICKED(IDC_FIRST, OnBnClickedFirst)
	ON_BN_CLICKED(IDC_LAST, OnBnClickedLast)
	ON_BN_CLICKED(IDC_NEXT, OnBnClickedNext)
	ON_BN_CLICKED(IDC_END, OnBnClickedEnd)
END_MESSAGE_MAP()

//�ؼ���
void CDlgMoneyRecord::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_RecordListCtrl);

	DDX_Control(pDX, IDC_FIRST, m_btFirstPage);
	DDX_Control(pDX, IDC_LAST, m_btLastPage);
	DDX_Control(pDX, IDC_NEXT, m_btNextPage);
	DDX_Control(pDX, IDC_END, m_btEndPage);
	DDX_Control(pDX, IDC_QUERY, m_btQuery);
	
	
}

////////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgMoneyRecord::CDlgMoneyRecord(CWnd* pParent): CDialog(IDD_RECORD, pParent)//, CSkinWndObject(this)
{
	//���ñ���

	m_pIClientSocket = NULL;
	m_enOperateStatus=enOperateMoneyRecordStatus_NULL;
	
	
}

//�鹹����
CDlgMoneyRecord::~CDlgMoneyRecord()
{
	//if (m_brBkground.m_hObject != NULL ) m_brBkground.DeleteObject();
	if(m_brBkground.GetSafeHandle()) m_brBkground.DeleteObject();
}



//��ʼ����
BOOL CDlgMoneyRecord::OnInitDialog()
{
	__super::OnInitDialog();

	//����ˢ��
	if(m_brBkground.GetSafeHandle()) m_brBkground.DeleteObject();
	m_brBkground.CreateSolidBrush(BGROUND_COLOR);

	//���ñ���
	SetWindowText(TEXT("���ֶ�����"));

	m_RecordListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES); 

	
	m_RecordListCtrl.InsertColumn(0,"���",LVCFMT_LEFT,40);
	m_RecordListCtrl.InsertColumn(1,"�Է��û���",LVCFMT_LEFT,80);
	m_RecordListCtrl.InsertColumn(2,"����",LVCFMT_LEFT,40);
	m_RecordListCtrl.InsertColumn(3,"��������",LVCFMT_LEFT,100);
	m_RecordListCtrl.InsertColumn(4,"����ʱ��",LVCFMT_LEFT,150);

	m_nCount=0;

	nPageNum = m_nCount/8;

	if(m_nCount%8==0)
		nPageNum=nPageNum+1;

	CString strPage;
	strPage.Format("�� %d ҳ",nPageNum);;

	GetDlgItem(IDC_PAGE)->SetWindowText(strPage);


	nCurrentPage = 1;
	m_btFirstPage.EnableWindow(0);
	m_btEndPage.EnableWindow(0);
	m_btLastPage.EnableWindow(0);
	m_btNextPage.EnableWindow(0);
	
	
	
	//���¿ؼ�
	UpdateControls();

	//��ʼ������
	//__super::InitSkinObject();

	return TRUE;  
}

//�滭����
void CDlgMoneyRecord::OnPaint()
{
	CPaintDC dc(this);

	
	//�滭����
//	__super::DrawSkinView(&dc);
}


//��ʾ��Ϣ
void CDlgMoneyRecord::ShowMessage(TCHAR *pMessage)
{
	ShowInformationEx(pMessage,0,MB_ICONQUESTION,TEXT("���ͻ��ֶ�"));

	return;
}

//��������
bool CDlgMoneyRecord::ConnectServer()
{
	if ( m_pIClientSocket != NULL && m_pIClientSocket->GetSocketStatus() == SOCKET_STATUS_CONNECT && m_enOperateStatus != enOperateMoneyRecordStatus_NULL )
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
bool CDlgMoneyRecord::SendData()
{
	
	//״̬�ж�
	ASSERT(m_enOperateStatus != enOperateMoneyRecordStatus_NULL);
	if ( m_enOperateStatus == enOperateMoneyRecordStatus_NULL ) return false;

	if ( m_pIClientSocket == NULL || m_pIClientSocket->GetSocketStatus() != SOCKET_STATUS_CONNECT ) return false;

	if ( m_enOperateStatus == enOperateRecordStatus_Save )
	{
		CMD_GP_BankRecord GiveRecord;
		ZeroMemory(&GiveRecord, sizeof(GiveRecord));

		tagGlobalUserData &GlobalUserData = g_GlobalUnits.GetGolbalUserData();
		GiveRecord.dwUserID = GlobalUserData.dwUserID;
		//Ͷ����Ϣ
		m_pIClientSocket->SendData(MDM_GP_USER, SUB_GP_GIVE_RECORD, &GiveRecord, sizeof(GiveRecord));        

		//���½���
		InvalidateRect(NULL);
	}
	
	//����״̬
	m_enOperateStatus = enOperateMoneyRecordStatus_NULL ;

	return true;
}

//���¿ؼ�
void CDlgMoneyRecord::UpdateControls()
{
	//��������
	tagGlobalUserData & GlobalUserInfo=g_GlobalUnits.GetGolbalUserData();
	
	UpdateData(false);

}

//��ɫ����
HBRUSH CDlgMoneyRecord::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
void CDlgMoneyRecord::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	//���¿ؼ�
	if ( bShow ) UpdateControls();
}

//������Ϣ
void CDlgMoneyRecord::OnClose()
{
	if ( m_pIClientSocket != NULL && m_pIClientSocket->GetSocketStatus() == SOCKET_STATUS_CONNECT && m_enOperateStatus != enOperateMoneyStatus_NULL )
	{
		m_pIClientSocket->CloseSocket();
	}
	CDialog::OnClose();
		DestroyWindow();
}


//��ʼ������
bool CDlgMoneyRecord::InitRoomName()
{
	

	return true;
}


////////////////////////////////////////////////////////////////////////////////////
void CDlgMoneyRecord::OnBnClickedGet()
{
	

	m_enOperateStatus = enOperateRecordStatus_Save;
	//��������
	if ( ConnectServer() == false )
	{
		//����״̬
		m_enOperateStatus = enOperateMoneyRecordStatus_NULL;
		return;
	}
}

void CDlgMoneyRecord::InitTable()
{
	m_RecordListCtrl.DeleteAllItems();

	int nShowPage=0;
	if(m_nCount>=PageNum)
	{
		nShowPage = PageNum;

		nCurrentPage = 1;
		m_btFirstPage.EnableWindow(0);
		m_btEndPage.EnableWindow(1);
		m_btLastPage.EnableWindow(0);
		m_btNextPage.EnableWindow(1);

		for(int i=0;i<PageNum;i++)
		{
			CString str;
			str.Format("%d",GiveRecord[i].dwRecordID);
			m_RecordListCtrl.InsertItem(i,str);
			if(strstr( GiveRecord[i].szGiveName,g_GlobalUnits.GetGolbalUserData().szAccounts) != NULL)
			{
				m_RecordListCtrl.SetItemText(i,1,GiveRecord[i].szTargetUserName);
				m_RecordListCtrl.SetItemText(i,2,"ת��");
			}
			else
			{
				m_RecordListCtrl.SetItemText(i,1,GiveRecord[i].szGiveName);
				m_RecordListCtrl.SetItemText(i,2,"ת��");
			}

			str.Format("%I64d",GiveRecord[i].lGiveScore);
			m_RecordListCtrl.SetItemText(i,3,str);
			m_RecordListCtrl.SetItemText(i,4,GiveRecord[i].szTime);
		}

	
	}
	else
	{
		for(int i=0;i<m_nCount;i++)
		{
			CString str;
			str.Format("%d",GiveRecord[i].dwRecordID);
			m_RecordListCtrl.InsertItem(i,str);
			if(strstr( GiveRecord[i].szGiveName,g_GlobalUnits.GetGolbalUserData().szAccounts) != NULL)
			{
				m_RecordListCtrl.SetItemText(i,1,GiveRecord[i].szTargetUserName);
				m_RecordListCtrl.SetItemText(i,2,"ת��");
			}
			else
			{
				m_RecordListCtrl.SetItemText(i,1,GiveRecord[i].szGiveName);
				m_RecordListCtrl.SetItemText(i,2,"ת��");
			}
			str.Format("%I64d",GiveRecord[i].lGiveScore);
			m_RecordListCtrl.SetItemText(i,3,str);
			m_RecordListCtrl.SetItemText(i,4,GiveRecord[i].szTime);
		}
	}

	nPageNum = m_nCount/PageNum;
	

	if(m_nCount%PageNum!=0)
		nPageNum=nPageNum+1;

	CString strPage;
	strPage.Format("�� %d ҳ",nPageNum);

	GetDlgItem(IDC_PAGE)->SetWindowText(strPage);
}

void CDlgMoneyRecord::OnBnClickedFirst()
	{
		m_RecordListCtrl.DeleteAllItems();
		nCurrentPage = 1;
		m_btFirstPage.EnableWindow(0);
		m_btEndPage.EnableWindow(1);
		m_btLastPage.EnableWindow(0);
		m_btNextPage.EnableWindow(1);
		
		for(int i=0;i<PageNum;i++)
		{
			CString str;
			str.Format("%d",GiveRecord[i].dwRecordID);
			m_RecordListCtrl.InsertItem(i,str);
			if(strstr( GiveRecord[i].szGiveName,g_GlobalUnits.GetGolbalUserData().szAccounts) != NULL)
			{
				m_RecordListCtrl.SetItemText(i,1,GiveRecord[i].szTargetUserName);
				m_RecordListCtrl.SetItemText(i,2,"ת��");
			}
			else
			{
				m_RecordListCtrl.SetItemText(i,1,GiveRecord[i].szGiveName);
				m_RecordListCtrl.SetItemText(i,2,"ת��");
			}
			str.Format("%i64",GiveRecord[i].lGiveScore);
			m_RecordListCtrl.SetItemText(i,3,str);
			m_RecordListCtrl.SetItemText(i,4,GiveRecord[i].szTime);
		}

		CString strPage;
		strPage.Format("�� %d ҳ",nCurrentPage);

		GetDlgItem(IDC_PAGE)->SetWindowText(strPage);

	
	}
void CDlgMoneyRecord::OnBnClickedLast()
	{
		m_RecordListCtrl.DeleteAllItems();
		nCurrentPage --;

		if(nCurrentPage==1)
		{
			m_btFirstPage.EnableWindow(0);
			m_btEndPage.EnableWindow(1);
			m_btLastPage.EnableWindow(0);
			m_btNextPage.EnableWindow(1);
		}
		else
		{
			m_btFirstPage.EnableWindow(1);
			m_btEndPage.EnableWindow(1);
			m_btLastPage.EnableWindow(1);
			m_btNextPage.EnableWindow(1);
		}
		
		/*if(nCurrentPage==nPageNum)
			{
				for(int i=0;i<m_nCount-PageNum*(nPageNum-1);i++)
				{
					CString str;
					str.Format("%d",i);
					m_RecordListCtrl.InsertItem(i,str);	
					m_RecordListCtrl.SetItemText(i,1,GiveRecord[i+PageNum*(nPageNum-1)].szGiveName);

					str.Format("%d",GiveRecord[i+PageNum*(nPageNum-1)].cbType);
					m_RecordListCtrl.SetItemText(i,2,str);
					str.Format("%ld",GiveRecord[i+PageNum*(nPageNum-1)].lGiveScore);
					m_RecordListCtrl.SetItemText(i,3,str);
					m_RecordListCtrl.SetItemText(i,4,GiveRecord[i+PageNum*(nPageNum-1)].szTime);
				}
			}
		else
			{*/
				
				for(int i=0;i<PageNum;i++)
				{
					CString str;
					str.Format("%d",GiveRecord[i+PageNum*(nCurrentPage-1)].dwRecordID);
					m_RecordListCtrl.InsertItem(i,str);
					if(strstr( GiveRecord[i+PageNum*(nCurrentPage-1)].szGiveName,g_GlobalUnits.GetGolbalUserData().szAccounts) != NULL)
					{
						m_RecordListCtrl.SetItemText(i,1,GiveRecord[i+PageNum*(nCurrentPage-1)].szTargetUserName);
						m_RecordListCtrl.SetItemText(i,2,"ת��");
					}
					else
					{
						m_RecordListCtrl.SetItemText(i,1,GiveRecord[i+PageNum*(nCurrentPage-1)].szGiveName);
						m_RecordListCtrl.SetItemText(i,2,"ת��");
					}
// 					CString str;
// 					str.Format("%d",i+PageNum*(nCurrentPage-1));
// 					m_RecordListCtrl.InsertItem(i,str);	
// 					m_RecordListCtrl.SetItemText(i,1,GiveRecord[i+PageNum*(nCurrentPage-1)].szGiveName);
// 					str.Format("%d",GiveRecord[i+PageNum*(nCurrentPage-1)].cbType);
// 					m_RecordListCtrl.SetItemText(i,2,str);
					str.Format("%I64d",GiveRecord[i+PageNum*(nCurrentPage-1)].lGiveScore);
					m_RecordListCtrl.SetItemText(i,3,str);
					m_RecordListCtrl.SetItemText(i,4,GiveRecord[i+PageNum*(nCurrentPage-1)].szTime);
				}
	
			
			
		//	}
		CString strPage;
		strPage.Format("�� %d ҳ",nCurrentPage);

		GetDlgItem(IDC_PAGE)->SetWindowText(strPage);

	
	
	}
void CDlgMoneyRecord::OnBnClickedNext()
	{
		m_RecordListCtrl.DeleteAllItems();
		nCurrentPage ++;

		if(nCurrentPage==nPageNum)
		{
			m_btFirstPage.EnableWindow(1);
			m_btEndPage.EnableWindow(0);
			m_btLastPage.EnableWindow(1);
			m_btNextPage.EnableWindow(0);
		}
		else
		{
			m_btFirstPage.EnableWindow(1);
			m_btEndPage.EnableWindow(1);
			m_btLastPage.EnableWindow(1);
			m_btNextPage.EnableWindow(1);
		}
		
		if(nCurrentPage==nPageNum)
			{
				for(int i=0;i<m_nCount-PageNum*(nPageNum-1);i++)
				{
// 					CString str;
// 					str.Format("%d",i+PageNum*(nPageNum-1));
// 					m_RecordListCtrl.InsertItem(i,str);	
// 					m_RecordListCtrl.SetItemText(i,1,GiveRecord[i+PageNum*(nPageNum-1)].szGiveName);
// 
// 					str.Format("%d",GiveRecord[i+PageNum*(nPageNum-1)].cbType);
// 					m_RecordListCtrl.SetItemText(i,2,str);
					CString str;
					str.Format("%d",GiveRecord[i+PageNum*(nPageNum-1)].dwRecordID);
					m_RecordListCtrl.InsertItem(i,str);
					if(strstr( GiveRecord[i+PageNum*(nPageNum-1)].szGiveName,g_GlobalUnits.GetGolbalUserData().szAccounts) != NULL)
					{
						m_RecordListCtrl.SetItemText(i,1,GiveRecord[i+PageNum*(nPageNum-1)].szTargetUserName);
						m_RecordListCtrl.SetItemText(i,2,"ת��");
					}
					else
					{
						m_RecordListCtrl.SetItemText(i,1,GiveRecord[i+PageNum*(nPageNum-1)].szGiveName);
						m_RecordListCtrl.SetItemText(i,2,"ת��");
					}

					str.Format("%I64d",GiveRecord[i+PageNum*(nPageNum-1)].lGiveScore);
					m_RecordListCtrl.SetItemText(i,3,str);
					m_RecordListCtrl.SetItemText(i,4,GiveRecord[i+PageNum*(nPageNum-1)].szTime);
				}
			}
		else
			{
				
				for(int i=0;i<PageNum;i++)
				{
					CString str;
					str.Format("%d",GiveRecord[i+PageNum*(nPageNum-1)].dwRecordID);
					m_RecordListCtrl.InsertItem(i,str);
					if(strstr( GiveRecord[i+PageNum*(nPageNum-1)].szGiveName,g_GlobalUnits.GetGolbalUserData().szAccounts) != NULL)
					{
						m_RecordListCtrl.SetItemText(i,1,GiveRecord[i+PageNum*(nPageNum-1)].szTargetUserName);
						m_RecordListCtrl.SetItemText(i,2,"ת��");
					}
					else
					{
						m_RecordListCtrl.SetItemText(i,1,GiveRecord[i+PageNum*(nPageNum-1)].szGiveName);
						m_RecordListCtrl.SetItemText(i,2,"ת��");
					}

					str.Format("%I64d",GiveRecord[i+PageNum*(nCurrentPage-1)].lGiveScore);
					m_RecordListCtrl.SetItemText(i,3,str);
					m_RecordListCtrl.SetItemText(i,4,GiveRecord[i+PageNum*(nCurrentPage-1)].szTime);
				}
	
			
			
			}
		CString strPage;
		strPage.Format("�� %d ҳ",nCurrentPage);

		GetDlgItem(IDC_PAGE)->SetWindowText(strPage);

	
	
	}
void CDlgMoneyRecord::OnBnClickedEnd()
{
		m_RecordListCtrl.DeleteAllItems();
		nCurrentPage = nPageNum;
		m_btFirstPage.EnableWindow(1);
		m_btEndPage.EnableWindow(1);
		m_btLastPage.EnableWindow(1);
		m_btNextPage.EnableWindow(0);
		
		for(int i=0;i<m_nCount-PageNum*(nPageNum-1);i++)
		{
// 			CString str;
// 			str.Format("%d",i+PageNum*(nPageNum-1));
// 			m_RecordListCtrl.InsertItem(i,str);	
// 			m_RecordListCtrl.SetItemText(i,1,GiveRecord[i+PageNum*(nPageNum-1)].szGiveName);
// 
// 			str.Format("%d",GiveRecord[i+PageNum*(nPageNum-1)].cbType);
// 			m_RecordListCtrl.SetItemText(i,2,str);
			CString str;
			str.Format("%d",GiveRecord[i+PageNum*(nPageNum-1)].dwRecordID);
			m_RecordListCtrl.InsertItem(i,str);
			if(strstr( GiveRecord[i+PageNum*(nPageNum-1)].szGiveName,g_GlobalUnits.GetGolbalUserData().szAccounts) != NULL)
			{
				m_RecordListCtrl.SetItemText(i,1,GiveRecord[i+PageNum*(nPageNum-1)].szTargetUserName);
				m_RecordListCtrl.SetItemText(i,2,"ת��");
			}
			else
			{
				m_RecordListCtrl.SetItemText(i,1,GiveRecord[i+PageNum*(nPageNum-1)].szGiveName);
				m_RecordListCtrl.SetItemText(i,2,"ת��");
			}

			str.Format("%I64d",GiveRecord[i+PageNum*(nPageNum-1)].lGiveScore);
			m_RecordListCtrl.SetItemText(i,3,str);
			m_RecordListCtrl.SetItemText(i,4,GiveRecord[i+PageNum*(nPageNum-1)].szTime);
		}

		CString strPage;
		strPage.Format("�� %d ҳ",nCurrentPage);

		GetDlgItem(IDC_PAGE)->SetWindowText(strPage);

}
