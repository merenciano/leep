#include "leep.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

using namespace leep;

void Init()
{
    Logger::Init();
    GM.init();
    LuaScripting::Init();
    DisplayList init_dl;
    Texture trex_texture;
    Geometry cube_geo;

    trex_texture.create("../assets/tex/trex.jpg");
    cube_geo.createCube();

    init_dl.addCommand<RenderOptions>()
        .set_depth(true, true)
        .set_cull_face(CullFace::BACK)
        .set_blend(BlendFunc::ONE, BlendFunc::ZERO);

    init_dl.submit();

    PbrData pbr;
    pbr.tiling_x_ = 1.0f;
    pbr.tiling_y_ = 1.0f;

    GM.memory().createContainer(EntityType::FALLING_CUBE);
    GM.memory().createContainer(EntityType::RENDERABLE);

    for (int32_t i = 0; i < 3; ++i)
    {
        for(int32_t j = 0; j < 3; ++j)
        {
            Entity e = Entity::CreateEntity(
                "Cube_" + std::to_string(i) + "_" + std::to_string(j), EntityType::FALLING_CUBE);

            LTransform &tr = e.getComponent<LTransform>();
            tr.transform_ = glm::scale(tr.transform_, glm::vec3(0.3f, 0.3f, 0.3f));
            tr.transform_ = glm::translate(tr.transform_, glm::vec3(1.5f * i, -1.1f * j, -5.0f));

            Drawable &cube_dw = e.getComponent<Drawable>();
            cube_dw.geometry_ = cube_geo;
            cube_dw.material_.set_type(MaterialType::MT_PBR);
            cube_dw.material_.set_data(pbr);
            cube_dw.material_.set_texture(trex_texture);

            e.getComponent<FallSpeed>().speed_ = 0.1f;
            e.getComponent<InfiniteFallingLimits>().limit_down_ = -15.0f;
            e.getComponent<InfiniteFallingLimits>().limit_up_= 15.0f;
        }
    }

    Entity e = Entity::CreateEntity("1", EntityType::RENDERABLE);
    LTransform &tr = e.getComponent<LTransform>();
    tr.transform_ = glm::scale(tr.transform_, glm::vec3(0.3f, 0.3f, 0.3f));
    Drawable &cube_dw = e.getComponent<Drawable>();
    cube_dw.geometry_ = cube_geo;
    cube_dw.material_.set_type(MaterialType::MT_PBR);
    cube_dw.material_.set_data(pbr);
    cube_dw.material_.set_texture(trex_texture);

    GM.scene_graph().createNode(&e.getComponent<LTransform>(), &e.getComponent<GTransform>());

    Entity child = Entity::CreateEntity("2", EntityType::RENDERABLE);
    LTransform &child_tr = child.getComponent<LTransform>();
    child_tr.transform_ = glm::scale(child_tr.transform_, glm::vec3(1.0f, 1.0f, 1.0f));
    child_tr.transform_ = glm::translate(child_tr.transform_, glm::vec3(3.0f, 0.0f, 0.0f));
    Drawable &child_dw = child.getComponent<Drawable>();
    child_dw.geometry_ = cube_geo;
    child_dw.material_.set_type(MaterialType::MT_PBR);
    child_dw.material_.set_data(pbr);
    child_dw.material_.set_texture(trex_texture);

    GM.scene_graph().createNode(&child.getComponent<LTransform>(), &child.getComponent<GTransform>());
    GM.scene_graph().setParent(&child.getComponent<LTransform>(), &e.getComponent<LTransform>());

    LuaScripting::ExecuteScript("../assets/scripts/init.lua");

    Entity::RemoveEntity("Cube_1_1");
    Entity::RemoveEntity("1");
}

void Logic()
{
    DisplayList dl;
    Entity::GetEntity("2").getComponent<LTransform>().rotateYWorld(0.01f);

    Chrono logic_timer;
    GM.input().updateInput();
    CameraMovement(1.0f, 1.0f).executeSystem();
    logic_timer.start();
    Fall(GM.memory().container(EntityType::FALLING_CUBE)).executeSystem();
    InfiniteFalling(GM.memory().container(EntityType::FALLING_CUBE)).executeSystem();
    UpdateTransform(GM.memory().container(EntityType::FALLING_CUBE)).executeSystem();
    UpdateTransform(GM.memory().container(EntityType::RENDERABLE)).executeSystem();
    UpdateSceneGraph().executeSystem();

    dl.addCommand<Clear>()
        .set_clear_buffer(true, true, true)
        .set_clear_color(0.2f, 0.2f, 0.2f, 1.0f);
    dl.submit();

    Render(GM.memory().container(EntityType::FALLING_CUBE)).executeSystem();
    Render(GM.memory().container(EntityType::RENDERABLE)).executeSystem();
    logic_timer.end();
    int64_t duration = logic_timer.duration();
#ifdef LEEP_DEBUG
    LEEP_INFO("Logic time in microseconds: {0}", duration);
#else
    // Logger works only on debug
    printf("Logic time in microseconds: %d\n", duration);
#endif
}

void RenderScene()
{
    Manager::instance().renderer().renderFrame();
    GM.ui_tools().update(GM.window().delta_time(), GM.window().width(), GM.window().height());
}

int main()
{
    Function logic = Logic;
    Init();

    Logic();
    GM.renderer().submitFrame();

    while (!GM.window().windowShouldClose())
    {
        Thread l(logic);
        RenderScene();
        l.join();
        GM.renderer().submitFrame();

        GM.window().swap();
        GM.window().pollEvents();
    }

    return 0;
}
