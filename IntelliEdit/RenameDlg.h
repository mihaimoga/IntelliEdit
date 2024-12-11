/*
Module : RenameDlg.h
Purpose: Defines the rename dialog for EZView

Copyright (c) 2000 - 2022 by PJ Naughter (Web: www.naughter.com, Email: pjna@naughter.com)

All rights reserved.

Copyright / Usage Details:

You are allowed to include the source code in any product (commercial, shareware, freeware or otherwise)
when your product is released in binary form. You are allowed to modify the source code in any way you want
except you cannot modify the copyright details at the top of each module. If you want to distribute source
code with your application, then you are only allowed to distribute versions released by the author. This is
to maintain a single distribution point for the source code.

*/

//////////////////// Macros / Defines /////////////////////////////////////////

#pragma once

//////////////////// Classes //////////////////////////////////////////////////

class CRenameDlg : public CDialog
{
public:
	//Constructors / Destructors
	CRenameDlg(_In_opt_ CWnd* pParent = nullptr);
	CRenameDlg(const CRenameDlg&) = delete;
	CRenameDlg(CRenameDlg&&) = delete;
	~CRenameDlg() = default; //NOLINT(modernize-use-override)

	//Methods
	CRenameDlg& operator=(const CRenameDlg&) = delete;
	CRenameDlg& operator=(CRenameDlg&&) = delete;

	//Member variables
	enum { IDD = IDD_RENAME };
	CString m_sFilename;

protected:
	//Methods
	void DoDataExchange(CDataExchange* pDX) override;

	DECLARE_MESSAGE_MAP()
};
