#define WM_NULL                         0x0000
#define WM_CREATE                       0x0001
#define WM_DESTROY                      0x0002
#define WM_MOVE                         0x0003
#define WM_SIZE                         0x0005

#define WM_ACTIVATE                     0x0006

#define WM_SETFOCUS                     0x0007
#define WM_KILLFOCUS                    0x0008
#define WM_ENABLE                       0x000A
#define WM_SETREDRAW                    0x000B
#define WM_SETTEXT                      0x000C
#define WM_GETTEXT                      0x000D
#define WM_GETTEXTLENGTH                0x000E
#define WM_PAINT                        0x000F
#define WM_CLOSE                        0x0010

#define WM_QUERYENDSESSION              0x0011
#define WM_QUERYOPEN                    0x0013
#define WM_ENDSESSION                   0x0016

#define WM_QUIT                         0x0012
#define WM_ERASEBKGND                   0x0014
#define WM_SYSCOLORCHANGE               0x0015
#define WM_SHOWWINDOW                   0x0018
#define WM_WININICHANGE                 0x001A

#define WM_SETTINGCHANGE                WM_WININICHANGE



#define WM_DEVMODECHANGE                0x001B
#define WM_ACTIVATEAPP                  0x001C
#define WM_FONTCHANGE                   0x001D
#define WM_TIMECHANGE                   0x001E
#define WM_CANCELMODE                   0x001F
#define WM_SETCURSOR                    0x0020
#define WM_MOUSEACTIVATE                0x0021
#define WM_CHILDACTIVATE                0x0022
#define WM_QUEUESYNC                    0x0023

#define WM_GETMINMAXINFO                0x0024

#define WM_PAINTICON                    0x0026
#define WM_ICONERASEBKGND               0x0027
#define WM_NEXTDLGCTL                   0x0028
#define WM_SPOOLERSTATUS                0x002A
#define WM_DRAWITEM                     0x002B
#define WM_MEASUREITEM                  0x002C
#define WM_DELETEITEM                   0x002D
#define WM_VKEYTOITEM                   0x002E
#define WM_CHARTOITEM                   0x002F
#define WM_SETFONT                      0x0030
#define WM_GETFONT                      0x0031
#define WM_SETHOTKEY                    0x0032
#define WM_GETHOTKEY                    0x0033
#define WM_QUERYDRAGICON                0x0037
#define WM_COMPAREITEM                  0x0039

#define WM_GETOBJECT                    0x003D

#define WM_COMPACTING                   0x0041
#define WM_COMMNOTIFY                   0x0044  
#define WM_WINDOWPOSCHANGING            0x0046
#define WM_WINDOWPOSCHANGED             0x0047

#define WM_POWER                        0x0048

#define WM_COPYDATA                     0x004A
#define WM_CANCELJOURNAL                0x004B

#define WM_NOTIFY                       0x004E
#define WM_INPUTLANGCHANGEREQUEST       0x0050
#define WM_INPUTLANGCHANGE              0x0051
#define WM_TCARD                        0x0052
#define WM_HELP                         0x0053
#define WM_USERCHANGED                  0x0054
#define WM_NOTIFYFORMAT                 0x0055

#define WM_CONTEXTMENU                  0x007B
#define WM_STYLECHANGING                0x007C
#define WM_STYLECHANGED                 0x007D
#define WM_DISPLAYCHANGE                0x007E
#define WM_GETICON                      0x007F
#define WM_SETICON                      0x0080

#define WM_NCCREATE                     0x0081
#define WM_NCDESTROY                    0x0082
#define WM_NCCALCSIZE                   0x0083
#define WM_NCHITTEST                    0x0084
#define WM_NCPAINT                      0x0085
#define WM_NCACTIVATE                   0x0086
#define WM_GETDLGCODE                   0x0087

#define WM_SYNCPAINT                    0x0088

#define WM_NCMOUSEMOVE                  0x00A0
#define WM_NCLBUTTONDOWN                0x00A1
#define WM_NCLBUTTONUP                  0x00A2
#define WM_NCLBUTTONDBLCLK              0x00A3
#define WM_NCRBUTTONDOWN                0x00A4
#define WM_NCRBUTTONUP                  0x00A5
#define WM_NCRBUTTONDBLCLK              0x00A6
#define WM_NCMBUTTONDOWN                0x00A7
#define WM_NCMBUTTONUP                  0x00A8
#define WM_NCMBUTTONDBLCLK              0x00A9

#define WM_NCXBUTTONDOWN                0x00AB
#define WM_NCXBUTTONUP                  0x00AC
#define WM_NCXBUTTONDBLCLK              0x00AD
#define WM_INPUT                        0x00FF

#define WM_KEYFIRST                     0x0100
#define WM_KEYDOWN                      0x0100
#define WM_KEYUP                        0x0101
#define WM_CHAR                         0x0102
#define WM_DEADCHAR                     0x0103
#define WM_SYSKEYDOWN                   0x0104
#define WM_SYSKEYUP                     0x0105
#define WM_SYSCHAR                      0x0106
#define WM_SYSDEADCHAR                  0x0107

