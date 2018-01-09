#pragma once

#include "xframe.h"

namespace Hxsoft{namespace XFrame
{
	typedef enum _SYNTAX_COLORING_TYPE
	{
		SCT_NONE = 0,
		SCT_C_ANSI,
		SCT_C_PLUS_PLUS,
		SCT_HTML,
		SCT_JSCRIPT,
		SCT_SQL

	} SYNTAX_COLORING_TYPE;

	typedef struct _LINEINFO
	{
		DWORD dwOffset;
		int nLen;

	} LINEINFO, *LPLINEINFO;

	typedef struct _TAGINFO
	{
		LPTSTR lpszTagName;
		int nLen;

	} TAGINFO, *LPTAGINFO;

	typedef void (*LPLINEDRAWPROC)(HDC hDC, LPTSTR lpszText, int iLen, RECT rect, int iSelectionStart, int iSelectionEnd);

	class XCONTROL_API xcSourceEdit :
		public xfControl
	{
	public:
		xcSourceEdit(void);
		~xcSourceEdit(void);
	public:
		// Public methods
		BOOL Load(LPTSTR lpszFilePath);
		BOOL Save(LPTSTR lpszFilePath);
		void AddText(LPTSTR lpszText, int iLen);
		void DeleteText(int iStartLine, int iStartChar, int iEndLine, int iEndChar);
		LPTSTR GetTextBuffer();
		int GetBufferLen();
		void GetTextFromLine(int iLineIndex, LPTSTR lpszText, int iLen);
		void SetKeywordColor(COLORREF cKeywordColor);
		COLORREF GetKeywordColor();
		void SetKeywords(LPTSTR lpszKeywords, int iLen, BOOL bCaseSensitive);
		void SetConstantColor(COLORREF cConstantColor);
		COLORREF GetConstantColor();
		void SetConstants(LPTSTR lpszConstants, int iLen, BOOL bCaseSensitive);
		void SetCommentColor(COLORREF cCommentColor);
		COLORREF GetCommentColor();
		void SetTextColor(COLORREF cTextColor);
		COLORREF GetTextColor();
		void SetSyntaxColoring(SYNTAX_COLORING_TYPE syntaxColoring);
		SYNTAX_COLORING_TYPE GetSyntaxColoring();
		void SetCurrentLine(int index);
		int GetCurrentLine();
		void SetCurrentChar(int index);
		int GetCurrentChar();
		void SetTABs(int iTABs);
		int GetTABs();
		void EnsureVisible();
		void SetSelection(int iStartLine, int iStartChar, int iEndLine, int iEndChar);
		void GetSelection(int& iStartLine, int& iStartChar, int& iEndLine, int& iEndChar);
		int GetNumberLines();
		void SetBackgroundColor(COLORREF cBgColor);
		COLORREF GetBackgroundColor();
		void SetLineDrawProc(LPLINEDRAWPROC lpfnLineDrawProc);
		void DefaultLineDrawProc(LPTSTR lpszText, int iLen, RECT rect, int iSelectionStart, int iSelectionEnd);

	private:
		// Private methods
		void UpdateControl();
		void DeleteText();
		SIZE GetLineSize(LPTSTR lpszText, int iLen);
		void DrawContent();
		void DrawLine(LPTSTR lpszText, int iLen, RECT rect, int iSelectionStart, int iSelectionEnd);
		void ProcessLine(LPTSTR lpszText, int iLen);
		int LineFromPosition(POINT pt);
		int CharFromPosition(int iLineIndex, POINT pt);
		POINT PositionFromChar(int iLineIndex, int iCharIndex);
		int GetCharOffset(int iLineIndex, int iCharIndex);
		void UpdateCaret();
		void EnsureVisible(int iLineIndex, int iCharIndex);
		LPTSTR GetLine(int& iOffset);
		void SelectWord(POINT pt);
		BOOL IsSpecial(_TCHAR tch);
		BOOL IsLetter(_TCHAR tch);
		BOOL IsKeyword(LPTSTR lpszText, int iLen, int& iKeywordIndex, int& iKeywordOffset);
		BOOL IsConstant(LPTSTR lpszText, int iLen, int& iConstantIndex, int& iConstantOffset);
		BOOL IsText(LPTSTR lpszText, int iLen, int& iTextStart, int& iTextEnd);
		void ShowAutoComplete();

	public:
		virtual bool EvtEraseBkgnd(TEvent *pEvent,LPARAM lparam);
		virtual bool EvtSize(TEvent *pEvent,LPARAM lparam);
		virtual bool EvtPaint(TEvent *pEvent,LPARAM lparam);
		virtual bool EvtVScroll(TEvent *pEvent,LPARAM lparam);
		virtual bool EvtHScroll(TEvent *pEvent,LPARAM lparam);
		virtual bool EvtMouseWheel(TEvent *pEvent,LPARAM lparam);
		virtual bool EvtLButtonDown(TEvent *pEvent,LPARAM lparam);
		virtual bool EvtMouseMove(TEvent *pEvent,LPARAM lparam);
		virtual bool EvtLButtonUp(TEvent *pEvent,LPARAM lparam);
		virtual bool EvtLButtonDblClk(TEvent *pEvent,LPARAM lparam);
		virtual bool EvtSetFocus(TEvent *pEvent,LPARAM lparam);
		virtual bool EvtKeyDown(TEvent *pEvent,LPARAM lparam);
		virtual bool EvtKeyUp(TEvent *pEvent,LPARAM lparam);
		virtual bool EvtChar(TEvent *pEvent,LPARAM lparam);
		virtual bool EvtSetCursor(TEvent *pEvent,LPARAM lparam);

		int SetEventListions();
		int UnSetEventListions();

	public:
		BOOL OnEraseBkgnd(HDC pDC);
		void OnSize(UINT nType, int cx, int cy);
		void OnPaint();
		void OnVScroll(UINT nSBCode, UINT nPos, void* pScrollBar);
		void OnHScroll(UINT nSBCode, UINT nPos, void* pScrollBar);
		BOOL OnMouseWheel(UINT nFlags, short zDelta, POINT pt);
		void OnLButtonDown(UINT nFlags, POINT point);
		void OnMouseMove(UINT nFlags, POINT point);
		void OnLButtonUp(UINT nFlags, POINT point);
		void OnLButtonDblClk(UINT nFlags, POINT point);
		void OnSetFocus(HWND pOldWnd);
		void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
		void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
		void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
		BOOL OnSetCursor(HWND pWnd, UINT nHitTest, UINT message);

	private:
		HDC m_hMemDC;
		HBITMAP m_hMemBitmap;
		HBITMAP m_pOldMemBitmap;
		HFONT m_hFont;
		HFONT m_pOldFont;
		LPTSTR m_lpszText;
		int m_iSize;
		SIZE m_szTextSize;
		int m_iLineStartSelection;
		int m_iLineEndSelection;
		int m_iCharStartSelection;
		int m_iCharEndSelection;
		int m_iNumberLines;
		LPLINEINFO m_lpLineInfo;
		COLORREF m_cKeywordColor;
		LPTAGINFO m_lpKeywordInfo;
		int m_iNumberKeywords;
		BOOL m_bKeywordsCaseSensitive;
		COLORREF m_cConstantColor;
		LPTAGINFO m_lpConstantInfo;
		int m_iNumberConstants;
		BOOL m_bConstantsCaseSensitive;
		COLORREF m_cCommentColor;
		COLORREF m_cTextColor;
		SYNTAX_COLORING_TYPE m_SyntaxColoring;
		int m_iCurrentLine;
		int m_iCurrentChar;
		int m_iTABs;
		BOOL m_bComment;
		BOOL m_bText;
		BOOL m_bSelecting;
		BOOL m_bCopying;
		HWND  m_hListBox;
		COLORREF m_cBgColor;
		LPLINEDRAWPROC m_lpfnLineDrawProc;
	public:
		int Invalidate(BOOL bDraw);
		int UpdateWindow();
		int FillSolidRect(HDC hDC,LPCRECT lpRect, COLORREF clr);
	public:
		virtual int DoDraw(HDC hdc,RECT * pRect);
		int LoadContent(LPTSTR pStr,SYNTAX_COLORING_TYPE type = SCT_C_PLUS_PLUS);
		int GetContent(BSTR &bstr);
	};
}}