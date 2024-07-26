/* Copyright (C) 2024 Stefan-Mihai MOGA
This file is part of IntelliEdit application developed by Stefan-Mihai MOGA.

IntelliEdit is free software: you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the Open
Source Initiative, either version 3 of the License, or any later version.

IntelliEdit is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
IntelliEdit. If not, see <http://www.opensource.org/licenses/gpl-3.0.html>*/

// IntelliEditView.cpp : implementation of the CIntelliEditView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "IntelliEdit.h"
#endif

#include "IntelliEditDoc.h"
#include "IntelliEditView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const TCHAR* g_cppKeywords
{
	/* _T("alignas alignof and and_eq asm atomic_cancel atomic_commit atomic_noexcept auto bitand bitor bool break ")
	_T("case catch char char8_t char16_t char32_t class compl concept const consteval constexpr constinit const_cast continue ")
	_T("co_await co_return co_yield decltype default delete do double dynamic_cast else enum explicit export extern false float for ")
	_T("friend goto if inline int long mutable namespace new noexcept not not_eq nullptr ")
	_T("operator or or_eq private protected public ")
	_T("reflexpr register reinterpret_cast requires return short signed sizeof static static_assert static_cast struct switch synchronized ")
	_T("template this thread_local throw true try typedef typeid typename union unsigned using ")
	_T("virtual void volatile wchar_t while xor xor_eq") */

	/* https://www.w3schools.com/cpp/cpp_ref_keywords.asp */
	_T("and and_eq bitand bitor bool break case catch char class compl const ")
	_T("continue default delete do double else enum false float for friend goto ")
	_T("if int long namespace new not not_eq or or_eq private protected public ")
	_T("return short signed sizeof static struct switch template this throw true ")
	_T("try typedef unsigned using virtual void while xor xor_eq")
};

const TCHAR* g_csKeywords
{
	_T("abstract as base bool break byte case catch char checked class const ")
	_T("continue decimal default delegate do double else enum event explicit ")
	_T("extern false finally fixed float for foreach goto if implicit in int ")
	_T("interface internal is lock long namespace new null object operator ")
	_T("out override params private protected public readonly ref return sbyte ")
	_T("sealed short sizeof stackalloc static string struct switch this throw ")
	_T("true try typeof uint ulong unchecked unsafe ushort using virtual void ")
	_T("volatile while")
};

const TCHAR* g_javaKeywords
{
	/* https://www.w3schools.com/java/java_ref_keywords.asp */
	_T("abstract assert boolean break byte case catch char class continue const ")
	_T("default do double else enum exports extends final finally float for goto ")
	_T("if implements import instanceof int interface long module native new ")
	_T("package private protected public requires return short static strictfp ")
	_T("super switch synchronized this throw throws transient try var void ")
	_T("volatile while")
};

const TCHAR* g_pyKeywords
{
	/* https://www.w3schools.com/python/python_ref_keywords.asp */
	_T("and as assert break class continue def del elif else except False finally ")
	_T("for from global if import in is lambda None nonlocal not or pass raise ")
	_T("return True try while with yield")
};

// CIntelliEditView

IMPLEMENT_DYNCREATE(CIntelliEditView, CScintillaView)

