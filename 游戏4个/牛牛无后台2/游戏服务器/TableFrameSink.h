#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//��Ϸ������
class CTableFrameSink : public ITableFrameSink
{
	//��Ϸ����
protected:
	WORD							m_wBankerUser;							//ׯ���û�
	WORD							m_wFisrtCallUser;						//ʼ���û�
	WORD							m_wCurrentUser;							//��ǰ�û�
	LONGLONG						m_lExitScore;							//ǿ�˷���

	//�û�����
protected:
	BYTE							m_bPlayStatus[GAME_PLAYER];				//��Ϸ״̬
	BYTE							m_bCallStatus[GAME_PLAYER];				//��ׯ״̬
	BYTE							m_bOxCard[GAME_PLAYER];					//ţţ����
	LONGLONG						m_lTableScore[GAME_PLAYER];				//��ע��Ŀ
	BYTE							m_bIsUserLeave[GAME_PLAYER];			//����뿪

	//�˿˱���
protected:
	BYTE							m_cbHandCardData[GAME_PLAYER][MAX_COUNT];//�����˿�

	//��ע��Ϣ
protected:
	LONGLONG						m_lTurnMaxScore[GAME_PLAYER];			//�����ע

	//AI����
protected:
	CHAR							m_szGameRoomName[SERVER_LEN];
	CHAR							m_szConfigFileName[MAX_PATH];
	LONGLONG						m_lStockScore;							//����Ӯ��
	LONGLONG						m_lStorageDeduct;							//�ؿ۱���
	//��ҿ���						
protected:
	BYTE							m_cbExcuteTimes;								//ִ�д���
	BYTE							m_cbControlStyle;								//���Ʒ�ʽ
	WORD							m_wAdminUser;							//Ӯ��ID

	//�������
protected:
	CGameLogic						m_GameLogic;							//��Ϸ�߼�
	ITableFrame						* m_pITableFrame;						//��ܽӿ�
	const tagGameServiceOption		* m_pGameServiceOption;					//���ò���

	//���Ա���
protected:
	static const WORD				m_wPlayerCount;							//��Ϸ����
	static const enStartMode		m_GameStartMode;						//��ʼģʽ

	//��������
public:
	//���캯��
	CTableFrameSink();
	//��������
	virtual ~CTableFrameSink();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID __cdecl Release() { }
	//�Ƿ���Ч
	virtual bool __cdecl IsValid() { return AfxIsValidAddress(this,sizeof(CTableFrameSink))?true:false; }
	//�ӿڲ�ѯ
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//����ӿ�
public:
	//��ʼ��
	virtual bool __cdecl InitTableFrameSink(IUnknownEx * pIUnknownEx);
	//��λ����
	virtual void __cdecl RepositTableFrameSink();

	//��Ϣ�ӿ�
public:
	//��ʼģʽ
	virtual enStartMode __cdecl GetGameStartMode();
	//��Ϸ״̬
	virtual bool __cdecl IsUserPlaying(WORD wChairID);

	//��Ϸ�¼�
public:
	//��Ϸ��ʼ
	virtual bool __cdecl OnEventGameStart();
	//��Ϸ����
	virtual bool __cdecl OnEventGameEnd(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
	//���ͳ���
	virtual bool __cdecl SendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE bGameStatus, bool bSendSecret);

	//�¼��ӿ�
public:
	//��ʱ���¼�
	virtual bool __cdecl OnTimerMessage(WORD wTimerID, WPARAM wBindParam);
	//��Ϸ��Ϣ����
	virtual bool __cdecl OnGameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//�����Ϣ����
	virtual bool __cdecl OnFrameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);

	//��Ϸ�¼�
protected:
	//��ׯ�¼�
	bool OnUserCallBanker(WORD wChairID, BYTE bBanker);
	//��ע�¼�
	bool OnUserAddScore(WORD wChairID, LONGLONG lScore);
	//̯���¼�
	bool OnUserOpenCard(WORD wChairID, BYTE bOx);
	//����Ա����
	bool OnSubAmdinCommand(IServerUserItem*pIServerUserItem,const void*pDataBuffer);

	//���ܺ���
protected:
	void AnalyseAndroidCard();
	//�˿˷���
	void AnalyseUserCard();
	//�˿˷���
	void AnalyseAdminCard(WORD dwAdminChairID, bool bIsWin);
	//�����˿�
	bool ChangeCard(WORD Winer, WORD Loser);
	//�Է����
	WORD GetOtherUserChair(WORD wUserChair);
};

//////////////////////////////////////////////////////////////////////////

#endif
