#include "StdAfx.h"
#include "Commonality.h"
#include "DataBaseSink.h"
#include "LogonServer.h"


//��������
extern CLogonServerApp theApp;
//////////////////////////////////////////////////////////////////////////

//���캯��
CDataBaseSink::CDataBaseSink()
{
	//���ñ���
	m_pInitParamter=NULL;
	m_pIDataBaseEngineEvent=NULL;

	return;
}

//��������
CDataBaseSink::~CDataBaseSink()
{
}

//�ӿڲ�ѯ
VOID * __cdecl CDataBaseSink::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IDataBaseEngineSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IDataBaseEngineSink,Guid,dwQueryVer);
	return NULL;
}

//ģ������
bool __cdecl CDataBaseSink::OnDataBaseEngineStart(IUnknownEx * pIUnknownEx)
{
	//��������
	if ((m_AccountsDBModule.GetInterface()==NULL)&&(m_AccountsDBModule.CreateInstance()==false))
	{
		ASSERT(FALSE);
		return false;
	}

	try
	{
		//�����û����ݿ�
		m_AccountsDBModule->SetConnectionInfo(m_pInitParamter->m_szUserDataBaseAddr,m_pInitParamter->m_wUserDataBasePort,
			m_pInitParamter->m_szUserDataBaseName,m_pInitParamter->m_szUserDataBaseUser,m_pInitParamter->m_szUserDataBasePass);
		m_AccountsDBModule->OpenConnection();

		//��������
		m_AccountsDBModule->OpenConnection();
		m_AccountsDBAide.SetDataBase(m_AccountsDBModule.GetInterface());

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);

		return false;
	}

	return true;
}

//ģ��ر�
bool __cdecl CDataBaseSink::OnDataBaseEngineStop(IUnknownEx * pIUnknownEx)
{
	//���ö���
	m_AccountsDBAide.SetDataBase(NULL);

	//�ر�����
	if (m_AccountsDBModule.GetInterface()!=NULL)
	{
		m_AccountsDBModule->CloseConnection();
		m_AccountsDBModule.CloseInstance();
	}

	//�������
	m_pInitParamter=NULL;
	m_pIDataBaseEngineEvent=NULL;

	return true;
}

//���ݲ�������
bool __cdecl CDataBaseSink::OnDataBaseEngineRequest(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	switch (wRequestID)
	{
	case DBR_GP_LOGON_BY_ACCOUNTS:		//�ʺŵ�¼
		{
			return OnRequestLogon(wRequestID,dwContextID,pData,wDataSize);
		}
	case DBR_GP_LOGON_BY_USERID:		//I D ��¼
		{
			return OnRequestLogon(wRequestID,dwContextID,pData,wDataSize);
		}
	case DBR_GP_REGISTER_ACCOUNTS:		//ע���ʺ�
		{
			return OnRequestLogon(wRequestID,dwContextID,pData,wDataSize);
		}
	case DBR_GP_USER_UPLOAD_FACE:		//�ϴ�ͷ��
		{
			return OnRequestUploadFace(wRequestID,dwContextID,pData,wDataSize);
		}
	case DBR_GP_DOWNLOAD_FACE:			//����ͷ��
		{
			return OnRequestDownloadFace(wRequestID,dwContextID,pData,wDataSize);
		}
	case DBR_GP_DELETE_FACE:			//ɾ����Ϣ
		{
			return OnRequestCustomFaceDelete(wRequestID,dwContextID,pData,wDataSize);
		}
	case DBR_GP_MODIFY_INDIVIDUAL:		//�޸�����
		{
			return OnRequestModifyIndividual(wRequestID,dwContextID,pData,wDataSize);
		}
	case DBR_GP_SAFE_UNBIND:			//�����
		{
			return OnRequestSafeUnbind(wRequestID,dwContextID,pData,wDataSize);
		}	
	case DBR_GP_SAFE_BIND:				//���ʺ�
		{
			return OnRequestSafeBind(wRequestID,dwContextID,pData,wDataSize);
		}
	case DBR_GP_OPER_MONEY:				//�������չ�	22012-4-10
		{
			return OnRequestOperMoney(wRequestID,dwContextID,pData,wDataSize);
		}
	case DBR_GP_GIVE_MONEY:				//�������չ�	22012-4-10
		{
			return OnRequestGiveMoney(wRequestID,dwContextID,pData,wDataSize);
		}
	case DBR_GP_BANK_PASS:				//���չ�����	22012-4-10
		{
			return OnRequestBankPass(wRequestID,dwContextID,pData,wDataSize);
		}
	case DBR_GP_GIVE_RECORD:				//���ͼ�¼	22012-4-10
		{
			return OnRequestGiveRecord(wRequestID,dwContextID,pData,wDataSize);
		}
	case DBR_GP_USER_PASS:
		{
			return OnRequestUserPass(wRequestID,dwContextID,pData,wDataSize);
		}
	case DBR_GP_USER_WRITE:
		{
			return OnRequestUserWrite(wRequestID,dwContextID,pData,wDataSize);
		}
	case DBR_GP_USER_INFO:
		{
			return OnRequestUserInfo(wRequestID,dwContextID,pData,wDataSize);
		}

	}

	return false;
}

