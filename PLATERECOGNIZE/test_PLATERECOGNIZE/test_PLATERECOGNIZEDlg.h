
// test_PLATERECOGNIZEDlg.h : 头文件
//

#pragma once

#define WM_MYMSG WM_USER+8

// Ctest_PLATERECOGNIZEDlg 对话框
class Ctest_PLATERECOGNIZEDlg : public CDialogEx
{
// 构造
public:
	Ctest_PLATERECOGNIZEDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TEST_PLATERECOGNIZE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedButtonInitdevice();
    afx_msg void OnBnClickedButtonClosedevice();
    afx_msg void OnBnClickedButtonGetinfo();
    afx_msg void OnBnClickedButtonGetimage();

private:
    char* GetCurrentDir();
public:
    afx_msg void OnBnClickedButtonGetstate();
    afx_msg void OnBnClickedButtonCapture();
    afx_msg void OnBnClickedButtonInitvideo();
    afx_msg void OnBnClickedButtonClosevideo();
    afx_msg void OnBnClickedButtonScreenshot();
    afx_msg void OnBnClickedButtonTiming();
    afx_msg void OnBnClickedButtonShowtime();
    afx_msg void OnBnClickedButtonClosetime();
    afx_msg void OnBnClickedButtonGetdeviceinfo();


    afx_msg LRESULT OnUserResult(WPARAM wParam, LPARAM lParam);

    /**
    @name    ShowMsg
    @brief   在编辑框控件显示信息
    @param[in]        CEdit * pEdit     编辑框控件指针
    @param[in]        CString strMsg    要显示的信息
    @return           void
    */
    void ShowMsg(CEdit *pEdit, CString strMsg);
    void ShowMessage(CString strMsg);

    afx_msg void OnBnClickedButtonUsemsg();
private:
    bool m_bUseMessageModel;
};
