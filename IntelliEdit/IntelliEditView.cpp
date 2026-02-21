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

/**
 * @brief CMake language keywords for syntax highlighting.
 *
 * Contains all CMake commands including scripting commands, project commands,
 * and CTest commands. Used by the CMake lexer for keyword highlighting.
 * @see https://cmake.org/cmake/help/latest/manual/cmake-commands.7.html
 */
const TCHAR* g_cmakeKeywords
{
	/* https://cmake.org/cmake/help/latest/manual/cmake-commands.7.html */
	_T("block break cmake_host_system_information cmake_language cmake_minimum_required ")
	_T("cmake_parse_arguments cmake_path cmake_policy configure_file continue ")
	_T("else elseif endblock endforeach endfunction endif endmacro endwhile ")
	_T("execute_process file find_file find_library find_package find_path ")
	_T("find_program foreach function get_cmake_property get_directory_property ")
	_T("get_filename_component get_property if include include_guard list macro ")
	_T("mark_as_advanced math message option return separate_arguments set ")
	_T("set_directory_properties set_property site_name string unset variable_watch while ")
	// Project
	_T("add_compile_definitions add_compile_options add_custom_command add_custom_target ")
	_T("add_definitions add_dependencies add_executable add_library add_link_options ")
	_T("add_subdirectory add_test aux_source_directory build_command cmake_file_api ")
	_T("create_test_sourcelist define_property enable_language enable_testing export ")
	_T("fltk_wrap_ui get_source_file_property get_target_property get_test_property ")
	_T("include_directories include_external_msproject include_regular_expression ")
	_T("install link_directories link_libraries load_cache project remove_definitions ")
	_T("set_source_files_properties set_target_properties set_tests_properties ")
	_T("source_group target_compile_definitions target_compile_features ")
	_T("target_compile_options target_include_directories target_link_directories ")
	_T("target_link_libraries target_link_options target_precompile_headers ")
	_T("target_sources try_compile try_run ctest_build ctest_configure ctest_coverage ")
	_T("ctest_empty_binary_directory ctest_memcheck ctest_read_custom_files ctest_run_script ")
	_T("ctest_sleep ctest_start ctest_submit ctest_test ctest_update ctest_upload")
};

/**
 * @brief C++ language keywords for syntax highlighting.
 *
 * Contains standard C++ keywords including control structures, data types,
 * and operators. Used by the C++ lexer for keyword highlighting.
 * @see https://www.w3schools.com/cpp/cpp_ref_keywords.asp
 */
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

/**
 * @brief C# language keywords for syntax highlighting.
 *
 * Contains C# keywords including type definitions, modifiers, and control
 * structures. Used by the C# lexer for keyword highlighting.
 */
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

/**
 * @brief Groovy language keywords for syntax highlighting.
 *
 * Contains Groovy keywords for the Apache Groovy programming language.
 * Used by the Groovy lexer for keyword highlighting.
 * @see https://docs.oracle.com/cloud/latest/big-data-discovery-cloud/BDDDE/rsu_transform_unsupported_features.htm
 */
const TCHAR* g_groovyKeywords
{
	/* https://docs.oracle.com/cloud/latest/big-data-discovery-cloud/BDDDE/rsu_transform_unsupported_features.htm */
	_T("abstract as assert boolean break byte case catch char class const ")
	_T("continue def default do double else enum extends false final finally ")
	_T("float for goto if implements import in instanceof int interface long ")
	_T("native new null package private protected public return short static ")
	_T("strictfp super switch synchronized this threadsafe throw throws ")
	_T("transient true try void volatile while")
};

/**
 * @brief Java language keywords for syntax highlighting.
 *
 * Contains Java keywords including all standard Java language keywords
 * and modifiers. Used by the Java lexer for keyword highlighting.
 * @see https://www.w3schools.com/java/java_ref_keywords.asp
 */
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

/**
 * @brief JavaScript language keywords for syntax highlighting.
 *
 * Contains JavaScript/ECMAScript keywords including ES6+ features.
 * Used by the JavaScript lexer for keyword highlighting.
 * @see https://www.w3schools.com/js/js_reserved.asp
 */
const TCHAR* g_jsKeywords
{
	/* https://www.w3schools.com/js/js_reserved.asp */
	_T("abstract arguments await boolean break byte case catch char class const ")
	_T("continue debugger default delete do double else enum eval export extends ")
	_T("false final finally float for function goto if implements import in ")
	_T("instanceof int interface let long native new null package private ")
	_T("protected public return short static super switch synchronized this throw ")
	_T("throws transient true try typeof var void volatile while with yield")
};

/**
 * @brief MATLAB language keywords for syntax highlighting.
 *
 * Contains MATLAB keywords for control flow and function definitions.
 * Used by the MATLAB lexer for keyword highlighting.
 */
const TCHAR* g_matlabKeywords
{
	_T("break case catch classdef continue else elseif end for function global ")
	_T("if otherwise parfor persistent return spmd switch try while")
};

