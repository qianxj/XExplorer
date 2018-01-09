#include "StdAfx.h"
#include "..\include\xcSourceEdit.hpp"

namespace Hxsoft{namespace XFrame
{
	xcSourceEdit::xcSourceEdit()
	{
		m_hMemDC = NULL;
		m_hMemBitmap = NULL;
		m_pOldMemBitmap = NULL;
		m_pOldFont = NULL;

		m_hListBox = NULL;

		// Init members
		m_iSize = 2;
		m_lpszText = (LPTSTR)malloc(m_iSize*sizeof(_TCHAR));
		m_lpszText[0] = '\r';
		m_lpszText[1] = '\n';
		m_hFont = ::CreateFont(-14,0,0,0,0,0,0,0,0,0,0,0,0,_T("Courier New"));
		m_iLineStartSelection = m_iLineEndSelection = -1;
		m_iCharStartSelection = m_iCharEndSelection = -1;
		m_iNumberLines = 1;
		m_lpLineInfo = (LPLINEINFO)malloc(m_iNumberLines*sizeof(LINEINFO));
		m_lpLineInfo[m_iNumberLines-1].dwOffset = 0;
		m_lpLineInfo[m_iNumberLines-1].nLen = 0;
		m_cKeywordColor = RGB(0,0,0);
		m_lpKeywordInfo = NULL;
		m_iNumberKeywords = 0;
		m_bKeywordsCaseSensitive = NULL;
		m_cConstantColor = RGB(0,0,0);
		m_lpConstantInfo = NULL;
		m_iNumberConstants = 0;
		m_bConstantsCaseSensitive = NULL;
		m_cCommentColor = RGB(0,0,0);
		m_cTextColor = RGB(0,0,0);
		m_SyntaxColoring = SCT_NONE;
		m_iCurrentLine = 0;
		m_iCurrentChar = 0;
		m_iTABs = 4;
		m_bComment = FALSE;
		m_bText = FALSE;
		m_bSelecting = FALSE;
		m_bCopying = FALSE;
		m_cBgColor = RGB(255,255,255);
		m_lpfnLineDrawProc = NULL;

		SetEventListions();
	}

	xcSourceEdit::~xcSourceEdit()
	{
		// Destroy caret
		DestroyCaret();

		// Clear data
		if (m_lpszText != NULL)
			free(m_lpszText);
		if (m_lpLineInfo != NULL)
		{
			free(m_lpLineInfo);
			m_lpLineInfo = NULL;
		}
		for (int i=0; i<m_iNumberKeywords; i++)
			delete m_lpKeywordInfo[i].lpszTagName;
		free(m_lpKeywordInfo);
		m_lpKeywordInfo = NULL;
		for (int i=0; i<m_iNumberConstants; i++)
			delete m_lpConstantInfo[i].lpszTagName;
		free(m_lpConstantInfo);
		m_lpConstantInfo = NULL;


		if(m_hMemDC)
		{
			::SelectObject(m_hMemDC,m_pOldMemBitmap);
			::SelectObject(m_hMemDC,m_pOldFont);
			::DeleteObject(m_hMemDC);
			::DeleteObject(m_hMemBitmap);
			::DeleteObject(m_hFont);
		}

		m_hMemDC = NULL;
		m_hMemBitmap = NULL;
		m_pOldMemBitmap = NULL;
		m_hFont = NULL;
		m_pOldFont = NULL;

		UnSetEventListions();
	}

	int xcSourceEdit::SetEventListions()
	{
		this->AttachListon(WM_ERASEBKGND, (FNEvent)&xcSourceEdit::EvtEraseBkgnd);
		this->AttachListon(WM_SIZE, (FNEvent)&xcSourceEdit::EvtSize);
		this->AttachListon(WM_PAINT, (FNEvent)&xcSourceEdit::EvtPaint);
		this->AttachListon(WM_VSCROLL, (FNEvent)&xcSourceEdit::EvtVScroll);
		this->AttachListon(WM_HSCROLL, (FNEvent)&xcSourceEdit::EvtHScroll);
		this->AttachListon(WM_MOUSEWHEEL, (FNEvent)&xcSourceEdit::EvtMouseWheel);
		this->AttachListon(WM_LBUTTONDOWN, (FNEvent)&xcSourceEdit::EvtLButtonDown);

		this->AttachListon(WM_MOUSEMOVE, (FNEvent)&xcSourceEdit::EvtMouseMove);
		this->AttachListon(WM_LBUTTONUP, (FNEvent)&xcSourceEdit::EvtLButtonUp);
		this->AttachListon(WM_LBUTTONDBLCLK, (FNEvent)&xcSourceEdit::EvtLButtonDblClk);

		this->AttachListon(WM_SETFOCUS, (FNEvent)&xcSourceEdit::EvtSetFocus);
		this->AttachListon(WM_KEYDOWN, (FNEvent)&xcSourceEdit::EvtKeyDown);
		this->AttachListon(WM_KEYUP, (FNEvent)&xcSourceEdit::EvtKeyUp);
		this->AttachListon(WM_CHAR, (FNEvent)&xcSourceEdit::EvtChar);
		this->AttachListon(WM_SETCURSOR,(FNEvent)&xcSourceEdit::EvtSetCursor);

		return 1;
	}
	int xcSourceEdit::UnSetEventListions()
	{
		this->ClearAllListonsTo();
		return 1;
	}

	bool xcSourceEdit::EvtEraseBkgnd(TEvent *pEvent,LPARAM lparam)
	{
		pEvent->dwRet = OnEraseBkgnd((HDC)pEvent->wParam);
		return true;
	}
	bool xcSourceEdit::EvtSize(TEvent *pEvent,LPARAM lparam)
	{
		OnSize(pEvent->size.nFlags,pEvent->size.point.x,pEvent->size.point.y);
		return true;
	}
	bool xcSourceEdit::EvtPaint(TEvent *pEvent,LPARAM lparam)
	{
		OnPaint();
		return true;
	}
	bool xcSourceEdit::EvtVScroll(TEvent *pEvent,LPARAM lparam)
	{
		OnVScroll(pEvent->scroll.nSBCode,pEvent->scroll.nPos,NULL);
		return true;
	}
	bool xcSourceEdit::EvtHScroll(TEvent *pEvent,LPARAM lparam)
	{
		OnHScroll(pEvent->scroll.nSBCode,pEvent->scroll.nPos,NULL);
		return true;
	}
	bool xcSourceEdit::EvtMouseWheel(TEvent *pEvent,LPARAM lparam)
	{
		OnMouseWheel((UINT)pEvent->wheel.fwKeys,pEvent->wheel.zDelta,pEvent->wheel.point);
		return true;
	}
	bool xcSourceEdit::EvtLButtonDown(TEvent *pEvent,LPARAM lparam)
	{
		OnLButtonDown(pEvent->mouse.nFlags,pEvent->mouse.point);
		return true;
	}
	bool xcSourceEdit::EvtMouseMove(TEvent *pEvent,LPARAM lparam)
	{
		OnMouseMove(pEvent->mouse.nFlags,pEvent->mouse.point);
		return true;
	}
	bool xcSourceEdit::EvtLButtonUp(TEvent *pEvent,LPARAM lparam)
	{
		::SetFocus(m_hWnd);
		OnLButtonUp(pEvent->mouse.nFlags,pEvent->mouse.point);
		return true;
	}
	bool xcSourceEdit::EvtLButtonDblClk(TEvent *pEvent,LPARAM lparam)
	{
		OnLButtonDblClk(pEvent->mouse.nFlags,pEvent->mouse.point);
		return true;
	}
	bool xcSourceEdit::EvtSetFocus(TEvent *pEvent,LPARAM lparam)
	{
		OnSetFocus((HWND)pEvent->wParam);
		return true;
	}
	bool xcSourceEdit::EvtKeyDown(TEvent *pEvent,LPARAM lparam)
	{
		OnKeyDown(pEvent->key.nChar,pEvent->key.nRepCnt,pEvent->key.nFlags);
		return true;
	}
	bool xcSourceEdit::EvtKeyUp(TEvent *pEvent,LPARAM lparam)
	{
		OnKeyUp(pEvent->key.nChar,pEvent->key.nRepCnt,pEvent->key.nFlags);
		return true;
	}
	bool xcSourceEdit::EvtChar(TEvent *pEvent,LPARAM lparam)
	{
		OnChar(pEvent->key.nChar,pEvent->key.nRepCnt,pEvent->key.nFlags);
		return true;
	}

	bool xcSourceEdit::EvtSetCursor(TEvent *pEvent,LPARAM lparam)
	{
		OnSetCursor(pEvent->cursor.hWnd,pEvent->cursor.nHitTest,pEvent->cursor.mouseMsg);
		return true;
	}


	/*
	BEGIN_MESSAGE_MAP(xcSourceEdit, CWnd)
		ON_WM_ERASEBKGND()
		ON_WM_PAINT()
		ON_WM_SIZE()
		ON_WM_VSCROLL()
		ON_WM_HSCROLL()
		ON_WM_LBUTTONDOWN()
		ON_WM_MOUSEMOVE()
		ON_WM_LBUTTONUP()
		ON_WM_LBUTTONDBLCLK()
		ON_WM_SETFOCUS()
		ON_WM_KEYDOWN()
		ON_WM_KEYUP()
		ON_WM_CHAR()
		ON_WM_SETCURSOR()
		ON_WM_MOUSEWHEEL()
	END_MESSAGE_MAP()
	*/


	void xcSourceEdit::DrawContent()
	{
		// Check for valid data
		if (m_lpszText != NULL)
		{
			// Get client rectangle
			RECT rcClient;
			GetClientRect(&rcClient);

			// Get scroll info
			int iVerticalOffset = GetScrollPos(m_hWnd,SB_VERT);
			int iHorizontalOffset = GetScrollPos(m_hWnd, SB_HORZ);

			// Get visible lines range
			POINT ptTop = {0, rcClient.top+iVerticalOffset};
			POINT ptBottom = {0, (rcClient.bottom-rcClient.top)+iVerticalOffset};
			int iLineStart = LineFromPosition(ptTop);
			int iLineEnd = LineFromPosition(ptBottom);

			// Process invisible lines
			m_bComment = FALSE;
			m_bText = FALSE;
			int iTmpOffset = 0;
			int iTmpOldOffset = iTmpOffset;
			int iTmpCurrentLine = 0;
			LPTSTR lpszTmpNextLine = NULL;
			while ((lpszTmpNextLine=GetLine(iTmpOffset)) != NULL)
			{
				// Process only invisible lines
				if (iTmpCurrentLine < iLineStart)
				{
					// Process line
					int iLen = iTmpOffset - iTmpOldOffset;
					ProcessLine(lpszTmpNextLine, m_lpLineInfo[iTmpCurrentLine].nLen);
				}
				iTmpOldOffset = iTmpOffset;
				iTmpCurrentLine++;

				// Exit
				if (iTmpCurrentLine >= iLineStart)
					break;
			}

			// Draw lines
			RECT rcLine = {-iHorizontalOffset, 0, rcClient.right, m_szTextSize.cy};
			int iOffset = m_lpLineInfo[iLineStart].dwOffset;
			int iOldOffset = iOffset;
			int iCurrentLine = iLineStart;
			LPTSTR lpszNextLine = NULL;
			while ((lpszNextLine=GetLine(iOffset)) != NULL)
			{
				// Draw only visible lines
				if ((iCurrentLine >= iLineStart) && (iCurrentLine <= iLineEnd))
				{
					// Get visible chars range
					POINT ptLeft = {iHorizontalOffset, 0};
					POINT ptRight = {rcLine.right+iHorizontalOffset, 0};
					int iCharStart = CharFromPosition(iCurrentLine, ptLeft);
					int iCharEnd = CharFromPosition(iCurrentLine, ptRight);

					// Calculate text length
					int iLen = iOffset - iOldOffset;

					// Check for valid selection
					if ((m_iLineStartSelection != m_iLineEndSelection) || (m_iCharStartSelection != m_iCharEndSelection))
					{
						// Format selection
						int iLineStartSelection = min(m_iLineStartSelection, m_iLineEndSelection);
						int iLineEndSelection = max(m_iLineStartSelection, m_iLineEndSelection);
						int iCharStartSelection = m_iCharStartSelection;
						int iCharEndSelection = m_iCharEndSelection;

						// Check for selected text
						if ((iCurrentLine >= iLineStartSelection) && (iCurrentLine <= iLineEndSelection))
						{
							// Calculate selected chars
							int iSelectionStart;
							if (iCurrentLine == iLineStartSelection)
							{
								if (m_iLineStartSelection < m_iLineEndSelection)
									iSelectionStart = max(iCharStartSelection, iCharStart);
								else if (m_iLineStartSelection > m_iLineEndSelection)
									iSelectionStart = max(iCharEndSelection, iCharStart);
								else
									iSelectionStart = max(min(m_iCharStartSelection, m_iCharEndSelection), iCharStart);
							}
							else if (iCurrentLine == iLineEndSelection)
							{
								if (m_iLineStartSelection < m_iLineEndSelection)
									iSelectionStart = iCharStart;
								else if (m_iLineStartSelection > m_iLineEndSelection)
									iSelectionStart = iCharStart;
								else
									iSelectionStart = max(min(m_iCharStartSelection, m_iCharEndSelection), iCharStart);
							}
							else
								iSelectionStart = iCharStart;
							int iSelectionEnd;
							if (iCurrentLine == iLineStartSelection)
							{
								if (m_iLineStartSelection < m_iLineEndSelection)
									iSelectionEnd = iCharEnd;
								else if (m_iLineStartSelection > m_iLineEndSelection)
									iSelectionEnd = iCharEnd;
								else
									iSelectionEnd = min(max(m_iCharStartSelection, m_iCharEndSelection), iCharEnd);
							}
							else if (iCurrentLine == iLineEndSelection)
							{
								if (m_iLineStartSelection < m_iLineEndSelection)
									iSelectionEnd = min(iCharEndSelection, iCharEnd);
								else if (m_iLineStartSelection > m_iLineEndSelection)
									iSelectionEnd = min(iCharStartSelection, iCharEnd);
								else
									iSelectionEnd = min(max(m_iCharStartSelection, m_iCharEndSelection), iCharEnd);
							}
							else
								iSelectionEnd = iCharEnd;
							int iSelected = iSelectionEnd - iSelectionStart;
							if (iSelected > 0)
							{
								// Draw selected text
								if (m_lpfnLineDrawProc == NULL)
									DrawLine(lpszNextLine, m_lpLineInfo[iCurrentLine].nLen, rcLine, iSelectionStart, iSelectionEnd);
								else
									m_lpfnLineDrawProc(m_hMemDC, lpszNextLine, m_lpLineInfo[iCurrentLine].nLen, rcLine, iSelectionStart, iSelectionEnd);
							}
							else
							{
								// Draw normal text
								if (m_lpfnLineDrawProc == NULL)
									DrawLine(lpszNextLine, m_lpLineInfo[iCurrentLine].nLen, rcLine, -1, -1);
								else
									m_lpfnLineDrawProc(m_hMemDC, lpszNextLine, m_lpLineInfo[iCurrentLine].nLen, rcLine, -1, -1);
							}
						}
						else
						{
							// Draw normal text
							if (m_lpfnLineDrawProc == NULL)
								DrawLine(lpszNextLine, m_lpLineInfo[iCurrentLine].nLen, rcLine, -1, -1);
							else
								m_lpfnLineDrawProc(m_hMemDC, lpszNextLine, m_lpLineInfo[iCurrentLine].nLen, rcLine, -1, -1);
						}
					}
					else
					{
						// Draw normal text
						if (m_lpfnLineDrawProc == NULL)
							DrawLine(lpszNextLine, m_lpLineInfo[iCurrentLine].nLen, rcLine, -1, -1);
						else
							m_lpfnLineDrawProc(m_hMemDC, lpszNextLine, m_lpLineInfo[iCurrentLine].nLen, rcLine, -1, -1);
					}

					// Update text rectangle
					OffsetRect(&rcLine, 0, m_szTextSize.cy);
				}
				iOldOffset = iOffset;
				iCurrentLine++;

				// Exit
				if (iCurrentLine > iLineEnd)
					break;
			}
		}
	}

