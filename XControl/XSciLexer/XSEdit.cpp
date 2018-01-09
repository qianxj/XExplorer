
#include "stdafx.h"
#include "xsedit.hpp"
#include "ixsedit.hpp"

#include "scintilla.h"
#include "SciLexer.h"

using namespace Scintilla;

#include "SciTEWin.h"
#include "MultiplexExtension.h"
#include "utf8_16.h"


MultiplexExtension multiExtender;
Extension *extender = &multiExtender;

//#include <fstream>
#include <io.h> 
#include "Extender.h"


/*
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif*/

class MyExtension :public  Extension
{
public:
	virtual bool Initialise(ExtensionAPI *host_){return true;}
	virtual bool Finalise(){return true;}
	virtual bool Clear(){return true;}
	virtual bool Load(const char *filename){return true;}

	virtual bool OnChar(char ch){return m_fnOnChar(m_pOwner,m_pEdit,ch);}
	virtual bool OnKey(int, int){return true;}
	
	virtual bool OnDwellStart(int pos, const char * word)
	{ if(m_fnOnDwell)return m_fnOnDwell(m_pOwner,m_pEdit,pos,word); else return false; }

public:
	void *	  m_pOwner;
	Hxsoft::XFrame::XSEdit*	  m_pEdit;
public:
	bool (*m_fnOnChar)(void * pOwner, Hxsoft::XFrame::XSEdit *pEdit,char ch) ;
	bool (*m_fnOnDwell)(void * pOwner, Hxsoft::XFrame::XSEdit *pEdit,int pos, const char *word);
};

//using namespace XSciLexer;
namespace Hxsoft
{
	namespace XFrame
	{
		// the next 2 arrays are used to determine lexer format from file extensions
		static TCHAR *szExtensions[] = 
		{
			_T("|py|"),                                                                // SCLEX_PYTHON
			_T("|c|cc|cpp|cxx|cs|h|hh|hpp|hxx|sma|dlg|rc|rc2|idl|odl|mak|js|java|xscript|"),   // SCLEX_CPP
			_T("|htm|html|shtml|htt|cfm|tpl|hta|"),         // SCLEX_HTML
			_T("|xml|gcl|xsl|svg|xul|xsd|dtd|xslt|axl|xpage|"),   // SCLEX_XML
			_T("|pl|pm|cgi|pod|"),                          // SCLEX_PERL
			_T("|sql|spec|body|sps|spb|sf|sp|"),            // SCLEX_SQL
			_T("|vb|bas|frm|cls|ctl|pag|dsr|dob|"),         // SCLEX_VB
			_T("|properties|"),                             // SCLEX_PROPERTIES
			_T("|err|"),                                    // SCLEX_ERRORLIST
			_T("|iface|mak|"),                              // SCLEX_MAKEFILE
			_T("|bat|cmd|nt|"),                             // SCLEX_BATCH
			_T("|xcode|"),                                  // SCLEX_XCODE
			_T("|tex|sty|"),                                // SCLEX_LATEX
			_T("|lua|"),                                    // SCLEX_LUA
			_T("|diff|"),                                   // SCLEX_DIFF
			_T("|conf|"),                                   // SCLEX_CONF
			_T("|pas|inc|pp|"),                             // SCLEX_PASCAL
			_T("|ave|"),                                    // SCLEX_AVE
			_T("|ada|ads|adb|"),                            // SCLEX_ADA
			_T("|lsp|lisp|scm|smd|ss|"),                    // SCLEX_LISP
			_T("|rb|"),                                     // SCLEX_RUBY
			_T("|e|"),                                      // SCLEX_EIFFEL
			_T("|e|"),                                      // SCLEX_EIFFELKW
			_T("|tcl|"),                                    // SCLEX_TCL
			_T("|tab|spf|"),                                // SCLEX_NNCRONTAB
			_T("|ant|"),                                    // SCLEX_BULLANT
			_T("|vbs|dsm|"),                                // SCLEX_VBSCRIPT
			_T("|vl|vx|vm|vf|vt|voc"),                      // SCLEX_VL
//			_T("|asp|aspx|"),                               // SCLEX_ASP
//			_T("|php|php3|php4|"),                          // SCLEX_PHP
			_T("|bc|cln|"),                                 // SCLEX_BAAN
			_T("|m|"),                                      // SCLEX_MATLAB
			_T("|sol|"),                                    // SCLEX_SCRIPTOL
			_T("|asm|"),                                    // SCLEX_ASM
			_T("|cpp|"),                                    // SCLEX_CPPNOCASE
			_T("|f|for|f90|f95|"),                          // SCLEX_FORTRAN
			_T("|f77|"),                                    // SCLEX_F77
			_T("|css|"),                                    // SCLEX_CSS
			_T("|pov|"),                                    // SCLEX_POV
			_T("|lt|lot|"),                                 // SCLEX_LOUT
			_T("|src|em|"),                                 // SCLEX_ESCRIPT
			0,
		};
		static int nFormats[] = {
			SCLEX_PYTHON,
			SCLEX_CPP,
			SCLEX_HTML,
			SCLEX_XML,
			SCLEX_PERL,
			SCLEX_SQL,
			SCLEX_VB,
			SCLEX_PROPERTIES,
			SCLEX_ERRORLIST,
			SCLEX_MAKEFILE,
			SCLEX_BATCH,
			SCLEX_XCODE,
			SCLEX_LATEX,
			SCLEX_LUA,
			SCLEX_DIFF,
			SCLEX_CONF,
			SCLEX_PASCAL,
			SCLEX_AVE,
			SCLEX_ADA,
			SCLEX_LISP,
			SCLEX_RUBY,
			SCLEX_EIFFEL,
			SCLEX_EIFFELKW,
			SCLEX_TCL,
			SCLEX_NNCRONTAB,
			SCLEX_BULLANT,
			SCLEX_VBSCRIPT,
			SCLEX_VL,
//			SCLEX_ASP,
//			SCLEX_PHP,
			SCLEX_BAAN,
			SCLEX_MATLAB,
			SCLEX_SCRIPTOL,
			SCLEX_ASM,
			SCLEX_CPPNOCASE,
			SCLEX_FORTRAN,
			SCLEX_F77,
			SCLEX_CSS,
			SCLEX_POV,
			SCLEX_LOUT,
			SCLEX_ESCRIPT,
			0,
		};

