#include "Stdafx.h"
#include "resource.h"
#include "GlobalUnits.h"
#include "DlgPlayerInfo.h"


//////////////////////////////////////////////////////////////////////////


BEGIN_MESSAGE_MAP(CDlgPlayerInfo, CSkinDialogEx)
	//ON_WM_PAINT()
	//ON_WM_CTLCOLOR()
	//ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDOK,OnBnClickedOk)   //ȷ��
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CDlgPlayerInfo::CDlgPlayerInfo(CWnd* pParent, tagUserData* pUserData):CSkinDialogEx(IDD_PLAYER_INFO, pParent)
{
	m_pUserData = pUserData;
}

//��������
CDlgPlayerInfo::~CDlgPlayerInfo()
{
}
//�ؼ���
void CDlgPlayerInfo::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	//DDX_Control(pDX, IDOK, m_btOK);

}
//��ȡ��ť
void CDlgPlayerInfo::OnBnClickedOk()
{

	//AfxGetMainWnd()->PostMessage(WM_CLOSE,0,0);
	return;
}

//��ʼ������
BOOL CDlgPlayerInfo::OnInitDialog()
{
	__super::OnInitDialog();

	SetWindowText("�û���Ϣ");
	if (m_pUserData == NULL)
	{
		//AfxMessageBox("�û���Ϣ�����ڣ��˳���");
		DestroyWindow();
		return FALSE;
	}
	//SetDlgItemText(IDD_PLAYER_INFO)

	
	//��Ϸ�ȼ�
	m_GameRankManager.CreateInstance();
	ASSERT(m_GameRankManager.GetInterface()!=NULL);//pszGameName
	//bool bSuccess=m_GameRankManager->LoadGameLevel("ox", GAME_GENRE_GOLD);

	UpdataUserInfo();
	return TRUE;
}

LPCTSTR CDlgPlayerInfo::GetUserStatus( tagUserData* pUserData )
{
	static TCHAR szBuff[128]={0};
	if(pUserData != NULL)
	{
		switch(pUserData->cbUserStatus)
		{
		case US_PLAY:
			_snprintf(szBuff,CountArray(szBuff), TEXT("������Ϸ��[����:%d]"), pUserData->wTableID);
			break;
		case US_SIT:
			_snprintf(szBuff,CountArray(szBuff), TEXT("�û�����[����:%d]"), pUserData->wTableID);
			break;
		case US_READY:
			_snprintf(szBuff,CountArray(szBuff), TEXT("׼����[����:%d]"), pUserData->wTableID);
			break;
		default:
			lstrcpy(szBuff, TEXT("������"));
			break;
		}
	}
	return szBuff;
}

void CDlgPlayerInfo::UpdataUserData( tagUserData* pUserData )
{
	if(m_hWnd!=NULL && pUserData != NULL)
		m_pUserData = pUserData;
}

void CDlgPlayerInfo::UpdataUserInfo()
{
	//��д�û���������
	TCHAR szBuff[256];
	SetDlgItemText(IDC_DLGPLAYERINFO_USERNAME, m_pUserData->szName);
	_snprintf(szBuff, CountArray(szBuff), TEXT("%d"), m_pUserData->dwGameID);
	SetDlgItemText(IDC_DLGPLAYERINFO_ID, szBuff);

	_snprintf(szBuff, CountArray(szBuff), TEXT("%s"), GetUserStatus(m_pUserData));
	SetDlgItemText(IDC_DLGPLAYERINFO_STATUS, szBuff);
	SetDlgItemText(IDC_DLGPLAYERINFO_UNWRITE, m_pUserData->szUnderWrite);

	//��Ϸ����
	_snprintf(szBuff, CountArray(szBuff), TEXT("%d"), m_pUserData->lExperience);
	SetDlgItemText(IDC_DLGPLAYERINFO_JIFEN, szBuff);

	_snprintf(szBuff, CountArray(szBuff), TEXT("%I64d"), m_pUserData->lScore);
	SetDlgItemText(IDC_DLGPLAYERINFO_SCORE, szBuff);

	TCHAR szLevel[128];
	if (m_GameRankManager.GetInterface()!=NULL)
	{
		LPCTSTR pszGameRank=m_GameRankManager->GetLevelDescribe(m_pUserData->lScore);
		lstrcpyn(szLevel,pszGameRank,sizeof(szLevel));
	}else szLevel[0] = 0;
	SetDlgItemText(IDC_DLGPLAYERINFO_LEVER, szLevel);

	_snprintf(szBuff, CountArray(szBuff), TEXT("Ӯ: %ld,��: %ld,��: %ld,��: %ld"), m_pUserData->lWinCount, m_pUserData->lLostCount, m_pUserData->lDrawCount, m_pUserData->lFleeCount);
	SetDlgItemText(IDC_DLGPLAYERINFO_ZHANJI, szBuff);
	UpdateData(FALSE);
}

