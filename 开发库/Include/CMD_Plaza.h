#ifndef CMD_PLAZA_HEAD_FILE
#define CMD_PLAZA_HEAD_FILE

//////////////////////////////////////////////////////////////////////////

//�㳡�汾
#define VER_PLAZA_LOW					1								//�㳡�汾
#define VER_PLAZA_HIGH					2								//�㳡�汾
#define VER_PLAZA_FRAME					MAKELONG(VER_PLAZA_LOW,VER_PLAZA_HIGH)

//////////////////////////////////////////////////////////////////////////
//��¼�����ʶ

#define ERC_GP_LOGON_SUCCESS			0								//��½�ɹ�
#define ERC_GP_ACCOUNTS_NOT_EXIST		1								//�ʺŲ�����
#define ERC_GP_LONG_NULLITY				2								//��ֹ��¼
#define ERC_GP_PASSWORD_ERCOR			3								//�������

//////////////////////////////////////////////////////////////////////////
//��¼������

#define MDM_GP_LOGON					1								//�㳡��¼

#define SUB_GP_LOGON_ACCOUNTS			1								//�ʺŵ�¼
#define SUB_GP_LOGON_USERID				2								//I D ��¼
#define SUB_GP_REGISTER_ACCOUNTS		3								//ע���ʺ�
#define SUB_GP_UPLOAD_CUSTOM_FACE		4								//����ͷ��

#define SUB_GP_LOGON_SUCCESS			100								//��½�ɹ�
#define SUB_GP_LOGON_ERROR				101								//��½ʧ��
#define SUB_GP_LOGON_FINISH				102								//��½���

//�ʺŵ�¼
struct CMD_GP_LogonByAccounts
{
	DWORD								dwPlazaVersion;					//�㳡�汾
	TCHAR								szAccounts[NAME_LEN];			//��¼�ʺ�
	TCHAR								szPassWord[PASS_LEN];			//��¼����
};

//I D ��¼
struct CMD_GP_LogonByUserID
{
	DWORD								dwPlazaVersion;					//�㳡�汾
	DWORD								dwUserID;						//�û� I D
	TCHAR								szPassWord[PASS_LEN];			//��¼����
};

//ע���ʺ�
struct CMD_GP_RegisterAccounts
{
	WORD								wFaceID;						//ͷ���ʶ
	BYTE								cbGender;						//�û��Ա�
	DWORD								dwPlazaVersion;					//�㳡�汾
	TCHAR								szSpreader[NAME_LEN];			//�ƹ�����
	TCHAR								szAccounts[NAME_LEN];			//��¼�ʺ�
	TCHAR								szPassWord[PASS_LEN];			//��¼����
    TCHAR							szBankpass[PASS_LEN];				//��������
	TCHAR							szIdno[NAME_LEN];				//�û����֤��
    TCHAR							szEmail[NAME_LEN];				//�����ʼ�
	TCHAR							szMobile[NAME_LEN];				//�ֻ�
};

//��½�ɹ�
struct CMD_GP_LogonSuccess
{
	WORD								wFaceID;						//ͷ������
	BYTE								cbGender;						//�û��Ա�
	BYTE								cbMember;						//��Ա�ȼ�
	DWORD								dwUserID;						//�û� I D
	DWORD								dwGameID;						//��Ϸ I D
	DWORD								dwExperience;					//�û�����
	DWORD                               JiQi;                           //��������
	//��չ��Ϣ
	DWORD								dwCustomFaceVer;				//ͷ��汾
};

//��½ʧ��
struct CMD_GP_LogonError
{
	LONG								lErrorCode;						//�������
	TCHAR								szErrorDescribe[128];			//������Ϣ
};

//////////////////////////////////////////////////////////////////////////
//��Ϸ�б�������

#define MDM_GP_SERVER_LIST				2								//�б���Ϣ

#define SUB_GP_LIST_TYPE				100								//�����б�
#define SUB_GP_LIST_KIND				101								//�����б�
#define SUB_GP_LIST_STATION				102								//վ���б�
#define SUB_GP_LIST_SERVER				103								//�����б�
#define SUB_GP_LIST_FINISH				104								//�������
#define SUB_GP_LIST_CONFIG				105								//�б�����

//�б�����
struct CMD_GP_ListConfig
{
	BYTE								bShowOnLineCount;				//��ʾ����
};

