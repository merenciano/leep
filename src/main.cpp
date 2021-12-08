#include "leep.h"

//#define LEEP_SINGLE_THREAD

void leep::GameInit()
{
	ResourceMap &rm = GM.resource_map();
	const String tp = "../assets/tex/";
	rm.addGeometry("MatBall", "../assets/obj/matball-n.obj");
	rm.addTexture("Skybox", 1024.0f, 1024.0f, TexType::ENVIRONMENT);
	rm.addTexture("IrradianceEnv", 1024.0f, 1024.0f, TexType::ENVIRONMENT);
	rm.addTexture("PrefilterSpec", 128.0f, 128.0f, TexType::PREFILTER_ENVIRONMENT);
	rm.addTexture("LutMap", 512.0f, 512.0f, TexType::LUT);

	rm.addTexture("CelticGold_A" ,tp + "celtic-gold/celtic-gold_A.png", TexType::SRGB);
	rm.addTexture("CelticGold_N" ,tp + "celtic-gold/celtic-gold_N.png", TexType::RGB);
	rm.addTexture("CelticGold_M" ,tp + "celtic-gold/celtic-gold_M.png", TexType::R);
	rm.addTexture("CelticGold_R" ,tp + "celtic-gold/celtic-gold_R.png", TexType::R);

	rm.addTexture("Peeling_A" ,tp + "peeling/peeling_A.png", TexType::SRGB);
	rm.addTexture("Peeling_N" ,tp + "peeling/peeling_N.png", TexType::RGB);
	rm.addTexture("Peeling_M" ,tp + "peeling/peeling_M.png", TexType::R);
	rm.addTexture("Peeling_R" ,tp + "peeling/peeling_R.png", TexType::R);

	rm.addTexture("Rusted_A" ,tp + "rusted/rusted_A.png", TexType::SRGB);
	rm.addTexture("Rusted_N" ,tp + "rusted/rusted_N.png", TexType::RGB);
	rm.addTexture("Rusted_M" ,tp + "rusted/rusted_M.png", TexType::R);
	rm.addTexture("Rusted_R" ,tp + "rusted/rusted_R.png", TexType::R);

	rm.addTexture("Tiles_A" ,tp + "tiles/tiles_A.png", TexType::SRGB);
	rm.addTexture("Tiles_N" ,tp + "tiles/tiles_N.png", TexType::RGB);
	rm.addTexture("Tiles_M" ,tp + "tiles/tiles_M.png", TexType::R);
	rm.addTexture("Tiles_R" ,tp + "tiles/tiles_R.png", TexType::R);

	rm.addTexture("ShipPanels_A" ,tp + "ship-panels/ship-panels_A.png", TexType::SRGB);
	rm.addTexture("ShipPanels_N" ,tp + "ship-panels/ship-panels_N.png", TexType::RGB);
	rm.addTexture("ShipPanels_M" ,tp + "ship-panels/ship-panels_M.png", TexType::R);
	rm.addTexture("ShipPanels_R" ,tp + "ship-panels/ship-panels_R.png", TexType::R);

	rm.addTexture("Shore_A" ,tp + "shore/shore_A.png", TexType::SRGB);
	rm.addTexture("Shore_N" ,tp + "shore/shore_N.png", TexType::RGB);
	rm.addTexture("Shore_M" ,tp + "shore/shore_M.png", TexType::R);
	rm.addTexture("Shore_R" ,tp + "shore/shore_R.png", TexType::R);

	rm.addTexture("Cliff_A" ,tp + "cliff/cliff_A.png", TexType::SRGB);
	rm.addTexture("Cliff_N" ,tp + "cliff/cliff_N.png", TexType::RGB);
	rm.addTexture("Cliff_M" ,tp + "cliff/cliff_M.png", TexType::R);
	rm.addTexture("Cliff_R" ,tp + "cliff/cliff_R.png", TexType::R);

	rm.addTexture("Granite_A" ,tp + "granite/granite_A.png", TexType::SRGB);
	rm.addTexture("Granite_N" ,tp + "granite/granite_N.png", TexType::RGB);
	rm.addTexture("Granite_M" ,tp + "granite/granite_M.png", TexType::R);
	rm.addTexture("Granite_R" ,tp + "granite/granite_R.png", TexType::R);

	rm.addTexture("Foam_A" ,tp + "foam/foam_A.png", TexType::SRGB);
	rm.addTexture("Foam_N" ,tp + "foam/foam_N.png", TexType::RGB);
	rm.addTexture("Foam_M" ,tp + "foam/foam_M.png", TexType::R);
	rm.addTexture("Foam_R" ,tp + "foam/foam_R.png", TexType::R);

	PbrData pbr;
	pbr.color_ = glm::vec3(1.0f, 1.0f, 1.0f);
	pbr.tiling_x_ = 4.0f;
	pbr.tiling_y_ = 4.0f;
	pbr.use_albedo_map_ = 1.0f;
	pbr.use_pbr_maps_ = 1.0f;
	pbr.metallic_ = 0.5f;
	pbr.roughness_ = 0.5f;
	pbr.normal_map_intensity_ = 1.0f;

	GM.renderer().sun_dir_intensity_ = glm::vec4(0.0f, -1.0f, -0.1f, 1.0f);
	GM.renderer().initMaterial(MaterialType::MT_PBR, "pbr");

	GM.scene().createContainer(EntityType::RENDERABLE);

	// CelticGold
	{
	    Entity e = Entity::CreateEntity("CelticGold", EntityType::RENDERABLE);
	    LTransform &tr = e.getComponent<LTransform>();
	    tr.transform_ = glm::translate(tr.transform_, glm::vec3(2.0f, 0.0f, 0.0f));
	    Drawable &dw = e.getComponent<Drawable>();
	    dw.geometry_ = GM.resource_map().getGeometry("MatBall");
	    dw.material_.set_type(MaterialType::MT_PBR);
	    dw.material_.set_data((float*)&pbr, sizeof(PbrData) / 4);
	    Texture t[4];
	    t[0] = GM.resource_map().getTexture("CelticGold_A");
	    t[1] = GM.resource_map().getTexture("CelticGold_M");
	    t[2] = GM.resource_map().getTexture("CelticGold_R");
	    t[3] = GM.resource_map().getTexture("CelticGold_N");
	    dw.material_.set_tex(t, 4);
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
	    dw.material_.set_data((float*)&pbr, sizeof(PbrData) / 4);
	    Texture t[4];
	    t[0] = GM.resource_map().getTexture("Shore_A");
	    t[1] = GM.resource_map().getTexture("Shore_M");
	    t[2] = GM.resource_map().getTexture("Shore_R");
	    t[3] = GM.resource_map().getTexture("Shore_N");
	    dw.material_.set_tex(t, 4);
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
	    dw.material_.set_data((float*)&pbr, sizeof(PbrData) / 4);
	    Texture t[4];
	    t[0] = GM.resource_map().getTexture("Peeling_A");
	    t[1] = GM.resource_map().getTexture("Peeling_M");
	    t[2] = GM.resource_map().getTexture("Peeling_R");
	    t[3] = GM.resource_map().getTexture("Peeling_N");
	    dw.material_.set_tex(t, 4);
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
	    dw.material_.set_data((float*)&pbr, sizeof(PbrData) / 4);
	    Texture t[4];
	    t[0] = GM.resource_map().getTexture("Rusted_A");
	    t[1] = GM.resource_map().getTexture("Rusted_M");
	    t[2] = GM.resource_map().getTexture("Rusted_R");
	    t[3] = GM.resource_map().getTexture("Rusted_N");
	    dw.material_.set_tex(t, 4);
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
	    dw.material_.set_data((float*)&pbr, sizeof(PbrData) / 4);
	    Texture t[4];
	    t[0] = GM.resource_map().getTexture("Tiles_A");
	    t[1] = GM.resource_map().getTexture("Tiles_M");
	    t[2] = GM.resource_map().getTexture("Tiles_R");
	    t[3] = GM.resource_map().getTexture("Tiles_N");
	    dw.material_.set_tex(t, 4);
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
	    dw.material_.set_data((float*)&pbr, sizeof(PbrData) / 4);
	    Texture t[4];
	    t[0] = GM.resource_map().getTexture("ShipPanels_A");
	    t[1] = GM.resource_map().getTexture("ShipPanels_M");
	    t[2] = GM.resource_map().getTexture("ShipPanels_R");
	    t[3] = GM.resource_map().getTexture("ShipPanels_N");
	    dw.material_.set_tex(t, 4);
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
	    dw.material_.set_data((float*)&pbr, sizeof(PbrData) / 4);
	    Texture t[4];
	    t[0] = GM.resource_map().getTexture("Cliff_A");
	    t[1] = GM.resource_map().getTexture("Cliff_M");
	    t[2] = GM.resource_map().getTexture("Cliff_R");
	    t[3] = GM.resource_map().getTexture("Cliff_N");
	    dw.material_.set_tex(t, 4);
	}

	// Granite
	{
	    pbr.tiling_x_ = 2.0f;
	    pbr.tiling_y_ = 2.0f;
	    pbr.normal_map_intensity_ = 1.0f;
	    Entity e = Entity::CreateEntity("Granite", EntityType::RENDERABLE);
	    LTransform &tr = e.getComponent<LTransform>();
	    tr.translate(4.0f, 0.0f, 4.0f);
	    Drawable &dw = e.getComponent<Drawable>();
	    dw.geometry_ = GM.resource_map().getGeometry("MatBall");
	    dw.material_.set_type(MaterialType::MT_PBR);
	    dw.material_.set_data((float*)&pbr, sizeof(PbrData) / 4);
	    Texture t[4];
	    t[0] = GM.resource_map().getTexture("Granite_A");
	    t[1] = GM.resource_map().getTexture("Granite_M");
	    t[2] = GM.resource_map().getTexture("Granite_R");
	    t[3] = GM.resource_map().getTexture("Granite_N");
	    dw.material_.set_tex(t, 4);
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
	    dw.material_.set_data((float*)&pbr, sizeof(PbrData) / 4);
	    Texture t[4];
	    t[0] = GM.resource_map().getTexture("Foam_A");
	    t[1] = GM.resource_map().getTexture("Foam_M");
	    t[2] = GM.resource_map().getTexture("Foam_R");
	    t[3] = GM.resource_map().getTexture("Foam_N");
	    dw.material_.set_tex(t, 4);
	}

	THE_RenderCommand *rendops = (THE_RenderCommand*)malloc(sizeof(THE_RenderCommand));
	rendops->data.renderops.depth_test = 1;
	rendops->data.renderops.write_depth = 1;
	rendops->data.renderops.cull_face = THE_CULLFACE_BACK;
	rendops->data.renderops.blend = 1;
	rendops->data.renderops.sfactor = THE_BLENDFUNC_ONE;
	rendops->data.renderops.dfactor = THE_BLENDFUNC_ZERO;
	rendops->data.renderops.changed_mask = 0xFF; // Changed all
	rendops->execute = THE_RenderOptionsExecute;

	THE_RenderCommand *sky = (THE_RenderCommand*)malloc(sizeof(THE_RenderCommand));
	strcpy(sky->data.eqr_cube.in_path, "../assets/tex/env/helipad-env.hdr");
	sky->data.eqr_cube.out_cube = GM.resource_map().getTexture("Skybox");
	sky->data.eqr_cube.out_prefilt = GM.resource_map().getTexture("PrefilterSpec");
	sky->data.eqr_cube.out_lut = GM.resource_map().getTexture("LutMap");
	sky->execute = THE_EquirectToCubeExecute;
	rendops->next = sky;

	THE_RenderCommand *irradiance = (THE_RenderCommand*)malloc(sizeof(THE_RenderCommand));
	strcpy(irradiance->data.eqr_cube.in_path, "../assets/tex/env/helipad-dif.hdr");
	irradiance->data.eqr_cube.out_cube = GM.resource_map().getTexture("IrradianceEnv");
	irradiance->data.eqr_cube.out_prefilt = *(new (&(irradiance->data.eqr_cube.out_prefilt)) Texture());
	irradiance->data.eqr_cube.out_lut = *(new (&(irradiance->data.eqr_cube.out_lut)) Texture());
	irradiance->execute = THE_EquirectToCubeExecute;
	sky->next = irradiance;
	irradiance->next = NULL;

	THE_AddCommands(GM.rendererptr(), rendops);
}

