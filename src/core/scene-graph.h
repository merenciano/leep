// Lucas Merenciano Martinez  <lucasmermar@gmail.com>

#ifndef __LEEP_CORE_SCENE_GRAPH_H__
#define __LEEP_CORE_SCENE_GRAPH_H__

#include "ecs/components/ltransform.h"
#include "ecs/components/gtransform.h"

#include <unordered_map>
#include <list>

namespace leep
{
    struct SceneGraphNode
    {
        LTransform *parent = nullptr;
        GTransform *global = nullptr;
        std::list<LTransform*> childs;
    };

    class SceneGraph 
    {
    public:
        SceneGraph();
        ~SceneGraph();

        void addNode(LTransform *tr);
        void removeNode(LTransform *tr);
        void setParent(LTransform *tr, LTransform *parent);
        void detachFromParent(LTransform *tr);
        bool exists(LTransform *tr) const;
        void reset();


    private:
        std::unordered_map<LTransform*, SceneGraphNode> map_;
    };
}

#endif // __LEEP_CORE_SCENE_GRAPH_H__