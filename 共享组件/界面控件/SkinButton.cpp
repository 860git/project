#include "Stdafx.h"
#include "Resource.h"
#include "SkinButton.h"
#include ".\skinbutton.h"


//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CSkinButton, CButton)

BEGIN_MESSAGE_MAP(CSkinButton, CButton)
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_MESSAGE(WM_MOUSELEAVE,OnMouseLeave)
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

IMPLEMENT_DYNAMIC(CRgnButton, CButton)

BEGIN_MESSAGE_MAP(CRgnButton, CButton)
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_MESSAGE(WM_MOUSELEAVE,OnMouseLeave)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//��̬����
CSkinButtonAttribute				CSkinButton::m_SkinAttribute;			//��ť����

//////////////////////////////////////////////////////////////////////////

//���캯��
CSkinButtonAttribute::CSkinButtonAttribute() 
{
	//��ʼ������
	m_crTextColor=RGB(0,0,0);
	return;
}

//��������
CSkinButtonAttribute::~CSkinButtonAttribute() 
{
}

//��������
bool CSkinButtonAttribute::UpdateSkinResource(ISkinResource * pISkinResource)
{
	//��ȡ��Ϣ
	tagSkinButtonResource SkinButtonResource;
	if (pISkinResource!=NULL) pISkinResource->GetResourceInfo(SkinButtonResource);

	//���ñ���
	m_crTextColor=SkinButtonResource.crButtonText;
	m_ImageBack.SetLoadInfo(SkinButtonResource.LoadInfoButton);
	//m_ImageCheck.SetLoadInfo(SkinButtonResource.LoadInfoCheck);
	LoadInfoCheck = SkinButtonResource.LoadInfoCheck;
	LoadInfoPng = SkinButtonResource.LoadInfoPng;
	//m_pngButtonBack.LoadImage(SkinButtonResource.LoadInfoPng.hResourceDLL, SkinButtonResource.LoadInfoPng.strFileName);
	return true;
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CSkinButton::CSkinButton()
{
	m_hIcon=NULL;
	m_bExpand=true;
	m_bHovering=false;
	m_bShowTextFrame=false;
	m_pIMouseEvent=NULL;
	m_crTextColor=m_SkinAttribute.m_crTextColor;
	m_crTextFrameColor=DEF_TEXT_FRAME_COLOR;
//	m_BtnFont = (CSkinResourceManager::GetDefaultFont());
	
}

//��������
CSkinButton::~CSkinButton()
{
}

//���󸽼ӵ����д���
void CSkinButton::PreSubclassWindow()
{
	__super::PreSubclassWindow();
	SetButtonStyle(GetButtonStyle()|BS_OWNERDRAW);
	SendMessage(WM_SETFONT,(WPARAM)CSkinResourceManager::GetDefaultFont(),TRUE);
		CSkinResourceManager::m_DefaultFont.GetLogFont(&m_BtnFont);
	return;
}

//������Ϣ
int CSkinButton::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;
	SetButtonStyle(GetButtonStyle()|BS_OWNERDRAW);
	SendMessage(WM_SETFONT,(WPARAM)CSkinResourceManager::GetDefaultFont(),TRUE);

	CSkinResourceManager::m_DefaultFont.GetLogFont(&m_BtnFont);
	return 0;
}

//������ɫ
bool CSkinButton::SetTextColor(COLORREF crTextColor,COLORREF crTextFrameColor,bool bShowFrame)
{
	m_crTextColor=crTextColor;
	m_bShowTextFrame=bShowFrame;
	m_crTextFrameColor=crTextFrameColor;

	if (GetSafeHwnd()) Invalidate(FALSE);
	return true;
}

//����ͼ��
bool CSkinButton::SetButtonIcon(HICON hIcon)
{
	m_hIcon=hIcon;
	if (GetSafeHwnd()) Invalidate(FALSE);
	return true;
}

//����λͼ
bool CSkinButton::SetButtonImage(LPCTSTR pszFileName, bool bExpandImage)
{
	//Ч�����
	ASSERT(pszFileName);
	if (pszFileName==NULL) return false;

	//����λͼ
	m_bExpand=bExpandImage;
	m_ImageBack.SetLoadInfo(pszFileName);

	//����λ��
	if (m_bExpand==false) FixButtonSize();
	if (GetSafeHwnd()) Invalidate(FALSE);

	return true;
}

//����λͼ
bool CSkinButton::SetButtonImage(UINT uBitmapID, HINSTANCE hInstance, bool bExpandImage)
{
	//����λͼ
	ASSERT(uBitmapID!=0);
	if (uBitmapID==0) return false;
	m_bExpand=bExpandImage;
	m_ImageBack.SetLoadInfo(uBitmapID,hInstance);

	//����λ��
	if (m_bExpand==false) FixButtonSize();
	if (GetSafeHwnd()) Invalidate(FALSE);

	return true;
}

//��ȡλͼ����
bool CSkinButton::GetButtonLoadInfo(tagImageLoadInfo & LoadInfo)
{
	if (m_ImageBack.IsSetLoadInfo()) return m_ImageBack.GetLoadInfo(LoadInfo);
	return m_SkinAttribute.m_ImageBack.GetLoadInfo(LoadInfo);
}

//Ĭ�ϰ�ť
void CSkinButton::SetDefaultButton(bool bResetIcon, bool bResetColor, bool bResetImage)
{
	//�ָ�Ĭ��
	if (bResetImage) 
	{
		m_bExpand=true;
		m_ImageBack.RemoveLoadInfo();
	}
	if (bResetIcon)	m_hIcon=NULL;
	if (bResetColor)
	{
		m_bShowTextFrame = false;
		m_crTextColor=m_SkinAttribute.m_crTextColor;
		m_crTextFrameColor = DEF_TEXT_FRAME_COLOR;
	}

	//ˢ�½���
	if (GetSafeHwnd()) Invalidate(FALSE);

	return;
}

//����λ��
bool CSkinButton::FixButtonSize()
{
	if (m_ImageBack.IsSetLoadInfo()&&GetSafeHwnd())
	{
		CImageHandle ImageHandle(&m_ImageBack);
		SetWindowPos(NULL,0,0,ImageHandle->GetWidth()/5,ImageHandle->GetHeight(),SWP_NOMOVE);
		return true;
	}
	return false;
}

//����ƶ���Ϣ
void CSkinButton::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_bHovering==false)
	{
		//ע����Ϣ
		m_bHovering=true;
		Invalidate(FALSE);
		TRACKMOUSEEVENT TrackMouseEvent;
		TrackMouseEvent.cbSize=sizeof(TrackMouseEvent);
		TrackMouseEvent.dwFlags=TME_LEAVE;
		TrackMouseEvent.hwndTrack=GetSafeHwnd();
		TrackMouseEvent.dwHoverTime=HOVER_DEFAULT;
		_TrackMouseEvent(&TrackMouseEvent);
	}
	__super::OnMouseMove(nFlags, point);
}

//����뿪��Ϣ
LRESULT CSkinButton::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	m_bHovering=false;
	Invalidate(FALSE);
	return 0;
}

//��������
BOOL CSkinButton::OnEraseBkgnd(CDC * pDC)
{
	Invalidate(FALSE);
	UpdateWindow();
	return TRUE;
}

