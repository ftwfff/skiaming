
// SkiaTestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SkiaTest.h"
#include "SkiaTestDlg.h"
#include "afxdialogex.h"
#include "SkCanvas.h"
#include "SkBitmap.h"
#include "SkiaTestFunc.h"
// #include "SkGraphics.h"
 #include "SkImageEncoder.h"
// #include "SkString.h"
// #include "SkTemplates.h"
 #include "SkImageDecoder.h"
// #include "SkStream.h"
// #include "SkGradientShader.h"
// #include "SkComposeShader.h"
// #include "SkColorFilter.h"
// #include "SkBlurMaskFilter.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static struct SkiaContentAndSpecification
{
	const TCHAR * pContent;
	const TCHAR* pSpecification;
}gSkiaTest[]=
{
	{L"TempTest", L"TempTest"},
	{L"1.01.beginBitmapFill", L"1.01.Graphics::beginBitmapFill"},
	{L"1.02. beginFill", L"1.02. Graphics::beginFill"},
	{L"1.03. beginGradientFill", L"1.03. Graphics::beginGradientFill"},
	{L"1.04. beginShaderFill", L"1.04. Graphics::beginShaderFill"},
	
	{L"2.01. BlurFilter",L"2.01. flash.filters.BlurFilter"},
	{L"2.02. ColorMatrixFilter", L"flash.filters.ColorMatrixFilter"},
	{L"3.01 SkBlendImageFilter", L"3.01 SkImageFilter\r\nSkBlendImageFilter"},
	{L"3.02 SkBlurImageFilter", L"3.02 SkImageFilter\r\nSkBlurImageFilter"},
	{L"3.03 SkColorFilterImageFilter", L"3.03 SkImageFilter\r\nSkColorFilterImageFilter"},
	{L"3.04 SkLightingImageFilter", L"3.04 SkImageFilter\r\nSkLightingImageFilter"},
	{L"3.05 SkMagnifierImageFilter", L"3.05 SkImageFilter\r\nSkMagnifierImageFilter"},
	{L"4.01 SkModeColorFilter", L"4.01 SkImageFilter\r\nSkModeColorFilter"},
	{L"4.02 SkColorMatrixFilter", L"4.02 SkColorFilter\r\nSkColorMatrixFilter"},
	{L"4.03 SkLightingColorFilter", L"4.03 SkImageFilter\r\nSkLightingColorFilter"},
	{L"4.04 SkSimpleColorFilter", L"4.04 SkImageFilter\r\nSkSimpleColorFilter"},
	{L"4.05 SkModeColorFilter", L"4.05 SkImageFilter\r\nSkModeColorFilter"},
// 	{L"4.06 SkModeColorFilter", L"4.06 SkImageFilter\r\nSkModeColorFilter"},
// 	{L"4.07 SkModeColorFilter", L"4.07 SkImageFilter\r\nSkModeColorFilter"},
// 	{L"4.08 SkModeColorFilter", L"4.08 SkImageFilter\r\nSkModeColorFilter"},
// 	{L"4.09 SkModeColorFilter", L"4.09 SkImageFilter\r\nSkModeColorFilter"},
};

typedef bool (*FuncSkiaTest)(SkCanvas*, SkBitmap*);
FuncSkiaTest gSkiaTestFunc[] =
{
	SkiaTempTestFunc,
	SkiaTestFunc0101beginBitmapFill,
	SkiaTestFunc0102beginFill,
	SkiaTestFunc0103beginGradientFill,
	SkiaTestFunc0104beginShaderFill,
	SkiaTestFunc0201BlurFilter,
 	SkiaTestFunc0202ColorMatrixFilter,
	SkiaTestFunc0301SkBlendImageFilter,
	SkiaTestFunc0302SkBlurImageFilter,
	SkiaTestFunc0303SkColorFilterImageFilter,
	SkiaTestFunc0304SkLightingImageFilter,
	SkiaTestFunc0305SkMagnifierImageFilter,
	SkiaTestFunc0401SkTable_ColorFilter,
	SkiaTestFunc0402SkColorMatrixFilter,
	SkiaTestFunc0403SkLightingColorFilter,
	SkiaTestFunc0404SkSimpleColorFilter,
	SkiaTestFunc0405SkModeColorFilter,

// 	SkiaTestFunc7,
// 	SkiaTestFunc8
};
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CSkiaTestDlg 对话框




CSkiaTestDlg::CSkiaTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSkiaTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pCanvas = new SkCanvas();
	m_pBitmap = new SkBitmap();
}

CSkiaTestDlg::~CSkiaTestDlg()
{
	if(m_pBitmap != NULL)
		delete m_pBitmap;
	m_pBitmap = NULL;
	if(m_pCanvas != NULL)
		delete m_pCanvas;

}

void CSkiaTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSkiaTestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BNT_DRAW, &CSkiaTestDlg::OnBnClickedBntDraw)
	ON_BN_CLICKED(IDC_BTN_OUT, &CSkiaTestDlg::OnBnClickedBtnOut)
	ON_CBN_SELCHANGE(IDC_COMBO, &CSkiaTestDlg::OnSelchangeCombo)
END_MESSAGE_MAP()


// CSkiaTestDlg 消息处理程序

