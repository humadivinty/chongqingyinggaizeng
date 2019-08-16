
// test_PLATERECOGNIZEDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "test_PLATERECOGNIZE.h"
#include "test_PLATERECOGNIZEDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "../PLATERECOGNIZE/PLATERECOGNIZE.h"
#ifdef DEBUG
#pragma comment(lib,"../Debug/PLATERECOGNIZE.lib")
#else
#pragma comment(lib, "../release/PLATERECOGNIZE.lib")
#endif

#define BIN_IMG 0
#define PLATE_IMG 1
#define COLOR_VEHICLE_HEAD_IMG 2
#define FULL_IMG 3



// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// Ctest_PLATERECOGNIZEDlg �Ի���



Ctest_PLATERECOGNIZEDlg::Ctest_PLATERECOGNIZEDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(Ctest_PLATERECOGNIZEDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Ctest_PLATERECOGNIZEDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(Ctest_PLATERECOGNIZEDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_MESSAGE(WM_MYMSG, &Ctest_PLATERECOGNIZEDlg::OnUserResult)
    ON_BN_CLICKED(IDC_BUTTON_InitDevice, &Ctest_PLATERECOGNIZEDlg::OnBnClickedButtonInitdevice)
    ON_BN_CLICKED(IDC_BUTTON_CloseDevice, &Ctest_PLATERECOGNIZEDlg::OnBnClickedButtonClosedevice)
    ON_BN_CLICKED(IDC_BUTTON_GetInfo, &Ctest_PLATERECOGNIZEDlg::OnBnClickedButtonGetinfo)
    ON_BN_CLICKED(IDC_BUTTON_GetImage, &Ctest_PLATERECOGNIZEDlg::OnBnClickedButtonGetimage)
    ON_BN_CLICKED(IDC_BUTTON_GetState, &Ctest_PLATERECOGNIZEDlg::OnBnClickedButtonGetstate)
    ON_BN_CLICKED(IDC_BUTTON_Capture, &Ctest_PLATERECOGNIZEDlg::OnBnClickedButtonCapture)
    ON_BN_CLICKED(IDC_BUTTON_InitVideo, &Ctest_PLATERECOGNIZEDlg::OnBnClickedButtonInitvideo)
    ON_BN_CLICKED(IDC_BUTTON_CloseVideo, &Ctest_PLATERECOGNIZEDlg::OnBnClickedButtonClosevideo)
    ON_BN_CLICKED(IDC_BUTTON_Screenshot, &Ctest_PLATERECOGNIZEDlg::OnBnClickedButtonScreenshot)
    ON_BN_CLICKED(IDC_BUTTON_Timing, &Ctest_PLATERECOGNIZEDlg::OnBnClickedButtonTiming)
    ON_BN_CLICKED(IDC_BUTTON_ShowTime, &Ctest_PLATERECOGNIZEDlg::OnBnClickedButtonShowtime)
    ON_BN_CLICKED(IDC_BUTTON_CloseTime, &Ctest_PLATERECOGNIZEDlg::OnBnClickedButtonClosetime)
    ON_BN_CLICKED(IDC_BUTTON_GetDeviceInfo, &Ctest_PLATERECOGNIZEDlg::OnBnClickedButtonGetdeviceinfo)
    ON_BN_CLICKED(IDC_BUTTON_USEmsg, &Ctest_PLATERECOGNIZEDlg::OnBnClickedButtonUsemsg)
END_MESSAGE_MAP()


// Ctest_PLATERECOGNIZEDlg ��Ϣ�������

BOOL Ctest_PLATERECOGNIZEDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
    m_bUseMessageModel = false;

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void Ctest_PLATERECOGNIZEDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void Ctest_PLATERECOGNIZEDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR Ctest_PLATERECOGNIZEDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void Ctest_PLATERECOGNIZEDlg::OnBnClickedButtonInitdevice()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    CString cstrIP;
    GetDlgItem(IDC_IPADDRESS1)->GetWindowText(cstrIP);
    int iwidth = 0, iheight = 0;
    CWnd* ph = GetDlgItem(IDC_STATIC_video);
    CRect rectCtrl;
    ph->GetWindowRect(rectCtrl);
    char chpix[32] = {0};
    sprintf_s(chpix, sizeof(chpix), "%d*%d", rectCtrl.Width(), rectCtrl.Height());
    char chIP[64] = {0};
    sprintf_s(chIP, sizeof(chIP), "%s", cstrIP.GetBuffer());
    cstrIP.ReleaseBuffer();

    BOOL bRet = Plate_InitDevice(m_hWnd, WM_MYMSG, 2, chIP, 0, chpix);

    char chLog[256] = {0};
    sprintf_s(chLog, sizeof(chLog), "Plate_InitDevice,  handle = %p, msg= %d, portType = %d, portName= %s, Baudrate= %d, Px= %s, return code = %d",
        m_hWnd,
        WM_MYMSG,
        2,
        chIP,
        0,
        chpix, bRet);
    ShowMessage(chLog);
    MessageBox(chLog);
}


void Ctest_PLATERECOGNIZEDlg::OnBnClickedButtonClosedevice()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    BOOL bRet = Plate_CloseDevice();
    char chLog[256] = { 0 };
    sprintf_s(chLog, sizeof(chLog), "Plate_CloseDevice,  return code = %d", bRet);
    ShowMessage(chLog);
    MessageBox(chLog);
}


