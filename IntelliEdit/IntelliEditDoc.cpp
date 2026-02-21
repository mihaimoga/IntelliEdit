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
#include "RenameDlg.h"
#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/**
 * @class CIntelliEditDoc
 * @brief Document class for IntelliEdit application.
 *
 * This class manages the document state and file operations for the IntelliEdit
 * text editor. It handles file manipulation operations such as rename, move, copy,
 * delete, and provides property access for the currently opened file.
 */

// CIntelliEditDoc

IMPLEMENT_DYNCREATE(CIntelliEditDoc, CScintillaDoc)

BEGIN_MESSAGE_MAP(CIntelliEditDoc, CScintillaDoc)
	ON_COMMAND(ID_FILE_SEND_MAIL, &CIntelliEditDoc::OnFileSendMail)
	ON_UPDATE_COMMAND_UI(ID_FILE_SEND_MAIL, &CIntelliEditDoc::OnUpdateFileSendMail)
	ON_COMMAND(ID_FILE_RENAME, &CIntelliEditDoc::OnFileRename)
	ON_UPDATE_COMMAND_UI(ID_FILE_RENAME, &CIntelliEditDoc::OnUpdateFileRename)
	ON_COMMAND(ID_FILE_MOVE, &CIntelliEditDoc::OnFileMove)
	ON_UPDATE_COMMAND_UI(ID_FILE_MOVE, &CIntelliEditDoc::OnUpdateFileMove)
	ON_COMMAND(ID_FILE_COPY, &CIntelliEditDoc::OnFileCopy)
	ON_UPDATE_COMMAND_UI(ID_FILE_COPY, &CIntelliEditDoc::OnUpdateFileCopy)
	ON_COMMAND(ID_FILE_DELETE, &CIntelliEditDoc::OnFileDelete)
	ON_UPDATE_COMMAND_UI(ID_FILE_DELETE, &CIntelliEditDoc::OnUpdateFileDelete)
	ON_COMMAND(ID_PROPERTIES, &CIntelliEditDoc::OnProperties)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES, &CIntelliEditDoc::OnUpdateProperties)
END_MESSAGE_MAP()

// CIntelliEditDoc construction/destruction

/**
 * @brief Default constructor for CIntelliEditDoc.
 *
 * Initializes a new instance of the document class.
 */
CIntelliEditDoc::CIntelliEditDoc() noexcept
{
	// TODO: add one-time construction code here

}

/**
 * @brief Destructor for CIntelliEditDoc.
 *
 * Cleans up resources used by the document.
 */
CIntelliEditDoc::~CIntelliEditDoc()
{
}

/**
 * @brief Called when a new document is created.
 *
 * Initializes the document for a new file. This is called when the user
 * creates a new document or when the SDI framework reuses this document.
 *
 * @return TRUE if the document was successfully initialized; FALSE otherwise.
 */
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

/**
 * @brief Draws a thumbnail representation of the document.
 *
 * This method is called by the Windows shell to generate a thumbnail
 * preview of the document content.
 *
 * @param dc Reference to the device context to draw on.
 * @param lprcBounds Pointer to the bounding rectangle for the thumbnail.
 */
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

/**
 * @brief Initializes the search content for the document.
 *
 * This method is called by the Windows shell search handler to extract
 * searchable content from the document. The content should be formatted
 * with semicolon separators between different parts.
 */
void CIntelliEditDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data.
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

/**
 * @brief Sets the search content for the document.
 *
 * Stores searchable text content that can be indexed by the Windows
 * shell search handler. If the value is empty, the search content
 * chunk is removed.
 *
 * @param value The search content string to store. Parts should be
 *              separated by semicolons.
 */
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
/**
 * @brief Validates the document object in debug builds.
 *
 * Performs assertion checks to verify the document object is in a
 * valid state. Called by the framework in debug builds.
 */
void CIntelliEditDoc::AssertValid() const
{
	CDocument::AssertValid();
}

/**
 * @brief Dumps the document object state to a dump context.
 *
 * Outputs diagnostic information about the document object for debugging
 * purposes. Called by the framework in debug builds.
 *
 * @param dc Reference to the dump context to write to.
 */
void CIntelliEditDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

// CIntelliEditDoc commands

/**
 * @brief Handles the File Rename command.
 *
 * Prompts the user to enter a new filename and renames the current document
 * file using the Windows shell API. The operation supports undo through the
 * Recycle Bin. After a successful rename, the document is reopened with the
 * new filename and the old document is closed.
 */
