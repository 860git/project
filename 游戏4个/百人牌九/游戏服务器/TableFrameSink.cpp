#include "StdAfx.h"
#include "TableFrameSink.h"

//////////////////////////////////////////////////////////////////////////

//��������
#define SEND_COUNT					600									//���ʹ���

//��������
#define INDEX_PLAYER				0									//�м�����
#define INDEX_BANKER				1									//ׯ������

//��עʱ��
#define IDI_FREE					1									//����ʱ��
#ifdef _DEBUG
#define TIME_FREE					10									//����ʱ��
#else
#define TIME_FREE					10									//����ʱ��
#endif

//��עʱ��
#define IDI_PLACE_JETTON			2									//��עʱ��

#ifdef _DEBUG
#define TIME_PLACE_JETTON			10									//��עʱ��
#else
#define TIME_PLACE_JETTON			18									//��עʱ��
#endif

//����ʱ��
#define IDI_GAME_END				20									//����ʱ��

#ifdef _DEBUG
#define TIME_GAME_END				25									//����ʱ��
#else
#define TIME_GAME_END				25									//����ʱ��
#endif

//////////////////////////////////////////////////////////////////////////

//��̬����
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;				//��Ϸ����
const enStartMode	CTableFrameSink::m_GameStartMode=enStartMode_TimeControl;	//��ʼģʽ

//////////////////////////////////////////////////////////////////////////

//���캯��
CTableFrameSink::CTableFrameSink()
{
	//����ע��
	ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));
	
	//������ע
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));

	//���Ʊ���
	m_cbWinSideControl=0;
	m_nSendCardCount=0;

	//��ҳɼ�	
	ZeroMemory(m_lUserWinScore,sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore,sizeof(m_lUserReturnScore));
	ZeroMemory(m_lUserRevenue,sizeof(m_lUserRevenue));

	//�˿���Ϣ
	ZeroMemory(m_cbCardCount,sizeof(m_cbCardCount));
	ZeroMemory(m_cbTableCardArray,sizeof(m_cbTableCardArray));

	ZeroMemory(m_cbTableHavaSendCardArray,sizeof(m_cbTableHavaSendCardArray));
	ZeroMemory(m_cbTableHavaSendCount,sizeof(m_cbTableHavaSendCount));


	//״̬����
	m_dwJettonTime=0L;

	//ׯ����Ϣ
	m_ApplyUserArray.RemoveAll();
	m_wCurrentBanker=INVALID_CHAIR;
	m_wBankerTime=0;
	m_lBankerWinScore=0L;		
	m_lBankerCurGameScore=0L;
	m_bEnableSysBanker=true;
	m_cbLeftCardCount=0;
	m_bContiueCard=false;

	//��¼����
	ZeroMemory(m_GameRecordArrary,sizeof(m_GameRecordArrary));
	m_nRecordFirst=0;
	m_nRecordLast=0;
	m_dwRecordCount=0;

	//�����ļ���
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(sizeof(szPath),szPath);
	_snprintf(m_szConfigFileName,sizeof(m_szConfigFileName),TEXT("%s\\PaiJiuBattleConfig.ini"),szPath);

	//���Ʊ���
	LONGLONG lMaxScore=2147483647;
	m_lApplyBankerCondition=GetPrivateProfileInt(TEXT("BankerCondition"), TEXT("Score"), 100, m_szConfigFileName);
	m_lAreaLimitScore=GetPrivateProfileInt(TEXT("ScoreLimit"), TEXT("AreaLimitScore"), (int)lMaxScore, m_szConfigFileName);
	m_lUserLimitScore=GetPrivateProfileInt(TEXT("ScoreLimit"), TEXT("UserLimitScore"), (int)lMaxScore, m_szConfigFileName);
	BYTE cbEnableSysBanker=GetPrivateProfileInt(TEXT("BankerCondition"), TEXT("EnableSysBanker"), 1, m_szConfigFileName);
	m_bEnableSysBanker=cbEnableSysBanker?true:false;

	return;
}

//��������
CTableFrameSink::~CTableFrameSink(void)
{
}

//�ӿڲ�ѯ
void * __cdecl CTableFrameSink::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableFrameSink,Guid,dwQueryVer);
	QUERYINTERFACE(ITableUserAction,Guid,dwQueryVer);	
#ifdef __BANKER___
	QUERYINTERFACE(ITableUserActionEX,Guid,dwQueryVer);	
#endif
	QUERYINTERFACE_IUNKNOWNEX(ITableFrameSink,Guid,dwQueryVer);
	return NULL;
}

//��ʼ��
bool __cdecl CTableFrameSink::InitTableFrameSink(IUnknownEx * pIUnknownEx)
{
	//��ѯ�ӿ�
	ASSERT(pIUnknownEx!=NULL);
	m_pITableFrame=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrame);
	if (m_pITableFrame==NULL) return false;

	//���ƽӿ�
	m_pITableFrameControl=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrameControl);
	if (m_pITableFrameControl==NULL) return false;

	//��ȡ����
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	ASSERT(m_pGameServiceOption!=NULL);

	return true;
}

//��λ����
void __cdecl CTableFrameSink::RepositTableFrameSink()
{
	//����ע��
	ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));
	
	//������ע
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));

	//��ҳɼ�	
	ZeroMemory(m_lUserWinScore,sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore,sizeof(m_lUserReturnScore));
	ZeroMemory(m_lUserRevenue,sizeof(m_lUserRevenue));

	return;
}

//��ʼģʽ
enStartMode __cdecl CTableFrameSink::GetGameStartMode()
{
	return m_GameStartMode;
}

//��Ϸ״̬
bool __cdecl CTableFrameSink::IsUserPlaying(WORD wChairID)
{
	return true;
}

//��Ϸ��ʼ
bool __cdecl CTableFrameSink::OnEventGameStart()
{
	//��������
	CMD_S_GameStart GameStart;
	ZeroMemory(&GameStart,sizeof(GameStart));

	//��ȡׯ��
	IServerUserItem *pIBankerServerUserItem=NULL;
	if (INVALID_CHAIR!=m_wCurrentBanker) 
	{
		pIBankerServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
		m_lBankerScore=pIBankerServerUserItem->GetUserScore()->lScore;
	}

	//���ñ���
	GameStart.cbTimeLeave=TIME_PLACE_JETTON;
	GameStart.wBankerUser=m_wCurrentBanker;
	if (pIBankerServerUserItem!=NULL) GameStart.lBankerScore=m_lBankerScore;
	GameStart.bContiueCard=m_bContiueCard;

	GameStart.cbLeftCardCount=m_cbLeftCardCount+8;

	for (int i = 0;i<4;i++)
	{	
		CopyMemory(GameStart.cbTableHavaSendCardArray[i],m_cbTableHavaSendCardArray[i],CountArray(m_cbTableHavaSendCardArray[i]));
	}

	CopyMemory(GameStart.cbTableHavaSendCount,m_cbTableHavaSendCount,CountArray(m_cbTableHavaSendCount));

    //�Թ����
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));	

	//��Ϸ���
	for (WORD wChairID=0; wChairID<GAME_PLAYER; ++wChairID)
	{
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);
		if (pIServerUserItem==NULL) continue;

		//���û���
		GameStart.lUserMaxScore=min(pIServerUserItem->GetUserScore()->lScore,m_lUserLimitScore);

		m_pITableFrame->SendTableData(wChairID,SUB_S_GAME_START,&GameStart,sizeof(GameStart));	
	}

	return true;
}

