#include "Stdafx.h"
#include "AndroidUserItemSink.h"

//////////////////////////////////////////////////////////////////////////

//����ʱ��
#define TIME_LESS					2									//����ʱ��

//��Ϸʱ��
#define TIME_USER_CALL_BANKER		2									//��ׯ��ʱ��
#define TIME_USER_START_GAME		3									//��ʼ��ʱ��
#define TIME_USER_ADD_SCORE			3									//��ע��ʱ��
#define TIME_USER_OPEN_CARD			12									//̯�ƶ�ʱ��

//��Ϸʱ��
#define IDI_START_GAME			(IDI_ANDROID_ITEM_SINK+0)			//��ʼʱ��
#define IDI_CALL_BANKER			(IDI_ANDROID_ITEM_SINK+1)			//��ׯʱ��
#define IDI_USER_ADD_SCORE	(IDI_ANDROID_ITEM_SINK+2)			//��עʱ��
#define IDI_OPEN_CARD		(IDI_ANDROID_ITEM_SINK+3)			//����ʱ��
#define IDI_USER_LEFT		(IDI_ANDROID_ITEM_SINK+4)			//�û��Ƴ�


//////////////////////////////////////////////////////////////////////////

//���캯��
CAndroidUserItemSink::CAndroidUserItemSink()
{
	m_lTurnMaxScore = 0;
	ZeroMemory(m_HandCardData,sizeof(m_HandCardData));

	//�ӿڱ���
	m_pIAndroidUserItem=NULL;

	return;
}

//��������
CAndroidUserItemSink::~CAndroidUserItemSink()
{
}

//�ӿڲ�ѯ
void * __cdecl CAndroidUserItemSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IAndroidUserItemSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IAndroidUserItemSink,Guid,dwQueryVer);
	return NULL;
}

//��ʼ�ӿ�
bool __cdecl CAndroidUserItemSink::InitUserItemSink(IUnknownEx * pIUnknownEx)
{
	//��ѯ�ӿ�
	m_pIAndroidUserItem=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IAndroidUserItem);
	if (m_pIAndroidUserItem==NULL) return false;

	return true;
}

//���ýӿ�
bool __cdecl CAndroidUserItemSink::RepositUserItemSink()
{
	m_lTurnMaxScore = 0;
	ZeroMemory(m_HandCardData,sizeof(m_HandCardData));

	return true;
}

//ʱ����Ϣ
bool __cdecl CAndroidUserItemSink::OnEventTimer(UINT nTimerID)
{
	switch (nTimerID)
	{
	case IDI_USER_LEFT:
			m_pIAndroidUserItem->SendMainUser(SUB_GR_USER_LEFT_GAME_REQ, NULL, 0);
			return true;
	case IDI_START_GAME:		//��ʼ��ʱ��
		{

			//����׼��
			m_pIAndroidUserItem->SendUserReady(NULL,0);

			return true;
		}
	case IDI_CALL_BANKER:		//��ׯ��ʱ
		{
			//���ñ���
			CMD_C_CallBanker CallBanker;
			CallBanker.bBanker = rand()%2==0?0:1;

			//������Ϣ
			m_pIAndroidUserItem->SendSocketData(SUB_C_CALL_BANKER,&CallBanker,sizeof(CallBanker));

			return true;
		}
	case IDI_USER_ADD_SCORE:	//��ע��ʱ
		{
			//����ע����
			LONGLONG lUserMaxScore[GAME_PLAYER];
			ZeroMemory(lUserMaxScore,sizeof(lUserMaxScore));
			LONGLONG lTemp=m_lTurnMaxScore;
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				if(i>0)lTemp/=2;
				lUserMaxScore[i]=__max(lTemp,1L);
			}

			////������Ϣ
			//CMD_C_AddScore AddScore;
			//
			//int countinfor = rand()%100;
			//if (countinfor > 100)
			//{
			//	AddScore.lScore=lUserMaxScore[0];
			//}
			//else if (countinfor > 85)
			//{
			//	AddScore.lScore=lUserMaxScore[1];
			//}			
			////else if (countinfor > 60)
			////{
			////	AddScore.lScore=lUserMaxScore[2];
			////}
			////else
			////{
			////	AddScore.lScore=lUserMaxScore[3];
			////}

			//m_pIAndroidUserItem->SendSocketData(SUB_C_ADD_SCORE,&AddScore,sizeof(AddScore));
						//������Ϣ
			CMD_C_AddScore AddScore;
			AddScore.lScore=lUserMaxScore[rand()%2];
			m_pIAndroidUserItem->SendSocketData(SUB_C_ADD_SCORE,&AddScore,sizeof(AddScore));

			return true;	
		}
	case IDI_OPEN_CARD:			//���ƶ�ʱ
		{
			//������Ϣ
		   COLORREF Color;
			CMD_C_OxCard OxCard;
			OxCard.bOX=(m_GameLogic.GetCardType(m_HandCardData,MAX_COUNT)>0)?TRUE:FALSE;
			m_pIAndroidUserItem->SendSocketData(SUB_C_OPEN_CARD,&OxCard,sizeof(OxCard));

			return true;	
		}

	}

	return false;
}

