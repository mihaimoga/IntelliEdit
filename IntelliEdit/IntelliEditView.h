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

// IntelliEditView.h : interface of the CIntelliEditView class
//

#pragma once

#include "ScintillaDocView.h"

class CScintillaDemoCtrl : public Scintilla::CScintillaCtrl
{
};

class CIntelliEditView : public Scintilla::CScintillaView
{
protected: // create from serialization only
	CIntelliEditView() noexcept;
	DECLARE_DYNCREATE(CIntelliEditView)

// Attributes
public:
	CIntelliEditDoc* GetDocument() const;
protected:
	//Member variables
	Scintilla::ILexer5* m_batLexer;
	Scintilla::ILexer5* m_cppLexer;
	Scintilla::ILexer5* m_cssLexer;
	Scintilla::ILexer5* m_htmlLexer;
	Scintilla::ILexer5* m_jsonLexer;
	Scintilla::ILexer5* m_makLexer;
	Scintilla::ILexer5* m_matLexer;
	Scintilla::ILexer5* m_mdLexer;
	Scintilla::ILexer5* m_pyLexer;
	Scintilla::ILexer5* m_rLexer;
	Scintilla::ILexer5* m_rsLexer;
	Scintilla::ILexer5* m_shLexer;
	Scintilla::ILexer5* m_sqlLexer;
	Scintilla::ILexer5* m_xmlLexer;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	void OnInitialUpdate() override;

// Implementation
public:
	virtual ~CIntelliEditView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//Methods
	void OnCharAdded(_Inout_ Scintilla::NotificationData* pSCNotification) override;
	void OnDwellStart(_Inout_ Scintilla::NotificationData* pSCNotification) override;
	void OnDwellEnd(_Inout_ Scintilla::NotificationData* pSCNotification) override;
	void OnModifyAttemptRO(_Inout_ Scintilla::NotificationData* pSCNotification) override;
	void OnModified(_Inout_ Scintilla::NotificationData* pSCNotification) override;
	std::unique_ptr<Scintilla::CScintillaCtrl> CreateScintillaControl() override;
	void SetAStyle(int style, COLORREF fore, COLORREF back = RGB(0xff, 0xff, 0xff), int size = -1, const char* face = nullptr);
	void DefineMarker(int marker, Scintilla::MarkerSymbol markerType, COLORREF fore, COLORREF back);

	//Message handlers
	afx_msg void OnOptionsAddmarker();
	afx_msg void OnOptionsDeletemarker();
	afx_msg void OnUpdateOptionsDeletemarker(CCmdUI* pCmdUI);
	afx_msg void OnOptionsFindNextmarker();
	afx_msg void OnOptionsFindPrevmarker();
	afx_msg void OnOptionsFoldMargin();
	afx_msg void OnOptionsSelectionMargin();
	afx_msg void OnUpdateOptionsSelectionMargin(CCmdUI* pCmdUI);
	afx_msg void OnOptionsViewLinenumbers();
	afx_msg void OnUpdateOptionsViewLinenumbers(CCmdUI* pCmdUI);
	afx_msg void OnUpdateOptionsAddmarker(CCmdUI* pCmdUI);
	afx_msg void OnUpdateOptionsFoldMargin(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLine(CCmdUI* pCmdUI);
	afx_msg void OnUpdateStyle(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFold(CCmdUI* pCmdUI);
	afx_msg void OnUpdateInsert(CCmdUI* pCmdUI);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in IntelliEditView.cpp
inline CIntelliEditDoc* CIntelliEditView::GetDocument() const
   { return reinterpret_cast<CIntelliEditDoc*>(m_pDocument); }
#endif
