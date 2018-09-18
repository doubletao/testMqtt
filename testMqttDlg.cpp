
// testMqttDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "testMqtt.h"
#include "testMqttDlg.h"
#include "afxdialogex.h"
#include <string>
#pragma comment(lib,"mqtt/lib/paho-mqtt3a.lib")
#pragma comment(lib,"mqtt/lib/paho-mqtt3c.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const int WM_USER_MESSAGE_MSG_ARRIVE = WM_USER + 1;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

std::string ConverCStringToStdString(CString cstrValue)
{
	std::string strRet;
	strRet.clear();
	int nSize = ::WideCharToMultiByte(CP_UTF8, 0, cstrValue.GetBuffer(), -1, NULL, 0, NULL, NULL);
	char * szcSrc = new char[nSize];
	memset(szcSrc, 0, sizeof(char) * nSize);
	::WideCharToMultiByte(CP_UTF8, 0, cstrValue.GetBuffer(), -1, szcSrc, nSize, NULL, NULL);
	strRet = szcSrc;
	delete[] szcSrc;
	szcSrc = NULL;
	return strRet;

}

std::string ConverCStringToStdString(CString cstrValue, DWORD dwFormatW)
{
	std::string strRet;
	strRet.clear();
	int nSize = ::WideCharToMultiByte(dwFormatW, 0, cstrValue.GetBuffer(), -1, NULL, 0, NULL, NULL);
	char * szcSrc = new char[nSize];
	memset(szcSrc, 0, sizeof(char) * nSize);
	::WideCharToMultiByte(dwFormatW, 0, cstrValue.GetBuffer(), -1, szcSrc, nSize, NULL, NULL);
	strRet = szcSrc;
	delete[] szcSrc;
	szcSrc = NULL;
	return strRet;
}

CString ConvertStdStringToCString(const std::string & strValue)
{
	CString cstrRet;
	cstrRet.Empty();
	int nLength = ::MultiByteToWideChar(CP_UTF8, 0, strValue.c_str(), -1, NULL, 0);
	TCHAR * szText1 = new TCHAR[nLength + 1];
	::MultiByteToWideChar(CP_UTF8, 0, strValue.c_str(), -1, szText1, nLength + 1);
	cstrRet = szText1;
	delete[] szText1;
	szText1 = NULL;
	return cstrRet;
}


class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CtestMqttDlg 对话框



CtestMqttDlg::CtestMqttDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TESTMQTT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	InitializeCriticalSection(&m_Lock);
}

CtestMqttDlg::~CtestMqttDlg()
{
	DeleteCriticalSection(&m_Lock);
}

void CtestMqttDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CtestMqttDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_CONNECT, &CtestMqttDlg::OnBnClickedBtnConnect)
	ON_BN_CLICKED(IDC_BTN_SUB_THEME, &CtestMqttDlg::OnBnClickedBtnSubTheme)
	ON_BN_CLICKED(IDC_BTN_UNSUB_THEME, &CtestMqttDlg::OnBnClickedBtnUnsubTheme)
	ON_BN_CLICKED(IDC_BTN_SEND, &CtestMqttDlg::OnBnClickedBtnSend)
	ON_MESSAGE(WM_USER_MESSAGE_MSG_ARRIVE, &CtestMqttDlg::OnMsgArrive)
END_MESSAGE_MAP()


// CtestMqttDlg 消息处理程序
#define ADDRESS     "tcp://localhost:1883"
#define CLIENTID    "ExampleClientPub"
#define TOPIC       "MQTT Examples"
#define PAYLOAD     "Hello World!"
#define QOS         1
#define TIMEOUT     10000L

volatile MQTTClient_deliveryToken deliveredtoken;
void delivered(void *context, MQTTClient_deliveryToken dt)
{
	printf("Message with token value %d delivery confirmed\n", dt);
	deliveredtoken = dt;
}
int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
	char * payloadptr = new char[message->payloadlen + 1];
	memset(payloadptr, 0, message->payloadlen + 1);
	printf("Message arrived\n");
	printf("     topic: %s\n", topicName);
	printf("   message: ");
	memcpy(payloadptr, message->payload, message->payloadlen);
	CtestMqttDlg * pThis = (CtestMqttDlg *)context;
	if (pThis)
	{
		pThis->PushMsg(ConvertStdStringToCString(payloadptr));
	}
	MQTTClient_freeMessage(&message);
	MQTTClient_free(topicName);
	return 1;
}
void connlost(void *context, char *cause)
{
	printf("\nConnection lost\n");
	printf("     cause: %s\n", cause);
}