//����滭����
void CSkinButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{

	//���ر���ͼ
	CImageHandle ImageHandle;
	if (m_ImageBack.IsSetLoadInfo()) ImageHandle.AttachResource(&m_ImageBack);
	else 
	{
		CRect ClientRect1;
		GetClientRect(&ClientRect1);
		ImageHandle.AttachResource(&m_SkinAttribute.m_ImageBack);
		SetWindowPos(NULL,0,0,ClientRect1.Width(),ImageHandle->GetHeight(),SWP_NOMOVE);
	}

	//�������
	CRect ClientRect;
	GetClientRect(&ClientRect);

	bool bDisable=((lpDrawItemStruct->itemState&ODS_DISABLED)!=0);
	bool bButtonDown=((lpDrawItemStruct->itemState&ODS_SELECTED)!=0);
	bool bFocus = ((lpDrawItemStruct->itemState & ODS_FOCUS)!=0);

	//���� DC
	HDC hDC=lpDrawItemStruct->hDC;

	//��ȡ����
	CString strText;
	GetWindowText(strText);

	if (ImageHandle.IsResourceValid())
	{
		//����λͼλ��
		int iPartWidth=ImageHandle->GetWidth()/5;
		int iPartHeight=ImageHandle->GetHeight();
		int iDrawPos=0;

		if (bDisable) 
			iDrawPos = iPartWidth * 4;
		else if (bButtonDown) 
			iDrawPos = iPartWidth;
		else if (m_bHovering) 
			iDrawPos = iPartWidth * 3;
		else if( bFocus )
			iDrawPos = iPartWidth * 2;

		//�滭����ͼ
		if (m_bExpand==false) 
			ImageHandle->BitBlt(hDC,0,0,ClientRect.Width(),ClientRect.Height(),iDrawPos,0,SRCCOPY);
		else 
			ImageHandle->DrawSkin(CDC::FromHandle(hDC), &ClientRect, iDrawPos, 0, iPartWidth, iPartHeight, DS_STRETCH_ALL, 3, 3, 3, 3);
	}
	else
	{
		//�滭Ĭ�Ͻ���
		CDC * pDC=CDC::FromHandle(hDC);
		pDC->FillSolidRect(&ClientRect,GetSysColor(COLOR_BTNFACE));
		if (bButtonDown) pDC->Draw3dRect(&ClientRect,GetSysColor(COLOR_WINDOWFRAME),GetSysColor(COLOR_3DHILIGHT));
		else pDC->Draw3dRect(&ClientRect,GetSysColor(COLOR_3DHILIGHT),GetSysColor(COLOR_WINDOWFRAME));
	}

	//�滭ͼ��
	if (bButtonDown) 
	{
		ClientRect.top+=2;
//		ClientRect1.top+=2;
	}
	if (m_hIcon)
	{
		DrawIconEx(hDC,ClientRect.left+6,ClientRect.top+(ClientRect.Height()-16)/2+1,m_hIcon,16,16,0,NULL,DI_NORMAL);
		ClientRect.left+=22;
	}

	//�滭����
	ClientRect.top+=1;
//	ClientRect1.top+=2;
	//ClientRect1.left+=2;
	::SetBkMode(hDC,TRANSPARENT);

// 	CFont InfoFont;
// 	InfoFont.CreateFont(-24,0,0,0,700,0,0,0,134,3,2,1,1,TEXT("����_GB2312"));
	CFont Font;
	Font.CreateFontIndirect(&m_BtnFont);

	//////////////////////////////////////////////////////////////////////////
	CDC* dc = CDC::FromHandle(hDC);
	CFont* pOldFont = dc->SelectObject (&Font);
	if(!m_bShowTextFrame)
	{
		if (bDisable) ::SetTextColor(hDC,m_crTextColor/*RGB(0,0,0)*/);
		else ::SetTextColor(hDC,m_crTextColor/*RGB(0,0,0)*/);
		DrawText(hDC,strText,strText.GetLength(),ClientRect,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
	}
	//��������
	else
	{
       if(bDisable) ::SetTextColor(hDC,m_crTextColor/*RGB(0,0,0)*/);
		else ::SetTextColor(hDC,m_crTextColor/*RGB(0,0,0)*/);
		//DrawText(hDC,strText,strText.GetLength(),ClientRect1,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
		DrawTextString(CDC::FromHandle(hDC), strText, m_crTextColor, m_crTextFrameColor, ClientRect);
	}
	
	// Select old font in
	dc->SelectObject(pOldFont);


	return;
}

//Ĭ�ϻص�����
LRESULT CSkinButton::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_pIMouseEvent)
	{
		switch (message)
		{
		case WM_MOUSELEAVE:
			{
				UINT uButtonID=GetWindowLong(GetSafeHwnd(),GWL_ID);
				m_pIMouseEvent->OnEventMouseLeft(uButtonID,wParam,lParam);
				break;
			}
		case WM_MOUSEMOVE:
			{
				UINT uButtonID=GetWindowLong(GetSafeHwnd(),GWL_ID);
				m_pIMouseEvent->OnEventMouseMove(uButtonID,wParam,lParam);
				break;
			}
		case WM_LBUTTONDOWN:
			{
				UINT uButtonID=GetWindowLong(GetSafeHwnd(),GWL_ID);
				m_pIMouseEvent->OnEventLButtonDown(uButtonID,wParam,lParam);
				break;
			}
		case WM_LBUTTONUP:
			{
				UINT uButtonID=GetWindowLong(GetSafeHwnd(),GWL_ID);
				m_pIMouseEvent->OnEventLButtonUp(uButtonID,wParam,lParam);
				break;
			}
		case WM_RBUTTONDOWN:
			{
				UINT uButtonID=GetWindowLong(GetSafeHwnd(),GWL_ID);
				m_pIMouseEvent->OnEventRButtonDown(uButtonID,wParam,lParam);
				break;
			}
		case WM_RBUTTONUP:
			{
				UINT uButtonID=GetWindowLong(GetSafeHwnd(),GWL_ID);
				m_pIMouseEvent->OnEventRButtonUp(uButtonID,wParam,lParam);
				break;
			}
		}
	}
	return __super::DefWindowProc(message, wParam, lParam);
}

//��������
void CSkinButton::DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, LPRECT lpRect)
{
	//��������
	int nStringLength=lstrlen(pszString);
	int nXExcursion[8]={1,1,1,0,-1,-1,-1,0};
	int nYExcursion[8]={-1,0,1,1,1,0,-1,-1};

	//�滭�߿�
	pDC->SetTextColor(crFrame);
	CRect rcDraw;
	for (int i=0;i<CountArray(nXExcursion);i++)
	{
		rcDraw.CopyRect(lpRect);
		rcDraw.OffsetRect(nXExcursion[i],nYExcursion[i]);
		pDC->DrawText(pszString,nStringLength,&rcDraw,DT_VCENTER|DT_CENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
	}

	//�滭����
	rcDraw.CopyRect(lpRect);
	pDC->SetTextColor(crText);
	pDC->DrawText(pszString,nStringLength,&rcDraw,DT_VCENTER|DT_CENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

	return;
}

//��������
void CSkinButton::DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos)
{
	//��������
	int nStringLength=lstrlen(pszString);
	int nXExcursion[8]={1,1,1,0,-1,-1,-1,0};
	int nYExcursion[8]={-1,0,1,1,1,0,-1,-1};

	//�滭�߿�
	pDC->SetTextColor(crFrame);
	for (int i=0;i<CountArray(nXExcursion);i++)
	{
		pDC->TextOut(nXPos+nXExcursion[i],nYPos+nYExcursion[i],pszString,nStringLength);
	}

	//�滭����
	pDC->SetTextColor(crText);
	pDC->TextOut(nXPos,nYPos,pszString,nStringLength);

	return;
}

//˫���¼�
void CSkinButton::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	//������Ϣ
	AfxGetMainWnd()->SendMessage(WM_BT_LBUTTONDBLCLK,GetDlgCtrlID(),GetDlgCtrlID());

	CButton::OnLButtonDblClk(nFlags, point);
}

bool CSkinButton::SetTextFont(CFont* pFont, COLORREF crTextColor,COLORREF crTextFrameColor,bool bShowFrame)
{
	ASSERT(pFont);
	if(crTextColor != DEF_TEXT_COLOR)
		m_crTextColor=crTextColor;
	if(crTextFrameColor != DEF_TEXT_FRAME_COLOR)
		m_crTextFrameColor=crTextFrameColor;

	m_bShowTextFrame=bShowFrame;


	pFont->GetLogFont(&m_BtnFont);
	return true;
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CRgnButton::CRgnButton()
{
	m_bHovering=false;
	m_pIMouseEvent=NULL;
	m_crTrans=RGB(255,0,255);
	m_crTextColor=CSkinButton::m_SkinAttribute.m_crTextColor;
}

//��������
CRgnButton::~CRgnButton()
{
}

//���󸽼ӵ����д���
void CRgnButton::PreSubclassWindow()
{
	__super::PreSubclassWindow();
	SetButtonStyle(GetButtonStyle()|BS_OWNERDRAW);
	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);
	SendMessage(WM_SETFONT,(WPARAM)CSkinResourceManager::GetDefaultFont(),TRUE);
	return;
}

