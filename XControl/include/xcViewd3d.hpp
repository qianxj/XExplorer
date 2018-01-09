#pragma once
#include <xframe.h>
#ifdef d3d
#include <mmsystem.h>
#include <d3dx9.h>
#endif
#pragma warning( disable : 4996 ) // disable deprecated warning 
#include <strsafe.h>
#pragma warning( default : 4996 )

namespace Hxsoft
{
	namespace XFrame
	{
		class XCONTROL_API xcViewd3d:
			public xfControl
		{
#ifdef d3d
		public:
			// A structure for our custom vertex type
			struct CUSTOMVERTEX
			{
				FLOAT x, y, z;      // The untransformed, 3D position for the vertex
				DWORD color;        // The vertex color
			};

			// Our custom FVF, which describes our custom vertex structure
			#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)
		public:
			LPDIRECT3D9             m_pD3D; // Used to create the D3DDevice
			LPDIRECT3DDEVICE9       m_pd3dDevice; // Our rendering device
			LPDIRECT3DVERTEXBUFFER9 m_pVB; // Buffer to hold vertices
#endif		

		public:
			xcViewd3d();
		public:
			~xcViewd3d();
		public:
			virtual	void Initial();
			virtual int DoDraw(HDC hPaintDC,RECT * pDrawRect=NULL);
#ifdef d3d
			bool EvtEraseBkgn(TEvent* pEvent,LPARAM lParam);
			bool EvtPaint(TEvent* pEvent,LPARAM lParam);
			bool EvtMouseMove(TEvent* pEvent,LPARAM lParam);
			bool EvtMouseLeave(TEvent* pEvent,LPARAM lParam);
			bool EvtLButtonUp(TEvent* pEvent,LPARAM lParam);
			bool EvtTimer(TEvent* pEvent,LPARAM lParam);
		public:
			HRESULT InitD3D();
			HRESULT InitGeometry();
			VOID Cleanup();
			VOID SetupMatrices();
			VOID Render();
#endif		
		};
	}
}