void CIntelliEditDoc::OnFileRename()
{
	// Get the name of the current file
	CString sOriginalPath{ GetPathName() };
	std::filesystem::path path(sOriginalPath.GetString());
	CString sPath{ path.filename().c_str() };

	// Bring up the rename dialog
	CRenameDlg dlg;
	dlg.m_sFilename = sPath;
	if (dlg.DoModal() == IDOK)
	{
		// Let the shell do the rename
		SHFILEOPSTRUCT shfo{};
		shfo.hwnd = AfxGetMainWnd()->GetSafeHwnd();
		shfo.wFunc = FO_RENAME;
		shfo.fFlags = FOF_ALLOWUNDO;
		CString sFrom{ sOriginalPath };
		const int nFromLength{ sFrom.GetLength() };
#pragma warning(suppress: 26472)
		std::vector<TCHAR> pszFrom{ static_cast<size_t>(nFromLength) + 2, std::allocator<TCHAR>{} };
		_tcscpy_s(pszFrom.data(), pszFrom.size(), sFrom);
#pragma warning(suppress: 26446 26472)
		pszFrom[static_cast<size_t>(nFromLength) + 1] = _T('\0');
		shfo.pFrom = pszFrom.data();
		path.replace_filename(dlg.m_sFilename.GetString());
		CString sTo{ path.c_str() };
		const int nToLength{ sTo.GetLength() };
#pragma warning(suppress: 26472)
		std::vector<TCHAR> pszTo{ static_cast<size_t>(nToLength) + 2, std::allocator<TCHAR>{} };
		_tcscpy_s(pszTo.data(), pszTo.size(), sTo);
#pragma warning(suppress: 26446 26472)
		pszTo[static_cast<size_t>(nToLength) + 1] = _T('\0');
		shfo.pTo = pszTo.data();

		// Let the shell perform the actual deletion
		const int nSuccess{ SHFileOperation(&shfo) };
		if (nSuccess == 0)
		{
			// Open the newly renamed file
			CDocTemplate* pTemplate{ GetDocTemplate() };
#pragma warning(suppress: 26496)
			AFXASSUME(pTemplate != nullptr);
			if (pTemplate->OpenDocumentFile(sTo, FALSE, TRUE) == nullptr)
				pTemplate->OpenDocumentFile(nullptr, FALSE, TRUE);
			// Since we have deleted the current file
			// move onto the next file in the directory
			GetView()->SendMessage(WM_COMMAND, ID_FILE_CLOSE, 0L);
		}
		/* else
		{
			// Reset this document to the current image before the failed rename above
			CDocTemplate* pTemplate{ GetDocTemplate() };
#pragma warning(suppress: 26496)
			AFXASSUME(pTemplate != nullptr);
			if (pTemplate->OpenDocumentFile(sPath, FALSE, TRUE) == nullptr)
				pTemplate->OpenDocumentFile(nullptr, FALSE, TRUE);
		} */
	}
}

/**
 * @brief Updates the UI state for the File Rename command.
 *
 * Enables the rename command only when a file is currently open and has
 * no unsaved modifications.
 *
 * @param pCmdUI Pointer to the command UI object to update.
 */
void CIntelliEditDoc::OnUpdateFileRename(CCmdUI* pCmdUI)
{
#pragma warning(suppress: 26486 26489)
	pCmdUI->Enable(!GetPathName().IsEmpty() && !IsModified());
}

/**
 * @brief Handles the File Move command.
 *
 * Prompts the user to select a destination folder and moves the current
 * document file to that location using the Windows shell API. The operation
 * supports undo through the Recycle Bin. After a successful move, the
 * document is reopened from the new location and the old document is closed.
 */
