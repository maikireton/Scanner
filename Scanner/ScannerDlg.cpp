
// ScannerDlg.cpp : ʵ���ļ�
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


// CScannerDlg �Ի���



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


// CScannerDlg ��Ϣ�������

BOOL CScannerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	SetTimer(1, 1000, NULL);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CScannerDlg::OnPaint()
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
		m_buttonStart.SetWindowText("ֹͣ");
	}
	else
	{
		gGlobal->getTasks()->stopAll();
		gGlobal->setState(Run_State_Stopping);
		m_buttonStart.SetWindowText("ֹͣ��");
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
				m_buttonStart.SetWindowText("����");
				m_buttonStart.EnableWindow(TRUE);
			}
		}
		CString info;
		info.Format("������:%d ɨ:%d ɨ��:%d δ:%d", 
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
		AfxMessageBox("����ֹͣ");
		return;
	}
	CDialogEx::OnClose();
}