#define WM_UNICHAR                      0x0109
#define WM_KEYLAST                      0x0109
#define UNICODE_NOCHAR                  0xFFFF


#define WM_IME_STARTCOMPOSITION         0x010D
#define WM_IME_ENDCOMPOSITION           0x010E
#define WM_IME_COMPOSITION              0x010F
#define WM_IME_KEYLAST                  0x010F


#define WM_INITDIALOG                   0x0110
#define WM_COMMAND                      0x0111
#define WM_SYSCOMMAND                   0x0112
#define WM_TIMER                        0x0113
#define WM_HSCROLL                      0x0114
#define WM_VSCROLL                      0x0115
#define WM_INITMENU                     0x0116
#define WM_INITMENUPOPUP                0x0117
#define WM_MENUSELECT                   0x011F
#define WM_MENUCHAR                     0x0120
#define WM_ENTERIDLE                    0x0121

#define WM_MENURBUTTONUP                0x0122
#define WM_MENUDRAG                     0x0123
#define WM_MENUGETOBJECT                0x0124
#define WM_UNINITMENUPOPUP              0x0125
#define WM_MENUCOMMAND                  0x0126

#define WM_CHANGEUISTATE                0x0127
#define WM_UPDATEUISTATE                0x0128
#define WM_QUERYUISTATE                 0x0129


#define WM_CTLCOLORMSGBOX               0x0132
#define WM_CTLCOLOREDIT                 0x0133
#define WM_CTLCOLORLISTBOX              0x0134
#define WM_CTLCOLORBTN                  0x0135
#define WM_CTLCOLORDLG                  0x0136
#define WM_CTLCOLORSCROLLBAR            0x0137
#define WM_CTLCOLORSTATIC               0x0138
#define MN_GETHMENU                     0x01E1

#define WM_MOUSEFIRST                   0x0200
#define WM_MOUSEMOVE                    0x0200
#define WM_LBUTTONDOWN                  0x0201
#define WM_LBUTTONUP                    0x0202
#define WM_LBUTTONDBLCLK                0x0203
#define WM_RBUTTONDOWN                  0x0204
#define WM_RBUTTONUP                    0x0205
#define WM_RBUTTONDBLCLK                0x0206
#define WM_MBUTTONDOWN                  0x0207
#define WM_MBUTTONUP                    0x0208
#define WM_MBUTTONDBLCLK                0x0209
#define WM_MOUSEWHEEL                   0x020A
#define WM_XBUTTONDOWN                  0x020B
#define WM_XBUTTONUP                    0x020C
#define WM_XBUTTONDBLCLK                0x020D
#define WM_MOUSELAST                    0x020D




#define WM_PARENTNOTIFY                 0x0210
#define WM_ENTERMENULOOP                0x0211
#define WM_EXITMENULOOP                 0x0212


#define WM_NEXTMENU                     0x0213
#define WM_SIZING                       0x0214
#define WM_CAPTURECHANGED               0x0215
#define WM_MOVING                       0x0216
#define WM_POWERBROADCAST               0x0218
#define WM_DEVICECHANGE                 0x0219


#define WM_MDICREATE                    0x0220
#define WM_MDIDESTROY                   0x0221
#define WM_MDIACTIVATE                  0x0222
#define WM_MDIRESTORE                   0x0223
#define WM_MDINEXT                      0x0224
#define WM_MDIMAXIMIZE                  0x0225
#define WM_MDITILE                      0x0226
#define WM_MDICASCADE                   0x0227
#define WM_MDIICONARRANGE               0x0228
#define WM_MDIGETACTIVE                 0x0229


#define WM_MDISETMENU                   0x0230
#define WM_ENTERSIZEMOVE                0x0231
#define WM_EXITSIZEMOVE                 0x0232
#define WM_DROPFILES                    0x0233
#define WM_MDIREFRESHMENU               0x0234



#define WM_IME_SETCONTEXT               0x0281
#define WM_IME_NOTIFY                   0x0282
#define WM_IME_CONTROL                  0x0283
#define WM_IME_COMPOSITIONFULL          0x0284
#define WM_IME_SELECT                   0x0285
#define WM_IME_CHAR                     0x0286

#define WM_IME_REQUEST                  0x0288

#define WM_IME_KEYDOWN                  0x0290
#define WM_IME_KEYUP                    0x0291


#define WM_MOUSEHOVER                   0x02A1
#define WM_MOUSELEAVE                   0x02A3

#define WM_NCMOUSEHOVER                 0x02A0
#define WM_NCMOUSELEAVE                 0x02A2

#define WM_WTSSESSION_CHANGE            0x02B1

#define WM_TABLET_FIRST                 0x02c0
#define WM_TABLET_LAST                  0x02df

