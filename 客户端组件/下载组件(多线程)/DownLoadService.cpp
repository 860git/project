#include "StdAfx.h"
#include "Direct.h"
#include "DownLoadService.h"

#define WM_LBUTTONUPU (WM_USER+11)		// ����ȡ������֪ͨ
//////////////////////////////////////////////////////////////////////////
//�궨��

//��ʱ����ʶ
#define IDI_UPDATE_TIMER						10							//���¶�ʱ��
#define IDI_DEL_DOWNLOAD_TIMER					11							//ȡ����ʱ��
//��ȡ���� HTML
const TCHAR szErrorHtml1[]=TEXT("<html>");
const TCHAR szErrorHtml2[]=TEXT("<!DOCTYPE HTML PUBLIC");

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDownLoadMission, CSkinDialogEx)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_RETRY, OnBnClickedReTry)
	ON_MESSAGE(WM_LBUTTONUPU, OnButtonPu)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CDownLoadThread::CDownLoadThread()
{
	//���ñ���
	m_dwMissionID=0;
	m_dwDownLoadType=0;

	m_bPreparative=false;
	m_szTempFile[0]=0;
	m_szLocalFile[0]=0;
	m_dwOldDownLoadSize = 0;
	m_dwDownLoadSize=0;
	m_dwTotalFileSize=0;
	m_pHttpFile=NULL;
	m_pHttpConnection=NULL;
//	m_pDownLoadRequest=NULL;
	memset(&m_DownLoadRequest,0,sizeof(m_DownLoadRequest));

	m_DownLoadResult=enDownLoadResult_Noknow;
	m_DownLoadStatus=enDownLoadStatus_Unknow;

	return;
}

//��������
CDownLoadThread::~CDownLoadThread()
{
	DownLoadCleanUp();
	return;
}

//��ʼ���߳�
bool CDownLoadThread::InitThread(tagDownLoadRequest * pDownLoadRequest)
{
//	ASSERT(pDownLoadRequest!=NULL);
//	m_pDownLoadRequest=pDownLoadRequest;
	return true;
}

//����״̬
void CDownLoadThread::GetDownLoadStatus(tagDownLoadStatus & DownLoadStatus)
{
	//������Դ
//	CThreadLock ThreadLockHandle(m_CriticalSection);

	//���ñ���
	DownLoadStatus.DownLoadStatus=m_DownLoadStatus;
	switch (m_DownLoadStatus)
	{
	case enDownLoadStatus_Unknow:
	case enDownLoadStatus_Ready:
		{
			DownLoadStatus.wProgress=0;
			lstrcpyn(DownLoadStatus.szStatus,TEXT("���ڻ�ȡ�ļ�..."),CountArray(DownLoadStatus.szStatus));
			break;
		}
	case enDownLoadStatus_DownLoadIng:
		{
			DownLoadStatus.wProgress=(WORD)(m_dwDownLoadSize*100L/m_dwTotalFileSize);
			_snprintf(DownLoadStatus.szStatus,sizeof(DownLoadStatus.szStatus),
				TEXT("�Ѹ��ƣ�%ld KB ���� %ld KB��"),m_dwDownLoadSize/1000L,m_dwTotalFileSize/1000L);
			break;
		}
	case enDownLoadStatus_Finish:
		{
			DownLoadStatus.wProgress=100;
			lstrcpyn(DownLoadStatus.szStatus,TEXT("���سɹ�"),CountArray(DownLoadStatus.szStatus));
			break;
		}
	case enDownLoadStatus_Fails:
		{
			DownLoadStatus.wProgress=0;
			_snprintf(DownLoadStatus.szStatus,sizeof(DownLoadStatus.szStatus),TEXT("����ʧ�ܣ�����ţ�%ld"),m_DownLoadResult);
			break;
		}
	case enDownLoadStatus_Stop:
		{
			break;
		}
	default: 
		{
			ASSERT(FALSE);
			memset(&DownLoadStatus,0,sizeof(DownLoadStatus));
			break;
		}
	}

	return;
}

	//����״̬
void CDownLoadThread::SetDownLoadStatus( enDownLoadStatus status1 )
{
		//������Դ
	CThreadLock ThreadLockHandle(m_CriticalSection);
	m_DownLoadStatus = status1;
}