//��Ϸ����
bool __cdecl CTableFrameSink::OnEventGameEnd(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_NORMAL:		//�������	
		{
			//�ɷ��˿�
			DispatchTableCard();

			//�������
			LONGLONG lBankerWinScore=CalculateScore();

			//��������
			m_wBankerTime++;

			//������Ϣ
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//ׯ����Ϣ
			GameEnd.nBankerTime = m_wBankerTime;
			GameEnd.lBankerTotallScore=m_lBankerWinScore;
			GameEnd.lBankerScore=lBankerWinScore;
			GameEnd.bcFirstCard = m_bcFirstPostCard;
			GameEnd.bcNextCard = m_bcNextPostCard;
			//�˿���Ϣ
			CopyMemory(GameEnd.cbTableCardArray,m_cbTableCardArray,sizeof(m_cbTableCardArray));

			GameEnd.cbLeftCardCount=m_cbLeftCardCount+8;
			for (int i = 0;i<4;i++)
			{
				CopyMemory(&m_cbTableHavaSendCardArray[i][m_cbTableHavaSendCount[i]],GameEnd.cbTableCardArray[i],2);		
				m_cbTableHavaSendCount[i]+=2;
				CopyMemory(GameEnd.cbTableHavaSendCardArray[i],m_cbTableHavaSendCardArray[i],CountArray(m_cbTableHavaSendCardArray[i]));
			}
			
			CopyMemory(GameEnd.cbTableHavaSendCount,m_cbTableHavaSendCount,CountArray(m_cbTableHavaSendCount));
			//���ͻ���
			GameEnd.cbTimeLeave=TIME_GAME_END;	
			for ( WORD wUserIndex = 0; wUserIndex < GAME_PLAYER; ++wUserIndex )
			{
				IServerUserItem *pIServerUserItem = m_pITableFrame->GetServerUserItem(wUserIndex);
				if ( pIServerUserItem == NULL ) continue;

				//���óɼ�
				GameEnd.lUserScore=m_lUserWinScore[wUserIndex];

				//��������
				GameEnd.lUserReturnScore=m_lUserReturnScore[wUserIndex];

				//����˰��
				if (m_lUserRevenue[wUserIndex]>0) GameEnd.lRevenue=m_lUserRevenue[wUserIndex];
				else if (m_wCurrentBanker!=INVALID_CHAIR) GameEnd.lRevenue=m_lUserRevenue[m_wCurrentBanker];
				else GameEnd.lRevenue=0;

				//������Ϣ					
				m_pITableFrame->SendTableData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
				m_pITableFrame->SendLookonData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			}

			return true;
		}
	case GER_USER_LEFT:		//�û��뿪
		{
			//�м��ж�
			if (m_wCurrentBanker!=wChairID)
			{
				//��������
				LONGLONG lScore=0;
				LONGLONG lRevenue=0;
				enScoreKind ScoreKind=enScoreKind_Flee;

				//ͳ�Ƴɼ�
				for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) lScore -= m_lUserJettonScore[nAreaIndex][wChairID];

				//д�����
				if (lScore!=0L) m_pITableFrame->WriteUserScore(pIServerUserItem, lScore,lRevenue, ScoreKind);

				return true;
			}

			//״̬�ж�
			if (m_pITableFrame->GetGameStatus()!=GS_GAME_END)
			{
				//��ʾ��Ϣ
				TCHAR szTipMsg[128];
				_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("����ׯ��[ %s ]ǿ�ˣ���Ϸ��ǰ������"),pIServerUserItem->GetAccounts());

				//������Ϣ
				SendGameMessage(INVALID_CHAIR,szTipMsg);	

				//����״̬
				m_pITableFrame->SetGameStatus(GS_GAME_END);

				//����ʱ��
				m_pITableFrame->KillGameTimer(IDI_PLACE_JETTON);
				m_pITableFrame->SetGameTimer(IDI_GAME_END,TIME_GAME_END*1000,1,0L);

				//�ɷ��˿�
				DispatchTableCard();

				//�������
				CalculateScore();

				//������Ϣ
				CMD_S_GameEnd GameEnd;
				ZeroMemory(&GameEnd,sizeof(GameEnd));

				//ׯ����Ϣ
				GameEnd.nBankerTime = m_wBankerTime;
				GameEnd.lBankerTotallScore=m_lBankerWinScore;
				if (m_lBankerWinScore>0) GameEnd.lBankerScore=0;

				//�˿���Ϣ
				CopyMemory(GameEnd.cbTableCardArray,m_cbTableCardArray,sizeof(m_cbTableCardArray));
				GameEnd.cbLeftCardCount=m_cbLeftCardCount;

				//���ͻ���
				GameEnd.cbTimeLeave=TIME_GAME_END;	
				for ( WORD wUserIndex = 0; wUserIndex < GAME_PLAYER; ++wUserIndex )
				{
					IServerUserItem *pIServerUserItem = m_pITableFrame->GetServerUserItem(wUserIndex);
					if ( pIServerUserItem == NULL ) continue;

					//���óɼ�
					GameEnd.lUserScore=m_lUserWinScore[wUserIndex];

					//��������
					GameEnd.lUserReturnScore=m_lUserReturnScore[wUserIndex];

					//����˰��
					if (m_lUserRevenue[wUserIndex]>0) GameEnd.lRevenue=m_lUserRevenue[wUserIndex];
					else if (m_wCurrentBanker!=INVALID_CHAIR) GameEnd.lRevenue=m_lUserRevenue[m_wCurrentBanker];
					else GameEnd.lRevenue=0;

					//������Ϣ					
					m_pITableFrame->SendTableData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
					m_pITableFrame->SendLookonData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
				}
			}

			//�۳�����
			if (m_lUserWinScore[m_wCurrentBanker]<0)
			{
				m_pITableFrame->WriteUserScore(m_wCurrentBanker,m_lUserWinScore[m_wCurrentBanker], m_lUserRevenue[m_wCurrentBanker], enScoreKind_Flee);
			}

			//�л�ׯ��
			ChangeBanker(true);

			return true;
		}
	}

	return false;
}