//��¼������
bool CDataBaseSink::OnRequestLogon(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//��������
		LONG lReturnValue=-1;
		LPCTSTR pszPassword=NULL;

		//��¼����
		switch (wRequestID)
		{
		case DBR_GP_LOGON_BY_ACCOUNTS:		//�ʺŵ�¼
			{
				//Ч�����
				ASSERT(wDataSize==sizeof(DBR_GP_LogonByAccounts));
				if (wDataSize!=sizeof(DBR_GP_LogonByAccounts)) return false;

				//ִ�в�ѯ
				DBR_GP_LogonByAccounts * pLogonByAccounts=(DBR_GP_LogonByAccounts *)pData;
				lReturnValue=SPLogonByAccounts(pLogonByAccounts->szAccounts,pLogonByAccounts->szPassWord,pLogonByAccounts->dwClientIP,pLogonByAccounts->szComputerID);
				pszPassword=pLogonByAccounts->szPassWord;

				break;
			}
		case DBR_GP_LOGON_BY_USERID:		//I D ��¼
			{
				//Ч�����
				ASSERT(wDataSize==sizeof(DBR_GP_LogonByUserID));
				if (wDataSize!=sizeof(DBR_GP_LogonByUserID)) return false;

				//ִ�в�ѯ
				DBR_GP_LogonByUserID * pLogonByUserID=(DBR_GP_LogonByUserID *)pData;
				lReturnValue=SPLogonByUserID(pLogonByUserID->dwUserID,pLogonByUserID->szPassWord,pLogonByUserID->dwClientIP,pLogonByUserID->szComputerID);
				pszPassword=pLogonByUserID->szPassWord;

				break;
			}
		case DBR_GP_REGISTER_ACCOUNTS:		//ע���ʺ�
			{
				//Ч�����
				ASSERT(wDataSize==sizeof(DBR_GP_RegisterAccounts));
				if (wDataSize!=sizeof(DBR_GP_RegisterAccounts)) return false;

				//ִ�в�ѯ
				DBR_GP_RegisterAccounts * pRegisterAccounts=(DBR_GP_RegisterAccounts *)pData;
				lReturnValue=SPRegisterAccounts(pRegisterAccounts->szAccounts,pRegisterAccounts->szPassWord,pRegisterAccounts->szSpreader,
					pRegisterAccounts->wFaceID,pRegisterAccounts->cbGender,pRegisterAccounts->dwClientIP,pRegisterAccounts->szComputerID,pRegisterAccounts->szBankpass,pRegisterAccounts->szIdno,pRegisterAccounts->szEmail,pRegisterAccounts->szMobile);
				pszPassword=pRegisterAccounts->szPassWord;

				break;
			}
		default :	//Ĭ�ϴ���
			{
				ASSERT(FALSE); 
				return false;
			}
		}

		//����ж�
		if (lReturnValue==0L)
		{
			//��¼�ɹ�
			DBR_GP_LogonSuccess LogonSuccess;
			ZeroMemory(&LogonSuccess,sizeof(LogonSuccess));

			//��ȡ����
			LogonSuccess.wFaceID=m_AccountsDBAide.GetValue_WORD(TEXT("FaceID"));
			LogonSuccess.dwUserID=m_AccountsDBAide.GetValue_DWORD(TEXT("UserID"));
			LogonSuccess.dwGameID=m_AccountsDBAide.GetValue_DWORD(TEXT("GameID"));
			LogonSuccess.cbGender=m_AccountsDBAide.GetValue_BYTE(TEXT("Gender"));
			LogonSuccess.cbMember=m_AccountsDBAide.GetValue_BYTE(TEXT("MemberOrder"));
			LogonSuccess.dwExperience=m_AccountsDBAide.GetValue_DWORD(TEXT("Experience"));
			LogonSuccess.dwCustomFaceVer=m_AccountsDBAide.GetValue_DWORD(TEXT("CustomFaceVer"));
			LogonSuccess.JiQi=m_AccountsDBAide.GetValue_DWORD(TEXT("MoorMachine"));//��������
			m_AccountsDBAide.GetValue_String(TEXT("Accounts"),LogonSuccess.szAccounts,CountArray(LogonSuccess.szAccounts));
			m_AccountsDBAide.GetValue_String(TEXT("UnderWrite"),LogonSuccess.szUnderWrite,CountArray(LogonSuccess.szUnderWrite));
			m_AccountsDBAide.GetValue_String(TEXT("ErrorDescribe"),LogonSuccess.szDescribeString,CountArray(LogonSuccess.szDescribeString));
            //m_AccountsDBAide.GetValue_String(TEXT("NickName"),LogonSuccess.szGroupName,CountArray(LogonSuccess.szGroupName));
            m_AccountsDBAide.GetValue_String(TEXT("BankPass"),LogonSuccess.szBankPassword,CountArray(LogonSuccess.szBankPassword));  //�޸�
			
			//��������
			CopyMemory(LogonSuccess.szPassword,pszPassword,sizeof(LogonSuccess.szPassword));

			//Ͷ�ݽ��
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_LOGON_SUCCESS,dwContextID,&LogonSuccess,sizeof(LogonSuccess));
		}
		else
		{
			//��¼ʧ��
			DBR_GP_LogonError LogonError;
			ZeroMemory(&LogonError,sizeof(LogonError));

			//���ñ���
			LogonError.lErrorCode=lReturnValue;
			m_AccountsDBAide.GetValue_String(TEXT("ErrorDescribe"),LogonError.szErrorDescribe,CountArray(LogonError.szErrorDescribe));

			//Ͷ�ݽ��
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_LOGON_ERROR,dwContextID,&LogonError,sizeof(LogonError));
		}
	}
	catch (IDataBaseException * pIException)
	{
		//��¼ʧ��
		DBR_GP_LogonError LogonError;
		ZeroMemory(&LogonError,sizeof(LogonError));

		//�������
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);

		//����ʧ��
		LogonError.lErrorCode=-1;
		lstrcpyn(LogonError.szErrorDescribe,TEXT("�������ݿ�����쳣�������Ժ����Ի�ѡ����һ��������¼��"),CountArray(LogonError.szErrorDescribe));

		//Ͷ�ݽ��
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_LOGON_ERROR,dwContextID,&LogonError,sizeof(LogonError));
	}

	//�رռ�¼��
	m_AccountsDBModule->CloseRecordset();

	return true;
}