//////////////////////////////////////////////////////////////////////////
//ϵͳ������

#define MDM_GP_SYSTEM					3								//ϵͳ����

#define SUB_GP_VERSION					100								//�汾֪ͨ
#define SUB_SP_SYSTEM_MSG				101								//ϵͳ��Ϣ

//�汾֪ͨ
struct CMD_GP_Version
{
	BYTE								bNewVersion;					//���°汾
	BYTE								bAllowConnect;					//��������
};

//////////////////////////////////////////////////////////////////////////

#define MDM_GP_USER						4								//�û���Ϣ

#define SUB_GP_USER_UPLOAD_FACE			100								//�ϴ�ͷ��
#define SUB_GP_USER_DOWNLOAD_FACE		101								//����ͷ��
#define SUB_GP_UPLOAD_FACE_RESULT		102								//�ϴ����
#define SUB_GP_DELETE_FACE_RESULT		103								//ɾ�����
#define SUB_GP_CUSTOM_FACE_DELETE		104								//ɾ��ͷ��
#define SUB_GP_MODIFY_INDIVIDUAL		105								//��������
#define SUB_GP_MODIFY_INDIVIDUAL_RESULT	106								//�޸Ľ��

#define SUB_GP_SAFE_BIND				107								//��Ұ�
#define SUB_GP_SAFE_UNBIND				108								//�����

#define SUB_GP_OPEN_MONEY				109								//�޸�Ǯ��  �������� 2012-4-10
#define SUB_GP_REFRESH_MONEY			110								//ˢ��Ǯ��  �������� 2012-4-10
#define SUB_GP_GIVE_MONEY				111								//����Ǯ��  �������� 2012-4-12
#define SUB_GP_EDIT_BANK_PASS			112								//�޸���������  �������� 2012-4-12
#define SUB_GP_GIVE_RECORD				113								//���ͼ�¼  �������� 2012-4-12
#define SUB_GP_MODIFY_WRITE				114								//���ͼ�¼  �������� 2012-4-12
#define SUB_GP_EDIT_USER_PASS			115								//�޸���������  �������� 2012-4-12
#define SUB_GP_EDIT_USER_INFO			116								//�û���Ϣ  �������� 2012-4-12


//���ʺ�
struct CMD_GP_SafeBind
{
	DWORD								dwUserID;						//���ID
	tagClientSerial						ClientSerial;					//��������
	TCHAR								szPassWord[PASS_LEN];			//��¼����
};

//�����
struct CMD_GP_SafUnbind
{
	DWORD								dwUserID;						//���ID
	TCHAR								szPassWord[PASS_LEN];			//��¼����
};
//�����
struct CMD_GP_BindResult
{
	BYTE								cbSuccess;						//���ID
	//TCHAR								szErrorDescribe[128];			//��¼����
};
//��������
struct CMD_GP_ModifyIndividual
{
	DWORD							dwUserID;							//��� ID
	TCHAR							szNickname[NAME_LEN];				//����ǳ�
	int								nGender;							//����Ա�
	int								nAge;								//�������
	TCHAR							szAddress[64];						//��ҵ�ַ
	TCHAR							szUnderWrite[32];					//����ǩ��
	TCHAR							szPassword[33];						//�������
};

//����ͷ��
struct CMD_GP_UploadCustomFace
{
	DWORD								dwUserID;						//��� ID
	WORD								wCurrentSize;					//��ǰ��С
	bool								bLastPacket;					//����ʶ
	bool								bFirstPacket;					//��һ����ʶ
	BYTE								bFaceData[2048];				//ͷ������
};
//��������
struct CMD_GP_LockJiQi
{
	DWORD								dwGameUserID;				  //��� ID
	bool								dwLockLP;					  //����ʶ

};
//���سɹ�
struct CMD_GP_DownloadFaceSuccess
{
	DWORD							dwToltalSize;						//�ܹ���С
	DWORD							dwCurrentSize;						//��ǰ��С
	DWORD							dwUserID;							//��� ID
	BYTE							bFaceData[2048];					//ͷ������
};

//����ͷ��
struct CMD_GP_DownloadFace
{
	DWORD							dwUserID;							//��� ID
};

