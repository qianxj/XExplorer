#pragma once
#include "stdafx.h"
#include "ixsedit.hpp"
#include "XCodeFindDlg.hpp"
//#include "xcodereplacedlg.hpp"

namespace Hxsoft{ namespace XFrame
{
		// @access public macro members
		// @cmember return linenumber display flag
		BOOL IXSEdit::GetDisplayLinenumbers (){return m_pEdit->GetDisplayLinenumbers();}

		// @cmember return selection display flag
		BOOL IXSEdit::GetDisplaySelection (){return m_pEdit->GetDisplaySelection();}

		// @cmember return folding margin display flag
		BOOL IXSEdit::GetDisplayFolding (){return m_pEdit->GetDisplayFolding();}

		// @cmember set search flags
		void IXSEdit::SetSearchflags (int nSearchflags){m_pEdit->SetSearchflags(nSearchflags);}

		int IXSEdit::GetSearchflags ()
		{
			return m_pEdit->GetSearchflags();
		}

		// @access public function members
		// @cmember register a window class to use scintilla in a dialog
		//static BOOL Register(CWinApp *app, UINT id);
		// @cmember try to load Lexer DLL
		HMODULE IXSEdit::LoadScintillaDll (LPCTSTR szLexerDll)
		{
			return XSEdit::LoadScintillaDll(szLexerDll);
		}
		// @cmember create window
		//BOOL Create (LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
		// @cmember Set Text to the Scintilla control
		void IXSEdit::SetText(LPCTSTR szText)
		{
			int _convert = WideCharToMultiByte(CP_ACP/*CP_UTF8*/, 0, szText, -1, NULL, 0, NULL, NULL);
			char* str = new char[_convert];
			int nRet = WideCharToMultiByte(CP_ACP/*CP_UTF8*/, 0, szText, -1, str, _convert, NULL, NULL);
			m_pEdit->SetText(str);
		}
		// @cmember Get Text from the Scintilla control
		void IXSEdit::GetText (LPTSTR &strText)
		{
			LPSTR pStr;
			m_pEdit->GetText(pStr);

			int nLen =(int)strlen(pStr);
			int _convert = MultiByteToWideChar(CP_ACP/*CP_UTF8*/, 0, pStr, nLen, NULL, 0);
			strText = new TCHAR[_convert + 1];
			int nRet = MultiByteToWideChar(CP_ACP/*CP_UTF8*/, 0, pStr, nLen, strText, _convert);
			strText[_convert]='\0';
		}

		// @cmember Get Text from the Scintilla control
		LPTSTR IXSEdit::GetText()
		{
			LPTSTR strText;
			GetText(strText);
			return strText;
		}
		// @cmember Load a file
		BOOL IXSEdit::LoadFile (LPCTSTR szFile)
		{
			return m_pEdit->LoadFile(szFile);
		}
		// @cmember Save a file
		BOOL IXSEdit::SaveFile (LPCTSTR szFile)
		{
			return m_pEdit->SaveFile(szFile);
		}
		// @cmember try to find lexer format from extension
		int IXSEdit::GetFormatFromExtension (LPCTSTR szExtension)
		{
			return m_pEdit->GetFormatFromExtension(szExtension);
		}
		// @cmember calcluate number of chars needed for linenumberdisplay
		int IXSEdit::GetLinenumberChars ()
		{
			return m_pEdit->GetLinenumberChars();
		}

		// @cmember calcluate number of pixels for linenumber display
		int IXSEdit::GetLinenumberWidth ()
		{
			return m_pEdit->GetLinenumberWidth();
		}
		// @cmember set display of linenumbers on/off
		void IXSEdit::SetDisplayLinenumbers(BOOL bFlag )
		{
			return m_pEdit->SetDisplayLinenumbers(bFlag);
		}
		// @cmember set display of selection/bookmark margin on/off
		void IXSEdit::SetDisplaySelection(BOOL bFlag )
		{
			return m_pEdit->SetDisplaySelection(bFlag);
		}
		// @cmember set display of source folding margin on/off
		void IXSEdit::SetDisplayFolding(BOOL bFlag )
		{
			return m_pEdit->SetDisplayFolding(bFlag);
		}

		// @cmember cut selection to clipboard
		void IXSEdit::Cut(){return m_pEdit->Cut();}

		// @cmember copy selection to clipboard
		void IXSEdit::Copy(){return m_pEdit->Copy();}

		// @cmember paste from clipboard
		void IXSEdit::Paste(){return m_pEdit->Paste();}

		// @cmember clear selection
		void IXSEdit::Clear(){return m_pEdit->Clear();}

		// @cmember undo last change
		void IXSEdit::Undo(){return m_pEdit->Undo();}

		// @cmember redo last change
		void IXSEdit::Redo(){return m_pEdit->Redo();}