BEGIN_MESSAGE_MAP(CIntelliEditView, CScintillaView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CIntelliEditView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_OPTIONS_ADDMARKER, &CIntelliEditView::OnOptionsAddmarker)
	ON_COMMAND(ID_OPTIONS_DELETEMARKER, &CIntelliEditView::OnOptionsDeletemarker)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_DELETEMARKER, &CIntelliEditView::OnUpdateOptionsDeletemarker)
	ON_COMMAND(ID_OPTIONS_FIND_NEXTMARKER, &CIntelliEditView::OnOptionsFindNextmarker)
	ON_COMMAND(ID_OPTIONS_FIND_PREVMARKER, &CIntelliEditView::OnOptionsFindPrevmarker)
	ON_COMMAND(ID_OPTIONS_FOLD_MARGIN, &CIntelliEditView::OnOptionsFoldMargin)
	ON_COMMAND(ID_OPTIONS_SELECTION_MARGIN, &CIntelliEditView::OnOptionsSelectionMargin)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_SELECTION_MARGIN, &CIntelliEditView::OnUpdateOptionsSelectionMargin)
	ON_COMMAND(ID_OPTIONS_VIEW_LINENUMBERS, &CIntelliEditView::OnOptionsViewLinenumbers)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_VIEW_LINENUMBERS, &CIntelliEditView::OnUpdateOptionsViewLinenumbers)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_ADDMARKER, &CIntelliEditView::OnUpdateOptionsAddmarker)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_FOLD_MARGIN, &CIntelliEditView::OnUpdateOptionsFoldMargin)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_LINE, &CIntelliEditView::OnUpdateLine)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_STYLE, &CIntelliEditView::OnUpdateStyle)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_FOLD, &CIntelliEditView::OnUpdateFold)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_OVR, &CIntelliEditView::OnUpdateInsert)
	ON_WM_ACTIVATE()
	ON_WM_CREATE()
END_MESSAGE_MAP()

// CIntelliEditView construction/destruction

CIntelliEditView::CIntelliEditView() noexcept : m_cppLexer { nullptr }, m_pyLexer { nullptr }
{
	LoadMarginSettings();
}

CIntelliEditView::~CIntelliEditView()
{
}

BOOL CIntelliEditView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CIntelliEditView drawing

void CIntelliEditView::OnDraw(CDC* /*pDC*/)
{
	CIntelliEditDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}

void CIntelliEditView::SetAStyle(int style, COLORREF fore, COLORREF back, int size, const char* face)
{
	auto& rCtrl{ GetCtrl() };

	rCtrl.StyleSetFore(style, fore);
	rCtrl.StyleSetBack(style, back);
	if (size >= 1)
		rCtrl.StyleSetSize(style, size);
	if (face)
		rCtrl.StyleSetFont(style, face);
}

void CIntelliEditView::DefineMarker(int marker, Scintilla::MarkerSymbol markerType, COLORREF fore, COLORREF back)
{
	auto& rCtrl{ GetCtrl() };

	rCtrl.MarkerDefine(marker, markerType);
	rCtrl.MarkerSetFore(marker, fore);
	rCtrl.MarkerSetBack(marker, back);
}