#define WM_CUT                          0x0300
#define WM_COPY                         0x0301
#define WM_PASTE                        0x0302
#define WM_CLEAR                        0x0303
#define WM_UNDO                         0x0304
#define WM_RENDERFORMAT                 0x0305
#define WM_RENDERALLFORMATS             0x0306
#define WM_DESTROYCLIPBOARD             0x0307
#define WM_DRAWCLIPBOARD                0x0308
#define WM_PAINTCLIPBOARD               0x0309
#define WM_VSCROLLCLIPBOARD             0x030A
#define WM_SIZECLIPBOARD                0x030B
#define WM_ASKCBFORMATNAME              0x030C
#define WM_CHANGECBCHAIN                0x030D
#define WM_HSCROLLCLIPBOARD             0x030E
#define WM_QUERYNEWPALETTE              0x030F
#define WM_PALETTEISCHANGING            0x0310
#define WM_PALETTECHANGED               0x0311
#define WM_HOTKEY                       0x0312


#define WM_PRINT                        0x0317
#define WM_PRINTCLIENT                  0x0318

#define WM_APPCOMMAND                   0x0319

#define WM_THEMECHANGED                 0x031A



#define WM_HANDHELDFIRST                0x0358
#define WM_HANDHELDLAST                 0x035F

#define WM_AFXFIRST                     0x0360
#define WM_AFXLAST                      0x037F

#define WM_PENWINFIRST                  0x0380
#define WM_PENWINLAST                   0x038F


#define WM_APP                          0x8000
#define WM_USER                         0x0400


#define NM_FIRST                (0U-  0U)       // generic to all controls
#define NM_LAST                 (0U- 99U)

//====== Generic WM_NOTIFY notification codes =================================
#define LVN_FIRST               (0U-100U)       // listview
#define LVN_LAST                (0U-199U)

// Property sheet reserved      (0U-200U) -  (0U-299U) - see prsht.h
#define PSN_FIRST               (0U-200U)
#define PSN_LAST                (0U-299U)

#define HDN_FIRST               (0U-300U)       // header
#define HDN_LAST                (0U-399U)

#define TVN_FIRST               (0U-400U)       // treeview
#define TVN_LAST                (0U-499U)

#define TTN_FIRST               (0U-520U)       // tooltips
#define TTN_LAST                (0U-549U)

#define TCN_FIRST               (0U-550U)       // tab control
#define TCN_LAST                (0U-580U)

// Shell reserved               (0U-580U) -  (0U-589U)

#define CDN_FIRST               (0U-601U)       // common dialog (new)
#define CDN_LAST                (0U-699U)

#define TBN_FIRST               (0U-700U)       // toolbar
#define TBN_LAST                (0U-720U)

#define UDN_FIRST               (0U-721)        // updown
#define UDN_LAST                (0U-740)
#if (_WIN32_IE >= 0x0300)
#define MCN_FIRST               (0U-750U)       // monthcal
#define MCN_LAST                (0U-759U)

#define DTN_FIRST               (0U-760U)       // datetimepick
#define DTN_LAST                (0U-799U)

#define CBEN_FIRST              (0U-800U)       // combo box ex
#define CBEN_LAST               (0U-830U)

#define RBN_FIRST               (0U-831U)       // rebar
#define RBN_LAST                (0U-859U)
#endif

#if (_WIN32_IE >= 0x0400)
#define IPN_FIRST               (0U-860U)       // internet address
#define IPN_LAST                (0U-879U)       // internet address

#define SBN_FIRST               (0U-880U)       // status bar
#define SBN_LAST                (0U-899U)

#define PGN_FIRST               (0U-900U)       // Pager Control
#define PGN_LAST                (0U-950U)

#endif

#if (_WIN32_IE >= 0x0500)
#ifndef WMN_FIRST
#define WMN_FIRST               (0U-1000U)
#define WMN_LAST                (0U-1200U)
#endif
#endif

#if (_WIN32_WINNT >= 0x0501)
#define BCN_FIRST               (0U-1250U)
#define BCN_LAST                (0U-1350U)
#endif

#define NM_OUTOFMEMORY          (NM_FIRST-1)
#define NM_CLICK                (NM_FIRST-2)    // uses NMCLICK struct
#define NM_DBLCLK               (NM_FIRST-3)
#define NM_RETURN               (NM_FIRST-4)
#define NM_RCLICK               (NM_FIRST-5)    // uses NMCLICK struct
#define NM_RDBLCLK              (NM_FIRST-6)
#define NM_SETFOCUS             (NM_FIRST-7)
#define NM_KILLFOCUS            (NM_FIRST-8)
#if (_WIN32_IE >= 0x0300)
#define NM_CUSTOMDRAW           (NM_FIRST-12)
#define NM_HOVER                (NM_FIRST-13)
#endif
#if (_WIN32_IE >= 0x0400)
#define NM_NCHITTEST            (NM_FIRST-14)   // uses NMMOUSE struct
#define NM_KEYDOWN              (NM_FIRST-15)   // uses NMKEY struct
#define NM_RELEASEDCAPTURE      (NM_FIRST-16)
#define NM_SETCURSOR            (NM_FIRST-17)   // uses NMMOUSE struct
#define NM_CHAR                 (NM_FIRST-18)   // uses NMCHAR struct
#endif
#if (_WIN32_IE >= 0x0401)
#define NM_TOOLTIPSCREATED      (NM_FIRST-19)   // notify of when the tooltips window is create
#endif
#if (_WIN32_IE >= 0x0500)
#define NM_LDOWN                (NM_FIRST-20)
#define NM_RDOWN                (NM_FIRST-21)
#define NM_THEMECHANGED         (NM_FIRST-22)
#endif

