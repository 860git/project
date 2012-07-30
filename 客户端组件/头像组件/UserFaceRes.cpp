#include "StdAfx.h"
#include "Resource.h"
#include "UserFaceRes.h"

//////////////////////////////////////////////////////////////////////////

//�궨��
#define COLOR_TRANS						RGB(255,0,255)				//͸����ɫ

//////////////////////////////////////////////////////////////////////////

//���캯��
CUserFaceRes::CUserFaceRes(void)
{
	//������Դ
	HINSTANCE hInstancle=GetModuleHandle(USER_FACE_DLL_NAME);
	m_NormalFace.LoadFromResource(hInstancle,IDB_FACE_NORMAL);

	//���ñ���
	m_wCount=m_NormalFace.GetWidth()/NOR_FACE_WIDTH;
	m_NormalSize.SetSize(NOR_FACE_WIDTH,NOR_FACE_HEIGHT);
	m_szDirWork[0]=0;

	//���ù���Ŀ¼
	GetModuleFileName(GetModuleHandle(USER_FACE_DLL_NAME),m_szDirWork,sizeof(m_szDirWork));
	int nModuleLen=lstrlen(m_szDirWork);
	int nProcessLen=lstrlen(USER_FACE_DLL_NAME);
	m_szDirWork[nModuleLen-nProcessLen]=0;

 	m_FaceBoby.LoadFromResource(hInstancle,IDB_FACE_BOBY);
 	m_FaceGirl.LoadFromResource(hInstancle,IDB_FACE_GIRL);
//	m_FaceBoby.SetLoadInfo(IDB_FACE_BOBY,hInstancle);
//	m_FaceGirl.SetLoadInfo(IDB_FACE_GIRL, hInstancle);

	return;
}

//��������
CUserFaceRes::~CUserFaceRes(void)
{
}

//�ӿڲ�ѯ
void * __cdecl CUserFaceRes::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IUserFaceRes,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IUserFaceRes,Guid,dwQueryVer);
	return NULL;
}

//����б�
WORD __cdecl CUserFaceRes::FillImageList(CImageList & ImageList)
{
	//������Դ
	AfxSetResourceHandle(GetModuleHandle(USER_FACE_DLL_NAME));

	//����ͷ��
	CBitmap Image;
	Image.LoadBitmap(IDB_FACE_NORMAL);
	ImageList.Create(NOR_FACE_WIDTH,NOR_FACE_HEIGHT,ILC_COLOR16|ILC_MASK,0,0);
	ImageList.Add(&Image,RGB(255,0,255));
    
	//������Դ
	AfxSetResourceHandle(GetModuleHandle(NULL));
    
	return m_wCount;
}

//�滭ͷ��
void __cdecl CUserFaceRes::DrawNormalFace(CDC * pDC, int nXPos, int nYPos, WORD wFaceID, DWORD dwUserID, DWORD dwCustomFace,DWORD X,DWORD Y)
{
	//��������
	if (wFaceID>=m_wCount) wFaceID=0;

	//�滭ͷ��
	CDC DCFace;
	DCFace.CreateCompatibleDC(NULL);
	DCFace.SelectObject(m_NormalFace);

	//////////////////////////////////////////////////////////////////////////
	CDC DCFaceGril;
	DCFaceGril.CreateCompatibleDC(NULL);
	DCFaceGril.SelectObject(m_FaceGirl);

	CDC DCFaceBoby;
	DCFaceBoby.CreateCompatibleDC(NULL);
	DCFaceBoby.SelectObject(m_FaceBoby);
	//////////////////////////////////////////////////////////////////////////

	bool bDrawFace = false;
	if(dwCustomFace!=0)
	{
		//ͷ������
		CString strDirName;
		strDirName.Format(TEXT("%s\\CustomFace"),m_szDirWork);
		CString strFileName;
		strFileName.Format(TEXT("\\%ld_%d.bmp"), dwUserID, dwCustomFace);

		//��ȡ�ļ�
		CImage FaceImage;
		HRESULT hResult = FaceImage.Load(strDirName + strFileName);
		if (SUCCEEDED(hResult))
		{
			//����ģʽ
			int nOldBltMode = pDC->SetStretchBltMode(HALFTONE);

			//���ô�С
			CRect rcFace;
			rcFace.left = nXPos;
			rcFace.top = nYPos;
			rcFace.right = rcFace.left + m_NormalSize.cx;
			rcFace.bottom = rcFace.top + m_NormalSize.cy;

			//�滭ͼƬ
			FaceImage.StretchBlt(pDC->GetSafeHdc(), rcFace);

			//ɾ����Դ
			pDC->SetStretchBltMode(nOldBltMode);
			FaceImage.Destroy();

			bDrawFace=true;
		}
	}

	//����ͷ��
	if(!bDrawFace) 
	{

		//m_NormalFace.StretchBlt(pDC->GetSafeHdc(), nXPos, nYPos, 32, 32, 0,0,0,0);
		AlphaDrawImage(pDC,nXPos,nYPos,m_NormalSize.cx,m_NormalSize.cy,&DCFace,wFaceID*m_NormalSize.cx,0,X, Y);

// 		//���ô�С
// 		CRect rcFace;
// 		rcFace.left = nXPos;
// 		rcFace.top = nYPos;
// 		rcFace.right = rcFace.left + X;
// 		rcFace.bottom = rcFace.top + Y;
// 
// 		if(wFaceID==0)
//  		{
// 			//ͷ������
// 			CSize szFace = CSize(X,Y);
// 			AlphaDrawImage(pDC,nXPos,nYPos,szFace.cx,szFace.cy,&DCFaceGril,0,0);
// 
//  		}else
// 		{
// 		  	//ͷ������
// 			CSize szFace = CSize(X,Y);
// 			AlphaDrawImage(pDC,nXPos,nYPos,szFace.cx,szFace.cy,&DCFaceBoby,0,0);
// 		//	m_FaceBoby.AlphaDrawImage(pDC, nXPos, nYPos, RGB(255,0,255));
// 		}
		
	}

	return;
}

