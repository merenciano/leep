#include "create-buffer.h"
#include "core/common-defs.h"

namespace motoret
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

#ifdef MOTORET_OPENGL
    #include "opengl/opengl-create-buffer.cpp"
#endif 