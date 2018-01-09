#pragma once
#include "wpp4.h"

const static int word_group = vord_t::getVordSvr()->askgroup();

namespace blocktype
{
	static const int document	= 1;
	static const int chapter	= 2; 
	static const int paragraph	= 3;
	static const int box		= 4;
	static const int text		= 5;
	static const int control	= 6;
	static const int page		= 7;
	static const int graph		= 8;
	static const int shape		= 9;
}

namespace css
{
	const field_t<int_t> index((int)L"css::index");
	const field_t<int_t> breakpage((int)L"css::breakpage");
}

namespace text
{
	const field_t<int_t>	offset((int)L"test:offset");
	const field_t<int_t>	length((int)L"test:length");
	const field_t<wstring_t>content((int)L"test:content");
	const field_t<int_t*>	pdx((int)L"test:pdx");
	const field_t<int_t>	limit((int)L"test:limit");
}

namespace block
{
	const field_t<int_t> type((int)L"block:type");
}

namespace control
{
	const field_t<wstring_t>type((int)L"control:type");
	const field_t<int_t>	left((int)L"control:left");
	const field_t<int_t>	right		((int)L"control:right");
	const field_t<int_t>	top			((int)L"control:top");
	const field_t<int_t>	bottom		((int)L"control:bottom");
	//const field_t<int_t>	width		(0x200 + 53);
	//const field_t<int_t>	height		(0x200 + 54);
	const field_t<ptr_t>	object		(0x200 + 55);
	const field_t<ptr_t>	element		(0x200 + 56);
	const field_t<wstring_t> src		((int)L"control:src");
}

namespace shape
{
	const field_t<wstring_t> ref	((int)L"shape:ref");
}

namespace image
{
	const field_t<wstring_t> href	((int)L"image:ref");
}

namespace extent_c
{
	const field_t<int_t> cx		((int)L"extent:cx");
	const field_t<int_t> cy		((int)L"extent:cy");
	const field_t<int_t> before	((int)L"extent:before");
	const field_t<int_t> foot	((int)L"extent:foot");
}
namespace render
{
	const field_t<unsigned char *> code(0x200 + 80);
	const field_t<int_t> codelimit(0x200 + 81);
	const field_t<int_t> pc(0x200 + 82);
	const field_t<int_t> epc(0x200 + 83);
	const field_t<int_t> rowcount(0x200 + 84);
}

namespace draw
{
	const field_t<int_t> x(0x200 + 85);
	const field_t<int_t> y(0x200 + 86);
	const field_t<int_t> left(0x200 + 87);
	const field_t<int_t> top	(0x200 + 88);
	const field_t<int_t> right	(0x200 + 89);
	const field_t<int_t> bottom	(0x200 + 90);
}

