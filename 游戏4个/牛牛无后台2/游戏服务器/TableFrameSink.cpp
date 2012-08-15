#include "StdAfx.h"
#include "TableFrameSink.h"
#include "debuglog.h"

//////////////////////////////////////////////////////////////////////////

//��̬����
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;			//��Ϸ����
const enStartMode	CTableFrameSink::m_GameStartMode=enStartMode_AllReady;	//��ʼģʽ

//////////////////////////////////////////////////////////////////////////

#define	IDI_SO_OPERATE							12							//����ʱ��
#define	TIME_SO_OPERATE							33333						//����ʱ��

//////////////////////////////////////////////////////////////////////////

//���캯��
CTableFrameSink::CTableFrameSink()
{
	//��Ϸ����	
	m_lExitScore=0;	
	m_wBankerUser=INVALID_CHAIR;
	m_wFisrtCallUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_wAdminUser=INVALID_CHAIR;
	m_cbExcuteTimes=0;
	m_cbControlStyle=0;

	//�û�״̬
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	ZeroMemory(m_bPlayStatus,sizeof(m_bPlayStatus));
	ZeroMemory(m_bCallStatus,sizeof(m_bCallStatus));
	for(BYTE i=0;i<m_wPlayerCount;i++){m_bOxCard[i]=0xff;m_bIsUserLeave[i] = FALSE;}

	//�˿˱���
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));

	//��ע��Ϣ
	ZeroMemory(m_lTurnMaxScore,sizeof(m_lTurnMaxScore));

	//�������
	m_pITableFrame=NULL;
	m_pGameServiceOption=NULL;

	return;
}

//��������
CTableFrameSink::~CTableFrameSink(void)
{
}

//�ӿڲ�ѯ--��������Ϣ�汾
void * __cdecl CTableFrameSink::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableFrameSink,Guid,dwQueryVer);
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

	//��ȡ����
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	ASSERT(m_pGameServiceOption!=NULL);

	//��ȡ����
	
	memcpy(m_szGameRoomName, m_pGameServiceOption->szGameRoomName, sizeof(m_szGameRoomName));

	//�����ļ���
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(sizeof(szPath),szPath);
	_snprintf(m_szConfigFileName,sizeof(m_szConfigFileName),TEXT("%s\\OxConfig.ini"),szPath);

	TCHAR OutBuf[255];
	memset(OutBuf,0,255);
	GetPrivateProfileString(m_szGameRoomName,TEXT("StorageStart"),"0",OutBuf,255,m_szConfigFileName);
	_snscanf(OutBuf,strlen(OutBuf),"%I64d",&m_lStockScore);

	return true;
}

//��λ����
void __cdecl CTableFrameSink::RepositTableFrameSink()
{
	//��Ϸ����
	m_lExitScore=0;	
	m_wCurrentUser=INVALID_CHAIR;
	//m_wOperaCount=0;

	//�û�״̬
	//ZeroMemory(m_wWinCount,sizeof(m_wWinCount));		
	//ZeroMemory(m_wWinTimes,sizeof(m_wWinTimes));
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	ZeroMemory(m_bPlayStatus,sizeof(m_bPlayStatus));
	ZeroMemory(m_bCallStatus,sizeof(m_bCallStatus));
	for(BYTE i=0;i<m_wPlayerCount;i++){m_bOxCard[i]=0xff;m_bIsUserLeave[i]= FALSE;}

	//�˿˱���
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));

	//��ע��Ϣ
	ZeroMemory(m_lTurnMaxScore,sizeof(m_lTurnMaxScore));

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
	ASSERT(wChairID<m_wPlayerCount && m_bPlayStatus[wChairID]==TRUE);
	if(wChairID<m_wPlayerCount && m_bPlayStatus[wChairID]==TRUE)return true;
	return false;
}

//��Ϸ��ʼ
bool __cdecl CTableFrameSink::OnEventGameStart()
{
	//����״̬
	m_pITableFrame->SetGameStatus(GS_TK_CALL);

	//�û�״̬
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//��ȡ�û�
		IServerUserItem *pIServerUser=m_pITableFrame->GetServerUserItem(i);

		if(pIServerUser==NULL)
		{
			m_bPlayStatus[i]=FALSE;
		}
		else
		{
			m_bPlayStatus[i]=TRUE;
		}
	}

	//�׾����ʼ��
	if(m_wFisrtCallUser==INVALID_CHAIR)
	{
		m_wFisrtCallUser=rand()%m_wPlayerCount;
	}
	else
	{
		//m_wFisrtCallUser=(m_wFisrtCallUser+1)%m_wPlayerCount;
		//////////////////////////////////////////////////////////////////////////
		//
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			if(m_bPlayStatus[i]==TRUE) 
			{
				if (i == m_wFisrtCallUser) 
					continue;
				else
				{
					m_wFisrtCallUser = i;
					break;
				}
			}
				
		}
		//////////////////////////////////////////////////////////////////////////
		 
	}

	//ʼ���û�
	while(m_bPlayStatus[m_wFisrtCallUser]!=TRUE)
	{
		m_wFisrtCallUser=(m_wFisrtCallUser+1)%m_wPlayerCount;
	}

	//��ǰ�û�
	m_wCurrentUser=m_wFisrtCallUser;

	//���ñ���
	CMD_S_CallBanker CallBanker;
	CallBanker.wCallBanker=m_wCurrentUser;
	CallBanker.bFirstTimes=true;

	//��������
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if(m_bPlayStatus[i]!=TRUE)continue;
		m_pITableFrame->SendTableData(i,SUB_S_CALL_BANKER,&CallBanker,sizeof(CallBanker));
	}
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_CALL_BANKER,&CallBanker,sizeof(CallBanker));
	m_pITableFrame->SetGameTimer(IDI_SO_OPERATE,TIME_SO_OPERATE,1,0);

	return true;
}

