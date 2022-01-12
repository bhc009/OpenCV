#pragma once

#include "opencv2/opencv.hpp"


// CImageViewer ���Դϴ�.

class CImageViewer : public CScrollView
{
	DECLARE_DYNCREATE(CImageViewer)

protected:
	CImageViewer();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CImageViewer();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void OnDraw(CDC* pDC);      // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual void OnInitialUpdate();     // ������ �� ó���Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	void init(void);

	void SetImage(cv::Mat *img);

	// ���� ����
	cv::Mat *m_pImage;
	BITMAPINFO *m_pDib;

	//
	CPoint m_ptCurrent;
	int m_iCurGray;

	CPoint GetCurrentPosition();
	int GetCurrentGray();

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};


