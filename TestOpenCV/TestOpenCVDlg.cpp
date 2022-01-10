
// TestOpenCVDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "TestOpenCV.h"
#include "TestOpenCVDlg.h"
#include "afxdialogex.h"

//#include "common/BlobResult.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTestOpenCVDlg ��ȭ ����




CTestOpenCVDlg::CTestOpenCVDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTestOpenCVDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	Init();
}

void CTestOpenCVDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTestOpenCVDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_LOAD_IMAGE, &CTestOpenCVDlg::OnBnClickedButtonLoadImage)
	ON_BN_CLICKED(IDC_BUTTON_PROCESSING, &CTestOpenCVDlg::OnBnClickedButtonProcessing)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_BLOB, &CTestOpenCVDlg::OnBnClickedButtonBlob)
	ON_BN_CLICKED(IDC_BUTTON_BLOB2, &CTestOpenCVDlg::OnBnClickedButtonBlob2)
END_MESSAGE_MAP()


// CTestOpenCVDlg �޽��� ó����

BOOL CTestOpenCVDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	{
		CRect rtVeiwRegion;
		GetDlgItem(IDC_STATIC_VIEWER)->GetWindowRect(&rtVeiwRegion);
		ScreenToClient(rtVeiwRegion);

		if (!m_pViewer->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
			rtVeiwRegion, this, AFX_IDW_PANE_FIRST, NULL))
		{
			TRACE0("Failed to create view window\n");
			return -1;
		}
	}


	//
	SetTimer( 1, 50, NULL );


	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CTestOpenCVDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CTestOpenCVDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CTestOpenCVDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTestOpenCVDlg::Init(void)
{
	// viewer
	CRuntimeClass  *pObject;
	pObject = RUNTIME_CLASS( CImageViewer );
	m_pViewer = (CImageViewer*) pObject->CreateObject();
}


void CTestOpenCVDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}


void CTestOpenCVDlg::OnBnClickedButtonLoadImage()
{
	CFileDialog dlg(	TRUE, 
						NULL, 
						NULL, 
						OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
						"bmp files(*.bmp)|*.bmp|", 
						this ); 


	if ( dlg.DoModal() == IDOK ) 
	{ 
		//
		CString strFile;
		strFile = dlg.GetFolderPath() + "\\" + dlg.GetFileName();

		//
// 		m_srcImage = cv::imread((LPSTR)(LPCTSTR)strFile, CV_8UC1 );
		m_srcImage = cv::imread((LPSTR)(LPCTSTR)strFile, cv::IMREAD_UNCHANGED );

		//
		m_pViewer->SetImage(&m_srcImage);
	} 
}


void CTestOpenCVDlg::OnBnClickedButtonProcessing()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	// Canny
	cv::Mat edgeImage;
	cv::Canny(m_srcImage, edgeImage, 60, 150 );
	m_srcImage.copyTo(m_rstImage, edgeImage);


	// gaussian
// 	cv::GaussianBlur(m_srcImage, m_rstImage, cv::Size(5,5), 0, 0 );
	

	// display
	m_pViewer->SetImage(&m_rstImage);
	
}


//////////////////////////////////////////////////////////////////////////
//
// Blob �Լ� �׽�Ʈ
//
//////////////////////////////////////////////////////////////////////////
void CTestOpenCVDlg::OnBnClickedButtonBlob()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
/*	using namespace cv;


	cv::Mat thresholdImage;
	cv::threshold(m_srcImage, thresholdImage, 50, 255, cv::THRESH_BINARY );
	cv::imwrite("d:\\test\\1.bmp", thresholdImage);

	// Setup SimpleBlobDetector parameters.
	SimpleBlobDetector::Params params;

	// Change thresholds
	params.minThreshold = 1;
	params.maxThreshold = 200;
	params.thresholdStep = 1;

	params.filterByArea = true;
	params.minArea = 1;
	params.maxArea = 9999999999;

	params.filterByCircularity = false;
	params.filterByInertia = false;
	params.filterByConvexity = false;

	params.filterByColor = false;
	params.blobColor = 0;

	SimpleBlobDetector detector(params);

	//
// 	SimpleBlobDetector detector;


	// Detect
	std::vector<KeyPoint> keyPoints;
	detector.detect( thresholdImage, keyPoints );


	// Draw result
	Mat im_with_keypoints;
	drawKeypoints(	m_srcImage,
					keyPoints,
					im_with_keypoints,
					Scalar(255,0,0),
					DrawMatchesFlags::DRAW_RICH_KEYPOINTS );


	// display result
	imshow("key points", im_with_keypoints);*/
}


void CTestOpenCVDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CString str;
	str.Format("��ġ : %d, %d", m_pViewer->GetCurrentPosition().x, m_pViewer->GetCurrentPosition().y );
	SetDlgItemText(IDC_STATIC_POSITION, str);

	str.Format("��Ⱚ : %d", m_pViewer->GetCurrentGray() );
	SetDlgItemText(IDC_STATIC_GRAY, str);


	CDialogEx::OnTimer(nIDEvent);
}


void CTestOpenCVDlg::OnBnClickedButtonBlob2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
/*	CBlobResult blobs;
	CBlob *pCurrentBlob;


	//
	// 
	//
	IplImage *iplImage = cvCreateImage(cvSize(m_srcImage.cols, m_srcImage.rows) , IPL_DEPTH_8U, 1);

	memcpy( iplImage->imageData, m_srcImage.data, m_srcImage.cols*m_srcImage.rows);


	//
	//
	//
	blobs = CBlobResult(	iplImage, 
							NULL, 
							255 );	// Background color


	//
	// ��� ǥ��
	//
	cv::Mat displayImage;
	m_srcImage.copyTo(displayImage);

	int num_blobs = blobs.GetNumBlobs(); 
	for ( int i = 0; i < num_blobs; i++ )
	{
		pCurrentBlob = blobs.GetBlob( i );

		int area = pCurrentBlob->Area();
		int MinX = pCurrentBlob->MinX();
		int MaxX = pCurrentBlob->MaxX();

		if( area>100 )
		{
			cv::circle(	displayImage, 
						cv::Point( (pCurrentBlob->MaxX()+pCurrentBlob->MinX())/2, (pCurrentBlob->MaxY()+pCurrentBlob->MinY())/2 ) , 
						150, 
						cv::Scalar(0,0,0) );
		}
	}

	cv::imshow("key points", displayImage);


	//
	//
	//
	cvReleaseImage(&iplImage);*/
}
