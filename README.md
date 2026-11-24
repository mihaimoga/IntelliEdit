![IntelliEdit.png](IntelliEdit.png)

Download:
- [IntelliEditSetup.msi](https://www.moga.doctor/freeware/IntelliEditSetup.msi)
- [IntelliEdit.zip](https://www.moga.doctor/freeware/IntelliEdit.zip)

## Introduction

_IntelliEdit_ is a free (as in “free speech” and also as in “free beer”) source code editor and Microsoft Notepad replacement that supports several languages. Running in the Microsoft Windows environment, its use is governed by [GNU General Public License v3.0](https://www.gnu.org/licenses/gpl-3.0.html). Based on the powerful editing component Scintilla, _IntelliEdit_ is written in C++ and uses pure Win32 API and STL which ensures a higher execution speed and smaller program size. By optimizing as many routines as possible without losing user friendliness, _IntelliEdit_ is trying to reduce the world carbon dioxide emissions. When using less CPU power, the PC can throttle down and reduce power consumption, resulting in a greener environment. I hope you enjoy _IntelliEdit_ as much as I enjoy coding it!

## Getting started

### Install IntelliEdit using the installer

- Download the installer
- Run the executable binary and follow the installation flow

The installer will likely require Administrative privileges in order to install _IntelliEdit_ (and later, to update _IntelliEdit_ or install or update plugins, or anything else that requires writing to the installation directory). If you do not have Administrative privileges, you either need to tell the installer to use a location where you do have write permission (though that may still ask for Administrator privileges), or you may choose not use the installer and instead run a portable edition from a directory where you have write permission.

### Install IntelliEdit from zip

These instructions will allow you to run a portable or mini-portable (also called “minimalist”), without requiring administrative privileges.

- Create a new folder somewhere that you have write-permission
- Unzip the content into the new folder
- Run _IntelliEdit_ from the new folder

The portable zip edition of _IntelliEdit_ can be removed by deleting the directory they came in. If you manually set up file associations or context-menu entries in the OS, it is your responsibility to remove them yourself.

## Working with Files

A “file” is the basic unit of what is edited in _IntelliEdit_, but that term actually covers multiple related concepts. Primarily, the “file” is the series of bytes stored on a disk or other storage medium and accessed through your computer’s filesystem; and pedantically, if the document you are editing has never been saved to the filesystem, it’s not technically a file, though common usage applies that term to unsaved documents as well. The “document” refers to the text being edited, whether it’s a new, unsaved document, or whether it’s a document that’s been previously saved as a file on the filesystem. And finally, in _IntelliEdit_, each document is presented in a Tab in one of the two Views of the _IntelliEdit_ user interface, which are the graphical containers which _IntelliEdit_ uses to manipulate files and other documents, though many users think of the user interface element as the “file” as well.

The **main button** contains many of the normal file-operation actions found in most applications. _IntelliEdit_ also includes other custom actions which are useful to understand.

- **New**: Creates a new document, and displays that empty document in a Tab in the active View.
- **Open**: Opens an existing file from the filesystem, and displays that document in a Tab in the active View.
- **Save**: Saves the current document to the filesystem using the same filename. 
If the document does not currently have a file associated with it, this action will be grayed out and the menu entry cannot be used. Use **Save As** instead.
- **Save As**: Saves the current document to the filesystem, prompting for the new filename to use. 
If the document had previously been saved to a file, this will create as new copy of the file, and _IntelliEdit_ will continue working with the document associated with the new file, leaving the old file with the contents that were last saved.
- The **Print** action will pull up a Windows-standard print dialog, from which you can choose your printer and send your text to the selected printer. 
Normally, it will print the whole document, but you can use the print dialog to choose only certain pages; if you have an active selection in the editor, only the selected text will be printed.
- **Rename File**: Renames the active document.
- **Move File**: Moves the active document.
- **Copy File**: Copies the active document.
- **Delete File**: Deletes the active document.
- **Properties**: Shows properties for the active document.
- **Close**: Closes the active document, prompting to save if it has been modified since it was opened or created.
- **Exit**: Closes the _IntelliEdit_ application.

The **Clipboard** menu features typical editing commands which any Windows user should be familiar with:

- **Undo**: reverts the text to its content before the previous operation; can be used one or more times consecutively to step back through a document’s textual history.
- **Redo**: if **Undo** was executed previously, this will reinstate the change(s) previously undone; may be executed multiple times.
- **Cut**: will remove any selected text from the document and place it on the Windows clipboard.
- **Copy**: will put a copy of any selected text on the Windows clipboard; document content is unaltered.
- **Paste**: if the Windows clipboard contains text, this will insert a copy of that text at the point of the caret; if text is selected when this command is executed, the selected text will be replaced by the text from the clipboard; if the clipboard does not contain text, nothing will happen.
- **Delete**: will remove any selected text from the document.
- **Select All**: selects all text in the document into a stream selection.

![Find dialog](IntelliEdit-Find.png)

![Replace dialog](IntelliEdit-Replace.png)

The **Find/Replace** dialogs have certain features in common, though some are not available (greyed out) under certain circumstances.

- **Find what**: This is the text you are searching for.
- **Replace with**: This is the text that will replace what was matched.
- **Match whole word only:** If checked, searches will only match if the result is a whole word (so "it" will not be found inside "hitch").
- **Match case:** If checked, searches must match in case (so a search for "it" will not find "It" or "IT").
- **Regular Expression:** Uses the Boost regular expression engine to perform very powerful search and replace actions.

The various action buttons available include:

- **Find Next**: Finds the next matching text.
- **Replace**: Replaces the currently-selected match. (If no match is currently selected, it behaves like Find Next and just highlights the next match in the specified direction.)
- **Replace All**: It makes one pass through the active document, from the very top to the very bottom, and replaces all occurrences found.

The **Bookmark** menu allows you to navigate and manipulate Bookmarks.

- **Add**: Adds the Bookmark indicator on the active line.
- **Delete**: Deletes the Bookmark indicator on the active line.
- **Find Next**: Navigates to the next Bookmark in the active document.
- **Find Previous**: Navigates to the previous Bookmark in the active document.

## Supported Programming Languages

The following programming languages are supported by _IntelliEdit_:

- ASP
- Bash
- Batch
- C/C++
- C#
- CMake
- CSS
- Groovy
- HTML
- Java
- JavaScript
- JSON
- Makefile
- Markdown
- MATLAB
- PHP
- Python
- R
- Rust
- SQL
- TCL/TK
- XML

## Roadmap

This roadmap outlines where _IntelliEdit_ is heading. It brings together planned improvements, ideas for features that are not available yet, and the maintenance work that goes into every release. Items are grouped by horizon rather than by date, and their order may change as feedback arrives or as the libraries _IntelliEdit_ builds on evolve, so please treat it as a statement of intent, not a promise. For everything that has already shipped, see the [Release Notes](https://github.com/mihaimoga/IntelliEdit/releases).

_Last reviewed: November 2026 (version 1.21 in development)._

### Now (in progress and ongoing)

- **Up-to-date components**: each release picks up the latest Scintilla and Lexilla (editing and syntax highlighting), PJ Naughter's CScintillaCtrl, Microsoft Edge WebView2 (embedded browser) and genUp4win (automatic updates).
- **Stability, security and performance**: bug fixes, security updates and performance tuning continue alongside new features, so that _IntelliEdit_ stays fast and small.

### Next (planned, next in line)

- **Go To Line**: jump straight to a given line number, a basic navigation command that is not available yet.
- **Brace matching and auto-indent**: highlight the bracket that matches the one next to the caret, and keep the indentation of the previous line when starting a new one.
- **View options**: ribbon toggles for word wrap (always on today), line numbers, the selection and fold margins, visible whitespace and line-end markers, and indentation guides.
- **More languages and manual language selection**: the language is chosen from the file extension today, and unknown extensions fall back to C/C++ highlighting. Lexilla already provides lexers for many languages that are not enabled yet, for example YAML, TOML, PowerShell, Ruby, Perl and Lua, and a language picker would let you override the automatic choice.
- **Encoding and line endings**: show the encoding (ASCII, UTF-8, UTF-16) and the line-ending style (CRLF, LF, CR) of the active document in the status bar, and convert between them. Today the encoding is detected when a file is opened and preserved when it is saved, but it can neither be seen nor changed.

### Later (planned, larger efforts)

- **Word and keyword completion**: suggest words from the current document and the keywords of its language while typing, replacing the few sample completions that are built in today.
- **Fuller session restore**: reopen each document at its last caret and scroll position and, later, recover unsaved and untitled documents after a restart or a crash. Today the session only brings back documents that were saved to a file.
- **Find in Files**: search, and optionally replace, across all the files of a folder, with the matches listed in a results panel.
- **Themes and dark mode**: choose a color theme for the editor and the ribbon, including a dark one. Syntax colors are fixed today.

### Under consideration (ideas, not yet committed)

- **Multiple selections**: edit several places at once. Scintilla supports it, but it is turned off in the editor today.
- **Native ARM64 and high-DPI support**: a native ARM64 build for Windows on Arm devices (_IntelliEdit_ is currently built for x86 and x64) and Per-Monitor V2 DPI awareness for a sharper interface on high-DPI and mixed-DPI displays.
- **Localized user interface**: translated menus, commands and messages (the interface is English only today).
- **Automated builds**: a GitHub Actions workflow that builds the Win32 and x64 configurations for every pull request, so that contributions get quick feedback.

### Help shape the roadmap

Want to see something sooner, or have an idea that is not listed here? [Open an issue](https://github.com/mihaimoga/IntelliEdit/issues) or start a conversation in [Discussions](https://github.com/mihaimoga/IntelliEdit/discussions). Contributions are welcome as well: any item above can be picked up through a pull request, as described in the next section. For enhancements, the issue must be approved by a project administrator (it will carry the `Accepted` label) before the pull request can be merged, so it is best to open the issue before you start coding.

## Create and Submit your Pull Request

As noted in the [Contributing Rules](https://github.com/mihaimoga/IntelliEdit/blob/main/CONTRIBUTING.md) for _IntelliEdit_, all Pull Requests need to be attached to a issue on GitHub. So the first step is to create an issue which requests that the functionality be improved (if it was already there) or added (if it was not yet there); in your issue, be sure to explain that you have the functionality definition ready, and will be submitting a Pull Request. The second step is to use the GitHub interface to create the Pull Request from your fork into the main repository. The final step is to wait for and respond to feedback from the developers as needed, until such time as your PR is accepted or rejected.

## Acknowledges

This open source project uses the following libraries:

- Stefan-Mihai MOGA's [genUp4win](https://github.com/mihaimoga/genUp4win)
- PJ Naughter's [CHLinkCtrl](https://www.naughter.com/hlinkctrl.html)
- PJ Naughter's [CInstanceChecker](https://www.naughter.com/sinstance.html)
- [Lexilla, Scintilla, and SciTE](https://www.scintilla.org/)
- PJ Naughter's [CScintillaCtrl, CScintillaDoc, CScintillaView](https://www.naughter.com/scintilla.html)
- PJ Naughter's [CVersionInfo](https://www.naughter.com/versioninfo.html)
