// Lucas Merenciano Martinez  <lucasmermar@gmail.com>

#ifndef __LEEP_HELPER_FUNCTIONS_H__
#define __LEEP_HELPER_FUNCTIONS_H__

#include <string>

namespace leep
{
    void CreateRenderable(std::string entity_name,
                          std::string geometry_name,
                          std::string texture_name);
    void RemoveEntity(std::string entity_name);
    void SetParent(std::string entity_name, std::string parent_name);
    void DetachFromParent(std::string entity_name);
    void SetLocation(std::string entity_name, float x, float y, float z);
}

#endif // __LEEP_HELPER_FUNCTIONS_H__