		// @cmember check if we can undo
		BOOL IXSEdit::CanUndo(){return m_pEdit->CanUndo();}

		// @cmember check if we can redo
		BOOL IXSEdit::CanRedo(){return m_pEdit->CanRedo();}

		// @cmember check if we have something to paste from clipboard
		BOOL IXSEdit::CanPaste(){return m_pEdit->CanPaste();}

		// @cmember select complete text
		void IXSEdit::SelectAll(){return m_pEdit->SelectAll();}

		// @cmember return the current line number
		long IXSEdit::GetCurrentLine(){return m_pEdit->GetCurrentLine();}

		// @cmember return the current column number
		long IXSEdit::GetCurrentColumn(){return m_pEdit->GetCurrentColumn();}

		// @cmember return the current character position within the file
		long IXSEdit::GetCurrentPosition(){return m_pEdit->GetCurrentPosition();}

		// @cmember return the current style number at the current character position
		int IXSEdit::GetCurrentStyle(){return m_pEdit->GetCurrentStyle();}

		// @cmember return the current folding level at the current character position
		int IXSEdit:: GetCurrentFoldinglevel(){return m_pEdit->GetCurrentFoldinglevel();}

		// @cmember set the fontname for a style number
		void IXSEdit:: SetFontname(int nStyle, LPCTSTR szFontname){return m_pEdit->SetFontname(nStyle, szFontname);}

		// @cmember set the fontname height in points for a style number
		void IXSEdit:: SetFontheight(int nStyle, int nHeight){return m_pEdit->SetFontheight(nStyle, nHeight);}

		// @cmember set the foregroundcolor for a style number
		void IXSEdit:: SetForeground(int nStyle, COLORREF crForeground){return m_pEdit->SetForeground(nStyle, crForeground);}

		// @cmember set the backgroundcolor for a style number
		void IXSEdit:: SetBackground(int nStyle, COLORREF crBackground){return m_pEdit->SetBackground(nStyle, crBackground);}

		// @cmember set given style to bold
		void IXSEdit:: SetBold(int nStyle, BOOL bFlag){return m_pEdit->SetBold(nStyle, bFlag);}

		// @cmember set given style to bold
		void IXSEdit:: SetItalic(int nStyle, BOOL bFlag){return m_pEdit->SetItalic(nStyle, bFlag);}

		// @cmember set given style to bold
		void IXSEdit:: SetUnderline(int nStyle, BOOL bFlag){return m_pEdit->SetUnderline(nStyle, bFlag);}

		// @cmember get flag if we are in overstrike mode
		BOOL IXSEdit:: GetOverstrike(){return m_pEdit->GetOverstrike();}

		// @cmember set flag for overstrike mode
		void IXSEdit:: SetOverstrike(BOOL bOverstrike){return m_pEdit->SetOverstrike(bOverstrike);}

		// @cmember init the view with reasonable defaults
		void IXSEdit:: Init(){return m_pEdit->Init();}

		// @cmember called whenever the text is changed - we can update any indicators and do brace matching
		void IXSEdit:: UpdateUI(){return m_pEdit->UpdateUI();}

		// @cmember do default folding 
		void IXSEdit:: DoDefaultFolding(int nMargin, long lPosition){return m_pEdit->DoDefaultFolding(nMargin, lPosition);}

		// @cmember refresh lexer and display after new settings
		void IXSEdit:: Refresh(){return m_pEdit->Refresh();}

		// @cmember add a bookmark = marker type 0
		void IXSEdit:: AddBookmark(long lLine){return m_pEdit->AddBookmark(lLine);}

		// @cmember delete a bookmark = marker type 0
		void IXSEdit:: DeleteBookmark(long lLine){return m_pEdit->DeleteBookmark(lLine);}

		// @cmember check for bookmark = marker type 0
		BOOL IXSEdit:: HasBookmark(long lLine){return m_pEdit->HasBookmark(lLine);}

		// @cmember Find next bookmark
		void IXSEdit:: FindNextBookmark(){return m_pEdit->FindNextBookmark();}

		// @cmember Find previous bookmark
		void IXSEdit:: FindPreviousBookmark(){return m_pEdit->FindPreviousBookmark();}

		// @cmember goto line
		void IXSEdit:: GotoLine(long lLine){return m_pEdit->GotoLine(lLine);}

		// @cmember goto position
		void IXSEdit:: GotoPosition(long lPos){return m_pEdit->GotoPosition(lPos);}

		// @cmember search forward for a given text
		BOOL IXSEdit:: SearchForward(LPTSTR szText)
		{
			int _convert = WideCharToMultiByte(CP_UTF8, 0, szText, -1, NULL, 0, NULL, NULL);
			char* str = new char[_convert];
			int nRet = WideCharToMultiByte(CP_UTF8, 0, szText, -1, str, _convert, NULL, NULL);

			int ret = m_pEdit->SearchForward(str);
			if(str)delete str;
			return ret;
		}