//������Ϣ
int CRgnButton::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;
	SetButtonStyle(GetButtonStyle()|BS_OWNERDRAW);
	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);
	SendMessage(WM_SETFONT,(WPARAM)CSkinResourceManager::GetDefaultFont(),TRUE);
	return 0;
}

//����λͼ
bool CRgnButton::LoadRgnImage(HINSTANCE hInstance, UINT uBitmapID, COLORREF crTrans)
{
	m_ImageBack.SetLoadInfo(uBitmapID,hInstance);
	return CreateControlRgn(crTrans);
}

//����λͼ
bool CRgnButton::LoadRgnImage(LPCTSTR pszFileName, COLORREF crTrans)
{
	m_ImageBack.SetLoadInfo(pszFileName);
	return CreateControlRgn(crTrans);
}

//��������
bool CRgnButton::CreateControlRgn(COLORREF crTrans)
{
	CImageHandle ImageHandle(&m_ImageBack);
	if (ImageHandle.IsResourceValid())
	{
		//������ʱͼ
		CSkinImage BufferImage;
		int nButtonWidth=m_ImageBack.GetWidth()/5;
		int nButtomHeight=m_ImageBack.GetHeight();
		BufferImage.Create(nButtonWidth,nButtomHeight,24);
		ImageHandle->BitBlt(BufferImage.GetDC(),0,0,nButtonWidth,nButtomHeight,0,0);
		BufferImage.ReleaseDC();

		//��������
		CRgn RgnControl;
		BufferImage.CreateImageRegion(RgnControl,crTrans);

		//��������
		if (RgnControl.GetSafeHandle()!=NULL)
		{
			//���ñ���
			m_crTrans=crTrans;

			//���ð�ť
			SetWindowRgn(RgnControl,TRUE);
			SetWindowPos(NULL,0,0,nButtonWidth,nButtomHeight,SWP_NOMOVE|SWP_NOACTIVATE);

			//��������
			m_ButtonRgn.DeleteObject();
			m_ButtonRgn.Attach(RgnControl.Detach());

			return true;
		}
	}

	return false;
}

//����ƶ���Ϣ
void CRgnButton::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_bHovering==false)
	{
		//ע����Ϣ
		m_bHovering=true;
		Invalidate(FALSE);
		TRACKMOUSEEVENT TrackMouseEvent;
		TrackMouseEvent.cbSize=sizeof(TrackMouseEvent);
		TrackMouseEvent.dwFlags=TME_LEAVE;
		TrackMouseEvent.hwndTrack=GetSafeHwnd();
		TrackMouseEvent.dwHoverTime=HOVER_DEFAULT;
		_TrackMouseEvent(&TrackMouseEvent);
	}

	__super::OnMouseMove(nFlags, point);
}

//����뿪��Ϣ
LRESULT CRgnButton::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	m_bHovering=false;
	Invalidate(FALSE);
	return 0;
}

//����滭����
void CRgnButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	//�������
	CRect ClientRect;
	GetClientRect(&ClientRect);
	bool bDisable=((lpDrawItemStruct->itemState&ODS_DISABLED)!=0);
	bool bButtonDown=((lpDrawItemStruct->itemState&ODS_SELECTED)!=0);

	//���ر���ͼ
	CImageHandle ImageHandle(&m_ImageBack);
	if (ImageHandle.IsResourceValid())
	{
		//����λͼλ��
		int iPartWidth=ImageHandle->GetWidth()/5,iDrawPos=0;
		if (bDisable) iDrawPos=iPartWidth*4;
		else if (bButtonDown) iDrawPos=iPartWidth;
		else if (m_bHovering) iDrawPos=iPartWidth*3;
		else if (lpDrawItemStruct->itemState&ODS_FOCUS) iDrawPos=iPartWidth*2;

		//�滭����ͼ
		CDC * pDesDC=CDC::FromHandle(lpDrawItemStruct->hDC);
		m_ImageBack.AlphaDrawImage(pDesDC,0,0,iPartWidth,m_ImageBack.GetHeight(),iDrawPos,0,m_crTrans);
	}

	//��ȡ����
	CString strText;
	GetWindowText(strText);

	//�滭����
	ClientRect.top+=1;
	::SetBkMode(lpDrawItemStruct->hDC,TRANSPARENT);
	if (bDisable) ::SetTextColor(lpDrawItemStruct->hDC,GetSysColor(COLOR_GRAYTEXT));
	else ::SetTextColor(lpDrawItemStruct->hDC,m_crTextColor);
	DrawText(lpDrawItemStruct->hDC,strText,strText.GetLength(),ClientRect,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

	return;
}

//Ĭ�ϻص�����
LRESULT CRgnButton::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_pIMouseEvent)
	{
		switch (message)
		{
		case WM_MOUSELEAVE:
			{
				UINT uButtonID=GetWindowLong(GetSafeHwnd(),GWL_ID);
				m_pIMouseEvent->OnEventMouseLeft(uButtonID,wParam,lParam);
				break;
			}
		case WM_MOUSEMOVE:
			{
				UINT uButtonID=GetWindowLong(GetSafeHwnd(),GWL_ID);
				m_pIMouseEvent->OnEventMouseMove(uButtonID,wParam,lParam);
				break;
			}
		case WM_LBUTTONDOWN:
			{
				UINT uButtonID=GetWindowLong(GetSafeHwnd(),GWL_ID);
				m_pIMouseEvent->OnEventLButtonDown(uButtonID,wParam,lParam);
				break;
			}
		case WM_LBUTTONUP:
			{
				UINT uButtonID=GetWindowLong(GetSafeHwnd(),GWL_ID);
				m_pIMouseEvent->OnEventLButtonUp(uButtonID,wParam,lParam);
				break;
			}
		case WM_RBUTTONDOWN:
			{
				UINT uButtonID=GetWindowLong(GetSafeHwnd(),GWL_ID);
				m_pIMouseEvent->OnEventRButtonDown(uButtonID,wParam,lParam);
				break;
			}
		case WM_RBUTTONUP:
			{
				UINT uButtonID=GetWindowLong(GetSafeHwnd(),GWL_ID);
				m_pIMouseEvent->OnEventRButtonUp(uButtonID,wParam,lParam);
				break;
			}
		}
	}
	return __super::DefWindowProc(message, wParam, lParam);
}