//Ŀ���ļ�
LPCTSTR CDownLoadThread::GetDownLoadFileName()
{
	ASSERT(m_DownLoadStatus==enDownLoadStatus_Finish);
	return m_szLocalFile;
}

//���к���
bool CDownLoadThread::OnEventThreadRun()
{
	try
	{

		if ( enDownLoadStatus_Stop == m_DownLoadStatus )		// ����ȡ��
		{
			return false;		// ֹͣ�߳�
		}

		if (m_bPreparative==true)
		{
			//��ȡ�ļ�
			TCHAR szBuffer[4096];
			UINT uReadCount=m_pHttpFile->Read(szBuffer,sizeof(szBuffer));
			if (uReadCount>0)
			{
				//д���ļ�
				m_LocalFile.Write(szBuffer,uReadCount);

				//������Դ
				CThreadLock ThreadLockHandle(m_CriticalSection);

				//���ñ���
				m_dwDownLoadSize+=uReadCount;
				m_DownLoadStatus=enDownLoadStatus_DownLoadIng;

				return true;
			}
			else 
			{
				//�ر��ļ�
				m_LocalFile.Close();

				//�ƶ��ļ�
				_snprintf(m_szLocalFile,sizeof(m_szLocalFile),TEXT("%s\\%s"),m_DownLoadRequest.szLocalPath,m_DownLoadRequest.szFileName);
				DeleteFile(m_szLocalFile);
				if (MoveFileWithProgress(m_szTempFile,m_szLocalFile,NULL, NULL, MOVEFILE_WRITE_THROUGH|MOVEFILE_REPLACE_EXISTING)==FALSE)
					throw enDownLoadResult_CreateFileFails;

				//������Դ
				CThreadLock ThreadLockHandle(m_CriticalSection);

				//���ñ���
				m_DownLoadStatus=enDownLoadStatus_Finish;

				//��������
				DownLoadCleanUp();

				return false;
			}
		}
		else 
		{
			//������Դ
			CThreadLock ThreadLockHandle(m_CriticalSection);

			//���ñ���
			m_DownLoadStatus=enDownLoadStatus_Ready;

			//����׼��
			DownLoadPreparative();

			return true;
		}
	}
	catch (enDownLoadResult DownLoadResult)
	{
		//������Դ
		CThreadLock ThreadLockHandle(m_CriticalSection);

		//���ñ���
		m_DownLoadStatus=enDownLoadStatus_Fails;
		m_DownLoadResult=DownLoadResult;
	}
	catch (CInternetException * pInternetException)
	{
		//�����쳣
		pInternetException->Delete();

		//������Դ
		CThreadLock ThreadLockHandle(m_CriticalSection);

		//���ñ���
		m_DownLoadStatus=enDownLoadStatus_Fails;
		m_DownLoadResult=enDownLoadResult_Exception;
	}
	catch (...) 
	{
		//������Դ
		CThreadLock ThreadLockHandle(m_CriticalSection);

		//���ñ���
		m_DownLoadStatus=enDownLoadStatus_Fails;
		m_DownLoadResult=enDownLoadResult_Exception;
	}

	//������
	DownLoadCleanUp();

	return false;
}

//�ر��¼�
bool CDownLoadThread::OnEventThreadConclude()
{
	DownLoadCleanUp();
	return true;
}

