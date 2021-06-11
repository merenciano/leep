#include "internal-resources.h"
#include "core/manager.h"
#include "core/memory/memory.h"

static void LoadFile(const char *path, char **buffer) noexcept
{
    int32_t length;
    FILE *fp = fopen(path, "r");

    if (fp)
    {
        fseek (fp, 0, SEEK_END);
        length = ftell (fp);
        fseek (fp, 0, SEEK_SET);
        *buffer = (char*)leep::GM.memory().generalAlloc(length+1);
        if (*buffer)
        {
            memset(*buffer, '\0', length + 1);
            fread(*buffer, 1, length, fp);
            // TODO: Check wtf is happening here because
            // the fucking VisualStudio is throwing exception
            // here. When fixed remember to delete the memset call
            //*buffer[length] = '\0';
        }
        fclose(fp);
    }
}

namespace leep
{
    uint32_t InternalMaterial::internal_id() const
    {
        return internal_id_;
    }
}

#if defined(LEEP_OPENGL) || defined(LEEP_OPENGL_ES)
    #include "api-impl/internal-material-gl.cpp"
#endif