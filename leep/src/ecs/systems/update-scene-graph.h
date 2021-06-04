#ifndef __LEEP_ECS_SYSTEMS_UPDATE_SCENE_GRAPH_H__
#define __LEEP_ECS_SYSTEMS_UPDATE_SCENE_GRAPH_H__

#include "ecs/system.h"
#include "core/scene-graph.h"

namespace leep
{
    class UpdateSceneGraph : public System
    {
    public:
        UpdateSceneGraph();
        UpdateSceneGraph(SceneGraph &sg);
        ~UpdateSceneGraph() = default;
        virtual void executeSystem() const override;

    private:
        SceneGraph &sg_;
    };
}

#endif // __LEEP_ECS_SYSTEMS_UPDATE_SCENE_GRAPH_H__