//��������
BOOL CRgnButton::OnEraseBkgnd(CDC * pDC)
{
	Invalidate(FALSE);
	UpdateWindow();
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

// CPngButton
BEGIN_MESSAGE_MAP(CPngButton, CButton)
	ON_WM_DRAWITEM()
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
END_MESSAGE_MAP()


//���캯��
CPngButton::CPngButton()
{
	m_bHaveBitmaps = FALSE;
	m_bHaveAltImage = FALSE;

	m_pCurBtn = NULL;

	m_bIsDisabled = FALSE;
	m_bIsToggle = FALSE;

	m_bIsHovering = FALSE;
	m_bIsTracking = FALSE;

	m_nCurType = STD_TYPE;
}

//��������
CPngButton::~CPngButton()
{
}

//���ر�׼ͼ
BOOL CPngButton::LoadStdImage(UINT id, LPCTSTR pType)
{
	BOOL bIsSuccess = m_pStdImage.Load(id, pType);
	if (!bIsSuccess)
	{
		CString strError;
		strError.Format("���棺PNG��ť��ԴID[ %d ]����ʧ�ܣ�",id);
		AfxMessageBox(strError);
	}
	return bIsSuccess;
}

//���ر�׼ͼ
BOOL CPngButton::LoadStdImage(LPCTSTR pFilePath,bool bFixSize)
{
	CT2CW strFileName(pFilePath);
	bool bIsSuccess = m_pStdImage.LoadFromFile(strFileName);
	if (!bIsSuccess)
	{
		CString strError(strFileName);
		strError+=TEXT("  PNG��ť��Դ����ʧ�ܣ�");
		AfxMessageBox(strError);
	}
	if(bFixSize)
	{
		CRect rect;
		GetWindowRect(rect);
		int width = m_pStdImage.m_pBitmap->GetWidth()/5;
		int height = m_pStdImage.m_pBitmap->GetHeight();
		SetWindowPos(NULL,rect.left,rect.top,width,height,NULL);
		if (GetSafeHwnd()) Invalidate(FALSE);
	}
	return bIsSuccess;
}

//���ؽ���ͼ
BOOL CPngButton::LoadAltImage(UINT id, LPCTSTR pType,HINSTANCE hInstance)
{
	/*m_bHaveAltImage = TRUE;
	BOOL bIsSuccess = (m_pAltImage.Load(id, pType,hInstance));
	if (!bIsSuccess)
	{
		CString strError;
		strError.Format("���棺PNG��ť��ԴID[ %d ]����ʧ�ܣ�",id);
		AfxMessageBox(strError);
	}*/
//	return bIsSuccess;

	return 1;
}

//���ؽ���ͼ
BOOL CPngButton::LoadAltImage(LPCTSTR pFilePath,bool bFixSize)
{
	m_bHaveAltImage = TRUE;
	CT2CW strFileName(pFilePath);
	bool bIsSuccess = m_pAltImage.LoadFromFile(strFileName);
	if (!bIsSuccess)
	{
		CString strError(pFilePath);
		strError+=TEXT("  PNG��ť��Դ����ʧ�ܣ�");
		AfxMessageBox(strError);
	}
	if(bFixSize)
	{
		CRect rect;
		GetWindowRect(rect);
		int width = m_pAltImage.m_pBitmap->GetWidth()/5;
		int height = m_pAltImage.m_pBitmap->GetHeight();
		SetWindowPos(NULL,rect.left,rect.top,width,height,NULL);
		if (GetSafeHwnd()) Invalidate(FALSE);
	}
	return bIsSuccess;
}

//�ؼ���ɫ
HBRUSH CPngButton::CtlColor(CDC* pScreenDC, UINT nCtlColor) 
{
	if (!m_bHaveBitmaps)
	{
		//��֤�ж�
		if(!m_pStdImage.m_pBitmap)	return NULL; 
	
		//��������
		CBitmap bmp,	*pOldBitmap;

		//��ȡ����
		CRect rect;
		GetClientRect(rect);

		//�����ڴ�DC
		CMemDC2 pDC(pScreenDC,rect);

		Gdiplus::Graphics graphics(pDC->m_hDC);

		//����DC
		if(m_dcBk.m_hDC==NULL)
		{
			CRect rect1;
			CClientDC clDC(GetParent());

			GetWindowRect(rect1);
			GetParent()->ScreenToClient(rect1);

			m_dcBk.CreateCompatibleDC(&clDC);
			bmp.CreateCompatibleBitmap(&clDC,rect.Width(),rect.Height());
			pOldBitmap = m_dcBk.SelectObject(&bmp);
			m_dcBk.BitBlt(0,0,rect.Width(),rect.Height(),&clDC,rect1.left,rect1.top,SRCCOPY);
			bmp.DeleteObject();
		}

		//��׼��ť
		if (m_dcStd.m_hDC == NULL)
		{
			PaintBk(pDC);

			float width = (float)m_pStdImage.m_pBitmap->GetWidth()/5;
			float height = (float)m_pStdImage.m_pBitmap->GetHeight();
			RectF grect; grect.X=0, grect.Y=0; grect.Width = width; grect.Height = height;

			graphics.DrawImage(m_pStdImage,grect,0,0,width,height,UnitPixel);

			m_dcStd.CreateCompatibleDC(pDC);
			bmp.CreateCompatibleBitmap(pDC,rect.Width(),rect.Height());
			pOldBitmap=m_dcStd.SelectObject(&bmp);
			m_dcStd.BitBlt(0,0,rect.Width(),rect.Height(),pDC,0,0,SRCCOPY);
			bmp.DeleteObject();

			// ����״̬
			if (m_dcStdP.m_hDC == NULL)
			{
				PaintBk(pDC);

				float width = (float)m_pStdImage.m_pBitmap->GetWidth()/5;
				float height = (float)m_pStdImage.m_pBitmap->GetHeight();

				RectF grect; grect.X=0, grect.Y=0; grect.Width = width; grect.Height = height;

				graphics.DrawImage(m_pStdImage,grect,width*2,0,width,height,UnitPixel);

				m_dcStdP.CreateCompatibleDC(pDC);
				bmp.CreateCompatibleBitmap(pDC,rect.Width(),rect.Height());
				pOldBitmap = m_dcStdP.SelectObject(&bmp);
				m_dcStdP.BitBlt(0,0,rect.Width(),rect.Height(),pDC,0,0,SRCCOPY);
				bmp.DeleteObject();
			}

			// ����״̬  
			if(m_dcStdH.m_hDC == NULL)
			{     
				PaintBk(pDC);

				ColorMatrix HotMat = { 1.05f, 0.00f, 0.00f, 0.00f, 0.00f,
					0.00f, 1.05f, 0.00f, 0.00f, 0.00f,
					0.00f, 0.00f, 1.05f, 0.00f, 0.00f,
					0.00f, 0.00f, 0.00f, 1.00f, 0.00f,
					0.05f, 0.05f, 0.05f, 0.00f, 1.00f };

				ImageAttributes ia;
				ia.SetColorMatrix(&HotMat);

				float width = (float)m_pStdImage.m_pBitmap->GetWidth()/5;
				float height = (float)m_pStdImage.m_pBitmap->GetHeight();

				RectF grect; grect.X=0, grect.Y=0; grect.Width = width; grect.Height = height;

				graphics.DrawImage(m_pStdImage, grect, width, 0, width, height, UnitPixel, &ia);

				m_dcStdH.CreateCompatibleDC(pDC);
				bmp.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
				pOldBitmap = m_dcStdH.SelectObject(&bmp);
				m_dcStdH.BitBlt(0, 0, rect.Width(), rect.Height(), pDC, 0, 0, SRCCOPY);
				bmp.DeleteObject();
			}

			// ��Ч״̬   
			if(m_dcGS.m_hDC == NULL)
			{     
				PaintBk(pDC);

				ColorMatrix GrayMat = { 0.30f, 0.30f, 0.30f, 0.00f, 0.00f,
					0.59f, 0.59f, 0.59f, 0.00f, 0.00f,
					0.11f, 0.11f, 0.11f, 0.00f, 0.00f,
					0.00f, 0.00f, 0.00f, 1.00f, 0.00f,
					0.00f, 0.00f, 0.00f, 0.00f, 1.00f };

				ImageAttributes ia;
				ia.SetColorMatrix(&GrayMat);

				float width = (float)m_pStdImage.m_pBitmap->GetWidth()/5;
				float height = (float)m_pStdImage.m_pBitmap->GetHeight();

				RectF grect; grect.X=0, grect.Y=0; grect.Width = width; grect.Height = height;

				graphics.DrawImage(m_pStdImage, grect, 0, 0, width, height, UnitPixel, &ia);
				m_dcGS.CreateCompatibleDC(pDC);
				bmp.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
				pOldBitmap = m_dcGS.SelectObject(&bmp);
				m_dcGS.BitBlt(0, 0, rect.Width(), rect.Height(), pDC, 0, 0, SRCCOPY);
				bmp.DeleteObject();
			}
		}
		// ���水ť
		if( (m_dcAlt.m_hDC == NULL) && m_bHaveAltImage )
		{    
			PaintBk(pDC);

			float width = (float)m_pAltImage.m_pBitmap->GetWidth()/5;
			float height = (float)m_pAltImage.m_pBitmap->GetHeight();
			RectF grect; grect.X=0, grect.Y=0; grect.Width = width; grect.Height = height;

			graphics.DrawImage(m_pAltImage,grect,0,0,width,height,UnitPixel);   

			m_dcAlt.CreateCompatibleDC(pDC);    
			bmp.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());    
			pOldBitmap = m_dcAlt.SelectObject(&bmp);    
			m_dcAlt.BitBlt(0, 0, rect.Width(), rect.Height(), pDC, 0, 0, SRCCOPY);    
			bmp.DeleteObject();

			// ����״̬
			if( (m_dcAltP.m_hDC == NULL) && m_bHaveAltImage )    
			{     
				PaintBk(pDC);

				float width = (float)m_pAltImage.m_pBitmap->GetWidth()/5;
				float height = (float)m_pAltImage.m_pBitmap->GetHeight();

				RectF grect; grect.X=0, grect.Y=0; grect.Width = width; grect.Height = height;
				
				graphics.DrawImage(m_pAltImage,grect,width*2,0,width,height,UnitPixel);

				m_dcAltP.CreateCompatibleDC(pDC);
				bmp.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
				pOldBitmap = m_dcAltP.SelectObject(&bmp);
				m_dcAltP.BitBlt(0, 0, rect.Width(), rect.Height(), pDC, 0, 0, SRCCOPY);
				bmp.DeleteObject();
			}

			// ����״̬
			if(m_dcAltH.m_hDC == NULL)
			{     
				PaintBk(pDC);

				ColorMatrix HotMat = { 1.05f, 0.00f, 0.00f, 0.00f, 0.00f,           
					0.00f, 1.05f, 0.00f, 0.00f, 0.00f,           
					0.00f, 0.00f, 1.05f, 0.00f, 0.00f,           
					0.00f, 0.00f, 0.00f, 1.00f, 0.00f,           
					0.05f, 0.05f, 0.05f, 0.00f, 1.00f };

				ImageAttributes ia;     
				ia.SetColorMatrix(&HotMat);

				float width = (float)m_pAltImage.m_pBitmap->GetWidth()/5; 
				float height = (float)m_pAltImage.m_pBitmap->GetHeight();

				RectF grect; grect.X=0, grect.Y=0; grect.Width = width; grect.Height = height;

				graphics.DrawImage(m_pAltImage, grect, 0, 0, width, height, UnitPixel, &ia);
				m_dcAltH.CreateCompatibleDC(pDC); 
				bmp.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
				pOldBitmap = m_dcAltH.SelectObject(&bmp); 
				m_dcAltH.BitBlt(0, 0, rect.Width(), rect.Height(), pDC, 0, 0, SRCCOPY);
				bmp.DeleteObject();
			}   
		}
		if(m_pCurBtn == NULL)   
		{    
			m_pCurBtn = &m_dcStd;  
		}
		m_bHaveBitmaps = TRUE;
	}
	return NULL;
}