//��Ϸ��Ϣ
bool __cdecl CAndroidUserItemSink::OnEventGameMessage(WORD wSubCmdID, void * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_CALL_BANKER:	//�û���ׯ
		{
			//��Ϣ����
			return OnSubCallBanker(pData,wDataSize);
		}
	case SUB_S_GAME_START:	//��Ϸ��ʼ
		{
			//��Ϣ����
			return OnSubGameStart(pData,wDataSize);
		}
	case SUB_S_ADD_SCORE:	//�û���ע
		{
			//��Ϣ����
			return OnSubAddScore(pData,wDataSize);
		}
	case SUB_S_SEND_CARD:	//������Ϣ
		{
			//��Ϣ����
			return OnSubSendCard(pData,wDataSize);
		}
	case SUB_S_OPEN_CARD:	//�û�̯��
		{
			//��Ϣ����
			return OnSubOpenCard(pData,wDataSize);
		}
	case SUB_S_PLAYER_EXIT:	//�û�ǿ��
		{
			//��Ϣ����
			return OnSubPlayerExit(pData,wDataSize);
		}
	case SUB_S_GAME_END:	//��Ϸ����
		{
			//��Ϣ����
			return OnSubGameEnd(pData,wDataSize);
		}
	}

	//�������
	ASSERT(FALSE);

	return true;
}

//��Ϸ��Ϣ
bool __cdecl CAndroidUserItemSink::OnEventFrameMessage(WORD wSubCmdID, void * pData, WORD wDataSize)
{
	return true;
}

//������Ϣ
bool __cdecl CAndroidUserItemSink::OnEventGameScene(BYTE cbGameStatus, bool bLookonOther, void * pData, WORD wDataSize)
{
	switch (cbGameStatus)
	{
	case GS_FREE:		//����״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;
			//��Ϣ����
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pData;
			//////////////////////////////////////////////////////////////////////////
			memcpy(m_szRoomName, pStatusFree->szGameRoomName, sizeof(m_szRoomName));
			//lstrcpyn(m_szRoomName, "��ɱţţ����", 128);
			//////////////////////////////////////////////////////////////////////////
			ReadConfigInformation(m_szRoomName);

			m_iPlayCount = 0;

			//��ʼʱ��
			UINT nElapse=rand()%(2)+TIME_LESS;
			if(rand()%60==0)
				nElapse+=20;

			//��ȡ�Լ�״̬
			LONGLONG lUserScore = pStatusFree->lOtherScore;
			//////////////////////////////////////////////////////////////////////////
			//���˺ͻ��������̫�󣬾Ͳ���ʼ
			if (lUserScore > 0)
			{
				LONGLONG lAndroid=m_pIAndroidUserItem->GetMeUserItem()->GetUserScore()->lScore;
				WORD iSub = lAndroid/lUserScore;
				if(iSub > 10)
				{
					if(rand()%20!=0)
					{
						m_pIAndroidUserItem->SetGameTimer(IDI_USER_LEFT,nElapse+2);
						return true;
					}
					else
						m_iPlayCount = 3 + rand()%5;
				}
			}
			//////////////////////////////////////////////////////////////////////////
			m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME,nElapse);

			return true;
		}
	case GS_TK_CALL:	// ��ׯ״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusCall)) return false;
			CMD_S_StatusCall * pStatusCall=(CMD_S_StatusCall *)pData;

			//ʼ���û�
			if(pStatusCall->wCallBanker==m_pIAndroidUserItem->GetChairID())
			{
				//��ׯʱ��
				UINT nElapse=rand()%(2)+TIME_LESS;
				m_pIAndroidUserItem->SetGameTimer(IDI_CALL_BANKER,nElapse);
			}

			return true;
		}
	case GS_TK_SCORE:	//��ע״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusScore)) return false;
			CMD_S_StatusScore * pStatusScore=(CMD_S_StatusScore *)pData;

			//���ñ���
			m_lTurnMaxScore=pStatusScore->lTurnMaxScore;
			WORD wMeChairId = m_pIAndroidUserItem->GetChairID();

			//���ó���
			if (pStatusScore->lTurnMaxScore>0L && pStatusScore->lTableScore[wMeChairId]==0L)
			{
				//��עʱ��
				UINT nElapse=rand()%(3)+TIME_LESS;
				m_pIAndroidUserItem->SetGameTimer(IDI_USER_ADD_SCORE,nElapse);
			}

			return true;
		}
	case GS_TK_PLAYING:	//��Ϸ״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pData;

			//���ñ���
			m_lTurnMaxScore=pStatusPlay->lTurnMaxScore;
			WORD wMeChiarID=m_pIAndroidUserItem->GetChairID();
			CopyMemory(m_HandCardData,pStatusPlay->cbHandCardData[wMeChiarID],MAX_COUNT);

			//�ؼ�����
			if(pStatusPlay->bOxCard[wMeChiarID]==0xff)
			{
				//����ʱ��
				UINT nElapse=rand()%(2)+TIME_LESS;
				m_pIAndroidUserItem->SetGameTimer(IDI_OPEN_CARD,nElapse);
			}

			return true;
		}
	}

	ASSERT(FALSE);

	return false;
}