//����׼��
void CDownLoadThread::DownLoadPreparative()
{
	//Ч��״̬
	ASSERT(m_pHttpFile==NULL);
	ASSERT(m_bPreparative==false);

	//���ñ���
	m_szTempFile[0]=0;
	m_szLocalFile[0]=0;
	m_dwOldDownLoadSize = 0;
	m_dwDownLoadSize=0;
	m_dwTotalFileSize=0;

	//����Ŀ¼
	int nExcursion=0;
	TCHAR szDirectory[MAX_PATH]=TEXT("");
	lstrcpyn(szDirectory,m_DownLoadRequest.szLocalPath,CountArray(szDirectory));
	do
	{
		if (szDirectory[nExcursion]==0) 
		{
			::CreateDirectory(szDirectory,NULL);
			break;
		}
		if (szDirectory[nExcursion]==TEXT('\\'))
		{
			szDirectory[nExcursion]=0;
			::CreateDirectory(szDirectory,NULL);
			szDirectory[nExcursion]=TEXT('\\');
		}
		nExcursion++;
	} while (true);

	//�ж���������
	int nStepCount=0;
	DWORD dwStatusCode =0;
	TCHAR szDownBuffer[4096];

	try
	{
		//��ȡ����
		UINT nReadCount=0;
		
		m_pHttpFile=(CHttpFile *)m_InternetSession.OpenURL(m_DownLoadRequest.szDownLoadUrl,1,INTERNET_FLAG_TRANSFER_ASCII|INTERNET_FLAG_DONT_CACHE);
//		TRACE("%s\n", m_DownLoadRequest.szDownLoadUrl);

		nStepCount++;
		m_pHttpFile->QueryInfoStatusCode(dwStatusCode);
		nReadCount=m_pHttpFile->Read(szDownBuffer,sizeof(szDownBuffer));
		nStepCount++;
		if (nReadCount<sizeof(szErrorHtml1)) throw 0;
		nStepCount++;
		if (nReadCount<sizeof(szErrorHtml2)) throw 0;
		nStepCount++;
		if (memcmp(szErrorHtml1,szDownBuffer,lstrlen(szErrorHtml1))==0) throw 0;
		nStepCount++;
		if (memcmp(szErrorHtml2,szDownBuffer,lstrlen(szErrorHtml2))==0)
		{
			
			throw 0;
		}
		nStepCount++;

		//��ȡ��С
		ASSERT(m_pHttpFile!=NULL);
		BOOL bSuccess=m_pHttpFile->QueryInfo(HTTP_QUERY_CONTENT_LENGTH|HTTP_QUERY_FLAG_NUMBER,m_dwTotalFileSize);
		nStepCount++;
		if (bSuccess==FALSE)
		{
			//CString strError;
			//DWORD dwError=GetLastError();
			//strError.Format(TEXT("����ţ�%ld"),dwError);
			//AfxMessageBox(strError);
			throw enDownLoadResult_InternetReadError;
		}

		nStepCount++;

		//�����ļ�
		_snprintf(m_szTempFile,sizeof(m_szTempFile),TEXT("%s\\%s.DTM"),m_DownLoadRequest.szLocalPath,m_DownLoadRequest.szFileName);
		if (m_LocalFile.Open(m_szTempFile,CFile::modeReadWrite|CFile::modeCreate)==FALSE) throw enDownLoadResult_CreateFileFails;
		nStepCount++;

		//д���ļ�
		m_LocalFile.Write(szDownBuffer,nReadCount);
		nStepCount++;

		//������Դ
		CThreadLock ThreadLockHandle(m_CriticalSection);
		nStepCount++;

		//���ñ���
		m_dwDownLoadSize+=nReadCount;
		m_DownLoadStatus=enDownLoadStatus_DownLoadIng;

		//���ñ���
		m_bPreparative=true;

		return;
	}
	catch (...)
	{ 
		//CString strError;
		//DWORD dwError=GetLastError();
		//strError.Format(TEXT("ִ�в��裺%ld,״̬�룺%ld"),nStepCount,dwStatusCode);
		//AfxMessageBox(szDownBuffer);
		//SafeDelete(m_pHttpFile);
		throw enDownLoadResult_InternetReadError;
	}

	return;
}

//�������
void CDownLoadThread::DownLoadCleanUp()
{
	//���ñ���
	m_dwDownLoadSize=0;
	m_dwTotalFileSize=0;
	m_bPreparative=false;

	//������Դ
	if (m_pHttpFile!=NULL) 
	{
		m_pHttpFile->Close();
		SafeDelete(m_pHttpFile);
	}
	if (m_pHttpConnection!=NULL)
	{
		m_pHttpConnection->Close();
		SafeDelete(m_pHttpConnection);
	}
	if (m_LocalFile!=INVALID_HANDLE_VALUE) m_LocalFile.Close();

	//ɾ���ļ�
	if (m_szTempFile[0]!=0) DeleteFile(m_szTempFile);

	return;
}

