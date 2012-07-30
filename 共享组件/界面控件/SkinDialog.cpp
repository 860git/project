#include "Stdafx.h"
#include "AfxPriv.h"
#include "SkinDialog.h"

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CSkinDialog, CDialog)

BEGIN_MESSAGE_MAP(CSkinDialog, CDialog)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_CTLCOLOR()
	ON_WM_ACTIVATE()
	ON_WM_ACTIVATEAPP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_SETTINGCHANGE()
	ON_WM_WINDOWPOSCHANGED()
	ON_MESSAGE(WM_SETTEXT,OnSetTextMesage)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CSkinDialog1, CDialog)

BEGIN_MESSAGE_MAP(CSkinDialog1, CDialog)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_CTLCOLOR()
	ON_WM_ACTIVATE()
	ON_WM_ACTIVATEAPP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_SETTINGCHANGE()
	ON_WM_WINDOWPOSCHANGED()
	ON_MESSAGE(WM_SETTEXT,OnSetTextMesage)
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CSkinDialogEx, CSkinDialog)

BEGIN_MESSAGE_MAP(CSkinDialogEx,CSkinDialog)
	ON_WM_SIZE()
	ON_WM_PAINT()
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CSkinDialogEx1, CSkinDialog1)

BEGIN_MESSAGE_MAP(CSkinDialogEx1,CSkinDialog1)
	ON_WM_SIZE()
	ON_WM_PAINT()
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CRgnDialog, CDialog)

BEGIN_MESSAGE_MAP(CRgnDialog,CDialog)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CSkinDialog::CSkinDialog(UINT nIDTemplate, CWnd * pParentWnd) 
	: CDialog(nIDTemplate,pParentWnd), CSkinWndObject(this)
{
}

//��������
CSkinDialog::~CSkinDialog()
{
}

//��ʼ����Ϣ
BOOL CSkinDialog::OnInitDialog()
{
	__super::OnInitDialog();

	//��ʼ��
	InitSkinObject();
	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);
    

	return TRUE;
}

//������Ϣ
void CSkinDialog::OnActivate(UINT nState, CWnd * pWndOther, BOOL bMinimized)
{
	__super::OnActivate(nState,pWndOther,bMinimized);

	m_bActive=(nState!=WA_INACTIVE);
	DrawSkinView(NULL);

	return;
}

//������Ϣ
void CSkinDialog::OnActivateApp(BOOL bActive, DWORD dwThreadID)
{
	__super::OnActivateApp(bActive,dwThreadID);

	m_bActive=bActive?true:false;
	DrawSkinView(NULL);

	return;
}

//�����Ϣ
void CSkinDialog::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	__super::OnLButtonDblClk(nFlags,point);
	OnLMouseDoubleEvent(point);
	return;
}

//�����Ϣ
void CSkinDialog::OnLButtonDown(UINT nFlags, CPoint point)
{
	__super::OnLButtonDown(nFlags,point);
	OnLMouseDownEvent(point);
	return;
}

//�����Ϣ
void CSkinDialog::OnLButtonUp(UINT nFlags, CPoint point)
{
	__super::OnLButtonUp(nFlags,point);
	OnLMouseUpEvent(point);
	return;
}

//�����Ϣ
void CSkinDialog::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	__super::OnRButtonDblClk(nFlags,point);
	OnRMouseDoubleEvent(point);
	return;
}

//�����Ϣ
void CSkinDialog::OnRButtonDown(UINT nFlags, CPoint point)
{
	__super::OnRButtonDown(nFlags,point);
	OnRMouseDownEvent(point);
	return;
}

//�����Ϣ
void CSkinDialog::OnRButtonUp(UINT nFlags, CPoint point)
{
	__super::OnRButtonUp(nFlags,point);
	OnRMouseUpEvent(point);
	return;
}

//�����Ϣ
void CSkinDialog::OnMouseMove(UINT nFlags, CPoint point)
{
	__super::OnMouseMove(nFlags,point);
	OnMouseMoveEvent(point);
	return;
}

//�滭��Ϣ 
void CSkinDialog::OnPaint()
{
	CPaintDC dc(this);
	DrawSkinView(&dc);
	return;
}