BOOL CtestMqttDlg::OnInitDialog()
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

	GetDlgItem(IDC_EDIT_SERVER_ADDR)->SetWindowText(_T("tcp://192.168.100.69:1883"));
	GetDlgItem(IDC_EDIT_CLIENT_ID)->SetWindowText(_T("client_num"));
	GetDlgItem(IDC_EDIT_SUB_THEME)->SetWindowText(_T("theme"));
	GetDlgItem(IDC_EDIT_UNSUB_THEME)->SetWindowText(_T("theme"));
	GetDlgItem(IDC_EDIT_SEND_THEME)->SetWindowText(_T("theme"));
	GetDlgItem(IDC_EDIT_SEND_CONTENT)->SetWindowText(_T("content"));
	((CEdit *)GetDlgItem(IDC_EDIT_SEND_CONTENT))->SetLimitText(UINT_MAX);
	((CEdit *)GetDlgItem(IDC_EDIT_RECV_CONTENT))->SetLimitText(UINT_MAX);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CtestMqttDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CtestMqttDlg::OnPaint()
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
HCURSOR CtestMqttDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CtestMqttDlg::OnDestroy()
{
	if (client)
	{
		MQTTClient_disconnect(client, 10000);
		MQTTClient_destroy(&client);
	}
}

void CtestMqttDlg::OnBnClickedBtnConnect()
{
	if (client)
	{
		MQTTClient_disconnect(client, 10000);
		MQTTClient_destroy(&client);
	}
	CString strServerAddr;
	GetDlgItem(IDC_EDIT_SERVER_ADDR)->GetWindowText(strServerAddr);
	CString strClientID;
	GetDlgItem(IDC_EDIT_CLIENT_ID)->GetWindowText(strClientID);
	MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
	int rc;
	MQTTClient_create(&client, ConverCStringToStdString(strServerAddr).c_str(), ConverCStringToStdString(strClientID).c_str(), MQTTCLIENT_PERSISTENCE_NONE, NULL);
	conn_opts.keepAliveInterval = 3;
	conn_opts.cleansession = 1;
	MQTTClient_setCallbacks(client, this, connlost, msgarrvd, delivered);
	if ((rc = MQTTClient_connect(client, &conn_opts)) == MQTTCLIENT_SUCCESS)
	{
		AfxMessageBox(_T("连接成功"));
	}
}

void CtestMqttDlg::OnBnClickedBtnSubTheme()
{
	if (client)
	{
		CString strTheme;
		GetDlgItem(IDC_EDIT_SUB_THEME)->GetWindowText(strTheme);
		MQTTClient_subscribe(client, ConverCStringToStdString(strTheme).c_str(), QOS);
		AfxMessageBox(_T("已订阅"));
	}
}

void CtestMqttDlg::OnBnClickedBtnUnsubTheme()
{
	if (client)
	{
		CString strTheme;
		GetDlgItem(IDC_EDIT_SUB_THEME)->GetWindowText(strTheme);
		MQTTClient_unsubscribe(client, ConverCStringToStdString(strTheme).c_str());
	}
}

void CtestMqttDlg::OnBnClickedBtnSend()
{
	if (client)
	{
		CString strTheme;
		GetDlgItem(IDC_EDIT_SEND_THEME)->GetWindowText(strTheme);
		CString strContent;
		GetDlgItem(IDC_EDIT_SEND_CONTENT)->GetWindowText(strContent);

		std::string cstrContent = ConverCStringToStdString(strContent).c_str();

		MQTTClient_message pubmsg = MQTTClient_message_initializer;
		MQTTClient_deliveryToken token;
		pubmsg.payload = (void *)cstrContent.c_str();
		pubmsg.payloadlen = static_cast<int>(strlen(cstrContent.c_str()));
		pubmsg.qos = QOS;
		pubmsg.retained = 0;
		MQTTClient_publishMessage(client, ConverCStringToStdString(strTheme).c_str(), &pubmsg, &token);
	}
}

LRESULT CtestMqttDlg::OnMsgArrive(WPARAM wParam, LPARAM lParam)
{
	CString strContent = PopMsg();
	CEdit * pEdit = (CEdit *)GetDlgItem(IDC_EDIT_RECV_CONTENT);
	pEdit->SetSel(-1, 0);
	pEdit->ReplaceSel(strContent + _T("\r\n"));
	return 0;
}

void CtestMqttDlg::PushMsg(CString strMsg)
{
	EnterCriticalSection(&m_Lock);
	m_queMsg.push(strMsg);
	LeaveCriticalSection(&m_Lock);
	PostMessage(WM_USER_MESSAGE_MSG_ARRIVE);
}

CString CtestMqttDlg::PopMsg()
{
	CString strRet;
	EnterCriticalSection(&m_Lock);
	strRet = m_queMsg.front();
	m_queMsg.pop();
	LeaveCriticalSection(&m_Lock);
	return strRet;
}
