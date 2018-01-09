#pragma once

namespace Hxsoft{ namespace XUtil { namespace Xml
{
	template <class Ch>
	inline bool IsSpaceLetter(Ch ch)
	{
		return ch==' '||ch=='\t'||ch=='\r'||ch=='n';
	}

	template <class Ch>
	inline bool  IsDigits(Ch ch)
	{
		return ch>='0' && ch<='9';
	}

	template <class Ch>
	bool IsIdentLetter(Ch ch)
	{
		return ch >= 0x100 ||(ch >='a' && ch <='z')  ||(ch >='A' && ch <='Z') || (ch=='_' || ch=='$');
	}

	template <class Ch>
	inline const Ch* SkipSpace(const Ch * xmlText)
	{
		while(IsSpaceLetter(*xmlText))xmlText++;
		return xmlText;
	}

	template <class Ch>
	const Ch* SkipComment(const Ch* xmlText)
	{
		while(*xmlText)
		{
			if(xmlText[0]=='-'||xmlText[1]=='-'||xmlText[2]=='>')
			{
				xmlText+=3;
				break;
			}
			xmlText++;
		}
		return xmlText;
	}

	template <class Ch>
	const Ch* SkipPI(const Ch* xmlText)
	{
		if(xmlText[0]=='<' && xmlText[1]=='?') 
		{
			xmlText +=2;
		}else
			return xmlText;
		while(*++xmlText)
		{
			if(xmlText[0]=='?' && xmlText[1]=='>')
			{
				xmlText+=2;
				break;
			}
			xmlText++;
		}
		return xmlText;
	}

	//only eat text content 
	template <class Ch>
	const Ch* SkipElementContent(const Ch* xmlText)
	{
		xmlText = SkipSpace(xmlText);
		while(*xmlText)
		{
			if(*xmlText=='<')
			{
				//skip pcdata
				if(xmlText[1]=='!' && xmlText[2]=='[' && xmlText[3]=='C' &&
					xmlText[4]=='D' && xmlText[5]=='A' && xmlText[6]=='T' &&
					xmlText[7]=='A' && xmlText[8]=='[')
				{
					xmlText += 9;
					while(*xmlText)
					{
						if(xmlText[0]==']' && xmlText[1]==']' && xmlText[2]=='>')
						{
							xmlText += 3;
							break;
						}
						xmlText++;
					}
				}else break;
			} else break;
		}
		return xmlText;
	}

	//skip attribute, child element node, child content node
	template <class Ch>
	const Ch* SkipElement(const Ch * xmlText)
	{
		//skip element head
		while(*xmlText)
		{
			if(*xmlText=='>')break;
			if(*xmlText=='/')
			{
				*xmlText++;
				xmlText = SkipSpace(xmlText);
				if(*xmlText=='>')
				{
					//found element tail
					return ++xmlText;
				}
			}
			xmlText++;
		}
		if(!xmlText) return xmlText;
		
		while(*xmlText)
		{
			//skip content
			xmlText = SkipElementContent(xmlText);
			if(*xmlText=='<')
			{
				//skip comment
				if(xmlText[1]=='!' && xmlText[2]=='-' && xmlText[3]=='-')
					xmlText = SkipComment(xmlText);
				else
				{
					xmlText = SkipSpace(xmlText + 1);
					//skip element tail then exit
					if(*xmlText=='/')
					{
						while(*++xmlText != '>');
						if(*xmlText)xmlText++;
						break;
					}else
						xmlText = SkipElement(xmlText);
				}
			}
			else
				xmlText++;
		}
		return xmlText;
	}

	template <class Ch>
	const Ch * FirstChildElement(const Ch * xmlText ,bool skipOwnerHead )
	{
		//skip ownwe head
		if(skipOwnerHead)
		{
			while(*xmlText)
			{
				if(*xmlText=='>')break;
				if(*xmlText=='/')
				{
					*xmlText++;
					xmlText = SkipSpace(xmlText);
					if(*xmlText=='>')
					{
						//found element tail ,then none child element
						return NULL;
					}
				}
				xmlText++;
			}
		}
		if(!xmlText) return xmlText;
		
		while(*xmlText)
		{
			//skip content
			xmlText = SkipElementContent(xmlText);
			if(*xmlText=='<')
			{
				//skip comment
				if(xmlText[1]=='!' && xmlText[2]=='-' && xmlText[3]=='-')
					xmlText = SkipComment(xmlText);
				else
				{
					//meet element tail no child element
					if(SkipSpace(xmlText + 1)[0]=='/') return NULL;
					//found
					return xmlText;
				}
			}
			xmlText++;
		}
		return xmlText;		
	}

	template <class Ch >
	const Ch * NextSblingElement(const Ch * xmlText)
	{
		xmlText = SkipElement(xmlText);
		while(*xmlText)
		{
			//skip content
			xmlText = SkipElementContent(xmlText);
			if(*xmlText=='<')
			{
				//skip comment
				if(xmlText[1]=='!' && xmlText[2]=='-' && xmlText[3]=='-')
					xmlText = SkipComment(xmlText);
				else
				{
					//meet element tail no child element
					if(SkipSpace(xmlText + 1)[0]=='/') return NULL;
					//found
					return xmlText;
				}
			}
			xmlText++;
		}
		return xmlText;
	}
}}}