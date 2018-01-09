#include "StdAfx.h"
#include "..\include\xcAgentEdit.hpp"
#include "xclistwin.hpp"

namespace Hxsoft{ namespace XFrame
{

LPTSTR pStrData1 =_T("<data> \
<row no=\"101\" name=\"丝带及其它带类产品\" /> \
<row no=\"102\" name=\"线,纱及长丝品种\" /> \
<row no=\"104\" name=\"拉链\" /> \
<row no=\"105\" name=\"钮扣\" /> \
<row no=\"108\" name=\"裁缝用品\" /> \
<row no=\"109\" name=\"缝纫机及配件\" /> \
<row no=\"111\" name=\"窗帘用品\" /> \
<row no=\"190\" name=\"肩垫及其它垫类\" /> \
<row no=\"189\" name=\"文胸/内衣及配件\" /> \
<row no=\"178\" name=\"花边\" /> \
<row no=\"186\" name=\"面料\" /> \
<row no=\"196\" name=\"新娘用品\" /> \
<row no=\"139\" name=\"装饰带(类似流苏)\" /> \
<row no=\"201\" name=\"亮片,钻, 珠子和链\" /> \
<row no=\"144\" name=\"衬衫配件\" /> \
<row no=\"134\" name=\"箱包配件\" /> \
<row no=\"203\" name=\"缝纫小包装及其它小包装\" /> \
<row no=\"158\" name=\"绳类\" /> \
<row no=\"191\" name=\"衣架/塑料钩\" /> \
<row no=\"167\" name=\"滚边带\" /> \
<row no=\"173\" name=\"松紧带,扁紧带,绑腰带\" /> \
<row no=\"165\" name=\"波浪带兰花花边\" /> \
<row no=\"193\" name=\"帽辅料\" /> \
<row no=\"198\" name=\"裤子辅料\" /> \
<row no=\"175\" name=\"粘扣带\" /> \
<row no=\"176\" name=\"反光带及其制品\" /> \
<row no=\"177\" name=\"商标辅料\" /> \
<row no=\"187\" name=\"衬布\" /> \
<row no=\"166\" name=\"流苏\" /> \
<row no=\"199\" name=\"鞋/鞋子辅料\" /> \
<row no=\"192\" name=\"手机吊带、胸卡及配件\" /> \
<row no=\"202\" name=\"贴花,丝带花,手钩花及其它手工花\" /> \
<row no=\"188\" name=\"服饰\" /> \
<row no=\"197\" name=\"羽毛制品\" /> \
</data>");

LPTSTR pStrData2 =_T("<data> \
<row no=\"1\" name=\"反光\" /> \
<row no=\"2\" name=\"金银\" /> \
<row no=\"3\" name=\"苏格兰\" /> \
<row no=\"4\" name=\"涤纶\" /> \
<row no=\"5\" name=\"棉\" /> \
<row no=\"6\" name=\"涤棉\" /> \
<row no=\"7\" name=\"皮\" /> \
<row no=\"8\" name=\"松紧\" /> \
</data>");

LPTSTR pStrData3 =_T("<data> \
<row no=\"1\" name=\"无芯\" /> \
<row no=\"2\" name=\"有芯\" /> \
<row no=\"3\" name=\"双折\" /> \
</data>");

LPTSTR pStrData4 =_T("<data> \
<row no=\"01\" name=\"12MM(1/2&quot;)\" /> \
<row no=\"02\" name=\"13MM\" /> \
<row no=\"03\" name=\"15MM(5/8&quot;)\" /> \
<row no=\"04\" name=\"16MM\" /> \
<row no=\"05\" name=\"19MM(3/4&quot;)\" /> \
<row no=\"06\" name=\"20MM\" /> \
<row no=\"07\" name=\"25MM(1&quot;)\" /> \
<row no=\"08\" name=\"其它规格\" /> \
</data>");

 LPTSTR pStrReport1 =_T("<sheet version=\"3.0\" editer=\"xreport\"> \
    <stylesheet> \
      <item name=\"x0\"> \
        align:left;background-color:#ebead8 none #ffffff none; \
      </item> \
      <item name=\"x1\"> \
        align:left;background-color:none #f0f4f0 none #f0f4f0; \
      </item> \
    </stylesheet> \
    <datasource xroot=\"//row\"> \
      <col name=\"cnt_no\" xpath=\"@no\" /> \
      <col name=\"order_no\" xpath=\"@name\" /> \
    </datasource> \
    <band class=\"x0\"> \
      <detail height=\"25\"> \
        <col class=\"x1\" refcol=\"1\" width=\"40\"  /> \
        <col class=\"x1\" refcol=\"2\" width=\"150\"  /> \
      </detail> \
      </band> \
  </sheet>");


 LPTSTR pStrReport2 =_T("<sheet version=\"3.0\" editer=\"xreport\"> \
    <stylesheet> \
      <item name=\"x0\"> \
        align:left;background-color:#ebead8 none #ffffff none; \
      </item> \
      <item name=\"x1\"> \
        align:left;background-color:none #f0f4f0 none #f0f4f0; \
      </item> \
    </stylesheet> \
    <datasource xroot=\"//row\"> \
      <col name=\"cnt_no\" xpath=\"@no\" /> \
      <col name=\"order_no\" xpath=\"@name\" /> \
    </datasource> \
    <band class=\"x0\"> \
      <detail height=\"25\"> \
        <col class=\"x1\" refcol=\"1\" width=\"40\"  /> \
        <col class=\"x1\" refcol=\"2\" width=\"150\"  /> \
      </detail> \
      </band> \
  </sheet>");
 