//���ͳ���
bool __cdecl CTableFrameSink::SendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	switch (cbGameStatus)
	{
	case GS_FREE:			//����״̬
		{
			//���ͼ�¼
			SendGameRecord(pIServerUserItem);

			//��������
			CMD_S_StatusFree StatusFree;
			ZeroMemory(&StatusFree,sizeof(StatusFree));			

			//������Ϣ
			StatusFree.lApplyBankerCondition = m_lApplyBankerCondition;
			StatusFree.lAreaLimitScore = m_lAreaLimitScore;

			//�˿���Ϣ
			StatusFree.cbLeftCardCount=m_cbLeftCardCount;
			for (int i = 0;i<4;i++)
			{	
				CopyMemory(StatusFree.cbTableHavaSendCardArray[i],m_cbTableHavaSendCardArray[i],CountArray(m_cbTableHavaSendCardArray[i]));
			}

			CopyMemory(StatusFree.cbTableHavaSendCount,m_cbTableHavaSendCount,CountArray(m_cbTableHavaSendCount));

			//ׯ����Ϣ
			StatusFree.bEnableSysBanker=m_bEnableSysBanker;
			StatusFree.wBankerUser=m_wCurrentBanker;	
			StatusFree.cbBankerTime=m_wBankerTime;
			StatusFree.lBankerWinScore=m_lBankerWinScore;
			if (m_wCurrentBanker!=INVALID_CHAIR)
			{
				IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
				StatusFree.lBankerScore=pIServerUserItem->GetUserScore()->lScore;
			}

			//�����Ϣ
			if (pIServerUserItem->GetUserStatus()!=US_LOOKON)
			{
				StatusFree.lUserMaxScore=min(pIServerUserItem->GetUserScore()->lScore,m_lUserLimitScore); 
			}

			//ȫ����Ϣ
			DWORD dwPassTime=(DWORD)time(NULL)-m_dwJettonTime;
			StatusFree.cbTimeLeave=(BYTE)(TIME_FREE-__min(dwPassTime,TIME_FREE));

			//���ͳ���
			bool bSuccess = m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));

			//������ʾ
			TCHAR szTipMsg[128];
			_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("��������ׯ����Ϊ��%I64d,��������Ϊ��%I64d,�������Ϊ��%I64d"),m_lApplyBankerCondition,
				m_lAreaLimitScore,m_lUserLimitScore);
			
			m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_INFO);
						
			//����������
			SendApplyUser(pIServerUserItem);

			return bSuccess;
		}
	case GS_PLACE_JETTON:		//��Ϸ״̬
	case GS_GAME_END:			//����״̬
		{
			//���ͼ�¼
			SendGameRecord(pIServerUserItem);		

			//��������
			CMD_S_StatusPlay StatusPlay={0};

			//ȫ����ע
			CopyMemory(StatusPlay.lAllJettonScore,m_lAllJettonScore,sizeof(StatusPlay.lAllJettonScore));

			//�����ע
			if (pIServerUserItem->GetUserStatus()!=US_LOOKON)
			{
				for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
					StatusPlay.lUserJettonScore[nAreaIndex] = m_lUserJettonScore[nAreaIndex][wChiarID];

				//�����ע
				StatusPlay.lUserMaxScore=min(pIServerUserItem->GetUserScore()->lScore,m_lUserLimitScore);
			}

			//������Ϣ
			StatusPlay.lApplyBankerCondition=m_lApplyBankerCondition;		
			StatusPlay.lAreaLimitScore=m_lAreaLimitScore;		

			//ׯ����Ϣ
			StatusPlay.bEnableSysBanker=m_bEnableSysBanker;
			StatusPlay.wBankerUser=m_wCurrentBanker;			
			StatusPlay.cbBankerTime=m_wBankerTime;
			StatusPlay.lBankerWinScore=m_lBankerWinScore;	


			StatusPlay.cbLeftCardCount=m_cbLeftCardCount;
			for (int i = 0;i<4;i++)
			{
				CopyMemory(StatusPlay.cbTableHavaSendCardArray[i],m_cbTableHavaSendCardArray[i],CountArray(m_cbTableHavaSendCardArray[i]));
			}

			CopyMemory(StatusPlay.cbTableHavaSendCount,m_cbTableHavaSendCount,CountArray(m_cbTableHavaSendCount));


			if (m_wCurrentBanker!=INVALID_CHAIR)
			{
				IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
				StatusPlay.lBankerScore=pIServerUserItem->GetUserScore()->lScore;
			}	

			//ȫ����Ϣ
			DWORD dwPassTime=(DWORD)time(NULL)-m_dwJettonTime;
			StatusPlay.cbTimeLeave=(BYTE)(TIME_FREE-__min(dwPassTime,TIME_FREE));
			StatusPlay.cbGameStatus=m_pITableFrame->GetGameStatus();			

			//�����ж�
			if (cbGameStatus==GS_GAME_END)
			{
				//���óɼ�
				StatusPlay.lEndUserScore=m_lUserWinScore[wChiarID];

				//��������
				StatusPlay.lEndUserReturnScore=m_lUserReturnScore[wChiarID];

				//����˰��
				if (m_lUserRevenue[wChiarID]>0) StatusPlay.lEndRevenue=m_lUserRevenue[wChiarID];
				else if (m_wCurrentBanker!=INVALID_CHAIR) StatusPlay.lEndRevenue=m_lUserRevenue[m_wCurrentBanker];
				else StatusPlay.lEndRevenue=0;

				//ׯ�ҳɼ�
				StatusPlay.lEndBankerScore=m_lBankerCurGameScore;

				//�˿���Ϣ
				CopyMemory(StatusPlay.cbTableCardArray,m_cbTableCardArray,sizeof(m_cbTableCardArray));
			}

			//����������
			SendApplyUser( pIServerUserItem );

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));
		}
	}

	return false;
}

//��ʱ���¼�
bool __cdecl CTableFrameSink::OnTimerMessage(WORD wTimerID, WPARAM wBindParam)
{
	switch (wTimerID)
	{
	case IDI_FREE:		//����ʱ��
		{
			//��ʼ��Ϸ
			m_pITableFrameControl->StartGame();

			//����ʱ��
			m_pITableFrame->SetGameTimer(IDI_PLACE_JETTON,TIME_PLACE_JETTON*1000,1,0L);

			//����״̬
			m_pITableFrame->SetGameStatus(GS_PLACE_JETTON);

			return true;
		}
	case IDI_PLACE_JETTON:		//��עʱ��
		{
			//״̬�ж�(��ֹǿ���ظ�����)
			if (m_pITableFrame->GetGameStatus()!=GS_GAME_END)
			{
				//����״̬
				m_pITableFrame->SetGameStatus(GS_GAME_END);			

				//������Ϸ
				OnEventGameEnd(INVALID_CHAIR,NULL,GER_NORMAL);

				//����ʱ��
				m_pITableFrame->SetGameTimer(IDI_GAME_END,TIME_GAME_END*1000,1,0L);			
			}

			return true;
		}
	case IDI_GAME_END:			//������Ϸ
		{
			//д�����
			for ( WORD wUserChairID = 0; wUserChairID < GAME_PLAYER; ++wUserChairID )
			{
				IServerUserItem *pIServerUserItem = m_pITableFrame->GetServerUserItem(wUserChairID);
				if ( pIServerUserItem == NULL ) continue;

				//ʤ������
				enScoreKind ScoreKind=(m_lUserWinScore[wUserChairID]>0L)?enScoreKind_Win:enScoreKind_Lost;

				//д�����
				if (m_lUserWinScore[wUserChairID]!=0L) m_pITableFrame->WriteUserScore(wUserChairID,m_lUserWinScore[wUserChairID], m_lUserRevenue[wUserChairID], ScoreKind);

				//��ׯ�ж�
				LONGLONG lUserScore=pIServerUserItem->GetUserScore()->lScore;
				if (wUserChairID!=m_wCurrentBanker && lUserScore<m_lApplyBankerCondition)
				{
					for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
					{
						if (wUserChairID!=m_ApplyUserArray[i]) continue;

						//ɾ�����
						m_ApplyUserArray.RemoveAt(i);

						//������Ϣ
						CMD_S_CancelBanker CancelBanker;
						ZeroMemory(&CancelBanker,sizeof(CancelBanker));

						//���ñ���
						lstrcpyn(CancelBanker.szCancelUser,pIServerUserItem->GetAccounts(),CountArray(CancelBanker.szCancelUser));

						//������Ϣ
						m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
						m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));

						//��ʾ��Ϣ
						TCHAR szTipMsg[128];
						_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("������Ľ����������ׯ����������%I64d��,���Զ���ׯ��"),m_lApplyBankerCondition);
						SendGameMessage(wUserChairID,szTipMsg);

						break;
					}
				}
			}

			//������Ϸ
			m_pITableFrame->ConcludeGame();

			//�л�ׯ��
			ChangeBanker(false);

			//����ʱ��
			m_dwJettonTime=(DWORD)time(NULL);
			m_pITableFrame->SetGameTimer(IDI_FREE,TIME_FREE*1000,1,0L);

			//������Ϣ
			CMD_S_GameFree GameFree;
			ZeroMemory(&GameFree,sizeof(GameFree));
			GameFree.cbTimeLeave=TIME_FREE;
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_FREE,&GameFree,sizeof(GameFree));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_FREE,&GameFree,sizeof(GameFree));

			return true;
		}
	}

	return false;
}

