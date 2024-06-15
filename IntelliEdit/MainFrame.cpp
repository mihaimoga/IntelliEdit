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

// MainFrame.cpp : implementation of the CMainFrame class
//

#include "pch.h"
#include "framework.h"
#include "IntelliEdit.h"
#include "ScintillaDocView.h"
#include "MainFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_WINDOW_MANAGER, &CMainFrame::OnWindowManager)
	ON_COMMAND(IDC_TWITTER, &CMainFrame::OnTwitter)
	ON_COMMAND(IDC_LINKEDIN, &CMainFrame::OnLinkedin)
	ON_COMMAND(IDC_FACEBOOK, &CMainFrame::OnFacebook)
	ON_COMMAND(IDC_INSTAGRAM, &CMainFrame::OnInstagram)
	ON_COMMAND(IDC_ISSUES, &CMainFrame::OnIssues)
	ON_COMMAND(IDC_DISCUSSIONS, &CMainFrame::OnDiscussions)
	ON_COMMAND(IDC_WIKI, &CMainFrame::OnWiki)
	ON_WM_SETTINGCHANGE()
	ON_WM_SYSCOLORCHANGE()
	ON_WM_PALETTECHANGED()
	ON_WM_QUERYNEWPALETTE()
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_LINE, &CMainFrame::OnUpdateLine)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_STYLE, &CMainFrame::OnUpdateStyle)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_FOLD, &CMainFrame::OnUpdateFold)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_OVR, &CMainFrame::OnUpdateInsert)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()

std::array<UINT, 8> g_Indicators
{
	ID_SEPARATOR, //status line indicator
	ID_INDICATOR_FOLD,
	ID_INDICATOR_STYLE,
	ID_INDICATOR_LINE,
	ID_INDICATOR_OVR,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL
};

// CMainFrame construction/destruction

CMainFrame::CMainFrame() noexcept
{
	// TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	CMDITabInfo mdiTabParams;
	mdiTabParams.m_style = CMFCTabCtrl::STYLE_3D_ONENOTE; // other styles available...
	mdiTabParams.m_bActiveTabCloseButton = TRUE;      // set to FALSE to place close button at right of tab area
	mdiTabParams.m_bTabIcons = FALSE;    // set to TRUE to enable document icons on MDI taba
	mdiTabParams.m_bAutoColor = TRUE;    // set to FALSE to disable auto-coloring of MDI tabs
	mdiTabParams.m_bDocumentMenu = TRUE; // enable the document menu at the right edge of the tab area
	EnableMDITabbedGroups(TRUE, mdiTabParams);

	m_wndRibbonBar.Create(this);
	m_wndRibbonBar.LoadFromResource(IDR_RIBBON);

	//Create the status bar
#pragma warning(suppress: 26472)
	if (!m_wndStatusBar.Create(this) /* || !m_wndStatusBar.SetIndicators(g_Indicators.data(), static_cast<int>(g_Indicators.size()))*/)
	{
		TRACE(_T("Failed to create status bar\n"));
		return -1;
	}

	// enable Visual Studio 2005 style docking window behavior
	CDockingManager::SetDockingMode(DT_SMART);
	// enable Visual Studio 2005 style docking window auto-hide behavior
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// set the visual manager used to draw all user interface elements
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// Enable enhanced windows management dialog
	EnableWindowsDialog(ID_WINDOW_MANAGER, ID_WINDOW_MANAGER, TRUE);

	// Switch the order of document name and application name on the window title bar. This
	// improves the usability of the taskbar because the document name is visible with the thumbnail.
	ModifyStyle(0, FWS_PREFIXTITLE);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWndEx::Dump(dc);
}
#endif //_DEBUG

// CMainFrame message handlers

void CMainFrame::OnWindowManager()
{
	ShowWindowsDialog();
}

void CMainFrame::OnTwitter()
{
	::ShellExecute(GetSafeHwnd(), _T("open"), _T("https://x.com/stefanmihaimoga"), nullptr, nullptr, SW_SHOW);
}

void CMainFrame::OnLinkedin()
{
	::ShellExecute(GetSafeHwnd(), _T("open"), _T("https://www.linkedin.com/in/stefanmihaimoga/"), nullptr, nullptr, SW_SHOW);
}

