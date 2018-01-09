#pragma once

#include "xbxbase.h"
#include <vector>
using namespace std;

namespace Hxsoft{ namespace XFrame{
	class XSHAPE_API XShapeObjectSetItem :
		public xbObject
	{
	public:
		XShapeObjectSetItem(void);
		~XShapeObjectSetItem(void);
	public:
		vector<class XShapeObject *> * m_ShapeObjects;
		int LoadShapeSetItemXml(LPTSTR pStrXml);
		int LoadShapeSetItem(IXMLDOMElement * m_pElement);
	public:
		LPTSTR m_pShapeUriName;
	public:
		class XShapeObject * Find(LPTSTR strShape);
	};
}}
