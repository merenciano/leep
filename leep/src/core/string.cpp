#include "core/string.h"

namespace leep {

String ToString(int32_t number)
{
    // TODO: Implement using ITOA or look for something that does not
    // allocate any memory on the heap
    // (I think small std::strings are allcoated in the stack anyway so
    // for the moment it will stay like this)
    //return static_cast<String>(std::to_string(number).c_str());
    return static_cast<String>(std::to_string(number));
}

} // namespace leep