void Ctest_PLATERECOGNIZEDlg::OnBnClickedButtonGetinfo()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    CString cstrPicID;
    GetDlgItem(IDC_EDIT_PICID)->GetWindowText(cstrPicID);
    char szPlateNo[64] = { 0 }, szPicId[128] = { 0 };
    sprintf_s(szPicId, sizeof(szPicId), "%s", cstrPicID.GetBuffer());
    cstrPicID.ReleaseBuffer();
    int iColorInfo = -1;
    BOOL bRet = Plate_GetInfo(szPlateNo, &iColorInfo, szPicId);

    char chLog[256] = { 0 };
    sprintf_s(chLog, sizeof(chLog), "Plate_GetInfo,  szPlateNo = %s, iColorInfo = %d, szPicId= %s, return value = %d", szPlateNo, iColorInfo, szPicId, bRet);
    //MessageBox(chLog);
    ShowMessage(chLog);
}


void Ctest_PLATERECOGNIZEDlg::OnBnClickedButtonGetimage()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    char szPlateNo[64] = { 0 }, szPicId[128] = { 0 }, szOverlayInfo[512] = {0};
    CString cstrImgType, cstrOverlayInfo, cstrPicID;
    GetDlgItem(IDC_EDIT_IMGTYPE)->GetWindowText(cstrImgType);
    GetDlgItem(IDC_EDIT_OVERLAY)->GetWindowText(cstrOverlayInfo);
    GetDlgItem(IDC_EDIT_PICID)->GetWindowText(cstrPicID);
    int iImgType = atoi(cstrImgType.GetBuffer());
    cstrImgType.ReleaseBuffer();
    sprintf_s(szOverlayInfo, sizeof(szOverlayInfo), "%s", cstrOverlayInfo.GetBuffer());
    cstrOverlayInfo.ReleaseBuffer();
    sprintf_s(szPicId, sizeof(szPicId), "%s", cstrPicID.GetBuffer());
    cstrPicID.ReleaseBuffer();

    char szImageInfo[256] = {0};
    char szImgType[64] = {0};
    switch (iImgType)
    {
    case BIN_IMG:
        strcpy_s(szImgType,sizeof(szImgType), "bin");
        break;
    case PLATE_IMG:
        strcpy_s(szImgType, sizeof(szImgType), "Plate");
        break;
    case COLOR_VEHICLE_HEAD_IMG:
        strcpy_s(szImgType, sizeof(szImgType), "head");
        break;
    case FULL_IMG:
        strcpy_s(szImgType, sizeof(szImgType), "full");
        break;
    default:
        strcpy_s(szImgType, sizeof(szImgType), "full");
        break;
    }
    sprintf_s(szImageInfo, sizeof(szImageInfo), "%s\\Getimage_%s_%s.jpg", GetCurrentDir(), szPicId, szImgType);

    BOOL bRet = Plate_GetImage(iImgType, szImageInfo, szPicId, szOverlayInfo);

    char chLog[1024] = { 0 };
    sprintf_s(chLog, sizeof(chLog), "Plate_GetImage,  iImgType = %d, szImageInfo = %s, szPicId= %s, szOverlayInfo= %s, return value = %d", iImgType, szImageInfo, szPicId, szOverlayInfo, bRet);
    MessageBox(chLog);
}

char* Ctest_PLATERECOGNIZEDlg::GetCurrentDir()
{
    static char szFileName[MAX_PATH] = {0};
    memset(szFileName, 0, sizeof(szFileName));
    GetModuleFileNameA(NULL, szFileName, MAX_PATH - 1);

    PathRemoveFileSpecA(szFileName);
    return szFileName;
}


