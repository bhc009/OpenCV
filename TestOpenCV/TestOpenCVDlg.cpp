
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

using namespace std;


#define FUNCTION_NONE			0
#define FUNCTION_BLOB			1
#define FUNCTION_CONVEX_HULL	2
#define FUNCTION_CONTOUR		3


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

	m_ctrlSelectFunction.InsertString(0, "blob");
	m_ctrlSelectFunction.InsertString(1, "convex hull");
	m_ctrlSelectFunction.InsertString(2, "get contour");

	m_ctrlSelectFunction.SetCurSel(0);

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
	m_pViewer = NULL;

	// viewer
	CRuntimeClass  *pObject;
	pObject = RUNTIME_CLASS( CImageViewer );
	m_pViewer = (CImageViewer*) pObject->CreateObject();
}


void CTestOpenCVDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
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


void CTestOpenCVDlg::OnBnClickedButtonDo()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CString str;
	str.Format("��ġ : %d, %d", m_pViewer->GetCurrentPosition().x, m_pViewer->GetCurrentPosition().y );
	SetDlgItemText(IDC_STATIC_POSITION, str);

	str.Format("��Ⱚ : %d", m_pViewer->GetCurrentGray() );
	SetDlgItemText(IDC_STATIC_GRAY, str);


	CDialogEx::OnTimer(nIDEvent);
}


int CTestOpenCVDlg::GetFunction()
{
	// TODO: ���⿡ ���� �ڵ� �߰�.

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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
		if (i == 0)	// ��濡 ���� label
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	using namespace cv;

	int iCh = m_srcImage.channels();
	if (iCh != 1)
	{
		AfxMessageBox("Error > 1 ä�� ���� ��밡��");
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	using namespace cv;

	int iCh = m_srcImage.channels();
	if (iCh != 1)
	{
		AfxMessageBox("Error > 1 ä�� ���� ��밡��");
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


