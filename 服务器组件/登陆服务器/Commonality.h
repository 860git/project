#ifndef COMMONALITY_HEAD_FILE
#define COMMONALITY_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////
//���ݿ������

//���ݿ������ʶ
#define	DBR_GP_LOGON_BY_ACCOUNTS	1									//�ʺŵ�¼
#define	DBR_GP_LOGON_BY_USERID		2									//I D ��¼
#define DBR_GP_REGISTER_ACCOUNTS	3									//ע���ʺ�
#define DBR_GP_USER_UPLOAD_FACE		4									//�ϴ�ͷ��
#define DBR_GP_USER_LEAVE			5									//����뿪
#define DBR_GP_DOWNLOAD_FACE		6									//����ͷ��
#define DBR_GP_DELETE_FACE			7									//ɾ��ͷ��
#define DBR_GP_MODIFY_INDIVIDUAL	8									//�޸�����
#define DBR_GP_SAFE_BIND			9									//���ʺ�
#define DBR_GP_SAFE_UNBIND			10									//�����
#define DBR_GP_OPER_MONEY			11									//����Ǯ��  �������� 2012-4-10
#define DBR_GP_GIVE_MONEY			12									//����Ǯ��  �������� 2012-4-10
#define DBR_GP_BANK_PASS			13									//��������  �������� 2012-4-10
#define DBR_GP_GIVE_RECORD			14									//���ͼ�¼  �������� 2012-4-10
#define DBR_GP_USER_PASS			15									//��������  �������� 2012-4-10
#define DBR_GP_USER_WRITE			16									//��������  �������� 2012-4-10
#define DBR_GP_USER_INFO			17									//��������  �������� 2012-4-10

//���ݿ������ʶ
#define DBR_GP_LOGON_SUCCESS		100									//��¼�ɹ�
#define DBR_GP_LOGON_ERROR			101									//��¼ʧ��
#define DBR_GP_DOWNLOADFACE_SUCCESS	102									//���سɹ�
#define DBR_GP_UPLOAD_FACE_RESULT	103									//�ϴ����
#define DBR_GP_DELETE_FACE_RESULT	104									//ɾ�����
#define DBR_GP_MODIFY_RESULT		105									//�޸Ľ��

#define DBR_GP_OPERMONEY_RESULT		106									//����Ǯ�� �������� 2012-4-10
#define DBR_GP_REFRESHEMONEY_RESULT	107									//�޸�Ǯ�� �������� 2012-4-10

#define DBR_GP_GIVEMONEY_RESULT		108									//����Ǯ�� �������� 2012-4-10

#define DBR_GP_BANKPASS_RESULT		109									//����Ǯ�� �������� 2012-4-10
#define DBR_GP_GIVERECORD_RESULT	110								//����Ǯ�� �������� 2012-4-10

//���ʺ�
struct DBR_GP_SafeBind
{
	DWORD							dwUserID;							//���ID
	TCHAR							szComputerID[COMPUTER_ID_LEN];		//��������
	TCHAR								szPassWord[PASS_LEN];			//��¼����
};

//�����
struct DBR_GP_SafeUnbind
{
	DWORD							dwUserID;							//���ID
	TCHAR								szPassWord[PASS_LEN];			//��¼����
};
//�����
struct DBR_GP_BindResult
{
	BYTE								cbSuccess;						//���ID
	//TCHAR								szErrorDescribe[128];			//��¼����
};
//�ʺŵ�¼
struct DBR_GP_LogonByAccounts
{
	DWORD							dwClientIP;							//���ӵ�ַ
	TCHAR							szAccounts[NAME_LEN];				//��¼�ʺ�
	TCHAR							szPassWord[PASS_LEN];				//��¼����
	TCHAR							szComputerID[COMPUTER_ID_LEN];		//��������
};

//ID ��¼
struct DBR_GP_LogonByUserID
{
	DWORD							dwClientIP;							//���ӵ�ַ
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassWord[PASS_LEN];				//��¼����
	TCHAR							szComputerID[COMPUTER_ID_LEN];		//��������
};

//�ʺ�ע��
struct DBR_GP_RegisterAccounts
{
	WORD							wFaceID;							//ע��ͷ��
	BYTE							cbGender;							//�û��Ա�
	DWORD							dwClientIP;							//���ӵ�ַ
	TCHAR							szSpreader[NAME_LEN];				//�Ƽ�����
	TCHAR							szAccounts[NAME_LEN];				//��¼�ʺ�
	TCHAR							szPassWord[PASS_LEN];				//��¼����
	TCHAR							szComputerID[COMPUTER_ID_LEN];		//��������
    TCHAR							szBankpass[PASS_LEN];				//��������
	TCHAR							szIdno[NAME_LEN];				//�û����֤��
    TCHAR							szEmail[NAME_LEN];				//�����ʼ�
	TCHAR							szMobile[NAME_LEN];				//�ֻ�
};

//��¼�ɹ�
struct DBR_GP_LogonSuccess
{
	//��������
	DWORD							dwUserID;							//�û���ʶ
	DWORD							dwGameID;							//��Ϸ��ʶ
	DWORD							dwExperience;						//������ֵ
	TCHAR							szPassword[PASS_LEN];				//��¼����
	TCHAR							szAccounts[NAME_LEN];				//��¼�ʺ�
	TCHAR							szGroupName[GROUP_LEN];				//��������

	//�û�����
	WORD							wFaceID;							//ͷ���ʶ
	BYTE							cbGender;							//�û��Ա�
	DWORD							dwCustomFaceVer;					//ͷ��汾
	BYTE							cbMember;							//��Ա�ȼ�
	TCHAR							szUnderWrite[UNDER_WRITE_LEN];		//����ǩ��
     DWORD                            JiQi;                              //������ʾ
	//������Ϣ
	TCHAR							szDescribeString[128];				//������Ϣ
	TCHAR							szBankPassword[PASS_LEN];				//��¼����
};

