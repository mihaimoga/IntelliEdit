# AGENTS.md — Agent & Developer Guide for IntelliEdit

Welcome to the **IntelliEdit** codebase. This guide provides AI coding agents and human contributors with essential context, architectural layout, coding standards, and build instructions for working effectively in this repository.

---

## 1. Project Overview

**IntelliEdit** is a free, high-performance Win32/MFC-based source code editor and Windows Notepad replacement. It is designed for minimal CPU overhead, low memory consumption, and rich editing capabilities.

### Key Components & Technologies
- **IntelliEdit (`IntelliEdit/`)**: Main MFC/Win32 application featuring a modern Ribbon interface, Scintilla Document/View architecture, Find/Replace engine, single-instance management, and WebView2 embedded browser support.
- **Scintilla (`scintilla/`)**: High-performance core text editing engine (Win32 port).
- **Lexilla (`lexilla/`)**: Syntax highlighting and lexer library powering Scintilla.
- **genUp4win (`genUp4win/`)**: Lightweight auto-update library managing XML-based version checks and HTTPS downloads via Win32 APIs (`URLDownloadToFile`).

---

## 2. Solution Structure & Projects

```text
├── IntelliEdit/               # Primary MFC Desktop Application (.exe)
│   ├── res/                   # Icons, bitmaps, ribbon resources (.mfcribbon-ms)
│   ├── IntelliEdit.rc         # Win32 resource script & dialogs
│   ├── ScintillaCtrl.*        # CWnd wrapper around Scintilla control
│   ├── ScintillaDocView.*     # Document/View architecture integrating Scintilla
│   ├── EdgeWebBrowser.*       # Microsoft Edge WebView2 wrapper dialog
│   ├── CheckForUpdatesDlg.*   # Update notification and checking dialog
│   └── pch.h / pch.cpp        # Precompiled header
├── scintilla/                 # Scintilla text editing component
│   ├── include/               # Scintilla public API headers
│   ├── src/                   # Core editor implementation
│   └── win32/                 # Win32 platform layer & Scintilla.vcxproj
├── lexilla/                   # Lexilla syntax highlighting library
│   ├── include/               # Lexilla public API headers
│   ├── lexlib/                # Lexer library utilities
│   └── src/                   # Lexilla.vcxproj & lexer modules
├── genUp4win/                 # Generic updater DLL & CMake build files
│   ├── AppSettings.h          # XML & configuration parser
│   └── genUp4win.vcxproj      # Updater dynamic library project
└── packages/                  # NuGet native packages (WebView2, WIL)
```

---

## 3. Toolchain & Build Specifications

- **IDE / Toolset**: Visual Studio 2022+ / MSVC `v143` or `v145` toolset.
- **SDK**: Windows 10 / 11 SDK (`10.0.*`).
- **Language Standards**:
  - C++: `/std:c++latest` (C++20/C++23 features supported).
  - C: `/std:clatest`.
- **Character Set**: Unicode (`_UNICODE`, `UNICODE`).
- **MFC Linkage**: Static MFC (`<UseOfMfc>Static</UseOfMfc>`).
- **External Dependencies**:
  - `Microsoft.Web.WebView2` (NuGet)
  - `Microsoft.Windows.ImplementationLibrary` (WIL, NuGet)

### Building via MSBuild
To build the solution from the developer command prompt or terminal:
```powershell
msbuild IntelliEdit.sln /p:Configuration=Release /p:Platform=x64
```
For 32-bit Debug builds:
```powershell
msbuild IntelliEdit.sln /p:Configuration=Debug /p:Platform=Win32
```

---

## 4. Coding Standards & Conventions

All contributions and AI-generated code must adhere to the conventions outlined below (derived from `CONTRIBUTING.md`):

### Formatting & Bracing
- **Indentation**: Use **Tabs** (configured to 4 spaces width). Do not use pure spaces for indentation.
- **Braces (Allman style)**: Place opening braces `{` on a new line for functions, control flow, and classes.
  - *Exception*: Single-line inline method definitions in header files (`.h`) may use same-line braces:
	```cpp
	int getCount() { return _count; }
	```
- **Operator Spacing**: Always leave one space before and after binary and ternary operators:
  ```cpp
  if (a == 10 && b == 42) // Correct
  ```
- **Control Flow Spacing**:
  - Space between keywords (`if`, `for`, `while`, `switch`) and opening parenthesis:
	```cpp
	if (condition)
	while (running)
	```
  - No space between function names and opening parenthesis:
	```cpp
	executeCommand(id); // Correct
	```
  - Only one space after semicolons in `for` loops: `for (int i = 0; i != 10; ++i)`
- **Switch Statements**:
  ```cpp
  switch (type)
  {
	  case 1:
	  {
		  // Action
		  break;
	  }
	  default:
		  break;
  }
  ```

### Naming Conventions
- **Classes & Structs**: `PascalCase` (e.g., `ChildFrame`, `ScintillaCtrl`).
- **Methods & Functions**: `camelCase` or standard Win32/MFC `PascalCase` handlers when overriding framework virtuals (e.g., `myMethod(uint param)`).
- **Member Variables**: Prefix with an underscore `_` (e.g., `_publicAttribute`, `_pDocument`, `_viewCount`).
- **Variables**: Descriptive names; avoid abbreviations or single-character variables except for simple loop counters (`i`, `j`).

### Modern C++ Best Practices
- **Memory Management**:
  - Avoid raw `new` and `delete`. Prefer automatic (stack) variables.
  - Prefer `std::unique_ptr` for ownership; avoid `std::shared_ptr` unless multi-ownership is strictly required.
  - Prefer references over raw pointers. When nullable semantics are required, use smart pointers or check pointer validity explicitly.
- **Initialization**: Prefer uniform brace initialization:
  ```cpp
  MyClass instance{10.4};
  ```
- **Type Conversions**: Avoid C-style casts `(int)val`. Use C++ explicit casts (`static_cast`, `reinterpret_cast`, `const_cast`).
- **Strings**: Always use `.empty()` (or `IsEmpty()` for MFC `CString`) rather than comparing against empty literal `""`.
- **Operators**: Use `!` instead of `not`, `&&` instead of `and`, `||` instead of `or`.
- **Pre-increment**: Prefer `++i` over `i++`.
- **Headers**: Never place `using namespace ...` in header files.
- **Magic Numbers**: Avoid raw constants; use `enum class` or named `constexpr` constants.
- **Performance**: Code legibility and length should never compromise end-user execution speed and responsiveness.

---

## 5. Agent Instructions for Modifying Code

When implementing changes, resolving issues, or adding features:
1. **Preserve Context & Style**: Match the formatting and conventions of existing files precisely.
2. **Precompiled Headers**: Ensure `pch.h` is included first in all `.cpp` files under `IntelliEdit/`.
3. **MFC & Resource Sync**: When updating UI elements, ensure `Resource.h`, `IntelliEdit.rc`, and ribbon markup (`res/ribbon.mfcribbon-ms`) stay in sync.
4. **Minimal Diffs**: Keep modifications targeted and compact. Avoid mass whitespace reformatting.
5. **Verification**: Always run project builds via MSBuild to verify compilation and lack of warnings before concluding tasks.