//�û�����
void __cdecl CAndroidUserItemSink::OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û��뿪
void __cdecl CAndroidUserItemSink::OnEventUserLeave(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û�����
void __cdecl CAndroidUserItemSink::OnEventUserScore(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û�״̬
void __cdecl CAndroidUserItemSink::OnEventUserStatus(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û���λ
void __cdecl CAndroidUserItemSink::OnEventUserSegment(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û���ׯ
bool CAndroidUserItemSink::OnSubCallBanker(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_CallBanker)) return false;
	CMD_S_CallBanker * pCallBanker=(CMD_S_CallBanker *)pBuffer;

	//ʼ���û�
	if(pCallBanker->wCallBanker==m_pIAndroidUserItem->GetChairID())
	{
		//��ׯʱ��
		UINT nElapse=rand()%(2)+TIME_LESS;
		m_pIAndroidUserItem->SetGameTimer(IDI_CALL_BANKER,nElapse);
	}

	return true;
}

//��Ϸ��ʼ
bool CAndroidUserItemSink::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

	//���ñ���
	m_lTurnMaxScore=pGameStart->lTurnMaxScore;

	//���ó���
	if (pGameStart->lTurnMaxScore>0)
	{
		//��עʱ��
		UINT nElapse=rand()%(2)+TIME_LESS;
		m_pIAndroidUserItem->SetGameTimer(IDI_USER_ADD_SCORE,nElapse);
	}

	return true;
}

//�û���ע
bool CAndroidUserItemSink::OnSubAddScore(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_AddScore)) return false;
	CMD_S_AddScore * pAddScore=(CMD_S_AddScore *)pBuffer;

	return true;
}

//������Ϣ
bool CAndroidUserItemSink::OnSubSendCard(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_SendCard)) return false;
	CMD_S_SendCard * pSendCard=(CMD_S_SendCard *)pBuffer;

	//��������
	WORD wMeChiarID=m_pIAndroidUserItem->GetChairID();
	CopyMemory(m_HandCardData,pSendCard->cbCardData[wMeChiarID],sizeof(m_HandCardData));

	//����ʱ��
	UINT nElapse=rand()%(2)+TIME_LESS;
	m_pIAndroidUserItem->SetGameTimer(IDI_OPEN_CARD,nElapse);

	return true;
}

//�û�̯��
bool CAndroidUserItemSink::OnSubOpenCard(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_Open_Card)) return false;
	CMD_S_Open_Card * pOpenCard=(CMD_S_Open_Card *)pBuffer;

	return true;
}

//�û�ǿ��
bool CAndroidUserItemSink::OnSubPlayerExit(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_PlayerExit)) return false;
	CMD_S_PlayerExit * pPlayerExit=(CMD_S_PlayerExit *)pBuffer;

	return true;
}

//��Ϸ����
bool CAndroidUserItemSink::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//Ч�����
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;

	//ɾ����ʱ��
	m_pIAndroidUserItem->KillGameTimer(IDI_CALL_BANKER);
	m_pIAndroidUserItem->KillGameTimer(IDI_USER_ADD_SCORE);
	m_pIAndroidUserItem->KillGameTimer(IDI_OPEN_CARD);
	srand(time(NULL)+m_pIAndroidUserItem->GetChairID());

	UINT nElapse=rand()%2+TIME_LESS;
	if(rand()%60 ==0)
		nElapse +=rand()%20;

	//��ȡ�Լ�״̬
	LONGLONG lUserScore =pGameEnd->lOtherScore;

	if(rand()%30 == 0)
	{
		m_pIAndroidUserItem->SetGameTimer(IDI_USER_LEFT,nElapse);
		return true;
	}

	if(m_iPlayCount -- > 0)
	{
		m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME,nElapse);
		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	//���˺ͻ��������̫�󣬾Ͳ���ʼ
	if (lUserScore > 0)
	{
		LONGLONG lAndroid=m_pIAndroidUserItem->GetMeUserItem()->GetUserScore()->lScore;
		WORD iSub = lAndroid/lUserScore;
		if(iSub > 10)
		{
			if(rand()%20!=0)
			{
				m_pIAndroidUserItem->SetGameTimer(IDI_USER_LEFT,nElapse+2);
				return true;
			}
			else
				m_iPlayCount = 3 + rand()%5;
		}
	}

	m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME,nElapse);

	//�������
	m_lTurnMaxScore = 0;
	ZeroMemory(m_HandCardData,sizeof(m_HandCardData));

	ReadConfigInformation(m_szRoomName);

	return true;
}