//�ϴ�ͷ��
bool CDataBaseSink::OnRequestUploadFace(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//������֤
	ASSERT(wDataSize==sizeof(DBR_GP_UploadCustomFace));
	if (wDataSize!=sizeof(DBR_GP_UploadCustomFace)) return false;

	//����ת��
	DBR_GP_UploadCustomFace *pUploadCustomFace = (DBR_GP_UploadCustomFace*)pData;

	//�ļ�����
	CFile fileCustomFace;
	TCHAR szFileName[128];
	_snprintf(szFileName, CountArray(szFileName), TEXT("%s\\UploadFile_%ld.zip"), theApp.m_szDirWork, pUploadCustomFace->dwUserID);

	try
	{
		if ( fileCustomFace.Open( szFileName, CFile::modeRead|CFile::typeBinary) )
		{
			BYTE cbCustomFaceData[1024*30];

			//��ȡ�ļ�
			UINT uReadSize = fileCustomFace.Read(cbCustomFaceData, CountArray(cbCustomFaceData));

			//�ر��ļ�
			fileCustomFace.Close();

			LONG lRet=-1;
			try
			{
				lRet = SPUploadFace(pUploadCustomFace->dwUserID, cbCustomFaceData, uReadSize);
			}
			catch (IDataBaseException * pIException)
			{
				//������Ϣ
				LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
				CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
			}

			//�ɹ��ж�
			if ( lRet != -1 )
			{
				DBR_GP_UploadFaceResult UploadFaceResult;
				ZeroMemory(&UploadFaceResult, sizeof(UploadFaceResult));
				lstrcpyn(UploadFaceResult.szDescribeMsg, TEXT("ͷ���ϴ��ɹ���"), sizeof(UploadFaceResult.szDescribeMsg));
				UploadFaceResult.dwFaceVer=lRet;
				UploadFaceResult.bOperateSuccess=true;

				m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_UPLOAD_FACE_RESULT,dwContextID,
					&UploadFaceResult,sizeof(UploadFaceResult));
			}
			else
			{
				//����ʧ��
				DBR_GP_UploadFaceResult UploadFaceResult;
				ZeroMemory(&UploadFaceResult, sizeof(UploadFaceResult));
				lstrcpyn(UploadFaceResult.szDescribeMsg, TEXT("�������ݿ�����쳣���������ϴ���"), sizeof(UploadFaceResult.szDescribeMsg));
				UploadFaceResult.bOperateSuccess=false;

				m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_UPLOAD_FACE_RESULT,dwContextID,
					&UploadFaceResult,sizeof(UploadFaceResult));
			}

			//ɾ���ļ�
			CFile::Remove(szFileName);
		}
	}catch(...){}	

	return true;
}
///////////
//���ʺ�
bool CDataBaseSink::OnRequestSafeBind(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//����У��
	ASSERT(wDataSize == sizeof(DBR_GP_SafeBind) );
	if ( wDataSize != sizeof(DBR_GP_SafeBind) ) return false;

	try
	{
		DBR_GP_SafeBind *pSafeBind = (DBR_GP_SafeBind*)pData;

		
		//ִ�д洢����
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pSafeBind->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineSerial"),pSafeBind->szComputerID);
		m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pSafeBind->szPassWord);		

		LONG lReturnValue=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_SafeBind"),true);
        
		if(lReturnValue != 0)
		{
			TCHAR szError[128];
			m_AccountsDBAide.GetValue_String(TEXT("ErrorDescribe"),szError,CountArray(szError));
		}
		DBR_GP_BindResult BindResult={0};
		BindResult.cbSuccess=BYTE(lReturnValue);
        
		//Ͷ����Ϣ
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_SAFE_BIND, dwContextID, &BindResult, sizeof(BindResult));

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}
	return true;
}

//�����
bool CDataBaseSink::OnRequestSafeUnbind(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
		//����У��
	ASSERT(wDataSize == sizeof(DBR_GP_SafeUnbind) );
	if ( wDataSize != sizeof(DBR_GP_SafeUnbind) ) return false;

	try
	{
		DBR_GP_SafeUnbind *pSafeUnbind = (DBR_GP_SafeUnbind*)pData;
        
		//ִ�д洢����
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pSafeUnbind->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pSafeUnbind->szPassWord);
		
		LONG lReturnValue=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_SafeUnBind"),false);

		DBR_GP_BindResult BindResult={0};
		BindResult.cbSuccess=BYTE(lReturnValue);

		//Ͷ����Ϣ
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_SAFE_UNBIND, dwContextID, &BindResult, sizeof(BindResult));
       

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}
	return true;
}
///////////
//����ͷ��
bool CDataBaseSink::OnRequestDownloadFace(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//У�����
	ASSERT(wDataSize == sizeof(DBR_GP_DownloadCustomFace));
	if ( wDataSize != sizeof(DBR_GP_DownloadCustomFace)) return false;

	//����ת��
	DBR_GP_DownloadCustomFace *pDownloadCustomFace = (DBR_GP_DownloadCustomFace*)pData;

	LONG lReturnValue=-1;
	long lSize=0;
	bool bRet = false;
	try
	{
		lReturnValue=SPDownloadFace(pDownloadCustomFace->dwUserID);

		//��ȡ����
		if ( lReturnValue==0 )
		{	
			DWORD dwImageSize=0;
			dwImageSize=m_AccountsDBAide.GetValue_DWORD(TEXT("ImgSize"));
			
			//��ȡ����
			IDataBase * pIDataBase=m_AccountsDBModule.GetInterface();
			_variant_t vtFld;
			pIDataBase->GetRecordsetValue(TEXT("CustomFaceImage"), vtFld);
			BYTE * pcbBuf=NULL;
			SafeArrayAccessData(vtFld.parray,(LPVOID *)&pcbBuf);

			//Ͷ������
			DBR_GP_DownloadFaceSuccess DownloadFaceSuccess;
			ZeroMemory(&DownloadFaceSuccess, sizeof(DownloadFaceSuccess));
			DownloadFaceSuccess.dwToltalSize = dwImageSize;
			DownloadFaceSuccess.dwUserID = pDownloadCustomFace->dwUserID;
			DWORD dwSendSize = 0;
			while( dwSendSize <dwImageSize )
			{
				//��������
				if ( 1024 < dwImageSize - dwSendSize ) DownloadFaceSuccess.dwCurrentSize = 1024;
				else DownloadFaceSuccess.dwCurrentSize = dwImageSize - dwSendSize ;
				CopyMemory(DownloadFaceSuccess.bFaceData, pcbBuf+dwSendSize, DownloadFaceSuccess.dwCurrentSize);

				//Ͷ����Ϣ
				WORD wPostSize = WORD(sizeof(DownloadFaceSuccess)-sizeof(DownloadFaceSuccess.bFaceData)+DownloadFaceSuccess.dwCurrentSize);
				m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_DOWNLOADFACE_SUCCESS, dwContextID, &DownloadFaceSuccess, wPostSize);

				//��������
				dwSendSize += DownloadFaceSuccess.dwCurrentSize;
			}

			//�ͷ���Դ
			SafeArrayUnaccessData(vtFld.parray);
		}
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}


	return true;
}

