/* Copyright (C) 2024-2025 Stefan-Mihai MOGA
This file is part of IntelliEdit application developed by Stefan-Mihai MOGA.
IntelliEdit is an alternative Windows version to the famous Microsoft Notepad!

IntelliEdit is free software: you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the Open
Source Initiative, either version 3 of the License, or any later version.

IntelliEdit is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
IntelliEdit. If not, see <http://www.opensource.org/licenses/gpl-3.0.html>*/

// IntelliEdit.cpp : Defines the class behaviors for the application.
//

#include "pch.h"
#include "framework.h"
#include "IntelliEdit.h"
#include "MainFrame.h"

#include "ChildFrame.h"
#include "IntelliEditDoc.h"
#include "IntelliEditView.h"

#include "VersionInfo.h"
#include "HLinkCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CIntelliEditApp

BEGIN_MESSAGE_MAP(CIntelliEditApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CIntelliEditApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()

// CIntelliEditApp construction

CIntelliEditApp::CIntelliEditApp() noexcept : m_pInstanceChecker(_T("IntelliEdit"))
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// If the application is built using Common Language Runtime support (/clr):
	//     1) This additional setting is needed for Restart Manager support to work properly.
	//     2) In your project, you must add a reference to System.Windows.Forms in order to build.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: replace application ID string below with unique ID string; recommended
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("IntelliEdit.AppID.NoVersion"));

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_pCreateLexer = nullptr;
	m_hScintilla = nullptr;
	m_hLexilla = nullptr;
}

// The one and only CIntelliEditApp object

CIntelliEditApp theApp;

// CIntelliEditApp initialization

BOOL CIntelliEditApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction();

	// AfxInitRichEdit2() is required to use RichEdit control
	AfxInitRichEdit2();

	// Check for the previous instance as soon as possible
	if (m_pInstanceChecker.PreviousInstanceRunning())
	{
		CCommandLineInfo cmdInfo;
		ParseCommandLine(cmdInfo);

		AfxMessageBox(_T("Previous version detected, will now restore it..."), MB_OK | MB_ICONINFORMATION);
		m_pInstanceChecker.ActivatePreviousInstance(cmdInfo.m_strFileName);
		return FALSE;
	}

	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// Load the Scintilla \ Lexilla dlls
	CString sLexillaDLL;
	sLexillaDLL.Format(_T("%s%s"), _T(LEXILLA_LIB), _T(LEXILLA_EXTENSION));
	m_hLexilla = LoadLibraryFromApplicationDirectory(sLexillaDLL);
	if (m_hLexilla == nullptr)
	{
		CString sMsg;
		sMsg.Format(_T("%s is not installed, Please built the Scintilla '%s' and copy it into this application's directory"), sLexillaDLL.GetString(), sLexillaDLL.GetString());
		AfxMessageBox(sMsg);
		return FALSE;
	}
	m_hScintilla = LoadLibraryFromApplicationDirectory(_T("Scintilla.dll"));
	if (m_hScintilla == nullptr)
	{
		AfxMessageBox(_T("Scintilla DLL is not installed, Please built the Scintilla 'Scintilla.dll' and copy it into this application's directory"));
		return FALSE;
	}

	// Create the C++ Lexer from Lexilla
#pragma warning(suppress: 26490)
	m_pCreateLexer = reinterpret_cast<Lexilla::CreateLexerFn>(GetProcAddress(m_hLexilla, LEXILLA_CREATELEXER));
	if (m_pCreateLexer == nullptr)
	{
		AfxMessageBox(_T("Could not find the Lexilla CreateLexer function"));
		return FALSE;
	}

	SetRegistryKey(_T("Mihai Moga"));

	LoadStdProfileSettings(10); //Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_IntelliEditTYPE,
		RUNTIME_CLASS(CIntelliEditDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CIntelliEditView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;

	// call DragAcceptFiles only if there's a suffix
	// In an MDI app, this should occur immediately after setting m_pMainWnd
	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// The main window has been initialized, so show and update it
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();
	// pMainFrame->MoveWindow(CRect(0, 0, 1214, 907));
	// pMainFrame->CenterWindow();

	// If this is the first instance of our App then track it so any other instances can find us
	m_pInstanceChecker.TrackFirstInstanceRunning(m_pMainWnd->GetSafeHwnd());

	return TRUE;
}

int CIntelliEditApp::ExitInstance()
{
	//TODO: handle additional resources you may have added
	AfxOleTerm(FALSE);

	//Unload the Scintilla \ Lexilla dlls
	if (m_hScintilla != nullptr)
	{
		FreeLibrary(m_hScintilla);
		m_hScintilla = nullptr;
	}
	if (m_hLexilla != nullptr)
	{
		FreeLibrary(m_hLexilla);
		m_hLexilla = nullptr;
	}

	//Let the base class do its thing
	return CWinAppEx::ExitInstance();
}