BOOL CSkiaTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO);
	for(int i = 0; i < sizeof(gSkiaTest)/sizeof(gSkiaTest[0]); ++i)
		pCombo->AddString(gSkiaTest[i].pContent);
	int iInitCombo = AfxGetApp()->GetProfileInt(TEXT("Skia"), TEXT("ComboInit"), 0);
	if(iInitCombo > sizeof(gSkiaTest)/sizeof(gSkiaTest[0])) iInitCombo = 0;
	pCombo->SetCurSel(iInitCombo);
	SetDlgItemText(IDC_EDIT, gSkiaTest[iInitCombo].pSpecification);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CSkiaTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSkiaTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSkiaTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSkiaTestDlg::OnBnClickedBntDraw()
{
	// TODO: 在此添加控件通知处理程序代码
	RECT rt;
	GetDlgItem(IDC_STATIC_SHOW)->GetWindowRect(&rt);
	ScreenToClient(&rt);
	m_pBitmap->reset();
	m_pBitmap->setConfig(SkBitmap::kARGB_8888_Config, 
		abs(rt.left - rt.right), abs(rt.bottom - rt.top));
	m_pBitmap->allocPixels();
	m_pCanvas->setBitmapDevice(*m_pBitmap);
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO);
	if(!gSkiaTestFunc[pCombo->GetCurSel()](m_pCanvas, m_pBitmap))	return;
	
	
	BITMAPINFO bmi;
	memset(&bmi, 0, sizeof(bmi));
	bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth       = m_pBitmap->width();
	bmi.bmiHeader.biHeight      = -m_pBitmap->height(); // top-down image
	bmi.bmiHeader.biPlanes      = 1;
	bmi.bmiHeader.biBitCount    = 32;
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biSizeImage   = 0;

	CDC* pDC = GetDC();
	m_pBitmap->lockPixels();
	int iRet = SetDIBitsToDevice(pDC->GetSafeHdc(),
		rt.left, rt.top,
		abs(rt.left - rt.right)/*m_pBitmap->width()*/, abs(rt.top - rt.bottom)/*m_pBitmap->height()*/,
		0, 0,
		0, m_pBitmap->height(),
		m_pBitmap->getPixels(),
		&bmi,
		DIB_RGB_COLORS);
	m_pBitmap->unlockPixels();
	//将数据显示到屏幕上
	ReleaseDC(pDC);
}


void CSkiaTestDlg::OnBnClickedBtnOut()
{
	// TODO: 在此添加控件通知处理程序代码
	RECT rt;
	GetDlgItem(IDC_STATIC_SHOW)->GetWindowRect(&rt);
	ScreenToClient(&rt);
	m_pBitmap->reset();
	m_pBitmap->setConfig(SkBitmap::kARGB_8888_Config, 
		abs(rt.left - rt.right), abs(rt.bottom - rt.top));
	m_pBitmap->allocPixels();
	m_pCanvas->setBitmapDevice(*m_pBitmap);
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO);
	if(!gSkiaTestFunc[pCombo->GetCurSel()](m_pCanvas, m_pBitmap)) return;

	BITMAPINFO bmi;
	memset(&bmi, 0, sizeof(bmi));
	bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth       = m_pBitmap->width();
	bmi.bmiHeader.biHeight      = -m_pBitmap->height(); // top-down image
	bmi.bmiHeader.biPlanes      = 1;
	bmi.bmiHeader.biBitCount    = 32;
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biSizeImage   = 0;
	CDC* pDC = GetDC();
	m_pBitmap->lockPixels();
	int iRet = SetDIBitsToDevice(pDC->GetSafeHdc(),
		rt.left, rt.top,
		m_pBitmap->width(), m_pBitmap->height(),
		0, 0,
		0, m_pBitmap->height(),
		m_pBitmap->getPixels(),
		&bmi,
		DIB_RGB_COLORS);
	m_pBitmap->unlockPixels();
	if(!iRet) AfxMessageBox(TEXT("Error!"));
	//将数据显示到屏幕上
	ReleaseDC(pDC);
	CFileDialog fileDlg(FALSE);
	//if(GetVersion()>5) fileDlg.m_ofn.lStructSize = 88;
	fileDlg.m_ofn.lpstrFilter = TEXT("PNG Files(*.png)\0(*.png)\0\0");
	if(IDOK != fileDlg.DoModal()) return;
	CString sFilePathTemp(fileDlg.GetPathName());
	char sFilePath[256];
#ifdef _UNICODE
	wchar_t* pWChar = sFilePathTemp.GetBuffer(sFilePathTemp.GetLength());
	int iAnsiLen = ::WideCharToMultiByte(CP_ACP, NULL, pWChar, wcslen(pWChar), NULL, 0, NULL, NULL); 
	::WideCharToMultiByte(CP_ACP, NULL, pWChar, wcslen(pWChar), sFilePath, iAnsiLen, NULL, NULL); 
	sFilePath[iAnsiLen] = '\0';
#endif
	
	bool success = SkImageEncoder::EncodeFile(sFilePath,	*m_pBitmap, 
		SkImageEncoder::kPNG_Type, 100);
	if(!success)
		AfxMessageBox(TEXT("Output error !"));
}


void CSkiaTestDlg::OnSelchangeCombo()
{
	// TODO: 在此添加控件通知处理程序代码
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO);
	int iCombo = pCombo->GetCurSel();
	SetDlgItemText(IDC_EDIT, gSkiaTest[iCombo].pSpecification);
	AfxGetApp()->WriteProfileInt(TEXT("Skia"), TEXT("ComboInit"), iCombo);
}