//ɾ����Ϣ
bool CDataBaseSink::OnRequestCustomFaceDelete(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//������֤
	ASSERT(wDataSize == sizeof(DBR_GP_CustomFaceDelete));
	if ( wDataSize != sizeof(DBR_GP_CustomFaceDelete)) return false;

	//����ת��
	DBR_GP_CustomFaceDelete *pCustomFaceDelete = (DBR_GP_CustomFaceDelete*)pData;

	//ִ�д洢����
	LONG lReturnValue = -1;
	try
	{
		lReturnValue = SPDeleteCustomFace(pCustomFaceDelete->dwUserID);
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}

	//�ɹ��ж�
	if ( lReturnValue == 0 )
	{
		DBR_GP_DeleteFaceResult DeleteFaceResult;
		ZeroMemory(&DeleteFaceResult, sizeof(DeleteFaceResult));
		lstrcpyn(DeleteFaceResult.szDescribeMsg, TEXT("ͷ��ɾ���ɹ���"), sizeof(DeleteFaceResult.szDescribeMsg));
		DeleteFaceResult.dwFaceVer=pCustomFaceDelete->dwFaceVer;
		DeleteFaceResult.bOperateSuccess = true;

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_DELETE_FACE_RESULT,dwContextID,
			&DeleteFaceResult,sizeof(DeleteFaceResult));
	}
	else
	{
		DBR_GP_DeleteFaceResult DeleteFaceResult;
		ZeroMemory(&DeleteFaceResult, sizeof(DeleteFaceResult));
		lstrcpyn(DeleteFaceResult.szDescribeMsg, TEXT("�������ݿ�����쳣��ɾ��ʧ�ܣ�"), sizeof(DeleteFaceResult.szDescribeMsg));
		DeleteFaceResult.dwFaceVer=pCustomFaceDelete->dwFaceVer;
		DeleteFaceResult.bOperateSuccess = false;

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_DELETE_FACE_RESULT,dwContextID,
			&DeleteFaceResult,sizeof(DeleteFaceResult));
	}

	return true;
}
//��������
bool CDataBaseSink::OnRequestLockJiQi(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
 ASSERT(wDataSize == sizeof(DBR_GP_DownLockJiQi));
	if ( wDataSize != sizeof(DBR_GP_DownLockJiQi)) return false;
 
	//����ת��
 DBR_GP_DownLockJiQi *DB_LockJiQi = (DBR_GP_DownLockJiQi*)pData;

 m_AccountsDBAide.ResetParameter();
 m_AccountsDBAide.AddParameter(TEXT("@dwGameID"),DB_LockJiQi->dwGameUserID);
 m_AccountsDBAide.AddParameter(TEXT("@dwMoorMachine"),DB_LockJiQi->dwLockLP);


  return m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_MoorMachineByGameID"),false);
}
//�޸�����
bool CDataBaseSink::OnRequestModifyIndividual(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//������֤
	ASSERT(wDataSize == sizeof(DBR_GP_ModifyIndividual));
	if ( wDataSize != sizeof(DBR_GP_ModifyIndividual)) return false;

	//����ת��
	DBR_GP_ModifyIndividual *pModifyIndividual = (DBR_GP_ModifyIndividual*)pData;

	//ִ�д洢����
	LONG lReturnValue = -1;
	try
	{
		lReturnValue = SPModifyIndividual(pModifyIndividual->dwUserID, pModifyIndividual->szNickname, pModifyIndividual->nGender,
			pModifyIndividual->nAge, pModifyIndividual->szAddress, pModifyIndividual->szUnderWrite, pModifyIndividual->szPassword);
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}

	//�ɹ��ж�
	if ( lReturnValue == 0 )
	{
		DBR_GP_ModifyIndividualResult ModifyIndividualResult;
		ZeroMemory(&ModifyIndividualResult, sizeof(ModifyIndividualResult));
		lstrcpyn(ModifyIndividualResult.szDescribeMsg, TEXT("�����޸ĳɹ���"), sizeof(ModifyIndividualResult.szDescribeMsg));
		ModifyIndividualResult.bOperateSuccess = true;

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_MODIFY_RESULT,dwContextID,
			&ModifyIndividualResult,sizeof(ModifyIndividualResult));
	}
	else
	{
		DBR_GP_ModifyIndividualResult ModifyIndividualResult;
		ZeroMemory(&ModifyIndividualResult, sizeof(ModifyIndividualResult));
		lstrcpyn(ModifyIndividualResult.szDescribeMsg, TEXT("�������ݿ�����쳣���޸�ʧ�ܣ�"), sizeof(ModifyIndividualResult.szDescribeMsg));
		ModifyIndividualResult.bOperateSuccess = false;

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_MODIFY_RESULT,dwContextID,
			&ModifyIndividualResult,sizeof(ModifyIndividualResult));
	}
	return true;
}

//�ʺŴ洢����
LONG CDataBaseSink::SPLogonByAccounts(LPCTSTR pszAccounts, LPCTSTR pszPassword, DWORD dwClientIP, LPCTSTR pszComputerID)
{
	//Ч�����
	ASSERT(pszAccounts!=NULL);
	ASSERT(pszPassword!=NULL);

	//ת����ַ
	TCHAR szClientIP[16]=TEXT("");
	BYTE * pClientIP=(BYTE *)&dwClientIP;
	_snprintf(szClientIP,sizeof(szClientIP),TEXT("%d.%d.%d.%d"),pClientIP[0],pClientIP[1],pClientIP[2],pClientIP[3]);

	//ִ�д洢����
	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@strAccounts"),pszAccounts);
	m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pszPassword);
	m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientIP);
	m_AccountsDBAide.AddParameter(TEXT("@strMachineSerial"),pszComputerID);

	return m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_EfficacyAccounts"),true);
}

//���´洢����
LONG CDataBaseSink::SPRegisterAccounts(LPCTSTR pszAccounts, LPCTSTR pszPassword, LPCTSTR pszSpreader, WORD wFaceID, BYTE cbGender, DWORD dwClientIP, LPCTSTR pszComputerID, LPCTSTR pszBankpass, LPCTSTR pszIdno, LPCTSTR pszEmail, LPCTSTR pszMobile)
{
	//Ч�����
	ASSERT(pszAccounts!=NULL);
	ASSERT(pszPassword!=NULL);

	//ת����ַ
	TCHAR szClientIP[16]=TEXT("");
	BYTE * pClientIP=(BYTE *)&dwClientIP;
	_snprintf(szClientIP,sizeof(szClientIP),TEXT("%d.%d.%d.%d"),pClientIP[0],pClientIP[1],pClientIP[2],pClientIP[3]);

	//ִ�д洢����
	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@strAccounts"),pszAccounts);
	m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pszPassword);
	m_AccountsDBAide.AddParameter(TEXT("@strSpreader"),pszSpreader);
	m_AccountsDBAide.AddParameter(TEXT("@nFaceID"),wFaceID);
	m_AccountsDBAide.AddParameter(TEXT("@cbGender"),cbGender);
	m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientIP);
	m_AccountsDBAide.AddParameter(TEXT("@strMachineSerial"),pszComputerID);
    m_AccountsDBAide.AddParameter(TEXT("@strBankpass"),pszBankpass);
	m_AccountsDBAide.AddParameter(TEXT("@strPassWordCode"),pszIdno);
	m_AccountsDBAide.AddParameter(TEXT("@strEmail"),pszEmail);
	m_AccountsDBAide.AddParameter(TEXT("@strMobile"),pszMobile);

	return m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_RegisterAccounts"),true);
}

