#ifndef __LEEP_STRING_H__
#define __LEEP_STRING_H__

#include "core/memory/buddy-alloc-stl.h"
#include <string>

namespace leep
{
#ifdef LEEP_LINUX
    typedef std::string String;
#else
    typedef std::basic_string<char, std::char_traits<char>, BuddySTL<char>> String;
#endif
    String ToString(int32_t number);
}

#endif // __LEEP_STRING_H__