void Ctest_PLATERECOGNIZEDlg::OnBnClickedButtonGetstate()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    int iState = -1;
    BOOL bRet = Plate_GetState(&iState);

    char chLog[256] = { 0 };
    sprintf_s(chLog, sizeof(chLog), "Plate_GetState,  iState = %d, return value = %d", iState, bRet);
    //MessageBox(chLog);
    ShowMessage(chLog);
}


void Ctest_PLATERECOGNIZEDlg::OnBnClickedButtonCapture()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    BOOL bRet = Plate_Capture();

    char chLog[256] = { 0 };
    sprintf_s(chLog, sizeof(chLog), "Plate_Capture,  return value = %d", bRet);
    //MessageBox(chLog);
    ShowMessage(chLog);
}


void Ctest_PLATERECOGNIZEDlg::OnBnClickedButtonInitvideo()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    char szErrInfo[256] = {0};
    HWND hVideoHandle = GetDlgItem(IDC_STATIC_video)->GetSafeHwnd();
    BOOL bRet = Plate_InitVideo(hVideoHandle, szErrInfo);

    char chLog[512] = { 0 };
    sprintf_s(chLog, sizeof(chLog), "Plate_InitVideo, handle = %p, szErrInfo= %s,  return value = %d", hVideoHandle, szErrInfo, bRet);
    //MessageBox(chLog);
    ShowMessage(chLog);
}


void Ctest_PLATERECOGNIZEDlg::OnBnClickedButtonClosevideo()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    HWND hVideoHandle = GetDlgItem(IDC_STATIC_video)->GetSafeHwnd();
    BOOL bRet = Plate_CloseVideo(hVideoHandle);

    char chLog[512] = { 0 };
    sprintf_s(chLog, sizeof(chLog), "Plate_CloseVideo, handle = %p, return value = %d", hVideoHandle,  bRet);
    //MessageBox(chLog);
    ShowMessage(chLog);
}


void Ctest_PLATERECOGNIZEDlg::OnBnClickedButtonScreenshot()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    char szFilePath[256] = { 0 };
    sprintf_s(szFilePath, sizeof(szFilePath), "%s\\Screenshot_%lu.jpg",GetCurrentDir(), GetTickCount());
    BOOL bRet = Plate_Screenshot(szFilePath);

    char chLog[512] = { 0 };
    sprintf_s(chLog, sizeof(chLog), "Plate_Screenshot, path= %s, return code = %d", szFilePath, bRet);
    //MessageBox(chLog);
    ShowMessage(chLog);
}


void Ctest_PLATERECOGNIZEDlg::OnBnClickedButtonTiming()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������    
    char szTimeBegin[128] = { 0 }, szTimeAfter[128] = {0};

    time_t t = time(0);
    struct tm pTM;
    localtime_s(&pTM, &t);
    strftime(szTimeBegin, sizeof(szTimeBegin), "%Y-%m-%d %H:%M:%S", &pTM);
    //strftime(szTimeBegin, sizeof(szTimeBegin), "%Y-%m-%d %H:%M:%S", localtime(&t));

    BOOL bRet = Plate_Timing(szTimeBegin, szTimeAfter);

    char chLog[512] = { 0 };
    sprintf_s(chLog, sizeof(chLog), "Plate_Timing, szTimeBegin= %s, return time=%s, return code = %d", szTimeBegin, szTimeAfter, bRet);
    //MessageBox(chLog);
    ShowMessage(chLog);
}


void Ctest_PLATERECOGNIZEDlg::OnBnClickedButtonShowtime()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    BOOL bRet = Plate_ShowTime();

    char chLog[128] = { 0 };
    sprintf_s(chLog, sizeof(chLog), "Plate_ShowTime,  return code = %d", bRet);
    //MessageBox(chLog);
    ShowMessage(chLog);
}


void Ctest_PLATERECOGNIZEDlg::OnBnClickedButtonClosetime()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    BOOL bRet = Plate_CloseTime();

    char chLog[128] = { 0 };
    sprintf_s(chLog, sizeof(chLog), "Plate_CloseTime,  return code = %d", bRet);
    //MessageBox(chLog);
    ShowMessage(chLog);
}


void Ctest_PLATERECOGNIZEDlg::OnBnClickedButtonGetdeviceinfo()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    char szDeviceInfo[256] = { 0 };
    BOOL bRet = Plate_GetDeviceInfo(szDeviceInfo);

    char chLog[256] = { 0 };
    sprintf_s(chLog, sizeof(chLog), "Plate_GetDeviceInfo, szDeviceInfo= %s,  return code = %d", szDeviceInfo, bRet);
    //MessageBox(chLog);
    ShowMessage(chLog);
}