// BOOL CDlgPlayerInfo::OnEraseBkgnd( CDC * pDC )
// {
// 	Invalidate(FALSE);
// 	UpdateWindow();
// 	return TRUE;
// }
// 
// void CDlgPlayerInfo::OnPaint()
// {
// // 	CPaintDC dc(this);
// // 	//�滭�߿�
// // 	DrawBorder(&dc);
// // 
// // 	//�滭����
// // 	DrawBackGround(&dc);
// 
// 	CPaintDC dc(this);
// 
// 	//��ȡλ��
// 	CRect rcRect;
// 	GetClientRect(&rcRect);
// // 
// // 	//������Դ
// // 	CImageHandle ImageHotHandle(&m_ImageHot);
// // 	CImageHandle ImageActiveHandle(&m_ImageActive);
// // 	CImageHandle ImageNormalHandle(&m_ImageNormal);
// // 	CImageHandle m_ImageBackGroundHandle(&m_ImageBackGround);
// 
// 	//��������
// 	CDC BufferDC;
// 	CBitmap BufferBmp;
// 	BufferDC.CreateCompatibleDC(&dc);
// 	BufferBmp.CreateCompatibleBitmap(&dc,rcRect.Width(),rcRect.Height());
// 	BufferDC.SelectObject(&BufferBmp);
// 
// 	//�滭����
// 	CBrush BackBrush(RGB(255,255,254));
// 	CPen BorderPen(PS_SOLID,1,CSkinWndObject::m_SkinAttribute.m_crInsideBorder);
// 	CPen * pOldPen=BufferDC.SelectObject(&BorderPen);
// 	CBrush * pOldBrush=BufferDC.SelectObject(&BackBrush);
// 	BufferDC.FillSolidRect(&rcRect,CSkinWndObject::m_SkinAttribute.m_crBackGround);
// 	BufferDC.RoundRect(&rcRect,CPoint(10,10));
// 	BufferDC.SelectObject(pOldPen);
// 	BufferDC.SelectObject(pOldBrush);
// 	BackBrush.DeleteObject();
// 	BorderPen.DeleteObject();
// 
// 	//�滭��ť
// 	BufferDC.SetBkMode(TRANSPARENT);
// 	BufferDC.SelectObject(CSkinResourceManager::GetDefaultFont());
// 
// 
// 	//tagGlobalUserData & GlobalUserInfo=g_GlobalUnits.GetGolbalUserData();
// 	if( m_pUserData->dwUserID != 0)
// 	{
// 		CRect m_rcFace = CRect(27-4,27-4,27+48+4,27+48+4);
// 
// 		//�û�ͷ��
// 		g_GlobalUnits.m_UserFaceRes->DrawNormalFace(&BufferDC,27,27,m_pUserData->wFaceID,m_pUserData->dwUserID,m_pUserData->dwCustomFaceVer,48,48);
// 	}
// 
// 
// 	//�滭����
// 	dc.BitBlt(0,0,rcRect.Width(),rcRect.Height(),&BufferDC,0,0,SRCCOPY);
// 
// 	//������Դ
// 	BufferDC.DeleteDC();
// 	BufferBmp.DeleteObject();
// 
// 	DrawBorder(&dc);
// 	//�滭����
// 	DrawCaption(&dc);
// 	//�滭����
// 	DrawBackGround(&dc);
// 
// }
// 
// //��ɫ��Ϣ
// HBRUSH  CDlgPlayerInfo::OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor)
// {
// 
// 	if(pWnd->GetDlgCtrlID()==IDC_GROUP1||pWnd->GetDlgCtrlID()==IDC_GROUP2)
// 	{
// 		pDC->SetBkColor(CSkinDialog::m_SkinAttribute.m_crBackGround);
// 		pDC->SetTextColor(CSkinDialog::m_SkinAttribute.m_crControlTXColor);
// 
// 		HBRUSH hr = CreateSolidBrush(CSkinDialog::m_SkinAttribute.m_crBackGround);
// 		return hr;
// 
// 	}
// 
// 	switch (nCtlColor)
// 	{
// 	case CTLCOLOR_DLG:
// 	case CTLCOLOR_STATIC:
// 		{
// 			pDC->SetBkMode(TRANSPARENT);
// 			pDC->SetBkColor(CSkinDialog::m_SkinAttribute.m_crBackGround);
// 			pDC->SetTextColor(CSkinDialog::m_SkinAttribute.m_crControlTXColor);
// 			return CSkinDialog::m_SkinAttribute.m_brBackGround;
// 		}
// 	}
// 	return __super::OnCtlColor(pDC,pWnd,nCtlColor);
// }