void CIntelliEditDoc::OnFileMove()
{
	// Get the name of the current file
	CString sPath{ GetPathName() };

	// Prompt the user for where they want to move the file to
	CString sTitle;
	if (!sTitle.LoadString(IDS_FOLDER_MOVE_TITLE))
		return;
	CString sDir;
	if (theApp.GetShellManager()->BrowseForFolder(sDir, AfxGetMainWnd(), nullptr, sTitle, BIF_RETURNONLYFSDIRS, nullptr))
	{
		// Ensure the directory has a "\" at its end
		const int nLen{ sDir.GetLength() };
		if (sDir[nLen - 1] != _T('\\'))
			sDir += _T("\\");

		// Let the shell do the move
		SHFILEOPSTRUCT shfo{};
		shfo.hwnd = AfxGetMainWnd()->GetSafeHwnd();
		shfo.wFunc = FO_MOVE;
		shfo.fFlags = FOF_ALLOWUNDO;
		CString sFrom{ sPath };
		const int nFromLength{ sFrom.GetLength() };
#pragma warning(suppress: 26472)
		std::vector<TCHAR> pszFrom{ static_cast<size_t>(nFromLength) + 2, std::allocator<TCHAR>{} };
		_tcscpy_s(pszFrom.data(), pszFrom.size(), sFrom);
#pragma warning(suppress: 26446 26472)
		pszFrom[static_cast<size_t>(nFromLength) + 1] = _T('\0');
		shfo.pFrom = pszFrom.data();
		std::filesystem::path path1{ sDir.GetString() };
		std::filesystem::path path2{ sPath.GetString() };
		path1.replace_filename(path2.filename());
		CString sTo{ path1.c_str() };
		const int nToLength{ sTo.GetLength() };
#pragma warning(suppress: 26472)
		std::vector<TCHAR> pszTo{ static_cast<size_t>(nToLength) + 2, std::allocator<TCHAR>{} };
		_tcscpy_s(pszTo.data(), pszTo.size(), sTo);
#pragma warning(suppress: 26446 26472)
		pszTo[static_cast<size_t>(nToLength) + 1] = _T('\0');
		shfo.pTo = pszTo.data();

		// Let the shell perform the actual deletion
		const int nSuccess{ SHFileOperation(&shfo) };
		if (nSuccess == 0)
		{
			// Open the newly renamed file
			CDocTemplate* pTemplate{ GetDocTemplate() };
#pragma warning(suppress: 26496)
			AFXASSUME(pTemplate != nullptr);
			if (pTemplate->OpenDocumentFile(sTo, FALSE, TRUE) == nullptr)
				pTemplate->OpenDocumentFile(nullptr, FALSE, TRUE);
			// Since we have deleted the current file
			// move onto the next file in the directory
			GetView()->SendMessage(WM_COMMAND, ID_FILE_CLOSE, 0L);
		}
		/* else
		{
			// Reset this document to the current image before the failed rename above
			CDocTemplate* pTemplate{ GetDocTemplate() };
#pragma warning(suppress: 26496)
			AFXASSUME(pTemplate != nullptr);
			if (pTemplate->OpenDocumentFile(sPath, FALSE, TRUE) == nullptr)
				pTemplate->OpenDocumentFile(nullptr, FALSE, TRUE);
		} */
	}
}

/**
 * @brief Updates the UI state for the File Move command.
 *
 * Enables the move command only when a file is currently open and has
 * no unsaved modifications.
 *
 * @param pCmdUI Pointer to the command UI object to update.
 */
void CIntelliEditDoc::OnUpdateFileMove(CCmdUI* pCmdUI)
{
#pragma warning(suppress: 26486 26489)
	pCmdUI->Enable(!GetPathName().IsEmpty() && !IsModified());
}

/**
 * @brief Handles the File Copy command.
 *
 * Prompts the user to select a destination folder and copies the current
 * document file to that location using the Windows shell API. The operation
 * supports undo through the Recycle Bin. After a successful copy, the
 * document path is updated to reflect the new location.
 */
void CIntelliEditDoc::OnFileCopy()
{
	// Get the name of the current file
	CString sPath{ GetPathName() };

	// Prompt the user for where they want to copy the file to
	CString sTitle;
	if (!sTitle.LoadString(IDS_FOLDER_COPY_TITLE))
		return;
	CString sDir;
	if (theApp.GetShellManager()->BrowseForFolder(sDir, AfxGetMainWnd(), nullptr, sTitle, BIF_RETURNONLYFSDIRS, nullptr))
	{
		// Ensure the directory has a "\" at its end
		const int nLen{ sDir.GetLength() };
		if (sDir[nLen - 1] != _T('\\'))
			sDir += _T("\\");

		// Let the shell do the copy
		SHFILEOPSTRUCT shfo{};
		shfo.hwnd = AfxGetMainWnd()->GetSafeHwnd();
		shfo.wFunc = FO_COPY;
		shfo.fFlags = FOF_ALLOWUNDO;
		CString sFrom{ sPath };
		const int nFromLength{ sFrom.GetLength() };
#pragma warning(suppress: 26472)
		std::vector<TCHAR> pszFrom{ static_cast<size_t>(nFromLength) + 2, std::allocator<TCHAR>{} };
		_tcscpy_s(pszFrom.data(), pszFrom.size(), sFrom);
#pragma warning(suppress: 26446 26472)
		pszFrom[static_cast<size_t>(nFromLength) + 1] = _T('\0');
		shfo.pFrom = pszFrom.data();
		std::filesystem::path path1{ sDir.GetString() };
		std::filesystem::path path2{ sPath.GetString() };
		path1.replace_filename(path2.filename());
		CString sTo{ path1.c_str() };
		const int nToLength{ sTo.GetLength() };
#pragma warning(suppress: 26472)
		std::vector<TCHAR> pszTo{ static_cast<size_t>(nToLength) + 2, std::allocator<TCHAR>{} };
		_tcscpy_s(pszTo.data(), pszTo.size(), sTo);
#pragma warning(suppress: 26446 26472)
		pszTo[static_cast<size_t>(nToLength) + 1] = _T('\0');
		shfo.pTo = pszTo.data();

		// Let the shell perform the actual copy
		const int nSuccess{ SHFileOperation(&shfo) };
		if (nSuccess == 0)
		{
			// Set the document title to the new filename
			SetPathName(sTo);
		}
	}
}

