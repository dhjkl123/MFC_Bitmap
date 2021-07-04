
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
	, m_edit_strThreshHold(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_RGBtripTmp = NULL;
	m_RGBtrip = NULL;
	m_nRGBquadSize = 0;
	//m_nHisto = NULL;
	//m_fHisto = NULL;
}

void CBITMAPStduyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_TH, m_edit_strThreshHold);
	DDX_Control(pDX, IDC_SLIDER, m_slid);
	DDX_Control(pDX, IDC_SLIDER2, m_slid2);
}

BEGIN_MESSAGE_MAP(CBITMAPStduyDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CBITMAPStduyDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON_GRAY, &CBITMAPStduyDlg::OnBnClickedButtonGray)
	ON_BN_CLICKED(IDC_BUTTON_BMP_SAVE, &CBITMAPStduyDlg::OnBnClickedButtonBmpSave)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_TH, &CBITMAPStduyDlg::OnBnClickedButtonTh)
//	ON_EN_CHANGE(IDC_EDIT_TH, &CBITMAPStduyDlg::OnEnChangeEditTh)
	ON_EN_UPDATE(IDC_EDIT_TH, &CBITMAPStduyDlg::OnEnUpdateEditTh)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CBITMAPStduyDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_SUB, &CBITMAPStduyDlg::OnBnClickedButtonSub)