// �����ٶȣ�
DWORD CDownLoadThread::GetBual()
{ 
	DWORD dwResult = 0;

	dwResult = m_dwDownLoadSize - m_dwOldDownLoadSize;
	m_dwOldDownLoadSize = m_dwDownLoadSize;
	if ( dwResult< 0)
	{
		dwResult = 0;
	}

	return dwResult;

}

//////////////////////////////////////////////////////////////////////////

//���캯��
//CDownLoadMission::CDownLoadMission(IDownLoadMissionSink * pIDownLoadMissionSink) : CSkinDialogEx(IDD_DOWN_LOAD)
CDownLoadMission::CDownLoadMission( ) : CSkinDialogEx(IDD_DOWN_LOAD)
{
	//���ñ���
//	m_dwMissionID=0;
//	m_dwDownLoadType=0;
//	m_pIDownLoadMissionSink=pIDownLoadMissionSink;
	m_pIDownLoadMissionSink = NULL;
//	m_DownLoadThread.InitThread(&m_DownLoadRequest);
//	m_DownLoadThread.InitThread( NULL );
//	memset(&m_DownLoadRequest,0,sizeof(m_DownLoadRequest));

	return;
}

//��������
CDownLoadMission::~CDownLoadMission()
{
}

//�ؼ���
void CDownLoadMission::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RETRY, m_btReTry);
	DDX_Control(pDX, IDCANCEL, m_btCancel);
//	DDX_Control(pDX, IDC_PROGRESS, m_ProgressCtrl);
//	DDX_Control(pDX, IDC_DOWN_LOAD_URL, m_DownLoadUrl);
	DDX_Control(pDX, IDC_DOWNLOAD_LIST, m_DownloadListView);

}

//��ʼ������
BOOL CDownLoadMission::OnInitDialog()
{
	__super::OnInitDialog();

	
	COLORREF crBkColor = RGB(0,0,0);
	m_DownloadListView.SetTextColor(crBkColor);

	DWORD dwExStyle = LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;
	m_DownloadListView.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LPARAM(dwExStyle));

	m_DownloadListView.InsertColumn(0,"No",LVCFMT_CENTER,0);
	m_DownloadListView.InsertColumn(1,"����",LVCFMT_CENTER,38);
	m_DownloadListView.InsertColumn(2,"��������",LVCFMT_CENTER,100);
	m_DownloadListView.InsertColumn(3,"��С",LVCFMT_CENTER,60);
	m_DownloadListView.InsertColumn(4,"����",LVCFMT_RIGHT,195);
	m_DownloadListView.InsertColumn(5,"�ٶ�(kb/s)",LVCFMT_CENTER,90);

	DWORD dwStype=GetWindowLong(m_DownloadListView.GetSafeHwnd(),GWL_STYLE);
	dwStype&=~LVS_TYPEMASK;		//Remove the current stype flags
	dwStype|=LVS_REPORT;		//Add the List stype
	SetWindowLong(m_DownloadListView.GetSafeHwnd(),GWL_STYLE,dwStype);	//Set it back into the list view

	m_DownloadListView.SetBkColor(RGB(255,255,254));
	//m_DownloadListView.ShowWindow(0);

	//��������
//	SetDlgItemText(IDC_DESCRIPTION,m_DownLoadRequest.szDescribe);

	//���ö�ʱ��
	SetTimer(IDI_UPDATE_TIMER,300,NULL);
//	SetTimer(IDI_UPDATE_TIMER,1000,NULL);
	return TRUE;
}


//��Ϣ����
BOOL CDownLoadMission::PreTranslateMessage(MSG * pMsg)
{
	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_ESCAPE)) return TRUE;
	return __super::PreTranslateMessage(pMsg);
}

//ȡ����Ϣ
void CDownLoadMission::OnCancel()
{
	//�رս���
	DestroyWindow();

	//ֹͣ����
//	m_DownLoadThread.ConcludeThread(INFINITE);
	
	//�¼�֪ͨ
//	m_pIDownLoadMissionSink->OnMissionFinish(enDownLoadStatus_Fails,this);

	StopDownLoad();

	return;
}

