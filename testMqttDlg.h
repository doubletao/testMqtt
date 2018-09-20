
// testMqttDlg.h : 头文件
//

#pragma once
#include "mqtt/include/MQTTClient.h"
#include <queue>

// CtestMqttDlg 对话框
class CtestMqttDlg : public CDialogEx
{
// 构造
public:
	CtestMqttDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CtestMqttDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TESTMQTT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	CRITICAL_SECTION m_Lock;
	std::queue<CString> m_queMsg;
protected:
	MQTTClient client;

	// 生成的消息映射函数
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
