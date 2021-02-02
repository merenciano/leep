#include "scene-graph.h"
#include "core/common-defs.h"


namespace leep
{
    SceneGraph::SceneGraph()
    {

    }

    SceneGraph::~SceneGraph()
    {

    }

    void SceneGraph::createNode(LTransform *tr, GTransform *gtr)
    {
        if (!exists(tr))
        {
            SceneGraphNode new_node;
            new_node.self = tr;
            new_node.global = gtr;
            map_[tr] = new_node;
        }
    }

    void SceneGraph::setParent(LTransform *tr, LTransform *parent)
    {
        LEEP_ASSERT(exists(tr), "The entity has to have a node");
        LEEP_ASSERT(exists(parent), "The parent has to have a node");
        detachFromParent(tr);
        
        map_[tr].parent = parent;
        map_[parent].childs.push_back(tr);
    }

    void SceneGraph::removeNode(LTransform *tr)
    {
        // Remove from parent's childs
        if (map_[tr].parent)
        {
            map_[map_[tr].parent].childs.remove(tr);
        }
        // Remove from childs parent
        for (LTransform *c : map_[tr].childs)
        {
            map_[c].parent = nullptr;
        }
        map_.erase(tr);
    }
    
    void SceneGraph::detachFromParent(LTransform *tr)
    {
        if (map_[tr].parent)
        {
            map_[map_[tr].parent].childs.remove(tr);
            map_[tr].parent = nullptr;
        }
    }

    void SceneGraph::changeTransform(LTransform *old_tr, LTransform *new_tr, GTransform *new_gtr)
    {
        if (exists(old_tr))
        {
            // First of all create the new node
            createNode(new_tr, new_gtr);
            // Set the parent
            setParent(new_tr, map_[old_tr].parent);
            // Set the childs
            for (LTransform *t : map_[old_tr].childs)
            {
                setParent(t, new_tr);
            }
            // Remove old node
            removeNode(old_tr);
        }
    }

    bool SceneGraph::exists(LTransform *tr) const
    {
        return map_.find(tr) != map_.end(); 
    }

    void SceneGraph::reset()
    {
        map_.clear();
    }
}