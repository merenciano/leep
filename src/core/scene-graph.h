// Lucas Merenciano Martinez  <lucasmermar@gmail.com>

#ifndef __LEEP_CORE_SCENE_GRAPH_H__
#define __LEEP_CORE_SCENE_GRAPH_H__

#include "core/memory/buddy-alloc-stl.h"
#include "ecs/components/ltransform.h"
#include "ecs/components/gtransform.h"

#include <unordered_map>
#include <list>

namespace leep
{
    struct SceneGraphNode
    {
        LTransform *parent = nullptr;
        LTransform *self   = nullptr;
        GTransform *global = nullptr;
        std::list<LTransform*, BuddySTL<LTransform*>> childs;
    };

    class SceneGraph 
    {
    public:
        SceneGraph();
        ~SceneGraph();

        // Creates a default node using the transform as key
        void createNode(LTransform *tr, GTransform *gtr);

        // When a node is removed, the entity will lose its transform hierarchy
        // and the local transform will act as a global transform.
        void removeNode(LTransform *tr);

        void setParent(LTransform *tr, LTransform *parent);

        // Detach the node from its parent.
        // This could move the entity.
        void detachFromParent(LTransform *tr);

        /*  When an entity is removed, the last entity of the container moves
            its components to the free spot in order to fill the gap.
            When that happens the local transform pointer changes its value
            so it needs to be updated in the scene graph with this method.    */
        void changeTransform(LTransform *old_tr,
                             LTransform *new_tr,
                             GTransform *new_gtr);

        // Checks if there is an entry with that key in the dictionary
        bool exists(LTransform *tr) const;

        // Clears the scene graph.
        // Caution: All the transform hierarchy of the scene will be lost!
        void reset();
    
        std::unordered_map<
            LTransform*,
            SceneGraphNode,
            std::hash<LTransform*>,
            std::equal_to<LTransform*>,
            BuddySTL<std::pair<LTransform* const, SceneGraphNode>>> map_;
    };
}

#endif // __LEEP_CORE_SCENE_GRAPH_H__