/**
 * @brief Python language keywords for syntax highlighting.
 *
 * Contains Python keywords including all standard Python 3 keywords.
 * Used by the Python lexer for keyword highlighting.
 * @see https://www.w3schools.com/python/python_ref_keywords.asp
 */
const TCHAR* g_pyKeywords
{
	/* https://www.w3schools.com/python/python_ref_keywords.asp */
	_T("and as assert break class continue def del elif else except False finally ")
	_T("for from global if import in is lambda None nonlocal not or pass raise ")
	_T("return True try while with yield")
};

/**
 * @brief R language keywords for syntax highlighting.
 *
 * Contains R statistical computing language keywords and special constants.
 * Used by the R lexer for keyword highlighting.
 */
const TCHAR* g_rKeywords
{
	_T("if else repeat while function for next break TRUE FALSE NULL Inf NaN NA ")
	_T("NA_integer_ NA_real_ NA_complex_ NA_character_")
};

/**
 * @brief Rust language keywords for syntax highlighting.
 *
 * Contains Rust keywords including strict and reserved keywords.
 * Used by the Rust lexer for keyword highlighting.
 * @see https://doc.rust-lang.org/reference/keywords.html
 */
const TCHAR* g_rsKeywords
{
	/* https://doc.rust-lang.org/reference/keywords.html */
	_T("as break const continue crate else enum extern false fn for if impl in ")
	_T("let loop match mod move mut pub ref return self static struct super trait ")
	_T("true type unsafe use where while async await dyn abstract become box do ")
	_T("final macro override priv typeof unsized virtual yield try macro_rules ")
	_T("union dyn")
};

/**
 * @brief SQL language keywords for syntax highlighting.
 *
 * Contains SQL keywords including standard SQL commands, MySQL data types,
 * string functions, numeric functions, date functions, and advanced functions.
 * Used by the SQL lexer for keyword highlighting.
 * @see https://www.w3schools.com/sql/sql_ref_keywords.asp
 */
const TCHAR* g_sqlKeywords
{
	/* https://www.w3schools.com/sql/sql_ref_keywords.asp */
	_T("add constraint all alter all and any as asc backup between by case check ")
	_T("column constraint create database default delete desc distinct drop exec ")
	_T("exists foreign from full group having if in into index inner insert is join ")
	_T("left like limit not null or order outer primary key procedure replace right ")
	_T("rownum select set table top truncate union unique update values view where ")
	/* MySQL Data Types */
	_T("char varchar binary varbinary tinyblob tinytext text blob mediumtext ")
	_T("mediumblob longtext longblob enum set bit tinyint bool boolean smallin ")
	_T("mediumint int integer bigint float double precision decimal dec date ")
	_T("datetime timestamp time year references ")
	/* MySQL String Functions */
	_T("ascii char_length character_length concat concat_ws field find_in_set ")
	_T("format instr lcase left length locate lower lpad ltrim mid position ")
	_T("repeat replace reverse right rpad rtrim space strcmp substr substring ")
	_T("substring_index trim ucase upper ")
	/* MySQL Numeric Functions */
	_T("abs acos asin atan atan2 avg ceil ceiling cos cot count degrees div exp ")
	_T("floor greatest least ln log log10 log2 max min mod pi pow power radians ")
	_T("rand round sign sin sqrt sum tan truncate ")
	/* MySQL Date Functions */
	_T("adddate addtime curdate current_date current_time current_timestamp curtime ")
	_T("date datediff date_add date_format date_sub day dayname dayofmonth dayofweek ")
	_T("dayofyear extract from_days hour last_day localtime localtimestamp makedate ")
	_T("maketime microsecond minute month monthname now period_add period_diff quarter ")
	_T("second sec_to_time str_to_date subdate subtime sysdate time time_format ")
	_T("time_to_sec timediff timestamp to_days week weekday weekofyear year yearweek ")
	/* MySQL Advanced Functions */
	_T("bin case cast coalesce connection_id conv convert current_user database ifnull ")
	_T("isnull last_insert_id nullif session_user system_user user version")
};

/**
 * @brief TCL/TK language keywords for syntax highlighting.
 *
 * Contains TCL scripting language keywords including Tcl commands and
 * Tk widget commands. Used by the TCL lexer for keyword highlighting.
 * @see https://www.tcl-lang.org/man/tcl8.5/UserCmd/contents.htm
 */
