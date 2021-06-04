#include "create-buffer.h"
#include "core/common-defs.h"

namespace leep 
{
    CreateBuffer::CreateBuffer()
    {
    }

    CreateBuffer::~CreateBuffer()
    {
    }

    CreateBuffer& CreateBuffer::set_buffer(const Buffer &buffer)
    {
        buffer_ = buffer;
        return *this;
    }

}

#ifdef LEEP_OPENGL
    #include "render/commands/opengl/opengl-create-buffer.cpp"
#endif 

#ifdef LEEP_OPENGL_ES
    #include "render/commands/opengl/opengl-create-buffer.cpp"
#endif 