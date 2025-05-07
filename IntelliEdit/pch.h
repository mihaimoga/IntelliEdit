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

// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#include "framework.h"

#define VC_EXTRALEAN
#define _AFX_ALL_WARNINGS

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#include <afxwin.h>
#include <afxext.h>
#include <atlsync.h>
#include <string>
#include <vector>
#include <algorithm>
#include <memory>
#include <array>
#include <filesystem>

#include <afxsock.h>            // MFC socket extensions

//Pull in support for Scintilla \ Lexilla
#pragma warning(push)
#pragma warning(disable: 26812)
#pragma warning(pop)
#include <ScintillaTypes.h>
#include <ScintillaCall.h>
#include <ScintillaMessages.h>
#include <ScintillaStructures.h>
#include <ILexer.h>
#include <Lexilla.h>
#include <SciLexer.h>
#include <ILoader.h>
#include <Scintilla.h>

#define INSTALLER_URL _T("https://www.moga.doctor/freeware/IntelliEditSetup.msi")
#define APPLICATION_URL _T("https://www.moga.doctor/freeware/IntelliEdit.xml")
#define USER_MANUAL_URL _T("https://www.moga.doctor/intelliedit/")

#endif //PCH_H