//��Ϸ����
bool __cdecl CTableFrameSink::OnEventGameEnd(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_NORMAL:		//�������
		{
			//�������
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));
			WORD m_wWinTimes[GAME_PLAYER],m_wWinCount[GAME_PLAYER];
			ZeroMemory(m_wWinCount,sizeof(m_wWinCount));
			ZeroMemory(m_wWinTimes,sizeof(m_wWinTimes));

			//�����˿�
			BYTE cbUserCardData[GAME_PLAYER][MAX_COUNT];
			CopyMemory(cbUserCardData,m_cbHandCardData,sizeof(cbUserCardData));

			//ׯ�ұ���
			ASSERT(m_bOxCard[m_wBankerUser]<2);
			if(m_bOxCard[m_wBankerUser]==TRUE)
				m_wWinTimes[m_wBankerUser]=m_GameLogic.GetTimes(cbUserCardData[m_wBankerUser],MAX_COUNT);
			else m_wWinTimes[m_wBankerUser]=1;

			//�Ա����
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if(i==m_wBankerUser || m_bPlayStatus[i]==FALSE)continue;

				ASSERT(m_bOxCard[i]<2);

				//�Ա��˿�
				if (m_GameLogic.CompareCard(cbUserCardData[i],cbUserCardData[m_wBankerUser],MAX_COUNT,m_bOxCard[i],m_bOxCard[m_wBankerUser])) 
				{
					m_wWinCount[i]++;
					//��ȡ����
					if(m_bOxCard[i]==TRUE)
						m_wWinTimes[i]=m_GameLogic.GetTimes(cbUserCardData[i],MAX_COUNT);
					else m_wWinTimes[i]=1;
				}
				else
				{
					m_wWinCount[m_wBankerUser]++;
				}
			}

			//ͳ�Ƶ÷�
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if(i==m_wBankerUser || m_bPlayStatus[i]==FALSE)continue;

				WORD j=i;
				if(m_wWinCount[j]>0)	//�м�ʤ��
				{
					GameEnd.lGameScore[j]=m_lTableScore[j]*m_wWinTimes[j];
					GameEnd.lGameScore[m_wBankerUser]-=GameEnd.lGameScore[j];
					m_lTableScore[j]=0;
				}
				else					//ׯ��ʤ��
				{
					GameEnd.lGameScore[j]=(-1)*m_lTableScore[j]*m_wWinTimes[m_wBankerUser];
					GameEnd.lGameScore[m_wBankerUser]+=(-1)*GameEnd.lGameScore[j];
					m_lTableScore[j]=0;
				}
			}

			//�м�ǿ�˷���	
			GameEnd.lGameScore[m_wBankerUser]+=m_lExitScore;

			//������
			if(GameEnd.lGameScore[m_wBankerUser]<0L)
			{
				//ׯ�һ���
				IServerUserItem *pIServerUser=m_pITableFrame->GetServerUserItem(m_wBankerUser);
				LONGLONG lBankerScore=pIServerUser->GetUserScore()->lScore+pIServerUser->GetUserData()->lStorageScore;

				//�����⳥,�ͻ�Ҫ��
				if(lBankerScore+GameEnd.lGameScore[m_wBankerUser]<0L)
				{
					//��ҵ÷����
					LONGLONG lBankerGetScore=lBankerScore;

					//�м��ܵ÷�
					LONGLONG lIdlerGetScore=(-1)*GameEnd.lGameScore[m_wBankerUser];

					//ׯ���ܵ÷�
					for (WORD i=0;i<m_wPlayerCount;i++)
					{
						if(i==m_wBankerUser || m_bPlayStatus[i]==FALSE)continue;
						else if(GameEnd.lGameScore[i]<0L)		
						{
							lBankerGetScore+=(-1)*GameEnd.lGameScore[i];
						}
					}

					//ʧ�ָ��� 1-��ׯ�ұ�Ǯ+Ӯ�мҵ�Ǯ��/��(Ӯ���м���ע��*�мұ���)
					ASSERT(lIdlerGetScore>lBankerGetScore);

					//ͳ�Ƶ÷�
					LONGLONG dTemp = 0;
					for (WORD i=0;i<m_wPlayerCount;i++)
					{
						if(GameEnd.lGameScore[i]>0L)
						{
							dTemp = GameEnd.lGameScore[i]-GameEnd.lGameScore[i]*(lIdlerGetScore-lBankerGetScore)/lIdlerGetScore;
							GameEnd.lGameScore[i] = dTemp;
						}
					}

					//�۳��ܷ�
					GameEnd.lGameScore[m_wBankerUser]=(-1)*lBankerScore;
				}
			}

			//�뿪�û�
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if(m_lTableScore[i]>0)GameEnd.lGameScore[i]=-m_lTableScore[i];
			}

			//��˰����
			WORD cbRevenue=m_pGameServiceOption->wRevenue;

			//����˰��
			for(WORD i=0;i<m_wPlayerCount;i++)
			{
				if(GameEnd.lGameScore[i]>0L)
				{
					GameEnd.lGameTax[i]=LONGLONG(LONGLONG(GameEnd.lGameScore[i])*LONGLONG(cbRevenue)/1000L);
					GameEnd.lGameScore[i]-=GameEnd.lGameTax[i];
				}
			}

			//�Լһ���
			IServerUserItem * pIOtherUserItem =NULL;
			for (WORD i=0;i<m_wPlayerCount;i++){
				pIOtherUserItem = m_pITableFrame->GetServerUserItem(i);
				if(pIOtherUserItem != NULL && pIOtherUserItem->IsAndroidUser() == FALSE)
				{
					GameEnd.lOtherScore = pIOtherUserItem->GetUserScore()->lScore;
					break;
				}
			}

			//������Ϣ
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if(m_bPlayStatus[i]==FALSE)continue;

				m_pITableFrame->SendTableData(i,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			}
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//�޸Ļ���
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if(m_bPlayStatus[i]==FALSE)continue;
				enScoreKind nScoreKind=(GameEnd.lGameScore[i]>0L)?enScoreKind_Win:enScoreKind_Lost;
				m_pITableFrame->WriteUserScore(i,GameEnd.lGameScore[i],GameEnd.lGameTax[i],nScoreKind);
			}

			//���ͳ��
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				//��ȡ�û�
				IServerUserItem * pIServerUserIte=m_pITableFrame->GetServerUserItem(i);
				if ((pIServerUserIte==NULL)||(pIServerUserIte->IsAndroidUser()==false)) continue;
			}

			//������Ϸ
			m_pITableFrame->ConcludeGame();

			return true;
		}
	case GER_USER_LEFT:		//�û�ǿ��
		{

			//Ч�����
			ASSERT(pIServerUserItem!=NULL);
			ASSERT(wChairID<m_wPlayerCount && m_bPlayStatus[wChairID]==TRUE);

			//����״̬
			//m_bPlayStatus[wChairID]=FALSE;

			//�������
			CMD_S_PlayerExit PlayerExit;
			PlayerExit.wPlayerID=wChairID;

			//������Ϣ
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if(i==wChairID || m_bPlayStatus[i]==FALSE)continue;
				m_pITableFrame->SendTableData(i,SUB_S_PLAYER_EXIT,&PlayerExit,sizeof(PlayerExit));
			}
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_PLAYER_EXIT,&PlayerExit,sizeof(PlayerExit));

			//m_bIsUserLeave[wChairID]=TRUE;
			WORD wOtherChair=GetOtherUserChair(wChairID);

			//���Ƿ���ע
			if(m_lTableScore[wChairID]>0L || m_lTableScore[wOtherChair]>0L)
			{
				if(m_bOxCard[wChairID]==0xff)
				{
					BYTE bOx = (m_GameLogic.GetCardType(m_cbHandCardData[wChairID],MAX_COUNT)>0)?TRUE:FALSE;
					OnUserOpenCard(wChairID,bOx);
				}
				if(m_bOxCard[wOtherChair]==0xff)
				{
					BYTE bOx = (m_GameLogic.GetCardType(m_cbHandCardData[wOtherChair],MAX_COUNT)>0)?TRUE:FALSE;
					OnUserOpenCard(wOtherChair,bOx);
				}

				return TRUE;
			}
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//������Ϣ
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if( m_bPlayStatus[i]==FALSE)continue;
				m_pITableFrame->SendTableData(i,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			}
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			m_pITableFrame->ConcludeGame();	
			return TRUE;