const TCHAR* g_tclKeywords
{
	/* https://www.tcl-lang.org/man/tcl8.5/UserCmd/contents.htm */
	_T("tclsh wish ")
	/* Tcl commands */
	_T("after append apply array auto_execok auto_import auto_load auto_mkindex ")
	_T("auto_mkindex_old auto_qualify auto_reset bgerror binary break catch cd ")
	_T("chan clock close concat continue dde dict encoding eof error eval exec ")
	_T("exit expr fblocked fconfigure fcopy file fileevent filename flush for ")
	_T("foreach format gets glob global history http if incr info interp join ")
	_T("lappend lassign lindex linsert list llength load lrange lrepeat lreplace ")
	_T("lreverse lsearch lset lsort mathfunc mathop memory msgcat namespace open ")
	_T("package parray pid pkg::create pkg_mkIndex platform platform::shell proc ")
	_T("puts pwd re_syntax read refchan regexp registry regsub rename return ")
	_T("Safe Base scan seek set socket source split string subst switch Tcl ")
	_T("tcl_endOfWord tcl_findLibrary tcl_startOfNextWord tcl_startOfPreviousWord ")
	_T("tcl_wordBreakAfter tcl_wordBreakBefore tcltest tclvars tell time tm trace ")
	_T("unknown unload unset update uplevel upvar variable vwait while ")
	/* Tk commands */
	_T("bell bind bindtags bitmap button canvas checkbutton clipboard colors ")
	_T("console cursors destroy entry event focus font frame grab grid image ")
	_T("keysyms label labelframe listbox loadTk lower menu menubutton message ")
	_T("option options pack panedwindow photo place radiobutton raise scale ")
	_T("scrollbar selection send spinbox text tk tk::mac tk_bisque tk_chooseColor ")
	_T("tk_chooseDirectory tk_dialog tk_focusFollowsMouse tk_focusNext tk_focusPrev ")
	_T("tk_getOpenFile tk_getSaveFile tk_menuSetFocus tk_messageBox tk_optionMenu ")
	_T("tk_popup tk_setPalette tk_textCopy tk_textCut tk_textPaste tkerror tkvars ")
	_T("tkwait toplevel ttk::button ttk::checkbutton ttk::combobox ttk::entry ")
	_T("ttk::frame ttk::intro ttk::label ttk::labelframe ttk::menubutton ")
	_T("ttk::notebook ttk::panedwindow ttk::progressbar ttk::radiobutton ")
	_T("ttk::scale ttk::scrollbar ttk::separator ttk::sizegrip ttk::spinbox ")
	_T("ttk::style ttk::treeview ttk::widget ttk_image ttk_vsapi winfo wm ")
};

/**
 * @class CIntelliEditView
 * @brief View class for IntelliEdit text editor.
 *
 * This class provides the main text editing view using the Scintilla editor
 * control. It handles syntax highlighting for multiple programming languages,
 * manages editor margins, markers, folding, and provides auto-completion
 * and call tip functionality.
 */

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

/**
 * @brief Default constructor for CIntelliEditView.
 *
 * Initializes all lexer pointers to nullptr and sets initial state for
 * lexer keywords flag. Loads margin settings from configuration.
 */
// CIntelliEditView construction/destruction

CIntelliEditView::CIntelliEditView() noexcept :
	m_batLexer { nullptr },
	m_cmakeLexer { nullptr },
	m_cppLexer { nullptr },
	m_cssLexer { nullptr },
	m_htmlLexer { nullptr },
	m_jsonLexer { nullptr },
	m_makLexer { nullptr },
	m_matLexer { nullptr },
	m_mdLexer { nullptr },
	m_pyLexer { nullptr },
	m_rLexer { nullptr },
	m_rsLexer { nullptr },
	m_shLexer { nullptr },
	m_sqlLexer { nullptr },
	m_tclLexer { nullptr },
	m_xmlLexer { nullptr }
{
	bLexerKeywords = false;
	LoadMarginSettings();
}

/**
 * @brief Destructor for CIntelliEditView.
 *
 * Cleans up resources used by the view.
 */
CIntelliEditView::~CIntelliEditView()
{
}

/**
 * @brief Called before window creation.
 *
 * Allows modification of window class or styles before the window is created.
 *
 * @param cs Reference to the CREATESTRUCT containing window creation parameters.
 * @return TRUE if the window should be created; FALSE otherwise.
 */
BOOL CIntelliEditView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/**
 * @brief Draws the view.
 *
 * Called by the framework to render the view. The actual rendering is
 * handled by the Scintilla control.
 *
 * @param pDC Pointer to the device context (unused for Scintilla control).
 */
// CIntelliEditView drawing

void CIntelliEditView::OnDraw(CDC* /*pDC*/)
{
	CIntelliEditDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}

/**
 * @brief Sets a style for the Scintilla editor.
 *
 * Configures foreground color, background color, font size, and font face
 * for a specific style identifier.
 *
 * @param style The style identifier to configure.
 * @param fore Foreground (text) color as a COLORREF.
 * @param back Background color as a COLORREF (default: white).
 * @param size Font size in points (default: -1 to keep current).
 * @param face Font face name (default: nullptr to keep current).
 */
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

/**
 * @brief Defines a marker symbol with colors.
 *
 * Configures a margin marker with a specific symbol type and colors.
 *
 * @param marker The marker number to define.
 * @param markerType The symbol type for the marker.
 * @param fore Foreground color for the marker.
 * @param back Background color for the marker.
 */