void CIntelliEditView::OnInitialUpdate()
{
	TCHAR lpszDrive[_MAX_DRIVE] = { 0 };
	TCHAR lpszFolder[_MAX_DIR] = { 0 };
	TCHAR lpszFileName[_MAX_FNAME] = { 0 };
	TCHAR lpszExtension[_MAX_EXT] = { 0 };

	//Let the base class do its thing
	__super::OnInitialUpdate();

	auto& rCtrl{ GetCtrl() };

	CIntelliEditDoc* pDocument = GetDocument();
	ASSERT_VALID(pDocument);
	CString strTempPath = pDocument->GetPathName();
	strTempPath.MakeLower();
	_tsplitpath_s(strTempPath,
		lpszDrive, _MAX_DRIVE,
		lpszFolder, _MAX_DIR,
		lpszFileName, _MAX_FNAME,
		lpszExtension, _MAX_EXT);

	if ((_tcsicmp(lpszExtension, _T(".c")) == 0) ||
		(_tcsicmp(lpszExtension, _T(".cpp")) == 0) ||
		(_tcsicmp(lpszExtension, _T(".cxx")) == 0) ||
		(_tcsicmp(lpszExtension, _T(".h")) == 0) ||
		(_tcsicmp(lpszExtension, _T(".hpp")) == 0))
	{
		// Setup the C++ Lexer
		rCtrl.SetILexer(m_cppLexer);
		rCtrl.SetKeyWords(0, g_cppKeywords);
	}
	else
	{
		if (_tcsicmp(lpszExtension, _T(".cs")) == 0)
		{
			// Setup the C++ Lexer
			rCtrl.SetILexer(m_cppLexer);
			rCtrl.SetKeyWords(0, g_csKeywords);
		}
		else
		{
			if (_tcsicmp(lpszExtension, _T(".java")) == 0)
			{
				// Setup the C++ Lexer
				rCtrl.SetILexer(m_cppLexer);
				rCtrl.SetKeyWords(0, g_javaKeywords);
			}
			else
			{
				if (_tcsicmp(lpszExtension, _T(".py")) == 0)
				{
					// Setup the Python Lexer
					rCtrl.SetILexer(m_pyLexer);
					rCtrl.SetKeyWords(0, g_pyKeywords);
				}
			}
		}
	}

	// Setup styles
	SetAStyle(static_cast<int>(Scintilla::StylesCommon::Default), RGB(0, 0, 0), RGB(0xff, 0xff, 0xff), 11, "Consolas");
	rCtrl.StyleClearAll();
	SetAStyle(SCE_C_DEFAULT, RGB(0, 0, 0));
	SetAStyle(SCE_C_COMMENT, RGB(0, 0x80, 0));
	SetAStyle(SCE_C_COMMENTLINE, RGB(0, 0x80, 0));
	SetAStyle(SCE_C_COMMENTDOC, RGB(0, 0x80, 0));
	SetAStyle(SCE_C_COMMENTLINEDOC, RGB(0, 0x80, 0));
	SetAStyle(SCE_C_COMMENTDOCKEYWORD, RGB(0, 0x80, 0));
	SetAStyle(SCE_C_COMMENTDOCKEYWORDERROR, RGB(0, 0x80, 0));
	SetAStyle(SCE_C_NUMBER, RGB(0, 0x80, 0x80));
	SetAStyle(SCE_C_WORD, RGB(0, 0, 0x80));
	rCtrl.StyleSetBold(SCE_C_WORD, 1);
	SetAStyle(SCE_C_STRING, RGB(0x80, 0, 0x80));
	SetAStyle(SCE_C_IDENTIFIER, RGB(0, 0, 0));
	SetAStyle(SCE_C_PREPROCESSOR, RGB(0x80, 0, 0));
	SetAStyle(SCE_C_OPERATOR, RGB(0x80, 0x80, 0));

	// Setup folding
	rCtrl.SetMarginWidthN(2, 16);
	rCtrl.SetMarginSensitiveN(2, TRUE);
	rCtrl.SetMarginTypeN(2, Scintilla::MarginType::Symbol);
	rCtrl.SetMarginMaskN(2, Scintilla::MaskFolders);
	rCtrl.SetSCIProperty(_T("fold"), _T("1"));

	// Setup markers
	DefineMarker(static_cast<int>(Scintilla::MarkerOutline::FolderOpen), Scintilla::MarkerSymbol::Minus, RGB(0xff, 0xff, 0xff), RGB(0, 0, 0xFF));
	DefineMarker(static_cast<int>(Scintilla::MarkerOutline::Folder), Scintilla::MarkerSymbol::Plus, RGB(0xff, 0xff, 0xff), RGB(0, 0, 0));
	DefineMarker(static_cast<int>(Scintilla::MarkerOutline::FolderSub), Scintilla::MarkerSymbol::Empty, RGB(0xff, 0xff, 0xff), RGB(0, 0, 0));
	DefineMarker(static_cast<int>(Scintilla::MarkerOutline::FolderTail), Scintilla::MarkerSymbol::Empty, RGB(0xff, 0xff, 0xff), RGB(0, 0, 0));
	DefineMarker(static_cast<int>(Scintilla::MarkerOutline::FolderEnd), Scintilla::MarkerSymbol::Empty, RGB(0xff, 0xff, 0xff), RGB(0, 0, 0));
	DefineMarker(static_cast<int>(Scintilla::MarkerOutline::FolderOpenMid), Scintilla::MarkerSymbol::Empty, RGB(0xff, 0xff, 0xff), RGB(0, 0, 0));
	DefineMarker(static_cast<int>(Scintilla::MarkerOutline::FolderMidTail), Scintilla::MarkerSymbol::Empty, RGB(0xff, 0xff, 0xff), RGB(0, 0, 0));

	//Setup auto completion
	rCtrl.AutoCSetSeparator(10); //Use a separator of line feed

	// Setup call tips
	rCtrl.SetMouseDwellTime(1000);

	// Enable Multiple selection
	rCtrl.SetMultipleSelection(TRUE);

	// Show line numbers
	rCtrl.SetMarginWidthN(0, 32);

#ifdef _DEBUG
	AfxDump(this);
#endif
}