		// @cmember search backward for a given text
		BOOL IXSEdit:: SearchBackward(LPTSTR szText)
		{
			int _convert = WideCharToMultiByte(CP_UTF8, 0, szText, -1, NULL, 0, NULL, NULL);
			char* str = new char[_convert];
			int nRet = WideCharToMultiByte(CP_UTF8, 0, szText, -1, str, _convert, NULL, NULL);

			int ret = m_pEdit->SearchBackward(str);
			if(str)delete str;
			return ret;
		}

		// @cmember replace a text found by SearchForward or SearchBackward
		void IXSEdit:: ReplaceSearchedText(LPCTSTR szText)
		{
			int _convert = WideCharToMultiByte(CP_UTF8, 0, szText, -1, NULL, 0, NULL, NULL);
			char* str = new char[_convert];
			int nRet = WideCharToMultiByte(CP_UTF8, 0, szText, -1, str, _convert, NULL, NULL);

			m_pEdit->ReplaceSearchedText(str);
			if(str)delete str;
		}

		// @cmember Set your own lexer
		void IXSEdit:: SetLexer(int nLexer){return m_pEdit->SetLexer(nLexer);}

		// @cmember return start position of selection
		long IXSEdit:: GetSelectionStart(){return m_pEdit->GetSelectionStart();}

		// @cmember return end position of selection
		long IXSEdit:: GetSelectionEnd(){return m_pEdit->GetSelectionEnd();}

		// @cmember get selected text
		LPTSTR IXSEdit:: GetSelectedText(){return m_pEdit->GetSelectedText();}

		// @cmember replace all in buffer or selection
		int IXSEdit:: ReplaceAll(LPTSTR szFind, LPTSTR szReplace, BOOL bSelection )
		{
			int _convert = WideCharToMultiByte(CP_UTF8, 0, szFind, -1, NULL, 0, NULL, NULL);
			char* str = new char[_convert];
			int nRet = WideCharToMultiByte(CP_UTF8, 0, szFind, -1, str, _convert, NULL, NULL);


			_convert = WideCharToMultiByte(CP_UTF8, 0, szReplace, -1, NULL, 0, NULL, NULL);
			char* strReplace = new char[_convert];
			nRet = WideCharToMultiByte(CP_UTF8, 0, szReplace, -1, strReplace, _convert, NULL, NULL);

			int ret = m_pEdit->ReplaceAll(str,strReplace,bSelection);
			if(str)delete str;
			if(strReplace)delete strReplace;

			return ret;
		}

		// @cmember load text
		int IXSEdit::LoadContent (BSTR bstr,LPCTSTR ext)
		{
			return m_pEdit->LoadContent(bstr,ext);
		}
		// @cmember set text
		int IXSEdit::SetContent (BSTR bstr,LPCTSTR ext)
		{
			return m_pEdit->SetContent(bstr,ext);
		}
		// @cmember get text
		int IXSEdit::GetContent (BSTR *bstr)
		{
			return m_pEdit->GetContent(bstr);
		}
		
		int IXSEdit::OnFindDlg()
		{
			XControl::XCodeFindDlg * pFindDlg = new XControl::XCodeFindDlg();
			xfXPage * pPage = new xfXPage();


			pPage->LoadXPage(NULL,_T("uriserver/uri.hxsoft.com/xaserver/dev:xpage[xcode.finddlg]"));
			pPage->m_pStrUrl = _tcsdup(m_pEdit->GetWin()->m_pXPage->m_pStrUrl);
			pFindDlg->Open(pPage,WS_EX_TOPMOST,WS_OVERLAPPEDWINDOW|WS_POPUP,NULL,m_pEdit->GetWin()->GetHWND(),xfApp::GetApp()->GetInstance(),this);
			
			return 1;
		}

		int IXSEdit::OnReplaceDlg()
		{
			XControl::XCodeFindDlg * pFindDlg = new XControl::XCodeFindDlg();
			xfXPage * pPage = new xfXPage();


			pPage->LoadXPage(NULL,_T("uriserver/uri.hxsoft.com/xaserver/dev:xpage[xcode.replacedlg]"));
			pPage->m_pStrUrl = _tcsdup(m_pEdit->GetWin()->m_pXPage->m_pStrUrl);
			pFindDlg->Open(pPage,WS_EX_TOPMOST,WS_OVERLAPPEDWINDOW|WS_POPUP,NULL,m_pEdit->GetWin()->GetHWND(),xfApp::GetApp()->GetInstance(),this);

			return 1;
		}

		int IXSEdit::SetSearchText(LPTSTR  pStr)
		{
			m_pSearchText = pStr;
			return 1;
		}
		int IXSEdit::SetReplaceText(LPTSTR  pStr)
		{
			m_pReplaceText = pStr;
			return 1;
		}

}}