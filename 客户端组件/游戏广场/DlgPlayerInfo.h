#pragma once

#include "Stdafx.h"
#include "Resource.h"
//////////////////////////////////////////////////////////////////////////

//�һ���Ϸ�� 
class  CDlgPlayerInfo : public CSkinDialogEx
{

protected:
	tagUserData*				 m_pUserData;

	//�������
public:
	CGameRankManagerHelper				m_GameRankManager;					//��Ϸ�ȼ�
public:
	//�����û���Ϣ
	void	UpdataUserData(tagUserData* pUserData=NULL);
	//ˢ����ʾ
	void	UpdataUserInfo();
	//��������
public:
	//���캯��
	CDlgPlayerInfo(CWnd* pParent = NULL, tagUserData* pUserData = NULL); 
	//��������
	virtual ~CDlgPlayerInfo();
protected:
	//�û�״̬
	LPCTSTR GetUserStatus(tagUserData* pUserData);

	//�ؼ���
	virtual void DoDataExchange(CDataExchange* pDX);
	//��ʼ������
	virtual BOOL OnInitDialog();
	//ȷ����ť
	afx_msg void OnBnClickedOk();
	//�滭����
	//BOOL OnEraseBkgnd(CDC * pDC);
	//�滭��Ϣ
	//afx_msg void OnPaint();
	//��ɫ��Ϣ
	//afx_msg HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);

	DECLARE_MESSAGE_MAP()
};