//Property Sheet Pages 
#define PSP_DEFAULT                0x00000000
#define PSP_DLGINDIRECT            0x00000001
#define PSP_USEHICON               0x00000002
#define PSP_USEICONID              0x00000004
#define PSP_USETITLE               0x00000008
#define PSP_RTLREADING             0x00000010

#define PSP_HASHELP                0x00000020
#define PSP_USEREFPARENT           0x00000040
#define PSP_USECALLBACK            0x00000080
#define PSP_PREMATURE              0x00000400

#if (_WIN32_IE >= 0x0400)
//----- New flags for wizard97 --------------
#define PSP_HIDEHEADER             0x00000800
#define PSP_USEHEADERTITLE         0x00001000
#define PSP_USEHEADERSUBTITLE      0x00002000
//-------------------------------------------
#endif
#if (_WIN32_WINNT >= 0x0501) || ISOLATION_AWARE_ENABLED
#define PSP_USEFUSIONCONTEXT       0x00004000
#endif

//====== HEADER CONTROL =======================================================
#define HDN_ITEMCHANGINGA       (HDN_FIRST-0)
#define HDN_ITEMCHANGINGW       (HDN_FIRST-20)
#define HDN_ITEMCHANGEDA        (HDN_FIRST-1)
#define HDN_ITEMCHANGEDW        (HDN_FIRST-21)
#define HDN_ITEMCLICKA          (HDN_FIRST-2)
#define HDN_ITEMCLICKW          (HDN_FIRST-22)
#define HDN_ITEMDBLCLICKA       (HDN_FIRST-3)
#define HDN_ITEMDBLCLICKW       (HDN_FIRST-23)
#define HDN_DIVIDERDBLCLICKA    (HDN_FIRST-5)
#define HDN_DIVIDERDBLCLICKW    (HDN_FIRST-25)
#define HDN_BEGINTRACKA         (HDN_FIRST-6)
#define HDN_BEGINTRACKW         (HDN_FIRST-26)
#define HDN_ENDTRACKA           (HDN_FIRST-7)
#define HDN_ENDTRACKW           (HDN_FIRST-27)
#define HDN_TRACKA              (HDN_FIRST-8)
#define HDN_TRACKW              (HDN_FIRST-28)
#if (_WIN32_IE >= 0x0300)
#define HDN_GETDISPINFOA        (HDN_FIRST-9)
#define HDN_GETDISPINFOW        (HDN_FIRST-29)
#define HDN_BEGINDRAG           (HDN_FIRST-10)
#define HDN_ENDDRAG             (HDN_FIRST-11)
#endif
#if (_WIN32_IE >= 0x0500)
#define HDN_FILTERCHANGE        (HDN_FIRST-12)
#define HDN_FILTERBTNCLICK      (HDN_FIRST-13)
#endif

#ifdef UNICODE
#define HDN_ITEMCHANGING         HDN_ITEMCHANGINGW
#define HDN_ITEMCHANGED          HDN_ITEMCHANGEDW
#define HDN_ITEMCLICK            HDN_ITEMCLICKW
#define HDN_ITEMDBLCLICK         HDN_ITEMDBLCLICKW
#define HDN_DIVIDERDBLCLICK      HDN_DIVIDERDBLCLICKW
#define HDN_BEGINTRACK           HDN_BEGINTRACKW
#define HDN_ENDTRACK             HDN_ENDTRACKW
#define HDN_TRACK                HDN_TRACKW
#if (_WIN32_IE >= 0x0300)
#define HDN_GETDISPINFO          HDN_GETDISPINFOW
#endif
#else
#define HDN_ITEMCHANGING         HDN_ITEMCHANGINGA
#define HDN_ITEMCHANGED          HDN_ITEMCHANGEDA
#define HDN_ITEMCLICK            HDN_ITEMCLICKA
#define HDN_ITEMDBLCLICK         HDN_ITEMDBLCLICKA
#define HDN_DIVIDERDBLCLICK      HDN_DIVIDERDBLCLICKA
#define HDN_BEGINTRACK           HDN_BEGINTRACKA
#define HDN_ENDTRACK             HDN_ENDTRACKA
#define HDN_TRACK                HDN_TRACKA
#if (_WIN32_IE >= 0x0300)
#define HDN_GETDISPINFO          HDN_GETDISPINFOA
#endif
#endif

