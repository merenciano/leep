#include "create-buffer.h"
#include "core/common-defs.h"

namespace motoret
{
    CreateBuffer::CreateBuffer()
    {
    }

    CreateBuffer::CreateBuffer(Buffer b)
    {
        buffer_ = b;
    }

    CreateBuffer::~CreateBuffer()
    {
    }

}

#ifdef MOTORET_OPENGL
    #include "opengl/opengl-create-buffer.cpp"
#endif 