	void xcSourceEdit::DrawLine(LPTSTR lpszText, int iLen, RECT rect, int iSelectionStart, int iSelectionEnd)
	{
		// Draw single line of text
		int iStartTAB=-1, iEndTAB, iTABs;
		int iStart=0, i=0;
		int iEnd=-1, iCount;
		POINT pt = {rect.left, rect.top};
		BOOL bComment = m_bComment;
		BOOL bText = m_bText;
		while (i <= iLen)
		{
			// Check for special char
			if ((IsSpecial(lpszText[i])) || (i == iLen))
			{
				// Get word end position
				iEnd = i - 1;
				iCount = iEnd - iStart + 1;

				// Check for comment
				BOOL bTurnCommentOff = FALSE;
				if ((!bComment) && (!m_bComment))
				{
					switch (m_SyntaxColoring)
					{
					case SCT_C_ANSI:
					case SCT_C_PLUS_PLUS:
					case SCT_JSCRIPT:
						{
							if (_tcsncmp(lpszText+iStart, _T("//"), 2) == 0)
								bComment = TRUE;
							else if (_tcsncmp(lpszText+iStart, _T("/*"), 2) == 0)
							{
								m_bComment = TRUE;
								bComment = TRUE;
							}
						}
						break;

					case SCT_HTML:
						{
							if (_tcsncmp(lpszText+iStart, _T("<!--"), 4) == 0)
							{
								m_bComment = TRUE;
								bComment = TRUE;
							}
						}
						break;

					case SCT_SQL:
						{
							if (_tcsncmp(lpszText+iStart, _T("--"), 2) == 0)
								bComment = TRUE;
							else if (_tcsncmp(lpszText+iStart, _T("/*"), 2) == 0)
							{
								m_bComment = TRUE;
								bComment = TRUE;
							}
						}
						break;
					}
				}
				else if (m_bComment)
				{
					switch (m_SyntaxColoring)
					{
					case SCT_C_ANSI:
					case SCT_C_PLUS_PLUS:
					case SCT_JSCRIPT:
					case SCT_SQL:
						{
							if (_tcsncmp(lpszText+iStart, _T("*/"), 2) == 0)
							{
								m_bComment = FALSE;
								bTurnCommentOff = TRUE;
							}
						}
						break;

					case SCT_HTML:
						{
							if (_tcsncmp(lpszText+iStart, _T("-->"), 3) == 0)
							{
								m_bComment = FALSE;
								bTurnCommentOff = TRUE;
							}
						}
						break;
					}
				}

				// Check for comment
				if (!bComment)
				{
					// Draw normal word
					TextOut(m_hMemDC,pt.x, pt.y, lpszText+iStart, iCount);

					// Check for keyword
					int iKeywordIndex = -1;
					int iKeywordOffset = -1;
					if (IsKeyword(lpszText+iStart, iCount, iKeywordIndex, iKeywordOffset))
					{
						// Draw keyword
						COLORREF cOldTextColor = ::SetTextColor(m_hMemDC,m_cKeywordColor);
						TextOut(m_hMemDC, pt.x+iKeywordOffset*m_szTextSize.cx, pt.y, lpszText+iStart+iKeywordOffset, m_lpKeywordInfo[iKeywordIndex].nLen-1);
						::SetTextColor(m_hMemDC,cOldTextColor);
					}

					// Check for constant
					int iConstantIndex = -1;
					int iConstantOffset = -1;
					if (IsConstant(lpszText+iStart, iCount, iConstantIndex, iConstantOffset))
					{
						// Draw constant
						COLORREF cOldTextColor = ::SetTextColor(m_hMemDC,m_cConstantColor);
						TextOut(m_hMemDC,pt.x+iConstantOffset*m_szTextSize.cx, pt.y, m_lpConstantInfo[iConstantIndex].lpszTagName, m_lpConstantInfo[iConstantIndex].nLen-1);
						::SetTextColor(m_hMemDC,cOldTextColor);
					}

					// Check for text
					int iTextStart = -1;
					int iTextEnd = -1;
					if (IsText(lpszText+iStart, iCount, iTextStart, iTextEnd) || (bText))
					{
						// Update text offsets
						if (!bText)
						{
							if (iTextEnd == -1)
							{
								iTextEnd = iCount - 1;
								bText = TRUE;
							}
						}
						else
						{
							if (iTextEnd == -1)
							{
								iTextEnd = iTextStart;
								if (iTextEnd == -1)
									iTextEnd = iCount - 1;
								else
									bText = FALSE;
								iTextStart = 0;
							}
						}

						// Draw text
						COLORREF cOldTextColor = ::SetTextColor(m_hMemDC,m_cTextColor);
						TextOut(m_hMemDC,pt.x+iTextStart*m_szTextSize.cx, pt.y, lpszText+iStart+iTextStart, iTextEnd-iTextStart+1);
						::SetTextColor(m_hMemDC,cOldTextColor);
						m_bText = bText;
					}
				}
				else
				{
					// Draw comment
					COLORREF cOldTextColor = ::SetTextColor(m_hMemDC,m_cCommentColor);
					TextOut(m_hMemDC,pt.x, pt.y, lpszText+iStart, iCount);
					::SetTextColor(m_hMemDC,cOldTextColor);
					if (bTurnCommentOff == TRUE)
						bComment = FALSE;
				}

				// Check for valid selection
				int iWordStartSelection = max(iStart, iSelectionStart);
				int iWordEndSelection = min(iEnd+1, iSelectionEnd);
				int iNumberSelected = iWordEndSelection - iWordStartSelection;
				int iSelectionOffset = (iWordStartSelection-iStart) * m_szTextSize.cx;
				if (iNumberSelected > 0)
				{
					// Draw selection
					COLORREF cOldTextColor = ::SetTextColor(m_hMemDC,RGB(255,255,255));
					COLORREF cOldBkColor = ::SetBkColor(m_hMemDC,RGB(0,0,0));
					TextOut(m_hMemDC,pt.x+iSelectionOffset, pt.y, lpszText+iWordStartSelection, iNumberSelected);
					::SetBkColor(m_hMemDC,cOldBkColor);
					::SetTextColor(m_hMemDC,cOldTextColor);
				}

				// Update offset
				pt.x += (iCount * m_szTextSize.cx);

				// Check for SPACE char
				if (lpszText[i] == ' ')
				{
					// Check for valid selection
					if ((i >= iSelectionStart) && (i < iSelectionEnd))
					{
						// Draw selection
						COLORREF cOldTextColor = ::SetTextColor(m_hMemDC,RGB(255,255,255));
						COLORREF cOldBkColor = ::SetBkColor(m_hMemDC,RGB(0,0,0));
						TextOut(m_hMemDC,pt.x, pt.y, _T(" "), 1);
						::SetBkColor(m_hMemDC,cOldBkColor);
						::SetTextColor(m_hMemDC,cOldTextColor);
					}

					// Update offset
					pt.x += m_szTextSize.cx;
					iStart = i + 1;
				}
				// Check for TAB char
				else if (lpszText[i] == '\t')
				{
					// Update TABs
					if (iStartTAB == -1)
					{
						iTABs = m_iTABs;
						iStartTAB = i;
					}
					else
					{
						iEndTAB = i;
						iTABs = m_iTABs - ((iEndTAB - iStartTAB) % m_iTABs);
						if (((iEndTAB - iStartTAB) % m_iTABs) != 0)
							iTABs++;
						if ((iEndTAB - iStartTAB) == 1)
							iTABs = m_iTABs;
						iStartTAB = iEndTAB;
					}

					// Check for valid selection
					if ((i >= iSelectionStart) && (i < iSelectionEnd))
					{
						// Draw selection
						COLORREF cOldTextColor = ::SetTextColor(m_hMemDC,RGB(255,255,255));
						COLORREF cOldBkColor = ::SetBkColor(m_hMemDC,RGB(0,0,0));
						TextOut(m_hMemDC,pt.x, pt.y, _T("        "), iTABs);
						::SetBkColor(m_hMemDC,cOldBkColor);
						::SetTextColor(m_hMemDC,cOldTextColor);
					}

					// Update offset
					pt.x += (iTABs * m_szTextSize.cx);
					iStart = i + 1;
				}
				else
					break;
			}
			i++;
		}
	}

	void xcSourceEdit::ProcessLine(LPTSTR lpszText, int iLen)
	{
		// Process single line of text
		int iStartTAB=-1, iEndTAB, iTABs;
		int iStart=0, i=0;
		int iEnd=-1, iCount;
		BOOL bComment = m_bComment;
		BOOL bText = m_bText;
		while (i <= iLen)
		{
			// Check for special char
			if ((IsSpecial(lpszText[i])) || (i == iLen))
			{
				// Get word end position
				iEnd = i - 1;
				iCount = iEnd - iStart + 1;

				// Check for comment
				BOOL bTurnCommentOff = FALSE;
				if ((!bComment) && (!m_bComment))
				{
					switch (m_SyntaxColoring)
					{
					case SCT_C_ANSI:
					case SCT_C_PLUS_PLUS:
					case SCT_JSCRIPT:
						{
							if (_tcsncmp(lpszText+iStart, _T("//"), 2) == 0)
								bComment = TRUE;
							else if (_tcsncmp(lpszText+iStart, _T("/*"), 2) == 0)
							{
								m_bComment = TRUE;
								bComment = TRUE;
							}
						}
						break;

					case SCT_HTML:
						{
							if (_tcsncmp(lpszText+iStart, _T("<!--"), 4) == 0)
							{
								m_bComment = TRUE;
								bComment = TRUE;
							}
						}
						break;

					case SCT_SQL:
						{
							if (_tcsncmp(lpszText+iStart, _T("--"), 2) == 0)
								bComment = TRUE;
							else if (_tcsncmp(lpszText+iStart, _T("/*"), 2) == 0)
							{
								m_bComment = TRUE;
								bComment = TRUE;
							}
						}
						break;
					}
				}
				else if (m_bComment)
				{
					switch (m_SyntaxColoring)
					{
					case SCT_C_ANSI:
					case SCT_C_PLUS_PLUS:
					case SCT_JSCRIPT:
					case SCT_SQL:
						{
							if (_tcsncmp(lpszText+iStart, _T("*/"), 2) == 0)
							{
								m_bComment = FALSE;
								bTurnCommentOff = TRUE;
							}
						}
						break;

					case SCT_HTML:
						{
							if (_tcsncmp(lpszText+iStart, _T("-->"), 3) == 0)
							{
								m_bComment = FALSE;
								bTurnCommentOff = TRUE;
							}
						}
						break;
					}
				}

				// Check for comment
				if (!bComment)
				{
					// Check for text
					int iTextStart = -1;
					int iTextEnd = -1;
					if (IsText(lpszText+iStart, iCount, iTextStart, iTextEnd) || (bText))
					{
						// Update text offsets
						if (!bText)
						{
							if (iTextEnd == -1)
							{
								iTextEnd = iCount - 1;
								bText = TRUE;
							}
						}
						else
						{
							if (iTextEnd == -1)
							{
								iTextEnd = iTextStart;
								if (iTextEnd == -1)
									iTextEnd = iCount - 1;
								else
									bText = FALSE;
								iTextStart = 0;
							}
						}
						m_bText = bText;
					}
				}
				else
				{
					// Turn-off comment
					if (bTurnCommentOff == TRUE)
						bComment = FALSE;
				}

				// Check for SPACE char
				if (lpszText[i] == ' ')
				{
					// Update offset
					iStart = i + 1;
				}
				// Check for TAB char
				else if (lpszText[i] == '\t')
				{
					// Update TABs
					if (iStartTAB == -1)
					{
						iTABs = m_iTABs;
						iStartTAB = i;
					}
					else
					{
						iEndTAB = i;
						iTABs = m_iTABs - ((iEndTAB - iStartTAB) % m_iTABs);
						if (((iEndTAB - iStartTAB) % m_iTABs) != 0)
							iTABs++;
						if ((iEndTAB - iStartTAB) == 1)
							iTABs = m_iTABs;
						iStartTAB = iEndTAB;
					}

					// Update offset
					iStart = i + 1;
				}
				else
					break;
			}
			i++;
		}
	}

	void xcSourceEdit::UpdateControl()
	{
		// Clear drawing flag
		//SetRedraw(FALSE);
		::SendMessage(m_hWnd, WM_SETREDRAW, FALSE, 0);

		// Clear line info
		if (m_lpLineInfo != NULL)
		{
			free(m_lpLineInfo);
			m_lpLineInfo = NULL;
		}

		// Get client rectangle
		RECT rcClient;
		GetClientRect(&rcClient);

		// Get line size
		//m_szTextSize = ::GetTextExtent(m_hMemDC,(_T("A"), 1);
		::GetTextExtentPoint32(m_hMemDC,_T("A"), 1 ,&m_szTextSize);

		// Calculate content size
		int iWidth = 0;
		int iHeight = 0;
		int iOffset = 0;
		int iOldOffset = iOffset;
		m_iNumberLines = 0;
		LPTSTR lpszNextLine = NULL;
		while ((lpszNextLine=GetLine(iOffset)) != NULL)
		{
			// Update line info
			m_iNumberLines++;
			if (m_lpLineInfo == NULL)
				m_lpLineInfo = (LPLINEINFO)malloc(m_iNumberLines*sizeof(LINEINFO));
			else
				m_lpLineInfo = (LPLINEINFO)realloc(m_lpLineInfo, m_iNumberLines*sizeof(LINEINFO));
			m_lpLineInfo[m_iNumberLines-1].dwOffset = iOldOffset;

			// Calculate line length
			int iLen = iOffset - iOldOffset;
			iOldOffset = iOffset;
			if ((lpszNextLine[iLen-2] == '\r') && (lpszNextLine[iLen-1] == '\n'))
				m_lpLineInfo[m_iNumberLines-1].nLen = iLen - 2;
			else if (lpszNextLine[iLen-1] == '\n')
				m_lpLineInfo[m_iNumberLines-1].nLen = iLen - 1;
			else
				m_lpLineInfo[m_iNumberLines-1].nLen = iLen;

			// Calculate single line size
			SIZE szLine = GetLineSize(lpszNextLine, m_lpLineInfo[m_iNumberLines-1].nLen);
			if (iWidth < szLine.cx)
				iWidth = szLine.cx;
			iHeight += szLine.cy;
		}

		// Get scrollbar sizes
		int iHorizontalOffset = GetSystemMetrics(SM_CYHSCROLL);
		if (iHeight <= (rcClient.bottom - rcClient.top))
			iHorizontalOffset = 0;
		int iVerticalOffset = GetSystemMetrics(SM_CXVSCROLL);
		if (iWidth <= (rcClient.right - rcClient.left))
			iVerticalOffset = 0;

		// Check content width
		if (iWidth >= (rcClient.right - rcClient.left))
		{
			// Show horizontal scrollbar
			SCROLLINFO si;
			memset(&si, 0, sizeof(SCROLLINFO));
			si.cbSize = sizeof(SCROLLINFO);
			si.fMask = SIF_ALL;
			si.nMax = iWidth;
			si.nPage = ((rcClient.right - rcClient.left - iVerticalOffset) / m_szTextSize.cx + 1) * m_szTextSize.cx;
			si.nPos = GetScrollPos(m_hWnd,SB_HORZ);
			SetScrollInfo(m_hWnd,SB_HORZ, &si, TRUE);
			ShowScrollBar(m_hWnd,SB_HORZ, TRUE);
		}
		else
		{
			// Update horizontal scrollbar
			SCROLLINFO si;
			memset(&si, 0, sizeof(SCROLLINFO));
			si.cbSize = sizeof(SCROLLINFO);
			si.fMask = SIF_ALL;
			si.nMax = iWidth;
			si.nPage = ((rcClient.right - rcClient.left - iVerticalOffset) / m_szTextSize.cx + 1) * m_szTextSize.cx;
			SetScrollInfo(m_hWnd,SB_HORZ, &si, TRUE);
			SetScrollPos(m_hWnd,SB_HORZ, 0, TRUE);
			ShowScrollBar(m_hWnd,SB_HORZ, FALSE);
		}

		// Check content height
		if (iHeight >= (rcClient.bottom - rcClient.top))
		{
			// Show vertical scrollbar
			SCROLLINFO si;
			memset(&si, 0, sizeof(SCROLLINFO));
			si.cbSize = sizeof(SCROLLINFO);
			si.fMask = SIF_ALL;
			si.nMax = iHeight;
			si.nPage = ((rcClient.bottom - rcClient.top - iHorizontalOffset) / m_szTextSize.cy) * m_szTextSize.cy;
			si.nPos = GetScrollPos(m_hWnd,SB_VERT);
			SetScrollInfo(m_hWnd,SB_VERT, &si, TRUE);
			ShowScrollBar(m_hWnd,SB_VERT, TRUE);
		}
		else
		{
			// Update vertical scrollbar
			SCROLLINFO si;
			memset(&si, 0, sizeof(SCROLLINFO));
			si.cbSize = sizeof(SCROLLINFO);
			si.fMask = SIF_ALL;
			si.nMax = iHeight;
			si.nPage = ((rcClient.bottom - rcClient.top - iHorizontalOffset) / m_szTextSize.cy) * m_szTextSize.cy;
			SetScrollInfo(m_hWnd,SB_VERT, &si, TRUE);
			SetScrollPos(m_hWnd,SB_VERT, 0, TRUE);
			ShowScrollBar(m_hWnd,SB_VERT, FALSE);
		}

		// Set drawing flag
		//SetRedraw(TRUE);
		::SendMessage(m_hWnd, WM_SETREDRAW, TRUE, 0);
	}

	void xcSourceEdit::DeleteText()
	{
		// Check for valid data
		if (m_lpszText != NULL)
		{
			// Check for valid selection
			if ((m_iLineStartSelection != m_iLineEndSelection) || (m_iCharStartSelection != m_iCharEndSelection))
			{
				// Get selection range
				int iStartChar, iEndChar;
				int iStartLine, iEndLine;
				if (m_iLineStartSelection < m_iLineEndSelection)
				{
					iStartChar = m_iCharStartSelection;
					iEndChar = m_iCharEndSelection;
					iStartLine = m_iLineStartSelection;
					iEndLine = m_iLineEndSelection;
				}
				else if (m_iLineStartSelection > m_iLineEndSelection)
				{
					iStartChar = m_iCharEndSelection;
					iEndChar = m_iCharStartSelection;
					iStartLine = m_iLineEndSelection;
					iEndLine = m_iLineStartSelection;
				}
				else
				{
					iStartChar = min(m_iCharStartSelection, m_iCharEndSelection);
					iEndChar = max(m_iCharStartSelection, m_iCharEndSelection);
					iStartLine = m_iLineStartSelection;
					iEndLine = m_iLineEndSelection;
				}
				int iStartOffset = GetCharOffset(iStartLine, iStartChar);
				if (m_lpszText[iStartOffset] == '\r')
					iStartOffset += 2;
				int iEndOffset = GetCharOffset(iEndLine, iEndChar);
				int iCount = iEndOffset - iStartOffset;
				if (m_lpszText != NULL)
				{
					int iOldSize = m_iSize;
					LPTSTR lpszTemp = new _TCHAR[iOldSize];
					memcpy(lpszTemp, m_lpszText, iOldSize*sizeof(_TCHAR));
					m_iSize -= iCount;
					free(m_lpszText);
					m_lpszText = (LPTSTR)malloc(m_iSize*sizeof(_TCHAR));
					if (m_lpszText != NULL)
					{
						memcpy(m_lpszText, lpszTemp, iStartOffset*sizeof(_TCHAR));
						memcpy(m_lpszText+iStartOffset, lpszTemp+iEndOffset, (iOldSize-iEndOffset)*sizeof(_TCHAR));
					}
					m_iCurrentChar = iStartChar;
					m_iCurrentLine = iStartLine;

					// Clear selection
					m_iLineStartSelection = -1;
					m_iLineEndSelection = -1;
					m_iCharStartSelection = -1;
					m_iCharEndSelection = -1;
				}
			}
		}
	}

	SIZE xcSourceEdit::GetLineSize(LPTSTR lpszText, int iLen)
	{
		SIZE szResult = {0, m_szTextSize.cy};

		// Get line width
		BOOL bDone = FALSE;
		int iStartTAB=-1;
		int iEndTAB, iTABs;
		for (int i=0; i<iLen; i++)
		{
			// Check for TAB char
			if (lpszText[i] == '\t')
			{
				if (iStartTAB == -1)
				{
					iTABs = m_iTABs;
					iStartTAB = i;
				}
				else
				{
					iEndTAB = i;
					iTABs = m_iTABs - ((iEndTAB - iStartTAB) % m_iTABs);
					if (((iEndTAB - iStartTAB) % m_iTABs) != 0)
						iTABs++;
					if ((iEndTAB - iStartTAB) == 1)
						iTABs = m_iTABs;
					iStartTAB = iEndTAB;
				}
				szResult.cx += (iTABs * m_szTextSize.cx);
			}
			else
				szResult.cx += m_szTextSize.cx;
		}

		return szResult;
	}

	BOOL xcSourceEdit::Load(LPTSTR lpszFilePath)
	{
		BOOL bResult = FALSE;

		/*
		// Try to open the file on disk
		CFile file;
		if (file.Open(lpszFilePath, CFile::modeRead, NULL))
		{
			// Clear old data
			if (m_lpszText != NULL)
				free(m_lpszText);

			// Clear selection
			m_iLineStartSelection = -1;
			m_iLineEndSelection = -1;
			m_iCharStartSelection = -1;
			m_iCharEndSelection = -1;

			// Read data from file
			DWORD dwSize = (DWORD)file.GetLength();
			LPBYTE lpData = (LPBYTE)malloc(dwSize*sizeof(BYTE));
			file.Read(lpData, dwSize);
			file.Close();
			bResult = TRUE;

			// Check for file type
			WORD wFlag = 0;
			memcpy(&wFlag, lpData, 2);
			DWORD dwFlag = 0;
			memcpy(&dwFlag, lpData, 3);
			int iOffset;
			if (wFlag == 0xFEFF)
			{
				// Copy UNICODE data
				iOffset = 1;
				m_iSize = (dwSize-2) / sizeof(_TCHAR);
				m_lpszText = (LPTSTR)malloc(m_iSize*sizeof(_TCHAR));
				_tcscpy(m_lpszText, (LPTSTR)lpData+iOffset);
			}
			else if (dwFlag == 0xBFBBEF)
			{
				// Convert from UTF-8 to UNICODE data
				iOffset = 3;
				m_iSize = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)lpData+iOffset, dwSize, NULL, 0);
				m_lpszText = (LPTSTR)malloc(m_iSize*sizeof(_TCHAR));
				MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)lpData+iOffset, dwSize, m_lpszText, m_iSize);
			}
			else
			{
				// Convert from ANSI to UNICODE data
				iOffset = 0;
				m_iSize = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)lpData+iOffset, dwSize, NULL, 0);
				m_lpszText = (LPTSTR)malloc(m_iSize*sizeof(_TCHAR));
				MultiByteToWideChar(CP_ACP, 0, (LPCSTR)lpData+iOffset, dwSize, m_lpszText, m_iSize);
			}
			free(lpData);

			// Update text
			m_iSize += 2;
			m_lpszText = (LPTSTR)realloc(m_lpszText, m_iSize*sizeof(_TCHAR));
			m_lpszText[m_iSize-2] = '\r';
			m_lpszText[m_iSize-1] = '\n';

			// Update control
			UpdateControl();

			// Update screen
			Invalidate(FALSE);
			UpdateWindow();
		}
		*/

		return bResult;
	}

	BOOL xcSourceEdit::Save(LPTSTR lpszFilePath)
	{
		
		BOOL bResult = FALSE;
		/*
		// Try to open the file on disk
		CFile file;
		if (file.Open(lpszFilePath, CFile::modeCreate | CFile::modeWrite, NULL))
		{
			// Write data to file
			file.Write(m_lpszText, m_iSize*sizeof(_TCHAR));
			file.Close();
			bResult = TRUE;
		}
		*/
		return bResult;
	}

	void xcSourceEdit::AddText(LPTSTR lpszText, int iLen)
	{
		// Check for valid data
		if ((m_lpszText != NULL) && (lpszText != NULL))
		{
			// Add text
			int iOldSize = m_iSize;
			int iCurrentOffset = GetCharOffset(m_iCurrentLine, m_iCurrentChar);
			LPTSTR lpszTemp = new _TCHAR[iOldSize];
			memcpy(lpszTemp, m_lpszText, iOldSize*sizeof(_TCHAR));
			m_iSize += iLen;
			free(m_lpszText);
			m_lpszText = (LPTSTR)malloc(m_iSize*sizeof(_TCHAR));
			memcpy(m_lpszText, lpszTemp, iCurrentOffset*sizeof(_TCHAR));
			memcpy(m_lpszText+iCurrentOffset, lpszText, iLen*sizeof(_TCHAR));
			memcpy(m_lpszText+iCurrentOffset+iLen, lpszTemp+iCurrentOffset, (iOldSize-iCurrentOffset)*sizeof(_TCHAR));
			delete lpszTemp;
		}
	}

	void xcSourceEdit::DeleteText(int iStartLine, int iStartChar, int iEndLine, int iEndChar)
	{
		// Check for valid data
		if (m_lpszText != NULL)
		{
			// Format selection
			m_iLineStartSelection = max(0, iStartLine);
			m_iCharStartSelection = max(0, iStartChar);
			m_iLineEndSelection = min(m_iNumberLines-1, iEndLine);
			if (m_iLineEndSelection == -1)
				m_iLineEndSelection = m_iNumberLines - 1;
			m_iCharEndSelection = min(m_lpLineInfo[m_iLineEndSelection].nLen, iEndChar);
			if (m_iCharEndSelection == -1)
				m_iCharEndSelection = m_lpLineInfo[m_iLineEndSelection].nLen;

			// Delete selected text
			OnChar(VK_BACK, 0, 0);
		}
	}

	LPTSTR xcSourceEdit::GetTextBuffer()
	{
		// Get text buffer
		return m_lpszText;
	}

	int xcSourceEdit::GetBufferLen()
	{
		// Get text len
		return m_iSize;
	}

	void xcSourceEdit::GetTextFromLine(int iLineIndex, LPTSTR lpszText, int iLen)
	{
		// Check for valid data
		int iLine = max(0, min(m_iNumberLines-1, iLineIndex));
		if (lpszText != NULL)
		{
			// Get line text
			int iOffset = m_lpLineInfo[iLine].dwOffset;
			int iOldOffset = iOffset;
			LPTSTR lpszNextLine = NULL;
			if ((lpszNextLine=GetLine(iOffset)) != NULL)
			{
				// Calculate line length
				int iLen = iOffset - iOldOffset;
				if ((lpszNextLine[iLen-2] == '\r') && (lpszNextLine[iLen-1] == '\n'))
					iLen = iLen - 2;
				else if (lpszNextLine[iLen-1] == '\n')
					iLen = iLen - 1;

				// Copy line
				_tcsncpy_s(lpszText, iLen ,lpszNextLine, iLen);
				lpszText[iLen] = '\0';
			}
		}
	}

	int xcSourceEdit::LineFromPosition(POINT pt)
	{
		int iResult = -1;

		// Check for valid data
		if (m_lpszText != NULL)
		{
			// Get line index
			iResult = min(pt.y/m_szTextSize.cy, m_iNumberLines-1);
		}

		return iResult;
	}

	int xcSourceEdit::CharFromPosition(int iLineIndex, POINT pt)
	{
		int iResult = -1;

		// Check for valid data
		if (m_lpszText != NULL)
		{
			// Get char index
			int iOffset = m_lpLineInfo[iLineIndex].dwOffset;
			int iOldOffset = iOffset;
			int iCurrentLine = iLineIndex;
			LPTSTR lpszNextLine = NULL;
			while ((lpszNextLine=GetLine(iOffset)) != NULL)
			{
				// Calculate line length
				int iLen = iOffset - iOldOffset;
				iOldOffset = iOffset;

				// Check for valid line
				if (iCurrentLine == iLineIndex)
				{
					// Get char index
					BOOL bDone = FALSE;
					int iStartTAB=-1;
					int iEndTAB, iTABs;
					int iCharOffset = 0;
					for (int i=0; i<=m_lpLineInfo[iLineIndex].nLen; i++)
					{
						// Check for TAB char
						if (lpszNextLine[i] == '\t')
						{
							if (iStartTAB == -1)
							{
								iTABs = m_iTABs;
								iStartTAB = i;
							}
							else
							{
								iEndTAB = i;
								iTABs = m_iTABs - ((iEndTAB - iStartTAB) % m_iTABs);
								if (((iEndTAB - iStartTAB) % m_iTABs) != 0)
									iTABs++;
								if ((iEndTAB - iStartTAB) == 1)
									iTABs = m_iTABs;
								iStartTAB = iEndTAB;
							}
							iCharOffset += (iTABs * m_szTextSize.cx);
						}
						else
							iCharOffset += m_szTextSize.cx;
						if (iCharOffset >= pt.x)
						{
							bDone = TRUE;
							iResult = i;
							break;
						}
					}
					if (!bDone)
						iResult = m_lpLineInfo[iLineIndex].nLen;
				}
				iCurrentLine++;
				break;
			}
		}

		return iResult;
	}

	POINT xcSourceEdit::PositionFromChar(int iLineIndex, int iCharIndex)
	{
		POINT ptResult = {0, 0};

		// Check for valid data
		if (m_lpszText != NULL)
		{
			// Get char position
			int iOffset = m_lpLineInfo[iLineIndex].dwOffset;
			int iOldOffset = iOffset;
			int iCurrentLine = iLineIndex;
			LPTSTR lpszNextLine = NULL;
			while ((lpszNextLine=GetLine(iOffset)) != NULL)
			{
				// Calculate line length
				int iLen = iOffset - iOldOffset;
				iOldOffset = iOffset;

				// Check for valid line
				if (iCurrentLine == iLineIndex)
				{
					// Get char index
					BOOL bDone = FALSE;
					int iStartTAB=-1;
					int iEndTAB, iTABs;
					int iCharOffset = 0;
					for (int i=0; i<=m_lpLineInfo[iLineIndex].nLen; i++)
					{
						// Check for char found
						if (i == iCharIndex)
						{
							bDone = TRUE;
							ptResult.y = iLineIndex * m_szTextSize.cy;
							ptResult.x = iCharOffset;
							break;
						}
						else
						{
							// Check for TAB char
							if (lpszNextLine[i] == '\t')
							{
								if (iStartTAB == -1)
								{
									iTABs = m_iTABs;
									iStartTAB = i;
								}
								else
								{
									iEndTAB = i;
									iTABs = m_iTABs - ((iEndTAB - iStartTAB) % m_iTABs);
									if (((iEndTAB - iStartTAB) % m_iTABs) != 0)
										iTABs++;
									if ((iEndTAB - iStartTAB) == 1)
										iTABs = m_iTABs;
									iStartTAB = iEndTAB;
								}
								iCharOffset += (iTABs * m_szTextSize.cx);
							}
							else
								iCharOffset += m_szTextSize.cx;
						}
					}
					if (!bDone)
					{
						ptResult.y = iLineIndex * m_szTextSize.cy;
						ptResult.x = iCharOffset - m_szTextSize.cx;
					}
				}
				iCurrentLine++;
				break;
			}
		}

		return ptResult;
	}

	int xcSourceEdit::GetCharOffset(int iLineIndex, int iCharIndex)
	{
		int iResult = 0;

		// Check for valid data
		if (m_lpszText != NULL)
		{
			// Get char offset
			iResult = m_lpLineInfo[iLineIndex].dwOffset + iCharIndex;
		}

		return iResult;
	}

	void xcSourceEdit::UpdateCaret()
	{
		// Ensure caret visible
		EnsureVisible(m_iCurrentLine, m_iCurrentChar);

		// Get scroll info
		int iVerticalOffset = GetScrollPos(m_hWnd,SB_VERT);
		int iHorizontalOffset = GetScrollPos(m_hWnd,SB_HORZ);

		// Set char position
		POINT pt = PositionFromChar(m_iCurrentLine, m_iCurrentChar);
		pt.x -= iHorizontalOffset;
		pt.y -= iVerticalOffset;
		SetCaretPos(pt.x,pt.y);
	}

	void xcSourceEdit::EnsureVisible(int iLineIndex, int iCharIndex)
	{
		// Check for valid data
		if (m_lpszText != NULL)
		{
			// Check for valid text size
			if (m_iSize == 2)
			{
				// Update screen
				Invalidate(FALSE);
				UpdateWindow();
				return;
			}

			// Clear drawing flag
			//SetRedraw(FALSE);
			::SendMessage(m_hWnd, WM_SETREDRAW, FALSE, 0);

			// Get client rectangle
			RECT rcClient;
			GetClientRect(&rcClient);

			// Get scroll info
			int iVerticalOffset = GetScrollPos(m_hWnd,SB_VERT);
			int iHorizontalOffset = GetScrollPos(m_hWnd,SB_HORZ);

			// Get visible lines range
			POINT ptTop = {0, rcClient.top+iVerticalOffset};
			POINT ptBottom = {0, (rcClient.bottom-rcClient.top)+iVerticalOffset};
			int iLineStart = LineFromPosition(ptTop);
			int iLineEnd = LineFromPosition(ptBottom);
			RECT rcLine = {-iHorizontalOffset, 0, rcClient.right, m_szTextSize.cy};
			POINT ptLeft = {iHorizontalOffset, 0};
			POINT ptRight = {rcLine.right+iHorizontalOffset, 0};
			int iCharStart = CharFromPosition(iLineIndex, ptLeft);
			int iCharEnd = CharFromPosition(iLineIndex, ptRight);

			// Get current caret position
			POINT ptCurrent = PositionFromChar(iLineIndex, iCharIndex);

			// Ensure caret is visible
			if (iLineIndex >= iLineEnd)
			{
				// Scroll down
				int iNumberScrolls = iLineIndex - iLineEnd;
				for (int i=0; i<=iNumberScrolls; i++)
					OnVScroll(SB_LINEDOWN, 0, NULL);
			}
			else if (iLineIndex < iLineStart)
			{
				// Scroll up
				int iNumberScrolls = iLineStart - iLineIndex;
				for (int i=0; i<iNumberScrolls; i++)
					OnVScroll(SB_LINEUP, 0, NULL);
			}
			if ((iCharIndex > iCharEnd) && (iLineIndex <= m_iNumberLines-1))
			{
				// Scroll right
				POINT ptEnd = PositionFromChar(iLineIndex, iCharEnd);
				int iNumberScrolls = (ptCurrent.x - ptEnd.x) / m_szTextSize.cx;
				for (int i=0; i<iNumberScrolls; i++)
					OnHScroll(SB_LINERIGHT, 0, NULL);
			}
			else if (iCharIndex <= iCharStart)
			{
				// Scroll left
				int iNumberScrolls = (iHorizontalOffset - ptCurrent.x) / m_szTextSize.cx;
				for (int i=0; i<=iNumberScrolls; i++)
					OnHScroll(SB_LINELEFT, 0, NULL);
			}

			// Set drawing flag
			//SetRedraw(TRUE);
			::SendMessage(m_hWnd, WM_SETREDRAW, TRUE, 0);
		}

		// Update screen
		Invalidate(FALSE);
		UpdateWindow();
	}

	LPTSTR xcSourceEdit::GetLine(int& iOffset)
	{
		LPTSTR lpszResult = NULL;

		// Check for valid offset
		if (iOffset < m_iSize)
		{
			// Get next line
			lpszResult = m_lpszText + iOffset;
			LPTSTR lpszNextLine = _tcsstr(lpszResult, _T("\r\n"));
			if (lpszNextLine != NULL)
			{
				// Update offset
				iOffset = (int)(lpszNextLine - m_lpszText + 2);
			}
			else
			{
				// Update offset
				iOffset = m_iSize;
			}
		}

		return lpszResult;
	}

	void xcSourceEdit::SelectWord(POINT pt)
	{
		// Get scroll info
		int iVerticalOffset = GetScrollPos(m_hWnd,SB_VERT);
		int iHorizontalOffset = GetScrollPos(m_hWnd,SB_HORZ);

		// Get char
		int iSelectedLine = LineFromPosition(pt);
		int iSelectedChar = CharFromPosition(iSelectedLine, pt);

		// Check for word double-clicked
		int iOffset = m_lpLineInfo[iSelectedLine].dwOffset;
		int iOldOffset = iOffset;
		int iCurrentLine = iSelectedLine;
		LPTSTR lpszNextLine = NULL;
		while ((lpszNextLine=GetLine(iOffset)) != NULL)
		{
			// Calculate line length
			int iLen = iOffset - iOldOffset;
			iOldOffset = iOffset;

			// Check for valid line
			if (iCurrentLine == iSelectedLine)
			{
				// Check for word selected
				if (!IsSpecial(lpszNextLine[iSelectedChar]))
				{
					// Select word
					int iStart = iSelectedChar;
					while ((IsLetter(lpszNextLine[iStart])) && (iStart > 0))
						iStart--;
					if ((iStart >= 0) && !IsLetter(lpszNextLine[iStart]))
						iStart++;
					int iEnd = iSelectedChar;
					while ((IsLetter(lpszNextLine[iEnd])) && (iEnd < iLen))
						iEnd++;

					// Set selection
					m_iLineStartSelection = iSelectedLine;
					m_iLineEndSelection = iSelectedLine;
					m_iCharStartSelection = iStart;
					m_iCharEndSelection = iEnd;

					// Set caret position
					m_iCurrentLine = iSelectedLine;
					m_iCurrentChar = iEnd;
					POINT pt = PositionFromChar(m_iCurrentLine, m_iCurrentChar);
					pt.x -= iHorizontalOffset;
					pt.y -= iVerticalOffset;
					SetCaretPos(pt.x,pt.y);
				}
				else
				{
					// Clear selection
					m_iLineStartSelection = -1;
					m_iLineEndSelection = -1;
					m_iCharStartSelection = -1;
					m_iCharEndSelection = -1;
				}
				break;
			}
			iCurrentLine++;
		}
	}

	BOOL xcSourceEdit::IsSpecial(_TCHAR tch)
	{
		BOOL bResult = FALSE;

		// Check for special char (SPACE, TAB, CR, LF)
		if ((tch == ' ') || (tch == '\t') || (tch == '\r') || (tch == '\n'))
		{
			// Exit
			bResult = TRUE;
		}

		return bResult;
	}

	BOOL xcSourceEdit::IsLetter(_TCHAR tch)
	{
		BOOL bResult = FALSE;

		// Check for letter ('A' to 'Z', 'a' to 'z', '0' to '9')
		if (((tch >= 'A') && (tch <= 'Z')) || ((tch >= 'a') && (tch <= 'z')) || ((tch >= '0') && (tch <= '9')) || (tch == '_') || (tch == '#'))
		{
			// Exit
			bResult = TRUE;
		}

		return bResult;
	}

	BOOL xcSourceEdit::IsKeyword(LPTSTR lpszText, int iLen, int& iKeywordIndex, int& iKeywordOffset)
	{
		BOOL bResult = FALSE;

		// Check for valid length
		if (iLen == 0)
			return FALSE;

		// Format text
		LPTSTR lpszKeywordText = new _TCHAR[iLen+1];
		_tcsncpy_s(lpszKeywordText,iLen + 1, lpszText, iLen);
		lpszKeywordText[iLen] = '\0';
		if (!m_bKeywordsCaseSensitive)
			_tcsupr_s(lpszKeywordText,iLen+1);

		// Check for keyword
		for (int i=0; i<m_iNumberKeywords; i++)
		{
			// Check for keyword
			LPTSTR lpszSearch = _tcsstr(lpszKeywordText, m_lpKeywordInfo[i].lpszTagName);
			if (lpszSearch != NULL)
			{
				// Check for valid keyword
				int iFirst = (int)(lpszSearch - lpszKeywordText - 1);
				int iLast = iFirst + m_lpKeywordInfo[i].nLen;
				bResult = TRUE;
				if (iFirst >= 0)
				{
					if (m_SyntaxColoring != SCT_HTML)
					{
						if (((lpszKeywordText[iFirst] >= 'A') && (lpszKeywordText[iFirst] <= 'Z')) || 
							((lpszKeywordText[iFirst] >= 'a') && (lpszKeywordText[iFirst] <= 'z')))
							bResult = FALSE;
					}

				}
				if (bResult)
				{
					if (((lpszKeywordText[iLast] >= 'A') && (lpszKeywordText[iLast] <= 'Z')) || 
						((lpszKeywordText[iLast] >= 'a') && (lpszKeywordText[iLast] <= 'z')))
						bResult = FALSE;
					else
					{
						// Exit
						iKeywordIndex = i;
						iKeywordOffset = iFirst + 1;
						break;
					}
				}
			}
		}
		delete lpszKeywordText;

		return bResult;
	}

	BOOL xcSourceEdit::IsConstant(LPTSTR lpszText, int iLen, int& iConstantIndex, int& iConstantOffset)
	{
		BOOL bResult = FALSE;

		// Format text
		LPTSTR lpszConstantText = new _TCHAR[iLen+1];
		_tcsncpy_s(lpszConstantText,iLen+1, lpszText, iLen);
		lpszConstantText[iLen] = '\0';
		if (!m_bConstantsCaseSensitive)
			_tcsupr_s(lpszConstantText,iLen+1);

		// Check for constant
		for (int i=0; i<m_iNumberConstants; i++)
		{
			// Check for constant
			LPTSTR lpszSearch = _tcsstr(lpszConstantText, m_lpConstantInfo[i].lpszTagName);
			if (lpszSearch != NULL)
			{
				// Check for valid constant
				int iFirst = (int)(lpszSearch - lpszConstantText - 1);
				int iLast = iFirst + m_lpConstantInfo[i].nLen;
				bResult = TRUE;
				if (iFirst >= 0)
				{
					if (((lpszConstantText[iFirst] >= 'A') && (lpszConstantText[iFirst] <= 'Z')) || 
						((lpszConstantText[iFirst] >= 'a') && (lpszConstantText[iFirst] <= 'z')))
						bResult = FALSE;

				}
				if (bResult)
				{
					if (((lpszConstantText[iLast] >= 'A') && (lpszConstantText[iLast] <= 'Z')) || 
						((lpszConstantText[iLast] >= 'a') && (lpszConstantText[iLast] <= 'z')))
						bResult = FALSE;
					else
					{
						// Exit
						iConstantIndex = i;
						iConstantOffset = iFirst + 1;
						break;
					}
				}
			}
		}
		delete lpszConstantText;

		return bResult;
	}

	BOOL xcSourceEdit::IsText(LPTSTR lpszText, int iLen, int& iTextStart, int& iTextEnd)
	{
		BOOL bResult = FALSE;

		// Check for text
		iTextStart = -1;
		iTextEnd = -1;
		for (int i=0; i<iLen; i++)
		{
			// Check for text start index
			if ((iTextStart == -1) && ((lpszText[i] == '\"') || (lpszText[i] == '\'')))
			{
				iTextStart = i;
				bResult = TRUE;
			}
			else
			{
				// Check for text end index
				if ((iTextStart != -1) && ((lpszText[i] == '\"') || (lpszText[i] == '\'')))
					iTextEnd = i;
			}
		}

		return bResult;
	}

	void xcSourceEdit::ShowAutoComplete()
	{
		// Get scroll info
		int iVerticalOffset = GetScrollPos(m_hWnd,SB_VERT);
		int iHorizontalOffset = GetScrollPos(m_hWnd,SB_HORZ);

		// Create listbox
		POINT pt = PositionFromChar(m_iCurrentLine, m_iCurrentChar);
		SelectWord(pt);
		pt.x -= iHorizontalOffset;
		pt.y -= iVerticalOffset;
		RECT rect = {pt.x, pt.y+m_szTextSize.cy, pt.x+100, pt.y+m_szTextSize.cy+200};

		HINSTANCE hInst = (HINSTANCE)GetWindowLongPtr(m_hWnd,GWL_HINSTANCE);
		m_hListBox = ::CreateWindow(_T("listbox"),NULL,WS_CHILD | WS_VISIBLE | WS_HSCROLL | LBS_STANDARD | LBS_WANTKEYBOARDINPUT,
			rect.left,rect.top,rect.right - rect.left,rect.bottom - rect.top,m_hWnd,(HMENU)0x10001,hInst,NULL);
		::SendMessage(m_hListBox,WM_SETFONT,(WPARAM)m_hFont,TRUE);
		//m_pListBox.Create(WS_CHILD | WS_VISIBLE | WS_HSCROLL | LBS_STANDARD | LBS_WANTKEYBOARDINPUT, rect, this, 0x10001);
		//m_pListBox.SetFont(&m_hFont, TRUE);

		// Check for keyword or constant
		int iEndOffset = GetCharOffset(m_iCurrentLine, m_iCurrentChar);
		if (!IsLetter(m_lpszText[iEndOffset]))
			iEndOffset--;
		int iStartOffset = iEndOffset;
		while ((iStartOffset > 0) && (IsLetter(m_lpszText[iStartOffset])))
			iStartOffset--;
		if (iStartOffset > 0)
			iStartOffset++;
		int iLen = iEndOffset - iStartOffset + 1;
		if (iLen > 0)
		{
			LPTSTR lpszSearch = new _TCHAR[iLen+1];
			_tcsncpy_s(lpszSearch,iLen+1, m_lpszText+iStartOffset, iLen);
			lpszSearch[iLen] = '\0';
			for (int i=0; i<m_iNumberKeywords; i++)
			{
				// Check for valid keyword
				if (_tcsncmp(m_lpKeywordInfo[i].lpszTagName, lpszSearch, iLen) == 0)
				{
					// Add keyword
					::SendMessage(m_hListBox,LB_ADDSTRING,0,(LPARAM)m_lpKeywordInfo[i].lpszTagName);
					//m_pListBox.AddString(m_lpKeywordInfo[i].lpszTagName);
				}
			}
			for (int j=0; j<m_iNumberConstants; j++)
			{
				// Check for valid constant
				if (_tcsncmp(m_lpConstantInfo[j].lpszTagName, lpszSearch, iLen) == 0)
				{
					// Add constant
					//m_pListBox.AddString(m_lpConstantInfo[j].lpszTagName);
					::SendMessage(m_hListBox,LB_ADDSTRING,0,(LPARAM)m_lpConstantInfo[j].lpszTagName);
				}
			}
			delete lpszSearch;
		}

		// Show listbox
		if (::SendMessage(m_hListBox,LB_GETCOUNT,0,0)>0)
		{
			::SetFocus(m_hListBox);
			::SendMessage(m_hListBox,CB_SETCURSEL,0,0);
			//m_pListBox.SetFocus();
			//m_pListBox.SetCurSel(0);
		}
		else
		{
			::DestroyWindow(m_hListBox);
		}
	}

	void xcSourceEdit::SetKeywordColor(COLORREF cKeywordColor)
	{
		// Set keyword color
		m_cKeywordColor = cKeywordColor;
	}

	COLORREF xcSourceEdit::GetKeywordColor()
	{
		// Get keyword color
		return m_cKeywordColor;
	}

	void xcSourceEdit::SetKeywords(LPTSTR lpszKeywords, int iLen, BOOL bCaseSensitive)
	{
		// Clear keywords
		for (int i=0; i<m_iNumberKeywords; i++)
			delete m_lpKeywordInfo[i].lpszTagName;
		free(m_lpKeywordInfo);
		m_lpKeywordInfo = NULL;

		// Set case sensitive flag
		m_bKeywordsCaseSensitive = bCaseSensitive;

		// Extract keywords
		m_iNumberKeywords = 0;
		int iOffset = 0;
		int iOldOffset = iOffset;
		int iCount;
		LPTSTR lpszNextKeyword = NULL;
		while ((lpszNextKeyword=_tcsstr(lpszKeywords+iOffset, _T(";"))) != NULL)
		{
			// Extract keyword
			iCount = (int)(lpszNextKeyword - (lpszKeywords+iOffset));
			iOffset = (int)(lpszNextKeyword - lpszKeywords);
			m_iNumberKeywords++;
			if (m_lpKeywordInfo == NULL)
				m_lpKeywordInfo = (LPTAGINFO)malloc(m_iNumberKeywords*sizeof(TAGINFO));
			else
				m_lpKeywordInfo = (LPTAGINFO)realloc(m_lpKeywordInfo, m_iNumberKeywords*sizeof(TAGINFO));
			m_lpKeywordInfo[m_iNumberKeywords-1].nLen = iCount + 1;
			m_lpKeywordInfo[m_iNumberKeywords-1].lpszTagName = new _TCHAR[m_lpKeywordInfo[m_iNumberKeywords-1].nLen];
			_tcsncpy_s(m_lpKeywordInfo[m_iNumberKeywords-1].lpszTagName,iCount + 1, lpszKeywords+iOldOffset, m_lpKeywordInfo[m_iNumberKeywords-1].nLen-1);
			m_lpKeywordInfo[m_iNumberKeywords-1].lpszTagName[m_lpKeywordInfo[m_iNumberKeywords-1].nLen-1] = '\0';
			if (!m_bKeywordsCaseSensitive)
				_tcsupr_s(m_lpKeywordInfo[m_iNumberKeywords-1].lpszTagName,iCount + 1);
			iOffset++;
			iOldOffset = iOffset;
		}
	}

	void xcSourceEdit::SetConstantColor(COLORREF cConstantColor)
	{
		// Set constant color
		m_cConstantColor = cConstantColor;
	}

	COLORREF xcSourceEdit::GetConstantColor()
	{
		// Get constant color
		return m_cConstantColor;
	}

	void xcSourceEdit::SetConstants(LPTSTR lpszConstants, int iLen, BOOL bCaseSensitive)
	{
		// Clear constants
		for (int i=0; i<m_iNumberConstants; i++)
			delete m_lpConstantInfo[i].lpszTagName;
		free(m_lpConstantInfo);
		m_lpConstantInfo = NULL;

		// Set case sensitive flag
		m_bConstantsCaseSensitive = bCaseSensitive;

		// Extract constants
		m_iNumberConstants = 0;
		int iOffset = 0;
		int iOldOffset = iOffset;
		int iCount;
		LPTSTR lpszNextConstant = NULL;
		while ((lpszNextConstant=_tcsstr(lpszConstants+iOffset, _T(";"))) != NULL)
		{
			// Extract constant
			iCount = (int)(lpszNextConstant - (lpszConstants+iOffset));
			iOffset = (int)(lpszNextConstant - lpszConstants);
			m_iNumberConstants++;
			if (m_lpConstantInfo == NULL)
				m_lpConstantInfo = (LPTAGINFO)malloc(m_iNumberConstants*sizeof(TAGINFO));
			else
				m_lpConstantInfo = (LPTAGINFO)realloc(m_lpConstantInfo, m_iNumberConstants*sizeof(TAGINFO));
			m_lpConstantInfo[m_iNumberConstants-1].nLen = iCount + 1;
			m_lpConstantInfo[m_iNumberConstants-1].lpszTagName = new _TCHAR[m_lpConstantInfo[m_iNumberConstants-1].nLen];
			_tcsncpy_s(m_lpConstantInfo[m_iNumberConstants-1].lpszTagName, iCount + 1,lpszConstants+iOldOffset, m_lpConstantInfo[m_iNumberConstants-1].nLen-1);
			m_lpConstantInfo[m_iNumberConstants-1].lpszTagName[m_lpConstantInfo[m_iNumberConstants-1].nLen-1] = '\0';
			if (!m_bConstantsCaseSensitive)
				_tcsupr_s(m_lpConstantInfo[m_iNumberConstants-1].lpszTagName,iCount + 1);
			iOffset++;
			iOldOffset = iOffset;
		}
	}

	void xcSourceEdit::SetCommentColor(COLORREF cCommentColor)
	{
		// Set comment color
		m_cCommentColor = cCommentColor;
	}

	COLORREF xcSourceEdit::GetCommentColor()
	{
		// Get comment color
		return m_cCommentColor;
	}

	void xcSourceEdit::SetTextColor(COLORREF cTextColor)
	{
		// Set text color
		m_cTextColor = cTextColor;
	}

	COLORREF xcSourceEdit::GetTextColor()
	{
		// Get text color
		return m_cTextColor;
	}

	void xcSourceEdit::SetSyntaxColoring(SYNTAX_COLORING_TYPE syntaxColoring)
	{
		// Set syntax coloring
		m_SyntaxColoring = syntaxColoring;

		// Update syntax coloring
		switch (m_SyntaxColoring)
		{
		case SCT_C_ANSI:
			{
				// Set ANSI-C syntax coloring
				std::wstring strKeywords = _T("auto;__asm;__based;break;case;__cdecl;char;const;continue;__declspec;default;");
				strKeywords += _T("dllexport;dllimport;do;double;__except;else;enum;extern;__fastcall;__finally;float;for;");
				strKeywords += _T("goto;__inline;__int8;__int16;__int32;__int64;if;int;__leave;long;naked;register;return;");
				strKeywords += _T("__stdcall;short;signed;sizeof;static;struct;switch;__try;thread;typedef;union;unsigned;");
				strKeywords += _T("void;volatile;while;#define;#elif;#else;#endif;#error;#if;#ifdef;#ifndef;#import;#include;");
				strKeywords += _T("#line;#pragma;#undef;#using;once;pack;");
				SetKeywordColor(RGB(0,0,255));
				SetKeywords((LPTSTR)((LPCTSTR)strKeywords.c_str()), (int)strKeywords.length(), TRUE);
				SetCommentColor(RGB(0,128,0));
				SetTextColor(RGB(163,21,21));
			}
			break;

		case SCT_C_PLUS_PLUS:
			{
				// Set C++ syntax coloring
				std::wstring strKeywords = _T("__abstract;__alignof;__asm;__assume;__based;__box;__cdecl;__declspec;__delegate;");
				strKeywords += _T("__event;__except;__fastcall;__finally;__forceinline;__gc;__hook;__identifier;__if_exists;");
				strKeywords += _T("__if_not_exists;__inline;__int8;__int16;__int32;__int64;__interface;__leave;__m64;__m128;");
				strKeywords += _T("__m128d;__m128i;__multiple_inheritance;__nogc;__noop;__pin;__property;__raise;__sealed;");
				strKeywords += _T("__single_inheritance;__stdcall;__super;__try_cast;__try;__except;__finally;__unhook;__uuidof;");
				strKeywords += _T("__value;__virtual_inheritance;__w64;bool;break;case;catch;char;class;const;const_cast;continue;");
				strKeywords += _T("default;delete;deprecated;dllexport;dllimport;do;double;dynamic_cast;else;enum;explicit;extern;");
				strKeywords += _T("false;float;for;friend;goto;if;inline;int;long;mutable;naked;namespace;new;noinline;noreturn;");
				strKeywords += _T("nothrow;novtable;operator;private;property;protected;public;register;reinterpret_cast;return;");
				strKeywords += _T("selectany;short;signed;sizeof;static;static_cast;struct;switch;template;this;thread;throw;true;");
				strKeywords += _T("try;typedef;typeid;typename;union;unsigned;using;uuid;virtual;void;volatile;__wchar_t;wchar_t;while;");
				strKeywords += _T("#define;#elif;#else;#endif;#error;#if;#ifdef;#ifndef;#import;#include;");
				strKeywords += _T("#line;#pragma;#undef;#using;auto_rename;auto_search;embedded_idl;exclude;once;pack;");
				SetKeywordColor(RGB(0,0,255));
				SetKeywords((LPTSTR)((LPCTSTR)strKeywords.c_str()), (int)strKeywords.length(), TRUE);
				std::wstring strConstants = _T("TRUE;FALSE;NULL;");
				SetConstantColor(RGB(128,128,128));
				SetConstants((LPTSTR)((LPCTSTR)strConstants.c_str()), (int)strConstants.length(), TRUE);
				SetCommentColor(RGB(0,128,0));
				SetTextColor(RGB(163,21,21));
			}
			break;

		case SCT_HTML:
			{
				// Set HTML syntax coloring
				std::wstring strKeywords = _T("<a>;</a>;<a;<acronym>;</acronym>;<acronym;<address>;</address>;<address;<applet>;");
				strKeywords += _T("</applet>;<applet;<area>;</area>;<area;<b>;</b>;<b;<base>;</base>;<base;<baseFont>;</baseFont>;");
				strKeywords += _T("<baseFont;<bdo>;</bdo>;<bdo;<bgSound>;</bgSound>;<bgSound;<big>;</big>;<big;<blockQuote>;");
				strKeywords += _T("</blockQuote>;<blockQuote;<body>;</body>;<body;<br>;</br>;<br;<button>;</button>;<button;");
				strKeywords += _T("<caption>;</caption>;<caption;<center>;</center>;<center;<cite>;</cite>;<cite;<code>;</code>;<code;");
				strKeywords += _T("<col>;</col>;<col;<colGroup>;</colGroup>;<colGroup;<comment>;</comment>;<comment;<custom>;</custom>;<custom;");
				strKeywords += _T("<dd>;</dd>;<dd;<del>;</del>;<del;<dfn>;</dfn>;<dfn;<dir>;</dir>;<dir;<div>;</div>;<div;<dl>;</dl>;<dl;");
				strKeywords += _T("<dt>;</dt>;<dt;<em>;</em>;<em;<embed>;</embed>;<embed;<fieldSet>;</fieldSet>;<fieldSet;<font>;</font>;<font;");
				strKeywords += _T("<form>;</form>;<form;<frame>;</frame>;<frame;<frameSet>;</frameSet>;<frameSet;<head>;</head>;<head;");
				strKeywords += _T("<hn>;</hn>;<hn;<hr>;</hr>;<hr;<html>;</html>;<html;<i>;</i>;<i;<iframe>;</iframe>;<iframe;<img>;</img>;<img;");
				strKeywords += _T("<input>;</input>;<input;<ins>;</ins>;<ins;<isIndex>;</isIndex>;<isIndex;<kbd>;</kbd>;<kbd;<label>;</label>;<label;");
				strKeywords += _T("<legend>;</legend>;<legend;<li>;</li>;<li;<link>;</link>;<link;<listing>;</listing>;<listing;<map>;</map>;<map;");
				strKeywords += _T("<marquee>;</marquee>;<marquee;<menu>;</menu>;<menu;<meta>;</meta>;<meta;<noBR>;</noBR>;<noBR;");
				strKeywords += _T("<noFrames>;</noFrames>;<noFrames;<noScript>;</noScript>;<noScript;<object>;</object>;<object;<ol>;</ol>;<ol;");
				strKeywords += _T("<optGroup>;</optGroup>;<optGroup;<option>;</option>;<option;<p>;</p>;<p;<param>;</param>;<param;");
				strKeywords += _T("<plainText>;</plainText>;<plainText;<pre>;</pre>;<pre;<q>;</q>;<q;<rt>;</rt>;<rt;<ruby>;</ruby>;<ruby;");
				strKeywords += _T("<s>;</s>;<s;<samp>;</samp>;<samp;<script>;</script>;<script;<select>;</select>;<select;<small>;</small>;<small;");
				strKeywords += _T("<span>;</span>;<span;<strike>;</strike>;<strike;<strong>;</strong>;<strong;<style>;</style>;<style;");
				strKeywords += _T("<sub>;</sub>;<sub;<sup>;</sup>;<sup;<table>;</table>;<table;<tBody>;</tBody>;<tBody;<td>;</td>;<td;");
				strKeywords += _T("<textArea>;</textArea>;<textArea;<tFoot>;</tFoot>;<tFoot;<th>;</th>;<th;<tHead>;</tHead>;<tHead;");
				strKeywords += _T("<title>;</title>;<title;<tr>;</tr>;<tr;<tt>;</tt>;<tt;<u>;</u>;<u;<ul>;</ul>;<ul;<var>;</var>;<var;");
				strKeywords += _T("<wbr>;</wbr>;<wbr;<xml>;</xml>;<xml;<xmp>;</xmp>;<xmp;<h1>;</h1>;<h1;<h2>;</h2>;<h2;<h3>;</h3>;<h3;");
				strKeywords += _T("<h4>;</h4>;<h4;<h5>;</h5>;<h5;<h6></h6>;<h6;<;/>;>;");
				SetKeywordColor(RGB(0,0,255));
				SetKeywords((LPTSTR)((LPCTSTR)strKeywords.c_str()), (int)strKeywords.length(), FALSE);
				SetCommentColor(RGB(0,128,0));
				SetTextColor(RGB(163,21,21));
			}
			break;

		case SCT_JSCRIPT:
			{
				// Set JScript syntax coloring
				std::wstring strKeywords = _T("break;catch;@cc_on;continue;debugger;do;else;@elif;@else;@end;for;function;in;@if;if;");
				strKeywords += _T("return;switch;@set;this;throw;try;var;while;with;");
				SetKeywordColor(RGB(0,0,255));
				SetKeywords((LPTSTR)((LPCTSTR)strKeywords.c_str()), (int)strKeywords.length(), FALSE);
				std::wstring strConstants = _T("true;false;null;");
				SetConstantColor(RGB(128,128,128));
				SetConstants((LPTSTR)((LPCTSTR)strConstants.c_str()), (int)strConstants.length(), FALSE);
				SetCommentColor(RGB(0,128,0));
				SetTextColor(RGB(163,21,21));
			}
			break;

		case SCT_SQL:
			{
				// Set SQL syntax coloring
				std::wstring strKeywords = _T("@@IDENTITY;ENCRYPTION;ORDER;ADD;END;ALL;ERRLVL;OVER;ALTER;ESCAPE;PERCENT;");
				strKeywords += _T("AND;EXCEPT;PLAN;ANY;EXEC;PRECISION;AS;EXECUTE;PRIMARY;EXISTS;PRINT;AUTHORIZATION;");
				strKeywords += _T("EXIT;PROC;EXPRESSION;PROCEDURE;BACKUP;FETCH;PUBLIC;BEGIN;FILE;RAISERROR;BETWEEN;");
				strKeywords += _T("FILLFACTOR;READ;ASC;BREAK;FOR;READTEXT;BROWSE;FOREIGN;RECONFIGURE;BULK;FREETEXT;");
				strKeywords += _T("REFERENCES;BY;FREETEXTTABLE;REPLICATION;CASCADE;FROM;RESTORE;CASE;RESTRICT;CHECK;");
				strKeywords += _T("FUNCTION;RETURN;CHECKPOINT;GOTO;REVOKE;CLOSE;GRANT;CLUSTERED;GROUP;ROLLBACK;");
				strKeywords += _T("COALESCE;HAVING;ROWCOUNT;COLLATE;HOLDLOCK;ROWGUIDCOL;COLUMN;IDENTITY;RULE;COMMIT;IDENTITY_INSERT;");
				strKeywords += _T("SAVE;COMPUTE;IDENTITYCOL;SCHEMA;CONSTRAINT;IF;SELECT;CONTAINS;IN;SESSION_USER;CONTAINSTABLE;");
				strKeywords += _T("INDEX;SET;CONTINUE;SETUSER;INSERT;SHUTDOWN;INTERSECT;SOME;CREATE;INTO;");
				strKeywords += _T("STATISTICS;IS;CURRENT;JOIN;SYSTEM_USER;CURRENT_DATE;KEY;TABLE;CURRENT_TIME;KILL;");
				strKeywords += _T("TEXTSIZE;CURRENT_TIMESTAMP;THEN;CURRENT_USER;LIKE;TO;CURSOR;LINENO;TOP;DATABASE;LOAD;");
				strKeywords += _T("TRAN;DATABASEPASSWORD;TRANSACTION;DATEADD;TRIGGER;DATEDIFF;NATIONAL;TRUNCATE;DATENAME;");
				strKeywords += _T("NOCHECK;TSEQUAL;DATEPART;NONCLUSTERED;UNION;DBCC;NOT;UNIQUE;DEALLOCATE;UPDATE;DECLARE;NULLIF;");
				strKeywords += _T("UPDATETEXT;DEFAULT;OF;USE;DELETE;OFF;USER;DENY;OFFSETS;VALUES;DESC;ON;VARYING;DISK;OPEN;VIEW;");
				strKeywords += _T("DISTINCT;OPENDATASOURCE;WAITFOR;DISTRIBUTED;OPENQUERY;WHEN;DOUBLE;OPENROWSET;WHERE;DROP;OPENXML;");
				strKeywords += _T("WHILE;DUMP;OPTION;WITH;ELSE;OR;WRITETEXT;");
				SetKeywordColor(RGB(0,0,255));
				SetKeywords((LPTSTR)((LPCTSTR)strKeywords.c_str()),(int) strKeywords.length(), FALSE);
				std::wstring strConstants = _T("INNER;OUTER;CROSS;FULL;LEFT;RIGHT;TRUE;FALSE;NULL;COLUMNS;INDEXES;KEY_COLUMN_USAGE;DECIMAL;");
				strConstants += _T("PROVIDER_TYPES;TABLES;TABLE_CONSTRAINTS;BIGINT;INTEGER;SMALLINT;TINYINT;BIT;NUMERIC;MONEY;CHAR;VARCHAR;");
				strConstants += _T("FLOAT;REAL;DATETIME;NCHAR;NVARCHAR;NTEXT;BINARY;VARBINARY;IMAGE;UNIQUEIDENTIFIER;IDENTITY;ROWGUIDCOL;");
				strConstants += _T("AVG;MAX;MIN;SUM;COUNT;DATEADD;DATEDIFF;DATENAME;DATEPART;GETDATE;ABS;ACOS;ASIN;ATAN;ATN2;CEILING;COS;");
				strConstants += _T("COT;DEGREES;EXP;FLOOR;LOG;LOG10;PI;POWER;RADIANS;RAND;ROUND;SIGN;SIN;SQRT;TAN;NCHAR;CHARINDEX;LEN;");
				strConstants += _T("LOWER;LTRIM;PATINDEX;REPLACE;REPLICATE;RTRIM;SPACE;STR;STUFF;SUBSTRING;UNICODE;UPPER;CONVERT;");
				SetConstantColor(RGB(128,128,128));
				SetConstants((LPTSTR)((LPCTSTR)strConstants.c_str()), (int)strConstants.length(), FALSE);
				SetCommentColor(RGB(0,128,0));
				SetTextColor(RGB(163,21,21));
			}
			break;
		}

		// Update screen
		Invalidate(FALSE);
		UpdateWindow();
	}

	SYNTAX_COLORING_TYPE xcSourceEdit::GetSyntaxColoring()
	{
		// Get syntax coloring
		return m_SyntaxColoring;
	}

	void xcSourceEdit::SetCurrentLine(int index)
	{
		// Set current line
		m_iCurrentLine = max(0, min(m_iNumberLines-1, index));
	}

	int xcSourceEdit::GetCurrentLine()
	{
		// Get current line
		return m_iCurrentLine;
	}

	void xcSourceEdit::SetCurrentChar(int index)
	{
		// Set current char
		m_iCurrentLine = max(0, min(m_lpLineInfo[m_iCurrentLine].nLen, index));
	}

	int xcSourceEdit::GetCurrentChar()
	{
		// Get current char
		return m_iCurrentChar;
	}

	void xcSourceEdit::SetTABs(int iTABs)
	{
		// Set TABs
		m_iTABs = max(1, min(8, iTABs));
	}

	int xcSourceEdit::GetTABs()
	{
		// Get TABs
		return m_iTABs;
	}

	void xcSourceEdit::EnsureVisible()
	{
		// Ensure caret is visible
		UpdateCaret();
	}

	void xcSourceEdit::SetSelection(int iStartLine, int iStartChar, int iEndLine, int iEndChar)
	{
		// Set selection
		m_iLineStartSelection = max(0, iStartLine);
		m_iCharStartSelection = max(0, iStartChar);
		m_iLineEndSelection = min(m_iNumberLines-1, iEndLine);
		if (m_iLineEndSelection == -1)
			m_iLineEndSelection = m_iNumberLines - 1;
		m_iCharEndSelection = min(m_lpLineInfo[m_iLineEndSelection].nLen, iEndChar);
		if (m_iCharEndSelection == -1)
			m_iCharEndSelection = m_lpLineInfo[m_iLineEndSelection].nLen;
	}

	void xcSourceEdit::GetSelection(int& iStartLine, int& iStartChar, int& iEndLine, int& iEndChar)
	{
		// Get selection
		iStartLine = m_iLineStartSelection;
		iStartChar = m_iCharStartSelection;
		iEndLine = m_iLineEndSelection;
		iEndChar = m_iCharEndSelection;
	}

	int xcSourceEdit::GetNumberLines()
	{
		// Get number lines
		return m_iNumberLines;
	}

	void xcSourceEdit::SetBackgroundColor(COLORREF cBgColor)
	{
		// Set background color
		m_cBgColor = cBgColor;
	}

	COLORREF xcSourceEdit::GetBackgroundColor()
	{
		// Get background color
		return m_cBgColor;
	}

	void xcSourceEdit::SetLineDrawProc(LPLINEDRAWPROC lpfnLineDrawProc)
	{
		// Set LineDrawProc
		m_lpfnLineDrawProc = lpfnLineDrawProc;
	}

	void xcSourceEdit::DefaultLineDrawProc(LPTSTR lpszText, int iLen, RECT rect, int iSelectionStart, int iSelectionEnd)
	{
		// Draw line by default
		DrawLine(lpszText, iLen, rect, iSelectionStart, iSelectionEnd);
	}

	BOOL xcSourceEdit::OnEraseBkgnd(HDC pDC)
	{
		return FALSE;
	}

	void xcSourceEdit::OnSize(UINT nType, int cx, int cy)
	{
		//CWnd::OnSize(nType, cx, cy);

		// Get client rectangle
		RECT rcClient;
		GetClientRect(&rcClient);

		// Clear members
		if (m_hMemDC != NULL)
		{
			SelectObject(m_hMemDC,m_pOldMemBitmap);
			SelectObject(m_hMemDC,m_pOldFont);
			::DeleteObject(m_hMemDC);
			DeleteObject(m_hMemBitmap);
		}

		// Create memory DC and bitmap
		HDC hDC = GetDC(m_hWnd);
		m_hMemDC = CreateCompatibleDC(hDC);
		m_hMemBitmap = CreateCompatibleBitmap(hDC, rcClient.right-rcClient.left, rcClient.bottom-rcClient.top);
		m_pOldMemBitmap = (HBITMAP)SelectObject(m_hMemDC,m_hMemBitmap);
		m_pOldFont = (HFONT)SelectObject(m_hMemDC,m_hFont);
		ReleaseDC(m_hWnd,hDC);

		// Create caret
		DestroyCaret();
		//CSize sz = ::GetTextExtent(m_hMemDC,(_T("A"), 1);
		SIZE sz;
		::GetTextExtentPoint32(m_hMemDC,_T("A"), 1 ,&sz);

		::CreateCaret(m_hWnd, NULL, 2, sz.cy);
		POINT pt;
		GetCaretPos(&pt);
		SetCaretPos(pt.x,pt.y);
		ShowCaret(m_hWnd);
	}

	void xcSourceEdit::OnPaint()
	{
		//CPaintDC dc(this);
		HDC dc = ::GetDC(m_hWnd);

		// Clear background
		RECT rcClient;
		GetClientRect(&rcClient);
		FillSolidRect(m_hMemDC,&rcClient, m_cBgColor);

		// Draw content
		DrawContent();

		// Draw on screen DC
		BitBlt(dc,0, 0, rcClient.right-rcClient.left, rcClient.bottom-rcClient.top, m_hMemDC, 0, 0, SRCCOPY);
	}

	void xcSourceEdit::OnVScroll(UINT nSBCode, UINT nPos, void* pScrollBar)
	{
		// Check for listbox visible
		if (m_hListBox != NULL)
		{
			// Destroy listbox
			::DestroyWindow(m_hListBox);
		}

		// Get scroll info
		SCROLLINFO si;
		memset(&si, 0, sizeof(SCROLLINFO));
		si.cbSize = sizeof(SCROLLINFO);
		si.fMask = SIF_ALL;
		GetScrollInfo(m_hWnd,SB_VERT, &si);

		// Update vertical scrolling
		switch (nSBCode)
		{
		case SB_THUMBPOSITION:
		case SB_THUMBTRACK:
			{
				// Update scroll position
				si.nPos = (si.nTrackPos / m_szTextSize.cy) * m_szTextSize.cy;
			}
			break;

		case SB_LINEDOWN:
		case SB_PAGEDOWN:
			{
				// Update scroll position
				si.nPos += m_szTextSize.cy;
				if (si.nPos > (int)(si.nMax-si.nPage))
					si.nPos -= m_szTextSize.cy;
			}
			break;

		case SB_LINEUP:
		case SB_PAGEUP:
			{
				// Update scroll position
				si.nPos -= m_szTextSize.cy;
				if (si.nPos < si.nMin)
					si.nPos = si.nMin;
			}
			break;
		}

		// Set scroll info
		SetScrollInfo(m_hWnd,SB_VERT, &si, TRUE);

		// Get scroll info
		int iVerticalOffset = GetScrollPos(m_hWnd,SB_VERT);
		int iHorizontalOffset = GetScrollPos(m_hWnd,SB_HORZ);

		// Get char position
		POINT pt = PositionFromChar(m_iCurrentLine, m_iCurrentChar);
		pt.x -= iHorizontalOffset;
		pt.y -= iVerticalOffset;
		SetCaretPos(pt.x,pt.y);

		// Update screen
		Invalidate(FALSE);
		UpdateWindow();

		//CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
	}

	void xcSourceEdit::OnHScroll(UINT nSBCode, UINT nPos, void * pScrollBar)
	{
		// Check for listbox visible
		if (m_hListBox != NULL)
		{
			// Destroy listbox
			::DestroyWindow(m_hListBox);
		}

		// Get scroll info
		SCROLLINFO si;
		memset(&si, 0, sizeof(SCROLLINFO));
		si.cbSize = sizeof(SCROLLINFO);
		si.fMask = SIF_ALL;
		GetScrollInfo(m_hWnd,SB_HORZ, &si);

		// Update vertical scrolling
		switch (nSBCode)
		{
		case SB_THUMBPOSITION:
		case SB_THUMBTRACK:
			{
				// Update scroll position
				si.nPos = (si.nTrackPos / m_szTextSize.cx) * m_szTextSize.cx;
			}
			break;

		case SB_LINERIGHT:
		case SB_PAGERIGHT:
			{
				// Update scroll position
				si.nPos += m_szTextSize.cx;
				if (si.nPos > (int)(si.nMax-si.nPage))
					si.nPos -= m_szTextSize.cx;
			}
			break;

		case SB_LINELEFT:
		case SB_PAGELEFT:
			{
				// Update scroll position
				si.nPos -= m_szTextSize.cx;
				if (si.nPos < si.nMin)
					si.nPos = si.nMin;
			}
			break;
		}

		// Set scroll info
		SetScrollInfo(m_hWnd,SB_HORZ, &si, TRUE);

		// Get scroll info
		int iVerticalOffset = GetScrollPos(m_hWnd,SB_VERT);
		int iHorizontalOffset = GetScrollPos(m_hWnd,SB_HORZ);

		// Get char position
		POINT pt = PositionFromChar(m_iCurrentLine, m_iCurrentChar);
		pt.x -= iHorizontalOffset;
		pt.y -= iVerticalOffset;
		SetCaretPos(pt.x,pt.y);

		// Update screen
		Invalidate(FALSE);
		UpdateWindow();

		//CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
	}

	BOOL xcSourceEdit::OnMouseWheel(UINT nFlags, short zDelta, POINT pt)
	{
		// Check for listbox visible
		if (m_hListBox != NULL)
		{
			// Destroy listbox
			::DestroyWindow(m_hListBox);
		}

		// Process mouse-wheel message
		if (zDelta < 0)
		{
			// Scroll down
			OnVScroll(SB_LINEDOWN, 0, 0);
		}
		else
		{
			// Scroll up
			OnVScroll(SB_LINEUP, 0, 0);
		}

		return TRUE;
		//return CWnd::OnMouseWheel(nFlags, zDelta, pt);
	}

	void xcSourceEdit::OnLButtonDown(UINT nFlags, POINT point)
	{
		// Check for valid data
		if (m_lpszText != NULL)
		{
			// Check for listbox visible
			if (m_hListBox!= NULL)
			{
				// Destroy listbox
				::DestroyWindow(m_hListBox);
			}

			// Capture mouse
			::SetCapture(m_hWnd);

			// Get scroll info
			int iVerticalOffset = GetScrollPos(m_hWnd,SB_VERT);
			int iHorizontalOffset = GetScrollPos(m_hWnd,SB_HORZ);

			// Get mouse position
			POINT pt = {point.x+iHorizontalOffset, point.y+iVerticalOffset};

			// Check for valid selection
			if (!m_bSelecting)
			{
				// Set selection start position
				m_iLineStartSelection = LineFromPosition(pt);
				m_iLineEndSelection = m_iLineStartSelection;
				m_iCharStartSelection = CharFromPosition(m_iLineStartSelection, pt);
				m_iCharEndSelection = m_iCharStartSelection;

				// Set caret position
				m_iCurrentLine = m_iLineStartSelection;
				m_iCurrentChar = m_iCharStartSelection;
				POINT ptCaret = PositionFromChar(m_iCurrentLine, m_iCurrentChar);
				ptCaret.x -= iHorizontalOffset;
				ptCaret.y -= iVerticalOffset;
				SetCaretPos(ptCaret.x,ptCaret.y);
			}
			else
			{
				// Set selection end position
				m_iLineEndSelection = LineFromPosition(pt);
				m_iCharEndSelection = CharFromPosition(m_iLineEndSelection, pt);

				// Set caret position
				m_iCurrentLine = m_iLineEndSelection;
				m_iCurrentChar = m_iCharEndSelection;
				POINT ptCaret = PositionFromChar(m_iCurrentLine, m_iCurrentChar);
				ptCaret.x -= iHorizontalOffset;
				ptCaret.y -= iVerticalOffset;
				SetCaretPos(ptCaret.x,ptCaret.y);
			}

			// Update screen
			Invalidate(FALSE);
			UpdateWindow();
		}

		//CWnd::OnLButtonDown(nFlags, point);
	}

	void xcSourceEdit::OnMouseMove(UINT nFlags, POINT point)
	{
		// Check for valid data
		if (m_lpszText != NULL)
		{
			// Check for mouse dragging
			if ((nFlags & MK_LBUTTON) && (m_iLineStartSelection != -1) && (m_iCharStartSelection != -1))
			{
				// Get scroll info
				int iVerticalOffset = GetScrollPos(m_hWnd,SB_VERT);
				int iHorizontalOffset = GetScrollPos(m_hWnd,SB_HORZ);

				// Get mouse position
				POINT pt = {max(0, point.x+iHorizontalOffset), max(0, point.y+iVerticalOffset)};

				// Set selection end position
				m_iLineEndSelection = LineFromPosition(pt);
				m_iCharEndSelection = CharFromPosition(m_iLineEndSelection, pt);

				// Set caret position
				m_iCurrentLine = m_iLineEndSelection;
				m_iCurrentChar = m_iCharEndSelection;
				POINT ptCaret = PositionFromChar(m_iCurrentLine, m_iCurrentChar);
				ptCaret.x -= iHorizontalOffset;
				ptCaret.y -= iVerticalOffset;
				SetCaretPos(ptCaret.x,ptCaret.y);

				// Update screen
				Invalidate(FALSE);
				UpdateWindow();
			}
		}

		//CWnd::OnMouseMove(nFlags, point);
	}

	void xcSourceEdit::OnLButtonUp(UINT nFlags, POINT point)
	{
		// Check for valid data
		if (m_lpszText != NULL)
		{
			// Release mouse
			::ReleaseCapture();

			// Check for invalid selection
			if ((m_iLineStartSelection == m_iLineEndSelection) && (m_iCharStartSelection == m_iCharEndSelection))
			{
				// Clear selection
				m_iLineStartSelection = -1;
				m_iLineEndSelection = -1;
				m_iCharStartSelection = -1;
				m_iCharEndSelection = -1;
			}

			// Update screen
			Invalidate(FALSE);
			UpdateWindow();
		}

		//CWnd::OnLButtonUp(nFlags, point);
	}

	void xcSourceEdit::OnLButtonDblClk(UINT nFlags, POINT point)
	{
		// Check for valid data
		if (m_lpszText != NULL)
		{
			// Get scroll info
			int iVerticalOffset = GetScrollPos(m_hWnd,SB_VERT);
			int iHorizontalOffset = GetScrollPos(m_hWnd,SB_HORZ);

			// Get mouse position
			POINT pt = {point.x+iHorizontalOffset, point.y+iVerticalOffset};

			// Select word
			SelectWord(pt);

			// Update screen
			Invalidate(FALSE);
			UpdateWindow();
		}

		//CWnd::OnLButtonDblClk(nFlags, point);
	}

	void xcSourceEdit::OnSetFocus(HWND pOldWnd)
	{
		//CWnd::OnSetFocus(pOldWnd);

		// Create caret
		DestroyCaret();
		
		SIZE sz;
		::GetTextExtentPoint32(m_hMemDC,_T("A"), 1 ,&sz);
		//CSize sz = ::GetTextExtent(m_hMemDC,(_T("A"), 1);
		::CreateCaret(m_hWnd, NULL, 2, sz.cy);
		POINT pt;
		GetCaretPos(&pt);
		SetCaretPos(pt.x,pt.y);
		ShowCaret(m_hWnd);
	}

	void xcSourceEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		// Check for valid data
		if (m_lpszText != NULL)
		{
			// Process keyboard input
			BOOL bUpdate = FALSE;
			switch (nChar)
			{
			case VK_LEFT:
				{
					// Check for listbox visible
					if (m_hListBox!= NULL)
					{
						// Send message to listbox
						::SendMessage(m_hListBox,WM_KEYDOWN, (WPARAM)nChar, 0);

						// Update mode
						bUpdate = TRUE;
					}
					else
					{
						// Move caret left
						if (m_iCurrentChar > 0)
							m_iCurrentChar--;
						else if (m_iCurrentLine > 0)
						{
							m_iCurrentLine--;
							m_iCurrentChar = m_lpLineInfo[m_iCurrentLine].nLen;
						}

						// Check for valid selection
						if (m_bSelecting)
						{
							// Set selection end position
							m_iLineEndSelection = m_iCurrentLine;
							m_iCharEndSelection = m_iCurrentChar;
						}
					}
				}
				break;

			case VK_RIGHT:
				{
					// Check for listbox visible
					if (m_hListBox!= NULL)
					{
						// Send message to listbox
						::SendMessage(m_hListBox,WM_KEYDOWN, (WPARAM)nChar, 0);

						// Update mode
						bUpdate = TRUE;
					}
					else
					{
						// Move caret right
						if (m_iCurrentChar < m_lpLineInfo[m_iCurrentLine].nLen)
							m_iCurrentChar++;
						else if (m_iCurrentLine < m_iNumberLines-1)
						{
							m_iCurrentLine++;
							m_iCurrentChar = 0;
						}

						// Check for valid selection
						if (m_bSelecting)
						{
							// Set selection end position
							m_iLineEndSelection = m_iCurrentLine;
							m_iCharEndSelection = m_iCurrentChar;
						}
					}
				}
				break;

			case VK_UP:
				{
					// Check for listbox visible
					if (m_hListBox!= NULL)
					{
						// Send message to listbox
						::SendMessage(m_hListBox,WM_KEYDOWN, (WPARAM)nChar, 0);

						// Update mode
						bUpdate = TRUE;
					}
					else
					{
						// Move caret up
						if (m_iCurrentLine > 0)
						{
							m_iCurrentLine--;
							if (m_iCurrentChar > m_lpLineInfo[m_iCurrentLine].nLen)
								m_iCurrentChar = m_lpLineInfo[m_iCurrentLine].nLen;
						}
						else
							m_iCurrentChar = 0;

						// Check for valid selection
						if (m_bSelecting)
						{
							// Set selection end position
							m_iLineEndSelection = m_iCurrentLine;
							m_iCharEndSelection = m_iCurrentChar;
						}
					}
				}
				break;

			case VK_DOWN:
				{
					// Check for listbox visible
					if (m_hListBox!= NULL)
					{
						// Send message to listbox
						::SendMessage(m_hListBox,WM_KEYDOWN, (WPARAM)nChar, 0);

						// Update mode
						bUpdate = TRUE;
					}
					else
					{
						// Move caret down
						if (m_iCurrentLine < m_iNumberLines-1)
						{
							m_iCurrentLine++;
							if (m_iCurrentChar > m_lpLineInfo[m_iCurrentLine].nLen)
								m_iCurrentChar = m_lpLineInfo[m_iCurrentLine].nLen;
						}
						else
							m_iCurrentChar = m_lpLineInfo[m_iCurrentLine].nLen;

						// Check for valid selection
						if (m_bSelecting)
						{
							// Set selection end position
							m_iLineEndSelection = m_iCurrentLine;
							m_iCharEndSelection = m_iCurrentChar;
						}
					}
				}
				break;

			case VK_HOME:
				{
					// Check for listbox visible
					if (m_hListBox!= NULL)
					{
						// Send message to listbox
						::SendMessage(m_hListBox,WM_KEYDOWN, (WPARAM)nChar, 0);
					}
					else
					{
						// Move caret leftmost
						m_iCurrentChar = 0;

						// Check for valid selection
						if (m_bSelecting)
						{
							// Set selection end position
							m_iLineEndSelection = m_iCurrentLine;
							m_iCharEndSelection = m_iCurrentChar;
						}
					}
				}
				break;

			case VK_END:
				{
					// Check for listbox visible
					if (m_hListBox!= NULL)
					{
						// Send message to listbox
						::SendMessage(m_hListBox,WM_KEYDOWN, (WPARAM)nChar, 0);
					}
					else
					{
						// Move caret rightmost
						m_iCurrentChar = m_lpLineInfo[m_iCurrentLine].nLen;

						// Check for valid selection
						if (m_bSelecting)
						{
							// Set selection end position
							m_iLineEndSelection = m_iCurrentLine;
							m_iCharEndSelection = m_iCurrentChar;
						}
					}
				}
				break;

			case VK_SHIFT:
				{
					// Check for listbox visible
					if (m_hListBox!= NULL)
					{
						// Send message to listbox
						::SendMessage(m_hListBox,WM_KEYDOWN, (WPARAM)nChar, 0);
					}
					else
					{
						// Check for valid selection
						if (!m_bSelecting)
						{
							// Selection mode
							m_bSelecting = TRUE;

							// Set selection start and end position
							m_iLineStartSelection = m_iCurrentLine;
							m_iCharStartSelection = m_iCurrentChar;
							m_iLineEndSelection = m_iLineStartSelection;
							m_iCharEndSelection = m_iCharStartSelection;
						}
					}
				}
				break;

			case VK_CONTROL:
				{
					// Check for listbox visible
					if (m_hListBox!= NULL)
					{
						// Send message to listbox
						::SendMessage(m_hListBox,WM_KEYDOWN, (WPARAM)nChar, 0);
					}
					else
					{
						// Copying mode
						m_bCopying = TRUE;
					}
				}
				break;

			case VK_DELETE:
				{
					// Check for listbox visible
					if (m_hListBox!= NULL)
					{
						// Send message to listbox
						::SendMessage(m_hListBox,WM_KEYDOWN, (WPARAM)nChar, 0);
					}
					else
					{
						// Check for valid selection
						if ((m_iLineStartSelection != m_iLineEndSelection) || (m_iCharStartSelection != m_iCharEndSelection))
						{
							// Delete selected text
							DeleteText();
						}
						else
						{
							// Delete next char
							int iCurrentOffset = GetCharOffset(m_iCurrentLine, m_iCurrentChar);
							if ((m_lpszText != NULL) && (iCurrentOffset < m_iSize-2))
							{
								int iOldSize = m_iSize;
								LPTSTR lpszTemp = new _TCHAR[iOldSize];
								memcpy(lpszTemp, m_lpszText, iOldSize*sizeof(_TCHAR));
								if (m_lpszText[iCurrentOffset+1] == '\n')
								{
									m_iSize -= 2;
									free(m_lpszText);
									m_lpszText = (LPTSTR)malloc(m_iSize*sizeof(_TCHAR));
									if (m_lpszText != NULL)
									{
										memcpy(m_lpszText, lpszTemp, iCurrentOffset*sizeof(_TCHAR));
										if (iCurrentOffset <= m_iSize)
											memcpy(m_lpszText+iCurrentOffset, lpszTemp+iCurrentOffset+2, (iOldSize-iCurrentOffset+2)*sizeof(_TCHAR));
									}
									m_iCurrentChar = m_lpLineInfo[m_iCurrentLine].nLen;
								}
								else
								{
									m_iSize--;
									free(m_lpszText);
									m_lpszText = (LPTSTR)malloc(m_iSize*sizeof(_TCHAR));
									if (m_lpszText != NULL)
									{
										memcpy(m_lpszText, lpszTemp, iCurrentOffset*sizeof(_TCHAR));
										if (iCurrentOffset <= m_iSize)
											memcpy(m_lpszText+iCurrentOffset, lpszTemp+iCurrentOffset+1, (iOldSize-iCurrentOffset+1)*sizeof(_TCHAR));
									}
								}
								delete lpszTemp;
							}
						}

						// Update control
						UpdateControl();

						// Update mode
						bUpdate = TRUE;
					}
				}
				break;

			case VK_BACK:
				{
					// Check for listbox visible
					if (m_hListBox!= NULL)
					{
						// Send message to listbox
						::SendMessage(m_hListBox,WM_KEYDOWN, (WPARAM)nChar, 0);
					}
					else
					{
						// Update mode
						bUpdate = TRUE;
					}
				}
				break;

			default:
				{
					// Check for listbox visible
					if (m_hListBox!= NULL)
					{
						// Send message to listbox
						::SendMessage(m_hListBox,WM_KEYDOWN, (WPARAM)nChar, 0);
					}
					else
					{
						// Check for copying flag
						if (!m_bCopying)
						{

							// Check for valid selection
							if ((m_iLineStartSelection != m_iLineEndSelection) || (m_iCharStartSelection != m_iCharEndSelection))
							{
								// Delete selected text
								DeleteText();
							}
						}
					}
				}
				break;
			}

			// Check for valid selection
			if ((!m_bSelecting) && (!bUpdate) && (!m_bCopying))
			{
				// Clear selection
				m_iLineStartSelection = -1;
				m_iLineEndSelection = -1;
				m_iCharStartSelection = -1;
				m_iCharEndSelection = -1;
			}

			// Update caret
			UpdateCaret();
		}

		//CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
	}

	void xcSourceEdit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		// Check for valid data
		if (m_lpszText != NULL)
		{
			// Process keyboard input
			switch (nChar)
			{
			case VK_SHIFT:
				{
					// Check for listbox visible
					if (m_hListBox!= NULL)
					{
						// Send message to listbox
						::SendMessage(m_hListBox,WM_KEYDOWN, (WPARAM)nChar, 0);
					}
					else
					{
						// Selection mode
						m_bSelecting = FALSE;
					}
				}
				break;

			case VK_CONTROL:
				{
					// Check for listbox visible
					if (m_hListBox!= NULL)
					{
						// Send message to listbox
						::SendMessage(m_hListBox,WM_KEYDOWN, (WPARAM)nChar, 0);
					}
					else
					{
						// Copying mode
						m_bCopying = FALSE;
					}
				}
				break;
			}
		}

		//CWnd::OnKeyUp(nChar, nRepCnt, nFlags);
	}

	void xcSourceEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		// Check for keyboard input
		switch (nChar)
		{
		case VK_RETURN:
			{
				// Check for listbox visible
				if (m_hListBox!= NULL)
				{
					// Get selected keyword
					TCHAR strKeyword[255];
					int nIndex = (int)::SendMessage(m_hListBox,CB_GETCURSEL,0,0);
					::SendMessage(m_hListBox,CB_GETLBTEXT,nIndex,(LPARAM)strKeyword);
					//m_pListBox.GetText(m_pListBox.GetCurSel(), (LPARAM)strKeyword);

					// Destroy listbox
					::DestroyWindow(m_hListBox);

					// Delete selected text
					DeleteText();

					// Add text
					AddText((LPTSTR)strKeyword, (int)_tcslen(strKeyword));

					// Update caret position
					m_iCurrentChar += (int)_tcslen(strKeyword);
					UpdateCaret();
				}
				else
				{
					// Add new line
					int iOldSize = m_iSize;
					int iCurrentOffset = GetCharOffset(m_iCurrentLine, m_iCurrentChar);
					LPTSTR lpszTemp = new _TCHAR[iOldSize];
					memcpy(lpszTemp, m_lpszText, iOldSize*sizeof(_TCHAR));
					m_iSize += 2;
					free(m_lpszText);
					m_lpszText = (LPTSTR)malloc(m_iSize*sizeof(_TCHAR));
					memcpy(m_lpszText, lpszTemp, iCurrentOffset*sizeof(_TCHAR));
					m_lpszText[iCurrentOffset] = '\r';
					m_lpszText[iCurrentOffset+1] = '\n';
					memcpy(m_lpszText+iCurrentOffset+2, lpszTemp+iCurrentOffset, (iOldSize-iCurrentOffset)*sizeof(_TCHAR));
					delete lpszTemp;
					m_iCurrentChar = 0;
					m_iCurrentLine++;
				}
			}
			break;

		case VK_BACK:
			{
				// Check for valid selection
				if ((m_iLineStartSelection != m_iLineEndSelection) || (m_iCharStartSelection != m_iCharEndSelection))
				{
					// Delete selected text
					DeleteText();
				}
				else
				{
					// Delete previous char
					int iCurrentOffset = GetCharOffset(m_iCurrentLine, m_iCurrentChar);
					if ((m_lpszText != NULL) && (iCurrentOffset > 0))
					{
						int iOldSize = m_iSize;
						LPTSTR lpszTemp = new _TCHAR[iOldSize];
						memcpy(lpszTemp, m_lpszText, iOldSize*sizeof(_TCHAR));
						if (m_lpszText[iCurrentOffset-1] == '\n')
						{
							m_iSize -= 2;
							free(m_lpszText);
							m_lpszText = (LPTSTR)malloc(m_iSize*sizeof(_TCHAR));
							if (m_lpszText != NULL)
							{
								memcpy(m_lpszText, lpszTemp, (iCurrentOffset-2)*sizeof(_TCHAR));
								if (iCurrentOffset <= m_iSize)
									memcpy(m_lpszText+iCurrentOffset-2, lpszTemp+iCurrentOffset, (iOldSize-iCurrentOffset)*sizeof(_TCHAR));
							}
							m_iCurrentLine = max(0, m_iCurrentLine-1);
							m_iCurrentChar = m_lpLineInfo[m_iCurrentLine].nLen;
						}
						else
						{
							m_iSize--;
							free(m_lpszText);
							m_lpszText = (LPTSTR)malloc(m_iSize*sizeof(_TCHAR));
							if (m_lpszText != NULL)
							{
								memcpy(m_lpszText, lpszTemp, (iCurrentOffset-1)*sizeof(_TCHAR));
								if (iCurrentOffset <= m_iSize)
									memcpy(m_lpszText+iCurrentOffset-1, lpszTemp+iCurrentOffset, (iOldSize-iCurrentOffset)*sizeof(_TCHAR));
							}
							m_iCurrentChar = max(0, m_iCurrentChar-1);
						}
						delete lpszTemp;

						// Check for listbox visible
						if (m_hListBox!= NULL)
						{
							// Destroy listbox
							::DestroyWindow(m_hListBox);

							// Show auto-complete listbox
							ShowAutoComplete();
						}
					}
				}
			}
			break;

		case VK_ESCAPE:
			{
				// Check for listbox visible
				if (m_hListBox!= NULL)
				{
					// Destroy listbox
					::DestroyWindow(m_hListBox);
				}
			}
			break;

		case 0x03: //CTRL + C
			{
				// Check for valid selection
				if ((m_iLineStartSelection != m_iLineEndSelection) || (m_iCharStartSelection != m_iCharEndSelection))
				{
					// Get selection range
					int iStartChar, iEndChar;
					int iStartLine, iEndLine;
					if (m_iLineStartSelection < m_iLineEndSelection)
					{
						iStartChar = m_iCharStartSelection;
						iEndChar = m_iCharEndSelection;
						iStartLine = m_iLineStartSelection;
						iEndLine = m_iLineEndSelection;
					}
					else if (m_iLineStartSelection > m_iLineEndSelection)
					{
						iStartChar = m_iCharEndSelection;
						iEndChar = m_iCharStartSelection;
						iStartLine = m_iLineEndSelection;
						iEndLine = m_iLineStartSelection;
					}
					else
					{
						iStartChar = min(m_iCharStartSelection, m_iCharEndSelection);
						iEndChar = max(m_iCharStartSelection, m_iCharEndSelection);
						iStartLine = m_iLineStartSelection;
						iEndLine = m_iLineEndSelection;
					}
					int iStartOffset = GetCharOffset(iStartLine, iStartChar);
					if (m_lpszText[iStartOffset] == '\r')
						iStartOffset += 2;
					int iEndOffset = GetCharOffset(iEndLine, iEndChar);
					int iCount = iEndOffset - iStartOffset;

					// Copy selection on the clipboard
					if (::OpenClipboard(m_hWnd))
					{
						::EmptyClipboard();
						HGLOBAL hMem = ::GlobalAlloc(GMEM_MOVEABLE, (iCount+1)*sizeof(_TCHAR));
						LPTSTR lpszText = (LPTSTR)::GlobalLock(hMem);
						memcpy(lpszText, m_lpszText+iStartOffset, iCount*sizeof(_TCHAR));
						lpszText[iCount] = '\0';
						::SetClipboardData(CF_UNICODETEXT, hMem);
						::GlobalUnlock(hMem);
						::GlobalFree(hMem);
						::CloseClipboard();
					}
				}
			}
			break;

		case 0x16: //CTRL+V
			{
				// Check for valid selection
				if ((m_iLineStartSelection != m_iLineEndSelection) || (m_iCharStartSelection != m_iCharEndSelection))
				{
					// Delete selected text
					DeleteText();
				}

				// Paste selection from the clipboard
				if (::OpenClipboard(m_hWnd))
				{
					HGLOBAL hMem = ::GetClipboardData(CF_UNICODETEXT);
					LPTSTR lpszText = (LPTSTR)::GlobalLock(hMem);
					if (lpszText != NULL)
					{
						// Add UNICODE text
						int iLen = (int)_tcslen(lpszText);
						AddText(lpszText, iLen);
					}
					else
					{
						// Add ANSI text
						hMem = ::GetClipboardData(CF_TEXT);
						LPSTR lpszText = (LPSTR)::GlobalLock(hMem);
						if (lpszText != NULL)
						{
							int iLen = (int)strlen(lpszText);
							int iSize = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)lpszText, iLen, NULL, 0);
							LPTSTR lpszNewText = (LPTSTR)malloc(iSize*sizeof(_TCHAR));
							MultiByteToWideChar(CP_ACP, 0, (LPCSTR)lpszText, iLen, lpszNewText, iSize);
							AddText(lpszNewText, iLen);
							free(lpszNewText);
						}
					}
					::GlobalUnlock(hMem);
					::CloseClipboard();
				}
			}
			break;

		case 0x01: //ctrl+A
			{
				// Select all text
				m_iCharStartSelection = 0;
				m_iLineStartSelection = 0;
				m_iCharEndSelection = m_lpLineInfo[m_iNumberLines-1].nLen;
				m_iLineEndSelection = m_iNumberLines - 1;
			}
			break;

		default:
			{
				// Check for copying flag
				if (!(GetKeyState(VK_CONTROL)& 0x8000))
				{
					// Add new char
					int iOldSize = m_iSize;
					int iCurrentOffset = GetCharOffset(m_iCurrentLine, m_iCurrentChar);
					LPTSTR lpszTemp = new _TCHAR[iOldSize];
					memcpy(lpszTemp, m_lpszText, iOldSize*sizeof(_TCHAR));
					m_iSize++;
					free(m_lpszText);
					m_lpszText = (LPTSTR)malloc(m_iSize*sizeof(_TCHAR));
					memcpy(m_lpszText, lpszTemp, iCurrentOffset*sizeof(_TCHAR));
					m_lpszText[iCurrentOffset] = nChar;
					memcpy(m_lpszText+iCurrentOffset+1, lpszTemp+iCurrentOffset, (iOldSize-iCurrentOffset)*sizeof(_TCHAR));
					delete lpszTemp;
					m_iCurrentChar++;

					// Check for listbox visible
					if (m_hListBox!= NULL)
					{
						// Destroy listbox
						::DestroyWindow(m_hListBox);

						// Show auto-complete listbox
						ShowAutoComplete();
					}
				}
				else
				{
					// Clear flags
					m_bSelecting = FALSE;
					m_bCopying = FALSE;

					// Show auto-complete listbox
					ShowAutoComplete();
				}
			}
		}

		// Update control
		UpdateControl();

		// Update caret
		UpdateCaret();

	}

	BOOL xcSourceEdit::OnSetCursor(HWND pWnd, UINT nHitTest, UINT message)
	{
		// Update cursor
		if ((nHitTest == HTCLIENT) && (m_hListBox== NULL))
		{
			SetCursor(LoadCursor(NULL, IDC_IBEAM));
			return TRUE;
		}

		return FALSE;
	}
	
	int xcSourceEdit::Invalidate(BOOL bDraw)
	{
		::InvalidateRect(m_hWnd,NULL,bDraw);
		return 1;
	}
	int xcSourceEdit::UpdateWindow()
	{
		::UpdateWindow(m_hWnd);
		return 1;
	}
	int xcSourceEdit::FillSolidRect(HDC hDC,LPCRECT lpRect, COLORREF clr)
	{
		::SetBkColor(hDC, clr);
		::ExtTextOut(hDC, 0, 0, ETO_OPAQUE, lpRect, NULL, 0, NULL);
		return 1;
	}
	int xcSourceEdit::DoDraw(HDC hdc,RECT * pRect)
	{
		return 1;
	}

	int xcSourceEdit::LoadContent(LPTSTR pStr,SYNTAX_COLORING_TYPE type)
	{
		SetSyntaxColoring(type);

		m_iSize = (int)_tcslen(pStr) + 2;
		if(m_lpszText) free(m_lpszText);
		m_lpszText = (LPTSTR)malloc(m_iSize*sizeof(_TCHAR));
		_tcscpy_s(m_lpszText,m_iSize,pStr);
		m_lpszText[m_iSize-2] = '\r';
		m_lpszText[m_iSize-1] = '\n';

		// Update control
		UpdateControl();

		// Update screen
		Invalidate(FALSE);
		UpdateWindow();

		return 1;
	}
	
	int xcSourceEdit::GetContent(BSTR &bstr)
	{
		bstr = NULL;
		if(m_iSize- 2 < 1)  return 0;
		bstr = ::SysAllocStringLen(NULL,m_iSize);
		_tcsncpy_s(bstr,m_iSize ,m_lpszText,m_iSize- 2);
		bstr[m_iSize - 2] = '\0';
		bstr[m_iSize - 1] = '\0';
		return m_iSize- 2;
	}

}}
