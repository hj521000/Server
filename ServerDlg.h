
// ServerDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"

UINT thread(LPVOID);

// CServerDlg 对话框
class CServerDlg : public CDialogEx
{
// 构造
public:
	CListCtrl m_list;
	SOCKET sock, msgsock[50], clisock;
	int addlen;
	sockaddr_in serv;
	int count;
	int getcount();
	void sendtoall(SOCKET, char *);
	CServerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SERVER_DIALOG };

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
	//CListCtrl m_list;
	CString m_edit;
	afx_msg void OnBnClickedButton1();
};
