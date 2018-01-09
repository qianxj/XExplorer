cl start.c  /O2 /GL /I "./xframe" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_UNICODE" /D "UNICODE" /FD /GS-   /W3 /nologo /c /Wp64 /Zi /TC /errorReport:prompt
cl XExplorer.c  /O2 /GL /I "./xframe" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_UNICODE" /D "UNICODE" /FD /GS-   /W3 /nologo /c /Wp64 /Zi /TP /errorReport:prompt

rc /d "_AFXDLL" /d "_UNICODE" /d "UNICODE" /fo"XExplorer.res" XExplorer.rc 

link start.obj  XExplorer.res XExplorer.obj /OUT:"D:\qianxj\vc\XExplorer\XExplorer.exe" /INCREMENTAL:NO /NOLOGO /NODEFAULTLIB /DEBUG /SUBSYSTEM:WINDOWS /OPT:REF /OPT:ICF /LTCG /ENTRY:"xCRTStartup" /MACHINE:X86 /ERRORREPORT:PROMPT user32.lib kernel32.lib gdi32.lib

