// CORE
#include "../src/core/common-defs.h"
#include "../src/core/manager.h"
#include "../src/core/window.h"  // Inside manager.h, not needed here
#include "../src/core/logger.h"

// ECS
#include "../src/ecs/entity.h"
#include "../src/ecs/component.h"
#include "../src/ecs/system.h"
#include "../src/ecs/internal-entity.h"
#include "../src/ecs/components/drawable.h"
#include "../src/ecs/components/transform.h"
#include "../src/ecs/systems/render.h"

// RENDER
#include "../src/render/renderer.h" // Inside manager.h, not needed here
#include "../src/render/display-list.h"
#include "../src/render/display-list-command.h"

#include "../src/render/commands/clear.h"
#include "../src/render/commands/create-buffer.h"
#include "../src/render/commands/init-material.h"
#include "../src/render/commands/use-pbr-material.h"
#include "../src/render/commands/use-plain-color-material.h"
#include "../src/render/commands/draw.h"


// Utility functions
namespace motoret
{
}