void leep::GameLogic()
{
	LuaScripting::ExecuteScript("../assets/scripts/update.lua");
	GM.input().updateInput();
	CameraMovement(1.0f, 1.0f).executeSystem();
	UpdateTransform(GM.scene().container(EntityType::RENDERABLE)).executeSystem();
	UpdateSceneGraph().executeSystem();

	// Render commands
	PbrSceneData pbr_sd;
	pbr_sd.view_projection_ = GM.camera().view_projection();
	pbr_sd.camera_position_ = GM.camera().position();
	pbr_sd.light_direction_intensity_ = GM.renderer().sun_dir_intensity_;

	Material full_screen_img;
	full_screen_img.set_type(MaterialType::MT_FULL_SCREEN_IMAGE);
	Texture fbtex = GM.camera().framebuffer().color();
	full_screen_img.set_tex(&fbtex, 1);

	THE_RenderCommand *fbuff = (THE_RenderCommand*)malloc(sizeof(THE_RenderCommand));
	fbuff->data.usefb.fb = GM.camera().framebuffer();
	fbuff->data.usefb.def = 0;
	fbuff->execute = THE_UseFramebufferExecute;

	THE_RenderCommand *rops = (THE_RenderCommand*)malloc(sizeof(THE_RenderCommand));
	rops->data.renderops.blend = 1;
	rops->data.renderops.sfactor = THE_BLENDFUNC_ONE;
	rops->data.renderops.dfactor = THE_BLENDFUNC_ZERO;
	rops->data.renderops.depth_test = 1;
	rops->data.renderops.write_depth = 1;
	rops->data.renderops.cull_face = THE_CULLFACE_BACK;
	rops->data.renderops.changed_mask = 0xFF; // All changed
	rops->execute = THE_RenderOptionsExecute;
	fbuff->next = rops;

	THE_RenderCommand *clear = (THE_RenderCommand*)malloc(sizeof(THE_RenderCommand));
	clear->data.clear.bcolor = 1;
	clear->data.clear.bdepth = 1;
	clear->data.clear.bstencil = 0;
	clear->data.clear.color[0] = 0.2f;
	clear->data.clear.color[1] = 0.2f;
	clear->data.clear.color[2] = 0.2f;
	clear->data.clear.color[3] = 1.0f;
	clear->execute = THE_ClearExecute;
	rops->next = clear;

	Texture scene_tex[3];
	scene_tex[0] = GM.resource_map().getTexture("LutMap");
	scene_tex[1] = GM.resource_map().getTexture("IrradianceEnv");
	scene_tex[2] = GM.resource_map().getTexture("PrefilterSpec");

	THE_RenderCommand *usemat = (THE_RenderCommand*)malloc(sizeof(THE_RenderCommand));
	usemat->data.usemat.mat = *(new (&(usemat->data.usemat.mat)) Material());
	usemat->data.usemat.mat.set_type(MaterialType::MT_PBR);
	usemat->data.usemat.mat.set_data((float*)&pbr_sd, sizeof(PbrSceneData) / sizeof (float));
	usemat->data.usemat.mat.set_tex(scene_tex, 3, 1);
	usemat->execute = THE_UseMaterialExecute;
	clear->next = usemat;
	usemat->next = NULL;

	THE_AddCommands(GM.rendererptr(), fbuff);

	Render(GM.scene().container(EntityType::RENDERABLE)).executeSystem();

	rops = (THE_RenderCommand*)malloc(sizeof(THE_RenderCommand));
	rops->data.renderops.cull_face = THE_CULLFACE_DISABLED;
	rops->data.renderops.changed_mask = THE_CULL_FACE_BIT;
	rops->execute = THE_RenderOptionsExecute;

	THE_RenderCommand *sky = (THE_RenderCommand*)malloc(sizeof(THE_RenderCommand));
	sky->data.skybox.cubemap = GM.resource_map().getTexture("Skybox");
	sky->execute = THE_SkyboxExecute;
	rops->next = sky;

	fbuff = (THE_RenderCommand*)malloc(sizeof(THE_RenderCommand));
	fbuff->data.usefb.def = 1;
	fbuff->execute = THE_UseFramebufferExecute;
	sky->next = fbuff;

	THE_RenderCommand *rops2 = (THE_RenderCommand*)malloc(sizeof(THE_RenderCommand));
	rops2->data.renderops.depth_test = 0;
	rops2->data.renderops.changed_mask = THE_DEPTH_TEST_BIT;
	rops2->execute = THE_RenderOptionsExecute;
	fbuff->next = rops2;

	clear = (THE_RenderCommand*)malloc(sizeof(THE_RenderCommand));
	clear->data.clear.bcolor = 1;
	clear->data.clear.bdepth = 0;
	clear->data.clear.bstencil = 0;
	clear->data.clear.color[0] = 1.0f;
	clear->data.clear.color[1] = 0.0f;
	clear->data.clear.color[2] = 0.0f;
	clear->data.clear.color[3] = 1.0f;
	clear->execute = THE_ClearExecute;
	rops2->next = clear;

	THE_RenderCommand *draw = (THE_RenderCommand*)malloc(sizeof(THE_RenderCommand));
	draw->data.draw.geometry = GM.resource_map().getGeometry("Quad");
	// TODO quitar esta shiiiiet
	draw->data.draw.mat = *(new (&draw->data.draw.mat) Material());
	draw->data.draw.mat = full_screen_img;
	draw->data.draw.inst_count = 1;
	draw->execute = THE_DrawExecute;
	clear->next = draw;
	draw->next = NULL;

	THE_AddCommands(GM.rendererptr(), rops);
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
