#include  "stdafx.h"
#include "imail.hpp"
#include "cdosys.h"
#include "xfuriloader.hpp"

#import "c:\Program Files\Common Files\System\ado\msado15.dll"  no_namespace rename("EOF", "EndOfFile")

namespace Hxsoft{ namespace XFrame 
{
	void * IMail::LoadMessage(LPTSTR mailId)
	{
		LPTSTR nMailID = mailId;
		TCHAR buf[255];
		LPBYTE pData;
		int nLen;
		::_stprintf_s(buf,64,_T("uri.hxsoft.com/xaserver/entity/mail/querybody")); 
		if(xfUriLoader::ProcessCmd(NULL,buf,mailId,pData,nLen)!=1) return NULL;

		CDO::IMessage* pMsg = NULL;
		HRESULT hr = CoCreateInstance( __uuidof(CDO::Message), NULL, CLSCTX_INPROC_SERVER, __uuidof(CDO::IMessage), (void**) &pMsg );

		/*
		int _convert = MultiByteToWideChar(CP_ACP, 0, (LPSTR)pData, -1,  NULL, 0);
		TCHAR* pszA = new TCHAR[_convert];
		int nRet = MultiByteToWideChar(CP_ACP, 0, (LPSTR)pData, -1, pszA, _convert);
		*/

		SAFEARRAY FAR* psa;
		SAFEARRAYBOUND rgsabound[1];
		rgsabound[0].lLbound = 0;
		rgsabound[0].cElements = nLen - 2;
		psa = SafeArrayCreate(VT_UI1, 1, rgsabound);
		psa->pvData = pData;

		VARIANT var;
		::VariantInit(&var);
		var.vt = VT_ARRAY|VT_UI1;
		var.parray = psa;

		_variant_t   varOptional(DISP_E_PARAMNOTFOUND,VT_ERROR);  
		_StreamPtr   adostream;
		adostream.CreateInstance(_T( "ADODB.Stream"));
		adostream-> Type   =   adTypeBinary;
		//adostream-> Type   =   adTypeText;//adTypeBinary;
		adostream->put_Mode(adModeReadWrite);
		adostream-> Open(varOptional,adModeUnknown,   adOpenStreamUnspecified,   _T(""),   _T(""));  
		//adostream-> LoadFromFile( "d:\\aa.txt");
		//adostream->WriteText((LPTSTR)pData,adWriteChar);
		adostream->Write(var);
		//adostream->put_Position(0);
		//_variant_t   vReadTo   =   adostream-> ReadText(adReadAll);  
		long   blob_size   =   adostream-> GetSize();


		CDO::IDataSource * pDataSource;
		pMsg->get_DataSource(&pDataSource);
		hr = pDataSource->OpenObject(adostream,L"_Stream");
		//adostream-> Close(); 
		//adostream->Release();

		//SafeArrayDestroy(psa);

		return pMsg;
	}
}}