//��Ϸ��Ϣ����
bool __cdecl CTableFrameSink::OnGameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	switch (wSubCmdID)
	{
	case SUB_C_PLACE_JETTON:		//�û���ע
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_PlaceJetton));
			if (wDataSize!=sizeof(CMD_C_PlaceJetton)) return false;

			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;

			//��Ϣ����
			CMD_C_PlaceJetton * pPlaceJetton=(CMD_C_PlaceJetton *)pDataBuffer;
			return OnUserPlaceJetton(pUserData->wChairID,pPlaceJetton->cbJettonArea,pPlaceJetton->lJettonScore);
		}
	case SUB_C_APPLY_BANKER:		//������ׯ
		{
			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus==US_LOOKON) return true;

			return OnUserApplyBanker(pIServerUserItem);	
		}
	case SUB_C_CANCEL_BANKER:		//ȡ����ׯ
		{
			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus==US_LOOKON) return true;

			return OnUserCancelBanker(pIServerUserItem);	
		}
	case SUB_C_CONTINUE_CARD:		//��������
		{
			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus==US_LOOKON) return true;
			if (pUserData->wChairID!=m_wCurrentBanker) return true;
			if (m_cbLeftCardCount < 8) return true;

			//���ñ���
			m_bContiueCard=true;

			//������Ϣ
			SendGameMessage(pUserData->wChairID,TEXT("���óɹ�����һ�ֽ��������ƣ�"));

			return true;
		}
	}

	return false;
}

//�����Ϣ����
bool __cdecl CTableFrameSink::OnFrameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

//�û�����
bool __cdecl CTableFrameSink::OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//����ʱ��
	if ((bLookonUser==false)&&(m_dwJettonTime==0L))
	{
		m_dwJettonTime=(DWORD)time(NULL);
		m_pITableFrame->SetGameTimer(IDI_FREE,TIME_FREE*1000,1,NULL);
		m_pITableFrame->SetGameStatus(GS_FREE);
	}

	//������ʾ
	TCHAR szTipMsg[128];
	_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("��������ׯ����Ϊ��%I64d,��������Ϊ��%I64d,�������Ϊ��%I64d"),m_lApplyBankerCondition,
		m_lAreaLimitScore,m_lUserLimitScore);
	m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_INFO);

	return true;
}

//�û�����
bool __cdecl CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//��¼�ɼ�
	if (bLookonUser==false)
	{
		//�л�ׯ��
		if (wChairID==m_wCurrentBanker)
		{
			ChangeBanker(true);
			m_bContiueCard=false;
		}

		//ȡ������
		for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
		{
			if (wChairID!=m_ApplyUserArray[i]) continue;

			//ɾ�����
			m_ApplyUserArray.RemoveAt(i);

			//�������
			CMD_S_CancelBanker CancelBanker;
			ZeroMemory(&CancelBanker,sizeof(CancelBanker));

			//���ñ���
			lstrcpyn(CancelBanker.szCancelUser,pIServerUserItem->GetAccounts(),CountArray(CancelBanker.szCancelUser));

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
			m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));

			break;
		}

		return true;
	}

	return true;
}

//��ע�¼�
bool CTableFrameSink::OnUserPlaceJetton(WORD wChairID, BYTE cbJettonArea, LONGLONG lJettonScore)
{
	//Ч�����
	ASSERT((cbJettonArea<=ID_JIAO_R && cbJettonArea>=ID_SHUN_MEN)&&(lJettonScore>0L));
	if ((cbJettonArea>ID_JIAO_R)||(lJettonScore<=0L) || cbJettonArea<ID_SHUN_MEN) return false;

	//Ч��״̬
	//ASSERT(m_pITableFrame->GetGameStatus()==GS_PLACE_JETTON);
	if (m_pITableFrame->GetGameStatus()!=GS_PLACE_JETTON)
	{
		CMD_S_PlaceJettonFail PlaceJettonFail;
		ZeroMemory(&PlaceJettonFail,sizeof(PlaceJettonFail));
		PlaceJettonFail.lJettonArea=cbJettonArea;
		PlaceJettonFail.lPlaceScore=lJettonScore;
		PlaceJettonFail.wPlaceUser=wChairID;

		//������Ϣ
		m_pITableFrame->SendTableData(wChairID,SUB_S_PLACE_JETTON_FAIL,&PlaceJettonFail,sizeof(PlaceJettonFail));
		return true;
	}

	//ׯ���ж�
	if (m_wCurrentBanker==wChairID) return true;
	if (m_bEnableSysBanker==false && m_wCurrentBanker==INVALID_CHAIR) return true;

	//��������
	IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);
	LONGLONG lJettonCount=0L;
	for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) lJettonCount += m_lUserJettonScore[nAreaIndex][wChairID];

	//��һ���
	LONGLONG lUserScore = pIServerUserItem->GetUserScore()->lScore;

	//�Ϸ�У��
	if (lUserScore < lJettonCount + lJettonScore) return true;
	if (m_lUserLimitScore < lJettonCount + lJettonScore) return true;

	//�ɹ���ʶ
	bool bPlaceJettonSuccess=true;

	//�Ϸ���֤
	if (GetUserMaxJetton(wChairID) >= lJettonScore)
	{
		//������ע
		m_lAllJettonScore[cbJettonArea] += lJettonScore;
		m_lUserJettonScore[cbJettonArea][wChairID] += lJettonScore;			
	}
	else
	{
		bPlaceJettonSuccess=false;
	}

	if (bPlaceJettonSuccess)
	{
		//��������
		CMD_S_PlaceJetton PlaceJetton;
		ZeroMemory(&PlaceJetton,sizeof(PlaceJetton));

		//�������
		PlaceJetton.wChairID=wChairID;
		PlaceJetton.cbJettonArea=cbJettonArea;
		PlaceJetton.lJettonScore=lJettonScore;

		//������Ϣ
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_PLACE_JETTON,&PlaceJetton,sizeof(PlaceJetton));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_PLACE_JETTON,&PlaceJetton,sizeof(PlaceJetton));
	}
	else
	{
		CMD_S_PlaceJettonFail PlaceJettonFail;
		ZeroMemory(&PlaceJettonFail,sizeof(PlaceJettonFail));
		PlaceJettonFail.lJettonArea=cbJettonArea;
		PlaceJettonFail.lPlaceScore=lJettonScore;
		PlaceJettonFail.wPlaceUser=wChairID;

		//������Ϣ
		m_pITableFrame->SendTableData(wChairID,SUB_S_PLACE_JETTON_FAIL,&PlaceJettonFail,sizeof(PlaceJettonFail));
	}

	return true;
}