		WNDPROC XSEdit::m_OldWinProc = NULL;
		/////////////////////////////////////
		// @mfunc This is an empty constructor
		// @rvalue void | not used
		//
		XSEdit::XSEdit():m_pSciTEWin(new SciTEWin(new MultiplexExtension))
		{
			m_bLinenumbers = FALSE;
			m_bSelection = TRUE;
			m_bFolding = FALSE;
			m_nSearchflags = 0;

			
			((SciTEWin*)m_pSciTEWin)->ReadAbbrevPropFile();
			AttachListon(NULL,-1,(FNEvent)&XSEdit::EvtNotify);
			
			//extender.(Extension *)new MyExtension)

		}

		bool XSEdit::EvtNotify(TEvent* pEvent,LPARAM lParam)
		{
			((SciTEWin*)m_pSciTEWin)->Notify(reinterpret_cast<SCNotification *>(pEvent->lParam));
			return true;
		}


		/////////////////////////////////////
		// @mfunc This is a destructor
		// @rvalue void | not used
		//
		XSEdit::~XSEdit()
		{

			Extension * extender = ((SciTEWin *)m_pSciTEWin)->extender;
			if(m_pSciTEWin)delete (SciTEWin *)m_pSciTEWin;
			if(extender)delete extender;
		}

		void XSEdit::Initial()
		{
			((SciTEWin*)m_pSciTEWin)->wEditor = m_hWnd;
			((SciTEWin*)m_pSciTEWin)->fnEditor = reinterpret_cast<SciFnDirect>(::SendMessage(
				m_hWnd, SCI_GETDIRECTFUNCTION, 0, 0));
			((SciTEWin*)m_pSciTEWin)->ptrEditor = ::SendMessage(m_hWnd,
				SCI_GETDIRECTPOINTER, 0, 0);

			((SciTEWin*)m_pSciTEWin)->CreateBuffers();
			New();

			if(GetWindowLongPtr(m_hWnd,GWLP_WNDPROC)!=(LONG)XSEdit::WndProc)
			{
				m_OldWinProc = (WNDPROC)GetWindowLongPtr(m_hWnd,GWLP_WNDPROC);
			}
			SetWindowLongPtr(m_hWnd,GWLP_WNDPROC,(LONG)XSEdit::WndProc);
			::SetProp(m_hWnd,L"this",this);
		}

		LRESULT PASCAL XSEdit::WndProc(HWND hWnd,UINT iMessage, WPARAM wParam, LPARAM lParam)
		{
			if(iMessage==WM_CHAR)
			{
				if ((GetKeyState(VK_CONTROL)& 0x8000))
				{
					if(wParam==0x02) //ctrl-b
					{
						return 0;
					}
				}
			}
			if(iMessage==WM_KEYDOWN)
			{
				if ((GetKeyState(VK_CONTROL)& 0x8000))
				{
					if(wParam=='B')
					{
						XSEdit * pEdit = (XSEdit *)GetProp(hWnd,L"this");
						if(pEdit)
						{
							SendMessage(hWnd,SCI_AUTOCCANCEL,0,0);
							((SciTEWin *)pEdit->m_pSciTEWin)->StartExpandAbbreviation();
						}
						return 0;
					}
				}
			}
			if(iMessage==WM_LBUTTONDOWN)
			{
				if ((GetKeyState(VK_MENU)& 0x8000))
				{
					XSEdit * pEdit = (XSEdit *)GetProp(hWnd,L"this");
					NMHDR hdr;
					memset(&hdr,0,sizeof(NMHDR));
					hdr.hwndFrom = pEdit->GetHWND();
					hdr.idFrom = 0;
					hdr.code = WM_LBUTTONDOWN;
					::SendMessage(::GetParent(pEdit->GetHWND()),WM_NOTIFY,0,(LPARAM)&hdr);
					//return 0;
				}
			}
			return CallWindowProc(m_OldWinProc,hWnd,iMessage,wParam,lParam);
		}

		int XSEdit::New()
		{
			((SciTEWin*)m_pSciTEWin)->MenuCommand(IDM_NEW,0);
			return 1;
		}

		int XSEdit::SetContent (BSTR bstr,LPCTSTR ext)
		{
			int _convert = WideCharToMultiByte(CP_ACP, 0, ext, -1, NULL, 0, NULL, NULL);
			char* extA = new char[_convert];
			int nRet = WideCharToMultiByte(CP_ACP, 0, ext, -1, extA, _convert, NULL, NULL);
	
			Utf8_16_Read convert;
			convert.m_eEncoding=Utf8_16::eUtf16LittleEndian;
			convert.m_bFirstRead = false;

			((SciTEWin*)m_pSciTEWin)->SendEditor(SCI_BEGINUNDOACTION);	// Group together clear and insert
			((SciTEWin*)m_pSciTEWin)->SendEditor(SCI_CLEARALL);
			
			SString languageOverride;

			int nLen = (int)_tcslen(bstr)*sizeof(TCHAR);
			bool firstBlock = true;
			nLen = (int)convert.convert((char *)bstr, nLen);

			char *dataBlock = convert.getNewBuf();
			languageOverride = ((SciTEWin*)m_pSciTEWin)->DiscoverLanguage(dataBlock, nLen);
			
			((SciTEWin*)m_pSciTEWin)->SendEditor(SCI_ALLOCATE, nLen + 1000);
			((SciTEWin*)m_pSciTEWin)->SendEditorString(SCI_ADDTEXT, nLen, dataBlock);

			((SciTEWin*)m_pSciTEWin)->SendEditor(SCI_ENDUNDOACTION);
			if (languageOverride.length() && !ext)
			{
				((SciTEWin*)m_pSciTEWin)->CurrentBuffer()->overrideExtension = languageOverride;
				((SciTEWin*)m_pSciTEWin)->ReadProperties();
				((SciTEWin*)m_pSciTEWin)->SetIndentSettings();
			}else
			{
				((SciTEWin*)m_pSciTEWin)->CurrentBuffer()->overrideExtension = extA;
				((SciTEWin*)m_pSciTEWin)->ReadProperties();
				((SciTEWin*)m_pSciTEWin)->SetIndentSettings();
			}
			((SciTEWin*)m_pSciTEWin)->SendEditor(SCI_SETCODEPAGE, SC_CP_UTF8);
			((SciTEWin*)m_pSciTEWin)->DiscoverEOLSetting();

			if (((SciTEWin*)m_pSciTEWin)->props.GetInt("indent.auto")) {
				((SciTEWin*)m_pSciTEWin)->DiscoverIndentSetting();
			}
			if (!((SciTEWin*)m_pSciTEWin)->SendEditor(SCI_GETUNDOCOLLECTION)) {
				((SciTEWin*)m_pSciTEWin)->SendEditor(SCI_SETUNDOCOLLECTION, 1);
			}
			((SciTEWin*)m_pSciTEWin)->SendEditor(SCI_SETSAVEPOINT);
			if (((SciTEWin*)m_pSciTEWin)->props.GetInt("fold.on.open") > 0) {
				((SciTEWin*)m_pSciTEWin)->FoldAll();
			}
			((SciTEWin*)m_pSciTEWin)->SendEditor(SCI_GOTOPOS, 0);
			
			if(extA )delete extA ;

			return 1;
		}