void CIntelliEditView::DefineMarker(int marker, Scintilla::MarkerSymbol markerType, COLORREF fore, COLORREF back)
{
	auto& rCtrl{ GetCtrl() };

	rCtrl.MarkerDefine(marker, markerType);
	rCtrl.MarkerSetFore(marker, fore);
	rCtrl.MarkerSetBack(marker, back);
}

/**
 * @brief Initializes the view after creation.
 *
 * Sets up the Scintilla editor with default styles, colors, margins,
 * folding configuration, markers, auto-completion, and call tips.
 * Configures syntax highlighting styles for C/C++ and TCL languages.
 */
void CIntelliEditView::OnInitialUpdate()
{
	// Let the base class do its thing
	__super::OnInitialUpdate();

	auto& rCtrl{ GetCtrl() };

	/* CIntelliEditDoc* pDocument = GetDocument();
	ASSERT_VALID(pDocument);
	CString strTempPath = pDocument->GetPathName();
	// SelectHighlight(strTempPath); */

	SetAStyle(static_cast<int>(Scintilla::StylesCommon::Default), RGB(0, 0, 0), RGB(0xff, 0xff, 0xff), 10, "Consolas");
	// Setup styles
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
	/* TCL/TK */
	SetAStyle(SCE_TCL_DEFAULT, RGB(0, 0, 0));
	SetAStyle(SCE_TCL_COMMENT, RGB(0, 0x80, 0));
	SetAStyle(SCE_TCL_COMMENTLINE, RGB(0, 0x80, 0));
	SetAStyle(SCE_TCL_COMMENT_BOX, RGB(0, 0x80, 0));
	SetAStyle(SCE_TCL_BLOCK_COMMENT, RGB(0, 0x80, 0));
	SetAStyle(SCE_TCL_NUMBER, RGB(0, 0x80, 0x80));
	SetAStyle(SCE_TCL_WORD, RGB(0, 0, 0x80));
	rCtrl.StyleSetBold(SCE_TCL_WORD, 1);
	SetAStyle(SCE_TCL_WORD2, RGB(0, 0, 0x80));
	rCtrl.StyleSetBold(SCE_TCL_WORD2, 1);
	SetAStyle(SCE_TCL_WORD3, RGB(0, 0, 0x80));
	rCtrl.StyleSetBold(SCE_TCL_WORD3, 1);
	SetAStyle(SCE_TCL_WORD4, RGB(0, 0, 0x80));
	rCtrl.StyleSetBold(SCE_TCL_WORD4, 1);
	SetAStyle(SCE_TCL_WORD5, RGB(0, 0, 0x80));
	rCtrl.StyleSetBold(SCE_TCL_WORD5, 1);
	SetAStyle(SCE_TCL_WORD6, RGB(0, 0, 0x80));
	rCtrl.StyleSetBold(SCE_TCL_WORD6, 1);
	SetAStyle(SCE_TCL_WORD7, RGB(0, 0, 0x80));
	rCtrl.StyleSetBold(SCE_TCL_WORD7, 1);
	SetAStyle(SCE_TCL_WORD8, RGB(0, 0, 0x80));
	rCtrl.StyleSetBold(SCE_TCL_WORD8, 1);
	SetAStyle(SCE_TCL_IN_QUOTE, RGB(0x80, 0, 0x80));
	SetAStyle(SCE_TCL_IDENTIFIER, RGB(0, 0, 0));
	SetAStyle(SCE_TCL_OPERATOR, RGB(0x80, 0x80, 0));

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

	// Setup auto completion
	rCtrl.AutoCSetSeparator(10); // Use a separator of line feed

	// Setup call tips
	rCtrl.SetMouseDwellTime(1000);

	// Enable Multiple selection
	// rCtrl.SetMultipleSelection(TRUE);

	// Show line numbers
	rCtrl.SetMarginWidthN(0, 32);

	// Set word wrap
	rCtrl.SetWrapMode(Scintilla::Wrap::Word);

#ifdef _DEBUG
	AfxDump(this);
#endif
}

/**
 * @brief Handles print preview command.
 *
 * Displays the print preview window for the current document.
 */
// CIntelliEditView printing

void CIntelliEditView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

/**
 * @brief Handles right mouse button up event.
 *
 * Converts the point to screen coordinates and displays the context menu.
 *
 * @param nFlags Mouse event flags (unused).
 * @param point The point where the right button was released.
 */
void CIntelliEditView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

/**
 * @brief Displays the context menu.
 *
 * Shows the popup edit menu at the specified screen coordinates.
 *
 * @param pWnd Pointer to the window that owns the menu (unused).
 * @param point The screen coordinates where the menu should appear.
 */
void CIntelliEditView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}

/**
 * @brief Validates the view object in debug builds.
 *
 * Performs assertion checks to verify the view object is in a valid state.
 */
// CIntelliEditView diagnostics

#ifdef _DEBUG
void CIntelliEditView::AssertValid() const
{
	CView::AssertValid();
}

/**
 * @brief Dumps the view object state for debugging.
 *
 * Outputs diagnostic information about the view object.
 *
 * @param dc Reference to the dump context to write to.
 */