// CIntelliEditView printing

void CIntelliEditView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

void CIntelliEditView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CIntelliEditView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}

// CIntelliEditView diagnostics

#ifdef _DEBUG
void CIntelliEditView::AssertValid() const
{
	CView::AssertValid();
}

void CIntelliEditView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CIntelliEditDoc* CIntelliEditView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CIntelliEditDoc)));
	return (CIntelliEditDoc*)m_pDocument;
}
#endif //_DEBUG

// CIntelliEditView message handlers

void CIntelliEditView::OnOptionsAddmarker()
{
	auto& rCtrl{ GetCtrl() };
	const Scintilla::Position nPos{ rCtrl.GetCurrentPos() };
	const auto nLine{ rCtrl.LineFromPosition(nPos) };
	rCtrl.MarkerAdd(nLine, 0);
}

#pragma warning(suppress: 26429)
void CIntelliEditView::OnUpdateOptionsAddmarker(CCmdUI* pCmdUI)
{
	auto& rCtrl{ GetCtrl() };
	const Scintilla::Position nPos{ rCtrl.GetCurrentPos() };
	const auto nLine{ rCtrl.LineFromPosition(nPos) };
	const int nBits{ rCtrl.MarkerGet(nLine) };
#pragma warning(suppress: 26486)
	pCmdUI->Enable((nBits & 0x1) == 0);
}

void CIntelliEditView::OnOptionsDeletemarker()
{
	auto& rCtrl{ GetCtrl() };
	const Scintilla::Position nPos{ rCtrl.GetCurrentPos() };
	const auto nLine{ rCtrl.LineFromPosition(nPos) };
	rCtrl.MarkerDelete(nLine, 0);
}

#pragma warning(suppress: 26429)
void CIntelliEditView::OnUpdateOptionsDeletemarker(CCmdUI* pCmdUI)
{
	auto& rCtrl{ GetCtrl() };
	const Scintilla::Position nPos{ rCtrl.GetCurrentPos() };
	const auto nLine{ rCtrl.LineFromPosition(nPos) };
	const int nBits{ rCtrl.MarkerGet(nLine) };
#pragma warning(suppress: 26486)
	pCmdUI->Enable(nBits & 0x1);
}

void CIntelliEditView::OnOptionsFindNextmarker()
{
	auto& rCtrl{ GetCtrl() };
	const Scintilla::Position nPos{ rCtrl.GetCurrentPos() };
	const auto nLine{ rCtrl.LineFromPosition(nPos) };
	const auto nFoundLine{ rCtrl.MarkerNext(nLine + 1, 0x1) };
	if (nFoundLine >= 0)
		rCtrl.GotoLine(nFoundLine);
	else
		MessageBeep(MB_ICONHAND);
}

