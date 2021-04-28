#include "leep.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <stdlib.h>
#include <time.h>

#include <ImGui/imgui.h>

int32_t g_times_created;
std::list<void*> g_alloc_list;
std::string g_error;

void CreateEntities(int32_t amount)
{
    using namespace leep;
    for (int32_t i = 0; i < amount; ++i)
    {
        String name = 
            "Cube_" + ToString(i) + "_" + ToString(g_times_created);
        CreateRenderable(name, "Cube", "Default");
        SetLocation(name, i * 2.0f, g_times_created * 2.0f, -2.0f);
    }
    g_times_created++;
}

void RemoveRandomEntities(int32_t amount)
{
    using namespace leep;
    EntityContainer &c = GM.scene().container(EntityType::RENDERABLE);
    int32_t count = (c.blocks_.size() - 1) * kEntitiesPerChunk;
    if (count < 0)
    {
        return;
    }
    else
    {
        count += c.blocks_.back()->last_;
    }
    for (int32_t i = 0; i < amount; ++i)
    {
        if (count <= 0) return;

        int32_t random = rand() % count;
        RemoveEntity(GM.scene().entity_map_.getEntityName(random, EntityType::RENDERABLE));
        count--;
    }
}

// ImGui seems the fastest way to implement this quick functionalities
// for the demo
void ImguiFunction()
{
    static bool show = true;
    if (!ImGui::Begin("Entity and memory manipulator", &show, ImGuiWindowFlags_AlwaysAutoResize))
    {
        // Early out if the window is collapsed, as an optimization.
        ImGui::End();
        return;
    }
    if (ImGui::Button("Create 10 entities"))
    {
        CreateEntities(10);
    }

    if (ImGui::Button("Create 100 entities"))
    {
        CreateEntities(100);
    }

    if (ImGui::Button("Remove a random entity"))
    {
        RemoveRandomEntities(1);
    }

    if (ImGui::Button("Remove 10 random entities"))
    {
        RemoveRandomEntities(10);
    }

    if (ImGui::Button("Remove 100 random entities"))
    {
        RemoveRandomEntities(100);
    }

    if (ImGui::Button("Allocate 1KB"))
    {
        void *ptr = leep::GM.memory().generalAlloc(KILOBYTES(1));
        if (ptr)
        {
            g_alloc_list.push_back(ptr);
        }
        else
        {
            g_error = "Couldn't allocate 1KB";
        }
    }

    if (ImGui::Button("Allocate 100KB"))
    {
        void *ptr = leep::GM.memory().generalAlloc(KILOBYTES(100));
        if (ptr)
        {
            g_alloc_list.push_back(ptr);
        }
        else
        {
            g_error = "Couldn't allocate 100KB";
        }
    }

    if (ImGui::Button("Allocate 1MB"))
    {
        void *ptr = leep::GM.memory().generalAlloc(MEGABYTES(1));
        if (ptr)
        {
            g_alloc_list.push_back(ptr);
        }
        else
        {
            g_error = "Couldn't allocate 1MB";
        }
    }

    if (ImGui::Button("Allocate 100MB"))
    {
        void *ptr = leep::GM.memory().generalAlloc(MEGABYTES(100));
        if (ptr)
        {
            g_alloc_list.push_back(ptr);
        }
        else
        {
            g_error = "Couldn't allocate 100MB";
        }
    }

    if (ImGui::Button("Free last allocation"))
    {
        if (!g_alloc_list.empty())
        {
            leep::GM.memory().generalFree(g_alloc_list.back());
            g_alloc_list.pop_back();
        }
        else
        {
            g_error = "Allocate something first.";
        }
    }

    if (ImGui::Button("Free first allocation"))
    {
        if (!g_alloc_list.empty())
        {
            leep::GM.memory().generalFree(g_alloc_list.front());
            g_alloc_list.pop_front();
        }
        else
        {
            g_error = "Allocate something first.";
        }
    }

    ImGui::Text("Last error: %s", g_error.c_str());
    if (ImGui::Button("Clear error"))
    {
        g_error = "";
    }
    ImGui::End();
}

void leep::Init()
{
    srand(time(0));
    {
        // Resource creation CPU side (GPU allocation will occur on first use)
        ResourceMap &rm = GM.resource_map();
        const String tp = "../assets/tex/";
        rm.addTexture("Default", "../assets/tex/default.jpg", TexType::SRGB);
        rm.addTexture("Skybox", 1024.0f, 1024.0f, TexType::ENVIRONMENT);
        rm.addTexture("IrradianceEnv", 1024.0f, 1024.0f, TexType::ENVIRONMENT);
        rm.addTexture("PrefilterSpec", 128.0f, 128.0f, TexType::PREFILTER_ENVIRONMENT);
        rm.addTexture("LutMap", 512.0f, 512.0f, TexType::LUT);
    }

    GM.scene().createContainer(EntityType::RENDERABLE);
    GM.ui_tools().set_callback(&ImguiFunction);

    DisplayList init_dl;
    init_dl.addCommand<RenderOptions>()
        .enable_depth_test(true)
        .enable_write_depth(true)
        .set_cull_face(CullFace::BACK)
        .enable_blend(true)
        .set_blend_func(BlendFunc::ONE, BlendFunc::ZERO);
    init_dl.addCommand<EquirectangularToCubemap>()
        .set_in_path("../assets/tex/env/rooftop-env.hdr")
        .set_out_cube(GM.resource_map().getTexture("Skybox"))
        .set_out_prefilter(GM.resource_map().getTexture("PrefilterSpec"))
        .set_out_lut(GM.resource_map().getTexture("LutMap"));
    init_dl.addCommand<EquirectangularToCubemap>()
        .set_in_path("../assets/tex/env/rooftop-dif.hdr")
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
    pbr_sd.light_direction_intensity_ = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);


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
    //DeleteReleased().executeSystem();
}

void leep::Close()
{

}