/**
 * @brief Updates the UI state for the File Copy command.
 *
 * Enables the copy command only when a file is currently open and has
 * no unsaved modifications.
 *
 * @param pCmdUI Pointer to the command UI object to update.
 */
void CIntelliEditDoc::OnUpdateFileCopy(CCmdUI* pCmdUI)
{
#pragma warning(suppress: 26486 26489)
	pCmdUI->Enable(!GetPathName().IsEmpty() && !IsModified());
}

/**
 * @brief Handles the File Delete command.
 *
 * Deletes the current document file using the Windows shell API. If the SHIFT
 * key is not held down, the operation supports undo through the Recycle Bin.
 * After successful deletion, the document is closed.
 *
 * @note If SHIFT is held down during the operation, the file is permanently
 *       deleted without being sent to the Recycle Bin.
 */
void CIntelliEditDoc::OnFileDelete()
{
	// Create a Multi SZ string with the file to delete
	CString sPath{ GetPathName() };
#pragma warning(suppress: 26472)
	const size_t nChars{ static_cast<size_t>(sPath.GetLength()) + 2 };

	// Let the shell do the delete
	SHFILEOPSTRUCT shfo{};
	const CWnd* pMainWnd{ AfxGetMainWnd() };
#pragma warning(suppress: 26496)
	AFXASSUME(pMainWnd != nullptr);
	shfo.hwnd = pMainWnd->GetSafeHwnd(); //NOLINT(clang-analyzer-core.CallAndMessage)
	shfo.wFunc = FO_DELETE;

	// Undo is not allowed if the SHIFT key is held down
	if (!(GetKeyState(VK_SHIFT) & 0x8000))
		shfo.fFlags = FOF_ALLOWUNDO;

	// Should we display any UI
	// if (!theApp.m_bAskMeOnDelete)
	shfo.fFlags |= FOF_NOCONFIRMATION;

	std::vector<TCHAR> pszFrom{ nChars, std::allocator<TCHAR>{} };
	_tcscpy_s(pszFrom.data(), pszFrom.size(), sPath);
#pragma warning(suppress: 26446)
	pszFrom[nChars - 1] = _T('\0');
	shfo.pFrom = pszFrom.data();

	// Let the shell perform the actual deletion
	const int nSuccess{ SHFileOperation(&shfo) };
	if (nSuccess == 0)
	{
		// Since we have deleted the current file
		// move onto the next file in the directory
		GetView()->SendMessage(WM_COMMAND, ID_FILE_CLOSE, 0L);
	}
}

/**
 * @brief Updates the UI state for the File Delete command.
 *
 * Enables the delete command only when a file is currently open and has
 * no unsaved modifications.
 *
 * @param pCmdUI Pointer to the command UI object to update.
 */
void CIntelliEditDoc::OnUpdateFileDelete(CCmdUI* pCmdUI)
{
#pragma warning(suppress: 26486 26489)
	pCmdUI->Enable(!GetPathName().IsEmpty() && !IsModified());
}

/**
 * @brief Handles the Properties command.
 *
 * Opens the Windows shell properties dialog for the current document file.
 * This displays file system information such as size, attributes, dates,
 * and security settings.
 */
void CIntelliEditDoc::OnProperties()
{
	CString sPathName{ GetPathName() };
	SHELLEXECUTEINFO sei;
	memset(&sei, 0, sizeof(sei));
	sei.cbSize = sizeof(sei);
	sei.hwnd = AfxGetMainWnd()->GetSafeHwnd();
	sei.nShow = SW_SHOW;
	sei.lpFile = sPathName.GetBuffer(sPathName.GetLength());
	sei.lpVerb = _T("properties");
	sei.fMask = SEE_MASK_INVOKEIDLIST;
#pragma warning(suppress: 26486)
	ShellExecuteEx(&sei);
	sPathName.ReleaseBuffer();
}

/**
 * @brief Updates the UI state for the Properties command.
 *
 * Enables the properties command only when a file is currently open.
 *
 * @param pCmdUI Pointer to the command UI object to update.
 */
void CIntelliEditDoc::OnUpdateProperties(CCmdUI* pCmdUI)
{
#pragma warning(suppress: 26486 26489)
	pCmdUI->Enable(!GetPathName().IsEmpty());
}
