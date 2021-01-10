#include "core/scene-graph.h"
#include "core/common-defs.h"


namespace leep
{
    SceneGraph::SceneGraph()
    {

    }

    SceneGraph::~SceneGraph()
    {

    }

    void SceneGraph::addNode(Transform *tr)
    {
        LEEP_ASSERT(!exists(tr), "That node exists already");
        map_[tr] = SceneGraphNode();
    }

    void SceneGraph::setParent(Transform *tr, Transform *parent)
    {
        if (!exists(parent))
        {
            addNode(parent);
        }
        else
        {
            detachFromParent(tr);
        }
        
        map_[tr].parent = parent;
        map_[parent].childs.push_back(tr);
    }

    void SceneGraph::removeNode(Transform *tr)
    {
        // Remove from parent's childs
        if (map_[tr].parent)
        {
            map_[map_[tr].parent].childs.remove(tr);
        }
        // Remove from childs parent
        for (Transform *c : map_[tr].childs)
        {
            map_[c].parent = nullptr;
        }
        map_.erase(tr);
    }
    
    void SceneGraph::detachFromParent(Transform *tr)
    {
        if (map_[tr].parent)
        {
            map_[map_[tr].parent].childs.remove(tr);
            map_[tr].parent = nullptr;
        }
    }

    bool SceneGraph::exists(Transform *tr) const
    {
        return map_.find(tr) != map_.end(); 
    }

    void SceneGraph::reset()
    {
        map_.clear();
    }
}