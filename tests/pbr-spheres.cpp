#include "leep.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

void leep::Init()
{
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

            Entity e = Entity::CreateEntity(
                         "Sph" + std::to_string(i) +
                         "_" + std::to_string(j), 
                         EntityType::RENDERABLE);
            LTransform &tr = e.getComponent<LTransform>();
            tr.transform_ = glm::scale(tr.transform_,
                         glm::vec3(0.2f, 0.2f, 0.2f));
            tr.transform_ = glm::translate(tr.transform_,
                         glm::vec3(i * 2.0f , 0.0f, j * 2.0f));
            Drawable &dw = e.getComponent<Drawable>();
            dw.geometry_ = GM.resource_map().getGeometry("Sphere");
            dw.material_.set_type(MaterialType::MT_PBR);
            dw.material_.set_data(mat_data);
            dw.material_.set_texture(GM.resource_map().getTexture("T-Rex"));
        }
    }
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
    pbr_sd.light_direction_intensity_ = glm::vec4(0.0f, 1.0f, 0.0f, 2.0f);

    Material full_screen_img;
    full_screen_img.set_type(MaterialType::MT_FULL_SCREEN_IMAGE);
    full_screen_img.set_texture(GM.camera().framebuffer().color());
    
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