//I D �洢����
LONG CDataBaseSink::SPLogonByUserID(DWORD dwGameID, LPCTSTR pszPassword, DWORD dwClientIP, LPCTSTR pszComputerID)
{
	//Ч�����
	ASSERT(dwGameID!=0L);
	ASSERT(pszPassword!=NULL);

	//ת����ַ
	TCHAR szClientIP[16]=TEXT("");
	BYTE * pClientIP=(BYTE *)&dwClientIP;
	_snprintf(szClientIP,sizeof(szClientIP),TEXT("%d.%d.%d.%d"),pClientIP[0],pClientIP[1],pClientIP[2],pClientIP[3]);

	//ִ�д洢����
	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@dwGameID"),dwGameID);
	m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pszPassword);
	m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientIP);
	m_AccountsDBAide.AddParameter(TEXT("@strMachineSerial"),pszComputerID);

	return m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_EfficacyGameID"),true);
}

//�ϴ��洢����
LONG CDataBaseSink::SPUploadFace(DWORD dwUserID, BYTE *bFaceData, DWORD dwFaceDataSize)
{
	//Ч�����
	ASSERT(dwUserID!=0L);
	ASSERT(dwFaceDataSize!=0L);
	ASSERT(bFaceData!=NULL);

	//Ч�����
	ASSERT(dwUserID!=0L);
	ASSERT(dwFaceDataSize!=0L);
	ASSERT(bFaceData!=NULL);

	//ִ�д洢����
	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),dwUserID);

	VARIANT varChunk;
	SAFEARRAY *psa=NULL;
	SAFEARRAYBOUND rgsabound[1];
	rgsabound[0].cElements = dwFaceDataSize;
    rgsabound[0].lLbound = 0;
    psa = SafeArrayCreate(VT_UI1,1,rgsabound);
    for (long index =0; index < long(dwFaceDataSize); index++ )
	{
		SafeArrayPutElement(psa,&index,&bFaceData[index]);
	}
    varChunk.vt = VT_ARRAY|VT_UI1;
    varChunk.parray = psa;

	IDataBase * pIDataBase=m_AccountsDBModule.GetInterface();
	pIDataBase->AddParameter(TEXT("@imgCustomFaceImage"),adLongVarBinary,adParamInput,dwFaceDataSize,_variant_t(varChunk));

	LONG lReturnValue=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_CF_InsertCustomFace"),false);

	//�ͷ���Դ
	SafeArrayDestroy(psa);

	return lReturnValue;
}

//���ش洢����
LONG CDataBaseSink::SPDownloadFace(DWORD dwUserID)
{
	//Ч�����
	ASSERT(dwUserID!=0L);

	//ִ�д洢����
	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),dwUserID);


	return m_AccountsDBAide.ExecuteProcess(TEXT("GSP_CF_LoadCustomFace"),true);
}

//ɾ���洢����
LONG CDataBaseSink::SPDeleteCustomFace(DWORD dwUserID)
{
	//Ч�����
	ASSERT(dwUserID!=0L);

	//ִ�д洢����
	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),dwUserID);

	return m_AccountsDBAide.ExecuteProcess(TEXT("GSP_CF_DeleteCustomFace"),true);
}