CString CIntelliEditApp::GetModuleFileName(_Inout_opt_ DWORD* pdwLastError)
{
	CString sModuleFileName;
	DWORD dwSize{ _MAX_PATH };
	while (true)
	{
		TCHAR* pszModuleFileName{ sModuleFileName.GetBuffer(dwSize) };
		const DWORD dwResult{ ::GetModuleFileName(nullptr, pszModuleFileName, dwSize) };
		if (dwResult == 0)
		{
			if (pdwLastError != nullptr)
				*pdwLastError = GetLastError();
			sModuleFileName.ReleaseBuffer(0);
			return CString{};
		}
		else if (dwResult < dwSize)
		{
			if (pdwLastError != nullptr)
				*pdwLastError = ERROR_SUCCESS;
			sModuleFileName.ReleaseBuffer(dwResult);
			return sModuleFileName;
		}
		else if (dwResult == dwSize)
		{
			sModuleFileName.ReleaseBuffer(0);
			dwSize *= 2;
		}
	}
}

HMODULE CIntelliEditApp::LoadLibraryFromApplicationDirectory(_In_z_ LPCTSTR lpFileName)
{
	// Get the Application directory
	CString sFullPath{ GetModuleFileName() };
	if (sFullPath.IsEmpty())
#pragma warning(suppress: 26487)
		return nullptr;

	// Form the new path
	std::filesystem::path path{ sFullPath.GetString() };
	std::filesystem::path DLLPath{ lpFileName };
	path.replace_filename(DLLPath.filename());

	// Delegate to LoadLibrary
	return LoadLibraryW(path.c_str());
}

// CIntelliEditApp message handlers

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
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();

protected:
	CStatic m_ctrlVersion;
	CEdit m_ctrlWarning;
	CVersionInfo m_pVersionInfo;
	CHLinkCtrl m_ctrlWebsite;
	CHLinkCtrl m_ctrlEmail;
	CHLinkCtrl m_ctrlContributors;

	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_VERSION, m_ctrlVersion);
	DDX_Control(pDX, IDC_WARNING, m_ctrlWarning);
	DDX_Control(pDX, IDC_WEBSITE, m_ctrlWebsite);
	DDX_Control(pDX, IDC_EMAIL, m_ctrlEmail);
	DDX_Control(pDX, IDC_CONTRIBUTORS, m_ctrlContributors);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

CString GetModuleFileName(_Inout_opt_ DWORD* pdwLastError = nullptr)
{
	CString strModuleFileName;
	DWORD dwSize{ _MAX_PATH };
	while (true)
	{
		TCHAR* pszModuleFileName{ strModuleFileName.GetBuffer(dwSize) };
		const DWORD dwResult{ ::GetModuleFileName(nullptr, pszModuleFileName, dwSize) };
		if (dwResult == 0)
		{
			if (pdwLastError != nullptr)
				*pdwLastError = GetLastError();
			strModuleFileName.ReleaseBuffer(0);
			return CString{};
		}
		else if (dwResult < dwSize)
		{
			if (pdwLastError != nullptr)
				*pdwLastError = ERROR_SUCCESS;
			strModuleFileName.ReleaseBuffer(dwResult);
			return strModuleFileName;
		}
		else if (dwResult == dwSize)
		{
			strModuleFileName.ReleaseBuffer(0);
			dwSize *= 2;
		}
	}
}

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString strFullPath{ GetModuleFileName() };
	if (strFullPath.IsEmpty())
#pragma warning(suppress: 26487)
		return FALSE;

	if (m_pVersionInfo.Load(strFullPath.GetString()))
	{
		CString strName = m_pVersionInfo.GetProductName().c_str();
		CString strVersion = m_pVersionInfo.GetProductVersionAsString().c_str();
		strVersion.Replace(_T(" "), _T(""));
		strVersion.Replace(_T(","), _T("."));
		const int nFirst = strVersion.Find(_T('.'));
		const int nSecond = strVersion.Find(_T('.'), nFirst + 1);
		strVersion.Truncate(nSecond);
#if _WIN32 || _WIN64
#if _WIN64
		m_ctrlVersion.SetWindowText(strName + _T(" version ") + strVersion + _T(" (64-bit)"));
#else
		m_ctrlVersion.SetWindowText(strName + _T(" version ") + strVersion + _T(" (32-bit)"));
#endif
#endif
	}

	m_ctrlWarning.SetWindowText(_T("This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version. This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>."));

	m_ctrlWebsite.SetHyperLink(_T("https://www.moga.doctor/"));
	m_ctrlEmail.SetHyperLink(_T("mailto:stefan-mihai@moga.doctor"));
	m_ctrlContributors.SetHyperLink(_T("https://github.com/mihaimoga/IntelliEdit/graphs/contributors"));

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CAboutDlg::OnDestroy()
{
	CDialog::OnDestroy();
}

// App command to run the dialog
void CIntelliEditApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CIntelliEditApp customization load/save methods

void CIntelliEditApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void CIntelliEditApp::LoadCustomState()
{
}

void CIntelliEditApp::SaveCustomState()
{
}

// CIntelliEditApp message handlers