//====== TOOLBAR CONTROL ======================================================
#define TBN_GETBUTTONINFOA      (TBN_FIRST-0)
#define TBN_BEGINDRAG           (TBN_FIRST-1)
#define TBN_ENDDRAG             (TBN_FIRST-2)
#define TBN_BEGINADJUST         (TBN_FIRST-3)
#define TBN_ENDADJUST           (TBN_FIRST-4)
#define TBN_RESET               (TBN_FIRST-5)
#define TBN_QUERYINSERT         (TBN_FIRST-6)
#define TBN_QUERYDELETE         (TBN_FIRST-7)
#define TBN_TOOLBARCHANGE       (TBN_FIRST-8)
#define TBN_CUSTHELP            (TBN_FIRST-9)
#if (_WIN32_IE >= 0x0300)
#define TBN_DROPDOWN            (TBN_FIRST - 10)
#endif
#if (_WIN32_IE >= 0x0400)
#define TBN_GETOBJECT           (TBN_FIRST - 12)
#endif

#define TBN_HOTITEMCHANGE       (TBN_FIRST - 13)
#define TBN_DRAGOUT             (TBN_FIRST - 14) // this is sent when the user clicks down on a button then drags off the button
#define TBN_DELETINGBUTTON      (TBN_FIRST - 15) // uses TBNOTIFY
#define TBN_GETDISPINFOA        (TBN_FIRST - 16) // This is sent when the  toolbar needs  some display information
#define TBN_GETDISPINFOW        (TBN_FIRST - 17) // This is sent when the  toolbar needs  some display information
#define TBN_GETINFOTIPA         (TBN_FIRST - 18)
#define TBN_GETINFOTIPW         (TBN_FIRST - 19)
#define TBN_GETBUTTONINFOW      (TBN_FIRST - 20)
#if (_WIN32_IE >= 0x0500)
#define TBN_RESTORE             (TBN_FIRST - 21)
#define TBN_SAVE                (TBN_FIRST - 22)
#define TBN_INITCUSTOMIZE       (TBN_FIRST - 23)
#endif // (_WIN32_IE >= 0x0500)

#ifdef UNICODE
#define TBN_GETINFOTIP          TBN_GETINFOTIPW
#else
#define TBN_GETINFOTIP          TBN_GETINFOTIPA
#endif

#ifdef UNICODE
#define TBN_GETDISPINFO       TBN_GETDISPINFOW
#else
#define TBN_GETDISPINFO       TBN_GETDISPINFOA
#endif

#ifdef UNICODE
#define TBN_GETBUTTONINFO       TBN_GETBUTTONINFOW
#else
#define TBN_GETBUTTONINFO       TBN_GETBUTTONINFOA
#endif


//====== REBAR CONTROL ========================================================
#define RBN_HEIGHTCHANGE    (RBN_FIRST - 0)

#define RBN_GETOBJECT       (RBN_FIRST - 1)
#define RBN_LAYOUTCHANGED   (RBN_FIRST - 2)
#define RBN_AUTOSIZE        (RBN_FIRST - 3)
#define RBN_BEGINDRAG       (RBN_FIRST - 4)
#define RBN_ENDDRAG         (RBN_FIRST - 5)
#define RBN_DELETINGBAND    (RBN_FIRST - 6)     // Uses NMREBAR
#define RBN_DELETEDBAND     (RBN_FIRST - 7)     // Uses NMREBAR
#define RBN_CHILDSIZE       (RBN_FIRST - 8)

#if (_WIN32_IE >= 0x0500)
#define RBN_CHEVRONPUSHED   (RBN_FIRST - 10)
#endif      // _WIN32_IE >= 0x0500


#if (_WIN32_IE >= 0x0500)
#define RBN_MINMAX          (RBN_FIRST - 21)
#endif

#if (_WIN32_WINNT >= 0x0501)
#define RBN_AUTOBREAK       (RBN_FIRST - 22)
#endif


//====== TOOLTIPS CONTROL =====================================================
#define TTN_GETDISPINFOA        (TTN_FIRST - 0)
#define TTN_GETDISPINFOW        (TTN_FIRST - 10)
#define TTN_SHOW                (TTN_FIRST - 1)
#define TTN_POP                 (TTN_FIRST - 2)
#define TTN_LINKCLICK           (TTN_FIRST - 3)

#ifdef UNICODE
#define TTN_GETDISPINFO         TTN_GETDISPINFOW
#else
#define TTN_GETDISPINFO         TTN_GETDISPINFOA
#endif

#define TTN_NEEDTEXT            TTN_GETDISPINFO
#define TTN_NEEDTEXTA           TTN_GETDISPINFOA
#define TTN_NEEDTEXTW           TTN_GETDISPINFOW

//====== STATUS BAR CONTROL ===================================================
/// status bar notifications
#if (_WIN32_IE >= 0x0400)
#define SBN_SIMPLEMODECHANGE    (SBN_FIRST - 0)
#endif

//====== MENU HELP ============================================================

