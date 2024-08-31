/* Copyright (C) 2024 Stefan-Mihai MOGA
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

// CheckForUpdatesDlg.cpp : implementation file
//

#include "pch.h"
#include "IntelliEdit.h"
#include "CheckForUpdatesDlg.h"
#include "AppSettings.h"

// CCheckForUpdatesDlg dialog

IMPLEMENT_DYNAMIC(CCheckForUpdatesDlg, CDialogEx)

CCheckForUpdatesDlg::CCheckForUpdatesDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CheckForUpdatesDlg, pParent)
{
	m_nUpdateThreadID = 0;
	m_hUpdateThread = nullptr;
}

CCheckForUpdatesDlg::~CCheckForUpdatesDlg()
{
}

void CCheckForUpdatesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATUS, m_ctrlStatusMessage);
	DDX_Control(pDX, IDC_PROGRESS, m_ctrlProgress);
}

BEGIN_MESSAGE_MAP(CCheckForUpdatesDlg, CDialogEx)
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CCheckForUpdatesDlg message handlers

bool g_bThreadRunning = false;
DWORD WINAPI UpdateThreadProc(LPVOID lpParam)
{
	const HRESULT hr{ CoInitialize(nullptr) };
	if (FAILED(hr))
		return 1;

	TCHAR lpszDrive[_MAX_DRIVE];
	TCHAR lpszDirectory[_MAX_DIR];
	TCHAR lpszFilename[_MAX_FNAME];
	TCHAR lpszExtension[_MAX_EXT];
	TCHAR lpszFullPath[_MAX_PATH];

	g_bThreadRunning = true;
	CCheckForUpdatesDlg* dlgCheckForUpdates = (CCheckForUpdatesDlg*)lpParam;
	dlgCheckForUpdates->m_ctrlProgress.SetMarquee(TRUE, 30);
	dlgCheckForUpdates->m_ctrlStatusMessage.SetWindowText(_T("Connecting..."));

	TCHAR lpszTempPath[_MAX_PATH] = { 0 };
	DWORD nLength = GetTempPath(_MAX_PATH, lpszTempPath);
	if (nLength > 0)
	{
		TCHAR lpszFilePath[_MAX_PATH] = { 0 };
		nLength = GetTempFileName(lpszTempPath, nullptr, 0, lpszFilePath);
		if (nLength > 0)
		{
			CString strFileName = lpszFilePath;
			strFileName.Replace(_T(".tmp"), _T(".xml"));
			if (URLDownloadToFile(nullptr, APPLICATION_URL, strFileName, 0, nullptr) == S_OK)
			{
				try {
					CXMLAppSettings pAppSettings(std::wstring(strFileName), true, true);
					std::wstring strVersion = pAppSettings.GetString(IntelliEditSection, _T("Version"));
					std::wstring strDownload = pAppSettings.GetString(IntelliEditSection, _T("Download"));

					if (g_bThreadRunning)
					{
						VERIFY(0 == _tsplitpath_s(AfxGetApp()->m_pszHelpFilePath, lpszDrive, _MAX_DRIVE, lpszDirectory, _MAX_DIR, lpszFilename, _MAX_FNAME, lpszExtension, _MAX_EXT));
						VERIFY(0 == _tmakepath_s(lpszFullPath, _MAX_PATH, lpszDrive, lpszDirectory, lpszFilename, _T(".exe")));

						if (dlgCheckForUpdates->m_pVersionInfo.Load(lpszFullPath))
						{
							g_bThreadRunning = _tcscmp(strVersion.c_str(), dlgCheckForUpdates->m_pVersionInfo.GetProductVersionAsString().c_str());
						}
					}

					if (g_bThreadRunning)
					{
						nLength = GetTempFileName(lpszTempPath, nullptr, 0, lpszFilePath);
						strFileName = lpszFilePath;
						strFileName.Replace(_T(".tmp"), _T(".msi"));

						dlgCheckForUpdates->m_ctrlStatusMessage.SetWindowText(_T("Downloading..."));
						if (URLDownloadToFile(nullptr, strDownload.c_str(), strFileName, 0, nullptr) == S_OK)
						{
							SHELLEXECUTEINFO pShellExecuteInfo;
							pShellExecuteInfo.cbSize = sizeof(SHELLEXECUTEINFO);
							pShellExecuteInfo.fMask = SEE_MASK_FLAG_DDEWAIT | SEE_MASK_NOCLOSEPROCESS | SEE_MASK_DOENVSUBST;
							pShellExecuteInfo.hwnd = dlgCheckForUpdates->GetSafeHwnd();
							pShellExecuteInfo.lpVerb = _T("open");
							pShellExecuteInfo.lpFile = strFileName;
							pShellExecuteInfo.lpParameters = nullptr;
							pShellExecuteInfo.lpDirectory = nullptr;
							pShellExecuteInfo.nShow = SW_SHOWNORMAL;
							VERIFY(ShellExecuteEx(&pShellExecuteInfo));

							dlgCheckForUpdates->m_ctrlStatusMessage.SetWindowText(_T("Please close the application to start the installer!"));
						}
						else
						{
							dlgCheckForUpdates->m_ctrlStatusMessage.SetWindowText(_T("Unknown Error!"));
						}
					}
				}
				catch (CAppSettingsException& pException)
				{
					const int nErrorLength = 0x100;
					TCHAR lpszErrorMessage[nErrorLength] = { 0, };
					pException.GetErrorMessage(lpszErrorMessage, nErrorLength);
					TRACE(_T("%s\n"), lpszErrorMessage);
				}
			}
			else
			{
				dlgCheckForUpdates->m_ctrlStatusMessage.SetWindowText(_T("Unknown Error!"));
			}
		}
	}
	dlgCheckForUpdates->m_ctrlProgress.SetMarquee(FALSE, 30);
	MessageBeep(MB_OK);
	::Sleep(3000);
	g_bThreadRunning = false;

	::ExitThread(0);
	return 0;
}

bool WaitWithMessageLoop(HANDLE hEvent, DWORD dwTimeout)
{
	DWORD dwRet;
	MSG msg;
	hEvent = hEvent ? hEvent : CreateEvent(NULL, FALSE, FALSE, NULL);

	while (true)
	{
		dwRet = MsgWaitForMultipleObjects(1, &hEvent, FALSE, dwTimeout, QS_ALLINPUT);
		if (dwRet == WAIT_OBJECT_0)
			return TRUE;
		if (dwRet != WAIT_OBJECT_0 + 1)
			break;
		while (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if (WaitForSingleObject(hEvent, 0) == WAIT_OBJECT_0)
				return true;
		}
	}
	return false;
}

BOOL CCheckForUpdatesDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_hUpdateThread = ::CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)UpdateThreadProc, this, 0, &m_nUpdateThreadID);

	m_nTimerID = SetTimer(0x1234, 100, nullptr);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CCheckForUpdatesDlg::OnCancel()
{
	if (g_bThreadRunning)
	{
		g_bThreadRunning = false;
		VERIFY(WaitWithMessageLoop(m_hUpdateThread, INFINITE));
	}
	CDialogEx::OnCancel();
}

void CCheckForUpdatesDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (m_nTimerID == nIDEvent)
	{
		if (!g_bThreadRunning)
		{
			CDialogEx::OnCancel();
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}
