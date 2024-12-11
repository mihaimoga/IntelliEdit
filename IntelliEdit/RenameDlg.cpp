/*
Module : RenameDlg.cpp
Purpose: Implements the rename dialog for EZView

Copyright (c) 2000 - 2022 by PJ Naughter (Web: www.naughter.com, Email: pjna@naughter.com)

All rights reserved.

Copyright / Usage Details:

You are allowed to include the source code in any product (commercial, shareware, freeware or otherwise)
when your product is released in binary form. You are allowed to modify the source code in any way you want
except you cannot modify the copyright details at the top of each module. If you want to distribute source
code with your application, then you are only allowed to distribute versions released by the author. This is
to maintain a single distribution point for the source code.

*/


//////////////////// Includes /////////////////////////////////////////////////

#include "pch.h"
#include "IntelliEdit.h"
#include "RenameDlg.h"


//////////////////// Macros / Defines /////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#endif //#ifdef _DEBUG


//////////////////// Implementation ///////////////////////////////////////////

#pragma warning(suppress: 26455)
CRenameDlg::CRenameDlg(_In_opt_ CWnd* pParent) : CDialog{ CRenameDlg::IDD, pParent }
{
}

#pragma warning(suppress: 26429)
void CRenameDlg::DoDataExchange(CDataExchange* pDX)
{
	//Let the base class do its thing
	__super::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_FILENAME, m_sFilename);

	if (pDX->m_bSaveAndValidate && m_sFilename.IsEmpty())
	{
		pDX->PrepareEditCtrl(IDC_FILENAME);
		AfxMessageBox(AFX_IDP_INVALID_FILENAME);
		pDX->Fail();
	}
}

#pragma warning(suppress: 26433 26440)
BEGIN_MESSAGE_MAP(CRenameDlg, CDialog) //NOLINT(clang-diagnostic-unused-local-typedef, modernize-avoid-c-arrays)
END_MESSAGE_MAP()