//��¼ʧ��
struct DBR_GP_LogonError
{
	LONG							lErrorCode;							//�������
	TCHAR							szErrorDescribe[128];				//������Ϣ
};

//�ϴ�ͷ��
struct DBR_GP_UploadCustomFace
{
	DWORD							dwUserID;							//��� ID
};

//�ϴ����
struct DBR_GP_UploadFaceResult
{
	TCHAR							szDescribeMsg[128];					//������Ϣ
	DWORD							dwFaceVer;							//ͷ��汾
	bool							bOperateSuccess;					//�ɹ���ʶ
};

//ɾ�����
struct DBR_GP_DeleteFaceResult
{
	TCHAR							szDescribeMsg[128];					//������Ϣ
	DWORD							dwFaceVer;							//ͷ��汾
	bool							bOperateSuccess;					//�ɹ���ʶ
};

//����ͷ��
struct DBR_GP_DownloadCustomFace
{
	DWORD							dwUserID;							//��� ID
};
struct DBR_GP_DownLockJiQi
{
	DWORD								dwGameUserID;						//��� ID
	bool								dwLockLP;					//����ʶ
};
//���سɹ�
struct DBR_GP_DownloadFaceSuccess
{
	DWORD							dwToltalSize;						//�ܹ���С
	DWORD							dwCurrentSize;						//��ǰ��С
	DWORD							dwUserID;							//��� ID
	BYTE							bFaceData[2048];					//ͷ������
};

//ɾ����Ϣ
struct DBR_GP_CustomFaceDelete
{
	DWORD							dwUserID;							//��� ID
	DWORD							dwFaceVer;							//ͷ��汾
};

//��������
struct DBR_GP_ModifyIndividual
{
	DWORD							dwUserID;							//��� ID
	TCHAR							szNickname[NAME_LEN];				//����ǳ�
	int								nGender;							//����Ա�
	int								nAge;								//�������
	TCHAR							szAddress[64];						//��ҵ�ַ
	TCHAR							szUnderWrite[32];					//����ǩ��
	TCHAR							szPassword[33];						//�������
};

//�޸Ľ��
struct DBR_GP_ModifyIndividualResult
{
	TCHAR							szDescribeMsg[128];					//������Ϣ
	bool							bOperateSuccess;					//�ɹ���ʶ
};


struct DBR_GP_OperMoney
{
	DWORD							dwUserID;							//��� ID
	DWORD							dwKindID;							//�����ʶ
	LONGLONG						lScore;								//Ǯ��
	BYTE							cbType;								//����  0:SAVE 1:GET  3��REFRESH
};

//�޸Ľ��
struct DBR_GP_MoneyOperResult
{
	TCHAR							szDescribeMsg[128];					//������Ϣ
	bool							bOperateSuccess;					//�ɹ���ʶ
};

struct DBR_GP_MoneyRefreshResult
{
	LONGLONG						lTotalScore;						//������Ǯ��
	LONGLONG						lRoomScore;							//����Ǯ��
};

struct DBR_GP_GiveMoney
{
	DWORD							dwClientIP;							//���ӵ�ַ
	DWORD							dwUserID;							//��� ID
	TCHAR							szNickname[NAME_LEN];				//����ǳ�
	LONGLONG						lScore;								//Ǯ��
	LONGLONG						lRevenue;							//˰��
	BYTE							cbType;								//����  0:�û��� 1:�û�ID
	BYTE							cbQueryInfo;						//��ѯ�û���Ϣ 1��ѯ
};

//�޸Ľ��
struct DBR_GP_GiveMoneyResult
{
	TCHAR							szDescribeMsg[128];					//������Ϣ
	TCHAR							szTargeAccount[NAME_LEN];			//�û���Ϣ
	DWORD							dwTargetUserId;						//�Է�ID
	LONGLONG						lRevenue;							//˰��
	LONGLONG						lSwapScore;							//˰��
	bool							bOperateSuccess;					//�ɹ���ʶ
	bool							bQueryInfo;							//��Ѱ�û�
};

struct DBR_GP_BankPass
{
	DWORD							dwUserID;							//��� ID
	TCHAR							szPassword[33];						//�������
	TCHAR							szPassword1[33];						//�������

};

//�޸Ľ��
struct DBR_GP_BankPassResult
{
	TCHAR							szDescribeMsg[128];					//������Ϣ
	bool							bOperateSuccess;					//�ɹ���ʶ
};

struct DBR_GP_GiveRecord
{
	DWORD							dwUserID;							//��� ID
};

//�޸Ľ��
struct DBR_GP_GiveRecordResult
{
	int								nCount;								//����															
	bool							bOperateSuccess;					//�ɹ���ʶ
	TCHAR							szDescribeMsg[128];					//������Ϣ
	tagUserGiveRecord				GiveRecord[200];					//��¼
};

struct DBR_GP_UserWrite
{
	DWORD							dwUserID;							//��� ID
	TCHAR							szUnderWrite[32];					//����ǩ��
};

struct DBR_GP_ModifyUserInfo
{
	DWORD							dwUserID;							//��� ID
	TCHAR							szAddress[64];						//����ǳ�
	TCHAR							szSchool[32];						//��ҵ�ַ
	TCHAR							szEmail[32];						//EMAIL
	TCHAR							szUrl[64];							//������ҳ
	TCHAR							szTel[32];							//�绰
	TCHAR							szMsn[32];							//MSN
};

//////////////////////////////////////////////////////////////////////////

#endif