void CMainFrame::OnFacebook()
{
	::ShellExecute(GetSafeHwnd(), _T("open"), _T("https://www.facebook.com/stefanmihaimoga"), nullptr, nullptr, SW_SHOW);
}

void CMainFrame::OnInstagram()
{
	::ShellExecute(GetSafeHwnd(), _T("open"), _T("https://www.instagram.com/stefanmihaimoga/"), nullptr, nullptr, SW_SHOW);
}

void CMainFrame::OnIssues()
{
	::ShellExecute(GetSafeHwnd(), _T("open"), _T("https://github.com/mihaimoga/IntelliEdit/issues"), nullptr, nullptr, SW_SHOW);
}

void CMainFrame::OnDiscussions()
{
	::ShellExecute(GetSafeHwnd(), _T("open"), _T("https://github.com/mihaimoga/IntelliEdit/discussions"), nullptr, nullptr, SW_SHOW);
}

void CMainFrame::OnWiki()
{
	::ShellExecute(GetSafeHwnd(), _T("open"), _T("https://github.com/mihaimoga/IntelliEdit/wiki"), nullptr, nullptr, SW_SHOW);
}
#pragma warning(suppress: 26434)
void CMainFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
    //Validate our parameters
    const CWinApp* pApp{ AfxGetApp() };
#pragma warning(suppress: 26496)
    AFXASSUME(pApp != nullptr);

    //Pass the message on to all scintilla control
    POSITION posTemplate{ pApp->GetFirstDocTemplatePosition() };
    if (posTemplate != nullptr)
    {
#pragma warning(suppress: 26429)
        const CDocTemplate* pTemplate{ pApp->GetNextDocTemplate(posTemplate) };
        ASSERT(pTemplate != nullptr);
        POSITION posDoc{ pTemplate->GetFirstDocPosition() };
        if (posDoc != nullptr)
        {
            const CDocument* pDocument{ pTemplate->GetNextDoc(posDoc) };
            if (pDocument != nullptr)
            {
                POSITION posView{ pDocument->GetFirstViewPosition() };
                if (posView != nullptr)
                {
#pragma warning(suppress: 26429 26466)
                    auto pView{ static_cast<Scintilla::CScintillaView*>(pDocument->GetNextView(posView)) };
                    ASSERT(pView != nullptr);
                    // if (pView->IsKindOf(RUNTIME_CLASS(Scintilla::CScintillaView)))
                    {
                        const MSG* pMsg{ GetCurrentMessage() };
                        pView->GetCtrl().SendMessage(WM_SETTINGCHANGE, pMsg->wParam, pMsg->lParam);
                    }
                }
            }
        }
    }

    //Let the base class do its thing
    __super::OnSettingChange(uFlags, lpszSection);
}

#pragma warning(suppress: 26434)
void CMainFrame::OnSysColorChange()
{
    const CWinApp* pApp{ AfxGetApp() };
#pragma warning(suppress: 26496)
    AFXASSUME(pApp != nullptr);

    //Pass the message on to all scintilla control
    POSITION posTemplate{ pApp->GetFirstDocTemplatePosition() };
    if (posTemplate != nullptr)
    {
#pragma warning(suppress: 26429)
        const CDocTemplate* pTemplate{ pApp->GetNextDocTemplate(posTemplate) };
        ASSERT(pTemplate != nullptr);
        POSITION posDoc{ pTemplate->GetFirstDocPosition() };
        if (posDoc != nullptr)
        {
            const CDocument* pDocument{ pTemplate->GetNextDoc(posDoc) };
            if (pDocument != nullptr)
            {
                POSITION posView{ pDocument->GetFirstViewPosition() };
                if (posView != nullptr)
                {
#pragma warning(suppress: 26429 26466)
                    auto pView{ static_cast<Scintilla::CScintillaView*>(pDocument->GetNextView(posView)) };
                    ASSERT(pView != nullptr);
                    // if (pView->IsKindOf(RUNTIME_CLASS(Scintilla::CScintillaView)))
                    {
                        const MSG* pMsg{ GetCurrentMessage() };
                        pView->GetCtrl().SendMessage(WM_SYSCOLORCHANGE, pMsg->wParam, pMsg->lParam);
                    }
                }
            }
        }
    }

    //Let the base class do its thing
    __super::OnSysColorChange();
}

