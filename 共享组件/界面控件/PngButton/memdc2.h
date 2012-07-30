#ifndef MEMORY_DC_HEAD_FILE
#define MEMORY_DC_HEAD_FILE

#pragma once

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//�ڴ�DC��
class CMemDC2 : public CDC 
{
	//��������
private:
	CBitmap						m_bitmap;										// ��Ļ��λͼ
	CBitmap*					m_oldBitmap;									// ���λͼ����
	CDC*						m_pDC;											// ���澭������CDC
	CRect						m_rect;											// �����λ�������
	BOOL						m_bMemDC;										// �Ƿ�Ϊ�ڴ�DC

	//��������
public:
	//���캯������
	CMemDC2(CDC* pDC, CRect rect = CRect(0,0,0,0), BOOL bCopyFirst = FALSE) : CDC(), m_oldBitmap(NULL), m_pDC(pDC)
	{
		ASSERT(m_pDC != NULL);			// ���Բ�����Ϊ��
		
		m_bMemDC = !pDC->IsPrinting();
		
		if (m_bMemDC)
		{
			//����һ���ڴ�DC
			CreateCompatibleDC(pDC);
			if ( rect == CRect(0,0,0,0) )
			{
				pDC->GetClipBox(&m_rect);
			}
			else
			{
				m_rect = rect;
			}
			//��������λͼ
			m_bitmap.CreateCompatibleBitmap(pDC, m_rect.Width(), m_rect.Height());
			//������λͼ����
			m_oldBitmap = SelectObject(&m_bitmap);
			//�ƶ�����ԭ��
			SetWindowOrg(m_rect.left, m_rect.top);
			//���θ���
			if(bCopyFirst)
			{
				this->BitBlt(m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(),m_pDC, m_rect.left, m_rect.top, SRCCOPY);
			}
		} 
		else
		{
			//����һ������ǰDC�����йصĸ���
			m_bPrinting = pDC->m_bPrinting;
			m_hDC = pDC->m_hDC;
			m_hAttribDC = pDC->m_hAttribDC;
		}
	}
	
	//��������
	~CMemDC2()
	{
		if (m_bMemDC) 
		{
			//������Ļ��λͼ����Ļ��
			m_pDC->BitBlt(m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(),	this, m_rect.left, m_rect.top, SRCCOPY);
			//����ԭ����λͼ
			SelectObject(m_oldBitmap);
		} 
		else
		{
			m_hDC = m_hAttribDC = NULL;
		}
	}
	
	// ����ʹ��ָ��
	CMemDC2* operator->() {return this;}
	
	// ����ʹ��ָ��
	operator CMemDC2*() {return this;}
};

#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

