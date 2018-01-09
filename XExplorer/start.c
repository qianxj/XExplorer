#include "start.h"
extern IMAGE_DOS_HEADER __ImageBase;
int xCRTStartup( void );
int xCRTStartup( void )
{
		int mainret=0;
                               mainret = xWinMain( (HINSTANCE)&__ImageBase, NULL, NULL,SW_SHOWDEFAULT);
	   return 1;
}
