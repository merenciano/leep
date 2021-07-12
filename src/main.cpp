#include "leep.h"
#include "systems/draw-voxels.h"
#include "dungeon.h"

//#define LEEP_SINGLE_THREAD
using namespace clicktaclysm;
Dungeon g_dun;

void leep::GameInit()
{
    ResourceMap &rm = GM.resource_map();
    const String tp = "../assets/tex/";
    rm.addGeometry("MatBall", "../assets/obj/matball-n.obj");
    rm.addTexture("Skybox", 1024.0f, 1024.0f, TexType::ENVIRONMENT);
    rm.addTexture("IrradianceEnv", 1024.0f, 1024.0f, TexType::ENVIRONMENT);
    rm.addTexture("PrefilterSpec", 128.0f, 128.0f, TexType::PREFILTER_ENVIRONMENT);
    rm.addTexture("LutMap", 512.0f, 512.0f, TexType::LUT);
    rm.addTexture("Default", tp + "default.jpg", TexType::SRGB);

    GM.renderer().sun_dir_intensity_ = glm::vec4(0.0f, -1.0f, -0.1f, 1.0f);
    GM.renderer().initMaterial(MaterialType::MT_VOXMAT, "voxmat");

    GM.scene().createContainer(EntityType::RENDERABLE);

    DisplayList init_dl;
    init_dl.addCommand<RenderOptions>()
        .enable_depth_test(true)
        .enable_write_depth(true)
        .set_cull_face(CullFace::BACK)
        .enable_blend(true)
        .set_blend_func(BlendFunc::ONE, BlendFunc::ZERO);
    init_dl.addCommand<EquirectangularToCubemap>()
        .set_in_path("../assets/tex/env/helipad-env.hdr")
        .set_out_cube(GM.resource_map().getTexture("Skybox"))
        .set_out_prefilter(GM.resource_map().getTexture("PrefilterSpec"))
        .set_out_lut(GM.resource_map().getTexture("LutMap"));
    init_dl.addCommand<EquirectangularToCubemap>()
        .set_in_path("../assets/tex/env/helipad-dif.hdr")
        .set_out_cube(GM.resource_map().getTexture("IrradianceEnv"));
    init_dl.submit();

    g_dun.generate(128, 128);

}

void leep::GameLogic()
{
    LuaScripting::ExecuteScript("../assets/scripts/update.lua");
    GM.input().updateInput();
    CameraMovement(1.0f, 20.0f).executeSystem();
    UpdateTransform(GM.scene().container(EntityType::RENDERABLE)).executeSystem();
    UpdateSceneGraph().executeSystem();

    // Render commands
    DisplayList dl;
    VoxMatSceneData voxmat_sd;
    voxmat_sd.vp_ = GM.camera().view_projection();
    voxmat_sd.camera_position_ = GM.camera().position();
    voxmat_sd.light_direction_intensity_ = GM.renderer().sun_dir_intensity_;

    Material full_screen_img;
    full_screen_img.set_type(MaterialType::MT_FULL_SCREEN_IMAGE);
    Texture fb_tex = GM.camera().framebuffer().color();
    full_screen_img.set_tex(&fb_tex, 1);
    
    dl.addCommand<UseFramebuffer>()
        .set_framebuffer(GM.camera().framebuffer());

    dl.addCommand<RenderOptions>()
        .enable_depth_test(true)
        .enable_write_depth(true)
        .set_cull_face(CullFace::BACK)
        .enable_blend(true)
        .set_blend_func(BlendFunc::ONE, BlendFunc::ZERO);

    dl.addCommand<Clear>()
        .set_clear_buffer(true, true, false)
        .set_clear_color(0.2f, 0.2f, 0.2f, 1.0f);

    // TODO this out of loop (this textures do not change)
    Texture scene_tex[3];
    scene_tex[0] = GM.resource_map().getTexture("LutMap");
    scene_tex[1] = GM.resource_map().getTexture("IrradianceEnv");
    scene_tex[2] = GM.resource_map().getTexture("PrefilterSpec");
    dl.addCommand<UseMaterial>()
        .set_type(MaterialType::MT_VOXMAT)
        .set_float((float*)&voxmat_sd, sizeof(VoxMatSceneData) / sizeof (float))
        .set_tex(scene_tex, 3, 1);

    dl.submit();

    //Render(GM.scene().container(EntityType::RENDERABLE)).executeSystem();

    DrawVoxels().set_offset_buffer(g_dun.ground_buf_, g_dun.vox_count_).executeSystem();
    
    dl.addCommand<RenderOptions>()
        .set_cull_face(CullFace::DISABLED);
    dl.addCommand<DrawSkybox>()
        .set_cubemap(GM.resource_map().getTexture("Skybox"));
   
    dl.addCommand<UseFramebuffer>();

    dl.addCommand<RenderOptions>()
        .enable_depth_test(false);

    dl.addCommand<Clear>()
        .set_clear_buffer(true, false, false)
        .set_clear_color(1.0f, 0.0f, 0.0f, 1.0f);

    dl.addCommand<Draw>()
        .set_geometry(GM.resource_map().getGeometry("Quad"))
        .set_material(full_screen_img);
        
    dl.submit();
}

void leep::GameClose()
{

}

int main(int argc, char **argv)
{
    leep::LeepConfig cnfg;
    leep::Init(cnfg);
    leep::LogicThread lt(leep::Logic);
    leep::GM.startFrameTimer();
    while (!leep::GM.window().windowShouldClose())
    {
#ifndef LEEP_SINGLE_THREAD
        lt.run();
        leep::RenderFrame();
        while (lt.running())
        {
            std::this_thread::sleep_for(std::chrono::microseconds(100));
        }
#else
        leep::Logic();
        leep::RenderFrame();
#endif
        leep::ShowFrame();
    }
    leep::Close();

    return 0;
}