//��ȡ����
void CAndroidUserItemSink::ReadConfigInformation(TCHAR szRoomName[])
{
	//�����ļ���
	TCHAR szPath[MAX_PATH] = TEXT("");
	TCHAR szConfigFileName[MAX_PATH] = TEXT("");
	TCHAR OutBuf[255] = TEXT("");
	GetCurrentDirectory(sizeof(szPath), szPath);
	_snprintf(szConfigFileName, sizeof(szConfigFileName), _T("%s\\%s"), szPath, TEXT("OxConfig.ini"));


	//�����˴�ȡ����Ϣ
	ZeroMemory(OutBuf, sizeof(OutBuf));
	GetPrivateProfileString(szRoomName, TEXT("RobotMaxMoney"), _T("10000000"), OutBuf, 255, szConfigFileName);
	_snscanf(OutBuf, strlen(OutBuf), _T("%I64d"), &m_lRobotMaxMoney);

	ZeroMemory(OutBuf, sizeof(OutBuf));
	GetPrivateProfileString(szRoomName, TEXT("RobotMinMoney"), _T("100"), OutBuf, 255, szConfigFileName);
	_snscanf(OutBuf, strlen(OutBuf), _T("%I64d"), &m_lRobotMinMoney);

	ZeroMemory(OutBuf, sizeof(OutBuf));
	GetPrivateProfileString(szRoomName, TEXT("RobotStorageMoneyMin"), _T("100"), OutBuf, 255, szConfigFileName);
	_snscanf(OutBuf, strlen(OutBuf), _T("%I64d"), &m_lRobotStorageMoneyMin);

	ZeroMemory(OutBuf, sizeof(OutBuf));
	GetPrivateProfileString(szRoomName, TEXT("RobotStorageMoneyMax"), _T("100"), OutBuf, 255, szConfigFileName);
	_snscanf(OutBuf, strlen(OutBuf), _T("%I64d"), &m_lRobotStorageMoneyMax);

	ZeroMemory(OutBuf, sizeof(OutBuf));
	GetPrivateProfileString(szRoomName, TEXT("RobotGETMoneyMin"), _T("100"), OutBuf, 255, szConfigFileName);
	_snscanf(OutBuf, strlen(OutBuf), _T("%I64d"), &m_lRobotGETMoneyMin);

	ZeroMemory(OutBuf, sizeof(OutBuf));
	GetPrivateProfileString(szRoomName, TEXT("RobotGETMoneyMax"), _T("100"), OutBuf, 255, szConfigFileName);
	_snscanf(OutBuf, strlen(OutBuf), _T("%I64d"), &m_lRobotGETMoneyMax);

	ZeroMemory(OutBuf, sizeof(OutBuf));
	GetPrivateProfileString(szRoomName, TEXT("RobotStoreagePrecision"), _T("100000"), OutBuf, 255, szConfigFileName);
	_snscanf(OutBuf, strlen(OutBuf), _T("%I64d"), &m_lRobotStoragePrecision);
	ZeroMemory(OutBuf, sizeof(OutBuf));
	GetPrivateProfileString(szRoomName, TEXT("RobotGetPrecision"), _T("100000"), OutBuf, 255, szConfigFileName);
	_snscanf(OutBuf, strlen(OutBuf), _T("%I64d"), &m_lRobotGetPrecision);

	//MyDebug(_T("������ ��ȡ���� [%I64d %I64d %d %d] %d [%d %d %d] ��ע %d �½� %d"), m_lRobotJettonLimit[0], m_lRobotJettonLimit[1],
	//	m_nRobotBetTimeLimit, m_nRobotBetTimeLimit, m_bRobotBanker, m_nRobotBankerCount, m_nRobotWaitBanker, m_nRobotApplyBanker, m_bReduceJettonLimit);
}
//////////////////////////////////////////////////////////////////////////
