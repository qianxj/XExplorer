#pragma once
namespace Hxsoft
{
	namespace XFrame
	{
		class XCell::CFrame;
		class XFRAME_EXPORTS XCellCtrl:
			public XControl
		{
		public:
			XCellCtrl();
			~XCellCtrl();
		public:
			virtual int HandleMessage(HWND hWnd, UINT message,WPARAM wParam, LPARAM lParam);
		public:
			XCell::CFrame* m_pCellFrame;
		};
	}
}