void CIntelliEditView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

/**
 * @brief Gets the document associated with this view.
 *
 * Returns a pointer to the CIntelliEditDoc document object.
 *
 * @return Pointer to the associated document.
 */
CIntelliEditDoc* CIntelliEditView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CIntelliEditDoc)));
	return (CIntelliEditDoc*)m_pDocument;
}
#endif //_DEBUG

/**
 * @brief Adds a marker to the current line.
 *
 * Places a marker (bookmark) on the line where the cursor is located.
 */
// CIntelliEditView message handlers

void CIntelliEditView::OnOptionsAddmarker()
{
	auto& rCtrl{ GetCtrl() };
	const Scintilla::Position nPos{ rCtrl.GetCurrentPos() };
	const auto nLine{ rCtrl.LineFromPosition(nPos) };
	rCtrl.MarkerAdd(nLine, 0);
}

/**
 * @brief Updates the UI state for the Add Marker command.
 *
 * Enables the command only if the current line doesn't already have a marker.
 *
 * @param pCmdUI Pointer to the command UI object to update.
 */
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

/**
 * @brief Deletes a marker from the current line.
 *
 * Removes the marker (bookmark) from the line where the cursor is located.
 */
void CIntelliEditView::OnOptionsDeletemarker()
{
	auto& rCtrl{ GetCtrl() };
	const Scintilla::Position nPos{ rCtrl.GetCurrentPos() };
	const auto nLine{ rCtrl.LineFromPosition(nPos) };
	rCtrl.MarkerDelete(nLine, 0);
}

/**
 * @brief Updates the UI state for the Delete Marker command.
 *
 * Enables the command only if the current line has a marker.
 *
 * @param pCmdUI Pointer to the command UI object to update.
 */
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

/**
 * @brief Finds and navigates to the next marker.
 *
 * Searches forward from the current line for the next marker and
 * moves the cursor to that line. Beeps if no marker is found.
 */
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

/**
 * @brief Finds and navigates to the previous marker.
 *
 * Searches backward from the current line for the previous marker and
 * moves the cursor to that line. Beeps if no marker is found.
 */
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

/**
 * @brief Toggles the fold margin visibility.
 *
 * Shows or hides the margin used for code folding (collapsing/expanding
 * code blocks).
 */
void CIntelliEditView::OnOptionsFoldMargin()
{
	auto& rCtrl{ GetCtrl() };
	const int nMarginWidth{ rCtrl.GetMarginWidthN(2) };
	if (nMarginWidth)
		rCtrl.SetMarginWidthN(2, 0);
	else
		rCtrl.SetMarginWidthN(2, 16);
}

/**
 * @brief Updates the UI state for the Fold Margin command.
 *
 * Sets the check mark based on whether the fold margin is visible.
 *
 * @param pCmdUI Pointer to the command UI object to update.
 */
#pragma warning(suppress: 26429)
void CIntelliEditView::OnUpdateOptionsFoldMargin(CCmdUI* pCmdUI)
{
#pragma warning(suppress: 26486)
	pCmdUI->SetCheck(GetCtrl().GetMarginWidthN(2) != 0);
}

/**
 * @brief Toggles the selection margin visibility.
 *
 * Shows or hides the margin used for line selection.
 */
void CIntelliEditView::OnOptionsSelectionMargin()
{
	auto& rCtrl{ GetCtrl() };
	const int nMarginWidth{ rCtrl.GetMarginWidthN(1) };
	if (nMarginWidth)
		rCtrl.SetMarginWidthN(1, 0);
	else
		rCtrl.SetMarginWidthN(1, 16);
}

/**
 * @brief Updates the UI state for the Selection Margin command.
 *
 * Sets the check mark based on whether the selection margin is visible.
 *
 * @param pCmdUI Pointer to the command UI object to update.
 */
#pragma warning(suppress: 26429)
void CIntelliEditView::OnUpdateOptionsSelectionMargin(CCmdUI* pCmdUI)
{
#pragma warning(suppress: 26486)
	pCmdUI->SetCheck(GetCtrl().GetMarginWidthN(1) != 0);
}

/**
 * @brief Toggles line number visibility.
 *
 * Shows or hides line numbers in the editor margin.
 */
void CIntelliEditView::OnOptionsViewLinenumbers()
{
	auto& rCtrl{ GetCtrl() };
	const int nMarginWidth{ rCtrl.GetMarginWidthN(0) };
	if (nMarginWidth)
		rCtrl.SetMarginWidthN(0, 0);
	else
		rCtrl.SetMarginWidthN(0, 32);
}

/**
 * @brief Updates the UI state for the View Line Numbers command.
 *
 * Sets the check mark based on whether line numbers are visible.
 *
 * @param pCmdUI Pointer to the command UI object to update.
 */
#pragma warning(suppress: 26429)
void CIntelliEditView::OnUpdateOptionsViewLinenumbers(CCmdUI* pCmdUI)
{
#pragma warning(suppress: 26486)
	pCmdUI->SetCheck(GetCtrl().GetMarginWidthN(0) != 0);
}

