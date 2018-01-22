// MTV4ConsoleDlg.h : header file
//



#pragma once

#include "MTrackerCommands.h"
#include "WinCom.h"
#include <iostream>
#include <fstream>
#include "controltask.hpp"


using namespace std;

// CMTV4ConsoleDlg dialog
class CMTV4ConsoleDlg : public CDialog
{
// Construction
public:
	CMTV4ConsoleDlg(CWnd* pParent = NULL);	// standard constructor

	afx_msg void OnClose();

	double m_VL,m_VR,X,Y,Th;
	int m_Start,robot_nr,recv_count,send_count,semRS_recv,New_Frame;
	CString str;


	ControlTask cntrlTask;


// Dialog Data
	enum { IDD = IDD_MTV4CONSOLE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedSend();
public:
	afx_msg void OnBnClickedStart();
public:
	int m_Combo_R_nr,m_Combo_R_GR_nr;
public:
	afx_msg void OnCbnSelchangeComboRobotNr();

	
public:
	afx_msg void OnCbnSelchangeComboRobotGroupNr();
public:
	afx_msg void OnBnClickedStop();
};