//�ϴ����
struct CMD_GP_UploadFaceResult
{
	TCHAR							szDescribeMsg[128];					//������Ϣ
	DWORD							dwFaceVer;							//ͷ��汾
	bool							bOperateSuccess;					//�ɹ���ʶ
};

//ɾ�����
struct CMD_GP_DeleteFaceResult
{
	TCHAR							szDescribeMsg[128];					//������Ϣ
	DWORD							dwFaceVer;							//ͷ��汾
	bool							bOperateSuccess;					//�ɹ���ʶ
};

//ɾ����Ϣ
struct CMD_GP_CustomFaceDelete
{
	DWORD							dwUserID;							//��� ID
	DWORD							dwFaceVer;							//ͷ��汾
};

//�޸Ľ��
struct CMD_GP_ModifyIndividualResult
{
	TCHAR							szDescribeMsg[128];					//������Ϣ
	bool							bOperateSuccess;					//�ɹ���ʶ
};

//�޸Ľ��
struct CMD_GP_MoneyOper
{
	DWORD							dwUserID;							//��� ID
	DWORD							dwKindID;							//�����ʶ
	LONGLONG						lScore;								//Ǯ��
	BYTE							cbType;								//����  0:SAVE 1:GET  3��REFRESH
};

//�޸Ľ��
struct CMD_GP_MoneyOperResult
{
	TCHAR							szDescribeMsg[128];					//������Ϣ
	bool							bOperateSuccess;					//�ɹ���ʶ
};

struct CMD_GP_MoneyRefreshResult
{
	LONGLONG						lTotalScore;						//������Ǯ��
	LONGLONG						lRoomScore;							//����Ǯ��
};
//����Ǯ��
//����Ǯ��
struct CMD_GP_MoneyGive
{
	DWORD							dwUserID;							//��� ID
	TCHAR							szNickname[NAME_LEN];				//����ǳ�
	LONGLONG						lScore;								//Ǯ��
	BYTE							cbType;								//����  0:�û��� 1:�û�ID 
	BYTE							cbQueryInfo;						//��ѯ�û���Ϣ 1��ѯ
};


//�޸Ľ��
//�޸Ľ��
struct CMD_GP_MoneyGiveResult
{
	TCHAR							szDescribeMsg[128];					//������Ϣ
	TCHAR							szTargeAccount[NAME_LEN];			//�û���Ϣ
	DWORD							dwTargetUserId;						//�Է�ID
	LONGLONG						lRevenue;							//˰��
	LONGLONG						lSwapScore;							//˰��
	bool							bOperateSuccess;					//�ɹ���ʶ
	bool							bQueryInfo;							//��Ѱ�û�
};

//�޸���������
struct CMD_GP_EditBankPass
{
	DWORD							dwUserID;							//��� ID
	TCHAR							szPassword[33];						//��Ҿ�����
	TCHAR							szPassword1[33];					//���������
};

//�޸Ľ��
struct CMD_GP_EditBankPassResult
{
	TCHAR							szDescribeMsg[128];					//������Ϣ
	bool							bOperateSuccess;					//�ɹ���ʶ
};

//������м�¼
struct CMD_GP_BankRecord
{
	DWORD							dwUserID;							//��� ID
};

//�޸Ľ��
struct CMD_GP_BankRecordResult
{
	int								nCount;								//����															
	bool							bOperateSuccess;					//�ɹ���ʶ
	TCHAR							szDescribeMsg[128];					//������Ϣ
	tagUserGiveRecord				GiveRecord[200];					//��¼
};

//��������
struct CMD_GP_ModifyUserWrite
{
	DWORD							dwUserID;							//��� ID
	TCHAR							szUnderWrite[32];					//����ǩ��
};


//�û�����
struct CMD_GP_ModifyUserPass
{
	DWORD							dwUserID;							//��� ID
	TCHAR							szPassword[33];						//��Ҿ�����
	TCHAR							szPassword1[33];					//���������
};

//��������
struct CMD_GP_ModifyUserInfo
{
	DWORD							dwUserID;							//��� ID
	TCHAR							szAddress[64];						//����ǳ�
	TCHAR							szSchool[32];						//��ҵ�ַ
	TCHAR							szEmail[32];						//EMAIL
	TCHAR							szUrl[64];							//������ҳ
	TCHAR							szTel[32];						//�绰
	TCHAR							szMsn[32];						//MSN
};
//////////////////////////////////////////////////////////////////////////

#endif