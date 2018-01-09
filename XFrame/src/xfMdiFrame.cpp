#include "StdAfx.h"
#include "xfwinbase.hpp"
#include "xfMdiFrame.hpp"

namespace Hxsoft{ namespace XFrame
{
	xfMdiFrame::xfMdiFrame(void):m_hMdiClient(NULL)
	{
	}

	xfMdiFrame::~xfMdiFrame(void)
	{
	}
	
	int xfMdiFrame::OnXCommand(LPCTSTR	pStrID, class xfControl * pControl)
	{
		if(xfWin::OnXCommand(pStrID,pControl)!=1)
		{
			HWND hSheet = (HWND)::SendMessage(m_hMdiClient,WM_MDIGETACTIVE,0,0);
			if(hSheet)
			{
				//WPARAM wParam =	(WPARAM)pStrID ;
				//LPARAM lParam = (LPARAM)pControl ;
				//return (int)SendMessage(hSheet,WM_XCOMMAND,wParam,lParam);
				
				int nRet = 0;
				xfWin * pWin = (xfWin *) GetProp(hSheet,_T("this"));
				if(pWin)
				{
					nRet = pWin->OnXCommand(pStrID,pControl);
					if(nRet > 0) return nRet;
					if(pWin->m_pEventListons && pWin->m_pEventListons->size() >0 )
					{
						multimap <DWORD,TListonEvent>::iterator tr,m_lIter,m_uIter;
						m_uIter = pWin->m_pEventListons->upper_bound(WM_XCOMMAND);
						m_lIter = pWin->m_pEventListons->lower_bound(WM_XCOMMAND);

						pair<multimap<DWORD,TListonEvent>::iterator,
							multimap<DWORD,TListonEvent>::iterator> pr;
						pr = pWin->m_pEventListons->equal_range(WM_XCOMMAND);
						while(pr.first != pr.second)
						{				
							tr = pr.first;
							if(tr!= pWin->m_pEventListons->end() && tr->first == WM_XCOMMAND  && tr->second.pXvmEvent )
							{
								TEvent tEvent;
								tEvent.xcommand.pStrID = (LPTSTR)pStrID;
								tEvent.xcommand.pCmdFrom = (xfAreaObject *)pControl;
								tEvent.hWnd = hSheet;
								tEvent.wParam = (WPARAM)pStrID ;
								tEvent.lParam = (LPARAM)pControl ;
								tEvent.pSrcObject = pWin;   

								if(tr->second.pXCmdID)
								{
									if(::_tcsicmp(tr->second.pXCmdID,tEvent.xcommand.pStrID)==0)
									{
										if(this->xvmEventRecall(tr->second.pXvmEvent,&tEvent,tr->second.lparam)) return 1;
									}
								}else
								{
									if(this->xvmEventRecall(tr->second.pXvmEvent,&tEvent,tr->second.lparam)) return 1;
								}
							}
							pr.first++;
						}
					}
					return 0;
				}
			}else
			{
				return 0;
			}
		}
		return 1;
	}
	
	HWND xfMdiFrame::GetActiveSheet()
	{
		return (HWND)::SendMessage(m_hMdiClient,WM_MDIGETACTIVE,0,0);
	}
}}
