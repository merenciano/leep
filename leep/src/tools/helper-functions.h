// Lucas Merenciano Martinez  <lucasmermar@gmail.com>

#ifndef __LEEP_HELPER_FUNCTIONS_H__
#define __LEEP_HELPER_FUNCTIONS_H__

#include "core/string.h"

namespace leep
{
    void CreateRenderable(String entity_name,
                          String geometry_name,
                          String texture_name);
    void RemoveEntity(String entity_name);
    void SetParent(String entity_name, String parent_name);
    void DetachFromParent(String entity_name);
    void SetLocation(String entity_name, float x, float y, float z);
    void SetSunDirection(float x, float y, float z);
    void SetSunIntensity(float intensity);
}

#endif // __LEEP_HELPER_FUNCTIONS_H__