#pragma warning(suppress: 26434)
void CMainFrame::OnPaletteChanged(CWnd* pFocusWnd)
{
    const CWinApp* pApp{ AfxGetApp() };
#pragma warning(suppress: 26496)
    AFXASSUME(pApp != nullptr);

    //Pass the message on to all scintilla control
    POSITION posTemplate{ pApp->GetFirstDocTemplatePosition() };
    if (posTemplate != nullptr)
    {
#pragma warning(suppress: 26429)
        const CDocTemplate* pTemplate{ pApp->GetNextDocTemplate(posTemplate) };
        ASSERT(pTemplate != nullptr);
        POSITION posDoc{ pTemplate->GetFirstDocPosition() };
        if (posDoc != nullptr)
        {
            const CDocument* pDocument{ pTemplate->GetNextDoc(posDoc) };
            if (pDocument != nullptr)
            {
                POSITION posView{ pDocument->GetFirstViewPosition() };
                if (posView != nullptr)
                {
#pragma warning(suppress: 26429 26466)
                    auto pView{ static_cast<Scintilla::CScintillaView*>(pDocument->GetNextView(posView)) };
                    ASSERT(pView != nullptr);
                    // if (pView->IsKindOf(RUNTIME_CLASS(Scintilla::CScintillaView)))
                    {
                        const MSG* pMsg{ GetCurrentMessage() };
                        pView->GetCtrl().SendMessage(WM_PALETTECHANGED, pMsg->wParam, pMsg->lParam);
                    }
                }
            }
        }
    }

    //Let the base class do its thing
    __super::OnPaletteChanged(pFocusWnd);
}

#pragma warning(suppress: 26434)
BOOL CMainFrame::OnQueryNewPalette()
{
    const CWinApp* pApp{ AfxGetApp() };
#pragma warning(suppress: 26496)
    AFXASSUME(pApp);

    //Pass the message on to all scintilla control
    POSITION posTemplate{ pApp->GetFirstDocTemplatePosition() };
    if (posTemplate != nullptr)
    {
#pragma warning(suppress: 26429)
        const CDocTemplate* pTemplate{ pApp->GetNextDocTemplate(posTemplate) };
        ASSERT(pTemplate != nullptr);
        POSITION posDoc{ pTemplate->GetFirstDocPosition() };
        if (posDoc != nullptr)
        {
            const CDocument* pDocument{ pTemplate->GetNextDoc(posDoc) };
            if (pDocument != nullptr)
            {
                POSITION posView{ pDocument->GetFirstViewPosition() };
                if (posView != nullptr)
                {
#pragma warning(suppress: 26429 26466)
                    auto pView{ static_cast<Scintilla::CScintillaView*>(pDocument->GetNextView(posView)) };
                    ASSERT(pView != nullptr);
                    // if (pView->IsKindOf(RUNTIME_CLASS(Scintilla::CScintillaView)))
                    {
                        const MSG* pMsg{ GetCurrentMessage() };
                        pView->GetCtrl().SendMessage(WM_QUERYNEWPALETTE, pMsg->wParam, pMsg->lParam);
                    }
                }
            }
        }
    }

    //Let the base class do its thing
    return __super::OnQueryNewPalette();
}

#pragma warning(suppress: 26429)
void CMainFrame::OnUpdateInsert(CCmdUI* pCmdUI)
{
#pragma warning(suppress: 26486)
    pCmdUI->SetText(_T(""));
}

#pragma warning(suppress: 26429)
void CMainFrame::OnUpdateStyle(CCmdUI* pCmdUI)
{
#pragma warning(suppress: 26486)
    pCmdUI->SetText(_T(""));
}

#pragma warning(suppress: 26429)
void CMainFrame::OnUpdateFold(CCmdUI* pCmdUI)
{
#pragma warning(suppress: 26486)
    pCmdUI->SetText(_T(""));
}

#pragma warning(suppress: 26429)
void CMainFrame::OnUpdateLine(CCmdUI* pCmdUI)
{
#pragma warning(suppress: 26486)
    pCmdUI->SetText(_T(""));
}