//	ON_NOTIFY(TRBN_THUMBPOSCHANGING, IDC_SLIDER, &CBITMAPStduyDlg::OnTRBNThumbPosChangingSlider)
	ON_CONTROL_RANGE(BN_CLICKED,IDC_RADIO_NON,IDC_RADIO_EQ,  &CBITMAPStduyDlg::OnBnClickedRadio)
	ON_WM_HSCROLL()
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
	
	m_slid.SetRange(0, 255);
	m_slid.SetPos(127);
	m_slid.SetLineSize(10);

	m_slid2.SetRange(-50, 50);
	m_slid2.SetPos(1);
	m_slid2.SetLineSize(1);
	
	CButton* pCheck;
	pCheck = (CButton*)GetDlgItem(IDC_RADIO_NON);
	pCheck->SetCheck(1);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다. m_slid2
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
	
	}
	else
	{
		int nCnt = m_nRGBquadSize / sizeof(tagRGBTRIPLE);

		if (nCnt <= 0) return;

		CPaintDC dc(this);
		CRect rect, buffrect;
		BITMAP bitmap;
		GetDlgItem(IDC_STATIC_HISTO)->GetWindowRect(rect);
		ScreenToClient(&rect);
		buffrect = rect;

		buffrect.left = 0;
		buffrect.top = 0;
		buffrect.right = rect.right - rect.left;
		buffrect.bottom = rect.bottom - rect.top;



		MakeMemBitmap(&buffrect);

		CDC MemDC;
		CBitmap* pOldBmp;
		
		MemDC.CreateCompatibleDC(&dc);


		pOldBmp = MemDC.SelectObject(&m_Bitmap);

		pOldBmp->GetBitmap(&bitmap);
		

//		dc.BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &MemDC, 0, 0, SRCCOPY);

		dc.StretchBlt(rect.left, rect.top, rect.Width(), rect.Height(),
			&MemDC, 0, 0, buffrect.Width(), buffrect.Height(), SRCCOPY);

		MemDC.SelectObject(pOldBmp);
		MemDC.DeleteDC();

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

		memset(m_nHisto, 0x00, 256 * 4);
		memset(m_fHisto, 0x00, 256 * 4);
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

	int RGBCnt = CopyRGB();

	if (RGBCnt <= 0) return;

	memset(m_nHisto, 0x00, 256 * 4);

	for (int i = 0; i < RGBCnt; i++) //i++
	{	
		//BYTE rgbtGray = m_RGBtrip[i].rgbtRed * 0.299 + m_RGBtrip[i].rgbtGreen * 0.587 + m_RGBtrip[i].rgbtBlue * 0.114;
		//BYTE rgbtGray = m_RGBtrip[i].rgbtRed * 0.2126 + m_RGBtrip[i].rgbtGreen * 0.7152 + m_RGBtrip[i].rgbtBlue * 0.0722;
		BYTE rgbtGray = (m_RGBtrip[i].rgbtRed  + m_RGBtrip[i].rgbtGreen + m_RGBtrip[i].rgbtBlue)/3;
		m_RGBtripTmp[i].rgbtBlue = m_RGBtripTmp[i].rgbtGreen = m_RGBtripTmp[i].rgbtRed = rgbtGray;

		m_nHisto[m_RGBtripTmp[i].rgbtBlue]++;
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

	if (m_RGBtripTmp != NULL)
	{
		delete m_RGBtripTmp;
		m_RGBtripTmp = NULL;
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

int  CBITMAPStduyDlg::CopyRGB()
{

	int nCnt = m_nRGBquadSize / sizeof(tagRGBTRIPLE);

	if (nCnt <= 0) return -1;

	memcpy_s(m_RGBtripTmp, m_nRGBquadSize, m_RGBtrip, m_nRGBquadSize);

	return nCnt;
}

void CBITMAPStduyDlg::OnBnClickedButtonTh()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int RGBCnt = CopyRGB();

	if (RGBCnt <= 0) return;

	memset(m_nHisto, 0x00, 256 * 4);

	for (int i = 0; i < RGBCnt; i++)
	{
		BYTE rgbtGray = m_RGBtrip[i].rgbtRed * 0.299 + m_RGBtrip[i].rgbtGreen * 0.587 + m_RGBtrip[i].rgbtBlue * 0.114;

		if (_ttoi(m_edit_strThreshHold) < rgbtGray) rgbtGray = 0;
		else rgbtGray = 255;

		m_RGBtripTmp[i].rgbtBlue = m_RGBtripTmp[i].rgbtGreen = m_RGBtripTmp[i].rgbtRed = rgbtGray;

		m_nHisto[m_RGBtripTmp[i].rgbtBlue]++;
	}

	DrawBitMap();

	

}


//void CBITMAPStduyDlg::OnEnChangeEditTh()
//{
//	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
//	// CDialog::OnInitDialog() 함수를 재지정 
//	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
//	// 이 알림 메시지를 보내지 않습니다.
//
//	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
//
//	if (_ttoi(m_edit_strThreshHold) > 256) m_edit_strThreshHold.Format(_T("255"));
//	
//	UpdateData();
//}


void CBITMAPStduyDlg::OnEnUpdateEditTh()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialog::OnInitDialog() 함수를 재지정 
	//하여, IParam 마스크에 OR 연산하여 설정된 ENM_SCROLL 플래그를 지정하여 컨트롤에 EM_SETEVENTMASK 메시지를 보내지 않으면
	// 편집 컨트롤이 바뀐 텍스트를 표시하려고 함을 나타냅니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData();
	if (_ttoi(m_edit_strThreshHold) > 256) m_edit_strThreshHold.Format(_T("255"));

	UpdateData(FALSE);
	Invalidate();
}


void CBITMAPStduyDlg::OnBnClickedButtonAdd()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int RGBCnt = CopyRGB();

	if (RGBCnt <= 0) return;

	// 픽셀은 BYTE 단위로 RGB를 표현 함으로 255를 넘지 않아야한다.

	memset(m_nHisto, 0x00, 256 * 4);

	for (int i = 0; i < RGBCnt; i++) 
	{ 
		int ntmp = 0;
		BYTE rgbtGray = (m_RGBtrip[i].rgbtRed + m_RGBtrip[i].rgbtGreen + m_RGBtrip[i].rgbtBlue) / 3; 

		ntmp = rgbtGray + 100;

		OnCalcHisto(i, ntmp, RGBCnt);
	}

	DrawBitMap();


}