/*
			//Ч�����
			ASSERT(pIServerUserItem!=NULL);
			ASSERT(wChairID<m_wPlayerCount && m_bPlayStatus[wChairID]==TRUE);

			//����״̬
			m_bPlayStatus[wChairID]=FALSE;

			//�������
			CMD_S_PlayerExit PlayerExit;
			PlayerExit.wPlayerID=wChairID;

			//������Ϣ
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if(i==wChairID || m_bPlayStatus[i]==FALSE)continue;
				m_pITableFrame->SendTableData(i,SUB_S_PLAYER_EXIT,&PlayerExit,sizeof(PlayerExit));
			}
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_PLAYER_EXIT,&PlayerExit,sizeof(PlayerExit));

			WORD m_wWinTimes[GAME_PLAYER];
			if (m_pITableFrame->GetGameStatus()>GS_TK_CALL)
			{
				if (wChairID==m_wBankerUser)	//ׯ��ǿ��
				{
					//�������
					CMD_S_GameEnd GameEnd;
					ZeroMemory(&GameEnd,sizeof(GameEnd));
					ZeroMemory(m_wWinTimes,sizeof(m_wWinTimes));

					BYTE cbUserCardData[GAME_PLAYER][MAX_COUNT];
					CopyMemory(cbUserCardData,m_cbHandCardData,sizeof(cbUserCardData));

					//�÷ֱ���
					for (WORD i=0;i<m_wPlayerCount;i++)
					{
						if(i==m_wBankerUser || m_bPlayStatus[i]==FALSE)continue;
						m_wWinTimes[i]=(m_pITableFrame->GetGameStatus()!=GS_TK_PLAYING)?(1):(m_GameLogic.GetTimes(cbUserCardData[i],MAX_COUNT));
					}

					//ͳ�Ƶ÷� ���»�û��
					for (WORD i=0;i<m_wPlayerCount;i++)
					{
						if(i==m_wBankerUser || m_bPlayStatus[i]==FALSE)continue;
						GameEnd.lGameScore[i]=m_lTableScore[i]*m_wWinTimes[i];
						GameEnd.lGameScore[m_wBankerUser]-=GameEnd.lGameScore[i];
						m_lTableScore[i]=0;
					}

					//�м�ǿ�˷��� 
					GameEnd.lGameScore[m_wBankerUser]+=m_lExitScore;

					//������
					if(GameEnd.lGameScore[m_wBankerUser]<0L)
					{
						//ׯ�һ���
						IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wBankerUser);
						LONGLONG lBankerScore=pIServerUserItem->GetUserScore()->lScore+pIServerUserItem->GetUserData()->lStorageScore;

						//�����⳥,�ͻ�Ҫ��
						if(lBankerScore+GameEnd.lGameScore[m_wBankerUser]<0L)
						{
							//��ҵ÷����
							LONGLONG lBankerGetScore=lBankerScore;
							LONGLONG lIdlerGetScore=0;
							for (WORD i=0;i<m_wPlayerCount;i++)
							{
								if(i==m_wBankerUser || m_bPlayStatus[i]==FALSE)continue;
								if(GameEnd.lGameScore[i]>0L)			//�м��ܵ÷�
								{
									lIdlerGetScore+=GameEnd.lGameScore[i];
								}
							}

							//ʧ�ָ��� 1-��ׯ�ұ�Ǯ+Ӯ�мҵ�Ǯ��/��(Ӯ���м���ע��*�мұ���)
							ASSERT(lIdlerGetScore>lBankerGetScore);

							//ͳ�Ƶ÷�
							for (WORD i=0;i<m_wPlayerCount;i++)
							{
								if(GameEnd.lGameScore[i]>0L)
								{
									GameEnd.lGameScore[i]-=(GameEnd.lGameScore[i]*
										(lIdlerGetScore-lBankerGetScore)/lIdlerGetScore);
								}
							}

							//�۳��ܷ�
							GameEnd.lGameScore[m_wBankerUser]=(-1)*lBankerScore;
						}
					}

					//�뿪�û�
					for (WORD i=0;i<m_wPlayerCount;i++)
					{
						if(m_lTableScore[i]>0)GameEnd.lGameScore[i]=-m_lTableScore[i];
					}

					//��˰����
					WORD cbRevenue=m_pGameServiceOption->wRevenue;

					//����˰��
					for(WORD i=0;i<m_wPlayerCount;i++)
					{
						if(GameEnd.lGameScore[i]>0L)
						{
							GameEnd.lGameTax[i]=LONGLONG(LONGLONG(GameEnd.lGameScore[i])*LONGLONG(cbRevenue)/1000L);
							GameEnd.lGameScore[i]-=GameEnd.lGameTax[i];
						}
					}

					//������Ϣ
					for (WORD i=0;i<m_wPlayerCount;i++)
					{
						if(i==m_wBankerUser || m_bPlayStatus[i]==FALSE)continue;
						m_pITableFrame->SendTableData(i,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
					}
					m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

					//�޸Ļ���
					for (WORD i=0;i<m_wPlayerCount;i++)
					{
						if(m_bPlayStatus[i]==FALSE && i!=m_wBankerUser)continue;
						enScoreKind nScoreKind=(GameEnd.lGameScore[i]>0L)?enScoreKind_Win:enScoreKind_Lost;
						m_pITableFrame->WriteUserScore(i,GameEnd.lGameScore[i],GameEnd.lGameTax[i],nScoreKind);
					}

					//��ȡ�û�
					IServerUserItem * pIServerUserIte=m_pITableFrame->GetServerUserItem(m_wBankerUser);
			
					//������Ϸ
					m_pITableFrame->ConcludeGame();
				}
				else						//�м�ǿ��
				{
					//�Ѿ���ע
					if (m_lTableScore[wChairID]>0L)
					{
						ZeroMemory(m_wWinTimes,sizeof(m_wWinTimes));

						//�û��˿�
						BYTE cbUserCardData[MAX_COUNT];
						CopyMemory(cbUserCardData,m_cbHandCardData[m_wBankerUser],MAX_COUNT);

						//�û�����
						m_wWinTimes[m_wBankerUser]=(m_pITableFrame->GetGameStatus()==GS_TK_SCORE)?(1):(m_GameLogic.GetTimes(cbUserCardData,MAX_COUNT));

						//�޸Ļ���
						LONGLONG lScore=-m_lTableScore[wChairID]*m_wWinTimes[m_wBankerUser];
						m_lExitScore+=(-1*lScore);
						m_lTableScore[wChairID]=(-1*lScore);
						m_pITableFrame->WriteUserScore(wChairID,lScore,0,enScoreKind_Lost);

						//��ȡ�û�
						IServerUserItem * pIServerUserIte=m_pITableFrame->GetServerUserItem(wChairID);
					}

					//�������
					WORD wUserCount=0;
					for (WORD i=0;i<m_wPlayerCount;i++)if(m_bPlayStatus[i]==TRUE)wUserCount++;

					//������Ϸ
					if(wUserCount==1)
					{
						//�������
						CMD_S_GameEnd GameEnd;
						ZeroMemory(&GameEnd,sizeof(GameEnd));
						ASSERT(m_lExitScore>=0L); 

						//��˰����
						WORD cbRevenue=m_pGameServiceOption->wRevenue;

						//ͳ�Ƶ÷�
						GameEnd.lGameScore[m_wBankerUser]+=m_lExitScore;
						GameEnd.lGameTax[m_wBankerUser]=LONGLONG(LONGLONG(GameEnd.lGameScore[m_wBankerUser])*LONGLONG(cbRevenue)/1000L);
						GameEnd.lGameScore[m_wBankerUser]-=GameEnd.lGameTax[m_wBankerUser];

						//�뿪�û�
						for (WORD i=0;i<m_wPlayerCount;i++)
						{
							if(m_lTableScore[i]>0)GameEnd.lGameScore[i]=-m_lTableScore[i];
						}

						//������Ϣ
						m_pITableFrame->SendTableData(m_wBankerUser,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
						m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

						WORD Zero=0;
						for (;Zero<m_wPlayerCount;Zero++)if(m_lTableScore[Zero]!=0)break;
						if(Zero!=m_wPlayerCount)
						{
							//�޸Ļ���
							LONGLONG lRevenue = GameEnd.lGameTax[m_wBankerUser];
							LONGLONG lScore=GameEnd.lGameScore[m_wBankerUser];
							m_pITableFrame->WriteUserScore(m_wBankerUser,lScore,lRevenue,enScoreKind_Win);

							//��ȡ�û�
							IServerUserItem * pIServerUserIte=m_pITableFrame->GetServerUserItem(wChairID);
						}

						//������Ϸ
						m_pITableFrame->ConcludeGame();		
					}
					else if	(m_pITableFrame->GetGameStatus()==GS_TK_SCORE && m_lTableScore[wChairID]==0L)
					{
						OnUserAddScore(wChairID,0);
					}
					else if (m_pITableFrame->GetGameStatus()==GS_TK_PLAYING && m_bOxCard[wChairID]==0xff)
					{
						OnUserOpenCard(wChairID,0);
					}
				}
			}
			else 
			{
				//�������
				WORD wUserCount=0;
				for (WORD i=0;i<m_wPlayerCount;i++)if(m_bPlayStatus[i]==TRUE)wUserCount++;

				//������Ϸ
				if(wUserCount==1)
				{
					//�������
					CMD_S_GameEnd GameEnd;
					ZeroMemory(&GameEnd,sizeof(GameEnd));

					//������Ϣ
					for (WORD i=0;i<m_wPlayerCount;i++)
					{
						if(i==m_wBankerUser || m_bPlayStatus[i]==FALSE)continue;
						m_pITableFrame->SendTableData(i,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
					}
					m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

					//������Ϸ
					m_pITableFrame->ConcludeGame();			
				}
				else if(m_wCurrentUser==wChairID)OnUserCallBanker(wChairID,0);
			}
*/
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
	case GS_FREE:		//����״̬
		{
			//��������
			CMD_S_StatusFree StatusFree;
			ZeroMemory(&StatusFree,sizeof(StatusFree));

			//�Լһ���
			IServerUserItem * pIOtherUserItem =NULL;
			for (WORD i=0;i<m_wPlayerCount;i++){
				pIOtherUserItem = m_pITableFrame->GetServerUserItem(i);
				if(pIOtherUserItem ==NULL || pIOtherUserItem== pIServerUserItem) continue;
				if(!pIOtherUserItem->IsAndroidUser())
					StatusFree.lOtherScore = pIOtherUserItem->GetUserScore()->lScore;
				break;
			}

			//��������
			CopyMemory(StatusFree.szGameRoomName, m_pGameServiceOption->szGameRoomName, sizeof(StatusFree.szGameRoomName));
			//���ñ���
			StatusFree.lCellScore=0L;

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	case GS_TK_CALL:	//��ׯ״̬
		{
			//��������
			CMD_S_StatusCall StatusCall;
			ZeroMemory(&StatusCall,sizeof(StatusCall));

			//���ñ���
			StatusCall.wCallBanker=m_wCurrentUser;

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusCall,sizeof(StatusCall));
		}
	case GS_TK_SCORE:	//��ע״̬
		{
			//��������
			CMD_S_StatusScore StatusScore;
			memset(&StatusScore,0,sizeof(StatusScore));

			//��ע��Ϣ
			StatusScore.lTurnMaxScore=m_lTurnMaxScore[wChiarID];
			StatusScore.wBankerUser=m_wBankerUser;

			//���û���
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if(m_bPlayStatus[i]==FALSE)continue;
				StatusScore.lTableScore[i]=m_lTableScore[i];
			}

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusScore,sizeof(StatusScore));
		}
	case GS_TK_PLAYING:	//��Ϸ״̬
		{
			//��������
			CMD_S_StatusPlay StatusPlay;
			memset(&StatusPlay,0,sizeof(StatusPlay));

			//������Ϣ
			StatusPlay.lTurnMaxScore=m_lTurnMaxScore[wChiarID];
			StatusPlay.wBankerUser=m_wBankerUser;
			CopyMemory(StatusPlay.bOxCard,m_bOxCard,sizeof(StatusPlay.bOxCard));

			//�����˿�
			for (WORD i=0;i< m_wPlayerCount;i++)
			{
				if(m_bPlayStatus[i]==FALSE)continue;
				WORD j= i;
				StatusPlay.lTableScore[j]=m_lTableScore[j];
				CopyMemory(StatusPlay.cbHandCardData[j],m_cbHandCardData[j],MAX_COUNT);
			}

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));
		}
	}
	//Ч�����
	ASSERT(FALSE);

	return false;
}