//�����˿�
bool CTableFrameSink::DispatchTableCard()
{
	//��������
	int const static nDispatchCardCount=8;
	bool bContiueCard=m_bContiueCard;
	if (m_cbLeftCardCount<nDispatchCardCount) bContiueCard=false;

	//��������
	if (true==bContiueCard)
	{
		//����ϴ��
		BYTE bcCard[CARD_COUNT];
		m_GameLogic.RandCardList(bcCard,32);
		BYTE Sizi[]={1,2,3,4,5,6,1,2,3,4,5,6};
		RandCount(Sizi,12);
		m_bcFirstPostCard = Sizi[0];
		m_bcNextPostCard = Sizi[1];

		//�����˿�
		CopyMemory(&m_cbTableCardArray[0][0],m_cbTableCard,sizeof(m_cbTableCardArray));
		m_cbLeftCardCount -= nDispatchCardCount;
		if (m_cbLeftCardCount>0) CopyMemory(m_cbTableCard, m_cbTableCard + nDispatchCardCount, sizeof(BYTE)*m_cbLeftCardCount);
		//m_bContiueCard = true;
	}
	else
	{
		//����ϴ��
		BYTE bcCard[CARD_COUNT];
		m_GameLogic.RandCardList(bcCard,32);
		BYTE Sizi[]={1,2,3,4,5,6,1,2,3,4,5,6};
		RandCount(Sizi,12);
		m_bcFirstPostCard = Sizi[0];
		m_bcNextPostCard = Sizi[1];
		 for (int i = 0;i<2;i++)
		 {
			 m_GameLogic.RandCardList(m_cbTableCard,CountArray(m_cbTableCard));	
		 }
		//�����˿�
		CopyMemory(&m_cbTableCardArray[0][0], m_cbTableCard, sizeof(m_cbTableCardArray));

		m_cbLeftCardCount=CountArray(m_cbTableCard)-nDispatchCardCount;
		CopyMemory(m_cbTableCard, m_cbTableCard + nDispatchCardCount, sizeof(BYTE)*m_cbLeftCardCount);
				
		for (int i = 0;i<4;i++){ memset(&m_cbTableHavaSendCardArray[i],0,2) ;m_cbTableHavaSendCount[i] = 0; }

		//m_bContiueCard = false;
							

		////ׯ���ж�
		//if (INVALID_CHAIR != m_wCurrentBanker)
		//{
		//	m_cbLeftCardCount=CountArray(m_cbTableCard)-nDispatchCardCount;
		//	CopyMemory(m_cbTableCard, m_cbTableCard + nDispatchCardCount, sizeof(BYTE)*m_cbLeftCardCount);
		//}
		//else
		//{
		//	m_cbLeftCardCount=0;
		//}
	}

	//���Ʊ�־
	m_bContiueCard=false;

	////������Ŀ
	//for (int i=0; i<CountArray(m_cbCardCount); ++i) m_cbCardCount[i]=2;

	////���Ʊ�־
	//m_bContiueCard=false;

	////��ȡ����
 //  BYTE	m_cbWinSideControl =	  GetPrivateProfileInt( TEXT("ManagerControl"), TEXT("WinSide"), 0, m_szConfigFileName );

 //  BYTE	m_cbFailSideControl =	  GetPrivateProfileInt( TEXT("ManagerControl"), TEXT("FailSide"), 0, m_szConfigFileName );

 //  bool static bWinShunMen, bWinDuiMen, bWinDaoMen;

 //  DeduceWinner(bWinShunMen, bWinDuiMen, bWinDaoMen);

 //  if(m_cbWinSideControl ==m_cbFailSideControl)
 //  {
	//   return true;
 //  }
	//   switch (m_cbWinSideControl)
	//   {
	//   case 1:
	//	//   if(bWinShunMen!=false&&bWinDuiMen!=false&&bWinDaoMen!=false&&m_nSendCardCount<SEND_COUNT)
	//	   {
	//		   ChangeMaxUser();
	//		   return true;
	//		  /* m_nSendCardCount++;
	//		   DispatchTableCard();*/
	//	   }/*else
	//	   {

	//		    m_nSendCardCount= 0;
	//	   }*/
	//   case 2:
	//	   if(bWinShunMen!=true&&m_nSendCardCount<SEND_COUNT)
	//	   {
	//		   m_nSendCardCount++;
	//		   DispatchTableCard();
	//	   }else
	//	   {

	//		   m_nSendCardCount= 0;
	//	   }
	//	   break;
	//   case 3:
	//	   if(bWinDuiMen!=true&&m_nSendCardCount<SEND_COUNT)
	//	   {
	//		   m_nSendCardCount++;
	//		   DispatchTableCard();
	//	   }else
	//	   {

	//		   m_nSendCardCount= 0;
	//	   }
	//	   break;
	//   case 4:
	//	   if(bWinDaoMen!=true&&m_nSendCardCount<SEND_COUNT)
	//	   {
	//		   m_nSendCardCount++;
	//		   DispatchTableCard();
	//	   }else
	//	   {

	//		   m_nSendCardCount= 0;
	//	   }
	//	   break;
	//   }

	//   switch (m_cbFailSideControl)
	//   {
	//   case 1:
	//	   ChangeMaxUser(true);
	//	   return true;
	//   case 2:
	//	   if(bWinShunMen==true&&m_nSendCardCount<SEND_COUNT)
	//	   {
	//		   m_nSendCardCount++;
	//		   DispatchTableCard();
	//	   }else
	//	   {

	//		   m_nSendCardCount= 0;
	//	   }
	//	   break;
	//   case 3:
	//	   if(bWinDuiMen==true&&m_nSendCardCount<SEND_COUNT)
	//	   {
	//		   m_nSendCardCount++;
	//		   DispatchTableCard();
	//	   }else
	//	   {

	//		   m_nSendCardCount= 0;
	//	   }
	//	   break;
	//   case 4:
	//	   if(bWinDaoMen==true&&m_nSendCardCount<SEND_COUNT)
	//	   {
	//		   m_nSendCardCount++;
	//		   DispatchTableCard();
	//	   }else
	//	   {

	//		   m_nSendCardCount= 0;
	//	   }
	//	   break;
	//   }
	    
       return true;
}
void CTableFrameSink::RandList(BYTE cbCardBuffer[], BYTE cbBufferCount)
{

		//����׼��
		BYTE *cbCardData = new BYTE[cbBufferCount];
		CopyMemory(cbCardData,cbCardBuffer,cbBufferCount);

		//�����˿�
		BYTE cbRandCount=0,cbPosition=0;
		do
		{
			cbPosition=rand()%(cbBufferCount-cbRandCount);
			cbCardBuffer[cbRandCount++]=cbCardData[cbPosition];
			cbCardData[cbPosition]=cbCardData[cbBufferCount-cbRandCount];
		} while (cbRandCount<cbBufferCount);

		delete []cbCardData;
		cbCardData = NULL;

		return;
}
//����ׯ��
bool CTableFrameSink::OnUserApplyBanker(IServerUserItem *pIApplyServerUserItem)
{
	//�Ϸ��ж�
	LONGLONG lUserScore=pIApplyServerUserItem->GetUserScore()->lScore;
	if (lUserScore<m_lApplyBankerCondition)
	{
		m_pITableFrame->SendGameMessage(pIApplyServerUserItem,TEXT("��Ľ�Ҳ���������ׯ�ң�����ʧ�ܣ�"),SMT_INFO|SMT_EJECT);
		return true;
	}

	//�����ж�
	WORD wApplyUserChairID=pIApplyServerUserItem->GetChairID();
	for (INT_PTR nUserIdx=0; nUserIdx<m_ApplyUserArray.GetCount(); ++nUserIdx)
	{
		WORD wChairID=m_ApplyUserArray[nUserIdx];
		if (wChairID==wApplyUserChairID)
		{
			m_pITableFrame->SendGameMessage(pIApplyServerUserItem,TEXT("���Ѿ�������ׯ�ң�����Ҫ�ٴ����룡"),SMT_INFO|SMT_EJECT);
			//CMD_S_ApplyBanker ApplyBanker;
			//ZeroMemory(&ApplyBanker,sizeof(ApplyBanker));
			////���ñ���
			//ApplyBanker.wApplyUser=wApplyUserChairID;
			//m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_APPLY_FAIL, &ApplyBanker, sizeof(ApplyBanker));
			//m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_APPLY_FAIL, &ApplyBanker, sizeof(ApplyBanker));
			return true;
		}
	}

	//������Ϣ 
	m_ApplyUserArray.Add(wApplyUserChairID);

	//�������
	CMD_S_ApplyBanker ApplyBanker;
	ZeroMemory(&ApplyBanker,sizeof(ApplyBanker));

	//���ñ���
	ApplyBanker.wApplyUser=wApplyUserChairID;

	//������Ϣ
	m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));
	m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));

	//�л��ж�
	if (m_pITableFrame->GetGameStatus()==GS_FREE && m_ApplyUserArray.GetCount()==1)
	{
		ChangeBanker(false);
	}

	return true;
}

