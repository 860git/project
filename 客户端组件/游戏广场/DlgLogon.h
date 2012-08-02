#ifndef DLG_LOGON_HEAD_FILE
#define DLG_LOGON_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"

//////////////////////////////////////////////////////////////////////////

//��¼ģʽ
enum enLogonMode
{
	LogonMode_Accounts,			//�ʺŵ�¼
	LogonMode_UserID,			//I D ��¼
};

//ע�������
#define REG_OPTION_LOGON		TEXT("OptionLogon")
#define REG_USER_INFO			TEXT("Software\\7x78Game\\GamePlaza\\UserInfo")
#define REG_LOGON_SERVER		TEXT("Software\\7x78Game\\GamePlaza\\LogonServer")


//////////////////////////////////////////////////////////////////////////

//��¼ TAB �ؼ�
class CTabCtrlLogon : public CTabCtrl
{
	//��������
public:
	//���캯��
	CTabCtrlLogon();
	//��������
	virtual ~CTabCtrlLogon();

	//��Ϣ����
protected:
	//�ػ�����	
	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

//�û�ע��
class CDlgRegister : public CSkinDialogEx
{
	friend class CDlgLogon;

	//��¼��Ϣ
public:
	WORD								m_wFaceID;						//ͷ���ʶ
	BYTE								m_cbGender;						//�û��Ա�
	TCHAR								m_szSpreader[NAME_LEN];			//��������
	TCHAR								m_szAccounts[NAME_LEN];			//��Ϸ�ʺ�
	TCHAR								m_szPassword[PASS_LEN];			//��Ϸ����
	TCHAR								m_szEmail[NAME_LEN];			//��������
	TCHAR								m_szIdno[NAME_LEN];			    //���֤��
	TCHAR								m_szBankpass[PASS_LEN];			//��������
	TCHAR								m_szMobile[PASS_LEN];				//qq����
//	TCHAR								m_szAnQuan[PASS_LEN];			//��ȫ��


	//�ؼ�����
public:
	CImageList							m_ImageList;
	CComboBoxEx							m_FaceSelect;
	CSkinButton							m_btLogon;						//��¼��ť
	CSkinButton							m_btCancel;						//ȡ����ť
	CSkinButton							m_btCancel1;						//ȡ����ť
	CSkinHyperLink						m_LineMainPage;					//��Ϸ��ҳ
	CSkinHyperLink						m_LinePassWord;					//���뱣��

    CSkinImage                          m_ImageKuang;                   //��½��ͼ
	 CPngImage						    m_ImageLogo;			        //������ͼƬ

	//λ�ñ���
protected:
	INT									m_nFullWidth;					//ȫ�����
	INT									m_nFullHeight;					//ȫ���߶�
	bool								m_bNetOption;					//��������
	CRect								m_rcNormalFrame;				//���λ��
    CImage							    m_ImageBuffer;						//����λͼ

		CSkinEdit							m_edRegPassword;				//�û�����
		CSkinEdit							m_edRegPassword1;				//�û�����
		CSkinEdit							m_edRegPassword2;				//�û�����
		CSkinEdit							m_edRegPassword3;				//�û�����
		CSkinEdit							m_edRegPassword4;				//�û�����
		CSkinEdit							m_edRegPassword5;				//�û�����
		CSkinEdit							m_edRegPassword6;				//�û�����
		CMyComboBox							m_edRegPassword7;				//�û�����
		CSkinEdit							m_edRegPassword8;				//�û�����
		CSkinEdit							m_edRegPassword9;				//�û�����
		CSkinEdit							m_edRegPassword10;				//�û�����
		CSkinEdit							m_edRegPassword11;				//�û�����
	//��������
public:
	//���캯��
	CDlgRegister();
	//��������
	virtual ~CDlgRegister();