//====== TRACKBAR CONTROL =====================================================
//====== DRAG LIST CONTROL ====================================================
//====== UPDOWN CONTROL =======================================================
#define UDN_DELTAPOS            (UDN_FIRST - 1)

//====== PROGRESS CONTROL =====================================================
//====== HOTKEY CONTROL =======================================================
//====== COMMON CONTROL STYLES ================================================
//====== LISTVIEW CONTROL =====================================================
#define LVN_ITEMCHANGING        (LVN_FIRST-0)
#define LVN_ITEMCHANGED         (LVN_FIRST-1)
#define LVN_INSERTITEM          (LVN_FIRST-2)
#define LVN_DELETEITEM          (LVN_FIRST-3)
#define LVN_DELETEALLITEMS      (LVN_FIRST-4)
#define LVN_BEGINLABELEDITA     (LVN_FIRST-5)
#define LVN_BEGINLABELEDITW     (LVN_FIRST-75)
#define LVN_ENDLABELEDITA       (LVN_FIRST-6)
#define LVN_ENDLABELEDITW       (LVN_FIRST-76)
#define LVN_COLUMNCLICK         (LVN_FIRST-8)
#define LVN_BEGINDRAG           (LVN_FIRST-9)
#define LVN_BEGINRDRAG          (LVN_FIRST-11)

#if (_WIN32_IE >= 0x0300)
#define LVN_ODCACHEHINT         (LVN_FIRST-13)
#define LVN_ODFINDITEMA         (LVN_FIRST-52)
#define LVN_ODFINDITEMW         (LVN_FIRST-79)

#define LVN_ITEMACTIVATE        (LVN_FIRST-14)
#define LVN_ODSTATECHANGED      (LVN_FIRST-15)

#ifdef UNICODE
#define LVN_ODFINDITEM          LVN_ODFINDITEMW
#else
#define LVN_ODFINDITEM          LVN_ODFINDITEMA
#endif
#endif      // _WIN32_IE >= 0x0300


#if (_WIN32_IE >= 0x0400)
#define LVN_HOTTRACK            (LVN_FIRST-21)
#endif

#define LVN_GETDISPINFOA        (LVN_FIRST-50)
#define LVN_GETDISPINFOW        (LVN_FIRST-77)
#define LVN_SETDISPINFOA        (LVN_FIRST-51)
#define LVN_SETDISPINFOW        (LVN_FIRST-78)

#ifdef UNICODE
#define LVN_BEGINLABELEDIT      LVN_BEGINLABELEDITW
#define LVN_ENDLABELEDIT        LVN_ENDLABELEDITW
#define LVN_GETDISPINFO         LVN_GETDISPINFOW
#define LVN_SETDISPINFO         LVN_SETDISPINFOW
#else
#define LVN_BEGINLABELEDIT      LVN_BEGINLABELEDITA
#define LVN_ENDLABELEDIT        LVN_ENDLABELEDITA
#define LVN_GETDISPINFO         LVN_GETDISPINFOA
#define LVN_SETDISPINFO         LVN_SETDISPINFOA
#endif


#define LVN_GETINFOTIPA          (LVN_FIRST-57)
#define LVN_GETINFOTIPW          (LVN_FIRST-58)

#ifdef UNICODE
#define LVN_GETINFOTIP          LVN_GETINFOTIPW
#define NMLVGETINFOTIP          NMLVGETINFOTIPW
#define LPNMLVGETINFOTIP        LPNMLVGETINFOTIPW
#else
#define LVN_GETINFOTIP          LVN_GETINFOTIPA
#define NMLVGETINFOTIP          NMLVGETINFOTIPA
#define LPNMLVGETINFOTIP        LPNMLVGETINFOTIPA
#endif

#define LVN_BEGINSCROLL          (LVN_FIRST-80)          
#define LVN_ENDSCROLL            (LVN_FIRST-81)

//====== TREEVIEW CONTROL =====================================================
#define TVN_SELCHANGINGA        (TVN_FIRST-1)
#define TVN_SELCHANGINGW        (TVN_FIRST-50)
#define TVN_SELCHANGEDA         (TVN_FIRST-2)
#define TVN_SELCHANGEDW         (TVN_FIRST-51)

#define TVN_GETDISPINFOA        (TVN_FIRST-3)
#define TVN_GETDISPINFOW        (TVN_FIRST-52)
#define TVN_SETDISPINFOA        (TVN_FIRST-4)
#define TVN_SETDISPINFOW        (TVN_FIRST-53)

