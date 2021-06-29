
// BITMAP_StduyDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "BITMAP_Stduy.h"
#include "BITMAP_StduyDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBITMAPStduyDlg 대화 상자



CBITMAPStduyDlg::CBITMAPStduyDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_BITMAP_STDUY_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_RGBtripTmp = NULL;
	m_RGBtrip = NULL;
	m_nRGBquadSize = 0;
}

void CBITMAPStduyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CBITMAPStduyDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CBITMAPStduyDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON_GRAY, &CBITMAPStduyDlg::OnBnClickedButtonGray)
	ON_BN_CLICKED(IDC_BUTTON_BMP_SAVE, &CBITMAPStduyDlg::OnBnClickedButtonBmpSave)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CBITMAPStduyDlg 메시지 처리기

BOOL CBITMAPStduyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다. IDC_STATIC_ORIGIN

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	GetDlgItem(IDC_STATIC)->GetWindowRect(m_rcPicture);
	ScreenToClient(&m_rcPicture);

	GetDlgItem(IDC_STATIC_ORIGIN)->GetWindowRect(m_rcOriginPicture);
	ScreenToClient(&m_rcOriginPicture);
	
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CBITMAPStduyDlg::OnPaint()
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


		/*memDC.CreateCompatibleDC(&dc);
		memDC.SelectObject(bmp);*/
	
	}
	else
	{
		CPaintDC dc(this);

		CDialog::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CBITMAPStduyDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CBITMAPStduyDlg::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CClientDC dc(this);

	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_EXPLORER | OFN_HIDEREADONLY, L"파일 선택(*.bmp, *.jpg) | *.bmp; *.jpg");
	CString strPath;
	int iReturn = fileDlg.DoModal();

	if (iReturn == IDOK)
	{
		strPath = fileDlg.GetPathName();       // 경로 받기
	}

	if (strPath.IsEmpty()) return;

	HANDLE bmp;
	CBitmap Bitmap;
	CBitmap* pOldBmp = NULL;
	BITMAP bmpinfo;
	CDC memDC;

	memDC.CreateCompatibleDC(&dc);
	bmp = LoadImage(NULL, strPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	Bitmap.Attach(bmp);
	Bitmap.GetBitmap(&bmpinfo);

	if (bmpinfo.bmBitsPixel >= 32)
	{
		CFile file;

		if (!file.Open(strPath, CFile::modeRead | CFile::typeBinary))
		{
			AfxMessageBox(_T("파일 열기 실패!"));
			return;
		}

		char* pbuf = NULL;
		ULONGLONG nFileLen = file.GetLength();
		pbuf = new char[(int)nFileLen];
		UINT nRead = file.Read(pbuf, (UINT)nFileLen);
		file.Close();

		m_nRGBquadSize = nFileLen - (sizeof(tagBITMAPFILEHEADER) + sizeof(tagBITMAPINFOHEADER));
		int RGBCnt = m_nRGBquadSize / sizeof(tagRGBTRIPLE);
		if (m_RGBtrip != NULL)
		{
			delete m_RGBtrip;
			m_RGBtrip = NULL;
		}

		if (m_RGBtripTmp != NULL)
		{
			delete m_RGBtripTmp;
			m_RGBtripTmp = NULL;
		}

		m_RGBtrip = new tagRGBTRIPLE[RGBCnt];
		m_RGBtripTmp = new tagRGBTRIPLE[RGBCnt];

		memcpy_s(&m_BitFileHeader, sizeof(tagBITMAPFILEHEADER), pbuf, sizeof(tagBITMAPFILEHEADER));
		memcpy_s(&m_BitInfoHeader, sizeof(tagBITMAPINFOHEADER), pbuf + sizeof(tagBITMAPFILEHEADER), sizeof(tagBITMAPINFOHEADER));
		memcpy_s(m_RGBtrip, m_nRGBquadSize, pbuf + sizeof(tagBITMAPFILEHEADER) + sizeof(tagBITMAPINFOHEADER), m_nRGBquadSize);		

		pOldBmp = memDC.SelectObject(&Bitmap);
		dc.StretchBlt(m_rcOriginPicture.left, m_rcOriginPicture.top, m_rcOriginPicture.Width(), m_rcOriginPicture.Height(),
			&memDC,0,0, bmpinfo.bmWidth, bmpinfo.bmHeight, SRCCOPY);
		
		Invalidate(FALSE);

		memDC.SelectObject(pOldBmp);
		memDC.DeleteDC();

		

		delete pbuf;

	}



}