//���Ʊ���
inline void CPngButton::PaintBk(CDC *pDC)
{
	CRect rect;
	GetClientRect(rect);
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &m_dcBk, 0, 0, SRCCOPY);
}

//���ư�ť
inline void CPngButton::PaintBtn(CDC *pDC)
{
	CRect rect;
	GetClientRect(rect);
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), m_pCurBtn, 0, 0, SRCCOPY);
}
//���ð�ť
void CPngButton::EnableButton(BOOL bEnable)
{
	m_bIsDisabled = !bEnable;

	__super::EnableWindow(bEnable);
}

//�ж��Ƿ�ر�
BOOL CPngButton::IsDisabled()
{
	if (!__super::IsWindowEnabled())
	{
		return FALSE;
	}
	return (m_bIsDisabled == FALSE); 
}

//��������
void CPngButton::EnableToggle(BOOL bEnable)
{
	if(!m_bHaveAltImage) return;

	m_bIsToggle = bEnable; 

	// ������水ť������,��ǰ��ť�Զ����н���
	if(bEnable)	m_pCurBtn = &m_dcAlt;
	else		m_pCurBtn = &m_dcStd;

}

//���ð�ť����
void CPngButton::SetButtonType(int type)
{
	m_nCurType = type;

	(type == DIS_TYPE) ? m_bIsDisabled = TRUE : m_bIsDisabled = FALSE;

	Invalidate();
}

//���໯����
void CPngButton::PreSubclassWindow()
{
	//�޸ķ����ʽ
	ModifyStyle(0, BS_OWNERDRAW, SWP_FRAMECHANGED);

	CButton::PreSubclassWindow();
}

//ת����Ϣ
BOOL CPngButton::PreTranslateMessage(MSG* pMsg) 
{
	if (m_bIsDisabled) return CButton::PreTranslateMessage(pMsg);
	//����˫����Ϣ
	if (pMsg->message == WM_LBUTTONDBLCLK)
		pMsg->message = WM_LBUTTONDOWN;

	if (m_pToolTip.m_hWnd!=NULL)
	{
		if (::IsWindow(m_pToolTip.m_hWnd))
		{
			m_pToolTip.RelayEvent(pMsg);		
		}
	}

	return CButton::PreTranslateMessage(pMsg);
}


//��������
BOOL CPngButton::OnEraseBkgnd(CDC* pDC) 
{
	Invalidate(FALSE);
	UpdateWindow();
	return TRUE;
}

//��������
void CPngButton::DrawItem(LPDRAWITEMSTRUCT lpDIS) 
{
	CDC* pDC = CDC::FromHandle(lpDIS->hDC);

	// ��Ч״̬
	if(m_bIsDisabled)
	{
		m_pCurBtn = &m_dcGS;
		PaintBtn(pDC);
		return;
	}

	BOOL bIsPressed = (lpDIS->itemState & ODS_CHECKED);

	// ��������
	if(m_bIsToggle && bIsPressed)
	{
		(m_nCurType == STD_TYPE) ? m_nCurType = ALT_TYPE : m_nCurType = STD_TYPE;
	}

	if(bIsPressed)
	{
		if(m_nCurType == STD_TYPE)
			m_pCurBtn = &m_dcStdP;
		else
			m_pCurBtn = &m_dcAltP;
	}
	else if(m_bIsHovering)
	{

		if(m_nCurType == STD_TYPE)
			m_pCurBtn = &m_dcStdH;
		else
			m_pCurBtn = &m_dcAltH;
	}
	else
	{
		if(m_nCurType == STD_TYPE)
			m_pCurBtn = &m_dcStd;
		else
			m_pCurBtn = &m_dcAlt;
	}

	//���ư�ť
	PaintBtn(pDC);
}

//�������
LRESULT CPngButton::OnMouseHover(WPARAM wparam, LPARAM lparam) 
{
	m_bIsHovering = TRUE;
	Invalidate();
	DeleteToolTip();

	// �ڵ�ǰ��ťλ���ϴ���һ���µ���ʾ�ؼ�
	SetToolTipText(m_tooltext);

	if (m_pToolTip.m_hWnd!=NULL)
	{
		if (::IsWindow(m_pToolTip.m_hWnd))
		{
			//��ʾ��ʾ
			m_pToolTip.Update();
		}
	}


	return 0;
}


//����뿪
LRESULT CPngButton::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	m_bIsTracking = FALSE;
	m_bIsHovering = FALSE;
	Invalidate();
	return 0;
}

//����ƶ�
void CPngButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (!m_bIsTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE|TME_HOVER;
		tme.dwHoverTime = 1;
		m_bIsTracking = _TrackMouseEvent(&tme);
	}
	
	__super::OnMouseMove(nFlags, point);
}

//�������ָ��
BOOL CPngButton::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (!m_bIsDisabled)
	{
		::SetCursor(::LoadCursor(NULL,/*IDC_HAND*/MAKEINTRESOURCE(32649)));
		return TRUE;
	}
	 
	return 	__super::OnSetCursor(pWnd,nHitTest,message);
}

//���ñ���
void CPngButton::SetBkGnd(CDC* pDC)
{
	CRect rect, rectS;
	CBitmap bmp, *pOldBitmap;

	GetClientRect(rect);
	GetWindowRect(rectS);
	GetParent()->ScreenToClient(rectS);

	m_dcBk.DeleteDC();

	m_dcBk.CreateCompatibleDC(pDC);
	bmp.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	pOldBitmap = m_dcBk.SelectObject(&bmp);
	m_dcBk.BitBlt(0, 0, rect.Width(), rect.Height(), pDC, rectS.left, rectS.top, SRCCOPY);
	bmp.DeleteObject();
}