void CBITMAPStduyDlg::OnBnClickedButtonSub()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int RGBCnt = CopyRGB();

	if (RGBCnt <= 0) return;

	memset(m_nHisto, 0x00, 256 * 4);

	for (int i = 0; i < RGBCnt; i++)
	{
		int ntmp = 0;
		BYTE rgbtGray = (m_RGBtrip[i].rgbtRed + m_RGBtrip[i].rgbtGreen + m_RGBtrip[i].rgbtBlue) / 3;

		ntmp = rgbtGray - 100;

		OnCalcHisto(i, ntmp, RGBCnt);

	}

	DrawBitMap();

}


//void CBITMAPStduyDlg::OnTRBNThumbPosChangingSlider(NMHDR* pNMHDR, LRESULT* pResult)
//{
//	// 이 기능을 사용하려면 Windows Vista 이상이 있어야 합니다.
//	// _WIN32_WINNT 기호는 0x0600보다 크거나 같아야 합니다.
//	NMTRBTHUMBPOSCHANGING* pNMTPC = reinterpret_cast<NMTRBTHUMBPOSCHANGING*>(pNMHDR);
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//
//
//
//	DrawBitMap();
//
//	*pResult = 0;
//}


void CBITMAPStduyDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	int RGBCnt = CopyRGB();

	if (RGBCnt <= 0) return;

	memset(m_nHisto, 0x00, 256 * 4);
	memset(m_fHisto, 0x00, 256 * 4);

	if (IDC_SLIDER == pScrollBar->GetDlgCtrlID()) //밝기 조절
	{

		for (int i = 0; i < RGBCnt; i++)
		{
			int ntmp = 0;
			int nPos = m_slid.GetPos();
			BYTE rgbtGray = (m_RGBtrip[i].rgbtRed + m_RGBtrip[i].rgbtGreen + m_RGBtrip[i].rgbtBlue) / 3;

			ntmp = rgbtGray + (nPos - 127);

			OnCalcHisto(i, ntmp, RGBCnt);

		}
	}
	else if (IDC_SLIDER2 == pScrollBar->GetDlgCtrlID()) //대비 조절
	{	
		OnContrast(RGBCnt);
	}

	m_fMaxHisto = m_fHisto[0];
	for (int i = 1; i < 256; i++)
		if (m_fHisto[i] > m_fMaxHisto) m_fMaxHisto = m_fHisto[i];

	MinMaxHisto(m_RGBtripTmp);
	Invalidate(FALSE);
	DrawBitMap();

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

BYTE CBITMAPStduyDlg::LimiteBYTE(int* byte)
{
	if (*byte < 0)
		*byte = 0;
	else if (*byte > 255)
		*byte = 255;

	return (BYTE)*byte;
}

void CBITMAPStduyDlg::OnContrast(int Cnt)
{
	for (int i = 0; i < Cnt; i++)
	{
		int ntmp = 0;
		int nPos = m_slid2.GetPos();
		BYTE rgbtGray = (m_RGBtrip[i].rgbtRed + m_RGBtrip[i].rgbtGreen + m_RGBtrip[i].rgbtBlue) / 3;

		ntmp = rgbtGray;

		ntmp = ntmp + ((ntmp - 128) * ((double)nPos / 100));

		OnCalcHisto(i, ntmp, Cnt);

	}
}

void CBITMAPStduyDlg::MinMaxHisto(tagRGBTRIPLE* tagRGB)
{
	m_max_histo = tagRGB[0].rgbtBlue;
	m_min_histo = tagRGB[0].rgbtBlue;

	for (int i = 1; i < 256; i++)
	{
		if (tagRGB[i].rgbtBlue > m_max_histo)
			m_max_histo = tagRGB[i].rgbtBlue;

		if (tagRGB[i].rgbtBlue < m_min_histo)
			m_min_histo = tagRGB[i].rgbtBlue;

	}
}

