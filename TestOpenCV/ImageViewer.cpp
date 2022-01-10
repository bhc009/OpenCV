// ImageViewer.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "TestOpenCV.h"
#include "ImageViewer.h"


// CImageViewer

IMPLEMENT_DYNCREATE(CImageViewer, CScrollView)

CImageViewer::CImageViewer()
{
	init();
}

CImageViewer::~CImageViewer()
{
	if( m_pDib )
	{
		delete[] m_pDib;
	}
	m_pDib = NULL;
}


BEGIN_MESSAGE_MAP(CImageViewer, CScrollView)
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CImageViewer �׸����Դϴ�.

void CImageViewer::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: �� ���� ��ü ũ�⸦ ����մϴ�.
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void CImageViewer::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.
	if( m_pImage )
	{
		StretchDIBits(	pDC->m_hDC,
						0, 0, m_pImage->cols, m_pImage->rows,
						0, 0, m_pImage->cols, m_pImage->rows,
						m_pImage->data,
						m_pDib,
						DIB_RGB_COLORS,
						SRCCOPY);
	}
}


// CImageViewer �����Դϴ�.

#ifdef _DEBUG
void CImageViewer::AssertValid() const
{
	CScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void CImageViewer::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif
#endif //_DEBUG


// CImageViewer �޽��� ó�����Դϴ�.


void CImageViewer::init(void)
{
	//
	SetScrollSizes(MM_TEXT, CSize(100,100) );


	//
	m_pImage = NULL;


	//
	m_pDib = (BITMAPINFO *)malloc(sizeof(BITMAPINFOHEADER) + (sizeof(RGBQUAD)*256));

	m_pDib->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_pDib->bmiHeader.biWidth = 100;
	m_pDib->bmiHeader.biHeight = -100;
	m_pDib->bmiHeader.biBitCount = 24;
	m_pDib->bmiHeader.biPlanes = 1;
	m_pDib->bmiHeader.biCompression = BI_RGB;
	m_pDib->bmiHeader.biSizeImage = 0;
	m_pDib->bmiHeader.biXPelsPerMeter = 0;
	m_pDib->bmiHeader.biYPelsPerMeter = 0;
	m_pDib->bmiHeader.biClrUsed = 0;
	m_pDib->bmiHeader.biClrImportant = 0;

	// �÷� ����
	for ( int i=0 ; i<256 ; ++i )
	{
		m_pDib->bmiColors[i].rgbRed = i;
		m_pDib->bmiColors[i].rgbGreen = i;
		m_pDib->bmiColors[i].rgbBlue = i;
		m_pDib->bmiColors[i].rgbReserved = 0;
	} 

	//
	m_ptCurrent = CPoint(0,0);
	m_iCurGray = 0;
}


//////////////////////////////////////////////////////////////////////////
//
// ���� ����
//
//////////////////////////////////////////////////////////////////////////
void CImageViewer::SetImage(cv::Mat *img)
{
	// Bitmap ���� ����
	m_pDib->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_pDib->bmiHeader.biWidth = img->cols;
	m_pDib->bmiHeader.biHeight = -img->rows;
	m_pDib->bmiHeader.biBitCount = 8*img->channels();	// Gray
	m_pDib->bmiHeader.biPlanes = 1;
	m_pDib->bmiHeader.biCompression = BI_RGB;
	m_pDib->bmiHeader.biSizeImage = 0;
	m_pDib->bmiHeader.biXPelsPerMeter = 0;
	m_pDib->bmiHeader.biYPelsPerMeter = 0;
	m_pDib->bmiHeader.biClrUsed = 0;
	m_pDib->bmiHeader.biClrImportant = 0;

	// �̹��� ����
	m_pImage = img;

	// ȭ�� ũ�� ����
	SetScrollSizes(MM_TEXT, CSize(img->cols, img->rows) );


	cv::circle(*img, cv::Point(20,20), 20, CV_RGB(255,0,0));



	// ȭ�� ����
	Invalidate();
}


void CImageViewer::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	// Scroll bar ��ġ
	CPoint ptScroll = GetScrollPosition();

	// ���콺 ��ġ
	m_ptCurrent = point + ptScroll;

	// ��Ⱚ
	if( m_pImage )
	{
		if( m_ptCurrent.x > -1 && 
			m_ptCurrent.x < m_pImage->cols && 
			m_ptCurrent.y > -1 && 
			m_ptCurrent.y < m_pImage->rows )
		{
			m_iCurGray = m_pImage->at<uchar>(m_ptCurrent.y, m_ptCurrent.x);
		}
	}

	CScrollView::OnMouseMove(nFlags, point);
}


//////////////////////////////////////////////////////////////////////////
//
// ���콺 ��ġ 
//
//////////////////////////////////////////////////////////////////////////
CPoint CImageViewer::GetCurrentPosition()
{
	return m_ptCurrent;
}


//////////////////////////////////////////////////////////////////////////
//
// ���� ��Ⱚ 
//
//////////////////////////////////////////////////////////////////////////
int CImageViewer::GetCurrentGray()
{
	return m_iCurGray;
}
