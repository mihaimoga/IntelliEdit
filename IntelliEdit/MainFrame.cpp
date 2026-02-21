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

// MainFrame.cpp : implementation of the CMainFrame class
//

#include "pch.h"
#include "framework.h"
#include "IntelliEdit.h"
#include "ScintillaDocView.h"

#include "MainFrame.h"
#include "WebBrowserDlg.h"
#include "CheckForUpdatesDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/**
 * @class CMainFrame
 * @brief Main frame window for the IntelliEdit MDI application.
 *
 * This class implements the main application window using the MDI (Multiple Document
 * Interface) framework. It manages the ribbon bar, status bar, tabbed MDI groups,
 * and provides handlers for application commands, social media links, and system
 * notifications that need to be propagated to all open documents.
 */

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
    ON_COMMAND(IDC_USER_MANUAL, &CMainFrame::OnUserManual)
    ON_COMMAND(IDC_CHECK_FOR_UPDATES, &CMainFrame::OnCheckForUpdates)
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

/**
 * @brief Status bar indicator IDs.
 *
 * Array defining the indicators shown in the status bar, including:
 * - Status line separator
 * - Fold level indicator
 * - Style indicator
 * - Line/column indicator
 * - Insert/overtype indicator
 * - Caps Lock indicator
 * - Num Lock indicator
 * - Scroll Lock indicator
 */
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

/**
 * @brief Default constructor for CMainFrame.
 *
 * Initializes the main frame window. Member initialization is handled
 * in the initializer list or by default constructors.
 */
// CMainFrame construction/destruction

CMainFrame::CMainFrame() noexcept
{
	// TODO: add member initialization code here
}

/**
 * @brief Destructor for CMainFrame.
 *
 * Cleans up resources used by the main frame window.
 */
CMainFrame::~CMainFrame()
{
}

/**
 * @brief Handles main frame window creation.
 *
 * Creates and initializes the main window components including:
 * - MDI tabbed groups with customizable styles
 * - Ribbon bar loaded from resources
 * - Status bar with indicators
 * - Docking manager with Visual Studio 2005 style behavior
 * - Auto-hide panes support
 * - Enhanced windows management dialog
 * - Document name prefix in window title for better taskbar usability
 *
 * @param lpCreateStruct Pointer to the window creation structure.
 * @return 0 on success; -1 on failure.
 */
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

/**
 * @brief Called before window creation.
 *
 * Allows modification of window class or styles before the window is created.
 *
 * @param cs Reference to the CREATESTRUCT containing window creation parameters.
 * @return TRUE if the window should be created; FALSE otherwise.
 */
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/**
 * @brief Validates the frame object in debug builds.
 *
 * Performs assertion checks to verify the frame object is in a valid state.
 */
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWndEx::AssertValid();
}

/**
 * @brief Dumps the frame object state for debugging.
 *
 * Outputs diagnostic information about the frame object.
 *
 * @param dc Reference to the dump context to write to.
 */
void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWndEx::Dump(dc);
}
#endif //_DEBUG

/**
 * @brief Handles the Window Manager command.
 *
 * Displays the enhanced windows management dialog that allows users to
 * manage open MDI child windows.
 */
// CMainFrame message handlers

void CMainFrame::OnWindowManager()
{
	ShowWindowsDialog();
}

/**
 * @brief Handles the Twitter social media link command.
 *
 * Opens the developer's Twitter/X profile in the default web browser.
 */
void CMainFrame::OnTwitter()
{
	::ShellExecute(GetSafeHwnd(), _T("open"), _T("https://x.com/stefanmihaimoga"), nullptr, nullptr, SW_SHOW);
}

/**
 * @brief Handles the LinkedIn social media link command.
 *
 * Opens the developer's LinkedIn profile in the default web browser.
 */
void CMainFrame::OnLinkedin()
{
	::ShellExecute(GetSafeHwnd(), _T("open"), _T("https://www.linkedin.com/in/stefanmihaimoga/"), nullptr, nullptr, SW_SHOW);
}

/**
 * @brief Handles the Facebook social media link command.
 *
 * Opens the developer's Facebook profile in the default web browser.
 */
void CMainFrame::OnFacebook()
{
	::ShellExecute(GetSafeHwnd(), _T("open"), _T("https://www.facebook.com/stefanmihaimoga"), nullptr, nullptr, SW_SHOW);
}

/**
 * @brief Handles the Instagram social media link command.
 *
 * Opens the developer's Instagram profile in the default web browser.
 */
void CMainFrame::OnInstagram()
{
	::ShellExecute(GetSafeHwnd(), _T("open"), _T("https://www.instagram.com/stefanmihaimoga/"), nullptr, nullptr, SW_SHOW);
}