		int XSEdit::GetContent (BSTR *bstr)
		{
			LPSTR  pContent = GetText();
			
			int cchWide = ::MultiByteToWideChar(CP_UTF8, 0, pContent, (int)strlen(pContent), NULL, 0);
			*bstr = SysAllocStringLen(NULL,cchWide + 1);
			(*bstr)[cchWide]='\0';
			::MultiByteToWideChar(CP_UTF8, 0, pContent, (int)strlen(pContent), *bstr, cchWide + 1);

			return 1;
		}

		xbObject * XSEdit::GetInterface()
		{
			if(!m_pInterFace)
			{
				IXSEdit * pIXEdit = new IXSEdit();
				pIXEdit->m_pEdit = this;
				pIXEdit->m_pControl = pIXEdit->m_pEdit;
				m_pInterFace = (xbObject *)pIXEdit;
			}
			return this->m_pInterFace;
		}


		/////////////////////////////////////
		// @mfunc Try to load the Scintilla dll - usually named "SciLexer.dll" or "Scintilla.dll".  We try to locate the dll in 
		// the current dirtectory and along the path environment.
		// Call this function in your CWinApp derived application in the InitInstance function by calling:<nl>
		// XSEdit::LoadScintillaDll()<nl>
		// @rvalue BOOL | FALSE on error - TRUE on success
		//
		HMODULE XSEdit::LoadScintillaDll (
			LPCTSTR szDllFile) //@parm filename of the lexer dll - default "SciLexer.dll"
		{
			LPTSTR strLexer = STR_LEXERDLL;
			if (szDllFile != NULL)
				strLexer = (LPTSTR)szDllFile;
			// this call to LoadLibrary searches in:
			// 1.) current directory
			// 2.) wint
			// 3.) winnt/system32
			// 4.) path
			HMODULE hModule = ::LoadLibrary(strLexer);
			// if load fails get an extended error message 
			if (hModule == NULL)
			{
				LPVOID lpMsgBuf;
				::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &lpMsgBuf,0,NULL);
				// Write to stderr
				//TRACE (_T("%s:%s\n"), (LPCTSTR)strLexer, (LPCTSTR)lpMsgBuf);
				// Free the buffer.
				LocalFree( lpMsgBuf );
				return NULL;
			}
			return hModule;
		}
		/////////////////////////////////////
		// @mfunc Reset the Scintiall control and add new Text
		// @rvalue void | not used
		//
		void XSEdit::SetText (
			LPCSTR szText) //@parm pointer to new text
		{
			LRESULT lResult = 0;
			if (szText != NULL)
				lResult = SendMessage(this->m_hWnd,SCI_SETTEXT,0,(LPARAM)szText);
			GotoPosition(0);
			SetFocus(this->m_hWnd);
			

		}
		/////////////////////////////////////
		// @mfunc Get the text from the control
		// @rvalue void | not used
		//
		void XSEdit::GetText (
			LPSTR &strText) //@parm handle to receive text
		{
			LPSTR szText = GetText();
			if (szText != NULL)
			{
				strText = szText;
				//delete [] szText;
			}
		}
		/////////////////////////////////////
		// @mfunc Get the text from the control
		// @rvalue LPTSTR | a character string with text from the control - NULL on error - the caller has to free pointer
		//
		LPSTR XSEdit::GetText ()
		{
			long lLen = (long)SendMessage(this->m_hWnd,SCI_GETLENGTH, 0, 0) + 1;
			if (lLen > 0)
			{
				char *pReturn = new char[lLen];
				if (pReturn != NULL)
				{
					*pReturn = '\0';
					SendMessage(this->m_hWnd,SCI_GETTEXT, lLen, (LPARAM)pReturn);
					return pReturn;
				}
			}
			return NULL;
		}
		/////////////////////////////////////
		// @mfunc Try to load a new file
		// @rvalue BOOL | FALSE on error - TRUE on success
		//
		BOOL XSEdit::LoadFile (
			LPCTSTR szPath) //@parm filename of to load
		{
			// if pathname is empty do nothing
			if (szPath == NULL || *szPath == '\0')
				return TRUE;
			// try to get extension and figure out what lexer to use
			int i;
			for(i=(int)::_tcslen(szPath) -1;i>=0 && szPath[i]!='.';i--);
			if(i>=0)
				SendMessage(this->m_hWnd,SCI_SETLEXER, GetFormatFromExtension(szPath + i+1 ), 0);
			else
				SendMessage(this->m_hWnd,SCI_SETLEXER, SCLEX_CPP, 0);

			BOOL bReturn = TRUE;
			// ty to open file in sharing mode

			HANDLE hFile = NULL;
			hFile = ::CreateFile(szPath,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
			long len = 0L;
			long nTotal;
			char *szBuffer = NULL;
			// ok success - try to get length of file
			if (hFile)
			{
				len = ::GetFileSize(hFile,NULL);
				if (len > 0)
				{
					//       alloc new buffer of sie = filesize+1 for termination NULL
					szBuffer = new char[len+1];
					if (szBuffer != NULL)
					{
						 if(::ReadFile(hFile, szBuffer, len,(LPDWORD)&nTotal,NULL))
						 {
							if (nTotal > 0 && nTotal <= len)szBuffer[nTotal] = '\0';
						}else
						{
							::CloseHandle(hFile);
							bReturn = FALSE;
						}
					}
					else
					{
						::CloseHandle(hFile);
						bReturn = FALSE;
					}
				}
				::CloseHandle(hFile);
				//    set text to control
				SetText (szBuffer);
				//    tell scintilla that we have an unmodified document
				SendMessage(this->m_hWnd,SCI_SETSAVEPOINT,0 , 0);
				GotoPosition(0);

			}
			// file open error - return
			else
				bReturn = FALSE;
			// clean up
			if (szBuffer != NULL)
				delete [] szBuffer;
			return bReturn;
		}
		/////////////////////////////////////
		// @mfunc Try to save the file
		// @rvalue BOOL | FALSE on error - TRUE on success
		//
		BOOL XSEdit::SaveFile (
			LPCTSTR szPath) //@parm filename to save to
		{
			HANDLE hFile = NULL;
			hFile = ::CreateFile(szPath,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
			if (!hFile)
			{
				return FALSE;
			}
			int buflen = SendMessage(this->m_hWnd,SCI_GETLENGTH,0,0)+1; //last NULL
			long rlen = 0;
			TCHAR *pBuffer = new TCHAR[buflen];
			if (pBuffer != NULL)
			{
				SendMessage(this->m_hWnd,SCI_GETTEXT, buflen,(LPARAM)pBuffer);
				::WriteFile(hFile,pBuffer, buflen-1, (LPDWORD)&rlen,NULL);
				delete [] pBuffer;
			}
			::CloseHandle(hFile);
			return TRUE;
		}
		/////////////////////////////////////
		// @mfunc Try to find format for lexer by looking at the file extension.<nl>
		// See global arrays at top of file.
		// @rvalue int | Scintilla integer format for lexer
		//
		int XSEdit::GetFormatFromExtension (
			LPCTSTR szExtension) //@parm filename extension without dot e.g. "cpp"
		{
			int i = 0;
			TCHAR *pExtension = szExtensions[i];
			LPTSTR buf = new TCHAR[::_tcslen(szExtension) + 5];
			::_stprintf_s(buf,::_tcslen(szExtension) + 5,_T("|%s|\0"),szExtension);
			while (pExtension != NULL)
			{
				if(::_tcsstr(pExtension,szExtension))return nFormats[i];
				i++;
				pExtension = szExtensions[i];
			}
			return SCLEX_NULL;
		}
		/////////////////////////////////////
		// @mfunc Try to calculate the number of characters needed for the display of the linenumbers.
		// This function returns 3 for 10-99 lines in file, 4 for 100-999 lines and so on.
		// @rvalue int | number of characters needed to display linenumbers
		//
		int XSEdit::GetLinenumberChars ()
		{
			// try to get number of lines in control
			LRESULT lLines = SendMessage(this->m_hWnd,SCI_GETLINECOUNT, 0, 0);

			int nChars = 1;
			while (lLines > 0)
			{
				lLines = lLines / 10;
				nChars++;
			}
			return nChars; // allow an extra char for safety
		}
		/////////////////////////////////////
		// @mfunc Calculate a reasonable width for Linenumberdisplay
		// @rvalue int | number of pixels for the margin width of margin (0) 
		//
		int XSEdit::GetLinenumberWidth ()
		{
			// get number of chars needed to display highest linenumber
			int nChars = GetLinenumberChars ()+1;
			// get width of character '9' in pixels
			LRESULT lWidth = SendMessage(this->m_hWnd,SCI_TEXTWIDTH, STYLE_LINENUMBER, (LPARAM)_T("9"));
			return nChars * lWidth;
		}
		/////////////////////////////////////
		// @mfunc Set the display of line numbers on or off.
		// Scintilla uses 3 "Margin" at the left of the edit window. The Margin (0)
		// is used to display the linenumbers. If set to 0 no numbers are displayed. If
		// set to a width > 0 - linenumbers are displayed. We use a helper function to 
		// calculate the size of this margin.
		// @rvalue int | number of pixels for the margin width of margin (0) 
		//
		void XSEdit::SetDisplayLinenumbers(
			BOOL bFlag) //@parm flag if we shuld display line numbers
		{
			// if nothing changes just return
			if (GetDisplayLinenumbers() == bFlag)
				return;
			// if display is turned off we set margin 0 to 0
			if (!bFlag)
			{
				SendMessage(this->m_hWnd,SCI_SETMARGINWIDTHN, 0, 0);
			}
			// if display is turned o we set margin 0 to the calculated width
			else
			{
				int nWidth = GetLinenumberWidth() + 4;
				SendMessage(this->m_hWnd,SCI_SETMARGINWIDTHN, 0, nWidth);
			}
			m_bLinenumbers = bFlag;
		}
		/////////////////////////////////////
		// @mfunc Cut the selected text to the clipboard
		// @rvalue void | not used
		//
		void XSEdit::Cut()
		{
			SendMessage(this->m_hWnd,SCI_CUT, 0, 0);
		}
		/////////////////////////////////////
		// @mfunc Copy the selected text to the clipboard
		// @rvalue void | not used
		//
		void XSEdit::Copy()
		{
			SendMessage(this->m_hWnd,SCI_COPY, 0, 0);
		}
		/////////////////////////////////////
		// @mfunc Paste the text from the clipboard to the control
		// @rvalue void | not used
		//
		void XSEdit::Paste()
		{
			SendMessage(this->m_hWnd,SCI_PASTE, 0, 0);
		}
		/////////////////////////////////////
		// @mfunc Delete the selected text
		// @rvalue void | not used
		//
		void XSEdit::Clear()
		{
			SendMessage(this->m_hWnd,SCI_CLEAR, 0, 0);
		}
		/////////////////////////////////////
		// @mfunc Select the complete text
		// @rvalue void | not used
		//
		void XSEdit::SelectAll()
		{
			SendMessage(this->m_hWnd,SCI_SELECTALL, 0, 0);
		}
		/////////////////////////////////////
		// @mfunc Undo the last action
		// @rvalue void | not used
		//
		void XSEdit::Undo()
		{
			SendMessage(this->m_hWnd,SCI_UNDO, 0, 0);
		}
		/////////////////////////////////////
		// @mfunc Redo the last undone action
		// @rvalue void | not used
		//
		void XSEdit::Redo()
		{
			SendMessage(this->m_hWnd,SCI_REDO, 0, 0);
		}
		/////////////////////////////////////
		// @mfunc Returns a flag if we can undo the last action
		// @rvalue BOOL | TRUE if we can undo - else FALSE
		//
		BOOL XSEdit::CanUndo()
		{
			return SendMessage(this->m_hWnd,SCI_CANUNDO, 0, 0) != 0;
		}
		/////////////////////////////////////
		// @mfunc Returns a flag if we can redo the last undone action
		// @rvalue BOOL | TRUE if we can redo - else FALSE
		//
		BOOL XSEdit::CanRedo()
		{
			return SendMessage(this->m_hWnd,SCI_CANREDO, 0, 0) != 0;
		}
		/////////////////////////////////////
		// @mfunc Returns a flag if there is text in the clipboard which we can paste
		// @rvalue BOOL | TRUE if the clipboard contains text to paste - else FALSE
		//
		BOOL XSEdit::CanPaste()
		{
			return SendMessage(this->m_hWnd,SCI_CANPASTE, 0, 0) != 0;
		}
		/////////////////////////////////////
		// @mfunc Get the current line number - this the with the caret in it
		// @rvalue long | line number with the caret in it - starts with 1
		//
		long XSEdit::GetCurrentLine()
		{
			long lPos = SendMessage(this->m_hWnd,SCI_GETCURRENTPOS, 0, 0);
			return SendMessage(this->m_hWnd,SCI_LINEFROMPOSITION, lPos, 0) + 1;
		}
		/////////////////////////////////////
		// @mfunc Get the current column number = position of the caret within the line.
		// This return value my be affected by the TAB setting! Starts with 1
		// @rvalue long | current column number
		//
		long XSEdit::GetCurrentColumn()
		{
			long lPos = SendMessage(this->m_hWnd,SCI_GETCURRENTPOS, 0, 0);
			return SendMessage(this->m_hWnd,SCI_GETCOLUMN, lPos, 0) + 1;
		}
		/////////////////////////////////////
		// @mfunc Return the current character position within the file.
		// @rvalue long | current character position
		//
		long XSEdit::GetCurrentPosition()
		{
			return SendMessage(this->m_hWnd,SCI_GETCURRENTPOS, 0, 0);
		}
		/////////////////////////////////////
		// @mfunc Return the current style at the caret
		// @rvalue int | the current style index (0...127)
		//
		int XSEdit::GetCurrentStyle()
		{
			long lPos = SendMessage(this->m_hWnd,SCI_GETCURRENTPOS, 0, 0);
			return SendMessage(this->m_hWnd,SCI_GETSTYLEAT, lPos, 0);
		}
		/////////////////////////////////////
		// @mfunc Return the current folding level at the caret line
		// @rvalue int | the current folding level (0...n)
		//
		int XSEdit::GetCurrentFoldinglevel()
		{
			long lLine = GetCurrentLine();
			int level = (SendMessage(this->m_hWnd,SCI_GETFOLDLEVEL, lLine, 0)) & SC_FOLDLEVELNUMBERMASK;
			return level-1024;
		}
		/////////////////////////////////////
		// @mfunc set the fontname e.g. "Arial"
		// @rvalue void | not used
		//
		void XSEdit::SetFontname(int nStyle, LPCTSTR szFontname)
		{
			SendMessage(this->m_hWnd,SCI_STYLESETFONT, nStyle, (LPARAM)szFontname);
		}
		/////////////////////////////////////
		// @mfunc Set the font height in points
		// @rvalue void | not used
		//
		void XSEdit::SetFontheight(int nStyle, int nHeight)
		{
			SendMessage(this->m_hWnd,SCI_STYLESETSIZE, nStyle, (long)nHeight);
		}
		/////////////////////////////////////
		// @mfunc Set the foreground color
		// @rvalue void | not used
		//
		void XSEdit::SetForeground(int nStyle, COLORREF crForeground)
		{
			SendMessage(this->m_hWnd,SCI_STYLESETFORE, nStyle, (long)crForeground);
		}
		/////////////////////////////////////
		// @mfunc set the backgroundcolor
		// @rvalue void | not used
		//
		void XSEdit::SetBackground(int nStyle, COLORREF crBackground)
		{
			SendMessage(this->m_hWnd,SCI_STYLESETBACK, nStyle, (long)crBackground);
		}
		/////////////////////////////////////
		// @mfunc set given style to bold
		// @rvalue void | not used
		//
		void XSEdit::SetBold(int nStyle, BOOL bBold)
		{
			SendMessage(this->m_hWnd,SCI_STYLESETBOLD, nStyle, (long)bBold);
		}
		/////////////////////////////////////
		// @mfunc set given style to bold
		// @rvalue void | not used
		//
		void XSEdit::SetItalic(int nStyle, BOOL bItalic)
		{
			SendMessage(this->m_hWnd,SCI_STYLESETITALIC, nStyle, (long)bItalic);
		}
		/////////////////////////////////////
		// @mfunc set given style to bold
		// @rvalue void | not used
		//
		void XSEdit::SetUnderline(int nStyle, BOOL bUnderline)
		{
			SendMessage(this->m_hWnd,SCI_STYLESETUNDERLINE, nStyle, (long)bUnderline);
		}
		/////////////////////////////////////
		// @mfunc Return flag if we are in overstrike mode
		// @rvalue BOOL | TRUE if we are in overstrike mode - else FALSE
		//
		BOOL XSEdit::GetOverstrike()
		{
			return SendMessage(this->m_hWnd,SCI_GETOVERTYPE , 0, 0);
		}
		/////////////////////////////////////
		// @mfunc set set overstrike mode (TRUE) or insert mode (FALSE)
		// @rvalue void | not used
		//
		void XSEdit::SetOverstrike(BOOL bOverstrike)
		{
			SendMessage(this->m_hWnd,SCI_SETOVERTYPE, bOverstrike, 0);
		}
		/////////////////////////////////////
		// @mfunc Toggle the display of the selection bookmark margin
		// @rvalue void | not used
		//
		void XSEdit::SetDisplaySelection(BOOL bFlag)
		{
			m_bSelection = bFlag;
			if (bFlag)
				SendMessage(this->m_hWnd,SCI_SETMARGINWIDTHN, 1, 16);
			else
				SendMessage(this->m_hWnd,SCI_SETMARGINWIDTHN, 1, 0);
		}
		/////////////////////////////////////
		// @mfunc Toggle the display of the folding margin
		// @rvalue void | not used
		//
		void XSEdit::SetDisplayFolding(BOOL bFlag)
		{
			m_bFolding = bFlag;
			if (bFlag)
				SendMessage(this->m_hWnd,SCI_SETMARGINWIDTHN, 2, 16);
			else
				SendMessage(this->m_hWnd,SCI_SETMARGINWIDTHN, 2, 0);
		}
		/////////////////////////////////////
		// @mfunc init the view with reasonable defaults
		// @rvalue void | not used
		//
		void XSEdit::Init()
		{
			// clear all text styles
			SendMessage(this->m_hWnd,SCI_CLEARDOCUMENTSTYLE, 0, 0);
			// set the number of styling bits to 7 - the asp/html views need a lot of styling - default is 5
			// If you leave the default you will see twiggle lines instead of ASP code
			SendMessage(this->m_hWnd,SCI_SETSTYLEBITS, 7, 0);
			// set the display for indetation guides to on - this displays virtical dotted lines from the beginning of 
			// a code block to the end of the block
			SendMessage(this->m_hWnd,SCI_SETINDENTATIONGUIDES, TRUE, 0);
			// set tabwidth to 3
			SendMessage(this->m_hWnd,SCI_SETTABWIDTH,3,0);
			// set indention to 3
			SendMessage(this->m_hWnd,SCI_SETINDENT,3,0);
			// set the caret blinking time to 400 milliseconds
			SendMessage(this->m_hWnd,SCI_SETCARETPERIOD,400,0);
			// source folding section
			// tell the lexer that we want folding information - the lexer supplies "folding levels"
/*			SendMessage(this->m_hWnd,SCI_SETPROPERTY, (WPARAM)_T("fold"), (LPARAM)_T("1"));
			SendMessage(this->m_hWnd,SCI_SETPROPERTY, (WPARAM)_T("fold.html"), (LPARAM)_T("1"));
			SendMessage(this->m_hWnd,SCI_SETPROPERTY, (WPARAM)_T("fold.html.preprocessor"), (LPARAM)_T("1"));
			SendMessage(this->m_hWnd,SCI_SETPROPERTY, (WPARAM)_T("fold.comment"), (LPARAM)_T("1"));
			SendMessage(this->m_hWnd,SCI_SETPROPERTY, (WPARAM)_T("fold.at.else"), (LPARAM)_T("1"));
			SendMessage(this->m_hWnd,SCI_SETPROPERTY, (WPARAM)_T("fold.flags"), (LPARAM)_T("1"));
			SendMessage(this->m_hWnd,SCI_SETPROPERTY, (WPARAM)_T("fold.preprocessor"), (LPARAM)_T("1"));
			SendMessage(this->m_hWnd,SCI_SETPROPERTY, (WPARAM)_T("styling.within.preprocessor"), (LPARAM)_T("1"));
			SendMessage(this->m_hWnd,SCI_SETPROPERTY, (WPARAM)_T("asp.default.language"), (LPARAM)_T("1"));*/

			SendMessage(this->m_hWnd,SCI_SETPROPERTY, (WPARAM)("fold"), (LPARAM)("1"));
			SendMessage(this->m_hWnd,SCI_SETPROPERTY, (WPARAM)("fold.html"), (LPARAM)("1"));
			SendMessage(this->m_hWnd,SCI_SETPROPERTY, (WPARAM)("fold.html.preprocessor"), (LPARAM)("1"));
			SendMessage(this->m_hWnd,SCI_SETPROPERTY, (WPARAM)("fold.comment"), (LPARAM)("1"));
			SendMessage(this->m_hWnd,SCI_SETPROPERTY, (WPARAM)("fold.at.else"), (LPARAM)("1"));
			SendMessage(this->m_hWnd,SCI_SETPROPERTY, (WPARAM)("fold.flags"), (LPARAM)("1"));
			SendMessage(this->m_hWnd,SCI_SETPROPERTY, (WPARAM)("fold.preprocessor"), (LPARAM)("1"));
			SendMessage(this->m_hWnd,SCI_SETPROPERTY, (WPARAM)("styling.within.preprocessor"), (LPARAM)("1"));
			SendMessage(this->m_hWnd,SCI_SETPROPERTY, (WPARAM)("asp.default.language"), (LPARAM)("1"));

			// Tell scintilla to draw folding lines UNDER the folded line
			SendMessage(this->m_hWnd,SCI_SETFOLDFLAGS, 16,0);
			// Set margin 2 = folding margin to display folding symbols
			SendMessage(this->m_hWnd,SCI_SETMARGINMASKN, 2, SC_MASK_FOLDERS);
			// allow notifications for folding actions
			SendMessage(this->m_hWnd,SCI_SETMODEVENTMASK, SC_MOD_INSERTTEXT|SC_MOD_DELETETEXT, 0);
			//   SendMessage(this->m_hWnd,SCI_SETMODEVENTMASK, SC_MOD_CHANGEFOLD|SC_MOD_INSERTTEXT|SC_MOD_DELETETEXT, 0);
			// make the folding margin sensitive to folding events = if you click into the margin you get a notification event
			SendMessage(this->m_hWnd,SCI_SETMARGINSENSITIVEN, 2, TRUE);
			SendMessage(this->m_hWnd,SCI_SETMARGINSENSITIVEN, 0, TRUE);
			// define a set of markers to displa folding symbols
			SendMessage(this->m_hWnd,SCI_MARKERDEFINE, SC_MARKNUM_FOLDEROPEN, SC_MARK_MINUS);
			SendMessage(this->m_hWnd,SCI_MARKERDEFINE, SC_MARKNUM_FOLDER, SC_MARK_PLUS);
			SendMessage(this->m_hWnd,SCI_MARKERDEFINE, SC_MARKNUM_FOLDERSUB, SC_MARK_EMPTY);
			SendMessage(this->m_hWnd,SCI_MARKERDEFINE, SC_MARKNUM_FOLDERTAIL, SC_MARK_EMPTY);
			SendMessage(this->m_hWnd,SCI_MARKERDEFINE, SC_MARKNUM_FOLDEREND, SC_MARK_EMPTY);
			SendMessage(this->m_hWnd,SCI_MARKERDEFINE, SC_MARKNUM_FOLDEROPENMID, SC_MARK_EMPTY);
			SendMessage(this->m_hWnd,SCI_MARKERDEFINE, SC_MARKNUM_FOLDERMIDTAIL, SC_MARK_EMPTY);
			// set the forground color for some styles
			SendMessage(this->m_hWnd,SCI_STYLESETFORE, 0, RGB(0,0,0));
			SendMessage(this->m_hWnd,SCI_STYLESETFORE, 2, RGB(0,64,0));
			SendMessage(this->m_hWnd,SCI_STYLESETFORE, 5, RGB(0,0,255));
			SendMessage(this->m_hWnd,SCI_STYLESETFORE, 6, RGB(200,20,0));
			SendMessage(this->m_hWnd,SCI_STYLESETFORE, 9, RGB(0,0,255));
			SendMessage(this->m_hWnd,SCI_STYLESETFORE, 10, RGB(255,0,64));
			SendMessage(this->m_hWnd,SCI_STYLESETFORE, 11, RGB(0,0,0));
			// set the backgroundcolor of brace highlights
			SendMessage(this->m_hWnd,SCI_STYLESETBACK, STYLE_BRACELIGHT, RGB(0,255,0));
			// set end of line mode to CRLF
			SendMessage(this->m_hWnd,SCI_CONVERTEOLS, 2, 0);
			SendMessage(this->m_hWnd,SCI_SETEOLMODE, 2, 0);
			//   SendMessage(this->m_hWnd,SCI_SETVIEWEOL, TRUE, 0);
			// set markersymbol for marker type 0 - bookmark
			SendMessage(this->m_hWnd,SCI_MARKERDEFINE, 0, SC_MARK_CIRCLE);
			// display all margins
			SetDisplayLinenumbers(TRUE);
			SetDisplayFolding(TRUE);
			SetDisplaySelection(TRUE);

		}
		/////////////////////////////////////
		// @mfunc Update UI and do brace matching
		// @rvalue void | not used
		//
		void XSEdit::UpdateUI()
		{
			// do brace matching
			long lStart = SendMessage(this->m_hWnd,SCI_GETCURRENTPOS, 0, 0);
			long lEnd = SendMessage(this->m_hWnd,SCI_BRACEMATCH, lStart-1, 0);
			// if there is a matching brace highlight it
			if (lEnd >= 0)
				SendMessage(this->m_hWnd,SCI_BRACEHIGHLIGHT, lStart-1, lEnd);
			// if there is NO matching brace erase old highlight
			else
				SendMessage(this->m_hWnd,SCI_BRACEHIGHLIGHT, -1, -1);
		}
		/////////////////////////////////////
		// @mfunc Do default folding for given margin and position - usually called from notification handler
		// @rvalue void | not used
		//
		void XSEdit::DoDefaultFolding(
			int nMargin,      //@parm maring we want to handle - normally (2)
			long lPosition)   //@parm character position where user clicked margin
		{
			// simply toggle fold
			if (nMargin == 2)
			{
				long lLine = SendMessage(this->m_hWnd,SCI_LINEFROMPOSITION, lPosition, 0);
				SendMessage(this->m_hWnd,SCI_TOGGLEFOLD, lLine, 0);
			}
		}
		/////////////////////////////////////
		// @mfunc Do default folding for given margin and position - usually called from notification handler
		// @rvalue void | not used
		//
		void XSEdit::Refresh()
		{
			SendMessage(this->m_hWnd,SCI_COLOURISE, 0, -1);
		}
		/////////////////////////////////////
		// @mfunc Add a bookmark at given line
		// @rvalue void | not used
		//
		void XSEdit::AddBookmark(
			long lLine) //@parm line where to add bookmark - lines start at 1
		{
			SendMessage(this->m_hWnd,SCI_MARKERADD, lLine-1, 0);
		}
		/////////////////////////////////////
		// @mfunc Delete a bookmark at given line
		// @rvalue void | not used
		//
		void XSEdit::DeleteBookmark(
			long lLine) //@parm line where to delete bookmark - lines start at 1
		{
			if (HasBookmark(lLine))
			{
				SendMessage(this->m_hWnd,SCI_MARKERDELETE, lLine-1, 0);
			}
		}
		/////////////////////////////////////
		// @mfunc Check if given line has a bookmark
		// @rvalue BOOL | TRUE if given line has a bookmark - else FALSE
		//
		BOOL XSEdit::HasBookmark(
			long lLine) //@parm line where to add bookmark - lines start at 1
		{
			int n = SendMessage(this->m_hWnd,SCI_MARKERGET, lLine-1, 0);
			// check mask for markerbit 0
			if (n & 0x1)
				return TRUE;
			return FALSE;
		}
		/////////////////////////////////////
		// @mfunc Find next bookmark from current line
		// @rvalue void | not used
		//
		void XSEdit::FindNextBookmark()
		{
			long lLine = SendMessage(this->m_hWnd,SCI_MARKERNEXT, GetCurrentLine(), 1);
			if (lLine >= 0)
				SendMessage(this->m_hWnd,SCI_GOTOLINE, lLine,0);
		}
		/////////////////////////////////////
		// @mfunc Find previous bookmark from current line
		// @rvalue void | not used
		//
		void XSEdit::FindPreviousBookmark()
		{
			long lLine = SendMessage(this->m_hWnd,SCI_MARKERPREVIOUS, GetCurrentLine()-2, 1);
			if (lLine >= 0)
				SendMessage(this->m_hWnd,SCI_GOTOLINE, lLine,0);
		}
		/////////////////////////////////////
		// @mfunc Goto givven character position
		// @rvalue void | not used
		//
		void XSEdit::GotoPosition(
			long lPos) //@parm new character position
		{
			SendMessage(this->m_hWnd,SCI_GOTOPOS, lPos, 0);
		}
		/////////////////////////////////////
		// @mfunc Goto givven line position
		// @rvalue void | not used
		//
		void XSEdit::GotoLine(
			long lLine) //@parm new line - lines start at 1
		{
			SendMessage(this->m_hWnd,SCI_GOTOLINE, lLine-1, 0);
		}
		/////////////////////////////////////
		// @mfunc Search forward for a given string and select it if found. You may use regular expressions on the text.
		// @rvalue BOOL | TRUE if text is ffound else FALSE
		//
		BOOL XSEdit::SearchForward(
			LPSTR szText) //@parm text to search
		{
			if (szText == NULL)
				return FALSE;
			long lPos = GetCurrentPosition();
			TextToFind tf;
			tf.lpstrText = szText;
			tf.chrg.cpMin = lPos+1;
			tf.chrg.cpMax = SendMessage(this->m_hWnd,SCI_GETLENGTH, 0, 0);
			lPos = SendMessage(this->m_hWnd,SCI_FINDTEXT, m_nSearchflags, (LPARAM)&tf);
			if (lPos >= 0)
			{
				SetFocus(this->m_hWnd);
				GotoPosition(lPos);
				SendMessage(this->m_hWnd,SCI_SETSEL, tf.chrgText.cpMin, tf.chrgText.cpMax);
				SendMessage(this->m_hWnd,SCI_FINDTEXT, m_nSearchflags, (LPARAM)&tf);
				return TRUE;
			}
			return FALSE;
		}
		/////////////////////////////////////
		// @mfunc Search backward for a given string and select it if found. You may use regular expressions on the text.
		// @rvalue BOOL | TRUE if text is ffound else FALSE
		//
		BOOL XSEdit::SearchBackward(
			LPSTR szText) //@parm text to search
		{
			if (szText == NULL)
				return FALSE;
			long lPos = GetCurrentPosition();
			long lMinSel = GetSelectionStart();
			TextToFind tf;
			tf.lpstrText = szText;
			if (lMinSel >= 0)
				tf.chrg.cpMin = lMinSel-1;
			else
				tf.chrg.cpMin = lPos-1;
			tf.chrg.cpMax = 0;
			lPos = SendMessage(this->m_hWnd,SCI_FINDTEXT, m_nSearchflags, (LPARAM)&tf);
			if (lPos >= 0)
			{
				SetFocus(this->m_hWnd);
				GotoPosition(lPos);
				SendMessage(this->m_hWnd,SCI_SETSEL, tf.chrgText.cpMin, tf.chrgText.cpMax);
				SendMessage(this->m_hWnd,SCI_FINDTEXT, m_nSearchflags, (LPARAM)&tf);
				return TRUE;
			}
			return FALSE;
		}
		/////////////////////////////////////
		// @mfunc Replace a text with a new text. You can use regular expression with tagging on the replacement text.
		// @rvalue void | not used
		//
		void XSEdit::ReplaceSearchedText(
			LPCSTR szText) //@parm new text
		{
			if (szText == NULL)
				return;
			SendMessage(this->m_hWnd,SCI_TARGETFROMSELECTION, 0, 0);
			if (m_nSearchflags & SCFIND_REGEXP)
				SendMessage(this->m_hWnd,SCI_REPLACETARGETRE, strlen(szText), (LPARAM)szText);
			else
				SendMessage(this->m_hWnd,SCI_REPLACETARGET, strlen(szText), (LPARAM)szText);
		}
		/////////////////////////////////////
		// @mfunc Set lexer format - see scintilla.h
		// @rvalue void | not used
		//
		void XSEdit::SetLexer(
			int nLexer) //@parm lexer code e.g. 
		{
			SendMessage(this->m_hWnd,SCI_SETLEXER, nLexer, 0);
		}
		/////////////////////////////////////
		// @mfunc Get start of selection (anchor) as character position
		// @rvalue long | character position of selection start (anchor) - -1 on error
		//
		long XSEdit::GetSelectionStart() 
		{
			return SendMessage(this->m_hWnd,SCI_GETSELECTIONSTART, 0, 0); 
		}
		/////////////////////////////////////
		// @mfunc Get end of selection as character position
		// @rvalue long | character position of selection end - -1 on error
		//
		long XSEdit::GetSelectionEnd()
		{
			return SendMessage(this->m_hWnd,SCI_GETSELECTIONEND, 0, 0);
		}
		/////////////////////////////////////
		// @mfunc Get selected string
		// @rvalue CString | string with currentliy selected text
		//
		LPTSTR XSEdit::GetSelectedText()
		{
			long lLen = (GetSelectionEnd() - GetSelectionStart()) + 1;
			if (lLen > 0)
			{
				TCHAR *p = new TCHAR[lLen+1];
				if (p != NULL)
				{
					*p = '\0';
					SendMessage(this->m_hWnd,SCI_GETSELTEXT, 0, (LPARAM)p);
					return p;
				}
			}
			return _T("");
		}
		/////////////////////////////////////
		// @mfunc Replace a text in a selection or in the complete file multiple times
		// @rvalue int | number of replacements
		//
		int XSEdit::ReplaceAll(
			LPSTR szFind, 
			LPSTR szReplace, 
			BOOL bUseSelection)
		{
			int nCount = 0;
			// different branches for replace in selection or total file
			if (bUseSelection)
			{
				//    get starting selection range
				long lLen = 0;
				long lStart = GetSelectionStart();
				long lEnd = GetSelectionEnd();
				//    set target to selection
				SendMessage(this->m_hWnd,SCI_SETTARGETSTART, lStart,0);
				SendMessage(this->m_hWnd,SCI_SETTARGETEND, lEnd,0);
				//    try to find text in target for the first time
				long lPos = SendMessage(this->m_hWnd,SCI_SEARCHINTARGET, strlen(szFind), (LPARAM)szFind);
				//    loop over selection until end of selection reached - moving the target start each time
				while (lPos < lEnd && lPos >= 0)
				{
					if (m_nSearchflags & SCFIND_REGEXP) // check for regular expression flag
						lLen = SendMessage(this->m_hWnd,SCI_REPLACETARGETRE, strlen(szReplace), (LPARAM)szReplace);
					else
						lLen = SendMessage(this->m_hWnd,SCI_REPLACETARGET, strlen(szReplace), (LPARAM)szReplace);
					//       the end of the selection was changed - recalc the end
					lEnd = GetSelectionEnd();
					//       move start of target behind last change and end of target to new end of selection
					SendMessage(this->m_hWnd,SCI_SETTARGETSTART, lPos+lLen,0);
					SendMessage(this->m_hWnd,SCI_SETTARGETEND, lEnd,0);
					//       find again - if nothing found loop exits
					lPos = SendMessage(this->m_hWnd,SCI_SEARCHINTARGET, strlen(szFind), (LPARAM)szFind);
					nCount++;
				}
			}
			else
			{
				//    start with first and last char in buffer
				long lLen = 0;
				long lStart = 0;
				long lEnd = SendMessage(this->m_hWnd,SCI_GETTEXTLENGTH, 0, 0);
				//    set target to selection
				SendMessage(this->m_hWnd,SCI_SETTARGETSTART, lStart, 0);
				SendMessage(this->m_hWnd,SCI_SETTARGETEND, lEnd, 0);
				//    try to find text in target for the first time
				long lPos = SendMessage(this->m_hWnd,SCI_SEARCHINTARGET, strlen(szFind), (LPARAM)szFind);
				//    loop over selection until end of selection reached - moving the target start each time
				while (lPos < lEnd && lPos >= 0)
				{
					if (m_nSearchflags & SCFIND_REGEXP) // check for regular expression flag
						lLen = SendMessage(this->m_hWnd,SCI_REPLACETARGETRE, strlen(szReplace), (LPARAM)szReplace);
					else
						lLen = SendMessage(this->m_hWnd,SCI_REPLACETARGET, strlen(szReplace), (LPARAM)szReplace);
					//       the end of the selection was changed - recalc the end
					lEnd = SendMessage(this->m_hWnd,SCI_GETTEXTLENGTH, 0, 0);;
					//       move start of target behind last change and end of target to new end of buffer
					SendMessage(this->m_hWnd,SCI_SETTARGETSTART, lPos+lLen,0);
					SendMessage(this->m_hWnd,SCI_SETTARGETEND, lEnd,0);
					//       find again - if nothing found loop exits
					lPos = SendMessage(this->m_hWnd,SCI_SEARCHINTARGET, strlen(szFind), (LPARAM)szFind);
					nCount++;
				}
			}
			return nCount;
		}

		int XSEdit::AddExtent(void * pOwner,XSEdit *pEdit,bool (* pfOnChar)(void * pOwner,XSEdit *pEdit,char ch))
		{
			MyExtension* ext = new MyExtension();
			ext->m_fnOnChar = pfOnChar;
			ext->m_pOwner = pOwner;
			ext->m_pEdit = pEdit;

			((MultiplexExtension *)((SciTEWin*)m_pSciTEWin)->extender)->RegisterExtension(*ext);

			return 1;
		}

		int XSEdit::AddExtent(void * pOwner,XSEdit *pEdit,
			bool (* pfOnChar)(void * pOwner,XSEdit *pEdit,char ch),
			bool (* pfOnDwell)(void * pOwner,XSEdit *pEdit,int pos ,const char * word)
			)
		{
			MyExtension* ext = new MyExtension();
			ext->m_fnOnChar = pfOnChar;
			ext->m_fnOnDwell = pfOnDwell;
			ext->m_pOwner = pOwner;
			ext->m_pEdit = pEdit;

			((MultiplexExtension *)((SciTEWin*)m_pSciTEWin)->extender)->RegisterExtension(*ext);

			return 1;
		}

	}
}