//������ʾ��Ϣ
void CPngButton::SetToolTipText(UINT nId, BOOL bActivate)
{
	//�����ַ�����Դ
	m_tooltext.LoadString(nId);

	//�ж��ַ�����Դ�Ƿ�Ϊ��
	if (m_tooltext.IsEmpty() == FALSE)
	{
		SetToolTipText(m_tooltext, bActivate);
	}

}

//������ʾ��Ϣ
void CPngButton::SetToolTipText(CString spText, BOOL bActivate)
{
	//��֤�������
	if (spText.IsEmpty()) return;

	//��ʼ����ʾ�ؼ�
	InitToolTip();
	m_tooltext = spText;

	//�����ʾ�ؼ�����Ϊ�����һ���µ���ʾ
	if (m_pToolTip.GetToolCount() == 0)
	{
		CRect rectBtn; 
		GetClientRect(rectBtn);
		m_pToolTip.AddTool(this, m_tooltext, rectBtn, 1);
	}

	//ʹ�õ�ǰ��Ϣ
	m_pToolTip.UpdateTipText(m_tooltext, this, 1);
	m_pToolTip.SetDelayTime(2000);
	m_pToolTip.Activate(bActivate);
}

//��ʼ����ʾ�ؼ�
void CPngButton::InitToolTip()
{
	//������ʾ�ؼ�
	m_pToolTip.Create(this);
	m_pToolTip.Activate(TRUE);
} 

//ɾ����ʾ�ؼ�
void CPngButton::DeleteToolTip()
{
	if (m_pToolTip.m_hWnd!=NULL)
	{
		int Count=m_pToolTip.GetToolCount();
		for(int i=0;i<Count;i++) m_pToolTip.DelTool(this,i);
	}
}
//��ť���
int CPngButton::GetWidth()
{
	return m_pStdImage.m_pBitmap->GetWidth()/5;
} 


void CPngButton::UpdateBk()
{
	
//	m_dcStd.m_hDC = NULL;

	m_dcStd.DeleteDC();
	m_dcStdP.DeleteDC();
	m_dcStdH.DeleteDC();
	m_dcGS.DeleteDC();

	
	m_dcStd.m_hDC = NULL;

	m_bHaveBitmaps = FALSE;
	m_bHaveAltImage = FALSE;

	m_pCurBtn = NULL;

	m_bIsDisabled = FALSE;
	m_bIsToggle = FALSE;

	m_bIsHovering = FALSE;
	m_bIsTracking = FALSE;

	m_nCurType = STD_TYPE;
}



// CPngButton
BEGIN_MESSAGE_MAP(CCheckButton, CButton)
	//ON_WM_DRAWITEM()
	ON_WM_CREATE()
	//ON_WM_ERASEBKGND()
	//ON_WM_CTLCOLOR_REFLECT()
	ON_WM_MOUSEMOVE()
	//ON_WM_SETCURSOR()
	ON_WM_LBUTTONUP()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
//	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
END_MESSAGE_MAP()



CCheckButton::CCheckButton()
{
	m_bChecked = false;
	m_crBackColor =DEF_TEXT_FRAME_COLOR;
	m_crTextColor = DEF_TEXT_COLOR;
	m_bHovering=false;

	tagImageLoadInfo LoadInfo=CSkinButton::m_SkinAttribute.LoadInfoCheck;
	if(!m_pngBack.IsNull()) m_pngBack.DestroyImage();
	m_pngBack.LoadImage(LoadInfo.hResourceDLL, LoadInfo.strFileName);
}

CCheckButton::~CCheckButton()
{
	
}

void CCheckButton::DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC* pDc = CDC::FromHandle(lpDrawItemStruct->hDC);
	CRect rcClient=lpDrawItemStruct->rcItem;
	CRect rcClient2;
	GetWindowRect(&rcClient2);
	rcClient2-=rcClient2.TopLeft();

	//�滭����
	pDc->FillSolidRect(rcClient, m_crBackColor);
	//pDc->Draw3dRect(rcClient, m_crBackColor, m_crBackColor);

	//�滭״̬

	if(m_pngBack.IsNull()) return;
// 	CImageHandle imgCheck(&m_ImageBack);
// 	if(m_ImageBack.IsNull()) return;
	UINT iWidth = m_pngBack.GetWidth()/5;
	UINT iHeight = m_pngBack.GetHeight();
	if(m_bChecked)
	{
		if(m_bHovering)
			m_pngBack.DrawImage(pDc, 0,0,iWidth, iHeight, iWidth*1,0);
			//m_ImageBack.AlphaDrawImage(pDc, 0,0,iWidth, iHeight, iWidth*1,0, RGB(255,0,255));
		else
			//m_ImageBack.AlphaDrawImage(pDc, 0,0,iWidth, iHeight, iWidth*0,0, RGB(255,0,255));
			m_pngBack.DrawImage(pDc, 0,0,iWidth, iHeight, iWidth*0,0);
	}else
	{
		if(m_bHovering)
			//m_ImageBack.AlphaDrawImage(pDc, 0,0,iWidth, iHeight, iWidth*3,0, RGB(255,0,255));
			m_pngBack.DrawImage(pDc, 0,0,iWidth, iHeight, iWidth*3,0);
		else
			//m_ImageBack.AlphaDrawImage(pDc, 0,0,iWidth, iHeight, iWidth*2,0, RGB(255,0,255));
			m_pngBack.DrawImage(pDc, 0,0,iWidth, iHeight, iWidth*2,0);
	}

	//�滭�ı�
	CString strText;
	GetWindowText(strText);
	pDc->SetBkColor(m_crBackColor);
	pDc->SetTextColor(m_crTextColor);
	pDc->TextOut(iWidth+1, 2, strText);
}

void CCheckButton::OnMouseMove( UINT nFlags, CPoint point)
{
	if (m_bHovering==false)
	{
		//ע����Ϣ
		m_bHovering=true;
		Invalidate(FALSE);
		TRACKMOUSEEVENT TrackMouseEvent;
		TrackMouseEvent.cbSize=sizeof(TrackMouseEvent);
		TrackMouseEvent.dwFlags=TME_LEAVE;
		TrackMouseEvent.hwndTrack=GetSafeHwnd();
		TrackMouseEvent.dwHoverTime=HOVER_DEFAULT;
		_TrackMouseEvent(&TrackMouseEvent);
	}

	__super::OnMouseMove(nFlags, point);
}

//����뿪
LRESULT CCheckButton::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	m_bHovering = false;
	Invalidate();
	return 0;
}

int CCheckButton::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;
	SetButtonStyle(GetButtonStyle()|BS_OWNERDRAW);

	SendMessage(WM_SETFONT,(WPARAM)CSkinResourceManager::GetDefaultFont(),TRUE);
	return 1;
}

void CCheckButton::PreSubclassWindow()
{
	__super::PreSubclassWindow();

	SetButtonStyle(GetButtonStyle()|BS_OWNERDRAW);
	SendMessage(WM_SETFONT,(WPARAM)CSkinResourceManager::GetDefaultFont(),TRUE);

	//������ͼƬһ����С
	CRect rcClient;
	GetClientRect(&rcClient);
	SetWindowPos(NULL, 0, 0, rcClient.Width(), 17, SWP_NOMOVE);

	return;
}

void CCheckButton::OnLButtonUp( UINT nFlags, CPoint point )
{
	m_bChecked = !m_bChecked;
	Invalidate(FALSE);
	//AfxMessageBox("hello");
	__super::OnLButtonUp(nFlags, point);
}

void CCheckButton::SetCheck( int nCheck )
{
	if(nCheck == BST_CHECKED)
		m_bChecked = true;
	else
		m_bChecked = false;
	__super::SetCheck(nCheck);
	//Invalidate(FALSE);
}

int CCheckButton::GetCheck()
{
	if(m_bChecked==true)
		return BST_CHECKED;
	else
		return BST_UNCHECKED;
}

void CCheckButton::SetTextColor( COLORREF crText, COLORREF crBack)
{
	m_crBackColor = crBack;
	m_crTextColor = crText;
}