//�滭ͷ��
void __cdecl CUserFaceRes::DrawCustomBigFace(CDC * pDC, int nXPos, int nYPos, DWORD dwUserID, DWORD dwCustomFace,UINT nWidth,UINT nHeight)
{
	ASSERT(dwUserID!=0 && dwCustomFace!=0);
	if(!(dwUserID!=0 && dwCustomFace!=0))return;

	//ͷ������
	CString strDirName;
	strDirName.Format(TEXT("%s\\CustomFace"),m_szDirWork);
	CString strFileName;
	strFileName.Format(TEXT("\\%ld_%d.bmp"), dwUserID, dwCustomFace);

	//��ȡ�ļ�
	CImage FaceImage;
	HRESULT hResult = FaceImage.Load(strDirName + strFileName);
	if (SUCCEEDED(hResult))
	{
		//����ģʽ
		int nOldBltMode = pDC->SetStretchBltMode(HALFTONE);

		//���ô�С
		CRect rcFace;
		rcFace.left = nXPos;
		rcFace.top = nYPos;
		rcFace.right = rcFace.left + nWidth;
		rcFace.bottom = rcFace.top + nHeight;

		//�滭ͼƬ
		FaceImage.StretchBlt(pDC->GetSafeHdc(), rcFace);

		//ɾ����Դ
		pDC->SetStretchBltMode(nOldBltMode);
		FaceImage.Destroy();
	}

	return;
}

//͸���滭
bool CUserFaceRes::AlphaDrawImage(CDC * pDestDC, int xDest, int yDest, int cxDest, int cyDest, CDC * pScrDC, int xSrc, int ySrc, int X, int Y)
{
	//Ч�����
	ASSERT(pScrDC!=NULL);
	ASSERT(pDestDC!=NULL);

	//���� DC
	pScrDC->SetBkColor(COLOR_TRANS);
	pScrDC->SetBkMode(TRANSPARENT);

	//��������ͼ
	CDC DCMask;
	CBitmap BMPMask;
	BMPMask.CreateBitmap(cxDest,cyDest,1,1,NULL);
	DCMask.CreateCompatibleDC(pDestDC);
	DCMask.SelectObject(&BMPMask);
	DCMask.BitBlt(0,0,cxDest,cyDest,pScrDC,xSrc,ySrc,SRCCOPY);

	//�滭Ŀ��ͼ
	CDC DCDest;
	CBitmap BMPDest;
	DCDest.CreateCompatibleDC(pDestDC);
	BMPDest.CreateCompatibleBitmap(pDestDC,cxDest,cyDest);
	DCDest.SelectObject(&BMPDest);
	DCDest.BitBlt(0,0,cxDest,cyDest,pDestDC,xDest,yDest,SRCCOPY);
	DCDest.BitBlt(0,0,cxDest,cyDest,pScrDC,xSrc,ySrc,SRCINVERT);
	DCDest.BitBlt(0,0,cxDest,cyDest,&DCMask,0,0,SRCAND);
	DCDest.BitBlt(0,0,cxDest,cyDest,pScrDC,xSrc,ySrc,SRCINVERT);
	//pDestDC->BitBlt(xDest,yDest,cxDest,cyDest,&DCDest,0,0,SRCCOPY);
	pDestDC->StretchBlt(xDest,yDest,X/*cxDest*/,Y/*cyDest*/,&DCDest,0,0,cxDest,cyDest,SRCCOPY);

	//������Դ
	DCMask.DeleteDC();
	DCDest.DeleteDC();
	BMPMask.DeleteObject();
	BMPDest.DeleteObject();
	
	return true;
}

//////////////////////////////////////////////////////////////////////////

//����������
extern "C" __declspec(dllexport) void * __cdecl CreateUserFaceRes(const GUID & Guid, DWORD dwInterfaceVer)
{
	//��������
	CUserFaceRes * pUserFaceRes=NULL;
	try
	{
		pUserFaceRes=new CUserFaceRes();
		if (pUserFaceRes==NULL) throw TEXT("����ʧ��");
		void * pObject=pUserFaceRes->QueryInterface(Guid,dwInterfaceVer);
		if (pObject==NULL) throw TEXT("�ӿڲ�ѯʧ��");
		return pObject;
	}
	catch (...) {}

	//�������
	SafeDelete(pUserFaceRes);
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
