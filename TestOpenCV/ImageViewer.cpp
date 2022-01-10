// ImageViewer.cpp : 구현 파일입니다.
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


// CImageViewer 그리기입니다.

void CImageViewer::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: 이 뷰의 전체 크기를 계산합니다.
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void CImageViewer::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
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


// CImageViewer 진단입니다.

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


// CImageViewer 메시지 처리기입니다.


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

	// 컬러 세팅
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
// 영상 설정
//
//////////////////////////////////////////////////////////////////////////
void CImageViewer::SetImage(cv::Mat *img)
{
	// Bitmap 정보 생성
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

	// 이미지 연결
	m_pImage = img;

	// 화면 크기 변경
	SetScrollSizes(MM_TEXT, CSize(img->cols, img->rows) );


	cv::circle(*img, cv::Point(20,20), 20, CV_RGB(255,0,0));



	// 화면 갱신
	Invalidate();
}


void CImageViewer::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	// Scroll bar 위치
	CPoint ptScroll = GetScrollPosition();

	// 마우스 위치
	m_ptCurrent = point + ptScroll;

	// 밝기값
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
// 마우스 위치 
//
//////////////////////////////////////////////////////////////////////////
CPoint CImageViewer::GetCurrentPosition()
{
	return m_ptCurrent;
}


//////////////////////////////////////////////////////////////////////////
//
// 현재 밝기값 
//
//////////////////////////////////////////////////////////////////////////
int CImageViewer::GetCurrentGray()
{
	return m_iCurGray;
}
