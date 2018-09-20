
// testMqttDlg.h : ͷ�ļ�
//

#pragma once
#include "mqtt/include/MQTTClient.h"
#include <queue>

// CtestMqttDlg �Ի���
class CtestMqttDlg : public CDialogEx
{
// ����
public:
	CtestMqttDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CtestMqttDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TESTMQTT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	CRITICAL_SECTION m_Lock;
	std::queue<CString> m_queMsg;
protected:
	MQTTClient client;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedBtnConnect();
	afx_msg void OnBnClickedBtnSubTheme();
	afx_msg void OnBnClickedBtnUnsubTheme();
	afx_msg void OnBnClickedBtnSend();
	afx_msg LRESULT OnMsgArrive(WPARAM wParam, LPARAM lParam);
public:
	void PushMsg(CString strMsg);
	CString PopMsg();
	afx_msg void OnBnClickedBtnClearSend();
};
