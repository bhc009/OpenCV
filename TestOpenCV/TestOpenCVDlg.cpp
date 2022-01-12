
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

using namespace std;


#define FUNCTION_NONE			0
#define FUNCTION_BLOB			1
#define FUNCTION_CONVEX_HULL	2
#define FUNCTION_CONTOUR		3


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
	DDX_Control(pDX, IDC_COMBO1, m_ctrlSelectFunction);
}

BEGIN_MESSAGE_MAP(CTestOpenCVDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_LOAD_IMAGE, &CTestOpenCVDlg::OnBnClickedButtonLoadImage)
	ON_BN_CLICKED(IDC_BUTTON_PROCESSING, &CTestOpenCVDlg::OnBnClickedButtonProcessing)
	ON_WM_TIMER()
//	ON_BN_CLICKED(IDC_BUTTON_BLOB, &CTestOpenCVDlg::OnBnClickedButtonBlob)
//	ON_BN_CLICKED(IDC_BUTTON_BLOB2, &CTestOpenCVDlg::OnBnClickedButtonBlob2)
ON_BN_CLICKED(IDC_BUTTON_DO, &CTestOpenCVDlg::OnBnClickedButtonDo)
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

	m_ctrlSelectFunction.InsertString(0, "blob");
	m_ctrlSelectFunction.InsertString(1, "convex hull");
	m_ctrlSelectFunction.InsertString(2, "get contour");

	m_ctrlSelectFunction.SetCurSel(0);

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
	m_pViewer = NULL;

	// viewer
	CRuntimeClass  *pObject;
	pObject = RUNTIME_CLASS( CImageViewer );
	m_pViewer = (CImageViewer*) pObject->CreateObject();
}


void CTestOpenCVDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	//m_srcImage.deallocate();
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
 		m_srcImage = cv::imread((LPSTR)(LPCTSTR)strFile, CV_8UC1 );
//		m_srcImage = cv::imread((LPSTR)(LPCTSTR)strFile, cv::IMREAD_UNCHANGED );

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


void CTestOpenCVDlg::OnBnClickedButtonDo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	switch (GetFunction())
	{
	case FUNCTION_BLOB:
		blob();
		break;

	case FUNCTION_CONVEX_HULL:
		convex_hull();
		break;

	case FUNCTION_CONTOUR:
		contour();
		break;

	default:
		break;
	}
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


int CTestOpenCVDlg::GetFunction()
{
	// TODO: 여기에 구현 코드 추가.

	switch (m_ctrlSelectFunction.GetCurSel())
	{
	case 0:
		return FUNCTION_BLOB;

	case 1:
		return FUNCTION_CONVEX_HULL;

	case 2:
		return FUNCTION_CONTOUR;

	default :
		return FUNCTION_NONE;
	}

	//return 0;
}


//
//
//	labeling 
//
//
void CTestOpenCVDlg::blob(void)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	using namespace cv;

	Mat img_color;

	// threshold
	cv::Mat thresholdImage;
	cv::threshold(m_srcImage, thresholdImage, 50, 255, cv::THRESH_BINARY);
	//cv::imwrite("d:\\test\\1.bmp", thresholdImage);

	cvtColor(m_srcImage, img_color, COLOR_GRAY2BGR);

	Mat img_label, state, centroids;
	int nLables = connectedComponentsWithStats(thresholdImage,
		img_label,
		state,
		centroids,
		8,
		CV_16U);


	// draw region
	for (int y = 0; y < img_label.rows; y++)
	{
		ushort *pLabel = img_label.ptr<ushort>(y);
		Vec3b* pPixel = img_color.ptr<Vec3b>(y);

		for (int x = 0; x < img_label.cols; x++)
		{
			pPixel[x][2] = 0; // pLabel[x] * 30;
			pPixel[x][1] = pLabel[x] * 30;
			pPixel[x][0] = 0; //pLabel[x] * 30;
		}
	}


	// draw rect
	for (int i = 0; i < nLables; i++)
	{
		if (i == 0)	// 배경에 대한 label
		{
			continue;
		}

		int *pLabel = state.ptr<int>(i);

		rectangle(img_color, Rect(pLabel[0], pLabel[1], pLabel[2], pLabel[3]), Scalar(0, 0, 255));
	}


	imshow("label", img_color);

	/*
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
		imshow("key points", im_with_keypoints);
		*/
}


//
//
//
//
//
void CTestOpenCVDlg::convex_hull(void)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	using namespace cv;

	int iCh = m_srcImage.channels();
	if (iCh != 1)
	{
		AfxMessageBox("Error > 1 채널 영상만 사용가능");
		return;
	}

	int thresh = 10;

	// edge filter
	Mat imgCanny;
	Canny(m_srcImage, imgCanny, thresh, thresh);
	

	// find contour
	vector<vector<Point>> contours;
	findContours(imgCanny, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);


	// get convex hull
	vector<vector<Point>>hull(contours.size());
	for (size_t i = 0; i < contours.size(); i++)
	{
		convexHull(contours[i], hull[i]);
	}


	// draw result
	Mat imgResult;
	cvtColor(m_srcImage, imgResult, COLOR_GRAY2BGR);
	//Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
	for (size_t i = 0; i < contours.size(); i++)
	{
		Scalar color = Scalar(0, 0, 255);	// red
		//drawContours(drawing, contours, (int)i, color);
		drawContours(imgResult, contours, (int)i, color);
	}


	// *make image
	Mat imgPoly(m_srcImage.rows, m_srcImage.cols, CV_8U, Scalar(0) );
	fillPoly(imgPoly, hull, Scalar(255));
	imshow("convex", imgPoly);


	// show
	//imshow("convex", imgResult);
}



//
//
//
//
//
void CTestOpenCVDlg::contour(void)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	using namespace cv;

	int iCh = m_srcImage.channels();
	if (iCh != 1)
	{
		AfxMessageBox("Error > 1 채널 영상만 사용가능");
		return;
	}

	int thresh = 10;

	// threshold
	cv::Mat imgThreshold;
	cv::threshold(m_srcImage, imgThreshold, 50, 255, cv::THRESH_BINARY);


	// find contour
	vector<vector<Point>> contours;
	findContours(imgThreshold, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);


	// get convex hull
	vector<vector<Point>>hull(contours.size());
	for (size_t i = 0; i < contours.size(); i++)
	{
		convexHull(contours[i], hull[i]);
	}


	// draw result
	Mat imgResult;
	cvtColor(m_srcImage, imgResult, COLOR_GRAY2BGR);
	//Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
	for (size_t i = 0; i < contours.size(); i++)
	{
		Scalar color = Scalar(0, 0, 255);	// red
		//drawContours(drawing, contours, (int)i, color);
		drawContours(imgResult, contours, (int)i, color);
	}


	// *make image
	Mat imgPoly(m_srcImage.rows, m_srcImage.cols, CV_8U, Scalar(0));
///	fillPoly(imgPoly, hull, Scalar(255));
	fillConvexPoly(imgPoly, hull[0], Scalar(255));
	imshow("convex", imgResult);


	imwrite("d:\\test\\result.bmp", imgPoly);

	// show
	//imshow("convex", imgResult);
}