	//���غ���
protected:
	//�ؼ���
	virtual void DoDataExchange(CDataExchange * pDX);
	//��ʼ������
	virtual BOOL OnInitDialog();
	//ȷ������
	virtual void OnOK();
	//ȡ����Ϣ
	virtual void OnCancel();
	//�ػ���Ϣ
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

//��¼�Ի���
class CDlgLogon : public CSkinDialogEx
{
	friend class CRoomViewItem;
	friend class CPlazaViewItem;

	//��¼��Ϣ
protected:
	WORD								m_wFaceID;						//ͷ���ʶ
	BYTE								m_cbGender;						//�û��Ա�
	DWORD								m_dwUserID;						//�û� I D
	TCHAR								m_szSpreader[NAME_LEN];			//�ƹ�����
	TCHAR								m_szAccounts[NAME_LEN];			//��Ϸ�ʺ�
	TCHAR								m_szPassword[PASS_LEN];			//��Ϸ����
	TCHAR								m_szEmail[NAME_LEN];			//��������
	TCHAR								m_szIdno[NAME_LEN];			    //���֤��
	TCHAR								m_szBankpass[PASS_LEN];			//��������
	TCHAR								m_szMobile[PASS_LEN];
//    TCHAR								m_szAnQuan[PASS_LEN];			//��ȫ��

	//λ�ñ���
protected:
	INT									m_nFullWidth;					//ȫ�����
	INT									m_nFullHeight;					//ȫ���߶�
	bool								m_bNetOption;					//��������
	CRect								m_rcNormalFrame;				//���λ��
    CImage							    m_ImageBuffer;						//����λͼ
	//��¼ģʽ
protected:
	enLogonMode							m_LogonMode;					//��¼ģʽ

	//������Ϣ
protected:
	bool								m_bRegister;					//ע���־
	bool								m_bChangePassWord;				//�ı��־
    bool                                m_bJinRu;                       //�Ƿ�滭������
	//������Ϣ
protected:
	CString								m_strLogonServer;				//��������ַ
	bool                                LeftRight;                      //���ƽ������������ƶ�
	//��ť����
public:
	CSkinButton							m_btLogon;						//��¼��ť
	CSkinButton							m_btQuit;						//�˳���ť
	CSkinButton							m_btCancel;						//ȡ����ť
	CGdiButton							m_btDelete;						//ɾ����ť
	CSkinButton							m_btJiZhuMiMa;					//��¼����
	CGdiButton							m_btRegister;					//ע�ᰴť
	CSkinButton							m_btNetOption;					//���簴ť
	CSkinButton							m_btProxyTest;					//���԰�ť

	CSkinButton							m_tAbsel1;						//ȡ����ť
	CSkinButton							m_tAbsel2;						//ɾ����ť

	//CSkinButton							m_btUseAccount;					//�û�����¼
	//CSkinButton							m_btUseId;						//ID��¼

	//�ؼ�����
public:
	CWebBrowser							m_BrowerAD;						//�������
	CTabCtrlLogon						m_TabLogonMode;					//��¼ѡ��
	CSkinHyperLink						m_LineTiaokuan;					//��Ϸ��ҳ
	CSkinHyperLink						m_LinePassWord;					//���뱣��
	CSkinHyperLink						m_LineGetPassWord;				//ȡ������
	CSkinHyperLink						m_LineRegWeb;					//��վע��
	CSkinHyperLink						m_LineZhiNan;					//��Ϸָ��
    //CSkinImage                          m_ImageKuang;                   //��½��ͼ
	//CPngImage						    m_ImageLogo;			        //������ͼƬ
	//CPngImage							m_ImageBack;	
	//��¼����
	CSkinImage							m_ImageBack;					//��¼����
	CBrush								m_brBkground;					//������ˢ
	CFont*								m_boxFont;						//COMBOX����
	CPngImage							m_pngBack;						//PNG����
	CPngImage							m_pngStatus;					//����״̬
	UINT								m_iLoadingPos;					//�ٷֱ�λ��
	UINT								m_iLoaddingMax;					//��󳤶�
	CRect								m_rcLoading;					//������λ��

