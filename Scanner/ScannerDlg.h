
// ScannerDlg.h : 头文件
//

#pragma once
#include "afxwin.h"


// CScannerDlg 对话框
class CScannerDlg : public CDialogEx
{
// 构造
public:
	CScannerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SCANNER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	virtual BOOL PreTranslateMessage(MSG* pMsg);

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

public:
	CButton m_buttonStart;
	CStatic m_staticInfo;
	afx_msg void OnClose();
};