//��ʱ���¼�
bool __cdecl CTableFrameSink::OnTimerMessage(WORD wTimerID, WPARAM wBindParam)
{
	switch(wTimerID)
	{
	case IDI_SO_OPERATE:
		{
			//ɾ��ʱ��
			m_pITableFrame->KillGameTimer(IDI_SO_OPERATE);

			//��Ϸ״̬
			switch( m_pITableFrame->GetGameStatus() )
			{
			case GS_TK_CALL:			//�û���ׯ
				{
					OnUserCallBanker(m_wCurrentUser, 0);
					break;
				}
			case GS_TK_SCORE:			//��ע����
				{
					for(WORD i=0;i<m_wPlayerCount;i++)
					{
						if(m_lTableScore[i]>0L || m_bPlayStatus[i]==FALSE || i==m_wBankerUser)continue;
						OnUserAddScore(i,m_lTurnMaxScore[i]/8);
					}

					break;
				}
			case GS_TK_PLAYING:			//�û�����
				{
					for(WORD i=0;i<m_wPlayerCount;i++)
					{
						if(m_bOxCard[i]<2 || m_bPlayStatus[i]==FALSE)continue;
						OnUserOpenCard(i, 0);
					}

					break;
				}
			default:
				{
					break;
				}
			}

			if(m_pITableFrame->GetGameStatus()!=GS_TK_FREE)
				m_pITableFrame->SetGameTimer(IDI_SO_OPERATE,TIME_SO_OPERATE,1,0);
			return true;
		}
	}

	return false;
}

