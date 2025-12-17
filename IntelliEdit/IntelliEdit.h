/* Copyright (C) 2024-2026 Stefan-Mihai MOGA
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

// IntelliEdit.h : main header file for the IntelliEdit application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include "sinstance.h"

// CIntelliEditApp:
// See IntelliEdit.cpp for the implementation of this class
//

class CIntelliEditApp : public CWinAppEx
{
public:
	CIntelliEditApp() noexcept;

public:
	CInstanceChecker m_pInstanceChecker;
	Lexilla::CreateLexerFn m_pCreateLexer;
	//Member variables
	HINSTANCE m_hScintilla;
	HINSTANCE m_hLexilla;

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	[[nodiscard]] CString GetModuleFileName(_Inout_opt_ DWORD* pdwLastError = nullptr);
	[[nodiscard]] HMODULE LoadLibraryFromApplicationDirectory(_In_z_ LPCTSTR lpFileName);

// Implementation
	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CIntelliEditApp theApp;
