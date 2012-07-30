#ifndef SYSTEM_OPTION_HEAD_FILE
#define SYSTEM_OPTION_HEAD_FILE

#pragma once

#include "Stdafx.h"

//��˵��
class CListKind;
class CListServer;

//////////////////////////////////////////////////////////////////////////
//ö�ٶ���

//����ģʽ
enum enInviteMode
{
	enInviteMode_All					=1,								//ȫ����ʾ
	enInviteMode_Friend					=2,								//������ʾ
	enInviteMode_None					=3,								//ȫ����ʾ
};

//����ģʽ	
enum enCallMode
{
	enCallMode_All					=1,								//ȫ����ʾ
	enCallMode_Friend					=2,								//������ʾ
	enCallMode_None					=3,								//ȫ����ʾ
};
//�ʻ�����
enum enAcountsRule
{
	enAcountsRule_None					=1,								//����¼��Ϣ
	enAcountsRule_Accounts				=2,								//��ס�ʻ�
	enAcountsRule_AccountsAndPass		=3,								//�ʻ�������
};

//��������� ����
enum enBankRule
{
	enBankRule_LK					=1,								//������Ч
	enBankRule_ZD					=2,								//�Զ���Ч
	enBankRule_LEAVE				=3,								//�뿪��Ч
};

//////////////////////////////////////////////////////////////////////////

//����������Ϣ
class CServerOption
{
	//���Ա���
public:
	const WORD						m_wServerID;						//�����ʾ
	const CListServer 				* m_pListServer;					//��Ϸ����

	//Я������
public:
	bool							m_bPassword;						//��������
	TCHAR							m_szPassword[PASS_LEN];				//��������
	TCHAR							m_szEnterPassword[PASS_LEN];		//��������

	//��������
public:
	//���캯��
	CServerOption(WORD wServerID, CListServer * pListServer);
	//��������
	virtual ~CServerOption();

	//���ܺ���
public:
	//��������
	void LoadOptionParameter();
	//��������
	void SaveOptionParameter();
};

//////////////////////////////////////////////////////////////////////////

//��Ϸ������Ϣ
class CGameOption
{
	//���Ա���
public:
	const WORD						m_wKindID;							//��Ϸ��ʾ
	CListKind 						* m_pListKind;						//��Ϸ����

	//��������
public:
	LONGLONG							m_lMaxScore;						//��߷��� 
	LONGLONG							m_lLessScore;						//��ͷ���
	bool							m_bLimitScore;						//���Ʒ���

	//��������
public:
	WORD							m_wFleeRate;						//������
	bool							m_bLimitFlee;						//���ƶ���

	//ʤ������
public:
	WORD							m_wWinRate;							//ʤ����
	bool							m_bLimitWin;						//����ʤ��

	//��������
public:
	//���캯��
	CGameOption(WORD wKindID, CListKind * pListKind);
	//��������
	virtual ~CGameOption();

	//���ܺ���
public:
	//��������
	void LoadOptionParameter();
	//��������
	void SaveOptionParameter();
};

//////////////////////////////////////////////////////////////////////////

//��˵��
typedef CArrayTemplate<CGameOption *> CGameOptionArray;
typedef CArrayTemplate<CServerOption *> CServerOptionArray;

//ȫ��������
class CGlobalOption
{
	//ȫ������
public:
	bool							m_bExpandedList;					//չ���б�
	enAcountsRule					m_enAcountsRule;					//�ʻ�����

	//��������
public:
	bool							m_bLimitDetest;						//�������
	bool							m_bCheckSameIP;						//Ч���ַ

	//��Ϣ����
public:
	bool							m_bSaveWisper;						//����˽��
	bool							m_bShowInOutMessage;				//������Ϣ
	bool							m_bOnlyFriendMessage;				//������Ϣ
	bool							m_bHideDetestMessage;				//�����Ϣ

	//��������
public:
	bool							m_bAutoSit;							//�Զ�����
	WORD							m_wBossHotKey;						//�ϰ��ȼ�
	enInviteMode					m_InviteMode;						//����ģʽ
	enCallMode						m_CallMode;							//����ģʽ

	//��������
public:
	enProxyServerType				m_ProxyServerType;					//��������
	tagProxyServerInfo				m_ProxyServerInfo;					//������Ϣ

	//��ɫ����
public:
	COLORREF						m_crChatTX;							//��������
	COLORREF						m_crMsgName;						//������Ϣ
	COLORREF						m_crMsgNormal;						//��ͨ��Ϣ
	COLORREF						m_crMsgSysHead;						//ϵͳ��Ϣ
	COLORREF						m_crMsgSysString;					//ϵͳ��Ϣ
	COLORREF						m_crServerListTX;					//�б���ɫ
	COLORREF						m_crServerListBK;					//�б��ɫ
//����������
	bool							m_bCloseQuery;						//�ر����Ƿ��ѯ
	bool							m_BankCloseTip;						//�ر���ʾ
	enBankRule						m_BankCloseMode;					//�ر�ģʽ
	UINT							m_BankCloseTime;					//�ر�ʱ��
	COLORREF						m_crOwnColor;						//�Լ���ɫ
	COLORREF						m_crFriendColor;					//������ɫ
//��������
public:
	bool							m_bPassword;						//��������
	TCHAR							m_szPassword[PASS_LEN];				//��������
//��ͬIP
public:
	bool							m_bSameIp;							//��ͬIP
	WORD							m_wSameIpNum;						//��ͬIP��
	//�ڲ�����
private:
	CGameOptionArray				m_GameOptionArray;					//��Ϸ����
	CServerOptionArray				m_ServerOptionArray;				//��������

	//��������
public:
	//���캯��
	CGlobalOption();
	//��������
	virtual ~CGlobalOption();

	//���ܺ���
public:
	//��������
	void LoadOptionParameter();
	//��������
	void SaveOptionParameter();
	//��Ϸ����
	CGameOption * GetGameOption(CListKind * pListKind);
	//��������
	CServerOption * GetServerOption(CListServer * pListServer);
};

//////////////////////////////////////////////////////////////////////////

//ȫ����Ϣ 
extern	CGlobalOption					g_GlobalOption;					//ϵͳ����

//////////////////////////////////////////////////////////////////////////

#endif