//λ�øı�
void CSkinDialog::OnWindowPosChanged(WINDOWPOS * lpwndpos)
{
	__super::OnWindowPosChanged(lpwndpos);

	//����λ��
	CRect rcClient;
	m_cbNowHotButton=255;
	m_cbNowDownButton=255;
	GetClientRect(&rcClient);
	AdjustTitleButton(rcClient.Width(),rcClient.Height());

	//�ػ�����
	DrawSkinView(NULL);

	return;
}

//���øı�
void CSkinDialog::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	__super::OnSettingChange(uFlags, lpszSection);

	//������ܴ�С
	CSkinWndObject::OnSettingChange();

	return;
}

//�滭����
BOOL CSkinDialog::OnEraseBkgnd(CDC * pDC)
{
	Invalidate(FALSE);
	UpdateWindow();
	return TRUE;
}

//�ؼ���ɫ
HBRUSH CSkinDialog::OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor)
{
	switch (nCtlColor)
	{
	case CTLCOLOR_DLG:
	case CTLCOLOR_STATIC:
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetBkColor(m_SkinAttribute.m_crBackGround);
			pDC->SetTextColor(m_SkinAttribute.m_crControlTXColor);
			return m_SkinAttribute.m_brBackGround;
		}
	}
	return __super::OnCtlColor(pDC,pWnd,nCtlColor);
}

//������Ϣ
LRESULT	CSkinDialog::OnSetTextMesage(WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult=DefWindowProc(WM_SETTEXT,wParam,lParam);
	DrawSkinView(NULL);
	return lResult;
}

//////////////////////////////////////////////////////////////////////////
//���캯��
CSkinDialog1::CSkinDialog1(UINT nIDTemplate1, CWnd * pParentWnd1) 
	: CDialog(nIDTemplate1,pParentWnd1), CSkinWndObject(this)
{
}

//��������
CSkinDialog1::~CSkinDialog1()
{
}

//��ʼ����Ϣ
BOOL CSkinDialog1::OnInitDialog()
{
	__super::OnInitDialog();

	//��ʼ��
	InitSkinObject();
	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);

	return TRUE;
}

//������Ϣ
void CSkinDialog1::OnActivate(UINT nState, CWnd * pWndOther, BOOL bMinimized)
{
	__super::OnActivate(nState,pWndOther,bMinimized);

	m_bActive=(nState!=WA_INACTIVE);
	DrawSkinView1(NULL);

	return;
}

//������Ϣ
void CSkinDialog1::OnActivateApp(BOOL bActive, DWORD dwThreadID)
{
	__super::OnActivateApp(bActive,dwThreadID);

	m_bActive=bActive?true:false;
	DrawSkinView1(NULL);

	return;
}

//�����Ϣ
void CSkinDialog1::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	__super::OnLButtonDblClk(nFlags,point);
	OnLMouseDoubleEvent(point);
	return;
}

//�����Ϣ
void CSkinDialog1::OnLButtonDown(UINT nFlags, CPoint point)
{
	__super::OnLButtonDown(nFlags,point);
	OnLMouseDownEvent1(point);
	return;
}

//�����Ϣ
void CSkinDialog1::OnLButtonUp(UINT nFlags, CPoint point)
{
	__super::OnLButtonUp(nFlags,point);
	OnLMouseUpEvent1(point);
	return;
}

//�����Ϣ
void CSkinDialog1::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	__super::OnRButtonDblClk(nFlags,point);
	OnRMouseDoubleEvent(point);
	return;
}

//�����Ϣ
void CSkinDialog1::OnRButtonDown(UINT nFlags, CPoint point)
{
	__super::OnRButtonDown(nFlags,point);
	OnRMouseDownEvent(point);
	return;
}

//�����Ϣ
void CSkinDialog1::OnRButtonUp(UINT nFlags, CPoint point)
{
	__super::OnRButtonUp(nFlags,point);
	OnRMouseUpEvent(point);
	return;
}

//�����Ϣ
void CSkinDialog1::OnMouseMove(UINT nFlags, CPoint point)
{
	__super::OnMouseMove(nFlags,point);
	OnMouseMoveEvent1(point);
	return;
}