  LPTSTR pStrReport3 =_T("<sheet version=\"3.0\" editer=\"xreport\"> \
    <stylesheet> \
      <item name=\"x0\"> \
        align:left;background-color:#ebead8 none #ffffff none; \
      </item> \
      <item name=\"x1\"> \
        align:left;background-color:none #f0f4f0 none #f0f4f0; \
      </item> \
    </stylesheet> \
    <datasource xroot=\"//row\"> \
      <col name=\"cnt_no\" xpath=\"@no\" /> \
      <col name=\"order_no\" xpath=\"@name\" /> \
    </datasource> \
    <band class=\"x0\"> \
      <detail height=\"25\"> \
        <col class=\"x1\" refcol=\"1\" width=\"40\"  /> \
        <col class=\"x1\" refcol=\"2\" width=\"150\"  /> \
      </detail> \
      </band> \
  </sheet>");

    LPTSTR pStrReport4 =_T("<sheet version=\"3.0\" editer=\"xreport\"> \
    <stylesheet> \
      <item name=\"x0\"> \
        align:left;background-color:#ebead8 none #ffffff none; \
      </item> \
      <item name=\"x1\"> \
        align:left;background-color:none #f0f4f0 none #f0f4f0; \
      </item> \
    </stylesheet> \
    <datasource xroot=\"//row\"> \
      <col name=\"cnt_no\" xpath=\"@no\" /> \
      <col name=\"order_no\" xpath=\"@name\" /> \
    </datasource> \
    <band class=\"x0\"> \
      <detail height=\"25\"> \
        <col class=\"x1\" refcol=\"1\" width=\"40\"  /> \
        <col class=\"x1\" refcol=\"2\" width=\"150\"  /> \
      </detail> \
      </band> \
  </sheet>");

  xcAgentEdit::xcAgentEdit(void)
	{
		m_pListWin = new xcListWin;
		m_pListWin->AddReport(pStrReport1);
		m_pListWin->AddReport(pStrReport2);
		m_pListWin->AddReport(pStrReport3);
		m_pListWin->AddReport(pStrReport4);
		
		m_pListWin->SetReportData(0,pStrData1);
		m_pListWin->SetReportData(1,pStrData2);
		m_pListWin->SetReportData(2,pStrData3);
		m_pListWin->SetReportData(3,pStrData4);
	}

	xcAgentEdit::~xcAgentEdit(void)
	{
		m_pListWin->Show(false);
		if(m_pListWin)
		{
			::DestroyWindow(m_pListWin->m_hWnd);
			delete m_pListWin;
		}
	}

	void xcAgentEdit::Initial()
	{
		this->AttachCtrlListon(EN_CHANGE,(FNEvent)&xcAgentEdit::EvtChange);
	}
	bool xcAgentEdit::EvtChange(TEvent* pEvent,LPARAM lParam)
	{
		DWORD	dwLen;
		dwLen = ::GetWindowTextLength(m_hWnd);
		if(dwLen == 0||  dwLen == 3 || dwLen == 4 || dwLen == 5)
		{
			if(dwLen==0)m_pListWin->SwitchReport(0);
			if(dwLen==3)m_pListWin->SwitchReport(1);
			if(dwLen==4)m_pListWin->SwitchReport(2);
			if(dwLen==5)m_pListWin->SwitchReport(3);

			RECT rc;
			CalcListRect(200,300,rc);
			m_pListWin->Show(true,&rc);
			::SetFocus(m_hWnd);
		}
		else
			m_pListWin->Show(false);
		return true;
	}
	int xcAgentEdit::CalcListRect(int width,int height,RECT &rc)
	{
		int scWidth,scHeight;
		scWidth = GetSystemMetrics(SM_CXSCREEN);
		scHeight = GetSystemMetrics(SM_CYSCREEN);

		RECT	rt;
		RECT bound ;
		::GetWindowRect(m_hWnd,&bound);
		if(bound.bottom +  height< scHeight && bound.left + width < scWidth)
		{
			rt.left =  bound.left;
			rt.top  =  bound.bottom;
			rt.right = rt.left + width;
			rt.bottom = rt.top + height;
		}else if(bound.top +  height< scHeight && bound.right + width < scWidth)
		{
			rt.left =  bound.right;
			rt.top  =  bound.top;
			rt.right = rt.left + width;
			rt.bottom = rt.top + height;
		}else if(bound.top +  height< scHeight && bound.left - width > 0)
		{
			rt.right =  bound.left;
			rt.top  =  bound.top;
			rt.left = rt.right - width;
			rt.bottom = rt.top + height;
		}else if(bound.bottom +  height< scHeight)
		{
			rt.right =  scWidth;
			rt.top  =  bound.bottom;
			rt.left = rt.right - width;
			rt.bottom = rt.top + height;
		}else if(bound.bottom -  height>0 && bound.right + width < scWidth)
		{
			rt.left =  bound.right;
			rt.bottom =  bound.bottom;
			rt.right = rt.left + width;
			rt.top = rt.bottom - height;
		}else if(bound.bottom -  height>0 && bound.left - width > 0)
		{
			rt.right =  bound.left;
			rt.bottom =  bound.bottom;
			rt.left = rt.right - width;
			rt.top = rt.bottom - height;
		}else if(bound.bottom -  height>0)
		{
			rt.right =  scWidth;
			rt.top  =  bound.bottom;
			rt.left = rt.right - width;
			rt.bottom = rt.top + height;
		}else
			rt = rc;
		rc = rt;
		return 1;
	}
}}