//�޸Ĵ洢����
LONG CDataBaseSink::SPModifyIndividual(DWORD dwUserID, TCHAR szNickname[NAME_LEN], int nGender, int nAge, TCHAR szAddress[64], TCHAR szUnderWrite[32], TCHAR szPassword[32])
{
	//ִ�д洢����
	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),dwUserID);
	m_AccountsDBAide.AddParameter(TEXT("@strLogonPass"),szPassword);
	m_AccountsDBAide.AddParameter(TEXT("@strNickName"),szNickname);
	m_AccountsDBAide.AddParameter(TEXT("@nGender"),nGender);
	m_AccountsDBAide.AddParameter(TEXT("@nAge"),nAge);
	m_AccountsDBAide.AddParameter(TEXT("@strAddress"),szAddress);
	m_AccountsDBAide.AddParameter(TEXT("@strUnderWrite"),szUnderWrite);

	return m_AccountsDBAide.ExecuteProcess(TEXT("GSP_CF_ModifyIndividual"),false);
}
//���ʺ�
bool CDataBaseSink::OnRequestOperMoney(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//����У��
	ASSERT(wDataSize == sizeof(DBR_GP_OperMoney) );
	if ( wDataSize != sizeof(DBR_GP_OperMoney) ) return false;

	try
	{
		DBR_GP_OperMoney *pOperMoney = (DBR_GP_OperMoney*)pData;

		if(pOperMoney->cbType==2)
		{
			////ִ�д洢����
			m_AccountsDBAide.ResetParameter();
			m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pOperMoney->dwUserID);
			m_AccountsDBAide.AddParameter(TEXT("@nType"),pOperMoney->cbType);
			m_AccountsDBAide.AddParameter(TEXT("@dwKindID"),pOperMoney->dwKindID);
			m_AccountsDBAide.AddParameter(TEXT("@lScore"),pOperMoney->lScore);	

			LONG lReturnValue=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_OPER_MONEY"),true);

			if(lReturnValue==0)
			{
				DBR_GP_MoneyRefreshResult MoneyOperResult;
				ZeroMemory(&MoneyOperResult, sizeof(MoneyOperResult));

				MoneyOperResult.lTotalScore = m_AccountsDBAide.GetValue_LONGLONG(TEXT("a"));
				MoneyOperResult.lRoomScore = m_AccountsDBAide.GetValue_LONGLONG(TEXT("b"));

				//Ͷ����Ϣ
				m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_REFRESHEMONEY_RESULT, dwContextID, &MoneyOperResult, sizeof(MoneyOperResult));
			}

		}
		else
		{

			////ִ�д洢����
			m_AccountsDBAide.ResetParameter();
			m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pOperMoney->dwUserID);
			m_AccountsDBAide.AddParameter(TEXT("@nType"),pOperMoney->cbType);
			m_AccountsDBAide.AddParameter(TEXT("@dwKindID"),pOperMoney->dwKindID);
			m_AccountsDBAide.AddParameter(TEXT("@lScore"),pOperMoney->lScore);	

			LONG lReturnValue=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_OPER_MONEY"),true);


			if(lReturnValue==0)
			{
				DBR_GP_MoneyOperResult MoneyOperResult;
				ZeroMemory(&MoneyOperResult, sizeof(MoneyOperResult));
				if(pOperMoney->cbType==0)
					lstrcpyn(MoneyOperResult.szDescribeMsg, TEXT("����ɹ���"), sizeof(MoneyOperResult.szDescribeMsg));
				else
					lstrcpyn(MoneyOperResult.szDescribeMsg, TEXT("��ȡ�ɹ���"), sizeof(MoneyOperResult.szDescribeMsg));
				MoneyOperResult.bOperateSuccess = true;

				//Ͷ����Ϣ
				m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_OPERMONEY_RESULT, dwContextID, &MoneyOperResult, sizeof(MoneyOperResult));
			}
			else
			{
				DBR_GP_MoneyOperResult MoneyOperResult;
				ZeroMemory(&MoneyOperResult, sizeof(MoneyOperResult));
				m_AccountsDBAide.GetValue_String(TEXT("ErrorDescribe"),MoneyOperResult.szDescribeMsg,CountArray(MoneyOperResult.szDescribeMsg));
				//lstrcpyn(MoneyOperResult.szDescribeMsg, TEXT("�����޸ĳɹ���"), sizeof(MoneyOperResult.szDescribeMsg));
				MoneyOperResult.bOperateSuccess = false;

				//Ͷ����Ϣ
				m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_OPERMONEY_RESULT, dwContextID, &MoneyOperResult, sizeof(MoneyOperResult));
			}
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);

		DBR_GP_MoneyOperResult MoneyOperResult;
		ZeroMemory(&MoneyOperResult, sizeof(MoneyOperResult));
		//m_AccountsDBAide.GetValue_String(TEXT("ErrorDescribe"),MoneyOperResult.szDescribeMsg,CountArray(MoneyOperResult.szDescribeMsg));
		lstrcpyn(MoneyOperResult.szDescribeMsg, TEXT("����ʧ�ܣ�����ϵ����Ա��"), sizeof(MoneyOperResult.szDescribeMsg));
		MoneyOperResult.bOperateSuccess = false;

		//Ͷ����Ϣ
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_OPERMONEY_RESULT, dwContextID, &MoneyOperResult, sizeof(MoneyOperResult));

	}
	return true;
}
//////////////////////////////////////////////////////////////////////////
//���ʺ�
bool CDataBaseSink::OnRequestGiveMoney(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//����У��
	ASSERT(wDataSize == sizeof(DBR_GP_GiveMoney) );
	if ( wDataSize != sizeof(DBR_GP_GiveMoney) ) return false;

	try
	{
		DBR_GP_GiveMoney *pOperMoney = (DBR_GP_GiveMoney*)pData;

		
		{
			TCHAR szClientIP[16]=TEXT("");
			BYTE * pClientIP=(BYTE *)&(pOperMoney->dwClientIP);
			_snprintf(szClientIP,sizeof(szClientIP),TEXT("%d.%d.%d.%d"),pClientIP[0],pClientIP[1],pClientIP[2],pClientIP[3]);
			////ִ�д洢����
			m_AccountsDBAide.ResetParameter();
			m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pOperMoney->dwUserID);
			m_AccountsDBAide.AddParameter(TEXT("@nType"),pOperMoney->cbType);
			m_AccountsDBAide.AddParameter(TEXT("@strNickName"),pOperMoney->szNickname);
			m_AccountsDBAide.AddParameter(TEXT("@wGameID"),atoi(pOperMoney->szNickname));
			m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientIP);
			m_AccountsDBAide.AddParameter(TEXT("@bQueryInfo"),pOperMoney->cbQueryInfo);	
			m_AccountsDBAide.AddParameter(TEXT("@lScore"),pOperMoney->lScore);	
			m_AccountsDBAide.AddParameter(TEXT("@lRevenue"), pOperMoney->lRevenue);

			LONG lReturnValue=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GIVE_MONEY"),true);
	       
			if (lReturnValue==10)
			{
				DBR_GP_GiveMoneyResult MoneyOperResult;
				ZeroMemory(&MoneyOperResult, sizeof(MoneyOperResult));
				//lstrcpyn(MoneyOperResult.szDescribeMsg, TEXT("���ͳɹ���"), sizeof(MoneyOperResult.szDescribeMsg));
				m_AccountsDBAide.GetValue_String(TEXT("TargetAccount"),MoneyOperResult.szTargeAccount,CountArray(MoneyOperResult.szTargeAccount));
// 				CString str;
// 				DWORD dwuserid =  m_AccountsDBAide.GetValue_DWORD(TEXT("TargetGameID"));
// 				str.Format("userId%d", dwuserid);
// 				AfxMessageBox(str);
				MoneyOperResult.dwTargetUserId = m_AccountsDBAide.GetValue_DWORD(TEXT("TargetGameID"));
				//MoneyOperResult.lRevenue= m_AccountsDBAide.GetValue_LONGLONG(TEXT(""));
				MoneyOperResult.lSwapScore= m_AccountsDBAide.GetValue_LONGLONG(TEXT("SwapScore"));
				MoneyOperResult.bOperateSuccess = true;
				MoneyOperResult.bQueryInfo = true;
				//Ͷ����Ϣ
				m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_GIVEMONEY_RESULT, dwContextID, &MoneyOperResult, sizeof(MoneyOperResult));

			}
			else if(lReturnValue==0)
			{
					DBR_GP_GiveMoneyResult MoneyOperResult;
					ZeroMemory(&MoneyOperResult, sizeof(MoneyOperResult));
					lstrcpyn(MoneyOperResult.szDescribeMsg, TEXT("���ͳɹ���"), sizeof(MoneyOperResult.szDescribeMsg));
					MoneyOperResult.bOperateSuccess = true;
			        
					//Ͷ����Ϣ
					m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_GIVEMONEY_RESULT, dwContextID, &MoneyOperResult, sizeof(MoneyOperResult));
			}else
			{
					DBR_GP_GiveMoneyResult MoneyOperResult;
					ZeroMemory(&MoneyOperResult, sizeof(MoneyOperResult));
					m_AccountsDBAide.GetValue_String(TEXT("ErrorDescribe"),MoneyOperResult.szDescribeMsg,CountArray(MoneyOperResult.szDescribeMsg));
					//lstrcpyn(MoneyOperResult.szDescribeMsg, TEXT("�����޸ĳɹ���"), sizeof(MoneyOperResult.szDescribeMsg));
					MoneyOperResult.bOperateSuccess = false;
			        
					//Ͷ����Ϣ
					m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_GIVEMONEY_RESULT, dwContextID, &MoneyOperResult, sizeof(MoneyOperResult));
				
			}
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);

		DBR_GP_GiveMoneyResult MoneyOperResult;
		ZeroMemory(&MoneyOperResult, sizeof(MoneyOperResult));
		//m_AccountsDBAide.GetValue_String(TEXT("ErrorDescribe"),MoneyOperResult.szDescribeMsg,CountArray(MoneyOperResult.szDescribeMsg));
		lstrcpyn(MoneyOperResult.szDescribeMsg, TEXT("����ʧ��"), sizeof(MoneyOperResult.szDescribeMsg));
		MoneyOperResult.bOperateSuccess = false;
			        
					//Ͷ����Ϣ
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_GIVEMONEY_RESULT, dwContextID, &MoneyOperResult, sizeof(MoneyOperResult));
	}
	return true;
}