void CBITMAPStduyDlg::OnCalcHisto(int i, int nRet , int Max)
{
	BYTE bbyte = LimiteBYTE(&nRet);

	m_RGBtripTmp[i].rgbtBlue = m_RGBtripTmp[i].rgbtGreen = m_RGBtripTmp[i].rgbtRed
		= bbyte;

	m_nHisto[m_RGBtripTmp[i].rgbtBlue]++;

	m_fHisto[m_RGBtripTmp[i].rgbtBlue] = static_cast<float>(m_nHisto[m_RGBtripTmp[i].rgbtBlue]) / Max;

}

void CBITMAPStduyDlg::MakeMemBitmap(CRect* rc)
{
	if (m_nHisto == NULL) return;

	int nCnt = m_nRGBquadSize / sizeof(tagRGBTRIPLE);

	CClientDC dc(this);
	CBitmap* pOldBmp;
	CDC MemDC;

	m_Bitmap.DeleteObject();
	m_Bitmap.CreateCompatibleBitmap(&dc, rc->Width(), rc->Height());
	MemDC.CreateCompatibleDC(&dc);
	pOldBmp = MemDC.SelectObject(&m_Bitmap);

	//<--- 그리기
	CPen Pen(PS_SOLID, 3, RGB(255, 0, 0));
	CPen* pOldPen;
	CBrush* pOldBrush, brush;
	COLORREF color = 0x00ffffff;
	brush.CreateSolidBrush(color);
	

	pOldBrush = MemDC.SelectObject(&brush);

	MemDC.Rectangle(rc);

	MemDC.SelectObject(pOldBrush);

	//pOldPen = MemDC.SelectObject(&Pen);

	double dbRcSize = (double)rc->Width()/256;


	for (int i = 0; i < 256; i++)
	{
		int nXpos = dbRcSize * i;
		int nYpos = m_fHisto[i] * 100 / m_fMaxHisto;
		MemDC.MoveTo(nXpos, rc->bottom);
		MemDC.LineTo(nXpos, rc->bottom - (nYpos * rc->bottom / 100));
	}

	MemDC.SelectObject(pOldBmp);
	MemDC.DeleteDC();

}

void CBITMAPStduyDlg::OnBnClickedRadio(UINT unid)
{
	int nCnt = m_nRGBquadSize / sizeof(tagRGBTRIPLE);

	memset(m_nHisto, 0x00, 256 * 4);
	memset(m_fHisto, 0x00, 256 * 4);

	switch (unid)
	{
	case IDC_RADIO_NON: // 대비
		OnContrast(nCnt);
		break;

	case IDC_RADIO_STRET: // 히스토그램 스트레치 (어두울땐 밝게, 밝을땐 어둡게)

		MinMaxHisto(m_RGBtripTmp);

		for (int i = 0; i < nCnt; i++)
		{
			//m_fHisto[i] = (m_fHisto[i] - m_min_histo) * 255 / (m_max_histo - m_min_histo);
			int nRet = (m_RGBtripTmp[i].rgbtBlue - m_min_histo) * 255 / (m_max_histo - m_min_histo);

			OnCalcHisto(i, nRet, nCnt);
		}
		break;

	case IDC_RADIO_EQ: // 히스토그램 평활화 (대비를 더 극명하게)

		OnContrast(nCnt);
		int nHisto[256];
		int fHisto[256];
		nHisto[0] = m_nHisto[0];

		for (int i = 0; i < 255; i++)		
			nHisto[i + 1] = nHisto[i] + m_nHisto[i + 1];

		for (int i = 0; i < nCnt; i++)
		{
			double nRet = ((double)nHisto[m_RGBtripTmp[i].rgbtBlue] / (double)nHisto[255]) * 255;

			OnCalcHisto(i, nRet, nCnt);

			m_fMaxHisto = m_fHisto[0];
			for (int j = 1; j < 256; j++)
				if (m_fHisto[j] > m_fMaxHisto) m_fMaxHisto = m_fHisto[j];


		}

		break;

	default:
		break;
	}

	Invalidate(FALSE);
	DrawBitMap();

}