//��Ϸ��Ϣ���� 
bool __cdecl CTableFrameSink::OnGameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	bool bResult=false;
	switch (wSubCmdID)
	{
	case SUB_C_CALL_BANKER:			//�û���ׯ
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_CallBanker));
			if (wDataSize!=sizeof(CMD_C_CallBanker)) return false;

			//��������
			CMD_C_CallBanker * pCallBanker=(CMD_C_CallBanker *)pDataBuffer;

			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;

			//״̬�ж�
			ASSERT(IsUserPlaying(pUserData->wChairID));
			if (!IsUserPlaying(pUserData->wChairID)) return true;

			//��Ϣ����
			bResult=OnUserCallBanker(pUserData->wChairID,pCallBanker->bBanker);
			break;
		}
	case SUB_C_ADD_SCORE:			//�û���ע
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_AddScore));
			if (wDataSize!=sizeof(CMD_C_AddScore)) return false;

			//��������
			CMD_C_AddScore * pAddScore=(CMD_C_AddScore *)pDataBuffer;

			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;

			//״̬�ж�
			ASSERT(IsUserPlaying(pUserData->wChairID));
			if (!IsUserPlaying(pUserData->wChairID)) return true;

			//��Ϣ����
			bResult=OnUserAddScore(pUserData->wChairID,pAddScore->lScore);
			break;
		}
	case SUB_C_OPEN_CARD:			//�û�̯��
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_OxCard));
			if (wDataSize!=sizeof(CMD_C_OxCard)) return false;

			//��������
			CMD_C_OxCard * pOxCard=(CMD_C_OxCard *)pDataBuffer;

			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;

			//״̬�ж�
			ASSERT(m_bPlayStatus[pUserData->wChairID]!=FALSE);
			if(m_bPlayStatus[pUserData->wChairID]==FALSE)return true;

			//��Ϣ����
			bResult=OnUserOpenCard(pUserData->wChairID,pOxCard->bOX);
			break;

		}
	case SUB_C_AMDIN_COMMAND:
		{
			ASSERT(wDataSize==sizeof(CMD_C_AdminReq));
			if(wDataSize!=sizeof(CMD_C_AdminReq)) return false;
			return OnSubAmdinCommand(pIServerUserItem,pDataBuffer);
		}

	}

	//������ʱ��
	if(bResult)
	{
		m_pITableFrame->SetGameTimer(IDI_SO_OPERATE,TIME_SO_OPERATE,1,0);
		return true;
	}

	return false;
}

//�����Ϣ����
bool __cdecl CTableFrameSink::OnFrameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}


//��ׯ�¼�
bool CTableFrameSink::OnUserCallBanker(WORD wChairID, BYTE bBanker)
{
	//״̬Ч��
	ASSERT(m_pITableFrame->GetGameStatus()==GS_TK_CALL);
	if (m_pITableFrame->GetGameStatus()!=GS_TK_CALL) return true;
	ASSERT(m_wCurrentUser==wChairID);
	if (m_wCurrentUser!=wChairID) return true;

	//���ñ���
	m_bCallStatus[wChairID]=TRUE;

	//////////////////////////////////////////////////////////////////////////
	//��ׯ����
	WORD wCallUserCount=0;
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if(m_bPlayStatus[i]==TRUE && m_bCallStatus[i]==TRUE) 
			wCallUserCount++;
// 		else if(m_bPlayStatus[i]!=TRUE) 
// 			wCallUserCount++;
	}
	//////////////////////////////////////////////////////////////////////////


	//��ע��ʼ
	if(bBanker==TRUE || wCallUserCount== 2/*m_wPlayerCount*/ )
	{

		//////////////////////////////////////////////////////////////////////////
		//���ﴦ�����һ��ȡ����ׯ
		if(wCallUserCount == 2 && (bBanker != TRUE))
		{
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if(m_bPlayStatus[i]==TRUE) 
				{
					if (i == wChairID) 
						continue;
					else
					{
						m_wBankerUser = i;
						break;
					}
				}

			}
		}
		else
			m_wBankerUser=wChairID;
		//////////////////////////////////////////////////////////////////////////
		//ʼ���û�
		//m_wBankerUser=wChairID;
		m_wCurrentUser=INVALID_CHAIR;

		//�������һ����ׯ�û�ǿ�����
		while(m_bPlayStatus[m_wBankerUser]==FALSE)
		{
			m_wBankerUser=(m_wBankerUser+1)%GAME_PLAYER;
		}

		//����״̬
		m_pITableFrame->SetGameStatus(GS_TK_SCORE);

		//ׯ�һ���
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wBankerUser);
		LONGLONG lBankerScore=pIServerUserItem->GetUserScore()->lScore;

		//�������
		WORD wUserCount=0;
		for (WORD i=0;i<m_wPlayerCount;i++)if(m_bPlayStatus[i]==TRUE )wUserCount++;

		//�����ע
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			if(m_bPlayStatus[i]!=TRUE || i==m_wBankerUser)continue;

			//��ȡ�û�
			pIServerUserItem=m_pITableFrame->GetServerUserItem(i);

			//��ȡ����
			const tagUserScore * pUserScore=pIServerUserItem->GetUserScore();

			//��Ҽ���
			ASSERT(pUserScore->lScore>=m_pGameServiceOption->lCellScore);

			//��ע���� �ͻ�Ҫ��
			m_lTurnMaxScore[i]=__min(lBankerScore/(wUserCount-1)/5,pUserScore->lScore/5);
		}

		//���ñ���
		//m_wOperaCount=0;
		CMD_S_GameStart GameStart;
		GameStart.wBankerUser=m_wBankerUser;
		GameStart.lTurnMaxScore=0;

		//��������
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			if(m_bPlayStatus[i]!=TRUE)continue;
			GameStart.lTurnMaxScore=m_lTurnMaxScore[i];
			m_pITableFrame->SendTableData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
		}
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
	}
	else		 //�û���ׯ
	{
		//�����¸����
		do{
			m_wCurrentUser=(m_wCurrentUser+1)%m_wPlayerCount;
		}while(m_bPlayStatus[m_wCurrentUser]==FALSE);

		//���ñ���
		CMD_S_CallBanker CallBanker;
		CallBanker.wCallBanker=m_wCurrentUser;
		CallBanker.bFirstTimes=false;

		//��������
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			if(m_bPlayStatus[i]!=TRUE)continue;
			m_pITableFrame->SendTableData(i,SUB_S_CALL_BANKER,&CallBanker,sizeof(CallBanker));
		}
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_CALL_BANKER,&CallBanker,sizeof(CallBanker));
	}
	return true;
}

//��ȡ�Է����
WORD CTableFrameSink::GetOtherUserChair( WORD wUserChair )
{
	WORD wOtherUser=INVALID_CHAIR;

	for (WORD i=0;i<m_wPlayerCount;i++){
		if(m_bPlayStatus[i]==TRUE) {
			if (i == wUserChair) 
				continue;
			else{
				wOtherUser = i;
				break;
			}
		}
	}

	return wOtherUser;
}