/**
 * @brief Handles the GitHub Issues link command.
 *
 * Opens the IntelliEdit GitHub issues page in the default web browser,
 * where users can report bugs and request features.
 */
void CMainFrame::OnIssues()
{
	::ShellExecute(GetSafeHwnd(), _T("open"), _T("https://github.com/mihaimoga/IntelliEdit/issues"), nullptr, nullptr, SW_SHOW);
}

/**
 * @brief Handles the GitHub Discussions link command.
 *
 * Opens the IntelliEdit GitHub discussions page in the default web browser,
 * where users can participate in community discussions.
 */
void CMainFrame::OnDiscussions()
{
	::ShellExecute(GetSafeHwnd(), _T("open"), _T("https://github.com/mihaimoga/IntelliEdit/discussions"), nullptr, nullptr, SW_SHOW);
}

/**
 * @brief Handles the GitHub Wiki link command.
 *
 * Opens the IntelliEdit GitHub wiki page in the default web browser,
 * providing access to documentation and guides.
 */
void CMainFrame::OnWiki()
{
	::ShellExecute(GetSafeHwnd(), _T("open"), _T("https://github.com/mihaimoga/IntelliEdit/wiki"), nullptr, nullptr, SW_SHOW);
}

/**
 * @brief Handles the User Manual command.
 *
 * Displays the web browser dialog showing the application's user manual
 * and documentation.
 */
void CMainFrame::OnUserManual()
{
    CWebBrowserDlg dlgWebBrowser(this);
    dlgWebBrowser.DoModal();
}

/**
 * @brief Handles the Check for Updates command.
 *
 * Displays the update checker dialog that checks for newer versions
 * of IntelliEdit and provides download links if updates are available.
 */
void CMainFrame::OnCheckForUpdates()
{
    CCheckForUpdatesDlg dlgCheckForUpdates(this);
    dlgCheckForUpdates.DoModal();
}

/**
 * @brief Handles system settings changes.
 *
 * Propagates WM_SETTINGCHANGE messages to all open Scintilla editor controls
 * so they can update their appearance and behavior in response to system-wide
 * setting changes (e.g., theme changes, DPI changes).
 *
 * @param uFlags Flags indicating which system parameter changed.
 * @param lpszSection Name of the system parameter section that changed.
 */
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

/**
 * @brief Handles system color changes.
 *
 * Propagates WM_SYSCOLORCHANGE messages to all open Scintilla editor controls
 * so they can update their colors when the system color scheme changes.
 */
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

/**
 * @brief Handles palette changes.
 *
 * Propagates WM_PALETTECHANGED messages to all open Scintilla editor controls
 * when the system palette changes. This is important for proper color rendering
 * on systems with limited color depth.
 *
 * @param pFocusWnd Pointer to the window that changed the palette.
 */
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

/**
 * @brief Handles palette query requests.
 *
 * Propagates WM_QUERYNEWPALETTE messages to all open Scintilla editor controls
 * when the system needs to realize a new palette. Returns TRUE if the palette
 * was realized.
 *
 * @return TRUE if the palette was realized; FALSE otherwise.
 */
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

/**
 * @brief Updates the insert/overtype indicator in the status bar.
 *
 * Clears the indicator text. The actual state is displayed by the view.
 *
 * @param pCmdUI Pointer to the command UI object to update.
 */
#pragma warning(suppress: 26429)
void CMainFrame::OnUpdateInsert(CCmdUI* pCmdUI)
{
#pragma warning(suppress: 26486)
    pCmdUI->SetText(_T(""));
}

/**
 * @brief Updates the style indicator in the status bar.
 *
 * Clears the indicator text. The actual style is displayed by the view.
 *
 * @param pCmdUI Pointer to the command UI object to update.
 */
#pragma warning(suppress: 26429)
void CMainFrame::OnUpdateStyle(CCmdUI* pCmdUI)
{
#pragma warning(suppress: 26486)
    pCmdUI->SetText(_T(""));
}

/**
 * @brief Updates the fold level indicator in the status bar.
 *
 * Clears the indicator text. The actual fold level is displayed by the view.
 *
 * @param pCmdUI Pointer to the command UI object to update.
 */
#pragma warning(suppress: 26429)
void CMainFrame::OnUpdateFold(CCmdUI* pCmdUI)
{
#pragma warning(suppress: 26486)
    pCmdUI->SetText(_T(""));
}

/**
 * @brief Updates the line/column indicator in the status bar.
 *
 * Clears the indicator text. The actual line and column numbers are
 * displayed by the view.
 *
 * @param pCmdUI Pointer to the command UI object to update.
 */
#pragma warning(suppress: 26429)
void CMainFrame::OnUpdateLine(CCmdUI* pCmdUI)
{
#pragma warning(suppress: 26486)
    pCmdUI->SetText(_T(""));
}