void CBITMAPStduyDlg::OnBnClickedButtonGray()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int RGBCnt = m_nRGBquadSize / sizeof(tagRGBTRIPLE);

	if (RGBCnt <= 0) return;

	memcpy_s(m_RGBtripTmp, m_nRGBquadSize, m_RGBtrip, m_nRGBquadSize);

	for (int i = 0; i < RGBCnt; i++)
	{	
		BYTE rgbtGray = m_RGBtrip[i].rgbtRed * 0.299 + m_RGBtrip[i].rgbtGreen * 0.587 + m_RGBtrip[i].rgbtBlue * 0.114;
		m_RGBtripTmp[i].rgbtBlue = m_RGBtripTmp[i].rgbtGreen = m_RGBtripTmp[i].rgbtRed = rgbtGray;
	}

	DrawBitMap();

}


void CBITMAPStduyDlg::OnBnClickedButtonBmpSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFile file;
	char* pbuf = NULL;

	pbuf = new char[m_nRGBquadSize + sizeof(tagBITMAPFILEHEADER) + sizeof(tagBITMAPINFOHEADER)];

	memcpy_s(pbuf , sizeof(tagBITMAPFILEHEADER), &m_BitFileHeader, sizeof(tagBITMAPFILEHEADER)); // 파일 헤더 14 
	memcpy_s(pbuf + sizeof(tagBITMAPFILEHEADER) , sizeof(tagBITMAPINFOHEADER), &m_BitInfoHeader, sizeof(tagBITMAPINFOHEADER)); // 인포헤더
	memcpy_s(pbuf + sizeof(tagBITMAPFILEHEADER) + sizeof(tagBITMAPINFOHEADER), m_nRGBquadSize, m_RGBtripTmp, m_nRGBquadSize); // 나머지

	file.Open(_T(".\\Tmp.bmp"), CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);

	file.Write(pbuf, sizeof(tagBITMAPFILEHEADER) + sizeof(tagBITMAPINFOHEADER) + m_nRGBquadSize);
	file.Close();
	delete pbuf;
}


void CBITMAPStduyDlg::OnDestroy()
{
	CDialog::OnDestroy();

	if (m_RGBtrip != NULL)
	{
		delete m_RGBtrip;
		m_RGBtrip = NULL;
	}

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CBITMAPStduyDlg::DrawBitMap()
{
	int RGBCnt = m_nRGBquadSize / sizeof(tagRGBTRIPLE);

	if (RGBCnt <= 0) return;

	CClientDC dc(this);

	BITMAPINFO bitmapinfo;
	RGBQUAD* rgbquad = NULL; 

	rgbquad = new RGBQUAD[RGBCnt];

	memset(rgbquad, 0x00, RGBCnt * sizeof(RGBQUAD));

	for (int i = 0; i < RGBCnt; i++)
	{
		rgbquad[i].rgbBlue = m_RGBtripTmp[i].rgbtBlue;
		rgbquad[i].rgbGreen = m_RGBtripTmp[i].rgbtGreen;
		rgbquad[i].rgbRed = m_RGBtripTmp[i].rgbtRed;
	}

	bitmapinfo.bmiHeader = m_BitInfoHeader;
	bitmapinfo.bmiColors[0] = rgbquad[0];

	DWORD height = m_BitInfoHeader.biHeight;

	DWORD width = m_BitInfoHeader.biWidth;

	SetDIBitsToDevice(dc.GetSafeHdc(), m_rcPicture.left, m_rcPicture.top, width, height, 0, 0, 0, height, m_RGBtripTmp, &bitmapinfo, DIB_RGB_COLORS);

	delete rgbquad;
}