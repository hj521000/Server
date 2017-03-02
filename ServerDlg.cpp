
// ServerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Server.h"
#include "ServerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CServerDlg 对话框



CServerDlg::CServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CServerDlg::IDD, pParent)
	, m_edit(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Text(pDX, IDC_EDIT1, m_edit);
}

BEGIN_MESSAGE_MAP(CServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CServerDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CServerDlg 消息处理程序

BOOL CServerDlg::OnInitDialog()
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	count = 0;
	m_list.InsertColumn(0, _T("消息"));
	m_list.SetColumnWidth(0, 435);
	for (int i = 0; i < 50; i++)
		msgsock[i] = NULL;
	//设定地址
	serv.sin_addr.s_addr = htonl(INADDR_ANY);
	serv.sin_family = AF_INET;
	serv.sin_port = 5000;
	addlen = sizeof(serv);
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (bind(sock, (sockaddr *)&serv, addlen))
		m_edit = _T("绑定错误");
	else
	{
		m_edit = _T("快乐十分投注开始");
		listen(sock, 5);
		AfxBeginThread(&thread, 0);
	}
	UpdateData(false);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CServerDlg::OnPaint()
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
HCURSOR CServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CServerDlg::OnBnClickedButton1()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
	char buff[200];
	m_list.InsertItem(count++, m_edit);
	CSize size;
	size.cx = 0;
	size.cy = 30;
	m_list.Scroll(size);
	//CString to char
	DWORD len = ::WideCharToMultiByte(CP_OEMCP, NULL, m_edit, -1, NULL, 0, NULL, FALSE);
	::WideCharToMultiByte(CP_OEMCP, NULL, m_edit, -1, buff, len, NULL, FALSE);
	for (int i = 0; i < 50; i++)
	{
		if (msgsock[i] != NULL)
			send(msgsock[i], buff, 100, 0);
	}
	m_edit = _T("");
	UpdateData(false);
}

UINT thread(LPVOID p)
{
	char buff[100];
	CSize size;
	size.cx = 0;
	size.cy = 30;
	int s = 1, msgcount, loop = 1, flag = 0;
	CServerDlg * dlg = (CServerDlg *)AfxGetApp()->GetMainWnd();
	msgcount = dlg->getcount();
	if (msgcount == -1)
		loop = 0;
	if (loop)
	{
		s = 1;
		dlg->msgsock[msgcount] = accept(dlg->sock, (sockaddr *)&(dlg->serv), &(dlg->addlen));
		if (dlg->msgsock[msgcount] == INVALID_SOCKET)
			dlg->m_edit = _T("连接错误");
		else
		{
			AfxBeginThread(thread, 0);
			dlg->m_list.InsertItem(dlg->count++, _T("连接成功"));
			dlg->m_list.Scroll(size);
			while (s != SOCKET_ERROR)
			{
				s = recv(dlg->msgsock[msgcount], buff, 100, 0);
				//char to CString
				CString temp;
				DWORD dwNum = ::MultiByteToWideChar(CP_ACP, 0, buff, -1, NULL, 0);
				wchar_t * pwText = new wchar_t[dwNum];
				MultiByteToWideChar(CP_ACP, 0, buff, -1, pwText, dwNum);
				temp.Format(_T("%s"), pwText);
				delete[]pwText;
				if (s != SOCKET_ERROR)
				{
					dlg->m_list.InsertItem(dlg->count++, temp);
					dlg->m_list.Scroll(size);
					dlg->sendtoall(dlg->msgsock[msgcount], buff);
				}
			}
			send(dlg->msgsock[msgcount], "断开连接", 100, 0);
			dlg->m_list.InsertItem(dlg->count++, _T("断开连接"));
			dlg->m_list.Scroll(size);
			dlg->msgsock[msgcount] = NULL;
			for (int i = 0; i < 50; i++)
			if (dlg->msgsock[i] != NULL) flag = 1;
			closesocket(dlg->msgsock[msgcount]);
		}
	}
	//终止线程
	AfxEndThread(0);
	return 0;
}


int CServerDlg::getcount()
{
	for (int i = 0; i < 50; i++)
	if (msgsock[i] == NULL)
		return i;
	return -1;
}


void CServerDlg::sendtoall(SOCKET s, char * buff)
{
	for (int i = 0; i < 50; i++)
	{
		if (msgsock[i] != NULL && msgsock[i] != s)
			send(msgsock[i], buff, 100, 0);
	}
}