//ȡ������
bool CTableFrameSink::OnUserCancelBanker(IServerUserItem *pICancelServerUserItem)
{
	//��ǰׯ��
	if (pICancelServerUserItem->GetChairID()==m_wCurrentBanker && m_pITableFrame->GetGameStatus()!=GS_FREE)
	{
		//������Ϣ
		m_pITableFrame->SendGameMessage(pICancelServerUserItem,TEXT("��Ϸ�Ѿ���ʼ��������ȡ����ׯ��"),SMT_INFO|SMT_EJECT);

	/*	CMD_S_bCanCancelBanker CanCelBanker;
		CanCelBanker.blCancel = false;
		CanCelBanker.wChariID = m_wCurrentBanker;
		m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_CAN, &CanCelBanker, sizeof(CanCelBanker));
		m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_CAN, &CanCelBanker, sizeof(CanCelBanker));*/
		return true;
	}

	//�����ж�
	for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
	{
		//��ȡ���
		WORD wChairID=m_ApplyUserArray[i];
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);

		//��������
		if (pIServerUserItem==NULL) continue;
		if (pIServerUserItem->GetUserID()!=pICancelServerUserItem->GetUserID()) continue;

		//ɾ�����
		m_ApplyUserArray.RemoveAt(i);

		if (m_wCurrentBanker!=wChairID)
		{
			//�������
			CMD_S_CancelBanker CancelBanker;
			ZeroMemory(&CancelBanker,sizeof(CancelBanker));

			//���ñ���
			lstrcpyn(CancelBanker.szCancelUser,pIServerUserItem->GetAccounts(),CountArray(CancelBanker.szCancelUser));

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
			m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
		}
		else if (m_wCurrentBanker==wChairID)
		{
			//�л�ׯ�� 
			m_wCurrentBanker=INVALID_CHAIR;
			ChangeBanker(true);
		}
	/*	CMD_S_bCanCancelBanker CanCelBanker;
		CanCelBanker.blCancel = true;
		CanCelBanker.wChariID = m_wCurrentBanker;
		m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_CAN, &CanCelBanker, sizeof(CanCelBanker));
		m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_CAN, &CanCelBanker, sizeof(CanCelBanker));*/
		return true;
	}

	return false;
}

//����ׯ��
bool CTableFrameSink::ChangeBanker(bool bCancelCurrentBanker)
{
	//�л���ʶ
	bool bChangeBanker=false;
	//��ׯ����
	WORD wBankerTime=GetPrivateProfileInt(TEXT("BankerCondition"), TEXT("Time"), 10, m_szConfigFileName);
	WORD wAddTime=GetPrivateProfileInt(TEXT("BankerCondition"), TEXT("TimeAdd"), 5, m_szConfigFileName);
    LONGLONG lBigScore = 0; 

	for(int i = 0;i<m_ApplyUserArray.GetCount();i++)
	{
		    WORD wChairID=m_ApplyUserArray[i];
			if(wChairID==m_wCurrentBanker)
			{
				continue;
			}
			IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);
			if(pIServerUserItem->GetUserScore()->lScore>lBigScore)
			{
				lBigScore = pIServerUserItem->GetUserScore()->lScore;

			}	
	}
	if(m_wCurrentBanker!=INVALID_CHAIR)
	{
		IServerUserItem *pTempIServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
		if(pTempIServerUserItem!=NULL)
		{
			LONGLONG lBankerTempScore=pTempIServerUserItem->GetUserScore()->lScore;

			if(lBigScore<lBankerTempScore)
			{
				wBankerTime+=wAddTime;
			}

		}
		


	}
	
	//ȡ����ǰ
	if (bCancelCurrentBanker)
	{
		for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
		{
			//��ȡ���
			WORD wChairID=m_ApplyUserArray[i];

			//��������
			if (wChairID!=m_wCurrentBanker) continue;

			//ɾ�����
			m_ApplyUserArray.RemoveAt(i);

			break;
		}

		//����ׯ��
		m_wCurrentBanker=INVALID_CHAIR;

		//�ֻ��ж�
		if (m_pITableFrame->GetGameStatus()==GS_FREE && m_ApplyUserArray.GetCount()!=0)
		{
			m_wCurrentBanker=m_ApplyUserArray[0];
		}

		//���ñ���
		bChangeBanker=true;
	}
	//��ׯ�ж�
	else if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		//��ȡׯ��
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);

		if(pIServerUserItem!= NULL)
		{
			LONGLONG lBankerScore=pIServerUserItem->GetUserScore()->lScore;

			//�����ж�
			if (wBankerTime<=m_wBankerTime || lBankerScore<m_lApplyBankerCondition)
			{
				//�������
				for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
				{
					//��ȡ���
					WORD wChairID=m_ApplyUserArray[i];

					//��������
					if (wChairID!=m_wCurrentBanker) continue;

					//ɾ�����
					m_ApplyUserArray.RemoveAt(i);

					break;
				}

				//����ׯ��
				m_wCurrentBanker=INVALID_CHAIR;

				//�ֻ��ж�
				if (m_ApplyUserArray.GetCount()!=0)
				{
					m_wCurrentBanker=m_ApplyUserArray[0];
				}

				bChangeBanker=true;

				//��ʾ��Ϣ
				TCHAR szTipMsg[128];
				if (lBankerScore<m_lApplyBankerCondition)
					_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("[ %s ]��������(%I64d)��ǿ�л�ׯ!"),pIServerUserItem->GetAccounts(),m_lApplyBankerCondition);
				else
					_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("[ %s ]��ׯ�����ﵽ(%d)��ǿ�л�ׯ!"),pIServerUserItem->GetAccounts(),wBankerTime);

				//������Ϣ
				SendGameMessage(INVALID_CHAIR,szTipMsg);	
			}

		}
		else
		{
			for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
			{
				//��ȡ���
				WORD wChairID=m_ApplyUserArray[i];

				//��������
				if (wChairID!=m_wCurrentBanker) continue;

				//ɾ�����
				m_ApplyUserArray.RemoveAt(i);

				break;
			}
			//����ׯ��
			m_wCurrentBanker=INVALID_CHAIR;
		}
		
	}
	//ϵͳ��ׯ
	else if (m_wCurrentBanker==INVALID_CHAIR && m_ApplyUserArray.GetCount()!=0)
	{
		m_wCurrentBanker=m_ApplyUserArray[0];
		bChangeBanker=true;
	}

	//�л��ж�
	if (bChangeBanker)
	{
		//���ñ���
		m_wBankerTime = 0;
		m_lBankerWinScore=0;

		//������Ϣ
		CMD_S_ChangeBanker ChangeBanker;
		ZeroMemory(&ChangeBanker,sizeof(ChangeBanker));
		ChangeBanker.wBankerUser=m_wCurrentBanker;
		if (m_wCurrentBanker!=INVALID_CHAIR)
		{
			IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
			ChangeBanker.lBankerScore=pIServerUserItem->GetUserScore()->lScore;
		}
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CHANGE_BANKER,&ChangeBanker,sizeof(ChangeBanker));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_CHANGE_BANKER,&ChangeBanker,sizeof(ChangeBanker));

		if (m_wCurrentBanker!=INVALID_CHAIR)
		{
			//��ȡ��Ϣ
			LONGLONG lMessageCount=GetPrivateProfileInt(TEXT("Message"),TEXT("MessageCount"),0,m_szConfigFileName);
			if (lMessageCount!=0)
			{
				//��ȡ����
				LONGLONG lIndex=rand()%lMessageCount;
				TCHAR szKeyName[32],szMessage1[256],szMessage2[256];				
				_sntprintf(szKeyName,CountArray(szKeyName),TEXT("Item%I64d"),lIndex);
				GetPrivateProfileString(TEXT("Message"),szKeyName,TEXT("��ϲ[ %s ]��ׯ"),szMessage1,CountArray(szMessage1),m_szConfigFileName);

				//��ȡ���
				IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);

				//������Ϣ
				_sntprintf(szMessage2,CountArray(szMessage2),szMessage1,pIServerUserItem->GetAccounts());
				SendGameMessage(INVALID_CHAIR,szMessage2);
			}
		}
	}

	return bChangeBanker;
}