/**
 * @brief Updates the insert/overtype indicator.
 *
 * Updates the status bar indicator to show "OVR" when in overtype mode.
 *
 * @param pCmdUI Pointer to the command UI object to update.
 */
#pragma warning(suppress: 26429)
void CIntelliEditView::OnUpdateInsert(CCmdUI* pCmdUI)
{
	CString sText;
	if (GetCtrl().GetOvertype())
		VERIFY(sText.LoadString(ID_INDICATOR_OVR));
#pragma warning(suppress: 26486)
	pCmdUI->SetText(sText);
}

/**
 * @brief Updates the style indicator.
 *
 * Displays the current style number at the cursor position in the status bar.
 *
 * @param pCmdUI Pointer to the command UI object to update.
 */
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

/**
 * @brief Updates the fold level indicator.
 *
 * Displays the current fold level at the cursor position in the status bar.
 *
 * @param pCmdUI Pointer to the command UI object to update.
 */
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

/**
 * @brief Updates the line, column, and position indicator.
 *
 * Displays the current line number, column, and character position in the status bar.
 *
 * @param pCmdUI Pointer to the command UI object to update.
 */
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

/**
 * @brief Handles character added event for auto-completion.
 *
 * Provides examples of auto-completion functionality:
 * - Shows completion list for "scintilla is " phrase
 * - Shows completion list for "res" prefix (resize/restart/restore)
 * - Demonstrates indicator usage for "sample_indicator" text
 *
 * @param pSCNotification Pointer to Scintilla notification data containing character info.
 */
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

/**
 * @brief Handles mouse dwell start event for call tips.
 *
 * Displays a call tip when the mouse hovers over specific text. Example
 * implementation shows a call tip for the word "author".
 *
 * @param pSCNotification Pointer to Scintilla notification data containing dwell position.
 */
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

/**
 * @brief Handles mouse dwell end event.
 *
 * Cancels any active call tip when the mouse stops hovering.
 *
 * @param pSCNotification Pointer to Scintilla notification data (unused).
 */
void CIntelliEditView::OnDwellEnd(_Inout_ Scintilla::NotificationData* /*pSCNotification*/)
{
	auto& rCtrl{ GetCtrl() };

	//Cancel any outstanding call tip
	if (rCtrl.CallTipActive())
		rCtrl.CallTipCancel();
}

/**
 * @brief Handles window activation/deactivation.
 *
 * Cancels any active call tips when the window loses focus.
 *
 * @param nState Window activation state (WA_ACTIVE, WA_CLICKACTIVE, or WA_INACTIVE).
 * @param pWndOther Pointer to the window being activated or deactivated.
 * @param bMinimized TRUE if the window is minimized; FALSE otherwise.
 */
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

/**
 * @brief Handles attempt to modify read-only document.
 *
 * Prompts the user whether to allow modification of a read-only file.
 * If the user confirms, the read-only flag is cleared.
 *
 * @param pSCNotification Pointer to Scintilla notification data (unused).
 */
void CIntelliEditView::OnModifyAttemptRO(_Inout_ Scintilla::NotificationData* /*pSCNotification*/)
{
#pragma warning(suppress: 26493)
	if (AfxMessageBox(IDP_ALLOW_MODIFY_READONLY_FILE, MB_YESNO) == IDYES)
		GetCtrl().SetReadOnly(FALSE);
}

/**
 * @brief Handles document modification event.
 *
 * Responds to document modifications. In debug mode, demonstrates the
 * InsertCheck notification by replacing "Dublin" with "Capital of Ireland".
 *
 * @param pSCNotification Pointer to Scintilla notification data containing modification type and text.
 */
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

/**
 * @brief Creates the Scintilla editor control.
 *
 * Factory method that creates and returns a unique pointer to the
 * Scintilla control used by this view.
 *
 * @return Unique pointer to the created Scintilla control.
 */
std::unique_ptr<Scintilla::CScintillaCtrl> CIntelliEditView::CreateScintillaControl()
{
	return std::make_unique<CScintillaDemoCtrl>();
}

/**
 * @brief Handles window creation.
 *
 * Initializes all lexers for syntax highlighting support. Creates lexer
 * instances for: Batch, CMake, C++, CSS, HTML, JSON, Makefile, MATLAB,
 * Markdown, Python, R, Rust, Shell, SQL, TCL, and XML.
 *
 * @param lpCreateStruct Pointer to the window creation structure.
 * @return 0 on success; -1 on failure.
 */
#pragma warning(suppress: 26434)
int CIntelliEditView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	// Let the base class do its thing
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Create the Batch Lexer
#pragma warning(suppress: 26429)
	if (m_batLexer == nullptr)
	{
		m_batLexer = theApp.m_pCreateLexer("batch");
		if (m_batLexer == nullptr)
			return -1;
	}

	// Create the CMake Lexer