//��ע�¼�
bool CTableFrameSink::OnUserAddScore(WORD wChairID, LONGLONG lScore)
{
	//״̬Ч��
	ASSERT(m_pITableFrame->GetGameStatus()==GS_TK_SCORE);
	if (m_pITableFrame->GetGameStatus()!=GS_TK_SCORE) return true;

	//���Ч��
	if(m_bPlayStatus[wChairID]==TRUE)
	{
		ASSERT(lScore>0 && lScore<=m_lTurnMaxScore[wChairID]);
		if (lScore<=0 || lScore>m_lTurnMaxScore[wChairID]) return false;
	}
	else //û��ע���ǿ��
	{
		ASSERT(lScore==0);
		if (lScore!=0) return false;
	}

	if(lScore>0L)
	{
		//��ע���
		m_lTableScore[wChairID]=lScore;

		//��������
		CMD_S_AddScore AddScore;
		AddScore.wAddScoreUser=wChairID;
		AddScore.lAddScoreCount=m_lTableScore[wChairID];

		//��������
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			if(m_bPlayStatus[i]==FALSE)continue;
			m_pITableFrame->SendTableData(i,SUB_S_ADD_SCORE,&AddScore,sizeof(AddScore));
		}
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_ADD_SCORE,&AddScore,sizeof(AddScore));
	}

	//��ע����
	BYTE bUserCount=0;
	for(WORD i=0;i<m_wPlayerCount;i++)
	{
		if(m_lTableScore[i]>0L && m_bPlayStatus[i]==TRUE)bUserCount++;
		else if(m_bPlayStatus[i]==FALSE || i==m_wBankerUser)bUserCount++;
	}

	//�м�ȫ��
	if(bUserCount==m_wPlayerCount)
	{
		//����״̬
		m_pITableFrame->SetGameStatus(GS_TK_PLAYING);

		//��������
		CMD_S_SendCard SendCard;
		ZeroMemory(SendCard.cbCardData,sizeof(SendCard.cbCardData));

		//��ȡ�˿�
		BYTE bTempArray[GAME_PLAYER*MAX_COUNT];
		m_GameLogic.RandCardList(bTempArray,sizeof(bTempArray));
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			if(m_bPlayStatus[i]==FALSE)continue;

			//�ɷ��˿�
			CopyMemory(m_cbHandCardData[i],&bTempArray[i*MAX_COUNT],MAX_COUNT);
		}
		
		int nWiner = GetPrivateProfileInt(m_szGameRoomName,TEXT("WinerGL"), 50, m_szConfigFileName);
		srand( GetTickCount());
		
		//////////////////////////////////////////////////////////////////////////
		
		if (m_cbExcuteTimes > 0 && m_wAdminUser != INVALID_CHAIR)
		{
			WORD wOtherUser=GetOtherUserChair(m_wAdminUser);

			if(m_cbControlStyle == CS_BANKER_WIN)
				ChangeCard(m_wAdminUser, wOtherUser);//AnalyseAdminCard(m_wAdminUser, true);
			else
				ChangeCard(wOtherUser ,m_wAdminUser);//AnalyseAdminCard(m_wAdminUser, false);
			m_cbExcuteTimes--;
		}else
		{
			//�����˻����������ڿ�����
			if ((rand()%100)>=(100-nWiner))
				AnalyseUserCard();
			else
				AnalyseAndroidCard();
		}

		int i0=GetPrivateProfileInt(m_szGameRoomName,TEXT("Card0"), 0, m_szConfigFileName);
		if(i0!=0)
		{
			m_cbHandCardData[0][0]=i0;
			m_cbHandCardData[0][1]=GetPrivateProfileInt(m_szGameRoomName,TEXT("Card1"), 0, m_szConfigFileName);
			m_cbHandCardData[0][2]=GetPrivateProfileInt(m_szGameRoomName,TEXT("Card2"), 0, m_szConfigFileName);
			m_cbHandCardData[0][3]=GetPrivateProfileInt(m_szGameRoomName,TEXT("Card3"), 0, m_szConfigFileName);
			m_cbHandCardData[0][4]=GetPrivateProfileInt(m_szGameRoomName,TEXT("Card4"), 0, m_szConfigFileName);
			//m_cbHandCardData[0][0]=GetPrivateProfileInt(m_szGameRoomName,TEXT("Card0"), 0, m_szConfigFileName);
		}

		//�����˿�
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			if(m_bPlayStatus[i]==FALSE)continue;

			//�ɷ��˿�
			CopyMemory(SendCard.cbCardData[i],m_cbHandCardData[i],MAX_COUNT);
		}

		//��������
		for (WORD i=0;i< m_wPlayerCount;i++)
		{
			if(m_bPlayStatus[i]==FALSE)continue;
			m_pITableFrame->SendTableData(i,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));
		}
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));
	}

	return true;
}

//̯���¼�
bool CTableFrameSink::OnUserOpenCard(WORD wChairID, BYTE bOx)
{
	//״̬Ч��
	ASSERT (m_pITableFrame->GetGameStatus()==GS_TK_PLAYING);
	if (m_pITableFrame->GetGameStatus()!=GS_TK_PLAYING) return true;
	if (m_bOxCard[wChairID]!=0xff) return true;

	//Ч������
	ASSERT(bOx<=TRUE);
	if(bOx>TRUE)return false;

	//////////////////////////////////////////////////////////////////////////
	bOx = (m_GameLogic.GetCardType(m_cbHandCardData[wChairID],MAX_COUNT)>0)?TRUE:FALSE;
	//////////////////////////////////////////////////////////////////////////

	//Ч������
	if(bOx)
	{
		ASSERT(m_GameLogic.GetCardType(m_cbHandCardData[wChairID],MAX_COUNT)>0);
		if(!(m_GameLogic.GetCardType(m_cbHandCardData[wChairID],MAX_COUNT)>0))return false;
	}
	else if(m_bPlayStatus[wChairID]==TRUE)
	{
		if(m_GameLogic.GetCardType(m_cbHandCardData[wChairID],MAX_COUNT)>=OX_THREE_SAME)bOx=TRUE;
	}

	//ţţ����
	m_bOxCard[wChairID] = bOx;

	//̯������
	BYTE bUserCount=0;
	for(WORD i=0;i<m_wPlayerCount;i++)
	{
		if(m_bOxCard[i]<2 && m_bPlayStatus[i]==TRUE)bUserCount++;
		else if(m_bPlayStatus[i]==FALSE)bUserCount++;
	}

	 //�������
	CMD_S_Open_Card OpenCard;
	ZeroMemory(&OpenCard,sizeof(OpenCard));

	//���ñ���
	OpenCard.bOpen=bOx;
	OpenCard.wPlayerID=wChairID;

//	&& m_bIsUserLeave[1] == false && m_bIsUserLeave[0] == false

	bool bUserLeave=false;
	//��������
	for (WORD i=0;i< m_wPlayerCount;i++)
	{
		if(m_bIsUserLeave[i] == TRUE) bUserCount=true;
		if(m_bPlayStatus[i]==FALSE)continue;
		m_pITableFrame->SendTableData(i,SUB_S_OPEN_CARD,&OpenCard,sizeof(OpenCard));
	}
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OPEN_CARD,&OpenCard,sizeof(OpenCard));	

	
	//������Ϸ
	if(bUserCount == m_wPlayerCount && bUserLeave==false)
	{
		return OnEventGameEnd(INVALID_CHAIR,NULL,GER_NORMAL);
	}

	return true;
}

