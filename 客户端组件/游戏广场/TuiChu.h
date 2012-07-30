#pragma once

#include "Stdafx.h"
#include "Resource.h"
//////////////////////////////////////////////////////////////////////////

//�һ���Ϸ�� 
class  CDTuiChu : public CSkinDialogEx
{
	//��������
public:
    ITCPSocket					  * m_pIClientSocket;				//�������
	LONGLONG						m_lGameGold;					//������Ŀ
	LONG							m_lGameBank;					//������Ŀ
	LONGLONG						m_lStorageGold;					//�洢��Ŀ
	LONGLONG						m_lInCount;						//������Ϸ��
    DWORD                           UserID;                         //�Է��û�ID��
	TCHAR                           szPassword1[PASS_LEN];
	bool                            KongZhi;
	CWebBrowser							m_BrowerAD;						//�������
	//�ؼ�����
protected:
	CSkinButton						m_btOK;							//ȷ����ť
	CSkinButton						m_btCancel;						//ȡ����ť
	CSkinButton						m_btTQ;							//��ȡ��ť
	CSkinButton						m_btCR;							//���밴ť
	CSkinButton						m_btZZ;							//ת�˰�ť
	CSkinButton						m_btPB;							//�������밴ť
	bool                            tiqu;                           //��ȡ
	bool                            cunchu;                         //����
	bool                            zhuanzhang;                     //ת��
	bool                             mima;                          //���������޸�
    CString							TCText;				            //��ȡ�ʹ������
	CString 					    ZZText;				            //��ȡ�ʹ������
	CString 					    GGText;				            //��ȡ�ʹ������
	CString 					    BankText;				        //��ȡ�ʹ������
   
	//�ӿڱ���
protected:	
	IClientKernel					* m_pIClientKernel;				//�ں˽ӿ�
	ITCPSocket						* m_pClientSocket;				//����ӿ�
    protected:
	IPurchaseInfo					* m_pIPurchaseInfo;			    //��Ϣ�ӿ�
	//��������
public:
	//���캯��
	CDTuiChu(CWnd* pParent = NULL); 
	//��������
	virtual ~CDTuiChu();

	//��������
public:
	//���½���
	void UpdateView();
	bool ConnectServer();

	//���غ���
//ȷ����ť
	afx_msg void OnBnClickedOk();
	afx_msg void OnChangOk1();
protected:
	//�ؼ���
	virtual void DoDataExchange(CDataExchange* pDX);
	//��ʼ������
	virtual BOOL OnInitDialog();
	//ȷ����ť
	//afx_msg void OnBnClickedOk();
	//��ȡ��Ť
    afx_msg void OnBnTiQuOk();
	//���밴ť 
    afx_msg void OnBnCunRuOk();
	//ת�˰�ť 
    afx_msg void OnZhuanZhangOk();
	//��������
    afx_msg void OnPassBankOk();
	//�����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//�����Ϣ
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//������Ϣ
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//ʱ����Ϣ
	afx_msg void OnTimer(UINT nIDEvent);
	//������Ϣ
	afx_msg void OnEnChangeInCount();
	afx_msg void OnPaint();
	//������Ϣ
	afx_msg void OnEnChangeInID();

	afx_msg void OnSize(UINT nType, int cx, int cy);

	//���ܺ���
public:
	///������Ϣ�ӿ�
	void SetPurchaseInfoSink(IPurchaseInfo *pIPurchaseInfo);

	//�Ƿ�ѡ��
	bool IsButtonSelected(UINT uButtonID);
	//ѡ�а�ť
	void SetButtonSelected(UINT uButtonID, bool bSelected);
	//�������
	void SetSendInfo(IClientKernel *pClientKernel,ITCPSocket *pClientSocket);
	//������Ϣ
	void SendData(WORD wMainCmdID, WORD wSubCmdID, void * pBuffer, WORD wDataSize);
   //��������
	void DrawTextString();
	DECLARE_MESSAGE_MAP()
};