//�滭��Ϣ 
void CSkinDialog1::OnPaint()
{
	CPaintDC dc(this);
	DrawSkinView1(&dc);
	return;
}

//λ�øı�
void CSkinDialog1::OnWindowPosChanged(WINDOWPOS * lpwndpos)
{
	__super::OnWindowPosChanged(lpwndpos);

	//����λ��
	CRect rcClient;
	m_cbNowHotButton=255;
	m_cbNowDownButton=255;
	GetClientRect(&rcClient);
	AdjustTitleButton(rcClient.Width(),rcClient.Height());

	//�ػ�����
	DrawSkinView1(NULL);

	return;
}

//���øı�
void CSkinDialog1::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	__super::OnSettingChange(uFlags, lpszSection);

	//������ܴ�С
	CSkinWndObject::OnSettingChange();

	return;
}

//�滭����
BOOL CSkinDialog1::OnEraseBkgnd(CDC * pDC)
{
	Invalidate(FALSE);
	UpdateWindow();
	return TRUE;
}

//�ؼ���ɫ
HBRUSH CSkinDialog1::OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor)
{
	switch (nCtlColor)
	{
	case CTLCOLOR_DLG:
	case CTLCOLOR_STATIC:
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetBkColor(m_SkinAttribute.m_crBackGround);
			pDC->SetTextColor(m_SkinAttribute.m_crControlTXColor);
			return m_SkinAttribute.m_brBackGround;
		}
	}
	return __super::OnCtlColor(pDC,pWnd,nCtlColor);
}

//������Ϣ
LRESULT	CSkinDialog1::OnSetTextMesage(WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult=DefWindowProc(WM_SETTEXT,wParam,lParam);
	DrawSkinView1(NULL);
	return lResult;
}
//////////////////////////////////////////////////////////////////////////
//���캯��
CSkinDialogEx::CSkinDialogEx(UINT nIDTemplate, CWnd * pParentWnd) : CSkinDialog(nIDTemplate,pParentWnd)
{
}

//��������
CSkinDialogEx::~CSkinDialogEx()
{
}

//�滭�߿�
void CSkinDialogEx::DrawBorder(CDC * pDC)
{
	//��ȡλ��
	CRect ClientRect;
	GetClientRect(&ClientRect);
	int nXExcursion=GetXExcursionPos();
	int nYExcursion=GetYExcursionPos();

	//�����ʻ�
	CPen BorderPen(PS_SOLID,1,m_SkinAttribute.m_crBackGround);
	CPen * pOldPen=pDC->SelectObject(&BorderPen);

	CImageHandle TitleLeft1(&m_SkinAttribute.m_FramLeft);
	if (m_SkinAttribute.m_FramLeft.IsNull()) return ;
	//�滭�ڿ�
	CRect rcDrawRect(nXExcursion,nYExcursion,ClientRect.Width()-nXExcursion,ClientRect.Height());
	pDC->SelectObject(m_SkinAttribute.m_brBackGround);

// 	if (m_bMaxSize==true) rcDrawRect.DeflateRect(4,4,4,4);
// 	else rcDrawRect.DeflateRect(4,4,4,4);
	pDC->RoundRect(&rcDrawRect,CPoint(0,0));
    
	//	CImageHandle TitleLeft1(&m_SkinAttribute.m_TitleLeft1);
	//if (m_SkinAttribute.m_TitleLeft1.IsNull()) return ;
	//CImageHandle TitleMid1(&m_SkinAttribute.m_TitleMid1);
	//if (m_SkinAttribute.m_TitleMid1.IsNull()) return ;
	//CImageHandle TitleRight1(&m_SkinAttribute.m_TitleRight1);
	//if (m_SkinAttribute.m_TitleRight1.IsNull()) return ;

	//int nLeftWidth=TitleLeft1->GetHeight();
	//int nMidWidth=TitleMid1->GetWidth();
	//int nRightWidth=TitleRight1->GetWidth();

	////for (int i=rcDrawRect.top;i<rcDrawRect.bottom;i+=nLeftWidth)
	//TitleLeft1->BitBlt(pDC->m_hDC,rcDrawRect.left,rcDrawRect.top);
	//pDC->Draw3dRect(&rcDrawRect,RGB(255,0,0),RGB(255,0,0));

	//���Ʊ���
// 	CImageHandle BackGroundT(&m_SkinAttribute.m_BackGroundTop);
// 	CImageHandle BackGroundM(&m_SkinAttribute.m_BackGroundMid);
// 	CImageHandle BackGroundB(&m_SkinAttribute.m_BackGroundBtm);
// 	
// 	for (int xPox = rcDrawRect.left; xPox < rcDrawRect.right; xPox+=m_SkinAttribute.m_BackGroundMid.GetWidth())
// 	{
// 		m_SkinAttribute.m_BackGroundTop.BitBlt(pDC->GetSafeHdc(), xPox, rcDrawRect.top);
// 		for (int yMid=rcDrawRect.top+m_SkinAttribute.m_BackGroundTop.GetHeight(); 
// 			 yMid < rcDrawRect.bottom-m_SkinAttribute.m_BackGroundBtm.GetHeight(); yMid+=m_SkinAttribute.m_BackGroundMid.GetHeight())
// 		{
// 			m_SkinAttribute.m_BackGroundMid.BitBlt(pDC->GetSafeHdc(), xPox, yMid);
// 		}
// 		m_SkinAttribute.m_BackGroundBtm.BitBlt(pDC->GetSafeHdc(), xPox, rcDrawRect.bottom-m_SkinAttribute.m_BackGroundBtm.GetHeight());
// 	}

	//������Դ
	pDC->SelectObject(pOldPen);
	BorderPen.DeleteObject();

	return ;
}

