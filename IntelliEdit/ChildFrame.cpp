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

// ChildFrame.cpp : implementation of the CChildFrame class
//

#include "pch.h"
#include "framework.h"
#include "IntelliEdit.h"
#include "ChildFrame.h"
#include "IntelliEditDoc.h"
#include "IntelliEditView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWndEx)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWndEx)
	ON_COMMAND(ID_FILE_PRINT, &CChildFrame::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CChildFrame::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CChildFrame::OnFilePrintPreview)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_PREVIEW, &CChildFrame::OnUpdateFilePrintPreview)
	ON_WM_MOVE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CChildFrame construction/destruction

CChildFrame::CChildFrame() noexcept
{
	// TODO: add member initialization code here
}

CChildFrame::~CChildFrame()
{
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying the CREATESTRUCT cs
	if( !CMDIChildWndEx::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}

// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWndEx::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWndEx::Dump(dc);
}
#endif //_DEBUG

// CChildFrame message handlers

void CChildFrame::OnFilePrint()
{
	if (m_dockManager.IsPrintPreviewValid())
	{
		PostMessage(WM_COMMAND, AFX_ID_PREVIEW_PRINT);
	}
}

void CChildFrame::OnFilePrintPreview()
{
	if (m_dockManager.IsPrintPreviewValid())
	{
		PostMessage(WM_COMMAND, AFX_ID_PREVIEW_CLOSE);  // force Print Preview mode closed
	}
}

void CChildFrame::OnUpdateFilePrintPreview(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_dockManager.IsPrintPreviewValid());
}
#pragma warning(suppress: 26434)
void CChildFrame::OnMove(int x, int y)
{
	//Let the base class do its thing
	__super::OnMove(x, y);

#pragma warning(suppress: 26466)
	auto pView{ static_cast<CIntelliEditView*>(GetActiveView()) };
	if (pView != nullptr)
	{
		if (pView->IsKindOf(RUNTIME_CLASS(CIntelliEditView)))
		{
			auto& rCtrl{ pView->GetCtrl() };

			//Cancel any outstanding call tip
			if (rCtrl.CallTipActive())
				rCtrl.CallTipCancel();
		}
	}
}

#pragma warning(suppress: 26434)
void CChildFrame::OnSize(UINT nType, int cx, int cy)
{
	//Let the base class do its thing
	__super::OnSize(nType, cx, cy);

#pragma warning(suppress: 26466)
	auto pView{ static_cast<CIntelliEditView*>(GetActiveView()) };
	if (pView != nullptr)
	{
		if (pView->IsKindOf(RUNTIME_CLASS(CIntelliEditView)))
		{
			auto& rCtrl{ pView->GetCtrl() };

			//Cancel any outstanding call tip
			if (rCtrl.CallTipActive())
				rCtrl.CallTipCancel();
		}
	}
}