void CIntelliEditView::OnOptionsFindPrevmarker()
{
	auto& rCtrl{ GetCtrl() };
	const Scintilla::Position nPos{ rCtrl.GetCurrentPos() };
	const auto nLine{ rCtrl.LineFromPosition(nPos) };
	const auto nFoundLine{ rCtrl.MarkerPrevious(nLine - 1, 0x1) };
	if (nFoundLine >= 0)
		rCtrl.GotoLine(nFoundLine);
	else
		MessageBeep(MB_ICONHAND);
}

void CIntelliEditView::OnOptionsFoldMargin()
{
	auto& rCtrl{ GetCtrl() };
	const int nMarginWidth{ rCtrl.GetMarginWidthN(2) };
	if (nMarginWidth)
		rCtrl.SetMarginWidthN(2, 0);
	else
		rCtrl.SetMarginWidthN(2, 16);
}

#pragma warning(suppress: 26429)
void CIntelliEditView::OnUpdateOptionsFoldMargin(CCmdUI* pCmdUI)
{
#pragma warning(suppress: 26486)
	pCmdUI->SetCheck(GetCtrl().GetMarginWidthN(2) != 0);
}

void CIntelliEditView::OnOptionsSelectionMargin()
{
	auto& rCtrl{ GetCtrl() };
	const int nMarginWidth{ rCtrl.GetMarginWidthN(1) };
	if (nMarginWidth)
		rCtrl.SetMarginWidthN(1, 0);
	else
		rCtrl.SetMarginWidthN(1, 16);
}

#pragma warning(suppress: 26429)
void CIntelliEditView::OnUpdateOptionsSelectionMargin(CCmdUI* pCmdUI)
{
#pragma warning(suppress: 26486)
	pCmdUI->SetCheck(GetCtrl().GetMarginWidthN(1) != 0);
}

void CIntelliEditView::OnOptionsViewLinenumbers()
{
	auto& rCtrl{ GetCtrl() };
	const int nMarginWidth{ rCtrl.GetMarginWidthN(0) };
	if (nMarginWidth)
		rCtrl.SetMarginWidthN(0, 0);
	else
		rCtrl.SetMarginWidthN(0, 32);
}

#pragma warning(suppress: 26429)
void CIntelliEditView::OnUpdateOptionsViewLinenumbers(CCmdUI* pCmdUI)
{
#pragma warning(suppress: 26486)
	pCmdUI->SetCheck(GetCtrl().GetMarginWidthN(0) != 0);
}

#pragma warning(suppress: 26429)
void CIntelliEditView::OnUpdateInsert(CCmdUI* pCmdUI)
{
	CString sText;
	if (GetCtrl().GetOvertype())
		VERIFY(sText.LoadString(ID_INDICATOR_OVR));
#pragma warning(suppress: 26486)
	pCmdUI->SetText(sText);
}

#pragma warning(suppress: 26429)
void CIntelliEditView::OnUpdateStyle(CCmdUI* pCmdUI)
{
	auto& rCtrl{ GetCtrl() };
	const Scintilla::Position nPos{ rCtrl.GetCurrentPos() };
	const int nStyle{ rCtrl.GetStyleAt(nPos) };
	CString sLine;
	sLine.Format(IDS_STYLE_INDICATOR, nStyle);
#pragma warning(suppress: 26486)
	pCmdUI->SetText(sLine);
}

#pragma warning(suppress: 26429)
void CIntelliEditView::OnUpdateFold(CCmdUI* pCmdUI)
{
	auto& rCtrl{ GetCtrl() };
	const Scintilla::Position nPos{ rCtrl.GetCurrentPos() };
	const auto nLine{ rCtrl.LineFromPosition(nPos) };
	int nLevel{ static_cast<int>(rCtrl.GetFoldLevel(nLine) & Scintilla::FoldLevel::NumberMask) };
	nLevel -= 1024;
	CString sLine;
	sLine.Format(IDS_FOLD_INDICATOR, nLevel);
#pragma warning(suppress: 26486)
	pCmdUI->SetText(sLine);
}