//���ʺ�
bool CDataBaseSink::OnRequestBankPass(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//����У��
	ASSERT(wDataSize == sizeof(DBR_GP_BankPass) );
	if ( wDataSize != sizeof(DBR_GP_BankPass) ) return false;

	try
	{
		DBR_GP_BankPass *pBankPass = (DBR_GP_BankPass*)pData;

		
		{
		
			////ִ�д洢����
			m_AccountsDBAide.ResetParameter();
			m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pBankPass->dwUserID);
			m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pBankPass->szPassword);
			m_AccountsDBAide.AddParameter(TEXT("@strPassword1"),pBankPass->szPassword1);
			
			LONG lReturnValue=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_BANK_PASS"),true);
	       
			
			if(lReturnValue==0)
				{
					DBR_GP_BankPassResult MoneyOperResult;
					ZeroMemory(&MoneyOperResult, sizeof(MoneyOperResult));
					lstrcpyn(MoneyOperResult.szDescribeMsg, TEXT("����ɹ���"), sizeof(MoneyOperResult.szDescribeMsg));
					MoneyOperResult.bOperateSuccess = true;
			        
					//Ͷ����Ϣ
					m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_BANKPASS_RESULT, dwContextID, &MoneyOperResult, sizeof(MoneyOperResult));
				}
			else
				{
					DBR_GP_BankPassResult MoneyOperResult;
					ZeroMemory(&MoneyOperResult, sizeof(MoneyOperResult));
					m_AccountsDBAide.GetValue_String(TEXT("ErrorDescribe"),MoneyOperResult.szDescribeMsg,CountArray(MoneyOperResult.szDescribeMsg));
					//lstrcpyn(MoneyOperResult.szDescribeMsg, TEXT("�����޸ĳɹ���"), sizeof(MoneyOperResult.szDescribeMsg));
					MoneyOperResult.bOperateSuccess = false;
			        
					//Ͷ����Ϣ
					m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_BANKPASS_RESULT, dwContextID, &MoneyOperResult, sizeof(MoneyOperResult));
				
				}
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}
	return true;
}

//���ʺ�
bool CDataBaseSink::OnRequestGiveRecord(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//����У��
	ASSERT(wDataSize == sizeof(DBR_GP_GiveRecord) );
	if ( wDataSize != sizeof(DBR_GP_GiveRecord) ) return false;

	try
	{
		DBR_GP_GiveRecord *pGiveRecord = (DBR_GP_GiveRecord*)pData;

		
		{
		
			////ִ�д洢����
			m_AccountsDBAide.ResetParameter();
			m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pGiveRecord->dwUserID);
			
			LONG lReturnValue=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GIVE_RECORD"),true);
	       
			
			if(lReturnValue==0)
				{
					DBR_GP_GiveRecordResult MoneyOperResult;
					ZeroMemory(&MoneyOperResult, sizeof(MoneyOperResult));
				//	lstrcpyn(MoneyOperResult.szDescribeMsg, TEXT("����ɹ���"), sizeof(MoneyOperResult.szDescribeMsg));
				//	MoneyOperResult.bOperateSuccess = true;
					
					int num=0;
				
					//��ȡ��¼
					if (m_AccountsDBModule->IsRecordsetEnd()==false) 
					{
						m_AccountsDBModule->MoveToFirst();
						while ( m_AccountsDBModule->IsRecordsetEnd() == false )
						{
							
							MoneyOperResult.GiveRecord[num].dwRecordID=m_AccountsDBAide.GetValue_DWORD(TEXT("RecordID"));
							m_AccountsDBAide.GetValue_String(TEXT("SourceUserName"),MoneyOperResult.GiveRecord[num].szGiveName,CountArray(MoneyOperResult.GiveRecord[num].szGiveName));
							m_AccountsDBAide.GetValue_String(TEXT("TargetUserName"),MoneyOperResult.GiveRecord[num].szTargetUserName,CountArray(MoneyOperResult.GiveRecord[num].szTargetUserName));
							MoneyOperResult.GiveRecord[num].lGiveScore=m_AccountsDBAide.GetValue_LONGLONG(TEXT("SwapScore"));
							MoneyOperResult.GiveRecord[num].cbType=m_AccountsDBAide.GetValue_LONG(TEXT("TradeType"));
							SYSTEMTIME m_time;
							m_AccountsDBAide.GetValue_SystemTime(TEXT("CollectDate"),m_time);
							
							CString strTime;
							strTime.Format("%d-%d-%d %02d:%02d:%02d",m_time.wYear,m_time.wMonth,m_time.wDay,m_time.wHour,m_time.wMinute,m_time.wSecond);
							lstrcpy(MoneyOperResult.GiveRecord[num].szTime,strTime.GetBuffer());
							num++;
							MoneyOperResult.nCount = num;
						
							//�ƶ���¼
							m_AccountsDBModule->MoveToNext();
						}
					}

			        MoneyOperResult.bOperateSuccess = true;
					//Ͷ����Ϣ
					m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_GIVERECORD_RESULT, dwContextID, &MoneyOperResult, sizeof(MoneyOperResult));
				}
			else
				{
					DBR_GP_GiveRecordResult MoneyOperResult;
					ZeroMemory(&MoneyOperResult, sizeof(MoneyOperResult));
					m_AccountsDBAide.GetValue_String(TEXT("ErrorDescribe"),MoneyOperResult.szDescribeMsg,CountArray(MoneyOperResult.szDescribeMsg));
					//lstrcpyn(MoneyOperResult.szDescribeMsg, TEXT("�����޸ĳɹ���"), sizeof(MoneyOperResult.szDescribeMsg));
					MoneyOperResult.bOperateSuccess = false;
			        
					//Ͷ����Ϣ
					m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_GIVERECORD_RESULT, dwContextID, &MoneyOperResult, sizeof(MoneyOperResult));
				
				}
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);

			DBR_GP_GiveRecordResult MoneyOperResult;
			ZeroMemory(&MoneyOperResult, sizeof(MoneyOperResult));
		//	m_AccountsDBAide.GetValue_String(TEXT("ErrorDescribe"),MoneyOperResult.szDescribeMsg,CountArray(MoneyOperResult.szDescribeMsg));
			lstrcpyn(MoneyOperResult.szDescribeMsg, TEXT("��ȡʧ�ܣ�"), sizeof(MoneyOperResult.szDescribeMsg));
			MoneyOperResult.bOperateSuccess = false;
			        
					//Ͷ����Ϣ
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_GIVERECORD_RESULT, dwContextID, &MoneyOperResult, sizeof(MoneyOperResult));
	}
	return true;
}

