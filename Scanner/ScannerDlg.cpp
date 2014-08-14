
// ScannerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Scanner.h"
#include "ScannerDlg.h"
#include "afxdialogex.h"
#include "Config.h"
#include "Task.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CScannerDlg 对话框



CScannerDlg::CScannerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CScannerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CScannerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_START, m_buttonStart);
	DDX_Control(pDX, IDC_STATIC_INFO, m_staticInfo);
}

BEGIN_MESSAGE_MAP(CScannerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_START, &CScannerDlg::OnBnClickedButtonStart)
	ON_WM_TIMER()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CScannerDlg 消息处理程序

BOOL CScannerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	SetTimer(1, 1000, NULL);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CScannerDlg::OnPaint()
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
HCURSOR CScannerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CScannerDlg::OnBnClickedButtonStart()
{
	if (gGlobal->getState() == Run_State_Free)
	{
		for (int i=0; i<gGlobal->getConfig()->getThreadNum(); i++)
		{
			Task* a = new Task();
			a->start();
		}
		gGlobal->setState(Run_State_Running);
		m_buttonStart.SetWindowText("停止");
	}
	else
	{
		gGlobal->getTasks()->stopAll();
		gGlobal->setState(Run_State_Stopping);
		m_buttonStart.SetWindowText("停止中");
		m_buttonStart.EnableWindow(FALSE);
	}
}

void CScannerDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1)
	{
		if (gGlobal->getTasks()->isEmpty())
		{
			if (gGlobal->getState() != Run_State_Free)
			{
				gGlobal->setState(Run_State_Free);
				m_buttonStart.SetWindowText("启动");
				m_buttonStart.EnableWindow(TRUE);
			}
		}
		CString info;
		info.Format("任务数:%d 扫:%d 扫中:%d 未:%d", 
			gGlobal->getTasks()->size(), 
			gGlobal->getAccount()->getAccountStateNum(Account_State_Scanned),
			gGlobal->getAccount()->getAccountStateNum(Account_State_Scanning),
			gGlobal->getAccount()->getAccountStateNum(Account_State_Init));
		m_staticInfo.SetWindowText(info);
	}
	CDialogEx::OnTimer(nIDEvent);
}

BOOL CScannerDlg::PreTranslateMessage(MSG* pMsg)  
{  
    if (pMsg->message == WM_KEYDOWN)  
    {  
        switch(pMsg->wParam)  
        {  
            case VK_ESCAPE:
                return true;  
            case VK_RETURN:
                return true;  
            default:  
                ;  
        }  
    }  

    return CDialogEx::PreTranslateMessage(pMsg);  
}  

void CScannerDlg::OnClose()
{
	if (gGlobal->getState() != Run_State_Free)
	{
		AfxMessageBox("请先停止");
		return;
	}
	CDialogEx::OnClose();
}
