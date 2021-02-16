#include "leep.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

void leep::Init()
{
    {
        // Resource creation CPU side (GPU allocation will occur on first use)
        ResourceMap &rm = GM.resource_map();
        rm.addTexture("Skybox"    , "../assets/tex/skybox", TexType::CUBE);
        rm.addTexture("Cerberus_A", "../assets/tex/cerberus_A.png", TexType::SRGB);
        rm.addTexture("Cerberus_N", "../assets/tex/cerberus_N.png", TexType::RGB);
        rm.addTexture("Cerberus_M", "../assets/tex/cerberus_M.png", TexType::RGB);
        rm.addTexture("Cerberus_R", "../assets/tex/cerberus_R.png", TexType::RGB);
        rm.addGeometry("Cerberus" , "../assets/obj/cerberus.obj");
    }

    GM.memory().createContainer(EntityType::RENDERABLE);

    PbrData mat_data;
    mat_data.color_ = glm::vec3(1.0f, 0.0f, 0.0f);
    mat_data.use_albedo_map_ = 1.0f;
    mat_data.use_pbr_maps_ = 1.0f;
    mat_data.tiling_x_ = 1.0f;
    mat_data.tiling_y_ = 1.0f;
    mat_data.metallic_ = 0.5f;
    mat_data.roughness_ = 0.4f;
    mat_data.reflectance_ = 0.5f;

    Entity e = Entity::CreateEntity("Pipa", EntityType::RENDERABLE);
    LTransform &tr = e.getComponent<LTransform>();
    tr.transform_ = glm::scale(tr.transform_,
                    glm::vec3(0.03f, 0.03f, 0.03f));
    Drawable &dw = e.getComponent<Drawable>();
    dw.geometry_ = GM.resource_map().getGeometry("Cerberus");
    dw.material_.set_type(MaterialType::MT_PBR);
    dw.material_.set_data(mat_data);
    dw.material_.set_albedo(GM.resource_map().getTexture("Cerberus_A"));
    dw.material_.set_metallic(GM.resource_map().getTexture("Cerberus_M"));
    dw.material_.set_roughness(GM.resource_map().getTexture("Cerberus_R"));
    dw.material_.set_normal(GM.resource_map().getTexture("Cerberus_N"));
}

void leep::Logic()
{
    GM.input().updateInput();
    CameraMovement(1.0f, 1.0f).executeSystem();
    UpdateTransform(GM.memory().container(EntityType::RENDERABLE)).executeSystem();
    UpdateSceneGraph().executeSystem();

    // Render commands
    DisplayList dl;
    PbrSceneData pbr_sd;
    pbr_sd.view_projection_ = GM.camera().view_projection();
    pbr_sd.camera_position_ = GM.camera().position();
    pbr_sd.light_direction_intensity_ = glm::vec4(1.0f, 1.0f, 0.0f, 50.0f);

    Material full_screen_img;
    full_screen_img.set_type(MaterialType::MT_FULL_SCREEN_IMAGE);
    full_screen_img.set_albedo(GM.camera().framebuffer().color());
    
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

    dl.addCommand<UsePbrMaterial>()
        .set_scene_data(pbr_sd);

    dl.submit();

    Render(GM.memory().container(EntityType::RENDERABLE)).executeSystem();
    
    dl.addCommand<UseSkyboxMaterial>();
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
