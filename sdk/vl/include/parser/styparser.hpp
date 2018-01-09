#include "vmparser.hpp"

namespace vl
{
	class VL_API styparser : public vmparser3
	{
	public:
		styparser();
		~styparser();
	private:
		bool bstyParser_;
	public:
		bool isStyParser();
		void setStyParser(bool bsty);
	public:
		bool parse(int pl);
	};
}