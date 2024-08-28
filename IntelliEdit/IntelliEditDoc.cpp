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

// IntelliEditDoc.cpp : implementation of the CIntelliEditDoc class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "IntelliEdit.h"
#endif

#include "IntelliEditDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CIntelliEditDoc

IMPLEMENT_DYNCREATE(CIntelliEditDoc, CScintillaDoc)

BEGIN_MESSAGE_MAP(CIntelliEditDoc, CScintillaDoc)
	ON_COMMAND(ID_FILE_SEND_MAIL, &CIntelliEditDoc::OnFileSendMail)
	ON_UPDATE_COMMAND_UI(ID_FILE_SEND_MAIL, &CIntelliEditDoc::OnUpdateFileSendMail)
END_MESSAGE_MAP()

// CIntelliEditDoc construction/destruction

CIntelliEditDoc::CIntelliEditDoc() noexcept
{
	// TODO: add one-time construction code here

}

CIntelliEditDoc::~CIntelliEditDoc()
{
}

BOOL CIntelliEditDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}

// CIntelliEditDoc serialization

/*void CIntelliEditDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}*/

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CIntelliEditDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CIntelliEditDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data.
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CIntelliEditDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CIntelliEditDoc diagnostics

#ifdef _DEBUG
void CIntelliEditDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CIntelliEditDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

// CIntelliEditDoc commands
