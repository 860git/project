#pragma once

//��Ϸ���ƻ���
class IClientControlDlg : public CDialog 
{
public:
	IClientControlDlg(UINT UID, CWnd* pParent) : CDialog(UID, pParent){}
	virtual ~IClientControlDlg(void){}

public:
	//���¿���
	virtual void __cdecl OnAllowControl(bool bEnable) = NULL;
	//������
	virtual bool __cdecl ReqResult(const void * pBuffer) = NULL;
	virtual void __cdecl ResetUserBet() = NULL;
	virtual void __cdecl SetUserBetScore(BYTE cbArea,LONGLONG lScore) = NULL;
};
