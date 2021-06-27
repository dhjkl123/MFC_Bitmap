
// BITMAP_StduyDlg.h: 헤더 파일
//

#pragma once


// CBITMAPStduyDlg 대화 상자
class CBITMAPStduyDlg : public CDialog
{
// 생성입니다.
public:
	CBITMAPStduyDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BITMAP_STDUY_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

public:
	tagBITMAPFILEHEADER m_BitFileHeader; // 첫번째 14바이트
	tagBITMAPINFOHEADER m_BitInfoHeader; // 두번째 40바이트
	tagRGBTRIPLE* m_RGBtrip;
// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};
