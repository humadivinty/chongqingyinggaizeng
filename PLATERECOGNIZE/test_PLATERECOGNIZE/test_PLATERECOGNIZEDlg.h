
// test_PLATERECOGNIZEDlg.h : ͷ�ļ�
//

#pragma once

#define WM_MYMSG WM_USER+8

// Ctest_PLATERECOGNIZEDlg �Ի���
class Ctest_PLATERECOGNIZEDlg : public CDialogEx
{
// ����
public:
	Ctest_PLATERECOGNIZEDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TEST_PLATERECOGNIZE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
    @brief   �ڱ༭��ؼ���ʾ��Ϣ
    @param[in]        CEdit * pEdit     �༭��ؼ�ָ��
    @param[in]        CString strMsg    Ҫ��ʾ����Ϣ
    @return           void
    */
    void ShowMsg(CEdit *pEdit, CString strMsg);
    void ShowMessage(CString strMsg);

    afx_msg void OnBnClickedButtonUsemsg();
private:
    bool m_bUseMessageModel;
};
