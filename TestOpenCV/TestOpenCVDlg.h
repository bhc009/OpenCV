
// TestOpenCVDlg.h : 헤더 파일
//

#pragma once

#include "opencv2/opencv.hpp"

#include "ImageViewer.h"


// CTestOpenCVDlg 대화 상자
class CTestOpenCVDlg : public CDialogEx
{
// 생성입니다.
public:
	CTestOpenCVDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TESTOPENCV_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()


public:
	cv::Mat m_srcImage;
	cv::Mat m_rstImage;

	CImageViewer *m_pViewer;

	//
	void Init(void);

	//
	void blob();
	void convex_hull();
	void contour();

	//
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButtonLoadImage();
	afx_msg void OnBnClickedButtonProcessing();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CComboBox m_ctrlSelectFunction;
	int GetFunction();
	afx_msg void OnBnClickedButtonDo();
};