//���԰�ť
void CDownLoadMission::OnBnClickedReTry()
{
	//���ý���
	m_btReTry.EnableWindow(FALSE);

	//��������
//	m_DownLoadThread.StartThread();

	//����ʱ��
//	SetTimer(IDI_UPDATE_TIMER,300,NULL);

	return;
}

//��ʱ����Ϣ
void CDownLoadMission::OnTimer(UINT_PTR nIDEvent)
{
	__super::OnTimer(nIDEvent);

	//����״̬
	if (nIDEvent==IDI_UPDATE_TIMER)
	{

		CThreadLock ThreadLockHandle(m_CriticalSection);

		static	int nCurrentPos = 0;
		//���ÿؼ�
		tagDownLoadStatus DownLoadStatus;

		CDownLoadThread * pDownLoadThread=NULL;

		int nActiceCount = m_DownLoadThreadActive.GetCount();

		// ����λ��
		if ( nCurrentPos >= nActiceCount )
		{
			nCurrentPos = 0;
		}

//		for (INT_PTR i=0;i<m_DownLoadThreadActive.GetCount();i++)
		{
			pDownLoadThread=m_DownLoadThreadActive[nCurrentPos++];

			pDownLoadThread->GetDownLoadStatus(DownLoadStatus);

			int nListCount = m_DownloadListView.GetItemCount();

			int nErrorCount = 0;

			int nFlag = 0;
			 
	
			for ( int jj = 0; jj < nListCount; jj++)
			{
				CString strMissionID;
		//		strMissionID = m_DownloadListView.GetItemText( jj, 0 );
				if (m_DownloadListView.GetItemData(jj) == pDownLoadThread->m_dwMissionID)
				{
					CString strProgess;
					strProgess.Format("%d%%������", DownLoadStatus.wProgress);
					m_DownloadListView.SetItemText( jj, 4, strProgess);

					CString strTotalFileSize;
					strTotalFileSize.Format("%ldKB", pDownLoadThread->GetTotalFileSize()/1024);
					m_DownloadListView.SetItemText( jj, 3, strTotalFileSize);

					m_DownloadListView.SetProgress( jj, DownLoadStatus.wProgress);		// ������

					CString strBual;
					strBual.Format("%dkB/s", pDownLoadThread->GetBual()/1024L);
					m_DownloadListView.SetItemText( jj, 5, strBual);


					//////////////////
					{

						//״̬�л�
						switch (DownLoadStatus.DownLoadStatus)
						{
						case enDownLoadStatus_Fails:
							{

								CString strProgess;
								strProgess.Format("���ش���" );
								m_DownloadListView.SetItemText( jj, 4, strProgess);

					//			nErrorCount++;

					//			if ( nErrorCount == nListCount )
					//			{
					//				KillTimer(IDI_UPDATE_TIMER);
					//				return ;
					//			}

								break;
							}
						case enDownLoadStatus_Finish:
							{

						//		DestroyWindow();
								LPCTSTR pszLocalFile= pDownLoadThread->GetDownLoadFileName();
								TCHAR szRunFile[256];
								_snprintf(szRunFile, CountArray(szRunFile), TEXT("%s /sp- /silent /norestart"), pszLocalFile);
								ShellExecute(NULL,TEXT("open"),szRunFile,NULL,NULL,SW_SHOWDEFAULT);

								//�¼�֪ͨ
				//				m_pIDownLoadMissionSink->OnMissionFinish(enDownLoadStatus_Finish,this);
								pDownLoadThread->ConcludeThread(INFINITE);
								OnMissionFinish(enDownLoadStatus_Finish,pDownLoadThread);

								m_DownloadListView.DeleteItem(jj);

								if (m_DownloadListView.GetItemCount() == 0 )
								{
									DestroyWindow();
								}

						//		break;
								return;
							}
						case enDownLoadStatus_Stop:		// �û��ֶ�ֹͣ����
							{

								//�¼�֪ͨ
				//				m_pIDownLoadMissionSink->OnMissionFinish(enDownLoadStatus_Finish,this);
								pDownLoadThread->ConcludeThread(INFINITE);
								OnMissionFinish(enDownLoadStatus_Fails,pDownLoadThread);

								m_DownloadListView.DeleteItem(jj);

								if (m_DownloadListView.GetItemCount() == 0 )
								{
									DestroyWindow();
								}

						//		break;
								return;
							}
						default:
							break;
						}
					}


					///////////////////
					
					break;
				}

			}

	
		}
		return;
	}

	return;
}


