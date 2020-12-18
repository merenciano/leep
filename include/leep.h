// CORE
#include "../src/core/common-defs.h"
#include "../src/core/manager.h"
#include "../src/core/window.h"  // Inside manager.h, not needed here
#include "../src/core/logger.h"
#include "../src/core/chrono.h"
#include "../src/core/memory.h"

// ECS
#include "../src/ecs/entity.h"
#include "../src/ecs/component.h"
#include "../src/ecs/system.h"
#include "../src/ecs/internal-entity.h"
#include "../src/ecs/components/drawable.h"
#include "../src/ecs/components/transform.h"
#include "../src/ecs/components/fall-speed.h"
#include "../src/ecs/components/infinite-falling-limits.h"
#include "../src/ecs/systems/render.h"
#include "../src/ecs/systems/camera-movement.h"
#include "../src/ecs/systems/fall.h"
#include "../src/ecs/systems/infinite-falling.h"

// RENDER
#include "../src/render/renderer.h" // Inside manager.h, not needed here
#include "../src/render/texture.h"
#include "../src/render/display-list.h"
#include "../src/render/display-list-command.h"

#include "../src/render/commands/clear.h"
#include "../src/render/commands/create-buffer.h"
#include "../src/render/commands/init-material.h"
#include "../src/render/commands/use-pbr-material.h"
#include "../src/render/commands/use-plain-color-material.h"
#include "../src/render/commands/draw.h"
#include "../src/render/commands/create-texture.h"
#include "../src/render/commands/render-options.h"


// Utility functions
namespace leep 
{
    inline int32_t EntityI(int32_t entity_id)
    {
        // %kEntitiesPerChunk
        return entity_id&(kEntitiesPerChunk - 1);
    }

    inline int32_t ChunkI(int32_t entity_id)
    {
        return entity_id/kEntitiesPerChunk;
    }
}