#pragma warning(suppress: 26429)
void CIntelliEditView::OnUpdateLine(CCmdUI* pCmdUI)
{
	auto& rCtrl{ GetCtrl() };
	const Scintilla::Position nPos{ rCtrl.GetCurrentPos() };
	const auto nLine{ rCtrl.LineFromPosition(nPos) };
	const Scintilla::Position nColumn{ rCtrl.GetColumn(nPos) };

	CString sLine;
	sLine.Format(IDS_LINE_INDICATOR, nLine, nColumn, nPos);
#pragma warning(suppress: 26486)
	pCmdUI->SetText(sLine);
}

//Some simple examples of implementing auto completion
void CIntelliEditView::OnCharAdded(_Inout_ Scintilla::NotificationData* pSCNotification)
{
	UNREFERENCED_PARAMETER(pSCNotification);

	auto& rCtrl{ GetCtrl() };

	//Get the previous 13 characters and if it is "scintilla is " case insensitive
	//then display a list which allows "very cool", "easy" and "way cool!!"
	const Scintilla::Position nStartSel{ rCtrl.GetSelectionStart() };
	const Scintilla::Position nEndSel{ rCtrl.GetSelectionEnd() };
	if ((nStartSel == nEndSel) && (nStartSel >= 13))
	{
		Scintilla::TextRangeFull tr{};
		tr.chrg.cpMin = nStartSel - 13;
		tr.chrg.cpMax = nEndSel;
		CStringA sText;
		tr.lpstrText = sText.GetBuffer(13);
		rCtrl.GetTextRangeFull(&tr);
		sText.ReleaseBuffer();

		//Display the auto completion list
		if (sText.CompareNoCase("scintilla is ") == 0)
		{
			//Display the auto completion list
			//rCtrl.AutoCSetOrder(Scintilla::Ordering::PerformSort);
			//int nOrder{rCtrl.AutoCGetOrder()};
			rCtrl.AutoCShow(0, _T("very cool\neasy to use\nway cool!!"));
		}
	}

#ifdef _DEBUG
#endif //#ifdef _DEBUG

	//As another example, get the previous 2 characters and if it is "res" case sensitive
	//then display a list which allows "resize", "restart" and "restore"
	if ((nStartSel == nEndSel) && (nStartSel >= 3))
	{
		Scintilla::TextRangeFull tr{};
		tr.chrg.cpMin = nStartSel - 3;
		tr.chrg.cpMax = nEndSel;
		CStringA sText;
		tr.lpstrText = sText.GetBuffer(3);
		rCtrl.GetTextRangeFull(&tr);
		sText.ReleaseBuffer();

		//Display the auto completion list
		if (sText == "res")
		{
			//Display the auto completion list
			rCtrl.AutoCShow(3, _T("resize\nrestart\nrestore"));
		}
	}

	//As another example, get the previous 16 characters and if it is "SAMPLE_INDICATOR" case insensitive
	//then display that text with a squiggly underline
	if ((nStartSel == nEndSel) && (nStartSel >= 16))
	{
		Scintilla::TextRangeFull tr{};
		tr.chrg.cpMin = nStartSel - 16;
		tr.chrg.cpMax = nEndSel;
		CStringA sText;
		tr.lpstrText = sText.GetBuffer(16);
		rCtrl.GetTextRangeFull(&tr);
		sText.ReleaseBuffer();

		//Display the Sample indicator
		if (sText.CompareNoCase("sample_indicator") == 0)
		{
			//For demonstration purposes lets make the "C" Comment style into hotspots which turn red
			rCtrl.SetIndicatorCurrent(0);
			rCtrl.StyleSetHotSpot(SCE_C_COMMENT, TRUE);
			rCtrl.SetHotspotActiveFore(TRUE, RGB(255, 0, 0));

			//Show the indicator
#pragma warning(suppress: 26472)
			rCtrl.IndicatorFillRange(tr.chrg.cpMin, static_cast<int>(nStartSel));
		}
	}
}