// ����ȡ�����أ�
HRESULT CDownLoadMission::OnButtonPu(WPARAM wParam, LPARAM lParam)
{
	TRACE("OnButtonPu():%ld\n", wParam);
	CDownLoadThread * pDownLoadThread=NULL;

	CThreadLock ThreadLockHandle(m_CriticalSection);

	for (INT_PTR i=0;i<m_DownLoadThreadActive.GetCount();i++)
	{
		pDownLoadThread=m_DownLoadThreadActive[i];
		
		if ( pDownLoadThread->m_dwMissionID == (DWORD)wParam) 
		{
			TRACE("ConcludeThread(%d)\n\n", (DWORD)wParam );
			pDownLoadThread->ConcludeThread(INFINITE);
			pDownLoadThread->SetDownLoadStatus( enDownLoadStatus_Stop );

			break;
		}
	}

	return 0;
}

//��ʾ����
bool CDownLoadMission::DisplayDownLoadFace(bool bDisplay)
{
	//���ñ���
//	m_DownLoadRequest.bDisplay=bDisplay;

	//��ʾ����
	if (m_hWnd!=NULL) ShowWindow(bDisplay?SW_SHOW:SW_HIDE);

	return true;
}

//�Ա�����
bool CDownLoadMission::CompareRequest(tagDownLoadRequest * pDownLoadRequest)
{
//	if (lstrcmp(m_DownLoadRequest.szDownLoadUrl,pDownLoadRequest->szDownLoadUrl)!=0) return false;
	return true;
}

//��ʼ����
bool CDownLoadMission::StartDownLoad(DWORD dwMissionID, DWORD dwDownLoadType, tagDownLoadRequest * pDownLoadRequest)
{
//	m_DownLoadUrl.SetHyperLinkUrl(pDownLoadRequest->szDownLoadUrl);  	2011.3.9

/*	//���ñ���
	m_dwMissionID=dwMissionID;
	m_dwDownLoadType=dwDownLoadType;
	CopyMemory(&m_DownLoadRequest,pDownLoadRequest,sizeof(m_DownLoadRequest));
	if (m_DownLoadRequest.szLocalPath[0]==0) GetCurrentDirectory(sizeof(m_DownLoadRequest.szLocalPath),m_DownLoadRequest.szLocalPath);

	//��������
	m_DownLoadThread.StartThread();
*/
	//////////////////////////

	CThreadLock ThreadLockHandle(m_CriticalSection);

//Ѱ������
	CDownLoadThread * pDownLoadThread=SearchDownLoadThread( pDownLoadRequest );
	if (pDownLoadThread!=NULL)
	{
		return false;
	}

	/////////////////////////////

	if ( GetSafeHwnd() == NULL)
	{
		//��������
		AfxSetResourceHandle(GetModuleHandle(IDOWN_LOAD_DLL_NAME));
		Create(IDD_DOWN_LOAD,AfxGetMainWnd());
		AfxSetResourceHandle(GetModuleHandle(NULL));
	//	if (m_DownLoadRequest.bDisplay==true) ShowWindow(SW_SHOW);
	}

	ShowWindow(SW_SHOW);

	//��������
	INT_PTR nReleaseCount=m_DownLoadThreadRelease.GetCount();
	if (nReleaseCount>0)
	{
		pDownLoadThread=m_DownLoadThreadRelease[nReleaseCount-1];
		m_DownLoadThreadRelease.RemoveAt(nReleaseCount-1);
	}
	else
	{
		try { pDownLoadThread=new CDownLoadThread(); }
		catch (...) { return 0; }
	}
	ASSERT(pDownLoadThread!=NULL);
	if (pDownLoadThread==NULL) return 0;
	m_DownLoadThreadActive.Add(pDownLoadThread);

////////////////////////
	int nItem = m_DownloadListView.GetItemCount();

	CString strTemp;
	strTemp.Format("%d", nItem + 1 );
	m_DownloadListView.InsertItem(LVIF_TEXT|LVIF_STATE, nItem, strTemp, 0, 0, 0, 0);

	CString strFileName;
	strFileName.Format("%s", pDownLoadRequest->szFileName );
	m_DownloadListView.SetItemText( nItem, 2, strFileName);
	m_DownloadListView.SetItemData( nItem, dwMissionID);

	m_DownloadListView.CreateProgress( nItem, 4, dwMissionID );
	TRACE("CreateButton:%d\n", dwMissionID);
	m_DownloadListView.CreateButton( nItem, 1, m_hWnd, dwMissionID );

	CString strProgess;
	strProgess.Format("%d%%������", 0 );
	m_DownloadListView.SetItemText( nItem, 4, strProgess);

	CString strTotalFileSize;
	strTotalFileSize.Format("%ldKB", 0 );
	m_DownloadListView.SetItemText( nItem, 3, strTotalFileSize);

	m_DownloadListView.SetProgress( nItem, 1 );		// ������

	CString strBual;
	strBual.Format("%dkB/s", 0 );
	m_DownloadListView.SetItemText( nItem, 5, strBual);
////////////////////////

	//���ñ���
	pDownLoadThread->InitThread( NULL );
	pDownLoadThread->m_dwMissionID=dwMissionID;
	pDownLoadThread->m_dwDownLoadType=dwDownLoadType;
	CopyMemory(&(pDownLoadThread->m_DownLoadRequest),pDownLoadRequest,sizeof(pDownLoadThread->m_DownLoadRequest));
	if ( pDownLoadThread->m_DownLoadRequest.szLocalPath[0]==0) 
	{
		GetCurrentDirectory(sizeof(pDownLoadThread->m_DownLoadRequest.szLocalPath), pDownLoadThread->m_DownLoadRequest.szLocalPath);
	}

		//��������
	pDownLoadThread->StartThread();



	return true;
}