//���ʺ�
bool CDataBaseSink::OnRequestUserPass(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//����У��
	ASSERT(wDataSize == sizeof(DBR_GP_BankPass) );
	if ( wDataSize != sizeof(DBR_GP_BankPass) ) return false;

	try
	{
		DBR_GP_BankPass *pUserPass = (DBR_GP_BankPass*)pData;

		
		{
		
			////ִ�д洢����
			m_AccountsDBAide.ResetParameter();
			m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pUserPass->dwUserID);
			m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pUserPass->szPassword);
			m_AccountsDBAide.AddParameter(TEXT("@strPassword1"),pUserPass->szPassword1);
			
			LONG lReturnValue=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_USER_PASS"),true);
	       
			
			if(lReturnValue==0)
				{
					DBR_GP_ModifyIndividualResult OperResult;
					ZeroMemory(&OperResult, sizeof(OperResult));
					lstrcpyn(OperResult.szDescribeMsg, TEXT("����ɹ���"), sizeof(OperResult.szDescribeMsg));
					OperResult.bOperateSuccess = true;
			        
					//Ͷ����Ϣ
					m_pIDataBaseEngineEvent->OnEventDataBaseResult(SUB_GP_MODIFY_INDIVIDUAL_RESULT, dwContextID, &OperResult, sizeof(OperResult));
				}
			else
				{
					DBR_GP_ModifyIndividualResult OperResult;
					ZeroMemory(&OperResult, sizeof(OperResult));
					m_AccountsDBAide.GetValue_String(TEXT("ErrorDescribe"),OperResult.szDescribeMsg,CountArray(OperResult.szDescribeMsg));
					//lstrcpyn(MoneyOperResult.szDescribeMsg, TEXT("�����޸ĳɹ���"), sizeof(MoneyOperResult.szDescribeMsg));
					OperResult.bOperateSuccess = false;
			        
					//Ͷ����Ϣ
					m_pIDataBaseEngineEvent->OnEventDataBaseResult(SUB_GP_MODIFY_INDIVIDUAL_RESULT, dwContextID, &OperResult, sizeof(OperResult));
				
				}
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}
	return true;
}

//���ʺ�
bool CDataBaseSink::OnRequestUserWrite(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//����У��
	ASSERT(wDataSize == sizeof(DBR_GP_UserWrite) );
	if ( wDataSize != sizeof(DBR_GP_UserWrite) ) return false;

	try
	{
		DBR_GP_UserWrite *pUserWrite = (DBR_GP_UserWrite*)pData;
		{
		
			////ִ�д洢����
			m_AccountsDBAide.ResetParameter();
			m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pUserWrite->dwUserID);
			m_AccountsDBAide.AddParameter(TEXT("@szUnderWrite"),pUserWrite->szUnderWrite);
			
			
			LONG lReturnValue=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_USER_UNDERWRITE"),true);
	       
			
			if(lReturnValue==0)
				{
					DBR_GP_ModifyIndividualResult OperResult;
					ZeroMemory(&OperResult, sizeof(OperResult));
					lstrcpyn(OperResult.szDescribeMsg, TEXT("����ɹ���"), sizeof(OperResult.szDescribeMsg));
					OperResult.bOperateSuccess = true;
			        
					//Ͷ����Ϣ
					m_pIDataBaseEngineEvent->OnEventDataBaseResult(SUB_GP_MODIFY_INDIVIDUAL_RESULT, dwContextID, &OperResult, sizeof(OperResult));
				}
			else
				{
					DBR_GP_ModifyIndividualResult OperResult;
					ZeroMemory(&OperResult, sizeof(OperResult));
					m_AccountsDBAide.GetValue_String(TEXT("ErrorDescribe"),OperResult.szDescribeMsg,CountArray(OperResult.szDescribeMsg));
					//lstrcpyn(MoneyOperResult.szDescribeMsg, TEXT("�����޸ĳɹ���"), sizeof(MoneyOperResult.szDescribeMsg));
					OperResult.bOperateSuccess = false;
			        
					//Ͷ����Ϣ
					m_pIDataBaseEngineEvent->OnEventDataBaseResult(SUB_GP_MODIFY_INDIVIDUAL_RESULT, dwContextID, &OperResult, sizeof(OperResult));
				
				}
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}
	return true;
}

//���ʺ�
bool CDataBaseSink::OnRequestUserInfo(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//����У��
	ASSERT(wDataSize == sizeof(DBR_GP_ModifyUserInfo) );
	if ( wDataSize != sizeof(DBR_GP_ModifyUserInfo) ) return false;

	try
	{
		DBR_GP_ModifyUserInfo *pUserInfo = (DBR_GP_ModifyUserInfo*)pData;
		{
			////ִ�д洢����
			m_AccountsDBAide.ResetParameter();
			m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pUserInfo->dwUserID);
			m_AccountsDBAide.AddParameter(TEXT("@szAddress"),pUserInfo->szAddress);
			m_AccountsDBAide.AddParameter(TEXT("@szSchool"),pUserInfo->szSchool);
			m_AccountsDBAide.AddParameter(TEXT("@szEmail"),pUserInfo->szEmail);
			m_AccountsDBAide.AddParameter(TEXT("@szUrl"),pUserInfo->szUrl);
			m_AccountsDBAide.AddParameter(TEXT("@szMsn"),pUserInfo->szMsn);
			m_AccountsDBAide.AddParameter(TEXT("@szTel"),pUserInfo->szTel);
			
			
			LONG lReturnValue=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_USER_INFO"),true);
	       
			
			if(lReturnValue==0)
				{
					DBR_GP_ModifyIndividualResult OperResult;
					ZeroMemory(&OperResult, sizeof(OperResult));
					lstrcpyn(OperResult.szDescribeMsg, TEXT("����ɹ���"), sizeof(OperResult.szDescribeMsg));
					OperResult.bOperateSuccess = true;
			        
					//Ͷ����Ϣ
					m_pIDataBaseEngineEvent->OnEventDataBaseResult(SUB_GP_MODIFY_INDIVIDUAL_RESULT, dwContextID, &OperResult, sizeof(OperResult));
				}
			else
				{
					DBR_GP_ModifyIndividualResult OperResult;
					ZeroMemory(&OperResult, sizeof(OperResult));
					m_AccountsDBAide.GetValue_String(TEXT("ErrorDescribe"),OperResult.szDescribeMsg,CountArray(OperResult.szDescribeMsg));
					//lstrcpyn(MoneyOperResult.szDescribeMsg, TEXT("�����޸ĳɹ���"), sizeof(MoneyOperResult.szDescribeMsg));
					OperResult.bOperateSuccess = false;
			        
					//Ͷ����Ϣ
					m_pIDataBaseEngineEvent->OnEventDataBaseResult(SUB_GP_MODIFY_INDIVIDUAL_RESULT, dwContextID, &OperResult, sizeof(OperResult));
				
				}
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}
	return true;
}
