
// ServerDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"

UINT thread(LPVOID);

// CServerDlg �Ի���
class CServerDlg : public CDialogEx
{
// ����
public:
	CListCtrl m_list;
	SOCKET sock, msgsock[50], clisock;
	int addlen;
	sockaddr_in serv;
	int count;
	int getcount();
	void sendtoall(SOCKET, char *);
	CServerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SERVER_DIALOG };

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
	//CListCtrl m_list;
	CString m_edit;
	afx_msg void OnBnClickedButton1();
};