//�˿˷���
void CTableFrameSink::AnalyseAndroidCard()
{
	//��������
	bool bIsAiBanker = false;
	WORD wAiCount = 0;
	WORD wPlayerCount = 0;
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//��ȡ�û�
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(i);
		if (pIServerUserItem!=NULL)
		{
			ASSERT(m_bPlayStatus[i]==TRUE);
			if(pIServerUserItem->IsAndroidUser()) 
			{
				wAiCount++ ;
				if(!bIsAiBanker && i==m_wBankerUser)bIsAiBanker = true;
			}
			wPlayerCount++; 
		}
	}

	//ȫ������
	if(wPlayerCount == wAiCount || wAiCount==0)return;
//////////////////////////////////////////////////////////////////////////

	//�Ƿ����
	bool bIsAndroidWin= false;
	int nWiner = GetPrivateProfileInt(m_szGameRoomName,TEXT("AndroidWinerGL"), 50, m_szConfigFileName);
	srand( GetTickCount()+m_pITableFrame->GetPlayTimeCount());
	if ((rand()%100)>=(100-nWiner))		
		bIsAndroidWin = true;
//////////////////////////////////////////////////////////////////////////

	WORD wOtherChair=GetOtherUserChair(m_wBankerUser);

	//����Ӯ��������Ӯ
	if(bIsAndroidWin)
	{
		if(!bIsAiBanker)
		{
			ChangeCard(wOtherChair ,m_wBankerUser);
		}
		else
		{
			ChangeCard(m_wBankerUser ,wOtherChair);	
		}
	}else
	{
		if(bIsAiBanker)
			ChangeCard(wOtherChair ,m_wBankerUser);
		else
			ChangeCard(m_wBankerUser ,wOtherChair);

	}
	return ;
//////////////////////////////////////////////////////////////////////////

	//�˿˱���
	BYTE cbUserCardData[GAME_PLAYER][MAX_COUNT];
	CopyMemory(cbUserCardData,m_cbHandCardData,sizeof(m_cbHandCardData));

	//ţţ����
	BOOL bUserOxData[GAME_PLAYER];
	ZeroMemory(bUserOxData,sizeof(bUserOxData));
	for(WORD i=0;i<GAME_PLAYER;i++)
	{
		if(m_bPlayStatus[i]!=TRUE)continue;
		bUserOxData[i] = (m_GameLogic.GetCardType(cbUserCardData[i],MAX_COUNT)>0)?TRUE:FALSE;
	}

	//�����˿�
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		m_GameLogic.SortCardList(cbUserCardData[i],MAX_COUNT);
	}

	//��������
	LONGLONG lAndroidScore=0;

	//��������
	BYTE cbCardTimes[GAME_PLAYER];
	ZeroMemory(cbCardTimes,sizeof(cbCardTimes));

	//���ұ���
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if (m_bPlayStatus[i]==TRUE)
		{
			cbCardTimes[i]=m_GameLogic.GetTimes(cbUserCardData[i],MAX_COUNT);
		}
	}

	//����ׯ��
	if(bIsAiBanker)
	{
		//�Ա��˿�
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			//�û�����
			if ((i==m_wBankerUser)||(m_bPlayStatus[i]==FALSE)) continue;

			//��ȡ�û�
			IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(i);

			//��������
			if ((pIServerUserItem!=NULL)&&(pIServerUserItem->IsAndroidUser())) continue;

			//�Ա��˿�
			if (m_GameLogic.CompareCard(cbUserCardData[i],cbUserCardData[m_wBankerUser],MAX_COUNT,bUserOxData[i],bUserOxData[m_wBankerUser])==true)
			{
				lAndroidScore-=cbCardTimes[i]*m_lTableScore[i];
			}
			else
			{
				lAndroidScore+=cbCardTimes[m_wBankerUser]*m_lTableScore[i];
			}
		}
	}
	else//�û�ׯ��
	{
		//�Ա��˿�
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			//��ȡ�û�
			IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(i);

			//�û�����
			if ((i==m_wBankerUser)||(pIServerUserItem==NULL)||!(pIServerUserItem->IsAndroidUser())) continue;

			//�Ա��˿�
			if (m_GameLogic.CompareCard(cbUserCardData[i],cbUserCardData[m_wBankerUser],MAX_COUNT,bUserOxData[i],bUserOxData[m_wBankerUser])==true)
			{
				lAndroidScore+=cbCardTimes[i]*m_lTableScore[i];
			}
			else
			{
				lAndroidScore-=cbCardTimes[m_wBankerUser]*m_lTableScore[i];
			}
		}
	}

	//����ж�
	if ((m_lStockScore+lAndroidScore)<0)
	{
		//��������
		WORD wWinUser=INVALID_CHAIR;

		//�ж����
		bool bCardMax = (bIsAiBanker)?true:false;

		//��������
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			//�û�����
			if (m_bPlayStatus[i]==FALSE) continue;

			//�����û�
			if (wWinUser==INVALID_CHAIR)
			{
				wWinUser=i;
				continue;
			}

			//�Ա��˿�
			if (m_GameLogic.CompareCard(cbUserCardData[i],m_cbHandCardData[wWinUser],MAX_COUNT,bUserOxData[i],bUserOxData[wWinUser])==bCardMax)
			{
				wWinUser=i;
			}
		}

		//��������
		BYTE cbTempData[MAX_COUNT];
		CopyMemory(cbTempData,m_cbHandCardData[m_wBankerUser],MAX_COUNT);
		CopyMemory(m_cbHandCardData[m_wBankerUser],m_cbHandCardData[wWinUser],MAX_COUNT);
		CopyMemory(m_cbHandCardData[wWinUser],cbTempData,MAX_COUNT);
	}

	return;
}