//ֹͣ����
bool CDownLoadMission::StopDownLoad()
{
	//�رս���
	DestroyWindow();

	//ֹͣ�߳�
//	m_DownLoadThread.ConcludeThread(INFINITE);
	CThreadLock ThreadLockHandle(m_CriticalSection);
	//��������
	CDownLoadThread * pDownLoadThread=NULL;
	for (INT_PTR i=0;i<m_DownLoadThreadActive.GetCount();i++)
	{
		pDownLoadThread=m_DownLoadThreadActive[i];
		pDownLoadThread->ConcludeThread(INFINITE);
		SafeDelete(pDownLoadThread);
	}
	for (INT_PTR i=0;i<m_DownLoadThreadRelease.GetCount();i++)
	{
		pDownLoadThread=m_DownLoadThreadRelease[i];
		SafeDelete(pDownLoadThread);
	}
	m_DownLoadThreadActive.RemoveAll();
	m_DownLoadThreadRelease.RemoveAll();
	return true;
}

//��ȡ��Ŀ
INT_PTR __cdecl CDownLoadMission::GetDownLoadMissionCount()
{
	return m_DownLoadThreadActive.GetCount();
}

//����֪ͨ
void CDownLoadMission::OnMissionFinish(enDownLoadStatus DownLoadStatus, CDownLoadThread * pDownLoadThread)
{
	//Ѱ������
	INT_PTR i;
	for (i=0;i<m_DownLoadThreadActive.GetCount();i++)
	{
		if (pDownLoadThread==m_DownLoadThreadActive[i])
		{
			break;
		}
	}
	ASSERT(i!=m_DownLoadThreadActive.GetCount());
	if (i==m_DownLoadThreadActive.GetCount()) return;

	//��������
	pDownLoadThread->SetDownLoadStatus( enDownLoadStatus_Unknow);
	m_DownLoadThreadActive.RemoveAt(i);
	m_DownLoadThreadRelease.Add(pDownLoadThread);

	return;
}

//Ѱ������ (����)
CDownLoadThread * CDownLoadMission::SearchDownLoadThread( DWORD dwMissionID )
{
	CDownLoadThread * pDownLoadThread=NULL;
	for (INT_PTR i=0;i<m_DownLoadThreadActive.GetCount();i++)
	{
		pDownLoadThread=m_DownLoadThreadActive[i];
//		if (pDownLoadMission->CompareRequest(pDownLoadRequest)==true) return pDownLoadThread;
		if ( pDownLoadThread->m_dwMissionID == dwMissionID )
		{
			 return pDownLoadThread;
		}
	}

	return NULL;
}

