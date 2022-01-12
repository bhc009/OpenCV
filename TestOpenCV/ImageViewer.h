#pragma once

#include "opencv2/opencv.hpp"


// CImageViewer 뷰입니다.

class CImageViewer : public CScrollView
{
	DECLARE_DYNCREATE(CImageViewer)

protected:
	CImageViewer();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CImageViewer();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void OnDraw(CDC* pDC);      // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual void OnInitialUpdate();     // 생성된 후 처음입니다.

	DECLARE_MESSAGE_MAP()
public:
	void init(void);

	void SetImage(cv::Mat *img);

	// 영상 정보
	cv::Mat *m_pImage;
	BITMAPINFO *m_pDib;

	//
	CPoint m_ptCurrent;
	int m_iCurGray;

	CPoint GetCurrentPosition();
	int GetCurrentGray();

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};