#pragma warning(suppress: 26429)
	if (m_cmakeLexer == nullptr)
	{
		m_cmakeLexer = theApp.m_pCreateLexer("cmake");
		if (m_cmakeLexer == nullptr)
			return -1;
	}

	// Create the C++ Lexer
#pragma warning(suppress: 26429)
	if (m_cppLexer == nullptr)
	{
		m_cppLexer = theApp.m_pCreateLexer("cpp");
		if (m_cppLexer == nullptr)
			return -1;
	}

	// Create the CSS Lexer
#pragma warning(suppress: 26429)
	if (m_cssLexer == nullptr)
	{
		m_cssLexer = theApp.m_pCreateLexer("css");
		if (m_cssLexer == nullptr)
			return -1;
	}

	// Create the HTML Lexer
#pragma warning(suppress: 26429)
	if (m_htmlLexer == nullptr)
	{
		m_htmlLexer = theApp.m_pCreateLexer("hypertext");
		if (m_htmlLexer == nullptr)
			return -1;
	}

	// Create the JSON Lexer
#pragma warning(suppress: 26429)
	if (m_jsonLexer == nullptr)
	{
		m_jsonLexer = theApp.m_pCreateLexer("json");
		if (m_jsonLexer == nullptr)
			return -1;
	}

	// Create the Makefile Lexer
#pragma warning(suppress: 26429)
	if (m_makLexer == nullptr)
	{
		m_makLexer = theApp.m_pCreateLexer("makefile");
		if (m_makLexer == nullptr)
			return -1;
	}

	// Create the MATLAB Lexer
#pragma warning(suppress: 26429)
	if (m_matLexer == nullptr)
	{
		m_matLexer = theApp.m_pCreateLexer("matlab");
		if (m_matLexer == nullptr)
			return -1;
	}

	// Create the MARKDOWN Lexer
#pragma warning(suppress: 26429)
	if (m_mdLexer == nullptr)
	{
		m_mdLexer = theApp.m_pCreateLexer("markdown");
		if (m_mdLexer == nullptr)
			return -1;
	}

	// Create the Python Lexer
#pragma warning(suppress: 26429)
	if (m_pyLexer == nullptr)
	{
		m_pyLexer = theApp.m_pCreateLexer("python");
		if (m_pyLexer == nullptr)
			return -1;
	}

	// Create the R Lexer
#pragma warning(suppress: 26429)
	if (m_rLexer == nullptr)
	{
		m_rLexer = theApp.m_pCreateLexer("r");
		if (m_rLexer == nullptr)
			return -1;
	}

	// Create the Rust Lexer
#pragma warning(suppress: 26429)
	if (m_rsLexer == nullptr)
	{
		m_rsLexer = theApp.m_pCreateLexer("rust");
		if (m_rsLexer == nullptr)
			return -1;
	}

	// Create the Shell Lexer
#pragma warning(suppress: 26429)
	if (m_shLexer == nullptr)
	{
		m_shLexer = theApp.m_pCreateLexer("bash");
		if (m_shLexer == nullptr)
			return -1;
	}

	// Create the SQL Lexer
#pragma warning(suppress: 26429)
	if (m_sqlLexer == nullptr)
	{
		m_sqlLexer = theApp.m_pCreateLexer("sql");
		if (m_sqlLexer == nullptr)
			return -1;
	}

	// Create the TCL Lexer
#pragma warning(suppress: 26429)
	if (m_tclLexer == nullptr)
	{
		m_tclLexer = theApp.m_pCreateLexer("tcl");
		if (m_tclLexer == nullptr)
			return -1;
	}

	// Create the XML Lexer
#pragma warning(suppress: 26429)
	if (m_xmlLexer == nullptr)
	{
		m_xmlLexer = theApp.m_pCreateLexer("xml");
		if (m_xmlLexer == nullptr)
			return -1;
	}

	return 0;
}

/**
 * @brief Serializes the document.
 *
 * Called during document save/load operations. Triggers syntax highlighting
 * selection based on the file extension.
 *
 * @param ar Archive object for serialization.
 */
void CIntelliEditView::Serialize(CArchive& ar)
{
	__super::Serialize(ar);
	SelectHighlight(ar.m_strFileName);
}

/**
 * @brief Selects appropriate syntax highlighting based on file extension.
 *
 * Determines the file type from the extension and configures the appropriate
 * lexer and keywords for syntax highlighting. Supports multiple languages
 * including:
 * - Batch (.bat)
 * - CMake (.cmake, CMakeLists.txt)
 * - C/C++ (.c, .cpp, .cxx, .h, .hpp)
 * - C# (.cs)
 * - CSS (.css)
 * - HTML/Web (.htm, .html, .asp, .aspx, .php)
 * - Groovy (.groovy)
 * - Java (.java)
 * - JavaScript (.js)
 * - JSON (.json)
 * - MATLAB (.m)
 * - Makefile (.mak, makefile)
 * - Markdown (.md)
 * - Python (.py)
 * - R (.r)
 * - Rust (.rs)
 * - Shell (.sh)
 * - SQL (.sql)
 * - TCL (.tcl)
 * - XML (.xml)
 * - Plain text (.txt, .log, .ini)
 *
 * @param strTempPath Full path to the file being opened.
 */
