#ifndef __LEEP_CORE_MANAGER_H__
#define __LEEP_CORE_MANAGER_H__ 1


#define GM Manager::instance()

namespace leep 
{
    class Manager
    {
    public:
        static Manager& instance() { static Manager m; return m; }

        ~Manager() {}

        void init();

        struct Window&      window();
        struct Renderer&    renderer();
        struct Camera&      camera();
        struct Input&       input();
        struct Memory&      memory();
        struct ImguiTools&  ui_tools();
        struct SceneGraph&  scene_graph();
        struct ResourceMap& resource_map();

    private:
    
    private:
        Manager() {}
        Manager(const Manager&) {}
        Manager& operator=(const Manager&) { return *this; }

        struct ManagerData;
        ManagerData *data_;
    };
}

#endif // __LEEP_CORE_MANAGER_H__