//�滭����
void CSkinDialogEx::DrawCaption(CDC * pDC)
{
	//�滭����
	DrawSkinView(pDC);

	return ;
}

//�滭����
void CSkinDialogEx::DrawBackGround(CDC * pDC)
{
	//��ȡλ��
	CRect ClientRect;
	GetClientRect(&ClientRect);

	//�滭����
	int nXExcursion1=GetXExcursionPos();
	int nYExcursion1=GetYExcursionPos();
	CImageHandle TitleLeft1(&m_SkinAttribute.m_FramLeft);
	if (m_SkinAttribute.m_FramLeft.IsNull()) return ;
	CImageHandle TitleMid1(&m_SkinAttribute.m_BottomM);
	if (m_SkinAttribute.m_BottomM.IsNull()) return ;
	CImageHandle TitleRight1(&m_SkinAttribute.m_FramRight);
	if (m_SkinAttribute.m_FramRight.IsNull()) return ;

	CImageHandle TitleLeftL1(&m_SkinAttribute.m_BottomL);
	if (m_SkinAttribute.m_FramRight.IsNull()) return ;

	CImageHandle TitleRightR1(&m_SkinAttribute.m_BottomR);
	if (m_SkinAttribute.m_FramRight.IsNull()) return ;


	int nLeftWidth=TitleLeft1->GetHeight();
	int nMidWidth=TitleMid1->GetWidth();
	int nRightWidth=TitleRight1->GetHeight();
    
	for (int i=ClientRect.top+26;i<ClientRect.bottom-TitleLeftL1->GetHeight();i+=nLeftWidth)
	TitleLeft1->BitBlt(pDC->GetSafeHdc(),nXExcursion1,i);

	for (int i=ClientRect.top+26;i<ClientRect.bottom-TitleRightR1->GetHeight();i+=nRightWidth)
		TitleRight1->BitBlt(pDC->GetSafeHdc(),ClientRect.Width()-nXExcursion1-TitleRight1->GetWidth(),i);

	for (int i=nXExcursion1+TitleLeftL1->GetWidth();i<ClientRect.Width()-TitleRight1->GetWidth()-nXExcursion1;i+=nMidWidth)
		TitleMid1->BitBlt(pDC->GetSafeHdc(),i,ClientRect.Height()-TitleMid1->GetHeight());

    TitleLeftL1->BitBlt(pDC->GetSafeHdc(),nXExcursion1,ClientRect.Height()-TitleLeftL1->GetHeight());

	TitleRightR1->BitBlt(pDC->GetSafeHdc(),ClientRect.Width()-TitleRightR1->GetWidth()-nXExcursion1,ClientRect.Height()-TitleRightR1->GetHeight());
	return;
}