	//�༭�ؼ�
protected:
	CSkinEdit							m_edProxyPort;					//����˿�
	CSkinEdit							m_edProxyServer;				//�����ַ
	CSkinEdit							m_edProxyUserName;				//�����û�
	CPasswordControl							m_edProxyPassword;				//��������
	CMyComboBox							m_cmProxyServerType;			//��������
	CMyComboBox							m_boxUserName;					//�ǳ���
	CPasswordControl					m_edUserPassword;				//�û�����
    CMyComboBox							m_boxUserIP;					//�ǳ���
	CMyComboBox							m_boxUserID;					//�ǳ���
	CCheckButton						m_chkRem;						//��ס����
	CCheckButton						m_chkTiaoKuan;					//ͬ������
	CGdiButton							m_btPay;						//���ٳ�ֵ

	//��������
public:
	//���캯��
	CDlgLogon();
	//��������
	virtual ~CDlgLogon();

	//���غ���
protected:
	//�ؼ���
	virtual void DoDataExchange(CDataExchange * pDX);
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//��ʼ������
	virtual BOOL OnInitDialog();
	//ȷ������
	virtual void OnOK();
	//ȡ����Ϣ
	virtual void OnCancel();
   
	//���ܺ���
public:
	//��ȡ������
	LPCTSTR GetLogonServer() { return m_strLogonServer; }
	//���͵�¼��
	bool SendLogonPacket(ITCPSocket * pIClientSocke);
	//��¼�ɹ�����
	bool OnLogonSuccess();

	void  HuiFu();
	//�ڲ�����
private:
	//���ط�����
	void LoadLogonServer();
	//��ȡ�ʺ�
	void LoadAccountsInfo();
	//������Ϣ
	void LoadProxyServerInfo();
	//Ч������
	bool CheckLogonInput(bool bShowError);
	//����ģʽ
	void SetLogonMode(enLogonMode LogonMode);

	//��������
private:
	//��������
	void UpdateUserPassWord(DWORD dwUserDBID);
	//����ģʽ
	void SwitchNetOption(bool bNetOption);
	//����ѡ��
	void UpdateUserComboBox(UINT uComboBoxID);
	//�����ַ�
	int ComboBoxFindString(CMyComboBox * pComboBox, LPCTSTR pszFindString);
	//��ȡ����
	void GetProxyInfo(enProxyServerType &ProxyServerType, tagProxyServerInfo &ProxyServerInfo);
	//�����ж�
	bool EnableProxy() { return m_bNetOption;}

	//��������
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos);

	//��Ϣ����
public:
	//ע���ʺ�
	afx_msg void OnRegisterAccounts();
	//ɾ���û�
	afx_msg void OnDeleteAccounts();
	//��������
	afx_msg void OnBnClickedNetOption();
	//�������
	afx_msg void OnBnClickedProxyTest();
	//����ı�
	afx_msg void OnEnChangePassword();
	//ѡ��ı�
	afx_msg void OnSelchangeAccounts();
	//ѡ��ı�
	afx_msg void OnSelchangeUserID();

	afx_msg void OnJiZhuPassword();
	//���͸ı�
	afx_msg void OnTcnSelchangeLogonType();
	//���͸ı�
	//afx_msg void OnTcnSelchangeLogonType1(NMHDR * pNMHDR, LRESULT * pResult);
		//���͸ı�
	afx_msg void OnTcnSelchangeLogonType1();
	//ʹ��ID
	afx_msg void OnCheckUseid();
	afx_msg void OnLogonUseAccount();
	afx_msg void OnLogonUseId();
	afx_msg void OnOption();
	//������Ϣ
	afx_msg BOOL OnEraseBkgnd(CDC * pDC);
	//�ػ���Ϣ
	afx_msg void OnPaint();
	//��ʱ����Ϣ
	afx_msg void OnTimer(UINT nIDEvent);
	//������С
	afx_msg void OnSize(UINT nType, int cx, int cy);
		afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    
	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif
