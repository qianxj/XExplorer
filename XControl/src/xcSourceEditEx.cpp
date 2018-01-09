#include "StdAfx.h"
#include "..\include\xcSourceEditEx.hpp"

namespace Hxsoft{namespace XFrame
{
	xcSourceEditEx::xcSourceEditEx(void): m_pTexts(new std::vector<LPTSTR>)
	{
		m_hMemDC = NULL;
		m_hMemBitmap = NULL;
		m_pOldMemBitmap = NULL;
		m_pOldFont = NULL;

		m_hListBox = NULL;
		m_cBgColor = RGB(255,255,255);

		m_szTextExtent.cx = 0;
		m_szTextExtent.cy = 0;
		
		m_szTextSize.cx = 0;
		m_szTextSize.cy = 0;

		m_ptTopLeft.x = 0;
		m_ptTopLeft.y = 0;

		SetEventListions();
	}

	xcSourceEditEx::~xcSourceEditEx(void)
	{

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

	int xcSourceEditEx::SetEventListions()
	{
		this->AttachListon(WM_ERASEBKGND, (FNEvent)&xcSourceEditEx::EvtEraseBkgnd);
		this->AttachListon(WM_SIZE, (FNEvent)&xcSourceEditEx::EvtSize);
		this->AttachListon(WM_PAINT, (FNEvent)&xcSourceEditEx::EvtPaint);
		this->AttachListon(WM_VSCROLL, (FNEvent)&xcSourceEditEx::EvtVScroll);
		this->AttachListon(WM_HSCROLL, (FNEvent)&xcSourceEditEx::EvtHScroll);
		this->AttachListon(WM_MOUSEWHEEL, (FNEvent)&xcSourceEditEx::EvtMouseWheel);
		this->AttachListon(WM_LBUTTONDOWN, (FNEvent)&xcSourceEditEx::EvtLButtonDown);

		this->AttachListon(WM_MOUSEMOVE, (FNEvent)&xcSourceEditEx::EvtMouseMove);
		this->AttachListon(WM_LBUTTONUP, (FNEvent)&xcSourceEditEx::EvtLButtonUp);
		this->AttachListon(WM_LBUTTONDBLCLK, (FNEvent)&xcSourceEditEx::EvtLButtonDblClk);

		this->AttachListon(WM_SETFOCUS, (FNEvent)&xcSourceEditEx::EvtSetFocus);
		this->AttachListon(WM_KEYDOWN, (FNEvent)&xcSourceEditEx::EvtKeyDown);
		this->AttachListon(WM_KEYUP, (FNEvent)&xcSourceEditEx::EvtKeyUp);
		this->AttachListon(WM_CHAR, (FNEvent)&xcSourceEditEx::EvtChar);
		this->AttachListon(WM_SETCURSOR,(FNEvent)&xcSourceEditEx::EvtSetCursor);

		return 1;
	}

	int xcSourceEditEx::UnSetEventListions()
	{
		this->ClearAllListonsTo();
		return 1;
	}

	bool xcSourceEditEx::EvtEraseBkgnd(TEvent *pEvent,LPARAM lparam)
	{
		pEvent->dwRet = OnEraseBkgnd((HDC)pEvent->wParam);
		return true;
	}
	bool xcSourceEditEx::EvtSize(TEvent *pEvent,LPARAM lparam)
	{
		OnSize(pEvent->size.nFlags,pEvent->size.point.x,pEvent->size.point.y);
		return true;
	}
	bool xcSourceEditEx::EvtPaint(TEvent *pEvent,LPARAM lparam)
	{
		OnPaint();
		return true;
	}
	bool xcSourceEditEx::EvtVScroll(TEvent *pEvent,LPARAM lparam)
	{
		OnVScroll(pEvent->scroll.nSBCode,pEvent->scroll.nPos,NULL);
		return true;
	}
	bool xcSourceEditEx::EvtHScroll(TEvent *pEvent,LPARAM lparam)
	{
		OnHScroll(pEvent->scroll.nSBCode,pEvent->scroll.nPos,NULL);
		return true;
	}
	bool xcSourceEditEx::EvtMouseWheel(TEvent *pEvent,LPARAM lparam)
	{
		OnMouseWheel(pEvent->wheel.fwKeys,pEvent->wheel.zDelta,pEvent->wheel.point);
		return true;
	}
	bool xcSourceEditEx::EvtLButtonDown(TEvent *pEvent,LPARAM lparam)
	{
		OnLButtonDown(pEvent->mouse.nFlags,pEvent->mouse.point);
		return true;
	}
	bool xcSourceEditEx::EvtMouseMove(TEvent *pEvent,LPARAM lparam)
	{
		OnMouseMove(pEvent->mouse.nFlags,pEvent->mouse.point);
		return true;
	}
	bool xcSourceEditEx::EvtLButtonUp(TEvent *pEvent,LPARAM lparam)
	{
		::SetFocus(m_hWnd);
		OnLButtonUp(pEvent->mouse.nFlags,pEvent->mouse.point);
		return true;
	}
	bool xcSourceEditEx::EvtLButtonDblClk(TEvent *pEvent,LPARAM lparam)
	{
		OnLButtonDblClk(pEvent->mouse.nFlags,pEvent->mouse.point);
		return true;
	}
	bool xcSourceEditEx::EvtSetFocus(TEvent *pEvent,LPARAM lparam)
	{
		OnSetFocus((HWND)pEvent->wParam);
		return true;
	}
	bool xcSourceEditEx::EvtKeyDown(TEvent *pEvent,LPARAM lparam)
	{
		OnKeyDown(pEvent->key.nChar,pEvent->key.nRepCnt,pEvent->key.nFlags);
		return true;
	}
	bool xcSourceEditEx::EvtKeyUp(TEvent *pEvent,LPARAM lparam)
	{
		OnKeyUp(pEvent->key.nChar,pEvent->key.nRepCnt,pEvent->key.nFlags);
		return true;
	}
	bool xcSourceEditEx::EvtChar(TEvent *pEvent,LPARAM lparam)
	{
		OnChar(pEvent->key.nChar,pEvent->key.nRepCnt,pEvent->key.nFlags);
		return true;
	}

	bool xcSourceEditEx::EvtSetCursor(TEvent *pEvent,LPARAM lparam)
	{
		OnSetCursor(pEvent->cursor.hWnd,pEvent->cursor.nHitTest,pEvent->cursor.mouseMsg);
		return true;
	}

	//load text content
	int xcSourceEditEx::LoadText(LPTSTR  pText)
	{
		std::vector<LPTSTR>::const_iterator tr;
		tr = m_pTexts->begin();
		while((tr = m_pTexts->begin())!= m_pTexts->end())if(*tr)delete *tr;
		m_pTexts->clear();

		if(!pText) return 1;

		int nPos = 0;
		LPTSTR pNextStr = pText;
		LPTSTR pStr;
		while(true)
		{
			while(!(pNextStr[nPos]=='\0'||pNextStr[nPos]=='\r'||pNextStr[nPos]=='\n'))nPos++;
			if(pNextStr[nPos]=='\0')
			{
				m_pTexts->push_back(_tcsdup(pNextStr));
				break;
			}
			else if(pNextStr[nPos]=='\r' && pNextStr[nPos + 1]=='\n')
			{
				pStr = new TCHAR[nPos + 1];
				_tcsncpy_s(pStr,nPos + 1,pNextStr,nPos);
				m_pTexts->push_back(_tcsdup(pNextStr));
				pNextStr += nPos + 2;
			}
			else if(pNextStr[nPos]=='\n' && pNextStr[nPos + 1]=='\r')
			{
				pStr = new TCHAR[nPos + 1];
				_tcsncpy_s(pStr,nPos + 1,pNextStr,nPos);
				m_pTexts->push_back(_tcsdup(pNextStr));
				pNextStr += nPos + 2;
			}
			else if(pNextStr[nPos]=='\r')
			{
				pStr = new TCHAR[nPos + 1];
				_tcsncpy_s(pStr,nPos + 1,pNextStr,nPos);
				m_pTexts->push_back(_tcsdup(pNextStr));
				pNextStr += nPos + 1;
			}
			else if(pNextStr[nPos]=='\n' )
			{
				pStr = new TCHAR[nPos + 1];
				_tcsncpy_s(pStr,nPos + 1,pNextStr,nPos);
				m_pTexts->push_back(_tcsdup(pNextStr));
				pNextStr += nPos + 1;
			}
		}
		
		return 1;
	}
	BOOL xcSourceEditEx::OnEraseBkgnd(HDC pDC)
	{
		return FALSE;
	}

	void xcSourceEditEx::OnSize(UINT nType, int cx, int cy)
	{
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
		
		SIZE sz;
		::GetTextExtentPoint32(m_hMemDC,_T("A"), 1 ,&sz);

		::CreateCaret(m_hWnd, NULL, 2, sz.cy);
		POINT pt;
		GetCaretPos(&pt);
		SetCaretPos(pt.x,pt.y);
		ShowCaret(m_hWnd);
	}

	void xcSourceEditEx::OnPaint()
	{
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

	void xcSourceEditEx::OnVScroll(UINT nSBCode, UINT nPos, void* pScrollBar)
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
	}

	void xcSourceEditEx::OnHScroll(UINT nSBCode, UINT nPos, void * pScrollBar)
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

	BOOL xcSourceEditEx::OnMouseWheel(UINT nFlags, short zDelta, POINT pt)
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

	void xcSourceEditEx::OnLButtonDown(UINT nFlags, POINT point)
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

	void xcSourceEditEx::OnMouseMove(UINT nFlags, POINT point)
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

	void xcSourceEditEx::OnLButtonUp(UINT nFlags, POINT point)
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

	void xcSourceEditEx::OnLButtonDblClk(UINT nFlags, POINT point)
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

	void xcSourceEditEx::OnSetFocus(HWND pOldWnd)
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

	void xcSourceEditEx::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
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

	void xcSourceEditEx::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
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

	void xcSourceEditEx::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
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
					AddText((LPTSTR)strKeyword, _tcslen(strKeyword));

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

	BOOL xcSourceEditEx::OnSetCursor(HWND pWnd, UINT nHitTest, UINT message)
	{
		// Update cursor
		if ((nHitTest == HTCLIENT) && (m_hListBox== NULL))
		{
			SetCursor(LoadCursor(NULL, IDC_IBEAM));
			return TRUE;
		}

		return FALSE;
	}
	
	int xcSourceEditEx::Invalidate(BOOL bDraw)
	{
		::InvalidateRect(m_hWnd,NULL,bDraw);
		return 1;
	}
	int xcSourceEditEx::UpdateWindow()
	{
		::UpdateWindow(m_hWnd);
		return 1;
	}
	int xcSourceEditEx::FillSolidRect(HDC hDC,LPCRECT lpRect, COLORREF clr)
	{
		::SetBkColor(hDC, clr);
		::ExtTextOut(hDC, 0, 0, ETO_OPAQUE, lpRect, NULL, 0, NULL);
		return 1;
	}
	int xcSourceEditEx::DoDraw(HDC hdc,RECT * pRect)
	{
		return 1;
	}
}}