//A simple example of call tips
void CIntelliEditView::OnDwellStart(_Inout_ Scintilla::NotificationData* pSCNotification)
{
	auto& rCtrl{ GetCtrl() };

	//Only display the call tip if the scintilla window has focus
	const CWnd* pFocusWnd{ GetFocus() };
	if (pFocusWnd != nullptr)
	{
		if (pFocusWnd->GetSafeHwnd() == rCtrl.GetSafeHwnd())
		{
			//Get the previous 7 characters and next 7 characters around
			//the current dwell position and if it is "author " case insensitive
			//then display "PJ Naughter" as a call tip
			Scintilla::TextRangeFull tr{};
			tr.chrg.cpMin = max(0, pSCNotification->position - 7);
			tr.chrg.cpMax = min(pSCNotification->position + 7, rCtrl.GetLength());
			CStringA sText;
			tr.lpstrText = sText.GetBuffer(14);
			rCtrl.GetTextRangeFull(&tr);
			sText.ReleaseBuffer();

			//Display the call tip
			sText.MakeUpper();
			if (sText.Find("AUTHOR ") != -1)
				rCtrl.CallTipShow(pSCNotification->position, _T("PJ Naughter"));
		}
	}
}

void CIntelliEditView::OnDwellEnd(_Inout_ Scintilla::NotificationData* /*pSCNotification*/)
{
	auto& rCtrl{ GetCtrl() };

	//Cancel any outstanding call tip
	if (rCtrl.CallTipActive())
		rCtrl.CallTipCancel();
}

#pragma warning(suppress: 26434)
void CIntelliEditView::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	//Let the base class do its thing
	__super::OnActivate(nState, pWndOther, bMinimized);

	auto& rCtrl{ GetCtrl() };

	//Cancel any outstanding call tip
	if ((nState == WA_INACTIVE) && rCtrl.CallTipActive())
		rCtrl.CallTipCancel();
}

void CIntelliEditView::OnModifyAttemptRO(_Inout_ Scintilla::NotificationData* /*pSCNotification*/)
{
#pragma warning(suppress: 26493)
	if (AfxMessageBox(IDP_ALLOW_MODIFY_READONLY_FILE, MB_YESNO) == IDYES)
		GetCtrl().SetReadOnly(FALSE);
}

#pragma warning(suppress: 26440)
void CIntelliEditView::OnModified(_Inout_ Scintilla::NotificationData* pSCNotification)
{
	if (static_cast<int>(pSCNotification->modificationType) & static_cast<int>(Scintilla::ModificationFlags::InsertCheck))
	{
#ifdef _DEBUG
		//Just some demo code to test out "InsertCheck" notification
		auto& rCtrl{ GetCtrl() };
#pragma warning(suppress: 26486)
		if (strcmp(pSCNotification->text, "Dublin") == 0)
			rCtrl.ChangeInsertion(18, _T("Capital of Ireland"));
#endif
	}
}

std::unique_ptr<Scintilla::CScintillaCtrl> CIntelliEditView::CreateScintillaControl()
{
	return std::make_unique<CScintillaDemoCtrl>();
}

#pragma warning(suppress: 26434)
int CIntelliEditView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	//Let the base class do its thing
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;

	//Create the C++ Lexer
#pragma warning(suppress: 26429)
	if (m_cppLexer == nullptr)
	{
		m_cppLexer = theApp.m_pCreateLexer("cpp");
		if (m_cppLexer == nullptr)
			return -1;
	}

	//Create the Python Lexer
#pragma warning(suppress: 26429)
	if (m_pyLexer == nullptr)
	{
		m_pyLexer = theApp.m_pCreateLexer("python");
		if (m_pyLexer == nullptr)
			return -1;
	}

	return 0;
}
