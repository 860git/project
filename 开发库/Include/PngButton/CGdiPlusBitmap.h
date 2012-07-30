#ifndef GDIPLUS_BITMAP_HEAD_FILE
#define GDIPLUS_BITMAP_HEAD_FILE


#pragma once

//#include "Stdafx.h"
//////////////////////////////////////////////////////////////////////////

//λͼ��
class CGdiPlusBitmap
{
	//��������
public:
	Gdiplus::Bitmap*							m_pBitmap;					//λͼ����

	//��������
public:
	//���캯��
	CGdiPlusBitmap()
	{ 
		m_pBitmap = NULL; 
	}
	//���캯��
	CGdiPlusBitmap(LPCWSTR pFile)
	{ 
		m_pBitmap = NULL; 
		LoadFromFile(pFile); 
	}
	//��������
	virtual ~CGdiPlusBitmap()
	{ 
		Empty(); 
	}
	
	//���ܺ���
public:
	//��պ���
	void Empty()
	{ 
		delete m_pBitmap; 
		m_pBitmap = NULL; 
	}
	//����λͼ
	bool LoadFromFile(LPCWSTR pFile)
	{
		Empty();
		m_pBitmap = Gdiplus::Bitmap::FromFile(pFile);
		return m_pBitmap->GetLastStatus() == Gdiplus::Ok;
	}
	//���ز�������
	operator Gdiplus::Bitmap*() const
	{ 
		return m_pBitmap; 
	}
};

//λͼ��Դ��
class CGdiPlusBitmapResource : public CGdiPlusBitmap
{
	//��������
protected:
	HGLOBAL										m_hBuffer;					//����

	//��������
public:
	//���캯��
	CGdiPlusBitmapResource()					
	{ 
		m_hBuffer = NULL; 
	}
	//���캯��
	CGdiPlusBitmapResource(LPCTSTR pName, LPCTSTR pType = RT_RCDATA, HMODULE hInst = NULL)
	{ 
		m_hBuffer = NULL; 
		Load(pName, pType, hInst); 
	}
	//���캯��
	CGdiPlusBitmapResource(UINT id, LPCTSTR pType = RT_RCDATA, HMODULE hInst = NULL)
	{ 
		m_hBuffer = NULL; 
		Load(id, pType, hInst); 
	}
	//���캯��
	CGdiPlusBitmapResource(UINT id, UINT type, HMODULE hInst = NULL)
	{ 
		m_hBuffer = NULL; 
		Load(id, type, hInst); 
	}
	//��������
	virtual ~CGdiPlusBitmapResource()
	{
		Empty(); 
	}

	//���ܺ���
public:
	//��պ���
	void Empty();
	//������Դ
	bool Load(LPCTSTR pName, LPCTSTR pType = RT_RCDATA, HMODULE hInst = NULL);
	//������Դ
	bool Load(UINT id, LPCTSTR pType = RT_RCDATA, HMODULE hInst = NULL)
	{ 
		return Load(MAKEINTRESOURCE(id), pType, hInst); 
	}
	//������Դ
	bool Load(UINT id, UINT type, HMODULE hInst = NULL)
	{ 
		return Load(MAKEINTRESOURCE(id), MAKEINTRESOURCE(type), hInst); 
	}
};

//��������
inline void CGdiPlusBitmapResource::Empty()
{
	CGdiPlusBitmap::Empty();
	if (m_hBuffer)
	{
		::GlobalUnlock(m_hBuffer);
		::GlobalFree(m_hBuffer);
		m_hBuffer = NULL;
	} 
}

//��������
inline bool CGdiPlusBitmapResource::Load(LPCTSTR pName, LPCTSTR pType, HMODULE hInst)
{
	Empty();

	HRSRC hResource = ::FindResource(hInst, pName, pType);
	if (!hResource)		return false;
	
	DWORD imageSize = ::SizeofResource(hInst, hResource);
	if (!imageSize)		return false;

	const void* pResourceData = ::LockResource(::LoadResource(hInst, hResource));
	if (!pResourceData)		return false;

	m_hBuffer  = ::GlobalAlloc(GMEM_MOVEABLE, imageSize);
	if (m_hBuffer)
	{
		void* pBuffer = ::GlobalLock(m_hBuffer);
		if (pBuffer)
		{
			CopyMemory(pBuffer, pResourceData, imageSize);

			IStream* pStream = NULL;
			if (::CreateStreamOnHGlobal(m_hBuffer, FALSE, &pStream) == S_OK)
			{
				m_pBitmap = Gdiplus::Bitmap::FromStream(pStream);
				pStream->Release();
				if (m_pBitmap)
				{ 
					if (m_pBitmap->GetLastStatus() == Gdiplus::Ok)	return true;

					delete m_pBitmap;
					m_pBitmap = NULL;
				}
			}
			::GlobalUnlock(m_hBuffer);
		}
		::GlobalFree(m_hBuffer);
		m_hBuffer = NULL;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
#endif