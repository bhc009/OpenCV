
// TestOpenCVDlg.h : ��� ����
//

#pragma once

#include "opencv2/opencv.hpp"

#include "ImageViewer.h"


// CTestOpenCVDlg ��ȭ ����
class CTestOpenCVDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CTestOpenCVDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_TESTOPENCV_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
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
