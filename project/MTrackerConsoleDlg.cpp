#include "stdafx.h"
#include "MTrackerConsole.h"
#include "MTrackerConsoleDlg.h"
#include <process.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


CMTV4ConsoleDlg::CMTV4ConsoleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMTV4ConsoleDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

}

void CMTV4ConsoleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMTV4ConsoleDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDCANCEL, &CMTV4ConsoleDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CMTV4ConsoleDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_SEND, &CMTV4ConsoleDlg::OnBnClickedSend)
	ON_BN_CLICKED(IDC_START, &CMTV4ConsoleDlg::OnBnClickedStart)
	ON_BN_CLICKED(IDC_STOP, &CMTV4ConsoleDlg::OnBnClickedStop)
END_MESSAGE_MAP()


// CMTV4ConsoleDlg message handlers




BOOL CMTV4ConsoleDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon


	// TODO: Add extra initialization here
	m_VL=m_VR=X=Y=Th=0; New_Frame=0;
	recv_count = 0;
	send_count = 0;

	m_Start = 0;
	str.Format("Control stopped"); SetDlgItemText(IDC_STATIC_INFO,str); str.Empty();
	str.Format(" X = ?   Y = ?  Th = ?"); SetDlgItemText(IDC_STATIC1,str); str.Empty();
	str.Format(" WR = ? WL = ? \n Send/receive frames: "); SetDlgItemText(IDC_STATIC,str); str.Empty();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMTV4ConsoleDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMTV4ConsoleDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMTV4ConsoleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMTV4ConsoleDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	//OnCancel();
}

void CMTV4ConsoleDlg::OnBnClickedOk()
{
	OnBnClickedStop();
	Sleep(50); 
	OnOK();
}

void CMTV4ConsoleDlg::OnClose()
{
	m_Start = 0; Sleep(50); 
	CDialog::OnClose(); CDialog::OnOK();
}

void CMTV4ConsoleDlg::OnBnClickedSend()
{
	UpdateData(TRUE);
	UpdateData(FALSE);
}

void __cdecl Thread_Control(void* pParam)
{
	auto *pthis = reinterpret_cast<CMTV4ConsoleDlg*>(pParam);
	
	static LARGE_INTEGER fr,st,end;
	double dtime; int loopCounter = 1; int ansCounter = 1;

	auto b = QueryPerformanceFrequency(&fr);
	b = QueryPerformanceCounter(&st);

	while(pthis->m_Start==1)
	{	
		b = QueryPerformanceCounter(&end);

		pthis->cntrlTask.Run();  pthis->send_count++;
		if (pthis->cntrlTask.receivedStatus > 0) 
		{
			pthis->recv_count++;
			ansCounter++;
			
			if(!(ansCounter%5))
			{
				pthis->str.Format(" X = %2.3f   Y = %2.3f   Th = %2.3f  ",pthis->cntrlTask.mtracker.state.x,pthis->cntrlTask.mtracker.state.y,pthis->cntrlTask.mtracker.state.th); pthis->SetDlgItemText(IDC_STATIC1,pthis->str);pthis->str.Empty();
				pthis->str.Format(" WR = %2.2f WL = %2.2f\n Send/receive frames:  %d/%d ",pthis->cntrlTask.mtracker.state.wr,pthis->cntrlTask.mtracker.state.wl,pthis->send_count,pthis->recv_count); pthis->SetDlgItemText(IDC_STATIC,pthis->str);pthis->str.Empty();
			}
		}
		do
		{	
			dtime =1000*(double(end.QuadPart)-double(st.QuadPart))/(double(fr.QuadPart));
			b = QueryPerformanceCounter(&end);
		} 
		while(dtime<=20.0*loopCounter);

		if (!(ansCounter%5))
		{
			pthis->str.Format("Period: %2.2f",dtime/loopCounter);pthis->SetDlgItemText(IDC_STATIC_INFO,pthis->str);
			pthis->str.Empty();
		}
		loopCounter++; 
		
	} 
	_endthread();
}

void CMTV4ConsoleDlg::OnBnClickedStart()
{
	// TODO: Add your control notification handler code here
	if (m_Start==0)
	{ 
		if (cntrlTask.OpenCom()==0)
		{
			m_Start=1;
			cntrlTask.StartRecording(); Sleep(100);
			_beginthread(Thread_Control, 0, this);
		}
		else 
		{
			MessageBox("COM port cannot be opened!");
		}
    }
	
	str.Format("Control started"); SetDlgItemText(IDC_STATIC_INFO,str);str.Empty();
}

void CMTV4ConsoleDlg::OnBnClickedStop()
{
	// TODO: Add your control notification handler code here
	if (m_Start==1)
	{
		m_Start = 0; Sleep(100); m_VL=0; m_VR=0;
		cntrlTask.StopRecording();
		cntrlTask.mtracker.SetWheelsVelocities(robot_nr,m_VL,m_VR);
		UpdateData(TRUE);
		cntrlTask.Send();
		Sleep(100);
		cntrlTask.Stop();
		cntrlTask.CloseCom();
		UpdateData(FALSE);
		str.Format(" WL  WR:  %2.1f   %2.1f\n Send/receive frames:  %d/%d ",m_VL,m_VR,send_count,recv_count); SetDlgItemText(IDC_STATIC,str);str.Empty();
		str.Format("Control stopped"); SetDlgItemText(IDC_STATIC_INFO,str);str.Empty();
	}
}

void CMTV4ConsoleDlg::OnCbnSelchangeComboRobotNr()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	robot_nr=m_Combo_R_nr;

}

void CMTV4ConsoleDlg::OnCbnSelchangeComboRobotGroupNr()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	//commands.set_group_address(m_Combo_R_GR_nr);
}