void CTableFrameSink::AnalyseUserCard()
{
	//���ƴ���
	bool bBreak = false;
	bool bIsAiBanker = false;
	WORD wWinIndex = INVALID_CHAIR;
	WORD wLoserIndex = INVALID_CHAIR;
	int nAccountsCount = 0;
	//�˿˱���
	BYTE cbUserCardData[GAME_PLAYER][MAX_COUNT];
	CopyMemory(cbUserCardData,m_cbHandCardData,sizeof(m_cbHandCardData));

	//ţţ����
	BOOL bUserOxData[GAME_PLAYER];
	ZeroMemory(bUserOxData,sizeof(bUserOxData));
	for(WORD i=0;i<GAME_PLAYER;i++)
	{
		if(m_bPlayStatus[i]!=TRUE)continue;
		bUserOxData[i] = (m_GameLogic.GetCardType(cbUserCardData[i],MAX_COUNT)>0)?TRUE:FALSE;
	}

	//����
	for (WORD i=0; i<GAME_PLAYER; i++)
	{
		if(m_bPlayStatus[i]==FALSE)continue;
		//��ȡ�û�
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(i);
		if (pIServerUserItem==NULL) continue;

		nAccountsCount = GetPrivateProfileInt(m_szGameRoomName,TEXT("AccountsCount"), 0, m_szConfigFileName);
	
		if (nAccountsCount<=0) break;
		for (int j= 0; j < nAccountsCount; j++)
		{
			TCHAR szAccounts[NAME_LEN] = TEXT("");
			TCHAR szTmp[128] = TEXT("");
			_snprintf(szTmp, sizeof(szTmp), TEXT("Accounts%d"), j+1);
			GetPrivateProfileString(m_szGameRoomName,szTmp,TEXT(""),szAccounts,sizeof(szAccounts),m_szConfigFileName);

			if (lstrcmp(pIServerUserItem->GetAccounts(),szAccounts)==0)
			{
				if(!bIsAiBanker && i==m_wBankerUser) bIsAiBanker = true;
				//ʤ�����
				wWinIndex=i;
				wLoserIndex=wWinIndex==0?1:0;
				bBreak = true;
				break;
			}
		}
		if (bBreak) break;
	}

	//�����˿�
	if (nAccountsCount>0 && bBreak)
	{
		//��������
		WORD wWinUser=INVALID_CHAIR;

		//�ж����
		bool bCardMax = (bIsAiBanker)?true:false;

		//��������
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			//�û�����
			if (m_bPlayStatus[i]==FALSE) continue;

			//�����û�
			if (wWinUser==INVALID_CHAIR)
			{
				wWinUser=i;
				continue;
			}

			//�Ա��˿�
			if (m_GameLogic.CompareCard(cbUserCardData[i],m_cbHandCardData[wWinUser],MAX_COUNT,bUserOxData[i],bUserOxData[wWinUser])==bCardMax)
			{
				wWinUser=i;
			}
		}

		//��������
		BYTE cbTempData[MAX_COUNT];
		CopyMemory(cbTempData,m_cbHandCardData[m_wBankerUser],MAX_COUNT);
		CopyMemory(m_cbHandCardData[m_wBankerUser],m_cbHandCardData[wWinUser],MAX_COUNT);
		CopyMemory(m_cbHandCardData[wWinUser],cbTempData,MAX_COUNT);
	}
	else
		AnalyseAndroidCard();
	return;
}

//�����˿�
bool CTableFrameSink::ChangeCard(WORD Winer, WORD Loser)
{
	//BYTE cbUserCardData[GAME_PLAYER][MAX_COUNT];
	//CopyMemory(cbUserCardData,m_cbHandCardData,sizeof(m_cbHandCardData));

	if(Winer == INVALID_CHAIR || Loser == INVALID_CHAIR) return false;

	//ţţ����
	BOOL bUserOxData[GAME_PLAYER];
	ZeroMemory(bUserOxData,sizeof(bUserOxData));
	for(WORD i=0;i<GAME_PLAYER;i++)
	{
		if(m_bPlayStatus[i]!=TRUE)continue;
		bUserOxData[i] = (m_GameLogic.GetCardType(m_cbHandCardData[i],MAX_COUNT)>0)?TRUE:FALSE;
	}

	//�����˿�
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		m_GameLogic.SortCardList(m_cbHandCardData[i],MAX_COUNT);
	}

	//�Ա��˿� �����û���С
	if (m_GameLogic.CompareCard(m_cbHandCardData[Winer],m_cbHandCardData[Loser],MAX_COUNT,bUserOxData[Winer],bUserOxData[Loser])==false)
	{
		//��������
		BYTE cbTempData[MAX_COUNT];
		CopyMemory(cbTempData,m_cbHandCardData[Winer],MAX_COUNT);
		CopyMemory(m_cbHandCardData[Winer],m_cbHandCardData[Loser],MAX_COUNT);
		CopyMemory(m_cbHandCardData[Loser],cbTempData,MAX_COUNT);
	}

	return true;
}

bool CTableFrameSink::OnSubAmdinCommand(IServerUserItem*pIServerUserItem,const void*pDataBuffer)
{

	//��������й���ԱȨ�� �򷵻ش���
	if((pIServerUserItem->GetUserData()->dwUserRight&UR_GAME_CONTROL)==0)
	{
		return false;
	}

	m_wAdminUser=pIServerUserItem->GetChairID();

	const CMD_C_AdminReq* AdminReq=static_cast<const CMD_C_AdminReq*>(pDataBuffer);
	switch(AdminReq->cbReqType)
	{
	case RQ_RESET_CONTROL:
		{	
			m_cbControlStyle=0;
			m_cbExcuteTimes=0;

			CMD_S_CommandResult cResult;
			cResult.cbResult=CR_ACCEPT;
			cResult.cbAckType=ACK_RESET_CONTROL;
			m_pITableFrame->SendUserData(pIServerUserItem,SUB_S_AMDIN_COMMAND,&cResult,sizeof(CMD_S_CommandResult));
			break;
		}
	case RQ_SET_WIN_AREA:
		{

			const tagAdminReq*pAdminReq=reinterpret_cast<const tagAdminReq*>(AdminReq->cbExtendData);
			switch(pAdminReq->m_cbControlStyle)
			{
			case CS_BANKER_LOSE:	//ׯ������
				{
					m_cbControlStyle=pAdminReq->m_cbControlStyle;
					m_cbExcuteTimes=pAdminReq->m_cbExcuteTimes;
					CMD_S_CommandResult cResult;
					cResult.cbResult=CR_ACCEPT;
					cResult.cbAckType=ACK_SET_WIN_AREA;
					m_pITableFrame->SendUserData(pIServerUserItem,SUB_S_AMDIN_COMMAND,&cResult,sizeof(CMD_S_CommandResult));
					
					break;
				}
			case CS_BANKER_WIN:		//ׯ��Ӯ��
				{
					
					m_cbControlStyle=pAdminReq->m_cbControlStyle;
					m_cbExcuteTimes=pAdminReq->m_cbExcuteTimes;
					CMD_S_CommandResult cResult;
					cResult.cbResult=CR_ACCEPT;
					cResult.cbAckType=ACK_SET_WIN_AREA;
					m_pITableFrame->SendUserData(pIServerUserItem,SUB_S_AMDIN_COMMAND,&cResult,sizeof(CMD_S_CommandResult));
					break;
				}
			default:	//�ܾ�����
				{
					CMD_S_CommandResult cResult;
					cResult.cbResult=CR_REFUSAL;
					cResult.cbAckType=ACK_SET_WIN_AREA;
					m_pITableFrame->SendUserData(pIServerUserItem,SUB_S_AMDIN_COMMAND,&cResult,sizeof(CMD_S_CommandResult));
					break;
				}
			}

			break;
		}
	case RQ_PRINT_SYN:
		{
			CMD_S_CommandResult cResult;
			cResult.cbResult=CR_ACCEPT;
			cResult.cbAckType=ACK_PRINT_SYN;
			tagAdminReq*pAdminReq=reinterpret_cast<tagAdminReq*>(cResult.cbExtendData);
			pAdminReq->m_cbControlStyle=m_cbControlStyle;
			pAdminReq->m_cbExcuteTimes=m_cbExcuteTimes;
			m_pITableFrame->SendUserData(pIServerUserItem,SUB_S_AMDIN_COMMAND,&cResult,sizeof(CMD_S_CommandResult));
			break;
		}
	default:
		{
			break;
		}


	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
