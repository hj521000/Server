
// ServerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Server.h"
#include "ServerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CServerDlg �Ի���



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


// CServerDlg ��Ϣ�������

BOOL CServerDlg::OnInitDialog()
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
	count = 0;
	m_list.InsertColumn(0, _T("��Ϣ"));
	m_list.SetColumnWidth(0, 435);
	for (int i = 0; i < 50; i++)
		msgsock[i] = NULL;
	//�趨��ַ
	serv.sin_addr.s_addr = htonl(INADDR_ANY);
	serv.sin_family = AF_INET;
	serv.sin_port = 5000;
	addlen = sizeof(serv);
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (bind(sock, (sockaddr *)&serv, addlen))
		m_edit = _T("�󶨴���");
	else
	{
		m_edit = _T("����ʮ��Ͷע��ʼ");
		listen(sock, 5);
		AfxBeginThread(&thread, 0);
	}
	UpdateData(false);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CServerDlg::OnPaint()
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
HCURSOR CServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CServerDlg::OnBnClickedButton1()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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
			dlg->m_edit = _T("���Ӵ���");
		else
		{
			AfxBeginThread(thread, 0);
			dlg->m_list.InsertItem(dlg->count++, _T("���ӳɹ�"));
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
			send(dlg->msgsock[msgcount], "�Ͽ�����", 100, 0);
			dlg->m_list.InsertItem(dlg->count++, _T("�Ͽ�����"));
			dlg->m_list.Scroll(size);
			dlg->msgsock[msgcount] = NULL;
			for (int i = 0; i < 50; i++)
			if (dlg->msgsock[i] != NULL) flag = 1;
			closesocket(dlg->msgsock[msgcount]);
		}
	}
	//��ֹ�߳�
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