//����ׯ��
void CTableFrameSink::SendApplyUser( IServerUserItem *pRcvServerUserItem )
{
	for (INT_PTR nUserIdx=0; nUserIdx<m_ApplyUserArray.GetCount(); ++nUserIdx)
	{
		WORD wChairID=m_ApplyUserArray[nUserIdx];

		//��ȡ���
		IServerUserItem *pServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);
		if (!pServerUserItem) continue;

		//ׯ���ж�
		if (pServerUserItem->GetChairID()==m_wCurrentBanker) continue;

		//�������
		CMD_S_ApplyBanker ApplyBanker;
		ApplyBanker.wApplyUser=wChairID;

		//������Ϣ
		m_pITableFrame->SendUserData(pRcvServerUserItem, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));
	}
}

//�û�����
bool __cdecl CTableFrameSink::OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) 
{
	//�л�ׯ��
//	if (wChairID==m_wCurrentBanker) ChangeBanker(true);

	//ȡ������
	for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
	{
		if (wChairID!=m_ApplyUserArray[i]) continue;

		//ɾ�����
		m_ApplyUserArray.RemoveAt(i);

		//�������
		CMD_S_CancelBanker CancelBanker;
		ZeroMemory(&CancelBanker,sizeof(CancelBanker));

		//���ñ���
		lstrcpyn(CancelBanker.szCancelUser,pIServerUserItem->GetAccounts(),CountArray(CancelBanker.szCancelUser));

		//������Ϣ
		m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
		m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));

		break;
	}

	return true;
}

//�����ע
LONGLONG CTableFrameSink::GetUserMaxJetton(WORD wChairID)
{
	//��ȡ���
	IServerUserItem *pIMeServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);
	if (NULL==pIMeServerUserItem) return 0L;

	//����ע��
	LONGLONG lNowJetton = 0;
	ASSERT(AREA_COUNT<=CountArray(m_lUserJettonScore));
	for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) lNowJetton += m_lUserJettonScore[nAreaIndex][wChairID];

	//ׯ�ҽ��
	LONGLONG lBankerScore=2147483647;
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		lBankerScore=m_lBankerScore;
	}
	for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) lBankerScore-=m_lAllJettonScore[nAreaIndex];

	//��������
	LONGLONG lMeMaxScore = min((pIMeServerUserItem->GetUserScore()->lScore-lNowJetton), m_lUserLimitScore);

	//��������
	lMeMaxScore=min(lMeMaxScore,m_lAreaLimitScore);

	//ׯ������
	lMeMaxScore=min(lMeMaxScore,lBankerScore);

	//��������
	ASSERT(lMeMaxScore >= 0);
	lMeMaxScore = max(lMeMaxScore, 0);

	return lMeMaxScore;
}

