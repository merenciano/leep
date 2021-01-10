// Lucas Merenciano Martinez  <lucasmermar@gmail.com>

#ifndef __LEEP_CORE_SCENE_GRAPH_H__
#define __LEEP_CORE_SCENE_GRAPH_H__

#include "ecs/components/transform.h"

#include <unordered_map>
#include <list>

namespace leep
{
    struct SceneGraphNode
    {
        Transform *parent = nullptr;
        std::list<Transform*> childs;
    };

    class SceneGraph 
    {
    public:
        SceneGraph();
        ~SceneGraph();

        void addNode(Transform *tr);
        void removeNode(Transform *tr);
        void setParent(Transform *tr, Transform *parent);
        void detachFromParent(Transform *tr);
        bool exists(Transform *tr) const;
        void reset();


    private:
        std::unordered_map<Transform*, SceneGraphNode> map_;
    };
}

#endif // __LEEP_CORE_SCENE_GRAPH_H__