//Ѱ������
CDownLoadThread * CDownLoadMission::SearchDownLoadThread( tagDownLoadRequest * pDownLoadRequest )
{
	

	CDownLoadThread * pDownLoadThread=NULL;

	for (INT_PTR i=0;i<m_DownLoadThreadActive.GetCount();i++)
	{
		pDownLoadThread=m_DownLoadThreadActive[i];
//		if (pDownLoadMission->CompareRequest(pDownLoadRequest)==true) return pDownLoadThread;
//		if ( pDownLoadThread->m_dwMissionID == dwMissionID )
			if (lstrcmp(pDownLoadThread->m_DownLoadRequest.szDownLoadUrl,pDownLoadRequest->szDownLoadUrl)==0)
		{
			 return pDownLoadThread;
		}
	}

	return NULL;
}




//////////////////////////////////////////////////////////////////////////

//���캯��
CDownLoadService::CDownLoadService()
{
	m_dwMissionID=0;
	m_pIDownLoadServiceSink=NULL;
}

//��������
CDownLoadService::~CDownLoadService()
{
	//��������
/*	CDownLoadMission * pDownLoadMission=NULL;
	for (INT_PTR i=0;i<m_DownLoadMissionActive.GetCount();i++)
	{
		pDownLoadMission=m_DownLoadMissionActive[i];
		pDownLoadMission->StopDownLoad();
		SafeDelete(pDownLoadMission);
	}
	for (INT_PTR i=0;i<m_DownLoadMissionRelease.GetCount();i++)
	{
		pDownLoadMission=m_DownLoadMissionRelease[i];
		SafeDelete(pDownLoadMission);
	}
	m_DownLoadMissionActive.RemoveAll();
	m_DownLoadMissionRelease.RemoveAll();
*/
	m_CDownLoadMission.StopDownLoad();
	return;
}

//�ӿڲ�ѯ
void * __cdecl CDownLoadService::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IDownLoadService,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IDownLoadService,Guid,dwQueryVer);
	return NULL;
}

//��ȡ��Ŀ
INT_PTR __cdecl CDownLoadService::GetDownLoadMissionCount()
{
//	return m_DownLoadMissionActive.GetCount();
	return m_CDownLoadMission.GetDownLoadMissionCount();
}

//���ýӿ�
bool __cdecl CDownLoadService::SetDownLoadServiceSink(IUnknownEx * pIUnknownEx)
{
	ASSERT(pIUnknownEx!=NULL);
	m_pIDownLoadServiceSink=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IDownLoadServiceSink);
	ASSERT(m_pIDownLoadServiceSink!=NULL);
	return (m_pIDownLoadServiceSink!=NULL);
}

//��������
DWORD __cdecl CDownLoadService::AddDownLoadRequest(DWORD dwDownLoadType, tagDownLoadRequest * pDownLoadRequest)
{
	//Ч�����
	ASSERT(pDownLoadRequest!=NULL);
	if (pDownLoadRequest==NULL) return 0;


	//��������
	m_dwMissionID++;
	m_CDownLoadMission.StartDownLoad(m_dwMissionID,dwDownLoadType,pDownLoadRequest);

//	return m_CDownLoadMission.GetMissionID();
	return m_dwMissionID;
}



//////////////////////////////////////////////////////////////////////////

//����������
extern "C" __declspec(dllexport) void * __cdecl CreateDownLoadService(const GUID & Guid, DWORD dwInterfaceVer)
{
	//��������
	CDownLoadService * pDownLoadService=NULL;
	try
	{
		pDownLoadService=new CDownLoadService();
		if (pDownLoadService==NULL) throw TEXT("����ʧ��");
		void * pObject=pDownLoadService->QueryInterface(Guid,dwInterfaceVer);
		if (pObject==NULL) throw TEXT("�ӿڲ�ѯʧ��");
		return pObject;
	}
	catch (...) {}

	//�������
	SafeDelete(pDownLoadService);
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