//����÷�
LONGLONG CTableFrameSink::CalculateScore()
{
	//��������
	LONGLONG static cbRevenue=m_pGameServiceOption->wRevenue;

	//�ƶ����
	bool static bWinShunMen, bWinDuiMen, bWinDaoMen;
	DeduceWinner(bWinShunMen, bWinDuiMen, bWinDaoMen);

	//��Ϸ��¼
	tagServerGameRecord &GameRecord = m_GameRecordArrary[m_nRecordLast];
	GameRecord.bWinShunMen=bWinShunMen;
	GameRecord.bWinDuiMen=bWinDuiMen;
	GameRecord.bWinDaoMen=bWinDaoMen;

	//�ƶ��±�
	m_nRecordLast = (m_nRecordLast+1) % MAX_SCORE_HISTORY;
	if ( m_nRecordLast == m_nRecordFirst ) m_nRecordFirst = (m_nRecordFirst+1) % MAX_SCORE_HISTORY;

	//ׯ������
	LONGLONG lBankerWinScore = 0;

	//��ҳɼ�
	ZeroMemory(m_lUserWinScore, sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore, sizeof(m_lUserReturnScore));
	ZeroMemory(m_lUserRevenue, sizeof(m_lUserRevenue));
	LONGLONG lUserLostScore[GAME_PLAYER];
	ZeroMemory(lUserLostScore, sizeof(lUserLostScore));

	//�����ע
	LONGLONG *const pUserScore[] = {NULL, m_lUserJettonScore[ID_SHUN_MEN], m_lUserJettonScore[ID_JIAO_L], m_lUserJettonScore[ID_QIAO],
		m_lUserJettonScore[ID_DUI_MEN], m_lUserJettonScore[ID_DAO_MEN], m_lUserJettonScore[ID_JIAO_R]};	

	//������
	BYTE static const cbMultiple[] = {0, 1, 1, 1, 1, 1, 1};

	//ʤ����ʶ
	bool static bWinFlag[AREA_COUNT+1];
	bWinFlag[0]=false;
	bWinFlag[ID_SHUN_MEN]=bWinShunMen;
	bWinFlag[ID_JIAO_L]=(true==bWinShunMen && true==bWinDuiMen) ? true : false;
	bWinFlag[ID_QIAO]=(true==bWinShunMen && true==bWinDaoMen) ? true : false;
	bWinFlag[ID_DUI_MEN]=bWinDuiMen;
	bWinFlag[ID_DAO_MEN]=bWinDaoMen;
	bWinFlag[ID_JIAO_R]=(true==bWinDaoMen && true==bWinDuiMen) ? true : false;

	//�Ǳ�ʶ
	bool static bWinBankerJiaoL,bWinBankerJiaoR,bWinBankerQiao;
	bWinBankerJiaoL=(false==bWinShunMen && false==bWinDuiMen) ? true : false;
	bWinBankerJiaoR=(false==bWinDaoMen && false==bWinDuiMen) ? true : false;
	bWinBankerQiao=(false==bWinShunMen && false==bWinDaoMen) ? true : false;

	//�������
	for (WORD wChairID=0; wChairID<GAME_PLAYER; wChairID++)
	{
		//ׯ���ж�
		if (m_wCurrentBanker==wChairID) continue;

		//��ȡ�û�
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);
		if (pIServerUserItem==NULL) continue;

		for (WORD wAreaIndex = ID_SHUN_MEN; wAreaIndex <= ID_JIAO_R; ++wAreaIndex)
		{
			//���ж�
			bool bReturnScore=false;
			if (ID_JIAO_L==wAreaIndex && false==bWinBankerJiaoL) bReturnScore=true;
			if (ID_JIAO_R==wAreaIndex && false==bWinBankerJiaoR) bReturnScore=true;
			if (ID_QIAO==wAreaIndex && false==bWinBankerQiao) bReturnScore=true;

			if (true==bWinFlag[wAreaIndex]) 
			{
				m_lUserWinScore[wChairID] += ( pUserScore[wAreaIndex][wChairID] * cbMultiple[wAreaIndex] ) ;
				m_lUserReturnScore[wChairID] += pUserScore[wAreaIndex][wChairID] ;
				lBankerWinScore -= ( pUserScore[wAreaIndex][wChairID] * cbMultiple[wAreaIndex] ) ;
			}
			else if (true==bReturnScore)
			{
				m_lUserReturnScore[wChairID] += pUserScore[wAreaIndex][wChairID] ;
			}
			else
			{
				lUserLostScore[wChairID] -= pUserScore[wAreaIndex][wChairID];
				lBankerWinScore += pUserScore[wAreaIndex][wChairID];
			}
		}

		//����˰��
		if (0 < m_lUserWinScore[wChairID])
		{
			float fRevenuePer=float(cbRevenue/1000.);
			m_lUserRevenue[wChairID]  = LONGLONG(m_lUserWinScore[wChairID]*fRevenuePer);
			m_lUserWinScore[wChairID] -= m_lUserRevenue[wChairID];
		}
	  //	�ܵķ���
		m_lUserWinScore[wChairID] += lUserLostScore[wChairID];
	}

	//ׯ�ҳɼ�
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		m_lUserWinScore[m_wCurrentBanker] = lBankerWinScore;

		//����˰��
		if (0 < m_lUserWinScore[m_wCurrentBanker])
		{
			//float fRevenuePer=float(cbRevenue/1000.+0.000001);
			float fRevenuePer=float(cbRevenue/1000.);
			if(cbRevenue<=0)
			{
				fRevenuePer = 0;
			}
			m_lUserRevenue[m_wCurrentBanker]  = LONGLONG(m_lUserWinScore[m_wCurrentBanker]*fRevenuePer);
			m_lUserWinScore[m_wCurrentBanker] -= m_lUserRevenue[m_wCurrentBanker];
			lBankerWinScore = m_lUserWinScore[m_wCurrentBanker];
		}		
	}

	//�ۼƻ���
	m_lBankerWinScore += lBankerWinScore;

	//��ǰ����
	m_lBankerCurGameScore=lBankerWinScore;

	return lBankerWinScore;
}
void CTableFrameSink::ChangeMaxUser(bool blMin)
{
	int i ;
	BYTE m_cbTableCardArrayTemp[4][2];
	bool  bWinShunMen = 0;
	bool bWinDuiMen = 0;
	bool bWinDaoMen = 0;
	CopyMemory(&m_cbTableCardArrayTemp[0][0],m_cbTableCardArray,sizeof(m_cbTableCardArray));
	for (i= 0;i<4;i++)
	{
		  bWinShunMen=m_GameLogic.CompareCard(m_cbTableCardArray[i],2,m_cbTableCardArray[(i+1)%4],2)==1?true:false;
		 bWinDuiMen=m_GameLogic.CompareCard(m_cbTableCardArray[i],2,m_cbTableCardArray[(i+2)%4],2)==1?true:false;
		 bWinDaoMen=m_GameLogic.CompareCard(m_cbTableCardArray[i],2,m_cbTableCardArray[(i+3)%4],2)==1?true:false;
		if(bWinShunMen==bWinDuiMen&&bWinDaoMen==bWinDuiMen &&bWinDuiMen==blMin)
		{
			CopyMemory(m_cbTableCardArray[0],m_cbTableCardArrayTemp[i],sizeof(m_cbTableCardArrayTemp[i]));
			CopyMemory(m_cbTableCardArray[1],m_cbTableCardArrayTemp[(i+1)%4],sizeof(m_cbTableCardArrayTemp[(i+1)%4]));
			CopyMemory(m_cbTableCardArray[2],m_cbTableCardArrayTemp[(i+2)%4],sizeof(m_cbTableCardArrayTemp[(i+2)%4]));
			CopyMemory(m_cbTableCardArray[3],m_cbTableCardArrayTemp[(i+3)%4],sizeof(m_cbTableCardArrayTemp[(i+3)%4]));
			break ;
		}
	}
/*	if(bWinShunMen!=bWinDuiMen||bWinDaoMen!=bWinDuiMen ||bWinDuiMen!=blMin)
	{
		for (i= 0;i<4;i++)
		{
			bWinShunMen=m_GameLogic.CompareCard(m_cbTableCardArray[i],2,m_cbTableCardArray[(i+1)%4],2)==1?true:false;
			bWinDuiMen=m_GameLogic.CompareCard(m_cbTableCardArray[i],2,m_cbTableCardArray[(i+2)%4],2)==1?true:false;
			bWinDaoMen=m_GameLogic.CompareCard(m_cbTableCardArray[i],2,m_cbTableCardArray[(i+3)%4],2)==1?true:false;
			if(bWinShunMen==bWinDuiMen&&bWinDaoMen==bWinDuiMen &&bWinDuiMen==blMin)
			{
				CopyMemory(m_cbTableCardArray[0],m_cbTableCardArrayTemp[i],sizeof(m_cbTableCardArrayTemp[i]));
				CopyMemory(m_cbTableCardArray[1],m_cbTableCardArrayTemp[(i+1)%4],sizeof(m_cbTableCardArrayTemp[(i+1)%4]));
				CopyMemory(m_cbTableCardArray[2],m_cbTableCardArrayTemp[(i+2)%4],sizeof(m_cbTableCardArrayTemp[(i+2)%4]));
				CopyMemory(m_cbTableCardArray[3],m_cbTableCardArrayTemp[(i+3)%4],sizeof(m_cbTableCardArrayTemp[(i+3)%4]));
				break ;
			}
		}
		return ;
	}*/

	
}
void CTableFrameSink::DeduceWinner(bool &bWinShunMen, bool &bWinDuiMen, bool &bWinDaoMen)
{
	//��С�Ƚ�
	bWinShunMen=m_GameLogic.CompareCard(m_cbTableCardArray[BANKER_INDEX],2,m_cbTableCardArray[SHUN_MEN_INDEX],2)==1?true:false;
	bWinDuiMen=m_GameLogic.CompareCard(m_cbTableCardArray[BANKER_INDEX],2,m_cbTableCardArray[DUI_MEN_INDEX],2)==1?true:false;
	bWinDaoMen=m_GameLogic.CompareCard(m_cbTableCardArray[BANKER_INDEX],2,m_cbTableCardArray[DAO_MEN_INDEX],2)==1?true:false;
}

//���ͼ�¼
void CTableFrameSink::SendGameRecord(IServerUserItem *pIServerUserItem)
{
	WORD wBufferSize=0;
	BYTE cbBuffer[SOCKET_BUFFER];
	int nIndex = m_nRecordFirst;
	while ( nIndex != m_nRecordLast )
	{
		if ((wBufferSize+sizeof(tagServerGameRecord))>sizeof(cbBuffer))
		{
			m_pITableFrame->SendUserData(pIServerUserItem,SUB_S_SEND_RECORD,cbBuffer,wBufferSize);
			wBufferSize=0;
		}
		CopyMemory(cbBuffer+wBufferSize,&m_GameRecordArrary[nIndex],sizeof(tagServerGameRecord));
		wBufferSize+=sizeof(tagServerGameRecord);

		nIndex = (nIndex+1) % MAX_SCORE_HISTORY;
	}
	if (wBufferSize>0) m_pITableFrame->SendUserData(pIServerUserItem,SUB_S_SEND_RECORD,cbBuffer,wBufferSize);
}

//������Ϣ
void CTableFrameSink::SendGameMessage(WORD wChairID, LPCTSTR pszTipMsg)
{
	if (wChairID==INVALID_CHAIR)
	{
		//��Ϸ���
		for (WORD i=0; i<GAME_PLAYER; ++i)
		{
			IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(i);
			if (pIServerUserItem==NULL) continue;
			m_pITableFrame->SendGameMessage(pIServerUserItem,pszTipMsg,SMT_INFO);
		}

		//�Թ����
		WORD wIndex=0;
		do {
			IServerUserItem *pILookonServerUserItem=m_pITableFrame->EnumLookonUserItem(wIndex++);
			if (pILookonServerUserItem==NULL) break;

			m_pITableFrame->SendGameMessage(pILookonServerUserItem,pszTipMsg,SMT_INFO);

		}while(true);
	}
	else
	{
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);
		if (pIServerUserItem!=NULL) m_pITableFrame->SendGameMessage(pIServerUserItem,pszTipMsg,SMT_INFO|SMT_EJECT);
	}
}

#ifdef __BANKER___
bool __cdecl CTableFrameSink::OnActionUserBank(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	return true;
}
#endif

//////////////////////////////////////////////////////////////////////////