LRESULT Ctest_PLATERECOGNIZEDlg::OnUserResult(WPARAM wParam, LPARAM lParam)
{
    if (!m_bUseMessageModel)
    {
        return 0;
    }
    char chLog[512] = { 0 };
    char szPlateNo[64] = { 0 }, szPicId[128] = { 0 };
    sprintf_s(szPicId, sizeof(szPicId), "%lu",GetTickCount());
    int iColorInfo = -1;
    BOOL bRet = Plate_GetInfo(szPlateNo, &iColorInfo, szPicId);    
    sprintf_s(chLog, sizeof(chLog), "Plate_GetInfo,  szPlateNo = %s, iColorInfo = %d, szPicId= %s, return value = %d", szPlateNo, iColorInfo, szPicId, bRet);
    ShowMessage(chLog);

    char  szOverlayInfo[512] = { 0 };
    CString  cstrOverlayInfo;
    GetDlgItem(IDC_EDIT_OVERLAY)->GetWindowText(cstrOverlayInfo);
    sprintf_s(szOverlayInfo, sizeof(szOverlayInfo), "%s", cstrOverlayInfo.GetBuffer());
    cstrOverlayInfo.ReleaseBuffer();

    int iImgType = 0;
    char szImageInfo[256] = { 0 };
    char szImgType[64] = { 0 };
    for (int i = 0; i < 4;i++)
    {
        iImgType = i;
        memset(szImgType, 0, sizeof(szImgType));
        memset(szImageInfo, 0, sizeof(szImageInfo));
        switch (iImgType)
        {
        case BIN_IMG:
            strcpy_s(szImgType, sizeof(szImgType), "bin");
            break;
        case PLATE_IMG:
            strcpy_s(szImgType, sizeof(szImgType), "Plate");
            break;
        case COLOR_VEHICLE_HEAD_IMG:
            strcpy_s(szImgType, sizeof(szImgType), "head");
            break;
        case FULL_IMG:
            strcpy_s(szImgType, sizeof(szImgType), "full");
            break;
        default:
            strcpy_s(szImgType, sizeof(szImgType), "full");
            break;
        }
        if (bRet)
        {
            sprintf_s(szImageInfo, sizeof(szImageInfo), "%s\\Result\\PicId_%s_%s_PlateNo_%s_color_%d.jpg", GetCurrentDir(), szPicId, szImgType, szPlateNo, iColorInfo);
            bRet = Plate_GetImage(iImgType, szImageInfo, szPicId, szOverlayInfo);
            memset(chLog, 0, sizeof(chLog));
            sprintf_s(chLog, sizeof(chLog), "Plate_GetImage,  iImgType = %d, szImageInfo = %s, szPicId= %s, szOverlayInfo= %s, return value = %d",
                iImgType, szImageInfo, szPicId, szOverlayInfo, bRet);
            ShowMessage(chLog);
        }
    } 
    return 0;
}

void Ctest_PLATERECOGNIZEDlg::ShowMsg(CEdit *pEdit, CString strMsg)
{
    if (pEdit == NULL)
    {
        return;
    }

    CTime  time = CTime::GetCurrentTime();
    CString strTmp;
    pEdit->GetWindowText(strTmp);
    if (strTmp.IsEmpty() || strTmp.GetLength() > 4096)
    {
        strTmp = time.Format(_T("[%Y-%m-%d %H:%M:%S] "));
        strTmp += strMsg;
        pEdit->SetWindowText(strTmp);
        return;
    }

    strTmp += _T("\r\n");
    strTmp += time.Format(_T("[%Y-%m-%d %H:%M:%S] "));
    strTmp += strMsg;
    pEdit->SetWindowText(strTmp);

    if (pEdit != NULL)
    {
        pEdit->LineScroll(pEdit->GetLineCount() - 1);
    }
}

void Ctest_PLATERECOGNIZEDlg::ShowMessage(CString strMsg)
{
    CEdit *pEdit = (CEdit*)GetDlgItem(IDC_EDIT_Show);
    ShowMsg(pEdit, strMsg);
}


void Ctest_PLATERECOGNIZEDlg::OnBnClickedButtonUsemsg()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    CString  cstrButtonText;
    GetDlgItem(IDC_BUTTON_USEmsg)->GetWindowText(cstrButtonText);
    if (-1 != cstrButtonText.Find("����"))
    {
        m_bUseMessageModel = true;
        GetDlgItem(IDC_BUTTON_USEmsg)->SetWindowTextA("ͣ����Ϣģʽ");
    }
    else
    {
        m_bUseMessageModel = false;
        GetDlgItem(IDC_BUTTON_USEmsg)->SetWindowTextA("������Ϣģʽ");
    }
}