void CIntelliEditView::SelectHighlight(CString strTempPath)
{
	if (bLexerKeywords)
		return;

	TCHAR lpszDrive[_MAX_DRIVE] = { 0 };
	TCHAR lpszFolder[_MAX_DIR] = { 0 };
	TCHAR lpszFileName[_MAX_FNAME] = { 0 };
	TCHAR lpszExtension[_MAX_EXT] = { 0 };

	auto& rCtrl{ GetCtrl() };

	strTempPath.MakeLower();
	_tsplitpath_s(strTempPath,
		lpszDrive, _MAX_DRIVE,
		lpszFolder, _MAX_DIR,
		lpszFileName, _MAX_FNAME,
		lpszExtension, _MAX_EXT);
	bLexerKeywords = !strTempPath.IsEmpty();

	CString strExt(lpszExtension);
	CString strFileName(lpszFileName);

	if (strExt == _T(".bat"))
	{
		rCtrl.SetILexer(m_batLexer);
	}
	else if (strExt == _T(".cmake") || (strFileName == _T("cmakelists") && strExt == _T(".txt")))
	{
		rCtrl.SetILexer(m_cmakeLexer);
		rCtrl.SetKeyWords(0, g_cmakeKeywords);
	}
	else if (strExt == _T(".c") || strExt == _T(".cpp") || strExt == _T(".cxx") || 
			 strExt == _T(".h") || strExt == _T(".hpp"))
	{
		rCtrl.SetILexer(m_cppLexer);
		rCtrl.SetKeyWords(0, g_cppKeywords);
	}
	else if (strExt == _T(".cs"))
	{
		rCtrl.SetILexer(m_cppLexer);
		rCtrl.SetKeyWords(0, g_csKeywords);
	}
	else if (strExt == _T(".css"))
	{
		rCtrl.SetILexer(m_cssLexer);
	}
	else if (strExt == _T(".htm") || strExt == _T(".html") || strExt == _T(".asp") || 
			 strExt == _T(".aspx") || strExt == _T(".php"))
	{
		rCtrl.SetILexer(m_htmlLexer);
	}
	else if (strExt == _T(".groovy"))
	{
		rCtrl.SetILexer(m_cppLexer);
		rCtrl.SetKeyWords(0, g_groovyKeywords);
	}
	else if (strExt == _T(".java"))
	{
		rCtrl.SetILexer(m_cppLexer);
		rCtrl.SetKeyWords(0, g_javaKeywords);
	}
	else if (strExt == _T(".js"))
	{
		rCtrl.SetILexer(m_cppLexer);
		rCtrl.SetKeyWords(0, g_jsKeywords);
	}
	else if (strExt == _T(".json"))
	{
		rCtrl.SetILexer(m_jsonLexer);
	}
	else if (strExt == _T(".m"))
	{
		rCtrl.SetILexer(m_matLexer);
		rCtrl.SetKeyWords(0, g_matlabKeywords);
	}
	else if (strExt == _T(".mak") || strFileName == _T("makefile"))
	{
		rCtrl.SetILexer(m_makLexer);
	}
	else if (strExt == _T(".md"))
	{
		rCtrl.SetILexer(m_mdLexer);
	}
	else if (strExt == _T(".py"))
	{
		rCtrl.SetILexer(m_pyLexer);
		rCtrl.SetKeyWords(0, g_pyKeywords);
	}
	else if (strExt == _T(".r"))
	{
		rCtrl.SetILexer(m_rLexer);
		rCtrl.SetKeyWords(0, g_rKeywords);
	}
	else if (strExt == _T(".rs"))
	{
		rCtrl.SetILexer(m_rsLexer);
		rCtrl.SetKeyWords(0, g_rsKeywords);
	}
	else if (strExt == _T(".sh"))
	{
		rCtrl.SetILexer(m_shLexer);
	}
	else if (strExt == _T(".sql"))
	{
		rCtrl.SetILexer(m_sqlLexer);
		rCtrl.SetKeyWords(0, g_sqlKeywords);
	}
	else if (strExt == _T(".tcl"))
	{
		rCtrl.SetILexer(m_tclLexer);
		rCtrl.SetKeyWords(0, g_tclKeywords);
	}
	else if (strExt == _T(".xml"))
	{
		rCtrl.SetILexer(m_xmlLexer);
	}
	else if (strExt == _T(".txt") || strExt == _T(".log") || 
			 strExt == _T(".ini") || strExt.IsEmpty())
	{
		rCtrl.SetupDirectAccess();
		rCtrl.SetILexer(nullptr);
	}
	else
	{
		rCtrl.SetILexer(m_cppLexer);
		rCtrl.SetKeyWords(0, g_cppKeywords);
	}
}
