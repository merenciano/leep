#ifndef __LEEP_STRING_H__
#define __LEEP_STRING_H__

#include "core/memory/buddy-alloc-stl.h"
#include <string>

namespace leep
{
    typedef std::basic_string<char, std::char_traits<char>, BuddySTL<char>> String;
    String ToString(int32_t number);
}

#endif // __LEEP_STRING_H__