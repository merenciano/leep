#include "leep.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

void leep::Init()
{
    ResourceMap &rm = GM.resource_map();
    const String tp = "../assets/tex/";
    rm.addGeometry("MatBall", "../assets/obj/matball-n.obj");
    rm.addTexture("Skybox", 1024.0f, 1024.0f, TexType::ENVIRONMENT);
    rm.addTexture("IrradianceEnv", 1024.0f, 1024.0f, TexType::ENVIRONMENT);
    rm.addTexture("PrefilterSpec", 128.0f, 128.0f, TexType::PREFILTER_ENVIRONMENT);
    rm.addTexture("LutMap", 512.0f, 512.0f, TexType::LUT);

    rm.addTextureAsync("CelticGold_A" ,tp + "celtic-gold/celtic-gold_A.png", TexType::SRGB);
    rm.addTextureAsync("CelticGold_N" ,tp + "celtic-gold/celtic-gold_N.png", TexType::RGB);
    rm.addTextureAsync("CelticGold_M" ,tp + "celtic-gold/celtic-gold_M.png", TexType::RGB);
    rm.addTextureAsync("CelticGold_R" ,tp + "celtic-gold/celtic-gold_R.png", TexType::RGB);
    
    rm.addTextureAsync("Peeling_A" ,tp + "peeling/peeling_A.png", TexType::SRGB);
    rm.addTextureAsync("Peeling_N" ,tp + "peeling/peeling_N.png", TexType::RGB);
    rm.addTextureAsync("Peeling_M" ,tp + "peeling/peeling_M.png", TexType::R);
    rm.addTextureAsync("Peeling_R" ,tp + "peeling/peeling_R.png", TexType::R);

    rm.addTextureAsync("Rusted_A" ,tp + "rusted/rusted_A.png", TexType::SRGB);
    rm.addTextureAsync("Rusted_N" ,tp + "rusted/rusted_N.png", TexType::RGB);
    rm.addTextureAsync("Rusted_M" ,tp + "rusted/rusted_M.png", TexType::R);
    rm.addTextureAsync("Rusted_R" ,tp + "rusted/rusted_R.png", TexType::R);

    rm.addTextureAsync("Tiles_A" ,tp + "tiles/tiles_A.png", TexType::SRGB);
    rm.addTextureAsync("Tiles_N" ,tp + "tiles/tiles_N.png", TexType::RGB);
    rm.addTextureAsync("Tiles_M" ,tp + "tiles/tiles_M.png", TexType::R);
    rm.addTextureAsync("Tiles_R" ,tp + "tiles/tiles_R.png", TexType::R);

    rm.addTextureAsync("ShipPanels_A" ,tp + "ship-panels/ship-panels_A.png", TexType::SRGB);
    rm.addTextureAsync("ShipPanels_N" ,tp + "ship-panels/ship-panels_N.png", TexType::RGB);
    rm.addTextureAsync("ShipPanels_M" ,tp + "ship-panels/ship-panels_M.png", TexType::R);
    rm.addTextureAsync("ShipPanels_R" ,tp + "ship-panels/ship-panels_R.png", TexType::R);

    rm.addTextureAsync("Shore_A" ,tp + "shore/shore_A.png", TexType::SRGB);
    rm.addTextureAsync("Shore_N" ,tp + "shore/shore_N.png", TexType::RGB);
    rm.addTextureAsync("Shore_M" ,tp + "shore/shore_M.png", TexType::R);
    rm.addTextureAsync("Shore_R" ,tp + "shore/shore_R.png", TexType::R);

    rm.addTextureAsync("Cliff_A" ,tp + "cliff/cliff_A.png", TexType::SRGB);
    rm.addTextureAsync("Cliff_N" ,tp + "cliff/cliff_N.png", TexType::RGB);
    rm.addTextureAsync("Cliff_M" ,tp + "cliff/cliff_M.png", TexType::R);
    rm.addTextureAsync("Cliff_R" ,tp + "cliff/cliff_R.png", TexType::R);

    rm.addTextureAsync("Granite_A" ,tp + "granite/granite_A.png", TexType::SRGB);
    rm.addTextureAsync("Granite_N" ,tp + "granite/granite_N.png", TexType::RGB);
    rm.addTextureAsync("Granite_M" ,tp + "granite/granite_M.png", TexType::R);
    rm.addTextureAsync("Granite_R" ,tp + "granite/granite_R.png", TexType::R);

    rm.addTextureAsync("Foam_A" ,tp + "foam/foam_A.png", TexType::SRGB);
    rm.addTextureAsync("Foam_N" ,tp + "foam/foam_N.png", TexType::RGB);
    rm.addTextureAsync("Foam_M" ,tp + "foam/foam_M.png", TexType::R);
    rm.addTextureAsync("Foam_R" ,tp + "foam/foam_R.png", TexType::R);
    
    PbrData pbr;
    pbr.color_ = glm::vec3(1.0f, 1.0f, 1.0f);
    pbr.tiling_x_ = 4.0f;
    pbr.tiling_y_ = 4.0f;
    pbr.use_albedo_map_ = 1.0f;
    pbr.use_pbr_maps_ = 1.0f;
    pbr.metallic_ = 0.5f;
    pbr.roughness_ = 0.5f;
    pbr.normal_map_intensity_ = 1.0f;

    GM.renderer().sun_dir_intensity_ = glm::vec4(0.1f, 0.0f, -1.0f, 2.0f);

    GM.scene().createContainer(EntityType::RENDERABLE);

    // CelticGold
    {
        Entity e = Entity::CreateEntity("CelticGold", EntityType::RENDERABLE);
        LTransform &tr = e.getComponent<LTransform>();
        tr.transform_ = glm::translate(tr.transform_, glm::vec3(2.0f, 0.0f, 0.0f));
        Drawable &dw = e.getComponent<Drawable>();
        dw.geometry_ = GM.resource_map().getGeometry("MatBall");
        dw.material_.set_type(MaterialType::MT_PBR);
        dw.material_.set_data(pbr);
        dw.material_.set_albedo(GM.resource_map().getTexture("CelticGold_A"));
        dw.material_.set_metallic(GM.resource_map().getTexture("CelticGold_M"));
        dw.material_.set_roughness(GM.resource_map().getTexture("CelticGold_R"));
        dw.material_.set_normal(GM.resource_map().getTexture("CelticGold_N"));
    }

    // Shore
    {
        pbr.tiling_x_ = 2.0f;
        pbr.tiling_y_ = 2.0f;
        pbr.normal_map_intensity_ = 0.5f;
        Entity e = Entity::CreateEntity("Shore", EntityType::RENDERABLE);
        LTransform &tr = e.getComponent<LTransform>();
        tr.transform_ = glm::translate(tr.transform_, glm::vec3(4.0f, 0.0f, 0.0f));
        Drawable &dw = e.getComponent<Drawable>();
        dw.geometry_ = GM.resource_map().getGeometry("MatBall");
        dw.material_.set_type(MaterialType::MT_PBR);
        dw.material_.set_data(pbr);
        dw.material_.set_albedo(   GM.resource_map().getTexture("Shore_A"));
        dw.material_.set_metallic( GM.resource_map().getTexture("Shore_M"));
        dw.material_.set_roughness(GM.resource_map().getTexture("Shore_R"));
        dw.material_.set_normal(   GM.resource_map().getTexture("Shore_N"));
    }

    // Peeling 
    {
        pbr.tiling_x_ = 1.0f;
        pbr.tiling_y_ = 1.0f;
        pbr.normal_map_intensity_ = 0.7f;
        Entity e = Entity::CreateEntity("Peeling", EntityType::RENDERABLE);
        LTransform &tr = e.getComponent<LTransform>();
        tr.transform_ = glm::translate(tr.transform_, glm::vec3(6.0f, 0.0f, 0.0f));
        Drawable &dw = e.getComponent<Drawable>();
        dw.geometry_ = GM.resource_map().getGeometry("MatBall");
        dw.material_.set_type(MaterialType::MT_PBR);
        dw.material_.set_data(pbr);
        dw.material_.set_albedo(   GM.resource_map().getTexture("Peeling_A"));
        dw.material_.set_metallic( GM.resource_map().getTexture("Peeling_M"));
        dw.material_.set_roughness(GM.resource_map().getTexture("Peeling_R"));
        dw.material_.set_normal(   GM.resource_map().getTexture("Peeling_N"));
    }

    // Rusted
    {
        pbr.tiling_x_ = 1.0f;
        pbr.tiling_y_ = 1.0f;
        pbr.normal_map_intensity_ = 0.2f;
        Entity e = Entity::CreateEntity("Rusted", EntityType::RENDERABLE);
        LTransform &tr = e.getComponent<LTransform>();
        tr.transform_ = glm::translate(tr.transform_, glm::vec3(2.0f, 0.0f, 2.0f));
        Drawable &dw = e.getComponent<Drawable>();
        dw.geometry_ = GM.resource_map().getGeometry("MatBall");
        dw.material_.set_type(MaterialType::MT_PBR);
        dw.material_.set_data(pbr);
        dw.material_.set_albedo(   GM.resource_map().getTexture("Rusted_A"));
        dw.material_.set_metallic( GM.resource_map().getTexture("Rusted_M"));
        dw.material_.set_roughness(GM.resource_map().getTexture("Rusted_R"));
        dw.material_.set_normal(   GM.resource_map().getTexture("Rusted_N"));
    }

    // Tiles
    {
        pbr.tiling_x_ = 4.0f;
        pbr.tiling_y_ = 4.0f;
        pbr.normal_map_intensity_ = 1.0f;
        Entity e = Entity::CreateEntity("Tiles", EntityType::RENDERABLE);
        LTransform &tr = e.getComponent<LTransform>();
        tr.transform_ = glm::translate(tr.transform_, glm::vec3(4.0f, 0.0f, 2.0f));
        Drawable &dw = e.getComponent<Drawable>();
        dw.geometry_ = GM.resource_map().getGeometry("MatBall");
        dw.material_.set_type(MaterialType::MT_PBR);
        dw.material_.set_data(pbr);
        dw.material_.set_albedo(   GM.resource_map().getTexture("Tiles_A"));
        dw.material_.set_metallic( GM.resource_map().getTexture("Tiles_M"));
        dw.material_.set_roughness(GM.resource_map().getTexture("Tiles_R"));
        dw.material_.set_normal(   GM.resource_map().getTexture("Tiles_N"));
    }

    // Ship Panels 
    {
        pbr.tiling_x_ = 1.0f;
        pbr.tiling_y_ = 1.0f;
        pbr.normal_map_intensity_ = 1.0f;
        Entity e = Entity::CreateEntity("ShipPanels", EntityType::RENDERABLE);
        LTransform &tr = e.getComponent<LTransform>();
        tr.transform_ = glm::translate(tr.transform_, glm::vec3(6.0f, 0.0f, 2.0f));
        Drawable &dw = e.getComponent<Drawable>();
        dw.geometry_ = GM.resource_map().getGeometry("MatBall");
        dw.material_.set_type(MaterialType::MT_PBR);
        dw.material_.set_data(pbr);
        dw.material_.set_albedo(   GM.resource_map().getTexture("ShipPanels_A"));
        dw.material_.set_metallic( GM.resource_map().getTexture("ShipPanels_M"));
        dw.material_.set_roughness(GM.resource_map().getTexture("ShipPanels_R"));
        dw.material_.set_normal(   GM.resource_map().getTexture("ShipPanels_N"));
    }
    
    // Cliff
    {
        pbr.tiling_x_ = 8.0f;
        pbr.tiling_y_ = 8.0f;
        pbr.normal_map_intensity_ = 1.0f;
        Entity e = Entity::CreateEntity("Cliff", EntityType::RENDERABLE);
        LTransform &tr = e.getComponent<LTransform>();
        tr.transform_ = glm::translate(tr.transform_, glm::vec3(2.0f, 0.0f, 4.0f));
        Drawable &dw = e.getComponent<Drawable>();
        dw.geometry_ = GM.resource_map().getGeometry("MatBall");
        dw.material_.set_type(MaterialType::MT_PBR);
        dw.material_.set_data(pbr);
        dw.material_.set_albedo(   GM.resource_map().getTexture("Cliff_A"));
        dw.material_.set_metallic( GM.resource_map().getTexture("Cliff_M"));
        dw.material_.set_roughness(GM.resource_map().getTexture("Cliff_R"));
        dw.material_.set_normal(   GM.resource_map().getTexture("Cliff_N"));
    }

    // Granite
    {
        pbr.tiling_x_ = 2.0f;
        pbr.tiling_y_ = 2.0f;
        pbr.normal_map_intensity_ = 1.0f;
        Entity e = Entity::CreateEntity("Granite", EntityType::RENDERABLE);
        LTransform &tr = e.getComponent<LTransform>();
        tr.transform_ = glm::translate(tr.transform_, glm::vec3(4.0f, 0.0f, 4.0f));
        Drawable &dw = e.getComponent<Drawable>();
        dw.geometry_ = GM.resource_map().getGeometry("MatBall");
        dw.material_.set_type(MaterialType::MT_PBR);
        dw.material_.set_data(pbr);
        dw.material_.set_albedo(   GM.resource_map().getTexture("Granite_A"));
        dw.material_.set_metallic( GM.resource_map().getTexture("Granite_M"));
        dw.material_.set_roughness(GM.resource_map().getTexture("Granite_R"));
        dw.material_.set_normal(   GM.resource_map().getTexture("Granite_N"));
    }

    // Foam 
    {
        pbr.tiling_x_ = 2.0f;
        pbr.tiling_y_ = 2.0f;
        pbr.normal_map_intensity_ = 0.5f;
        Entity e = Entity::CreateEntity("Foam", EntityType::RENDERABLE);
        LTransform &tr = e.getComponent<LTransform>();
        tr.transform_ = glm::translate(tr.transform_, glm::vec3(6.0f, 0.0f, 4.0f));
        Drawable &dw = e.getComponent<Drawable>();
        dw.geometry_ = GM.resource_map().getGeometry("MatBall");
        dw.material_.set_type(MaterialType::MT_PBR);
        dw.material_.set_data(pbr);
        dw.material_.set_albedo(   GM.resource_map().getTexture("Foam_A"));
        dw.material_.set_metallic( GM.resource_map().getTexture("Foam_M"));
        dw.material_.set_roughness(GM.resource_map().getTexture("Foam_R"));
        dw.material_.set_normal(   GM.resource_map().getTexture("Foam_N"));
    }

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
}

void leep::Logic()
{
    GM.input().updateInput();
    CameraMovement(1.0f, 1.0f).executeSystem();
    UpdateTransform(GM.scene().container(EntityType::RENDERABLE)).executeSystem();
    UpdateSceneGraph().executeSystem();

    // Render commands
    DisplayList dl;
    PbrSceneData pbr_sd;
    pbr_sd.view_projection_ = GM.camera().view_projection();
    pbr_sd.camera_position_ = GM.camera().position();
    pbr_sd.light_direction_intensity_ = GM.renderer().sun_dir_intensity_;

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
        .set_scene_data(pbr_sd)
        .set_irradiance_map(GM.resource_map().getTexture("IrradianceEnv"))
        .set_prefilter_map(GM.resource_map().getTexture("PrefilterSpec"))
        .set_lut_map(GM.resource_map().getTexture("LutMap"));

    dl.submit();

    Render(GM.scene().container(EntityType::RENDERABLE)).executeSystem();
    
    dl.addCommand<RenderOptions>()
        .set_cull_face(CullFace::DISABLED);
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

void leep::Close()
{

}
