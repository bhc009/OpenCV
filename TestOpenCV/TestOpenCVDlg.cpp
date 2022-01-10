
// TestOpenCVDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "TestOpenCV.h"
#include "TestOpenCVDlg.h"
#include "afxdialogex.h"

//#include "common/BlobResult.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
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


// CTestOpenCVDlg 대화 상자




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


// CTestOpenCVDlg 메시지 처리기

BOOL CTestOpenCVDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
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

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
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


	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CTestOpenCVDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
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

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

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
// Blob 함수 테스트
//
//////////////////////////////////////////////////////////////////////////
void CTestOpenCVDlg::OnBnClickedButtonBlob()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CString str;
	str.Format("위치 : %d, %d", m_pViewer->GetCurrentPosition().x, m_pViewer->GetCurrentPosition().y );
	SetDlgItemText(IDC_STATIC_POSITION, str);

	str.Format("밝기값 : %d", m_pViewer->GetCurrentGray() );
	SetDlgItemText(IDC_STATIC_GRAY, str);


	CDialogEx::OnTimer(nIDEvent);
}


void CTestOpenCVDlg::OnBnClickedButtonBlob2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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
	// 결과 표시
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
