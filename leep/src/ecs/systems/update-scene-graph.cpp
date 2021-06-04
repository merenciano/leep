#include "update-scene-graph.h"
#include "core/common-defs.h"
#include "core/manager.h"
#include "core/scene.h"

namespace leep
{
    static void UpdateGlobalTransforms(SceneGraph &sg, LTransform *tr)
    {
        sg.map_[tr].global->gtr_ = sg.map_[sg.map_[tr].parent].global->gtr_ * tr->transform_;
        for (LTransform *c : sg.map_[tr].childs)
        {
            UpdateGlobalTransforms(sg, c);
        }
    }

    UpdateSceneGraph::UpdateSceneGraph() : sg_(GM.scene().scene_graph_)
    {
    }

    UpdateSceneGraph::UpdateSceneGraph(SceneGraph &sg) : sg_(sg)
    {
    }

    void UpdateSceneGraph::executeSystem() const
    {
        auto it = sg_.map_.begin();
        while(it != sg_.map_.end())
        {
            if (it->second.parent == nullptr)
            {
                if (it->second.childs.empty())
                {
                    // This entity will behave the same if it's not in this graph
                    it = sg_.map_.erase(it);
                    continue;
                }

                //for (LTransform *c : it->second.childs)
                for (LTransform* c : sg_.map_[it->first].childs)
                {
                    UpdateGlobalTransforms(sg_, c);
                }
            }
            ++it;
        }
    }
}