//�ػ���Ϣ
void CSkinDialogEx::OnPaint()
{
	CPaintDC dc(this);

	//�滭����
	//DrawBackGround(&dc);

	//�滭�߿�
	DrawBorder(&dc);
	//�滭����
	DrawCaption(&dc);
	//�滭����
	DrawBackGround(&dc);

// 	CRect rcClient1;
// 	GetWindowRect(&rcClient1);
// 	int nWidth1=rcClient1.Width();
// 	int nHeight1=rcClient1.Height();
// 	//�����ж�
// 	if ((nWidth1!=0)&&(nHeight1!=0))
// 	{
// 		//���λ��
// 		CRect rcFrame;//=rcClient1;
// 		rcFrame.SetRect(0,0,nWidth1,nHeight1);
// 
// 		//��������
// 		CRgn RegionWindow;
// 		RegionWindow.CreateRoundRectRgn(rcFrame.left,rcFrame.top,rcFrame.right+1,rcFrame.bottom+1,5,5);
// 
// 		//��������
// 		SetWindowRgn(RegionWindow,TRUE);
// 	}
	///////////////////////////////////////////////////////////////////////////


	return;
}

//λ����Ϣ
void CSkinDialogEx::OnSize(UINT nType, int cx, int cy)
{
	CSkinDialog::OnSize(nType, cx, cy);

	/////////////////////////////////////////////////////////////////////////////


	//ˢ�½���
	Invalidate(FALSE);

	return;
}

//////////////////////////////////////////////////////////////////////////
CSkinDialogEx1::CSkinDialogEx1(UINT nIDTemplate1, CWnd * pParentWnd1) : CSkinDialog1(nIDTemplate1,pParentWnd1)
{
}

//��������
CSkinDialogEx1::~CSkinDialogEx1()
{
}

//�滭�߿�
void CSkinDialogEx1::DrawBorder(CDC * pDC)
{
	//��ȡλ��
	CRect ClientRect;
	GetClientRect(&ClientRect);
	int nXExcursion=GetXExcursionPos();
	int nYExcursion=GetYExcursionPos();

	//�����ʻ�
	CPen BorderPen(PS_SOLID,1,m_SkinAttribute.m_crInsideBorder);
	CPen * pOldPen=pDC->SelectObject(&BorderPen);

	//�滭�ڿ�
	CRect rcDrawRect(nXExcursion,nYExcursion,ClientRect.Width()-2*nXExcursion,ClientRect.Height());
	pDC->SelectObject(m_SkinAttribute.m_brBackGround);
	if (m_bMaxSize==true) rcDrawRect.DeflateRect(3,3,3,2);
	else rcDrawRect.DeflateRect(3,4,2,4);
	pDC->RoundRect(&rcDrawRect,CPoint(8,8));

	//������Դ
	pDC->SelectObject(pOldPen);
	BorderPen.DeleteObject();

	return ;
}

//�滭����
void CSkinDialogEx1::DrawCaption(CDC * pDC)
{
	//�滭����
	DrawSkinView1(pDC);

	return ;
}

//�滭����
void CSkinDialogEx1::DrawBackGround(CDC * pDC)
{
	//��ȡλ��
	CRect ClientRect;
	GetClientRect(&ClientRect);

	//�滭����
	int nXExcursion=GetXExcursionPos();
	int nYExcursion=GetYExcursionPos();
	pDC->FillSolidRect(nXExcursion,nYExcursion,ClientRect.Width()-2*nXExcursion,4,m_SkinAttribute.m_crBackFrame);
	pDC->FillSolidRect(nXExcursion,ClientRect.Height()-8,ClientRect.Width()-2*nXExcursion,7,m_SkinAttribute.m_crBackFrame);
	pDC->FillSolidRect(nXExcursion,nYExcursion+4,12,ClientRect.Height()-nYExcursion-9,m_SkinAttribute.m_crBackFrame);
	pDC->FillSolidRect(ClientRect.Width()-nXExcursion-12,nYExcursion+4,12,ClientRect.Height()-nYExcursion-9,m_SkinAttribute.m_crBackFrame);

	return;
}

