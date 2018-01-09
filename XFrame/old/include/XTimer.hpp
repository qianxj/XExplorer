#pragma once
#include "xapp.hpp"
namespace Hxsoft
{
	namespace XFrame
	{
		class XFRAME_API XTimer
		{
		public:
			XTimer():m_pData(NULL),m_bContinue(true){};

		public:
			int m_delay;
			int m_current;
			int m_timerID;
			bool m_bContinue;
		public:
			void* m_pData;
			int (* fnTimer)(int ,void* );
		public:
			int EventTimer();
			int KillTimer();
		public:
			int OnTimer();
		};

		inline int XTimer::KillTimer()
		{
			XApp::GetXApp()->RemoveTimer(this);
			return 1;
		}

		inline int XTimer::EventTimer()
		{
			if(m_delay <= 0) return 1;
			if((int)GetTickCount() - m_current > m_delay)
			{
				OnTimer();
				m_current = GetTickCount();
			}
			if(!m_bContinue) KillTimer();
			return 1;
		}
		inline int XTimer::OnTimer()
		{
			DWORD dwThreadId;
			HANDLE	hThread;
			hThread = CreateThread( 
            NULL,              // default security attributes
            0,                 // use default stack size  
			XApp::TimerEventThreadProc,        // thread function 
            this,             // argument to thread function 
            0,                 // use default creation flags 
            &dwThreadId);   // returns the thread identifier
			::WaitForSingleObject(hThread,INFINITE);
			::CloseHandle(hThread);
			return 1;
		}
	}
}