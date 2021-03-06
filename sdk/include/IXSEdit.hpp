#pragma once

#include "xframe.h"
#include "xsedit.hpp"


namespace Hxsoft{ namespace XFrame
{


	class XCONTROL_API IXSEdit :public  IXFControl
	{
	public:
		XSEdit * m_pEdit;
		IXSEdit():m_pSearchText(NULL), m_pReplaceText(NULL),m_bSearchFound(false)
		{
		}
		~IXSEdit()
		{
			if(m_pSearchText)delete m_pSearchText;
			if( m_pReplaceText)delete  m_pReplaceText;
		}

	public:
		// @access public macro members
		// @cmember return linenumber display flag
		BOOL GetDisplayLinenumbers ();
		// @cmember return selection display flag
		BOOL GetDisplaySelection ();
		// @cmember return folding margin display flag
		BOOL GetDisplayFolding ();
		// @cmember set search flags
		virtual void SetSearchflags (int nSearchflags);
		virtual int GetSearchflags ();

	public:
		// @access public function members
		// @cmember register a window class to use scintilla in a dialog
		//static BOOL Register(CWinApp *app, UINT id);
		// @cmember try to load Lexer DLL
		static HMODULE LoadScintillaDll (LPCTSTR szLexerDll = NULL);
		// @cmember create window
		//virtual BOOL Create (LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
		// @cmember Set Text to the Scintilla control
		virtual void SetText (LPCTSTR szText);
		// @cmember Get Text from the Scintilla control
		virtual void GetText (LPTSTR &strText);
		// @cmember Get Text from the Scintilla control
		virtual LPTSTR GetText();
		// @cmember Load a file
		virtual BOOL LoadFile (LPCTSTR szFile);
		// @cmember Save a file
		virtual BOOL SaveFile (LPCTSTR szFile);
		// @cmember try to find lexer format from extension
		virtual int GetFormatFromExtension (LPCTSTR szExtension);
		// @cmember calcluate number of chars needed for linenumberdisplay
		virtual int GetLinenumberChars ();
		// @cmember calcluate number of pixels for linenumber display
		virtual int GetLinenumberWidth ();
		// @cmember set display of linenumbers on/off
		virtual void SetDisplayLinenumbers(BOOL bFlag = TRUE);
		// @cmember set display of selection/bookmark margin on/off
		virtual void SetDisplaySelection(BOOL bFlag = TRUE);
		// @cmember set display of source folding margin on/off
		virtual void SetDisplayFolding(BOOL bFlag = TRUE);
		// @cmember cut selection to clipboard
		virtual void Cut();
		// @cmember copy selection to clipboard
		virtual void Copy();
		// @cmember paste from clipboard
		virtual void Paste();
		// @cmember clear selection
		virtual void Clear();
		// @cmember undo last change
		virtual void Undo();
		// @cmember redo last change
		virtual void Redo();
		// @cmember check if we can undo
		virtual BOOL CanUndo();
		// @cmember check if we can redo
		virtual BOOL CanRedo();
		// @cmember check if we have something to paste from clipboard
		virtual BOOL CanPaste();
		// @cmember select complete text
		virtual void SelectAll();
		// @cmember return the current line number
		virtual long GetCurrentLine();
		// @cmember return the current column number
		virtual long GetCurrentColumn();
		// @cmember return the current character position within the file
		virtual long GetCurrentPosition();
		// @cmember return the current style number at the current character position
		virtual int GetCurrentStyle();
		// @cmember return the current folding level at the current character position
		virtual int GetCurrentFoldinglevel();
		// @cmember set the fontname for a style number
		virtual void SetFontname(int nStyle, LPCTSTR szFontname);
		// @cmember set the fontname height in points for a style number
		virtual void SetFontheight(int nStyle, int nHeight);
		// @cmember set the foregroundcolor for a style number
		virtual void SetForeground(int nStyle, COLORREF crForeground);
		// @cmember set the backgroundcolor for a style number
		virtual void SetBackground(int nStyle, COLORREF crBackground);
		// @cmember set given style to bold
		virtual void SetBold(int nStyle, BOOL bFlag);
		// @cmember set given style to bold
		virtual void SetItalic(int nStyle, BOOL bFlag);
		// @cmember set given style to bold
		virtual void SetUnderline(int nStyle, BOOL bFlag);
		// @cmember get flag if we are in overstrike mode
		virtual BOOL GetOverstrike();
		// @cmember set flag for overstrike mode
		virtual void SetOverstrike(BOOL bOverstrike);
		// @cmember init the view with reasonable defaults
		virtual void Init();
		// @cmember called whenever the text is changed - we can update any indicators and do brace matching
		virtual void UpdateUI();
		// @cmember do default folding 
		virtual void DoDefaultFolding(int nMargin, long lPosition);
		// @cmember refresh lexer and display after new settings
		virtual void Refresh();
		// @cmember add a bookmark = marker type 0
		virtual void AddBookmark(long lLine);
		// @cmember delete a bookmark = marker type 0
		virtual void DeleteBookmark(long lLine);
		// @cmember check for bookmark = marker type 0
		virtual BOOL HasBookmark(long lLine);
		// @cmember Find next bookmark
		virtual void FindNextBookmark();
		// @cmember Find previous bookmark
		virtual void FindPreviousBookmark();
		// @cmember goto line
		virtual void GotoLine(long lLine);
		// @cmember goto position
		virtual void GotoPosition(long lPos);
		// @cmember search forward for a given text
		virtual BOOL SearchForward(LPTSTR szText);
		// @cmember search backward for a given text
		virtual BOOL SearchBackward(LPTSTR szText);
		// @cmember replace a text found by SearchForward or SearchBackward
		virtual void ReplaceSearchedText(LPCTSTR szText);
		// @cmember Set your own lexer
		virtual void SetLexer(int nLexer);
		// @cmember return start position of selection
		virtual long GetSelectionStart();
		// @cmember return end position of selection
		virtual long GetSelectionEnd();
		// @cmember get selected text
		virtual LPTSTR GetSelectedText();
		// @cmember replace all in buffer or selection
		virtual int ReplaceAll(LPTSTR szFind, LPTSTR szReplace, BOOL bSelection = TRUE);
	public:
		// @cmember load text
		int LoadContent (BSTR bstr,LPCTSTR ext=_T(".xml"));
		// @cmember set text
		int SetContent (BSTR bstr,LPCTSTR ext=_T(".xml"));
		// @cmember get text
		int GetContent (BSTR *bstr);
	public:
		int OnFindDlg();
		int OnReplaceDlg();

	public:
		LPTSTR m_pSearchText;
		LPTSTR m_pReplaceText;
		bool m_bSearchFound;
		int SetSearchText(LPTSTR  pStr);
		int SetReplaceText(LPTSTR  pStr);
	};
}}