bool CCheckButton::SetCheckImage(LPCTSTR lpResName, HINSTANCE hInstance)
{
	//����λͼ
	if (lpResName==NULL) return false;

	if(!m_pngBack.IsNull()) m_pngBack.DestroyImage();
	m_pngBack.LoadImage(hInstance, lpResName);

	//����λ��
	//if (m_bExpand==false) FixButtonSize();
	if (GetSafeHwnd()) Invalidate(FALSE);

	return true;
}	

bool CCheckButton::SetCheckImage( LPCTSTR pszFileName )
{
	//Ч�����
	ASSERT(pszFileName);
	if (pszFileName==NULL) return false;

	//����λͼ
	if(!m_pngBack.IsNull()) m_pngBack.DestroyImage();
	m_pngBack.LoadImage(pszFileName);

	//����λ��
	if (GetSafeHwnd()) Invalidate(FALSE);

	return true;
}


//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CGdiButton, CButton)

BEGIN_MESSAGE_MAP(CGdiButton, CButton)
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	//ON_WM_LBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_MESSAGE(WM_MOUSELEAVE,OnMouseLeave)
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

//���캯��
CGdiButton::CGdiButton()
{
	m_hIcon=NULL;
	m_bExpand=true;
	m_bHovering=false;
	m_bShowTextFrame=false;
	m_crTextColor=RGB(0,0,0);
	m_crTextFrameColor=DEF_TEXT_FRAME_COLOR;
	m_pIMouseEvent=NULL;
	//m_pngBack.LoadImage("c:\\exit.png");
//	m_BtnFont = (CSkinResourceManager::GetDefaultFont());
	//����Ĭ�ϰ�ť
	tagImageLoadInfo LoadInfo=CSkinButton::m_SkinAttribute.LoadInfoPng;
	if(!m_pngBack.IsNull()) m_pngBack.DestroyImage();
	m_pngBack.LoadImage(LoadInfo.hResourceDLL, LoadInfo.strFileName);
}

//��������
CGdiButton::~CGdiButton()
{
}

//���󸽼ӵ����д���
void CGdiButton::PreSubclassWindow()
{
	__super::PreSubclassWindow();
	SetButtonStyle(GetButtonStyle()|BS_OWNERDRAW);
	SendMessage(WM_SETFONT,(WPARAM)CSkinResourceManager::GetDefaultFont(),TRUE);
//	CSkinResourceManager::m_DefaultFont.GetLogFont(&m_BtnFont);
	return;
}

//������Ϣ
int CGdiButton::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;
	SetButtonStyle(GetButtonStyle()|BS_OWNERDRAW);
	SendMessage(WM_SETFONT,(WPARAM)CSkinResourceManager::GetDefaultFont(),TRUE);

//	CSkinResourceManager::m_DefaultFont.GetLogFont(&m_BtnFont);
	return 0;
}

//������ɫ
bool CGdiButton::SetTextColor(COLORREF crTextColor,COLORREF crTextFrameColor,bool bShowFrame)
{
	m_crTextColor=crTextColor;
	m_bShowTextFrame=bShowFrame;
	m_crTextFrameColor=crTextFrameColor;

	if (GetSafeHwnd()) Invalidate(FALSE);
	return true;
}

//����ͼ��
bool CGdiButton::SetButtonIcon(HICON hIcon)
{
	m_hIcon=hIcon;
	if (GetSafeHwnd()) Invalidate(FALSE);
	return true;
}

//����λͼ
bool CGdiButton::SetButtonImage(LPCTSTR pszFileName, bool bExpandImage)
{
	//Ч�����
	ASSERT(pszFileName);
	if (pszFileName==NULL) return false;

	//����λͼ
	m_bExpand=bExpandImage;
	if(!m_pngBack.IsNull()) m_pngBack.DestroyImage();

	m_pngBack.LoadImage(pszFileName);

	//����λ��
	if (m_bExpand==false) FixButtonSize();
	if (GetSafeHwnd()) Invalidate(FALSE);

	return true;
}

//����λͼ
bool CGdiButton::SetButtonImage(LPCTSTR lpResName, HINSTANCE hInstance, bool bExpandImage)
{
	//����λͼ
	if (lpResName==NULL) return false;
	m_bExpand=bExpandImage;

	if(!m_pngBack.IsNull()) m_pngBack.DestroyImage();
	m_pngBack.LoadImage(hInstance, lpResName);

	//����λ��
	if (m_bExpand==false) FixButtonSize();
	if (GetSafeHwnd()) Invalidate(FALSE);

	return true;
}

//Ĭ�ϰ�ť
void CGdiButton::SetDefaultButton(bool bResetIcon, bool bResetColor, bool bResetImage)
{
	//�ָ�Ĭ��
	if (bResetImage) 
	{
		m_bExpand=true;
		if(!m_pngBack.IsNull())
		{
			m_pngBack.DestroyImage();
		}
	}
	if (bResetIcon)	m_hIcon=NULL;
	if (bResetColor)
	{
		m_bShowTextFrame = false;
		m_crTextColor=DEF_TEXT_COLOR;
		m_crTextFrameColor = DEF_TEXT_FRAME_COLOR;
	}
	//ˢ�½���
	if (GetSafeHwnd()) Invalidate(FALSE);

	return;
}

//����λ��
bool CGdiButton::FixButtonSize()
{
	if ((!m_pngBack.IsNull())/*m_ImageBack.IsSetLoadInfo()*/&&GetSafeHwnd())
	{
		SetWindowPos(NULL,0,0,m_pngBack.GetWidth()/5,m_pngBack.GetHeight(),SWP_NOMOVE);
		return true;
	}
	return false;
}

//����ƶ���Ϣ
void CGdiButton::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_bHovering==false)
	{
		//ע����Ϣ
		m_bHovering=true;
		Invalidate(FALSE);
		TRACKMOUSEEVENT TrackMouseEvent;
		TrackMouseEvent.cbSize=sizeof(TrackMouseEvent);
		TrackMouseEvent.dwFlags=TME_LEAVE;
		TrackMouseEvent.hwndTrack=GetSafeHwnd();
		TrackMouseEvent.dwHoverTime=HOVER_DEFAULT;
		_TrackMouseEvent(&TrackMouseEvent);
	}
	__super::OnMouseMove(nFlags, point);
}

//����뿪��Ϣ
LRESULT CGdiButton::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	m_bHovering=false;
	//�˲����ˢ��
// 	CWnd* pF = GetParent();
// 	CRect rcClient;
// 	GetWindowRect(&rcClient);
// 	CRect rcF;
// 	pF->GetWindowRect(&rcF);
// 	CRect rcRef;
// 	rcRef.left = rcClient.left-rcF.left;
// 	rcRef.top = rcClient.top - rcF.top;
// 	rcRef.right= rcRef.left+rcClient.Width();
// 	rcRef.bottom = rcRef.top + rcClient.Height();
// 	pF->InvalidateRect(rcRef, FALSE);
	return 0;
}

//��������
BOOL CGdiButton::OnEraseBkgnd(CDC * pDC)
{
 	Invalidate(FALSE);
 	UpdateWindow();
	return TRUE;
}

