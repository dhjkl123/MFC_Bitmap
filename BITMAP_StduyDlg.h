
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
	tagRGBTRIPLE* m_RGBtripTmp;
	int m_nRGBquadSize;

	CRect m_rcPicture;
	CRect m_rcOriginPicture;
	CBitmap m_Bitmap;
	int m_nHisto[256];
	float m_fHisto[256];

	void  DrawBitMap();
	int  CopyRGB();
	BYTE LimiteBYTE(int* byte);
	void MakeMemBitmap(CRect* rc);
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
	afx_msg void OnBnClickedButtonGray();
	afx_msg void OnBnClickedButtonBmpSave();
	afx_msg void OnDestroy();
	CString m_edit_strThreshHold;
	afx_msg void OnBnClickedButtonTh();
//	afx_msg void OnEnChangeEditTh();
	afx_msg void OnEnUpdateEditTh();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonSub();
	CSliderCtrl m_slid;
//	afx_msg void OnTRBNThumbPosChangingSlider(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CSliderCtrl m_slid2;
};
