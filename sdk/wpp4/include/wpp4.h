#ifndef WPP4_H
#define WPP4_H

#ifdef VLANG4_EXPORTS
#define VLANG4_API __declspec(dllexport)
#else
#define VLANG4_API __declspec(dllimport)
#endif

#include "vord.hpp"
#include "vordsvr.hpp"

#endif //WPP4_H