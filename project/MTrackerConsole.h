//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CMTV4ConsoleApp:
// See MTV4Console.cpp for the implementation of this class
//

class CMTV4ConsoleApp : public CWinApp
{
public:
	CMTV4ConsoleApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CMTV4ConsoleApp theApp;