//�ػ���Ϣ
void CSkinDialogEx1::OnPaint()
{
	CPaintDC dc(this);
   
	//�滭����
	//DrawCaption(&dc);

	////�滭����
	//DrawBackGround(&dc);

	////�滭�߿�
	//DrawBorder(&dc);

	return;
}

//λ����Ϣ
void CSkinDialogEx1::OnSize(UINT nType, int cx, int cy)
{
	CSkinDialog1::OnSize(nType, cx, cy);

	//ˢ�½���
	Invalidate(FALSE);
	/////////////////////////////////////////////////////////////////////////////
	//CRect rcClient;
	//GetWindowRect(&rcClient);
	//int nWidth=rcClient.Width();
	//int nHeight=rcClient.Height();
	////�����ж�
	//if ((nWidth!=0)&&(nHeight!=0))
	//{


	//	//���λ��
	//	CRect rcFrame;
	//	rcFrame.SetRect(5,5,nWidth-5,nHeight-5);

	//	//��������
	//	CRgn RegionWindow;
	//	RegionWindow.CreateRoundRectRgn(rcFrame.left,rcFrame.top,rcFrame.right+1,rcFrame.bottom+1,5,5);

	//	//��������
	//	SetWindowRgn(RegionWindow,TRUE);
	//}
	///////////////////////////////////////////////////////////////////////////
	return;
}

///////////////////////////////////////////////////////
//���캯��
CRgnDialog::CRgnDialog(UINT uTemplate, CWnd * pParent) : CDialog(uTemplate,pParent)
{
}

//��������
CRgnDialog::~CRgnDialog()
{
}

//��ʼ������
BOOL CRgnDialog::OnInitDialog()
{
	__super::OnInitDialog();

	//ɾ�����ڱ���ͻ�ȡ����
	ModifyStyle(WS_CAPTION,0,SWP_DRAWFRAME);
	ModifyStyleEx(WS_EX_CLIENTEDGE|WS_EX_WINDOWEDGE,0,0);
	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);

	return TRUE;
}

//�ػ�����
void CRgnDialog::OnPaint()
{
	CPaintDC dc(this);

	//�滭��ͼ
	CRect ClientRect;
	GetClientRect(&ClientRect);
	CImageHandle ImageHandle(&m_ImageBack);
	if (m_ImageBack.IsNull()==false) m_ImageBack.BitBlt(dc,0,0);

	return;
}

//����λͼ
bool CRgnDialog::LoadRgnImage(HINSTANCE hInstance, UINT uBitmapID, COLORREF crTrans)
{
	m_ImageBack.SetLoadInfo(uBitmapID,hInstance);
	return CreateControlRgn(crTrans);
}

//����λͼ
bool CRgnDialog::LoadRgnImage(LPCTSTR pszFileName, COLORREF crTrans)
{
	m_ImageBack.SetLoadInfo(pszFileName);
	return CreateControlRgn(crTrans);
}

//��������
bool CRgnDialog::CreateControlRgn(COLORREF crTrans)
{
	//������Դ
	CImageHandle ImageHandle(&m_ImageBack);

	//��������
	CRgn RgnControl;
	m_ImageBack.CreateImageRegion(RgnControl,crTrans);

	//���ô���
	if (RgnControl.GetSafeHandle()!=NULL)
	{
		//�ƶ�����
		SetWindowRgn(RgnControl,TRUE);
		SetWindowPos(NULL,0,0,m_ImageBack.GetWidth(),m_ImageBack.GetHeight(),SWP_NOMOVE);

		//ɾ������
		m_DialogRgn.DeleteObject();
		m_DialogRgn.Attach(RgnControl.Detach());

		return true;
	}

	return false;
}

//����Ҽ�������Ϣ
void CRgnDialog::OnLButtonDown(UINT nFlags, CPoint point)
{
	__super::OnLButtonDown(nFlags, point);

	//ģ�ⰴ����
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y));
}

//��������
BOOL CRgnDialog::OnEraseBkgnd(CDC * pDC)
{
	Invalidate(FALSE);
	UpdateWindow();
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
