#ifndef __LEEP_CORE_MANAGER_H__
#define __LEEP_CORE_MANAGER_H__ 1

#define GM Manager::instance()

namespace leep 
{
    class Manager
    {
    public:
        static Manager& instance() { static Manager m; return m; }

        // Not deleting data because this the unique instance of this class
        // will live until the end of the program
        ~Manager() {}

        void init();
        void nextFrame();
        void startFrameTimer();

        class Window&      window();
        class Renderer&    renderer();
        class Camera&      camera(); // TODO: Move camera to renderer
        class Input&       input();
        class Memory&      memory();
        class ImguiTools&  ui_tools();
        class SceneGraph&  scene_graph();
        class ResourceMap& resource_map();
        struct ToolsData&  tools_data();
        float delta_time() const;

    private:
        Manager() = default;
        Manager(const Manager&) = default;
        Manager& operator=(const Manager&) { return *this; }

        struct ManagerData;
        ManagerData *data_;
    };
}

#endif // __LEEP_CORE_MANAGER_H__