#define TVN_ITEMEXPANDINGA      (TVN_FIRST-5)
#define TVN_ITEMEXPANDINGW      (TVN_FIRST-54)
#define TVN_ITEMEXPANDEDA       (TVN_FIRST-6)
#define TVN_ITEMEXPANDEDW       (TVN_FIRST-55)
#define TVN_BEGINDRAGA          (TVN_FIRST-7)
#define TVN_BEGINDRAGW          (TVN_FIRST-56)
#define TVN_BEGINRDRAGA         (TVN_FIRST-8)
#define TVN_BEGINRDRAGW         (TVN_FIRST-57)
#define TVN_DELETEITEMA         (TVN_FIRST-9)
#define TVN_DELETEITEMW         (TVN_FIRST-58)
#define TVN_BEGINLABELEDITA     (TVN_FIRST-10)
#define TVN_BEGINLABELEDITW     (TVN_FIRST-59)
#define TVN_ENDLABELEDITA       (TVN_FIRST-11)
#define TVN_ENDLABELEDITW       (TVN_FIRST-60)
#define TVN_KEYDOWN             (TVN_FIRST-12)

#if (_WIN32_IE >= 0x0400)
#define TVN_GETINFOTIPA         (TVN_FIRST-13)
#define TVN_GETINFOTIPW         (TVN_FIRST-14)
#define TVN_SINGLEEXPAND        (TVN_FIRST-15)

#endif // 0x400

#ifdef UNICODE
#define TVN_SELCHANGING         TVN_SELCHANGINGW
#define TVN_SELCHANGED          TVN_SELCHANGEDW
#define TVN_GETDISPINFO         TVN_GETDISPINFOW
#define TVN_SETDISPINFO         TVN_SETDISPINFOW
#define TVN_ITEMEXPANDING       TVN_ITEMEXPANDINGW
#define TVN_ITEMEXPANDED        TVN_ITEMEXPANDEDW
#define TVN_BEGINDRAG           TVN_BEGINDRAGW
#define TVN_BEGINRDRAG          TVN_BEGINRDRAGW
#define TVN_DELETEITEM          TVN_DELETEITEMW
#define TVN_BEGINLABELEDIT      TVN_BEGINLABELEDITW
#define TVN_ENDLABELEDIT        TVN_ENDLABELEDITW
#else
#define TVN_SELCHANGING         TVN_SELCHANGINGA
#define TVN_SELCHANGED          TVN_SELCHANGEDA
#define TVN_GETDISPINFO         TVN_GETDISPINFOA
#define TVN_SETDISPINFO         TVN_SETDISPINFOA
#define TVN_ITEMEXPANDING       TVN_ITEMEXPANDINGA
#define TVN_ITEMEXPANDED        TVN_ITEMEXPANDEDA
#define TVN_BEGINDRAG           TVN_BEGINDRAGA
#define TVN_BEGINRDRAG          TVN_BEGINRDRAGA
#define TVN_DELETEITEM          TVN_DELETEITEMA
#define TVN_BEGINLABELEDIT      TVN_BEGINLABELEDITA
#define TVN_ENDLABELEDIT        TVN_ENDLABELEDITA
#endif

#ifdef UNICODE
#define TVN_GETINFOTIP          TVN_GETINFOTIPW
#else
#defne TVN_GETINFOTIP          TVN_GETINFOTIPA
#endif

////////////////////  ComboBoxEx ////////////////////////////////
#define CBEN_GETDISPINFOW        (CBEN_FIRST - 7)
#define CBEN_GETDISPINFOA        (CBEN_FIRST - 0)
#ifdef UNICODE
#define CBEN_GETDISPINFO        CBEN_GETDISPINFOW
#else
#define CBEN_GETDISPINFO        CBEN_GETDISPINFOA
#endif

#define CBEN_INSERTITEM          (CBEN_FIRST - 1)
#define CBEN_DELETEITEM          (CBEN_FIRST - 2)
#define CBEN_BEGINEDIT           (CBEN_FIRST - 4)
#define CBEN_ENDEDITA            (CBEN_FIRST - 5)
#define CBEN_ENDEDITW            (CBEN_FIRST - 6)

#define CBEN_DRAGBEGINA                  (CBEN_FIRST - 8)
#define CBEN_DRAGBEGINW                  (CBEN_FIRST - 9)

#ifdef UNICODE
#define CBEN_DRAGBEGIN CBEN_DRAGBEGINW
#else
#define CBEN_DRAGBEGIN CBEN_DRAGBEGINA
#endif
// lParam specifies why the endedit is happening
#ifdef UNICODE
#define CBEN_ENDEDIT CBEN_ENDEDITW
#else
#define CBEN_ENDEDIT CBEN_ENDEDITA
#endif

//====== TAB CONTROL ==========================================================
#define TCN_KEYDOWN             (TCN_FIRST - 0)
#define TCN_SELCHANGE           (TCN_FIRST - 1)
#define TCN_SELCHANGING         (TCN_FIRST - 2)
#if (_WIN32_IE >= 0x0400)
#define TCN_GETOBJECT           (TCN_FIRST - 3)
#endif      // _WIN32_IE >= 0x0400
#if (_WIN32_IE >= 0x0500)
#define TCN_FOCUSCHANGE         (TCN_FIRST - 4)
#endif      // _WIN32_IE >= 0x0500


//====== ANIMATE CONTROL ======================================================
#define ACN_START               1
#define ACN_STOP                2