//����滭����
void CGdiButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{

	//�������
	CRect ClientRect;
	GetClientRect(&ClientRect);
	CRect ClientRect1;
	GetClientRect(&ClientRect1);
	bool bDisable=((lpDrawItemStruct->itemState&ODS_DISABLED)!=0);
	bool bButtonDown=((lpDrawItemStruct->itemState&ODS_SELECTED)!=0);
	bool bFocus = ((lpDrawItemStruct->itemState & ODS_FOCUS)!=0);
	//���� DC
	HDC hDC=lpDrawItemStruct->hDC;
	//��ȡ����
	CString strText;
	GetWindowText(strText);

	if(m_pngBack.IsNull()) return;

	//���ر���ͼ
// 	CImageHandle ImageHandle;
// 	if (m_ImageBack.IsSetLoadInfo()) ImageHandle.AttachResource(&m_ImageBack);
// 	else ImageHandle.AttachResource(&m_SkinAttribute.m_ImageBack);
	if (!m_pngBack.IsNull())
	{
		//����λͼλ��
		int iPartWidth=m_pngBack.GetWidth()/5;
		int iPartHeight=m_pngBack.GetHeight();
		int iDrawPos=0;

		if (bDisable) 
			iDrawPos = iPartWidth * 4;
		else if (bButtonDown) 
			iDrawPos = iPartWidth;
		else if (m_bHovering) 
			iDrawPos = iPartWidth * 3;
		else if( bFocus )
			iDrawPos = iPartWidth * 2;
		
		//�滭����ͼ
		m_pngBack.DrawImage(CDC::FromHandle(hDC), 0,0,ClientRect.Width(),ClientRect.Height(),iDrawPos,0);
	}
	else
	{
		//�滭Ĭ�Ͻ���
		CDC * pDC=CDC::FromHandle(hDC);
		pDC->FillSolidRect(&ClientRect,GetSysColor(COLOR_BTNFACE));
		if (bButtonDown) pDC->Draw3dRect(&ClientRect,GetSysColor(COLOR_WINDOWFRAME),GetSysColor(COLOR_3DHILIGHT));
		else pDC->Draw3dRect(&ClientRect,GetSysColor(COLOR_3DHILIGHT),GetSysColor(COLOR_WINDOWFRAME));
	}

	//�滭ͼ��
	if (bButtonDown) 
	{
		ClientRect.top+=2;
		ClientRect1.top+=2;
	}
	if (m_hIcon)
	{
		DrawIconEx(hDC,ClientRect.left+6,ClientRect.top+(ClientRect.Height()-16)/2+1,m_hIcon,16,16,0,NULL,DI_NORMAL);
		ClientRect.left+=22;
	}

	//�滭����
	ClientRect.top+=1;
	ClientRect1.top+=2;
	//ClientRect1.left+=2;
	::SetBkMode(hDC,TRANSPARENT);

	// 	CFont InfoFont;
	// 	InfoFont.CreateFont(-24,0,0,0,700,0,0,0,134,3,2,1,1,TEXT("����_GB2312"));
//	CFont Font;
//	Font.CreateFontIndirect(&m_BtnFont);

	//////////////////////////////////////////////////////////////////////////
	CDC* dc = CDC::FromHandle(hDC);
//	CFont* pOldFont = dc->SelectObject (&Font);
	if(!m_bShowTextFrame)
	{
		if (bDisable) ::SetTextColor(hDC,m_crTextColor/*RGB(0,0,0)*/);
		else ::SetTextColor(hDC,m_crTextColor/*RGB(0,0,0)*/);
		DrawText(hDC,strText,strText.GetLength(),ClientRect,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
		//dc->DrawText(strText, 0,0,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
	}
	//��������
	else
	{
		if(bDisable) ::SetTextColor(hDC,m_crTextColor/*RGB(0,0,0)*/);
		else ::SetTextColor(hDC,m_crTextColor/*RGB(0,0,0)*/);
		//DrawText(hDC,strText,strText.GetLength(),ClientRect1,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
		DrawTextString(CDC::FromHandle(hDC), strText, m_crTextColor, m_crTextFrameColor, ClientRect);
	}

	// Select old font in
//	dc->SelectObject(pOldFont);


	return;
}

//��������
void CGdiButton::DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, LPRECT lpRect)
{
	//��������
	int nStringLength=lstrlen(pszString);
	int nXExcursion[8]={1,1,1,0,-1,-1,-1,0};
	int nYExcursion[8]={-1,0,1,1,1,0,-1,-1};

	//�滭�߿�
	pDC->SetTextColor(crFrame);
	CRect rcDraw;
	for (int i=0;i<CountArray(nXExcursion);i++)
	{
		rcDraw.CopyRect(lpRect);
		rcDraw.OffsetRect(nXExcursion[i],nYExcursion[i]);
		pDC->DrawText(pszString,nStringLength,&rcDraw,DT_VCENTER|DT_CENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
	}

	//�滭����
	rcDraw.CopyRect(lpRect);
	pDC->SetTextColor(crText);
	pDC->DrawText(pszString,nStringLength,&rcDraw,DT_VCENTER|DT_CENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

	return;
}

//��������
void CGdiButton::DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos)
{
	//��������
	int nStringLength=lstrlen(pszString);
	int nXExcursion[8]={1,1,1,0,-1,-1,-1,0};
	int nYExcursion[8]={-1,0,1,1,1,0,-1,-1};

	//�滭�߿�
	pDC->SetTextColor(crFrame);
	for (int i=0;i<CountArray(nXExcursion);i++)
	{
		pDC->TextOut(nXPos+nXExcursion[i],nYPos+nYExcursion[i],pszString,nStringLength);
	}

	//�滭����
	pDC->SetTextColor(crText);
	pDC->TextOut(nXPos,nYPos,pszString,nStringLength);

	return;
}

//˫���¼�
void CGdiButton::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	//������Ϣ
	AfxGetMainWnd()->SendMessage(WM_BT_LBUTTONDBLCLK,GetDlgCtrlID(),GetDlgCtrlID());

	CButton::OnLButtonDblClk(nFlags, point);
}

bool CGdiButton::SetTextFont(CFont* pFont, COLORREF crTextColor,COLORREF crTextFrameColor,bool bShowFrame)
{
	ASSERT(pFont);
	if(crTextColor != DEF_TEXT_COLOR)
		m_crTextColor=crTextColor;
	if(crTextFrameColor != DEF_TEXT_FRAME_COLOR)
		m_crTextFrameColor=crTextFrameColor;

	m_bShowTextFrame=bShowFrame;


	pFont->GetLogFont(&m_BtnFont);
	return true;
}

//Ĭ�ϻص�����
LRESULT CGdiButton::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_pIMouseEvent)
	{
		switch (message)
		{
		case WM_MOUSELEAVE:
			{
				UINT uButtonID=GetWindowLong(GetSafeHwnd(),GWL_ID);
				m_pIMouseEvent->OnEventMouseLeft(uButtonID,wParam,lParam);
				break;
			}
		case WM_MOUSEMOVE:
			{
				UINT uButtonID=GetWindowLong(GetSafeHwnd(),GWL_ID);
				m_pIMouseEvent->OnEventMouseMove(uButtonID,wParam,lParam);
				break;
			}
		case WM_LBUTTONDOWN:
			{
				UINT uButtonID=GetWindowLong(GetSafeHwnd(),GWL_ID);
				m_pIMouseEvent->OnEventLButtonDown(uButtonID,wParam,lParam);
				break;
			}
		case WM_LBUTTONUP:
			{
				UINT uButtonID=GetWindowLong(GetSafeHwnd(),GWL_ID);
				m_pIMouseEvent->OnEventLButtonUp(uButtonID,wParam,lParam);
				break;
			}
		case WM_RBUTTONDOWN:
			{
				UINT uButtonID=GetWindowLong(GetSafeHwnd(),GWL_ID);
				m_pIMouseEvent->OnEventRButtonDown(uButtonID,wParam,lParam);
				break;
			}
		case WM_RBUTTONUP:
			{
				UINT uButtonID=GetWindowLong(GetSafeHwnd(),GWL_ID);
				m_pIMouseEvent->OnEventRButtonUp(uButtonID,wParam,lParam);
				break;
			}
		}
	}
	return __super::DefWindowProc(message, wParam, lParam);
}

//���ñ���
void CGdiButton::SetBkGnd(CDC* pDC)
{
	CRect rect, rectS;
	CBitmap bmp, *pOldBitmap;

	GetClientRect(rect);
	GetWindowRect(rectS);
	GetParent()->ScreenToClient(rectS);

	m_dcBk.DeleteDC();

	m_dcBk.CreateCompatibleDC(pDC);
	bmp.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	pOldBitmap = m_dcBk.SelectObject(&bmp);
	m_dcBk.BitBlt(0, 0, rect.Width(), rect.Height(), pDC, rectS.left, rectS.top, SRCCOPY);
	bmp.DeleteObject();
}

inline void CGdiButton::PaintBk(CDC *pDC)
{
	CRect rect;
	GetClientRect(rect);
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &m_dcBk, 0, 0, SRCCOPY);
}


