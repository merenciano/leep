#include "leep.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#define LEEP_SINGLE_THREAD 0

using namespace leep;

void Init()
{
    Logger::Init();
    GM.init();
    LuaScripting::Init();
    GM.resource_map().addTexture("Skybox", "../assets/tex/skybox", true);
    GM.resource_map().addTexture("T-Rex", "../assets/tex/trex.jpg");


    PbrData pbr;
    pbr.tiling_x_ = 1.0f;
    pbr.tiling_y_ = 1.0f;
	pbr.metallic_ = 0.5f;
	pbr.roughness_ = 0.5f;
	pbr.reflectance_ = 0.5f;
	pbr.color_ = glm::vec3(1.0f, 0.0f, 0.0f);
	pbr.use_albedo_map_ = 0.0f;

    GM.memory().createContainer(EntityType::RENDERABLE);

    for (int32_t i = 0; i < 6; ++i)
    {
        for(int32_t j = 0; j < 6; ++j)
        {
			PbrData mat_data;
			mat_data.color_ = glm::vec3(1.0f, 0.0f, 0.0f);
			mat_data.use_albedo_map_ = 0.0f;
			mat_data.tiling_x_ = 1.0f;
			mat_data.tiling_y_ = 1.0f;
			mat_data.metallic_ = i / 6.0f;
			mat_data.roughness_ = j / 6.0f;
			mat_data.reflectance_ = 0.5f;

			Entity e = Entity::CreateEntity("Sph" + std::to_string(i) + "_" + std::to_string(j), 
											EntityType::RENDERABLE);
			LTransform &tr = e.getComponent<LTransform>();
			tr.transform_ = glm::scale(tr.transform_, glm::vec3(0.2f, 0.2f, 0.2f));
			tr.transform_ = glm::translate(tr.transform_, glm::vec3(i * 2.0f , 0.0f, j * 2.0f));
			Drawable &dw = e.getComponent<Drawable>();
			dw.geometry_ = GM.resource_map().getGeometry("Sphere");
			dw.material_.set_type(MaterialType::MT_PBR);
			dw.material_.set_data(mat_data);
			dw.material_.set_texture(GM.resource_map().getTexture("T-Rex"));
        }
    }

    /*Entity e = Entity::CreateEntity("1", EntityType::RENDERABLE);
    LTransform &tr = e.getComponent<LTransform>();
    tr.transform_ = glm::scale(tr.transform_, glm::vec3(0.3f, 0.3f, 0.3f));
    Drawable &cube_dw = e.getComponent<Drawable>();
    cube_dw.geometry_ = GM.resource_map().getGeometry("Sphere");
    cube_dw.material_.set_type(MaterialType::MT_PBR);
    cube_dw.material_.set_data(pbr);
    cube_dw.material_.set_texture(GM.resource_map().getTexture("T-Rex"));

    GM.scene_graph().createNode(&e.getComponent<LTransform>(), &e.getComponent<GTransform>());

    Entity child = Entity::CreateEntity("2", EntityType::RENDERABLE);
    LTransform &child_tr = child.getComponent<LTransform>();
    child_tr.transform_ = glm::scale(child_tr.transform_, glm::vec3(1.0f, 1.0f, 1.0f));
    child_tr.transform_ = glm::translate(child_tr.transform_, glm::vec3(3.0f, 0.0f, 0.0f));
    Drawable &child_dw = child.getComponent<Drawable>();
    child_dw.geometry_ = GM.resource_map().getGeometry("Sphere");
    child_dw.material_.set_type(MaterialType::MT_PBR);
    child_dw.material_.set_data(pbr);
    child_dw.material_.set_texture(GM.resource_map().getTexture("T-Rex"));

    GM.scene_graph().createNode(&child.getComponent<LTransform>(), &child.getComponent<GTransform>());
    GM.scene_graph().setParent(&child.getComponent<LTransform>(), &e.getComponent<LTransform>());
	*/

    LuaScripting::ExecuteScript("../assets/scripts/init.lua");
}

void Logic()
{
    Chrono logic_timer;

    logic_timer.start();
    //Entity::GetEntity("2").getComponent<LTransform>().rotateYWorld(1.0f * GM.delta_time());

    LuaScripting::ExecuteScript("../assets/scripts/update.lua");

    GM.input().updateInput();
    CameraMovement(1.0f, 1.0f).executeSystem();
    UpdateTransform(GM.memory().container(EntityType::RENDERABLE)).executeSystem();
    UpdateSceneGraph().executeSystem();

    // Render commands
    DisplayList dl;
    PbrSceneData pbr_sd;
    pbr_sd.view_projection_ = GM.camera().view_projection();
	pbr_sd.camera_position_ = GM.camera().position();
    pbr_sd.light_direction_intensity_ = glm::vec4(0.0f, 1.0f, 0.0f, 2.0f);

    Material full_screen_img;
    full_screen_img.set_type(MaterialType::MT_FULL_SCREEN_IMAGE);
    full_screen_img.set_texture(GM.camera().framebuffer().color());

    dl.addCommand<UseFramebuffer>()
        .set_framebuffer(GM.camera().framebuffer());

    // Dear ImGui overrides them so I call the command once per frame
    dl.addCommand<RenderOptions>()
        .set_depth(true, true)
        .set_cull_face(CullFace::BACK)
        .set_blend(BlendFunc::ONE, BlendFunc::ZERO);

    dl.addCommand<Clear>()
        .set_clear_buffer(true, true, true)
        .set_clear_color(0.2f, 0.2f, 0.2f, 1.0f);

    dl.addCommand<UsePbrMaterial>()
        .set_scene_data(pbr_sd);

    dl.submit();

    Render(GM.memory().container(EntityType::RENDERABLE)).executeSystem();

    dl.addCommand<UseSkyboxMaterial>();
    dl.addCommand<DrawSkybox>()
        .set_cubemap(GM.resource_map().getTexture("Skybox"));
        
    dl.addCommand<UseFramebuffer>();

    dl.addCommand<Clear>()
        .set_clear_buffer(true, true, true)
        .set_clear_color(1.0f, 0.0f, 0.0f, 1.0f);

    dl.addCommand<Draw>()
        .set_geometry(GM.resource_map().getGeometry("Quad"))
        .set_material(full_screen_img);

    dl.submit();

    logic_timer.end();
    GM.ui_tools().calcLogicAverage(logic_timer.duration());
}

void RenderScene()
{
    Chrono render_timer;
    render_timer.start();
    Manager::instance().renderer().renderFrame();

    GM.ui_tools().update();
    render_timer.end();
    GM.ui_tools().calcRenderAverage(render_timer.duration());
}

int main()
{
    Chrono init_timer;
    Function logic = Logic;
    init_timer.start();
    Init();
    init_timer.end();
    GM.tools_data().init_time_ms_ = init_timer.duration();
    LEEP_CORE_INFO("Init time: {0} ms", init_timer.duration());

    GM.startFrameTimer();
    while (!GM.window().windowShouldClose())
    {
#if LEEP_SINGLE_THREAD == 0
        Thread l(logic);
        RenderScene();
        l.join();
#elif LEEP_SINGLE_THREAD == 1
        Logic();
        RenderScene();
#endif
        GM.nextFrame();
    }

    return 0;
}