//====== MONTHCAL CONTROL ======================================================
#define MCN_SELCHANGE       (MCN_FIRST + 1)
#define MCN_GETDAYSTATE     (MCN_FIRST + 3)
#define MCN_SELECT          (MCN_FIRST + 4)

//====== DATETIMEPICK CONTROL ==================================================
#ifdef UNICODE
#define DTN_WMKEYDOWN           DTN_WMKEYDOWNW
#define NMDATETIMEWMKEYDOWN     NMDATETIMEWMKEYDOWNW
#define LPNMDATETIMEWMKEYDOWN   LPNMDATETIMEWMKEYDOWNW
#else
#define DTN_WMKEYDOWN           DTN_WMKEYDOWNA
#define NMDATETIMEWMKEYDOWN     NMDATETIMEWMKEYDOWNA
#define LPNMDATETIMEWMKEYDOWN   LPNMDATETIMEWMKEYDOWNA
#endif


#define DTN_FORMATA  (DTN_FIRST + 4) // query display for app format field (X)
#define DTN_FORMATW  (DTN_FIRST + 17)

#ifdef UNICODE
#define DTN_FORMAT             DTN_FORMATW
#define NMDATETIMEFORMAT        NMDATETIMEFORMATW
#define LPNMDATETIMEFORMAT      LPNMDATETIMEFORMATW
#else
#define DTN_FORMAT             DTN_FORMATA
#define NMDATETIMEFORMAT        NMDATETIMEFORMATA
#define LPNMDATETIMEFORMAT      LPNMDATETIMEFORMATA
#endif


#define DTN_FORMATQUERYA  (DTN_FIRST + 5) // query formatting info for app format field (X)
#define DTN_FORMATQUERYW (DTN_FIRST + 18)

#ifdef UNICODE
#define DTN_FORMATQUERY         DTN_FORMATQUERYW
#else
#define DTN_FORMATQUERY         DTN_FORMATQUERYA
#endif

#define DTN_DROPDOWN    (DTN_FIRST + 6) // MonthCal has dropped down
#define DTN_CLOSEUP     (DTN_FIRST + 7) // MonthCal is popping up


//    IP Address edit control
#define IPN_FIELDCHANGED                (IPN_FIRST - 0)

//  ====================== Pager Control =============================
#define PGN_CALCSIZE        (PGN_FIRST-2)
#define PGN_HOTITEMCHANGE   (PGN_FIRST-3)


// === Native Font Control ===
/// ====================== Button Control =============================
#define BN_CLICKED          0
#define BN_PAINT            1
#define BN_HILITE           2
#define BN_UNHILITE         3
#define BN_DISABLE          4
#define BN_DOUBLECLICKED    5
#if(WINVER >= 0x0400)
#define BN_PUSHED           BN_HILITE
#define BN_UNPUSHED         BN_UNHILITE
#define BN_DBLCLK           BN_DOUBLECLICKED
#define BN_SETFOCUS         6
#define BN_KILLFOCUS        7
#endif /* WINVER >= 0x0400 */

/// ====================== Static Control =============================
#define STN_CLICKED         0
#define STN_DBLCLK          1
#define STN_ENABLE          2
#define STN_DISABLE         3

/// ====================== Edit Control =============================
/*
 * Edit Control Notification Codes
 */
#define EN_SETFOCUS         0x0100
#define EN_KILLFOCUS        0x0200
#define EN_CHANGE           0x0300
#define EN_UPDATE           0x0400
#define EN_ERRSPACE         0x0500
#define EN_MAXTEXT          0x0501
#define EN_HSCROLL          0x0601
#define EN_VSCROLL          0x0602

#if(_WIN32_WINNT >= 0x0500)
#define EN_ALIGN_LTR_EC     0x0700
#define EN_ALIGN_RTL_EC     0x0701
#endif /* _WIN32_WINNT >= 0x0500 */
/// ====================== Listbox Control =============================
#define LBN_ERRSPACE        (-2)
#define LBN_SELCHANGE       1
#define LBN_DBLCLK          2
#define LBN_SELCANCEL       3
#define LBN_SETFOCUS        4
#define LBN_KILLFOCUS       5

/// ====================== Combobox Control =============================
#define CBN_ERRSPACE        (-1)
#define CBN_SELCHANGE       1
#define CBN_DBLCLK          2
#define CBN_SETFOCUS        3
#define CBN_KILLFOCUS       4
#define CBN_EDITCHANGE      5
#define CBN_EDITUPDATE      6
#define CBN_DROPDOWN        7
#define CBN_CLOSEUP         8
#define CBN_SELENDOK        9
#define CBN_SELENDCANCEL    10
/// ====================== Scrollbar Control ============================

//====== SysLink control =========================================
// === MUI APIs ===
//====== TrackMouseEvent  =====================================================
#ifndef WM_MOUSEHOVER
#define WM_MOUSEHOVER                   0x02A1
#define WM_MOUSELEAVE                   0x02A3
#endif
//====== Flat Scrollbar APIs=========================================
BN_CLICKED